#include	"defines.h"

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			 Û฿฿฿Û Û฿฿฿฿ Û฿฿฿Û ÛÛ฿฿฿ Û฿฿฿Û
			 ÛÛ฿฿฿ ÛÛ฿฿  ÛÛ฿Û฿ ฿฿฿฿Û ÛÛ  Û
			 ฿฿    ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

/*--------- divers ---------------------------*/

UBYTE	*BufSpeak ;

UBYTE	*LbaFont ;
UBYTE	*Screen	;
UBYTE	PalettePcx[768+500] ;
WORD	MyJoy = 0 ;
WORD	MyFire = 0 ;
WORD	MyKey = 0 ;
UBYTE	*PtrPal ;
WORD	LastFire = 0 ;

LONG	FlagVsync = 0 ;
WORD	FlagMCGA = FALSE ;
LONG	FlagCredits = FALSE ;

UBYTE	String[256] ;
WORD	Value ;

LONG	SampleVolume, MusicVolume, CDVolume, LineVolume, MasterVolume ;

WORD	SamplesEnable = FALSE ;

WORD	FlagPalettePcx = FALSE ;
WORD	FlagFade = FALSE ;
WORD	Bulle = TRUE ;
WORD	FlagBlackPal = FALSE ;

#ifdef	CDROM
WORD	FlaFromCD = TRUE ;
#else
#ifdef	MAGOUILLE_FLA_HD
WORD	FlaFromCD = TRUE ;	// magouille fla sur HD
#else
WORD	FlaFromCD = FALSE ;
#endif
#endif

UBYTE	PathFla[_MAX_PATH] ;

LONG	Version_US = TRUE ;

/*--------- ressources ---------------------------*/

T_HQR_HEADER	*HQR_Samples ;
T_HQR_HEADER	*HQR_Midi ;
T_HQR_HEADER	*InventoryObj ;

UBYTE	*BufferShadow ;

/*--------- disk ---------------------------*/

char	PleaseWait[60] ;

UBYTE	ProgDrive[_MAX_DRIVE] ;
UBYTE	ProgDir[_MAX_DIR] ;

ULONG	OrgDrive ;
UBYTE	OrgDir[_MAX_DIR] ;

/*
UBYTE	Drive[_MAX_DRIVE] ;
UBYTE	Dir[_MAX_DIR] ;
*/
UBYTE	Name[_MAX_FNAME] ;
UBYTE	Ext[_MAX_EXT] ;


//UBYTE	OrgPathname[_MAX_PATH] ;
//UBYTE	ProgramPathname[_MAX_PATH] ;

//UBYTE	Pathname[_MAX_PATH] ;

/*--------- script ---------------------------*/

//UBYTE	FicName[_MAX_PATH] ;
//UBYTE	*PtrFiche ;
//UBYTE	*PtrCommand ;
//UBYTE	*PtrFicName ;

/*--------- life tools ---------------------------*/

//UBYTE	DefPathname[_MAX_PATH] ;
//UBYTE	ScePathname[_MAX_PATH] ;
//UBYTE	PcxPathname[_MAX_PATH] ;

T_ZONE	*ListZone ;
WORD	NbZones = 0 ;

T_TRACK	*ListBrickTrack ;
WORD	NbBrickTrack = 0 ;

/*
T_FLAG	ListFlagCube[MAX_FLAGS_CUBE] ;
T_FLAG	ListFlagGame[MAX_FLAGS_GAME] ;
*/

UBYTE	ListFlagCube[MAX_FLAGS_CUBE] ;
UBYTE	ListFlagGame[MAX_FLAGS_GAME] ;

UBYTE	ListFlagInventory[MAX_INVENTORY] ;

/*--------- holomap ----------------------------*/

//T_HOLO_OBJ	ListHoloObj[MAX_HOLO_OBJ] ;
//WORD		NbHoloObj = 0 ;

T_HOLO_POS	*ListHoloPos ; // [MAX_HOLO_POS] ;
UBYTE		TabHoloPos[MAX_HOLO_POS] ;
//UBYTE		*TabHoloPos ;

/*--------- menus -----------------------------*/

WORD	GameChoice ;
WORD	GameNbChoices ;
WORD	GameListChoice[10] ; // max 7 (normally)

/*--------- perso ----------------------------*/

WORD	FlagTheEnd = FALSE ;

WORD	ActionNormal = FALSE ;
WORD	InventoryAction = -1 ;
WORD	Weapon = 0 ;		// magicball

