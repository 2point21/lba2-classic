//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\adeline.h>

#include	<svga\initmode.h>
#include	<svga\dirtybox.h>
#include	<svga\inimouse.h>
#include	<svga\mouse.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\winsys.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#else //_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\exit.h>
#include	<ail\mss.h>

#include 	<i86.h>
#include 	<stdlib.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
static	S32	Installed	= FALSE	;
static	S32	LastFlagMouse	= FALSE	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
volatile S32	Click = 0	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
volatile S32	MouseX		;
volatile S32	MouseY		;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32		MouseXDep = 0 	;
S32		MouseYDep = 0 	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32		FlagMouse = FALSE;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	__cplusplus
extern	"C"	{
#endif

extern	U8	BinGphMouse 	;

#ifdef	__cplusplus
}
#endif

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
extern	WINDOW_PROC_PTR	OldMouseWindowProc	;
extern	WINDOW_PROC	MouseWindowProc		;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	InitMouse()
{
	OldMouseWindowProc = WindowProc	;
	WindowProc = MouseWindowProc	;

	SetMouseBox(0, 0, ModeDesiredX, ModeDesiredY)	;

	GphSprite = &BinGphMouse 	;
	DefSprite = 0		 	;

	Click	  = 0		 	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	ClearMouse()
{
	WindowProc = OldMouseWindowProc	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#else //_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
static void _loadds far click_handler (int max, int mcx, int mdx)
{
	MouseX = mcx>>3	;
	MouseY = mdx>>3	;

	if( max & 0x02 ) Click |=  1	;
	if( max & 0x04 ) Click &= ~1	;
	if( max & 0x08 ) Click |=  2	;
	if( max & 0x10 ) Click &= ~2	;
}

#pragma aux click_handler 	\
	parm [eax] [ecx] [edx]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void 	InitMouse()
{
	struct SREGS sregs		;
	union REGS inregs, outregs	;

	// Lock VMM
	if(	!AIL_vmm_lock_range((void*)&MouseX, (void*)&MouseY)	OR
		!AIL_vmm_lock_range((void*)&Click, (void*)((&Click)+1))	OR
		!AIL_vmm_lock_range((void *)FP_OFF(click_handler), (void*)ShowMouse))
		exit(1);

	segread(&sregs)	;

	// check for mouse driver
	inregs.w.ax = 0			;
	int386 (0x33, &inregs, &outregs);

	SetMouseBox(0, 0, ModeDesiredX, ModeDesiredY)	;
	SetMouseSpeed(4, 4)		;

	GphSprite = &BinGphMouse 	;
	DefSprite = 0		 	;

	Click	  = 0		 	;

	UpdateMouseRes()		;

	// install click watcher
	inregs.w.ax = 0xC				;
	inregs.w.cx = 0x01 | 0x02 | 0x04 | 0x08 | 0x10 	;
	inregs.x.edx = FP_OFF( click_handler )		;
	sregs.es     = FP_SEG( click_handler )		;
	int386x( 0x33, &inregs, &outregs, &sregs )	;

	RegisterInitClear( InitMouse, ClearMouse )	;

	FlagMouse	= LastFlagMouse;

	Installed	= TRUE		;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	ClearMouse()
{
	union REGS inregs, outregs	;

	if(!Installed)
	{
		return	;
	}

	inregs.w.ax = 0				;
	int386( 0x33, &inregs, &outregs )	;

	LastFlagMouse = FlagMouse	;

	// Unlock VMM
	AIL_vmm_unlock_range((void*)&MouseX, (void*)&MouseY)			;
	AIL_vmm_unlock_range((void*)&Click, (void*)((&Click)+1))			;
	AIL_vmm_unlock_range((void *)FP_OFF(click_handler), (void*)ShowMouse)	;

	Installed	= FALSE		;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
