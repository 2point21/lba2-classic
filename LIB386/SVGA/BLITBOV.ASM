;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	VESAMode:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	BlitBoxVESA

BlitBoxVESA	dd	offset BlitBoxVESAInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	BlitBoxVESAI:PROC
		extrn	SYSCALL	BlitBoxVESAF:PROC
		extrn	SYSCALL	BlitBoxVESAM:PROC

		extrn	SYSCALL	BlitBoxI:PROC
		extrn	SYSCALL	BlitBoxF:PROC
		extrn	SYSCALL	BlitBoxM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	BlitBoxVESAInit

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

