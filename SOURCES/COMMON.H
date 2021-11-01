#ifndef	COMMON_H
#define	COMMON_H

// j'en ai marre de planter sur des Rnd(0) !!!!!!!!!!!!!!!!!!!!!
// Méfie quand même à ne pas passer d'affectation ou d'incrémentation de
// pointeur !!!!!!!!!!!!
#define	MyRnd(n)	(n!=0?Rnd(n):0)

// protec piratage
#if defined(ONE_GAME_DIRECTORY)&&defined(CDROM)&&!defined(DEBUG_TOOLS)&&!defined(TEST_TOOLS)
#define	PIRACY_PROTECT()	if( !CDDrive )	TheEnd( PROGRAM_OK, MESSAGE_NO_CD ) ;
#else
#define	PIRACY_PROTECT()
#endif

#define	PLAY_THE_END	(ListVarGame[157]>0)








// Macro appelée lors d'une error
#ifdef	LBA_EDITOR
#define	PlayErrorSample()	HQ_MixSample( COMMON_NUM_DEF, SAMPLE_ERROR, 0x1000, 0, 1, 64, 127 )
#else
#define	PlayErrorSample()	if( !IsSamplePlaying(SAMPLE_ERROR) )	HQ_MixSample(SAMPLE_ERROR,0x1000,0,1,64,127)
#endif

// Macro pour lancer une fonction de la lib Hercule
#ifdef	_WIN32
#define	HERCULE( func )
#else
#define	HERCULE( func )	if( ModeHercule )	func
#endif

//#define Max3(x, y, z) __max(x, __max(y, z))
//#define Min3(x, y, z) __min(x, __min(y, z))

// Macro definissant si un caracteres est valide pour un nom de fichier (Windows)

#define	IsValidChar(c)	!strchr( "<>:\"|\\/", c )

/*---------------- MAX LENGTH OF A PATH ----------------------*/
#define	ADELINE_MAX_PATH	144	// compatible with 10.5 and WindowsNT

/*---------------- BUFFER_ANIM ----------------------*/
#define	SIZE_BUFFER_ANIM	5000L

/*---------------- BUFFERS TEXT ----------------------*/

#define	BIG_FILE_DIA	(40000L+RECOVER_AREA)	// Big .DIA
#define BIG_FILE_ORD	(1024L+RECOVER_AREA)	// Big .ORD

#define	CLIP_NEAR	3000

/*---------------- pour BoxUpdate() .... ----------------------*/
//
// A Reajuster !!!!!!!!!!!!!!!!!!
//

#define	MAX_RAIN	200
#define	MAX_BOXES	(MAX_OBJETS+MAX_FLOWS+MAX_EXTRAS+MAX_DARTS+MAX_RAIN)

/*---------------- Flags pour AffScene ------------------*/

#define	AFF_OBJETS_FLIP		0
#define	AFF_ALL_FLIP		1
#define	AFF_OBJETS_NO_FLIP	2
#define	AFF_ALL_NO_FLIP		3

/*--------------------- RESSOURCES -----------------------*/

#define	SIZE_XPL                (131884L+RECOVER_AREA)	// size des .xpl
#define	MAX_PALETTES		4
#define	MAX_ARDOISE		5
//#define	MAX_PCX			7

#define	PATH_PCX	"f:\\projet\\lba2\\graph\\capture\\"

#define	RESS_HQR_NAME		PATH_RESSOURCE"ress.hqr"
//#define	RESS_HQR_NAME		"f:\\projet\\lba2\\sourcesn\\ress\\ress.hqr"

#define	SCREEN_HQR_NAME		PATH_RESSOURCE"screen.hqr"
//#define	SCREEN_HQR_NAME		"f:\\projet\\lba2\\sourcesn\\ress\\screen.hqr"

#define	BKG_HQR_NAME		PATH_RESSOURCE"lba_bkg.hqr"

typedef	struct	{
	S32	Version ;

	S32	OffsetPalette ;
	S32	OffsetShade ;
	S32	OffsetFog ;
	S32	OffsetTransp ;

	S32	ShadeStartPercent ;
	S32	ShadeNormalLevel ;
	S32	ShadeEndPercent ;

	S32	FogCoul ;

	S32	TranspStartPercent ;
	S32	TranspEndPercent ;

		}	XPL_HEADER ;

/*---------------- RESSOURCE (RESS_HQR) ------------------*/

#define RESS_PAL                0
#define RESS_FONT_GPM           1
#define	RESS_EXT_SIZE_INFO     	2
//#empty      	;: 3
//#define RESS_INIT_PLASMA        4
#define RESS_GOODIES_GPC        5
#define	RESS_TEXTURES		6
#define RESS_GAME_OVER          7
#define RESS_GOODRAW_GPC        8
#define RESS_PAL_BLACK          9
#define	RESS_PAL_ECLAIR		10
#define	RESS_SKYSEA0		11	// Citadel
#define RESS_SKYSEA1		12	// Puits de Sendell
#define	RESS_SKYSEA2	        13	// desert
#define	RESS_SKYSEA3		14	// emeraude
#define	RESS_SKYSEA4		15	// otringal
#define	RESS_SKYSEA5		16	// celebrat
#define RESS_SKYSEA6		17	// Blafards/Plateforme
#define	RESS_SKYSEA7		18	// Mosquibees
#define	RESS_SKYSEA8		19	// Knartas
#define	RESS_SKYSEA9		20	// Ilôt CX
#define	RESS_SKYSEA10		21	// Ascenceur
//#empty	;: 22
//#empty	;: 23
//#empty	;: 24
//#empty	;: 25
#define	RESS_SKYSEA00		26	// citabau
#define	RESS_XPL0		27	// Palette Citadel
#define RESS_XPL1		28	// Palette Puits de Sendell
#define	RESS_XPL2	        29	// Palette desert
#define	RESS_XPL3		30	// Palette emeraude
#define	RESS_XPL4		31	// Palette otringal
#define	RESS_XPL5		32	// Palette celebrat
#define RESS_XPL6		33	// Palette Blafards/Plateforme
#define	RESS_XPL7		34	// Palette Mosquibees
#define	RESS_XPL8		35	// Palette Knartas
#define	RESS_XPL9		36	// Palette Ilôt CX
#define	RESS_XPL10		37	// Palette Ascenceur
//#empty	;: 38
//#empty	;: 39
//#empty	;: 40
//#empty	;: 41
#define	RESS_XPL00		42	// citabau
#define	RESS_ANIM3DS_GPC	43	// Zv des Anims 3DS
#define	RESS_FILE3D		44
#define	RESS_FLOW		45
#define	RESS_POF		46
#define	RESS_IMPACT		47
#define	RESS_ACFLIST		48

/*---------------- SCREEN RESSOURCE (SCREEN_HQR) ------------------*/

#define PCR_LOGO           	0
#define PCR_BUMPER 	    	2
#define PCR_MENU		4
#define	PCR_ARDOISE		6
#define	PCR_PEGOUT		8
#define	PCR_AEGOUT		10
#define	PCR_PLABYRT		12
#define	PCR_ALABYRT		14
#define	PCR_PLUNE		16
#define	PCR_ALUNE		18
#define	PCR_HACIENDA		20
#define	PCR_VUPHARE		22
#define	PCR_VUPHARE2		24
#define	PCR_CDROM		60
#define	PCR_ACTIVISION          72
#define	PCR_EA		        74
#define	PCR_VIRGIN	        76

