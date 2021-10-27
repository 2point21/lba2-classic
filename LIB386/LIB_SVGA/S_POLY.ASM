;*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*
;				S_POLY.ASM 386
;			      (c) Adeline 1993
;*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*
		.486p
		jumps
		.model SMALL, SYSCALL

		.data

		include	svga.ash

		public	 NoLanguage	TabPoly
		public	 NoLanguage	TabVerticD
		public	 NoLanguage	TabVerticG
		public	 NoLanguage	TabCoulG
		public	 NoLanguage	TabCoulD
		public	 NoLanguage	TabGauche
		public	 NoLanguage	TabDroite
		public	 NoLanguage	TabX0
		public	 NoLanguage	TabY0
		public	 NoLanguage	TabX1
		public	 NoLanguage	TabY1

		public	 NoLanguage	Xmin
		public	 NoLanguage	Ymin
		public	 NoLanguage	Xmax
		public	 NoLanguage	Ymax
		public	 NoLanguage	NbPolyPoints
		public	 NoLanguage	TypePoly

;-------------------------------------------------------------------------

EVEN

Xmin		dw	00		; encombrement polygone
Ymin		dw	00
Xmax		dw	00
Ymax		dw	00

TypePoly	dw	00

NbPolyPoints	dw	00		; 32 max ?

TabPoly         dw	1234h
		dw	32*3	dup(00)
TabPolyClip	dw	32*3	dup(00)

	; ne pas changer l'ordre de ce qui suit


TabGauche	label	WORD
TabVerticG	dw	480	dup(00)
TabDroite	label	WORD
TabVerticD	dw	480	dup(00)
TabX0		label	WORD
TabCoulG	dw	480	dup(00)
TabY0		label	WORD
TabCoulD	dw	480	dup(00)

TabX1		dw	480 dup(0)
TabY1		dw	480 dup(0)


;--------------------------------------------------------------------------

boucle		dw	00		; nb points
newboucle	dw	00
flagrencadre	dw	00

intensite_1	db	0
intensite_2	db	0
last_intensite	db	0
		db	0		; dummy

coul1		dw	0

ALIGN 4

offtabpoly	dd	offset	TabPoly
		dd	offset	TabPolyClip

;--------------------------------------------------------------------------
		.code

		public	 NoLanguage	ComputePoly
		public	 NoLanguage	ComputePoly_A
		public	 NoLanguage	ComputeSphere
		public	 NoLanguage	ComputeSphere_A

;*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
;                     ÛÛßßß ÛßßßÛ Û   Û Ûßßßß ÛßßßÛ Ûßßßß
;                     ßßßßÛ ÛÛßßß ÛÛßßÛ ÛÛßß  ÛÛßÛß ÛÛßß
;                     ßßßßß ßß    ßß  ß ßßßßß ßß  ß ßßßßß
;*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
;*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*

ComputeSphere	proc	uses esi edi ebp ebx,\
		pxc:DWORD, pyc:DWORD, rayon:DWORD

		mov	ecx, pxc
		mov	esi, pyc
		mov	ebp, rayon

		call	ComputeSphere_A

		ret

ComputeSphere	endp

;*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*

ComputeSphere_A	proc				; uses si di bp

		; EBP = rayon
		; ECX = x screen centre
		; ESI = y screen centre

		; xc	equ	ecx
		; yc	equ	esi

		; px	equ	edi
		; py	equ	edx
		; sum	equ	ebp

		mov	edi, ebp
		xor	edx, edx
		neg	ebp

		xor	bx, bx

		mov	eax, esi
		sub	eax, edi			; esi - rayon
		cmp	eax, [ClipYmax]
		jg	nosphere
		cmp	eax, [ClipYmin]
		jge	csc0
		inc	bx
		mov	eax, [ClipYmin]
csc0:		mov	[Ymin], ax

		lea	eax, [esi+edi]			; esi + rayon
		cmp	eax, [ClipYmin]
		jl	nosphere
		cmp	eax, [ClipYmax]
		jle	csc1
		inc	bx
		mov	eax, [ClipYmax]
csc1:		mov	[Ymax], ax

		lea	eax, [ecx+edi]			; ecx + rayon
		cmp	eax, [ClipXmin]
		jl	nosphere
		cmp	eax, [ClipXmax]
		jle	csc2
		inc	bx

csc2:		mov	eax, ecx
		sub	eax, edi			; ecx - rayon
		cmp	eax, [ClipXmax]
		jg	nosphere

		cmp	eax, [ClipXmin]
		jge	csc3
		inc	bx

csc3:		or	bx, bx
		jnz	clipping

;------------------------------------------------------------------------
;        EVEN
	ALIGN 4
