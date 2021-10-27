;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		INCLUDE	IAMMX.INC		; Pour pouvoir assembler du MMx

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

		extrn SYSCALL	MMX_DEMI:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	InverseRotatePointM

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	InverseRotatePointM(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux InverseRotatePointM		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

InverseRotatePointM	PROC

		movq		mm0, [esi]	; mm0 =  0 13 12 11
		pxor		mm6, mm6	; mm6 =  0  0  0  0

		movq		mm2, [esi+8]	; mm2 =  0 23 22 21
		movq		mm4, mm0	; mm4 =  0 13 12 11

		movq		mm3, [esi+16]	; mm3 =  0 33 32 31
		punpckhwd 	mm4, mm2	; mm4 =  0  0 23 13

		movq		mm1, mm3	; mm1 =  0 33 32 31
		psrlq		mm3, 32		; mm3 =  0  0  0 33

		punpcklwd	mm1, mm6	; mm1 =  0 32  0 31
		shl		ebx, 16

		punpcklwd	mm0, mm2	; mm0 = 22 12 21 11
		and		eax, 0FFFFh

		movq		mm2, mm0	; mm2 = 22 12 21 11
		or		eax, ebx

		movdt		mm5, eax	; mm5 =  0  0  Y  X
		punpckldq 	mm0, mm1	; mm0 =  0 31 21 11

		movdt		mm6, ecx	; mm6 =  0  0  .  Z
		punpckhdq 	mm2, mm1	; mm2 =  0 32 22 12

		punpckldq	mm5, mm6	; mm5 =  .  Z  Y  X

		pmaddwd		mm0, mm5	; mm0 =  X'Z   X'XY
		punpckldq 	mm4, mm3	; mm4 =  0 33 23 13

		pmaddwd		mm2, mm5	; mm2 =  Y'Z   Y'XY

		pmaddwd		mm4, mm5	; mm4 =  Z'Z   Z'XY

		movq		mm1, mm0	; mm1 =  X'Z   X'XY

		punpckldq	mm0, mm2	; mm0 =  Y'XY  X'XY

		punpckhdq	mm1, mm2	; mm1 =  Y'Z    X'Z
		movq		mm3, mm4	; mm3 =  Z'Z   Z'XY

		paddd		mm0, mm1	; mm0 =   Y'     X'
		psrlq		mm3, 32		; mm3 =   0     Z'Z

		paddd		mm0, [MMX_DEMI]
		paddd		mm4, mm3	; mm4 =   ??     Z'

		paddd		mm4, [MMX_DEMI]
		psrad		mm0, 14

		psrad		mm4, 14

		movq		[X0], mm0

		movdf		[Z0], mm4

 		ret

InverseRotatePointM	ENDP

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End
