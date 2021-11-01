//████████████████████████████████████████████████████████████████████████████
//██                                                                        ██
//██  Project: IBM Audio Interface Library for 32-bit DPMI (AIL/32)         ██
//██   Author: John Miles                                                   ██
//██                                                                        ██
//████████████████████████████████████████████████████████████████████████████
//██                                                                        ██
//██  Gestionnaire Midi for Adeline Soft le 2/4/94                          ██
//██                                                                        ██
//████████████████████████████████████████████████████████████████████████████

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>

#include "f:\projet\lib386\lib_sys\adeline.h"
#include "f:\projet\lib386\lib_sys\lib_sys.h"
#include "f:\projet\lib386\lib_midi\lib_midi.h"

/***************************************************************/

void	NewProc08() ;

WORD	Midi_Driver_Enable = FALSE ;

char	*DriverError = "Error MidiDriver:" ;

char	*ListIdentifier[] = {	"MidiBase" ,
				"MidiIRQ" ,
				"MidiDMA" ,
				"" };

LONG	Midi_Base, Midi_IRQ, Midi_DMA ;


HDRIVER hdriver ;
HSEQUENCE hseq = -1 ;
drvr_desc *desc;

//FILE *GTL;
char *ptrGTL;
char MidiPath[_MAX_PATH];

char *state;
char *drvr,*dll;
char *timb;
char *tc_addr;

unsigned char *ail_buffer;

unsigned long state_size;
unsigned short ail_bank,patch,tc_size,seqnum;
unsigned short treq;

WORD	MidiLoop = FALSE ;

LONG	MaxVolume = 100 ;

//████████████████████████████████████████████████████████████████████████████

void	AskMidiVars( char ***listidentifier, LONG **ptrvars )
{
	*listidentifier = ListIdentifier ;
	*ptrvars = &Midi_Base ;
}

//████████████████████████████████████████████████████████████████████████████
//
// Standard C routine for Global Timbre Library access
//

#ifdef	ytuyjbd

void *load_global_timbre(FILE *GTL, unsigned short bank, unsigned short patch)
{
   unsigned short *timb_ptr;
   static unsigned short len;

   static struct                  // GTL file header entry structure
   {
      signed char patch;
      signed char bank;
      unsigned long offset;
   }
   GTL_hdr;


   if (GTL==NULL) return NULL;    // if no GTL, return failure

   rewind(GTL);                   // else rewind to GTL header

   do                             // search file for requested timbre
      {
      fread(&GTL_hdr,sizeof(GTL_hdr),1,GTL);

      if (GTL_hdr.bank == -1)
	 return NULL;             // timbre not found, return NULL
      }
   while ((GTL_hdr.bank != bank) ||
	  (GTL_hdr.patch != patch));

   fseek(GTL,GTL_hdr.offset,SEEK_SET);
   fread(&len,2,1,GTL);           // timbre found, read its length

   timb_ptr = malloc(len);        // allocate memory for timbre ..
   *timb_ptr = len;
				  // and load it
   fread((timb_ptr+1),len-2,1,GTL);

   if (ferror(GTL))               // return NULL if any errors
   {
      free(timb_ptr);
      return NULL;                // occurred
   }
   else
      return timb_ptr;            // else return pointer to timbre
}

#endif

void *load_global_timbre(unsigned short bank, unsigned short patch)
{
   static struct GTL_h             // GTL file header entry structure
   {
      signed char patch;
      signed char bank;
      unsigned long offset;
   }
   *GTL_hdr;


   GTL_hdr = (struct GTL_h *)ptrGTL ;// to GTL header

				  // search file for requested timbre
   while ((GTL_hdr->bank != bank) ||
	  (GTL_hdr->patch != patch))
      {
	 if (GTL_hdr->bank == -1)
	     return NULL;         // timbre not found, return NULL

	 GTL_hdr++;
      }

   return((void *)(ptrGTL + GTL_hdr->offset)) ;
}

//████████████████████████████████████████████████████████████████████████████

