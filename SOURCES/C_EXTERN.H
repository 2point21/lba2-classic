#include        "defines.h"

/*--------- divers ---------------------------------------------*/
extern	S32	BodyComportement;

extern	S32	DebugFps	;

extern	S32	DistribVersion	;

extern	char	MessageNoCD[50] ;

extern	U16	GameOptionMenu[] ;
extern	S32	FlecheForcee ;
extern  U16   	*BufOrder       ;
extern  U8   	*BufText        ;

extern	S32	DemoSlide 	;	// Mode démo non jouable
extern	S32	RestartMusic	;

extern	U8	FlagRain 	;
extern	U8	FlagDrawHorizon	;
extern	U8	SavingEnable 	;
extern	U8	FlagPlayAcf	;
extern	U8	RainEnable  	;

extern	char	NewGameTxt[20] 	;// ATTENTION: max 19 chars

#ifdef	DEMO
extern	U8	FlagSlideShow ;
#endif

extern "C" U8   *BufSpeak       ;

#pragma	aux	BufSpeak	"*"	;

extern  char   	PathSave[] ;
extern  char   	GamePathname[] ;
#ifdef	_WIN32
extern  char   	OldGamePathname[] ;
#endif
extern	char	PlayerName[MAX_SIZE_PLAYER_NAME+1] ;

extern	U8	*PtrScene	;

extern  U8   	*LbaFont ;
extern  U8   	PalettePcx[] 	;
extern  U8   	*PtrPal 	;
extern	U8	*PtrPalNormal 	;
extern	U8	*PtrPalCurrent 	;
extern	U8	*PtrPalEclair  	;
extern	U8	*PtrPalBlack  	;
extern	U8	*PtrTransPal  	;
extern	U8	*PtrXplPalette 	;
extern	XPL_HEADER	*PtrXplHeader ;

extern	S32	IdxPalette 	;	// numéro de palette dans le HQR
extern	S32	LastIdxPalette 	;	// numéro de la palette précédente dans le HQR

extern  S32    	MyKey ;

extern  S32	LastJoyFlag  ;
extern  S32	Jumping ;
extern  S32	Pushing ;
extern  S32	LastMyJoy  ;
extern  S32	LastMyFire ;

extern  char   	String[] ;
extern  S32    	Value ;

extern	S32	Nxw, Nyw, Nzw ;
extern	S32	SaveNxw, SaveNyw, SaveNzw ;
extern	S32	OldX, OldY, OldZ ;

extern	U8	AnimNumObj ;
extern	T_OBJET	*APtObj ;

//------------ Mode Cinema = 2 bandes noires de 40 pixels en haut et en bas de
//			     l'ecran

extern	U8	CinemaMode ;
extern	S32	TimerCinema ;
extern	S32	LastYCinema ;
extern	S32	DebCycleCinema ;	// en pixels Y
extern	S32	DureeCycleCinema ;	// en TimerRefHR

//---------------------------------------------------------------------------

extern	S32	TimerProto ;

extern  S32    	SampleVolume ;
extern  S32	VoiceVolume  ;
extern  S32	MasterVolume ;

extern  S32    	SamplesEnable ;

extern	U32	ParmSampleVolume    ; // a sauver dans les sauvegardes
extern	U32	ParmSampleDecalage  ;
extern	U32	ParmSampleFrequence ;

extern	S32	DetailLevel	;
extern	S32	VideoFullScreen ;

extern  S32    	FlecheForcee ;

extern  S32    	FlagPalettePcx ;
extern  S32    	FlagFade ;
extern	S32	FlagPal  ;
extern  S32    	FadeMenu ;
extern  S32    	FlagMessageShade ;
extern  S32    	FlagBlackPal ;

extern	S32	FlagShadeMenu ;

#ifdef	DEBUG_TOOLS
extern	S32	DrawZVObjets ;
#endif

extern  S32    	FlaFromCD ;
extern  char   	PathFla[] ;

extern  S32    	Version_US ;

/*-------------------------- memoire ---------------------------*/

