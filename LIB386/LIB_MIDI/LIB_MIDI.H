#ifdef	WATCOM9
#pragma	library ("f:\projet\lib386\lib_midi\lb9_midi.lib");
#else
#pragma	library ("f:\projet\lib386\lib_midi\lib_midi.lib");
#endif

#include "f:\projet\lib386\lib_midi\ail32.h"      // Audio Interface Library API function header

extern	char	Midi_Driver[_MAX_PATH] ;
extern  char	Midi_Driver_Name[80] ;
extern	WORD	Midi_Driver_Enable ;

extern	char GTL_filename[_MAX_PATH] ;		// fichier des sons

extern void AskMidiVars(char ***,long **);
extern long InitMidiDLL(char *);
extern long InitMidi(void);
extern void InitPathMidiSampleFile(unsigned char *);
extern void ClearMidi(void);
extern void PlayMidi(unsigned char *);
extern void StopMidi(void);
extern long IsMidiPlaying(void);
extern void FadeMidiDown(short );
extern void FadeMidiUp(short );
extern void WaitFadeMidi(void);
extern void VolumeMidi(short );
extern void SetLoopMidi(short );
extern void DoLoopMidi(void);
