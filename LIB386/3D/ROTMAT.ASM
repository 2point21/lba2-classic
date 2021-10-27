;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	RotateMatrix

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

RotateMatrix	dd	offset RotateMatrixInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	RotateMatrixI:PROC
		extrn	SYSCALL	RotateMatrixU:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	RotateMatrixInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateMatrix(S32 *MatDst, S32 *MatSrc, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux RotateMatrix				\
;	parm		[edi] [esi] [eax] [ebx] [ecx]	\
;	modify		[edx]

RotateMatrixInit proc

		mov	[Chooser_RtnI], offset RotateMatrixI	; Integer
		mov	[Chooser_RtnF], offset RotateMatrixU	; FPU
		mov	[Chooser_RtnM], offset RotateMatrixU	; MMX
		mov	[Chooser_Result], offset RotateMatrix	; Pointer
		jmp	Chooser

RotateMatrixInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
