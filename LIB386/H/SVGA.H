//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	LIB_SVGA
#define	LIB_SVGA

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	_BUILDING_LIB

#ifdef	_WIN32

#ifdef cedric

#ifdef	__SW_3R
#pragma library ("F:\PROJET\LIB386\SVGA\NT_SVGR.LIB")
#else//	__SW_3R
#pragma library ("F:\PROJET\LIB386\SVGA\NT_SVGS.LIB")
#endif//__SW_3R

#else//cedric

#ifdef	__SW_3R
#pragma library ("NT_SVGR.LIB")
#else//	__SW_3R
#pragma library ("NT_SVGS.LIB")
#endif//__SW_3R

#endif//cedric



#pragma library ("DDRAW.LIB")

#else//	_WIN32

#ifdef cedric

#ifdef	__SW_3R
#pragma library ("F:\PROJET\LIB386\SVGA\DOS_SVGR.LIB")
#else//	__SW_3R
#pragma library ("F:\PROJET\LIB386\SVGA\DOS_SVGS.LIB")
#endif//__SW_3R

#else //cedric

#ifdef	__SW_3R
#pragma library ("DOS_SVGR.LIB")
#else//	__SW_3R
#pragma library ("DOS_SVGS.LIB")
#endif//__SW_3R

#endif //cedric

#endif//_WIN32

#endif//_BUILDING_LIB

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <SYSTEM\ADELINE.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <POLYGON.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <SVGA\SCREENXY.H>
#include <SVGA\COPYBOX.H>
#include <SVGA\COPYBOV.H>
#include <SVGA\BLITBOX.H>
#include <SVGA\BLITBOV.H>
#include <SVGA\CLRBOX.H>
#include <SVGA\CLEABOV.H>
#include <SVGA\DIRTYBOX.H>
#include <SVGA\LOGPHYS.H>
#include <SVGA\INITMODE.H>
#include <SVGA\VESA.H>
#include <SVGA\DDRAW.H>
#include <SVGA\AFFSTR.H>
#include <SVGA\MASK.H>
#include <SVGA\CALCMASK.H>
#include <SVGA\COPYMASK.H>
#include <SVGA\GRAPH.H>
#include <SVGA\FONT.H>
#include <SVGA\GPRINTF.H>
#include <SVGA\CLIP.H>
#include <SVGA\SHADE.H>
#include <SVGA\CPYBLOCK.H>
#include <SVGA\CPYBLOCI.H>
#include <SVGA\RESBLOCK.H>
#include <SVGA\SAVBLOCK.H>
#include <SVGA\BOX.H>
#include <SVGA\FIL_LINE.H>
#include <SVGA\RECT.H>
#include <SVGA\PLOT.H>
#include <SVGA\SCALEBOX.H>
#include <SVGA\SCALESPT.H>
#include <SVGA\SCALESPI.H>
#include <SVGA\INIMOUSE.H>
#include <SVGA\MOUSE.H>
#include <SVGA\PAL.H>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_SVGA

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
