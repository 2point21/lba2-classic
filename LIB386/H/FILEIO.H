//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	LIB_FILEIO
#define	LIB_FILEIO

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	_BUILDING_LIB

#ifdef	_WIN32

#ifdef	__SW_3R
#pragma library ("NT_FIOR.LIB")
#else//	__SW_3R
#pragma library ("NT_FIOS.LIB")
#endif//__SW_3R

#else//	_WIN32

#ifdef	__SW_3R
#pragma library ("DOS_FIOR.LIB")
#else//	__SW_3R
#pragma library ("DOS_FIOS.LIB")
#endif//__SW_3R

#endif//_WIN32

#endif//_BUILDING_LIB

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<FILEIO\LOADPCX.H>
#include	<FILEIO\SAVEPCX.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_FILEIO

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
