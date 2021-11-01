#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"

#define	RECOVER_AREA	500

/*──────────────────────────────────────────────────────────────────────────*/

typedef	struct	{	ULONG	SizeFile ;
			ULONG	CompressedSizeFile ;
			WORD	CompressMethod ;	/* 0 stored */
							/* 1 LZS */
		}	T_HEADER ;

void	Expand( void *ptrsourcecomp, void *ptrblocdest, ULONG sizefile ) ;

/*──────────────────────────────────────────────────────────────────────────*/

LONG	HQRM_Load( UBYTE *name, UWORD index, void **ptrptr )
{
	LONG		handle ;
	UWORD		nbbloc ;
	ULONG		buffer ;
	ULONG		seekindex ;
//	UBYTE		*ptrbloc ;
	UBYTE		*ptrdecomp ;
	T_HEADER	header ;

	handle = OpenRead( name ) ;
	if( !handle )	return FALSE ;

	Read( handle, &buffer, 4L ) ;
	nbbloc = (UWORD)(buffer / 4L) ;

	if( index >= nbbloc )	return FALSE ;

	Seek( handle, index * 4L, SEEK_START ) ;
	Read( handle, &seekindex, 4L ) ;

	Seek( handle, seekindex, SEEK_START ) ;
	Read( handle, &header, sizeof( header ) ) ;

	if( !HQM_Alloc( header.SizeFile, ptrptr ) ) // il y a deja 500 de plus
	{
		return FALSE ;
	}

	switch( header.CompressMethod )
	{
		case 0:		/* Stored */
			Read(	handle,	*ptrptr, header.SizeFile ) ;
			break ;

		case 1:		/* LZS */
			ptrdecomp = (UBYTE*)(*ptrptr) + header.SizeFile - header.CompressedSizeFile + RECOVER_AREA ;
			Read(	handle, ptrdecomp, header.CompressedSizeFile ) ;
			Expand( ptrdecomp, *ptrptr, header.SizeFile ) ;
			break ;

		default:
			HQM_Free( *ptrptr ) ;
			return FALSE ;	/* UnKnown version */
	}

	Close( handle ) ;

	return header.SizeFile ;
}