nccs0:		cmp	edx, edi			; while( edx <= edi )
		jg	fin

		lea	ebx, [esi+edx]
		lea	eax, [ecx+edi]
		mov	word ptr[ TabVerticD + ebx*2 ], ax
		mov	ax, cx
		sub	ax, di
		mov	word ptr[ TabVerticG + ebx*2 ], ax

		mov	ebx, esi
		sub	ebx, edx
		mov	word ptr[ TabVerticG + ebx*2 ], ax
		lea	eax, [ecx+edi]
		mov	word ptr[ TabVerticD + ebx*2 ], ax

		add	ebp, edx
		jnc	ncnoovf

		lea	ebx, [esi+edi]
		lea	eax, [ecx+edx]
		mov	word ptr[ TabVerticD + ebx*2 ], ax
		mov	ax, cx
		sub	ax, dx
		mov	word ptr[ TabVerticG + ebx*2 ], ax

		mov	ebx, esi
		sub	ebx, edi
		mov	word ptr[ TabVerticG + ebx*2 ], ax
		lea	eax, [ecx+edx]
		mov	word ptr[ TabVerticD + ebx*2 ], ax

		dec	edi
		sub	ebp, edi

ncnoovf:	inc	edx
		jmp	nccs0

fin:		mov	eax, 1
		ret

;------------------------------------------------------------------------
clipping:
;	EVEN
	ALIGN 4
cs0:		cmp	edx, edi			; while( edx <= edi )
		jg	finclip

		lea	ebx, [esi+edx]
		cmp	bx, [Ymax]
		jg	cs1
		cmp	bx, [Ymin]
		jl	cs1

		; gauche
		mov	eax, ecx
		sub	eax, edi
		cmp	eax, [ClipXmin]
		jl	nocxi
		cmp	eax, [ClipXmax]
		jle	nocxi2
		mov	ax, bx
		dec	ax
		mov	[Ymax], ax
		jmp	short cs1
nocxi:		mov	eax, [ClipXmin]
nocxi2:		mov	word ptr[ TabVerticG + ebx*2 ], ax

		; droite
		lea	eax, [ecx+edi]
		cmp	eax, [ClipXmax]
		jg	nocxa
		cmp	eax, [ClipXmin]
		jge	nocxa2
		mov	ax, bx
		dec	ax
		mov	[Ymax], ax
		jmp	short cs1
nocxa:		mov	eax, [ClipXmax]
nocxa2:		mov	word ptr[ TabVerticD + ebx*2 ], ax

cs1:		mov	ebx, esi
		sub	ebx, edx
		cmp	bx, [Ymax]
		jg	cs2
		cmp	bx, [Ymin]
		jl	cs2
		; gauche
		mov	eax, ecx
		sub	eax, edi
		cmp	eax, [ClipXmin]
		jl	nocxia
		cmp	eax, [ClipXmax]
		jle	nocxia2
		lea	eax, [ebx+1]
		mov	[Ymin], ax
		jmp	short cs2
nocxia:		mov	eax, [ClipXmin]
nocxia2:	mov	word ptr[ TabVerticG + ebx*2 ], ax
		; droite
		lea	eax, [ecx+edi]
		cmp	eax, [ClipXmax]
		jg	nocxaa
		cmp	eax, [ClipXmin]
		jge	nocxaa2
		lea	eax, [ebx+1]
		mov	[Ymin], ax
		jmp	short cs2
nocxaa:		mov	eax, [ClipXmax]
nocxaa2:	mov	word ptr[ TabVerticD + ebx*2 ], ax

cs2:		add	ebp, edx
		jnc	noovf

		lea	ebx, [esi+edi]
		cmp	bx, [Ymax]
		jg	cs3
		cmp	bx, [Ymin]
		jl	cs3
		; gauche
		mov	eax, ecx
		sub	eax, edx
		cmp	eax, [ClipXmin]
		jl	nocxib
		cmp	eax, [ClipXmax]
		jle	nocxib2
		mov	ax, bx
		dec	ax
		mov	[Ymax], ax
		jmp	short cs3
nocxib:		mov	eax, [ClipXmin]
nocxib2:	mov	word ptr[ TabVerticG + ebx*2 ], ax
		mov	word ptr[ TabVerticG + ebx*2 ], ax
		; droite
		lea	eax, [ecx+edx]
		cmp	eax, [ClipXmax]
		jg	nocxab
		cmp	eax, [ClipXmin]
		jge	nocxab2
		mov	ax, bx
		dec	ax
		mov	[Ymax], ax
		jmp	short cs3
nocxab:		mov	eax, [ClipXmax]
nocxab2:	mov	word ptr[ TabVerticD + ebx*2 ], ax

cs3:		mov	ebx, esi
		sub	ebx, edi
		cmp	bx, [Ymax]
		jg	cs4
		cmp	bx, [Ymin]
		jl	cs4
		; gauche
		mov	eax, ecx
		sub	eax, edx
		cmp	eax, [ClipXmin]
		jl	nocxic
		cmp	eax, [ClipXmax]
		jle	nocxic2
		lea	eax, [ebx+1]
		mov	[Ymin], ax
		jmp	short cs4
