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

		public SYSCALL	RotatePointM

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotatePointM(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux RotatePointM			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotatePointM	proc

		movdt		mm0, eax	; mm0 =  0  0  .  X

		movdt		mm1, ebx	; mm0 =  0  0  .  Y

		movdt		mm2, ecx	; mm0 =  0  0  .  Z
		punpcklwd	mm0, mm1	; mm0 =  .  .  Y  X

		movq		mm5, [esi]	; mm5 =	 0 13 12 11
		punpckldq	mm0, mm2	; mm0 =  .  Z  Y  X

		movq		mm6, [esi+8]	; mm6 =	 0 23 22 21
		pmaddwd		mm5, mm0	; mm5 =  X'Z   X'XY

		pmaddwd		mm6, mm0	; mm6 =  Y'Z   Y'XY

		pmaddwd		mm0, [esi+16]	; mm0 =  Z'Z   Z'XY

		movq		mm1, mm5	; mm1 =  X'Z   X'XY

		punpckldq	mm5, mm6	; mm5 =  Y'XY  X'XY

		movq		mm2, mm0	; mm2 =  Z'Z   Z'XY
		punpckhdq	mm1, mm6	; mm1 =  Y'Z   X'Z

		paddd		mm5, mm1	; mm5 =  Y'    X'	(<<14)
		psrlq		mm0, 32		; mm0 =  0     Z'Z

		paddd		mm5, [MMX_DEMI]
		paddd		mm2, mm0	; mm1 =  .     Z'	(<<14)

		paddd		mm2, [MMX_DEMI]
		psrad		mm5, 14		; mm5 =  Y'    X'

		psrad		mm2, 14		; mm1 =  .     Z'

		movq		[X0], mm5

		;

		movdf		[Z0], mm2

 		ret

RotatePointM	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End
