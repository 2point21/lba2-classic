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

		public	SYSCALL LongProjectPoint3D

LongProjectPoint3D	dd	offset	LongProjectPoint3DInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongProjectPoint3DI:PROC
		extrn	SYSCALL	LongProjectPoint3DF:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongProjectPoint3D(S32 x, S32 y, S32 z)		;

;#pragma aux LongProjectPoint3D		\
;	parm		[eax] [ebx] [ecx]	\
;	modify		[edx]

LongProjectPoint3DInit proc

		mov	[Chooser_RtnI], offset LongProjectPoint3DI	; Integer
		mov	[Chooser_RtnF], offset LongProjectPoint3DF	; FPU
		mov	[Chooser_RtnM], offset LongProjectPoint3DI	; MMX = Integer
		mov	[Chooser_Result], offset LongProjectPoint3D	; Pointer
		jmp	Chooser

LongProjectPoint3DInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

