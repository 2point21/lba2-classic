//--------------------------------------------------------------------------//
#ifndef	_WIN32

//--------------------------------------------------------------------------//
#include <system\adeline.h>
#include <system\files.h>
#include <system\filecopy.h>

//--------------------------------------------------------------------------//
S32    Copy( char *sname, char *dname )
{
	S32	shandle, dhandle 	;
	U32   	size, n , buf		;

	size = FileSize( sname ) 	;
	if( !size )     return FALSE 	;

	shandle = OpenRead( sname ) 	;
	if( !shandle )  return FALSE 	;

	dhandle = OpenWrite( dname ) 	;
	if( !dhandle )
	{
		Close( shandle );
		return FALSE 	;
	}

	for( n=size; n>0; n-- )	// faster and smaller to count down 8-)
	{
		U32	s;

		s = Read( shandle, &buf, 4 ) ;
		if( s != Write( dhandle, &buf, s ) )
		{
			break	;
		}
	}

	Close( shandle ) ;
	Close( dhandle ) ;

	return (n==0)	;	// if n=0 then copy is ok so return TRUE
}

//--------------------------------------------------------------------------//
#endif//_WIN32

//--------------------------------------------------------------------------//
