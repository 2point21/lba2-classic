// **************************************************************
// *                     P L A Y F L A . C			*
// * 			 (c) Adeline 1994			*
// **************************************************************
#include	"c_extern.h"

typedef	struct	{	ULONG	SizeFile ;
			ULONG	CompressedSizeFile ;
			WORD	CompressMethod ;	/* 0 stored */
							/* 1 LZS */
		}	T_HEADER ;

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		   ÛßßßÛ Û     ÛßßßÛ Û  Üß Ûßßßß Û     ÛßßßÛ
		   ÛÛßßß ÛÛ    ÛÛßßÛ ÛÛß   ÛÛßß  ÛÛ    ÛÛßßÛ
		   ßß    ßßßßß ßß  ß ßß    ßß    ßßßßß ßß  ß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#define FLI 2
#define FLA 3
#define X_POS_MENU_PRINCIPAL 480
#define Y_POS_MENU_PRINCIPAL 10
#define X_POS_MENU_STORYBOARD 0
#define Y_POS_MENU_STORYBOARD 335

// Define pour les fichiers format FLI
#define	FLI_COLOR	11
#define	FLI_LC		12
#define	FLI_BLACK	13
#define	FLI_BRUN	15
#define	FLI_COPY	16

// Structures utilis‚es pour les fichiers format FLI
typedef struct
	{
	LONG	size		;
	UWORD 	type		;/* = FLIH_MAGIC */
	UWORD 	frame_count	;
	UWORD 	width		;
	UWORD 	height		;
	UWORD 	bits_a_pixel	;
	WORD 	flags		;
	WORD 	speed		;
	LONG 	next_head	;
	LONG 	frames_in_table	;
	WORD 	file		;
	LONG	frame1_off	;
	LONG 	strokes		;
	LONG 	session		;
	BYTE 	reserved[88]	;
	} T_HEADER_FLI		;

typedef struct
	{
	ULONG	size		;
	UWORD	typedata	;
	UBYTE	image[]		;
	} T_HEADER_IMAGE	;

typedef struct
	{
	ULONG	offset		;
	UWORD	signature	;
	UWORD	nbpasse		;
	UBYTE   reserved[6+2]	;
	} T_HEADER_SUB		;

// Define pour les fichiers format FLA
#define VERSION "V1.3"
#define	FLA_PALETTE		1
#define	FLA_INFO	 	2
#define	FLA_SAMPLE 		3
#define	FLA_SAMPLE_BALANCE 	4
#define	FLA_SAMPLE_STOP 	5
#define	FLA_LC			6
#define	FLA_BLACK		7
#define	FLA_BRUN		8
#define	FLA_COPY		9

// Structures utilis‚es pour les fichiers format FLA
typedef struct                  // entete de fichier FLA
	{
		char	Version[5]	;
		ULONG   NbFrame		;
		UBYTE	CadenceAnimation;
		UWORD	ResolutionX	;
		UWORD	ResolutionY	;
	} T_HEADER_FLA;

typedef struct			// header de passe
	{
		UBYTE	NbPasse		;
		ULONG	OffsetFrameSuivante	;
	} T_HEADER_FLA_PASSE;

typedef struct			// structure d'un type de passe
	{
		UBYTE	Type;
		UWORD	OffsetPasseSuivante;
	} T_FLA_TYPE;

typedef struct			// structure d'un element d'une liste de son
	{
		WORD	NbSample;
		WORD	OffsetFrameOne;
	} T_FLA_SAMPLE_LIST;

typedef struct			// structure d'une info
	{
		WORD	Info	;
	} T_FLA_INFO;

typedef struct			// structure d'un son
	{
		WORD	Numero	;
		WORD	Decalage;
		WORD	Repetition;
		UBYTE	Balance;
		UBYTE	VolumeG;
		UBYTE	VolumeD;
	} T_FLA_SAMPLE;

typedef struct			// structure d'un son
	{
		WORD	Numero	;
		UBYTE	Offset	;
		WORD	Balance	;
		UBYTE	VolumeG	;
		UBYTE	VolumeD	;
	} T_FLA_BALANCE;

typedef struct			// structure d'arret de son
	{
		UWORD	Numero	;
	} T_FLA_SAMPLE_STOP;

typedef struct			// structure d'une palette
	{
		UWORD	NbCouleur;
		UWORD	CouleurDepart;
	} T_FLA_PALETTE;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

UBYTE	*BufferFrame	;