nocxic:		mov	eax, [ClipXmin]
nocxic2:	mov	word ptr[ TabVerticG + ebx*2 ], ax
		; droite
		lea	eax, [ecx+edx]
		cmp	eax, [ClipXmax]
		jg	nocxac
		cmp	eax, [ClipXmin]
		jge	nocxac2
		lea	eax, [ebx+1]
		mov	[Ymin], ax
		jmp	short cs4
nocxac:		mov	eax, [ClipXmax]
nocxac2:	mov	word ptr[ TabVerticD + ebx*2 ], ax

cs4:		dec	edi
		sub	ebp, edi

noovf:		inc	edx
		jmp     cs0

finclip:	mov	ax, word ptr[Ymin]
		cmp	ax, word ptr[Ymax]

; modif pas de sphere d'une ligne ???

		jge	nosphere

		mov	eax, 1			; ok sphere
		ret

nosphere:	xor	eax, eax
		ret

ComputeSphere_A	endp


;*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
;               ÛßßßÛ ÛßßßÛ Û     Û  Üß Ûßßßß ÛßßßÛ ÛÛÜ Û Ûßßßß
;               ÛÛßßß ÛÛ  Û ÛÛ    ÛÛß   ÛÛ ßÛ ÛÛ  Û ÛÛßÛÛ ÛÛßß
;		ßß    ßßßßß ßßßßß ßß    ßßßßß ßßßßß ßß  ß ßßßßß
;*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
;*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*

ComputePoly_A	proc	uses	edi ebx

	movzx	ecx, word ptr [NbPolyPoints]

	mov	esi, offset TabPoly

		mov	[offtabpoly  ], esi
		mov	[offtabpoly+4], offset TabPolyClip

		mov	[boucle], cx

		mov	word ptr [flagrencadre], 0

		mov	ax, 32767
		mov	bx, ax			; Xmin
		mov	bp, ax			; Ymin
		mov	ax, -32768
		mov	di, ax			; Ymax
		mov	dx, ax			; Xmax

		jmp	short encadre

tt0a:		mov	bx, ax
		jmp	short tt0
tt1a:		mov	dx, ax
		jmp	short tt1
tt2a:		mov	bp, ax
		jmp	short tt2
tt3a:		mov	di, ax
		jmp	short tt3

;	EVEN
	ALIGN 4
encadre:	add	esi, 2			; saute couleur

		lodsw
		cmp	ax, bx			; X
		jl	tt0a
tt0:		cmp	ax, dx
		jg	tt1a
tt1:
		lodsw				; Y
		cmp	ax, bp
		jl	tt2a
tt2:		cmp	ax, di
		jg	tt3a
tt3:
		dec	ecx
		jnz	encadre

		mov	cx, di
		mov	edi, esi
		mov	esi, [offtabpoly]	; offset TabPoly

		movsw
		movsd				; transitivit‚ der point

		mov	[Ymin], bp
		mov	[Xmax], dx
		mov	[Ymax], cx
		mov	[Xmin], bx

		cmp	cx, bp			; si Ymin > Ymax fin poly
		jl	fin_poly

;------------------------------------------------------------------------

CLIP:		mov	eax, [ClipXmin]
		cmp	[Xmin], ax
		jge	cl1			; clipped
		cmp	[Xmax], ax		; outside screen
		jl	fin_poly
		call	ClipGauche
		jz	fin_poly

cl1:		mov	eax, [ClipXmax]
		cmp	[Xmax], ax
		jle	cl2			; clipped
		cmp	[Xmin], ax		; outside screen
		jg	fin_poly
		call	ClipDroit
		jz	fin_poly

cl2:		mov	eax, [ClipYmin]
		cmp	[Ymin], ax
		jge	cl3			; clipped
		cmp	[Ymax], ax		; outside screen
		jl	fin_poly
		call	ClipHaut
		jz	fin_poly

cl3:		mov	eax, [ClipYmax]
		cmp	[Ymax], ax
		jle	cl4			; clipped
		cmp	[Ymin], ax		; outside screen
		jg	fin_poly
		call	ClipBas
		jz	fin_poly
cl4:

;------------------------------------------------------------------------

		cmp	word ptr[flagrencadre], 0
		jz	filltabvertic
		jmp	short rencadre

tr21a:		mov	bx, ax
		jmp	short tr21
tr22a:		mov	dx, ax
		jmp	short tr22

rencadre:	; cherche si nouveau Ymin ou Ymax

		mov	esi, [offtabpoly]	; TabPoly 1 ou 2

		movzx	ecx, [boucle]
		mov	bx, 32767		; Ymin
		mov	dx, bx
		inc	dx			; - 32768

;	EVEN
	ALIGN 4
tr20:           add	esi, 4			; saute coul et X
		lodsw				; read Y
		cmp	ax, bx			; Ymin
		jl	short tr21a		; new Ymin
tr21:		cmp	ax, dx			; Ymax
		jg	short tr22a		; new Ymax
tr22:		dec	ecx
		jnz	tr20

		mov	[Ymin], bx
		mov	[Ymax], dx

		cmp	bx,dx			; si Ymin > Ymax fin poly

