;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL LongWorldRotateProjectPoint3D

LongWorldRotateProjectPoint3D	dd	offset	LongWorldRotateProjectPoint3DInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongWorldRotateProjectPoint3DI:PROC
		extrn	SYSCALL	LongWorldRotateProjectPoint3DF:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

