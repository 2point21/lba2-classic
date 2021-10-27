//****************************************************************************
#ifndef	LIB_3D_ROT3D
#define	LIB_3D_ROT3D

//****************************************************************************
#include	"lrot3d.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_RotatePoint)(TYPE_MAT *Mat, S32 x, S32 y, S32 z) ;

#pragma aux Arg_RotatePoint	"*"	       	\
	parm caller	[esi] [eax] [ebx] [ecx]	\
	modify exact	[eax ebx ecx edx edi]

#pragma	aux (Arg_RotatePoint)	Func_RotatePoint

//****************************************************************************
extern	Func_RotatePoint	*RotatePoint	;
extern	Func_RotatePoint	*RotatePointNoMMX;

#pragma aux	RotatePoint		"*"
#pragma aux	RotatePointNoMMX	"*"

//****************************************************************************
#define	WorldRotatePoint(x, y, z)	RotatePoint(&MatriceWorld, x, y, z)

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_RotatePoint RotatePointI	;
#define			 RotatePointF(mat, x, y, z)	LongRotatePointF(mat, x, y, z)
extern	Func_RotatePoint RotatePointM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_ROT3D

//****************************************************************************

