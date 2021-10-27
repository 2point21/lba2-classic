//--------------------------------------------------------------------------
#ifndef	LIB_AIL_SAMPLE
#define	LIB_AIL_SAMPLE

//--------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------
extern	char		SampleDriverName[]	;

extern	S32		Sample_Driver_Enabled	;

extern	S32		SamplingRate		;

extern	S32		SampleOverflow		;

//--------------------------------------------------------------------------
typedef	struct {
		S32	Usernum	;
		S32	Pitch  	;
		U8	Repeat	;
		U8	Volume	;
		U8	Pan	;
		U8	Dummy	;
	}	SAMPLE_PLAYING	;

//--------------------------------------------------------------------------
extern	VOID_FUNC	ClearSampleDriver	;
extern	VOID_FUNC	PauseSamples		;
extern	VOID_FUNC	ResumeSamples		;

//--------------------------------------------------------------------------
#ifdef	_WIN32
extern	S32		UseWaveMixer		;
#endif//_WIN32

//--------------------------------------------------------------------------
S32	InitSampleDriver(	char *driver_name	)	;

//--------------------------------------------------------------------------
//	SetMasterVolumeSample :		Set master volume for all samples
//
//			  volume : 	master volume (0-127)
//
//	Returns	: 			Nothing
//--------------------------------------------------------------------------
void	SetMasterVolumeSample(	S32 volume 		)	;

//--------------------------------------------------------------------------
//	GetMasterVolumeSample :		Get master volume for all samples
//
//	Returns	: 			Master Volume
//--------------------------------------------------------------------------
S32	GetMasterVolumeSample(	void			)	;

//--------------------------------------------------------------------------
//	FadeOutSamples :		Fade out all samples (except stream)
//
//			  delay : 	delay for fade (in ms)
//
//	Returns	: 			scaling factor (from delay to 0)
//--------------------------------------------------------------------------
S32	FadeOutSamples(		S32 delay 		)	;

//--------------------------------------------------------------------------
//	FadeInSamples :			Fade in all samples (except stream)
//
//			  delay : 	delay for fade (in ms)
//
//	Returns	: 			scaling factor (from 0 to delay)
//--------------------------------------------------------------------------
S32	FadeInSamples(		S32 delay 		)	;

//--------------------------------------------------------------------------
//	InverseStereoSample :		Reverse left and right channels
//
//			  inverse : 	TRUE for inverse, FALSE to reset
//
//	Returns	: 			Nothing
//--------------------------------------------------------------------------
void	InverseStereoSample(	S32 inverse 		)	;

//--------------------------------------------------------------------------
//	PlaySample : 			Play a Sample
//
//			  buffer    :	Pointer to the sample in mem
//			  userhandle: 	user handle (0 - 0xFFFF)
//			  pitchbend : 	4096 is no bend
//			  repeat    : 	number of times to play the sample
//			  volume    : 	Volume (127 max, clipped if > 127)
//			  pan	    : 	Paning (0-127, 64 dead center)
//
//	Returns	: 			lib sample handle
//--------------------------------------------------------------------------
U32	PlaySample( 		void *buffer,
				U32 userhandle,
				S32 pitchbend,
				S32 repeat,
				S32 volume,
				S32 pan		)	;

//--------------------------------------------------------------------------
//	TimerPlaySample :		Play a Sample and repeat with period
//
//			  buffer    :	Pointer to the sample in mem
//			  userhandle: 	user handle (0 - 0xFFFF)
//			  pitchbend : 	4096 is no bend
//			  repeat    : 	number of times to play the sample
//			  volume    : 	Volume (127 max)
//			  pan	    : 	Paning (0-127, 64 dead center)
//			  period    :   in ms
//
//	Returns	: 			TRUE / FALSE
//--------------------------------------------------------------------------
U32	TimerPlaySample(	void *ptrsample,
				U32 usernum,
				S32 pitchbend,
				S32 nbrepeat,
				S32 volume,
				S32 pan,
				S32 period	);

//--------------------------------------------------------------------------
//	TimerStopSample :		stop TimerPlaySample
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	TimerStopSample(	void		)	;

//--------------------------------------------------------------------------
//	ChangePitchbendSample :		Change a sample pitchbend
//
//			  sample :	sample handle
//			  pitchbend : 	4096 is no bend
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ChangePitchbendSample(	U32 sample,
				S32 pitchbend	)	;

//--------------------------------------------------------------------------
//	ChangeVolumePanSample :		Change a sample	volume and paning
//
//			  sample :	sample handle
//			  volume : 	Volume (127 max)
//			  pan    : 	Paning (0-127, 64 dead center)
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ChangeVolumePanSample(	U32 sample,
				S32 volume,
				S32 pan		)	;

//--------------------------------------------------------------------------
//	GetSampleMSSHandle :		return MSS handle for a sample
//
//			  sample :	sample handle
//
//	Returns	:			HSAMPLE (see MSS doc)
//--------------------------------------------------------------------------
HSAMPLE	GetSampleMSSHandle( 	U32 sample 	)	;

//--------------------------------------------------------------------------
//	StopOneSample :			stop a sample
//
//			  sample :	sample handle
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	StopOneSample( 		U32 sample 		)	;

//--------------------------------------------------------------------------
//	StopSample :			stop all samples
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	StopSamples(		void			)	;

//--------------------------------------------------------------------------
//	PauseOneSample :		pause a sample
//
//			  sample :	sample handle
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	PauseOneSample(		U32 sample 		)	;

//--------------------------------------------------------------------------
//	ResumeOneSample :		resume a sample
//
//			  sample :	sample handle
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ResumeOneSample(	U32 sample 		)	;

//--------------------------------------------------------------------------
//	PauseSample :			pause all samples
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	PauseSamples(		void			)	;

//--------------------------------------------------------------------------
//	ResumeSample :			resume all samples
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ResumeSamples(		void			)	;

//--------------------------------------------------------------------------
//	GetPlayingSamples :		get list of all playing samples
//
//			  tab :		array of sample descriptor
//			  max :		max number of samples in list
//
//	Returns	:			number of samples playing
//--------------------------------------------------------------------------
S32	GetPlayingSamples(	SAMPLE_PLAYING tab[],
				S32 max			)	;

//--------------------------------------------------------------------------
//	IsSamplePlaying :		check if a sample is playing
//
//			  sample :	sample handle
//
//	Returns	:			FALSE or starting TimerRefHR
//--------------------------------------------------------------------------
U32	IsSamplePlaying( 	U32 sample 		)	;

//--------------------------------------------------------------------------
//	GetSamplePostion :		get sample number currently playing
//
//			  sample :	sample handle
//
//	Returns	:			sample position
//--------------------------------------------------------------------------
S32	GetSamplePosition( U32 sample )				;

//--------------------------------------------------------------------------
//	GetSamplePostion :		get sample polyphonie
//
//	Returns	:			number of samples playing
//--------------------------------------------------------------------------
S32	GetSamplePoly( void )					;

//--------------------------------------------------------------------------
//	GetSamplePostion :		get a piece of data
//
//			  sample :	sample handle
//			  index  :	data index (0-6)
//			  value  :	data value
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	SetDataSample( U32 sample, S32 index, S32 value )	;

//--------------------------------------------------------------------------
//	GetSamplePostion :		get a piece of data
//
//			  sample :	sample handle
//			  index :	data index (0-6)
//
//	Returns	:			value
//--------------------------------------------------------------------------
S32	GetDataSample( U32 sample, S32 index )			;

//--------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_AIL_SAMPLE

//--------------------------------------------------------------------------
