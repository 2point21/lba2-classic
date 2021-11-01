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
		public	SYSCALL	FastCopy

FastCopy	dd	offset FastCopyInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	FastCopyI:PROC
		extrn	SYSCALL	FastCopyF:PROC
		extrn	SYSCALL	FastCopyM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	FastCopy(void *dst, void *src, U32 len) ;

;#pragma aux FastCopy	       	       	\
;	parm	[edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

FastCopyInit	proc

		mov	[Chooser_RtnI], offset FastCopyI	; Integer
		mov	[Chooser_RtnF], offset FastCopyF	; FPU
		mov	[Chooser_RtnM], offset FastCopyM	; MMX
		mov	[Chooser_Result], offset FastCopy	; Pointer
		jmp	Chooser

FastCopyInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