/*----------------- SAUVEGARDES ----------------------*/

#define	NUM_VERSION		36	// (max 127)
#define	SAVE_COMPRESS		0x80
#define	MASK_NUM_VERSION	~(SAVE_COMPRESS)

/*----------------- Recupere pointeur Anim3DS -------------------*/

#define	GetPtrAnim3DS(num)	(U8 *)HQR_Get( HQRPtrAnim3DS, num )

/*----------------- WORLD ----------------------*/
#define	SHADOW_LEVEL	12

/*----------------- CUBE ----------------------*/

// Cube Vide servant juste à aller se noyer en extérieur
#define	NUM_CUBE_PHANTOM	94

#define SIZE_CUBE_X     64
#define SIZE_CUBE_Y     25
#define SIZE_CUBE_Z     64

#define SIZE_BRICK_XZ   512
#define SIZE_BRICK_Y    256
#define DEMI_BRICK_XZ   256
#define DEMI_BRICK_Y    128

#define VIEW_X0         -50
#define VIEW_Y0         -30
#define VIEW_X1         680
#define VIEW_Y1         580

// Type de cube
#define CUBE_INTERIEUR	0
#define	CUBE_EXTERIEUR	1

// Valeurs par defaut de la camera
#define	DEFAULT_GAMMA_CAM	0
#define	DEFAULT_ALPHA_CAM	341	// (30°)
#define	DEFAULT_DISTANCE_CAM	10500

// Flag Activant une zone
#define	ZONE_INIT_ON		1
#define	ZONE_ON                 2
#define	ZONE_ACTIVE		4
#define	ZONE_OBLIGATOIRE	8
// Flag Permettant de ne pas reajuster la position de Twinsen lors d'un
// changement de cube
#define	ZONE_DONT_REAJUST_POS_TWINSEN		2
// Zone de changement de cube extérieur devant attendre que Twinsen rentre
// en collision avec la porte
#define	ZONE_TEST_BRICK	2

#define	CARRY_BY_DECORS		1024

// Valeur saisie dans Animit
#define	DELTAY_ANIM_ECHELLE	1178

/*---------------- VAR_GAME ------------------*/

//	Inventaire

#define FLAG_HOLOMAP            0
#define FLAG_BALLE_MAGIQUE      1
#define	FLAG_DART		2	// Flechettes
#define FLAG_BOULE_SENDELL	3
#define FLAG_TUNIQUE            4
#define	FLAG_PERLE		5
#define FLAG_CLEF_PYRAMID	6
#define	FLAG_VOLANT		7
#define	FLAG_MONEY		8	// Kashes ou Zlitos
#define	FLAG_PISTOLASER		9	// PistoLaser
#define	FLAG_SABRE		10	// Le Sabre
#define	FLAG_GANT		11	// Gant
#define	FLAG_PROTOPACK		12
#define	FLAG_TICKET_FERRY	13
#define FLAG_MECA_PINGOUIN      14
#define	FLAG_GAZOGEM		15	// GazoGem
#define	FLAG_DEMI_MEDAILLON	16
#define	FLAG_ACIDE_GALLIQUE	17
#define	FLAG_CHANSON		18
#define	FLAG_ANNEAU_FOUDRE	19
#define	FLAG_PARAPLUIE		20
#define	FLAG_GEMME		21
#define	FLAG_CONQUE		22
#define	FLAG_SARBACANE		23
#define	FLAG_DISQUE_ROUTE	24	// Disque de Route
#define	FLAG_VISIONNEUSE	24	// Visionneuse ACFs
#define	FLAG_TART_LUCI		25
#define	FLAG_RADIO		26
#define	FLAG_FLEUR		27
#define	FLAG_ARDOISE		28
#define	FLAG_TRADUCTEUR		29
#define	FLAG_DIPLOME		30
#define	FLAG_DMKEY_KNARTA	31
#define	FLAG_DMKEY_SUP		32
#define	FLAG_DMKEY_MOSQUI	33
#define	FLAG_DMKEY_BLAFARD	34
#define	FLAG_CLE_REINE		35	// Clé Reine Mosquibies
#define	FLAG_PIOCHE		36
#define	FLAG_CLEF_BOURGMESTRE	37
#define	FLAG_NOTE_BOURGMESTRE	38
#define	FLAG_PROTECTION		39	// Sort de protection

// ATTENTION: Magouille car le scaphandre ne fait pas parti de l'inventaire
// donc on utilise un VAR_GAME scénarique
#define	FLAG_SCAPHANDRE		40

//	Autres

//#define	FLAG_TEMPETE		67

#define	FLAG_CELEBRATION	79
#define	FLAG_DINO_VOYAGE	94

#define	FLAG_ACF		235
#define	FLAG_ACF2		236
#define	FLAG_ACF3		237
#define	FLAG_ESC		249
#define FLAG_CLOVER             251
#define FLAG_VEHICULE_PRIS      252
#define FLAG_CHAPTER            253
#define FLAG_PLANETE_ESMER      254
#define	FLAG_DONT_USE		255 	// Pour l'inventaire

#ifdef	LBA_EDITOR
#define	TEMPETE_ACTIVE	(ListVarGame[FLAG_CHAPTER].Value<2 OR FlagRain)
#define	TEMPETE_FINIE	(ListVarGame[FLAG_CHAPTER].Value>=2 AND !FlagRain)
#else
#define	TEMPETE_ACTIVE	(ListVarGame[FLAG_CHAPTER]<2 OR FlagRain)
#define	TEMPETE_FINIE	(ListVarGame[FLAG_CHAPTER]>=2 AND !FlagRain)
#endif

#define	INV_MAX_VALUE	100	// Capacité des objets inventaires VALUE (conque)

#define	BODY_3D_CLOVER		60	// voir objfix.lst

//----- Inventory/FoundObj/IncrustObj Alpha/Beta Light

#define	OBJ_ALPHA_LIGHT	414
#define	OBJ_BETA_LIGHT	136

/*-------------- DEGATS DES ARMES -----------*/
#define	DEGATS_HAND_LVL_0	5
#define	DEGATS_HAND_LVL_1	8
#define	DEGATS_HAND_LVL_2	18
#define	DEGATS_HAND_LVL_3	28
#define	DEGATS_HAND_LVL_4	38
#define	DEGATS_BALLE_LVL_01	10
#define	DEGATS_BALLE_LVL_2	20
#define	DEGATS_BALLE_LVL_3	30
#define	DEGATS_BALLE_LVL_4	40
#define	DEGATS_DART		8
//#define	DEGATS_MECA_PINGOUIN	15
#define	DEGATS_SARBACANE	10
#define	DEGATS_SARBATRON	30
#define	DEGATS_FOUDRE           25
#define	DEGATS_PISTOLASER	20
#define DEGATS_GANT		45
#define	DEGATS_SABRE		35

/*---------------- TYPES DE FLOW ------------*/

#define	FLOW_MAGIC_BALL	0
#define	FLOW_PINGOUIN	18

/*---------------- SAMPLES ------------------*/

// Temps minimum en TimerSystemHR pour relancer un sample qui est déjà en
// train de se jouer
#define	SAMPLE_TIME_REPEAT	100	// 40


