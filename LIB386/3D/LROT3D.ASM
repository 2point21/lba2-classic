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

		public	SYSCALL	LongRotatePoint

LongRotatePoint	dd	offset LongRotatePointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	LongRotatePointI:PROC
		extrn	SYSCALL	LongRotatePointF:PROC
		extrn	SYSCALL	LongRotatePointM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
