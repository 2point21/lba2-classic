/*
			DEF_FILE (c) Adeline 1993

*/

#include 	"\projet\lib386\lib_sys\adeline.h"
#include 	"\projet\lib386\lib_sys\lib_sys.h"

#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/*══════════════════════════════════════════════════════════════════════════*
		█▀▀▀▄ █▀▀▀▀ █▀▀▀▀       █▀▀▀▀  █    █     █▀▀▀▀
		██  █ ██▀▀  ██▀▀        ██▀▀   ██   ██    ██▀▀
		▀▀▀▀  ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀     ▀▀   ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

UBYTE	DefString[256] ;
UBYTE	DefValue[50] ;
LONG	DefHandle ;
//LONG	DefHandleC ;
//LONG	DefModeCopy = FALSE ;
UBYTE	*PtrDef ;
UBYTE	*OrgPtrDef ;
UBYTE	*EndPtrDef ;

/*══════════════════════════════════════════════════════════════════════════*/
#ifdef	OCAZOU
void	NextLine()
{
	UBYTE	c ;

	do	/* saute cr/lf espace */
	{
		if( Read( DefHandle, &c, 1L ) == 0L )	return ;

		if( DefModeCopy )	Write( DefHandleC, &c, 1L ) ;
	}
	while( c >= 32 ) ;
}
#endif
/*══════════════════════════════════════════════════════════════════════════*/
/*
LONG	ReadWord()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	do	// saute cr/lf espace
	{
		if( Read( DefHandle, &c, 1L ) == 0L )	return FALSE ;

		if( DefModeCopy )	Write( DefHandleC, &c, 1L ) ;
	}
	while( c <= 32 ) ;

	do	// lit mot
	{
		*ptr++ = c ;
		if( Read( DefHandle, &c, 1L ) == 0L )	c = 26 ;
		else
			if( DefModeCopy )	Write( DefHandleC, &c, 1L ) ;
	}
	while( c > 32 ) ;

	*ptr++ = 0 ;

	return	TRUE ;
}
*/

LONG	PtrDefReadWord()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	do	/* saute cr/lf espace */
	{
		c = *PtrDef++ ;
		if( PtrDef >= EndPtrDef )	return FALSE ;
	}
	while( c <= 32 ) ;

	do	/* lit mot */
	{
		*ptr++ = c ;
		c = *PtrDef++ ;
		if( PtrDef >= EndPtrDef )	c = 26 ;
	}
	while( c > 32 ) ;

	*ptr++ = 0 ;

	return	TRUE ;
}

LONG	PtrDefReadIdent()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	do	/* saute cr/lf espace */
	{
		c = *PtrDef++ ;
		if( PtrDef >= EndPtrDef )	return FALSE ;
	}
	while( c <= 32 ) ;

	do	/* lit mot */
	{
		*ptr++ = c ;
		c = *PtrDef++ ;
		if( PtrDef >= EndPtrDef )	c = 26 ;
	}
	while( (c >= 32) AND (c != ':') ) ;

	while ( *--ptr == 32 ) ;

	*(ptr+1) = 0 ;

	return	TRUE ;
}

/*══════════════════════════════════════════════════════════════════════════*/

/*
LONG	ReadString()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	do	// saute cr/lf espace
	{
		if( Read( DefHandle, &c, 1L ) == 0L )	return FALSE ;
	}
	while( c <= 32 ) ;

	do	// lit phrase jusqu'a CR/LF
	{
		*ptr++ = c ;
		if( Read( DefHandle, &c, 1L ) == 0L )	c = 13 ;
	}
	while( c != 13 ) ;
	Read( DefHandle, &c, 1L ) ;	// lit LF ou rien si fin

	*ptr++ = 0 ;

	return	TRUE ;
}
*/
void	PtrDefReadString()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	*ptr = 0 ;

	do	// saute espace / tab
	{
		c = *PtrDef++ ;
		if(( PtrDef >= EndPtrDef )	OR
		   ( c == 13 )			OR
		   ( c == 10 )			   )
		   return;
	}
	while( c <= 32 ) ;

	do	/* lit phrase jusqu'a CR/LF */
	{
		*ptr++ = c ;
		c = *PtrDef++ ;
		if( PtrDef >= EndPtrDef )	c = 13 ;
	}
	while( c >= 32 ) ;

	while ( *--ptr == 32 ) ;

