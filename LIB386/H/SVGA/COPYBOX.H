//****************************************************************************
#ifndef	LIB_SVGA_COPYBOX
#define	LIB_SVGA_COPYBOX

//****************************************************************************
#include	<svga\dirtybox.h>
#include	<svga\box.h>

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_CopyBox(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

#pragma aux	Arg_CopyBox	"*"	\
	parm caller	[edi] [esi] [edx] [ebx]	\
	modify	[eax ecx]

#pragma	aux (Arg_CopyBox)	Func_CopyBox

//****************************************************************************
extern	Func_CopyBox	*CopyBox	;

#pragma aux CopyBox	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_CopyBox	CopyBoxI	;
extern	Func_CopyBox	CopyBoxF	;
extern	Func_CopyBox	CopyBoxM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_COPYBOX

//****************************************************************************

