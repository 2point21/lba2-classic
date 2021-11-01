/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▄ █▀▀▀▀ █▀▀▀▀  █    ██▄ █ █▀▀▀▀ ██▀▀▀
		   ██  █ ██▀▀  ██▀▀   ██   ██▀██ ██▀▀  ▀▀▀▀█
		   ▀▀▀▀  ▀▀▀▀▀ ▀▀     ▀▀   ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
// Cdrom Version  si pas define version disk
//#define	CDROM	1

// mettre ce define pour une version travail seulement
// en cdrom pas d'importance ATTENTION à supprimer pour version disk
// #define MAGOUILLE_FLA_HD	1

/*----------------*/

// demo 3 cubes
// #define	DEMO	1

// sauvegarde ecran/acceleration/infos
//#define	DEBUG_TOOLS	1


#define	START_SLIDESHOW_TEXT	900

#define	MESSAGE_NO_CD		"You need LBA2 CD, sorry!"
#define	MESSAGE_NO_CD_US	"You need Twinsen's Odyssey CD, sorry!"

#define	TestBugMem()	Free( Malloc( 20000L ) )

#define	FOREVER	for(;;)

// Détermine le nombre d'éléments d'un tableau Static
#define	NbTabIndex(tab)	(sizeof(tab)/sizeof(tab[0]))

#define	LoadFile3D(index)       (BufferFile3D + *( (U32*)BufferFile3D+(index) ) )

/*----------------*/

#define	MAX_ANGLE	4096
#define	MUL_ANGLE	4

// Brick, Gri et Bll dans .HQR
#define	BRICK_HQR	1
// samples dans .HQR
#define	SAMPLE_HQR	1
// samples des FLAs dans .HQR
#define	SAMPLE_FLA_HQR	1

//=======================================================================
// tous fichiers dans même dir
//#define	ONE_GAME_DIRECTORY	1
//=======================================================================

//path des ressources
#ifdef	ONE_GAME_DIRECTORY
#define	PATH_RESSOURCE		""
#define	PATH_SAVE		PATH_RESSOURCE"SAVE\\"
#else
#ifdef	DEMO
#define	PATH_RESSOURCE		"f:\\projet\\lba2\\demo\\"
#else
#define	PATH_RESSOURCE		"f:\\projet\\lba2\\game\\"
#endif
#define	PATH_SAVE		"c:\\adeline\\"
#endif

#ifdef	ONE_GAME_DIRECTORY
#define	PATH_PCX_SAVE		PATH_SAVE"shoot\\"
#else
#define	PATH_PCX_SAVE		"f:\\projet\\lba2\\graph\\shoot\\"
#endif

// repertoire ou sauver les bugs
#define	PATH_SAVE_BUGS		"f:\\panier\\lba2\\save\\bugs\\"

//#define	PATH_ACF	PATH_RESSOURCE"ACF\\"
#define	PATH_ACF	PATH_RESSOURCE"VIDEO\\VIDEO.HQR"


#ifdef	ONE_GAME_DIRECTORY
#define	PATH_JINGLE	"MUSIC\\"
#else
#define	PATH_JINGLE	"F:\\PROJET\\LBA2\\MUSIC\\WAV_LBA2\\"
#endif

/*──────────────────────────────────────────────────────────────────────────*/
/*----------------------------- camera -----------------------------*/

#define	NB_VUE_CAMERA	2

/*──────────────────────────────────────────────────────────────────────────*/
/*----------------------------- details ----------------------------*/

#define	MAX_DETAIL_LEVEL	3

/*──────────────────────────────────────────────────────────────────────────*/
/*----------------------------- Volumes ----------------------------*/

// a regler

#define	DEF_SAMPLE_VOLUME	97
#define	DEF_VOICE_VOLUME	112
#define	DEF_MASTER_VOLUME	127

/*──────────────────────────────────────────────────────────────────────────*/

#define	CD_TRACK_MENU		6	// theme LBA2
#define	CD_TRACK_CREDITS	2	// Musique Mosquibees

