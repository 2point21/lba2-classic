//****************************************************************************
#ifndef	LIB_3D_IMATSTD
#define	LIB_3D_IMATSTD

//****************************************************************************
#include	"imat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
extern	Func_InitMatrix	*InitMatrixStd	;

#pragma aux	InitMatrixStd	"*"

//****************************************************************************
// These should not be called directly but through the above pointer
extern	Func_InitMatrix	InitMatrixStdI	;
extern	Func_InitMatrix	InitMatrixStdF	;
extern	Func_InitMatrix	InitMatrixStdM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_IMATSTD

//****************************************************************************

