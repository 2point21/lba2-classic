;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL


;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

		public	SYSCALL	QSqr
		public	SYSCALL	Sqr

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;U32	QSqr(U32 xLow, U32 xHigh)	;

;#pragma aux QSqr				\
;	parm		[eax] [edx]		\
;	modify		[ebx ecx edx esi edi]

QSqr		proc

		test	edx, edx
		jz	Sqr

		cmp	edx, 3
		jbe	easy

		mov	cl, 33
		mov	ebx, eax
		bsr	eax, edx		; highest bit
		xor	edi, edi
		sub	cl, al
		add	eax, 32
		and	cl, -2			; compute left shift
						; to get highest bit couple
						; in edi
		shld	edi, edx, cl		; do shifting
		shld	edx, ebx, cl
		shl	ebx, cl
		mov	ecx, eax		; couples of bits remaining
  		mov	eax, 1			; rslt = 1
		dec	edi
;		shr	ecx, 1
		db	0C1h, 0E9h, 01h		; shr ecx, 1 with better encoding
		jmp	Sqr_loop
Sqr_1:
		dec	ecx			; one less couple of bits
		jz	exit
Sqr_loop:
		add	ebx, ebx
		lea	esi, [eax*4+1]		; rslt*4+1
		adc	edx, edx
		lea	eax, [eax*2+1]		; rslt = rslt*2+1 (optimistic)
		adc	edi, edi		; the add/adc sequence correspond
		add	ebx, ebx		; to  shld edi, edx, 2
		adc	edx, edx		;     shld edx, ebx, 2
		adc	edi, edi		;     shl ebx, 2
		sub	edi, esi		; remainder - (rslt*4+1)
		jnc	Sqr_1			; ok -> Sqr_1
		add	edi, esi		; else undo
		dec	eax			; rslt = rslt*2
		dec	ecx			; one less couple of bits
		jnz	short Sqr_loop
exit:
		ret
easy:
		lea	edi, [edx-1]
		mov	edx, eax
		xor	ebx, ebx
		mov	ecx, 16
		mov	eax, 1
		jmp	Sqr_loop
ok:
QSqr		endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;U32	Sqr(U32 x)			;

;#pragma aux Sqr				\
;	parm		[eax]			\
;	modify		[ebx ecx edx esi]

Sqr		proc

		cmp	eax, 3
		jbe	short Sqr_0_1

		mov	cl, 33
		mov	ebx, eax
		bsr	eax, eax		; find last bit set
		xor	edx, edx
		sub	cl, al
		and	cl, -2			; compute how many left shifts
						; in order to get the first
						; non-0 couple in edx
		shld	edx, ebx, cl
		shl	ebx, cl
		mov	ecx, eax
		mov	eax, 1			; rslt = 1
		dec	edx			; remainder -= 1
;		shr	ecx, 1			; couples left
		db	0C1h, 0E9h, 01h		; shr ecx, 1 with better encoding
		jmp	Sqr_loop
Sqr_1:
		dec	ecx
		jz	exit
Sqr_loop:
		add	ebx, ebx		; the add/adc sequence correspond
		lea	esi, [eax*4+1]		; to   shld edx, ebx, 2
		adc	edx, edx		;      shl  ebx, 2
		add	ebx, ebx
		adc	edx, edx
		lea	eax, [eax*2+1]
		sub	edx, esi
		jnc	Sqr_1

		add	edx, esi
		dec	eax
		dec	ecx
		jnz	Sqr_loop
exit:
		ret
Sqr_0_1:
		test	eax, eax
		jz	exit0
		mov	eax, 1
exit0:
		ret

Sqr		endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END


