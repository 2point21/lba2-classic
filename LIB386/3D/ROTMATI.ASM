;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include	mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	SinTab:WORD
		extrn	SYSCALL	CosTab:WORD
		extrn	SYSCALL	MatrixLib1:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	RotateMatrixI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateMatrixI(S32 *MatDst, S32 *MatSrc, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux RotateMatrixI				\
;	parm		[edi] [esi] [eax] [ebx] [ecx]	\
;	modify		[edx]

RotateMatrixI	proc

		add	eax, eax
		add	ebx, ebx

		push	ebx		; beta
		add	ecx, ecx

		xor	edx, edx
		push	ecx		; gamma

		mov	[edi].MAT_MTX, edx
		or	ebx,eax

		or	ebx,ecx
		mov	[edi].MAT_MTY, edx

		mov	[edi].MAT_MTZ, edx
		and	ebx, 4095*2

		je	@@NothingToDo

a:		and	eax, 4095*2
		jz	g

doalpha:	;			EAX s=TabSinus[a&2233];
		;			ECX c=TabSinus[(a+256)&2233];
		movsx	ecx, CosTab[ eax ]
		movsx	eax, SinTab[ eax ]

		;			LMat11 = LMatT11 ;
		;			LMat21 = LMatT21 ;
		;			LMat31 = LMatT31 ;
		mov	ebx, [esi].MAT_M11
		mov	edx, [esi].MAT_M21
		mov	[edi].MAT_M11, ebx
		mov	ebx, [esi].MAT_M31
		mov	[edi].MAT_M21, edx
		mov	[edi].MAT_M31, ebx
		;			LMat12 = ( c*LMatT12 +  s*LMatT13 )>>14;
		mov	edx, [esi].MAT_M12
		mov	ebx, [esi].MAT_M13
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M12, ebx
		;			LMat13 = ( c*LMatT13 + -s*LMatT12 )>>14;
		mov	edx, [esi].MAT_M12
		mov	ebx, [esi].MAT_M13
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M13, ebx
		;			LMat22 = ( c*LMatT22 +  s*LMatT23 )>>14;
		mov	edx, [esi].MAT_M22
		mov	ebx, [esi].MAT_M23
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M22, ebx
		;			LMat23 = ( c*LMatT23 + -s*LMatT22 )>>14;
		mov	edx, [esi].MAT_M22
		mov	ebx, [esi].MAT_M23
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M23, ebx
		;			LMat32 = ( c*LMatT32 +  s*LMatT33 )>>14;
		mov	edx, [esi].MAT_M32
		mov	ebx, [esi].MAT_M33
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M32, ebx
		;			LMat33 = ( c*LMatT33 + -s*LMatT32 )>>14;
		mov	edx, [esi].MAT_M32
		mov	ebx, [esi].MAT_M33
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M33, ebx

		mov	esi, edi
		mov	edi, Offset MatrixLib1

		xor	eax,eax		; Gamma rot not applied

g:		pop	edx
		and	edx, 4095*2
		jz	b

dogamma:
		;			EAX s=TabSinus[g&2233];
		;			ECX c=TabSinus[(g+256)&2233];
		movsx	eax, SinTab[ edx ]
		movsx	ecx, CosTab[ edx ]

		;			LMatD13 = LMat13;
		;			LMatD23 = LMat23;
		;			LMatD33 = LMat33;
		mov	ebx, [esi].MAT_M13
		mov	edx, [esi].MAT_M23
		mov	[edi].MAT_M13, ebx
		mov	ebx, [esi].MAT_M33
		mov	[edi].MAT_M23, edx
		mov	[edi].MAT_M33, ebx
		;			LMatD11 = ( c*LMat11 +  s*LMat12 )>>14;
		mov	edx, [esi].MAT_M11
		mov	ebx, [esi].MAT_M12
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M11, ebx
		;			LMatD12 = ( c*LMat12 + -s*LMat11 )>>14;
		mov	edx, [esi].MAT_M11
		mov	ebx, [esi].MAT_M12
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M12, ebx
		;			LMatD21 = ( c*LMat21 +  s*LMat22 )>>14;
		mov	edx, [esi].MAT_M21
		mov	ebx, [esi].MAT_M22
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M21, ebx
		;			LMatD22 = ( c*LMat22 + -s*LMat21 )>>14;
		mov	edx, [esi].MAT_M21
		mov	ebx, [esi].MAT_M22
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M22, ebx
		;			LMatD31 = ( c*LMat31 +  s*LMat32 )>>14;
		mov	edx, [esi].MAT_M31
		mov	ebx, [esi].MAT_M32
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M31, ebx
		;			LMatD32 = ( c*LMat32 + -s*LMat31 )>>14;
		imul	eax, [esi].MAT_M31
		imul	ecx, [esi].MAT_M32
		sub	ecx, eax
		sar	ecx, 14
		adc	ebx, 0
		mov	[edi].MAT_M32, ecx


		mov	eax,1		; Gamma rot applied


b:		pop	edx
		and	edx, 4095*2
		jnz	dobeta

		test	eax, eax
		je	exit

		cmp	edi, Offset MatrixLib1
		jne	exit

		mov	edi, esi
		mov	esi, Offset MatrixLib1
		mov	ecx, 9
		rep	movsd
exit:
		ret
dobeta:
		cmp	edi, Offset MatrixLib1
		jne	nopb

		test	eax, eax
		mov	eax, esi
		jne	@@CorrectPtr
		jmp	@@Copy_Mat
@@Alph_Gam_Bet:
		mov	edi, esi
		mov	esi, Offset MatrixLib1
		jmp	@@NoAlp_NoGam

nopb:
		test	eax, eax		; Tu remarqueras que j'ai
		je	@@NoAlp_NoGam		; mis un espace apres la virgule :-]

		mov	eax, edi
		mov	esi, edi
		mov	edi, Offset MatrixLib1
