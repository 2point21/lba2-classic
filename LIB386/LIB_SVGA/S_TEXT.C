
#include	"\projet\lib386\lib_sys\adeline.h"
#include	"\projet\lib386\lib_sys\lib_sys.h"

#include	"\projet\lib386\lib_svga\lib_svga.h"

#include	<string.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<ctype.h>

/*--------------------------------------------------------------------------*/
/*			      (c) Adeline 1993			    	    */
/*--------------------------------------------------------------------------*/
/*══════════════════════════════════════════════════════════════════════════*
			 ▀▀█▀▀ █▀▀▀▀ ▀▄ ▄▀ ▀▀█▀▀
			   ██  ██▀▀   ▄▀▄    ██
			   ▀▀  ▀▀▀▀▀ ▀   ▀   ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	Text( WORD x,WORD y,char *msk, ... )
{
//	ULONG	*pt = (ULONG *)&str_in ;
	WORD	l ;
	char	c ;
	char	str[256] ;
	char	tempo[34] ;
	char	*str_out = str ;
	char	*ptr ;
	WORD	flagflip = 0 ;
	va_list ap;

	if( (y < ClipYmin)
	OR  (y > ClipYmax-7)
	OR  (x > ClipXmax)	)	return ;

	l = 0 ;

	va_start(ap, msk);

	*str_out = 0 ;
	while( (c=*msk++) != 0 )
	{
		if( c == '%' )
		{
			switch( toupper(*msk++) )
			{
				case 'C':	/* char */
					tempo[0] = va_arg(ap,char);
					tempo[1] = 0 ;
					strcat(str_out, tempo  ) ;
					l++ ;
					break ;

				case 'S':	/* string */
					ptr = va_arg(ap,char*) ;
					strcat(str_out, ptr) ;
					l += strlen(ptr) ;
					break ;

				case 'B':	/* byte */
					strcat(str_out, ltoa( va_arg(ap, UBYTE), tempo, 10 )) ;
					l += strlen( tempo ) ;
					break ;

				case 'D':	/* decimal */
					strcat(str_out, ltoa( va_arg(ap, int), tempo, 10 )) ;
					l += strlen( tempo ) ;
					break ;

				case 'L':	/* long decimal */
					strcat(str_out, ltoa( va_arg(ap, int), tempo, 10 )) ;
					l += strlen( tempo ) ;
					break ;

				case 'U':	/* unsigned decimal */
					strcat(str_out, ultoa( va_arg(ap, unsigned), tempo, 10 )) ;
					l += strlen( tempo ) ;
					break ;

				case 'P':	/* pointeur hexa */
					strcat(str_out, ultoa( va_arg(ap, unsigned), tempo, 16 )) ;
					l += strlen( tempo ) ;
					break ;

				case 'F':
					flagflip = 1 ;
					break ;

				default:
					return ;
			}
		}
		else
		{
			str[l] = c ;
			l++ ;
			str[l] = 0 ;
		}
	}

	ptr = str ;

	if( x + l*8 - 1 > ClipXmax )
	{
		ptr[ (ClipXmax-x)/8 ] = 0 ;
	}

	if( x < ClipXmin )
	{
		ptr += (ClipXmin-x)/8 ;
		x += ClipXmin-x ;
	}

	AffString( x, y, ptr )	;

	if( flagflip )
	{
		CopyBlockPhys( x,y, x+strlen(ptr)*8-1, y+7 ) ;
	}
	va_end(ap);
}


#ifdef	OLD_VERSION

void	Text( WORD x,WORD y,char *msk, ULONG str_in )
{
ULONG	*pt = (ULONG *)&str_in ;
WORD	l ;
char	c ;
char	str[256] ;
char	tempo[34] ;
char	*str_out = str ;
void	*ptr ;

	*str_out = 0 ;
	while( (c=*msk++) != 0 )
	{
		if( c == '%' )
		{
			switch( toupper(*msk++) )
			{
				case 'C':	/* char */
					tempo[0] = *(char *)pt;
					tempo[1] = 0 ;
					strcat(str_out, tempo  ) ;
					break ;

				case 'S':	/* string */
					strcat(str_out, *(char **)pt) ;
					break ;

				case 'B':	/* byte */
					strcat(str_out, ltoa( *(UBYTE *)pt, tempo, 10 )) ;
					break ;

				case 'D':	/* decimal */
					strcat(str_out, ltoa( *pt, tempo, 10 )) ;
					break ;

				case 'L':	/* long decimal */
					strcat(str_out, ltoa( *pt, tempo, 10 )) ;
					break ;

				case 'U':	/* unsigned decimal */
					strcat(str_out, ultoa( *pt, tempo, 10 )) ;
					break ;

				case 'P':	/* pointeur hexa */
					strcat(str_out, ltoa( *pt, tempo, 16 )) ;
					break ;

				default:
					return ;
			}
			pt++;
		}
		else
		{
			l = strlen( str ) ;
			str[l] = c ;
			str[l+1] = 0 ;
		}
	}

	AffString( x, y, str )	;
}
#endif