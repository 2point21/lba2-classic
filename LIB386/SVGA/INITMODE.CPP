//--------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\exit.h>
#include	<system\a_malloc.h>
#include	<system\logprint.h>
#include	<system\cmdline.h>
#include	<polygon\poly.h>
#include	<svga\clip.h>
#include	<svga\logphys.h>
#include	<svga\initmode.h>

#include	<mem.h>
#include	<ctype.h>

//--------------------------------------------------------------------------
#ifdef	_WIN32

//--------------------------------------------------------------------------
#include	<svga\ddraw.h>

//--------------------------------------------------------------------------
#else //_WIN32

//--------------------------------------------------------------------------
#include	<svga\vesa.h>
#include	<svga\copybov.h>

//--------------------------------------------------------------------------
#endif//_WIN32

//--------------------------------------------------------------------------
U32	ModeDesiredX	;
U32	ModeDesiredY	;
U32	ModeDesiredDepth;
U32	ModeDesiredSize	;
U32	ModePages	;

//--------------------------------------------------------------------------
U32	VESAMemory = VESA_LINEAR_OR_WINDOWED	;

//--------------------------------------------------------------------------
void	*PhysHard		;
void	*MemoLog		;
void	*MemoScreen		;

//----------------------------------------------------------------------------
S32	ParamsSvga()
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
		if(strnicmp("/SVGA", str, 5))
		{
			i++		;
			continue	;
		}

		RemoveParam(i)	;

		ptr = str+5	;

		if(!strnicmp("Linear=", ptr, 7))
		{
			if(atoi(ptr+7))
			{
				VESAMemory |= VESA_LINEAR	;
			}
			else
			{
				VESAMemory &= ~VESA_LINEAR	;
			}

			result = TRUE	;
		}
		else if(!strnicmp("Windowed=", ptr, 9))
		{
			if(atoi(ptr+9))
			{
				VESAMemory |= VESA_WINDOWED	;
			}
			else
			{
				VESAMemory &= ~VESA_WINDOWED	;
			}
			result = TRUE	;
		}
		else
		{
			LogPrintf("Unsupported SVGA override: %s", str)	;
		}
	}

	return	result	;
}

//--------------------------------------------------------------------------
void	ClearGraphSvga()
{
	if(MemoLog)
	{
		Free(MemoLog)			;
		Log = MemoLog = NULL		;
	}
	if(MemoScreen)
	{
		Free(MemoScreen)		;
		Screen = MemoScreen = NULL	;
	}
}

//--------------------------------------------------------------------------
static	S32	InitDesiredMode()
{
	S32	res	;
	S32	i, off	;

	if(ModeDesiredY>MAX_SCREEN_Y_RES)
	{
		return	-1	;
	}

	res = DetectInitVESAMode(ModeDesiredX, ModeDesiredY, ModeDesiredDepth, VESAMemory)	;

#ifndef	_WIN32
	CopyBoxVESA = CopyBoxVESAInit	;	// check gain for linear buffer
#endif//_WIN32

	ModeDesiredSize = ModeDesiredX*ModeDesiredY;

	SetClipWindow(0, 0, ModeDesiredX-1, ModeDesiredY-1)	;
	SetClip(0, 0, ModeDesiredX-1, ModeDesiredY-1)		;

	off = 0		; // fill the full phys table anyway
	for(i=0; i<MAX_SCREEN_Y_RES; i++)
	{
		TabOffPhysLine[i] = off	;
		off += BytesScanLine	;
	}

	off = 0		; // fill the full log table anyway
	for(i=0; i<MAX_SCREEN_Y_RES; i++)
	{
		TabOffLine[i] = off	;
		off += ModeDesiredX	;
	}

	SetScreenPitch(TabOffLine)	;

	return	res	;
}

//--------------------------------------------------------------------------
#ifdef	_WIN32

