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

		public	SYSCALL	CopyMatrix

CopyMatrix	dd	offset CopyMatrixInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	CopyMatrixI:PROC
		extrn	SYSCALL	CopyMatrixF:PROC
		extrn	SYSCALL	CopyMatrixM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	CopyMatrix(S32 *MatDst, S32 *MatSrc) ;

;#pragma aux CopyMatrix	"*"		\
;	parm		[edi] [esi]	\
;	modify exact	[eax ebx ecx]

CopyMatrixInit proc

		mov	[Chooser_RtnI], offset CopyMatrixI	; Integer
		mov	[Chooser_RtnF], offset CopyMatrixF	; FPU
		mov	[Chooser_RtnM], offset CopyMatrixM	; MMX
		mov	[Chooser_Result], offset CopyMatrix	; Pointer
		jmp	Chooser

CopyMatrixInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
