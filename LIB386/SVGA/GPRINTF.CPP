//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\adeline.h>
#include	<svga\affstr.h>
#include	<svga\clip.h>
#include	<svga\dirtybox.h>
#include	<svga\gprintf.h>

#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32	TextXMin	= 0	;
S32	TextXMax	= 0	;
S32	TextYMin	= 0	;
S32	TextYMax	= 0	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	GraphPrintf( U32 flagflip, U32 x, U32 y, char *format, ... )
{
	S32	len ;
	char	*ptr ;
	char	msgbuf[256];
	va_list arglist;

	if( (y<ClipYMin) OR (y>ClipYMax-7) OR (x>ClipXMax))	return ;

        va_start( arglist, format );
        len = _vbprintf( msgbuf, 256, format, arglist );
        va_end( arglist );

	if(len==-1)						return ;

	ptr = msgbuf ;

	if( x + len*8 - 1 > ClipXMax )
	{
		len = (ClipXMax-x)/8 ;
		ptr[ len ] = 0 ;
	}

	if( x < ClipXMin )
	{
		ptr += (ClipXMin-x)/8 ;
		x += ClipXMin-x ;
		len = strlen(ptr) ;
	}

	if( ptr < msgbuf + len )
	{
		AffString( x, y, ptr )	;

		TextXMin = x ;
		TextXMax = x+len*8-1 ;
		TextYMin = y ;
		TextYMax = y+7 ;

		BoxStaticAdd(TextXMin, TextYMin, TextXMax, TextYMax) ;

		if( flagflip )
		{
			BoxUpdate()	;
		}
	}
	else
	{
	       	TextXMin = -1 ;
	       	TextXMax = -1 ;
	       	TextYMin = -1 ;
	       	TextYMax = -1 ;
	}
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
