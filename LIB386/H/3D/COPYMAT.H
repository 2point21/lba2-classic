//****************************************************************************
#ifndef	LIB_3D_COPYMAT
#define	LIB_3D_COPYMAT

//****************************************************************************
#include	"mat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
extern	Func_CopyFlipMatrix	*CopyMatrix ;

#pragma	aux	CopyMatrix	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_CopyFlipMatrix	CopyMatrixI ;
extern	Func_CopyFlipMatrix	CopyMatrixF ;
extern	Func_CopyFlipMatrix	CopyMatrixM ;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_COPYMAT

//****************************************************************************

