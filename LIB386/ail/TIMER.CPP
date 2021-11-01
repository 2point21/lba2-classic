#include	<system\adeline.h>
#include	<system\exit.h>
#include	<system\initimer.h>
#include	<system\timerdos.h>

#include	<ail\timer.h>

#ifdef	_WIN32
#include	<ail\mssw.h>
#else//	_WIN32
#include	<ail\mss.h>
#endif//_WIN32

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
VOID_FUNC	ClearAIL;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
void	InitAIL()
{
	AIL_startup()	;

	RegisterInitClear( InitAIL, ClearAIL ) ;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
void	ClearAIL()
{
	AIL_shutdown()	;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
#ifndef	_WIN32

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
S32	InitTimerAIL()
{
	HTIMER  HandleTimer ;

	LockVMMTimer();
	HandleTimer = AIL_register_timer( (AILTIMERCB)NewProc08 ) ;
	if (HandleTimer == -1)
	{
		return FALSE ;
	}

	AIL_set_timer_frequency( HandleTimer, 200 ) ;
	AIL_start_timer( HandleTimer ) ;

	RegisterInitClear( (VOID_FUNC *)InitTimerAIL, NULL ) ;

	return TRUE;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
#endif//_WIN32

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
