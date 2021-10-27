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

		public	SYSCALL	RotTransList

RotTransList	dd	offset RotTransListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	RotTransListI:PROC
		extrn	SYSCALL	RotTransListF:PROC
		extrn	SYSCALL	RotTransListM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
