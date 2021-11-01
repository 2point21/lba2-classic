;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	LongRotTransListF

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotTransListF(MTYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LongRotTransListF			\
;	parm 		[ebx] [edi] [esi] [ecx]	\
;	modify exact	[ecx esi edi]

LongRotTransListF proc

		lea	ecx, [ecx+ecx*2]

		test	ecx, ecx
		jz	Exit

		lea	esi, [esi+ecx*4]
		lea	ecx, [ecx*4]

		add	edi, ecx
		xor	ecx, -1

		inc	ecx
		jmp	EnterLoop
					;  0   1   2   3   4   5   6   7
NextPoint:
		faddp	st(4), st(0)	;  P3  1   Z3  2
		fxch	st(1)		;  1   P3  Z3  2

		fistp	dword ptr[edi+ecx-12]; P3  Z3  2

		faddp	st(1), st(0)	;  3   2
		fxch	st(1)		;  2   3

		fistp	dword ptr[edi+ecx-8];  3

		fistp	dword ptr[edi+ecx-4]
EnterLoop:
		fild	dword ptr[esi+ecx];  X

		fild	dword ptr[esi+ecx+4];  Y   X

		fild	dword ptr[esi+ecx+8];  Z   Y   X

		fld	st(2)		;  X   Z   Y   X

		fmul	[ebx].MAT_M11	;  X1  Z   Y   X

		fld	st(2)		;  Y   X1  Z   Y   X

		fmul	[ebx].MAT_M12	;  Y1  X1  Z   Y   X
		fxch	st(1)		;  X1  Y1  Z   Y   X

		fadd	[ebx].MAT_MTX	;  TX1 Y1  Z   Y   X
		fxch	st(1)		;  Y1  TX1 Z   Y   X

		fld	st(2)		;  Z   Y1  TX1 Z   Y   X

		fmul	[ebx].MAT_M13	;  Z1  Y1  TX1 Z   Y   X

		fld	st(6)		;  X   Z1  Y1  TX1 Z   Y   X

		fmul	[ebx].MAT_M21   ;  X2  Z1  Y1  TX1 Z   Y   X

		fld	st(6)		;  Y   X2  Z1  Y1  TX1 Z   Y   X

		fmul	[ebx].MAT_M22   ;  Y2  X2  Z1  Y1  TX1 Z   Y   X
		fxch	st(1)		;  X2  Y2  Z1  Y1  TX1 Z   Y   X

		fadd	[ebx].MAT_MTY	;  TX2 Y2  Z1  Y1  TX1 Z   Y   X
		fxch	st(3)		;  Y1  Y2  Z1  TX2 TX1 Z   Y   X

		faddp	st(4), st(0)    ;  Y2  Z1  TX2 P1  Z   Y   X
		fxch	st(6)		;  X   Z1  TX2 P1  Z   Y   X2

		fmul	[ebx].MAT_M31	;  X3  Z1  TX2 P1  Z   Y   X2
		fxch	st(6)		;  Y2  Z1  TX2 P1  Z   Y   X3

		fld	st(4)		;  Z   Y2  Z1  TX2 P1  Z   Y   X3

		fmul	[ebx].MAT_M23	;  Z2  Y2  Z1  TX2 P1  Z   Y   X3
		fxch	st(1)		;  Y2  Z2  Z1  TX2 P1  Z   Y   X3

		faddp	st(3), st(0)	;  Z2  Z1  P2  P1  Z   Y   X3
		fxch	st(6)		;  X3  Z1  P2  P1  Z   Y   Z2

		fadd	[ebx].MAT_MTZ	;  TX3 Z1  P2  P1  Z   Y   Z2
		fxch	st(5)		;  Y   Z1  P2  P1  Z   TX3 Z2

		fmul	[ebx].MAT_M32	;  Y3  Z1  P2  P1  Z   TX3 Z2
		fxch	st(3)		;  P1  Z1  P2  Y3  Z   TX3 Z2

		faddp	st(1), st(0)	;  1   P2  Y3  Z   TX3 Z2
		fxch	st(3)		;  Z   P2  Y3  1   TX3 Z2

		fmul	[ebx].MAT_M33	;  Z3  P2  Y3  1   TX3 Z2
		fxch	st(5)		;  TX3 P2  Y3  1   Z3  Z2

		faddp	st(2), st(0)	;  P2  P3  1   Z3  Z2

		add	ecx, 12
		jnz	NextPoint

		faddp	st(4), st(0)	;  P3  1   Z3  2
		fxch	st(1)		;  1   P3  Z3  2

		fistp	dword ptr[edi+ecx-12]; P3  Z3  2

		faddp	st(1), st(0)	;  3   2
		fxch	st(1)		;  2   3

		fistp	dword ptr[edi+ecx-8]

		fistp	dword ptr[edi+ecx-4]
Exit:
		ret

LongRotTransListF endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