LONG	InitMidiDLL( char *driverpathname )
{
	char	*str;

   //
   // Load driver file
   //

	dll = FILE_read( driverpathname, NULL);
	if (dll==NULL)
	{
		printf("%s Could not load driver '%s'.\n", DriverError, driverpathname );
		return FALSE ;
	}

	drvr=DLL_load(dll,DLLMEM_ALLOC | DLLSRC_MEM,NULL);
	if (drvr==NULL)
	{
		printf("%s Invalid DLL image.\n", DriverError );
		return FALSE ;
	}

	Free(dll);

   //
   // Initialize API before calling any Library functions
   //

	AIL_startup();

   //
   // Register the driver with the API
   //

	hdriver = AIL_register_driver(drvr);
	if (hdriver==-1)
	{
		printf("%s Driver %s not compatible with linked API version.\n",
			DriverError, driverpathname );
		AIL_shutdown(NULL);
		return FALSE ;
	}

   //
   // Get driver type and factory default I/O parameters; exit if
   // driver is not capable of interpreting MIDI files
   //

	desc = AIL_describe_driver(hdriver);

	if (desc->drvr_type != XMIDI_DRVR)
	{
		printf("%s Driver %s not an XMIDI driver.\n", DriverError, driverpathname );
		AIL_shutdown(NULL);
		return FALSE ;
	}


   //
   // Print Driver name and copyright notice
   //

	printf("AIL/32 midi driver for:\n");

	str = desc->dev_name_table ;
	while(*str != 0)
	{
		printf("%s\n", str);
		while(*str != 0)
			str++;
		str++;
	}

	printf("\nCopyright (C) 1991,1992 Miles Design, Inc.\n\n");

	return TRUE ;
}

//████████████████████████████████████████████████████████████████████████████

LONG	InitMidi()
{
	char	GTL_filename[_MAX_PATH];

   // use if defined new parameters

	if( Midi_Base != -1 )	desc->default_IO  = Midi_Base ;
	if( Midi_IRQ  != -1 )	desc->default_IRQ = Midi_IRQ ;
	if( Midi_DMA  != -1 )
	{
		desc->default_DMA = Midi_DMA ;
		desc->default_DRQ = Midi_DMA ;
	}

   //
   // Verify presence of driver's sound hardware and prepare
   // driver/hardware for use
   //
	if (!AIL_detect_device(hdriver,desc->default_IO,desc->default_IRQ,
		desc->default_DMA,desc->default_DRQ))
	{
		AIL_shutdown(NULL);
		printf( "%s Sound Hardware not found.\n", DriverError ) ;
		return FALSE ;
	}


	Midi_Driver_Enable = TRUE ;

	AIL_init_driver(hdriver,desc->default_IO,desc->default_IRQ,
				desc->default_DMA,desc->default_DRQ);

	state_size = AIL_state_table_size(hdriver);
   //
   // Set up local timbre cache; open Global Timbre Library file
   //

	tc_size = AIL_default_timbre_cache_size(hdriver);

	if (tc_size)
	{
		tc_addr = malloc((unsigned long) tc_size);
		AIL_define_timbre_cache(hdriver,tc_addr,tc_size);
	}

	state = malloc(state_size);

   //
   // Get name of Global Timbre Library file by appending suffix
   // supplied by XMIDI driver to GTL filename prefix "SAMPLE."
   //
	strcpy(GTL_filename, MidiPath ) ;
	strcat(GTL_filename,"SAMPLE.");
	strcat(GTL_filename,desc->data_suffix);

   // Load it into memory

	if(!(ptrGTL = DosMalloc(FileSize(GTL_filename), NULL)))
		return FALSE ;

	Load( GTL_filename, ptrGTL );

	return TRUE ;
}

//████████████████████████████████████████████████████████████████████████████

void	InitPathMidiSampleFile( UBYTE *path )
{
//
// Get name of Global Timbre Library file by appending suffix
// supplied by XMIDI driver to GTL filename prefix "SAMPLE."
//
	strcpy(MidiPath, path ) ;
}

