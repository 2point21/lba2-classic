;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc
		include iammx.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	MatriceWorld:DWORD

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

		extrn	SYSCALL	MMX_DEMI:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	RotateProjectPoint3DM
		public SYSCALL	WorldRotateProjectPoint3DM

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	WorldRotateProjectPoint3DM(S32 x, S32 y, S32 z)	;

;#pragma aux WorldRotateProjectPoint3DM		\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

WorldRotateProjectPoint3DM	proc

		mov	esi, offset MatriceWorld

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateProjectPoint3DM(S32 *Mat, S32 x, S32 y, S32 z);

;#pragma aux RotateProjectPoint3DM		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotateProjectPoint3DM::

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
		punpckhdq	mm1, mm6	; mm5 =  Y'Z   X'Z

		paddd		mm5, mm1	; mm5 =  Y'    X'	(<<14)
		psrlq		mm0, 32		; mm0 =  0     Z'Z

		paddd		mm5, [MMX_DEMI]
		paddd		mm2, mm0	; mm1 =  .     Z'	(<<14)

		paddd		mm2, [MMX_DEMI]
		psrad		mm5, 14		; mm5 =  Y'    X'

		psrad		mm2, 14		; mm1 =  .     Z'

		movdf		eax, mm5
		psrlq		mm5, 32

		movdf		ecx, mm2

		movdf		ebx, mm5

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

WorldRotateProjectPoint3DM	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End
