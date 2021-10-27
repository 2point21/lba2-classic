#ifdef WATCOM9
#pragma	library	("f:\projet\lib386\lib_mix\lb9_mix.lib");
#else
#pragma	library	("f:\projet\lib386\lib_mix\lib_mix.lib");
#endif

extern	LONG	Mixer_Driver_Enable;

/*----------------------------------------------------------------------*/
/*	MixerAskVars	: Get list of vars for the lib			*/
/*									*/
/*			  listidentifier : pointer to an array of string*/
/*					   pointers, each string	*/
/*					   contains the name of the var	*/
/*					   to initialise for the DLL	*/
/*			  ptrvars : pointer to an array of DWORD to	*/
/*				    store the value of each var in	*/
/*				    listidentifier			*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	MixerAskVars( 		char ***listidentifier,
				LONG **ptrvars 		);


/*----------------------------------------------------------------------*/
/*	MixerChangeVolume : Change the hardware volume for MIDI,	*/
/*			    WAVE, CD, Line, Master			*/
/*									*/
/*			    VolWave   : Volume for WAVE   (0-255)	*/
/*			    VolMidi   : Volume for MIDI   (0-255)	*/
/*			    VolCD     : Volume for CD     (0-255)	*/
/*			    VolLine   : Volume for LINE   (0-255)	*/
/*			    VolMaster : Volume for MASTER (0-255)	*/
/*									*/
/*			    NOTE      : A VOLUME OF -1 MEANS NO CHANGE	*/
/*									*/
/*	Returns		  : nothing					*/
/*----------------------------------------------------------------------*/
void	MixerChangeVolume(	LONG VolWave,
				LONG VolMidi,
				LONG VolCD,
				LONG VolLine,
				LONG VolMaster		);


/*----------------------------------------------------------------------*/
/*	MixerGetVolume : Get the hardware volume for MIDI, WAVE,	*/
/*			 CD, Line and Master				*/
/*									*/
/*			 VolWave   : ptr on Volume for WAVE   (0-255)	*/
/*			 VolMidi   : ptr on Volume for MIDI   (0-255)	*/
/*			 VolCD     : ptr on Volume for CD     (0-255)	*/
/*		         VolLine   : ptr on Volume for LINE   (0-255)	*/
/*		         VolMaster : ptr on Volume for MASTER (0-255)	*/
/*									*/
/*			 NOTE      : A NULL PTR MEANS NO GET		*/
/*									*/
/*	Returns	       : nothing					*/
/*----------------------------------------------------------------------*/
void	MixerGetVolume(		LONG *VolWave,
				LONG *VolMidi,
				LONG *VolCD,
				LONG *VolLine,
				LONG *VolMaster		);


/*----------------------------------------------------------------------*/
/*	MixerGetInfo   : Get the hardware volume capabilities		*/
/*									*/
/*			 VolWave   : ptr on Volume for WAVE   (0/1)	*/
/*			 VolMidi   : ptr on Volume for MIDI   (0/1)	*/
/*			 VolCD     : ptr on Volume for CD     (0/1)	*/
/*		         VolLine   : ptr on Volume for LINE   (0/1)	*/
/*		         VolMaster : ptr on Volume for MASTER (0/1)	*/
/*									*/
/*			 NOTE      : A NULL PTR MEANS NO GET		*/
/*									*/
/*	Returns	       : nothing					*/
/*----------------------------------------------------------------------*/
void	MixerGetInfo(		LONG *VolWave,
				LONG *VolMidi,
				LONG *VolCD,
				LONG *VolLine,
				LONG *VolMaster		);
