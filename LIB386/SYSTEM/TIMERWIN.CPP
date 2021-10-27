//-------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\winsys.h>
#include	<system\timerwin.h>

//-------------------------------------------------------------------------
volatile U32	TimerSystemHR	;
volatile U32	TimerRefHR	;

//-------------------------------------------------------------------------
volatile S32	TimerLock	;

//-------------------------------------------------------------------------
volatile S32	CmptFrame	;
volatile S32	NbFramePerSecond;

//-------------------------------------------------------------------------
volatile S32	CmptMemoTimerRef;

//-------------------------------------------------------------------------
	U32	LastTime	;
	U32	LastEvaluate	;

//-------------------------------------------------------------------------
static	U32	MemoTimerRefHR	;

//----------------------------------------------------------------------------
WINDOW_PROC_PTR	OldTimerWindowProc;

//----------------------------------------------------------------------------
S32	TimerWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		LastTime = LastEvaluate = timeGetTime();
		ManageTime()	;
		break		;

	case WM_SETFOCUS:
		if(!AppActive)
		{
			UnlockTimer()	;
		}
		break		;

	case WM_KILLFOCUS:
		if(AppActive)
		{
			LockTimer()	;
		}
		break		;
	}

	return OldTimerWindowProc(hWnd, message, wParam, lParam);
}

//-------------------------------------------------------------------------
void	ManageTime()
{
	S32		time	;

	TimerSystemHR = timeGetTime() ;

	if(!TimerLock)
	{
		TimerRefHR += TimerSystemHR - LastTime 	;
	}

	LastTime = TimerSystemHR		;

	time = TimerSystemHR - LastEvaluate	;

	if(time >= 1000)
	{
		NbFramePerSecond = (CmptFrame*1000 + time/2) / time	;
		CmptFrame	 = 0					;
		LastEvaluate	+= (time/1000)*1000			;
	}
}

//-------------------------------------------------------------------------
void	SaveTimer()
{
	if(!CmptMemoTimerRef++)
	{
		ManageTime()			;
		MemoTimerRefHR = TimerRefHR	;
	}
}

//-------------------------------------------------------------------------
void	RestoreTimer()
{
	if(CmptMemoTimerRef)
	{
		if(!--CmptMemoTimerRef)
		{
			ManageTime()			;
			TimerRefHR = MemoTimerRefHR	;
		}
	}
}

//-------------------------------------------------------------------------
void	SetTimerHR(U32 timer)
{
	LockTimer()		;
	TimerRefHR = timer	;
	UnlockTimer()		;
}

//-------------------------------------------------------------------------
void	LockTimer()
{
	ManageTime()	;
	TimerLock++	;
}

//-------------------------------------------------------------------------
void	UnlockTimer()
{
	if(TimerLock)
	{
		ManageTime()	;
		TimerLock--	;
	}
}

//-------------------------------------------------------------------------
