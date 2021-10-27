//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef LIB_SYSTEM_ADELINE
#define LIB_SYSTEM_ADELINE

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32
#include	<windows.h>
#include 	<windowsx.h>
#include	<winbase.h>
#include 	<mmsystem.h>
#ifndef	_NO_DDRAW
#include 	<ddraw.h>
#endif//_NO_DDRAW
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef TYPEDEFS
#define TYPEDEFS

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef __cplusplus
extern "C" {
#endif

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
typedef	unsigned	char		U8	;
typedef	signed		char		S8	;
typedef	unsigned	short		U16	;
typedef	signed		short		S16	;
typedef	unsigned	long		U32	;
typedef	signed		long		S32	;

typedef	struct
	{
		U32	Low ;
		U32	High;
	}				U64	;
typedef	struct
	{
		U32	Low ;
		S32	High;
	}				S64	;

typedef	void				(VOID_FUNC)();

typedef	U64	*			PTR_U64	;
typedef	S64	*			PTR_S64	;
typedef	U32	*			PTR_U32	;
typedef	S32	*			PTR_S32	;
typedef	U16	*			PTR_U16	;
typedef	S16	*			PTR_S16	;
typedef	U8	*			PTR_U8	;
typedef	S8	*			PTR_S8	;

typedef	VOID_FUNC	*		PTR_VOID_FUNC;

typedef	void	*			PTR_VOID;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#pragma	aux	Flat_Name	"*"

#pragma	aux	(Flat_Name)	U64
#pragma	aux	(Flat_Name)	S64
#pragma	aux	(Flat_Name)	U32
#pragma	aux	(Flat_Name)	S32
#pragma	aux	(Flat_Name)	U16
#pragma	aux	(Flat_Name)	S16
#pragma	aux	(Flat_Name)	U8
#pragma	aux	(Flat_Name)	S8

#pragma aux     (Flat_Name)     VOID_FUNC

#pragma aux     (Flat_Name)     PTR_U64
#pragma aux     (Flat_Name)     PTR_S64
#pragma aux     (Flat_Name)     PTR_U32
#pragma aux     (Flat_Name)     PTR_S32
#pragma aux     (Flat_Name)     PTR_U16
#pragma aux     (Flat_Name)     PTR_S16
#pragma aux     (Flat_Name)     PTR_U8
#pragma aux     (Flat_Name)     PTR_S8

#pragma aux     (Flat_Name)     PTR_VOID

#pragma aux     (Flat_Name)     PTR_VOID_FUNC

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// DO NOT USE THOSE TYPES !!!

typedef unsigned char  UBYTE;
typedef unsigned short UWORD;
typedef unsigned long  ULONG;

#ifndef	_WIN32

typedef char  BYTE;
typedef short WORD;

#endif//_WIN32

typedef long  LONG;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define	AND		&&
#define	OR		||

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define	TRUE		1
#define	FALSE		0

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef  _WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
typedef	S32 (WINDOW_PROC)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#pragma	aux	(Flat_Name)	WINDOW_PROC

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
typedef	WINDOW_PROC * WINDOW_PROC_PTR	;

#pragma	aux	(Flat_Name)	WINDOW_PROC_PTR

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef	_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define	AIL_vmm_lock(a, b)
#define	AIL_vmm_unlock(a, b)

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#else//	_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define		ManageTime()
#define		ManageSystem()	ManageEvents()

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef _WIN32
#ifndef  DPMI
#define  DPMI	// we use DOS/4GW... mainly for DLLLOAD.C (Miles)
#endif// DPMI
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//TYPEDEFS

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifdef __cplusplus
}
#endif

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_SYSTEM_ADELINE

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
