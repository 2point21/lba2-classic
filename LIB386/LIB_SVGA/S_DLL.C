/*──────────────────────────────────────────────────────────────────────────*
				SAMP.C 386
			      (c) Adeline 1993
 *──────────────────────────────────────────────────────────────────────────*/

#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
#include	<i86.h>

extern	void	*NewBank;
extern	void	*InitSvgaMode;
extern	void	*Enable;

char	*SvgaError = "Error SvgaDriver:";

/*-------------------------------------------------------------------------*/
LONG	SvgaInitDLL(char *driverpathname)
{
	char	*dll, *drvr;
	void	**funcs;

	//
	// Load driver file
	//

	dll = FILE_read(driverpathname, NULL);
	if (dll==NULL)
	{
		printf("%s Could not load driver '%s'.\n", SvgaError, driverpathname );
		return FALSE ;
	}

	drvr = DLL_load(dll, DLLMEM_ALLOC | DLLSRC_MEM, NULL);

	Free(dll);

	if (drvr==NULL)
	{
		printf("%s Invalid DLL image.\n", SvgaError );
		return FALSE ;
	}

	funcs = *(void ***)drvr;

	InitSvgaMode	= funcs[0];
	NewBank		= funcs[1];
	Enable		= funcs[2];

	printf("%s", drvr+4);

	return TRUE;
}

/*-------------------------------------------------------------------------*/
