;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL
		INCLUDE	IAMMX.INC		; Pour pouvoir assembler du MMx

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

		extrn SYSCALL	MMX_DEMI2:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	LongInverseRotatePointM

;*══════════════════════════════════════════════════════════════════════════*
;void	LongInverseRotatePointM(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongInverseRotatePointM			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[]

LongInverseRotatePointM proc

		movq		mm0, [esi]	; mm0 =  0 13 12 11
		pxor		mm6, mm6	; mm6 =  0  0  0  0

		movq		mm2, [esi+8]	; mm2 =  0 23 22 21
		movq		mm4, mm0	; mm4 =  0 13 12 11

		movq		mm3, [esi+16]	; mm3 =  0 33 32 31
		punpckhwd 	mm4, mm2	; mm4 =  0  0 23 13

		movq		mm1, mm3	; mm1 =  0 33 32 31
		punpcklwd	mm0, mm2	; mm0 = 22 12 21 11

		movq		mm2, mm0	; mm2 = 22 12 21 11
		psrlq		mm3, 32		; mm3 =  0  0  0 33

		movdt		mm5, ecx	; mm5 = 0  0  Zh Zl
		punpcklwd	mm1, mm6	; mm1 =  0 32  0 31

		movdt		mm6, eax	; mm6 = 0  0  Xh Xl
		punpckldq 	mm0, mm1	; mm0 =  0 31 21 11		1

		movdt		mm7, ebx	; mm7 = 0  0  Yh Yl
		punpckhdq 	mm2, mm1	; mm2 =  0 32 22 12		2

		punpckldq 	mm4, mm3	; mm4 =  0 33 23 13		3
		movq		mm3, mm5	; mm3 = 0  0  Zh Zl

		movq		mm1, mm0	; mm1 = 0  3  2  1		1
		psllq		mm5, 16		; mm5 = 0  Zh Zl 0

		punpcklwd	mm6, mm7	; mm6 = Yh Xh Yl Xl

		movq		mm7, mm6	; mm7 = Yh Xh Yl Xl
		punpckldq	mm6, mm3	; mm6 = .  Zl Yl Xl

		psrlw		mm6, 1		; mm6 = .  Zl Yl Xl  ( >>1  )
		movq		mm3, mm2	; mm3 = 0  3  2  1		2

		punpckhdq	mm7, mm5	; mm7 = 0  Zh Yh Xh
		pmaddwd		mm0, mm6	; mm0 = Xl'Z  Xl'XY  ( <<13 )	1

		movq		mm5, mm4	; mm5 = 0  3  2  1		3
		pmaddwd		mm1, mm7	; mm1 = Xh'Z  Xh'XY  ( >>2  )	1

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

		punpckhdq	mm3, mm2	; mm3 =  Y'Z   X'Z
		paddd		mm4, [MMX_DEMI2]

		paddd		mm0, mm3	; mm0 =  Y'    X'
		pslld		mm5, 2		; mm5 = Zh'Z  Zh'XY		3

		psrad		mm4, 13		; mm4 = Zl'Z  Zl'XY		3

		movq		[X0], mm0
		paddd		mm4, mm5	; mm4 =  Z'Z   Z'XY		3

		movq		mm5, mm4	; mm5 =  Z'Z   Z'XY		3
		psrlq		mm4, 32		; mm4 =   0    Z'Z		3

		paddd		mm5, mm4	; mm5 =   .    Z'		3

		movdf		[Z0], mm5

		ret

LongInverseRotatePointM endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
