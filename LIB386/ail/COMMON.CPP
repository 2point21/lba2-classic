#include	<system\adeline.h>

#include	<ail\midi.h>
#include	<ail\common.h>

#include	<stdlib.h>

#ifdef	_WIN32
#include	<ail\mssw.h>
#else
#include	<ail\mss.h>
#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
HMDIDRIVER	MdiDriver		= NULL		;// Handle to midi driver
S32	       	Midi_Driver_Enabled	= FALSE		;
S32	       	Midi_Driver_Type	= MIDI		;

HDIGDRIVER	DigDriver		= NULL		;// Handle to digital audio driver
S32	       	Sample_Driver_Enabled	= FALSE		;
S32		SamplingRate		= 22050		;

char		MidiDriverName[_MAX_PATH]=""		;
char		SampleDriverName[_MAX_PATH]=""		;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
#ifdef	_WIN32

VOID_FUNC       *PauseMidiPtr    	= NULL		;
VOID_FUNC       *ResumeMidiPtr   	= NULL		;

VOID_FUNC       *PauseSamplesPtr 	= NULL		;
VOID_FUNC       *ResumeSamplesPtr	= NULL		;

#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
#ifdef	_WIN32

void	PauseBothMiles()
{
	(*PauseSamplesPtr)()	;
	(*PauseMidiPtr)()	;
}

#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
#ifdef	_WIN32

void	ResumeBothMiles()
{
	(*ResumeMidiPtr)()	;
	(*ResumeSamplesPtr)()	;
}

#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
