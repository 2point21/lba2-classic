#include	<system\adeline.h>
#include	<system\logprint.h>
#include	<system\exit.h>

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>

#ifdef	_WIN32
#include 	"mixer.h"
#include	<system\winsys.h>
#include	<system\timerwin.h>
#include	<ail\mssw.h>
#else
#include	<system\timerdos.h>
#include	<ail\mss.h>
#endif

#include	<ail\common.h>
#include	<ail\sample.h>
#include	<ail\stream.h>

//████████████████████████████████████████████████████████████████████████████

#pragma	aux AIL_error	"_*"

//████████████████████████████████████████████████████████████████████████████

#define	MAX_SYNTHESIZER_VOICES	0
#define	MAX_SAMPLE_VOICES	12	// THIS CAN BE CHANGED
#define	MAX_STREAMING_VOICES	2	// DO NOT CHANGE

#define	NB_VOICES	MAX_SAMPLE_VOICES+MAX_SYNTHESIZER_VOICES+MAX_STREAMING_VOICES

extern HDIGDRIVER	DigDriver 			;// Handle to digital audio driver

//████████████████████████████████████████████████████████████████████████████
typedef	struct		{
				HSAMPLE	handle	;
				U32	sample	;
				U32	timer	;
			}
			SAMP_DESC	;

//████████████████████████████████████████████████████████████████████████████
	S32		SampleOverflow = FALSE		;

//████████████████████████████████████████████████████████████████████████████
	S32		LibWaveMasterVolume = 128	;
#ifdef	_WIN32
	S32		UseWaveMixer = FALSE		;
#endif//_WIN32

//████████████████████████████████████████████████████████████████████████████
#ifdef	_WIN32
static	VOLUMECONTROL	WaveVolume			;
static	S32		OldWaveVolume			;
#endif//_WIN32

//████████████████████████████████████████████████████████████████████████████
static	HTIMER		HandleTimer	= NULL		;
static	void		*Timer_ptrsample		;
static	U32		Timer_usernum	     		;
static	S32		Timer_pitchbend			;
static	S32		Timer_nbrepeat			;
static	S32		Timer_volume	     		;
static	S32		Timer_pan	     		;

static	U32		InversePanMask = 0		;

	SAMP_DESC	Sample_Handle[MAX_SAMPLE_VOICES];
static	S32		Nb_Voices			;
static	U32		SampleSeed			;
static	S32		SamplesPaused = 0		;
static	S32		SamplesFade = 0			;
static	S32		FadeStartTime			;

//████████████████████████████████████████████████████████████████████████████

enum	{ 	USER_DATA_VOLUME,
		USER_DATA_PITCHBEND,
		USER_DATA_RATE,
		USER_DATA_USER
	}	;

//████████████████████████████████████████████████████████████████████████████

#define	Scale(value, scale, range)	( ( (value)*(scale) + (1<<(range-1)) ) >> range )

//████████████████████████████████████████████████████████████████████████████

VOID_FUNC	SpecialInitSampleDriver	;

//████████████████████████████████████████████████████████████████████████████

#ifdef	_WIN32

S32	InitDriver(S32 rate, S32 bits, S32 chans)
{
	static	PCMWAVEFORMAT	sPCMWF	;
	S32			res	;

	sPCMWF.wf.wFormatTag      = WAVE_FORMAT_PCM;
	sPCMWF.wf.nChannels       = (U16)chans;
	sPCMWF.wf.nSamplesPerSec  = rate;
	sPCMWF.wf.nAvgBytesPerSec = rate * (bits >> 3) * chans;
	sPCMWF.wf.nBlockAlign     = (U16)( (bits >> 3) * chans );
	sPCMWF.wBitsPerSample     = (U16)bits;

	res = AIL_waveOutOpen(&DigDriver, NULL, 0, (LPWAVEFORMAT) &sPCMWF);

	if(res)
	{
		DigDriver = NULL	;
	}

	return res	;
}

