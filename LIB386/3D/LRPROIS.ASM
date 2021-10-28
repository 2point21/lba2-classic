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

		public	SYSCALL LongRotateProjectPointIso

LongRotateProjectPointIso	dd	offset	LongRotateProjectPointIsoInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongRotateProjectPointIsoI:PROC
		extrn	SYSCALL	LongRotateProjectPointIsoF:PROC
		extrn	SYSCALL	LongRotateProjectPointIsoM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotateProjectPointIso(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux 	LongRotateProjectPointIso	\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongRotateProjectPointIsoInit proc

		mov	[Chooser_RtnI], offset LongRotateProjectPointIsoI	; Integer
		mov	[Chooser_RtnF], offset LongRotateProjectPointIsoF	; FPU
		mov	[Chooser_RtnM], offset LongRotateProjectPointIsoM	; MMX
		mov	[Chooser_Result], offset LongRotateProjectPointIso	; Pointer
		jmp	Chooser

LongRotateProjectPointIsoInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

