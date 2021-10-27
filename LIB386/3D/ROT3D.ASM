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

		public	SYSCALL	RotatePoint
		public	SYSCALL	RotatePointNoMMX

RotatePoint	dd	offset RotatePointInit
RotatePointNoMMX dd	offset RotatePointInitNoMMX
; Useful for ObjectDisplay, bcoz within Object/BodyDisplay, we use the FPU,
; either for triangles and for normal transforms (environment mapping). And, if
; we would use the MMX for normals transforms, then we should run an EMMS, which can
; be worse than FPU transform...

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	RotatePointI:PROC
		extrn	SYSCALL	LongRotatePointF:PROC
		extrn	SYSCALL	RotatePointM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotatePoint(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux RotatePoint			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotatePointInit proc

		mov	[Chooser_RtnI], offset RotatePointI	; Integer
		mov	[Chooser_RtnF], offset LongRotatePointF	; FPU
		mov	[Chooser_RtnM], offset RotatePointM	; MMX
		mov	[Chooser_Result], offset RotatePoint	; Pointer
		jmp	Chooser

RotatePointInit endp


RotatePointInitNoMMX proc

		mov	[Chooser_RtnI], offset RotatePointI	; Integer
		mov	[Chooser_RtnF], offset LongRotatePointF	; FPU
		mov	[Chooser_Result], offset RotatePointNoMMX; Pointer
		jmp	Chooser

RotatePointInitNoMMX endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
