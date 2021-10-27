//****************************************************************************
#ifndef	LIB_3D_ROTMAT
#define	LIB_3D_ROTMAT

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_RotateMatrix)(TYPE_MAT *MatDst, TYPE_MAT *MatSrc, S32 x, S32 y, S32 z) ;

#pragma aux Arg_RotateMatrix	"*"	       		\
	parm caller	[edi] [esi] [eax] [ebx] [ecx]	\
	modify		[edx]

#pragma	aux (Arg_RotateMatrix)	Func_RotateMatrix

//****************************************************************************
extern	Func_RotateMatrix	*RotateMatrix	;

#pragma aux	RotateMatrix		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_RotateMatrix	RotateMatrixI	; // optimized for Integer
extern	Func_RotateMatrix	RotateMatrixU	; // general routine

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_ROTMAT

//****************************************************************************

