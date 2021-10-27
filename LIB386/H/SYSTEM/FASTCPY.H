//****************************************************************************
#ifndef	LIB_SYSTEM_FASTCPY
#define	LIB_SYSTEM_FASTCPY

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_FastCopy(void *dst, void *src, U32 len) ;

#pragma aux	Arg_FastCopy	"*"	\
	parm caller	[edi] [esi] [ecx]	\
	modify	[eax ebx edx]

#pragma	aux (Arg_FastCopy)	Func_FastCopy

//****************************************************************************
extern	Func_FastCopy	*FastCopy	;

#pragma aux FastCopy	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_FastCopy	FastCopyI	;
extern	Func_FastCopy	FastCopyF	;
extern	Func_FastCopy	FastCopyM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SYSTEM_FASTCPY

//****************************************************************************

