//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\adeline.h>
#include	<system\initkeyb.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<system\winsys.h>
#include	<system\keybwin.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	InitKeyboard()
{
	AsciiMutex = CreateMutex(NULL, FALSE, NULL);

	if(!AsciiMutex)
	{
		exit(1)	;
	}

	OldKeybWindowProc = WindowProc	;
	WindowProc = KeybWindowProc	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	ClearKeyboard()
{
	WindowProc = OldKeybWindowProc	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#else //_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<ail\mss.h>
#include	<system\exit.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<i86.h>
#include	<stdlib.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	MemoStack()	;

#pragma	aux MemoStack	=		\
"	mov	[Stack], esp	"	\
	modify	[]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	InitKeyboard()
{
	union	REGS	r		;
	struct	SREGS	sr		;
	void 	far	*fh		;

	if(Old_PM09_Sel) return		;

	if(	!AIL_vmm_lock_range(&KeybCodeStart, &KeybCodeEnd)	OR
		!AIL_vmm_lock_range(&KeybDataStart, &KeybDataEnd)		)
	{
		exit(1);
	}

//----- Save Old Protected Mode Vector 09 ------
	r.x.eax = 0x3509		;//	Function 35h, for Int 9
	sr.ds = sr.es = 0		;//	Security (kill the cat)
	int386x( 0x21, &r, &r, &sr )	;//	Call Int 21
	Old_PM09_Sel = (U32)sr.es	;//	ES:EBX=Old Interrupt 09
	Old_PM09_Off = r.x.ebx		;

//----- Install New Protected Vector 09 --------
	r.x.eax = 0x2509		;//	Function 25h for int 09
	fh = (void far*)KeybHandler	;//	Get far Pointer
	r.x.edx = FP_OFF( fh )		;//	Get Offset
	sr.ds 	= FP_SEG( fh )		;//	Get Segment
	sr.es 	= 0			;//	Security
	int386x( 0x21, &r, &r, &sr )	;//	Invoke DPMI

//----- Memo stack pointer for CTRL-ALT-DEL exit
	MemoStack()			;

	RegisterInitClear(InitKeyboard, ClearKeyboard);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	ClearKeyboard()
{
	union	REGS	r		;
	struct	SREGS	sr		;

	if(!Old_PM09_Sel) return	;

//----- Restore Old Protected Vector 09 --------
	r.x.eax = 0x2509		;//	Set Vector Interupt PM09
	r.x.edx = Old_PM09_Off		;//	Adr Old Interrupt 09
	sr.ds = (U16)Old_PM09_Sel	;
	sr.es = 0			;//	Security
	int386x( 0x21, &r, &r, &sr )	;//	Invoke int 21

	Old_PM09_Sel = 0		;
	Old_PM09_Off = 0		;

	AIL_vmm_unlock_range(&KeybCodeStart, &KeybCodeEnd)	;
	AIL_vmm_unlock_range(&KeybDataStart, &KeybDataEnd)	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
