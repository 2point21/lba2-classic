;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	LongProjectPoint3DI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongProjectPoint3DI(S32 x, S32 y, S32 z)		;

;#pragma aux LongProjectPoint3DI		\
;	parm		[eax] [ebx] [ecx]	\
;	modify		[edx]

LongProjectPoint3DI proc

		cmp	ecx, [CameraZrClip]	; -Z
		jg	error

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

LongProjectPoint3DI endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END


