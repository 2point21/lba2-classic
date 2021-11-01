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

		public	SYSCALL LongProjectPoint3D

LongProjectPoint3D	dd	offset	LongProjectPoint3DInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	LongProjectPoint3DI:PROC
		extrn	SYSCALL	LongProjectPoint3DF:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