; modif pas de poly d'une ligne (jg -> jge)

		jge	fin_poly		;

;------------------------------------------------------------------------

filltabvertic:	; draw les lines inter sommets

		mov	esi, [offtabpoly]	; TabPoly 1 ou 2

		mov	ax, [esi]			; couleur sommet
		mov	[intensite_2], al
		mov	[last_intensite], al

		mov	bx, [esi+2]
		mov	cx, [esi+4]
		add	esi, 6

		; BX = X0
		; CX = Y0

;	EVEN
	ALIGN 4
gtg0:		mov	al, [last_intensite]
		mov	[intensite_1], al

		mov	ax, [esi]			; voir pour prestock 2 couls
		mov	[intensite_2], al
		mov	[last_intensite], al

		mov	dx, [esi+2]		; X
		mov	ax, [esi+4]		; Y
		add	esi, 6

		; DX = X1
		; AX = Y1

		cmp	ax, cx			; mettre les 2 X dans TabVertic
		je	same_y			; FAUX

		push	dx
		push	ax

		jl	y_monte

;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú
; Y descend donc buffer gauche

		mov	bp, ax
		sub	bp, cx			; bp abs delta y

		cmp	bx, dx			; X0 < X1
		jle	gtg1			; oui c'est bon

		xchg	bx, dx			; remet X0 < X1
		xchg	ax, cx			; fait suivre Y0 et Y1
		mov	ax, word ptr[intensite_1] ; couls aussi
		xchg	al, ah
		mov	word ptr[intensite_1], ax
		std
gtg1:
		movzx	edi, cx			; CX lig start
		shl	edi, 1

		mov	cx, bp			; abs delta y

		mov	ebp, edi		; save lig start * 4
		add	edi, offset TabVerticG

		mov	ax, dx			; x0
		sub	ax, bx			; x1 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		xchg	edx, eax
		shr	ax, 1
		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16
		mov	ax, bx
		mov	bx, cx

		; EAX = .cumul:X

		; EDX = .DeltaX:DeltaX

		add	cx, 2
		shr	cx, 1
		jc	even0
		add	eax, edx		; init Carry
		rcl	cx, 1
		sub	ax, dx
		shr	cx, 1
		jmp	short lt01
even0:		add	eax, edx		; init Carry
		rcl	cx, 1
		sub	ax, dx
		shr	cx, 1
;	EVEN
	ALIGN 4
lt0:		stosw
		adc	eax, edx
lt01:		stosw
		adc	eax, edx
		dec	ecx
		jnz	lt0

; ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú
; fill tab coul G

		cmp	word ptr[ TypePoly ], POLY_GOURAUD
		jb	gtg3

		mov	cx, bx			; delta Y

		mov	edi, ebp		; lig start
		add	edi, offset TabCoulG

		mov	ax, word ptr[intensite_1];
		mov	bx, ax
		sub	ah, al			; delta intensite
		jc	ftg0
		xor	dx, dx
		mov	al, dl
		div	cx

		xchg	dx, ax			; DX step intensite
		shr	al, 1
		add	al, 7Fh
		mov	ah, bl			; AH intensite start

		add	cx, 2
		shr	cx, 1			; divise par 2
		jnc	xloopg1			; impair ?

	ALIGN 4
xloopg:		stosw
		add	ax, dx			; cumul step
xloopg1:	stosw
		add	ax, dx			; cumul step
		dec	ecx
		jnz	xloopg

gtg3:           cld
		pop	cx			; XY1 = XY2
		pop	bx

		dec	word ptr[boucle]
		jnz	gtg0

		mov	eax, 1			; ok poly drawed (perhaps clipped)
		ret


;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú

ftg0:
		neg	ah
		xor	dx, dx
		mov	al, dl
		div	cx

		xchg	dx, ax			; DX step intensite
		shr	al, 1
		neg	al
		add	al, 7fh
		mov	ah, bl			; AH intensite start


		add	cx, 2
		shr	cx, 1			; divise par 2
		jnc	xloopg01		; impair ?
;	EVEN
	ALIGN 4
xloopg0:	stosw
		sub	ax, dx			; cumul step
xloopg01:	stosw
		sub	ax, dx			; cumul step
		dec	ecx
		jnz	xloopg0

;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú

		cld
		pop	cx			; XY1 = XY2
		pop	bx

		dec	word ptr[boucle]
		jnz	gtg0

		mov	eax, 1			; ok poly drawed (perhaps clipped)
		ret


;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú

same_y:
		mov	bp, dx
