//****************************************************************************
#ifndef	LIB_3D_MULMAT
#define	LIB_3D_MULMAT

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_MulMatrix)(S32 *MatDst, S32 *MatSrc1, S32* MatSrc2) ;

#pragma aux Arg_MulMatrix		       	\
	parm caller		[edi] [ebx] [esi]	\
	modify exact	[eax ecx edx]

#pragma	aux (Arg_MulMatrix)	Func_MulMatrix

//****************************************************************************
extern	Func_MulMatrix	*MulMatrix	;

#pragma aux MulMatrix	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_MulMatrix	MulMatrixI	;
extern	Func_MulMatrix	MulMatrixF	;
extern	Func_MulMatrix	MulMatrixM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_MULMAT

//****************************************************************************

