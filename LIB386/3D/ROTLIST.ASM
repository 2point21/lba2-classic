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

		public	SYSCALL	RotateList

RotateList	dd	offset RotateListInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	RotateListI:PROC
		extrn	SYSCALL	RotateListF:PROC
		extrn	SYSCALL	RotateListM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