;
;		cmp	bx, dx			; X0 <= X1
;		jl	gtg1s			; oui c'est bon
;		xchg	bx, dx			; remet X0 < X1
;		mov	ax, word ptr[intensite_1] ; couls aussi
;		xchg	al, ah
;		mov	word ptr[intensite_1], ax
;gtg1s:
;		movzx	edi, cx
;		mov	word ptr[edi*2+TabVerticG], bx
;		mov	word ptr[edi*2+TabVerticD], dx
;
;		xor	al, al
;		mov	ah, [intensite_1]
;
;		mov	word ptr[edi+TabCoulG], ax
;
;		mov	ah, [intensite_1+1]
;		mov	word ptr[edi+TabCoulD], ax
;
;		cld
		mov	bx, bp

		dec	word ptr[boucle]
		jnz	gtg0

		mov	eax, 1			; ok poly drawed (perhaps clipped)
		ret

;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú
; Y monte donc buffer droit

y_monte:	mov	bp, cx
		sub	bp, ax			; bp abs delta y
		std

		cmp	bx, dx			; X0 >= X1
		jge	gtg1m			; oui c'est bon

		xchg	bx, dx			; remet X0 < X1
		xchg	ax, cx			; fait suivre Y0 et Y1
		mov	ax, word ptr[intensite_1] ; couls aussi
		xchg	al, ah
		mov	word ptr[intensite_1], ax
		cld
gtg1m:
		movzx	edi, cx
		shl	edi, 1

		mov	cx, bp			; abs delta y

		mov	ebp, edi		; save CX lig start * 2
		add	edi, offset TabVerticD

		mov	ax, bx		; dx	; x0
		sub	ax, dx		; bx	; x1 delta X

		shl	eax, 16
		xor	edx, edx
		div	ecx

		xchg	edx, eax
		shr	ax, 1
		neg	ax
		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16
		mov	ax, bx
		mov	bx, cx			; save delta Y

		add	cx, 2
		shr	cx, 1
		jc	even0m
		sub	eax, edx		; init Carry
		rcl	cx, 1
		add	ax, dx
		shr	cx, 1
		jmp	short lt01m
even0m:		sub	eax, edx		; init Carry
		rcl	cx, 1
		add	ax, dx
		shr	cx, 1
;	EVEN
	ALIGN 4
lt0m:		stosw				; stock X
		sbb	eax, edx		; add cumul
lt01m:		stosw				; stock X
		sbb	eax, edx		; add cumul
		dec	ecx
		jnz	lt0m

; ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú ú
; fill tab coul D

		cmp	word ptr[ TypePoly ], POLY_GOURAUD
		jb	short gtg3m

		mov	cx, bx			; delta Y

		mov	edi, ebp		; lig start
		add	edi, offset TabCoulD

		mov	ax, word ptr[intensite_1];
		mov	bx, ax
		sub	ah, al			; delta intensite
		jc	ftd0

		xor	dx, dx
		mov	al, dl
		div	cx			; CX deltay

		xchg	dx, ax			; DX step intensite
		shr	al, 1			; reste / 2
		add	al, 7Fh
		mov	ah, bl			; AH intensite start

		add	cx, 2
		shr	cx, 1			; divise par 2
		jnc	xloop1			; impair ?

	ALIGN 4
xloop:		stosw
		add	ax, dx			; cumul step
xloop1:		stosw
		add	ax, dx			; cumul step
		dec	ecx
		jnz	xloop

;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú
gtg3m:          cld
		pop	cx			; XY1 = XY2
		pop	bx

		dec	word ptr[boucle]
		jnz	gtg0

		mov	eax, 1			; ok poly drawed (perhaps clipped)
		ret

;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú

ftd0:
		neg	ah
		xor	dx, dx
		mov	al ,dl
		div	cx			; CX deltay

		xchg	dx, ax			; DX step intensite
		shr	al, 1			; reste / 2
		neg	al
		add	al, 7Fh
		mov	ah, bl			; AH intensite start

		add	cx, 2
		shr	cx, 1			; divise par 2
		jnc	xloopn1			; impair ?

	ALIGN 4
xloopn:		stosw
		sub	ax, dx			; cumul step
xloopn1:	stosw
		sub	ax, dx			; cumul step
		dec	ecx
		jnz	xloopn

;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú

		cld
		pop	cx			; XY1 = XY2
		pop	bx

		dec	word ptr[boucle]
		jnz	gtg0

		mov	eax, 1			; ok poly drawed (perhaps clipped)
		ret


;úúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúúú

fin_poly:	xor	eax, eax		; no_poly
		ret

ComputePoly_A	endp

;------------------------------------------------------------------------

ComputePoly	proc	uses	esi ebp

		mov	[offtabpoly], offset TabPoly
		call	ComputePoly_A
		ret

ComputePoly	endp

;------------------------------------------------------------------------
;------------------------------------------------------------------------
;------------------------------------------------------------------------

ClipGauche 	proc	near

		mov	word ptr[newboucle], 0
		mov	word ptr[flagrencadre], 1

		mov	esi, [offtabpoly]
		mov	edi, [offtabpoly+4]

		mov	bp, [esi]		; BP = C0
		mov	cx, [esi+2]		; CX = X0
		mov	bx, [esi+4]		; BX = Y0
		add	esi, 6
;	EVEN
	ALIGN 4
