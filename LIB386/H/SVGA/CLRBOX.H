//****************************************************************************
#ifndef	LIB_SVGA_CLRBOX
#define	LIB_SVGA_CLRBOX

//****************************************************************************
#include	<svga\dirtybox.h>
#include	<svga\box.h>

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_ClearBox(void *dst, U32 *TabOffDst, T_BOX *box) ;

#pragma aux	Arg_ClearBox	"*"		\
	parm caller	[edi] [edx] [ebx]	\
	modify	[eax ecx esi]

#pragma	aux (Arg_ClearBox)	Func_ClearBox

//****************************************************************************
extern	Func_ClearBox	*ClearBox	;

#pragma aux ClearBox	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_ClearBox	ClearBoxI	;
extern	Func_ClearBox	ClearBoxF	;
extern	Func_ClearBox	ClearBoxM	;

//****************************************************************************
extern void	SetClearColor(U32 color);

#pragma aux SetClearColor	"*"	\
	parm caller	[eax]			\
	modify	[ecx edx]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_CLRBOX

//****************************************************************************

