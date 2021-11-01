//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<system\lz.h>
#include	<system\a_malloc.h>
#include	<system\hqmem.h>

#include	<string.h>

//──────────────────────────────────────────────────────────────────────────
typedef	struct
	{
		U32	Id	;
		U32	Size	;
		void	**Ptr	;
	}	HQM_HEADER 	;

typedef	union
	{
		HQM_HEADER	*h	;
		U8		*b	;
	}	P_HQM_HEADER	;


static	P_HQM_HEADER	Ptr_HQM_Memory	= { NULL }	;
static	P_HQM_HEADER	Ptr_HQM_Next	= { NULL }	;
static	U32		Size_HQM_Memory	= 0		;
static	U32		Size_HQM_Free	= 0		;

//──────────────────────────────────────────────────────────────────────────
// Init le buffer global
S32	HQM_Init_Memory(U32 size)
{
	if( Ptr_HQM_Memory.b )
	{
		return FALSE ;
	}

	Ptr_HQM_Memory.b = (U8*)Malloc( size + RECOVER_AREA ); // recover area
	if( !Ptr_HQM_Memory.b )
	{
		return FALSE ;
	}

	Size_HQM_Memory	= size		;
	Size_HQM_Free	= size		;
	Ptr_HQM_Next	= Ptr_HQM_Memory;
	return TRUE 			;
}

//──────────────────────────────────────────────────────────────────────────
// free le buffer global
void	HQM_Clear_Memory()
{
	if( !Ptr_HQM_Memory.b )
	{
		return	;
	}

	Free( (void*)Ptr_HQM_Memory.b )	;
	Ptr_HQM_Memory.b= NULL		;
	Size_HQM_Memory	= 0		;
	Size_HQM_Free	= 0		;
}

//──────────────────────────────────────────────────────────────────────────
// alloue un bloc de memoire
S32	HQM_Alloc(U32 size, void **ptr)
{
	if(	(!Ptr_HQM_Memory.b)	||
		(size > ( Size_HQM_Free+sizeof(HQM_HEADER) ) ) )
	{
		*ptr = 0 	;
		return FALSE	;
	}

	*ptr = Ptr_HQM_Next.b + sizeof( HQM_HEADER ) 	;

	Ptr_HQM_Next.h->Id   = 0x12345678	;
	Ptr_HQM_Next.h->Size = size		;
	Ptr_HQM_Next.h->Ptr  = ptr		;

	Ptr_HQM_Next.b	+= size + sizeof( HQM_HEADER )	;
	Size_HQM_Free	-= size + sizeof( HQM_HEADER )	;

	return TRUE	;
}

//──────────────────────────────────────────────────────────────────────────
// free tous les blocs dans le buffer global
void	HQM_Free_All()
{
	if(Ptr_HQM_Memory.b)
	{
		Ptr_HQM_Next	= Ptr_HQM_Memory	;
		Size_HQM_Free	= Size_HQM_Memory	;
	}
}

//──────────────────────────────────────────────────────────────────────────
// resize le dernier bloc de memoire
void	HQM_Shrink_Last(void *ptr, U32 newsize)
{
	HQM_HEADER	*ptrh		;
	U32		deltasize	;

	if(!Ptr_HQM_Memory.b)	return 	;

	ptrh = (HQM_HEADER*)((U32)ptr - sizeof(HQM_HEADER)) ;

	if( ptrh->Id != 0x12345678 )	return ;	// erreur grave

	deltasize = ptrh->Size - newsize;

	ptrh->Size	-= deltasize 	;
	Ptr_HQM_Next.b	-= deltasize 	;
	Size_HQM_Free	+= deltasize 	;
}

//──────────────────────────────────────────────────────────────────────────
// libere un bloc de memoire et bouche le trou (remap les ptrs)
void	HQM_Free(void *ptr)
{
	P_HQM_HEADER	header			;
	U8		*ptrs			;
	U32		delsize, movesize	;

	if(!Ptr_HQM_Memory.b)	return 		;

	header.b = (U8*)ptr - sizeof(HQM_HEADER);

	if(header.h->Id != 0x12345678)	return	;	// erreur grave

	delsize = sizeof(HQM_HEADER)+header.h->Size;
	ptrs	= header.b + delsize		;
	movesize= Ptr_HQM_Next.b - ptrs		;

	memmove((void*)header.b, (void*)ptrs, movesize);

	Ptr_HQM_Next.b	-= delsize 		;
	Size_HQM_Free	+= delsize 		;

	while(header.b < Ptr_HQM_Next.b)
	{
		*(U8**)(header.h->Ptr) -= delsize		;
		header.b += header.h->Size+sizeof(HQM_HEADER)	;
	}
}

//──────────────────────────────────────────────────────────────────────────
// test la cohérence du buffer global
S32	HQM_Check()
{
	P_HQM_HEADER	ptr	;

	if(!Ptr_HQM_Memory.b)	return FALSE ;

	ptr = Ptr_HQM_Memory 	;

	while( ptr.b < Ptr_HQM_Next.b )
	{
		if( ptr.h->Id != 0x12345678 )	return FALSE	;

		ptr.b += ptr.h->Size + sizeof(HQM_HEADER)	;
	}
	return TRUE ;
}

//──────────────────────────────────────────────────────────────────────────
