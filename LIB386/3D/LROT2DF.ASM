;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	SinTabF:DWORD
		extrn	SYSCALL	CosTabF:DWORD

		extrn	SYSCALL	X0:DWORD
		extrn	SYSCALL	Z0:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	LongRotateF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongRotateF(S32 x, S32 z, S32 angle)	;

;#pragma aux	LongRotateF			\
;	parm		[eax] [ecx] [edx]	\
;	modify exact	[edx]

LongRotateF	proc

		mov	[X0], eax
		mov	[Z0], ecx

		and	edx, 4095
		jz	norot			; opt edx=0

		fild	[X0]			;  X

		shl	edx, 2

		fild	[Z0]			;  Z   X

		fld	st(1)			;  X   Z   X

		fmul	CosTabF[edx]		;  XC  Z   X

		fld	st(1)			;  Z   XC  Z    X

		fmul	SinTabF[edx]		;  ZS  XC  Z    X
		fxch	st(3)			;  X   XC  Z    ZS

						;  NOP

		fmul	SinTabF[edx]		;  XS  XC  Z    ZS
		fxch	st(1)			;  XC  XS  Z    ZS

		faddp	st(3), st(0)		;  XS  Z   X'
		fxch	st(1)			;  Z   XS  X'

		fmul	CosTabF[edx]		;  ZC  XS  X'
		fxch	st(1)			;  XS  ZC  X'

						; NOP

						; NOP

		fsubp	st(1), st(0)		;  Y'  X'
		fxch	st(1)			;  X'  Y'

		fistp	[X0]			;  Y'

		fistp	[Z0]
norot:
		ret

LongRotateF	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
