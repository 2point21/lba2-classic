;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL RotateProjectPointIso

RotateProjectPointIso	dd	offset	RotateProjectPointIsoInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	RotateProjectPointIsoI:PROC
		extrn	SYSCALL	LongRotateProjectPointIsoF:PROC
		extrn	SYSCALL	RotateProjectPointIsoM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateProjectPointIso(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux 	RotateProjectPointIso	\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotateProjectPointIsoInit proc

		mov	[Chooser_RtnI], offset RotateProjectPointIsoI	; Integer
		mov	[Chooser_RtnF], offset LongRotateProjectPointIsoF; FPU
		mov	[Chooser_RtnM], offset RotateProjectPointIsoM	; MMX
		mov	[Chooser_Result], offset RotateProjectPointIso	; Pointer
		jmp	Chooser

RotateProjectPointIsoInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

