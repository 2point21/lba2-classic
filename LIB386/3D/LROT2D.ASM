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

		public	SYSCALL	LongRotate

LongRotate	dd	offset LongRotateInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongRotateI:PROC
		extrn	SYSCALL	LongRotateF:PROC
		extrn	SYSCALL	LongRotateM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotate(S32 x, S32 z, S32 angle)	;

;#pragma	aux	LongRotate		\
;	parm		[eax] [ecx] [edx]	\
;	modify		[ebx esi edi]

LongRotateInit	proc

		mov	[Chooser_RtnI], offset LongRotateI	; Integer
		mov	[Chooser_RtnF], offset LongRotateF	; FPU
		mov	[Chooser_RtnM], offset LongRotateM	; MMX
		mov	[Chooser_Result], offset LongRotate	; Pointer
		jmp	Chooser

LongRotateInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
