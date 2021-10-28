#include "\projet\lib386\lib_X\lib_X.h"

#define MAX_SAMPLES_FLA 255
#define NOM_FICHIER_DEF_CONFIGURATION "C:\\ADELINE.DEF"

#ifdef	ONE_GAME_DIRECTORY
#define PATH_FICHIER_SAMPLE "FLA\\"
#else
#define PATH_FICHIER_SAMPLE "F:\\projet\\lba\\samples\\flasamp\\"
#endif

#define NOM_FICHIER_TEMPORAIRE "Fla_"
#define NOM_FICHIER_FRAME_TEMPORAIRE "C:\\_Frame.tmp"

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

// Structures utilisées pour les fichiers format FLI
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

// Structures utilisées pour les fichiers format FLA
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

