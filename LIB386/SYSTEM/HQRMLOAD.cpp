//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\adeline.h>
#include	<system\hqmem.h>
#include	<system\hqfile.h>
#include	<system\hqrmload.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32	HQRM_Load(char *name, S32 index, void **handle)
{
	U32	size	;

	size = HQF_Init(name, index)	;
	if(!size)
	{
		return FALSE		;
	}

	if(!HQM_Alloc(size, handle))	// il y a deja 500 de plus
	{
		HQF_Close()		;
		return FALSE		;
	}

	if(!HQF_LoadClose(*handle))
	{
		HQM_Free(*handle)	;
		return FALSE		;
	}

	return TRUE			;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
