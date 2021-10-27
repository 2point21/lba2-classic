//----------------------------------------------------------------------------
#include <system\adeline.h>
#include <system\logprint.h>
#include <system\cmdline.h>
#include <system\dispos.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------
U32	OSType = -1 ;
U32	OSMajor= -1 ;
U32	OSMinor= -1 ;
char	OSString[128];

//----------------------------------------------------------------------------
#ifdef	_WIN32
void	DetectOS()
{
	OSVERSIONINFO	osVersionInfo	;

	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO) ;
	GetVersionEx(&osVersionInfo)	;

	switch(osVersionInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		OSType = OS_NT ;
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		OSType = OS_WIN95 ;
		break;

	case VER_PLATFORM_WIN32s:
		OSType = OS_WIN31 ;
		break;

	default:
		OSType = -1 ;
		break;
	}

	OSMajor = osVersionInfo.dwMajorVersion ;
	OSMinor = osVersionInfo.dwMinorVersion ;

	strcpy(OSString, osVersionInfo.szCSDVersion) ;
}
//----------------------------------------------------------------------------
#else //_WIN32

S32	DOSVer()	;

#pragma	aux DOSVer	=		\
"	mov	ah, 30h		"	\
"	int	21h		"	\
"	and	eax,0FFFFh	"	\
	modify	[ebx ecx]


void	DetectOS()
{
	S32	ver ;

	ver = DOSVer()	;

	OSType = OS_DOS	;

	OSMajor= ver & 0xFF	;
	OSMinor= (ver>>8) & 0xFF;
}

#endif//_WIN32

//----------------------------------------------------------------------------
void	DisplayOS()
{
	LogPrintf("Operating System : ");
	switch(OSType)
	{
	case OS_DOS:
		LogPuts("DOS");
		break;
	case OS_WIN31:
		LogPuts("Windows");
		break;
	case OS_WIN95:
		LogPuts("Windows 95");
		break;
	case OS_NT:
		LogPuts("NT");
		break;
	default:
		LogPuts("Unknown");
		break;
	}

	if(OSType!=-1)
	{
		LogPrintf("         Version : %d.%d %s\n", OSMajor, OSMinor, OSString);
	}
}

//----------------------------------------------------------------------------
S32	ParamsOS()
{
	char	str[256];
	char	*ptr	;
	S32	result	;
	S32	i	;

	str[256] = 0	;
	result = FALSE	;

	i = 1	;
	while(i<ArgC)
	{
		strncpy(str, ArgV[i], 255)	;
		if(strnicmp("/OS", str, 3))
		{
			i++		;
			continue	;
		}

		RemoveParam(i)	;

		ptr = str+3	;

		if(!strnicmp("Type=", ptr, 5))
		{
			OSType = atoi(ptr+5)	;
			result = TRUE	;
		}
		else if(!strnicmp("Major=", ptr, 6))
		{
			OSMajor= atoi(ptr+6)	;
			result = TRUE	;
		}
		else if(!strnicmp("Minor=", ptr, 6))
		{
			OSMinor= atoi(ptr+6)	;
			result = TRUE	;
		}
		else
		{
			LogPrintf("Unsupported OS override: %s", str)	;
		}
	}

	return	result	;
}

//----------------------------------------------------------------------------
