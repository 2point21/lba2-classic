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

		public	SYSCALL LongRotateProjectPoint3D

LongRotateProjectPoint3D	dd	offset	LongRotateProjectPoint3DInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongRotateProjectPoint3DI:PROC
		extrn	SYSCALL	LongRotateProjectPoint3DF:PROC
		extrn	SYSCALL	LongRotateProjectPoint3DM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotateProjectPoint3D(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux 	LongRotateProjectPoint3D	\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongRotateProjectPoint3DInit proc

		mov	[Chooser_RtnI], offset LongRotateProjectPoint3DI	; Integer
		mov	[Chooser_RtnF], offset LongRotateProjectPoint3DF	; FPU
		mov	[Chooser_RtnM], offset LongRotateProjectPoint3DM	; MMX
		mov	[Chooser_Result], offset LongRotateProjectPoint3D	; Pointer
		jmp	Chooser

LongRotateProjectPoint3DInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

