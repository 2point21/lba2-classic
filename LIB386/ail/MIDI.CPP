#include	<system\adeline.h>
#include	<system\logprint.h>
#include	<system\exit.h>

#include	<ail\common.h>
#include	<ail\midi.h>

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>

#ifdef	_WIN32
#include	<system\winsys.h>
#include	<ail\mssw.h>
#else
#include	<ail\mss.h>
#endif

//████████████████████████████████████████████████████████████████████████████

#pragma	aux AIL_error	"_*"

//████████████████████████████████████████████████████████████████████████████
#define		MAX_SEQUENCE	1

//████████████████████████████████████████████████████████████████████████████
typedef struct
	{
		S32		block_num;
	}
	SEQ_DESC	;

extern	HDIGDRIVER	DigDriver			;// Handle to digital driver
extern 	HMDIDRIVER	MdiDriver			;// Handle to midi driver

	HSEQUENCE      	Midi_Handle	= NULL		;

static	SEQ_DESC	Sequence[MAX_SEQUENCE]		;
static	volatile S32	Write_Seq	= -1		;
static	volatile S32	Read_Seq       	= 0		;
static	volatile S32	BlockPlaying	= -1		;
static	volatile S32	WaitReset	= FALSE		;
static	S32		MidiPaused	= 0		;

//████████████████████████████████████████████████████████████████████████████

static U8 ResetMidiXMI[]=
{
	0x46,0x4f,0x52,0x4d,0x00,0x00,0x00,0x0e,0x58,0x44,
	0x49,0x52,0x49,0x4e,0x46,0x4f,0x00,0x00,0x00,0x02,
	0x01,0x00,0x43,0x41,0x54,0x20,0x00,0x00,0x01,0x4e,
	0x58,0x4d,0x49,0x44,0x46,0x4f,0x52,0x4d,0x00,0x00,
	0x01,0x42,0x58,0x4d,0x49,0x44,0x45,0x56,0x4e,0x54,
	0x00,0x00,0x01,0x36,0xff,0x51,0x03,0x07,0xa1,0x20,
	0xff,0x58,0x04,0x04,0x02,0x18,0x08,0xb0,0x7b,0x7f,
	0xb1,0x7b,0x7f,0xb2,0x7b,0x7f,0xb3,0x7b,0x7f,0xb4,
	0x7b,0x7f,0xb5,0x7b,0x7f,0xb6,0x7b,0x7f,0xb7,0x7b,
	0x7f,0xb8,0x7b,0x7f,0xb9,0x7b,0x7f,0xba,0x7b,0x7f,
	0xbb,0x7b,0x7f,0xbc,0x7b,0x7f,0xbd,0x7b,0x7f,0xbe,
	0x7b,0x7f,0xbf,0x7b,0x7f,0x03,0xb0,0x65,0x7f,0xb1,
	0x65,0x7f,0xb2,0x65,0x7f,0xb3,0x65,0x7f,0xb4,0x65,
	0x7f,0xb5,0x65,0x7f,0xb6,0x65,0x7f,0xb7,0x65,0x7f,
	0xb8,0x65,0x7f,0xb9,0x65,0x7f,0xba,0x65,0x7f,0xbb,
	0x65,0x7f,0xbc,0x65,0x7f,0xbd,0x65,0x7f,0xbe,0x65,
	0x7f,0xbf,0x65,0x7f,0x04,0xb0,0x06,0x02,0xb1,0x06,
	0x02,0xb2,0x06,0x02,0xb3,0x06,0x02,0xb4,0x06,0x02,
	0xb5,0x06,0x02,0xb6,0x06,0x02,0xb7,0x06,0x02,0xb8,
	0x06,0x02,0xb9,0x06,0x02,0xba,0x06,0x02,0xbb,0x06,
	0x02,0xbc,0x06,0x02,0xbd,0x06,0x02,0xbe,0x06,0x02,
	0xbf,0x06,0x02,0x04,0xe0,0x00,0x40,0xe1,0x00,0x40,
	0xe2,0x00,0x40,0xe3,0x00,0x40,0xe4,0x00,0x40,0xe5,
	0x00,0x40,0xe6,0x00,0x40,0xe7,0x00,0x40,0xe8,0x00,
	0x40,0xe9,0x00,0x40,0xea,0x00,0x40,0xeb,0x00,0x40,
	0xec,0x00,0x40,0xed,0x00,0x40,0xee,0x00,0x40,0xef,
	0x00,0x40,0x03,0xb0,0x07,0x7f,0xb1,0x07,0x7f,0xb2,
	0x07,0x7f,0xb3,0x07,0x7f,0xb4,0x07,0x7f,0xb5,0x07,
	0x7f,0xb6,0x07,0x7f,0xb7,0x07,0x7f,0xb8,0x07,0x7f,
	0xb9,0x07,0x7f,0xba,0x07,0x7f,0xbb,0x07,0x7f,0xbc,
	0x07,0x7f,0xbd,0x07,0x7f,0xbe,0x07,0x7f,0xbf,0x07,
	0x7f,0x04,0xb0,0x79,0x7f,0xb1,0x79,0x7f,0xb2,0x79,
	0x7f,0xb3,0x79,0x7f,0xb4,0x79,0x7f,0xb5,0x79,0x7f,
	0xb6,0x79,0x7f,0xb7,0x79,0x7f,0xb8,0x79,0x7f,0xb9,
	0x79,0x7f,0xba,0x79,0x7f,0xbb,0x79,0x7f,0xbc,0x79,
	0x7f,0xbd,0x79,0x7f,0xbe,0x79,0x7f,0xbf,0x79,0x7f,
	0x0b,0xff,0x2f,0x00,0xf8
};

