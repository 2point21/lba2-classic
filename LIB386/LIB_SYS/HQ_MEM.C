#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	     Û   Û Û฿฿฿Û       ÛÜ ÜÛ Û฿฿฿฿ ÛÜ ÜÛ Û฿฿฿Û Û฿฿฿Û Û  Ü฿
	     ÛÛ฿฿Û ÛÛ ÜÛ       ÛÛ฿ Û ÛÛ฿฿  ÛÛ฿ Û ÛÛ  Û ÛÛ฿Û฿ ÛÛ฿
	     ฿฿  ฿ ฿฿฿฿  ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

ULONG	Size_HQM_Memory = 0 ;
ULONG	Size_HQM_Free = 0 ;
UBYTE	*Ptr_HQM_Memory = 0 ;
UBYTE	*Ptr_HQM_Next = 0 ;

typedef	struct	{	ULONG	Id ;
			ULONG	Size ;
			void	**Ptr ;		}	HQM_HEADER ;


/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// Init le buffer global

LONG	HQM_Init_Memory( ULONG size )
{
	if( !Ptr_HQM_Memory )
	{
		Ptr_HQM_Memory = Malloc( size + 500 ) ;	// recover area
		if( Ptr_HQM_Memory )
		{
			Size_HQM_Memory = size ;
			Size_HQM_Free = size ;
			Ptr_HQM_Next = Ptr_HQM_Memory ;
			return TRUE ;
		}
	}
	return FALSE ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// free le buffer global

void	HQM_Clear_Memory()
{
	if( Ptr_HQM_Memory )
	{
		Free( Ptr_HQM_Memory ) ;
		Size_HQM_Free = 0 ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// alloue un bloc de memoire

LONG	HQM_Alloc( ULONG size, void **ptr )
{
	if( !Ptr_HQM_Memory )
	{
		*ptr = 0 ;
		return FALSE ;
	}

	if( size <= (Size_HQM_Free + sizeof( HQM_HEADER )) )
	{
		*ptr = Ptr_HQM_Next + sizeof( HQM_HEADER ) ;

		((HQM_HEADER*)Ptr_HQM_Next)->Id   = 0x12345678 ;
		((HQM_HEADER*)Ptr_HQM_Next)->Size = size ;
		((HQM_HEADER*)Ptr_HQM_Next)->Ptr  = ptr ;

		Ptr_HQM_Next  += size + sizeof( HQM_HEADER ) ;
		Size_HQM_Free -= size + sizeof( HQM_HEADER ) ;

		return TRUE ;
	}
	*ptr = 0 ;
	return FALSE ;	// pas assez de place
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// free tous les blocs dans le buffer global

void	HQM_Free_All()
{
	if( Ptr_HQM_Memory )
	{
		Ptr_HQM_Next = Ptr_HQM_Memory ;
		Size_HQM_Free = Size_HQM_Memory ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// resize le dernier bloc de memoire

void	HQM_Shrink_Last( void *ptr, ULONG newsize )
{
	HQM_HEADER	*ptrh ;
	ULONG		deltasize ;

	if( !Ptr_HQM_Memory )	return ;

	ptrh = (HQM_HEADER*)((UBYTE*)ptr - sizeof( HQM_HEADER )) ;

	if( ptrh->Id != 0x12345678 )	return ;	// erreur grave

	deltasize = ptrh->Size - newsize ;

	ptrh->Size -= deltasize ;

	Ptr_HQM_Next  -= deltasize ;
	Size_HQM_Free += deltasize ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// libere un bloc de memoire et bouche le trou (remap les ptrs)

void	HQM_Free( void *ptr )
{
	HQM_HEADER	*ptrh ;
	UBYTE		*ptrs, *ptrd, *ptrm ;
	ULONG		delsize, movesize ;

	if( !Ptr_HQM_Memory )	return ;

	ptrs = ptrd = (UBYTE*)ptr - sizeof( HQM_HEADER ) ;

	ptrh = (HQM_HEADER*)ptrd ;
	if( ptrh->Id != 0x12345678 )	return ;	// erreur grave

	delsize = sizeof( HQM_HEADER ) + ptrh->Size ;
	ptrs = ptrd + delsize ;
	movesize = (ULONG)(Ptr_HQM_Next - ptrs) ;

	ptrm = ptrs ;
	while( ptrm < Ptr_HQM_Next )
	{
		ptrh = (HQM_HEADER*)ptrm ;
		*(UBYTE*)(ptrh->Ptr) = *(UBYTE*)(ptrh->Ptr) - delsize ;
		ptrm += ptrh->Size + sizeof( HQM_HEADER ) ;
	}

	memmove( ptrd, ptrs, movesize ) ;

	Ptr_HQM_Next  -= delsize ;
	Size_HQM_Free += delsize ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// test la cohrence du buffer global

LONG	HQM_Check()
{
	HQM_HEADER	*ptrh ;
	UBYTE		*ptr ;

	if( !Ptr_HQM_Memory )	return FALSE ;

	ptr = Ptr_HQM_Memory ;

	while( ptr < Ptr_HQM_Next )
	{
		ptrh = (HQM_HEADER*)ptr ;

		if( ptrh->Id != 0x12345678 )	return FALSE ;

		ptr += ptrh->Size + sizeof( HQM_HEADER ) ;
	}
	return TRUE ;
}