//UBYTE	TypeAnimCharge	= FALSE ;
UBYTE	PaletteOrg[768]	;

T_HEADER_FLA			HeaderFla	;
T_FLA_SAMPLE_LIST		HeaderSampleListFla;
T_HEADER_FLA_PASSE		HeaderPasseFla	;
T_FLA_SAMPLE			HeaderSampleFlaNew;
T_FLA_BALANCE			HeaderBalanceFlaNew;
T_FLA_SAMPLE_STOP		HeaderSampleStopFlaNew;
T_FLA_INFO			HeaderInfo ;

LONG	HandleFla	;
LONG	DeltaX, DeltaY	;
LONG	MaxPass		;
LONG	MaxFrame	;
ULONG	NbTip		;

WORD	CouleurDepart	;
WORD	NombreDeCouleur	;
ULONG	ImageCadence = 12	;
//WORD	Frame =0		;
//WORD	ImageCourante = 0	;
//WORD	ImageDebut = 0		;
//WORD	ImageFin = 0		;

WORD	FlagFirst = TRUE ;

// *************************************************************************

// **************************************************************************
void ChangeBalance( WORD numsample, UBYTE offset, UBYTE volg, UBYTE vold )
{
	WORD i;
	BYTE flagtrouve = FALSE;

#ifndef	SAMPLE_FLA_HQR
	i = NbSampleJoue;

	while ( (i > 0) AND ( flagtrouve == FALSE) )
	{
		if ( (PtrSampleJoue[i] & 0x0000ffff) == numsample )
		{
			// Message( Itoa(PtrSampleJoue[i]), FALSE );
			// Message( Itoa((LONG)volg), TRUE );
			// Message( Itoa((LONG)vold), TRUE );
			WaveChangeVolume( PtrSampleJoue[i], (ULONG)volg, (ULONG)vold );
			flagtrouve = TRUE;
		}
		i--;
	}
#endif
}

/*-------------------------------------------------------------------------*/
void	LoadNextFrameFla()
{
	Read( HandleFla, &HeaderPasseFla, sizeof( T_HEADER_FLA_PASSE))	;

	MaxPass = HeaderPasseFla.NbPasse			;/* pti->nbpasse*/

	// Texte pour le debug
/*	Box(10,420,500,450,0);
	AffString (10,420,"Structure entete de passe:");
	Text (10,430,"header_passe.NbPasse=%L", HeaderPasseFla.NbPasse);
	Text (10,440,"header_passe.OffsetFrameSuivante=%L", HeaderPasseFla.OffsetFrameSuivante);
	CopyBlockPhys(10,420,500,450); */

	Read( HandleFla, BufferFrame, HeaderPasseFla.OffsetFrameSuivante);
}