#endif

//████████████████████████████████████████████████████████████████████████████

S32	InitSampleDriver( char *driver_name )
{
	if( Sample_Driver_Enabled )
	{
		return TRUE	;
	}

#ifdef	_WIN32

	driver_name = driver_name ;

	AIL_set_preference(DIG_USE_WAVEOUT, FALSE)	;
retry:
	if(InitDriver(SamplingRate, 16, 2))
	{
		if(InitDriver(SamplingRate, 8, 2))
		{
			if(InitDriver(SamplingRate/2, 8, 1))
			{

				if(!AIL_get_preference(DIG_USE_WAVEOUT))
				{
					AIL_set_preference(DIG_USE_WAVEOUT, TRUE) ;
					goto retry	;
				}
				LogPuts( "Could find a compatible Digital Audio Driver. Digital Audio Disabled." );
				return TRUE		;
			}
		}
	}

#else
 	// Set application preferences
	AIL_set_preference(DIG_HARDWARE_SAMPLE_RATE, SamplingRate) ;
	AIL_set_preference(DIG_LATENCY,		     30		) ;
        AIL_set_preference(DIG_USE_16_BITS,          YES 	) ;
	AIL_set_preference(DIG_USE_STEREO,           YES 	) ;
	AIL_set_preference(DIG_MIXER_CHANNELS,       NB_VOICES	) ;
	AIL_set_preference(AIL_SCAN_FOR_HARDWARE,    YES	) ;

	// Initialize digital sound system
	if(driver_name && strcmp(driver_name, ""))
	{
		strcpy(SampleDriverName, driver_name)	;
		DigDriver = AIL_install_DIG_driver_file( driver_name, NULL ) ;
	}
	else
	{
		LONG res	;

		SampleDriverName[0] = (char)0	;

		res=AIL_install_DIG_INI(&DigDriver)	;
		switch(res)
		{
		case AIL_INIT_SUCCESS:
			break;
		case AIL_NO_INI_FILE:
			LogPuts( "No Digital Audio Driver specified. Digital Audio Disabled." );
			return TRUE		;
		default:
			DigDriver = NULL	;
			LogPuts("Please Run Setsound.exe again.");
			break			;
		}
	}
	if( DigDriver == NULL )
	{
		LogPuts( (char*)AIL_error )	;
		return FALSE		;

	}
#endif
	{
		char 	desc[128] = ""	;
		S32	format		;
		S32	rate		;

		AIL_digital_configuration(DigDriver, &rate, &format, desc)	;

		if(!desc[0])
		{
			strcpy(desc, SampleDriverName)	;
		}

		LogPrintf(	"Sample device         : %s\n"
				"Digital configuration : %d bit, %s, %d Hz\n",
				desc,
				format & DIG_F_16BITS_MASK ? 16 : 8,
				format & DIG_F_STEREO_MASK ? "Stereo" : "Mono",
				rate	)	;
	}


	for(Nb_Voices=0; Nb_Voices<MAX_SAMPLE_VOICES; Nb_Voices++)
	{
		HSAMPLE	handle	;

		handle = AIL_allocate_sample_handle(DigDriver)	;

		if(!handle)	break ;

		Sample_Handle[Nb_Voices].handle = handle;
		Sample_Handle[Nb_Voices].sample	= 0	;
	}

	SampleSeed		= 0x1000000	;

	Sample_Driver_Enabled	= TRUE 	;

	RegisterInitClear( SpecialInitSampleDriver, ClearSampleDriver );// Automatic clear

#ifdef	_WIN32
	UseWaveMixer = GetMixerVolDevice(&WaveVolume, MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT) ;
	if(UseWaveMixer)
	{
		LogPuts("Mixer Device found. Used for WAVE volume control.")	;
		OldWaveVolume = GetMixerVolume(&WaveVolume)	;
	}
	else
	{
		LogPuts("Mixer Device not found. Wave volume control handled in software.")	;
	}

        PauseSamplesPtr	= PauseSamples	;
	if(PauseMiles)
	{
		PauseMiles	= PauseBothMiles;
	}
	else
	{
	        PauseMiles	= PauseSamples	;
	}
        ResumeSamplesPtr= ResumeSamples	;
	if(ResumeMiles)
	{
		ResumeMiles	= ResumeBothMiles;
	}
	else
	{
	        ResumeSamplesPtr= ResumeSamples	;
	}
#endif

	return TRUE	;
}