cfg:		mov	ax, [esi]		;      C1
		mov	[coul1], ax
		mov	ax, [esi+2]		; AX = X1
		mov	dx, [esi+4]		; DX = Y1
		add	esi, 6

		cmp	cx, word ptr[ClipXmin]	; test si X0 clipp‚
		jl	cfg0			; oui clipp‚

		mov	[edi], bp
		mov	[edi+2], cx		; pas clipp‚ on stock
		mov	[edi+4], bx
		add	edi, 6
		inc	word ptr[newboucle]	; 1 point de plus

		cmp	ax, word ptr[ClipXmin]	; test si X1 clipp‚
		jl      cfg1			; si oui on clip

cfg2:		mov	bp, [coul1]
		mov	ecx, eax		; XYC0 = XYC1
		mov	ebx, edx		; et on passe au suivant

		dec	word ptr[boucle]
		jnz	cfg
		jmp	cfg3			; fini

cfg0:		cmp	ax, word ptr[ClipXmin]	; test si X2 clipp‚
		jl	cfg2			; oui 1 et 2 clipp‚ on oublie le 1
						; 1er clipp‚ on ajuste coor
cfg1:           ; BP = C0
		; CX = X0		BX < DX
		; BX = Y0
		;      C1
		; DX = X1
		; AX = Y1

		push	eax
		push	edx
		push	[coul1]

		cmp	ax, cx			; Ajuste X0 X1 pour clip 2 poly coll‚s
		jl	cfg4
		xchg	eax, ecx
		xchg	edx, ebx
		xchg	bp, [coul1]
cfg4:
		sub	[coul1], bp		; coul1 = DeltaCoul
		shl	ebp, 16			; save coul0

		sub	edx, ebx		; DX = DeltaY  BX = Y0
		sub	eax, ecx		; AX = DeltaX
		mov	bp, word ptr[ClipXmin]
		sub	bp, cx			; BP = deltaClipX
		mov	ecx, eax		; CX = DeltaX
		mov	eax, ebp		; AX = deltaClipX
		imul	dx              	; AX = AX * deltaY
		idiv	cx			; AX = AX / deltaX
		add	ebx, eax		; Y0 = Y0 + AX

		mov	ax, word ptr[ClipXmin]	; X0
		mov	[edi+2], ax
		mov	[edi+4], bx

		cmp	word ptr[ TypePoly ], POLY_GOURAUD
		jb	cfg5

		mov	ax, [coul1]		; AX = DeltaCoul
		imul	bp			; AX = AX * DeltaClipX
		idiv    cx			; AX = AX / deltaX
		shr	ebp, 16			; BP = Coul0
		add	eax, ebp		; AX = AX + coul0

		mov	[edi], ax
cfg5:
		add	edi, 6
		inc	word ptr[newboucle]	; new point

		pop	bp			; POP C1 dans C0
		pop	ebx			; POP Y1 dans Y0
		pop	ecx			; POP X1 dans X0

		dec	word ptr[boucle]
		jnz	cfg

cfg3:		mov	eax, [offtabpoly]	; inversion pointeurs buffer
		mov	esi, [offtabpoly+4]
		mov	[offtabpoly+4], eax
		mov	[offtabpoly], esi

		movsw
		movsd				; transitivit‚ der point

		mov	ax, [newboucle]
		mov	[boucle], ax
		or	ax, ax
		ret

ClipGauche	endp

;------------------------------------------------------------------------

ClipDroit 	proc	near

		mov	word ptr[newboucle], 0
		mov	word ptr[flagrencadre], 1

		mov	esi, [offtabpoly]
		mov	edi, [offtabpoly+4]

		mov	bp, [esi]		; BP = C0
		mov	cx, [esi+2]		; CX = X0
		mov	bx, [esi+4]		; BX = Y0
		add	esi, 6
;	EVEN
	ALIGN 4
cfg:		mov	ax, [esi]		;      C1
		mov	[coul1], ax
		mov	ax, [esi+2]		; AX = X1
		mov	dx, [esi+4]		; DX = Y1
		add	esi, 6

		cmp	cx, word ptr[ClipXmax]	; test si X0 clipp‚
		jg	cfg0			; oui clipp‚

		mov	[edi], bp
		mov	[edi+2], cx		; pas clipp‚ on stock
		mov	[edi+4], bx
		add	edi, 6
		inc	word ptr[newboucle]	; 1 point de plus

		cmp	ax, word ptr[ClipXmax]	; test si X1 clipp‚
		jg      cfg1			; si oui on clip

cfg2:		mov	bp, [coul1]
		mov	ecx, eax		; XYC0 = XYC1
		mov	ebx, edx		; et on passe au suivant

		dec	word ptr[boucle]
		jnz	cfg
		jmp	cfg3			; fini

cfg0:		cmp	ax, word ptr[ClipXmax]	; test si X2 clipp‚
		jg	cfg2			; oui 1 et 2 clipp‚ on oublie le 1
						; 1er clipp‚ on ajuste coor
