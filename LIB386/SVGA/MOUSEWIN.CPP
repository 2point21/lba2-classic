//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>

#include	<svga\logphys.h>
#include	<svga\dirtybox.h>
#include	<svga\inimouse.h>
#include	<svga\mouse.h>

//──────────────────────────────────────────────────────────────────────────
static	S32	LibMouseXDep, LibMouseYDep			;
static	S32	MouseBoxX0, MouseBoxX1, MouseBoxY0, MouseBoxY1	;

//──────────────────────────────────────────────────────────────────────────
WINDOW_PROC_PTR	OldMouseWindowProc;

//──────────────────────────────────────────────────────────────────────────
S32	MouseWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		Click |= 1;
		break;

	case WM_RBUTTONDOWN:
		Click |= 2;
		break;

	case WM_LBUTTONUP:
		Click &= ~1;
		break;

	case WM_RBUTTONUP:
		Click &= ~2;
		break;

	case WM_MOUSEMOVE:
		{
			S32	dx, dy	;

			dx = LOWORD(lParam)-160;
			dy = HIWORD(lParam)-100;

			if(!(dx|dy))	break;

			LibMouseXDep 	+= dx	;
			LibMouseYDep 	+= dy	;

			SetMousePos(MouseX+dx, MouseY+dy)	;
		}
		break;
	}

	return OldMouseWindowProc(hWnd, message, wParam, lParam);
}

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
void	SetMousePos( S32 x, S32 y )
{
	MouseX = x	;
	MouseY = y	;

	if(MouseX<MouseBoxX0)		MouseX = MouseBoxX0;
	else if(MouseX>MouseBoxX1)	MouseX = MouseBoxX1;

	if(MouseY<MouseBoxY0)		MouseY = MouseBoxY0;
	else if(MouseY>MouseBoxY1)	MouseY = MouseBoxY1;

	SetCursorPos(160, 100)	;
}

//──────────────────────────────────────────────────────────────────────────
void	GetMouseDep()
{
	MouseXDep = LibMouseXDep	;
	MouseYDep = LibMouseYDep	;

	SetCursorPos(160, 100)		;

	LibMouseXDep = 0		;
	LibMouseYDep = 0		;
}

//──────────────────────────────────────────────────────────────────────────
void	SetMouseBox( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	if(x0<0)		x0 = 0;
	if(y0<0)		y0 = 0;
	if(x1>=ModeDesiredX)	x1 = ModeDesiredX-1;
	if(y1>=ModeDesiredY)	y1 = ModeDesiredY-1;

	if((x0>x1)||(y0>y1))	return	;

	MouseBoxX0 = x0	;
	MouseBoxY0 = y0	;
	MouseBoxX1 = x1	;
	MouseBoxY1 = y1	;

	SetMousePos(MouseX, MouseY)	;
}

//──────────────────────────────────────────────────────────────────────────
void	SetMouseSpeed( S32 dx, S32 dy )
{
	dx = dx ;
	dy = dy ;
}

//──────────────────────────────────────────────────────────────────────────
void 	UpdateMouseRes()
{
	SetMouseBox(0, 0, ModeResX-1, ModeResY-1);
}

//──────────────────────────────────────────────────────────────────────────

