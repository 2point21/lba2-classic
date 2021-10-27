//****************************************************************************
#ifndef	LIB_3D_LROTRALI
#define	LIB_3D_LROTRALI

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_LongRotTransList)(TYPE_MAT *Mat, TYPE_VT16 *Dst, TYPE_VT16 *Src, S32 NbPoints) ;

#pragma aux Arg_LongRotTransList	"*"	\
	parm caller	[ebx] [edi] [esi] [ecx]	\
	modify		[eax edx]

#pragma	aux (Arg_LongRotTransList)	Func_LongRotTransList

//****************************************************************************
extern	Func_LongRotTransList	*LongRotTransList	;

#pragma aux	LongRotTransList		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongRotTransList	LongRotTransListI	;
extern	Func_LongRotTransList	LongRotTransListF	;
extern	Func_LongRotTransList	LongRotTransListM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LROTRALI

//****************************************************************************

