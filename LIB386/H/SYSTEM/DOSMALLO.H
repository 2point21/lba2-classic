//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	LIB_SYSTEM_DOSMALLOC
#define LIB_SYSTEM_DOSMALLOC

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	__cplusplus
extern	"C"	{
#endif

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	WIN32

#define	NormDosMalloc	NormMalloc

#else //_WIN32

extern	void	*NormDosMalloc(S32 size)	;

#pragma aux	NormDosMalloc	"*"		\
	parm caller	[ebx]			\
	modify		[eax ecx edx esi edi]

#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

#define	NormDosFree	NormFree

#else //_WIN32

extern	void	NormDosFree(void *ptr)		;

#pragma aux	NormDosFree	"*"		\
	parm caller	[edx]	  		\
	modify 		[eax ebx ecx esi edi]


#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

#define	NormDosMshrink	NormMshrink

#else //_WIN32

void	*NormDosMshrink(void *ptr, U32 size)	;

#pragma aux	NormDosMshrink	"*"	\
	parm	[edx] [ebx]	  	\
	modify 	[eax ecx esi edi]

#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

#define	DosAvailableMem()	0

#else //_WIN32

extern	U32	DosAvailableMem()		;

#pragma aux	DosAvailableMem	"*"		\
	modify 		[eax ebx ecx edx esi edi]


#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	__cplusplus
}
#endif

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_SYSTEM_DOSMALLOC

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
