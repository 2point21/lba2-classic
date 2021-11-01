//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<system\exit.h>

#include	<svga\logphys.h>
#include	<svga\dirtybox.h>
#include	<svga\inimouse.h>
#include	<svga\mouse.h>

#include 	<i86.h>
#include 	<stdlib.h>

//──────────────────────────────────────────────────────────────────────────
void	ShowMouse( S32 flag )
{
	FlagMouse = flag ;
}

//──────────────────────────────────────────────────────────────────────────
void	SetMouse( S16 num )
{
	DefSprite = num ;
}

//──────────────────────────────────────────────────────────────────────────
void	SetMouseBox( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	union REGS inregs, outregs	;

	inregs.x.eax = 0x07	;
	inregs.x.ecx = x0<<3	;
	inregs.x.edx = x1<<3	;
	int386 (0x33, &inregs, &outregs);

	inregs.x.eax = 0x08	;
	inregs.x.ecx = y0<<3	;
	inregs.x.edx = y1<<3	;
	int386 (0x33, &inregs, &outregs);

	inregs.x.eax = 0x3	;
	int386 (0x33, &inregs, &outregs);

	MouseX	= ((S16)outregs.w.cx)>>3;
	MouseY	= ((S16)outregs.w.dx)>>3;
}

//──────────────────────────────────────────────────────────────────────────
void	SetMouseSpeed( S32 dx, S32 dy )
{
	union REGS inregs, outregs	;

	inregs.x.eax = 0x0F	;
	inregs.x.ecx = dx	;
	inregs.x.edx = dy	;
	int386 (0x33, &inregs, &outregs);
}

//──────────────────────────────────────────────────────────────────────────
void	SetMousePos( S32 x, S32 y )
{
	union REGS inregs, outregs	;

	inregs.x.eax = 0x04		;
	inregs.x.ecx = x<<3		;
	inregs.x.edx = y<<3		;
	int386 (0x33, &inregs, &outregs);

	inregs.x.eax = 0x01		;
	int386 (0x33, &inregs, &outregs);

	MouseX = x	;
	MouseY = y	;
}

//──────────────────────────────────────────────────────────────────────────
void	GetMouseDep()
{
	union REGS inregs, outregs	;

	inregs.x.eax = 0x0B		;
	int386 (0x33, &inregs, &outregs);

	MouseXDep = (S16)outregs.w.cx	;
	MouseYDep = (S16)outregs.w.dx	;
}

//──────────────────────────────────────────────────────────────────────────
void 	UpdateMouseRes()
{
	SetMouseBox(0, 0, ModeResX-1, ModeResY-1);
}

//──────────────────────────────────────────────────────────────────────────

