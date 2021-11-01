//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_SYSTEM_AVAILMEM
#define LIB_SYSTEM_AVAILMEM

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
extern	U32	AvailableMem()	;

#ifndef	_WIN32

#pragma aux	AvailableMem		\
	modify	[ebx ecx edx esi edi]

#endif//_WIN32

//──────────────────────────────────────────────────────────────────────────

#ifdef	__cplusplus
}
#endif

#endif//LIB_SYSTEM_AVAILMEM

//──────────────────────────────────────────────────────────────────────────
