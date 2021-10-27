//----------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\logprint.h>

#include	<stdlib.h>
#include	<stdarg.h>
#include	<stdio.h>
#include	<string.h>

//----------------------------------------------------------------------------
	U32	QuietLog = FALSE	;
static	char	LogName[_MAX_PATH]=""	;
static	char	LogStr[256]		;

//----------------------------------------------------------------------------
void	CreateLog(char *fname)
{
	char 	drive[ _MAX_DRIVE ]	;
	char 	dir[ _MAX_DIR ]		;
	FILE	*logfile 		;

	_splitpath( fname, drive, dir, NULL, NULL )	;
	_makepath(LogName, drive, dir, "ADELINE", "LOG");

	logfile	= fopen(LogName, "wb")	;
	if(!logfile)
	{
		LogName[0] = 0		;
	}
	else
	{
		fclose(logfile)		;
	}
}

//----------------------------------------------------------------------------
void	LogPrintf(char *format, ...)
{
	char	*ptr	;
	va_list	arglist	;

	va_start(arglist, format);

	vsprintf(LogStr, format, arglist);

	for(ptr=LogStr; *ptr; ptr++)
	{
		if(*ptr=='\n')
		{
			memmove(ptr+2, ptr+1, strlen(ptr))	;
			*ptr++ = '\r'				;
			*ptr   = '\n'				;
		}
	}

	va_end(arglist);

	if(!QuietLog)
	{
		printf("%s", LogStr);
	}

	if(*LogName)
	{
		FILE	*logfile ;

		logfile	= fopen(LogName, "ab+");
		if(logfile)
		{
			fprintf(logfile, "%s", LogStr);
			fclose(logfile)	;
		}
	}
}

//----------------------------------------------------------------------------
void	LogPuts(char *string)
{
	LogPrintf("%s\n", string);
}

//----------------------------------------------------------------------------
