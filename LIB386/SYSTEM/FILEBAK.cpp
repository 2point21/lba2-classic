//--------------------------------------------------------------------------//
#include 	<system\adeline.h>
#include 	<system\filename.h>
#include 	<system\filecopy.h>
#include 	<system\filebak.h>

#include	<stdlib.h>
#include	<string.h>

//--------------------------------------------------------------------------//
S32	CopyBak( char *name )
{
	char	bakfname[_MAX_PATH] 	;

	strcpy( bakfname, name ) 	;
	AddExt( bakfname, ".BAK" ) 	;

	return Copy( name, bakfname ) 	;
}


//--------------------------------------------------------------------------//
