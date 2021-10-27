//****************************************************************************
#ifndef	LIB_3D_LROT2D
#define	LIB_3D_LROT2D

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_LongRotate)(S32 x, S32 z, S32 angle) ;

#pragma aux Arg_LongRotate	"*"	       	\
	parm caller	[eax] [ecx] [edx]	\
	modify		[ebx esi edi]

#pragma	aux (Arg_LongRotate)	Func_LongRotate

//****************************************************************************
extern	Func_LongRotate	*LongRotate	;

#pragma aux	LongRotate		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongRotate	LongRotateI	;
extern	Func_LongRotate	LongRotateF	;
extern	Func_LongRotate	LongRotateM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LROT2D

//****************************************************************************