//	PtrDef++ ;			/* lit LF ou rien si fin */

	*(ptr+1) = 0 ;
}
/*══════════════════════════════════════════════════════════════════════════*/
/*
LONG	ReadThisString()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	do	// saute espace
	{
		if( Read( DefHandle, &c, 1L ) == 0L )
		{
			*ptr = 0 ;
			return TRUE ;
		}
	}
	while( c == 32 ) ;

	if( c > 31 )
	{
		do	// lit phrase jusqu'a CR/LF
		{
			*ptr++ = c ;
			if( Read( DefHandle, &c, 1L ) == 0L )	c = 13 ;
		}
		while( c != 13 ) ;
	}
	Read( DefHandle, &c, 1L ) ;	// lit LF ou rien si fin

	*ptr++ = 0 ;

	return	TRUE ;
}
*/

LONG	PtrDefReadThisString()
{
	UBYTE	*ptr ;
	UBYTE	c ;

	ptr = DefString ;

	do	// saute espace
	{
		c = *PtrDef++ ;
		if( PtrDef >= EndPtrDef )	return FALSE ;
	}
	while( c == 32 ) ;

	if( c>31 )	// on a pu lire un cr
	{
		do	// lit phrase jusqu'a CR/LF
		{
			*ptr++ = c ;
			c = *PtrDef++ ;
			if( PtrDef >= EndPtrDef )	c = 13 ;
		}
		while( c != 13 ) ;
	}

	PtrDef++ ;			// lit LF ou rien si fin

	*ptr++ = 0 ;

	return	TRUE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*
LONG	SearchIdentificator( UBYTE *identificateur )
{
	while( ReadWord() )
	{
		if( DefString[strlen(DefString)-1] == ':' )
		{
			DefString[strlen(DefString)-1] = 0 ;
			if( !strnicmp(	identificateur,
					DefString,
					strlen( identificateur ) ) )
			{
				return TRUE ;	// identificateur trouve
			}
		}
	}
	return FALSE ;
}
*/

LONG	PtrDefSearchIdentificator( UBYTE *identificateur )
{
	while( PtrDefReadIdent() )
	{
/*		if( DefString[strlen(DefString)-1] == ':' )
		{
			DefString[strlen(DefString)-1] = 0 ;
*/
			if( !stricmp( identificateur, DefString ) )
			{
				return TRUE ;	// identificateur trouve
			}
//		}
	}
	return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*/

/*
UBYTE	*Def_ReadString( UBYTE *deffic, UBYTE *identificateur )
{
	DefHandle = OpenRead( deffic ) ;
	if( !DefHandle )	return 0L ;

	if( SearchIdentificator( identificateur ) )
	{
		if( ReadThisString() )
		{
			Close( DefHandle ) ;
			return DefString ;
		}
	}
	Close( DefHandle ) ;
	return 0L ;
}
*/

UBYTE	*Def_ReadString( UBYTE *deffic, UBYTE *identificateur )
{
	OrgPtrDef = PtrDef = LoadMalloc( deffic ) ;
	if( !PtrDef )	return 0 ;
	EndPtrDef = PtrDef + LoadMallocFileSize ;

	if( PtrDefSearchIdentificator( identificateur ) )
	{
		PtrDefReadString() ;
		Free( OrgPtrDef ) ;
		return DefString ;
	}
	Free( OrgPtrDef ) ;
	return 0 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*
LONG	Def_ReadValue( UBYTE *deffic, UBYTE *identificateur )
{
	LONG	i ;
	LONG	handle ;
	LONG	value ;
	UBYTE	c ;

	DefHandle = OpenRead( deffic ) ;
	if( !DefHandle )	return -1L ;

	if( SearchIdentificator( identificateur ) )
	{
		if( ReadWord() )
		{
			Close( DefHandle ) ;

			c =DefString[strlen(DefString)-1]&~32 ;
			if( c == 'H' )
			{
				// hexa
				value = 0 ;
				for( i=0; i<strlen(DefString)-1; i++ )
				{
					c = DefString[i] ;
					if( c <= '9' )	c -= '0' ;
					else		c = (c&~32) - 'A' + 10 ;
					value = value*16 + c ;
				}
				return value ;
			}
			else
			{
				// deci
				return atoi( DefString ) ;
			}
		}
	}
	Close( DefHandle ) ;
	return -1L ;
}
*/

LONG	Def_ReadValue( UBYTE *deffic, UBYTE *identificateur )
{
	LONG	i ;
	LONG	value ;
	UBYTE	c ;

	OrgPtrDef = PtrDef = LoadMalloc( deffic ) ;
	if( !PtrDef )	return -1 ;
	EndPtrDef = PtrDef + LoadMallocFileSize ;

	if( PtrDefSearchIdentificator( identificateur ) )
	{
		if( PtrDefReadWord() )
		{
			Free( OrgPtrDef ) ;
			c =DefString[strlen(DefString)-1]&~32 ;
			if( c == 'H' )
			{
				// hexa
				value = 0 ;
				for( i=0; i<strlen(DefString)-1; i++ )
				{
					c = DefString[i] ;
					if( c <= '9' )	c -= '0' ;
					else		c = (c&~32) - 'A' + 10 ;
					value = value*16 + c ;
				}
				return value ;
			}
			else
			{
				// deci
				return atoi( DefString ) ;
			}
		}
	}
	Free( OrgPtrDef ) ;
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*
LONG	Def_ReadValue2( UBYTE *deffic, UBYTE *identificateur, LONG *result )
{
	LONG	i ;
	LONG	handle ;
	LONG	value ;
	UBYTE	c ;

	DefHandle = OpenRead( deffic ) ;
	if( !DefHandle )	return FALSE ;

	if( SearchIdentificator( identificateur ) )
	{
		if( ReadWord() )
		{
			Close( DefHandle ) ;

			c =DefString[strlen(DefString)-1]&~32 ;
			if( c == 'H' )
			{
				// hexa
				value = 0 ;
				for( i=0; i<strlen(DefString)-1; i++ )
				{
					c = DefString[i] ;
					if( c <= '9' )	c -= '0' ;
					else		c = (c&~32) - 'A' + 10 ;
					value = value*16 + c ;
				}
				*result = value;
				return TRUE;
			}
			else
			{
				// deci
				*result = atoi( DefString ) ;
				return TRUE;
			}
		}
	}
	Close( DefHandle ) ;
	return FALSE;
}
*/

LONG	Def_ReadValue2( UBYTE *deffic, UBYTE *identificateur, LONG *result )
{
	LONG	i ;
	LONG	value ;
	UBYTE	c ;

	OrgPtrDef = PtrDef = LoadMalloc( deffic ) ;
	if( !PtrDef )	return FALSE ;
	EndPtrDef = PtrDef + LoadMallocFileSize ;

	if( PtrDefSearchIdentificator( identificateur ) )
	{
		if( PtrDefReadWord() )
		{
			Free( OrgPtrDef ) ;

			c =DefString[strlen(DefString)-1]&~32 ;
			if( c == 'H' )
			{
				// hexa
				value = 0 ;
				for( i=0; i<strlen(DefString)-1; i++ )
				{
					c = DefString[i] ;
					if( c <= '9' )	c -= '0' ;
					else		c = (c&~32) - 'A' + 10 ;
					value = value*16 + c ;
				}
				*result = value;
				return TRUE;
			}
			else
			{
				// deci
				*result = atoi( DefString ) ;
				return TRUE;
			}
		}
	}
	Free( OrgPtrDef ) ;
	return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

/*
LONG	Def_WriteString( UBYTE *deffic, UBYTE *identificateur, UBYTE *string )
{
	UBYTE	crlf[] = { 13, 10, 0 } ;

	DefHandleC = OpenWrite( "c:\\__tempo.def" ) ;
	if( !DefHandleC )	return	FALSE ;
	DefHandle = OpenRead( deffic ) ;
	if( DefHandle )
	{
		DefModeCopy = TRUE ;
		if( SearchIdentificator( identificateur ) )	// copie jusqu'a identificateur
		{
			ReadString() ;				// saute string
//			ReadThisString() ;				// saute string

			Write( DefHandleC, string, strlen( string ) ) ;
			Write( DefHandleC, crlf, 2L ) ;

			while( ReadWord() ) ;			// copie le reste

			DefModeCopy = FALSE ;
			Close( DefHandle ) ;
			Close( DefHandleC ) ;

//			Delete( deffic ) ;
//			rename( "c:\\__tempo.def", deffic ) ;
			if( Copy( "c:\\__tempo.def", deffic ) )
			{
				Delete( "c:\\__tempo.def" ) ;
			}

			DefModeCopy = FALSE ;
			return TRUE ;
		}
		Close( DefHandle ) ;
	}
	// creation ou ident pas trouve ecrit tout
	Write( DefHandleC, identificateur, strlen(identificateur) ) ;
	Write( DefHandleC, ": ", 2L ) ;
	Write( DefHandleC, string, strlen( string ) ) ;
	Write( DefHandleC, crlf, 2L ) ;

	Close( DefHandleC ) ;

//	Delete( deffic ) ;
	if( Copy( "c:\\__tempo.def", deffic ) )
	{
		Delete( "c:\\__tempo.def" ) ;
	}

	DefModeCopy = FALSE ;
	return TRUE ;
}
*/

LONG	Def_WriteString( UBYTE *deffic, UBYTE *identificateur, UBYTE *string )
{
	WORD	crlf = 0x0A0D ;

	OrgPtrDef = PtrDef = LoadMalloc( deffic ) ;

	if( PtrDef )	// fichier existe
	{
		EndPtrDef = PtrDef + LoadMallocFileSize ;

		DefHandle = OpenWrite( deffic ) ;
		if( !DefHandle ) return FALSE ;

		if( PtrDefSearchIdentificator( identificateur ) )
		{
			// ecrit début fichier
			Write( DefHandle, OrgPtrDef, PtrDef-OrgPtrDef ) ;

			// ecrit parametre
			Write( DefHandle, " ", 1 ) ;
			Write( DefHandle, string, strlen( string ) ) ;
			Write( DefHandle, &crlf, 2 ) ;

			PtrDefReadThisString() ;	// lit ancien parametre

			// fin du fichier
			Write( DefHandle, PtrDef, EndPtrDef-PtrDef ) ;
			Close( DefHandle ) ;
			Free( OrgPtrDef ) ;

			return TRUE ;
		}

		//identificateur pas trouvé

		// ecrit tout fichier
		Write( DefHandle, OrgPtrDef, LoadMallocFileSize ) ;

		Free( OrgPtrDef ) ;
	}
	else	// fichier n'existe pas
	{
		DefHandle = OpenWrite( deffic ) ;
		if( !DefHandle )	return FALSE ;
	}

	// ecrit identificateur
	Write( DefHandle, identificateur, strlen(identificateur) ) ;
	Write( DefHandle, ": ", 2 ) ;
	// ecrit parametre string
	Write( DefHandle, string, strlen( string ) ) ;
	Write( DefHandle, &crlf, 2 ) ;

	Close( DefHandle ) ;

	return TRUE ;
}

/*══════════════════════════════════════════════════════════════════════════*/

LONG	Def_WriteValue( UBYTE *deffic, UBYTE *identificateur, LONG value )
{
	itoa( value, DefValue, 10 ) ;
	return Def_WriteString( deffic, identificateur, DefValue ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

