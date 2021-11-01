#include        "c_extern.h"

/*#ifndef DEMO
#define THOMAS  1
#endif
*/

extern  char    *Version ;

extern  T_OBJ_3D        InvObjTwinsen[] ;

extern S32      PersoFlashTimer ;       // pour quand le perso eh ben y meurt !!!

S32     EscTimer ;

/*══════════════════════════════════════════════════════════════════════════*/
char    NamePcxSave[256] ;
S32     NumPcxSave = 0 ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
/*══════════════════════════════════════════════════════════════════════════*/

U32     MemoMemory ;
U32     MemEnd ;
U32     MemoDosMemory ;
U32     MemoMinDosMemory ;

S32     MinNbf = 1000 ;
S32     MaxNbf = 0 ;

S32     NbNbf = 0 ;
S32     TotalNbf = 0 ;

/*══════════════════════════════════════════════════════════════════════════*/

void    AffDebugMenu()
{
        S32     lig = 0 ;

        CoulText( LBAWHITE, 0 ) ;

        GraphPrintf( FALSE, 0,lig+=9,  "Island: %d", Island ) ;
        GraphPrintf( FALSE, 0,lig+=9,  "Cube: %d", NumCube ) ;
        GraphPrintf( FALSE, 0,lig+=9,  "Chapter: %d", ListVarGame[FLAG_CHAPTER] ) ;

        GraphPrintf( FALSE, 0,lig+=9,  "CmptMemoTimer: %d", CmptMemoTimerRef ) ;
        GraphPrintf( FALSE, 0,lig+=9,  "Nb Objs: %d", NbObjets ) ;
        GraphPrintf( FALSE, 0,lig+=9,  "Nb Zones: %d", NbZones ) ;
        GraphPrintf( FALSE, 0,lig+=9,  "Nb Tracks: %d", NbBrickTrack ) ;

        GraphPrintf( FALSE, 0,lig+=10, "NbFPS: %d", NbFramePerSecond ) ;

        GraphPrintf( FALSE, 0,lig+=9,  "Free(K): %d", (AvailableMem())/1024 ) ;
        GraphPrintf( FALSE, 0,lig+=10, "Memory at start: %d Ko", MemoMemory/1024 ) ;

        GraphPrintf( FALSE, 0,lig+=9,  "Total Extend Memory used: %d Ko", (MemoMemory-(AvailableMem()) )/1024 ) ;

        lig+=9 ;

        GraphPrintf( FALSE, 0,lig+=9,  "CameraY     : %d", CameraY ) ;

        lig+=9 ;

        GraphPrintf( FALSE, 0,lig+=9,  "AlphaCam     : %d", AlphaCam ) ;
        GraphPrintf( FALSE, 0,lig+=9,  "VueDistance  : %d", VueDistance ) ;
}
#endif

/*══════════════════════════════════════════════════════════════════════════*/

// Pour eviter d'ecraser les anciennes copies d'ecran !!!!
void    GetNumPcxSave()
{
        struct find_t find      ;
        S32 done                ;
        S32 num                 ;

        done = _dos_findfirst( PATH_PCX_SAVE"LBA*.PCX", _A_NORMAL, &find ) ;

        while (!done)
        {
                num = atoi( &find.name[3] ) ;
                if( num>=NumPcxSave )   NumPcxSave = num+1 ;

                done = _dos_findnext( &find ) ;
        }
}

void    CheckSavePcx()
{
        if( Key == K_F9 )
        {
                S32     saveflagfade    ;

		SaveTimer() ;
		MemoClipWindow()	;
		UnsetClipWindow()	;

		BackupScreen( FALSE ) 	;
		CopyScreen( Log, Screen ) ;

                GetNumPcxSave() ;

/*		strcpy( NamePcxSave, PATH_PCX_SAVE ) ;
		strcat( NamePcxSave, "LBA" ) ;
		strcat( NamePcxSave, itoa( NumPcxSave,"          ",10 ) ) ;
		AddExt( NamePcxSave, ".PCX" ) ;
*/
		sprintf( NamePcxSave, "%sLBA%05d.PCX", PATH_PCX_SAVE, NumPcxSave ) ;

                CoulText( LBAWHITE, 0 ) ;
                GraphPrintf( TRUE, 50, 0, "Saving Screen into %s", NamePcxSave ) ;

                SavePCX( NamePcxSave, Screen, ModeDesiredX, ModeDesiredY, PtrPal ) ;

                WaitNoInput() ;

                // RestoreScreen without BoxUpdate()
                CopyScreen( ScreenAux, Screen ) ;
                CopyScreen( ScreenAux, Log )    ;

                RestoreTimer()                  ;
                SaveTimer()                     ;

                saveflagfade = FlagFade         ;
                FlagFade = FALSE                ;

		RestoreClipWindow() 		;

		BoxReset() ;
		BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;

		AffScene( AFF_OBJETS_NO_FLIP )  ;
		FixeCinemaMode( FALSE )		;

                FlagFade = saveflagfade         ;
                GraphPrintf( FALSE, 50, 0, "Saving Screen into %s", NamePcxSave ) ;
                BoxBlit()       ;
                RestoreTimer()  ;
        }
}

void    CheckSaveLogPcx( U8 *pal )
{
        if( Key == K_F9 )
        {
                SaveTimer() ;

                GetNumPcxSave() ;

/*                strcpy( NamePcxSave, PATH_PCX_SAVE ) ;
		strcat( NamePcxSave, "LBA" ) ;
		strcat( NamePcxSave, itoa( NumPcxSave,"          ",10 ) ) ;
		AddExt( NamePcxSave, ".PCX" ) ;
*/
		sprintf( NamePcxSave, "%sLBA%05d.PCX", PATH_PCX_SAVE, NumPcxSave ) ;

                SavePCX( NamePcxSave, Log, ModeDesiredX, ModeDesiredY, pal ) ;

                CoulText( LBAWHITE, 0 ) ;
                GraphPrintf( TRUE, 50, 0, "Saving Screen into %s", NamePcxSave ) ;

                WaitNoInput() ;

                RestoreTimer()                  ;
        }
}

//****************************************************************************
/*══════════════════════════════════════════════════════════════════════════*
                          █    ██▄ █  █    ▀▀█▀▀ ██▀▀▀
                          ██   ██▀██  ██     ██  ▀▀▀▀█
                          ▀▀   ▀▀  ▀  ▀▀     ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void    LoadListAnim3DS( void )
{
        U32     handle, buffer ;
        S32     n ;

        // recupere nombre d'anim dans la banque
        handle = OpenRead( HQRPtrAnim3DS->Name ) ;
        if( handle )
        {
                Read( handle, &buffer, 4 ) ;
                Close ( handle ) ;
                n = (buffer / 4)-2 ;
        }

        // Charge table de description des anims
        ListAnim3DS     = (T_ANIM_3DS *)LoadMalloc_HQR( PATH_RESSOURCE"anim3ds.hqr", n ) ;
        if( !ListAnim3DS )      TheEndCheckFile( PATH_RESSOURCE"anim3ds.hqr" ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    InitGameLists()
{
        T_INCRUST_DISP  *ptrdisp = ListIncrustDisp ;
        S32     i ;

// Peut-etre inutile car fait ailleurs (a verifier ?)
        ClearExtra() ;

        for( i=0; i<MAX_INCRUST_DISP; i++, ptrdisp++ )
        {
                ptrdisp->Num = -1 ;
        }

        memset( ListVarCube, 0, MAX_VARS_CUBE*sizeof(ListVarCube[0]) ) ;
        memset( ListVarGame, 0, MAX_VARS_GAME*sizeof(ListVarGame[0]) ) ;

	ResetBuggy() ;
	ResetArdoise() ;

        // Init VarGames
        InitInventory() ;

        for( i=0; i<4; i++ )
        {
                SampleAmbiance[i] = -1 ;
                SampleRepeat[i] = 1 ;
                SampleRnd[i] = 1 ;
        }

        NbObjets = 0 ;
        NbZones = 0 ;
        NbBrickTrack = 0 ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void    InitGame( int argc )
{
/*-------------------------------------------------------------------------*/
        UnsetClip() ;

        AlphaLight = 896*MUL_ANGLE ;
        BetaLight = 950*MUL_ANGLE ;

        PtrInit3DGame() ;

        InitGameLists() ;

        InitPerso() ;
        InitHoloMap() ;

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
        NbZlitosPieces = 0 ;
        NbLittleKeys = 0 ;
        ListVarGame[FLAG_CHAPTER] = 0 ;
        Weapon = FLAG_BALLE_MAGIQUE ;
        Island = 0      ;
        ListVarGame[FLAG_CLOVER] = NB_CLOVER_DEFAULT ;
        NbCloverBox = NB_CLOVER_BOX_DEFAULT ;
        FlagWater = FALSE ;
        NumObjFollow = NUM_PERSO ;
        SaveComportement = Comportement = C_NORMAL ;

        // Mettre a jour ici pour commencer le jeu directement en mode cinema
        CinemaMode = FALSE ;

        if( argc == -1 )
        {
                FlagLoadGame = TRUE ;
                LoadGameNumCube( ) ;
        }
        else
        {
                FadeToBlack( PtrPal ) ;
                Cls() ;
                BoxUpdate() ;
        }
}

