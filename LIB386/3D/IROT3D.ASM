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

		public	SYSCALL	InverseRotatePoint

InverseRotatePoint	dd	offset InverseRotatePointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	InverseRotatePointI:PROC
		extrn	SYSCALL	LongInverseRotatePointF:PROC
		extrn	SYSCALL	InverseRotatePointM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
