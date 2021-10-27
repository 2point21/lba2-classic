/*
			MALLOC (c) Adeline 1993

		Functions:

			- Malloc
			- Free
			- Mshrink
*/

#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

#include "adeline.h"
#include "lib_sys.h"

struct meminfo {
    unsigned LargestBlockAvail;
    unsigned MaxUnlockedPage;
    unsigned LargestLockablePage;
    unsigned LinAddrSpace;
    unsigned NumFreePagesAvail;
    unsigned NumPhysicalPagesFree;
    unsigned TotalPhysicalPages;
    unsigned FreeLinAddrSpace;
    unsigned SizeOfPageFile;
    unsigned Reserved[3];
} MemInfo;

#define DPMI_INT	0x31


// #define	DEBUG_MALLOC	1

#ifdef	DEBUG_MALLOC
LONG	ModeTraceMalloc = FALSE ;
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
/*	Special, Allocate Memory Under First Meg			    */
void	*DosMalloc( LONG size, ULONG *handle )
{

/*	union	REGS	r	;

	r.x.eax = 0x0100	;//	Function allocate Dos Memory
	r.x.ebx =(size+15)>>4	;//	Number off Paragraphs Requested
	int386( 0x31, &r, &r )	;//	Invoke DPMI
	if( r.x.cflag )
		return(0)	;//	Failed
	return( (void*)((r.x.eax & 0xFFFF) << 4 )) ;//	Ok, Take this!
*/
	union	REGS	r	;
	ULONG	strat		;
	ULONG	addr		;

#ifdef	jhjkhjkhjlk
	r.x.eax = 0x5800	;
	int386( 0x21, &r, &r )	;
	strat	= r.x.eax	;/*	Save DOS alloc strategie	*/

	r.x.eax = 0x5801	;
	r.x.ebx = 1		;/*	Low mem, best fit		*/
	int386( 0x21, &r, &r )	;/*	Set alloc strategie		*/
#endif
	r.x.eax = 0x0100	;/*	Function allocate Dos Memory	*/
	if (size == -1)
		r.x.ebx = -1	;/*Number off Paragraphs Requested	*/
	else
		r.x.ebx =(size+15)>>4;/*Number off Paragraphs Requested	*/
	int386( 0x31, &r, &r )	;/*	Invoke DPMI			*/

	addr	= 0		;
	if (size == -1)
	{
		if( r.x.cflag )
			addr = (r.x.ebx & 0xFFFF) << 4	;
	}
	else
	{
		if( !r.x.cflag )
		{
			if (handle) *handle = r.x.edx & 0xFFFF;/*	DPMI selector*/
			addr = (r.x.eax & 0xFFFF) << 4;/*	Ok, Take this!	*/
		}
	}

#ifdef	hjhkjhlhjk
	r.x.eax = 0x5801	;
	r.x.ebx = strat		;/*	Odl startegie			*/
	int386( 0x21, &r, &r )	;/*	Set alloc strategie		*/
#endif
	return((void *)addr)	;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
/*	Special, Free Allocated Memory Under First Meg			    */
void	DosFree( ULONG handle )
{
	union	REGS	r	;

	r.x.eax = 0x0101	;/*	Function allocate Dos Memory	*/
	r.x.edx = handle	;/*	DPMI Selector			*/
	int386( 0x31, &r, &r )	;/*	Invoke DPMI			*/
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifdef	DEBUG_MALLOC

LONG	mymalloc( LONG lenalloc, void **memptr )
{
	union REGS regs		;
	struct SREGS sregs	;
	UBYTE	*ptr		;

	lenalloc+=16;
	regs.x.eax = 0x00000501		;
	regs.x.ebx = lenalloc>>16 ;
	regs.x.ecx = lenalloc & 0xFFFF ;
	sregs.es = 0		;
	sregs.ds = 0		;
	sregs.fs = 0		;
	sregs.gs = 0		;
	int386x( DPMI_INT, &regs, &regs, &sregs )	;

	if( regs.x.cflag )
	{
		*memptr = NULL ;
		return regs.x.eax ;
	}

	ptr=(UBYTE *)((regs.x.ebx<<16) | (regs.x.ecx & 0xFFFF));

	*(WORD*)(ptr+0)=(WORD)regs.x.esi ;
	*(WORD*)(ptr+2)=(WORD)regs.x.edi ;

	*memptr=(void*)(ptr+16) ;
	return 0 ;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

LONG	myfree( void *ptr )
{
	union REGS regs		;
	struct SREGS sregs	;
	UBYTE	*ptrh ;

	ptrh = ptr ;
	ptrh -= 16 ;

	regs.x.esi = *(WORD*)(ptrh+0) ;
	regs.x.edi = *(WORD*)(ptrh+2) ;
	regs.x.eax = 0x00000502		;
	sregs.es = 0		;
	sregs.ds = 0		;
	sregs.fs = 0		;
	sregs.gs = 0		;
	int386x( DPMI_INT, &regs, &regs, &sregs )	;

	return regs.x.cflag ? regs.x.eax : 0 ;
}

#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
void	*SmartMalloc( LONG lenalloc )
{
	union	REGS	r	;
	ULONG	strat		;
	ULONG	addr		;

#ifdef	jhghhlk
	r.x.eax = 0x5800	;
	int386( 0x21, &r, &r )	;
	strat	= r.x.eax	;/*	Save DOS alloc strategie	*/

	r.x.eax = 0x5801	;
	r.x.ebx = 0x81		;/*	UMB first then Low mem, best fit*/
	int386( 0x21, &r, &r )	;/*	Set alloc strategie		*/
#endif
	r.x.eax = 0x0100	;/*	Function allocate Dos Memory	*/
	r.x.ebx =(lenalloc+15)>>4;/*Number off Paragraphs Requested	*/
	int386( 0x31, &r, &r )	;/*	Invoke DPMI			*/

	if( !r.x.cflag )
		addr = (r.x.eax & 0xFFFF) << 4;/*	Ok, Take this!	*/
	else
		addr = (ULONG)malloc( lenalloc );
#ifdef	hjgkgjk
	r.x.eax = 0x5801	;
	r.x.ebx = strat		;/*	Odl startegie			*/
	int386( 0x21, &r, &r )	;/*	Set alloc strategie		*/
#endif
	return((void *)addr)	;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
void	*Malloc( LONG lenalloc )
{
	union REGS regs		;
	struct SREGS sregs	;
	void	*ptr ;
	FILE	*fh ;
	ULONG	size, error ;

	if ( lenalloc != -1 )
	{
#ifdef	DEBUG_MALLOC
		if( ModeTraceMalloc )
		{
			fh = fopen( "c:\\malloc.lst","a+t" ) ;
			if( fh != NULL )
			{
				regs.x.eax = 0x00000500		;
				sregs.es = FP_SEG( &MemInfo )	;
				sregs.ds = 0		;
				regs.x.edi = FP_OFF( &MemInfo )	;
				int386x( DPMI_INT, &regs, &regs, &sregs )	;
				size = MemInfo.LargestBlockAvail ;

				fprintf( fh, "Mem: %d\n", size ) ;
			}

			error = mymalloc(lenalloc, &ptr) ;
//			ptr = malloc( lenalloc ) ;

			if( fh != NULL )
			{
				fprintf( fh, "Malloc: %d bytes at %X to %X\n", lenalloc, ptr, (LONG)ptr+lenalloc ) ;
				if( error )
					fprintf( fh, "Error: %X\n", error ) ;

				regs.x.eax = 0x00000500		;
				sregs.es = FP_SEG( &MemInfo )	;
				sregs.ds = 0		;
				regs.x.edi = FP_OFF( &MemInfo )	;
				int386x( DPMI_INT, &regs, &regs, &sregs )	;
				size = MemInfo.LargestBlockAvail ;

				fprintf( fh, "Mem: %d\n", size ) ;

				fclose( fh ) ;
			}
		}
		else
#endif
		{
			ptr = malloc( lenalloc ) ;
		}

		return( ptr )	;

	}
	regs.x.eax = 0x00000500		;
	sregs.es = FP_SEG( &MemInfo )	;
	sregs.ds = 0		;
	regs.x.edi = FP_OFF( &MemInfo )	;

	int386x( DPMI_INT, &regs, &regs, &sregs )	;

	return( (void *)MemInfo.LargestBlockAvail )	;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
void	Free( void *buffer )
{
	union REGS regs		;
	struct SREGS sregs	;
	void	*ptr ;
	FILE	*fh ;
	ULONG	size, error ;

#ifdef	DEBUG_MALLOC
	if( ModeTraceMalloc )
	{
		fh = fopen( "c:\\malloc.lst","a+t" ) ;
		if( fh != NULL )
		{
			regs.x.eax = 0x00000500		;
			sregs.es = FP_SEG( &MemInfo )	;
			sregs.ds = 0		;
			regs.x.edi = FP_OFF( &MemInfo )	;
			int386x( DPMI_INT, &regs, &regs, &sregs )	;
			size = MemInfo.LargestBlockAvail ;

			fprintf( fh, "Mem: %d\n", size ) ;
		}

		error = myfree( buffer ) ;
//		free( buffer ) ;

		if( fh != NULL )
		{
			fprintf( fh, "Free: %X\n", buffer ) ;
			if( error )
				fprintf( fh, "Error: %X\n", error ) ;

			regs.x.eax = 0x00000500		;
			sregs.es = FP_SEG( &MemInfo )	;
			sregs.ds = 0		;
			regs.x.edi = FP_OFF( &MemInfo )	;
			int386x( DPMI_INT, &regs, &regs, &sregs )	;
			size = MemInfo.LargestBlockAvail ;

			fprintf( fh, "Mem: %d\n", size ) ;

			fclose( fh ) ;
		}
	}
	else
#endif
	{
		free( buffer ) ;
	}
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
void	*Mshrink( void *buffer, ULONG taille )
{
	return _expand( buffer, (size_t)taille )	;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