//████████████████████████████████████████████████████████████████████████████

VOID_FUNC	SpecialInitMidiDriver	;

//████████████████████████████████████████████████████████████████████████████

S32	InitMidiDriver( char *driver_name )
{
#ifndef	_WIN32
	S32	type ;
#endif

	if( Midi_Driver_Enabled )
	{
		return TRUE	;
	}

#ifdef	_WIN32

	driver_name = driver_name ;

	if(AIL_midiOutOpen(&MdiDriver, NULL, (WORD) MIDI_MAPPER))
	{
		if(AIL_midiOutOpen(&MdiDriver, NULL, 0))
		{
			MdiDriver = NULL	;
			return FALSE		;
		}
	}
	Midi_Driver_Type = MIDI ;

#else
 	// Set application preferences
	AIL_set_preference(MDI_SEQUENCES,		1	) ;
	AIL_set_preference(AIL_SCAN_FOR_HARDWARE,	YES	) ;

	// Initialize xmi system
	if(driver_name && strcmp(driver_name, ""))
	{
		strcpy(MidiDriverName, driver_name)	;
		MdiDriver = AIL_install_MDI_driver_file( driver_name, NULL );
	}
	else
	{
		LONG res	;

		MidiDriverName[0] = (char)0	;

		res=AIL_install_MDI_INI(&MdiDriver)	;
		switch(res)
		{
		case AIL_INIT_SUCCESS:
			break;
		case AIL_NO_INI_FILE:
			LogPuts( "No Midi Driver specified. Midi Disabled." );
			return TRUE		;
		default:
			MdiDriver = NULL	;
			LogPuts("Please Run Setsound.exe again.");
			break			;
		}
	}

	if( MdiDriver == NULL )
	{
		LogPuts( (char*)AIL_error )	;
		return FALSE		;
	}

	LogPrintf(	"Midi device : %s\nMidi device type  : ",
			MdiDriver->drvr->VHDR->driver_version < 0x112	?
			MidiDriverName					:
			(char*)(MdiDriver->drvr->VHDR->dev_name)	);

	type = AIL_MDI_driver_type(MdiDriver)	;
	if( type == MDIDRVRTYPE_SPKR )
	{
		AIL_uninstall_MDI_driver( MdiDriver )	;
		LogPuts("PC Speaker\nCan't handle PC speaker type device.");
		return FALSE		;
	}

	switch(type)
	{
		case MDIDRVRTYPE_FM_2:
			Midi_Driver_Type = OPL2 ;
			LogPuts("FM OPL2")	;
			break			;
		case MDIDRVRTYPE_FM_4:
			Midi_Driver_Type = OPL3 ;
			LogPuts("FM OPL3")	;
			break			;
		default:
			Midi_Driver_Type = MIDI ;
			LogPuts("General Midi / MT 32");
			break			;
	}
#endif

	Midi_Handle = AIL_allocate_sequence_handle(MdiDriver)	;

	if(!Midi_Handle)
	{
		LogPuts("Can't allocate midi handle.")	;
		return	FALSE				;
	}

	Midi_Driver_Enabled	= TRUE 	;

	Write_Seq	= -1	;
	Read_Seq	= 0	;

	RegisterInitClear( SpecialInitMidiDriver, ClearMidiDriver );// Automatic clear

#ifdef	_WIN32
        PauseMidiPtr	= PauseMidi	;
	if(PauseMiles)
	{
		PauseMiles	= PauseBothMiles;
	}
	else
	{
	        PauseMiles	= PauseMidi	;
	}
        ResumeMidiPtr	= ResumeMidi	;
	if(ResumeMiles)
	{
		ResumeMiles	= ResumeBothMiles;
	}
	else
	{
	        ResumeMiles	= ResumeMidi	;
	}
#endif

	return TRUE		;
}

