//----------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\exit.h>

#include	<string.h>
#include	<dos.h>
#include	<direct.h>
#include	<ctype.h>

//----------------------------------------------------------------------------
PTR_VOID_FUNC 	Inits[32]		;
PTR_VOID_FUNC 	Clears[32]		;
char		PathConfigFile[_MAX_PATH];
static U32	NbFuncs=0		;
static U32	DontRegister=0		;
static U32	CntClears=0		;
static char	Path1[_MAX_PATH]	;

//----------------------------------------------------------------------------
U32	RegisterInitClear(PTR_VOID_FUNC init, PTR_VOID_FUNC clear)
{
	if (DontRegister)	return 0 ;

	if (NbFuncs==32)	return 1 ;

	Inits[NbFuncs]	= init	;
	Clears[NbFuncs]	= clear	;

	NbFuncs++ 		;

	return clear ? atexit(clear) : 0 ;
}

//----------------------------------------------------------------------------
void	ClearAll()
{
	S32		i	;

	if(CntClears++)
	{
		return		;
	}

	getcwd(Path1, _MAX_PATH);

	for(i=NbFuncs-1; i>=0; i--)
	{
		if (Clears[i])	Clears[i]();
	}
}

//----------------------------------------------------------------------------
void	InitAll()
{
	char		path2[_MAX_PATH]	;
	S32		i			;
	unsigned	dummy			;
	char		full_path[ _MAX_PATH ]	;
	char 		drive[ _MAX_DRIVE ]	;
        char 		dir[ _MAX_DIR ]		;
	char		fname[ _MAX_FNAME ]	;
	char		ext[ _MAX_EXT ]		;

	if(!CntClears)
	{
		return	;
	}

	CntClears--	;

	if(CntClears)
	{
		return	;
	}

	_splitpath(PathConfigFile, drive, dir, fname, ext)	;
	_makepath(full_path, drive, dir, "", "")		;
	i = strlen(full_path)-1	;
	if(full_path[i]=='\\'||full_path[i]=='/')
	{
		full_path[i] = 0 ;
	}
	_dos_setdrive((char)(toupper(drive[0])-'A'+1), &dummy)	;
	getcwd(path2, _MAX_PATH)				;
	chdir(full_path)					;

	DontRegister = 1 ;

	for(i=0; i<NbFuncs; i++)
	{
		if (Inits[i])	Inits[i]();
	}

	DontRegister = 0 ;

	chdir(path2)			;
	_dos_setdrive((char)(toupper(Path1[0])-'A'+1), &dummy)	;
	chdir(Path1)			;
}

//----------------------------------------------------------------------------
S32	AdelineSystem(char *command)
{
	S32		ret		;

	ClearAll()			;
	ret = system(command)		;
	InitAll()			;
	return ret			;
}

//----------------------------------------------------------------------------

