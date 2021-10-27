;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		.386p

		.model SMALL, SYSCALL


;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		.CODE

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		public	SYSCALL	Distance2D
		public	SYSCALL	Distance3D

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		extrn	SYSCALL	QSqr:PROC

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;U32	Distance2D(S32 x0, S32 y0, S32 x1, S32 y1)	;

;#pragma aux Distance2D	"*"			\
;	parm		[eax] [ecx] [ebx] [edx]	\
;	modify		[esi edi]

Distance2D	proc

		sub	eax, ebx		; Dx = x0-x1
		sub	ecx, edx		; Dy = y0-y1
		imul	eax			; Dxý
		mov	esi, eax
		mov	eax, ecx
		mov	edi, edx
		imul	eax			; Dyý
		add	eax, esi		; Dxý + Dyý
		adc	edx, edi		; ...

		jmp	QSqr			; eax = ๛ (Dxý+Dyý)

Distance2D	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;U32	Distance3D(S32 x0, S32 y0, S32 z0, S32 x1, S32 y1, S32 z1)	;

;#pragma aux Distance3D	"*"					\
;	parm		[eax] [ecx] [ebx] [edx]	[esi] [edi]	\
;       modify		[esi edi]

Distance3D	proc

		sub	eax, edx
		sub	ecx, esi
		imul	eax			; Dxý
		sub	ebx, edi
		mov	esi, eax
		mov	eax, ecx
		mov	edi, edx
		imul	eax			; Dyý
		add	esi, eax
		mov	eax, ebx
		adc	edi, edx
		imul	eax			; Dzý
		add	eax, esi
		adc	edx, edi

		jmp	QSqr			; eax = ๛ (Dxý+Dyý+Dzý)

Distance3D	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		;The
		 End

