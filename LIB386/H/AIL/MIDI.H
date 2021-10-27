//--------------------------------------------------------------------------
#ifndef	LIB_AIL_MIDI
#define	LIB_AIL_MIDI

//--------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------
#define	OPL2			1
#define	OPL3			2
#define	MIDI			3

//--------------------------------------------------------------------------
extern	char		MidiDriverName[]	;

extern	S32		Midi_Driver_Enabled	;
extern	S32		Midi_Driver_Type	;

//--------------------------------------------------------------------------
extern	VOID_FUNC	ClearMidiDriver		;
extern	VOID_FUNC	PauseMidi		;
extern	VOID_FUNC	ResumeMidi		;

//--------------------------------------------------------------------------
S32	InitMidiDriver(	char *driver_name		)	;

//--------------------------------------------------------------------------
//	ReserMidi : 			Reset all controlers in Midi Synth
//
//	Returns	: 			nothing
//--------------------------------------------------------------------------
void	ResetMidi(		void			)	;

//--------------------------------------------------------------------------
//	PrepareMidi : 			Prepare a Midi for playback
//
//			  ptrxmi :	xmi pointer
//			  seqnum :	sequence num
//			  volume : 	Volume (127 max)
//
//	Returns	: 			boolean
//--------------------------------------------------------------------------
U32	PrepareMidi( 		void *ptrxmi,
				S32  seqnum,
				S32  volume 		)	;

//--------------------------------------------------------------------------
//	ClearMidi :			stop midi until next PrepareMidi
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ClearMidi(		void			)	;

//--------------------------------------------------------------------------
//	PlayBlockMidi :			Play a Midi block
//
//			  blocknum :	block num
//
//	Returns	: 			nothing
//--------------------------------------------------------------------------
void	PlayBlockMidi( 		S32 blocknum		)	;

//--------------------------------------------------------------------------
//	ChainBlockMidi :		Chain a Midi block in the sequence
//
//			  blocknum :	block num
//
//	Returns	: 			nothing
//--------------------------------------------------------------------------
void	ChainBlockMidi(		S32 blocknum		)	;

//--------------------------------------------------------------------------
//	ChangeVolumeMidi :		Change the midi volume
//
//			  volume : 	Volume (127 max)
//			  delay	 : 	time to change the volume (in ms)
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ChangeVolumeMidi(	S32 volume,
				S32 delay		)	;

//--------------------------------------------------------------------------
//	StopMidi :			stop midi
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	StopMidi(		void			)	;

//--------------------------------------------------------------------------
//	PauseMidi :			pause midi
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ResumeMidi(		void			)	;

//--------------------------------------------------------------------------
//	ResumeMidi :			resume midi
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	ResumeMidi(		void			)	;

//--------------------------------------------------------------------------
//	StopChainMidi :			Stop chaining the midi sequence.
//					Current track keep playing
//
//	Returns	:			nothing
//--------------------------------------------------------------------------
void	StopChainMidi(		void			)	;

//--------------------------------------------------------------------------
//	GetBlockPlaying :		check which Midi block is playing
//
//	Returns	:			Midi block number (-1 if none)
//--------------------------------------------------------------------------
S32	GetBlockPlayingMidi(	void			)	;

//--------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_AIL_MIDI

//--------------------------------------------------------------------------

