;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn	SYSCALL	SinTab:WORD
		extrn	SYSCALL	CosTab:WORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public	SYSCALL	InitMatrixTransI

;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrixTransI(S32 *MatDst, S32 tx, S32 ty, S32 tz) ;

;#pragma aux InitMatrixTransI	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[]

InitMatrixTransI	proc

		mov	[edi].MAT_MTX, eax
		mov	[edi].MAT_MTY, ebx

		mov	[edi].MAT_MTZ, ecx

		ret

InitMatrixTransI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
