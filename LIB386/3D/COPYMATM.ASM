;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc
		INCLUDE	IAMMX.INC		; Pour pouvoir assembler du MMx

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public	SYSCALL	CopyMatrixM

;*══════════════════════════════════════════════════════════════════════════*
;void	CopyMatrixM(S32 *MatDst, S32 *MatSrc) ;

;#pragma aux CopyMatrixM	"*"	\
;	parm		[edi] [esi]	\
;	modify exact	[]

CopyMatrixM	proc

		movq	mm0,[esi]

		movq	mm1,[esi+8]

		movq	mm2,[esi+16]

		movq	[edi],mm0

		movq	[edi+8],mm1

		movq	[edi+16],mm2

		ret
CopyMatrixM	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
