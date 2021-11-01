;*══════════════════════════════════════════════════════════════════════════*

			.586

			.model SMALL, SYSCALL

			include mat.inc
			INCLUDE	IAMMX.INC	; Pour pouvoir assembler du MMx

;*══════════════════════════════════════════════════════════════════════════*

			.DATA


;*══════════════════════════════════════════════════════════════════════════*

			.CODE

			public	SYSCALL	FlipMatrixM

;*══════════════════════════════════════════════════════════════════════════*
;void	FlipMatrixM(S32 *MatDst, S32 *MatSrc) ;

;#pragma aux FlipMatrixM	"*"	\
;	parm		[edi] [esi]	\
;	modify exact	[]


; this routine can flip a matrix onto itself

FlipMatrixM		PROC

			movq		mm0,[esi]	; mm0 =  0 13 12 11
			pxor		mm6,mm6		; mm6 =  0  0  0  0

			movq		mm1,[esi+8]	; mm1 =  0 23 22 21
			movq		mm3,mm0		; mm3 =  0 13 12 11

			movq		mm2,[esi+16]	; mm2 =  0 33 32 31
			movq		mm4,mm1		; mm4 =  0 23 22 21

			movq		mm5,mm2		; mm5 =  0 33 32 31
			punpcklwd	mm0,mm1		; mm0 = 22 12 21 11

			psrlq		mm5,32		; mm5 =  0  0  0 33
			movq		mm1,mm0		; mm1 = 22 12 21 11

			punpcklwd	mm2,mm6		; mm2 =  0 32  0 31

			punpckldq 	mm0,mm2		; mm0 =  0 31 21 11

			punpckhdq 	mm1,mm2		; mm1 =  0 32 22 12

			movq		[edi],mm0
			punpckhwd 	mm3,mm4		; mm3 =  0  0 23 13

			movq		[edi+8],mm1
			punpckldq 	mm3,mm5		; mm3 =  0 33 23 13

			movq		[edi+16],mm3

			ret
FlipMatrixM		ENDP


;*══════════════════════════════════════════════════════════════════════════*
;			The
			End
