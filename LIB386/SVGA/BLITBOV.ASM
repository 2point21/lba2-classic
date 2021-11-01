;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	VESAMode:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	BlitBoxVESA

BlitBoxVESA	dd	offset BlitBoxVESAInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	BlitBoxVESAI:PROC
		extrn	SYSCALL	BlitBoxVESAF:PROC
		extrn	SYSCALL	BlitBoxVESAM:PROC

		extrn	SYSCALL	BlitBoxI:PROC
		extrn	SYSCALL	BlitBoxF:PROC
		extrn	SYSCALL	BlitBoxM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	BlitBoxVESAInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	BlitBoxVESA(void *dst, U32 *TabOffDst, void *box) ;

;#pragma aux BlitBoxVESA	       	\
;	parm	[edi] [edx] [esi]	\
;	modify	[eax ebx ecx]

BlitBoxVESAInit	proc

		mov	[Chooser_Result], offset BlitBoxVESA	; Pointer

		test	[VESAMode], 4000h
		jnz	OkLinear

		mov	[Chooser_RtnI], offset BlitBoxVESAI	; Integer
		mov	[Chooser_RtnF], offset BlitBoxVESAF	; FPU
		mov	[Chooser_RtnM], offset BlitBoxVESAM	; MMX

		jmp	Chooser
OkLinear:
		mov	[Chooser_RtnI], offset BlitBoxI		; Integer
		mov	[Chooser_RtnF], offset BlitBoxF		; FPU
		mov	[Chooser_RtnM], offset BlitBoxM		; MMX

		jmp	Chooser

BlitBoxVESAInit	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