#define	JINGLE_TRACK_MENU       21

/*──────────────────────────────────────────────────────────────────────────*/
/*----------------------------- memoire -----------------------------*/

#define	MIN_SPRITES_MEM	 30000L
#define	MAX_SPRITES_MEM	 200000L

#define	MIN_SPRITES_RAW_MEM	 50000L

// 10000L quand la fonction HQR_Get() ne buggera plus !!!!!!!!!

#define	MAX_SPRITES_RAW_MEM	50000L

#define	MIN_ANIM_3DS_MEM	100000L
#define	MAX_ANIM_3DS_MEM	300000L

/*
#define	MIN_IMPACTS_MEM	 5000L
#define	MAX_IMPACTS_MEM	 10000L
*/

#define	MIN_SAMPLES_MEM	150000L
#define	MAX_SAMPLES_MEM	600000L	// 1000000L

#define	MIN_ANIMS_MEM	40000L
#define	MAX_ANIMS_MEM	400000L

#define	MIN_BODYS_MEM	110000L
#define	MAX_BODYS_MEM	300000L

#define	MIN_OBJFIX_MEM	50000L
#define	MAX_OBJFIX_MEM	300000L

/*
#define	MIN_FLOW_MEM	5000L
#define	MAX_FLOW_MEM	5000L

#define	MIN_POF_MEM	5000L
#define	MAX_POF_MEM	5000L
*/

//#define	MIN_ISLE_MEM	300000L
//#define	MAX_ISLE_MEM	1500000L	// 1000000L

#define	MIN_ISLE_OBJ_MEM	200000L// a determiner dynamiquement par mounfrac
#define	MAX_ISLE_OBJ_MEM	800000L

#define	MIN_CUBE_INFOS_MEM	100L
#define	MAX_CUBE_INFOS_MEM	1000L

#define	MIN_LIST_DECORS_MEM	20000L// a determiner dynamiquement par mounfrac
#define	MAX_LIST_DECORS_MEM	200000L

#define	MIN_MAP_PGROUND_MEM	(64*64*2*sizeof(T_HALF_POLY)) // 32768
#define	MAX_MAP_PGROUND_MEM	500000L

#define	MIN_LIST_TEXDEF_MEM	40000L// a determiner dynamiquement par mounfrac
#define	MAX_LIST_TEXDEF_MEM	200000L

#define	MIN_MAP_SOMMETY_MEM	(65*65*2)	// 8450
#define	MAX_MAP_SOMMETY_MEM	300000L

#define	MIN_MAP_INTENSITY_MEM	(65*65*2)
#define	MAX_MAP_INTENSITY_MEM	200000L

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

#ifndef	EDITLBA2

#include	<stdlib.h>
#include	<string.h>
#include	<dos.h>
#include	<conio.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<i86.h>
#include	<process.h>
#include	<limits.h>
#include	<direct.h>
#include	<stdarg.h>
#include	<time.h>

#include	<system.h>
#include	<svga.h>
#include	<3d.h>
#include	<object.h>
#include	<anim.h>
#include	<ail.h>
#include	<smacker.h>

#include	<fileio.h>

#ifdef	DEMO
#include 	"\projet\lib386\lib_cd\lib_cd.h"
#endif
/*
#ifdef	CDROM
#include 	"\projet\lib386\lib_cd\lib_cd.h"
#endif
*/
#endif	// EDITLBA2

#include 	"common.h"

// Puisqu'on n'inclue pas LIB_MEN3\LIB_MENU.H :
#ifdef		WHITE
#undef		WHITE
#endif

#ifdef		BLACK
#undef		BLACK
#endif

//#define		WHITE	63	// blanc dans la banque de gris
//#define		BLACK   240	// debut dernière banque
#define		WHITE		255
#define		LBAWHITE	15
#define		BLACK   	0


/*---------------- macros ------------------*/

// Pour DistribVersion
#define UNKNOWN_VERSION		0
#define	ACTIVISION_VERSION	1
#define	ACTIVISION_SUD_VERSION	2
#define	EA_VERSION		3
#define	VIRGIN_VERSION		4
#define	VIRGIN_ASIA_VERSION	5

