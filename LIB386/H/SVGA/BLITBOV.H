//****************************************************************************
#ifndef	_WIN32

//****************************************************************************
#ifndef	LIB_SVGA_BLITBOV
#define	LIB_SVGA_BLITBOV

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_BlitBoxVESA(void *dst, void *src) ;

#pragma aux	Arg_BlitBoxVESA	"*"	\
	parm caller	[edi] [esi]	\
	modify		[eax ebx ecx edx]

#pragma	aux (Arg_BlitBoxVESA)	Func_BlitBoxVESA

//****************************************************************************
extern	Func_BlitBoxVESA	*BlitBoxVESA	;

#pragma aux BlitBoxVESA	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_BlitBoxVESA	BlitBoxVESAI	;
extern	Func_BlitBoxVESA	BlitBoxVESAF	;
extern	Func_BlitBoxVESA	BlitBoxVESAM	;

//****************************************************************************
// For the library internal use ONLY
extern	Func_BlitBoxVESA	BlitBoxVESAInit	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_BLITBOV

//****************************************************************************
#endif//_WIN32

//****************************************************************************