/*──────────────────────────────────────────────────────────────────────────*/
void    GamePaused( U32 flags )
{
        T_INCRUST_DISP  *ptrdisp = NULL ;
        S32     n ;
        S32     endtimer ;
        S32     fin = FALSE ;

        SaveTimer() ;

        if( flags&PAUSE_CLOVER )
        {
                n = InitIncrustDisp( INCRUST_OBJ|INCRUST_YCLIP,
                                 BODY_3D_CLOVER,
                                 0, 0,
                                 0, 0, 3 ) ;

                if( n!=-1 )
                {
                        ptrdisp = &ListIncrustDisp[n] ;
                }

                PersoFlashTimer = TimerRefHR ;
        }

        if( flags&PAUSE_MESSAGE )
        {
                ColorFont( LBAWHITE ) ;
                Font( 5, ClipWindowYMax-33, "Pause" ) ;

                BoxMovingAdd( 5,ClipWindowYMax-33,100,ClipWindowYMax ) ;
                BoxBlit() ;
        }

        InitWaitNoInput( ALL_INPUTS ) ;
        InitWaitNoKey() ;

        MyGetInput() ;

        endtimer = TimerRefHR+1000*5    ;       // Wait 5 sec

        // Magouille pour ne pas avoir la gestion des extras, pof ...
        FlagAnimWhoSpeak = TRUE         ;

        while( !fin )
        {
                ManageTime() ;

                if( ptrdisp )
                {
                        ptrdisp->TimerEnd = TimerRefHR + 3*1000 ; // 3 s
                        AffScene( AFF_OBJETS_NO_FLIP ) ;
                        BoxUpdate() ;
                }

                MyGetInput() ;

                if( MyKey OR Input )    fin = TRUE ;

                if( flags&PAUSE_CLOVER AND TimerRefHR>endtimer )        fin = TRUE ;
        }

        FlagAnimWhoSpeak = FALSE        ;

        RestoreTimer() ;

        if( ptrdisp )
        {
                ptrdisp->TimerEnd = TimerRefHR + 1000 ;
        }

	PersoFlashTimer = 0 ;

	ClearWaitNoInput() ;
}

/*══════════════════════════════════════════════════════════════════════════*
             █▄ ▄█ █▀▀▀█  █    ██▄ █       █     █▀▀▀█ █▀▀▀█ █▀▀▀█
             ██▀ █ ██▀▀█  ██   ██▀██       ██    ██  █ ██  █ ██▀▀▀
             ▀▀  ▀ ▀▀  ▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

// en attendant les Input 64 bits (Watcom 11 ?)
U8      WaitNoKey32 = FALSE ;
U8      WaitNoKey33 = FALSE ;
U8      WaitNoKey34 = FALSE ;
U8      WaitNoKey35 = FALSE ;

/*──────────────────────────────────────────────────────────────────────────*/

