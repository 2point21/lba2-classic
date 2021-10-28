;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include iammx.inc
		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

		extrn SYSCALL	MMX_DEMI2:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	LongRotTransListM

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotTransListM(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LongRotTransListM	"*"		\
;	parm caller	[ebx] [edi] [esi] [ecx]	\
;	modify exact	[ecx esi edi]

LongRotTransListM	proc

		lea		ecx, [ecx+ecx*2]

		test		ecx, ecx
		jz		Exit

		lea		esi, [esi+ecx*4]
		lea		ecx, [ecx*4]

		add		edi, ecx
		xor		ecx, -1

		inc		ecx
NextPoint:
		movq		mm6, [esi+ecx]	; mm6 = Yh Yl Xh Xl

		movdt		mm5, [esi+ecx+8]; mm5 = 0  0  Zh Zl
		paddd		mm6, mm6

		movq		mm7, mm6	; mm6 = Yh Yl Xh Xl
		paddd		mm5, mm5

		movq		mm4, mm5	; mm4 = 0  0  Zh Zl

		movdt		mm7, [esi+ecx+4]; mm7 = 0  0  Yh Yl
		psllq		mm5, 16		; mm5 = 0  Zh Zl 0

		movq		mm0, [ebx]	; mm0 = 0  3  2  1		1
		punpcklwd	mm6, mm7	; mm6 = Yh Xh Yl Xl

		movq		mm7, mm6	; mm7 = Yh Xh Yl Xl
		punpckldq	mm6, mm4	; mm6 = .  Zl Yl Xl

		movq		mm1, mm0	; mm1 = 0  3  2  1		1
		punpckhdq	mm7, mm5	; mm7 = 0  Zh Yh Xh

		movq		mm2, [ebx+8]	; mm2 = 0  3  2  1		2
		psrlw		mm6, 1		; mm6 = .  Zl Yl Xl  ( >>1  )

		movq		mm3, mm2	; mm3 = 0  3  2  1		2
		pmaddwd		mm0, mm6	; mm0 = Xl'Z  Xl'XY  ( <<13 )	1

		movq		mm4, [ebx+16]	; mm4 = 0  3  2  1		3
		pmaddwd		mm1, mm7	; mm1 = Xh'Z  Xh'XY  ( >>2  )	1

		movq		mm5, mm4	; mm5 = 0  3  2  1		3
		pmaddwd		mm2, mm6	; mm2 = Yl'Z  Yl'XY  ( <<13 )	2

		paddd		mm0, [MMX_DEMI2]
		pmaddwd		mm3, mm7	; mm3 = Yh'Z  Yh'XY  ( >>2  )	2

		psrad		mm0, 13		; mm0 = Xl'Z  Xl'XY		1
		pmaddwd		mm5, mm7	; mm5 = Zh'Z  Zh'XY  ( >>2  )	3

		pslld		mm1, 2		; mm1 = Xh'Z  Xh'XY		1
		paddd		mm2, [MMX_DEMI2]

		psrad		mm2, 13		; mm2 = Yl'Z  Yl'XY		2
		paddd		mm0, mm1	; mm0 =  X'Z   X'XY		1

		pslld		mm3, 2		; mm3 = Yh'Z  Yh'XY		2
		pmaddwd		mm4, mm6	; mm4 = Zl'Z  Zl'XY  ( <<13 )	3

		paddd		mm2, mm3	; mm2 =  Y'Z   Y'XY		2
		movq		mm3, mm0	; mm3 =  X'Z   X'XY

		punpckldq	mm0, mm2	; mm0 =  Y'XY  X'XY

		paddd		mm4, [MMX_DEMI2]
		punpckhdq	mm3, mm2	; mm3 =  Y'Z   X'Z

		paddd		mm0, mm3	; mm0 =  Y'    X'
		pslld		mm5, 2		; mm5 = Zh'Z  Zh'XY		3

		paddd		mm0, [ebx+32]	; mm0 =  Y'    X'  + LONG TRANS
		psrad		mm4, 13		; mm4 = Zl'Z  Zl'XY		3

		paddd		mm4, mm5	; mm4 =  Z'Z   Z'XY		3
		psrad		mm0, 1

		movq		mm5, mm4	; mm5 =  Z'Z   Z'XY		3
		psrlq		mm4, 32		; mm4 =   0    Z'Z		3

		movq		[edi+ecx], mm0
		paddd		mm5, mm4	; mm5 =   .    Z'		3

		paddd		mm5, [ebx+40]	; mm5 =   .    Z'  + LONG TRANS

		movdf		[edi+ecx+8], mm5

		add		ecx, 12
		jnz		NextPoint
Exit:
		ret

LongRotTransListM	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		End

