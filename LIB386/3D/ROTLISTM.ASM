;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include iammx.inc

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

		extrn SYSCALL	MMX_DEMI:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	RotateListM

;*══════════════════════════════════════════════════════════════════════════*
;void	RotateListM(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux RotateListM	"*"		\
;	parm caller	[ebx] [edi] [esi] [ecx]	\
;	modify exact	[ecx esi edi]

RotateListM	proc

 		shl		ecx, 3
 		jz		Exit

 		lea		edi, [edi+ecx-8]
 		add		esi, ecx

		neg		ecx

		test		ecx, 8
		jz		EvenPoint

		movq		mm5, [esi+ecx]	; mm5 = . Z  Y  X	2
		add		ecx, 8

 		movq		mm6, mm5	; mm6 = . Z  Y  X	2
  		movq		mm7, mm5	; mm7 = . Z  Y  X	2

 		pmaddwd		mm5, [ebx]	; mm5 =  X'Z   X'XY	2

 		pmaddwd		mm6, [ebx+8]	; mm6 =  Y'Z   Y'XY	2
		movq		mm0, mm5	; mm0 =  X'Z   X'XY	2

 		pmaddwd		mm7, [ebx+16]	; mm7 =  Z'Z   Z'XY	2
		punpckldq	mm5, mm6	; mm5 =  Y'XY  X'XY	2

		movq		mm1, mm7	; mm1 =  Z'Z   Z'XY	2
		punpckhdq	mm0, mm6	; mm0 =  Y'Z   X'Z	2

		paddd		mm5, mm0	; mm5 =  Y'    X'	2 (<<14)
		psrlq		mm1, 32		; mm1 =  0     Z'Z	2

		paddd		mm5, [MMX_DEMI]
		paddd		mm1, mm7	; mm1 =  .     Z'	2 (<<14)

		paddd		mm1, [MMX_DEMI]
		psrad		mm5, 14		; mm5 =  Y'    X'	2

		psrad		mm1, 14		; mm1 =  .     Z'	2

		packssdw	mm5, mm1	; mm5 = . Z' Y' X'	2

		movq		[edi+ecx], mm5	;			2

		jz		Exit
EvenPoint:
 		movq		mm0, [esi+ecx]	; mm0 = . Z  Y  X

		movq		mm5, [esi+ecx+8]; mm6 = . Z  Y  X	2
 		movq		mm2, mm0	; mm2 = . Z  Y  X

 		pmaddwd		mm0, [ebx]	; mm0 =  X'Z   X'XY
 		movq		mm1, mm2	; mm1 = . Z  Y  X

 		pmaddwd		mm1, [ebx+8]	; mm1 =  Y'Z   Y'XY
  		movq		mm6, mm5	; mm7 = . Z  Y  X	2

 		pmaddwd		mm2, [ebx+16]	; mm2 =  Z'Z   Z'XY
 		movq		mm7, mm5	; mm6 = . Z  Y  X	2

		movq		mm3, mm0	; mm3 =  X'Z   X'XY
		jmp		EnterLoop
NextPoint:
 		movq		mm0, [esi+ecx]	; mm0 = . Z  Y  X
		packssdw	mm3, mm4	; mm3 = . Z' Y' X'

		movq		mm6, [esi+ecx+8]; mm6 = . Z  Y  X	2
		psrlq		mm1, 32		; mm1 =  0     Z'Z	2

		paddd		mm5, [MMX_DEMI]
 		movq		mm2, mm0	; mm2 = . Z  Y  X

		paddd		mm1, [MMX_DEMI]
		psrad		mm5, 14		; mm5 =  Y'    X'	2

		movq		[edi+ecx-8], mm3
		paddd		mm1, mm7	; mm1 =  .     Z'	2 (<<14)

 		pmaddwd		mm0, [ebx]	; mm0 =  X'Z   X'XY
		psrad		mm1, 14		; mm1 =  .     Z'	2

		packssdw	mm5, mm1	; mm5 = . Z' Y' X'	2
 		movq		mm1, mm2	; mm1 = . Z  Y  X

 		pmaddwd		mm1, [ebx+8]	; mm1 =  Y'Z   Y'XY
  		movq		mm7, mm6	; mm7 = . Z  Y  X	2

 		pmaddwd		mm2, [ebx+16]	; mm2 =  Z'Z   Z'XY
		movq		mm3, mm0	; mm3 =  X'Z   X'XY

		movq		[edi+ecx], mm5	;			2
		movq		mm5, mm6	; mm6 = . Z  Y  X	2
EnterLoop:
 		pmaddwd		mm5, [ebx]	; mm5 =  X'Z   X'XY	2
		punpckldq	mm0, mm1	; mm0 =  Y'XY  X'XY

 		pmaddwd		mm6, [ebx+8]	; mm6 =  Y'Z   Y'XY	2
		punpckhdq	mm3, mm1	; mm3 =  Y'Z   X'Z

 		pmaddwd		mm7, [ebx+16]	; mm7 =  Z'Z   Z'XY	2
		paddd		mm3, mm0	; mm3 =  Y'    X'	(<<14)

		add		ecx, 16
		movq		mm0, mm5	; mm0 =  X'Z   X'XY	2

		movq		mm4, mm2	; mm4 =  Z'Z   Z'XY
		punpckldq	mm5, mm6	; mm5 =  Y'XY  X'XY	2

		psrlq		mm4, 32		; mm4 =  0     Z'Z
		movq		mm1, mm7	; mm1 =  Z'Z   Z'XY	2

		punpckhdq	mm0, mm6	; mm0 =  Y'Z   X'Z	2
		paddd		mm3, [MMX_DEMI]

		paddd		mm4, mm2	; mm4 =  .     Z'	(<<14)
		psrad		mm3, 14		; mm3 =  Y'    X'

		psrad		mm4, 14		; mm4 =  .     Z'
		paddd		mm5, mm0	; mm5 =  Y'    X'	2 (<<14)

 		jnz		NextPoint

		paddd		mm1, [MMX_DEMI]
		packssdw	mm3, mm4	; mm3 = . Z' Y' X'

		paddd		mm5, [MMX_DEMI]
		psrlq		mm1, 32		; mm1 =  0     Z'Z	2

		paddd		mm1, mm7	; mm1 =  .     Z'	2 (<<14)
		psrad		mm5, 14		; mm5 =  Y'    X'	2

		movq		[edi+ecx-8], mm3
		psrad		mm1, 14		; mm1 =  .     Z'	2
EndPoint:
		packssdw	mm5, mm1	; mm5 = . Z' Y' X'	2

		movq		[edi+ecx], mm5	;			2
Exit:
		ret

RotateListM	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		End