#define	SAMPLE_MAGIC_BALL		0
#define	SAMPLE_MAGIC_BALL_FIRE		1
#define SAMPLE_BONUS_TROUVE     	2
#define SAMPLE_BONUS            	3
#define	SAMPLE_ESCALATOR		4
#define	SAMPLE_PROTOPACK		5
#define	SAMPLE_FOUND_OBJ		6
#define	SAMPLE_CHOC_MAGIC_BALL		7
#define	SAMPLE_EXPLO_MAGIC_BALL		8
#define	SAMPLE_LOGO			9
#define	SAMPLE_CONQUE_VIDE		10
#define	SAMPLE_PINGOUIN_ACTIF		11
#define	SAMPLE_BUGGY_INVALIDE		11
#define	SAMPLE_EXPLO_MECA_PINGOUIN	12
#define	SAMPLE_RAIN			13
#define	SAMPLE_DEAD			14
#define	SAMPLE_GAME_OVER		15
#define	SAMPLE_PROTECTION		90
#define	SAMPLE_PROTECTION_ALARME	91
#define	SAMPLE_FLASH_PERSO		99
#define	SAMPLE_ERROR			11
#define	SAMPLE_FOUDRE_STEP2		102
#define	SAMPLE_FOUDRE_STEP3		100
#define	SAMPLE_BUGGY			109
#define	SAMPLE_VOICE_MENU		110

//---------------- VOLUMES des samples en dur

#define	VOLUME_RAIN			70
#define	VOLUME_FLASH_PERSO		127
#define	VOLUME_ESCALATOR		80	// tapis roulants

/*---------------- Angles des cadres (menus, messages,...) ---------------*/

#define	DRAW_SG		(1<<0)	// Angle Supérieur Gauche des cadres
#define	DRAW_IG		(1<<1)	// Angle Inférieur Gauche des cadres
#define	DRAW_ID		(1<<2)	// Angle Inférieur Droit des cadres
#define	DRAW_SD		(1<<3)	// Angle Supérieur Droit des cadres

#define	ARROW_SG	(1<<4)	// Flèche Supérieur Gauche des cadres
#define	ARROW_IG	(1<<5)	// Flèche Inférieur Gauche des cadres
#define	ARROW_ID	(1<<6)	// Flèche Inférieur Droit des cadres
#define	ARROW_SD	(1<<7)	// Flèche Supérieur Droit des cadres

#define	BACKUP_ANGLES	(1<<8)	// Backup puis Restore les pixels autour des angles

#define	CADRE_RED	(1<<16)	// Cadres rouges

#define	A_FLIP		(1<<31)	// Flip les boites dessinées

#define	ALL_ANGLES	(DRAW_SG|DRAW_IG|DRAW_ID|DRAW_SD)
#define	ALL_ARROWS	(ARROW_SG|ARROW_IG|ARROW_ID|ARROW_SD)

/*---------------- SPRITES SYSTEMES (GOODIES) ------------------*/

#define	SYS_SPRITE_SG	0	// Angle Supérieur Gauche des cadres
#define	SYS_SPRITE_IG	1	// Angle Inférieur Gauche des cadres
#define	SYS_SPRITE_ID	2	// Angle Inférieur Droit des cadres
#define	SYS_SPRITE_SD	3	// Angle Supérieur Droit des cadres

#define	SYS_SPRITE_INV	13	// Objet inventaire inutilisable
#define	SYS_SPRITE_BALD	14	// Vignette Radio Baldino
#define	SYS_SPRITE_ZOE	15	// Vignette Radio Zoe

/*---------------- SPRITES (GOODRAW) ------------------*/

#define DEF_SCALE_FACTOR 65536	// facteur de zoom pour sprites en 3d ext

#define	SPRITE_CLOVER_BOX	0
#define	SPRITE_FULL_CLOVER_BOX	1
#define	SPRITE_DART		2
#define SPRITE_COEUR    	4
#define SPRITE_MAGIE    	5
#define SPRITE_CLE      	6
#define SPRITE_CLOVER   	7

#define	SPRITE_BALLE_LVL_01	8
#define	SPRITE_BALLE_LVL_2	9
#define	SPRITE_BALLE_LVL_3	10
#define	SPRITE_BALLE_LVL_4	11
#define	SPRITE_BALLE_SHADOW	12
#define	SPRITE_BALLE_SHADOW_2	13
#define	SPRITE_BALLE_SHADOW_3	14
#define	SPRITE_TRAINEE_BALLE_1	15
#define	SPRITE_TRAINEE_BALLE_2	16
#define	SPRITE_TRAINEE_BALLE_3	17

#define SPRITE_KASHES    	18
#define	SPRITE_ZLITOS		19

#define	SPRITE_FOUDRE		21
#define	SPRITE_PROTECT		30

// Pour definir si on utilise des Kashes ou des Zlitos
#define	SPRITE_PIECE	(SPRITE_KASHES+(Planet>=2?1:0))

#define	SPRITE_ARDOISE_LEFT_UP	164
#define	SPRITE_ARDOISE_RIGHT_UP	165
#define	SPRITE_ARDOISE_LEFT_DN	166
#define	SPRITE_ARDOISE_RIGHT_DN	167

#define	SPRITE_BULLE_LEFT	170
#define	SPRITE_BULLE_RIGHT	171

#define	SPRITE_DISK		172
#define	SPRITE_CURSOR		173

/*---------------- OBJECTS ------------------*/

#define MAX_OBJETS              100

#define MAX_EXTRAS              50

#define SIZE_NAME               19

#define MAX_ANIMS               400
#define MAX_BODYS               200

#define MAX_FLAGS               255

#define MAX_ZONES               255
/*#define       MAX_CODES               255     */
#define MAX_TRACKS              255

#define MAX_VARS_CUBE           80
#define MAX_VARS_GAME           256

#define	MAX_INVENTORY		((U8)40)
#define	MAX_BOX_INVENTORY	((U8)35)

#define MAX_CHAPTER             20

#ifdef	LBA_EDITOR

#ifndef	COMPILATOR
#define	MAX_TRI	(MAX_OBJETS+MAX_FLAGS+MAX_EXTRAS+MAX_DARTS+MAX_FLOWS)
#else
#define	MAX_TRI	(MAX_OBJETS+MAX_FLAGS+MAX_EXTRAS)
#endif  // COMPILATOR

#else
#define	MAX_TRI	(MAX_OBJETS+MAX_EXTRAS+MAX_DARTS+MAX_FLOWS)
#endif

/*──────────────────────────────────────────────────────────────────────────*/

#define	PAUSE_CLOVER	1
#define	PAUSE_MESSAGE	2

#define MAX_CLOVER_BOX          10
#define	MAX_LIFE_POINTS		255
#define	START_LIFE_POINTS	200

#define	NB_CLOVER_BOX_DEFAULT	2
#define	NB_CLOVER_DEFAULT	1

/*──────────────────────────────────────────────────────────────────────────*/
/* type objet pour affscene */

#define TYPE_OBJ_3D             (1024*0)
#define TYPE_FLAG_RED           (1024*1)
#define TYPE_FLAG_YELLOW        (1024*2)
#define TYPE_SHADOW             (1024*3)
#define TYPE_OBJ_SPRITE         (1024*4)
#define TYPE_ZONE_DEC           (1024*5)
#define TYPE_EXTRA              (1024*6)
#define TYPE_FLOW               (1024*7)
#define TYPE_DART               (1024*8)
#define	TYPE_LABY		(1024*9)
#define	TYPE_TRAINEE		(1024*10)
#define	TYPE_OBJ_ANIM_3DS	(1024*11)