//████████████████████████████████████████████████████████████████████████████

static void SpecialInitSampleDriver()
{
	InitSampleDriver(SampleDriverName);
}

//████████████████████████████████████████████████████████████████████████████

void	ClearSampleDriver()
{
	if( !Sample_Driver_Enabled )	return	;

#ifdef	_WIN32
	// Restore mixer volume
	if(UseWaveMixer)
	{
		 SetMixerVolume(&WaveVolume, OldWaveVolume)	;
	}
#endif//_WIN32

	Sample_Driver_Enabled = FALSE 		;
}
//████████████████████████████████████████████████████████████████████████████

static S32 GetHandleIndice(U32 sample)
{
	S32		hnum	;

	if(sample<0x1000000)
	{
		for(hnum=0; hnum<Nb_Voices; hnum++)
		{
			if(((Sample_Handle[hnum].sample>>8)&0xFFFF)==sample)
			{
				return hnum ;
			}
		}
		return -1 ;
	}
	else
	{
		// get array indice
		hnum = sample & 0xFF	;

		// right sample playing?
		if(Sample_Handle[hnum].sample == sample)
			return hnum	;

		// No already stopped
		return	-1	;

	}
}

//████████████████████████████████████████████████████████████████████████████

static HSAMPLE FindFreeHandle(U32 *pnum)
{
	HSAMPLE		handle	;
	U32		hnum	;

	if( !Sample_Driver_Enabled )	return 0 ;

	// Search a sample with SMP_DONE status
	for(hnum=0; hnum<Nb_Voices; hnum++)
	{
		if(Sample_Handle[hnum].sample==0)
			break;
		if(AIL_sample_status(Sample_Handle[hnum].handle)==SMP_DONE)
			break;
	}

	// Found one?
	if(hnum==Nb_Voices)
	{
		// If none find the oldest

		U32	bestsample	;

		SampleOverflow	= TRUE	;

		bestsample = 0xFFFFFFFF	;

		// search a one shot
		for(hnum=0; hnum<Nb_Voices; hnum++)
		{
			if(    (bestsample>Sample_Handle[hnum].sample)
			    && (AIL_sample_loop_count(Sample_Handle[hnum].handle) == 1) )
				bestsample=Sample_Handle[hnum].sample	;
		}
		if(bestsample != 0xFFFFFFFF)
		{
			goto ok	;
		}

		// search a sample that is not in infinite loop
		for(hnum=0; hnum<Nb_Voices; hnum++)
		{
			if(    (bestsample>Sample_Handle[hnum].sample)
			    && AIL_sample_loop_count(Sample_Handle[hnum].handle) )
				bestsample=Sample_Handle[hnum].sample	;
		}
		if(bestsample != 0xFFFFFFFF)
		{
			goto ok	;
		}

		// search any sample
		for(hnum=0; hnum<Nb_Voices; hnum++)
		{
			if( bestsample>Sample_Handle[hnum].sample )
				bestsample=Sample_Handle[hnum].sample	;
		}
ok:
		// get array indice
		hnum = bestsample & 0xFF		;

		// No sample associated with handle
		Sample_Handle[hnum].sample = 0		;
	}

	// Get sample
	handle = Sample_Handle[hnum].handle	;

	// stop the sample (security)
	AIL_end_sample(handle)			;

	// Clear internal structures
	AIL_init_sample(handle)			;

	*pnum = hnum	;

	Sample_Handle[hnum].timer = TimerSystemHR;

	return	handle 	;
}

