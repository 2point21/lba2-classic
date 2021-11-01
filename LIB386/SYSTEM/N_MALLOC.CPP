//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<system\dosmallo.h>
#include	<system\n_malloc.h>

#include	<malloc.h>

//──────────────────────────────────────────────────────────────────────────
#define	MALLOC_MASK	(~(MALLOC_ALIGN-1))

//──────────────────────────────────────────────────────────────────────────
void	*NormMalloc(U32 size)
{
	S32	addr, temp	;

	temp = (S32)malloc(size+MALLOC_ALIGN)	;

	if(!temp)
	{
		return	NULL	;
	}

	addr = (temp+MALLOC_ALIGN)&MALLOC_MASK	;

	*((S8*)addr-2)	= 0			;
	*((S8*)addr-1)	= (S8)(addr - temp)	;

	return (void*)addr	;
}

//──────────────────────────────────────────────────────────────────────────
void	NormFree(void *addr)
{
#ifndef	_WIN32
	if(*((S8*)addr-2))
	{
		NormDosFree(addr);
	}
	else
	{
#endif//_WIN32
		addr = (void*)( (S32)addr - *((S8*)addr-1) )	;
		free(addr)	;
#ifndef	_WIN32
	}
#endif//_WIN32
}

//──────────────────────────────────────────────────────────────────────────
void	*NormMshrink(void *addr, U32 size)
{
	void	*temp	;

#ifndef	_WIN32
	if(*((S8*)addr-2))
	{
		addr = NormDosMshrink(addr, size)	;
	}
	else
	{
#endif//_WIN32
		temp = (void*)( (S32)addr - *((S8*)addr-1) )	;

		if(!_expand(temp, (size_t)(size+MALLOC_ALIGN)))
		{
			addr = NULL	;
		}
#ifndef	_WIN32
	}
#endif//_WIN32

	return addr	;
}

//──────────────────────────────────────────────────────────────────────────