#define	SORT_NO_PRECLIP		(0x4000)

/*──────────────────────────────────────────────────────────────────────────*/


/* work flags */

#define WAIT_HIT_FRAME          (1<<0)
#define OK_HIT                  (1<<1)
#define ANIM_END                (1<<2)
#define NEW_FRAME               (1<<3)

#define WAS_DRAWN               (1<<4)

#define OBJ_DEAD                (1<<5)
#define AUTO_STOP_DOOR          (1<<6)

#define ANIM_MASTER_ROT         (1<<7)

#define FALLING                 (1<<8)

#define OK_SUPER_HIT            (1<<9)
#define	FRAME_SHIELD		(1<<10)

#define	DRAW_SHADOW		(1<<11)
#define	ANIM_MASTER_GRAVITY	(1<<12)

#define	SKATING			(1<<13)	// Ouch! je glisse dans une collision
                                        // interdite

#define	OK_RENVOIE		(1<<14)	// prêt à renvoyer un projectile

#define	LEFT_JUMP		(1<<15)	// prêt à sauter du pied gauche
#define	RIGHT_JUMP		(1<<16)	// prêt à sauter du pied droit
#define WAIT_SUPER_HIT          (1<<17)	// attend la fin de l'anim avant de redonner un super_hit
#define	TRACK_MASTER_ROT	(1<<18)	// c'est le track qui gere la direction
#define	FLY_JETPACK		(1<<19) // en train de voler avec le Jetpack
#define	DONT_PICK_CODE_JEU	(1<<20)	// Magouille Zones Tapis Roulant
#define	MANUAL_INTER_FRAME	(1<<21)	// Fait le ObjectSetInterFrame() manuellement
#define	WAIT_COORD		(1<<22)	// attend d'avoir été affiché pour passer les coordonnées d'1 point à 1 extra
#define	CHECK_FALLING		(1<<23)	// force objet à tester le FALLING pdt une frame

/* Flags */

#define CHECK_OBJ_COL		(1<<0)	// test des collisions et hit obj
#define CHECK_BRICK_COL         (1<<1)  // test des collisions decors
#define CHECK_ZONE              (1<<2)	// test des zones scenariques
#define SPRITE_CLIP             (1<<3)	// (portes) zone de clip fixe
#define PUSHABLE                (1<<4)	// poussable
#define COL_BASSE               (1<<5)	// 1 = pas test des collisions hautes TWINSEN

#define CHECK_CODE_JEU          (1<<6)	// test la noyade
#define CHECK_ONLY_FLOOR        (1<<7)	// test uniquement les collisions au sol

#define INVISIBLE               (1<<9)	// not drawn but all computed

#define SPRITE_3D               (1<<10)	// un sprite pas un 3DO

#define OBJ_FALLABLE            (1<<11)	// peut tomber
#define NO_SHADOW               (1<<12)	// pas d'ombre auto
#define OBJ_BACKGROUND          (1<<13)	// s'incruste dans le decor la 1er fois

#define OBJ_CARRIER             (1<<14)	// peut porter et deplacer un obj

#define MINI_ZV                 (1<<15)	// zv carre sur plus petit cote (si 3DO)
#define POS_INVALIDE            (1<<16)	// Carrier considéré comme Pos invalide
#define	NO_CHOC			(1<<17)	// Ne déclenche pas d'anim choc
#define	ANIM_3DS		(1<<18)	// Animation 3DS (extension de sprite_3D)
#define	NO_PRE_CLIP		(1<<19)	// Ne préclippe pas l'objet (pour les grands objets)
#define	OBJ_ZBUFFER		(1<<20)	// Affiche objet en ZBuffer (extérieur only !)
#define	OBJ_IN_WATER		(1<<21)	// Affiche objet en ZBuffer dans l'eau (extérieur only !)

/* Option Flags */

#define EXTRA_MASK      (16+32+64+128+256)

#define EXTRA_GIVE_NOTHING      1

#define EXTRA_GIVE_MONEY        16
#define EXTRA_GIVE_LIFE         32
#define EXTRA_GIVE_MAGIC        64
#define EXTRA_GIVE_KEY          128
#define EXTRA_GIVE_CLOVER       256

/* FlagAnim */

#define ANIM_REPEAT     0	// anim par defaut + anims scenariques
#define ANIM_THEN       1	// anim jouee une fois et interruptible
#define ANIM_ALL_THEN   2	// anim forcee jouee 1 fois et ininterruptible
#define	ANIM_TEMPO	3	// anim jouee en repeat temporairement
#define	ANIM_FINAL	4	// anim forcee ininterruptible de mort

// FlagClimbing
#define	CLIMBING_UP	1
#define	CLIMBING_DOWN	2

#define	TEMPO_PROTO_AUTO	500

/*──────────────────────────────────────────────────────────────────────────*/
//                              CODES JEU

#define CJ_WATER                1       // eau
#define CJ_ELECTRIC             2       // Pseudo électrifié (mort instantanée)
#define CJ_ESCALATOR_OUEST      3       // Tapis roulant OUEST
#define CJ_ESCALATOR_EST        4       // Tapis roulant EST
#define CJ_ESCALATOR_NORD       5       // Tapis roulant NORD
#define CJ_ESCALATOR_SUD        6       // Tapis roulant SUD
#define	CJ_LABYRINTHE		7	// Pseudo visible seulement sous
					// les pieds de l'objet
#define	CJ_POS_INVALID		8	// Position non valide
#define	CJ_LAVE			9	// psshhht !
#define	CJ_GAZ			11	// prout !
#define	CJ_FOOT_WATER		12	// Eau où l'on a pied
#define	CJ_ANIMATED_LAVE	13
#define	CJ_ANIMATED_GAZ		14
#define CJ_ANIMATED_WATER	15      // eau animée où on se noie

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/


#define EXTRA_TIME_OUT          (1L<<0L)
#define EXTRA_FLY               (1L<<1L)
#define EXTRA_END_OBJ           (1L<<2L)
#define EXTRA_END_COL           (1L<<3L)
#define EXTRA_STOP_COL          (1L<<4L)
#define EXTRA_TAKABLE           (1L<<5L)
#define EXTRA_FLASH             (1L<<6L)
#define EXTRA_SEARCH_OBJ        (1L<<7L)
#define EXTRA_IMPACT            (1L<<8L)
#define EXTRA_SEARCH_KEY        (1L<<9L)
#define EXTRA_TIME_IN           (1L<<10L)
#define	EXTRA_PROTECT		(1L<<11L)
#define	EXTRA_CLIGNOTE		(1L<<12L)
#define EXTRA_WAIT_NO_COL       (1L<<13L)
#define	EXTRA_BONUS		(1L<<14L)

