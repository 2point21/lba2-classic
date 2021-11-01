#include	"c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
			 █▀▀▀█ █▀▀▀▀ █▀▀▀█ ██▀▀▀ █▀▀▀█
			 ██▀▀▀ ██▀▀  ██▀█▀ ▀▀▀▀█ ██  █
			 ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

/*--------- divers ---------------------------*/
#ifdef	ONE_GAME_DIRECTORY
S32	DebugFps = FALSE ;
#else
S32	DebugFps = TRUE ;
#endif

S32	DistribVersion = ACTIVISION_VERSION ;

char	MessageNoCD[50] ;

S32	DemoSlide = FALSE ;	// Mode démo non jouable
S32	RestartMusic = FALSE ;

U8	FlagRain = FALSE ;
U8	FlagDrawHorizon=TRUE;
U8	SavingEnable = TRUE ;
U8	FlagPlayAcf = FALSE ;
U8	RainEnable  = FALSE ;

#ifdef	DEMO
U8	FlagSlideShow = FALSE ;
#endif

U8	*BufSpeak ;
//U8	*BufTempo ;

U8	*LbaFont ;
U8	PalettePcx[768+500] ;
S32	MyKey = 0 ;
U8	*PtrPal ;
U8	*PtrPalNormal ;
U8	*PtrPalCurrent ;
U8	*PtrPalEclair  ;
U8	*PtrPalBlack   ;
U8	*PtrTransPal   ;
U8	*PtrXplPalette ;
XPL_HEADER	*PtrXplHeader ;

S32	IdxPalette = 0 ;	// numéro de palette dans le HQR
S32	LastIdxPalette = 0 ;	// numéro de la palette précédente dans le HQR

char	String[256] ;
S32	Value ;

//------------ Mode Cinema = 2 bandes noires de 40 pixels en haut et en bas de
//			     l'ecran

U8	CinemaMode = FALSE ;
S32	TimerCinema = 0 ;
S32	LastYCinema = 0 ;
S32	DebCycleCinema = 0 ;		// en pixels Y
S32	DureeCycleCinema = 500 ;	// en TimerRefHR

//---------------------------------------------------------------------------

S32	TimerProto ;

S32	SampleVolume = DEF_SAMPLE_VOLUME;
S32	VoiceVolume  = DEF_VOICE_VOLUME	;
S32	MasterVolume = DEF_MASTER_VOLUME;

U32	ParmSampleVolume    = 127 ; // a sauver dans les sauvegardes
U32	ParmSampleDecalage  = 0	  ;
U32	ParmSampleFrequence = 0	  ;

S32	SamplesEnable = FALSE ;

S32	DetailLevel = MAX_DETAIL_LEVEL	;

S32	VideoFullScreen = TRUE ;

//S32	FlagPalettePcx = FALSE ;
S32	FlagFade = FALSE ;
S32	FlagPal  = FALSE ;
S32	FadeMenu = FALSE ;
S32	FlagBlackPal = FALSE ;

S32	FlagShadeMenu = FALSE ;

#ifdef	DEBUG_TOOLS
S32	DrawZVObjets = FALSE ;
#endif

char	PathFla[_MAX_PATH] ;

S32	Version_US = TRUE ;

/*-------------------------- memoire ---------------------------*/

U32	SpriteMem	= MIN_SPRITES_MEM ;
U32	SpriteRawMem	= MIN_SPRITES_RAW_MEM ;
U32	Anim3DSMem	= MIN_ANIM_3DS_MEM ;
U32	SampleMem	= MIN_SAMPLES_MEM ;
U32	AnimMem		= MIN_ANIMS_MEM ;
U32	BodyMem		= MIN_BODYS_MEM ;
U32	ObjFixMem	= MIN_OBJFIX_MEM ;
U32	IsleObjMem	= MIN_ISLE_OBJ_MEM ;
U32	CubeInfosMem	= MIN_CUBE_INFOS_MEM ;
U32	ListDecorsMem	= MIN_LIST_DECORS_MEM ;
U32	MapPGroundMem	= MIN_MAP_PGROUND_MEM ;
U32	ListTexDefMem	= MIN_LIST_TEXDEF_MEM ;
U32	MapSommetYMem	= MIN_MAP_SOMMETY_MEM ;
U32	MapIntensityMem	= MIN_MAP_INTENSITY_MEM ;
U32	SCCMem = 0	;	// Calculé dynamiquement (contient le scenar)

