//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_3D
#define LIB_3D

//──────────────────────────────────────────────────────────────────────────
#ifndef	_BUILDING_LIB

#ifdef	_WIN32
#pragma library ("NT_3D.LIB")
#else//	_WIN32
#ifdef	cedric
#pragma library ("F:\PROJET\LIB386\3D\DOS_3D.LIB")
#else	// cedric
#pragma library ("DOS_3D.LIB")
#endif // cedric
#endif//_WIN32

#endif//_BUILDING_LIB

//──────────────────────────────────────────────────────────────────────────
#include <SYSTEM\ADELINE.H>

//──────────────────────────────────────────────────────────────────────────
#include <3D\ARCSIN.H>
#include <3D\CAMERA.H>
#include <3D\COPYMAT.H>
#include <3D\DATAMAT.H>
#include <3D\DISTANCE.H>
#include <3D\FLIPMAT.H>
#include <3D\GETANG2D.H>
#include <3D\GETANG3D.H>
#include <3D\IMAT.H>
#include <3D\IMAT3DS.H>
#include <3D\IMATSTD.H>
#include <3D\IMATTRA.H>
#include <3D\GMATTRA.H>
#include <3D\INVCAM.H>
#include <3D\LIGHT.H>
#include <3D\LIROT3D.H>
#include <3D\PROJ.H>
#include <3D\LPROJ.H>
#include <3D\LROT2D.H>
#include <3D\LROT3D.H>
#include <3D\MAT.H>
#include <3D\MULMAT.H>
#include <3D\REALVAL.H>
#include <3D\REGLE3.H>
#include <3D\ROT2D.H>
#include <3D\ROT3D.H>
#include <3D\IROT3D.H>
#include <3D\ROTLIST.H>
#include <3D\ROTRALIS.H>
#include <3D\LROTLIST.H>
#include <3D\LROTRALI.H>
#include <3D\ROTMAT.H>
#include <3D\ROTVECT.H>
#include <3D\SINTAB.H>
#include <3D\SINTABF.H>
#include <3D\SQRROOT.H>
#include <3D\TANTAB.H>
#include <3D\MOVE.H>

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_3D

//──────────────────────────────────────────────────────────────────────────
