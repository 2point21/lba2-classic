//****************************************************************************
#ifndef	LIB_3D_IMAT3DS
#define	LIB_3D_IMAT3DS

//****************************************************************************
#include	"imat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
extern	Func_InitMatrix	*InitMatrix3DS	;

#pragma	aux	InitMatrix3DS	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_InitMatrix	InitMatrix3DSI	;
extern	Func_InitMatrix	InitMatrix3DSF	;
extern	Func_InitMatrix	InitMatrix3DSM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_IMAT3DS

//****************************************************************************