WORD	Comportement = C_NORMAL ;
//WORD	AnimRien = -1;
WORD	AnimRienNormal, AnimRienSportif, AnimRienAgressif, AnimRienDiscret, AnimRienProtopack ;

UBYTE	*PtrFile3dNormal ;
UBYTE	*PtrFile3dSportif ;
UBYTE	*PtrFile3dAgressif ;
UBYTE	*PtrFile3dDiscret ;
UBYTE	*PtrFile3dProtopack ;

WORD	SceZoom = 1 ;	// hard

WORD	Shadow = 2 ;	// full
WORD	ShadowX, ShadowY, ShadowZ ;
UBYTE	ShadowCol ;

WORD	NumHolomapTraj = -1 ;
WORD	NumCube = 0 ;			// num cube en cours
WORD	NewCube	= -1 ;			// change cube demand
WORD	FlagChgCube = 0 ;
WORD	NewPosX, NewPosY, NewPosZ ;
WORD	SceneStartX, SceneStartY, SceneStartZ ;
WORD	CubeStartX, CubeStartY, CubeStartZ ;
WORD	GameOverCube = 0 ;

WORD	MagicBall = -1 ;
WORD	MagicBallType = 1 ;
WORD	MagicBallCount = 3 ;
WORD	CombatAuto = TRUE ;

WORD	MagicLevel = 0 ;
WORD	MagicPoint = 0 ;
WORD	NbGoldPieces = 0 ;
WORD	NbLittleKeys = 0 ;
WORD	Chapitre = 0 ;
WORD	NbFourLeafClover = 2 ;
WORD	NbCloverBox = 2 ;

WORD	Island = 0	;
WORD	Fuel = 0 ;
WORD	NumPingouin = -1 ;

WORD		FlagClimbing = FALSE ;
WORD		StartYFalling = 0 ;
T_REAL_VALUE	RealFalling ;
WORD		StepFalling = 1 ;

WORD		FlagWater = FALSE ;

// perso sauvegarde entre cube

//UBYTE	SaveGenBody ;

//WORD	SaveCube ;
//WORD	SaveChapitre ;

WORD	SaveComportement = C_NORMAL ;
WORD	SaveBeta = 0 ;

//WORD	SaveLifePoint ;
//WORD	SaveNbGoldPieces ;
//WORD	SaveMagicLevel ;
//WORD	SaveMagicPoint ;

//WORD	SaveNbCloverBox ;
//WORD	SaveNbFourLeafClover ;

//UBYTE	SaveListFlagGame[MAX_FLAGS_GAME] ;

/*--------- Extra objs ----------------------------*/

T_EXTRA	ListExtra[MAX_EXTRAS] ;

//UBYTE		*PtrSpriteExtra ;
T_HQR_HEADER	*HQRPtrSpriteExtra ;
WORD		*PtrZvExtra ;

/*---------------- incrust display ----------------*/

T_INCRUST_DISP	ListIncrustDisp[MAX_INCRUST_DISP] ;

/*--------- moteur 3D ----------------------------*/

UBYTE	*BufferAnim ;
UBYTE	*PtrBufferAnim ;

LONG	NbBodys = 0 ;
UBYTE	*PtrBody[MAX_BODYS] ;
// T_HQR_HEADER	*HQR_Bodys ;

//LONG	NbAnims = 0 ;
//UBYTE	*PtrAnim[MAX_ANIMS] ;
T_HQR_HEADER	*HQR_Anims ;

LONG	NbObjets = 0 ;
T_OBJET	ListObjet[MAX_OBJETS] ;

T_SORT	ListTri[MAX_OBJETS+MAX_EXTRAS] ;

WORD	NumObjFollow = NUM_PERSO ;

/*--------- ambiance ----------------------------*/

WORD	SampleAmbiance[4] ;
WORD	SampleRepeat[4] ;
WORD	SampleRnd[4] ;
WORD	SamplePlayed = 0 ;
ULONG	TimerNextAmbiance = 0 ;
WORD	SecondEcart = 30 ;
WORD	SecondMin = 10 ;
WORD	CubeJingle = 255 ;
UBYTE	*PtrXmi = 0 ;
WORD	NumXmi = -2 ;

/*--------- brick iso ----------------------------*/

WORD	FirstTime = TRUE ;
WORD	CameraZone = FALSE ;
WORD	FlagAffGrille = TRUE ;