// Type d'extras particuliers
#define	EXTRA_MAGIC_BALL	(1L<<15L)
#define	EXTRA_DART		(1L<<16L)
#define	EXTRA_CONQUE		(1L<<17L)	// Bonus Conque
#define	EXTRA_WAIT		(1L<<18L)	// Tempo avant gestion
#define	EXTRA_TRAINEE		(1L<<19L)	// Trainee balle magique
#define	EXTRA_NEW_IMPACT	(1L<<20L)	// Genere un impact
#define	EXTRA_ANIM_SPRITE	(1L<<21L)	// Animation de sprite
#define	EXTRA_RENVOYABLE	(1L<<22L)	// Peut etre renvoyé par le gant
#define	EXTRA_TRANSPARENT	(1L<<23L)	// Sprite Transparent
#define	EXTRA_HIT_ANY		(1L<<24L)	// Porte un coup même à son propriétaire
#define	EXTRA_WAIT_NO_COL_OWNER	(1L<<25L)	// Attend de sortir de la collision de
						// son propriétaire avant de valider la collision
#define	EXTRA_FALLING		(1L<<26L)	// Extra en train de chuter
#define	EXTRA_FOUDRE_STEP1	(1L<<27L)	// Foudre etape 1
#define	EXTRA_FOUDRE_STEP2	(1L<<28L)	// Foudre etape 2
#define	EXTRA_FOUDRE_STEP3	(1L<<29L)	// Foudre etape 3

#define	EXTRA_NO_SHADOW		(1L<<31L)	// Ne pas Afficher l'ombre de l'extra

// En masquant le champ Flags des Extras avec EXTRA_MASK_WEAPON,
// on, sait si l'extra est une arme et laquelle !!
#define	EXTRA_MASK_WEAPON       (EXTRA_MAGIC_BALL|EXTRA_DART|EXTRA_CONQUE)

typedef struct	// Normalement bien alignée
{
	S32    	PosX ;
	S32    	PosY ;
	S32    	PosZ ;

	union   {
		MOVE Move ;
		struct {
			S32    	X ;	// ATTENTION: sert aussi de T_REAL_VALUE_HR (4 S32)
			S32    	Y ;	// donc OrgX, OrgY, OrgZ
			S32    	Z ;
		} Org ;
		} U ;

	S32    	Info ;

	U8     	*PtrBody ;              // pour les projectiles 3D

	S16    	Sprite ;                /* num sprite */

	S16    	Vx ;
	S16    	Vy ;
	S16    	Vz ;

	U32    	Flags ;
	U32    	Timer ;                 /* memo timer 50hz */

	S16    	Body ;
	S16    	Beta ;                  // pour les projectiles 3D

	S16    	TimeOut ;               /* life time */
	S16    	Divers ;                /* nombre pour zone giver */

	U8     	Poids ;
	U8     	HitForce ;              /* si !=0 force de frappe */

	U8      Owner ;
	U8	NewForce ;		// HitForce des extras renvoyables

#ifdef	LBA_EDITOR
	char	Impact[13] ;		// Impact a générer
	S32	Angle ;
#else
	S32	Impact ;
#endif
	S32	Scale  ;		// Facteur de zoom
}       T_EXTRA ;

/*──────────────────────────────────────────────────────────────────────────*/
// Specifique a la balle magique, pour MagicBallFlags

#define MAGIC_BALL_LANCEE       1
#define MAGIC_BALL_LACHEE       2
#define MAGIC_BALL_RAPPELEE     4

/*──────────────────────────────────────────────────────────────────────────*/
// Sort de Protection

#define	PROTECT_SPEED_MAGIC	2	// Nb Pts Magie Par Seconde
#define	PROTECT_ALARME_SEUIL	15	// Pts de Magie à partir duquel
					// le sort clignote

/*──────────────────────────────────────────────────────────────────────────*/
// Anim 3DS

typedef struct
{
	char 	Name[4] ;	// Nom de l'anim
	S16	Deb ;		// Frame de début dans le HQR
	S16	Fin ;		// Frame de fin dans le HQR
}       T_ANIM_3DS ;

/*──────────────────────────────────────────────────────────────────────────*/

typedef struct
{
	S32    X0 ;
	S32    Y0 ;
	S32    Z0 ;
	S32    X1 ;
	S32    Y1 ;
	S32    Z1 ;
	S32    Info0 ;
	S32    Info1 ;
	S32    Info2 ;
	S32    Info3 ;
	S32    Info4 ;	       	// Rajout de 4 champs infos le 08/12/95
	S32    Info5 ;
	S32    Info6 ;
	S32    Info7 ;

	S16    Type ;
	S16    Num ;

#ifdef  LBA_EDITOR
	S16    Snap  ;         	// ce champ ne sert que
				// pour l'edition dans Visu3d !!!!
#endif
	}       T_ZONE ;

/*---------------- holomap -----------------*/

#define MAX_HOLO_POS    150

typedef struct  {       S16    Alpha ;
			S16    Beta ;
			S16    Size ;

//                      S16    XIsland ;
//                      S16    ZIsland ;

//                      U8   Type ;

			S16    Mess ;
//                      S16    Fleche ;
						} T_HOLO_POS ;

/*---------------- incrust display ---------*/

#define MAX_INCRUST_DISP        10

#define INCRUST_SPRITE          0
#define INCRUST_NUM             1
#define INCRUST_CMPT            2
#define INCRUST_OBJ             3
#define INCRUST_TEXT            4
#define INCRUST_INVENTORY       5
#define	INCRUST_SYS_TEXT	6
#define	INCRUST_ECLAIR		7
#define	INCRUST_PLUIE		8

#define	INCRUST_YCLIP		(1<<8)

typedef struct  {       S16    Num ;
			S16    X ;
			S16    Y ;
			S16    Type ;
			S16    Info ;
			S16    Move ;
			U32   TimerEnd ;      }       T_INCRUST_DISP ;

/*---------------- Comportements du heros ---------*/

#define C_NORMAL        	0	// Normal
#define C_SPORTIF       	1	// Sportif
#define C_AGRESSIF      	2	// Agressif
#define C_DISCRET       	3	// Discret
#define C_PROTOPACK     	4	// Protopack
#define	C_DOUBLE		5	// Twinsen + Zoé
#define	C_CONQUE		6	// Conque
#define	C_SCAPH_INT_NORM	7	// Scaphandre Interieur Normal
#define	C_JETPACK		8	// SuperJetPack
#define	C_SCAPH_INT_SPOR	9	// Scaphandre Interieur Sportif
#define	C_SCAPH_EXT_NORM       10	// Scaphandre Exterieur Normal
#define	C_SCAPH_EXT_SPOR       11	// Scaphandre Exterieur Sportif
#define	C_BUGGY		       12	// Conduite du buggy
#define	C_SKELETON	       13	// Squelette Electrique

#define	MAX_COMPORTEMENTS      14

#define	START_TEXT_COMPORTEMENT	80

typedef struct	{
#ifdef	LBA_EDITOR
	char	Name[50] ;	// Nom du comportement (pour l'éditeur)
#endif

	S32	Menu ;		// Index sur ListMenuComp
	S32	AnimRien ;	// AnimRien specifique pour chaque comportement
	U8	*PtrFile3d ;	// Pointeur sur fiche 3D

#ifdef	LBA_EDITOR
	char	File3d[ADELINE_MAX_PATH] ;	// .3D (en version jeu, cela
						// correspond au numero de
						// comportement)
#endif

	U32	Flags ;				// Caractéristiques du comportement

	}	T_COMPORTEMENT ;

// Valeurs possibles du champ Flags

