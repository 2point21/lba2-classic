;----------------------------------------------------------------------------
;				S_LINE.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
		.386p
		jumps
		.model FLAT, SYSCALL

		.data

		include	svga.ash

EVEN

couleur		dw	00

		.code

		public	NoLanguage	Line
		public	NoLanguage	Line_A

;----------------------------------------------------------------------------
;		Line( x0, y0, x1, y1, coul )
;
Line		proc	uses esi edi ebp ebx,\
		x0:DWORD, y0:DWORD,\
		x1:DWORD, y1:DWORD,\
		coul:DWORD

; 		X0:ax  Y0:bx
;		X1:cx  Y1:DX
;		couleur: bp

		mov	eax, x0
		mov	ebx, y0
		mov	ecx, x1
		mov	edx, y1

		mov	ebp, coul

		call	Line_A

		ret

Line		endp

Line_A		proc

; 		X0:eax  Y0:ebx
;		X1:ecx  Y1:edx

		mov	[couleur], bp

		cmp	eax, ecx		; x0 < x1
		jle	sensok
		xchg	eax, ecx
		xchg	ebx, edx
sensok:
		; XY 0

onceagain:	xor	bp, bp

		cmp	eax, [ClipXmin]			; x0
		jge	ncxi
		or	bp, 1
		jmp	short ncxg

ncxi:		cmp	eax, [ClipXmax]
		jg      notseen

ncxg:		cmp	ebx, [ClipYmin]
		jge	ncyi
		or	bp, 8
		jmp	short ncya

ncyi:		cmp	ebx, [ClipYmax]
		jle	ncya
		or	bp, 4
ncya:
		; XY 1

		shl	bp, 8

		cmp	ecx, [ClipXmin]
		jl	notseen

		cmp	ecx, [ClipXmax]
		jle	ncx2a
		or	bp, 2
ncx2a:
		cmp	edx, [ClipYmin]
		jge	ncy2i
		or	bp, 8
		jmp	short ncy2a

ncy2i:		cmp	edx, [ClipYmax]
		jle	ncy2a
		or	bp, 4
ncy2a:

		; test

		mov	si, bp
		shr	si, 8
		test	si, bp
		jnz	notseen
		or	si, bp
		and	si, 0FFh
		jz	draw

		mov	edi, ecx
		sub	edi, eax		; edi delta x

		mov	esi, edx
		sub	esi, ebx		; esi delta y


		test	bp, 100h
		jz	c0

		mov	ebp, edx

		sub	eax, [ClipXmin]		; xmin - x0
		neg	eax
		imul	si			; * dy
		idiv	di			; / dx
		movsx	eax, ax
		add     ebx, eax		; y0 +=
		mov	eax, [ClipXmin]		; x0 = xmin

		mov	edx, ebp

		jmp     onceagain
c0:
		test	bp, 800h
		jz	c1

		mov	ebp, edx

		xchg	eax, ebx
		sub	eax, [ClipYmin]		; ymin - y0
		neg	eax
		imul	di			; * dx
		idiv	si			; / dy
		movsx	eax, ax
		add     eax, ebx		; x0 +=
		mov	ebx, [ClipYmin]		; y0 = ymin

		mov	edx, ebp

		jmp     onceagain
c1:
		test	bp, 400h
		jz	c2

		mov	ebp, edx

		xchg	eax, ebx
		sub	eax, [ClipYmax]		; ymax - y0
		neg	eax
		imul	di			; * dx
		idiv	si			; / dy
		movsx	eax, ax
		add     eax, ebx		; x0 +=
		mov	ebx, [ClipYmax]		; y0 = ymax

		mov	edx, ebp

		jmp     onceagain



c2:
		test	bp, 2
		jz	c3

		mov	ecx, eax			; save x0
		mov	eax, [ClipXmax]
		sub	eax, ecx			; xmax - x0
		imul	si				; * dy
		idiv	di				; / dx
		movsx	edx, ax
		add     edx, ebx			; + y0
		mov	eax, ecx			; restore x0
		mov	ecx, [ClipXmax]

		jmp     onceagain

c3:
		test	bp, 8
		jz	c4

		mov	ecx, eax
		mov	eax, ebx
		sub	eax, [ClipYmin]			; ymin - y0
		neg	eax
		imul	di				; * dx
		idiv	si				; / dy
		movsx	edx, ax
		mov	eax, ecx
		add     ecx, edx			; x0 +=
		mov	edx, [ClipYmin]

		jmp     onceagain

c4:
		test	bp, 4
		jz	onceagain

		mov	ecx, eax
		mov	eax, ebx
		sub	eax, [ClipYmax]			; ymax - y0
		neg	eax
		imul	di				; * dx
		idiv	si				; / dy
		movsx	edx, ax
		mov	eax, ecx
		add	ecx, edx
		mov	edx, [ClipYmax]

		jmp     onceagain

notseen:	ret

;----------------------------------------------------------------------------

draw:           mov	esi, 640
		sub	ecx, eax
		sub	edx, ebx
		jns	descend

		neg	esi
		neg	edx

descend:	mov	edi, Log
		add	edi, TabOffLine[ ebx * 4 ]
		add	edi, eax

		mov	ax, [couleur]

		cmp	ecx, edx
		jl	vertical

horizontal:	mov	ebp, ecx
		shl	ebp, 1
		mov	ebx, ecx
		shl	edx, 1
		inc	ecx
		jmp	loopX

noovfy:		dec	ecx
		jz	exit
loopX:		mov	byte ptr[edi], al
		inc	edi
		sub	ebx, edx
		jnc	noovfy
		add	ebx, ebp
		add	edi, esi
		dec	ecx
		jnz	loopX

		ret

vertical:	xchg	ecx, edx
		mov	ebp, ecx
		shl	ebp, 1
		mov	ebx, ecx
		shl	edx, 1
		inc	ecx
		jmp	loopY

noovfx:		add	edi, esi
		dec	ecx
		jz	exit
loopY:		mov	byte ptr[edi], al
		sub	ebx, edx
		jnc	noovfx
		add	ebx, ebp
		adc	edi, esi
		dec	ecx
		jnz	loopY

exit:
		ret

Line_A		endp


		end
