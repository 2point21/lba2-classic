;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	LongRotatePointF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongRotatePointF(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotatePointF			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[]

LongRotatePointF proc

		mov	[X0], eax
		mov	[Y0], ebx

		mov	[Z0], ecx
					; 0  1  2  3  4  5  6  7

		fild	[X0]		; X

		fild	[Y0]		; Y  X

		fild	[Z0]		; Z  Y  X

		fld	st(2)		; X  Z  Y  X

		fmul	[esi].MAT_M11	; X1 Z  Y  X

		fld	st(2)		; Y  X1 Z  Y  X

		fmul	[esi].MAT_M12	; Y1 X1 Z  Y  X

		fld	st(2)		; Z  Y1 X1 Z  Y  X

		fmul	[esi].MAT_M13	; Z1 Y1 X1 Z  Y  X

		fld	st(5)		; X  Z1 Y1 X1 Z  Y  X

		fmul	[esi].MAT_M21   ; X2 Z1 Y1 X1 Z  Y  X

		fld	st(5)		; Y  X2 Z1 Y1 X1 Z  Y  X

		fmul	[esi].MAT_M22   ; Y2 X2 Z1 Y1 X1 Z  Y  X
		fxch	st(3)		; Y1 X2 Z1 Y2 X1 Z  Y  X

		faddp	st(4), st(0)    ; X2 Z1 Y2 P1 Z  Y  X
		fxch	st(6)		; X  Z1 Y2 P1 Z  Y  X2

		fmul	[esi].MAT_M31	; X3 Z1 Y2 P1 Z  Y  X2
		fxch	st(6)		; X2 Z1 Y2 P1 Z  Y  X3

		fld	st(4)		; Z  X2 Z1 Y2 P1 Z  Y  X3

		fmul	[esi].MAT_M23	; Z2 X2 Z1 Y2 P1 Z  Y  X3
		fxch	st(1)		; X2 Z2 Z1 Y2 P1 Z  Y  X3

		faddp	st(3), st(0)	; Z2 Z1 P2 P1 Z  Y  X3
		fxch	st(5)		; Y  Z1 P2 P1 Z  Z2 X3

		fmul	[esi].MAT_M32	; Y3 Z1 P2 P1 Z  Z2 X3
		fxch	st(1)		; Z1 Y3 P2 P1 Z  Z2 X3

		faddp	st(3), st(0)	; Y3 P2 1  Z  Z2 X3
		fxch	st(3)		; Z  P2 1  Y3 Z2 X3

		fmul	[esi].MAT_M33	; Z3 P2 1  Y3 Z2 X3
		fxch	st(3)		; Y3 P2 1  Z3 Z2 X3

		faddp	st(5), st(0)	; P2 1  Z3 Z2 P3

		faddp	st(3), st(0)	; 1  Z3 2  P3

; with translation

;		fadd	[esi].MAT_TX	; T1 Z3 2  P3
;		fxch	st(3)		; P3 Z3 2  T1

;		faddp	st(1), st(0)	; 3  2  T1
;		fxch	st(1)		; 2  3  T1

;		fadd	[esi].MAT_TY	; T2 3  T1
;		fxch	st(2)		; T1 3  T2

;		fistp	[X0]		; 3  T2

;		faddp	[esi].MAT_TZ	; T3 T2
;		fxch	st(1)		; T2 T3

; 		fistp	[Y0]		; T3

;		fistp	[Z0]

;		ret

; without translation

		fistp	[X0]		; Z3 2  P3

		faddp	st(2), st(0)	; 2  3

		fistp	[Y0]

		fistp	[Z0]

		ret

LongRotatePointF endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