#define	CF_COMPORTEMENT		(1<<0)	// Acces direct au comportement (F1-F4)
					// (Uniquement en mode jeu)
#define	CF_INVENTORY		(1<<1)	// Acces au menu Inventory
#define	CF_WALK_ON_CODES_JEU	(1<<2)	// Peut marcher sur les codes jeux (Protopack)
#define	CF_CLIMB		(1<<3)	// Peut grimper aux échelles
#define	CF_WEAPON		(1<<4)	// Peut utiliser la touche <ALT>
#define	CF_HOLOMAP		(1<<5)	// Acces direct à l'Holomap (touche H)
#define	CF_SHIELD		(1<<6)	// Acces au bouclier (touche ?)

#define	CF_STANDARD	CF_COMPORTEMENT|CF_INVENTORY|CF_CLIMB|CF_WEAPON|CF_HOLOMAP|CF_SHIELD

/*---------------- Menus Comportement ------------------*/
typedef struct {
	S32	Enable ;	// ce comportement-menu est-il disponible ?
	S32	Previous ;	// comportement-menu précédent
	S32	Next ;		// comportement-menu suivant
	S32	NbComps ;	// nombre de comportement
	S32	Comps[4]  ;	// Liste des comportements du menu
	} T_MENU_COMP ;

#define	MAX_MENUS_COMP	8

extern	T_MENU_COMP	ListMenuComp[MAX_MENUS_COMP] ;

/*---------------- divers ------------------*/

#define	MAX_ISLES		12
#define START_FILE_ISLAND       3

//#define Rnd(n)  (rand()%n)

#define NUM_PERSO       ((U8)0)
#define COUL_PERSO      12	// Couleur de dialogue de Twinsen

#define	NUM_ZOE		((U8)7)	// Numero de personnage scenarique
#define	COUL_ZOE	4	// Couleur de dialogue de Zoe

#define	COUL_CINEMA	0	// Couleur des bandes cinema

#define	MECA_PINGOUIN_DISTANCE	700
#define NUM_PINGOUIN    1

#define ACTIVE_LIFE     0
#define ACTIVE_TRACK    1

#define MAX_TYPES_ZONE  10

#define POS_MIDDLE      1
#define POS_LEFT        2
#define POS_RIGHT       4
#define POS_UP          8
#define POS_DOWN        16

#define	MAX_HIT_POFS	5

#define	POF_CHOC_TIMER	1000
#define POF_CHOC_DUREE  (50*20*2)
#define POF_CHOC_SCALE  2500	// ~ 12 cm
#define	POF_COL_DUREE	(5*20)
#define	POF_COL_SCALE_DEB	1000	// ~ 25 cm
#define	POF_COL_SCALE_FIN    	4000	// ~ 6 cm

/*---------------- Script: defines ------------------*/

#define NO_MOVE                 0
#define MOVE_MANUAL             1
#define MOVE_FOLLOW             2
#define MOVE_TRACK              3
#define MOVE_FOLLOW_2           4
#define MOVE_TRACK_ATTACK       5
#define MOVE_SAME_XZ            6
#define MOVE_PINGOUIN           7
#define MOVE_WAGON              8
#define	MOVE_CIRCLE		9 	// Beta = Tangeante au cercle
#define	MOVE_CIRCLE2		10	// Beta = face au drapeau
#define MOVE_SAME_XZ_BETA       11
#define	MOVE_BUGGY		12
#define	MOVE_BUGGY_MANUAL	13

#define NB_MOVES        	14

/*---------------- special Wagons -----------------*/

#define SROT_WAGON      70      // vitesse de rotation des wagons

// lignes droites
#define RAIL_NORD_SUD           55      /*  /  */
#define RAIL_EST_OUEST          54      /*  \  */
#define RAIL_UP_NORD            64      /*  /  */
#define RAIL_UP_SUD             66      /*  /  */
#define RAIL_UP_EST             67      /*  \  */
#define RAIL_UP_OUEST           65      /*  \  */

// virages
#define RAIL_NORD_EST           52      /*  /\ */

#define RAIL_NORD_OUEST         51      /*  \
                                            / */

#define RAIL_SUD_EST            50      /*  /
					    \ */

#define RAIL_SUD_OUEST          53      /*  \/ */

// aiguillages
#define RAIL_NORD_NORD_EST      57      /* /_
					  /   */

#define RAIL_NORD_NORD_OUEST    56      /* \/
					   /  */

#define RAIL_SUD_SUD_EST        58      /* /
					  /\  */

#define RAIL_SUD_SUD_OUEST      59      /* __/
					    /  */

#define RAIL_OUEST_OUEST_SUD    60      /* _\
					     \  */

#define RAIL_OUEST_OUEST_NORD   61      /*  \/
					     \  */

#define RAIL_EST_EST_SUD        62      /*  \
					    /\  */

#define RAIL_EST_EST_NORD       63      /*  \_
					     \  */

//*********** Rails de la planete ESMER

// lignes droites
#define RAIL_E_NORD_SUD         15      /*  /  */
#define RAIL_E_EST_OUEST        16      /*  \  */
#define RAIL_E_UP_NORD          59      /*  /  */
#define RAIL_E_UP_SUD           60      /*  /  */
#define RAIL_E_UP_EST           53      /*  \  */
#define RAIL_E_UP_OUEST         58      /*  \  */

// virages
#define RAIL_E_NORD_EST         17      /*  /\ */

#define RAIL_E_NORD_OUEST       19      /*  \
					    / */

#define RAIL_E_SUD_EST          20      /*  /
					    \ */

#define RAIL_E_SUD_OUEST        18      /*  \/ */

// aiguillages
#define RAIL_E_NORD_NORD_EST    67      /* /_
					  /   */

#define RAIL_E_NORD_NORD_OUEST  68      /* \/
					   /  */

#define RAIL_E_SUD_SUD_EST      51      /* /
					  /\  */

#define RAIL_E_SUD_SUD_OUEST    61      /* __/
					    /  */

#define RAIL_E_OUEST_OUEST_SUD  22      /* _\
					     \  */

#define RAIL_E_OUEST_OUEST_NORD 21      /*  \/
					     \  */

#define RAIL_E_EST_EST_SUD      63      /*  \
					    /\  */

#define RAIL_E_EST_EST_NORD     65      /*  \_
					     \  */

// Inclinaison des essieux
#define	PAS_ESSIEU	30

/*---------------- Track: macros ------------------*/

