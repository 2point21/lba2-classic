//****************************************************************************
#ifndef	LIB_3D_LROT3D
#define	LIB_3D_LROT3D

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_LongRotatePoint)(TYPE_MAT *Mat, S32 x, S32 y, S32 z) ;

#pragma aux Arg_LongRotatePoint	"*"	       	\
	parm caller	[esi] [eax] [ebx] [ecx]	\
	modify exact	[eax ebx ecx edx edi]

#pragma	aux (Arg_LongRotatePoint)	Func_LongRotatePoint

//****************************************************************************
extern	Func_LongRotatePoint	*LongRotatePoint	;

#pragma aux	LongRotatePoint		"*"

//****************************************************************************
extern	Func_LongRotatePoint	*FastLongRotatePoint	;

#pragma aux	FastLongRotatePoint	"*"

//****************************************************************************
#define	LongWorldRotatePoint(x, y, z)		LongRotatePoint(&MatriceWorld, x, y, z)

//****************************************************************************
#define	FastLongWorldRotatePoint(x, y, z)	FastLongRotatePoint(&MatriceWorld, x, y, z)

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongRotatePoint	LongRotatePointI	;
extern	Func_LongRotatePoint	LongRotatePointF	;
extern	Func_LongRotatePoint	LongRotatePointM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LROT3D

//****************************************************************************