//████████████████████████████████████████████████████████████████████████████

void	ClearMidi()
{
	if( !Midi_Driver_Enable ) 	return ;

	AIL_shutdown( "" );
	hseq = -1 ;
}

//████████████████████████████████████████████████████████████████████████████

void	PlayMidi( /*char *filename*/ UBYTE *ail_buffer )
{
	if( !Midi_Driver_Enable ) 	return ;

   //
   // Look up and register desired sequence in XMIDI file, loading
   // timbres if needed
   //
	if( hseq != -1 )
	{
		AIL_stop_sequence(hdriver,hseq) ;
		AIL_release_sequence_handle( hdriver, hseq ) ;
	}

	seqnum = 0 ;

	if ((hseq = AIL_register_sequence(	hdriver,
						ail_buffer,
						seqnum,
						state,
						NULL )
		) == -1)
	{
//		printf("Sequence %u not present in XMIDI file \"%s\".\n",seqnum,argv[1]);
		return ;
	}

//	GTL = NULL ;

	while ((treq=AIL_timbre_request(hdriver,hseq)) != 0xffff)
	{
//		if( GTL == NULL )
//		{
//			GTL = fopen(GTL_filename,"rb");	// sample.ad/.opl
//		}

		ail_bank = treq / 256; patch = treq % 256;

//		timb = load_global_timbre(GTL,ail_bank,patch);
		timb = load_global_timbre(ail_bank,patch);

		if (timb != NULL)
		{
//			printf("Installed timbre bank %u, patch %u\n",bank,patch);
			AIL_install_timbre(hdriver,ail_bank,patch,timb);
//			free(timb);
		}
		else
		{
//			printf("Timbre bank %u, patch %u not found ",bank,patch);
//			if (GTL != NULL) fclose(GTL);
			return ;
		}
//			AIL_install_timbre(hdriver,ail_bank,patch,timb);
	}

   //
   // Start music playback
   //

/*   printf("Playing sequence %u from XMIDI file \"%s\" ...\n\n",
      seqnum,argv[1]);	*/

	AIL_start_sequence(hdriver,hseq) ;
}

//████████████████████████████████████████████████████████████████████████████

void	StopMidi()
{
	if( hseq != -1 )
	{
		AIL_stop_sequence(hdriver,hseq) ;
		AIL_release_sequence_handle( hdriver, hseq ) ;
		hseq = -1 ;
	}
}

//████████████████████████████████████████████████████████████████████████████

LONG	IsMidiPlaying()
{
	if( hseq != -1 )
	{
		if( AIL_sequence_status( hdriver, hseq ) == 1 )
			return TRUE ;
	}
	return FALSE ;
}

//████████████████████████████████████████████████████████████████████████████

void	FadeMidiDown( WORD nbsec )
{
	if( hseq != -1 )
	{
		AIL_set_relative_volume(hdriver, 0, 0, 1000 * nbsec ) ;
	}
}

//████████████████████████████████████████████████████████████████████████████

void	FadeMidiUp( WORD nbsec )
{
	if( hseq != -1 )
	{
		AIL_set_relative_volume(hdriver, 0, MaxVolume, 1000 * nbsec ) ;
	}
}
//████████████████████████████████████████████████████████████████████████████

void	WaitFadeMidi()
{
	if( hseq != -1 )
	{
		while( AIL_relative_volume( hdriver, 0 ) != 0 ) ;
	}
}

//████████████████████████████████████████████████████████████████████████████

void	VolumeMidi( WORD volume )
{
	if( hseq != -1 )
	{
		AIL_set_relative_volume(hdriver, 0, (volume*MaxVolume)/100, 0 ) ;
	}
}

//████████████████████████████████████████████████████████████████████████████

void	DoLoopMidi()
{
	// loop track

	if( hseq != -1 )
		if( AIL_sequence_status( hdriver, 0 ) == 2 ) // seg done
			AIL_start_sequence(hdriver,hseq) ;
}

//████████████████████████████████████████████████████████████████████████████
