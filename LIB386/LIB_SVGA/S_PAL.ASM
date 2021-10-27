;----------------------------------------------------------------------------
;				S_PAL.ASM 386
;			    (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p
				jumps
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data

			include	svga.ash

MyTab		db	768	dup(0)

;----------------------------------------------------------------------------
				.code

			public	 NoLanguage	Palette
			public	 NoLanguage	PalMulti
			public	 NoLanguage	PalOne

;----------------------------------------------------------------------------
;		Palette( (UBYTE*)PalColor )
;
Palette			proc	uses esi edi,\
			PalColor:DWORD
;----------------------
			mov	esi, PalColor
			lea	edi, MyTab
			mov	ecx, 256
;----------------------
Again:			mov	al, [esi]
			shr	al, 2
			mov	[edi], al
			mov	al, [esi+1]
			shr	al, 2
			mov	[edi+1], al
			mov	al, [esi+2]
			shr	al, 2
			mov	[edi+2], al
			add	esi, 3
			add	edi, 3
			dec	ecx
			jnz	Again
;----------------------
			lea	esi, MyTab

			mov	dx, 03c8h
			xor	ax, ax
			out	dx, al
			inc	dx
			mov	ecx, 256
Encore:
			outsb			; R
			outsb			; G
			outsb			; B

			dec	ecx
			jnz	Encore
;----------------------
			ret

Palette			endp

;----------------------------------------------------------------------------
;		Palette( (UBYTE*)PalColor )
;
PalMulti		proc	uses esi edi ebx,\
			colstart:DWORD, nbcol:DWORD, PalColor:DWORD
;----------------------
			mov	esi, PalColor
			lea	edi, MyTab

			mov	ecx, nbcol
;----------------------
Again:			mov	al, [esi]
			shr	al, 2
			mov	[edi], al
			mov	al, [esi+1]
			shr	al, 2
			mov	[edi+1], al
			mov	al, [esi+2]
			shr	al, 2
			mov	[edi+2], al
			add	esi, 3
			add	edi, 3
			dec	ecx
			jnz	Again
;----------------------
			lea	esi, MyTab	; Mytab + start col

			mov	dx, 03c8h
			mov	ax, word ptr[colstart]
			out	dx, al
			inc	dx
			mov	ecx, nbcol
Encore:
			outsb			; R
			outsb			; G
			outsb			; B

			dec	ecx
			jnz	Encore
;----------------------
			ret

PalMulti		endp
;----------------------------------------------------------------------------
;		PalOne( col, red, green, blue )
;                       ax	col
;			bx	red
;			cx, green
;			dx, blue
;
PalOne			proc	uses ebx,\
			col:DWORD, red:DWORD, green:DWORD, blue:DWORD
;----------------------
			mov	eax, col
			mov	ebx, red
			mov	ecx, green
			mov	edx, blue

			shr 	bl, 2			; Red >> 2
			shr	cl, 2			; Green >> 2
			shr	dl, 2			; Blue >> 2
			mov	ch, dl			; Save Blue

			mov	dx, 03c8h
			out	dx, al			; al=Num Color
			inc	dx
			mov	al, bl
			out	dx, al			; Red
			mov	al, cl
			out	dx, al			; Green
			mov	al, ch
			out	dx, al			; Blue
;----------------------
			ret
PalOne			endp
;----------------------------------------------------------------------------
;				The
				End