/*--------- ressources ---------------------------*/

T_HQR_HEADER	*HQR_Samples ;

U8	*BufferTexture ;
U8	AnimateTexture = FALSE ;

/*--------- Ardoise ---------------------------*/
S8	ListArdoise[MAX_ARDOISE] = { -1, -1, -1, -1, -1 } ;

U8	CurrentArdoise = 0 ;
U8	NbArdoise = 0 ;

/*-------------------- Animations 3DS  ---------------------------*/

T_ANIM_3DS	*ListAnim3DS	;

/*--------- disk ---------------------------*/

char	PleaseWait[60] ;

//char	ProgDrive[_MAX_DRIVE] ;
//char	ProgDir[_MAX_DIR] ;

//unsigned	OrgDrive ;
//char	OrgDir[_MAX_DIR] ;

//char	Name[_MAX_FNAME] ;
//char	Ext[_MAX_EXT] ;


T_TRACK	*ListBrickTrack ;
S32	NbBrickTrack = 0 ;

/*--------- menus -----------------------------*/
U16	GameChoice ;
U16	GameNbChoices ;
S16	GameListChoice[10] ; // max 7 (normally)

/*--------- perso ----------------------------*/
S32	FlagTheEnd = FALSE ;

/*
S32	AnimRienNormal, AnimRienSportif, AnimRienAgressif, AnimRienDiscret, AnimRienProtopack ;

U8	*PtrFile3dNormal ;
U8	*PtrFile3dSportif ;
U8	*PtrFile3dAgressif ;
U8	*PtrFile3dDiscret ;
U8	*PtrFile3dProtopack ;
*/

extern	T_COMPORTEMENT	ListComportement[MAX_COMPORTEMENTS] ;

T_COMPORTEMENT	*PtrComportement = ListComportement ; // Normal

S32	Shadow = 3 ;	// full
S32	ShadowX, ShadowY, ShadowZ ;
U8	ShadowCol ;
U32	ShadowLevel = SHADOW_LEVEL ;

/*S32	ValidePosX, ValidePosY, ValidePosZ ;
S32	ValideCube ;
S32	ValideBeta ;
S16	ValideOffsetLife, ValideOffsetTrack ;
*/

S32	NumCube = 0 ;			// num cube en cours
S32	FlagChgCube = 0 ;
S32	FlagReajustPosTwinsen = TRUE ;	// Reajustement de twinsen lors d'un
					// changement de cube

S32	PhantomX = 0 ;	// offset du cube ext pour aller mourir
S32	PhantomY = 0 ;	// offset du cube ext pour aller mourir
S32	PhantomIsland ;

S32	FlagLoadGame = 0 ;
S32	NewPosX, NewPosY, NewPosZ ;
S32	CubeStartX, CubeStartY, CubeStartZ ;

S32	ExtraConque = -1 ;	// Numero extra visant Twinsen (avec Conque)

S32	AllCameras = TRUE ;
S32	ReverseStereo = FALSE ;
S32	Island = 0	;
S32	Planet = 0	;

S32	FlagWater = FALSE ;

S32	ScaleFactorSprite = DEF_SCALE_FACTOR ;

S32	SpriteX = 0 ;	// Coordonnees d'affichage des sprites
S32	SpriteY = 0 ;

U8	SaveComportement = C_NORMAL ;

/*--------- Extra objs ----------------------------*/
T_HQR_HEADER	*HQRPtrSprite ;
T_HQR_HEADER	*HQRPtrSpriteRaw ;
T_HQR_HEADER	*HQRPtrAnim3DS ;
S16	*PtrZvExtra ;
S16	*PtrZvExtraRaw ;
S16	*PtrZvAnim3DS	;

