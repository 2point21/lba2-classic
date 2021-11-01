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
		public	SYSCALL WorldRotateProjectPoint3D

WorldRotateProjectPoint3D	dd	offset	WorldRotateProjectPoint3DInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	WorldRotateProjectPoint3DI:PROC
		extrn	SYSCALL	LongWorldRotateProjectPoint3DF:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

