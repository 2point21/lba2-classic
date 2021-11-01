//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\adeline.h>
#include	<svga\fil_line.h>
#include	<svga\rect.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	Rect(S32 x0, S32 y0, S32 x1, S32 y1, S32 coul)
{
	Line( x0, y0, x1, y0, coul ) ;
	Line( x0, y1, x1, y1, coul ) ;
	Line( x0, y0, x0, y1, coul ) ;
	Line( x1, y0, x1, y1, coul ) ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
