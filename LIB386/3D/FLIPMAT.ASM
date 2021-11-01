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

		public	SYSCALL	FlipMatrix

FlipMatrix	dd	offset FlipMatrixInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	FlipMatrixI:PROC
		extrn	SYSCALL	FlipMatrixM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	FlipMatrix(S32 *MatDst, S32 *MatSrc) ;

;#pragma aux FlipMatrix	"*"		\
;	parm		[edi] [esi]	\
;	modify exact	[eax ebx ecx]

FlipMatrixInit proc

		mov	[Chooser_RtnI], offset FlipMatrixI	; Integer or FPU
		mov	[Chooser_RtnM], offset FlipMatrixM	; MMX
		mov	[Chooser_Result], offset FlipMatrix	; Pointer
		jmp	Chooser

FlipMatrixInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
