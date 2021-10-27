;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc
		include iammx.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL MatriceWorld:DWORD

		extrn	SYSCALL CameraXr:DWORD
		extrn	SYSCALL CameraYr:DWORD
		extrn	SYSCALL CameraZr:DWORD

		extrn	SYSCALL CameraZrClip:DWORD

		extrn	SYSCALL LFactorX:DWORD
		extrn	SYSCALL LFactorY:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL	Xp:DWORD
		extrn	SYSCALL	Yp:DWORD

		extrn SYSCALL	MMX_DEMI2:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	LongRotateProjectPoint3DM
		public SYSCALL	LongWorldRotateProjectPoint3DM

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongWorldRotateProjectPoint3DM(S32 x, S32 y, S32 z)	;

;#pragma aux LongWorldRotateProjectPoint3DM	\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

LongWorldRotateProjectPoint3DM proc

		mov	esi, offset MatriceWorld

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongRotateProjectPoint3DM(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotateProjectPoint3DM		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongRotateProjectPoint3DM::


		movdt		mm5, ecx	; mm5 = 0  0  Zh Zl

		movdt		mm6, eax	; mm6 = 0  0  Xh Xl
		movq		mm4, mm5	; mm4 = 0  0  Zh Zl

		movdt		mm7, ebx	; mm7 = 0  0  Yh Yl
		psllq		mm5, 16		; mm5 = 0  Zh Zl 0

		movq		mm0, [esi]	; mm0 = 0  3  2  1		1
		punpcklwd	mm6, mm7	; mm6 = Yh Xh Yl Xl

		movq		mm7, mm6	; mm7 = Yh Xh Yl Xl
		punpckldq	mm6, mm4	; mm6 = .  Zl Yl Xl

		movq		mm1, mm0	; mm1 = 0  3  2  1		1
		punpckhdq	mm7, mm5	; mm7 = 0  Zh Yh Xh

		movq		mm2, [esi+8]	; mm2 = 0  3  2  1		2
		psrlw		mm6, 1		; mm6 = .  Zl Yl Xl  ( >>1  )

		movq		mm3, mm2	; mm3 = 0  3  2  1		2
		pmaddwd		mm0, mm6	; mm0 = Xl'Z  Xl'XY  ( <<13 )	1

		pmaddwd		mm1, mm7	; mm1 = Xh'Z  Xh'XY  ( >>2  )	1

		pmaddwd		mm2, mm6	; mm2 = Yl'Z  Yl'XY  ( <<13 )	2

		paddd		mm0, [MMX_DEMI2]
		pmaddwd		mm3, mm7	; mm3 = Yh'Z  Yh'XY  ( >>2  )	2

		pmaddwd		mm7, [esi+16]	; mm7 = Zh'Z  Zh'XY  ( >>2  )	3
		psrad		mm0, 13		; mm0 = Xl'Z  Xl'XY		1

		paddd		mm2, [MMX_DEMI2]
		pslld		mm1, 2		; mm1 = Xh'Z  Xh'XY		1

		psrad		mm2, 13		; mm2 = Yl'Z  Yl'XY		2
		paddd		mm0, mm1	; mm0 =  X'Z   X'XY		1

		pmaddwd		mm6, [esi+16]	; mm6 = Zl'Z  Zl'XY  ( <<13 )	3
		pslld		mm3, 2		; mm3 = Yh'Z  Yh'XY		2

		paddd		mm2, mm3	; mm2 =  Y'Z   Y'XY		2
		movq		mm3, mm0	; mm3 =  X'Z   X'XY

		punpckldq	mm0, mm2	; mm0 =  Y'XY  X'XY

		paddd		mm6, [MMX_DEMI2]
		punpckhdq	mm3, mm2	; mm3 =  Y'Z   X'Z

		paddd		mm0, mm3	; mm0 =  Y'    X'
		pslld		mm7, 2		; mm7 = Zh'Z  Zh'XY		3

		movdf		eax, mm0
		psrad		mm6, 13		; mm6 = Zl'Z  Zl'XY		3

		psrlq		mm0, 32
		paddd		mm6, mm7	; mm6 =  Z'Z   Z'XY		3

		movq		mm7, mm6	; mm7 =  Z'Z   Z'XY		3
		psrlq		mm6, 32		; mm6 =   0    Z'Z		3

		movdf		ebx, mm0
		paddd		mm7, mm6	; mm7 =   .    Z'		3

		movdf		ecx, mm7

		cmp		ecx, [CameraZrClip]; -Z
		jg		error

		sub		eax, [CameraXr]
		sub		ebx, [CameraYr]

		neg		eax
		sub		ecx, [CameraZr]

		imul		[LFactorX]	; -X
		idiv		ecx		; / -Z
		add		eax, [XCentre]

		mov		[Xp], eax	; result

		mov		eax, ebx
		imul		[LFactorY]      ; Y
		idiv		ecx		; / -Z
		add		eax, [YCentre]

		mov		[Yp], eax	; result

		mov		eax, 1
		ret

error:          mov		ebx, 80000000h
		xor		eax, eax

		mov		[Xp], ebx
		mov		[Yp], ebx

		ret

LongWorldRotateProjectPoint3DM	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