// SaveGame
#define	SAVE_VERSION	4
// on stocke les sauvegardes dans BufSpeak
#define	MAX_PLAYER	((640*480)/(MAX_SIZE_PLAYER_NAME+1+sizeof(char*)))
#define	MAX_SIZE_PLAYER_NAME	100	// Calculé avec des ',' par Isabelle

#define	AUTOSAVE_NAME		"AUTOSAVE"
#define	AUTOSAVE_FILENAME	PATH_SAVE"autosave.lba"

#define	CURRENTSAVE_NAME	"CURRENT"
#define	CURRENTSAVE_FILENAME	PATH_SAVE"current.lba"

/*---------------- CONSTANTE ------------------*/

#define	SIZE_CUBE_X	64
#define	SIZE_CUBE_Y	25
#define	SIZE_CUBE_Z	64

#define	SIZE_BRICK_XZ	512
#define	SIZE_BRICK_Y	256
#define	DEMI_BRICK_XZ	256
#define	DEMI_BRICK_Y	128

/*---------------- FENETRE ------------------*/

#define	VIEW_X0		-50
#define	VIEW_Y0		-30
#define	VIEW_X1		680
#define	VIEW_Y1		580

/*──────────────────────────────────────────────────────────────────────────*/
/* type objet pour affscene */

#define	TYPE_OBJ_3D		(1024*0)
#define	TYPE_FLAG_RED		(1024*1)
#define	TYPE_FLAG_YELLOW	(1024*2)
#define	TYPE_SHADOW		(1024*3)
#define	TYPE_OBJ_SPRITE		(1024*4)
#define	TYPE_ZONE_DEC		(1024*5)
#define	TYPE_EXTRA		(1024*6)

/*──────────────────────────────────────────────────────────────────────────*/
/* pour les menus (GAMEMENU.CPP) */

#define	MENU_SHADE_LVL	       11	// Shade à l'interieur des menus
#define	SCREEN_SHADE_LVL	8	// Shade sous les menus

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

#define	MIN_S16	-32768
#define	MAX_S16	 32767

#define	BODY_3D_PROTECT		62

/*──────────────────────────────────────────────────────────────────────────*//*──────────────────────────────────────────────────────────────────────────*/
#define	NO_BODY			255

#define	GEN_BODY_NORMAL		0
#define	GEN_BODY_TUNIQUE	1
#define	GEN_BODY_SABRE		2
#define	GEN_BODY_SARBACANE	3
#define	GEN_BODY_SARBATRON	4
#define	GEN_BODY_GANT		5
#define	GEN_BODY_PISTOLASER	6
#define	GEN_BODY_MAGE		7
#define	GEN_BODY_MAGE_SARBACANE	8
#define	GEN_BODY_FEU		9
#define	GEN_BODY_TUNIQUE_TIR	10
#define	GEN_BODY_MAGE_TIR	11
#define	GEN_BODY_LABYRINTHE	12

#define	NO_ANIM			255

