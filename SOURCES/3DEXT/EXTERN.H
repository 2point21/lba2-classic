// extern.h

#ifndef	EXT_EXTERN_H
#define	EXT_EXTERN_H

// vars terrain game
#include	"lba_ext.h"
#include	"var_ext.h"


#define	SIZE_Z_BUFFER	(640*480*2)


#ifdef __cplusplus
extern "C" {
#endif

S32	LineRain(S32 x0, S32 y0, S32 z0, S32 x1, S32 y1, S32 z1, S32 coul );

#pragma aux LineRain	"*"				\
	parm caller [eax] [ebx] [edi] [ecx] [edx] [esi]

#ifdef __cplusplus
}
#endif

#define	DISTANCE_VISEE	2500

extern	char	IslandName[ADELINE_MAX_PATH] ;

extern	S32	PalLevel	;

extern	S32	CurrentCubeX	;
extern	S32	CurrentCubeY	;

extern	S32	DrawTerrain 	;

extern	U8	*ScreenAux 	;
extern	U8	*ScreenTempo 	;

#ifdef	MOUNFRAC
extern	S32	MyKey ;
extern	S32	MyJoy ;
extern	S32	MyFire ;
extern	S32	MyClick ;

extern	U8	CurrentPalette[768] ;
#endif

#ifndef	LBA_GAME
extern	S32	MyMouseX ;
extern	S32	MyMouseY ;
#endif


// vars edition terrain
#ifndef	LBA_GAME
extern	S32	IndXPolySelected ;
extern	S32	IndZPolySelected ;
#endif

extern	S32	LastTriangle ;

// test obj animé dans decors

#ifdef	MOUNFRAC
extern	T_OBJET	Objet ;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	S32	Pt_XObs;
	S32	Pt_YObs;
	S32	Pt_ZObs;
	S32	Pt_UObs;
	S32	Pt_VObs;
}	Struc_Point3D;


#ifdef __cplusplus
}
#endif


// Prototypes des fonctions de la 3DExt

#include	"DECORS.H"
#include	"LOADISLE.H"
#include	"MAPTOOLS.H"
#include	"SELECT.H"
#include	"TERRAIN.H"
#include	"BOXZBUF.H"

#endif	// EXT_EXTERN_H