@@Copy_Mat:
		mov	ecx, 9
		rep	movsd
@@CorrectPtr:	mov	edi, eax
		mov	esi, Offset MatrixLib1
@@NoAlp_NoGam:



		;			si s=TabSinus[g&2233];
		;			cx c=TabSinus[(g+256)&2233];
		movsx	eax, SinTab[ edx ]
		movsx	ecx, CosTab[ edx ]

		;			LMat12 = LMatD12 ;
		;			LMat22 = LMatD22 ;
		;			LMat32 = LMatD32 ;
		mov	ebx, [esi].MAT_M12
		mov	edx, [esi].MAT_M22
		mov	[edi].MAT_M12, ebx
		mov	ebx, [esi].MAT_M32
		mov	[edi].MAT_M22, edx
		mov	[edi].MAT_M32, ebx

		;			LMat11 = ( c*LMatD11 + -s*LMatD13 )>>14;
		mov	edx, [esi].MAT_M13
		mov	ebx, [esi].MAT_M11
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M11, ebx
		;			LMat13 = ( c*LMatD13 +  s*LMatD11 )>>14;
		mov	edx, [esi].MAT_M13
		mov	ebx, [esi].MAT_M11
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M13, ebx
		;			LMat21 = ( c*LMatD21 + -s*LMatD23 )>>14;
		mov	edx, [esi].MAT_M23
		mov	ebx, [esi].MAT_M21
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M21, ebx
		;			LMat23 = ( c*LMatD23 +  s*LMatD21 )>>14;
		mov	edx, [esi].MAT_M23
		mov	ebx, [esi].MAT_M21
		imul	edx, ecx
		imul	ebx, eax
		add	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M23, ebx
		;			LMat31 = ( c*LMatD31 + -s*LMatD33 )>>14;
		mov	edx, [esi].MAT_M33
		mov	ebx, [esi].MAT_M31
		imul	edx, eax
		imul	ebx, ecx
		sub	ebx, edx
		sar	ebx, 14
		adc	ebx, 0
		mov	[edi].MAT_M31, ebx
		;			LMat33 = ( c*LMatD33 +  s*LMatD31 )>>14;
		imul	ecx, [esi].MAT_M33
		imul	eax, [esi].MAT_M31
		add	ecx, eax
		sar	ecx, 14
		adc	ebx, 0
		mov	[edi].MAT_M33, ecx

		ret
@@NothingToDo:
		pop	eax			; Just copy the matrix if no
		pop	ebx			; rotation to apply
		mov	ecx, 9
		rep	movsd
		ret
RotateMatrixI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
