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

		public	SYSCALL	LongRotateList

LongRotateList	dd	offset LongRotateListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	LongRotateListI:PROC
		extrn	SYSCALL	LongRotateListF:PROC
		extrn	SYSCALL	LongRotateListM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