//--------------------------------------------------------------------------
S32	InitGraphSvga(U32 resx, U32 resy, U32 depth)
{
	static S32	flag = 0	;

	if(!flag)
	{
		if(!InitVESA())
		{
			LogPrintf("Error: Cannot initialize DirectDraw\n")	;
			return 1	;
		}

		atexit((PTR_VOID_FUNC)EndVESA);

		flag = 1	;
	}

	ModeDesiredX	= resx	;
	ModeDesiredY	= resy	;
	ModeDesiredDepth= depth	;

	LogPrintf("\nDesired resolution: %i*%i*%i\n", ModeDesiredX, ModeDesiredY, ModeDesiredDepth);

	if(!InitDesiredMode())
	{
		LogPuts("Error initializing video mode\n");
		return 1	;
	}

	LogPrintf("Best Mode Found   : %i*%i*%i\n", ModeResX, ModeResY, ModeDesiredDepth);
	LogPrintf("Frame buffer type : %sLinear\n",
			ModeMemory == 2 ?
			""		:
			"Virtual "					);

	if(!Log)
	{
		Log = Malloc(resx*resy+512)	;
		if(!Log)
		{
			LogPuts("Error: Not Enought Memory to allocate Log");
			return	2		;
		}
		memset(Log, 0, resx*resy+512)	;
	}

	MemoLog = Log 		;

	if(!Screen)
	{
		Screen = Malloc(resx*resy+512)	;
		if(!Screen)
		{
			LogPuts("Error: Not Enought Memory to allocate Screen");
			Free(Log)		;
			Log = NULL		;
			return	2		;
		}
		memset(Screen, 0, resx*resy+512);

	}

	MemoScreen = Screen 			;

	return	0				;
}

//--------------------------------------------------------------------------
#else //_WIN32

//--------------------------------------------------------------------------
static	char	IBM_COMP[]	="IBM COMPATIBLE   ";

