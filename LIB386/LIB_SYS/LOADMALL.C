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

#include "adeline.h"
#include "lib_sys.h"

ULONG	LoadMallocFileSize ;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
void	*LoadMalloc( char *name )
{
	LONG	handle	;
	ULONG	sizer	;
	void	*pt	;

	LoadMallocFileSize = FileSize( name )		;
	if ( !LoadMallocFileSize )	return( 0 )	;

	pt = Malloc( LoadMallocFileSize )		;
	if ( pt == 0 )	return( 0 )	;

	handle = OpenRead( name )	;
	if ( !handle)	return( 0 )	;

	sizer = Read( handle, pt, LoadMallocFileSize );
	if ( sizer != LoadMallocFileSize )
	{
		Close( handle ) ;
		return( 0 );
	}

	Close( handle )			;
	return( pt )			;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