extern	U32	SpriteMem ;
extern	U32	SpriteRawMem ;
extern	U32	Anim3DSMem ;
extern	U32	SampleMem ;
extern	U32	AnimMem	;
extern	U32	BodyMem	;
extern	U32	ObjFixMem ;
extern	U32	IsleObjMem ;
extern	U32	CubeInfosMem ;
extern	U32	ListDecorsMem ;
extern	U32	MapPGroundMem ;
extern	U32	ListTexDefMem ;
extern	U32	MapSommetYMem ;
extern	U32	MapIntensityMem ;
extern	U32	SCCMem ;	// Calculé dynamiquement

/*-------- RESSOURCES ---------------------------------------------*/

extern  T_HQR_HEADER    *HQR_Samples ;

/*-------- DISKFUNC.C ---------------------------------------------*/

extern  S32   	CubeMode ;
extern  S32   	LastCubeMode ;
extern	S32	ModeLabyrinthe ;

extern  char 	PleaseWait[] ;

//extern  char   	ProgDrive[] ;
//extern  char   	ProgDir[] ;

//extern  unsigned OrgDrive 	;
//extern  char   	 OrgDir[] ;

//extern  char   	Name[] ;
//extern  char   	Ext[] ;

/*-------- LIB_SVGA ---------------------------------------------*/
extern  S16    	NbPolyPoints ;

/*-------- life tool ---------------------------------------------*/
extern  T_TRACK *ListBrickTrack ;
extern  S32    	NbBrickTrack ;

/*--------- menus -----------------------------*/
extern  U16    GameChoice ;
extern  U16    GameNbChoices ;
extern  S16    GameListChoice[] ;

/*--------- perso ----------------------------*/
extern  S32    FlagTheEnd ;

extern	T_COMPORTEMENT	ListComportement[MAX_COMPORTEMENTS] ;
extern	T_COMPORTEMENT	*PtrComportement ; // Comportement courant

extern  S32    Shadow ;
extern  S32    ShadowX, ShadowY, ShadowZ ;
extern  U8     ShadowCol ;
extern	U32    ShadowLevel ;

extern  S32    FlagChgCube, NumCube ;
extern	S32    FlagReajustPosTwinsen ;	// Reajustement de twinsen lors d'un
					// changement de cube
extern	S32    PhantomX ;	// offset du cube ext pour aller mourir
extern	S32    PhantomY ;	// offset du cube ext pour aller mourir
extern	S32    PhantomIsland ;
extern	S32    FlagLoadGame ;
extern  S32    NewPosX, NewPosY, NewPosZ ;
extern  S32    CubeStartX, CubeStartY, CubeStartZ ;
extern  S32    Island  ;
extern  S32    Planet  ;

extern	S32    AllCameras ;
extern	S32    ReverseStereo ;

extern	S32    ScaleFactorSprite ;

extern	S32    SpriteX ;	// Coordonnees d'affichage des sprites
extern	S32    SpriteY ;

extern  S32    FlagWater ;

// perso sauvegarde entrée cube
extern  U8     SaveComportement ;

/*--------- Extra objs ----------------------------*/
extern  T_HQR_HEADER    *HQRPtrSprite ;
extern	T_HQR_HEADER	*HQRPtrSpriteRaw ;
extern	T_HQR_HEADER	*HQRPtrAnim3DS ;
extern	S16	*PtrZvExtra	;
extern	S16	*PtrZvExtraRaw	;
extern	S16	*PtrZvAnim3DS	;

/*-------- Moteur 3D ---------------------------------------------*/
extern	U8   *BufferTexture ;
extern	U8   AnimateTexture ;
extern  U8   *BufferAnim ;

/*--------- Ardoise ---------------------------*/
extern	S8	ListArdoise[MAX_ARDOISE]  ;

extern	U8	CurrentArdoise ;
extern	U8	NbArdoise ;

/*-------------------- Animations 3DS  ---------------------------*/

extern	T_ANIM_3DS	*ListAnim3DS	;


extern  T_HQR_HEADER    *HQR_Anims ;
extern  T_HQR_HEADER    *HQR_Bodys ;
extern  T_HQR_HEADER    *HQR_ObjFix ;

extern	U8	*BufferPof ;
extern	U8	*BufferFile3D ;

extern  T_SORT  ListTri[] ;

