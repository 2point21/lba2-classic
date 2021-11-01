;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL
		INCLUDE	IAMMX.INC		; Pour pouvoir assembler du MMx

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	SinTab:DWORD
		extrn	SYSCALL	CosTab:DWORD

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Z0:DWORD

		extrn SYSCALL	MMX_DEMI2:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	LongRotateM

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongRotateM(S32 x, S32 z, S32 angle)	;

;#pragma aux	LongRotateM			\
;	parm		[eax] [ecx] [edx]	\
;	modify exact	[edx]


comment	@

LongRotateM	proc

		and		edx, 4095
		jz		norot

		add		edx, edx
		pxor		mm2, mm2	; mm2 = 0  0  0  0

		movdt		mm6, eax	; mm6 = 0  0  Xh Xl

		movdt		mm7, ecx	; mm7 = 0  0  Zh Zl

		movdt		mm0, SinTab[edx]; mm0 = 0  0  .  S	Penalty of 3 cycles with 50% chances
		punpcklwd	mm6, mm7	; mm6 = Zh Xh Zl Xl

		movdt		mm1, CosTab[edx]; mm1 = 0  0  .  C	If penalty above then another 3 cyles penalty
		psubw		mm2, mm0	; mm2 = 0  0  .  -S

		punpcklwd	mm2, mm1	; mm2 = .  .  C  -S

		punpcklwd	mm1, mm0	; mm1 = .  .  S  C

		movq		mm3, mm1	; mm3 = .  .  S  C
		psrlw		mm1, 1		; mm1 = .  .  S2 C2

		movq		mm4, mm2	; mm4 = .  .  C  -S
		psrlw		mm2, 1		; mm2 = .  .  C2 -S2

		punpckldq	mm1, mm3	; mm1 = S  C  S2 C2

		pmaddwd		mm1, mm6	; mm1 =  X'h   X'l	(>>2  <<13)
		punpckldq	mm2, mm4	; mm2 = C  -S C2 -S2

		pmaddwd		mm6, mm2	; mm6 =  Z'h   Z'l	(>>2  <<13)
		movq		mm3, mm1

		punpckldq	mm1, mm6	; mm1 =  Z'l   X'l	(<<13)

		paddd		mm1, [MMX_DEMI2]
		punpckhdq	mm3, mm6	; mm3 =  Z'h   X'h	(>>2)

		psrad		mm1, 13		; mm1 =  Z'l   X'l

		pslld		mm3, 2		; mm3 =  Z'h   X'h

		paddd		mm1, mm3	; mm1 =  Z'    X'

		movq		mm2, mm1	; mm2 =  Z'    X'
		psrlq		mm1, 32		; mm1 =  0     Z'

		movdf		[X0], mm2	; X'

		movdf		[Z0], mm1	; Z'

		ret
norot:
		mov		[X0], eax
		mov		[Z0], ecx

		ret

LongRotateM	endp

	@


LongRotateM	proc

		and		edx, 4095
		jz		norot

		add		edx, edx
		pxor		mm2, mm2	; mm2 = 0  0  0  0

		movdt		mm6, eax	; mm6 = 0  0  Xh Xl

		movdt		mm7, ecx	; mm7 = 0  0  Zh Zl

		movdt		mm0, SinTab[edx]; mm0 = 0  0  .  S	Penalty of 3 cycles with 50% chances
		punpcklwd	mm6, mm7	; mm6 = Zh Xh Zl Xl

		movq		mm7, mm6	; mm7 = Zh Xh Zl Xl
		psrlw		mm6, 1		; mm6 = .  .  Zl Xl	(>>1)

		movdt		mm1, CosTab[edx]; mm1 = 0  0  .  C	If penalty above then another 3 cyles penalty
		psrlq		mm7, 32		; mm7 = 0  0  Zh Xh

		punpckldq	mm6, mm7	; mm6 = Zh Xh Zl Xl	(>>0  >>1)
		psubw		mm2, mm0	; mm2 = 0  0  .  -S

		punpcklwd	mm2, mm1	; mm2 = .  .  C  -S

		punpcklwd	mm1, mm0	; mm1 = .  .  S  C

		punpckldq	mm1, mm1	; mm1 = S  C  S  C

		pmaddwd		mm1, mm6	; mm1 =  X'h   X'l	(>>2  <<13)
		punpckldq	mm2, mm2	; mm2 = C  -S C  -S

		pmaddwd		mm6, mm2	; mm6 =  Z'h   Z'l	(>>2  <<13)
		movq		mm3, mm1

		punpckldq	mm1, mm6	; mm1 =  Z'l   X'l	(<<13)

		paddd		mm1, [MMX_DEMI2]
		punpckhdq	mm3, mm6	; mm3 =  Z'h   X'h	(>>2)

		psrad		mm1, 13		; mm1 =  Z'l   X'l

		pslld		mm3, 2		; mm3 =  Z'h   X'h

		paddd		mm1, mm3	; mm1 =  Z'    X'

		movq		mm2, mm1	; mm2 =  Z'    X'
		psrlq		mm1, 32		; mm1 =  0     Z'

		movdf		[X0], mm2	; X'

		movdf		[Z0], mm1	; Z'

		ret
norot:
		mov		[X0], eax
		mov		[Z0], ecx

		ret

LongRotateM	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
