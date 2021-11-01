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

		public	SYSCALL	LongInverseRotatePoint

LongInverseRotatePoint	dd	offset LongInverseRotatePointInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongInverseRotatePointI:PROC
		extrn	SYSCALL	LongInverseRotatePointF:PROC
		extrn	SYSCALL	LongInverseRotatePointM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
