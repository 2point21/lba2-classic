//****************************************************************************
#ifndef	LIB_3D_ROTLIST
#define	LIB_3D_ROTLIST

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_RotateList)(TYPE_MAT *Mat, TYPE_VT16 *Dst, TYPE_VT16 *Src, S32 NbPoints) ;

#pragma aux Arg_RotateList	"*"	       	\
	parm caller	[ebx] [edi] [esi] [ecx]	\
	modify		[eax edx]

#pragma	aux (Arg_RotateList)	Func_RotateList

//****************************************************************************
extern	Func_RotateList	*RotateList	;

#pragma aux	RotateList		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_RotateList	RotateListI	;
extern	Func_RotateList	RotateListF	;
extern	Func_RotateList	RotateListM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_ROTLIST

//****************************************************************************

