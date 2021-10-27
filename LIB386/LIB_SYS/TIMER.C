#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_svga\lib_svga.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
#include	<i86.h>


#define	DIVISOR		23864		/*    1193180 / 50	*/
#define	FREQUENCE	50		/*    1193180 / 23864	*/


#define	OLD_DIVISOR	0xFFFF


extern	void	__interrupt 		NewIntPM08(void)	;

ULONG	Old_PM08_Off	;
UWORD	Old_PM08_Sel	;

//UWORD	Old_RM08_Off	;
//UWORD	Old_RM08_Seg	;


//UWORD	Old_RMF2_Off	;
//UWORD	Old_RMF2_Seg	;


/*-------------------------------------------------------------------------*/

void	InitTimer()
{
	union	REGS	r	;
	struct	SREGS	sr	;
	void 	far	*fh	;

/*-----	Save Old Real Mode Vector F2 -----------*/
//	r.x.eax = 0x0200		;/*	DPMI Get Real Mode Vect	*/
//	r.h.bl = 0xF2			;/*	Interrupt Number	*/
//	int386( 0x31, &r, &r )		;/*	Invoke DPMI		*/
//	Old_RMF2_Seg = (UWORD)r.x.ecx	;/*	CX:DX=Old Interrupt F2	*/
//	Old_RMF2_Off = (UWORD)r.x.edx	;

/*-----	Save Old Real Mode Vector 08 -----------*/
//	r.x.eax = 0x0200		;/*	DPMI Get Real Mode Vect	*/
//	r.h.bl = 0x08			;/*	Interrupt Number	*/
//	int386( 0x31, &r, &r )		;/*	Invoke DPMI		*/
//	Old_RM08_Seg = (UWORD)r.x.ecx	;/*	CX:DX=Old Interrupt 08	*/
//	Old_RM08_Off = (UWORD)r.x.edx	;

/*----- Install it as New Real Vector F2 -------*/
//	r.x.eax = 0x0201		;/*	DPMI Set Real Mode Vect	*/
//	r.h.bl = 0xF2			;/*	Interrupt Number	*/
//	int386( 0x31, &r, &r )		;

/*----- Save Old Protected Mode Vector 08 ------*/
	r.x.eax = 0x3508		;/*	Function 35h, for Int 08*/
	sr.ds = sr.es = 0		;/*	Security (kill the cat)	*/
	int386x( 0x21, &r, &r, &sr )	;/*	Call Int 21		*/
	Old_PM08_Sel = (UWORD)sr.es	;/*	ES:EBX=Old Interrupt 08	*/
	Old_PM08_Off = r.x.ebx		;

/*----- Install New Protected Vector 08 --------*/
	r.x.eax = 0x2508		;/*	Function 25h for int 08	*/
	fh = (void far*)NewIntPM08	;/*	Get far Pointer		*/
	r.x.edx = FP_OFF( fh )		;/*	Get Offset		*/
	sr.ds 	= FP_SEG( fh )		;/*	Get	Segment		*/
	sr.es 	= 0			;/*	Security ( ... )	*/
	int386x( 0x21, &r, &r, &sr )	;/*	Invoke DPMI		*/

/*----- It's Ok, Now I can accelerate Timer 08	*/

	SetTimer( DIVISOR )		;
}

/*-------------------------------------------------------------------------*/

void	ClearTimer()
{
	union	REGS	r		;
	struct	SREGS	sr		;

/*----- Ok, it's finish, stop running... -------*/

	SetTimer( OLD_DIVISOR )		;

/*----- Restore Old Protected Vector 08 --------*/
	r.x.eax = 0x2508		;/*	Set Vector Interupt PM08*/
	r.x.edx = Old_PM08_Off		;/*	Adr Old Interrupt 08	*/
	sr.ds = Old_PM08_Sel		;
	sr.es = 0			;/*	... (...)		*/
	int386x( 0x21, &r, &r, &sr )	;/*	Invoke int 21		*/

/*-----	Restore Old Real Mode Vector 08 -----------*/
//	r.x.eax = 0x0200		;/*	DPMI Get Real Mode Vect	*/
//	r.h.bl = 0xF2			;/*	Interrupt Number	*/
//	int386( 0x31, &r, &r )		;/*	Invoke DPMI		*/

//	r.x.eax = 0x0201		;/*	DPMI Set Real Mode Vect	*/
//	r.h.bl = 0x08			;/*	Interrupt Number	*/
//	int386( 0x31, &r, &r )		;

/*-----	Restore Old Real Mode Vector F2 -----------*/
//	r.x.eax = 0x0201		;/*	DPMI Get Real Mode Vect	*/
//	r.h.bl = 0xF2			;/*	Interrupt Number	*/
//	r.x.ecx = Old_RMF2_Seg		;/*	CX:DX=Old Interrupt F2	*/
//	r.x.edx = Old_RMF2_Off	 	;
//	int386( 0x31, &r, &r )		;/*	Invoke DPMI		*/
}

/*-------------------------------------------------------------------------*/
