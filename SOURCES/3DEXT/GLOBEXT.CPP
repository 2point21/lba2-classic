#include	"defines.h"

#include	"var_ext.cpp"	// variables pour le terrain

char	IslandName[ADELINE_MAX_PATH] ;

S32	CurrentCubeX = 8 ;
S32	CurrentCubeY = 9 ;

S32	DrawTerrain = TRUE ;

U8	*ScreenAux ;
//U8	*ScreenTempo ;

#ifdef	MOUNFRAC
U8	*Screen ;

S32	MyKey ;
S32	MyJoy ;
S32	MyFire ;
S32	MyClick ;

U8	CurrentPalette[768] ;
#endif

#ifndef	LBA_GAME
S32	MyMouseX ;
S32	MyMouseY ;
#endif



// vars edition terrain
#ifndef	LBA_GAME
S32	IndXPolySelected = 0 ;
S32	IndZPolySelected = 0 ;
S32	PolySelectTriangle = 0 ;
#endif

S32	LastTriangle = 0 ;

// test obj animé dans decors
#ifdef	MOUNFRAC
T_OBJET	Objet ;
#endif
