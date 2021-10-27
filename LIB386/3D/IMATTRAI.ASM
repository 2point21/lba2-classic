;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	SinTab:WORD
		extrn	SYSCALL	CosTab:WORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	InitMatrixTransI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