//████████████████████████████████████████████████████████████████████████████

static U32 GenerateSampleHandle(U32 usernum, U32 hnum)
{
	U32		sample	;

	// Compute handle
	sample = SampleSeed | ((usernum&0xFFFF)<<8) | hnum ;

	Sample_Handle[hnum].sample = sample	;

	SampleSeed	+= 0x1000000		;
	if(SampleSeed==0)
	{
		SampleSeed = 0x1000000		;
	}

	return (sample)				;
}

//████████████████████████████████████████████████████████████████████████████

static void ScaleVolumeSamples( S32 fadevol )
{
	U32	hnum	;

	// update all the volumes
	for(hnum=0; hnum<Nb_Voices; hnum++)
	{
		HSAMPLE	handle	;
		S32	volume	;

		handle = Sample_Handle[hnum].handle	;
		volume = AIL_sample_user_data( handle, USER_DATA_VOLUME);
		AIL_set_sample_volume( handle, Scale(volume, fadevol, 7));
	}
}


//████████████████████████████████████████████████████████████████████████████

void	SetMasterVolumeSample( S32 volume )
{
#ifdef	_WIN32
	if(UseWaveMixer)
	{
		SetMixerVolume(&WaveVolume, volume)	;
	}
	else
#endif
	{
		if(volume<=0)
		{
			volume = 0	;
		}
		else
		{
			volume++	;

			if(volume>128)
			{
				volume = 128	;
			}
		}

		if(volume == LibWaveMasterVolume)
		{
			return	;
		}

		LibWaveMasterVolume = volume	;

		if(!SamplesFade)	ScaleVolumeSamples(volume)	;

		ChangeVolumeStream(GetVolumeStream())	;
	}
}

//████████████████████████████████████████████████████████████████████████████

S32	GetMasterVolumeSample()
{
#ifdef	_WIN32
	if(UseWaveMixer)
	{
		return GetMixerVolume(&WaveVolume) ;
	}
	else
#endif
	{
		if(LibWaveMasterVolume)
		{
			return(LibWaveMasterVolume - 1);
		}
		else
		{
			return(0)		;
		}
	}
}

//████████████████████████████████████████████████████████████████████████████

S32	FadeOutSamples( S32 delay )
{
	S32	delta	;

	if(!SamplesFade)
	{
		ManageTime()	;

		FadeStartTime = TimerSystemHR ;

		SamplesFade = TRUE ;
	}

	ManageTime()	;

	delta = FadeStartTime + delay - TimerSystemHR ;

	if(delta<0)	delta = 0	;

	if(Sample_Driver_Enabled)
	{
		S32	ratio	;

		if(delay)
		{
			ratio = delta*128/delay	;
		}
		else
		{
			ratio = 128 ;
		}

		ScaleVolumeSamples(ratio);
	}

	return delta	;
}

//████████████████████████████████████████████████████████████████████████████

S32	FadeInSamples( S32 delay )
{
	S32	delta	;

	if(SamplesFade)
	{
		ManageTime()	;

		FadeStartTime = TimerSystemHR ;

		SamplesFade = FALSE ;
	}

	ManageTime()	;

	delta = TimerSystemHR - FadeStartTime;

	if(delta>delay)	delta = delay	;

	if(Sample_Driver_Enabled)
	{
		S32	ratio	;

		if(delay)
		{
			ratio = delta*128/delay	;
		}
		else
		{
			ratio = 128 ;
		}

		ScaleVolumeSamples(ratio);
	}

	return delta	;
}

//████████████████████████████████████████████████████████████████████████████

void	InverseStereoSample( S32 inverse )
{
	InversePanMask = inverse ? 127 : 0 ;
}

//████████████████████████████████████████████████████████████████████████████

