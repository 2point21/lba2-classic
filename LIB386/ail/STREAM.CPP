#include	<system\adeline.h>

#ifdef	_WIN32
#include	<system\winsys.h>
#include	<ail\mssw.h>
#else
#include	<system\dossys.h>
#include	<ail\mss.h>
#endif

#include	<ail\common.h>
#include	<ail\sample.h>
#include	<ail\stream.h>

#include 	<stdlib.h>
#include 	<string.h>

//████████████████████████████████████████████████████████████████████████████

#define	Scale(value, scale, range)	( ( (value)*(scale) + (1<<(range-1)) ) >> range )

//████████████████████████████████████████████████████████████████████████████

extern	HDIGDRIVER	DigDriver	;// Handle to digital audio driver
extern	S32		LibWaveMasterVolume	;

//████████████████████████████████████████████████████████████████████████████

static	HSTREAM		Stream			;
static	S32		StreamVolume = 127	;
static	S32		PausedPos		;
static	char		StreamPathName[_MAX_PATH];
static	char		PausedPathName[_MAX_PATH];

//████████████████████████████████████████████████████████████████████████████

static	VOID_FUNC	*StreamManageProc	;

//████████████████████████████████████████████████████████████████████████████

static	VOID_FUNC	ManageStream		;

//████████████████████████████████████████████████████████████████████████████

static void	ManageStream()
{
	if( Sample_Driver_Enabled && Stream )
	{
		AIL_service_stream(Stream, 0)	;
	}

	if(StreamManageProc)
	{
		StreamManageProc()	;
	}
}

//████████████████████████████████████████████████████████████████████████████

static void PlayThisStream(char *name, S32 pos)
{
	if( !Sample_Driver_Enabled )	return	;

	StopStream()	;

	Stream = AIL_open_stream(DigDriver, name, 0)	;

	if(!Stream)			return	;

	strcpy(StreamPathName, name)		;

	AIL_set_stream_position(Stream, pos)	;

	ChangeVolumeStream(
#ifdef	_WIN32
			UseWaveMixer?StreamVolume:
#endif//_WIN32
			Scale(StreamVolume, LibWaveMasterVolume, 7));

	AIL_start_stream(Stream)		;

	ManageStream()				;
}

//████████████████████████████████████████████████████████████████████████████

void	OpenStream()
{
	static	char init = TRUE	;

	if(init)
	{
		StreamManageProc = ManageProc	;
		ManageProc	 = ManageStream	;
		init 		 = FALSE	;
	}
}

//████████████████████████████████████████████████████████████████████████████

void	PlayStream(char *name)
{
	PlayThisStream(name, 0)	;
}

//████████████████████████████████████████████████████████████████████████████

void	ChangeVolumeStream(S32 volume)
{
	if(volume>127)
	{
		volume = 127	;
	}
	else if (volume<0)
	{
		volume = 0	;
	}

	StreamVolume = volume	;

	if( !Sample_Driver_Enabled || !Stream )	return ;

	AIL_set_stream_volume(Stream,
#ifdef	_WIN32
			UseWaveMixer?StreamVolume:
#endif//_WIN32
			Scale(StreamVolume, LibWaveMasterVolume, 7));

}

//████████████████████████████████████████████████████████████████████████████

S32	GetVolumeStream()
{
	return	StreamVolume	;
}

//████████████████████████████████████████████████████████████████████████████

void	StopStream()
{
	if( !Sample_Driver_Enabled || !Stream )	return ;

	AIL_close_stream(Stream)	;

	Stream = NULL		;
	StreamPathName[0] = 0	;
}

//████████████████████████████████████████████████████████████████████████████

void	PauseStream()
{
	if(!IsStreamPlaying())		return	;

	AIL_pause_stream(Stream, TRUE)		;

	ManageStream()				;

	PausedPos = AIL_stream_position(Stream)	;
	strcpy(PausedPathName, StreamPathName)	;

	StopStream()				;
}

//████████████████████████████████████████████████████████████████████████████

void	ResumeStream()
{
	if( !PausedPathName[0] )	return ;

	PlayThisStream(PausedPathName, PausedPos) ;

	PausedPathName[0] = 0	;
}

//████████████████████████████████████████████████████████████████████████████

S32	IsStreamPlaying()
{
	S32	playing	;

	if( !Sample_Driver_Enabled || !Stream )	return FALSE ;

	ManageStream()			;

	playing = (AIL_stream_status(Stream) != SMP_DONE) ;

	if(!playing)	StopStream()	;

	return	playing	;
}

//████████████████████████████████████████████████████████████████████████████

char	*StreamName()
{
	IsStreamPlaying()	;

	return	StreamPathName	;
}

//████████████████████████████████████████████████████████████████████████████
