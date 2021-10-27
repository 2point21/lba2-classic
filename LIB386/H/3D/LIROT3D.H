//****************************************************************************
#ifndef	LIB_3D_LIROT3D
#define	LIB_3D_LIROT3D

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_LongInverseRotatePoint)(TYPE_MAT *Mat, S32 x, S32 y, S32 z) ;

#pragma aux Arg_LongInverseRotatePoint	"*"	       	\
	parm caller	[esi] [eax] [ebx] [ecx]	\
	modify exact	[eax ebx ecx edx edi]

#pragma	aux (Arg_LongInverseRotatePoint)	Func_LongInverseRotatePoint

//****************************************************************************
extern	Func_LongInverseRotatePoint	*LongInverseRotatePoint	;

#pragma aux	LongInverseRotatePoint		"*"

//****************************************************************************
#define	LongWorldInverseRotatePoint(x, y, z)	LongInverseRotatePoint(&MatriceWorld, x, y, z)

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongInverseRotatePoint	LongInverseRotatePointI	;
extern	Func_LongInverseRotatePoint	LongInverseRotatePointF	;
extern	Func_LongInverseRotatePoint	LongInverseRotatePointM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LIROT3D

//****************************************************************************


