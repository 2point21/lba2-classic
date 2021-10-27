#include 	"c_extern.h"

#include	<process.h>

extern	char	*Version ;

extern	LONG	MixMusic  ;
extern	LONG	MaxVolume ;		// Max Music Volume if no Mixer
extern	UWORD	GameVolumeMenu[] ;
extern	UWORD	GameOptionMenu[] ;
extern	LONG	FlecheForcee ;
#ifdef	CDROM
extern	LONG	FlagDisplayText ;
extern	UBYTE	*BufMemoSeek	;
#endif

ULONG	SpriteMem, SampleMem, AnimMem ;

//WORD	Lig=0 ;
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
#ifdef	DEBUG_TOOLS
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

ULONG	MemoMemory ;
ULONG	MemoDosMemory ;
ULONG	MemoMinDosMemory ;
ULONG	UsedHQMemory = 0 ;

UBYTE	NamePcxSave[256] ;
WORD	NumPcxSave = 0 ;

WORD	MinNbf = 1000 ;
WORD	MaxNbf = 0 ;

LONG	NbNbf = 0 ;
LONG	TotalNbf = 0 ;

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	AffDebugMenu()
{
	LONG	flag ;
	LONG	xm, ym ;
	WORD	select ;
	WORD	lig = 0 ;

	CoulText( 15, 0 ) ;

	Text( 0,lig+=9, "%FIsland: %d", Island ) ;
	Text( 0,lig+=9, "%FCube: %d", NumCube ) ;
	Text( 0,lig+=9, "%FChapter: %d", Chapitre ) ;

	Text( 0,lig+=10,"%FSceZoom: %d", SceZoom ) ;
	Text( 0,lig+=9, "%FFlagCredits: %d", FlagCredits ) ;
	Text( 0,lig+=9, "%FCmptMemoTimer: %d", CmptMemoTimerRef ) ;
	Text( 0,lig+=9, "%FNb Objs: %d", NbObjets ) ;
	Text( 0,lig+=9, "%FNb Bodys: %d", NbBodys ) ;
	Text( 0,lig+=9, "%FNb Zones: %d", NbZones ) ;
	Text( 0,lig+=9, "%fNb Tracks: %d", NbBrickTrack ) ;

	Text( 0,lig+=10,"%FNbFPS: %d", NbFramePerSecond ) ;
	Text( 0,lig+=9, "%FFree(K): %d", ((LONG)Malloc(-1L))/1024 ) ;

	Text( 0,lig+=10, "%FMemory at start: %d Ko", MemoMemory/1024 ) ;
	Text( 0,lig+=9,  "%FSize HQM_Memory: %d Ko", Size_HQM_Memory/1024 ) ;
	Text( 0,lig+=9,  "%FMax Used HQM_Memory: %d Ko", UsedHQMemory/1024 ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	CheckSavePcx()
{
	if( Key == K_F9 )
	{
		CopyScreen( Log, Screen ) ;
		strcpy( NamePcxSave, "LBA" ) ;
		strcat( NamePcxSave, itoa( NumPcxSave,"          ",10 ) ) ;
		AddExt( NamePcxSave, ".PCX" ) ;

		Save_Pcx( NamePcxSave, Screen, PtrPal ) ;

		NumPcxSave++ ;
		while( Key ) ;
		FirstTime = TRUE ;
	}
}

#endif


/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			  Û    ÛÛÜ Û  Û    ฿฿Û฿฿ ÛÛ฿฿฿
			  ÛÛ   ÛÛ฿ÛÛ  ÛÛ     ÛÛ  ฿฿฿฿Û
			  ฿฿   ฿฿  ฿  ฿฿     ฿฿  ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	InitGameLists()
{
	LONG	i ;

	ClearExtra() ;

	for( i=0; i<MAX_INCRUST_DISP; i++ )
	{
		ListIncrustDisp[i].Num = -1 ;
	}
	for( i=0; i<MAX_FLAGS_CUBE; i++ )
	{
		ListFlagCube[i] = 0 ;
	}
	for( i=0; i<MAX_FLAGS_GAME; i++ )
	{
		ListFlagGame[i] = 0 ;
	}
	for( i=0; i<MAX_INVENTORY; i++ )
	{
		ListFlagInventory[i] = 0 ;
	}
	for( i=0; i<4; i++ )
	{
		SampleAmbiance[i] = -1 ;
		SampleRepeat[i] = 1 ;
		SampleRnd[i] = 1 ;
	}
	for ( i = 0 ; i < MAX_HOLO_POS ; i++ )
	{
		TabHoloPos[i] = 0 ;
	}

	NbObjets = 0 ;
	NbBodys = 0 ;
	NbZones = 0 ;
	NbBrickTrack = 0 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	InitGame( int argc, UBYTE *argv[] )
{
	WORD	i ;
	WORD	objselect = -1 ;
	WORD	xm, ym, zm ;
	T_OBJET	*ptrobj ;
	T_EXTRA	*ptrextra ;

/*-------------------------------------------------------------------------*/

	UnSetClip() ;

	AlphaLight = 896 ;
	BetaLight = 950 ;

	Init3DGame() ;
	InitGameLists() ;
	InitPerso() ;

	SceneStartX = 16*SIZE_BRICK_XZ ;
	SceneStartY = 24*SIZE_BRICK_Y ;
	SceneStartZ = 16*SIZE_BRICK_XZ ;

/*-------------------------------------------------------------------------*/
/* init FIRST scene files */

	NewCube = 0 ;
	NumCube = -1 ;

	FlagTheEnd = -1 ;
	MagicLevel = 0 ;
	MagicPoint = 0 ;
	NbGoldPieces = 0 ;
	NbLittleKeys = 0 ;
	Chapitre = 0 ;
	NbFourLeafClover = 2 ;
	NbCloverBox = 2 ;
	Weapon = 0 ;
	Island = 0	;
	Fuel = 0 ;
	NumPingouin = -1 ;
	FlagWater = FALSE ;
	NumObjFollow = NUM_PERSO ;
	SaveBeta = 0 ;
	SaveComportement = Comportement = C_NORMAL ;

	if( argc == -1 )
	{
		LoadGame() ;

		if( SceneStartX == -1 )
		{
			FlagChgCube = 0 ;	// use startpos
		}
	}

	FadeToBlack( PtrPal ) ;
	Cls() ;
	Flip() ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
     Û    ÛÛÜ Û ฿฿Û฿฿ Û฿฿฿Û Û฿฿฿Û Û฿฿฿Ü Û   Û Û฿฿฿฿ ฿฿Û฿฿  Û    Û฿฿฿Û ÛÛÜ Û
     ÛÛ   ÛÛ฿ÛÛ   ÛÛ  ÛÛ฿Û฿ ÛÛ  Û ÛÛ  Û ÛÛ  Û ÛÛ      ÛÛ   ÛÛ   ÛÛ  Û ÛÛ฿ÛÛ
     ฿฿   ฿฿  ฿   ฿฿  ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿  ฿฿฿฿฿ ฿฿฿฿฿   ฿฿   ฿฿   ฿฿฿฿฿ ฿฿  ฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	Introduction()
{
	// history text

#ifdef	CDROM
	StopMusicCD() ;
//	FadeMusicMidi( 1 ) ;
#endif

	if( (NewCube == 0) AND (Chapitre == 0) )
	{
#ifdef	CDROM
		LONG	memoflagdisplaytext ;

		memoflagdisplaytext = FlagDisplayText ;
		FlagDisplayText = TRUE ;
#endif

		Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_TWINSUN_PCR ) ;
		CopyScreen( Screen, Log ) ;
		Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_TWINSUN_PAL ) ;
		Flip() ;
		FadeToPal( PalettePcx ) ;

		FlagMessageShade = FALSE ;
		FlecheForcee = TRUE ;
		InitDial( 2 ) ;
		BigWinDial() ;
		TestCoulDial( 15 ) ;

		Dial( 150 ) ;
		if( Key == K_ESC )	goto fin_intro ;

		SetBlackPal() ;
		Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_INTRO_2_PCR ) ;
		CopyScreen( Screen, Log ) ;
		Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_INTRO_2_PAL ) ;
		Flip() ;
		Palette( PalettePcx ) ;

		Dial( 151 ) ;
		if( Key == K_ESC )	goto fin_intro ;

		SetBlackPal() ;
		Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_INTRO_3_PCR ) ;
		CopyScreen( Screen, Log ) ;
		Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_INTRO_3_PAL ) ;
		Flip() ;
		Palette( PalettePcx ) ;

		FlecheForcee = FALSE ;
		Dial( 152 ) ;

