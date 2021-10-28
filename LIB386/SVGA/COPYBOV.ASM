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
		public	SYSCALL	CopyBoxVESA

CopyBoxVESA	dd	offset CopyBoxVESAInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	CopyBoxVESAI:PROC
		extrn	SYSCALL	CopyBoxVESAF:PROC
		extrn	SYSCALL	CopyBoxVESAM:PROC

		extrn	SYSCALL	CopyBoxI:PROC
		extrn	SYSCALL	CopyBoxF:PROC
		extrn	SYSCALL	CopyBoxM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	CopyBoxVESAInit

;*══════════════════════════════════════════════════════════════════════════*
;void	CopyBoxVESA(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBoxVESA	       	       	\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxVESAInit	proc

		mov	[Chooser_Result], offset CopyBoxVESA	; Pointer

		test	[VESAMode], 4000h
		jnz	OkLinear

		mov	[Chooser_RtnI], offset CopyBoxVESAI	; Integer
		mov	[Chooser_RtnF], offset CopyBoxVESAF	; FPU
		mov	[Chooser_RtnM], offset CopyBoxVESAM	; MMX

		jmp	Chooser
OkLinear:
		mov	[Chooser_RtnI], offset CopyBoxI		; Integer
		mov	[Chooser_RtnF], offset CopyBoxF		; FPU
		mov	[Chooser_RtnM], offset CopyBoxM		; MMX

		jmp	Chooser

CopyBoxVESAInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