#define TM_END                  0
#define TM_NOP                  1
#define TM_BODY                 2
#define TM_ANIM                 3
#define TM_GOTO_POINT           4
#define TM_WAIT_ANIM            5
#define TM_LOOP                 6
#define TM_ANGLE                7
#define TM_POS_POINT            8
#define TM_LABEL                9
#define TM_GOTO                 10
#define TM_STOP                 11
#define TM_GOTO_SYM_POINT       12
#define TM_WAIT_NB_ANIM         13
#define TM_SAMPLE               14
#define TM_GOTO_POINT_3D        15
#define TM_SPEED                16
#define TM_BACKGROUND           17
#define TM_WAIT_NB_SECOND       18
#define TM_NO_BODY              19
#define TM_BETA                 20
#define TM_OPEN_LEFT            21
#define TM_OPEN_RIGHT           22
#define TM_OPEN_UP              23
#define TM_OPEN_DOWN            24
#define TM_CLOSE                25
#define TM_WAIT_DOOR            26
#define TM_SAMPLE_RND           27
#define TM_SAMPLE_ALWAYS        28
#define TM_SAMPLE_STOP          29
#define TM_PLAY_ACF             30
#define TM_REPEAT_SAMPLE        31
#define TM_SIMPLE_SAMPLE        32
#define TM_FACE_TWINSEN         33
#define TM_ANGLE_RND            34
#define TM_REM                  35
#define TM_WAIT_NB_DIZIEME      36
#define TM_DO                   37
#define TM_SPRITE               38
#define	TM_WAIT_NB_SECOND_RND	39
#define	TM_AFF_TIMER		40
#define	TM_SET_FRAME		41
#define	TM_SET_FRAME_3DS	42
#define	TM_SET_START_3DS	43
#define	TM_SET_END_3DS		44
#define	TM_START_ANIM_3DS	45
#define	TM_STOP_ANIM_3DS	46
#define	TM_WAIT_ANIM_3DS	47
#define	TM_WAIT_FRAME_3DS	48
#define	TM_WAIT_NB_DIZIEME_RND	49
#define	TM_DECALAGE		50
#define	TM_FREQUENCE		51
#define	TM_VOLUME		52

#define NB_MACROS_TRACK         53

/*---------------- Life: macro ------------------*/

/*---*/
#define LM_END                  0
#define LM_NOP                  1
#define LM_SNIF                 2
#define LM_OFFSET               3
#define LM_NEVERIF              4

#define LM_PALETTE              10
#define LM_RETURN               11
#define LM_IF                   12
#define LM_SWIF                 13
#define LM_ONEIF                14
#define LM_ELSE                 15
#define LM_ENDIF                16
#define LM_BODY                 17
#define LM_BODY_OBJ             18
#define LM_ANIM                 19
#define LM_ANIM_OBJ             20
#define LM_SET_CAMERA  		21
#define LM_CAMERA_CENTER	22
#define LM_SET_TRACK            23
#define LM_SET_TRACK_OBJ        24
#define LM_MESSAGE              25
#define LM_FALLABLE             26
#define LM_SET_DIR              27
#define LM_SET_DIR_OBJ          28
#define LM_CAM_FOLLOW           29
#define LM_COMPORTEMENT_HERO    30
#define LM_SET_VAR_CUBE         31
#define LM_COMPORTEMENT         32
#define LM_SET_COMPORTEMENT     33
#define LM_SET_COMPORTEMENT_OBJ 34
#define LM_END_COMPORTEMENT     35
#define LM_SET_VAR_GAME         36
#define LM_KILL_OBJ             37
#define LM_SUICIDE              38
#define LM_USE_ONE_LITTLE_KEY   39
#define LM_GIVE_GOLD_PIECES     40
#define LM_END_LIFE             41
#define LM_STOP_L_TRACK         42
#define LM_RESTORE_L_TRACK      43
#define LM_MESSAGE_OBJ          44
#define LM_INC_CHAPTER          45
#define LM_FOUND_OBJECT         46
#define LM_SET_DOOR_LEFT        47
#define LM_SET_DOOR_RIGHT       48
#define LM_SET_DOOR_UP          49
#define LM_SET_DOOR_DOWN        50
#define LM_GIVE_BONUS           51
#define LM_CHANGE_CUBE          52
#define LM_OBJ_COL              53
#define LM_BRICK_COL            54
#define LM_OR_IF                55
#define LM_INVISIBLE            56
#define LM_SHADOW_OBJ          	57
#define LM_POS_POINT            58
#define LM_SET_MAGIC_LEVEL      59
#define LM_SUB_MAGIC_POINT      60
#define LM_SET_LIFE_POINT_OBJ   61
#define LM_SUB_LIFE_POINT_OBJ   62
#define LM_HIT_OBJ              63
#define LM_PLAY_ACF             64
#define LM_ECLAIR            	65
#define LM_INC_CLOVER_BOX       66
#define LM_SET_USED_INVENTORY   67
#define LM_ADD_CHOICE           68
#define LM_ASK_CHOICE           69
#define LM_INIT_BUGGY		70
#define LM_MEMO_ARDOISE 	71
#define LM_SET_HOLO_POS         72
#define LM_CLR_HOLO_POS         73
#define LM_ADD_FUEL             74
#define LM_SUB_FUEL             75
#define LM_SET_GRM              76
#define LM_SET_CHANGE_CUBE	77
#define LM_MESSAGE_ZOE		78
#define LM_FULL_POINT           79
#define LM_BETA                 80
#define LM_FADE_TO_PAL		81
#define LM_ACTION         	82
#define LM_SET_FRAME	        83
#define LM_SET_SPRITE		84
#define LM_SET_FRAME_3DS	85
#define LM_IMPACT_OBJ		86
#define LM_IMPACT_POINT       	87
#define LM_ADD_MESSAGE		88
#define LM_BULLE                89 // remplace BULLE_ON
#define LM_NO_CHOC     		90
#define LM_ASK_CHOICE_OBJ       91
#define LM_CINEMA_MODE 		92
#define LM_SAVE_HERO   		93
#define LM_RESTORE_HERO		94
#define LM_ANIM_SET             95
#define LM_PLUIE        	96
#define LM_GAME_OVER            97
#define LM_THE_END              98
#define LM_ESCALATOR            99
#define LM_PLAY_MUSIC		100
#define LM_TRACK_TO_VAR_GAME	101
#define LM_VAR_GAME_TO_TRACK	102
#define LM_ANIM_TEXTURE       	103
#define LM_ADD_MESSAGE_OBJ     	104
#define LM_BRUTAL_EXIT          105
#define LM_REM                  106
#define LM_ECHELLE              107
#define LM_SET_ARMURE           108
#define LM_SET_ARMURE_OBJ       109
#define LM_ADD_LIFE_POINT_OBJ   110
#define LM_STATE_INVENTORY	111
#define LM_AND_IF               112
#define LM_SWITCH               113
#define LM_OR_CASE              114
#define LM_CASE                 115
#define LM_DEFAULT              116
#define LM_BREAK                117
#define LM_END_SWITCH           118
#define LM_SET_HIT_ZONE         119
#define LM_SAVE_COMPORTEMENT    120
#define LM_RESTORE_COMPORTEMENT 121
#define LM_SAMPLE               122
#define LM_SAMPLE_RND           123
#define LM_SAMPLE_ALWAYS        124
#define LM_SAMPLE_STOP          125
#define LM_REPEAT_SAMPLE        126
#define LM_BACKGROUND           127
#define LM_ADD_VAR_GAME         128
#define LM_SUB_VAR_GAME         129
#define LM_ADD_VAR_CUBE         130
#define LM_SUB_VAR_CUBE         131
//#define LM_NOP      			132
#define LM_SET_RAIL             133
#define LM_INVERSE_BETA         134
#define LM_NO_BODY              135
#define LM_ADD_GOLD_PIECES      136
#define	LM_STOP_L_TRACK_OBJ	137
#define	LM_RESTORE_L_TRACK_OBJ	138
#define	LM_SAVE_COMPORTEMENT_OBJ	139
#define	LM_RESTORE_COMPORTEMENT_OBJ	140
#define	LM_SPY			141
#define	LM_DEBUG		142
#define	LM_DEBUG_OBJ		143
#define	LM_POPCORN		144
#define	LM_FLOW_POINT		145
#define	LM_FLOW_OBJ		146
#define	LM_SET_ANIM_DIAL	147
#define	LM_PCX			148
#define	LM_END_MESSAGE		149
#define	LM_END_MESSAGE_OBJ	150
#define	LM_PARM_SAMPLE		151
#define	LM_NEW_SAMPLE		152
#define	LM_POS_OBJ_AROUND	153
#define	LM_PCX_MESS_OBJ		154

