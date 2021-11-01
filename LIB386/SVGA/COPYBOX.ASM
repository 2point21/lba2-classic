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
		public	SYSCALL	CopyBox

CopyBox		dd	offset CopyBoxInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	CopyBoxI:PROC
		extrn	SYSCALL	CopyBoxF:PROC
		extrn	SYSCALL	CopyBoxM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	CopyBox(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBox	       	       	\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxInit	proc

		mov	[Chooser_RtnI], offset CopyBoxI	; Integer
		mov	[Chooser_RtnF], offset CopyBoxF	; FPU
		mov	[Chooser_RtnM], offset CopyBoxM	; MMX
		mov	[Chooser_Result], offset CopyBox; Pointer
		jmp	Chooser

CopyBoxInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

