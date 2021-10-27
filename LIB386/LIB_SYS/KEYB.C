
#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_svga\lib_svga.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
#include	<i86.h>

extern	UWORD	Old_PM09_Sel	;
extern	ULONG	Old_PM09_Off	;

#ifdef	KEYB_REAL
UWORD	Old_RM09_Seg	;
ULONG	Old_RM09_Off	;
#endif

extern	void	__interrupt	NewIntPM09(void)	;

//#define	int15

/*-------------------------------------------------------------------------*/
void	InitKeyboard()
{
	union	REGS	r	;
	struct	SREGS	sr	;
	void 	far	*fh	;
	void	*ptrealproc	;

/*----- Save Old Protected Mode Vector 09 ------*/
#ifdef	int15
	r.x.eax = 0x3515		;/*	Function 35h, for Int 9	*/
#else
	r.x.eax = 0x3509		;/*	Function 35h, for Int 9	*/
#endif
	sr.ds = sr.es = 0		;/*	Security (kill the cat)	*/
	int386x( 0x21, &r, &r, &sr )	;/*	Call Int 21		*/
	Old_PM09_Sel = (UWORD)sr.es	;/*	ES:EBX=Old Interrupt 09	*/
	Old_PM09_Off = r.x.ebx		;

#ifdef	KEYB_REAL
/*-----	Save Old Real Mode Vector 09 -----------*/
	r.x.eax = 0x0200		;/*	DPMI Get Real Mode Vect	*/
	r.h.bl = 0x09			;/*	Interrupt Number	*/
	int386( 0x31, &r, &r )		;/*	Invoke DPMI		*/
	Old_RM09_Seg = (UWORD)r.x.ecx	;/*	CX:DX=Old Interrupt 09	*/
	Old_RM09_Off = (UWORD)r.x.edx	;

/*----- Create 128 Byte under First Meg for NewIntRM09 in Real Mode ----*/
	ptrealproc = DosMalloc(512)	;/*	Prog will be copied here*/
	memcpy( ptrealproc, (void*)NewIntRM09, 512 )	;
#endif
/*----- Install New Protected Vector 09 --------*/
#ifdef	int15
	r.x.eax = 0x2515		;/*	Function 25h for int 09	*/
#else
	r.x.eax = 0x2509		;/*	Function 25h for int 09	*/
#endif
	fh = (void far*)NewIntPM09	;/*	Get far Pointer		*/
	r.x.edx = FP_OFF( fh )		;/*	Get Offset		*/
	sr.ds 	= FP_SEG( fh )		;/*	Get	Segment		*/
	sr.es 	= 0			;/*	Security ( ... )	*/
	int386x( 0x21, &r, &r, &sr )	;/*	Invoke DPMI		*/

#ifdef	KEYB_REAL
/*----- Install New Real Vector 09 -------------*/
	r.x.eax = 0x0201		;/*	DPMI Set Real Mode Vect	*/
	r.h.bl = 0x09			;/*	Interrupt Number	*/
	r.x.ecx = GetRealSeg(ptrealproc);/*	Get Segment of ptreal	*/
	r.x.edx = GetRealOff(ptrealproc);/*	Get Offset of ptreal	*/
	int386( 0x31, &r, &r )		;
#endif
}
/*-------------------------------------------------------------------------*/
void	ClearKeyboard()
{
	union	REGS	r		;
	struct	SREGS	sr		;

/*----- Restore Old Protected Vector 09 --------*/
#ifdef	int15
	r.x.eax = 0x2515		;/*	Set Vector Interupt PM09*/
#else
	r.x.eax = 0x2509		;/*	Set Vector Interupt PM09*/
#endif
	r.x.edx = Old_PM09_Off		;/*	Adr Old Interrupt 09	*/
	sr.ds = Old_PM09_Sel		;
	sr.es = 0			;/*	... (...)		*/
	int386x( 0x21, &r, &r, &sr )	;/*	Invoke int 21		*/

#ifdef	KEYB_REAL
/*----- Restore Old Real Vector 08--------------*/
	r.x.eax = 0x0201		;/*	DPMI Set Real Mode Vect	*/
	r.h.bl = 0x09			;/*	Interrupt Number 09	*/
	r.x.ecx = (ULONG)Old_RM09_Seg	;/*	Old Segment Interrupt	*/
	r.x.edx = (ULONG)Old_RM09_Off	;/*	Old Offset Interrupt	*/
	int386( 0x31, &r, &r )		;/*	Invoke DPMI		*/
#endif
}
/*-------------------------------------------------------------------------*/
























