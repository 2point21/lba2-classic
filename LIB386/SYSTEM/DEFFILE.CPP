//══════════════════════════════════════════════════════════════════════════
#include	<system\adeline.h>
#include 	<system\logprint.h>
#include	<system\files.h>
#include	<system\loadsave.h>
#include	<system\itoa.h>
#include	<system\deffile.h>

#include	<stdlib.h>
#include	<string.h>

//══════════════════════════════════════════════════════════════════════════
static	char	DefString[256]			;
static	S32	LenDefString			;
static	char	*PtrEndIdent			;
	char	*OrgPtrDef			;
	char	*EndPtrDef			;

	S32	DefFileReadSkipComments	= FALSE	;

static	S32	BufferSize			;
static	char	FileName[_MAX_PATH]		;

//══════════════════════════════════════════════════════════════════════════
static inline S32 ReadBufferString(char *identificateur)
{
	char	*ptrdef	;
	char	*ptr	;
	S32	len	;

	DefString[0]	= 0	;
	LenDefString	= 0	;

	if(!OrgPtrDef)
	{
		return FALSE	;
	}

	len = strlen(identificateur);

	ptrdef = OrgPtrDef ;

	while(ptrdef<EndPtrDef)
	{
		while (*ptrdef==32)	ptrdef++	;	// skip blanks

		if(!strnicmp(ptrdef,identificateur, len))	// look for ident
		{
			ptrdef+=len;

			while (*ptrdef==32)	ptrdef++;	// skip blanks

			if ((*ptrdef==':') OR (*ptrdef=='='))
			{
				ptrdef++	;

				PtrEndIdent = ptrdef ;

				while(*ptrdef==32) 	ptrdef++;//skip leading spaces

				ptr = ptrdef;

				while(*ptr>=32)
				{
					if(DefFileReadSkipComments&&(*ptr==';'))
					{
						ptr--	;
						break	;
					}
					ptr++;			// search EOL
				}

				while(*ptr<=32)    	ptr--;	// skip ending spaces

				LenDefString = ptr-ptrdef+1;

				if(LenDefString<0)	LenDefString = 0;	// security
				if(LenDefString>255)	LenDefString = 255;

				memcpy(DefString, ptrdef, LenDefString);	// copy line into buffer
				DefString[LenDefString] = 0 ;

				return TRUE ;
			}
		}

		while(*ptrdef>=32) ptrdef++;	// next line
		while(*ptrdef<32)  ptrdef++;
	}
	return FALSE ;
}

//══════════════════════════════════════════════════════════════════════════
S32	DefFileBufferInit(char *file, void *buffer, S32 maxsize)
{
	S32	buffersize	;

	buffersize	= FileSize(file);

	OrgPtrDef = NULL	;

	if(!buffer)
	{
		LogPuts("Error: Trying to use DefFileBufferInit with a NULL pointer");
		return FALSE 	;
	}

	if(buffersize>=maxsize-3)
	{
		LogPuts("Error: Buffer too small in DefFileBufferInit");
		return FALSE 	;
	}

	Load( file, buffer ) 	;

	strcpy(FileName, file)	;

	BufferSize	= maxsize		;
	OrgPtrDef	= (char*)buffer		;
	EndPtrDef	= OrgPtrDef + buffersize;
	do
	{
		EndPtrDef--	;
		if(EndPtrDef<OrgPtrDef)
		{
			break	;
		}
	}
	while(*EndPtrDef<=32)	;

	EndPtrDef++		;

	EndPtrDef[0]	= 0	;
	EndPtrDef[1]	= 32	;// Rajoute une ligne avec un espace tout à la fin
	EndPtrDef[2]	= 0	;

	return TRUE ;
}

//══════════════════════════════════════════════════════════════════════════
char	*DefFileBufferReadString(char *ident)
{
	char	*result ;

	if(ReadBufferString( ident ))
	{
		result = DefString;
	}
	else
	{
		result = NULL;
	}

	return result;
}

//══════════════════════════════════════════════════════════════════════════
char	*DefFileBufferReadStringDefault(char *ident, char *def)
{
	char	*temp	;

	temp = DefFileBufferReadString(ident)	;

	return temp ? temp : def ;
}

//══════════════════════════════════════════════════════════════════════════
S32	DefFileBufferReadValue2(char *ident, S32 *result)
{
	char	*ptrdef	;
	S32	i 	;
	char	c	;
	S32	value	;

	DefFileBufferReadString( ident ) ;
	if(!LenDefString)
	{
		return FALSE ;
	}

	ptrdef = DefString;
	while(*ptrdef>32)	ptrdef++;
	*ptrdef=0;
	LenDefString=ptrdef-DefString;

	c =(char)(DefString[LenDefString-1]&~32) ;
	if( c == 'H' )
	{
		// hexa
		value = 0 ;
		for( i=0; i<strlen(DefString)-1; i++ )
		{
			c = DefString[i] ;
			if( c <= '9' )  c -= '0' ;
			else		c = (char)((c&~32)-'A'+10) ;
			value = value*16 + c ;
		}
	}
	else
	{
		// deci
		value = atoi( DefString ) ;
	}

	*result = value;
	return TRUE;
}

//══════════════════════════════════════════════════════════════════════════
S32	DefFileBufferReadValueDefault(char *ident, S32 def)
{
	DefFileBufferReadValue2( ident, &def)	;

	return	def	;
}

//══════════════════════════════════════════════════════════════════════════
S32	DefFileBufferReadValue(char *ident)
{
	return DefFileBufferReadValueDefault( ident, -1)	;
}

//══════════════════════════════════════════════════════════════════════════
S32	DefFileBufferWriteString(char *ident, char *string)
{
	char	crlf[2]={13, 10}	;
	S32	handle			;
	char	*result			;

	if(!OrgPtrDef)
	{
		return FALSE		;
	}

	result = DefFileBufferReadString( ident );

	handle = OpenWrite( FileName ) ;
	if( !handle )
	{
		return FALSE ;
	}

	if(result)	// ident found
	{
		Write( handle, OrgPtrDef, PtrEndIdent-OrgPtrDef ) ;

		while(*PtrEndIdent>=32) PtrEndIdent++;	// skip until EOL
		if(*PtrEndIdent==13)	PtrEndIdent++;
		if(*PtrEndIdent==10)	PtrEndIdent++;
		if(PtrEndIdent>EndPtrDef)
		{
			PtrEndIdent=EndPtrDef;
		}
	}
	else		// ident not found
	{
		if( OrgPtrDef ) // file exist
		{
			Write( handle, OrgPtrDef, EndPtrDef-OrgPtrDef ) ;
		}
		Write( handle, crlf, 2 ) ;
		Write( handle, ident, strlen(ident) ) ;
		Write( handle, ":", 1 ) ;
	}

	// write new value
	Write( handle, " ", 1 ) ;
	Write( handle, string, strlen( string ) ) ;
	Write( handle, crlf, 2 ) ;

	if(result)
	{
		// end of file
		Write( handle, PtrEndIdent, EndPtrDef-PtrEndIdent ) ;
	}

	Close( handle ) ;

	DefFileBufferInit( FileName, (void*)OrgPtrDef, BufferSize)	;

	return TRUE;
}

//══════════════════════════════════════════════════════════════════════════