cfg1:           ; BP = C0
		; CX = X0		BX < DX
		; BX = Y0
		;      C1
		; DX = X1
		; AX = Y1

		push	eax
		push	edx
		push	[coul1]


		cmp	ax, cx			; Ajuste X0 X1 pour clip 2 poly coll‚s
		jl	cfg4
		xchg	eax, ecx
		xchg	edx, ebx
		xchg	bp, [coul1]
cfg4:
		sub	[coul1], bp		; coul1 = DeltaCoul
		shl	ebp, 16			; save coul0

		sub	edx, ebx		; DX = DeltaY  BX = Y0
		sub	eax, ecx		; AX = DeltaX
		mov	bp, word ptr[ClipXmax]
		sub	bp, cx			; BP = deltaClipX
		mov	ecx, eax		; CX = DeltaX
		mov	eax, ebp		; AX = deltaClipX
		imul	dx              	; AX = AX * deltaY
		idiv	cx			; AX = AX / deltaX
		add	ebx, eax		; Y0 = Y0 + AX

		mov	ax, word ptr[ClipXmax]	; X0
		mov	[edi+2], ax		; X0
		mov	[edi+4], bx		; Y0


		cmp	word ptr[ TypePoly ], POLY_GOURAUD
		jb	cfg5

		mov	ax, [coul1]		; AX = DeltaCoul
		imul	bp			; AX = AX * DeltaClipX
		idiv    cx			; AX = AX / deltaX
		shr	ebp, 16			; BP = Coul0
		add	eax, ebp		; AX = AX + coul0

		mov	[edi], ax
cfg5:
		add	edi, 6
		inc	word ptr[newboucle]	; new point

		pop	bp			; POP C1 dans C0
		pop	ebx			; POP Y1 dans Y0
		pop	ecx			; POP X1 dans X0

		dec	word ptr[boucle]
		jnz	cfg
cfg3:
		mov	eax, [offtabpoly]	; inversion pointeurs buffer
		mov	esi, [offtabpoly+4]
		mov	[offtabpoly+4], eax
		mov	[offtabpoly], esi

		movsw
		movsd				; transitivit‚ der point

		mov	ax, [newboucle]
		mov	[boucle], ax
		or	ax, ax
		ret


ClipDroit	endp

;------------------------------------------------------------------------

ClipHaut 	proc	near

		mov	word ptr[newboucle], 0
		mov	word ptr[flagrencadre], 1

		mov	esi, [offtabpoly]
		mov	edi, [offtabpoly+4]

		mov	bp, [esi]		; BP = C0
		mov	bx, [esi+2]		; BX = X0
		mov	cx, [esi+4]		; CX = Y0
		add	esi, 6
;	EVEN
	ALIGN 4
cfg:		mov	ax, [esi]		;      C1
		mov	[coul1], ax
		mov	dx, [esi+2]		; DX = X1
		mov	ax, [esi+4]		; AX = Y1
		add	esi, 6

		cmp	cx, word ptr[ClipYmin]	; test si Y0 clipp‚
		jl	cfg0			; oui clipp‚

		mov	[edi], bp
		mov	[edi+2], bx		; pas clipp‚ on stock
		mov	[edi+4], cx
		add	edi, 6
		inc	word ptr[newboucle]	; 1 point de plus

		cmp	ax, word ptr[ClipYmin]	; test si Y1 clipp‚
		jl      cfg1			; si oui on clip

cfg2:		mov	bp, [coul1]
		mov	ebx, edx		; XYC0 = XYC1
		mov	ecx, eax		; et on passe au suivant

		dec	word ptr[boucle]
		jnz	cfg
		jmp	cfg3			; fini

cfg0:		cmp	ax, word ptr[ClipYmin]	; test si Y2 clipp‚
		jl	cfg2			; oui 1 et 2 clipp‚ on oublie le 1
						; 1er clipp‚ on ajuste coor
cfg1:           ; BP = C0
		; BX = X0
		; CX = Y0
		;      C1
		; DX = X1
		; AX = Y1

		push	edx
		push	eax
		push	[coul1]

		cmp	ax, cx			; Ajuste Y0 Y1 pour clip 2 poly coll‚s
		jl	cfg4
		xchg	edx, ebx
		xchg	eax, ecx
		xchg	bp, [coul1]
cfg4:
		sub	[coul1], bp		; coul1 = DeltaCoul
		shl	ebp, 16			; save coul0

		sub	edx, ebx		; DX = DeltaX  BX = X0
		sub	eax, ecx		; AX = DeltaY
		mov	bp, word ptr[ClipYmin]
		sub	bp, cx			; BP = deltaClipY
		mov	ecx, eax		; CX = DeltaY
		mov	eax, ebp		; AX = deltaClipY
		imul	dx              	; AX = AX * deltaX
		idiv	cx			; AX = AX / deltaY
		add	ebx, eax		; X0 = X0 + AX

		mov	[edi+2], bx		; X0
		mov	ax, word ptr[ClipYmin]	; Y0
		mov	[edi+4], ax		; Y0

		cmp	word ptr[ TypePoly ], POLY_GOURAUD
		jb	cfg5

		mov	ax, [coul1]		; AX = DeltaCoul
		imul	bp			; AX = AX * DeltaClipY
		idiv    cx			; AX = AX / deltaY
		shr	ebp, 16			; BP = Coul0
		add	eax, ebp		; AX = AX + coul0

		mov	[edi], ax
