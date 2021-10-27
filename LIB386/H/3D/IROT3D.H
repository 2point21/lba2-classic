//****************************************************************************
#ifndef	LIB_3D_IROT3D
#define	LIB_3D_IROT3D

//****************************************************************************
#include	"lirot3d.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_InverseRotatePoint)(TYPE_MAT *Mat, S32 x, S32 y, S32 z) ;

#pragma aux Arg_InverseRotatePoint	"*"	\
	parm caller	[esi] [eax] [ebx] [ecx]	\
	modify exact	[eax ebx ecx edx edi]

#pragma	aux (Arg_InverseRotatePoint)	Func_InverseRotatePoint

//****************************************************************************
extern	Func_InverseRotatePoint	*InverseRotatePoint	;

#pragma aux	InverseRotatePoint		"*"

//****************************************************************************
#define	WorldInverseRotatePoint(x, y, z)	InverseRotatePoint(&MatriceWorld, x, y, z)

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_InverseRotatePoint InverseRotatePointI	;
#define			 InverseRotatePointF(mat, x, y, z)	LongInverseRotatePointF(mat, x, y, z)
extern	Func_InverseRotatePoint InverseRotatePointM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_IROT3D

//****************************************************************************

