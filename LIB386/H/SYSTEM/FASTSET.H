//****************************************************************************
#ifndef	LIB_SYSTEM_FASTSET
#define	LIB_SYSTEM_FASTSET

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	Func_FastSet(void *dst, S32 val, U32 len) ;

#pragma aux	Arg_FastSet	"*"	\
	parm caller	[edi] [eax] [ecx]	\
	modify	[ebx edx esi]

#pragma	aux (Arg_FastSet)	Func_FastSet

//****************************************************************************
extern	Func_FastSet	*FastSet	;

#pragma aux FastSet	"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_FastSet	FastSetI	;
extern	Func_FastSet	FastSetF	;
extern	Func_FastSet	FastSetM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SYSTEM_FASTSET

//****************************************************************************

