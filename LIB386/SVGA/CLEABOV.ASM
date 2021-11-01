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
		public	SYSCALL	ClearBoxVESA

ClearBoxVESA	dd	offset ClearBoxVESAInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	ClearBoxVESAI:PROC
		extrn	SYSCALL	ClearBoxVESAF:PROC
		extrn	SYSCALL	ClearBoxVESAM:PROC

		extrn	SYSCALL	ClearBoxI:PROC
		extrn	SYSCALL	ClearBoxF:PROC
		extrn	SYSCALL	ClearBoxM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	ClearBoxVESAInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

