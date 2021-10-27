;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL


;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

		extrn	SYSCALL MatrixLib2:DWORD
		extrn	SYSCALL InitMatrixStd:DWORD
		extrn	SYSCALL LongRotatePoint:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	RotateVector

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void RotateVector(S32 norme, S32 alpha, S32 beta, S32 gamma)	;

;#pragma aux RotateVector			\
;	parm		[edx] [eax] [ebx] [ecx]	\
;	modify		[esi edi]

RotateVector	proc

		mov	edi, offset MatrixLib2
		push	edx

		call	[InitMatrixStd]

		pop	ecx
		xor	eax, eax
		xor	ebx, ebx
		mov	esi, offset MatrixLib2

		jmp	[LongRotatePoint]

RotateVector	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