#define	GEN_ANIM_RIEN		0
#define	GEN_ANIM_MARCHE		1
#define	GEN_ANIM_RECULE		2
#define	GEN_ANIM_GAUCHE		3
#define	GEN_ANIM_DROITE		4
#define	GEN_ANIM_ENCAISSE	5
#define	GEN_ANIM_CHOC		6
#define	GEN_ANIM_TOMBE		7
#define	GEN_ANIM_RECEPTION	8
#define	GEN_ANIM_RECEPTION_2	9
#define	GEN_ANIM_MORT		10
#define	GEN_ANIM_ACTION		11
#define	GEN_ANIM_MONTE		12
#define	GEN_ANIM_ECHELLE	13
#define	GEN_ANIM_SAUTE		14
#define	GEN_ANIM_LANCE		15
#define	GEN_ANIM_CACHE		16
#define	GEN_ANIM_COUP_1		17
#define	GEN_ANIM_COUP_2		18
#define	GEN_ANIM_COUP_3		19
#define	GEN_ANIM_TROUVE		20
#define	GEN_ANIM_NOYADE		21
#define	GEN_ANIM_CHOC2		22
#define	GEN_ANIM_SABRE		23
#define	GEN_ANIM_DEGAINE	24
#define	GEN_ANIM_SAUTE_GAUCHE	25
#define	GEN_ANIM_SAUTE_DROIT	26
#define	GEN_ANIM_POUSSE		27
#define	GEN_ANIM_PARLE		28
#define	GEN_ANIM_DART		29
#define	GEN_ANIM_DESCEND	30
#define	GEN_ANIM_ECHDESC	31
#define	GEN_ANIM_ARRIMAGE	32
#define	GEN_ANIM_SKATE		33
#define	GEN_ANIM_SKATEG		34
#define	GEN_ANIM_SARBACANE	35
#define	GEN_ANIM_GANT_DROIT	36
#define	GEN_ANIM_GANT_GAUCHE	37
#define	GEN_ANIM_PISTOLASER	38
#define	GEN_ANIM_FOUDRE		39
#define	GEN_ANIM_ESQUIVE_DROITE	40	// Esquive à droite
#define	GEN_ANIM_ESQUIVE_GAUCHE	41	// Esquive à gauche
#define	GEN_ANIM_ESQUIVE_AVANT	42	// Esquive avant
#define	GEN_ANIM_ESQUIVE_ARRIERE 43	// Esquive arriere
#define	GEN_ANIM_FEU		44
#define	GEN_ANIM_SARBATRON	45
#define	GEN_ANIM_GAZ		46
#define	GEN_ANIM_LABYRINTHE	47	// Mort dans le labyrinthe du dôme

#define	IMPACT_PINGOUIN		0	// index dans le HQR

/*──────────────────────────────────────────────────────────────────────────*/
//--------------- Types de sauts pour Twinsen ------------------------

#define	DO_NORMAL_JUMP	1
#define	DO_LEFT_JUMP	2
#define	DO_RIGHT_JUMP	4

typedef	struct {
	U8	Func ;
	U8	TypeAnswer ;
	S16	Value ;
	} T_EXE_SWITCH ;

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

// structure normalement alignée