/*-------------------------------------------------------------------------*/
void	DrawNextFrameFla()
{
	T_FLA_TYPE	header_type;
	T_FLA_PALETTE	header_palette;

	UBYTE	*ptr_buffer, *ptr_donnee;
	LONG	i	;
	LONG	compteur	;

	LoadNextFrameFla()	;

	ptr_buffer = BufferFrame;

	for ( i = 0 ; i < MaxPass ; i++ )
	{
		header_type = *(T_FLA_TYPE *)ptr_buffer;

		ptr_buffer += sizeof(T_FLA_TYPE)	;/* Jump header type */
		ptr_donnee = ptr_buffer			;

		switch( header_type.Type )
		{
			case FLA_PALETTE:
				header_palette = *(T_FLA_PALETTE *)ptr_donnee;
				ptr_donnee += sizeof(T_FLA_PALETTE)	;/* Jump header type */
				MovMem( ptr_donnee, PaletteOrg + header_palette.CouleurDepart * 3, header_palette.NbCouleur * 3 );
				CouleurDepart = header_palette.CouleurDepart;
				NombreDeCouleur = header_palette.NbCouleur;
				break;

			case FLA_SAMPLE:
				HeaderSampleFlaNew = *(T_FLA_SAMPLE *)ptr_donnee;
				ptr_donnee += sizeof(T_FLA_SAMPLE)	;/* Jump header type */
				if( SamplesEnable )
				{
					HQ_MixSample( HeaderSampleFlaNew.Numero,
						      HeaderSampleFlaNew.Decalage,
						      HeaderSampleFlaNew.Repetition,
						      HeaderSampleFlaNew.VolumeG,
						      HeaderSampleFlaNew.VolumeD );
				}
				break;

			case FLA_SAMPLE_BALANCE:
				HeaderBalanceFlaNew = *(T_FLA_BALANCE *)ptr_donnee;
				ptr_donnee += sizeof(T_FLA_BALANCE)	;/* Jump header type */
				if( SamplesEnable )
				{
					ChangeBalance( HeaderBalanceFlaNew.Numero,
						       HeaderBalanceFlaNew.Offset,
						       HeaderBalanceFlaNew.VolumeG,
						       HeaderBalanceFlaNew.VolumeD );
				}
				break;

			case FLA_SAMPLE_STOP:
				HeaderSampleStopFlaNew = *(T_FLA_SAMPLE_STOP *)ptr_donnee;
				ptr_donnee += sizeof(T_FLA_SAMPLE_STOP)	;/* Jump header type */
				if ( SamplesEnable == TRUE )
				{
					if ( HeaderSampleStopFlaNew.Numero == -1 )
					{
						HQ_StopSample();
					}
					else
					{
						HQ_StopOneSample ( HeaderSampleStopFlaNew.Numero );
					}
				}
				break;

			case FLA_INFO:
				HeaderInfo = *(T_FLA_INFO *)ptr_donnee;
				ptr_donnee += sizeof(T_FLA_INFO)	;/* Jump header type */
				switch( HeaderInfo.Info )
				{
					case 1:	// fla flute
						if( Midi_Driver_Enable )
						{
							if( (26 != NumXmi)
							OR  (!IsMidiPlaying()) )
							{
								StopMusicMidi() ;
								PtrXmi = HQR_Get( HQR_Midi, 26 ) ;
								NumXmi = 26 ;
								PlayMidi( PtrXmi ) ;
								VolumeMidi( 100 ) ;
							}
						}
						break ;

					case 2:
						FadeToBlack( PaletteOrg ) ;
						break ;

					case 3:
						FlagFirst = TRUE ;
						break ;

					case 4:
						FadeMusicMidi( 1 ) ;
						break ;
				}
				break;


			case FLA_BLACK:
				BlackFrame()			;
				break				;
			case FLA_COPY:
				CopyFrame( ptr_donnee);
				break				;
			case FLA_BRUN:
				DrawFrame( ptr_donnee, 320, DeltaY )	;
				break				;
			case FLA_LC:
				UpdateFrame( ptr_donnee, 320 )	;
				break				;
			case FLI_COPY:
				CopyFrame( ptr_donnee );
				break				;
			default:
				break				;
		}

		ptr_buffer += header_type.OffsetPasseSuivante	;

	} // fin boucle de passe
}

/*-------------------------------------------------------------------------*/
/*
void	SeekFla()
{
	T_FLA_SAMPLE_LIST header_sample_list;

	Seek( HandleFla, sizeof(T_HEADER_FLA), SEEK_START )	;
	Read( HandleFla, &HeaderSampleListFla, sizeof(T_FLA_SAMPLE_LIST));
	Seek( HandleFla, HeaderSampleListFla.OffsetFrameOne, SEEK_CURRENT );
}
*/
/*-------------------------------------------------------------------------*/
WORD	InitFla( char *name )
{
	char	chaine[128]	;
	UBYTE	*ptr_list_sample	;
	WORD	i		;
	WORD	numero_sample	;
	WORD	nb_fois_joue	;
	LONG	size		;

	BufferFrame = Screen ;			// Malloc( 80000L ) For One Frame(image, couleur...)

	HandleFla = OpenRead( name )	;

#ifdef	DEBUG_TOOLS
	if ( !HandleFla ) return(FALSE)	;	// protect CD ?
#endif
#ifdef	DEMO
	if ( !HandleFla ) return(FALSE)	;	// protect CD ?
#endif

	if( SamplesEnable )
	{
		UBYTE	string[256] ;

		strcpy( string, PathFla ) ;
		strcat( string, "FLASAMP.HQR" ) ;

		HQR_Change_Ressource( HQR_Samples, string ) ;
	}

	// lecture de l'entˆte principale
	Read( HandleFla, &HeaderFla, sizeof(T_HEADER_FLA))	;

	MaxFrame   =	HeaderFla.NbFrame;
	DeltaX     = 	HeaderFla.ResolutionX	;
	DeltaY     = 	HeaderFla.ResolutionY	;
	ImageCadence =	HeaderFla.CadenceAnimation;

	// lecture de la liste des samples
	Read( HandleFla, &HeaderSampleListFla, sizeof(T_FLA_SAMPLE_LIST));

	for ( i = 0; i < HeaderSampleListFla.NbSample; i++ )
	{
		Read( HandleFla, &numero_sample, sizeof(WORD));
		Read( HandleFla, &nb_fois_joue, sizeof(WORD));

		// preload samples (if enough space)
		// (but if not, stupid method. need to write the
		//  list reversed)
		if( SamplesEnable )
		{
			HQR_GetSample( HQR_Samples, numero_sample ) ;
		}
	}

	// texte de debug
/*	AffString (10,330,"Structure entete de fichier FLA:");
	Text( 10,340,"HeaderFla.Version=%S", HeaderFla.Version );
	Text( 10,350,"HeaderFla.NbFrame=%L", HeaderFla.NbFrame );
	Text( 10,360,"HeaderFla.CadenceAnimation=%L", HeaderFla.CadenceAnimation );
	Text( 10,370,"HeaderFla.ResolutionX=%L", HeaderFla.ResolutionX );
	Text( 10,380,"HeaderFla.ResolutionY=%L", HeaderFla.ResolutionY );
	Flip(); */

	return TRUE ;
}

