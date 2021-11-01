;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include iammx.inc
		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	SinTab:WORD
		extrn	SYSCALL	CosTab:WORD
		extrn	SYSCALL	MatrixLib1:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE
		extrn	SYSCALL	MulMatrixM:PROC



		public	SYSCALL	InitMatrixStdM

COMMENT \
Explication sur la composition de la matrice de rotation:

M(Alpha) =	1   0   0
		0   Ca  -Sa
		0   Sa  Ca

M(Gamma) =	Cg  -Sg 0
		Sg  Cg  0
		0   0   1


M(Beta)  =	Cb  0   Sb
		0   1   0
		-Sb 0   Cb


M(Finale) = {M(Alpha)*M(Gamma)}*M(Beta)

\

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	InitMatrixStdM(S32 *MatDst, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux InitMatrixStdM	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx esi]

InitMatrixStdM		PROC

			and	eax,4095
			and	ecx,4095

			add	eax,eax
			add	ecx,ecx

			xor	edx,edx
			and	ebx,4095

			mov	dx,[CosTab+eax]
			xor	esi,esi
							; EDI = M(Alpha)
			mov	[edi].MMX_M22,dx

			mov	word ptr [edi].MMX_M21,si
			mov	dword ptr [edi],16384	; .MMX_M11=+0

			mov	word ptr [edi].MMX_M31,si
			mov	dword ptr [edi].MMX_M13,esi

			mov	dword ptr[edi].MMX_M33,edx

			mov	dx,[SinTab+eax]
			lea	eax, [ebx*2]

			mov	[edi].MMX_M32,dx
			lea	ebx,MatrixLib1

			neg	dx

			xor	esi,esi
			mov	dword ptr[edi].MMX_M23,edx


							; ebx = M(Gamma)
			mov	dx,[CosTab+ecx]
			mov	dword ptr [ebx].MMX_M13,esi

			mov	[ebx].MMX_M11,dx
			mov	dword ptr [ebx].MMX_M23,esi

			mov	[ebx].MMX_M22,dx
			mov	dword ptr[ebx].MMX_M31,esi

			mov	dx,[SinTab+ecx]
			mov	ecx,16384

			mov	[ebx].MMX_M21,dx
			mov	dword ptr[ebx].MMX_M33,ecx

			neg	dx

			mov	[ebx].MMX_M12,dx
			mov	esi,edi

			call	MulMatrixM		; Registers not modified
			lea	esi,MatrixLib1

							; ESI = M(Beta)
			mov	dx,[CosTab+eax]
			mov	ecx,16384 shl 16

			mov	dword ptr[esi].MMX_M11,edx
			mov	dword ptr[esi].MMX_M33,edx

			mov	dx,[SinTab+eax]
			mov	dword ptr[esi].MMX_M21,ecx

			mov	dword ptr[esi].MMX_M13,edx
			xor	ebx, ebx

			neg	dx
			mov	dword ptr[esi].MMX_M23, ebx

			mov	dword ptr[esi].MMX_M31, edx
			pxor	mm0, mm0

			movq	[esi+24], mm0
			mov	ebx, esi

			movq	[esi+32], mm0
			mov	esi, edi

			movq	[esi+40], mm0
			jmp	MulMatrixM

InitMatrixStdM		ENDP

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;			The
			End

