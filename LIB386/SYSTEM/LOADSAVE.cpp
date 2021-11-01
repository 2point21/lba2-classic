//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <system\adeline.h>
#include <system\files.h>
#include <system\loadsave.h>

#include <stdlib.h>
#include <limits.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
U32	Load( char *name, void *buffer )
{
	S32	handle	;
	U32	size	;

	size = FileSize(name)		;
	if (!size)	return 0	;

	handle = OpenRead(name)		;
	if (handle)
	{
		size = Read(handle, buffer, size) 	;
		Close(handle)				;
		return size				;
	}
	return 0;
}
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32	LoadSize( char *name, void *buffer, S32 size )
{
	S32	handle				;
	S32	maxsize, sizeread, sizetoread	;

	maxsize = FileSize( name )	;
	if ( !maxsize )	return(0)	;

	handle = OpenRead( name )	;
	if ( handle )
	{
		sizetoread = __min(maxsize, size)		;
		sizeread = Read( handle, buffer, sizetoread )	;
		Close( handle )					;
		if(sizeread < sizetoread)
		{
			if(sizeread < maxsize)
			{
				// error reading
				sizeread = LONG_MIN		;
			}
		}
		else
		{
			if(maxsize > size)
			{
				// file too large by...
				sizeread -= maxsize		;
			}
		}
		return( sizeread )				;
	}
	return( 0 )	;
}
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
U32	Save( char *name, void *buffer, U32 size )
{
	S32	handle		;
	U32	written = 0	;

	handle = OpenWrite( name )	;
	if( handle )
	{
		written = Write( handle, buffer, size )	;
		Close( handle )				;
	}
	return( written == size )	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