/*-------------------------------------------------------------------------*/
void	ClearFla()
{
	if( SamplesEnable )
	{
		HQR_Change_Ressource( HQR_Samples, PATH_RESSOURCE"samples.hqr" ) ;
	}

	Close( HandleFla )	;
}

/*--------------------- GESTION PALETTE PENDANT ANIM ----------------------*/
void GestionPalette()
{
	WORD compteur;

	if( FlagFirst  )
	{
		FadeToPal( PaletteOrg ) ;
		NombreDeCouleur = 0 ;
                FlagFirst = FALSE ;
	}

	if ( NombreDeCouleur != 0 )
	{
		if ( NombreDeCouleur == 256 )
		{
			Palette( PaletteOrg ) ;
		}
		else
		{
/*			for ( compteur=0; compteur<NombreDeCouleur; compteur++ )
			{
				PalOne( CouleurDepart+compteur, *(UBYTE *)PaletteOrg+(CouleurDepart+compteur)*3+0,
								*(UBYTE *)PaletteOrg+(CouleurDepart+compteur)*3+1,
								*(UBYTE *)PaletteOrg+(CouleurDepart+compteur)*3+2 );
			}//	LORAN LE 28/09/94
*/
			PalMulti( CouleurDepart, NombreDeCouleur, PaletteOrg+(CouleurDepart*3) )	;
		}
		NombreDeCouleur = 0;
	}
}

//***************************************************************************