//--------------------------------------------------------------------------
static	void	GetString(char *dst, char *src)
{
	int	i, j, k				;
	int	len				;
	char	*bios		= (char*)0xC0000;

	dst[0] = 0		;

	len = strlen(src)	;

	for(i=0x1E; i<0x7FFF-len; i++)
	{
		if(strnicmp(&bios[i], src, len))
		{
			continue	;
		}

		for(j=i-1; j>=0x1E; j--)
		{
			if((bios[j]<' ')||(bios[j]>127))
			{
				break	;
			}
		}
		do
		{
			j++	;
		}
		while(bios[j]==' ');

		for(k=i+7; k<0x8000; k++)
		{
			if((bios[k]<' ')||(bios[k]>127))
			{
				break	;
			}
		}
		do
		{
			k--	;
		}
		while(bios[k]==' ');

		k = k - j + 1	;

		if(k<len+2)
		{
			continue	;
		}

		if(k>255)	k = 255	;

		memcpy(dst, &bios[j], k);
		dst[k] = 0		;
		break			;
	}
}
//--------------------------------------------------------------------------
static	void	GetBiosInfo()
{
	int	i, j, k				;
	char	signature[256]	=""		;
	char	copyright[256]	=""		;
	char	version[512]	=""		;
	char	revision[256]	=""		;
	char	date[11]	=""		;
	char	*bios		= (char*)0xC0000;

	if((bios[0]!=0x55)||(bios[1]!=0xAA))	// BIOS SIGNATURE ?
	{
		return	;
	}

	if(memicmp(bios+0x1E, "IBM", 3))	// VGA BIOS ?
	{
		return	;
	}

	GetString(signature, "ibm")	;
	for(i=0; i<17; i++)
	{
		if(toupper(signature[i])!=IBM_COMP[i])
			break	;
	}
	memmove(signature, &signature[i], strlen(&signature[i])+1);

	if(!*signature)
	{
		if(!strcmp(&bios[0x31], "761295520"))
		{
			strcpy(signature, "ATI")	;
		}
		else if(!stricmp(&bios[0x7D], "VGA="))
		{
			strcpy(signature, "Paradise/Western Digital")	;
		}
		else if(!stricmp(&bios[0x08], "OAK VGA"))
		{
			strcpy(signature, "Oak Tech")	;
		}
		else if(!stricmp(&bios[0x25], "AHEAD"))
		{
			strcpy(signature, "Ahead")	;
		}
		else
		{
			U8	*ptr = (U8*)bios + *(U16*)&bios[0x37];

			if(	(ptr[0] == 0x77)		&&
				!((ptr[1]&0xF)-(ptr[1]>>4))	&&// check if XXh
				(ptr[2] == 0x99)		&&
				(ptr[3] == 0x66)		)
			{
				strcpy(signature, "Genoa")	;
			}
		}
	}

	GetString(copyright, "copyright");
	if(!*copyright)
	{
		GetString(copyright, "(c)");
	}
	if(!*copyright)
	{
		GetString(copyright, "BIOS");
	}

	GetString(version, "ver")	;
	GetString(revision, "rev")	;
	if(*revision && strcmp(version, revision))
	{
		strcat(version, ". ")	;
		strcat(version, revision);
	}

	for(i=0x1E; i<0x8000-10; i++)		// Get Date
	{
		if(bios[i]!='/')
		{
			continue	;
		}
		if(bios[i+3]!='/')
		{
			continue	;
		}
		if((bios[i+1]<'0')||(bios[i+1]>'9'))
		{
			continue	;
		}
		if((bios[i+2]<'0')||(bios[i+2]>'9'))
		{
			continue	;
		}
		for(j=i-1; j>=0x1E; j--)
		{
			if((bios[j]<'0')||(bios[j]>'9'))
			{
				break	;
			}
		}
		j++	;
		for(k=i+4; k<0x8000; k++)
		{
			if((bios[k]<'0')||(bios[k]>'9'))
			{
				break	;
			}
		}
		k -= j	;

		if((k<8)||(k>10))
		{
			continue	;
		}

		memcpy(date, &bios[j], k);
		date[k] = 0		;

		if(k==10)	// check if valid 4 digits year
		{
			int	n	;

			n = atoi(date)	;
			if((n>99)&&((n<1980)||(n>2099)))
			{
				memmove(date, date+2, 8);	// remove 2 first digits
			}
			else
			{
				n = atoi(date+6)	;
				if((n>99)&&((n<1980)||(n>2099)))
				{
					date[8] = 0;		// remove 2 last digits
				}
			}
		}

		break			;
	}

	if(!strcmp(signature, copyright))
	{
		signature[0] = 0	;
	}

	if(!strcmp(copyright, version))
	{
		version[0] = 0		;
	}

	if(!strcmp(signature, version))
	{
		version[0] = 0		;
	}

	if(*signature)
	{
		LogPrintf("VGA BIOS signature: %s\n", signature);
	}
	if(*copyright)
	{
		LogPrintf("VGA BIOS copyright: %s\n", copyright);
	}
	if(*version)
	{
		LogPrintf("VGA BIOS version  : %s\n", version);
	}
	if(*date)
	{
		LogPrintf("VGA BIOS date     : %s\n\n", date);
	}
}

