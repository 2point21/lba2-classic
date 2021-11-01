//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<system\files.h>
#include	<system\a_malloc.h>
#include	<system\loadmall.h>

//──────────────────────────────────────────────────────────────────────────
U32	LoadMallocFileSize ;

//──────────────────────────────────────────────────────────────────────────
void	*LoadMalloc(char *name)
{
	S32	handle	;
	U32	size	;
	void	*ptr	;

	LoadMallocFileSize = FileSize( name )		;
	if ( !LoadMallocFileSize )	goto Error3	;

	ptr = NormMalloc( LoadMallocFileSize );
	if ( !ptr )
	{
		goto Error2		;
	}

	handle = OpenRead( name )	;
	if ( !handle)
	{
		goto Error1		;
	}

	size = Read( handle, ptr, LoadMallocFileSize );
	Close( handle )			;

	if ( size != LoadMallocFileSize )
	{
Error1:		NormFree(ptr)		;
Error2:		LoadMallocFileSize = 0	;
Error3:		ptr = NULL		;
	}

	return ptr 			;
}

//──────────────────────────────────────────────────────────────────────────
