#include <stdlib.h>
#include <string.h>
#include <process.h>

#include "f:\projet\lib386\lib_sys\adeline.h"
#include "f:\projet\lib386\lib_sys\lib_sys.h"

#include "f:\projet\lib386\lib_midi\lib_midi.h"
#include "f:\projet\lib386\lib_samp\lib_wave.h"
#include "f:\projet\lib386\lib_mix\lib_mix.h"
#include "f:\projet\lib386\lib_svga\lib_svga.h"


char	Driver[_MAX_PATH] ;

// timer proc for ail_lib

HTIMER	HandleTimer ;

void	NewProc08() ; // dans timer.asm

extern	UBYTE	VESA_Error ;

char	PathConfigFile[_MAX_PATH] ;

char	*Adeline="ADELINE" ;

char	MidiFM = 1 ;

/*══════════════════════════════════════════════════════════════════════════*
		      █▀▀▀▀ █▀▀▀█ ██▄ █ █▀▀▀▀  █    █▀▀▀▀
		      ██    ██  █ ██▀██ ██▀▀   ██   ██ ▀█
		      ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀     ▀▀   ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

WORD	GiveIndexTab( UBYTE **tab, UBYTE *identificateur )
{
	UBYTE	**ptr ;
	WORD	i = 0 ;

	while( *tab[0] != 0 )
	{
		if( !stricmp( *tab, identificateur ) )
		{
			return i ;
		}
		tab++ ;
		i++ ;
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	InitAdelineSystem( char *name, LONG inits )
{
	char	**ptridentifier ;
	LONG	*ptrvar ;
	char	*ptr, *midiptr=NULL ;
	char	*defname ;
	char	*rien="" ;

	printf( "\nCopyright (c) Adeline Software International 1994, All Rights Reserved.\n\n" );

	*PathConfigFile = 0 ;

	if (!inits) return ;

	if (inits & INIT_VESA)
		inits |= INIT_SVGA ;

	defname = getenv(Adeline) ;

	if ((inits & ~(INIT_MCGA | INIT_VESA | INIT_SVGA)) OR (inits == INIT_SVGA))
	{
		if(!*name)
		{
			printf("Error: Empty configuration filename. Go kill the programmer! (Tell him to talk to Serge as well...)\n\n");
			exit(1);
		}
		if(!FileSize( defname ))
		{
			if (defname)
			{
				printf( "Warning: Cannot find configuration file %s as defined in the environment.\n", defname) ;
				printf( "         Using local configuration file %s.\n\n", name ) ;
			}
			if(!FileSize( name ))
			{
				printf( "Error: Cannot find configuration file %s.\n\n", name ) ;
				exit( 1 ) ;
			}
		}
		else
			name = defname ;

		printf("Please wait, loading drivers using %s...\n\n", name );
	}
	else
	{
		if(FileSize( defname ))
			name = defname ;
	}

	strcpy(PathConfigFile, name);

//··········································································
// midi device


	if(inits & INIT_MIDI)
	{
		printf("Initialising Midi device. Please wait...\n\n");

		midiptr = Def_ReadString( name, "MidiExec" ) ;
		if( midiptr )
			if ((*midiptr!=0) AND stricmp(midiptr, "NoExec"))
			{
				spawnl(P_WAIT, midiptr, midiptr, NULL);
				printf("\n");
			}

		ptr = Def_ReadString( name, "MidiDriver" ) ;
		if( !ptr )
		{
			printf( "Warning %s: 'MidiDriver' missing\n", name ) ;
			ptr = rien ;
		}

		if (stricmp(ptr, "NoMidi"))
			strcpy( Driver, ptr ) ;
		else
			Driver[0]=0 ;

		if( Driver[0] != 0 )	/* si driver midi */
		{

			if( !InitMidiDLL( Driver ) )	exit( 1 ) ;

			AskMidiVars( &ptridentifier, &ptrvar ) ;

			while( **ptridentifier )
			{
				if (!Def_ReadValue2( name, *ptridentifier, ptrvar++ ))
				{
					printf( "Error %s: '%s' missing\n", name, *ptridentifier ) ;
					exit( 1 ) ;
				}

				ptridentifier++ ;
			}

			if( !InitMidi() )	exit( 1 ) ;
		}

		ptr = Def_ReadString( name, "MidiType" ) ;
		if( !ptr )
		{
			printf( "Warning %s: 'MidiType' missing\n", name ) ;
			ptr = rien ;
		}

		if (*ptr)	MidiFM = !stricmp(ptr, "FM") ;
	}

