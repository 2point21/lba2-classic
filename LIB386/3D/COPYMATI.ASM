;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	CopyMatrixI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	CopyMatrixI(S32 *MatDst, S32 *MatSrc) ;

;#pragma aux CopyMatrixI	"*"	\
;	parm		[edi] [esi]	\
;	modify exact	[eax ebx ecx]

CopyMatrixI	proc

		mov	eax, [esi].MAT_M11
		mov	ebx, [esi].MAT_M12
		mov	ecx, [esi].MAT_M13
		mov	[edi].MAT_M11, eax
		mov	[edi].MAT_M12, ebx
		mov	[edi].MAT_M13, ecx

		mov	eax, [esi].MAT_M21
		mov	ebx, [esi].MAT_M22
		mov	ecx, [esi].MAT_M23
		mov	[edi].MAT_M21, eax
		mov	[edi].MAT_M22, ebx
		mov	[edi].MAT_M23, ecx

		mov	eax, [esi].MAT_M31
		mov	ebx, [esi].MAT_M32
		mov	ecx, [esi].MAT_M33
		mov	[edi].MAT_M31, eax
		mov	[edi].MAT_M32, ebx
		mov	[edi].MAT_M33, ecx

		ret

CopyMatrixI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
