//****************************************************************************
#ifndef	LIB_3D_LROTLIST
#define	LIB_3D_LROTLIST

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_LongRotateList)(TYPE_MAT *Mat, TYPE_VT32 *Dst, TYPE_VT32 *Src, S32 NbPoints) ;

#pragma aux Arg_LongRotateList	"*"	       	\
	parm caller	[ebx] [edi] [esi] [ecx]	\
	modify		[eax edx]

#pragma	aux (Arg_LongRotateList)	Func_LongRotateList

//****************************************************************************
extern	Func_LongRotateList	*LongRotateList	;

#pragma aux	LongRotateList		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongRotateList	LongRotateListI	;
extern	Func_LongRotateList	LongRotateListF	;
extern	Func_LongRotateList	LongRotateListM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LROTLIST

//****************************************************************************

