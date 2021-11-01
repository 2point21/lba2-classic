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

		public	SYSCALL	InitMatrixStd

InitMatrixStd	dd	offset InitMatrixStdInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	InitMatrixStdI:PROC
		extrn	SYSCALL	InitMatrixStdF:PROC
		extrn	SYSCALL	InitMatrixStdM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrixStd(S32 *MatDst, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux InitMatrixStd	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx esi]

InitMatrixStdInit proc

		mov	[Chooser_RtnI], offset InitMatrixStdI	; Integer
		mov	[Chooser_RtnF], offset InitMatrixStdF	; FPU
		mov	[Chooser_RtnM], offset InitMatrixStdM	; MMX
		mov	[Chooser_Result], offset InitMatrixStd	; Pointer
		jmp	Chooser

InitMatrixStdInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
