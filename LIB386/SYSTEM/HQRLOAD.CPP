/*──────────────────────────────────────────────────────────────────────────*/
#include	<system\adeline.h>
#include	<system\a_malloc.h>
#include	<system\loadmall.h>
#include	<system\lz.h>
#include	<system\hqfile.h>
#include	<system\hqrload.h>

//──────────────────────────────────────────────────────────────────────────
// alloue la memoire necessaire
// et charge une fiche(index) d'une ressource .HQR
void	*LoadMalloc_HQR(char *name, S32 index)
{
	void	*ptr	;

	LoadMallocFileSize = HQF_Init(name, index);
	if(!LoadMallocFileSize)
	{
		return NULL		;
	}

	ptr = Malloc(LoadMallocFileSize+RECOVER_AREA);
	if( !ptr )
	{
		HQF_Close()		;
		LoadMallocFileSize = 0	;
		return NULL		;
	}

	LoadMallocFileSize = HQF_LoadClose(ptr);
	if(!LoadMallocFileSize)
	{
		Free(ptr)		;
		return NULL		;
	}

	Mshrink(ptr, LoadMallocFileSize);

	return ptr 			;
}

//──────────────────────────────────────────────────────────────────────────
// charge une fiche(index) à l'adresse voulue (ptrdest) depuis une ressource
U32	Load_HQR(char *name, void *ptr, S32 index)
{
	HQF_Init(name, index)		;
	return HQF_LoadClose(ptr)	;
}

//──────────────────────────────────────────────────────────────────────────
