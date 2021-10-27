//****************************************************************************
#ifndef	LIB_3D_FLIPMAT
#define	LIB_3D_FLIPMAT

//****************************************************************************
#include	"mat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
extern	Func_CopyFlipMatrix	*FlipMatrix ;

#pragma	aux	FlipMatrix	"*"

// this routine can flip a matrix onto itself

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_CopyFlipMatrix	FlipMatrixI ;
extern	Func_CopyFlipMatrix	FlipMatrixF ;
extern	Func_CopyFlipMatrix	FlipMatrixM ;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_FLIPMAT

//****************************************************************************

