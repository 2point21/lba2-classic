;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	LongRotateProjectPoint3DI
		public SYSCALL	LongWorldRotateProjectPoint3DI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongWorldRotateProjectPoint3DI(S32 x, S32 y, S32 z)	;

;#pragma aux LongWorldRotateProjectPoint3DI	\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

LongWorldRotateProjectPoint3DI proc

		mov	esi, offset MatriceWorld

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongRotateProjectPoint3DI(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotateProjectPoint3DI		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongRotateProjectPoint3DI::

		push	ebp
		mov	[Yp], eax

		imul	[esi].MAT_M11

		mov     ebp, eax
 		mov	eax, [esi].MAT_M12
		mov     edi, edx

		imul	ebx

		add     ebp, eax
		mov	eax, [esi].MAT_M13
		adc     edi, edx

		imul	ecx

		add     ebp, eax
		mov     eax, [esi].MAT_M21
		adc     edi, edx

		imul    [Yp]
		shrd    ebp, edi, 14

		adc	ebp, 0
		mov     edi, edx
		mov	[Xp], ebp
		mov     ebp, eax
		mov	eax, [esi].MAT_M22

		imul	ebx

		add     ebp, eax
		mov	eax, [esi].MAT_M23
		adc     edi, edx

		imul	ecx

		add     ebp, eax
		mov     eax, [esi].MAT_M31
		adc     edi, edx

		imul    [Yp]
		shrd    ebp, edi, 14

		adc	ebp, 0
		mov     edi, edx
		mov	[Yp], ebp
		mov     ebp, eax
		mov	eax, [esi].MAT_M32

		imul	ebx

		add     ebp, eax
		mov	eax, [esi].MAT_M33
		adc     edi, edx

		imul	ecx

		add     eax, ebp
		pop	ebp
		adc     edx, edi

		shrd    eax, edx, 14

		adc	eax, 0

		cmp	eax, [CameraZrClip]	; -Z
		jg	error

		mov	ecx, eax

		mov	eax, [Xp]
		mov	ebx, [Yp]

		sub	eax, [CameraXr]
		sub	ebx, [CameraYr]

		neg	eax
		sub	ecx, [CameraZr]

		imul	[LFactorX]		; -X
		idiv	ecx			; / -Z
		add	eax, [XCentre]

		mov	[Xp], eax		; result

		mov	eax, ebx
		imul	[LFactorY]              ; Y
		idiv	ecx			; / -Z
		add	eax, [YCentre]

		mov	[Yp], eax		; result

		mov	eax, 1
		ret

error:          mov	ebx, 80000000h
		xor	eax, eax

		mov	[Xp], ebx
		mov	[Yp], ebx

		ret

LongWorldRotateProjectPoint3DI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
