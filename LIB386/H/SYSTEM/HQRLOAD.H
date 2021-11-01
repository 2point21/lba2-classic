//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
#ifndef	LIB_SYSTEM_HQRLOAD
#define LIB_SYSTEM_HQRLOAD

#ifdef	__cplusplus
extern	"C"	{
#endif

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
// alloue la memoire necessaire et charge une fiche d'une ressource .HQR
extern	void	*LoadMalloc_HQR(char *name, S32 index)		;

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
// charge une fiche … l'adresse voulue depuis une ressource
extern	U32	Load_HQR(char *name, void *ptr, S32 index)	;

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
#ifdef	__cplusplus
}
#endif

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
#endif//LIB_SYSTEM_HQRLOAD

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД

