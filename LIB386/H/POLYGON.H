//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	LIB_POLYGON
#define	LIB_POLYGON

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	_BUILDING_LIB

#ifdef	_WIN32

#ifdef	cedric
#pragma library ("F:\PROJET\LIB386\POL_WORK\NT_POLY.LIB")
#else
#pragma library ("NT_POLY.LIB")
#endif



#else//	_WIN32

#ifdef	cedric
#pragma library ("F:\PROJET\LIB386\POL_WORK\DOS_POLY.LIB")
#else
#pragma library ("DOS_POLY.LIB")
#endif

#endif//_WIN32

#endif//_BUILDING_LIB

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <SYSTEM\ADELINE.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <POLYGON\POLY.H>
#include <POLYGON\TESTVUE.H>
#include <POLYGON\CLIPPERZ.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_POLYGON

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
