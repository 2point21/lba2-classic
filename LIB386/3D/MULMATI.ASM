;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	MulMatrixI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	MulMAtrixI(S32 *MatDst, S32 *MatSrc1, S32* MatSrc2) ;

;#pragma aux MulMAtrixI		       		\
;	parm		[edi] [esi] [ebx]	\
;	modify exact	[eax ecx edx]

MulMatrixI	proc uses ebp

		mov	eax, [esi].MAT_M11
		mov	ebp, [esi].MAT_M12

		imul	eax, [ebx].MAT_M11
		imul	ebp, [ebx].MAT_M21

		add	eax, ebp
		mov	ebp, [esi].MAT_M13

		imul	ebp, [ebx].MAT_M31

		add	eax, ebp
		mov	ecx, [esi].MAT_M11

		mov	ebp, [esi].MAT_M12

		imul	ecx, [ebx].MAT_M12
		imul	ebp, [ebx].MAT_M22

		add	ecx, ebp
		mov	ebp, [esi].MAT_M13

		imul	ebp, [ebx].MAT_M32

		sar	eax, 14
		mov	edx, [esi].MAT_M11

		adc	eax, 0
		add	ecx, ebp

		sar	ecx, 14
		mov	ebp, [esi].MAT_M12

		adc	ecx, 0

		imul	edx, [ebx].MAT_M13
		imul	ebp, [ebx].MAT_M23

		add	edx, ebp
		mov	ebp, [esi].MAT_M13

		imul	ebp, [ebx].MAT_M33

		add	edx, ebp
		mov	[edi].MAT_M11, eax

		sar	edx, 14
		mov	[edi].MAT_M12, ecx

		adc	edx, 0
		mov	eax, [esi].MAT_M21

		mov	[edi].MAT_M13, edx
		mov	ebp, [esi].MAT_M22

		imul	eax, [ebx].MAT_M11
		imul	ebp, [ebx].MAT_M21

		add	eax, ebp
		mov	ebp, [esi].MAT_M23

		imul	ebp, [ebx].MAT_M31

		add	eax, ebp
		mov	ecx, [esi].MAT_M21

		sar	eax, 14
		mov	ebp, [esi].MAT_M22

		adc	eax, 0

		imul	ecx, [ebx].MAT_M12
		imul	ebp, [ebx].MAT_M22

		add	ecx, ebp
		mov	ebp, [esi].MAT_M23

		imul	ebp, [ebx].MAT_M32

		add	ecx, ebp
		mov	edx, [esi].MAT_M21

		mov	ebp, [esi].MAT_M22

		imul	edx, [ebx].MAT_M13
		imul	ebp, [ebx].MAT_M23

		add	edx, ebp
		mov	ebp, [esi].MAT_M23

		imul	ebp, [ebx].MAT_M33

		sar	ecx, 14
		mov	[edi].MAT_M21, eax

		adc	ecx, 0
		add	edx, ebp

		sar	edx, 14
		mov	[edi].MAT_M22, ecx

		adc	edx, 0
		mov	eax, [esi].MAT_M31

		mov	[edi].MAT_M23, edx
		mov	ebp, [esi].MAT_M32

		imul	eax, [ebx].MAT_M11
		imul	ebp, [ebx].MAT_M21

		add	eax, ebp
		mov	ebp, [esi].MAT_M33

		imul	ebp, [ebx].MAT_M31

		add	eax, ebp
		mov	ecx, [esi].MAT_M31

		mov	ebp, [esi].MAT_M32

		imul	ecx, [ebx].MAT_M12
		imul	ebp, [ebx].MAT_M22

		add	ecx, ebp
		mov	ebp, [esi].MAT_M33

		imul	ebp, [ebx].MAT_M32

		sar	eax, 14
		mov	edx, [esi].MAT_M31

		adc	eax, 0
		add	ecx, ebp

		sar	ecx, 14
		mov	ebp, [esi].MAT_M32

		adc	ecx, 0

		imul	edx, [ebx].MAT_M13
		imul	ebp, [ebx].MAT_M23

		add	edx, ebp
		mov	ebp, [esi].MAT_M33

		imul	ebp, [ebx].MAT_M33

		mov	[edi].MAT_M31, eax
		xor	eax, eax

		add	edx, ebp
		mov	[edi].MAT_M32, ecx

		sar	edx, 14
		mov	[edi].MAT_MTX, eax

		adc	edx, eax
		mov	[edi].MAT_MTY, eax

		mov	[edi].MAT_M33, edx
		mov	[edi].MAT_MTZ, eax

		ret

MulMatrixI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

