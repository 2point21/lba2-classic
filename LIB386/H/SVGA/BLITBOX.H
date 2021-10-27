//****************************************************************************
#ifndef	LIB_SVGA_BLITBOX
#define	LIB_SVGA_BLITBOX

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_BlitBox(void *dst, void *src) ;

#pragma aux	Arg_BlitBox	"*"		\
	parm caller	[edi] [esi]		\
	modify		[eax ebx ecx edx]

#pragma	aux (Arg_BlitBox)	Func_BlitBox

//****************************************************************************
extern	Func_BlitBox	*BlitBox	;

#pragma aux BlitBox	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_BlitBox	BlitBoxI	;
extern	Func_BlitBox	BlitBoxF	;
extern	Func_BlitBox	BlitBoxM	;

//****************************************************************************
// For the library internal use ONLY
extern	Func_BlitBox	BlitBoxInit	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_BLITBOX

//****************************************************************************

