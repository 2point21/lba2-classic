/*══════════════════════════════════════════════════════════════════════════*/

// lbaext.h

/*──────────────────────────────────────────────────────────────────────────*/

S32	Min3( S32 a, S32 b, S32 c ) ;
#pragma aux	Min3=					\
		"cmp	eax, edx"			\
		"jge	m0"				\
		"cmp	eax, ecx"			\
		"jl	mex"				\
		"jmp	m1"				\
	"m0:"						\
		"mov	eax, edx"			\
		"cmp	eax, ecx"			\
		"jl	mex"				\
	"m1:"						\
		"mov	eax, ecx"			\
	"mex:"						\
		parm caller [eax] [edx] [ecx]		\
		modify exact [eax]

S32	Max3( S32 a, S32 b, S32 c ) ;
#pragma aux	Max3=					\
		"cmp	eax, edx"			\
		"jle	m0"				\
		"cmp	eax, ecx"			\
		"jg	mex"				\
		"jmp	m1"				\
	"m0:"						\
		"mov	eax, edx"			\
		"cmp	eax, ecx"			\
		"jg	mex"				\
	"m1:"						\
		"mov	eax, ecx"			\
	"mex:"						\
		parm caller [eax] [edx] [ecx]		\
		modify exact [eax]

// maxima

#define	MAX_OBJ_DECORS		200

#define	NB_COTE		 	64
#define	SIZE_MAIN_MAP		16

#define	MAX_CUBES_PER_ISLE	20	// desert

// cube infos
#define		INFO_ALPHA_LIGHT	0
#define		INFO_BETA_LIGHT		1
#define		INFO_NB_DECORS		2
#define		INFO_SKY_Y		3
#define		INFO_START_Z_FOG	4
#define		INFO_CLIP_Z_FAR		5
#define		INFO_OFFSET_ANI_POLY_1	6
#define		INFO_OFFSET_ANI_POLY_2	7
#define		INFO_OFFSET_ANI_POLY_3	8
#define		INFO_OFFSET_ANI_POLY_4	9

#define		SIZE_CUBE_INFOS		10

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

/*──────────────────────────────────────────────────────────────────────────*/

// structure d'un element du decors
typedef struct {

		S32	Body	;		//
		S32	Xworld	;		//
		S32	Yworld	;        	// position dans le monde
		S32	Zworld	;	      	//
		S32	CodeJeu	;	      	// CodeJeu U8
		S32	Beta	;		// direction de l'objet

		S32	XMin ;			// ZV
		S32	YMin ;
		S32	ZMin ;
		S32	XMax ;
		S32	YMax ;
		S32	ZMax ;

		}	T_DECORS ;

// structure d'un masque
// ATTENTION : Structure utilisée en ASM dans MASQUE_A.ASM (donc à modifier
// 	       aussi !)

// structure tempo de rotation/tri
typedef struct {

		S32	Zrot;
		S16	X2D;	// old S32
		S16	Y2D;	// old S32

		} 	T_SOMMET_ROT;

//========== Flags pour les objets du decors (champ Body&0xFFFF0000 )

#define	DEC_INVISIBLE	(1<<16)	// objet invisible (sorte de GRM)
#define	DEC_DRAWN	(1<<17)	// objet affiché à l'ecran

typedef struct {
		S32	Zrot ;

		T_DECORS *PtrDec ;	// ca prend la meme place que Num
					// et c'est plus rapide dans les
					// boucles !
		S32	Num ;

		S32	NumMasque ;		// numero eventuel d'un masque écran

		}	T_LIST_TRI ;

//****************************************************************************
// structure map poly mode jeu ( 32 Bits)
typedef	struct	{

		U32	Bank		:4 ;	// coul bank poly
		U32	TexFlag		:2 ;	// flag texture 00 rien 01 triste 10 flat 11 gouraud
		U32	PolyFlag	:2 ;	// flag poly 00 rien 01 flat 10 gouraud 11 dither
		U32	SampleStep	:4 ;	// sample pas twinsen
		U32	CodeJeu		:4 ;	// code jeu
		U32	Sens		:1 ;	// sens diagonale
		U32	Col		:1 ;
		U32	Dummy		:1 ;
		U32	IndexTex	:13 ;	// index texture 8192

		}	T_HALF_POLY ;		// 1 long

typedef struct	{

		U16	Tx0 ;
		U16	Ty0 ;
		U16	Tx1 ;
		U16	Ty1 ;
		U16	Tx2 ;
		U16	Ty2 ;

		}	T_HALF_TEX ;		// 2 Longs


//****************************************************************************
// animated sea

typedef	struct	{

		Struc_Point	Tab_Points[3] ;

		U8	Type ;
		U8	Color ;

		S16	Fog ;

		S16	X0 ;
		S16	Y0 ;

		S16	X1 ;
		S16	Y1 ;


		}	S_ANI_POLY ;


//****************************************************************************
