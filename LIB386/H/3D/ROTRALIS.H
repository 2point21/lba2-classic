//****************************************************************************
#ifndef	LIB_3D_ROTRALIS
#define	LIB_3D_ROTRALIS

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_RotTransList)(TYPE_MAT *Mat, TYPE_VT16 *Dst, TYPE_VT16 *Src, S32 NbPoints) ;

#pragma aux Arg_RotTransList	"*"	       	\
	parm caller	[ebx] [edi] [esi] [ecx]	\
	modify		[eax edx]

#pragma	aux (Arg_RotTransList)	Func_RotTransList

//****************************************************************************
extern	Func_RotTransList	*RotTransList	;

#pragma aux	RotTransList		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_RotTransList	RotTransListI	;
extern	Func_RotTransList	RotTransListF	;
extern	Func_RotTransList	RotTransListM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_ROTRALIS

//****************************************************************************
