//-------------------------------------------------------------------------//
#include <system\adeline.h>
#include <system\filename.h>

#include <stdlib.h>

//-------------------------------------------------------------------------//
char	*AddExt( char *path, char *ext )
{

	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR]    ;
	char	fname[_MAX_FNAME];
	char	oldext[_MAX_EXT] ;

	_splitpath( path, drive, dir, fname, oldext ) ;
	_makepath( path, drive, dir, fname, ext ) ;

	return( path )	;
}

//-------------------------------------------------------------------------//
char	*AddExtIfNot( char *path, char *ext )
{

	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR]	 ;
	char	fname[_MAX_FNAME];
	char	oldext[_MAX_EXT] ;

	_splitpath( path, drive, dir, fname, oldext ) ;

	if(!oldext[0])	_makepath( path, drive, dir, fname, ext ) ;

	return( path )	;
}

//-------------------------------------------------------------------------//
char	*GetFileName( char *pathname )
{

	char		drive[_MAX_DRIVE];
	char		dir[_MAX_DIR] 	 ;
	static char	fname[_MAX_FNAME];
	char		ext[_MAX_EXT]    ;

	_splitpath( pathname, drive, dir, fname, ext ) ;

	return( fname )	;
}

//--------------------------------------------------------------------------//