//████████████████████████████████████████████████████████████████████████████

static void SpecialInitMidiDriver()
{
	InitMidiDriver(MidiDriverName);
}

//████████████████████████████████████████████████████████████████████████████

void	ClearMidiDriver()
{
	if( !Midi_Driver_Enabled )	return	;

	// Shut down AIL MDI system and exit

	Midi_Driver_Enabled = FALSE 		;
}

//████████████████████████████████████████████████████████████████████████████

#ifdef _WIN32
static void WINAPI
#else
static void pascal
#endif
TriggerCallBack(HSEQUENCE handle, S32 channel, S32 value)
{
	// Dummy code to avoid warnings (doesn't actually generate code)
	handle	= handle	;
	channel = channel	;
	value	= value		;
	// End Dummy

	// if not prog change and not the right block info then
	// it must be because start of block 1 is just before end of block 2
	// for example
	if((value!=0)&&(value!=BlockPlaying))
	{
		return	;
	}

	// if seq list is empty then stop playback
	if((Read_Seq==Write_Seq)||(Write_Seq==-1))
	{
		Write_Seq	= -1		;
		Read_Seq	= 0		;
		BlockPlaying	= -1		;
		AIL_stop_sequence(Midi_Handle)	;
		return				;
	}

	// Set flags
	BlockPlaying	= Sequence[Read_Seq].block_num	;

	// increment counter and loop if nescessary
	Read_Seq++	;
	if(Read_Seq>=MAX_SEQUENCE)
	{
		Read_Seq = 0 	;
	}

	// branch to right index
	AIL_branch_index(Midi_Handle, BlockPlaying)	;
}

//████████████████████████████████████████████████████████████████████████████

#ifdef _WIN32
static void WINAPI
#else
static void pascal
#endif
SequenceCallBack(HSEQUENCE handle)
{
	// Dummy code to avoid warnings (doesn't actually generate code)
	handle	= handle	;

	// Ok reset is done whatsoever
	WaitReset = FALSE	;

	// Check if not end from end_sequence in trigger_callback
	if(BlockPlaying==-1)
	{
		return	;
	}

	// Restart sequence as it should never actually end
	AIL_start_sequence(Midi_Handle)	;

	// process next block in sequence
	TriggerCallBack(0, 0, BlockPlaying);
}

//████████████████████████████████████████████████████████████████████████████

void	ResetMidi()
{
	if( !Midi_Driver_Enabled )		return	;

	// If already doing reset then exit
	if (WaitReset)				return  ;

	// End sequence if playing
	StopMidi()					;

	// link xmi data with xmi handle
	if(!AIL_init_sequence(Midi_Handle, (void *)ResetMidiXMI, 0))
	{
		return					;
	}

	// Set callback
	AIL_register_sequence_callback(Midi_Handle, SequenceCallBack);

	// Loop xmi ( 0 = indefinitely )
	AIL_set_sequence_loop_count( Midi_Handle, 1 ) 	;

	WaitReset = TRUE				;

	AIL_start_sequence(Midi_Handle)			;
}

//████████████████████████████████████████████████████████████████████████████

