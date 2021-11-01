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

		public	SYSCALL	LongRotateList

LongRotateList	dd	offset LongRotateListInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongRotateListI:PROC
		extrn	SYSCALL	LongRotateListF:PROC
		extrn	SYSCALL	LongRotateListM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotateList(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LongRotateList	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax edx]

LongRotateListInit proc

		mov	[Chooser_RtnI], offset LongRotateListI	; Integer
		mov	[Chooser_RtnF], offset LongRotateListF	; FPU
		mov	[Chooser_RtnM], offset LongRotateListM	; MMX
		mov	[Chooser_Result], offset LongRotateList	; Pointer
		jmp	Chooser

LongRotateListInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
