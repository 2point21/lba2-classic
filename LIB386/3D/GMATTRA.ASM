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

		public	SYSCALL	GetMatrixTrans

GetMatrixTrans	dd	offset GetMatrixTransGet

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	GetMatrixTransI:PROC
		extrn	SYSCALL	GetMatrixTransF:PROC
		extrn	SYSCALL	GetMatrixTransM:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	GetMatrixTrans(S32 *MatDst, S32 *tx, S32 *ty, S32 *tz) ;

;#pragma aux GetMatrixTrans	"*"		\
;	parm		[esi] [eax] [ebx] [ecx]	\
;	modify exact	[edx esi edi]

GetMatrixTransGet proc

		mov	[Chooser_RtnI], offset GetMatrixTransI	; Integer
		mov	[Chooser_RtnF], offset GetMatrixTransF	; FPU
		mov	[Chooser_RtnM], offset GetMatrixTransM	; MMX
		mov	[Chooser_Result], offset GetMatrixTrans	; Pointer
		jmp	Chooser

GetMatrixTransGet endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
