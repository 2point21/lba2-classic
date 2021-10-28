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

		public	SYSCALL	RotTransList

RotTransList	dd	offset RotTransListInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	RotTransListI:PROC
		extrn	SYSCALL	RotTransListF:PROC
		extrn	SYSCALL	RotTransListM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	RotTransList(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux RotTransList	"*"	\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax edx]

RotTransListInit proc

		mov	[Chooser_RtnI], offset RotTransListI	; Integer
		mov	[Chooser_RtnF], offset RotTransListF	; FPU
		mov	[Chooser_RtnM], offset RotTransListM	; MMX
		mov	[Chooser_Result], offset RotTransList	; Pointer
		jmp	Chooser

RotTransListInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