/*--------- ambiance ----------------------------*/
extern  S16    SampleAmbiance[] ;
extern  S16    SampleRepeat[] ;
extern  S16    SampleRnd[] ;
extern  S16    SampleFreq[] ;
extern  S16    SampleVol[] ;
extern  S32    SamplePlayed ;
extern  U32    TimerNextAmbiance ;
extern  S32    SecondEcart ;
extern  S32    SecondMin ;
extern  S32    CubeJingle ;

/*-------- Brick ISO ---------------------------------------------*/

#ifdef  BRICK_HQR
extern  U8   *BufferBrick    ;
#endif

extern  S32    FirstTime ;
extern	S32    FirstLoop ;
extern	S32    FirstSave ;

extern  S32    WorldXCube ;
extern  S32    WorldYCube ;
extern  S32    WorldZCube ;

extern  S32    XpOrgw ;
extern  S32    YpOrgw ;

/*--------- Message ---------------------------------------------*/
#ifdef	CDROM
extern  S32    NumObjSpeak ;
#endif
extern  S32    FlagSpeak ;

//-------------------- Message Chapter
//extern S16 MessChapitreTwinsen[MAX_CHAPTER][5] ;

//-------------------- ExtFunc.c
extern	U8	*PtrNuances ;	// Nuances gouraud

extern	U8	MagicHitForce[] ;
extern	U8	MagicBallHitForce[] ;
extern	U8	MagicBallSprite[] ;

/*══════════════════════════════════════════════════════════════════════════*
	Liste des Globales sauvées dans les fichiers de sauvegarde .LBA
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
extern	S32		NewCube ;	// change cube demandé
extern  U8   		ListVarCube[] ;
extern  S16    		ListVarGame[] ;
extern  U8    		Comportement ;
extern  U8    		SaveComportementHero ;
extern  S32    		NbGoldPieces ;
extern  S32    		NbZlitosPieces ;
extern	U8		NumVersion ;
extern  U8    		MagicLevel ;
extern  U8    		MagicPoint ;
extern  U8    		NbLittleKeys ;
extern  S16    		NbCloverBox ;
extern  S32    		SceneStartX, SceneStartY, SceneStartZ ;
extern  U8    		Weapon ;
			// TimerRef en extern sauvé ici
//extern  S32    		LastFire ;
extern  U8    		Bulle ;
extern  U8    		ActionNormal ;
extern  S32    		InventoryAction ;
extern  S32    		MagicBall ;
extern  U8    		MagicBallType ;
extern  U8    		MagicBallCount ;
extern	S32    		MagicBallFlags ;
extern	T_ZONE		*PtrZoneClimb ;	// Zone echelle dans laquelle se trouve Twinsen
extern  U8            	FlagClimbing ;
extern  S32            	StartYFalling ;
extern  MOVE		RealFalling ;
extern  S32            	StepFalling ;
extern	MOVE		RealShifting ;
extern	S32		StepShifting ;
extern  U8    		NumObjFollow ;
extern  U8    		CameraZone ;
extern  S32    		StartXCube ;
extern  S32    		StartYCube ;
extern  S32    		StartZCube ;
extern	T_INV_OBJ	TabInv[]     ;
extern	T_DART		ListDart[] ;
extern  S32    		NbObjets ;
extern  T_OBJET 	ListObjet[] ;
extern  T_EXTRA 	ListExtra[] ;
extern  S32    		NbZones ;
extern  T_ZONE  	*ListZone ;
extern  T_INCRUST_DISP  ListIncrustDisp[] ;
			// ListPartFlow en extern sauvé ici

extern	MOVE	SampleAlwaysMove ;	// pour repaner les samples always de tps en tps

extern	S32	FlagCameraForcee ;
extern	S32	AddBetaCam ;

extern	S32	VueCamera ;

extern	S32	DefVueDistance[NB_VUE_CAMERA] ;
extern	S32	DefAlphaCam[NB_VUE_CAMERA] ;

extern	S32	ExtraConque ;	// Numero extra visant Twinsen (avec Conque)
extern	U8	PingouinActif ;	// Pingouin en cours d'utilisation

extern	T_TABALLCUBE	TabAllCube[]	;
/*──────────────────────────────────────────────────────────────────────────*/

extern	U8	NumObjDial	;
extern	S32	DialNbLine 	;	// pour la version demo le 02/08/96
extern	U8	SaveBodyHero	;
extern	U32	Checksum ;