U32	PlaySample(void *ptrsample, U32 usernum, S32 pitchbend, S32 nbrepeat, S32 volume, S32 pan)
{
	HSAMPLE		handle	;
	U32		hnum	;

	// get one available handle
	handle = FindFreeHandle(&hnum)	;

	// if none, exit
	if(!handle)	return	NULL	;

	// link sample data with sample handle
	if(!AIL_set_sample_file(handle, ptrsample, -1 ))
	{
		return 0xFFFFFFFF	;
	}

	// Pichpend
	AIL_set_sample_user_data( handle, USER_DATA_RATE, AIL_sample_playback_rate(handle));
	if( pitchbend != 4096 )
	{
		AIL_set_sample_playback_rate(
			handle,
			Scale(AIL_sample_playback_rate(handle), pitchbend, 12)) ;

	}
	AIL_set_sample_user_data( handle, USER_DATA_PITCHBEND, pitchbend);

	// Loop sample ( 0 = indefinitely )
	AIL_set_sample_loop_count( handle, nbrepeat ) ;

	// Volume
	AIL_set_sample_user_data( handle, USER_DATA_VOLUME, volume);
	AIL_set_sample_volume( handle,
				SamplesFade?0:
#ifdef	_WIN32
				UseWaveMixer?volume:
#endif//_WIN32
				Scale(volume, LibWaveMasterVolume, 7));

	// Pan
	if(pan>127)		pan = 127	;
	else if(pan<0)		pan = 0		;
	AIL_set_sample_pan( handle, pan^InversePanMask);

	// GO !!!
	AIL_start_sample( handle )		;

	// Compute handle
	return GenerateSampleHandle(usernum, hnum);
}

//████████████████████████████████████████████████████████████████████████████

#ifdef _WIN32
static void WINAPI
#else
static void pascal
#endif
SampleCallBack(U32 user)
{
	user = user	;

	if(!SamplesPaused)
	{
		PlaySample(Timer_ptrsample, Timer_usernum, Timer_pitchbend, Timer_nbrepeat, Timer_volume, Timer_pan^InversePanMask);
	}
}

//████████████████████████████████████████████████████████████████████████████

U32	TimerPlaySample(void *ptrsample, U32 usernum, S32 pitchbend, S32 nbrepeat, S32 volume, S32 pan, S32 period)
{
	if( !Sample_Driver_Enabled )	return	FALSE	;

	if(!HandleTimer)
	{
#ifndef	_WIN32
		if(	!AIL_vmm_lock_range(&SampleCallBack, &TimerPlaySample) OR
			!AIL_vmm_lock_range(&HandleTimer, &Sample_Handle[0]))
		{
			HandleTimer = (HTIMER)-1;
			return	FALSE		;
		}
#endif
		HandleTimer = AIL_register_timer( SampleCallBack ) ;
		if (HandleTimer == -1)
		{
			return FALSE ;
		}
	}

	Timer_ptrsample	= ptrsample	;
	Timer_usernum	= usernum	;
	Timer_pitchbend	= pitchbend	;
	Timer_nbrepeat	= nbrepeat	;
	Timer_volume	= volume	;
	Timer_pan	= pan		;

	AIL_set_timer_period( HandleTimer, period*1000 ) ;
	AIL_start_timer( HandleTimer ) 	;
	return TRUE			;
}

//████████████████████████████████████████████████████████████████████████████

void	TimerStopSample()
{
	if( !Sample_Driver_Enabled )	return	;

	if(HandleTimer)
	{
		AIL_stop_timer( HandleTimer ) 	;
	}
}

//████████████████████████████████████████████████████████████████████████████
void	ChangePitchbendSample( U32 sample, S32 pitchbend)
{
	HSAMPLE	handle	;
	S32	hnum	;

	if( !Sample_Driver_Enabled )	return	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)		;

	// found the sample ?
	if(hnum==-1)			return 	;

	// Get sample handle
	handle = Sample_Handle[hnum].handle	;

	// check sample status
	if(AIL_sample_status( handle )==SMP_DONE)
		return	;

	// pitchend
	if( pitchbend != AIL_sample_user_data( handle, USER_DATA_PITCHBEND) )
	{
		AIL_set_sample_playback_rate(
			handle,
			Scale(AIL_sample_user_data( handle, USER_DATA_RATE), pitchbend, 12)) ;

		AIL_set_sample_user_data( handle, USER_DATA_PITCHBEND, pitchbend);
	}
}

