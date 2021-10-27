//****************************************************************************
#ifndef	LIB_3D_MAT
#define	LIB_3D_MAT

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_CopyFlipMatrix(TYPE_MAT *MatDst, TYPE_MAT *MatSrc) ;

#pragma	aux Arg_CopyFlipMatrix	"*"		\
	parm caller		[edi] [esi]		\
	modify exact	[eax ebx ecx]

#pragma	aux (Arg_CopyFlipMatrix)	Func_CopyFlipMatrix

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_MAT

//****************************************************************************