//··········································································
// svga device

	if(inits & INIT_SVGA)
	{
		printf("Initialising SVGA device. Please wait...\n\n");

		if(!(inits & INIT_VESA))
		{
			ptr = Def_ReadString( name, "SvgaDriver" ) ;
			if( !ptr )
			{
				printf( "Warning %s: 'SvgaDriver' missing\n", name ) ;
				ptr = rien ;
			}

			if ((stricmp( ptr, "VESA" )) AND (*ptr) )
			{
				strcpy( Driver, ptr ) ;
				if( !SvgaInitDLL( Driver ) )	exit( 1 ) ;
			}
			else
				printf( "Built-in VESA Super VGA Driver\n\n" ) ;

		}
	}
	else if (inits & INIT_MCGA)
				printf( "Built-in MCGA Driver\n\n" ) ;

//··········································································
// mixer device

	if(inits & INIT_MIXER)
	{
		printf("Initialising Mixer device. Please wait...\n\n");

		ptr = Def_ReadString( name, "MixerDriver" ) ;
		if( !ptr )
		{
			printf( "Warning %s: 'MixerDriver' missing\n", name ) ;
			ptr = rien ;
		}

		if (stricmp(ptr, "NoMixer"))
			strcpy( Driver, ptr ) ;
		else
			Driver[0]=0 ;

		if( Driver[0] != 0 )	/* si driver wave */
		{

			if( !MixerInitDLL( Driver ) )	exit( 1 ) ;

			MixerAskVars( &ptridentifier, &ptrvar ) ;

			while( **ptridentifier )
			{
				if (!Def_ReadValue2( name, *ptridentifier, ptrvar++ ))
				{
					printf( "Error %s: '%s' missing\n", name, *ptridentifier ) ;
					exit( 1 ) ;
				}
				ptridentifier++;
			}
		}
	}

//··········································································
// wave device

	if(inits & INIT_WAVE)
	{
		printf("Initialising Wave device. Please wait...\n\n");

		ptr = Def_ReadString( name, "WaveExec" ) ;
		if( ptr )
			if ((*ptr!=0) AND stricmp(ptr, "NoExec") AND stricmp(ptr, midiptr))
			{
				spawnl(P_WAIT, ptr, ptr, NULL);
				printf("\n");
			}

		ptr = Def_ReadString( name, "WaveDriver" ) ;
		if( !ptr )
		{
			printf( "Warning %s: 'WaveDriver' missing\n", name ) ;
			ptr = rien ;
		}

		if (stricmp(ptr, "NoWave"))
			strcpy( Driver, ptr ) ;
		else
			Driver[0]=0 ;

		if( Driver[0] != 0 )	/* si driver wave */
		{

			if( !WaveInitDLL( Driver ) )	exit( 1 ) ;

			WaveAskVars( &ptridentifier, &ptrvar ) ;

			while( **ptridentifier )
			{
				if (!Def_ReadValue2( name, *ptridentifier, ptrvar++ ))
				{
					printf( "Error %s: '%s' missing\n", name, *ptridentifier ) ;
					exit( 1 ) ;
				}
				ptridentifier++;
			}

			if( !InitWave() )	exit( 1 ) ;
		}
	}

//··········································································
// system etc

	InitSystem()		;
	if (inits & INIT_SVGA)
		InitGraphSvga()		;
	else if (inits & INIT_MCGA)
		InitGraphMcga()		;

	if (VESA_Error)
	{
		printf( "Error: SVGA card BIOS does not support VESA extensions.\n") ;
		printf( "       Please refer to your SVGA card documentation for installing VESA driver.\n") ;
		exit( 1 ) ;
	}

	InitKeyboard()		;

//··········································································
// init Timer (TimerRef et NbF) selon midi installée ou non

	if( Midi_Driver_Enable )
	{
		HandleTimer = AIL_register_timer( NewProc08 ) ;
		AIL_set_timer_frequency( HandleTimer, 50 ) ;
		AIL_start_timer( HandleTimer ) ;
	}
	else
	{
		InitTimer() ;
	}

//	InitSample() ;

}

/*══════════════════════════════════════════════════════════════════════════*/

void	ClearAdelineSystem()
{
//	ClearSample() ;

	if( !Midi_Driver_Enable )	ClearTimer() ;
	ClearKeyboard() ;
	ClearGraphSvga() ;
	ClearSystem() ;
	ClearWave();
	ClearMidi();
}

/*══════════════════════════════════════════════════════════════════════════*/