#define NB_MACROS_LIFE          155

/*---*/
#define LF_COL                  0
#define LF_COL_OBJ              1
#define LF_DISTANCE             2
#define LF_ZONE                 3
#define LF_ZONE_OBJ             4
#define LF_BODY                 5
#define LF_BODY_OBJ             6
#define LF_ANIM                 7
#define LF_ANIM_OBJ             8
#define LF_L_TRACK              9
#define LF_L_TRACK_OBJ          10
#define LF_VAR_CUBE             11
#define LF_CONE_VIEW            12
#define LF_HIT_BY               13
#define LF_ACTION               14
#define LF_VAR_GAME             15
#define LF_LIFE_POINT           16
#define LF_LIFE_POINT_OBJ       17
#define LF_NB_LITTLE_KEYS       18
#define LF_NB_GOLD_PIECES       19
#define LF_COMPORTEMENT_HERO    20
#define LF_CHAPTER              21
#define LF_DISTANCE_3D          22
#define LF_MAGIC_LEVEL          23
#define LF_MAGIC_POINT          24
#define LF_USE_INVENTORY        25
#define LF_CHOICE               26
#define LF_FUEL                 27
#define LF_CARRY_BY             28
#define LF_CDROM                29
#define LF_ECHELLE              30
#define LF_RND                  31
#define LF_RAIL                 32
#define LF_BETA                 33
#define LF_BETA_OBJ             34
#define LF_CARRY_OBJ_BY         35
#define LF_ANGLE                36
#define LF_DISTANCE_MESSAGE     37
#define	LF_HIT_OBJ_BY		38
#define LF_REAL_ANGLE           39
#define	LF_DEMO			40
#define LF_COL_DECORS		41
#define	LF_COL_DECORS_OBJ	42
#define	LF_PROCESSOR		43
#define	LF_OBJECT_DISPLAYED	44
#define LF_ANGLE_OBJ            45

#define NB_FUNCS_LIFE           46

/*---*/
#define LT_EQUAL                0
#define LT_SUP                  1
#define LT_LESS                 2
#define LT_SUP_EQUAL            3
#define LT_LESS_EQUAL           4
#define LT_DIFFERENT            5

#define NB_TESTS_LIFE           6

/*---------------- The End: error ------------------*/

#define ERROR_NOT_FOUND_FILE    0
#define NOT_ENOUGH_MEM          1
#define PROGRAM_OK              2
#define NAME_NOT_FOUND          3
#define ERROR_FILE_NOT_CREATED  4

/*---------------- Headers ------------------*/

#define H_END           0
#define H_OBJECT        1
#define H_CODE          2
#define H_TRACK         3
#define H_TRACK_PROG    4
#define H_MAP_FILE      5
#define H_ZONE          6
#define H_HERO_START    7
#define H_LIFE_PROG     8
#define H_AMBIANCE      9
#define H_INFO_WORLD    10
#define H_VAR_CUBE      11
#define H_VAR_GAME      12

#define NB_HEADER       13

/*──────────────────────────────────────────────────────────────────────────*/
//			Inventory
//	Tout a mettre dans common.h normalement...
#define	COUL_SELECT_MENU	166
#define	COUL_SLIDER_MENU	64&0xF0
#define	COUL_INPUT_NAME		166&0xF0
#define	FIRE_SELECT_MENU	0
#define	FIRE_SLIDER_MENU	1
#define	FIRE_INPUT_NAME		2
#define	FLAG_INV_USED	1	//	Already used -> Auto Use Now
#define	FLAG_INV_FEWER	2	//	Possibly fewer, must display quantity
#define	FLAG_INV_VALUE	4	//	Must display value Bar
#define	FLAG_INV_AUTO_USE 8	//	Utilisation automatique possible
#define	FLAG_INV_UNUSABLE 16	//	Temporarily unusable

typedef	struct
{
	S32	Box		;//	Inventory position
	S32	BetaObj		;//	Beta for 3D rotation
	S32	PtMagie		;//	Needed magie
	S32	FlagInv		;//	Flag : Used, New, Magic, etc.
	S16	IdObj3D		;//	Etat courant
	U8	TabObj3D[4]	;//	Objects evolution ( selon IdObj3D )
} T_INV_OBJ	;

//======================== Issus de Dial 
#define	DIAL_DEF	(1<<0)	// Texte normal (defaut)
#define DIAL_BIG	(1<<1)	// Texte affiche en BigMessage
#define	DIAL_FUL	(1<<2)	// Texte affiche en BigMessage sans cadre (Full Screen)
#define	DIAL_SAY	(1<<3)	// Texte affiche en SayMessage
#define	DIAL_INT	(1<<4)	// Resonnance de la voix en interieur
#define	DIAL_EXT	(1<<5)	// Resonnance de la voix en exterieur
#define	DIAL_CAV	(1<<6)	// Resonnance de la voix en cave

//======================== Commandes des fiches 3D (fiche.cpp)
#define	F_NOP			0
#define	F_BODY			1
#define	F_BODP			2
#define	F_ANIM			3
#define	F_ANIP			4
#define	F_HIT			5
#define	F_SAMPLE		6
#define	F_SAMPLE_RND		7
#define	F_THROW			8
#define	F_THROW_MAGIC		9
#define	F_SAMPLE_REPEAT		10
#define	F_THROW_SEARCH		11
#define	F_THROW_ALPHA		12
#define	F_SAMPLE_STOP		13
#define	F_ZV			14
#define	F_LEFT_STEP		15
#define	F_RIGHT_STEP		16
#define	F_HIT_TWINSEN		17
#define	F_THROW_3D		18
#define	F_THROW_3D_ALPHA	19
#define	F_THROW_3D_SEARCH	20
#define	F_THROW_3D_MAGIC	21
#define	F_SUPER_HIT		22
#define	F_THROW_OBJ_3D		23
#define	F_PATH			24
#define	F_FLOW			25
#define	F_FLOW_3D		26
#define	F_THROW_DART		27
#define	F_SHIELD		28
#define	F_SAMPLE_MAGIC		29
#define	F_THROW_3D_CONQUE	30
#define	F_ZV_ANIMIT		31
#define	F_IMPACT		32
#define	F_RENVOIE		33
#define	F_RENVOYABLE		34
#define	F_TRANSPARENT		35
#define	F_SCALE			36
#define	F_LEFT_JUMP		37
#define	F_RIGHT_JUMP		38
#define	F_NEW_SAMPLE		39
#define	F_IMPACT_3D		40
#define	F_THROW_MAGIC_EXTRA	41
#define	F_THROW_FOUDRE		42

#define	NB_F_COMMAND	((S8)43)

//=========================== Effets de Pcx
#define	PCX_FADE	0
#define	PCX_V_SHADE	1

#define	NB_PCX_EFFECTS	2
#endif	/* COMMON_H */


