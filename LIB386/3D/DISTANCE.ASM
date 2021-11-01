;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL


;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	Distance2D
		public	SYSCALL	Distance3D

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	QSqr:PROC

;*══════════════════════════════════════════════════════════════════════════*
;U32	Distance2D(S32 x0, S32 y0, S32 x1, S32 y1)	;

;#pragma aux Distance2D	"*"			\
;	parm		[eax] [ecx] [ebx] [edx]	\
;	modify		[esi edi]

Distance2D	proc

		sub	eax, ebx		; Dx = x0-x1
		sub	ecx, edx		; Dy = y0-y1
		imul	eax			; Dx²
		mov	esi, eax
		mov	eax, ecx
		mov	edi, edx
		imul	eax			; Dy²
		add	eax, esi		; Dx² + Dy²
		adc	edx, edi		; ...

		jmp	QSqr			; eax = √ (Dx²+Dy²)

Distance2D	endp

;*══════════════════════════════════════════════════════════════════════════*
;U32	Distance3D(S32 x0, S32 y0, S32 z0, S32 x1, S32 y1, S32 z1)	;

;#pragma aux Distance3D	"*"					\
;	parm		[eax] [ecx] [ebx] [edx]	[esi] [edi]	\
;       modify		[esi edi]

Distance3D	proc

		sub	eax, edx
		sub	ecx, esi
		imul	eax			; Dx²
		sub	ebx, edi
		mov	esi, eax
		mov	eax, ecx
		mov	edi, edx
		imul	eax			; Dy²
		add	esi, eax
		mov	eax, ebx
		adc	edi, edx
		imul	eax			; Dz²
		add	eax, esi
		adc	edx, edi

		jmp	QSqr			; eax = √ (Dx²+Dy²+Dz²)

Distance3D	endp

;*══════════════════════════════════════════════════════════════════════════*
		;The
		 End

