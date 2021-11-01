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

		public	SYSCALL	Rotate

Rotate	dd	offset RotateInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	RotateI:PROC
		extrn	SYSCALL	LongRotateF:PROC
		extrn	SYSCALL	RotateM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	Rotate(S32 x, S32 z, S32 angle)	;

;#pragma	aux	Rotate			\
;	parm		[eax] [ecx] [edx]	\
;	modify		[ebx esi edi]

RotateInit	proc

		mov	[Chooser_RtnI], offset RotateI		; Integer
		mov	[Chooser_RtnF], offset LongRotateF	; FPU
		mov	[Chooser_RtnM], offset RotateM		; MMX
		mov	[Chooser_Result], offset Rotate		; Pointer
		jmp	Chooser

RotateInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
