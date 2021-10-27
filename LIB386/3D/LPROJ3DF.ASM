;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include	mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL CameraX:DWORD
		extrn	SYSCALL CameraY:DWORD
		extrn	SYSCALL CameraZ:DWORD

		extrn	SYSCALL CameraXr:DWORD
		extrn	SYSCALL CameraYr:DWORD
		extrn	SYSCALL CameraZr:DWORD

		extrn	SYSCALL CameraZrClip:DWORD

		extrn	SYSCALL FRatioX:DWORD
		extrn	SYSCALL FRatioY:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL Xp:DWORD
		extrn	SYSCALL Yp:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	LongProjectPoint3DF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongProjectPoint3DF(S32 x, S32 y, S32 z)		;

;#pragma aux LongProjectPoint3DF		\
;	parm		[eax] [ebx] [ecx]	\
;	modify		[edx]

LongProjectPoint3DF proc

		cmp	ecx, [CameraZrClip]	; -Z
		jg	error

		mov	edx, [CameraZr]

		sub	edx, ecx

		push	 edx			; Z

		fild	dword ptr[esp]		; Z

		; PENALTY 2 cycle on Pentium

		fdivr	[FRatioX]		; fx/Z

		sub	eax, [CameraXr]
		sub	ebx, [CameraYr]

		mov	[esp], eax
		push	ebx

		mov	ecx, [XCentre]
		mov	edx, [YCentre]

		fild	dword ptr[esp]		; Y   fx/Z

		fild	dword ptr[esp+4]	; X   Y   fx/Z

		fxch	st(2)			; fx/Z Y    X

		fmul	st(1),st		; fx/Z Y'   X

		; PENALTY 1 cycle on Pentium

		fmulp	st(2),st		; Y'   Xe

		; PENALTY 1 cycle on Pentium

		fmul	[FRatioY]		; Ye   Xe
		fxch	st(1)			; Xe   Ye

		fistp	dword ptr[esp+4]	; Ye

		fistp	dword ptr[esp]

		pop	ebx
		pop	eax

		add	eax, ecx
		add	ebx, edx

		mov	[Xp], eax
		mov	[Yp], ebx

		mov	eax, 1

		ret

error:          mov	ebx, 80000000h
		xor	eax, eax

		mov	[Xp], ebx
		mov	[Yp], ebx

		ret

LongProjectPoint3DF endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
