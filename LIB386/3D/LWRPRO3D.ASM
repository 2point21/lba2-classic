;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL LongWorldRotateProjectPoint3D

LongWorldRotateProjectPoint3D	dd	offset	LongWorldRotateProjectPoint3DInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	LongWorldRotateProjectPoint3DI:PROC
		extrn	SYSCALL	LongWorldRotateProjectPoint3DF:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongWorldRotateProjectPoint3D(S32 x, S32 y, S32 z)	;

;#pragma aux 	LongWorldRotateProjectPoint3D	\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

LongWorldRotateProjectPoint3DInit proc

		mov	[Chooser_RtnI], offset LongWorldRotateProjectPoint3DI	; Integer
		mov	[Chooser_RtnF], offset LongWorldRotateProjectPoint3DF	; FPU
		mov	[Chooser_Result], offset LongWorldRotateProjectPoint3D	; Pointer
		jmp	Chooser

LongWorldRotateProjectPoint3DInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

