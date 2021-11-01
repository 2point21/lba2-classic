;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	InitMatrixStd:DWORD
		extrn	SYSCALL	CopyMatrix:DWORD
		extrn	SYSCALL	MulMatrix:DWORD

		extrn	SYSCALL	MatrixLib2:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	RotateMatrixU

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateMatrixU(S32 *MatDst, S32 *MatSrc, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux RotateMatrixU				\
;	parm		[edi] [esi] [eax] [ebx] [ecx]
;	modify exact	[eax ebx ecx edx esi]

RotateMatrixU	proc

		push	edi
		push	esi

		mov	edi, offset MatrixLib2	; because InitMatrixStd use MatrixLib1
		call	[InitMatrixStd]		; MatrixLib2 = Alpha * Gamma * Beta

		mov	ebx, edi
		pop	esi
		pop	edi

		jmp	[MulMatrix]		; MatDst = MatSrc * Alpha * Gamma * Beta

RotateMatrixU	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