typedef	struct
{
	U8	GenBody ;		// 0 à 254 mais je veux -1
	U8	Col ;			/* brick en collision (inutile?) */
	S16	SizeSHit ;		// Toujours carres

	U16	GenAnim ;
	U16	NextGenAnim ;

	S32	OldPosX ;		/* old pos world */
	S32	OldPosY ;
	S32	OldPosZ ;

	S32	Info ;		/* infos pour DoDir */
	S32	Info1 ;
	S32	Info2 ;
	S32	Info3 ;

	union 	{
		struct {
			S32	SHitX ;
			S32	SHitY ;			// Coups Super Hit
			S32	SHitZ ;			// Servent aussi pour les Anim3DS
			} SHit ;

		// ATTENTION: un PUSHABLE ne peut pas lancer de SUPER_HIT
		struct {
			S32	PushX ;
			S32	PushY ;
			S32	PushZ ;
			} Push ;

		struct {
			S32	Num ;
			S32	Deb ;
			S32	Fin ;
			} A3DS ;
		} Coord ;

/* B*/	U8	HitBy ;			/* frappe par */
/* B*/	U8	HitForce ;		/* si !=0 force de frappe anim */
/* B*/	S16	LifePoint ;		/* point de vie en cours */
	S16	OptionFlags ;		/* flag d'init 2 */

	U8	*PtrAnimAction ;

	S16	Sprite ;
	S16	OffsetLabelTrack ;

// divers
	T_OBJ_3D Obj	;

	U8	*PtrFile3D ;
	S32	IndexFile3D ;

// constantes
/* B*/	S16	NbBonus ;		/* nb bonus to give */
/* B*/	U8	Armure ;		/* resistance */
	U8	CoulObj ;		/* couleur dominante de l'objet */

/* game infos */
	S16	XMin ;			/* ZV */
	S16	XMax ;
	S16	YMin ;
	S16	YMax ;
	S16	ZMin ;
	S16	ZMax ;

	S32	OldBeta ;		/* angle de la boucle precedente */


	BOUND_MOVE	BoundAngle ;	/* valeur real time de rotation */

	U8	*PtrTrack ;	/* ptr track prog */
	S16	OffsetTrack ;	/* offset dans la track */

	S16	SRot ;			/* vitesse de rotation */

	U8	*PtrLife ;

	S16	OffsetLife ;	/* offset dans la vie */
	U16	AnimDial   ;	// se trouve ici pour l'alignement

/* B*/	S16	CarryBy ;
/* B*/	U8	Move ;		/* type de deplacement */
/* B*/	U8	ObjCol ;		/* num obj en collision */

	S16	ZoneSce ;		/* zone declenchement scenarique */
/* B*/	S16	LabelTrack ;		/* dernier label de track */
/* B*/	S16	MemoLabelTrack ;	/* memo dernier label de track */
/* B*/  S16     MemoComportement ;	/* memo comportement life */

	U32	Flags ;			/* flags divers permanent */
//	U16	Flags2 ;		/* flags divers permanent */
	U32	WorkFlags ;		/* flags de gestion */

	S16	DoorWidth ;		// pour les DOOR

/* B*/	U8	FlagAnim ;		/* type d'anim en cours (dans flags?) */

	U8	CodeJeu ;		// brick spéciale sample ou action

	T_EXE_SWITCH	ExeSwitch ;

//	S16	MessageChapter[MAX_CHAPTER] ;

	T_ZONE	*PtrZoneRail ;

	S32	SampleAlways	;	// Handle du sample always en train de se jouer
	U8	SampleVolume	;
}	T_OBJET ;

typedef	struct
{
	S32	X ;
	S32	Y ;
	S32	Z ;		}	T_TRACK ;

typedef	struct
{
	U8	Type	;
	U8	Num	;

} T_TABALLCUBE	;

#ifndef	EDITLBA2

#include	"F:\PROJET\LBA2\TOOLS\PERSO\3DEXT\EXTERN.H"
#include	"PTRFUNC.H"
//#include	"OLDSORT.H"
#include	"SORT.H"
#include	"INVENT.H"
#include	"COMPORTE.H"
#include	"MESSAGE.H"
#include	"FUNC.H"
#include	"FLOW.H"
#include	"POF.H"
#include	"DART.H"
#include	"PLAYACF.H"
#include	"DEC_XCF.H"
#include	"ANIMTEX.H"
#include	"IMPACT.H"
#include	"HOLO.H"
#include	"BUGGY.H"
#include	"WAGON.H"
#include	"PATCH.H"
#include	"INPUT.H"
#include	"BEZIER.H"
#include	"INTEXT.H"
#include	"EXTFUNC.H"
#include	"RAIN.H"

#include	"DISKFUNC.H"
#include	"FICHE.H"
#include	"OBJECT.H"
#include	"PERSO.H"
#include	"GRILLE.H"
#include	"GERETRAK.H"
#include	"GERELIFE.H"
#include	"GAMEMENU.H"
#include	"FIRE.H"
#include	"AMBIANCE.H"
#include	"INCRUST.H"
#include	"EXTRA.H"
#include	"LZSS.H"
#include	"SAVEGAME.H"
#include	"ADDKEYS.H"	// définition de touches supplémentaires
#include	"MEM.H"
#include	"VALIDPOS.H"
#include	"CHEATCOD.H"
#include	"CONFIG.H"
#include	"CREDITS.H"
#include	"MUSIC.H"
#include	"ZV.H"
#include	"SCAN.H"
#include	"KEYB.H"
#ifdef	_WIN32
#include	"JOYSTICK.H"
#else
#include	"CRITICAL.H"
#include	"HERCULE.H"
#endif

#endif	// EDITLBA2

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/