fin_intro:
		FlecheForcee = FALSE ;
		NormalWinDial() ;
		FlagMessageShade = TRUE ;
		FadeToBlack( PalettePcx ) ;
		Cls() ;
		Flip() ;

		// fla du rve
		// fm ! toujours
		PlayMidiFile( 1 ) ;

		PlayAnimFla( "INTROD" ) ;

		SetBlackPal() ;
		Cls() ;
		Flip() ;
#ifdef	CDROM
		FlagDisplayText = memoflagdisplaytext ;
#endif
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	     ÛÜ ÜÛ Û฿฿฿Û  Û    ÛÛÜ Û       Û     Û฿฿฿Û Û฿฿฿Û Û฿฿฿Û
	     ÛÛ฿ Û ÛÛ฿฿Û  ÛÛ   ÛÛ฿ÛÛ       ÛÛ    ÛÛ  Û ÛÛ  Û ÛÛ฿฿฿
	     ฿฿  ฿ ฿฿  ฿  ฿฿   ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

LONG	MainLoop()
{
	WORD	i ;
	WORD	objselect = -1 ;
	WORD	xm, ym, zm ;
	T_OBJET	*ptrobj ;
	T_EXTRA	*ptrextra ;
	LONG	memoflagdisplaytext ;
	ULONG	timeralign ;

	FirstTime = TRUE ;
	FlagFade = TRUE ;

/*-------------------------------------------------------------------------*/

	/* vitesse de chute propor pour tous les objets */
	InitRealValue( 0, -256, 5, &RealFalling ) ;
	timeralign = TimerRef ;

/*-------------------------------------------------------------------------*/

	while( TRUE )
	{
startloop:


//		while( TimerRef == timeralign ) ;
//		timeralign = TimerRef ;
		if( NbFramePerSecond > 500 )	Vsync() ;


/*		CoulText( 15, 0 ) ;
		Text( 0,0, "%FTimerRef: %l", TimerRef ) ;	*/

/*-------------------------------------------------------------------------*/
		if( NewCube != -1 )
		{
			ChangeCube() ;
		}

#ifdef	DEMO
		if( NumCube == 3 )	return 2 ;
#endif

/*-------------------------------------------------------------------------*/

		LastFire = MyFire ;

		MyJoy = Joy		;
		MyFire = Fire ; //& ~32 ;
		MyKey = Key ;

/*-------------------------------------------------------------------------*/
/* gestion clavier */	/* tools */

#ifdef	DEBUG_TOOLS

		if( MyKey == K_C )
		{
			ClearZoneSce() ;
		}

		if( MyKey == K_T )
		{
			TimerRef += 10 ;
		}

		if( MyKey == K_I )
		{
			AffDebugMenu() ;
		}

		if( MyKey == K_F11 )
		{
			AsciiMode ^= 1 ;
			while( Key ) ;
		}

		if( MyKey == K_W )
		{
			if( MagicBall != -1 )
			{
				ListObjet[NUM_PERSO].PosObjX = ListExtra[MagicBall].PosX ;
				ListObjet[NUM_PERSO].PosObjY = ListExtra[MagicBall].PosY ;
				ListObjet[NUM_PERSO].PosObjZ = ListExtra[MagicBall].PosZ ;
			}
		}

		CheckSavePcx() ;

//CoulText( 15,0 ) ;
//Text( 0,472, "%F%l ", (LONG)NbFramePerSecond ) ;

#endif	// debug tools

/*-------------------------------------------------------------------------*/
/* gestion clavier */	/* game */

	if( FlagCredits )
	{
#ifdef	CDROM
		if( GetMusicCD() != 8 )	PlayCdTrack( 8 ) ;
#else
		if( !IsMidiPlaying() )	PlayMidiFile( 9 ) ;
#endif
		if( Key OR Joy OR Fire )
		{
//			FlagCredits = FALSE ;
			break ;
		}
	}
	else
	if( !FlagFade )
	{
		if( (MyKey == K_ESC)
		AND (ListObjet[NUM_PERSO].LifePoint > 0)
		AND (ListObjet[NUM_PERSO].Body != -1)
		AND (!(ListObjet[NUM_PERSO].Flags&INVISIBLE))
		)
		{
			// confirmation sortie
			TestRestoreModeSVGA( TRUE ) ;
			SaveTimer() ;

			if( QuitMenu() )
			{
				RestoreTimer() ;
				AffScene( TRUE ) ;

				SaveTimer() ;
				SaveGame() ;
				RestoreTimer() ;
				break ;
			}
			else
			{
				RestoreTimer() ;
				AffScene( TRUE ) ;
			}
		}

		if( MyKey == K_F6 )		// options menu
		{
			LONG	memoflagspeak = FlagSpeak ;

			SaveTimer() ;
			TestRestoreModeSVGA( TRUE ) ;
			HQ_StopSample();
			GameOptionMenu[5] = 15 ; // retour au jeu

			FlagSpeak = FALSE ;
			InitDial( 0 )	;	//	SYS

			OptionsMenu() ;

			FlagSpeak = memoflagspeak ;
			InitDial( START_FILE_ISLAND+Island )	;

			// rustine scenarique pour twinsun cafe et credits

			if( NumCube == 80 )
			{
				if( ListFlagGame[90] == 1 )
				{
#ifdef	CDROM
					PlayCdTrack( 8 ) ;	// funkyrock
#endif
				}
				else
				{
					PlayMusic( CubeJingle ) ;
				}
			}
			else
			{
				PlayMusic( CubeJingle ) ;
			}

			RestoreTimer() ;
			AffScene( TRUE ) ;
		}

		InventoryAction = -1 ;
		if( (MyFire & F_SHIFT)
		AND (ListObjet[NUM_PERSO].Body != -1)
		AND (ListObjet[NUM_PERSO].Move == MOVE_MANUAL) )
		{
			SaveTimer() ;
			TestRestoreModeSVGA( TRUE ) ;
			Inventory() ;
			switch( InventoryAction )
			{
			case 0:	// holomap
				HoloMap() ;
				FlagFade = TRUE ;
				break ;

			case 1: // balle magique
				if( Weapon == 1 )
				{
					InitBody( GEN_BODY_NORMAL, NUM_PERSO ) ;
				}
				Weapon = 0 ;
				break ;

			case 2:	// sabre magique
				if( ListObjet[NUM_PERSO].GenBody != GEN_BODY_SABRE )
				{
					if( Comportement == C_PROTOPACK )
					{
						SetComportement( C_NORMAL ) ;
					}
					// anim degaine sabre
					InitBody( GEN_BODY_SABRE, NUM_PERSO ) ;
					InitAnim( GEN_ANIM_DEGAINE, ANIM_THEN, GEN_ANIM_RIEN, NUM_PERSO ) ;
				}
				Weapon = 1 ;
				break ;

			case 5: // livre de b–

				FadeToBlack( PtrPal ) ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_TWINSUN_PCR ) ;
				CopyScreen( Screen, Log ) ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_TWINSUN_PAL ) ;
				Flip() ;
				FadeToPal( PalettePcx ) ;

				InitDial( 2 ) ;

				FlagMessageShade = FALSE ;
				BigWinDial() ;
				TestCoulDial( 15 ) ;
#ifdef	CDROM
				memoflagdisplaytext = FlagDisplayText ;
				FlagDisplayText = TRUE ;
#endif
				Dial( 161 ) ;
#ifdef	CDROM
				FlagDisplayText = memoflagdisplaytext ;
#endif
				NormalWinDial() ;
				FlagMessageShade = TRUE ;
				InitDial( START_FILE_ISLAND+Island ) ;
				FadeToBlack( PalettePcx ) ;
				Cls() ;
				Flip() ;
				FlagFade = TRUE ;
				break ;

			case 12: // protopack
				if( ListFlagGame[FLAG_MEDAILLON] )
				{
					ListObjet[NUM_PERSO].GenBody = GEN_BODY_NORMAL ;  // avec mdaillon
				}
				else
				{
					ListObjet[NUM_PERSO].GenBody = GEN_BODY_TUNIQUE ; // sans mdaillon
				}
				if( Comportement == C_PROTOPACK )
				{
					SetComportement( C_NORMAL ) ;
				}
				else
				{
					SetComportement( C_PROTOPACK ) ;
				}
				Weapon = 0 ; // balle magique
				break ;

			case 14: // meca pingouin
				ptrobj = &ListObjet[NumPingouin] ;

				Rotate( 0,800, ListObjet[NUM_PERSO].Beta ) ;

				ptrobj->PosObjX = ListObjet[NUM_PERSO].PosObjX + X0 ;
				ptrobj->PosObjY = ListObjet[NUM_PERSO].PosObjY ;
				ptrobj->PosObjZ = ListObjet[NUM_PERSO].PosObjZ + Y0 ;

				ptrobj->Beta = ListObjet[NUM_PERSO].Beta ;

				if( CheckValidObjPos( NumPingouin ) )
				{
					ptrobj->LifePoint = 50 ;
					ptrobj->GenBody = NO_BODY ;
					InitBody( GEN_BODY_NORMAL, NumPingouin ) ;

					ptrobj->WorkFlags &= ~OBJ_DEAD ;
					ptrobj->Col = 0 ;

					InitRealAngleConst(	ptrobj->Beta,
								ptrobj->Beta,
								ptrobj->SRot,
								&ptrobj->RealAngle ) ;

					*(ULONG*)(&ptrobj->Info) = TimerRef + 30 * 50 ;

					ListFlagGame[FLAG_MECA_PINGOUIN] = 0 ; // a plus
				}

				break ;

			case 27: // four leaf clover
				if( ListObjet[NUM_PERSO].LifePoint < 50 )
				{
					if( NbFourLeafClover != 0 )
					{
						NbFourLeafClover-- ;

						MagicPoint = MagicLevel * 20 ;
						ListObjet[NUM_PERSO].LifePoint = 50 ;

						// mettre aff des barres de point
						InitIncrustDisp( INCRUST_OBJ,
								 FLAG_CLOVER,
								 0, 0,
								 0, 0, 3 ) ;
					}
				}
				break ;

			case 26: // liste emplacements
				{
					LONG	memoflagspeak = FlagSpeak ;

					RestoreTimer() ;
					AffScene( TRUE ) ;
					SaveTimer() ;
					FlagSpeak = FALSE ;
					InitDial( 2 ) ;
					BigWinDial() ;
					TestCoulDial( 15 ) ;
					Dial( 162 ) ;
					NormalWinDial() ;
					FlagSpeak = memoflagspeak ;
					InitDial( START_FILE_ISLAND+Island ) ;
				}
				break ;
			}
			RestoreTimer() ;
			AffScene( TRUE ) ;
		}

		if( (MyFire & F_CTRL)
		AND (ListObjet[NUM_PERSO].Body != -1)
		AND (ListObjet[NUM_PERSO].Move == MOVE_MANUAL) )
		{
			SaveTimer() ;
			TestRestoreModeSVGA( TRUE ) ;
			MenuComportement() ;
			RestoreTimer() ;
			AffScene(TRUE) ;
		}

		if( (MyKey >= K_F1) AND (MyKey <= K_F4)
		AND (ListObjet[NUM_PERSO].Body != -1)
		AND (ListObjet[NUM_PERSO].Move == MOVE_MANUAL) )
		{
			SaveTimer() ;
			TestRestoreModeSVGA( TRUE ) ;
			SetComportement( C_NORMAL + MyKey - K_F1 ) ;
			MenuComportement() ;
			RestoreTimer() ;
			AffScene(TRUE) ;
		}

		if( MyFire & F_RETURN )	/* recentre sur perso */
		{
			if( !CameraZone ) /* si pas camera force */
			{
				StartXCube = ((ListObjet[NumObjFollow].PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				StartYCube = ((ListObjet[NumObjFollow].PosObjY+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
				StartZCube = ((ListObjet[NumObjFollow].PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				FirstTime = TRUE ;
			}
		}

		if( (MyKey == K_H)
		AND (ListFlagGame[FLAG_HOLOMAP] == 1)
		AND (ListFlagGame[FLAG_CONSIGNE] == 0) )
		{
			SaveTimer() ;
			TestRestoreModeSVGA( TRUE ) ;
			HoloMap() ;
			FlagFade = TRUE ;
			RestoreTimer() ;
			AffScene(TRUE) ;
		}

		if( MyKey == K_P )
		{
			WavePause() ;
			SaveTimer() ;
			if( !FlagMCGA )
			{
				CoulFont( 15 ) ;
				Font( 5,446, "Pause" ) ;
				CopyBlockPhys( 5,446,100,479 ) ;
			}
			while( Key ) ;
			while( !Key AND !Joy AND !Fire ) ;
			if( !FlagMCGA )
			{
				CopyBlock( 5,446,100,479, Screen, 5,446, Log ) ;
				CopyBlockPhys( 5,446,100,479 ) ;
			}
			RestoreTimer() ;
			WaveContinue() ;
		}

		if( MyKey == K_F5 )		// zoom on/off
		{
			if( FlagMCGA ^= 1 )
			{
				ExtInitMcga() ;
				while( Key ) ;
			}
			else
			{
				ExtInitSvga() ;
				FirstTime = TRUE ;
				while( Key ) ;
			}
		}
/*
		if( MyKey == K_B )
		{
			if( Wave_Driver_Enable )
			{
				if( (SamplesEnable ^= 1) == 0 )
				{
					HQ_StopSample() ;
					Message( "Samples OFF", TRUE ) ;
				}
				else
				{
					Message( "Samples ON", TRUE ) ;
				}
			}
		}
*/
	} // if !flagfade

/*-------------------------------------------------------------------------*/
/* gere l'ambiance */

		/* vitesse de chute propor pour tous les objets */
		StepFalling = GetRealValue( &RealFalling ) ;
		if( StepFalling == 0 )	StepFalling = 1 ;
		InitRealValue( 0, -256, 5, &RealFalling ) ;

		CameraZone = FALSE ;

		GereAmbiance() ;

/*-------------------------------------------------------------------------*/
/* gere les objets */

		// asm
		ptrobj = ListObjet ;
		for( i=0; i<NbObjets; i++, ptrobj++ )
		{
			ptrobj->HitBy = -1 ;
		}

		GereExtras() ;

		ptrobj = ListObjet ;
		for( i=0; i<NbObjets; i++, ptrobj++ )
		{
			if( ptrobj->WorkFlags & OBJ_DEAD )	continue ;

			// test mort d'un objet

			if( ptrobj->LifePoint == 0 )
			{
				if( i == NUM_PERSO )	// twinsen
				{
					InitAnim( GEN_ANIM_MORT, ANIM_SET, GEN_ANIM_RIEN, NUM_PERSO ) ;
					ptrobj->Move = NO_MOVE ;
				}
				else	// tout objet
				{
					// sample specifique mort ?
					HQ_3D_MixSample( 37, 0x1000+Rnd(2000)-(2000/2), 1,
						ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;

					// 'explosion' de l'objet
/*					InitSpecial(	ptrobj->PosObjX,
							ptrobj->PosObjY+1000,
							ptrobj->PosObjZ,
							2 ) ;
*/
					if( i == NumPingouin )
					{
						ExtraExplo( ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
					}
				}

				if( (ptrobj->OptionFlags & EXTRA_MASK)
				AND !(ptrobj->OptionFlags & EXTRA_GIVE_NOTHING) )
				{
					GiveExtraBonus( ptrobj ) ;
				}
			}

			DoDir( i ) ;

			ptrobj->OldPosX = ptrobj->PosObjX ;
			ptrobj->OldPosY = ptrobj->PosObjY ;
			ptrobj->OldPosZ = ptrobj->PosObjZ ;

			if( ptrobj->OffsetTrack != -1 )
			{
				DoTrack( i ) ;
			}
			DoAnim( i ) ;

			if( ptrobj->Flags & CHECK_ZONE )
			{
				CheckZoneSce( ptrobj, i ) ;
			}

			if( ptrobj->OffsetLife != -1 )
			{
				DoLife( i ) ;
			}

			if( FlagTheEnd != -1 )	return FlagTheEnd ; // mmm violent

			// test des codes jeu ici ?

			if( ptrobj->Flags & CHECK_CODE_JEU )
			{
				ptrobj->CodeJeu = WorldCodeBrick(ptrobj->PosObjX,
								 ptrobj->PosObjY-1,
								 ptrobj->PosObjZ ) ;

				if( (ptrobj->CodeJeu & 0xF0) == 0xF0 )
				{
				switch( ptrobj->CodeJeu & 0x0F )
				{
					case 1: // eau
					if( i == NUM_PERSO )
					{
						if( (Comportement == C_PROTOPACK)
						AND (ptrobj->GenAnim == GEN_ANIM_MARCHE) )
							break ;

						if( !FlagWater )
						{
							InitAnim( GEN_ANIM_NOYADE, ANIM_SET, GEN_ANIM_RIEN, NUM_PERSO ) ;

							ProjettePoint( ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
							FlagWater = Yp ;

							// init clipping Y noyade
						}

						ProjettePoint(	ptrobj->PosObjX-WorldXCube,
								ptrobj->PosObjY-WorldYCube,
								ptrobj->PosObjZ-WorldZCube ) ;
						FlagWater = Yp ;

						ptrobj->Move = NO_MOVE ;
						ptrobj->LifePoint = -1 ;
						ptrobj->Flags |= NO_SHADOW ;
					}
					else	// tout objet meurt dans l'eau
					{
						HQ_3D_MixSample( 37, 0x1000+Rnd(2000)-(2000/2), 1,
							ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
						// 'explosion' de l'objet

						if( (ptrobj->OptionFlags & EXTRA_MASK)
						AND !(ptrobj->OptionFlags & EXTRA_GIVE_NOTHING) )
						{
							GiveExtraBonus( ptrobj ) ;
						}
						ptrobj->LifePoint = 0 ;
					}
					break ;
				}
				}
			}


			// si aprs la vie on … toujours 0 point de vie
			// destruction definitive
			if( ptrobj->LifePoint <= 0 )
			{
				if( i == NUM_PERSO )
				{
				if( ptrobj->WorkFlags & ANIM_END )
				{
				    if( NbFourLeafClover > 0 )
				    {
					FlagWater = FALSE ;

					// restart
					NbFourLeafClover-- ;

					ListObjet[NUM_PERSO].PosObjX = SceneStartX ;
					ListObjet[NUM_PERSO].PosObjY = SceneStartY ;
					ListObjet[NUM_PERSO].PosObjZ = SceneStartZ ;

					NewCube = NumCube ;
					FlagChgCube = 3 ;

					ListObjet[NUM_PERSO].LifePoint = 50 ;
					MagicPoint = MagicLevel * 20 ;

					StartXCube = ((ListObjet[NUM_PERSO].PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
					StartYCube = ((ListObjet[NUM_PERSO].PosObjY+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
					StartZCube = ((ListObjet[NUM_PERSO].PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;

					FirstTime = TRUE ;
					FlagFade = TRUE ;
					goto startloop ;
				    }
				    else	// game over
				    {

					NbFourLeafClover = NbCloverBox / 2 ;
					ListObjet[NUM_PERSO].LifePoint = 25 ;
					MagicPoint = (MagicLevel*20) / 2 ;
					Comportement = SaveComportement ;
					ListObjet[NUM_PERSO].Beta = SaveBeta ;

					if( GameOverCube != NumCube )
					{
						NumCube = GameOverCube ;
						SceneStartX = SceneStartY = SceneStartZ = -1 ; // mean use startpos
					}
					SaveGame() ;

					GameOver() ;

					return 0 ;
				    }
				}
				}
				else	// autre obj
				{
					CheckCarrier( i ) ;
					ptrobj->WorkFlags |= OBJ_DEAD ;
					ptrobj->Body = -1 ;
					ptrobj->ZoneSce = -1 ;
				}
			}

			if( NewCube != -1 )
			{
				goto startloop ;
			}
		}

/*
ptrobj = &ListObjet[ 4 ] ;
CoulText( 15, 0 ) ;

Text( 10,10, "%F       GenAnim: %d ", ptrobj->GenAnim ) ;
Text( 10,20, "%F          Anim: %d ", ptrobj->Anim ) ;
Text( 10,30, "%F         Frame: %d ", ptrobj->Frame ) ;
Text( 10,40, "%F       GenBody: %d ", ptrobj->GenBody ) ;
Text( 10,50, "%F          Body: %d ", ptrobj->Body ) ;
Text( 10,60, "%F    LabelTrack: %d ", ptrobj->LabelTrack ) ;
Text( 10,70, "%FMemoLabelTrack: %d ", ptrobj->MemoLabelTrack ) ;
*/

// Text( 0,110, "%FListFlagGame[134] = %d",ListFlagGame[134] ) ;


/*-------------------------------------------------------------------------*/
/* recentre sur hero (numobjfollow) */

	if( !CameraZone )
	{
		T_OBJET	*ptrobj ;

		ptrobj = &ListObjet[ NumObjFollow ] ;

		ProjettePoint(	ptrobj->PosObjX-StartXCube*SIZE_BRICK_XZ,
				ptrobj->PosObjY-StartYCube*SIZE_BRICK_Y,
				ptrobj->PosObjZ-StartZCube*SIZE_BRICK_XZ ) ;

		if( Xp<80 OR Xp>539 OR Yp<80 OR Yp>429 )
		{
			xm = (ptrobj->PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ ;
			ym =  ptrobj->PosObjY/SIZE_BRICK_Y ;
			zm = (ptrobj->PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ ;

			StartXCube = xm + ((xm-StartXCube))/2;
			StartYCube = ym ;
			StartZCube = zm + ((zm-StartZCube))/2;

			if( StartXCube >= SIZE_CUBE_X )	StartXCube = SIZE_CUBE_X - 1 ;
			if( StartZCube >= SIZE_CUBE_Z )	StartZCube = SIZE_CUBE_Z - 1 ;

			FirstTime = TRUE ;
		}
	}

/*-------------------------------------------------------------------------*/
/* affiche tout */

		AffScene( FirstTime ) ;

		FirstTime = FALSE ;

		CmptFrame++ ;

#ifdef	DEBUG_TOOLS
		if( NbFramePerSecond < MinNbf )	MinNbf = NbFramePerSecond ;
		if( NbFramePerSecond > MaxNbf )	MaxNbf = NbFramePerSecond ;

		TotalNbf += NbFramePerSecond ;
		NbNbf++ ;
#endif
	}

	return 0 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			    ÛÜ ÜÛ Û฿฿฿Û  Û    ÛÛÜ Û
			    ÛÛ฿ Û ÛÛ฿฿Û  ÛÛ   ÛÛ฿ÛÛ
			    ฿฿  ฿ ฿฿  ฿  ฿฿   ฿฿  ฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ReadVolumeSettings()
{
	ULONG	i ;
	LONG	BSample, BMusic, BCD, BLine, BMaster ;

//	Test which settings are available

	BSample	= BMusic = BCD = BLine = BMaster = 0 ;

	MixerGetInfo(	&BSample,
			&BMusic,
			&BCD,
			&BLine,
			&BMaster ) ;

	MixMusic = BMusic ;

	//	Build the menu


	GameVolumeMenu[4+2] = 1  ;
	GameVolumeMenu[4+3] = 10 ;

	i = 2 ;
	if (BSample)
	{
		GameVolumeMenu[4+i*2]	= 2  ;
		GameVolumeMenu[4+i*2+1]	= 11 ;
		i++;
	}
	if (BCD)
	{
		GameVolumeMenu[4+i*2]	= 3  ;
		GameVolumeMenu[4+i*2+1]	= 12 ;
		i++;
	}
	if (BLine)
	{
		GameVolumeMenu[4+i*2]	= 4  ;
		GameVolumeMenu[4+i*2+1]	= 13 ;
		i++;
	}
	if (BMaster)
	{
		GameVolumeMenu[4+i*2]	= 5  ;
		GameVolumeMenu[4+i*2+1]	= 14 ;
		i++;
	}
	GameVolumeMenu[4+i*2]	= 0  ;
	GameVolumeMenu[4+i*2+1]	= 16 ;

	GameVolumeMenu[1] = i+1;

//	Read mixer settings

	MixerGetVolume(	&SampleVolume,
			&MusicVolume,
			&CDVolume,
			&LineVolume,
			&MasterVolume ) ;

//	Read config file

	Def_ReadValue2( PathConfigFile, "WaveVolume", &SampleVolume ) ;
	Def_ReadValue2( PathConfigFile, "MusicVolume", &MusicVolume ) ;
	Def_ReadValue2( PathConfigFile, "CDVolume", &CDVolume ) ;
	Def_ReadValue2( PathConfigFile, "LineVolume", &LineVolume ) ;
	Def_ReadValue2( PathConfigFile, "MasterVolume", &MasterVolume ) ;


//	Reset the mixer to correct values

	if (!MixMusic)
	{
		MaxVolume = MusicVolume ;
		VolumeMidi( 100 ) ;
	}

	MixerChangeVolume(	SampleVolume,
				MusicVolume,
				CDVolume,
				LineVolume,
				MasterVolume ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	WriteVolumeSettings()
{
	Def_WriteValue( PathConfigFile, "WaveVolume", SampleVolume ) ;
	Def_WriteValue( PathConfigFile, "MusicVolume", MusicVolume ) ;
	Def_WriteValue( PathConfigFile, "CDVolume", CDVolume ) ;
	Def_WriteValue( PathConfigFile, "LineVolume", LineVolume ) ;
	Def_WriteValue( PathConfigFile, "MasterVolume", MasterVolume ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	InitProgram()
{
	InitAdelineSystem( "LBA.CFG",	INIT_SVGA +
					INIT_WAVE +
					INIT_MIXER +
					INIT_MIDI ) ;

#ifdef	TRACE
	AsciiMode = TRUE 	;
#endif

#ifdef	CDROM
	InitVoiceFile()	;
#endif
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	TheEnd( WORD num, UBYTE *error )
{
#ifdef	DEBUG_TOOLS
	MemoMinDosMemory = (ULONG)DosMalloc( -1, NULL ) ;// Dos memory after inits
#endif

#ifdef	CDROM
	ClearVoiceFile()	;
	ClearCDR() ;
#endif
	ClearAdelineSystem() ;

	printf( Version ) ;	/*	dans version.c	*/

#ifdef	DEBUG_TOOLS
	printf( "* Start Extended Memory was %ld\n", MemoMemory ) ;
	printf( "* Start Dos Memory was %ld\n", MemoDosMemory ) ;
	printf( "* Min Dos Memory was %ld\n", MemoMinDosMemory ) ;
	printf( "* HQR Sprite: %ld\n", SpriteMem ) ;
	printf( "      Sample: %ld\n", SampleMem ) ;
	printf( "        Anim: %ld\n", AnimMem ) ;

	printf( "* Size HQM Memory was %ld\n", Size_HQM_Memory ) ;
	printf( "* Max Used HQM memory was %ld\n", UsedHQMemory ) ;
#endif
	switch( num )
	{
		case ERROR_FILE_NOT_FOUND:
			printf( "File not found: %s\n", error ) ;
			break ;

		case NAME_NOT_FOUND:
			printf( "Critical error: ident name not found: %s\n", error ) ;
			break ;

		case NOT_ENOUGH_MEM:
			printf( "Not Enough Memory: %s (SEE README.TXT)\n", error ) ;
			break ;

		case PROGRAM_OK:
#ifdef	DEBUG_TOOLS
			if( NbNbf )
			{
				printf( "* Frame speed status:\n" ) ;
//				printf( "  Minimal frame rate occured: %d\n", MinNbf ) ;
				printf( "  Maximal frame rate occured: %d\n", MaxNbf ) ;
//				printf( "  number of frame is %f\n", NbNbf ) ;
//				printf( "  (total of frame's NbFramePerSecon is %f )\n", TotalNbf ) ;
				printf( "  Average frame rate was %d\n", TotalNbf/NbNbf ) ;
				printf( "%s\n", error ) ;
			}
#endif
			printf( "%s\n", error )	;
			printf( "\nOK.\n" ) ;
			break ;
	}
	RestoreDiskEnv() ;
	exit( 0 ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

int	__far	Critical_Error_Handler( unsigned deverr,
					unsigned errcode,
					unsigned far *devhdr )
{
	return( _HARDERR_RETRY )	;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

#ifdef	DEBUG_TOOLS
void	Message( UBYTE *mess, WORD flag )
{
	WORD	x ;

	CoulText( 15, 0 ) ;
	PalOne( 15, 255, 255, 255 ) ;
	x = (strlen(mess)*8)/2 ;
	MemoClip() ;

	Text( 320-x,236, "%s", mess ) ;
	Rect( 320-x-4, 234, 320+x+4, 248, 15 ) ;
	CopyBlockPhys( 320-x-4, 234, 320+x+4, 248 ) ;

	if( flag )
	{
		while( Key OR Joy OR Fire ) ;
		while( !Key AND !Joy AND !Fire ) ;
	}
	RestoreClip() ;
}
#else
void	Message(  UBYTE *mess, WORD flag )
{
}
#endif

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	main( int argc, UBYTE *argv[] )
{
	WORD	n ;
	ULONG	memory ;
	ULONG	memotimer ;

	_harderr_( Critical_Error_Handler )	;

#ifdef	DEBUG_TOOLS
	MemoMemory = (ULONG)Malloc( -1 ) ;	// memory at start
	MemoDosMemory = (ULONG)DosMalloc( -1, NULL ) ;// Dos memory at start
#endif

	GetDiskEnv( argv[0] ) ;		/* org path program path... */

	InitProgram() ;			/* init graphmode timer ... */

// infos from lba.cfg

	ReadVolumeSettings() ;

	Version_US = Def_ReadValue( "setup.lst", "Version_US" ) ;

	InitLanguage() ;	// multilangue

// dclarations HQR ressource

	if( Midi_Driver_Enable )
	{
		if( MidiFM )
		{
			HQR_Midi =	HQR_Init_Ressource(
					PATH_RESSOURCE"midi_sb.hqr",
// ATTENTION size du plus gros xmi
					32000,
					2 ) ;
		}
		else
		{
			HQR_Midi =	HQR_Init_Ressource(
					PATH_RESSOURCE"midi_mi.hqr",
// ATTENTION size du plus gros xmi
					32000,
					2 ) ;
		}

		if( !HQR_Midi )
		{
			Message( "HQR_Midi not enough memory", TRUE ) ;
			Midi_Driver_Enable = FALSE ;
		}

//		InitPathMidiSampleFile( PATH_RESSOURCE ) ;
	}

// presentation

	Screen = Malloc( 640*480 + 500 ) ;	// + decomp marge
	if( !Screen )	TheEnd( NOT_ENOUGH_MEM, "Screen" ) ;

#ifndef	DEBUG_TOOLS

// logo adeline
	AdelineLogo() ;

// pause logo
	memotimer = TimerRef ;
	while( TimerRef < (memotimer+50*4) ) ;
#endif

// check cd rom

#ifdef	DEMO
	FlaFromCD = TRUE ;		// fla sur HD
	strcpy( PathFla, "" ) ;		// version demo fla in current dir
#else

#ifdef	CDROM
	if( InitCDR( "CD_LBA" ) )
	{
		UBYTE	*drive = "D:" ;

		// cherche un fichier pour version preview

		drive[0] = 'A' + DriveCDR ;
		strcpy( PathFla, drive ) ;
		strcat( PathFla, "\\LBA\\FLA\\" ) ;
	}
	else	TheEnd(PROGRAM_OK, "No CD")	;

	if ( ProgDrive[0]-'A' == DriveCDR )// A=0 , B=1 etc.
		TheEnd(PROGRAM_OK, "Type INSTALL")	;

#else
	strcpy( PathFla, "FLA\\" ) ;	// version cdrom sur hd (fla only)
#endif

#endif

// divers malloc

	BufSpeak = DosMalloc( 256*1024 + 34, NULL ) ;
	if( !BufSpeak )	TheEnd( NOT_ENOUGH_MEM, "BufSpeak (Dos Memory)" ) ;

#ifdef	CDROM
	BufMemoSeek = SmartMalloc( 2048L ) ;
	if( !BufMemoSeek )	TheEnd( NOT_ENOUGH_MEM, "BufMemoSeek" ) ;
#endif
	BufText = SmartMalloc( 25000L ) ;
	if( !BufText )	TheEnd( NOT_ENOUGH_MEM, "BufText" ) ;

	BufOrder = SmartMalloc( 1024L ) ;
	if( !BufOrder )	TheEnd( NOT_ENOUGH_MEM, "BufOrder" ) ;


	PtrBufferAnim = BufferAnim = SmartMalloc( 5000L ) ;
	if( !BufferAnim ) TheEnd( NOT_ENOUGH_MEM, "Buffer Anim" ) ;

	InitBufferCube() ;

	InventoryObj = HQR_Init_Ressource( PATH_RESSOURCE"invobj.hqr", 20000, 30 ) ;
	if( !InventoryObj )	TheEnd( NOT_ENOUGH_MEM, "HQR Inventory" ) ;

	if( !HQM_Init_Memory( 400000 ) )
	{
		TheEnd( NOT_ENOUGH_MEM, "HQMemory" ) ;
	}

// load ressources diverses

	PtrPal = LoadMalloc_HQR( PATH_RESSOURCE"ress.hqr", RESS_PAL ) ;
	BufferShadow = LoadMalloc_HQR( PATH_RESSOURCE"ress.hqr", RESS_SHADOW_GPH ) ;
	PtrZvExtra = LoadMalloc_HQR( PATH_RESSOURCE"ress.hqr", RESS_GOODIES_GPC ) ;
	LbaFont = LoadMalloc_HQR( PATH_RESSOURCE"ress.hqr", RESS_FONT_GPM ) ;
	if( !LbaFont )	TheEnd( NOT_ENOUGH_MEM, "LbaFont" ) ;

	SetFont( LbaFont, 2, 8 )	;
	CoulFont( 14 )			;
	CoulDial( 136, 143, 2 )		;

// buffers variables en fonctions de la mmoire dispo

//	Message( Itoa( Malloc(-1) ), TRUE ) ;

	memory = (ULONG)Malloc( -1 ) ;

	SpriteMem = (memory/8) ;
	SampleMem = (memory/8)*4 ;
	AnimMem   = (memory/8)*2 ;

	if( SpriteMem < 50000 )	 	SpriteMem = 50000 ;
	if( SampleMem < 200000 ) 	SampleMem = 200000 ;
	if( AnimMem   < 100000 )	AnimMem   = 100000 ;

	if( SpriteMem > 400000 )	SpriteMem = 400000 ;
	if( SampleMem > 4500000 ) 	SampleMem = 4500000 ;
	if( AnimMem   > 300000 )	AnimMem   = 300000 ;

// buffer sprites

	HQRPtrSpriteExtra =	HQR_Init_Ressource(
				PATH_RESSOURCE"sprites.hqr",
				SpriteMem,
				SpriteMem/1000 ) ;

	if( !HQRPtrSpriteExtra ) TheEnd( NOT_ENOUGH_MEM, "HQRPtrSpriteExtra" ) ;

// buffer samples

	if( Wave_Driver_Enable )
	{
		// dclare ressource samples buffer
		HQR_Samples = HQR_Init_Ressource(
				PATH_RESSOURCE"samples.hqr",
				SampleMem,
				SampleMem/5000 ) ;

		SamplesEnable = TRUE ;

		if( !HQR_Samples )
		{
			Message( "HQR_Samples not enough memory", TRUE ) ;
			Wave_Driver_Enable = FALSE ;
			SamplesEnable = FALSE ;
		}
	}

// buffer animations

	HQR_Anims =	HQR_Init_Ressource(
			PATH_RESSOURCE"Anim.hqr",
			AnimMem,
			AnimMem/800 ) ;

	if( !HQR_Anims )
	{
		TheEnd( NOT_ENOUGH_MEM, "HQR_Anims" ) ;
	}


#ifndef	DEBUG_TOOLS
// bumper
	FadeToBlack( PalettePcx ) ;

	if( Version_US )	RessPict( RESS_BUMPER_PCR ) ;
	else			RessPict( RESS_BUMPER2_PCR ) ;

	TimerPause( 4 ) ;
	FadeToBlack( PalettePcx ) ;

// logo EA

	RessPict( RESS_BUMPER_EA_PCR ) ;
	TimerPause( 2 ) ;
	FadeToBlack( PalettePcx ) ;

// FLA intro

	PlayAnimFla( "DRAGON3" ) ;
#endif

// main game menu

//	FadeToBlack( PalettePcx ) ;

	Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_MENU_PCR ) ;
	CopyScreen( Screen, Log ) ;
	Flip() ;
	FadeToPal( PtrPal ) ;

	MainGameMenu() ;

	TheEnd( PROGRAM_OK, "" ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/