void PlayAnimFla(char *name_anim)
{
	WORD	i ;
	ULONG	cadence;
	UBYTE	sortie = FALSE;
	UBYTE	ficname[_MAX_PATH] ;

	if( !FlaFromCD )
	{
		PlayDiskFla( name_anim ) ;
		return ;
	}

#ifdef	CDROM
	StopMusicCD() ;
#endif

	strcpy( ficname, PathFla ) ;
	strcat( ficname, name_anim ) ;
	AddExt( ficname, ".FLA" ) ;

	if( !InitFla( ficname ) )	return ;

	if ( strcmp( HeaderFla.Version, VERSION ) == 0 )
	{
		ExtInitMcga();
		SetBlackPal();
		Mcga_Cls();
		Mcga_Flip();
		Mcga_Cls();

		i=0;
		FlagFirst = TRUE ;
		while (sortie == FALSE )
		{
			if( Key == K_ESC ) sortie = TRUE;
			cadence = TimerRef;

			DrawNextFrameFla();

			Mcga_Flip();
			GestionPalette();
			do
			{
				NbTip = TimerRef - cadence;
			}
			while (NbTip < 50 / ImageCadence);

			i++;
			if( i == MaxFrame )	sortie = TRUE ;
		}

		FadeToBlack( PaletteOrg ) ;
		Mcga_Cls() ;
		Mcga_Flip() ;
		HQ_StopSample();
		ClearFla();

		ExtInitSvga() ;
		SetBlackPal() ;
		Cls() ;
	}
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		ÛßßßÜ  Û    ÛÛßßß Û  Üß       Ûßßßß Û     ÛßßßÛ
		ÛÛ  Û  ÛÛ   ßßßßÛ ÛÛßÜ        ÛÛßß  ÛÛ    ÛÛßßÛ
		ßßßß   ßß   ßßßßß ßß  ß ßßßßß ßß    ßßßßß ßß  ß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	CutPoint( UBYTE *ptr )
{
	UBYTE	c ;

	do
	{
		c = *ptr ;
		if( c == '.' )
		{
			*ptr = 0 ;
			return ;
		}
		ptr++ ;
	}
	while( c!=0 ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

WORD	SearchFla( UBYTE *name_anim, UBYTE *ptrtxt, LONG maxsize, UBYTE *ptrlist )
{
	UBYTE	string[256] ;
	LONG	n = 0 ;
	LONG	nb = 0 ;
	UBYTE	*ptrs, *ptrd ;

	do
	{
		ptrd = string ;
		while( (*ptrd++ = *ptrtxt++) > 32 ) n++ ;
		ptrd[-1] = 0 ;
		n++ ;

		if( !stricmp( name_anim, string ) )
		{
			do
			{
				*ptrlist++ = atoi( ptrtxt ) ;
				nb++ ;
				while( *ptrtxt++ > 32 )	n++ ;
				n++ ;
			}
			while( *ptrtxt > 32 ) ;
			return nb ;
		}
		while( *ptrtxt++ >= 32 ) n++ ;
		n++ ;
		while( *ptrtxt++ < 32 ) n++ ;
		n++ ;
		ptrtxt-- ;
	}
	while( n < maxsize ) ;

	return 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	PlayDiskFla( UBYTE *name_anim )
{
	UBYTE	*ptrtxt ;
	UBYTE	string[256] ;
	LONG	size,n, nb, timer ;
	UBYTE	listindex[20] ;

	size = HQRM_Load( PATH_RESSOURCE"ress.hqr", RESS_FLA_PCX, &ptrtxt ) ;
	CHECK_MEMORY
	if( !ptrtxt )
	{
		Message( "Not Enough Memory for fla_gif list", TRUE ) ;
		return ;
	}
	strcpy( string, name_anim ) ;
	CutPoint( string ) ;

	nb = SearchFla( string, ptrtxt, size, listindex ) ;

	if( !nb ) return ;

//	FadeToBlack( PtrPal ) ;

	for( n=0; n<nb; n++ )
	{
		switch( listindex[n] )
		{
		case 200:
			Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_BUMPER_PCR ) ;
			CopyScreen( Screen, Log ) ;
			Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_BUMPER_PAL ) ;
			break ;

		case 201:
			Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_TWINSUN_PCR ) ;
			CopyScreen( Screen, Log ) ;
			Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_TWINSUN_PAL ) ;
			break ;

		case 202:
			Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_INTRO_2_PCR ) ;
			CopyScreen( Screen, Log ) ;
			Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_INTRO_2_PAL ) ;
			break ;

		default:
/*                      // methode avec PCX raw format
			Load_HQR( PATH_RESSOURCE"fla_pcx.hqr", Screen, listindex[n]+1 ) ;
			CopyScreen( Screen, Log ) ;
			Load_HQR( PATH_RESSOURCE"fla_pcx.hqr", PalettePcx, listindex[n] ) ;
*/
			{
				LONG		handle ;
				UWORD		nbbloc ;
				ULONG		buffer ;
				ULONG		seekindex ;
				T_HEADER	header ;

				// entete hqr
				handle = OpenRead( PATH_RESSOURCE"fla_gif.hqr" ) ;
				if( !handle )	break ;
				Read( handle, &buffer, 4L ) ;
				nbbloc = (UWORD)(buffer / 4L) ;
				if( listindex[n] >= nbbloc )
				{
					Close( handle ) ;
					break ;
				}
				Seek( handle, listindex[n] * 4L, SEEK_START ) ;
				Read( handle, &seekindex, 4L ) ;
				Seek( handle, seekindex, SEEK_START ) ;
				Read( handle, &header, sizeof( header ) ) ;
				// methode de compression: stored

				// lit un gif normal
				Read_Gif( handle, Log, PalettePcx, BufSpeak ) ;

				Close( handle ) ;
			}
			break;
		}
		Flip() ;
		FadeToPal( PalettePcx ) ;

		timer = TimerRef + 4 * 50 ;
		while( TimerRef < timer ) if( Key == K_ESC ) break ;

		if( Key == K_ESC ) break ;

		FadeToBlack( PalettePcx ) ;
	}

	SetBlackPal() ;
	Cls() ;
	Flip() ;
	HQM_Free( ptrtxt ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

