//--------------------------------------------------------------------------
#ifndef	LIB_SVGA_FONT
#define	LIB_SVGA_FONT

//--------------------------------------------------------------------------
#include	<svga\mask.h>

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------
extern	void	*PtrFont	;

#pragma	aux	PtrFont	"*"

//--------------------------------------------------------------------------
extern	S32	InterLeave	;
extern	S32	InterSpace	;

//--------------------------------------------------------------------------
#define	ColorFont(color)	ColorMask(color)

//--------------------------------------------------------------------------
#define	SetFont(ptrfont, interleave, interspace)	\
{							\
	PtrFont		= (ptrfont)	;		\
	InterLeave	= (interleave)	;		\
	InterSpace	= (interspace)	;		\
}

//--------------------------------------------------------------------------
extern	S32	SizeFont(char *str)	;

#pragma aux SizeFont	"*"			\
	parm caller	[esi]			\
	modify		[eax ecx edx edi]

//--------------------------------------------------------------------------
extern	S32	CarFont(S32 x, S32 y, char c)	;

#pragma aux CarFont	"*"			\
	parm caller	[ebx] [ecx] [eax]	\
	modify		[edx esi edi]

//--------------------------------------------------------------------------
extern	S32	Font(S32 x, S32 y, char *str);

#pragma aux Font	"*"			\
	parm caller	[ebx] [ecx] [esi]	\
	modify		[edx edi]

//--------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_SVGA_FONT

//--------------------------------------------------------------------------

