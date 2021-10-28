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
		public	SYSCALL	FastSet

FastSet	dd	offset FastSetInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	FastSetI:PROC
		extrn	SYSCALL	FastSetF:PROC
		extrn	SYSCALL	FastSetM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	FastSet(void *dst, S32 val, U32 len) ;

;#pragma aux FastSet	       	       	\
;	parm	[edi] [eax] [ecx]	\
;	modify	[ebx edx esi]

FastSetInit	proc

		mov	[Chooser_RtnI], offset FastSetI	; Integer
		mov	[Chooser_RtnF], offset FastSetF	; FPU
		mov	[Chooser_RtnM], offset FastSetM	; MMX
		mov	[Chooser_Result], offset FastSet; Pointer
		jmp	Chooser

FastSetInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