U32	PrepareMidi( void *ptrxmi, S32 seq_num, S32 volume)
{
	if( !Midi_Driver_Enabled )	return FALSE ;

	// Reset controllers
	ResetMidi()		;

	// Wait for Reset to complete
	while(WaitReset)	;

	// link xmi data with xmi handle
	if(!AIL_init_sequence(Midi_Handle, ptrxmi, seq_num))
	{
		return FALSE	;
	}

	// Loop xmi ( 0 = indefinitely )
	AIL_set_sequence_loop_count( Midi_Handle, 1 ) 	;

	// Set callback
	AIL_register_trigger_callback(Midi_Handle, TriggerCallBack);
	AIL_register_sequence_callback(Midi_Handle, SequenceCallBack);

	// Volume
	AIL_set_sequence_volume( Midi_Handle, volume, 0) ;

	// Playing program changes
	BlockPlaying = 0		;

	// Go
	AIL_start_sequence(Midi_Handle)	;

	return TRUE			;
}

//████████████████████████████████████████████████████████████████████████████

void	ClearMidi()
{
	// Nothing playing
	Write_Seq	= -1		;
	Read_Seq       	= 0		;
	BlockPlaying	= -1		;
	MidiPaused	= 0		;

	AIL_end_sequence(Midi_Handle)	;
}

//████████████████████████████████████████████████████████████████████████████

void	PlayBlockMidi(S32 block_num)
{
	if( !Midi_Driver_Enabled )	return 	;

	// Stop Midi
	StopMidi()				;

	// block playing : block_num
	BlockPlaying		= block_num	;
	Write_Seq		= 0		;

	// branch to right index
	AIL_branch_index(Midi_Handle, BlockPlaying);

	// Go
	AIL_resume_sequence(Midi_Handle)	;
}

//████████████████████████████████████████████████████████████████████████████

void	ChainBlockMidi(S32 block_num)
{
	S32	n, m	;

	if( !Midi_Driver_Enabled )	return 	;

	// Insert sequence in chain
	m = Read_Seq	;
	if(Write_Seq<0)
	{
		n		= 1	;
		Write_Seq	= 0	;
	}
	else
	{
		n = Write_Seq + 1	;
		if( n >= MAX_SEQUENCE )
	 	{
			n = 0		;
		}
	}
	if (n == m)			return	;	// Overflow

	Sequence[Write_Seq].block_num = block_num;
	Write_Seq = n 				;

	// If music not actually playing
	if(BlockPlaying==-1)
	{
		PlayBlockMidi(block_num)	;
	}
}

//████████████████████████████████████████████████████████████████████████████

void	ChangeVolumeMidi(LONG volume, S32 delay)
{
	if( !Midi_Driver_Enabled )	return	;

	// Volume
	AIL_set_sequence_volume( Midi_Handle, volume, delay)	;
}

//████████████████████████████████████████████████████████████████████████████

void	StopChainMidi()
{
	if( !Midi_Driver_Enabled )	return	;

	// Reset chain
	Write_Seq	= -1	;
	Read_Seq	= 0	;
}

//████████████████████████████████████████████████████████████████████████████

void	StopMidi( )
{
	if( !Midi_Driver_Enabled )	return	;

	// Nothing playing
	Write_Seq	= -1		;
	Read_Seq       	= 0		;
	BlockPlaying	= -1		;
	MidiPaused	= 0		;

	// Wait for Reset to complete
	while(WaitReset)		;

	// Stop PlayBack
	AIL_stop_sequence(Midi_Handle)	;
}

//████████████████████████████████████████████████████████████████████████████

void	PauseMidi( )
{
	if( !Midi_Driver_Enabled )	return	;

	if(!MidiPaused)
	{
		// Wait for Reset to complete
		while(WaitReset)	;

		if(BlockPlaying==-1)
		{
			return	;	// Already paused
		}

		// Pause PlayBack
		AIL_stop_sequence(Midi_Handle)	;
	}
	MidiPaused++	;
}

//████████████████████████████████████████████████████████████████████████████

void	ResumeMidi( )
{
	if( !Midi_Driver_Enabled )	return	;

	if(MidiPaused)
	{
		MidiPaused--	;

		if(!MidiPaused)
		{
			// Resume PlayBack
			AIL_resume_sequence(Midi_Handle);
		}
	}
}

//████████████████████████████████████████████████████████████████████████████

S32	GetBlockPlayingMidi( )
{
	if( !Midi_Driver_Enabled )	return	-1;

	return BlockPlaying	;
}

//████████████████████████████████████████████████████████████████████████████
