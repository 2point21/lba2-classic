//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_MENU
#define	LIB_MENU

//──────────────────────────────────────────────────────────────────────────
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

//──────────────────────────────────────────────────────────────────────────
#include	<MENU\MENULIB.H>
#include	<MENU\MENUFUNC.H>
#include	<MENU\SELECTOR.H>

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_MENU

//──────────────────────────────────────────────────────────────────────────
