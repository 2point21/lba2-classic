//****************************************************************************
#ifndef	LIB_ANI_LIBINIT
#define	LIB_ANI_LIBINIT

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void	*(Func_TransNumPtr)(S32 num)	;

#pragma	aux	Arg_TransNumPtr		\
		parm caller	[esi]		\
		value	[esi]			// value returned in ESI

#pragma	aux	(Arg_TransNumPtr)	Func_TransNumPtr

//****************************************************************************
void	InitObjects(void *buffer, U32 size, Func_TransNumPtr *fctbody, Func_TransNumPtr *fctanim);

#pragma aux InitObjects	"*"    			\
	parm caller		[eax] [ecx] [edx] [ebx]	\
	modify exact 	[eax ecx edx]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_ANI_LIBINIT

//****************************************************************************

