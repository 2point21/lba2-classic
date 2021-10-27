;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA


;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	FlipMatrixI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	FlipMatrixI(S32 *MatDst, S32 *MatSrc) ;

;#pragma aux FlipMatrixI	"*"	\
;	parm		[edi] [esi]	\
;	modify exact	[eax ebx ecx]


; this routine can flip a matrix onto itself

FlipMatrixI	proc

		mov	eax, [esi].MAT_M11
		mov	ebx, [esi].MAT_M12

		mov	ecx, [esi].MAT_M21
		mov	[edi].MAT_M11, eax

		mov	[edi].MAT_M21, ebx
		mov	[edi].MAT_M12, ecx

		mov	eax, [esi].MAT_M31
		mov	ebx, [esi].MAT_M13

		mov	ecx, [esi].MAT_M33
		mov	[edi].MAT_M13, eax

		mov	[edi].MAT_M31, ebx
		mov	[edi].MAT_M33, ecx

		mov	eax, [esi].MAT_M22
		mov	ebx, [esi].MAT_M23

		mov	ecx, [esi].MAT_M32
		mov	[edi].MAT_M22, eax

		mov	[edi].MAT_M32, ebx
		mov	[edi].MAT_M23, ecx

		ret
FlipMatrixI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
