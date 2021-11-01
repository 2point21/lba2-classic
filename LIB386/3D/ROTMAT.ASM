;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	RotateMatrix

;*══════════════════════════════════════════════════════════════════════════*

RotateMatrix	dd	offset RotateMatrixInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	RotateMatrixI:PROC
		extrn	SYSCALL	RotateMatrixU:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	RotateMatrixInit

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
