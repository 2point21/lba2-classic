//****************************************************************************
#ifndef	LIB_POLY_TESTVUE
#define	LIB_POLY_TESTVUE

//****************************************************************************
#include	<polygon\poly.h>

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	S32 (Func_TestVuePoly)(Struc_Point *ptr) ;

#pragma aux Arg_TestVuePoly	"*"	       	\
	parm caller	[esi]			\
	modify		[eax ebx ecx edx edi]

#pragma	aux (Arg_TestVuePoly)	Func_TestVuePoly

//****************************************************************************
extern	Func_TestVuePoly	*TestVuePoly;

#pragma aux	TestVuePoly		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_TestVuePoly	TestVuePolyI;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_POLY_TESTVUE

//****************************************************************************

