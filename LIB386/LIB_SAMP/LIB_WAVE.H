#ifdef	WATCOM9
#pragma	library	("f:\projet\lib386\lib_samp\lb9_wave.lib");
#else
#pragma	library	("f:\projet\lib386\lib_samp\lib_wave.lib");
#endif

typedef	struct
	{
		ULONG	LongHandle;
		ULONG	Info0;
		ULONG	Position;
	}
	T_WAVE;

extern	char	Wave_Driver[];

extern	char	Wave_Driver_Name[];

extern	LONG	Wave_Driver_Enable;


/*----------------------------------------------------------------------*/
/*	WaveInitDLL	: Load and connect the DLL to the program	*/
/*									*/
/*			  dlldriver : Pathname of the DLL		*/
/*									*/
/*	Returns		: TRUE if ok, FALSE if problems			*/
/*----------------------------------------------------------------------*/
LONG	WaveInitDLL( 		char *dlldriver 	) ;

/*----------------------------------------------------------------------*/
/*	WaveAskVars	: Get the list of variables needed by the DLL	*/
/*									*/
/*			  listidentifier : pointer to an array of string*/
/*					   pointers, each string	*/
/*					   contains the name of the var	*/
/*					   to initialise for the DLL	*/
/*			  ptrvars : pointer to an array of DWORD to	*/
/*				    store the value of each var in	*/
/*				    listidentifier			*/
/*									*/
/*	Returns		: TRUE if ok, FALSE if problems			*/
/*----------------------------------------------------------------------*/
void	WaveAskVars( char ***listidentifier, LONG **ptrvars ) ;

/*----------------------------------------------------------------------*/
/*	InitWave	: Init the library				*/
/*									*/
/*	Returns		: TRUE if ok, FALSE if problems			*/
/*----------------------------------------------------------------------*/
ULONG	InitWave(		void			);

/*----------------------------------------------------------------------*/
/*	ClearWave	: Close the library				*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	ClearWave(		void			);

/*----------------------------------------------------------------------*/
/*	WavePlay	: Play a Sample					*/
/*									*/
/*			  Handle : a number identifying the sample	*/
/*			  Pitchbend : 4096 is no bend			*/
/*			  Repeat : number of times to play the sample	*/
/*			  Follow : if TRUE, this sample will play after	*/
/*				   the previous one			*/
/*			  VolLeft : Volume for Left channel (128 max)	*/
/*			  VolRight : Volume for Right channel (128 max)	*/
/*			  Buffer : Pointer to the sample in mem		*/
/*									*/
/*	Returns		: the lib internal handle			*/
/*----------------------------------------------------------------------*/
ULONG	WavePlay(		UWORD Handle,
				UWORD Pitchbend,
				UWORD Repeat,
				UBYTE Follow,
				UWORD VolLeft,
				UWORD VolRight,
				void *Buffer		);

/*----------------------------------------------------------------------*/
/*	WaveGiveInfo0	: Fill the user Info0 field			*/
/*									*/
/*			  LongHandle : Lib internal handle		*/
/*			  Info0 : the info to be stored (up to you)	*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveGiveInfo0(		ULONG LongHandle,
				ULONG Info0		);

/*----------------------------------------------------------------------*/
/*	WaveStop	: Stop all samples, clear all internal vars	*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveStop(		void			);

/*----------------------------------------------------------------------*/
/*	WaveStopOne	: Stop one sample				*/
/*									*/
/*			  Handle : Your Handle 				*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveStopOne(		UWORD Handle		);

/*----------------------------------------------------------------------*/
/*	WaveStopOneLong	: Stop one sample				*/
/*									*/
/*			  LongHandle : Lib internal handle		*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveStopOneLong(	ULONG LongHandle	);

/*----------------------------------------------------------------------*/
/*	WaveInList	: Test if one sample is currently used by	*/
/*			  the lib					*/
/*									*/
/*			  Handle : Your Handle 				*/
/*									*/
/*	Returns		: TRUE if used					*/
/*----------------------------------------------------------------------*/
int	WaveInList(		UWORD handle		);

/*----------------------------------------------------------------------*/
/*	WaveGetSnap	: Take a snapshot of all Internal handles and	*/
/*			  Info0	for currently playing samples in array 	*/
/*			  ListSnapSample				*/
/*									*/
/*			  Buffer : A pointer that will be filled with	*/
/*				   the address of the buffer		*/
/*									*/
/*	Returns		: The number of samples currently used by the 	*/
/*			  lib						*/
/*----------------------------------------------------------------------*/
int	WaveGetSnap(		void **Buffer		);

/*----------------------------------------------------------------------*/
/*	WavePause	: Pause all the playing samples			*/
/*									*/
/*	Returns		: TRUE if paused					*/
/*----------------------------------------------------------------------*/
int	WavePause(		void			);

/*----------------------------------------------------------------------*/
/*	WaveContinue	: Continue all the playing samples		*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveContinue(		void			);

/*----------------------------------------------------------------------*/
/*	WaveSaveState	: Save the internal state of the lib and	*/
/*			  get ready to play somme other samples		*/
/*									*/
/*	Remark		: Can be called only once			*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveSaveState(		void			);

/*----------------------------------------------------------------------*/
/*	WaveRestoreState: Restore the internal state of the lib		*/
/*			  and resume playback				*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveRestoreState(	void			);

/*----------------------------------------------------------------------*/
/*	WaveChangeVolume: Change the volume of a sample			*/
/*									*/
/*			  LongHandle : Lib internal handle		*/
/*			  VolLeft : New Left volume (128 max)		*/
/*			  VolRight : New Right volume (128 max)		*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveChangeVolume(	ULONG longhandle,
				ULONG VolGauche,
				ULONG VolDroit		);

/*----------------------------------------------------------------------*/
/*	WaveMove	: Tell the driver to move the samples from 	*/
/*			  SrcAddr to DestAddr. Size given by Size	*/
/*									*/
/*			  DestAddr : Dest				*/
/*			  SrcAddr  : Source				*/
/*			  Size     : number of bytes to move		*/
/*									*/
/*	Returns		: nothing					*/
/*----------------------------------------------------------------------*/
void	WaveMove(		void  *DestAddr,
				void  *SrcAddr,
				ULONG Size		);

/*----------------------------------------------------------------------*/
/*	WaveGetAddr	: Return a pointer to the current playing	*/
/*			  sample data					*/
/*									*/
/*	Returns		: a pointer (FLAT)				*/
/*----------------------------------------------------------------------*/
void	*WaveGetAddr(		void			);
