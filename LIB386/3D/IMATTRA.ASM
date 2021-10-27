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

		public	SYSCALL	InitMatrixTrans

InitMatrixTrans	dd	offset InitMatrixTransInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	InitMatrixTransI:PROC
		extrn	SYSCALL	InitMatrixTransF:PROC
		extrn	SYSCALL	InitMatrixTransM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	InitMatrixTrans(S32 *MatDst, S32 tx, S32 ty, S32 tz) ;

;#pragma aux InitMatrixTrans	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[]

InitMatrixTransInit proc

		mov	[Chooser_RtnI], offset InitMatrixTransI	; Integer
		mov	[Chooser_RtnF], offset InitMatrixTransF	; FPU
		mov	[Chooser_RtnM], offset InitMatrixTransM	; MMX
		mov	[Chooser_Result], offset InitMatrixTrans	; Pointer
		jmp	Chooser

InitMatrixTransInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
