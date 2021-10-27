//****************************************************************************
#ifndef	LIB_3D_ROT2D
#define	LIB_3D_ROT2D

//****************************************************************************
#include	"lrot2d.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_Rotate)(S32 x, S32 z, S32 angle) ;

#pragma aux Arg_Rotate	"*"	       		\
	parm caller	[eax] [ecx] [edx]	\
	modify		[ebx esi edi]

#pragma	aux (Arg_Rotate)	Func_Rotate

//****************************************************************************
extern	Func_Rotate	*Rotate	;

#pragma aux	Rotate		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_Rotate	RotateI	;
#define			RotateF(x, z, angle)	LongRotateF(x, z, angle)
extern	Func_Rotate	RotateM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_ROT2D

//****************************************************************************

