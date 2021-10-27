//****************************************************************************
#ifndef	_WIN32

//****************************************************************************
#ifndef	LIB_SVGA_COPYBOV
#define	LIB_SVGA_COPYBOV

//****************************************************************************
#include	<svga\dirtybox.h>
#include	<svga\box.h>

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_CopyBoxVESA(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

#pragma aux	Arg_CopyBoxVESA	"*"	\
	parm caller	[edi] [esi] [edx] [ebx]	\
	modify	[eax ecx]

#pragma	aux (Arg_CopyBoxVESA)	Func_CopyBoxVESA

//****************************************************************************
extern	Func_CopyBoxVESA	*CopyBoxVESA	;

#pragma aux CopyBoxVESA	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_CopyBoxVESA	CopyBoxVESAI	;
extern	Func_CopyBoxVESA	CopyBoxVESAF	;
extern	Func_CopyBoxVESA	CopyBoxVESAM	;

//****************************************************************************
// For the library internal use ONLY
extern	Func_CopyBoxVESA	CopyBoxVESAInit	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_COPYBOV

//****************************************************************************
#endif//_WIN32

//****************************************************************************

