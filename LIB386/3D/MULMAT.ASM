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

		public	SYSCALL	MulMatrix

MulMatrix	dd	offset MulMatrixInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	MulMatrixI:PROC
		extrn	SYSCALL	MulMatrixF:PROC
		extrn	SYSCALL	MulMatrixM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	MulMAtrix(S32 *MatDst, S32 *MatSrc1, S32* MatSrc2) ;

;#pragma aux MulMAtrix		       		\
;	parm		[edi] [esi] [ebx]	\
;	modify exact	[eax ecx edx]

; GOODIE: MatDst CAN BE EQUAL to MatSrc1

MulMatrixInit	proc

		mov	[Chooser_RtnI], offset MulMatrixI	; Integer
		mov	[Chooser_RtnF], offset MulMatrixF	; FPU
		mov	[Chooser_RtnM], offset MulMatrixM	; MMX
		mov	[Chooser_Result], offset MulMatrix	; Pointer
		jmp	Chooser

MulMatrixInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

