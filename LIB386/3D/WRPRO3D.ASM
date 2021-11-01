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
		public	SYSCALL WorldRotateProjectPoint3D

WorldRotateProjectPoint3D	dd	offset	WorldRotateProjectPoint3DInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	WorldRotateProjectPoint3DI:PROC
		extrn	SYSCALL	LongWorldRotateProjectPoint3DF:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	WorldRotateProjectPoint3D(S32 x, S32 y, S32 z)	;

;#pragma aux RotatePoint			\
;	parm caller	[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

WorldRotateProjectPoint3DInit proc

		mov	[Chooser_RtnI], offset WorldRotateProjectPoint3DI	; Integer
		mov	[Chooser_RtnF], offset LongWorldRotateProjectPoint3DF	; FPU
		mov	[Chooser_Result], offset WorldRotateProjectPoint3D	; Pointer
		jmp	Chooser

WorldRotateProjectPoint3DInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

