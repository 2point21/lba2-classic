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

		extrn	SYSCALL NearClip:DWORD

		extrn	SYSCALL FRatioX:DWORD
		extrn	SYSCALL FRatioY:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL Xp:DWORD
		extrn	SYSCALL Yp:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	LongWorldRotateProjectPoint3DF
		public SYSCALL	LongRotateProjectPoint3DF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongWorldRotateProjectPoint3DF(S32 x, S32 y, S32 z)	;

;#pragma aux LongWorldRotateProjectPoint3DF	\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[esi edx]

LongWorldRotateProjectPoint3DF proc

		mov	esi, offset MatriceWorld

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongRotateProjectPoint3DF(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotateProjectPoint3DF		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify		[edx]

LongRotateProjectPoint3DF::

		push	ecx
		mov	[Yp], ebx

		mov	[Xp], eax
					; 0  1  2  3  4  5  6  7

		fild	dword ptr[esp]  ; Z

		fild	[Yp]		; Y  Z

		fild	[Xp]		; X  Y  Z

		fld	st(2)		; Z  X  Y  Z

		fmul	[esi].MAT_M31	; Z1 X  Y  Z

		fld	st(2)		; Y  Z1 X  Y  Z

		fmul	[esi].MAT_M32	; Y1 Z1 X  Y  Z

		fld	st(2)		; X  Y1 Z1 X  Y  Z

		fmul	[esi].MAT_M33	; X1 Y1 Z1 X  Y  Z

		fld	st(5)		; Z  X1 Y1 Z1 X  Y  Z

		fmul	[esi].MAT_M21   ; Z2 X1 Y1 Z1 X  Y  Z

		fld	st(5)		; Y  Z2 X1 Y1 Z1 X  Y  Z

		fmul	[esi].MAT_M22   ; Y2 Z2 X1 Y1 Z1 X  Y  Z
		fxch	st(3)		; Y1 Z2 X1 Y2 Z1 X  Y  Z

		faddp	st(4), st(0)    ; Z2 X1 Y2 P1 X  Y  Z
		fxch	st(6)		; Z  X1 Y2 P1 X  Y  Z2

		fmul	[esi].MAT_M11	; Z3 X1 Y2 P1 X  Y  Z2
		fxch	st(6)		; Z2 X1 Y2 P1 X  Y  Z3

		fld	st(4)		; X  Z2 X1 Y2 P1 X  Y  Z3

		fmul	[esi].MAT_M23	; X2 Z2 X1 Y2 P1 X  Y  Z3
		fxch	st(1)		; Z2 X2 X1 Y2 P1 X  Y  Z3

		faddp	st(3), st(0)	; X2 X1 P2 P1 X  Y  Z3
		fxch	st(5)		; Y  X1 P2 P1 X  X2 Z3

		fmul	[esi].MAT_M12	; Y3 X1 P2 P1 X  X2 Z3

		fild	[CameraZrClip]	; ZC Y3 X1 P2 P1 X  X2 Z3

		fxch	st(2)		; X1 Y3 ZC P2 P1 X  X2 Z3

		faddp	st(4), st(0)	; Y3 ZC P2 Z' X  X2 Z3
		fxch	st(4)		; X  ZC P2 Z' Y3 X2 Z3

		fmul	[esi].MAT_M13	; X3 ZC P2 Z' Y3 X2 Z3
		fxch	st(4)		; Y3 ZC P2 Z' X3 X2 Z3

		faddp	st(6), st(0)	; ZC P2 Z' X3 X2 P3

		fsubrp	st(2), st(0)	; P2 Z" X3 X2 P3

		faddp	st(5), st(0)	; P2 Z" X3 X2 P3

		faddp	st(3), st(0)	; Z" X3 Y' P3

		fst	dword ptr[esp]	; Z" X3 Y' P3

		fxch	st(1)		; X3 Z" Y' P3

		faddp	st(3), st(0)	; Z" Y' X'

		pop	eax

		test	eax, eax
		jl	error

		fild	[NearClip]	; NC Z" Y' X'

		fild	[CameraYr]	; Yr NC Z" Y' X'

		fild	[CameraXr]	; Xr Yr NC Z" Y' X'

		fxch	st(2)		; NC Yr Xr Z" Y' X'

		faddp	st(3), st(0)	; Yr Xr Z  Y' X'

		fsubp	st(3), st(0)	; Xr Z  Y  X'

		fsubp	st(3), st(0)	; Z  Y  X

		fdivr	[FRatioX]	; fx/Z  Y  X

		mov	ecx, [XCentre]
		mov	edx, [YCentre]

		fmul	st(1), st(0)	; fx/Z Y'   X

		; PENALTY 1 cycle on Pentium

		fmulp	st(2), st(0)	; Y'   Xe

		; PENALTY 1 cycle on Pentium

		fmul	[FRatioY]	; Ye   Xe
		fxch	st(1)		; Xe   Ye

		fistp	[Xp]		; Ye

		fistp	[Yp]

		mov	eax, [Xp]
		mov	ebx, [Yp]

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

		fsubp	st(0), st(0)
		fsubp	st(0), st(0)
		fsubp	st(0), st(0)

		ret


LongWorldRotateProjectPoint3DF endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