cfg5:
		add	edi, 6
		inc	word ptr[newboucle]	; new point

		pop	bp			; POP C1 dans C0
		pop	ecx			; POP Y1 dans Y0
		pop	ebx			; POP X1 dans X0

		dec	word ptr[boucle]
		jnz	cfg

cfg3:		mov	eax, [offtabpoly]	; inversion pointeurs buffer
		mov	esi, [offtabpoly+4]
		mov	[offtabpoly+4], eax
		mov	[offtabpoly], esi

		movsw
		movsd				; transitivit‚ der point

		mov	ax, [newboucle]
		mov	[boucle], ax
		or	ax, ax
		ret

ClipHaut	endp

;------------------------------------------------------------------------

ClipBas 	proc	near

		mov	word ptr[newboucle], 0
		mov	word ptr[flagrencadre], 1

		mov	esi, [offtabpoly]
		mov	edi, [offtabpoly+4]

		mov	bp, [esi]		; BP = C0
		mov	bx, [esi+2]		; BX = X0
		mov	cx, [esi+4]		; CX = Y0
		add	esi, 6
;	EVEN
	ALIGN 4
cfg:		mov	ax, [esi]		;      C1
		mov	[coul1], ax
		mov	dx, [esi+2]		; DX = X1
		mov	ax, [esi+4]		; AX = Y1
		add	esi, 6

		cmp	cx, word ptr[ClipYmax]	; test si Y0 clipp‚
		jg	cfg0			; oui clipp‚

		mov	[edi], bp
		mov	[edi+2], bx		; pas clipp‚ on stock
		mov	[edi+4], cx
		add	edi, 6
		inc	word ptr[newboucle]	; 1 point de plus

		cmp	ax, word ptr[ClipYmax]	; test si Y1 clipp‚
		jg      cfg1			; si oui on clip

cfg2:		mov	bp, [coul1]
		mov	ebx, edx		; XYC0 = XYC1
		mov	ecx, eax		; et on passe au suivant

		dec	word ptr[boucle]
		jnz	cfg
		jmp	cfg3			; fini

cfg0:		cmp	ax, word ptr[ClipYmax]	; test si Y2 clipp‚
		jg	cfg2			; oui 1 et 2 clipp‚ on oublie le 1
						; 1er clipp‚ on ajuste coor
cfg1:           ; BP = C0
		; BX = X0
		; CX = Y0
		;      C1
		; DX = X1
		; AX = Y1

		push	edx
		push	eax
		push	[coul1]

		cmp	ax, cx			; Ajuste Y0 Y1 pour clip 2 poly coll‚s
		jl	cfg4
		xchg	edx, ebx
		xchg	eax, ecx
		xchg	bp, [coul1]
cfg4:
		sub	[coul1], bp		; coul1 = DeltaCoul
		shl	ebp, 16			; save coul0

		sub	edx, ebx		; DX = DeltaX  BX = X0
		sub	eax, ecx		; AX = DeltaY
		mov	bp, word ptr[ClipYmax]
		sub	bp, cx			; BP = deltaClipY
		mov	ecx, eax		; CX = DeltaY
		mov	eax, ebp		; AX = deltaClipY
		imul	dx              	; AX = AX * deltaX
		idiv	cx			; AX = AX / deltaY
		add	ebx, eax		; X0 = X0 + AX

		mov	[edi+2], bx		; X0
		mov	ax, word ptr[ClipYmax]	; Y0
		mov	[edi+4], ax		; Y0

		cmp	word ptr[ TypePoly ], POLY_GOURAUD
		jb	cfg5

		mov	ax, [coul1]		; AX = DeltaCoul
		imul	bp			; AX = AX * DeltaClipY
		idiv    cx			; AX = AX / deltaY
		shr	ebp, 16			; BP = Coul0
		add	eax, ebp		; AX = AX + coul0

		mov	[edi], ax
cfg5:
		add	edi, 6
		inc	word ptr[newboucle]	; new point

		pop	bp			; POP C1 dans C0
		pop	ecx			; POP Y1 dans Y0
		pop	ebx			; POP X1 dans X0

		dec	word ptr[boucle]
		jnz	cfg

cfg3:		mov	eax, [offtabpoly]	; inversion pointeurs buffer
		mov	esi, [offtabpoly+4]
		mov	[offtabpoly+4], eax
		mov	[offtabpoly], esi

		movsw
		movsd				; transitivit‚ der point

		mov	ax, [newboucle]
		mov	[boucle], ax
		or	ax, ax
		ret

ClipBas		endp

;------------------------------------------------------------------------

		END
