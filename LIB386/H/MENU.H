//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	LIB_MENU
#define	LIB_MENU

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	_BUILDING_LIB

#ifdef	_WIN32

#ifdef	__SW_3R
#pragma library ("NT_MENR.LIB")
#else//	__SW_3R
#pragma library ("NT_MENS.LIB")
#endif//__SW_3R

#else//	_WIN32

#ifdef	__SW_3R
#pragma library ("DOS_MENR.LIB")
#else//	__SW_3R
#pragma library ("DOS_MENS.LIB")
#endif//__SW_3R

#endif//_WIN32

#endif//_BUILDING_LIB

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<MENU\MENULIB.H>
#include	<MENU\MENUFUNC.H>
#include	<MENU\SELECTOR.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_MENU

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