//--------------------------------------------------------------------------
S32	InitGraphSvga(U32 resx, U32 resy, U32 depth)
{
	static S32	flag = 0	;

	if(!flag)
	{
		GetBiosInfo()		;

		if(!InitVESA())
		{
			LogPrintf("Error: Cannot Find VESA BIOS\n")	;
			return 1	;
		}

		LogPrintf("VESA BIOS version : %i.%i\n", PtrVbeInfoBlock->VbeVersion>>8, PtrVbeInfoBlock->VbeVersion&0xF);
		LogPrintf("OEM string        : %s\n", PtrVbeInfoBlock->VbeOemStringPtr);
		if(PtrVbeInfoBlock->VbeVersion>=0x200)
		{
			LogPrintf("Vendor name       : %s\n", PtrVbeInfoBlock->VbeOemVendorNamePtr);
			LogPrintf("OEM product name  : %s\n", PtrVbeInfoBlock->VbeOemProductNamePtr);
			LogPrintf("OEM product rev.  : %s\n", PtrVbeInfoBlock->VbeOemProductRevPtr);
		}

		atexit((PTR_VOID_FUNC)EndVESA);

		GetLastVESAMode()	;

		flag = 1		;
	}

	ModeDesiredX	= resx	;
	ModeDesiredY	= resy	;
	ModeDesiredDepth= depth	;

	LogPrintf("\nDesired resolution: %i*%i*%i\n", ModeDesiredX, ModeDesiredY, ModeDesiredDepth);

	if(InitDesiredMode()==-1)
	{
		LogPuts("Error initializing video mode");
		return 1	;
	}

	LogPrintf("VESA Mode Found   : %X (%i*%i*%i)\n", VESAMode, ModeResX, ModeResY, ModeDesiredDepth);
	LogPrintf("Frame buffer addr.: %X (Flat) %X (Physical)\n", Phys, PhysHard);
	LogPrintf("Frame buffer type : %sLinear",
			ModeMemory == 2 ?
			""		:
			"Virtual "					);

	if(Phys)
	{
		LogPuts("")	;
	}
	else
	{
		LogPrintf(", bank switching ")	;
		if(SwitchBankVESA==SwitchBankVESA1)
		{
			LogPuts("VESA 1 style (through INT 10)");
		}
		else if(SwitchBankVESA==SwitchBankVESA2)
		{
			LogPuts("VESA 2 style (direct 32 bit call)");
		}
		else if(SwitchBankVESA==SwitchBankVESA2Sel)
		{
			LogPuts("VESA 2 style (direct 32 bit call with memory mapped I/O)");
		}
		else
		{
			LogPuts("ERROR, NO BANK SWITCHING ROUTINE");
			exit(1)	;
		}
	}

	LogPrintf("Palette programing: ");
	if(SetPaletteVESA==SetPaletteVESA1)
	{
		LogPuts("VESA 1 style (through INT 10)");
	}
	else if(SetPaletteVESA==SetPaletteVESA2)
	{
		LogPuts("VESA 2 style (direct 32 bit call)");
	}
	else if(SetPaletteVESA==SetPaletteDirect)
	{
		LogPuts("Direct I/O.")	;
		LogPuts("                    "
			"WARNING: VESA BIOS doesn't support 'SET PALETTE DATA'");
		LogPuts("                    "
			"In case of problems, you might want to try with a");
		LogPuts("                    "
			"replacement VESA BIOS from your VGA card manufacturer.");
		LogPuts("                    "
			"You can also try 'Scitech Display Doctor'");
		LogPuts("                    "
			"(SDD, also called UNIVBE)."	);
	}
	else
	{
		LogPuts("ERROR, NO PALETTE ROUTINE");
		exit(1)	;
	}

	if(flag<2)
	{
		RegisterInitClear((PTR_VOID_FUNC)InitDesiredMode, (PTR_VOID_FUNC)ResetLastVESAMode);
		flag = 2	;
	}

	if(!Log)
	{
		Log = Malloc(resx*resy+512)	;
		if(!Log)
		{
			return	2		;
		}
		memset(Log,	0, resx*resy+512);
	}

	MemoLog = Log 		;

	if(!Screen)
	{
		Screen = Malloc(resx*resy+512)	;
		if(!Screen)
		{
			Free(Log)		;
			Log = NULL		;
			return	2		;
		}
		memset(Screen,	0, resx*resy+512);

	}

	MemoScreen = Screen 			;

	return	0				;
}

//--------------------------------------------------------------------------
#endif//_WIN32

//--------------------------------------------------------------------------
