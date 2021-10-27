#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "adeline.h"
#include "lib_sys.h"
#include "\projet\lib386\lib_samp\lib_wave.h"

#define	RECOVER_AREA	500

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
    Û   Û ÛßßßÛ       ÛßßßÛ Ûßßßß ÛÛßßß ÛÛßßß ÛßßßÛ Û   Û ÛßßßÛ Ûßßßß Ûßßßß
    ÛÛßßÛ ÛÛ ÜÛ       ÛÛßÛß ÛÛßß  ßßßßÛ ßßßßÛ ÛÛ  Û ÛÛ  Û ÛÛßÛß ÛÛ    ÛÛßß
    ßß  ß ßßßß  ßßßßß ßß  ß ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß ßß  ß ßßßßß ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

WORD	HQR_Flag = FALSE ;

typedef	struct	{	ULONG	SizeFile ;
			ULONG	CompressedSizeFile ;
			WORD	CompressMethod ;	/* 0 stored */
							/* 1 LZS */
		}	T_HEADER ;

void	Expand( void *ptrsourcecomp, void *ptrblocdest, ULONG sizefile ) ;

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		Û   Û ÛßßßÛ ÛßßßÛ       Û     ÛßßßÛ ÛßßßÛ ÛßßßÜ
		ÛÛßßÛ ÛÛ ÜÛ ÛÛßÛß       ÛÛ    ÛÛ  Û ÛÛßßÛ ÛÛ  Û
		ßß  ß ßßßß  ßß  ß ßßßßß ßßßßß ßßßßß ßß  ß ßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	*LoadMalloc_HQR( UBYTE *name, UWORD index )
{
	LONG		handle ;
	UWORD		nbbloc ;
	ULONG		buffer ;
	ULONG		seekindex ;
	UBYTE		*ptrbloc ;
	UBYTE		*ptrdecomp ;
	T_HEADER	header ;

	handle = OpenRead( name ) ;
	if( !handle )	return 0L ;

	Read( handle, &buffer, 4L ) ;
	nbbloc = (UWORD)(buffer / 4L) ;

	if( index >= nbbloc )
	{
		Close( handle ) ;
		return 0L ;
	}

	Seek( handle, index * 4L, SEEK_START ) ;
	Read( handle, &seekindex, 4L ) ;

	Seek( handle, seekindex, SEEK_START ) ;
	Read( handle, &header, sizeof( header ) ) ;

	ptrbloc = Malloc( header.SizeFile + RECOVER_AREA ) ;
	if( !ptrbloc )
	{
		Close( handle ) ;
		return 0L ;
	}

	switch( header.CompressMethod )
	{
		case 0:		/* Stored */
			Read(	handle,	ptrbloc, header.SizeFile ) ;
			break ;

		case 1:		/* LZS */
			ptrdecomp = ptrbloc + header.SizeFile - header.CompressedSizeFile + RECOVER_AREA ;
			Read(	handle, ptrdecomp, header.CompressedSizeFile ) ;
			Expand( ptrdecomp, ptrbloc, header.SizeFile ) ;
			break ;

		default:
			Free( ptrbloc ) ;
			Close( handle ) ;
			return 0L ;	/* UnKnown version */
	}

	Mshrink( ptrbloc, header.SizeFile ) ;
	Close( handle ) ;

	return ptrbloc ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

ULONG	Load_HQR( UBYTE *name, void *ptrdest, UWORD index )
{
	LONG		handle ;
	UWORD		nbbloc ;
	ULONG		buffer ;
	ULONG		seekindex ;
	UBYTE		*ptrdecomp ;
	T_HEADER	header ;

	handle = OpenRead( name ) ;
	if( !handle )	return 0L ;

	Read( handle, &buffer, 4L ) ;
	nbbloc = (UWORD)(buffer / 4L) ;

	if( index >= nbbloc )
	{
        	Close( handle ) ;
		return 0L ;
	}

	Seek( handle, index * 4L, SEEK_START ) ;
	Read( handle, &seekindex, 4L ) ;

	Seek( handle, seekindex, SEEK_START ) ;
	Read( handle, &header, sizeof( header ) ) ;

	switch( header.CompressMethod )
	{
		case 0:		/* Stored */
			Read(	handle,	ptrdest, header.SizeFile ) ;
			break ;

		case 1:		/* LZS */
			ptrdecomp = (UBYTE*)ptrdest + header.SizeFile - header.CompressedSizeFile + RECOVER_AREA ;
			Read(	handle, ptrdecomp, header.CompressedSizeFile ) ;
			Expand( ptrdecomp, ptrdest, header.SizeFile ) ;
			break ;

		default:
			Close( handle ) ;
			return 0L ;	/* UnKnown version */
	}

	Close( handle ) ;

	return header.SizeFile ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

ULONG	Size_HQR( char *name, UWORD index )
{
	ULONG		handle ;
	UWORD		nbbloc ;
	ULONG		buffer ;
	ULONG		seekindex ;
	UBYTE		*ptrdecomp ;
	T_HEADER	header ;

	handle = OpenRead( name ) ;
	if( !handle )	return 0 ;

	Read( handle, &buffer, 4 ) ;
	nbbloc = (UWORD)(buffer / 4) ;

	if( index >= nbbloc )
	{
		Close( handle ) ;
		return 0 ;
	}

	Seek( handle, index * 4, SEEK_START ) ;
	Read( handle, &seekindex, 4 ) ;

	if( seekindex )
	{
		Seek( handle, seekindex, SEEK_START ) ;
		Read( handle, &header, sizeof( T_HEADER ) ) ;

		Close( handle ) ;

		return header.SizeFile ;
	}
	else
	{
		Close( handle ) ;
		return 0 ;
	}
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	  Û   Û ÛßßßÛ ÛßßßÛ       ÛÜ ÜÛ ÛßßßÛ Û     Û     ÛßßßÛ Ûßßßß
	  ÛÛßßÛ ÛÛ ÜÛ ÛÛßÛß       ÛÛß Û ÛÛßßÛ ÛÛ    ÛÛ    ÛÛ  Û ÛÛ
	  ßß  ß ßßßß  ßß  ß ßßßßß ßß  ß ßß  ß ßßßßß ßßßßß ßßßßß ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

T_HQR_HEADER	*HQR_Init_Ressource(	char *hqrname,
					ULONG maxsize,
					UWORD maxindex )
{
	T_HQR_HEADER	*header ;
	void		*buffer ;

	if( !FileSize( hqrname ) )	return 0 ;	// fichier ok ?

	header =	Malloc(	sizeof(T_HQR_HEADER)
				+
				sizeof(T_HQR_BLOC) * maxindex	) ;

	if( !header )	return 0 ;			// mem ok ?

	buffer = Malloc( maxsize + RECOVER_AREA ) ;
	if( !buffer )	return 0 ;			// mem ok ?

	strcpy( header->Name, hqrname ) ;
	header->MaxSize =
	header->FreeSize = maxsize ;
	header->MaxIndex = maxindex ;
	header->NbIndex = 0 ;
	header->Buffer = buffer ;

	return header ;					// header
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	HQR_Reset_Ressource( T_HQR_HEADER *header )
{
	header->FreeSize = header->MaxSize ;
	header->NbIndex = 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

LONG	HQR_Change_Ressource( T_HQR_HEADER *header, char *newhqrname )
{
	if( !FileSize( newhqrname ) )	return FALSE ;	// fichier ok ?

	strcpy( header->Name, newhqrname ) ;

	header->FreeSize = header->MaxSize ;
	header->NbIndex = 0 ;

	return TRUE ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void    HQR_Free_Ressource( T_HQR_HEADER *header )
{
	if( header )
	{
		Free( header->Buffer ) ;
		Free( header ) ;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

UWORD	HQR_Del_Bloc( T_HQR_HEADER *header, WORD index )
{
	UWORD		n ;
	T_HQR_BLOC	*ptrbloc ;
	ULONG		delsize ;
	UBYTE		*ptr, *ptrs, *ptrd ;

	ptr = (UBYTE*)(header) + sizeof(T_HQR_HEADER) ;
	ptrbloc = (T_HQR_BLOC*)(ptr) ;
	delsize = ptrbloc[index].Size ;

	if( index < (header->NbIndex-1) )
	{
		// shift buffer
		ptrd = header->Buffer + ptrbloc[index].Offset ;
		ptrs = ptrd + delsize ;
		memmove(ptrd,
			ptrs,
			((header->Buffer+header->MaxSize) - ptrs) ) ;

		// shift index table

		ptrd = (UBYTE*)&ptrbloc[index] ;
		ptrs = ptrd + sizeof( T_HQR_BLOC ) ;
		memmove(ptrd,
			ptrs,
			(header->MaxIndex - (index + 1))*sizeof(T_HQR_BLOC) ) ;

		// shift index value

		for(n=index; n<(header->NbIndex-1); n++ )
		{
			ptrbloc[n].Offset -= delsize ;
		}
	}

	header->NbIndex-- ;
	header->FreeSize += delsize ;

	return delsize ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	*HQR_Get( T_HQR_HEADER *header, WORD index )
{
	UWORD		n, oldest ;
	ULONG		time, testtime ;
	UBYTE		*ptr ;
	T_HQR_BLOC	*ptrbloc ;
	ULONG		size ;
	ULONG		offset ;

	// ressources
	ULONG		handle ;
	UWORD		nbbloc ;
	ULONG		buffer ;
	ULONG		seekindex ;
	UBYTE		*ptrdecomp ;
	T_HEADER	lzssheader ;

	if( index < 0 )		return 0 ;

	if( (ptrbloc = HQR_GiveIndex(
				index,
				header->NbIndex,
				(UBYTE*)header+sizeof(T_HQR_HEADER) )) != 0 )
	{	// existing index
		ptrbloc->Time = TimerRef ;

		HQR_Flag = FALSE ;	// marque non chargement ressource

		return( header->Buffer + ptrbloc->Offset ) ;
	}
	else	// need load
	{
//		SaveTimer() ;
		size = Size_HQR( header->Name, index ) ;

		// load and expand hqr bloc

		handle = OpenRead( header->Name ) ;
		if( !handle )	return 0 ;

		Read( handle, &buffer, 4 ) ;
		nbbloc = (UWORD)(buffer / 4) ;

		if( index >= nbbloc )
		{
			Close( handle ) ;
			return 0 ;
		}

		Seek( handle, index * 4, SEEK_START ) ;
		Read( handle, &seekindex, 4 ) ;

		if( !seekindex )
		{
			Close( handle ) ;
			return 0 ;
		}

		Seek( handle, seekindex, SEEK_START ) ;
		Read( handle, &lzssheader, sizeof( T_HEADER ) ) ;

		// taille decompacte
		size = lzssheader.SizeFile ;

		if( !size )
		{
			Close( handle ) ;
			return 0 ;
		}

		// gestion m‚moire

		time = TimerRef ;

		ptr = (UBYTE*)(header) + sizeof(T_HQR_HEADER) ;
		ptrbloc = (T_HQR_BLOC*)(ptr) ;

		// check if enough space for bloc or index

		while( (size > header->FreeSize) OR (header->NbIndex >= header->MaxIndex) )
		{
			// delete oldest bloc
			oldest = 0 ;
			testtime = 0 ;

			for( n=0; n<header->NbIndex; n++ )
			{
				if( (time-ptrbloc[n].Time) > testtime )
				{
					testtime = time - ptrbloc[oldest].Time ;
					oldest = n ;
				}
			}
			HQR_Del_Bloc( header, oldest ) ;
		}

		// space size ok, load it

		ptr = header->Buffer + header->MaxSize - header->FreeSize ;

//		Load_HQR( header->Name, ptr, index ) ;

		switch( lzssheader.CompressMethod )
		{
			case 0:		/* Stored */
				Read(	handle,	ptr, lzssheader.SizeFile ) ;
				break ;

			case 1:		/* LZS */
				ptrdecomp = (UBYTE*)ptr + lzssheader.SizeFile - lzssheader.CompressedSizeFile + RECOVER_AREA ;
				Read(	handle, ptrdecomp, lzssheader.CompressedSizeFile ) ;
				Expand( ptrdecomp, ptr, lzssheader.SizeFile ) ;
				break ;

			default:
				Close( handle ) ;
				return 0 ;	/* UnKnown version */
		}

		Close( handle ) ;

		HQR_Flag = TRUE ;		// indicate loaded

		ptrbloc[header->NbIndex].Index  = index ;
		ptrbloc[header->NbIndex].Time   = TimerRef ;
		ptrbloc[header->NbIndex].Offset = header->MaxSize - header->FreeSize ;
		ptrbloc[header->NbIndex].Size	= size ;

		header->NbIndex++ ;
		header->FreeSize -= size ;

//		RestoreTimer() ;

		return ptr ;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

UWORD	HQR_Del_Bloc_Sample( T_HQR_HEADER *header, WORD index )
{
	UWORD		n ;
	T_HQR_BLOC	*ptrbloc ;
	ULONG		delsize ;
	UBYTE		*ptr, *ptrs, *ptrd ;

	ptr = (UBYTE*)(header) + sizeof(T_HQR_HEADER) ;
	ptrbloc = (T_HQR_BLOC*)(ptr) ;
	delsize = ptrbloc[index].Size ;

	if( index < (header->NbIndex-1) )
	{
		// shift buffer
		ptrd = header->Buffer + ptrbloc[index].Offset ;
		ptrs = ptrd + delsize ;
/*		memmove(ptrd,
			ptrs,
			((header->Buffer+header->MaxSize) - ptrs) ) ;
*/
		WaveMove(ptrd,
			 ptrs,
			 ((header->Buffer+header->MaxSize) - ptrs) ) ;

		// shift index table

		ptrd = (UBYTE*)&ptrbloc[index] ;
		ptrs = ptrd + sizeof( T_HQR_BLOC ) ;
		memmove(ptrd,
			ptrs,
			(header->MaxIndex - (index + 1))*sizeof(T_HQR_BLOC) ) ;

		// shift index value

		for(n=index; n<(header->NbIndex-1); n++ )
		{
			ptrbloc[n].Offset -= delsize ;
		}
	}

	header->NbIndex-- ;
	header->FreeSize += delsize ;

	return delsize ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	*HQR_GetSample( T_HQR_HEADER *header, WORD index )
{
	UWORD		n, oldest ;
	ULONG		time, testtime ;
	UBYTE		*ptr ;
	T_HQR_BLOC	*ptrbloc ;
	ULONG		size ;
	ULONG		offset ;

	// ressources
	ULONG		handle ;
	UWORD		nbbloc ;
	ULONG		buffer ;
	ULONG		seekindex ;
	UBYTE		*ptrdecomp ;
	T_HEADER	lzssheader ;

	if( index < 0 )		return 0 ;

	if( (ptrbloc = HQR_GiveIndex(
				index,
				header->NbIndex,
				(UBYTE*)header+sizeof(T_HQR_HEADER) )) != 0 )
	{	// existing index
		ptrbloc->Time = TimerRef ;

		HQR_Flag = FALSE ;	// marque non chargement ressource

		return( header->Buffer + ptrbloc->Offset ) ;
	}
	else	// need load
	{
//		SaveTimer() ;
		size = Size_HQR( header->Name, index ) ;

		// load and expand hqr bloc

		handle = OpenRead( header->Name ) ;
		if( !handle )	return 0 ;

		Read( handle, &buffer, 4 ) ;
		nbbloc = (UWORD)(buffer / 4) ;

		if( index >= nbbloc )
		{
			Close( handle ) ;
			return 0 ;
		}

		Seek( handle, index * 4, SEEK_START ) ;
		Read( handle, &seekindex, 4 ) ;

		if( !seekindex )
		{
			Close( handle ) ;
			return 0 ;
		}

		Seek( handle, seekindex, SEEK_START ) ;
		Read( handle, &lzssheader, sizeof( T_HEADER ) ) ;

		// taille decompacte
		size = lzssheader.SizeFile ;

		if( !size )
		{
			Close( handle ) ;
			return 0 ;
		}

		// gestion m‚moire

		time = TimerRef ;

		ptr = (UBYTE*)(header) + sizeof(T_HQR_HEADER) ;
		ptrbloc = (T_HQR_BLOC*)(ptr) ;

		// check if enough space for bloc or index

		while( (size > header->FreeSize) OR (header->NbIndex >= header->MaxIndex) )
		{
			// delete oldest bloc
			oldest = 0 ;
			testtime = 0 ;

			for( n=0; n<header->NbIndex; n++ )
			{
				if( (time-ptrbloc[n].Time) > testtime )
				{
					testtime = time - ptrbloc[oldest].Time ;
					oldest = n ;
				}
			}
			// m‚thode violente (attendre r‚flexions d‚sagr‚ables...)
			WaveStopOne( ptrbloc[oldest].Index ) ;

			HQR_Del_Bloc_Sample( header, oldest ) ;
		}

		// space size ok, load it

		ptr = header->Buffer + header->MaxSize - header->FreeSize ;

//		Load_HQR( header->Name, ptr, index ) ;

		switch( lzssheader.CompressMethod )
		{
			case 0:		/* Stored */
				Read(	handle,	ptr, lzssheader.SizeFile ) ;
				break ;

			case 1:		/* LZS */
				ptrdecomp = (UBYTE*)ptr + lzssheader.SizeFile - lzssheader.CompressedSizeFile + RECOVER_AREA ;
				Read(	handle, ptrdecomp, lzssheader.CompressedSizeFile ) ;
				Expand( ptrdecomp, ptr, lzssheader.SizeFile ) ;
				break ;

			default:
				Close( handle ) ;
				return 0 ;	/* UnKnown version */
		}

		Close( handle ) ;

		HQR_Flag = TRUE ;		// indicate loaded

		ptrbloc[header->NbIndex].Index  = index ;
		ptrbloc[header->NbIndex].Time   = TimerRef ;
		ptrbloc[header->NbIndex].Offset = header->MaxSize - header->FreeSize ;
		ptrbloc[header->NbIndex].Size	= size ;

		header->NbIndex++ ;
		header->FreeSize -= size ;

//		RestoreTimer() ;

		return ptr ;
	}
}