//████████████████████████████████████████████████████████████████████████████

void	ChangeVolumePanSample( U32 sample, S32 volume, S32 pan)
{
	HSAMPLE	handle	;
	S32	hnum	;

	if( !Sample_Driver_Enabled )	return	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)		;

	// found the sample ?
	if(hnum==-1)			return 	;

	// Get sample handle
	handle = Sample_Handle[hnum].handle	;

	// check sample status
	if(AIL_sample_status( handle )==SMP_DONE)
		return	;

	// Volume
	AIL_set_sample_user_data( handle, USER_DATA_VOLUME, volume);
	AIL_set_sample_volume( handle,
				SamplesFade?0:
#ifdef	_WIN32
				UseWaveMixer?volume:
#endif//_WIN32
				Scale(volume, LibWaveMasterVolume, 7));

	// Pan
	if(pan>127)		pan = 127	;
	else if(pan<0)		pan = 0		;
	AIL_set_sample_pan( handle, pan^InversePanMask);
}

//████████████████████████████████████████████████████████████████████████████

HSAMPLE	GetSampleMSSHandle( U32 sample )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled )	return NULL	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)	;

	// found the sample ?
	if(hnum==-1)			return NULL	;

	// stop the sample
	return Sample_Handle[hnum].handle		;
}

//████████████████████████████████████████████████████████████████████████████

void	StopOneSample( U32 sample )
{
	HSAMPLE	handle	;
	S32	hnum	;

	if( !Sample_Driver_Enabled )	return	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)	;

	// found the sample ?
	if(hnum==-1)			return ;

	// stop the sample
	handle = Sample_Handle[hnum].handle		;
	AIL_end_sample(handle)				;
	Sample_Handle[hnum].sample = 0			;
}

//████████████████████████████████████████████████████████████████████████████

void	StopSamples()
{
	U32		hnum	;

	if( !Sample_Driver_Enabled )	return	;

	// Stop sample on timer
	TimerStopSample()	;

	// Stop everything!
	for(hnum=0; hnum<Nb_Voices; hnum++)
	{
		HSAMPLE	handle	;

		handle = Sample_Handle[hnum].handle	;
		AIL_end_sample(handle)			;
		Sample_Handle[hnum].sample = 0		;
	}

	// Reset seed to 0x100 to limit risks of overflow
	SampleSeed = 0x1000000	;
	SamplesPaused = 0	;
}

//████████████████████████████████████████████████████████████████████████████

void	PauseOneSample( U32 sample )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled )	return	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)		;

	// found the sample ?
	if(hnum==-1)			return	;

	// stop the sample
	AIL_stop_sample(Sample_Handle[hnum].handle)	;
}

//████████████████████████████████████████████████████████████████████████████

void	ResumeOneSample( U32 sample )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled )	return	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)		;

	// found the sample ?
	if(hnum==-1)			return	;

	// stop the sample
	AIL_resume_sample(Sample_Handle[hnum].handle)	;
}

//████████████████████████████████████████████████████████████████████████████

void	PauseSamples()
{
	U32		hnum	;

	if( !Sample_Driver_Enabled )	return	;

	SamplesPaused++;

	if(SamplesPaused>1)	//  already in pause ?
	{
		return	;	// Yes -> exit
	}

	// Pause everything
	for(hnum=0; hnum<Nb_Voices; hnum++)
	{
		AIL_stop_sample(Sample_Handle[hnum].handle)	;
	}
}

//████████████████████████████████████████████████████████████████████████████

