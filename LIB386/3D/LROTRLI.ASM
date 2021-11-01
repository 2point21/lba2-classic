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

		public	SYSCALL	LongRotTransList

LongRotTransList	dd	offset LongRotTransListInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongRotTransListI:PROC
		extrn	SYSCALL	LongRotTransListF:PROC
		extrn	SYSCALL	LongRotTransListM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotTransList(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LongRotTransList	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax edx]

LongRotTransListInit proc

		mov	[Chooser_RtnI], offset LongRotTransListI	; Integer
		mov	[Chooser_RtnF], offset LongRotTransListF	; FPU
		mov	[Chooser_RtnM], offset LongRotTransListM	; MMX
		mov	[Chooser_Result], offset LongRotTransList	; Pointer
		jmp	Chooser

LongRotTransListInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
