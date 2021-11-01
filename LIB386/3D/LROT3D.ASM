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

		public	SYSCALL	LongRotatePoint

LongRotatePoint	dd	offset LongRotatePointInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongRotatePointI:PROC
		extrn	SYSCALL	LongRotatePointF:PROC
		extrn	SYSCALL	LongRotatePointM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotatePoint(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotatePoint			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongRotatePointInit proc

		mov	[Chooser_RtnI], offset LongRotatePointI	; Integer
		mov	[Chooser_RtnF], offset LongRotatePointF	; FPU
		mov	[Chooser_RtnM], offset LongRotatePointM	; MMX
		mov	[Chooser_Result], offset LongRotatePoint; Pointer
		jmp	Chooser

LongRotatePointInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