S32     MainLoop()
{
        S32     xm, ym, zm ;
        T_OBJET *ptrobj ;
        U8      i ;
        S32     pansample ;

        SetDetailLevel() ;// prise en compte des différentes options

restartloop:

        FirstTime    = AFF_ALL_FLIP ;
        FlagLoadGame = FALSE ;

	InitWaitNoKey() ;
	ClearWaitNoInput() ;
        InitWaitNoInput( I_FIRE|I_MENUS|I_OPTIONS|I_SAVE|I_LOAD ) ;

/*-------------------------------------------------------------------------*/

        if( EscTimer )
        {
                SetTimerHR( EscTimer ) ;
                ResumeMusic( FALSE ) ;
        }

        EscTimer = 0 ;

/*-------------------------------------------------------------------------*/

        /* vitesse de chute propor pour tous les objets */
        InitMove( &RealFalling, -2560 ) ;
        /* vitesse de decalage propor tapis roulant */
        InitMove( &RealShifting, 640  ) ;

        /* frequence de paning des samples always */
        InitMove( &SampleAlwaysMove, 2  ) ;

/*-------------------------------------------------------------------------*/

        FirstSave = TRUE ;

        while( TRUE )
        {
startloop:

/*#ifdef  THOMAS
		if( DefKeys[0].Key1==0 )
		{
			PalOne( LBAWHITE, 255, 255, 255 ) ;
			CoulText( LBAWHITE, 0 ) ;
			GraphPrintf( TRUE, 0, 0, "STOP: Patch Inputs trouvé !!!", TRUE ) ;
			while( Key ) ;
			while( !Key ) ;
		}
#endif
*/
                ptrobj = &ListObjet[NUM_PERSO] ;

        // DEBUG pour trouver bug Alpha

#ifndef _WIN32
/*              if( ModeHercule )
                {
                        HStringf( 0, 0, "Alpha: %4d", ptrobj->Obj.Alpha ) ;
                        HStringf( 0, 1, "Beta : %4d", ptrobj->Obj.Beta ) ;
                        HStringf( 0, 2, "Gamma: %4d", ptrobj->Obj.Gamma ) ;
                }
*/
#endif


                // debug patch Gamma
                if( ptrobj->Obj.Alpha!=0
                OR  ptrobj->Obj.Gamma!=0 )
                {
                        Message( "STOP: 'j'ai trouvé le Patch_Alpha' !", TRUE ) ;
                }

/*-------------------------------------------------------------------------*/

                if( NewCube != -1 )
                {
#ifdef  DEMO
                        if( NewDemoSave!=CurrentDemoSave )
                        {
                                FlagChgCube = 0 ;

                                DemoLogo() ;

                                if( NewDemoSave>NB_DEMOSAVE-1 )
                                {
                                        return 1 ;
                                }
                                else
                                {
                                        SetDemoSaveGame( NewDemoSave ) ;

                                        StopMusic() ;

                                        InitGame( -1 ) ;
                                        ChangeCube() ;

                                        CopyScreen( Log, ScreenAux ) ;

                                        RestoreTimer() ;
                                        SaveTimer() ;
                                        AffScene( AFF_ALL_NO_FLIP ) ;
                                        FixeCinemaMode( FALSE ) ;
                                        CopyScreen( Log, Screen ) ;

                                        // au cas ou le mec meurre avant de faire <ESC>
                                        CurrentSaveGame() ;

                                        BoxReset() ;    // pas de flip

                                        ZoomSavedGame( 240, 10 ) ;

                                        FlagFade = FALSE ;
                                        RestartRainSample = TRUE ;
                                        HQ_ResumeSamples() ;

                                        ClearPlasmaMenu() ;

                                        RestoreTimer() ;

                                        goto restartloop ;
                                }
                        }
                        else
#endif
                        {
                                ChangeCube() ;
                        }

                        FirstLoop = TRUE ;      // premiere boucle d'un cube
                                                // ATTENTION: a verifier si
                                                // cela ne pose pas de probleme !
                        FlagReajustPosTwinsen = TRUE ;
                }

/*-------------------------------------------------------------------------*/
#ifdef  DEBUG_TOOLS
                UnsetClip() ;

                if ( CmptMemoTimerRef != 0 )
                {
                        CoulText( LBAWHITE, 0 ) ;
                        GraphPrintf( FALSE, 0, 10, "Warning, SaveTimer & RestoreTimer Decale!!!")       ;
                        GraphPrintf( FALSE, 0, 20, "Valeur CmptMemoTimerRef = %d", CmptMemoTimerRef );
                }
#endif


/*-------------------------------------------------------------------------*/

                // en attendant une methode propre de gestion de memoire
                // des samples !
                if( RestartRainSample )
                {
                        StartRainSample() ;
                        LaunchAmbiance()  ;
                }

/*-------------------------------------------------------------------------*/

#ifndef _WIN32
                CriticalErrorCount = 0  ;// Init Critical Error Count
#endif

                LastInput = Input       ;

                MyGetInput()            ;
                ManageTime() ;

                if( DemoSlide )
		{
			if( MyKey
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
                        AND MyKey!=K_T
#endif
                        )
                        {
                                HQ_StopSample() ;
                                SavingEnable = FALSE ;
                                return (MyKey!=K_ESC) ;
                        }

/*                      if( HigTrack>1 AND GetMusicCD()==-1 )
                        {
                                S32     num = MyRnd(HigTrack-1)+1 ;

                                PlayCdTrack( num ) ;
                        }
*/
                }

/*-------------------------------------------------------------------------*/
/* gestion clavier */   /* tools */

                // Faut-il le laisser ? (non, à mon avis !)
                if( DemoSlide AND MyKey == K_T )
                {
                        SetTimerHR( TimerRefHR+200 ) ;
                }

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
                if( MyKey == K_D )
                {
                        AffDebugMenu() ;
                }

                if( MyKey == K_F )
                {
                        DebugFps ^= 1 ;
                        InitWaitNoKey() ;
                }
#endif

#ifdef  DEBUG_TOOLS
                if( MyKey == K_T )
                {
                        SetTimerHR( TimerRefHR+200 ) ;
                }

                if( MyKey == K_Z )
                {
                        DrawZVObjets ^= 1 ;
                        InitWaitNoKey() ;
                }

                if( MyKey == K_E )
                {
                        FlagDrawHorizon ^= 1 ;
                        FirstTime = AFF_ALL_FLIP ;
                        while( MyKey )  MyGetInput() ;
                }

                if( MyKey == K_F12 )
                {
                        AsciiMode ^= 1 ;
                        while( MyKey )  MyGetInput() ;
                }

                if( MyKey == K_F10 )
                {
                        if( CubeMode==CUBE_EXTERIEUR )
                        {
                                S32     n ;
                                S32     lasttimer = TimerRefHR ;

                                SaveTimer() ;

                                CoulText( LBAWHITE, 0 ) ;

                                for( n=0; n<20; n++ )
                                {
                                        ManageTime() ;

                                        if( FlagDrawHorizon )
                                        {
                                                DrawHorizon2ZBuf() ;
                                        }

                                        // Doit Rajouter sa boite
                                        AffichageTerrainZBuf() ;
                                        GraphPrintf( FALSE, 0,0, "%d Drawn", n ) ;
                                        BoxStaticFullflip() ;
                                }

                                GraphPrintf( TRUE, 0, 10, "Time for 20 Loops: %d", TimerRefHR-lasttimer ) ;

                                while( MyKey )  MyGetInput() ;
                                while( !MyKey ) MyGetInput() ;

                                RestoreTimer() ;
                        }
                }

                if( MyKey == K_F11 )
                {
                        if( CubeMode==CUBE_EXTERIEUR )
                        {
                                S32     n ;
                                S32     lasttimer = TimerRefHR ;

                                SaveTimer() ;

                                CoulText( LBAWHITE, 0 ) ;

                                for( n=0; n<20; n++ )
                                {
                                        ManageTime() ;

                                        AffScene( AFF_ALL_NO_FLIP ) ;
                                        GraphPrintf( FALSE, 0,0, "%d Drawn", n ) ;
                                        BoxUpdate() ;
                                }

                                GraphPrintf( TRUE, 0, 10, "Time for 20 Loops: %d", TimerRefHR-lasttimer ) ;

                                while( MyKey )  MyGetInput() ;
                                while( !MyKey ) MyGetInput() ;

                                RestoreTimer() ;
                        }
                }

                if( MyKey == K_V )
                {
                        if( MagicBall != -1 )
                        {
                                ptrobj->Obj.X = ListExtra[MagicBall].PosX ;
                                ptrobj->Obj.Y = ListExtra[MagicBall].PosY ;
                                ptrobj->Obj.Z = ListExtra[MagicBall].PosZ ;
                        }
                }

                if( MyKey == K_M )
                {
                        SaveTimer() ;

                        AffPalette() ;

                        FirstTime = AFF_ALL_FLIP ;
                        RestoreTimer() ;
                }

                if( MyKey == K_K )
                {
                        ExtraBonus(     ptrobj->Obj.X,
                                        ptrobj->Obj.Y+2000,
                                        ptrobj->Obj.Z,
                                        (256*MUL_ANGLE), 0,
                                        6, 1 ) ;
                }

                if( MyKey == K_A )
                {
                        SaveTimer() ;
                        if( !FlagFade ) FadeToBlackAndSamples( PtrPal ) ;

                        FlagFade = TRUE ;

                        PlayAcf( "bu" ) ;

                        if( CubeMode==CUBE_INTERIEUR )
                        {
                                InitGrille( NumCube ) ;
                        }
                        RazListPartFlow() ;
                        ChoicePalette()   ;

                        FlagFade = TRUE ;
                        FirstTime = AFF_ALL_FLIP ;
                        RestoreTimer() ;
                }
#endif  // debug tools

                CheckSavePcx() ;// F9: Sauve Screen dans PATH_PCX_SAVE

/*-------------------------------------------------------------------------*/
/* gestion clavier */   /* game */

        if( !FlagFade )
        {
                if( Input&I_HELP )
                {
                        SaveTimer() ;
                        StopSpeak() ;
                        BackupScreen( FALSE ) ;
                        CopyScreen( Log, Screen ) ;
                        BoxReset() ;
                        InitDial( 0 ) ;

                        HQ_StopSample() ;
                        RestartRainSample = TRUE ;

                        InitPlasmaMenu() ;
//                      PauseMusic( TRUE ) ;

                        MenuConfig() ;

                        ClearPlasmaMenu() ;

                        InitDial( START_FILE_ISLAND+Island ) ;
                        PaletteSync( PtrPal ) ;
                        RestoreScreen() ;
//                      ResumeMusic( TRUE ) ;
                        RestoreTimer() ;
                }

                if( (MyKey == K_ESC OR Input&I_MENUS) )
                {
                        if( CinemaMode>1 )
                        {
                                ResetCinemaMode() ;
                        }
                        else
                        {
                                StopSpeak() ;
                                PauseMusic( FALSE ) ;
                                EscTimer = TimerRefHR ;

                                if( Comportement==C_PROTOPACK
                                OR  Comportement==C_JETPACK )
                                {
                                        // pour pouvoir restaurer anim marche protopack
                                        if( ptrobj->GenAnim==GEN_ANIM_MARCHE )
                                        {
                                                TimerProto = EscTimer+TEMPO_PROTO_AUTO ;// 1/2 s
                                        }
                                }

                                if( (ptrobj->LifePoint > 0)
                                AND (ptrobj->Obj.Body.Num != -1)
                                AND (!(ptrobj->Flags&INVISIBLE))
                                AND (NumCube!=NUM_CUBE_PHANTOM)
                                AND !DemoSlide
                                AND !PLAY_THE_END
                                )
                                {
                                        SavingEnable = TRUE ;
                                }
                                else
                                {
                                        SavingEnable = FALSE ;
                                }
                                break ;
                        }
                }

                if( Input&I_OPTIONS )   // options menu
                {
                        SaveTimer() ;
                        MemoClipWindow() ;
                        UnsetClipWindow() ;
                        GameOptionMenu[5] = 15 ; // retour au jeu

                        StopSpeak() ;
                        InitDial( 0 )   ;       //      SYS

                        InitPlasmaMenu() ;

                        PauseMusic( FALSE ) ;

                        OptionsMenu( FALSE ) ;

                        GameOptionMenu[5] = 26 ; // retour au menu precedent

                        InitDial( START_FILE_ISLAND+Island )    ;

                        ClearPlasmaMenu() ;
//                        CopyScreen( ScreenAux, Screen ) ;
//                      RestoreClipWindow() ;// fait dans OptionsMenu
//                        BoxStaticFullflip() ;
                        PaletteSync( PtrPal ) ;

                        ResumeMusic( FALSE ) ;

                        RestoreTimer() ;
                }

                if( Input & I_SAVE )             // Sauvegarde
                {
#if !defined(DEMO)||defined(DEBUG_TOOLS)
                if( (ptrobj->LifePoint > 0)
                AND (ptrobj->Obj.Body.Num != -1)
                AND (!(ptrobj->Flags&INVISIBLE))
                AND (NumCube!=NUM_CUBE_PHANTOM)
		AND !PLAY_THE_END
                )
                {
                        S32     savenewcube = NewCube ;

                        InitDial( 0 ) ;

                        SaveTimer() ;
                        MemoClipWindow() ;
                        UnsetClipWindow() ;
                        StopSpeak() ;
                        BackupScreen( TRUE ) ;
                        HQ_PauseSamples() ;
#ifdef  DEMO
                        SetDemoSaveGame( CurrentDemoSave ) ;
                        NumVersion = NUM_VERSION|SAVE_COMPRESS ;
                        CopyScreen( Screen, Log ) ;
                        SaveGame( TRUE ) ;
#else
                        InitPlasmaMenu() ;
                        if( ChoosePlayerName( 42, FALSE, TRUE ) )
                        {
                                NumVersion = NUM_VERSION|SAVE_COMPRESS ;
                                CopyScreen( Screen, Log ) ;
#ifdef  _WIN32
                                if( FileSize(OldGamePathname) ) Delete( OldGamePathname ) ;
#endif
                                SaveGame( TRUE ) ;
                        }
                        ClearPlasmaMenu() ;
#endif
                        RestoreClipWindow() ;
                        RestoreScreen() ;
                        NewCube = savenewcube ;
                        InitDial( START_FILE_ISLAND+Island ) ;
                        HQ_ResumeSamples() ;
                        PaletteSync( PtrPal ) ;
                        RestoreTimer() ;

                        while( MyKey OR Input ) MyGetInput() ;
                }
                else    PlayErrorSample() ;
#else   // DEMO
                PlayErrorSample() ;
#endif
                }

#ifndef DEMO
                if( Input & I_LOAD )    // Chargement
                {
                        S32     savenewcube = NewCube ;

                        InitDial( 0 ) ;

                        SaveTimer() ;
                        MemoClipWindow() ;
                        UnsetClipWindow() ;
                        StopSpeak() ;
                        BackupScreen( TRUE ) ;
                        HQ_PauseSamples() ;
                        InitPlasmaMenu() ;
                        if( ChoosePlayerName( 41, FALSE, FALSE ) )
                        {
                                RestoreTimer();

                                StopMusic() ;

                                InitGame( -1 ) ;
                                ChangeCube( ) ;

                                CopyScreen( Log, ScreenAux ) ;

                                RestoreTimer() ;
                                SaveTimer() ;
                                AffScene( AFF_ALL_NO_FLIP ) ;
                                FixeCinemaMode( FALSE ) ;
                                CopyScreen( Log, Screen ) ;

                                // au cas ou le mec meurre avant de faire <ESC>
                                CurrentSaveGame() ;

                                BoxReset() ;    // pas de flip

                                ZoomSavedGame( 240, 10 ) ;

                                FlagFade = FALSE ;
                                RestartRainSample = TRUE ;
                                HQ_ResumeSamples() ;

                                ClearPlasmaMenu() ;

                                RestoreTimer() ;

                                goto restartloop ;
                        }
                        else
                        {
                                // on charge pas de nouvelle partie
                                NewCube = savenewcube ;
                                ClearPlasmaMenu() ;
                                RestoreClipWindow() ;
                                RestoreScreen() ;
                                InitDial( START_FILE_ISLAND+Island ) ;
                                HQ_ResumeSamples() ;
                                PaletteSync( PtrPal ) ;
                                RestoreTimer() ;

                                while( MyKey OR Input ) MyGetInput() ;
                        }
                }
#endif

#ifdef  DEBUG_TOOLS
                if( (MyKey == K_G)              // Sauvegarde d'un bug
                AND (ptrobj->LifePoint > 0)
                AND (ptrobj->Obj.Body.Num != -1)
                AND (!(ptrobj->Flags&INVISIBLE))
                AND (NumCube!=NUM_CUBE_PHANTOM)
                )
                {
                        S32     savenewcube = NewCube ;

                        InitDial( 0 ) ;

                        SaveTimer() ;
                        strcpy( PathSave, PATH_SAVE_BUGS ) ;
                        MemoClipWindow() ;
                        UnsetClipWindow() ;
                        StopSpeak() ;
                        BackupScreen( TRUE ) ;
                        HQ_PauseSamples() ;
                        InitPlasmaMenu() ;
                        if( ChoosePlayerName( 42|0x40000000, FALSE, TRUE ) )
                        {
                                NumVersion = NUM_VERSION|SAVE_COMPRESS ;
                                CopyScreen( Screen, Log ) ;
                                SaveGame( TRUE ) ;
                        }
                        ClearPlasmaMenu() ;
                        RestoreScreen() ;
                        NewCube = savenewcube ;
                        InitDial( START_FILE_ISLAND+Island ) ;
                        RestoreClipWindow() ;
                        HQ_ResumeSamples() ;
                        PaletteSync( PtrPal ) ;
                        strcpy( PathSave, PATH_SAVE ) ;
                        RestoreTimer() ;

                        while( MyKey OR Input ) MyGetInput() ;
                }
#endif

                if( Input&I_PAUSE )
                {
                        SaveTimer() ;

                        if( Comportement==C_PROTOPACK
                        OR  Comportement==C_JETPACK )
                        {
                                // pour pouvoir restaurer anim marche protopack
                                if( ptrobj->GenAnim==GEN_ANIM_MARCHE )
                                {
                                        TimerProto = TimerRefHR+TEMPO_PROTO_AUTO ;// 1/2 s
                                }
                        }

                        StopSpeak() ;
                        HQ_PauseSamples() ;
                        PauseMusic( FALSE ) ;

			GamePaused( PAUSE_MESSAGE ) ;

                        ResumeMusic( FALSE ) ;
                        HQ_ResumeSamples() ;
                        RestoreTimer() ;
                }

                InventoryAction = -1 ;

                if( (Input&I_INVENTORY)
                AND (PtrComportement->Flags & CF_INVENTORY)
                AND (ptrobj->Obj.Body.Num != -1)
                AND !(ptrobj->WorkFlags&SKATING)
                AND (ptrobj->Move == MOVE_MANUAL) )
                {
                        SaveTimer() ;
                        StopSpeak() ;
                        BackupScreen( TRUE )            ;
                        MenuInventory( 0 ) ;

                        switch( InventoryAction )
                        {
                        case FLAG_HOLOMAP:      // holomap
                                if( PtrComportement->Flags & CF_HOLOMAP )
                                {
                                        HoloMap() ;
                                }
                                break ;

                        case FLAG_BALLE_MAGIQUE:// balle magique
                        case FLAG_DART:         // Flechette
                        case FLAG_SARBACANE:    // Sarbacane
                        case FLAG_GANT:         // Gant (raquette)
                        case FLAG_PISTOLASER:   // PistoLaser
                                Weapon = (U8)InventoryAction ;
                                if( Comportement>C_DISCRET )
                                {
                                        SetComportement( C_NORMAL ) ;
                                }
                                InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                break ;

                        case FLAG_SABRE:        // SABRE MAGIQUE
                                Weapon = (U8)InventoryAction ;
                                if( Comportement>C_DISCRET )
                                {
                                        SetComportement( C_NORMAL ) ;
                                }

                                // anim degaine sabre
                                InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                InitAnim( GEN_ANIM_DEGAINE, ANIM_THEN, NUM_PERSO ) ;
                                break ;

                        case FLAG_PROTOPACK:    // protopack
                                {
                                        U8      comp ;

                                        if( TabInv[FLAG_PROTOPACK].IdObj3D==1 )
                                                comp = C_JETPACK ;
                                        else    comp = C_PROTOPACK ;

                                        if( Comportement==comp )
                                                SetComportement( C_NORMAL ) ;
                                        else    SetComportement( comp ) ;
                                }
                                break ;

                        case FLAG_CONQUE:       // Conque
                                if( Comportement==C_CONQUE )
                                        SetComportement( C_NORMAL ) ;
                                else    SetComportement( C_CONQUE ) ;
                                break ;

                        case FLAG_MECA_PINGOUIN: // meca pingouin
                                ThrowMecaPingouin() ;
                                break ;

                        case FLAG_ARDOISE:
                                GereArdoise() ;
                                break ;

                        case FLAG_VISIONNEUSE:
                                PlayAllAcf() ;

                                if( CubeMode==CUBE_INTERIEUR )
                                {
                                        InitGrille( NumCube ) ;
                                }

                                RazListPartFlow() ;
                                ChoicePalette()   ;
                                FirstTime = AFF_ALL_FLIP ;
                                break ;

                        case FLAG_ANNEAU_FOUDRE:
                                if( Comportement>C_DISCRET )
                                {
                                        SetComportement( C_NORMAL ) ;
                                }

                                InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                InitAnim( GEN_ANIM_FOUDRE, ANIM_ALL_THEN, NUM_PERSO ) ;
                                break ;

                        case FLAG_PROTECTION:
                                ToggleSortProtection() ;
                                break ;

                        case FLAG_NOTE_BOURGMESTRE:
                                if( FirstTime!=AFF_ALL_FLIP )   RestoreScreen() ;

                                // init incdisp symbol utilisation
                                InitIncrustDisp( INCRUST_INVENTORY|INCRUST_YCLIP,
                                                 FLAG_NOTE_BOURGMESTRE,
                                                 0, 0,
                                                 0, 0, 3 ) ;

                                InitDial( 2 ) ; // FR_GAM.TXT
#ifdef  CDROM
                                NumObjSpeak = NUM_PERSO ;
#endif
                                NumObjDial  = NUM_PERSO ;
                                Dial( 800, TRUE ) ;
                                InitDial( START_FILE_ISLAND+Island ) ;
                                break ;
                        }
                        if( FirstTime!=AFF_ALL_FLIP )   RestoreScreen() ;
                        RestoreTimer()  ;
                }

                if( (Input & I_COMPORTEMENT)
                AND (ptrobj->Obj.Body.Num != -1)
                AND !(ptrobj->WorkFlags&SKATING)
                AND ((ptrobj->Move == MOVE_MANUAL)
                     OR (ptrobj->Move == MOVE_BUGGY_MANUAL))
                )
                {
                        MenuComportement( FALSE ) ;
                }

                // changement d'arme ou de comportement
                if( PtrComportement->Flags & CF_INVENTORY
                AND (ptrobj->Obj.Body.Num != -1)
                AND !(ptrobj->WorkFlags&SKATING)
                AND (ptrobj->Move == MOVE_MANUAL) )
                {
                        if( Input&I_WEAPON_1 )  // Balle magique
                        {
                                if( ListVarGame[FLAG_BALLE_MAGIQUE] )
                                {
                                        Weapon = FLAG_BALLE_MAGIQUE ;
                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }
                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                }
                                InitWaitNoInput( I_WEAPON_1 ) ;
                        }

                        if( Input&I_WEAPON_2 )  // Fléchettes
                        {
                                if( ListVarGame[FLAG_DART] )
                                {
                                        Weapon = FLAG_DART ;
                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }
                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                }
                                InitWaitNoInput( I_WEAPON_2 ) ;
                        }

                        if( Input&I_WEAPON_3 )  // SARBACANE/SARBATRON
                        {
                                if( ListVarGame[FLAG_SARBACANE] )
                                {
                                        Weapon = FLAG_SARBACANE ;
                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }
                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                }
                                InitWaitNoInput( I_WEAPON_3 ) ;
                        }

                        if( Input&I_WEAPON_4 )  // CONQUE
                        {
                                if( ListVarGame[FLAG_CONQUE] )
                                {
                                        if( Comportement==C_CONQUE )
                                                SetComportement( C_NORMAL ) ;
                                        else    SetComportement( C_CONQUE ) ;
                                }
                                InitWaitNoInput( I_WEAPON_4 ) ;
                        }

                        if( Input&I_WEAPON_5 )  // GANT
                        {
                                if( ListVarGame[FLAG_GANT] )
                                {
                                        Weapon = FLAG_GANT ;
                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }
                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                }
                                InitWaitNoInput( I_WEAPON_5 ) ;
                        }

                        if( Input&I_WEAPON_6 )  // PISTOLASER
                        {
                                if( ListVarGame[FLAG_PISTOLASER]
                                AND TabInv[FLAG_PISTOLASER].IdObj3D==2 )
                                {
                                        Weapon = FLAG_PISTOLASER ;
                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }
                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                }
                                InitWaitNoInput( I_WEAPON_6 ) ;
                        }

                        if( Input&I_WEAPON_7 )  // SABRE
                        {
                                if( ListVarGame[FLAG_SABRE] )
                                {
                                        S32     oldweapon = Weapon ;

                                        Weapon = FLAG_SABRE ;

                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }

                                        // anim degaine sabre
                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;

                                        if( oldweapon!=FLAG_SABRE )
                                        {
                                                InitAnim( GEN_ANIM_DEGAINE, ANIM_THEN, NUM_PERSO ) ;
                                        }
                                }
                                InitWaitNoInput( I_WEAPON_7 ) ;
                        }

                        if( CheckKey(DefKeys[33].Key1)    // ProtoPack/SuperJetPack
                        OR  CheckKey(DefKeys[33].Key2) )
                        {
                                if( !WaitNoKey33
                                AND ListVarGame[FLAG_PROTOPACK] )
                                {
                                        U8      comp ;

                                        if( TabInv[FLAG_PROTOPACK].IdObj3D==1 )
                                                comp = C_JETPACK ;
                                        else    comp = C_PROTOPACK ;

                                        if( Comportement==comp )
                                                SetComportement( C_NORMAL ) ;
                                        else    SetComportement( comp ) ;
                                }

                                WaitNoKey33 = TRUE ;
                        }
                        else    WaitNoKey33 = FALSE ;

                        if( CheckKey(DefKeys[32].Key1)    // NitroMecaPingouin
                        OR  CheckKey(DefKeys[32].Key2) )
                        {
                                if( !WaitNoKey32
                                AND ListVarGame[FLAG_MECA_PINGOUIN]>0 )
                                {
                                        SaveTimer() ;
                                        InventoryAction = FLAG_MECA_PINGOUIN ;
                                        ThrowMecaPingouin() ;
                                        RestoreTimer() ;
                                }

                                WaitNoKey32 = TRUE ;
                        }
                        else    WaitNoKey32 = FALSE ;

                        if( CheckKey(DefKeys[34].Key1)    // Sort de Protection
                        OR  CheckKey(DefKeys[34].Key2) )
                        {
                                if( !WaitNoKey34
                                AND ListVarGame[FLAG_PROTECTION]>0
                                AND MagicPoint>0 )
                                {
                                        SaveTimer() ;
                                        InventoryAction = FLAG_PROTECTION ;
                                        ToggleSortProtection() ;
                                        RestoreTimer() ;
                                }

                                WaitNoKey34 = TRUE ;
                        }
                        else    WaitNoKey34 = FALSE ;

                        if( CheckKey(DefKeys[35].Key1)    // Sort de Foudre
                        OR  CheckKey(DefKeys[35].Key2) )
                        {
                                if( !WaitNoKey35
                                AND ListVarGame[FLAG_ANNEAU_FOUDRE]>0
                                AND MagicPoint>=(MagicLevel*20) )
                                {
                                        SaveTimer() ;
                                        InventoryAction = FLAG_ANNEAU_FOUDRE ;
                                        if( Comportement>C_DISCRET )
                                        {
                                                SetComportement( C_NORMAL ) ;
                                        }

                                        InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
                                        InitAnim( GEN_ANIM_FOUDRE, ANIM_ALL_THEN, NUM_PERSO ) ;
                                        RestoreTimer() ;
                                }

                                WaitNoKey35 = TRUE ;
                        }
                        else    WaitNoKey35 = FALSE ;
                }

                if( (Input&(I_NORMAL|I_SPORTIF|I_AGRESSIF|I_DISCRET))
                AND (PtrComportement->Flags & CF_COMPORTEMENT)
                AND (ptrobj->Obj.Body.Num != -1)
                AND !(ptrobj->WorkFlags&SKATING)
                AND (ptrobj->Move == MOVE_MANUAL) )
                {
                        switch( Comportement )
                        {
                                case C_NORMAL:
                                case C_SPORTIF:
                                case C_AGRESSIF:
                                case C_DISCRET:
                                case C_PROTOPACK:
                                case C_CONQUE:
                                case C_JETPACK:
                                        SaveTimer() ;

                                        if( Input&I_NORMAL )            SetComportement(C_NORMAL) ;
                                        else if( Input&I_SPORTIF )      SetComportement(C_SPORTIF) ;
                                        else if( Input&I_AGRESSIF )     SetComportement(C_AGRESSIF) ;
                                        else if( Input&I_DISCRET )      SetComportement(C_DISCRET) ;

                                        IncrustComportement( Comportement ) ;
                                        RestoreTimer() ;
                                        break ;

                                case C_SCAPH_INT_NORM:
                                        if( Input&I_SPORTIF )
                                        {
                                                SaveTimer() ;
                                                SetComportement( (U8)C_SCAPH_INT_SPOR ) ;
                                                IncrustComportement( Comportement ) ;
                                                RestoreTimer() ;
                                        }
                                        break ;

                                case C_SCAPH_INT_SPOR:
                                        if( Input&I_NORMAL )
                                        {
                                                SaveTimer() ;
                                                SetComportement( (U8)C_SCAPH_INT_NORM ) ;
                                                IncrustComportement( Comportement ) ;
                                                RestoreTimer() ;
                                        }
                                        break ;

                                case C_SCAPH_EXT_NORM:
                                        if( Input&I_SPORTIF )
                                        {
                                                SaveTimer() ;
                                                SetComportement( (U8)C_SCAPH_EXT_SPOR ) ;
                                                IncrustComportement( Comportement ) ;
                                                RestoreTimer() ;
                                        }
                                        break ;

                                case C_SCAPH_EXT_SPOR:
                                        if( Input&I_NORMAL )
                                        {
                                                SaveTimer() ;
                                                SetComportement( (U8)C_SCAPH_EXT_NORM ) ;
                                                IncrustComportement( Comportement ) ;
                                                RestoreTimer() ;
                                        }
                                        break ;
                        }
                }

                if( (Input&I_HOLOMAP)
                AND (PtrComportement->Flags & (CF_HOLOMAP|CF_INVENTORY))
                AND (ListVarGame[FLAG_HOLOMAP] == 1)
                AND (ptrobj->Obj.Body.Num != -1)
                AND (ptrobj->Move==MOVE_MANUAL OR ptrobj->Move==MOVE_BUGGY_MANUAL)
                AND !CinemaMode )
                {
			SaveTimer() ;
			StopSpeak() ;
//                      PauseMusic( TRUE ) ;
//			BackupScreen( FALSE ) ;
			InitWaitNoInput( I_HOLOMAP|I_MENUS|I_RETURN|I_ACTION ) ;
			InitWaitNoKey() ;
			HoloMap() ;
//                      ResumeMusic( TRUE ) ;
//                        RestoreScreen() ;
			RestoreTimer() ;
		}

		if( Input & I_RETURN )  /* recentre sur perso */
                {
                        if( !CameraZone /* si pas camera forcée */
                        AND !CinemaMode )
                        {
                                SaveTimer() ;

                                Rotate( 0, DISTANCE_VISEE, ListObjet[NumObjFollow].Obj.Beta ) ;

                                StartXCube = (ListObjet[NumObjFollow].Obj.X+X0)/SIZE_BRICK_XZ ;
                                StartYCube = ((ListObjet[NumObjFollow].Obj.Y+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
                                StartZCube = (ListObjet[NumObjFollow].Obj.Z+Z0)/SIZE_BRICK_XZ ;

                                CameraCenter( 1 ) ;
                                FirstTime = AFF_ALL_FLIP ;

                                RestoreTimer() ;

#ifndef DEBUG_TOOL
                                // faut-il le laisser ?
                                InitWaitNoInput( I_RETURN ) ;
#endif
                        }
                }

#ifdef  DEBUG_TOOLS
                if( MyKey == K_B )
                {
                        if( Sample_Driver_Enabled )
                        {
                                if( (SamplesEnable ^= 1) == 0 )
                                {
                                        HQ_PauseSamples() ;
                                        Message( "Samples OFF", TRUE ) ;
                                }
                                else
                                {
                                        HQ_ResumeSamples() ;
                                        Message( "Samples ON", TRUE ) ;
                                }
                        }
                }
#endif

        } // if !flagfade

/*-------------------------------------------------------------------------*/

                /* vitesse de chute propor pour tous les objets */
                StepFalling = GetDeltaMove( &RealFalling ) ;
                /* vitesse de decalage propor pour tous les objets */
                StepShifting = GetDeltaMove( &RealShifting )    ;

                // doit-on repaner les samples always
                pansample = GetDeltaMove( &SampleAlwaysMove ) ;

/*-------------------------------------------------------------------------*/

                CameraZone = FALSE ;

                GereAmbiance() ;
                CheckNextMusic() ;

/*-------------------------------------------------------------------------*/
/* gere les objets */

                // asm
                ptrobj = ListObjet ;
                for( i=0; i<NbObjets; i++, ptrobj++ )
                {
                        ptrobj->HitBy = 255 ;
                }

                GereExtras() ;
                AnimAllFlow( ) ;        // gere les flows de particules

                if( CubeMode==CUBE_EXTERIEUR )
                {
                        if( TEMPETE_ACTIVE AND RainEnable )    GereRain() ;

                        DoAnimatedPolys() ;
                }

                ptrobj = ListObjet ;
                for( i=0; i<NbObjets; i++, ptrobj++ )
                {
                        if( ptrobj->WorkFlags & OBJ_DEAD )      continue ;

                        // test mort d'un objet

                        if( ptrobj->LifePoint == 0 )
                        {
                                switch( i )
                                {
                                        case NUM_PERSO:    // twinsen
                                                if( ListVarGame[FLAG_CLOVER]>0 )
                                                {
                                                        UseOneClover() ;
                                                }
                                                else
                                                {
                                                        InitAnim( GEN_ANIM_MORT, ANIM_FINAL, NUM_PERSO ) ;
                                                        ptrobj->Move = NO_MOVE ;
                                                }
                                                break ;

                                        case NUM_PINGOUIN:
                                                ExplodeMecaPingouin() ;
                                                break ;

                                        default:
                                                // sample specifique mort ?
                                                HQ_3D_MixSample( SAMPLE_DEAD, 0x1000, 2000, 1,
                                                        ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;

                                                // 'explosion' de l'objet ?
                                                if(  (ptrobj->OptionFlags & EXTRA_MASK)
                                                AND !(ptrobj->OptionFlags & EXTRA_GIVE_NOTHING) )
                                                {
                                                        GiveExtraBonus( ptrobj ) ;
                                                }
                                }
                        }

                        // Save Beta pour les objets carrier
                        // peut-etre pourrait-on éviter le test
                        if( ptrobj->CarryBy==-1 )       ptrobj->OldBeta = ptrobj->Obj.Beta ;

                        DoDir( i ) ;

                        ptrobj->OldPosX = ptrobj->Obj.X ;
                        ptrobj->OldPosY = ptrobj->Obj.Y ;
                        ptrobj->OldPosZ = ptrobj->Obj.Z ;

                        if( ptrobj->OffsetTrack != -1 )
                        {
                                DoTrack( i ) ;
                        }

                        if( !FirstLoop )        PtrDoAnim( i ) ;

                        if( ptrobj->Flags & CHECK_ZONE )
                        {
                                CheckZoneSce( ptrobj, i ) ;
                        }

                        // Samples Always
                        if( pansample AND ptrobj->SampleAlways )
                        {
                                ParmSampleVolume = ptrobj->SampleVolume ;

                                if( !(ptrobj->SampleAlways&0xFF000000)
                                OR  !IsSamplePlaying( ptrobj->SampleAlways ) )
                                {
                                        ptrobj->SampleAlways = HQ_3D_MixSample( (ptrobj->SampleAlways>>8)&0xFFFF,
                                                                0x1000, 0, 0,
                                                                ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;

                                }
                                else
                                {
                                        HQ_3D_ChangePanSample( ptrobj->SampleAlways,
                                                               ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
                                }
                        }

                        if( i==NUM_PERSO )
                        {
                                // Pour la prochaine boucle
                                ptrobj->WorkFlags &= ~(FLY_JETPACK|CHECK_FALLING) ;

                                CheckDartCol( ptrobj ) ;

                                if( ptrobj->Move==MOVE_BUGGY_MANUAL )
                                {
                                        // teste les zones giver pour le
                                        // buggy au point devant le buggy
                                        CheckBuggyZoneGiver( ptrobj ) ;
                                }

                                // pour la demo
                                if( DemoSlide ) ActionNormal = FALSE ;
                        }

                        if( ptrobj->OffsetLife != -1 )
                        {
                                DoLife( i ) ;
                        }

                        if( FlagTheEnd != -1 )  return FlagTheEnd ; // mmm violent

                        // si aprés la vie on à toujours 0 point de vie
                        // destruction definitive
                        if( ptrobj->LifePoint <= 0 )
                        {
                                if( i==NUM_PERSO )
                                {
                                        if( ptrobj->WorkFlags & ANIM_END )
                                        {
                                            if( ListVarGame[FLAG_CLOVER] > 0 )
                                            {
                                                UseOneClover() ;
                                                goto startloop ;
                                            }
                                            else        // game over
                                            {
                                                if( FirstTime==AFF_ALL_FLIP )
                                                {
                                                        AffScene( AFF_ALL_FLIP ) ;
                                                }

                                                GameOver() ;
                                                return 2 ;
                                            }
                                        }
                                }
                                else    // autre obj
                                {
                                        CheckCarrier( i ) ;
                                        ptrobj->WorkFlags |= OBJ_DEAD ;
                                        ptrobj->Obj.Body.Num = -1 ;
                                        ptrobj->ZoneSce = -1 ;
                                }
                        }

                        if( NewCube != -1 )
                        {
                                goto startloop ;
                        }
                }

/*-------------------------------------------------------------------------*/
/* recentre sur hero (numobjfollow) */

        if( !CameraZone )
        {
                if( !CinemaMode )
                {
                        // AlphaCam (touches '+' et '-' *)
                        GereExtKeys( MyKey ) ;

                        if( Input&I_CAMERA )
                        {
                                SaveTimer() ;
                                AddBetaCam = (AddBetaCam+1024)&4095 ;
                                CameraCenter( 2 ) ;
                                FirstTime = AFF_ALL_FLIP ;
                                InitWaitNoInput( I_CAMERA ) ;
                                RestoreTimer() ;
                        }
                }

                if( FirstTime==AFF_OBJETS_FLIP )
                {
                        T_OBJET *ptrobj ;

                        ptrobj = &ListObjet[ NumObjFollow ] ;

                        PtrProjectPoint(        ptrobj->Obj.X,
                                                ptrobj->Obj.Y,
                                                ptrobj->Obj.Z ) ;

                        if( Xp<80 OR Xp>539 OR Yp<(80+ClipWindowYMin) OR Yp>(ClipWindowYMax-50)/*429*/ )
                        {
                                SaveTimer() ;

                                if( CubeMode==CUBE_INTERIEUR )
                                {
                                        xm = (ptrobj->Obj.X)/SIZE_BRICK_XZ ;
                                        zm = (ptrobj->Obj.Z)/SIZE_BRICK_XZ ;

                                        StartXCube = xm + ((xm-StartXCube))/2;
                                        StartZCube = zm + ((zm-StartZCube))/2;
                                }
                                else
                                {
                                        Rotate( 0, DISTANCE_VISEE, ptrobj->Obj.Beta ) ;
                                        xm = (ptrobj->Obj.X+X0)/SIZE_BRICK_XZ ;
                                        zm = (ptrobj->Obj.Z+Z0)/SIZE_BRICK_XZ ;

                                        StartXCube = xm ;
                                        StartZCube = zm ;
                                }

                                ym =  ptrobj->Obj.Y/SIZE_BRICK_Y ;
                                StartYCube = ym ;

                                if( StartXCube >= SIZE_CUBE_X ) StartXCube = SIZE_CUBE_X - 1 ;
                                if( StartZCube >= SIZE_CUBE_Z ) StartZCube = SIZE_CUBE_Z - 1 ;

                                if( FlagCameraForcee )
                                {
                                        FlagCameraForcee = FALSE ;
                                        AlphaCam = DefAlphaCam[VueCamera] ;
                                        GammaCam = DEFAULT_GAMMA_CAM ;
                                        VueDistance = DefVueDistance[VueCamera] ;
                                }

                                CameraCenter( 0 ) ;
                                FirstTime = AFF_ALL_FLIP ;

                                RestoreTimer() ;
                        }
                }
        }

/*-------------------------------------------------------------------------*/
/* affiche tout */

		AffScene( FirstTime ) ;

		if( (FirstLoop OR RestartMusic)
		AND !PLAY_THE_END )
		{
                        // on attend une boucle scenarique au cas ou le
                        // scenar aurait lancé une musique
                        if( CubeJingle != 255 )
                        {
                                PlayMusic( CubeJingle, FALSE ) ;
			}

			RestartMusic = FALSE ;
                }

                FirstTime = AFF_OBJETS_FLIP ;
                FirstLoop = FALSE ;

                CmptFrame++ ;

#ifdef  DEBUG_TOOLS
                if( NbFramePerSecond < MinNbf ) MinNbf = NbFramePerSecond ;
                if( NbFramePerSecond > MaxNbf ) MaxNbf = NbFramePerSecond ;

                TotalNbf += NbFramePerSecond ;
                NbNbf++ ;
#endif
        }

        return 0 ;
}

/*══════════════════════════════════════════════════════════════════════════*
                      █▀▀▀▀ █▀▀▀█ ██▄ █ █▀▀▀▀  █    █▀▀▀▀
                      ██    ██  █ ██▀██ ██▀▀   ██   ██ ▀█
                      ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀     ▀▀   ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void    ReadConfigFile( void )
{
        char *ptr ;

        ReadInputConfig() ;     // Lit Config Clavier dans LBA2.CFG
        ReadVolumeSettings() ;  // Lit Volumes dans LBA2.CFG

        Version_US = DefFileBufferReadValue( "Version_US" ) ;
        NumVersion = (U8)DefFileBufferReadValue( "CompressSave" ) ;

        if( NumVersion )        NumVersion = SAVE_COMPRESS ;

        NumVersion |= NUM_VERSION ;

        // LastSave
        ptr = DefFileBufferReadString( "LastSave" ) ;
        if( ptr AND strcmp(ptr,"") )    strcpy( PlayerName, ptr ) ;

        Shadow = DefFileBufferReadValueDefault( "Shadow", 3 ) ;
        AllCameras = DefFileBufferReadValueDefault( "AllCameras", TRUE ) ;
        ReverseStereo = DefFileBufferReadValueDefault( "ReverseStereo", FALSE ) ;

        InverseStereoSample( ReverseStereo ) ;

        DetailLevel  = DefFileBufferReadValueDefault( "DetailLevel", MAX_DETAIL_LEVEL ) ;

        if( DetailLevel<0 )                     DetailLevel = 0 ;
        if( DetailLevel>MAX_DETAIL_LEVEL )      DetailLevel = MAX_DETAIL_LEVEL ;

        VideoFullScreen  = DefFileBufferReadValueDefault( "FullScreen", TRUE ) ;

        if( VideoFullScreen<0 OR VideoFullScreen>1 )    VideoFullScreen = TRUE ;

        ptr = DefFileBufferReadStringDefault( "FlagDisplayText", "ON" ) ;

        if( ptr AND !strcmpi(ptr,"OFF") )       FlagDisplayText = 0     ;
        else                                    FlagDisplayText = 1     ;// default

        // version distributeur
        DistribVersion = DefFileBufferReadValueDefault( "Version", UNKNOWN_VERSION ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    WriteConfigFile( void )
{
        DefFileBufferInit( PathConfigFile, ScreenAux, (640*480+RECOVER_AREA) ) ;

        WriteInputConfig() ;    // Sauve Config Clavier dans LBA2.CFG
        WriteVolumeSettings() ; // Sauve volumes dans LBA2.CFG

        DefFileBufferWriteString( "LastSave", PlayerName ) ;

        DefFileBufferWriteValue( "Shadow", Shadow ) ;
        DefFileBufferWriteValue( "AllCameras", AllCameras ) ;
        DefFileBufferWriteValue( "ReverseStereo", ReverseStereo ) ;
        DefFileBufferWriteValue( "DetailLevel", DetailLevel ) ;
        DefFileBufferWriteValue( "FullScreen", VideoFullScreen ) ;

        if( FlagDisplayText )   DefFileBufferWriteString( "FlagDisplayText", "ON" ) ;
        else                    DefFileBufferWriteString( "FlagDisplayText", "OFF" ) ;
}

/*══════════════════════════════════════════════════════════════════════════*
                            █▄ ▄█ █▀▀▀█  █    ██▄ █
                            ██▀ █ ██▀▀█  ██   ██▀██
                            ▀▀  ▀ ▀▀  ▀  ▀▀   ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

static  S32     End_Num = -1    ;
static  char    *End_Error      ;

/*══════════════════════════════════════════════════════════════════════════*/

// Erreur dans le HQRGet() qui appel cette fonction: on sort violemment

HQR_GET_CALLBACK        ErrorHQRGet ;

void    ErrorHQRGet( char *name, S32 index )
{
        char errstring[256] ;

        sprintf( errstring, "HQR_Get(%s): buffer trop petit (index %d)", name, index ) ;

        TheEnd( NOT_ENOUGH_MEM, errstring ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void    InitProgram()
{
        BoxInit( MAX_BOXES ) ;

        Cls() ;
        BoxUpdate() ;

        MemoClipWindow() ;      // pour caler MemoClipWindow... sur ClipWindow...

#ifdef  TRACE
        AsciiMode = TRUE        ;
#endif

#ifdef  CDROM
        InitVoiceFile() ;
#endif

        ReadConfigFile() ;

        // init path de sauvegardes
        strcpy( PathSave, PATH_SAVE ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void    TheEndCheckFile( char *filename )
{
        if( FileSize( filename) )       End_Num = NOT_ENOUGH_MEM ;
        else                            End_Num = ERROR_NOT_FOUND_FILE ;

        End_Error = filename ;

        exit( 0 ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void    InitTheEnd( S32 num, char *error )
{
        End_Num = num           ;
        End_Error = error       ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void    TheEnd( S32 num, char *error )
{
        End_Num = num           ;
        End_Error = error       ;

        exit(0) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
void    DebugHQR( char *titre, T_HQR_HEADER *hqr )
{
        LogPrintf( titre ) ;

        if( hqr )
        {
                LogPrintf( ": %7ld - used: %7ld - MaxIndex: %4ld - NbIndex: %4ld\n", hqr->MaxSize, hqr->MaxSize-hqr->FreeSize, hqr->MaxIndex, hqr->NbIndex ) ;
        }
        else
        {
                LogPrintf( ": xxxxxxx - used: xxxxxxx - MaxIndex: xxxx - NbIndex: xxxx\n" ) ;
        }
}
#endif


/*══════════════════════════════════════════════════════════════════════════*/

// Trouver le moyen de cleaner cette @#&? de fonction !!!!!!

void    V_ColorLine( U8 text, U8 back, U16 ncars )
{
#ifdef  _WIN32
        text = text ;
        back = back ;
        ncars = ncars ;
#else
        union REGS      regs ;

        regs.h.ah = 0x09 ;
        regs.h.bh = 0x00 ;
        regs.w.cx = ncars ;
        regs.h.al = ' ' ;
        regs.h.bl = (U8)((back<<4)|text) ;

        int386( 0x10, &regs, &regs ) ;
#endif
}

/*══════════════════════════════════════════════════════════════════════════*/
void    TheEndInfo()
{
        S32     err = FALSE ;

#ifdef  CDROM
        ClearVoiceFile()        ;
#endif
        StopMusic() ;

        LogPrintf( Version ) ;  // dans version.cpp

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
        LogPrintf( "* Start Extended Memory was %ld\n", MemoMemory ) ;
        LogPrintf( "* Start Dos Memory was %ld\n", MemoDosMemory ) ;
        LogPrintf( "* Min Dos Memory was %ld\n", MemoMinDosMemory ) ;
        LogPrintf( "* Size PtrZBuffer: %ld\n", ListMem[MEM_PTR_ZBUFFER].Size ) ;

        DebugHQR( "* HQR Sprite     ", HQRPtrSprite ) ;
        DebugHQR( "      Sprite raw ", HQRPtrSpriteRaw ) ;
        DebugHQR( "    Anim 3DS     ", HQRPtrAnim3DS ) ;
        DebugHQR( "      Sample     ", HQR_Samples ) ;
        DebugHQR( "        Anim     ", HQR_Anims ) ;
        DebugHQR( "        Body     ", HQR_Bodys ) ;
        DebugHQR( "      ObjFix     ", HQR_ObjFix ) ;
        DebugHQR( "  HQR_Isle_Obj   ", HQR_Isle_Obj ) ;
        DebugHQR( "  HQR_CubeInfos  ", HQR_CubeInfos ) ;
        DebugHQR( "  HQR_ListDecors ", HQR_ListDecors ) ;
        DebugHQR( "  HQR_MapPGround ", HQR_MapPGround ) ;
        DebugHQR( "  HQR_ListTexDef ", HQR_ListTexDef ) ;
        DebugHQR( "  HQR_MapSommetY ", HQR_MapSommetY ) ;
        DebugHQR( "  HQR_MapIntens  ", HQR_MapIntensity ) ;

        LogPrintf( "* End Extended Memory was %ld\n", MemEnd ) ;
        LogPrintf( "* Total Extended Memory used: %ld\n", MemoMemory-MemEnd) ;

        LogPrintf( "\nNbTexEffects= %d\n", NbTexEffects ) ;
        if( PtrPolySea )        LogPrintf( "Animated Sea Enable.\n" ) ;
        else                    LogPrintf( "Animated Sea Disable.\n" ) ;
#endif

        LogPrintf( "\n" ) ;


        switch( End_Num )
        {
                case ERROR_NOT_FOUND_FILE:
                        V_ColorLine( 14, 0, 130 ) ;
                        LogPrintf( "*** ERROR-> File not found: %s\n\n", End_Error ) ;
                        V_ColorLine( 7, 0, 130 ) ;
                        break ;

                case NAME_NOT_FOUND:
                        V_ColorLine( 14, 0, 130 ) ;
                        LogPrintf( "*** ERROR-> Critical error: ident name not found: %s\n\n", End_Error ) ;
                        V_ColorLine( 7, 0, 130 ) ;
                        break ;

                case NOT_ENOUGH_MEM:
                        V_ColorLine( 14, 0, 130 ) ;
                        LogPrintf( "*** ERROR-> Not Enough Memory: %s (SEE README.TXT)\n\n", End_Error ) ;
                        V_ColorLine( 7, 0, 130 ) ;
                        break ;

                case PROGRAM_OK:
#ifdef  DEBUG_TOOLS
                        if( NbNbf )
                        {
                                LogPrintf( "* Frame speed status:\n" ) ;
                                LogPrintf( "  Maximal frame rate occured: %d\n", MaxNbf ) ;
                                LogPrintf( "  Average frame rate was %d\n", TotalNbf/NbNbf ) ;
                                LogPrintf( "%s\n", End_Error ) ;
                        }
#endif
                        LogPrintf( "%s\n", End_Error )  ;
                        LogPrintf( "\nOK.\n" ) ;
                        break ;
        }

        WriteConfigFile() ;

        Clear3dExt() ;
}

/*══════════════════════════════════════════════════════════════════════════*/

#if defined(DEBUG_TOOLS) || defined(TEST_TOOLS)
void    Message( char *mess, S32 flag )
{
        S32     x ;

        CoulText( LBAWHITE, 0 ) ;
        PalOne( LBAWHITE, 255, 255, 255 ) ;
        x = (strlen(mess)*8)/2 ;
        MemoClip() ;

        GraphPrintf( FALSE, 320-x,236, "%s", mess ) ;
        Rect( 320-x-4, 234, 320+x+4, 248, LBAWHITE ) ;

        BoxStaticAdd( 320-x-4, 234, 320+x+4, 248 ) ;

        if( flag )
        {
                BoxUpdate() ;
                WaitNoInput() ;
                WaitInput() ;
        }
        RestoreClip() ;
}
#else
void    Message(  char *mess, S32 flag )
{
        // pour virer warnings !!!! Baaahhhh ! Que j'aime pas ça, c'est pas
        // beau !!! mais ça marche et ça ne génère pas de code !!!

        mess = mess ;
        flag = flag ;
}
#endif

/*══════════════════════════════════════════════════════════════════════════*/

#ifdef  _WIN32

void    ChangeProgDir()
{
        unsigned        nb                      ;
        char            name[_MAX_PATH]         ;

        GetModuleFileName(NULL, name, _MAX_PATH);

        for(nb=strlen(name); (name[nb]!='\\')&&(name[nb]!='/'); nb--)  ;

        name[nb+1]=0  ;

	if(!SetCurrentDirectory(name))
	{
		switch( DistribVersion )
		{
			case UNKNOWN_VERSION:
			case EA_VERSION:
				TheEnd( ERROR_NOT_FOUND_FILE, "lba2" ) ;
				break ;

			default:
				TheEnd( ERROR_NOT_FOUND_FILE, "Twinsen" ) ;
		}
	}
}
#else
void    ChangeProgDir( char *path )
{
        unsigned        nb                      ;
        char            name[_MAX_PATH]         ;

        char            drive[_MAX_DRIVE]       ;
        char            dir[_MAX_DIR]           ;

        _splitpath( path, drive, dir, NULL, NULL ) ;
        _makepath( name, drive, dir, NULL, NULL ) ;

        if( name[strlen(name)-1]=='\\' )
        {
                name[strlen(name)-1] = '\0' ;
        }

        _dos_setdrive( toupper(name[0])-'A'+1, &nb ) ;

        if( chdir( name )!=0 )  TheEnd( ERROR_NOT_FOUND_FILE, path ) ;
}
#endif

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

#ifdef  _WIN32
int PASCAL WinMain(     HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nCmdShow                   )
{
        S32     load = FALSE ;

        hInstance = hInstance ;
        hPrevInstance = hPrevInstance ;
        lpCmdLine = lpCmdLine ;
        nCmdShow = nCmdShow ;

#else //_WIN32
int main(S32 argc, char *argv[])
{
        S32     load = FALSE ;
        U8      *ptrdos4gw ;

        // protect DOS4GW car il a besoin d'un peu de mémoire pour ses transferts fichiers
        ptrdos4gw = (U8*)DosMalloc( 256*256L ) ;
#endif//_WIN32

#ifdef  DEBUG_TOOLS
        MemoMemory = AvailableMem() ;  // memory at start
        MemoDosMemory = DosAvailableMem() ;// Dos memory at start
#endif

#ifndef _WIN32
	atexit(TheEndInfo)      ;
	ChangeProgDir( argv[0] ) ;
#else
	ChangeProgDir() ;
#endif

	HQRGetErrorFunc = ErrorHQRGet ;

	InitMainBuffer()    ;       // Alloue gros buffer pour tous les pointeurs

	End_Error = PathConfigFile ;

#ifdef  DEMO
	#define APPNAME "LBA2 Twinsen's Odyssey Demo"
#else //DEMO
	#define APPNAME "LBA2"
#endif//DEMO
	#define ibuffer ScreenAux
	#define ibuffersize     (640*480+RECOVER_AREA)
	#define lname   "lba2.cfg"
	#define inits   INIT_DEFFILE|INIT_LOG|INIT_TIMER|INIT_SVGA|INIT_KEYB|INIT_SAMPLE|INIT_SMACKER
	#include <initadel.c>

#ifdef	_WIN32
	atexit(TheEndInfo)      ;
#endif

	End_Error = ""  ;

	// Passe en clavier "Splitté" (différenciation de chaque touche)
	AllowExtendedKeyb() ;

	InitProgram()   ;
	InitMemory()    ;       // Alloue petits buffers

#ifdef  ONE_GAME_DIRECTORY
	CreateLbaDirectories() ;
#endif

#ifndef _WIN32
	InstallCriticalErrorHandler() ;
#endif

//-------------------   buffer samples

	if( Sample_Driver_Enabled )
	{
		// déclare ressource samples buffer
		HQR_Samples = HQR_Init_Ressource(
				PATH_RESSOURCE"samples.hqr",
				SampleMem,
				SampleMem/5000 ) ;

		SamplesEnable = TRUE ;

		if( !HQR_Samples )      TheEndCheckFile( PATH_RESSOURCE"samples.hqr" ) ;

		// pour Windows
                HQR_Lock_Ressource( HQR_Samples ) ;
        }

//-------------------

        InitLanguage()  ;       // multilangue

        BufText = (U8 *)SmartMalloc( BIG_FILE_DIA ) ;
        if( !BufText )  TheEnd( NOT_ENOUGH_MEM, "BufText" ) ;

        BufOrder = (U16 *)SmartMalloc( BIG_FILE_ORD ) ;
        if( !BufOrder ) TheEnd( NOT_ENOUGH_MEM, "BufOrder" ) ;

        InitDial( 0 ) ;
	GetMultiText( 49, PleaseWait ) ;
	GetMultiText( 71, NewGameTxt ) ;

	// chargement palettes ici car le InitDial(2) ou le AskForCD()
        // peuvent en avoir besoin...
        InitPalette() ;
        ChoicePalette() ;

#ifdef  CDROM
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
        InitCD( "" )    ;
#else
        SetBlackPal() ;// pour etre sur ...

	{
	char	volname[10] ;
        S32     cd = TRUE ;


	switch( DistribVersion )
	{
		case UNKNOWN_VERSION:
		case EA_VERSION:
			strcpy( volname, "LBA2" ) ;
			strcpy( MessageNoCD, MESSAGE_NO_CD ) ;
			break ;

		default:
			strcpy( volname, "TWINSEN" ) ;
			strcpy( MessageNoCD, MESSAGE_NO_CD_US ) ;
			break ;
	}

	do
	{
#ifdef  _WIN32
		// pour Windows car ca prend du temps a scaner le CD
		Load_HQR( SCREEN_HQR_NAME, Log, PCR_CDROM ) ;
		BoxStaticFullflip() ;

		// premiere fois
		if( cd )        FadeToPal( PtrPalNormal ) ;
#endif

		if( !InitCD( volname ) )
                {
                        // premiere fois
                        if( cd )        PlayErrorSample() ;

                        if( !AskForCD() )
                        {
			     TheEnd( PROGRAM_OK, MessageNoCD ) ;
                        }

                        cd = FALSE ;
                }
                else    cd = TRUE  ;
        }
        while( !cd ) ;
        }
#ifdef  _WIN32
        FadeToBlack( PtrPalNormal ) ;
#endif

#endif

//-------------------   divers malloc
        BufMemoSeek = (U32 *)SmartMalloc( 2048L ) ;
        if( !BufMemoSeek )      TheEnd( NOT_ENOUGH_MEM, "BufMemoSeek" ) ;
#endif

        InitJingle() ;

        BufferAnim = (U8 *)SmartMalloc( SIZE_BUFFER_ANIM ) ;
        if( !BufferAnim ) TheEnd( NOT_ENOUGH_MEM, "Buffer Anim" ) ;
//-------------------

#ifndef _WIN32
#ifdef  DEBUG_TOOLS
        TestHercule() ;

        if( ModeHercule )
        {
                HCls() ;
                HBox( 10, 10, 70, 14, HINV, HBOX_DOUBLE ) ;
                HCentre( 10, "╣ LBA2 GAME INTEGRATED HERCULE DEBUGGER ╠" ) ;
                HCentreC( 12, HINV|HBLIND, "Please Wait while Initialising LBA2 ..." ) ;
        }
#endif
#endif

        LbaFont = (U8 *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_FONT_GPM ) ;
        if( !LbaFont )  TheEndCheckFile( RESS_HQR_NAME ) ;

        SetFont( LbaFont, 2, 8 )        ;
        ColorFont( 14 )                 ;

        InitAcf()       ;

#ifndef DEBUG_TOOLS
//-------------------   logo adeline (affiche)

        DistribLogo() ;

        Cls() ;
        BoxUpdate() ;

        if( !IsFirstGameLaunched() )
        {
                AdelineLogo() ;

                Cls() ;
                BoxUpdate() ;
        }

#ifdef  DEMO
        DemoBumper() ;

        Cls() ;
        BoxUpdate() ;
#endif
#else
        Cls() ;
#endif

        InitDial( 2 ) ; // on s'assure que FR_GAM.VOX soit sur le Disque Dur

        Init3DExt() ;

        // a Faire Imperativement avant LoadFicPerso() !!!!!!!!
        if( !Load_HQR( RESS_HQR_NAME, BufferFile3D, RESS_FILE3D ) )
                TheEndCheckFile( RESS_HQR_NAME ) ;

        LoadFicPerso()          ;
        InitObjects( BufferAnim, SIZE_BUFFER_ANIM, GivePtrBody, GivePtrAnim )   ;

//-------------------   load ressources diverses

        // Textures inventaire, médaillon, etc... (256x256)
        BufferTexture = (U8 *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_TEXTURES ) ;
        if( !BufferTexture )    TheEndCheckFile( RESS_HQR_NAME ) ;

        ScanTextureAnimation( BufferTexture, 256 ) ;
        InitTextureAnimation() ;        // pour initialiser l'affaire

        PtrZvExtra = (S16 *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_GOODIES_GPC ) ;
        if( !PtrZvExtra )       TheEndCheckFile( RESS_HQR_NAME ) ;

        PtrZvExtraRaw   = (S16 *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_GOODRAW_GPC ) ;
        if( !PtrZvExtraRaw )    TheEndCheckFile( RESS_HQR_NAME ) ;

#ifndef DEMO
        PtrZvAnim3DS    = (S16 *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_ANIM3DS_GPC ) ;
        if( !PtrZvAnim3DS )     TheEndCheckFile( RESS_HQR_NAME ) ;
#endif

//-------------------   buffer sprites

        HQRPtrSprite =  HQR_Init_Ressource(
                                PATH_RESSOURCE"sprites.hqr",
                                SpriteMem,
                                SpriteMem/1000 ) ;

        if( !HQRPtrSprite ) TheEndCheckFile( PATH_RESSOURCE"sprites.hqr" ) ;

        HQRPtrSpriteRaw =       HQR_Init_Ressource(
                                PATH_RESSOURCE"spriraw.hqr",
                                SpriteRawMem,
                                SpriteRawMem/100 ) ;

        if( !HQRPtrSpriteRaw ) TheEndCheckFile( PATH_RESSOURCE"spriraw.hqr" ) ;

//-------------------   Anims 3DS
#ifndef DEMO
        HQRPtrAnim3DS = HQR_Init_Ressource(
                                PATH_RESSOURCE"anim3ds.hqr",
                                Anim3DSMem,
                                100 ) ;

        if( !HQRPtrAnim3DS )    TheEndCheckFile( PATH_RESSOURCE"anim3ds.hqr" ) ;

        LoadListAnim3DS() ;
#endif
//-------------------   Pof
        Load_HQR( RESS_HQR_NAME, BufferPof, RESS_POF ) ;

//-------------------   buffer Impacts
        Load_HQR( RESS_HQR_NAME, BufferImpact, RESS_IMPACT ) ;

//-------------------   buffer animations

        HQR_Anims =     HQR_Init_Ressource(
                        PATH_RESSOURCE"Anim.hqr",
                        AnimMem,
                        AnimMem/800 ) ;

        if( !HQR_Anims )        TheEndCheckFile( PATH_RESSOURCE"Anim.hqr" ) ;

//-------------------   buffer bodys

        HQR_Bodys =     HQR_Init_Ressource(
                        PATH_RESSOURCE"Body.hqr",
                        BodyMem,
                        BodyMem/1000 ) ;

        if( !HQR_Bodys )        TheEndCheckFile( PATH_RESSOURCE"Body.hqr" ) ;

        HQR_ObjFix =    HQR_Init_Ressource(
                        PATH_RESSOURCE"ObjFix.hqr",
                        ObjFixMem,
                        ObjFixMem/1000 ) ;

        if( !HQR_ObjFix )       TheEndCheckFile( PATH_RESSOURCE"ObjFix.hqr" ) ;

//-------------------   buffer flows de particules

        InitPartFlow()  ;
        InitDarts()     ;
        InitRain()      ;
        InitSysText()   ;

        // doit etre la derniere allocation faite
        PtrPolySea = (S_ANI_POLY*)Malloc( SIZE_BUFFER_POLY_SEA ) ;
        if( !PtrPolySea )       MaxPolySea = 0 ;// Animation de l'eau non dispo

#ifndef _WIN32
        // on redonne un peu de mémoire à DOS4GW avant de lancer l'usine
        if( ptrdos4gw ) Free( ptrdos4gw ) ;
#endif

#ifdef  DEBUG_TOOLS
        MemoMinDosMemory = DosAvailableMem() ;// Dos memory after inits
        MemEnd = AvailableMem() ; // Extend memory at the end
#endif

#ifndef DEBUG_TOOLS
        // Pour virer warning !!!!!!
        ArgC = ArgC ;
#else
        // temporaire pour virer Warning
        ArgC = ArgC ;

        Palette( PtrPalBlack ) ;
#endif

//-------------------   main game menu

        FadeToBlack( PalettePcx ) ;

        // Pour un SlideShow sur un salon, il ne faut pas que les p'tits gars
        // puissent jouer avec le clavier !!!!!
/*      if( ArgC==2 AND !stricmp(ArgV[1],"/NOKEY") )
        {
                KeyOff = TRUE ;
                Key = Joy = Fire = Input = 0 ;
        }

//      SlideDemo(ArgC,ArgV) ;

*/
        if( ArgC>1 )
        {
#ifdef  DEMO
#ifdef  DEBUG_TOOLS
                load = atoi( ArgV[1] ) ;
#endif
#else
		LogPrintf( "\nTry to load %s\n", ArgV[1] ) ;

		if( FileSize(ArgV[1]) )
		{
			strcpy( GamePathname, ArgV[1] ) ;
			load = TRUE ;
		}
		else
		{
			strcpy( GamePathname, PathSave ) ;
			strcat( GamePathname, ArgV[1] ) ;

			if( FileSize(GamePathname) )
			{
				load = TRUE ;
			}
			else
			{
				strcat( GamePathname, ".LBA" ) ;

				if( FileSize(GamePathname) )
				{
					load = TRUE ;
				}
				else	strcpy( GamePathname, "" ) ;
			}
		}

		if( load )
		{
			Palette( PtrPal ) ;
		}
#endif
	}

#ifdef  DEBUG_TOOLS
	HERCULE( HCls() ) ;
#endif

        MainGameMenu( load ) ;

#ifdef  DEMO
        if( !FlagShowEndDemo )
        {
                SlideShow() ;
        }

        DemoBumper() ;
#endif

        TheEnd( PROGRAM_OK, "" ) ;

        return 0 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
