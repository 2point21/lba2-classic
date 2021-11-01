//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_OBJECT
#define LIB_OBJECT

//──────────────────────────────────────────────────────────────────────────
#ifndef	_BUILDING_LIB

#ifdef	_WIN32

#pragma library ("NT_OBJ.LIB")

#else//	_WIN32

#ifdef	cedric
#pragma library ("F:\PROJET\LIB386\OBJECT\DOS_OBJ.LIB")
#else
#pragma library ("DOS_OBJ.LIB")
#endif
#endif//_WIN32

#endif//_BUILDING_LIB

//──────────────────────────────────────────────────────────────────────────
#include <SYSTEM\ADELINE.H>

//──────────────────────────────────────────────────────────────────────────
#include <OBJECT\AFF_OBJ.H>

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_OBJECT

//──────────────────────────────────────────────────────────────────────────
