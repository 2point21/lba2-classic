//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	LIB_SYSTEM
#define	LIB_SYSTEM

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#ifndef	_BUILDING_LIB

#ifdef	_WIN32

#ifdef	__SW_3R
#pragma library ("NT_SYSR.LIB")
#else//	__SW_3R
#pragma library ("NT_SYSS.LIB")
#endif//__SW_3R

#pragma library ("MSS32.LIB")

#else//	_WIN32

#ifdef	__SW_3R
#pragma library ("DOS_SYSR.LIB")
#else//	__SW_3R
#pragma library ("DOS_SYSS.LIB")
#endif//__SW_3R

#pragma library ("MSS.LIB")

#endif//_WIN32

#endif//_BUILDING_LIB

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<stdlib.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include	<SYSTEM\ADELINE.H>
#include	<SYSTEM\FASTCPY.H>
#include	<SYSTEM\FASTSET.H>
#include	<SYSTEM\AVAILMEM.H>
#include	<SYSTEM\A_MALLOC.H>
#include	<SYSTEM\N_MALLOC.H>
#include	<SYSTEM\S_MALLOC.H>
#include	<SYSTEM\DOSMALLO.H>
#include	<SYSTEM\CPU.H>
#include	<SYSTEM\DISPCPU.H>
#include	<SYSTEM\DISPOS.H>
//#include	<SYSTEM\DLLLOAD.H>
#include	<SYSTEM\EXIT.H>
#include	<SYSTEM\FILEBAK.H>
#include	<SYSTEM\FILECOPY.H>
#include	<SYSTEM\FILENAME.H>
#include	<SYSTEM\FILES.H>
#include	<SYSTEM\HQFILE.H>
#include	<SYSTEM\DEFFILE.H>
#include	<SYSTEM\HQMEM.H>
#include	<SYSTEM\HQR.H>
#include	<SYSTEM\HQRLOAD.H>
#include	<SYSTEM\HQRMEM.H>
#include	<SYSTEM\HQRMLOAD.H>
#include	<SYSTEM\HQRRESS.H>
#include	<SYSTEM\INITKEYB.H>
#include	<SYSTEM\INPUT.H>
#include	<SYSTEM\LOADMALL.H>
#include	<SYSTEM\LOADSAVE.H>
#include	<SYSTEM\LZ.H>
#include	<SYSTEM\PATCH.H>
#include	<SYSTEM\INITIMER.H>
#include	<SYSTEM\LOGPRINT.H>
#include	<SYSTEM\ITOA.H>
#include	<SYSTEM\CMDLINE.H>
#include	<SYSTEM\DISKDIR.H>

#ifdef	_WIN32
#include	<SYSTEM\WINSYS.H>
#include	<AIL\MSSW.H>
#else// _WIN32
#include	<SYSTEM\DOSSYS.H>
#include	<AIL\MSS.H>
#endif//_WIN32

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define INIT_SVGA               (1<<0)
#define INIT_SAMPLE             (1<<1)
#define INIT_MIDI               (1<<2)
#define INIT_SMACKER            (1<<3)
#define INIT_VESA               (1<<4)

#define INIT_SYSTEM             (1<<6)
#define INIT_KEYB               (1<<7)
#define INIT_TIMER              (1<<8)
#define INIT_MOUSE              (1<<9)
#define IGNORE_VESA_ERROR       (1<<10)

#define SMART_LOG               (1<<12)
#define INIT_DEFFILE		(1<<13)
#define INIT_3D                 (1<<14)
#define INIT_ADELINE            (1<<16)
#define INIT_LOG	        (1<<17)
#define INIT_QUIET	        (1<<18)


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#define Rnd(n)  (rand()%(n))

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
S32		MULDIV64( S32 x, S32 mul, S32 div ); // (x*mul)/div

#pragma	aux	MULDIV64= 				\
		"	imul	edx		"	\
		"	idiv	ecx		"	\
		parm	caller 	[eax] [edx] [ecx]	\
		modify	exact	[eax edx]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Hard coded breakpoint
void            BREAKPOINT();

#pragma	aux	BREAKPOINT = "int 3"

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Tuning functions

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	StartCycleCounter(U64 *counter)	;

#pragma	aux	StartCycleCounter =	\
"	.586			"	\
"	rdtsc			"	\
"	mov	[edi], eax	"	\
"	mov	[edi+4], edx	"	\
	parm		[edi]		\
	modify	exact	[eax edx]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
U32	GetCycleCounter(U64 *counter)	;

#pragma	aux	GetCycleCounter =	\
"	.586			"	\
"	rdtsc			"	\
"	mov	ecx, [edi]	"	\
"	mov	[edi], eax	"	\
"	mov	[edi+4], edx	"	\
"	sub	eax, ecx	"	\
	parm		[edi]		\
	modify	exact	[eax edx]	\
	value		[eax]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
U32	MeanCycleCounter(U64 *counter, U32 iter);

#pragma	aux	MeanCycleCounter =	\
"	.586			"	\
"	rdtsc			"	\
"	mov	ecx, [edi]	"	\
"	mov	ebx, [edi+4]	"	\
"	mov	[edi], eax	"	\
"	mov	[edi+4], edx	"	\
"	sub	eax, ecx	"	\
"	sbb	edx, ebx	"	\
"	div	esi		"	\
	parm		[edi] [esi]	\
	modify	exact	[eax edx]	\
	value		[eax]

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif//LIB_SYSTEM

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
