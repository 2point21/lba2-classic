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

		public	SYSCALL	MulMatrix

MulMatrix	dd	offset MulMatrixInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	MulMatrixI:PROC
		extrn	SYSCALL	MulMatrixF:PROC
		extrn	SYSCALL	MulMatrixM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

