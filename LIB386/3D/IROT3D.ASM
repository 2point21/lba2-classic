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

		public	SYSCALL	InverseRotatePoint

InverseRotatePoint	dd	offset InverseRotatePointInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	InverseRotatePointI:PROC
		extrn	SYSCALL	LongInverseRotatePointF:PROC
		extrn	SYSCALL	InverseRotatePointM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	InverseRotatePoint(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux InverseRotatePoint			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

InverseRotatePointInit proc

		mov	[Chooser_RtnI], offset InverseRotatePointI	; Integer
		mov	[Chooser_RtnF], offset LongInverseRotatePointF	; FPU
		mov	[Chooser_RtnM], offset InverseRotatePointM	; MMX
		mov	[Chooser_Result], offset InverseRotatePoint	; Pointer
		jmp	Chooser

InverseRotatePointInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
