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

		public	SYSCALL	GetMatrixTransI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	GetMatrixTransI(S32 *MatDst, S32 *tx, S32 *ty, S32 *tz) ;

;#pragma aux GetMatrixTransI	"*"		\
;	parm		[eds] [eax] [ebx] [ecx]	\
;	modify exact	[edx esi edi]

GetMatrixTransI	proc

		mov	edx, [esi].MAT_MTX
		mov	edi, [esi].MAT_MTY
		mov	esi, [esi].MAT_MTZ

		mov	[eax], edx
		mov	[ebx], edi
		mov	[ecx], esi

		ret

GetMatrixTransI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
