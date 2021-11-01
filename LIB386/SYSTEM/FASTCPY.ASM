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
		public	SYSCALL	FastCopy

FastCopy	dd	offset FastCopyInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	FastCopyI:PROC
		extrn	SYSCALL	FastCopyF:PROC
		extrn	SYSCALL	FastCopyM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
