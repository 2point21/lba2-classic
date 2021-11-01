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
		public	SYSCALL	CopyBox

CopyBox		dd	offset CopyBoxInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	CopyBoxI:PROC
		extrn	SYSCALL	CopyBoxF:PROC
		extrn	SYSCALL	CopyBoxM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

