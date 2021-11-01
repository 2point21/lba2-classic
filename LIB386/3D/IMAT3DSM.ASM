;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include iammx.inc
		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn	SYSCALL	SinTab:WORD
		extrn	SYSCALL	CosTab:WORD
		extrn	SYSCALL	MatrixLib1:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE
		extrn	SYSCALL	MulMatrixM:PROC

		public	SYSCALL	InitMatrix3DSM

COMMENT \
Explication sur la composition de la matrice de rotation 3DS:

M(Alpha) =	1   0   0
		0   Ca  -Sa
		0   Sa  Ca

M(Gamma) =	Cg  -Sg 0
		Sg  Cg  0
		0   0   1


M(Beta)  =	Cb  0   Sb
		0   1   0
		-Sb 0   Cb


M(Finale) = {M(Gamma)*M(Alpha)}*M(Beta)

\



;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrix3DSM(S32 *MatDst, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux InitMatrix3DSM	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx esi]

InitMatrix3DSM		PROC

			and	eax,4095
			and	ecx,4095

			add	eax,eax
			add	ecx,ecx

			xor	edx,edx
			mov	esi, ebx

			mov	dx,[CosTab+ecx]
			xor	ebx,ebx
							; EDI = M(Gamma)
			mov	[edi].MMX_M11,dx
			mov	dword ptr[edi].MMX_M13,ebx

			mov	[edi].MMX_M22,dx
			mov	dword ptr[edi].MMX_M23,ebx

			mov	dx,[SinTab+ecx]
			mov	ecx,16384

			mov	[edi].MMX_M21,dx
			mov	dword ptr[edi].MMX_M31,ebx

			neg	dx			; ET NON EDX !!!!!!
			mov	dword ptr[edi].MMX_M33,ecx

			mov	[edi].MMX_M12,dx
			lea	ebx,MatrixLib1

							; ebx = M(Alpha)
			mov	dx,[CosTab+eax]
			mov	dword ptr[ebx].MMX_M11,ecx

			mov	[ebx].MMX_M22,dx
			xor	ecx,ecx

			mov	[ebx].MMX_M21,cx
			mov	dword ptr[ebx].MMX_M33,edx

			mov	dx,[SinTab+eax]
			mov	dword ptr[ebx].MMX_M13,ecx

			mov	[ebx].MMX_M31,cx
			and	esi, 4095

			mov	[ebx].MMX_M32,dx
			add	esi, esi

			neg	dx
			mov	eax, esi

			mov	dword ptr [ebx].MMX_M23,edx
			mov	esi, edi

			call	MulMatrixM		; Registers not modified

							; ebx = M(Beta)
			mov	dx,[CosTab+eax]
			mov	eax,16384 shl 16

			mov	dword ptr[ebx].MMX_M11,edx
			mov	dword ptr[ebx].MMX_M33,edx

			mov	dx,[SinTab+eax]
			mov	dword ptr[ebx].MMX_M21,eax

			mov	dword ptr[ebx].MMX_M13,edx
			neg	dx			; ET NON EDX !!!!!!

			mov	dword ptr[ebx].MMX_M23,ecx
			mov	dword ptr[ebx].MMX_M31,edx

			pxor	mm0, mm0
			mov	ebx, esi

			movq	[esi+24], mm0
			mov	esi, edi

			movq	[esi+32], mm0

			movq	[esi+40], mm0

			jmp	MulMatrixM

InitMatrix3DSM		ENDP

;*══════════════════════════════════════════════════════════════════════════*
;			The
			End
