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

		public	SYSCALL	InitMatrix3DSI

;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrix3DSI(S32 *MatDst, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux InitMatrix3DSI	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx esi]


InitMatrix3DSI	proc

		and	eax, 4095
		and	ecx, 4095

		and	ebx, 4095
		add	eax, eax

		add	ecx, ecx
		add	ebx, ebx

		push	ebp
		push	ebx

		movsx	ebp, SinTab[ecx]
		movsx	esi, SinTab[eax]
		movsx	ebx, CosTab[ecx]
		movsx	ecx, CosTab[eax]

		mov	eax, esi
		mov	edx, ecx

		imul	eax, ebp
		imul	edx, ebp

		sar	eax, 14
		lea	edx, [edx - (1 SHL 13) - 1]	; START: neg edx + 0.5

		adc	eax, 0
		xor	edx, -1				; END : neg edx


		mov	[edi].MAT_M21, ebp
		mov	[edi].MAT_M13, eax

		sar	edx, 14
		mov	[edi].MAT_M32, esi

		mov	[edi].MAT_M12, edx
		mov	edx, esi

		pop	esi
		mov	eax, ecx

		imul	edx, ebx
		imul	eax, ebx

		movsx	ebp, SinTab[esi]

		sar	eax, 14
		lea	edx, [edx - (1 SHL 13) - 1]	; START: neg edx + 0.5

		adc	eax, 0
		xor	edx, -1				; END : neg edx

		sar	edx, 14
		mov	[edi].MAT_M22, eax

		movsx	esi, CosTab[esi]

		mov	[edi].MAT_M23, edx
		mov	eax, ecx

		imul	ecx, esi
		imul	eax, ebp

		sar	ecx, 14
		lea	eax, [eax - (1 SHL 13) - 1]	; START: neg eax + 0.5

		adc	ecx, 0
		xor	eax, -1				; END : neg eax

		sar	eax, 14
		mov	edx, [edi].MAT_M13

		mov	[edi].MAT_M33, ecx
		mov	[edi].MAT_M31, eax

		mov	eax, ebx
		mov	ecx, edx

		imul	ebx, ebp
		imul	ecx, esi
		imul	eax, esi
		imul	edx, ebp

		add	ecx, ebx
		sub	eax, edx

		sar	ecx, 14
		lea	eax, [eax + (1 SHL 13)]

		adc	ecx, 0
		mov	ebx, [edi].MAT_M21

		sar	eax, 14
		mov	[edi].MAT_M13, ecx

		mov	[edi].MAT_M11, eax
		mov	ecx, [edi].MAT_M23

		mov	eax, ebx
		mov	edx, ecx

		imul	ebx, ebp
		imul	ecx, esi
		imul	eax, esi
		imul	edx, ebp

		add	ecx, ebx
		sub	eax, edx

		sar	ecx, 14
		pop	ebp

		adc	ecx, 0
		xor	ebx, ebx

		sar	eax, 14
		mov	[edi].MAT_M23, ecx

		adc	eax, 0
		mov	[edi].MAT_MTX, ebx

		mov	[edi].MAT_M21, eax
		mov	[edi].MAT_MTY, ebx

		mov	[edi].MAT_MTZ, ebx
		ret

InitMatrix3DSI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
