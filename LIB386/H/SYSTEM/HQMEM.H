//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_SYSTEM_HQ_MEM
#define LIB_SYSTEM_HQ_MEM

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
// Init le buffer global
extern	S32	HQM_Init_Memory(U32 size)		;

//──────────────────────────────────────────────────────────────────────────
// free le buffer global
extern	void	HQM_Clear_Memory()			;

//──────────────────────────────────────────────────────────────────────────
// alloue un bloc de memoire
extern	S32	HQM_Alloc(U32 size, void **ptr)		;

//──────────────────────────────────────────────────────────────────────────
// free tous les blocs dans le buffer global
extern	void	HQM_Free_All()				;

//──────────────────────────────────────────────────────────────────────────
// resize le dernier bloc de memoire
extern	void	HQM_Shrink_Last(void *ptr, U32 newsize)	;

//──────────────────────────────────────────────────────────────────────────
// libere un bloc de memoire et bouche le trou (remap les ptrs)
extern	void	HQM_Free(void *ptr)			;

//──────────────────────────────────────────────────────────────────────────
// test la cohérence du buffer global
extern	S32	HQM_Check()				;

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_SYSTEM_HQ_MEM

//──────────────────────────────────────────────────────────────────────────
