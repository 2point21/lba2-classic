;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL


;*══════════════════════════════════════════════════════════════════════════*
		.DATA

		extrn	SYSCALL MatrixLib2:DWORD
		extrn	SYSCALL InitMatrixStd:DWORD
		extrn	SYSCALL LongRotatePoint:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	RotateVector

;*══════════════════════════════════════════════════════════════════════════*

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

