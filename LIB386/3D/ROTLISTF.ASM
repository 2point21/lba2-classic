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

		public SYSCALL	RotateListF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateListF(MTYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux RotateListF			\
;	parm 		[ebx] [edi] [esi] [ecx]	\
;	modify exact	[ecx esi edi]

RotateListF	proc
		lea	esi, [esi+ecx*8]
		lea	ecx, [ecx*8]

		add	edi, ecx
		xor	ecx, -1

		inc	ecx
		jne	EnterLoop
		ret
					;  0   1   2   3   4   5   6   7
NextPoint:
		faddp	st(3), st(0)	; 1  Z3 2  P3

		fistp	word ptr[edi+ecx-8]; Z3 2  P3

		faddp	st(2), st(0)	; 2  3

		fistp	word ptr[edi+ecx-6]; 3

		fistp	word ptr[edi+ecx-4]
EnterLoop:
		fild	word ptr[esi+ecx];X

		fild	word ptr[esi+ecx+2];Y   X

		fild	word ptr[esi+ecx+4];Z   Y   X

		fld	st(2)		; X  Z  Y  X

		fmul	[ebx].MAT_M11	; X1 Z  Y  X

		fld	st(2)		; Y  X1 Z  Y  X

		fmul	[ebx].MAT_M12	; Y1 X1 Z  Y  X

		fld	st(2)		; Z  Y1 X1 Z  Y  X

		fmul	[ebx].MAT_M13	; Z1 Y1 X1 Z  Y  X

		fld	st(5)		; X  Z1 Y1 X1 Z  Y  X

		fmul	[ebx].MAT_M21   ; X2 Z1 Y1 X1 Z  Y  X

		fld	st(5)		; Y  X2 Z1 Y1 X1 Z  Y  X

		fmul	[ebx].MAT_M22   ; Y2 X2 Z1 Y1 X1 Z  Y  X
		fxch	st(3)		; Y1 X2 Z1 Y2 X1 Z  Y  X

		faddp	st(4), st(0)    ; X2 Z1 Y2 X1 Z  Y  X
		fxch	st(6)		; X  Z1 Y2 X1 Z  Y  X2

		fmul	[ebx].MAT_M31	; X3 Z1 Y2 X1 Z  Y  X2
		fxch	st(6)		; X2 Z1 Y2 P1 Z  Y  X3

		fld	st(4)		; Z  X2 Z1 Y2 P1 Z  Y  X3

		fmul	[ebx].MAT_M23	; Z2 X2 Z1 Y2 P1 Z  Y  X3
		fxch	st(1)		; X2 Z2 Z1 Y2 P1 Z  Y  X3

		faddp	st(3), st(0)	; Z2 Z1 P2 P1 Z  Y  X3
		fxch	st(5)		; Y  Z1 P2 P1 Z  Z2 X3

		fmul	[ebx].MAT_M32	; Y3 Z1 P2 P1 Z  Z2 X3
		fxch	st(1)		; Z1 Y3 P2 P1 Z  Z2 X3

		faddp	st(3), st(0)	; Y3 P2 1  Z  Z2 X3
		fxch	st(3)		; Z  P2 1  Y3 Z2 X3

		fmul	[ebx].MAT_M33	; Z3 P2 1  Y3 Z2 X3
		fxch	st(3)		; Y3 P2 1  Z3 Z2 X3

		faddp	st(5), st(0)	; P2 1  Z3 Z2 P3

		add	ecx, 8
		jnz	NextPoint

		faddp	st(3), st(0)	; 1  Z3 2  P3

		fistp	word ptr[edi+ecx-8]; Z3 2  P3

		faddp	st(2), st(0)	; 2  3

		fistp	word ptr[edi+ecx-6]; 3

		fistp	word ptr[edi+ecx-4]

		ret
RotateListF	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
