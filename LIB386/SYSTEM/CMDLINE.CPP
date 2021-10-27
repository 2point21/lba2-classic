//----------------------------------------------------------------------------
#include <system\adeline.h>
#include <system\a_malloc.h>
#include <system\exit.h>
#include <system\files.h>
#include <system\deffile.h>
#include <system\cmdline.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------
#define	MAX_ARGC	30
#define	MAX_EXTRA_CMD	256

//----------------------------------------------------------------------------
	S32	ArgC			;
	char	*ArgV[MAX_ARGC+1]	;
static	char	ExtraCmd[MAX_EXTRA_CMD]	;
static	char	Stop = ' '		;

//----------------------------------------------------------------------------
static inline char *GetNextStart(char *ptr)
{
	while(*ptr==' ')	ptr++	;

	if(*ptr=='"')
	{
		Stop = '"'	;
		ptr++		;
		while(*ptr==' ')	ptr++	;
	}
	else
	{
		Stop = ' '	;
	}

	return 	ptr	;
}

//----------------------------------------------------------------------------
static inline char *GetNextEnd(char *ptr)
{
	while(*ptr&&*ptr!=Stop)	ptr++	;

	if(*ptr=='"')
	{
		*ptr = ' '	;
		while(*--ptr==' ');

		ptr++	;
	}

	return	ptr	;
}

//----------------------------------------------------------------------------
void	RemoveParam(S32	param)
{
	if(param>=ArgC)	return	;

	for(; param<ArgC; param++)
	{
		ArgV[param] = ArgV[param+1]	;
	}

	ArgC--	;
}

//----------------------------------------------------------------------------
S32	FindAndRemoveParam(char *str)
{
	S32 i, res ;

	res = FALSE ;

restart:
	for(i=1; i<ArgC; i++)
	{
		if(!stricmp(ArgV[i], str))
		{
			res = TRUE	;
			RemoveParam(i)	;
			goto restart	;
		}
	}

	return res ;
}

//----------------------------------------------------------------------------
static char *GetNextParam(char **param)
{
	char	*ptr, *end ;

	ptr	= GetNextStart(*param);
	if(!*ptr)
	{
		return	NULL	;
	}

	end	= GetNextEnd(ptr);
	if(*end)
	{
		*end	= 0	;
		end++		;
	}

	*param	= end		;

	return	ptr	 ;
}

//----------------------------------------------------------------------------
static void ParseString(char *str)
{
	while(ArgC<MAX_ARGC)
	{
		char	*tmp	;

		tmp = GetNextParam(&str);
		if(!tmp)
		{
			break	;
		}

		ArgV[ArgC++] = tmp;
	}
}

//----------------------------------------------------------------------------
#ifdef	_WIN32

//----------------------------------------------------------------------------
void	GetCmdLineWin(char *cmdline)
{
	static	char modulefilename[_MAX_PATH] ;
	char	*ptr	;

	GetModuleFileName(NULL, modulefilename, _MAX_PATH)	;
	ArgV[0] = modulefilename	;
	ArgC = 1			;

	// Get "CmdLine"
	ptr = DefFileBufferReadString( "CmdLine") ;

	// process it
	if(ptr)
	{
		strncpy(ExtraCmd, ptr, MAX_EXTRA_CMD-2);
		ParseString(ExtraCmd)	;
	}

	// process cmdline
	strncpy(ExtraCmd, cmdline, MAX_EXTRA_CMD-2)	;
	ParseString(ExtraCmd)	;
}

//----------------------------------------------------------------------------
#endif//_WIN32

//----------------------------------------------------------------------------
void	GetCmdLine(S32 argc, char *argv[])
{
	char	*ptr		;

	ArgV[0] = argv[0]	;

	ArgC = 1		;

	ptr = DefFileBufferReadString( "CmdLine") ;

	if(ptr)
	{
		strncpy(ExtraCmd, ptr, MAX_EXTRA_CMD-2)	;
		ParseString(ExtraCmd)	;
	}

	argc = __min(argc-1, MAX_ARGC-ArgC)	;

	memcpy(&ArgV[ArgC], &argv[1], argc*sizeof(char *));

	ArgC += argc	;
}
//----------------------------------------------------------------------------
