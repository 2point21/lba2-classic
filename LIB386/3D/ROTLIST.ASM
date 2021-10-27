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

		public	SYSCALL	RotateList

RotateList	dd	offset RotateListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	RotateListI:PROC
		extrn	SYSCALL	RotateListF:PROC
		extrn	SYSCALL	RotateListM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateList(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux RotateList	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax edx]

RotateListInit proc

		mov	[Chooser_RtnI], offset RotateListI	; Integer
		mov	[Chooser_RtnF], offset RotateListF	; FPU
		mov	[Chooser_RtnM], offset RotateListM	; MMX
		mov	[Chooser_Result], offset RotateList	; Pointer
		jmp	Chooser

RotateListInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
