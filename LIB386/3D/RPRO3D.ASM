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

		public	SYSCALL RotateProjectPoint3D

RotateProjectPoint3D	dd	offset	RotateProjectPoint3DInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	RotateProjectPoint3DI:PROC
		extrn	SYSCALL	LongRotateProjectPoint3DF:PROC
		extrn	SYSCALL	RotateProjectPoint3DM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateProjectPoint3D(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux RotatePoint			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotateProjectPoint3DInit proc

		mov	[Chooser_RtnI], offset RotateProjectPoint3DI	; Integer
		mov	[Chooser_RtnF], offset LongRotateProjectPoint3DF; FPU
		mov	[Chooser_RtnM], offset RotateProjectPoint3DM	; MMX
		mov	[Chooser_Result], offset RotateProjectPoint3D	; Pointer
		jmp	Chooser

RotateProjectPoint3DInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

