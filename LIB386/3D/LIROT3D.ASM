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

		public	SYSCALL	LongInverseRotatePoint

LongInverseRotatePoint	dd	offset LongInverseRotatePointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	LongInverseRotatePointI:PROC
		extrn	SYSCALL	LongInverseRotatePointF:PROC
		extrn	SYSCALL	LongInverseRotatePointM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LongInverseRotatePoint(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongInverseRotatePoint			\
;	parm caller	[esi] [eax] [ebx] [ecx]		\
;	modify exact	[eax ebx ecx edx edi]

LongInverseRotatePointInit proc

		mov	[Chooser_RtnI], offset LongInverseRotatePointI	; Integer
		mov	[Chooser_RtnF], offset LongInverseRotatePointF	; FPU
		mov	[Chooser_RtnM], offset LongInverseRotatePointM	; MMX
		mov	[Chooser_Result], offset LongInverseRotatePoint	; Pointer
		jmp	Chooser

LongInverseRotatePointInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
