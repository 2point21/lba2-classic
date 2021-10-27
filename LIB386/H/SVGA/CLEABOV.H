//****************************************************************************
#ifndef	_WIN32

//****************************************************************************
#ifndef	LIB_SVGA_CLEABOV
#define	LIB_SVGA_CLEABOV

//****************************************************************************
#include	<svga\dirtybox.h>
#include	<svga\box.h>

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_ClearBoxVESA(void *dst, U32 *TabOffDst, T_BOX *box) ;

#pragma aux	Arg_ClearBoxVESA	"*"	\
	parm caller	[edi] [edx] [ebx]	\
	modify		[eax ecx esi]

#pragma	aux (Arg_ClearBoxVESA)	Func_ClearBoxVESA

//****************************************************************************
extern	Func_ClearBoxVESA	*ClearBoxVESA	;

#pragma aux ClearBoxVESA	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_ClearBoxVESA	ClearBoxVESAI	;
extern	Func_ClearBoxVESA	ClearBoxVESAF	;
extern	Func_ClearBoxVESA	ClearBoxVESAM	;

//****************************************************************************
// For the library internal use ONLY
extern	Func_ClearBoxVESA	ClearBoxVESAInit;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_CLEABOV

//****************************************************************************
#endif//_WIN32

//****************************************************************************

