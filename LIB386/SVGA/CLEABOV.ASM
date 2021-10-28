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
		public	SYSCALL	ClearBoxVESA

ClearBoxVESA	dd	offset ClearBoxVESAInit

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	ClearBoxVESAI:PROC
		extrn	SYSCALL	ClearBoxVESAF:PROC
		extrn	SYSCALL	ClearBoxVESAM:PROC

		extrn	SYSCALL	ClearBoxI:PROC
		extrn	SYSCALL	ClearBoxF:PROC
		extrn	SYSCALL	ClearBoxM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	ClearBoxVESAInit

;*══════════════════════════════════════════════════════════════════════════*
;void	ClearBoxVESA(void *dst, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBoxVESA	       	\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxVESAInit	proc

		mov	[Chooser_Result], offset ClearBoxVESA	; Pointer

		test	[VESAMode], 4000h
		jnz	OkLinear

		mov	[Chooser_RtnI], offset ClearBoxVESAI	; Integer
		mov	[Chooser_RtnF], offset ClearBoxVESAF	; FPU
		mov	[Chooser_RtnM], offset ClearBoxVESAM	; MMX

		jmp	Chooser
OkLinear:
		mov	[Chooser_RtnI], offset ClearBoxI	; Integer
		mov	[Chooser_RtnF], offset ClearBoxF	; FPU
		mov	[Chooser_RtnM], offset ClearBoxM	; MMX

		jmp	Chooser

ClearBoxVESAInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