/*--------- moteur 3D ----------------------------*/
U8	*BufferAnim ;

T_HQR_HEADER	*HQR_Anims ;
T_HQR_HEADER	*HQR_Bodys ;
T_HQR_HEADER	*HQR_ObjFix ;

U8	*BufferPof ;
U8	*BufferFile3D ;

T_SORT	ListTri[MAX_TRI] ;

/*--------- ambiance ----------------------------*/
S16	SampleAmbiance[4] ;
S16	SampleRepeat[4] ;
S16	SampleRnd[4] ;
S16	SampleFreq[4] ;
S16	SampleVol[4] ;
S32	SamplePlayed = 0 ;
U32	TimerNextAmbiance = 0 ;
S32	SecondEcart = 30 ;
S32	SecondMin = 10 ;
S32	CubeJingle = 255 ;

/*--------- brick iso ----------------------------*/
S32	FirstTime = AFF_ALL_FLIP ;
S32	FirstLoop = FALSE ;
S32	FirstSave = FALSE ;

/*══════════════════════════════════════════════════════════════════════════*
	Liste des Globales sauvées dans les fichiers de sauvegarde .LBA
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
S32		NewCube	= -1 ;			// change cube demandé
U8		ListVarCube[MAX_VARS_CUBE] ;
S16		ListVarGame[MAX_VARS_GAME] ;
U8		Comportement = C_NORMAL ;
U8		SaveComportementHero = C_NORMAL ;
S32		NbGoldPieces = 0 ;
S32		NbZlitosPieces = 0 ;
U8		NumVersion = NUM_VERSION|SAVE_COMPRESS ;
U8		MagicLevel = 0 ;
U8		MagicPoint = 0 ;
U8		NbLittleKeys = 0 ;
S16		NbCloverBox = NB_CLOVER_BOX_DEFAULT ;
S32		SceneStartX, SceneStartY, SceneStartZ ;
U8		Weapon = FLAG_BALLE_MAGIQUE ;		// magicball
		// TimerRef en extern sauvé ici
//S32		LastFire = 0 ;
U8		Bulle = TRUE ;
U8		ActionNormal = FALSE ;
S32		InventoryAction = -1 ;
S32		MagicBall = -1 ;
U8		MagicBallType = 1 ;
U8		MagicBallCount = 3 ;
S32		MagicBallFlags = 0 ;
T_ZONE		*PtrZoneClimb ;	// Zone echelle dans laquelle se trouve Twinsen
U8		FlagClimbing = FALSE ;
S32		StartYFalling = 0 ;
MOVE		RealFalling ;
S32		StepFalling = 1 ;
MOVE		RealShifting ;
S32		StepShifting = 1 ;
U8		NumObjFollow = NUM_PERSO ;
U8		CameraZone = FALSE ;
S32		StartXCube = 0	;
S32		StartYCube = 0	;
S32		StartZCube = 0	;
T_DART		ListDart[MAX_DARTS] ;
S32		NbObjets = 0 ;
T_OBJET		ListObjet[MAX_OBJETS] ;
T_EXTRA		ListExtra[MAX_EXTRAS] ;
S32		NbZones = 0 ;
T_ZONE		*ListZone ;
T_INCRUST_DISP	ListIncrustDisp[MAX_INCRUST_DISP] ;
		// ListPartFlow en extern sauvé ici

MOVE	SampleAlwaysMove ;	// pour repaner les samples always de tps en tps

S32	FlagCameraForcee = FALSE ;
S32	ModeLabyrinthe = FALSE ;

S32	VueCamera = 0 ;

S32	DefVueDistance[NB_VUE_CAMERA] = { 10500, 17000 } ;
S32	DefAlphaCam[NB_VUE_CAMERA] = { 300, 530 } ;

T_TABALLCUBE	TabAllCube[256]	;//	A mettre en define
/*──────────────────────────────────────────────────────────────────────────*/

U8	NumObjDial = 0		;
U8	SaveBodyHero = GEN_BODY_NORMAL  ;

U32	Checksum = 0 ;

