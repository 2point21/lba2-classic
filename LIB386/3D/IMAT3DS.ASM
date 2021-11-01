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

		public	SYSCALL	InitMatrix3DS

InitMatrix3DS	dd	offset InitMatrix3DSInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	InitMatrix3DSI:PROC
		extrn	SYSCALL	InitMatrix3DSF:PROC
		extrn	SYSCALL	InitMatrix3DSM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrix3DS(S32 *MatDst, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux InitMatrix3DS	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx esi]

InitMatrix3DSInit proc

		mov	[Chooser_RtnI], offset InitMatrix3DSI	; Integer
		mov	[Chooser_RtnF], offset InitMatrix3DSF	; FPU
		mov	[Chooser_RtnM], offset InitMatrix3DSM	; MMX
		mov	[Chooser_Result], offset InitMatrix3DS	; Pointer
		jmp	Chooser

InitMatrix3DSInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
