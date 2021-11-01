#include	"c_extern.h"

S32	MyKey	;

U8	PtrPalNormal[768+RECOVER_AREA] ;
U8	*PtrPal	;

#define	BCL_FADE	50

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	FadePal( U8 r, U8 v, U8 b, U8 *ptrpal, S32 percent )
{
	U8	workpal[768] ;
	S32	n ;

	for( n=0; n<256; n++ )
	{
		workpal[n*3+0] = (U8)RegleTrois( r, ptrpal[n*3+0], BCL_FADE, percent ) ;
		workpal[n*3+1] = (U8)RegleTrois( v, ptrpal[n*3+1], BCL_FADE, percent ) ;
		workpal[n*3+2] = (U8)RegleTrois( b, ptrpal[n*3+2], BCL_FADE, percent ) ;
	}

	Palette( workpal ) ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	FadeToBlack( U8 *ptrpal )
{
	S32	n ;

	SaveTimer() ;

	for( n=BCL_FADE; n>=0; n-=2 )
	{
		FadePal( 0,0,0, ptrpal, n ) ;
	}

	RestoreTimer() ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	FadeToPal( U8 *ptrpal )
{
	S32	n ;

	SaveTimer() ;

	for( n=0; n<=BCL_FADE; n+=2 )
	{
		FadePal( 0,0,0, ptrpal, n ) ;
	}

	RestoreTimer() ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	SetBlackPal()
{
	S32	n ;

	SaveTimer() ;

	for( n=0; n<256; n++ )
	{
		PalOne( n, 0,0,0 ) ;
	}

	RestoreTimer() ;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	Beep()
{
	sound(500)	;
	delay(100)	;
	nosound()	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

char	*End_Error = "Ok" ;

//-------------------------------------------------------------------------
void	TheEnd( char *err )
{
	End_Error = err ;
	exit( 0 ) ;
}

//-------------------------------------------------------------------------
void	TheEndInfo( void )
{
	LogPrintf( End_Error ) ;
}

//-------------------------------------------------------------------------
void    main( int argc, char *argv[] )
{
	atexit(TheEndInfo)      ;

	#define ibuffer Screen
	#define ibuffersize     (640*480+RECOVER_AREA)
	#define lname   "lba2.cfg"
	#define inits   INIT_DEFFILE|INIT_LOG|INIT_TIMER|INIT_SVGA|INIT_KEYB
	#include <initadel.c>

	BoxInit( MAX_BOXES ) 	;

	Cls() ;

	if( !Load( "palette.pal", PtrPalNormal ) )	TheEnd( "palette.pal not found !" ) ;

	PtrPal = PtrPalNormal ;

	SetBlackPal() ;

	ReadInputConfig() 	;
	MenuConfig() 		;
	WriteInputConfig() 	;
}
