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

		public	SYSCALL	RotatePoint
		public	SYSCALL	RotatePointNoMMX

RotatePoint	dd	offset RotatePointInit
RotatePointNoMMX dd	offset RotatePointInitNoMMX
; Useful for ObjectDisplay, bcoz within Object/BodyDisplay, we use the FPU,
; either for triangles and for normal transforms (environment mapping). And, if
; we would use the MMX for normals transforms, then we should run an EMMS, which can
; be worse than FPU transform...

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	RotatePointI:PROC
		extrn	SYSCALL	LongRotatePointF:PROC
		extrn	SYSCALL	RotatePointM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