void	ResumeSamples()
{
	U32		hnum	;

	if( !Sample_Driver_Enabled )	return	;

	if(!SamplesPaused)	// in pause ?
	{
		return	;	// No -> exit
	}

	SamplesPaused--	;	// decrement pause

	if(SamplesPaused)	// end of pause ?
	{
		return	;	// No -> exit
	}

	// Resume everything
	for(hnum=0; hnum<Nb_Voices; hnum++)
	{
		AIL_resume_sample(Sample_Handle[hnum].handle)	;
	}
}

//████████████████████████████████████████████████████████████████████████████

U32	IsSamplePlaying( U32 sample )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled )
		return	FALSE	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)			;

	// found the sample ?
	if(hnum==-1)			return 	FALSE 	;

	// get sample status
	return	AIL_sample_status(Sample_Handle[hnum].handle) == SMP_DONE ?
		FALSE				:
		Sample_Handle[hnum].timer	;
}

//████████████████████████████████████████████████████████████████████████████

S32	GetSamplePosition( U32 sample )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled )
		return	-1	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)			;

	// found the sample ?
	if(hnum==-1)			return 	-1	;

	// get sample pos
	return(AIL_sample_position(Sample_Handle[hnum].handle))	;

}

//████████████████████████████████████████████████████████████████████████████

S32	GetPlayingSamples(SAMPLE_PLAYING tab[], S32 max)
{
	S32	i, hnum	;

	hnum = 0	;

	for(i=0; i<Nb_Voices; i++)
	{
		if(hnum==max)
		{
			break	;	// list full
		}

		if(!Sample_Handle[i].sample)
		{
			continue;	// not playing
		}
		tab[hnum].Usernum 	= (Sample_Handle[i].sample>>8)&0xFFFF		;
		tab[hnum].Pitch  	= AIL_sample_user_data(Sample_Handle[i].handle, USER_DATA_PITCHBEND);
		tab[hnum].Repeat	= (U8)AIL_sample_loop_count(Sample_Handle[i].handle)	;
		tab[hnum].Volume	= (U8)AIL_sample_user_data(Sample_Handle[i].handle, USER_DATA_VOLUME);
		tab[hnum].Pan		= (U8)(AIL_sample_pan(Sample_Handle[i].handle)^InversePanMask);

		if(AIL_sample_status(Sample_Handle[i].handle) == SMP_DONE)
		{
			Sample_Handle[i].sample = 0	;
			continue;	// not playing
		}
		hnum++	;
	}

	return	hnum	;
}

//████████████████████████████████████████████████████████████████████████████

S32	GetSamplePoly()
{
	S32	hnum, count	;

	if( !Sample_Driver_Enabled )
		return	-1	;

	count = 0	;

	for(hnum=0; hnum<Nb_Voices; hnum++)
	{
		if(Sample_Handle[hnum].sample==0)
			continue	;
		if(AIL_sample_status(Sample_Handle[hnum].handle)==SMP_DONE)
			continue	;

		count++	;
	}

	return count	;
}

//████████████████████████████████████████████████████████████████████████████

void	SetDataSample( U32 sample, S32 index, S32 value )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled || (index>8-USER_DATA_USER))
		return		;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)			;

	// found the sample ?
	if(hnum==-1)			return 		;

	// set data
	AIL_set_sample_user_data(Sample_Handle[hnum].handle, USER_DATA_USER+index, value)	;
}

//████████████████████████████████████████████████████████████████████████████

S32	GetDataSample( U32 sample, S32 index )
{
	S32	hnum	;

	if( !Sample_Driver_Enabled || (index>8-USER_DATA_USER))
		return	0	;

	// check if user or lib handle and do conversion
	hnum = GetHandleIndice(sample)			;

	// found the sample ?
	if(hnum==-1)			return 	0	;

	// get data
	return(AIL_sample_user_data(Sample_Handle[hnum].handle, USER_DATA_USER+index))	;
}

//████████████████████████████████████████████████████████████████████████████
