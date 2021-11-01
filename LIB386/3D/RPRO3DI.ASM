;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

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

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	RotateProjectPoint3DI
		public SYSCALL	WorldRotateProjectPoint3DI

;*══════════════════════════════════════════════════════════════════════════*
;void	WorldRotateProjectPoint3DI(S32 x, S32 y, S32 z)	;

;#pragma aux WorldRotateProjectPoint3DI		\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

WorldRotateProjectPoint3DI	proc

		mov	esi, offset MatriceWorld

;*══════════════════════════════════════════════════════════════════════════*
;void	RotateProjectPoint3DI(S32 *Mat, S32 x, S32 y, S32 z);

;#pragma aux RotateProjectPoint3DI		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotateProjectPoint3DI::

		push	ebp
		mov	edi, [esi].MAT_M11

		mov	ebp, [esi].MAT_M12

		imul	edi, eax
		imul	ebp, ebx

		mov	edx, [esi].MAT_M13
		add	edi, ebp

		imul	edx, ecx

		mov	ebp, [esi].MAT_M22
		lea	edx, [edx+edi+(1 SHL 13)]

		sar	edx, 14
		mov	edi, [esi].MAT_M21

		mov	[Xp], edx
		mov	edx, [esi].MAT_M23

		imul	ebp, ebx
		imul	edi, eax
		imul	edx, ecx
		imul	eax, [esi].MAT_M31
		imul	ebx, [esi].MAT_M32
		imul	ecx, [esi].MAT_M33

		lea	edx, [edx+ebp+(1 SHL 13)]
		lea	ebx, [ebx+eax+(1 SHL 13)]

		add	edx, edi
		add	ecx, ebx

		sar	edx, 14
		pop	ebp

		sar	ecx, 14

		cmp	ecx, [CameraZrClip]	; -Z
		jg	error

		mov	ebx, edx
		mov	eax, [Xp]

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

WorldRotateProjectPoint3DI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		End
