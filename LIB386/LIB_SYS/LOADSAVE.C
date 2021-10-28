/*
			LOADSAVE (c) Adeline 1993

		Functions:

			- Load
			- Save
*/

#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "adeline.h"
#include "lib_sys.h"


/*──────────────────────────────────────────────────────────────────────────*/
ULONG	Load( char *name, void *buffer )
{
	LONG	handle	;
	ULONG	size	;

	size = FileSize( name )		;
	if ( !size )	return(0L)	;

	handle = OpenRead( name )	;
	if ( handle )
	{
		size = Read( handle, buffer, size ) 	;
		Close( handle )				;
		return( size )				;
	}
	return( 0L );
}
/*──────────────────────────────────────────────────────────────────────────*/

ULONG	Save( char *name, void *buffer, ULONG size )
{
	LONG	handle ;
	ULONG	nbwrote ;

	handle = OpenWrite( name ) ;
	if( handle )
	{
		nbwrote = Write( handle, buffer, size ) ;
		Close( handle ) ;
		if( nbwrote != size )	return FALSE ;
		return TRUE ;
	}
	return FALSE ;
}




