//-------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\initimer.h>

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#ifdef	_WIN32
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include	<system\winsys.h>
#include	<system\timerwin.h>

//-------------------------------------------------------------------------
void	InitTimer()
{
	OldTimerWindowProc = WindowProc	;
	WindowProc = TimerWindowProc	;
}

//-------------------------------------------------------------------------
void	ClearTimer()
{
	WindowProc = OldTimerWindowProc	;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#else// _WIN32

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include	<system\exit.h>
#include	<ail\mss.h>

//-------------------------------------------------------------------------
#include	<i86.h>
#include	<stdlib.h>

//-------------------------------------------------------------------------
#define	OLD_DIVISOR	0xFFFF

#define	DIVISOR		5966		//    1193180 / 200
#define	FREQUENCE	200		//    1193180 / 5966

//-------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//-------------------------------------------------------------------------
extern	void __interrupt NewIntPM08(void)		;

#pragma	aux NewIntPM08	"*"

//-------------------------------------------------------------------------
//extern	U32	Old_PM08_Off	;
//extern	U16	Old_PM08_Sel	;

extern  U32	TimerDataStart	;
extern  U32	TimerDataEnd	;

extern  U32	TimerStart	;
extern  U32	TimerEnd	;

//-------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//-------------------------------------------------------------------------
void LockVMMTimer()
{
	if(	!AIL_vmm_lock_range(&TimerStart, &TimerEnd)		OR
		!AIL_vmm_lock_range(&TimerDataStart, &TimerDataEnd)		)
		exit(1);
}

//-------------------------------------------------------------------------
static inline void UnlockVMMTimer()
{
	AIL_vmm_unlock_range(&TimerStart, &TimerEnd);
	AIL_vmm_unlock_range(&TimerDataStart, &TimerDataEnd);
}
/*
//-------------------------------------------------------------------------
void	InitTimer()
{
	union	REGS	r	;
	struct	SREGS	sr	;
	void 	far	*fh	;

	if(Old_PM08_Sel)	return	;

	LockVMMTimer()		;

//----- Save Old Protected Mode Vector 08 ------
	r.x.eax = 0x3508		;//	Function 35h, for Int 08
	sr.ds = sr.es = 0		;//	Security (kill the cat)
	int386x( 0x21, &r, &r, &sr )	;//	Call Int 21
	Old_PM08_Sel = (U16)sr.es	;//	ES:EBX=Old Interrupt 08
	Old_PM08_Off = r.x.ebx		;

//----- Install New Protected Vector 08 --------
	r.x.eax = 0x2508		;//	Function 25h for int 08
	fh = (void far*)NewIntPM08	;//	Get far Pointer
	r.x.edx = FP_OFF( fh )		;//	Get Offset
	sr.ds 	= FP_SEG( fh )		;//	Get	Segment
	sr.es 	= 0			;//	Security ( ... )
	int386x( 0x21, &r, &r, &sr )	;//	Invoke int 21h

//----- It's Ok, Now I can accelerate Timer 08

	SetDivisor( DIVISOR )		;

	RegisterInitClear( InitTimer, ClearTimer )	;
}

//-------------------------------------------------------------------------
void	ClearTimer()
{
	union	REGS	r		;
	struct	SREGS	sr		;
	void 	far	*fh		;

	if(!Old_PM08_Sel)	return	;

	fh = (void far*)NewIntPM08	;//	Get far Pointer

//----- Ok, it's over, stop running... -------

	SetDivisor( OLD_DIVISOR )	;

//----- Restore Old Protected Vector 08 --------
	r.x.eax = 0x2508		;//	Set Vector Interupt PM08
	r.x.edx = Old_PM08_Off		;//	Adr Old Interrupt 08
	sr.ds = Old_PM08_Sel		;
	sr.es = 0			;//	... (...)
	int386x( 0x21, &r, &r, &sr )	;//	Invoke int 21

	Old_PM08_Sel = 0		;
	Old_PM08_Off = 0		;

	UnlockVMMTimer()		;
}
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#endif//_WIN32
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
