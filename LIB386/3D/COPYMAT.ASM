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

		public	SYSCALL	CopyMatrix

CopyMatrix	dd	offset CopyMatrixInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	CopyMatrixI:PROC
		extrn	SYSCALL	CopyMatrixF:PROC
		extrn	SYSCALL	CopyMatrixM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
