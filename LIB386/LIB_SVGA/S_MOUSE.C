
#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_svga\lib_svga.h"

#include <stdio.h>
#include <dos.h>
#include <i86.h>

/*══════════════════════════════════════════════════════════════════════════*
			 █▄ ▄█ █▀▀▀█ █   █ ██▀▀▀ █▀▀▀▀
			 ██▀ █ ██  █ ██  █ ▀▀▀▀█ ██▀▀
			 ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

volatile LONG	Click = 0	;
volatile LONG	Mouse_X = 0	;
volatile LONG	Mouse_Y = 0	;
LONG		Mouse_X_Dep = 0 ;
LONG		Mouse_Y_Dep = 0 ;

LONG	OldX0Mouse=-1, OldY0Mouse, OldX1Mouse, OldY1Mouse ;
WORD	DefMouse = 0 ;
LONG	FlagMouse = TRUE ;

UBYTE	*GphMouse ;
UBYTE	*BufferMouse ;

	extern	UBYTE	*Log ;
	extern	UBYTE	BinGphMouse ;

/*══════════════════════════════════════════════════════════════════════════*/

#pragma off (check_stack)
void _loadds far click_handler (int max, int mcx, int mdx)
{
#pragma aux click_handler parm [EAX] [ECX] [EDX]
	Mouse_X = mcx>>3	;
	Mouse_Y = mdx>>3	;
	if( max & 0x02 ) Click |=  1	;
	if( max & 0x04 ) Click &= ~1	;
	if( max & 0x08 ) Click |=  2	;
	if( max & 0x10 ) Click &= ~2	;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	SetMouseBox( LONG x0, LONG y0, LONG x1, LONG y1 )
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	inregs.w.ax = 0x07	;
	inregs.w.cx = x0<<3	;
	inregs.w.dx = x1<<3	;
	int386 (0x33, &inregs, &outregs);

	inregs.w.ax = 0x08	;
	inregs.w.cx = y0<<3	;
	inregs.w.dx = y1<<3	;
	int386 (0x33, &inregs, &outregs);
}
/*--------------------------------------------------------------------------*/
void	SetMouseSpeed( LONG dx, LONG dy )
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	inregs.w.ax = 0x0F	;
	inregs.w.cx = (UWORD)dx	;
	inregs.w.dx = (UWORD)dy	;
	int386 (0x33, &inregs, &outregs);
}
/*--------------------------------------------------------------------------*/
void	SetMousePos( LONG x, LONG y )
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	inregs.w.ax = 0x04		;
	inregs.w.cx = (UWORD)x<<3	;
	inregs.w.dx = (UWORD)y<<3	;
	int386 (0x33, &inregs, &outregs);

	inregs.w.ax = 0x01		;
	int386 (0x33, &inregs, &outregs);

	Mouse_X = x	;
	Mouse_Y = y	;
}

/*--------------------------------------------------------------------------*/
void	GetMouseDep()
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	inregs.w.ax = 0x0B		;
	int386 (0x33, &inregs, &outregs);

	Mouse_X_Dep = (LONG)((WORD)outregs.w.cx)	;
	Mouse_Y_Dep = (LONG)((WORD)outregs.w.dx)	;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ShowMouse( LONG flag )
{
	if( !flag )
	{
		if( FlagMouse )
		{
			if( OldX0Mouse != -1 )
			{
				CopyBlockPhys( OldX0Mouse,OldY0Mouse, OldX1Mouse, OldY1Mouse ) ;
				OldX0Mouse = -1 ;
			}
			FlagMouse = FALSE ;
		}
	}
	else
	{
		if( !FlagMouse )
		{
			OldX0Mouse = -1 ;	/* force aff et evite restore */
			AffMouse() ;
			FlagMouse = TRUE ;
		}
	}
}

/*--------------------------------------------------------------------------*/

void	AffMouse()
{
	LONG	dx, dy ;
	LONG	x0, y0 ;
	LONG	x1, y1 ;

	if( FlagMouse )
	{
		x0 = Mouse_X ;
		y0 = Mouse_Y ;

		if( (OldX0Mouse == x0) AND (OldY0Mouse == y0) )	return ;

		GetDxDyGraph( DefMouse, &dx, &dy, GphMouse ) ;
		x1 = x0 + dx - 1 ;
		y1 = y0 + dy - 1 ;

		MemoClip() ;

		SetClip( x0,y0,x1,y1 ) ;

		if( OldX0Mouse != -1 )
			CopyBlockPhys( OldX0Mouse,OldY0Mouse, OldX1Mouse, OldY1Mouse ) ;

		SaveBlock( Log, BufferMouse, x0,y0, ClipXmax,ClipYmax ) ;

		AffGraph( DefMouse, x0, y0, GphMouse ) ;

		CopyBlockPhys( x0,y0,  ClipXmax,ClipYmax  ) ;

		RestoreBlock( Log, BufferMouse, x0,y0, ClipXmax,ClipYmax  ) ;

		OldX0Mouse = x0 ;
		OldY0Mouse = y0 ;
		OldX1Mouse = ClipXmax ;
		OldY1Mouse = ClipYmax ;

		RestoreClip() ;
	}
}
/*--------------------------------------------------------------------------*/

void	SetMouse( WORD num )
{
	DefMouse = num ;
	OldX0Mouse = -1 ;
	AffMouse() ;
}


/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void 	InitMouse()
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	int installed = 0		;
	int orig_mode = 0		;
	int far *ptr			;
	int (far *function_ptr)()	;

	segread(&sregs)	;

	/* check for mouse driver	*/
	inregs.w.ax = 0			;
	int386 (0x33, &inregs, &outregs);
	if( installed = (outregs.w.ax != -1) )	return	;

	SetMouseBox( 0, 0, 639, 479 )	;
	SetMouseSpeed( 4, 4 )		;

	GphMouse = &BinGphMouse ;

	BufferMouse = Malloc( 48*48 ) ;	/* max size mouse ! */

	Click = 0	;
	FlagMouse = TRUE ;

	/* install click watcher */

	inregs.w.ax = 0xC			;
	inregs.w.cx = 0x01 | 0x02 | 0x04 | 0x08 |0x10 	;
	function_ptr = click_handler		;
	inregs.x.edx = FP_OFF( function_ptr )	;
	sregs.es     = FP_SEG( function_ptr )	;
	int386x( 0x33, &inregs, &outregs, &sregs );
}
/*--------------------------------------------------------------------------*/
void	ClearMouse()
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	inregs.w.ax = 0			;
	int386( 0x33, &inregs, &outregs );

	FlagMouse = FALSE ;

	Free( BufferMouse ) ;
}
/*══════════════════════════════════════════════════════════════════════════*/
