//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_SYSTEM_HQRLOAD
#define LIB_SYSTEM_HQRLOAD

#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
// alloue la memoire necessaire et charge une fiche d'une ressource .HQR
extern	void	*LoadMalloc_HQR(char *name, S32 index)		;

//──────────────────────────────────────────────────────────────────────────
// charge une fiche à l'adresse voulue depuis une ressource
extern	U32	Load_HQR(char *name, void *ptr, S32 index)	;

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_SYSTEM_HQRLOAD

//──────────────────────────────────────────────────────────────────────────

