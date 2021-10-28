;----------------------------------------------------------------------------
;				 S_FILLV.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
				.model FLAT, SYSCALL
				.data

			include	\projet\lib386\lib_svga\svga.ash

			extrn	NoLanguage	TabVerticG	:WORD
			extrn	NoLanguage	TabCoulG	:WORD
			extrn	NoLanguage	Ymin		:WORD
			extrn	NoLanguage	Ymax		:WORD

ALIGN 4

Save1		dd	0000
newloop		dd	0000

MAX_TYPE_POLY	equ	9

TabJumpPoly	dd	SVGAPolyTriste ;S
		dd	SVGAPolyTele
		dd	SVGAPolyCopper
		dd	SVGAPolyBopper
		dd	SVGAPolyMarbre
		dd	SVGAPolyTrans
		dd	SVGAPolyTrame
		dd	SVGAPolyGouraud
		dd	SVGAPolyDith

TabPoly_0	dd	SVGAPolyTriste ;S
		dd	SVGAPolyTriste ;S
		dd	SVGAPolyCopper
		dd	SVGAPolyBopper
		dd	SVGAPolyMarbre
		dd	SVGAPolyTrans
		dd	SVGAPolyTrame
		dd	SVGAPolyTriche
		dd	SVGAPolyTriche

TabPoly_1	dd	SVGAPolyTriste ;S
		dd	SVGAPolyTele
		dd	SVGAPolyCopper
		dd	SVGAPolyBopper
		dd	SVGAPolyMarbre
		dd	SVGAPolyTrans
		dd	SVGAPolyTrame
		dd	SVGAPolyGouraud
		dd	SVGAPolyGouraud

TabPoly_2	dd	SVGAPolyTriste ;S
		dd	SVGAPolyTele
		dd	SVGAPolyCopper
		dd	SVGAPolyBopper
		dd	SVGAPolyMarbre
		dd	SVGAPolyTrans
		dd	SVGAPolyTrame
		dd	SVGAPolyGouraud
		dd	SVGAPolyDith

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	FillVertic
			public	NoLanguage	FillVertic_A
			public	NoLanguage	SetFillDetails

;----------------------------------------------------------------------------


;*══════════════════════════════════════════════════════════════════════════*

FillVertic		proc	uses	esi edi ebx ebp,\
			typepoly:DWORD, coulpoly:DWORD

			mov	ecx, typepoly
			mov	edi, coulpoly

			call	FillVertic_A

			ret
FillVertic		endp

;*══════════════════════════════════════════════════════════════════════════*

SetFillDetails	proc	uses esi edi,\
			level:DWORD

		mov	eax, level
		cmp	eax, 2
		jbe	ok
		mov	eax, 2
ok:		imul	eax, MAX_TYPE_POLY*4
		mov     edi, offset TabJumpPoly
		mov	esi, offset TabPoly_0
		add	esi, eax
		mov	ecx, MAX_TYPE_POLY
		rep	movsd
		ret

SetFillDetails	endp

;*══════════════════════════════════════════════════════════════════════════*
		; cx type polygone
		; di coul polygone

FillVertic_A	proc

		xor	eax, eax
		xor	edx, edx
		mov	ax, [Ymin]
		mov	dx, [Ymax]

		mov	ebp, [Log]
		add	ebp, TabOffLine[eax*4]

		lea	esi, TabVerticG[eax*2]
		lea	ebx, TabCoulG[eax*2]

		sub	edx, eax		; delta Y
		inc	edx

		mov	eax, edi		; coul

		; EAX coul(s)
		; EBP debut 1 ere ligne
		; EDX delta Y
		; ESI TabVerticG
		; EBX TabCoulG

		jmp	dword ptr TabJumpPoly[ecx*4]

FillVertic_A	endp

;*══════════════════════════════════════════════════════════════════════════*
;		      ▀▀█▀▀ █▀▀▀█  █    ██▀▀▀ ▀▀█▀▀ █▀▀▀▀
;			██  ██▀█▀  ██   ▀▀▀▀█   ██  ██▀▀
;			▀▀  ▀▀  ▀  ▀▀   ▀▀▀▀▀   ▀▀  ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

comment	@

SVGAPolyTristeS	proc

		; al coul

		mov	bl, al

blig:		xor	ecx, ecx
		xor	eax, eax
		mov	cx, word ptr[esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; XG

		sub	ecx, eax		; cx = DeltaX
		jc	nextlig
		inc	ecx

		mov	edi, ebp		; Debut Ligne ecran
		add	edi, eax		; Debut Line

		mov	al, bl
		rep	stosb

nextlig:	add	ebp, 640
		dec	dx
		jnz	blig

		ret

SVGAPolyTristeS	endp

	@

SVGAPolyTriste	proc

		; al coul

		mov	ah, al
		mov	bx, ax
		shl	eax, 16
		mov	ax, bx

		xor	ecx, ecx
		xor	ebx, ebx

blig:		mov	cx, word ptr[esi+960]	; xD
		mov	bx, word ptr[esi]	; XG
		add	esi, 2

		sub	cx, bx			; cx = DeltaX
		jc	nextlig
		inc	cx

		lea	edi, [ebp+ebx]		; Debut Ligne ecran + Debut Line

		mov	bl, cl
		shr	ecx, 2
		rep	stosd

		mov	cl, bl
		and	cl, 3
		rep	stosb

nextlig:	add	ebp, 640
		dec	edx
		jnz	blig

		ret

SVGAPolyTriste	endp

;*══════════════════════════════════════════════════════════════════════════*
;		     █▀▀▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀█ █▀▀▀▀ █▀▀▀█
;		     ██    ██  █ ██▀▀▀ ██▀▀▀ ██▀▀  ██▀█▀
;		     ▀▀▀▀▀ ▀▀▀▀▀ ▀▀    ▀▀    ▀▀▀▀▀ ▀▀  ▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyCopper	proc

		; AL couleur

		mov	bl,al			; coul
		xor	ecx, ecx

blig:		xor	eax, eax
		mov	cx, [esi+960]		; xD
		mov	ax, [esi]		; xG
		add	esi, 2

		sub	cx, ax			; cx = DeltaX
		jc	sensdown
		inc	cx

		lea	edi, [ebp+eax]		; Debut Ligne ecran + Debut Line

		mov	bh, bl
		mov	ax, bx
		shl	eax, 16
		mov	ax, bx

		test	edi, 1
		jz	aligne
		mov	byte ptr[edi], al
		inc	edi
		dec	ecx
aligne:         mov	bh, cl
		shr	ecx, 2			; By Word
		rep	stosd
		mov	cl, bh
		and	cl, 2
		rep	stosb

		inc	bl
		test	bl, 15
		jz	sensup

sensdown:	add	ebp, 640
		dec	edx
		jnz	blig

		ret

;------------------------------------------------

blig2:		xor	eax, eax
		mov	cx, [esi+960]		; xD
		mov	ax, [esi]		; xG
		add	esi, 2

		sub	cx, ax			; cx = DeltaX
		jc	sensup
		inc	cx

		lea	edi, [ebp+eax]		; Debut Ligne ecran + Debut Line

		mov	bh, bl
		mov	ax, bx
		shl	eax, 16
		mov	ax, bx

		test	edi,1
		jz	aligne1
		mov	byte ptr[edi], al
		inc	edi
		dec	ecx
aligne1:        mov	bh, cl
		shr	ecx, 2			; By Word
		rep	stosd
		mov	cl, bh
		and	cl, 2
		rep	stosb

sensup:		dec	bl
		test	bl, 15
		jz	sensdown

		add	ebp, 640
		dec	edx
		jnz	blig2

fin_poly:	ret

SVGAPolyCopper	endp

;*══════════════════════════════════════════════════════════════════════════*
;	  █▀▀█   █    █▀▀▀▀       █▀▀▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀█ █▀▀▀▀ █▀▀▀█
;	  ██▀▀█  ██   ██ ▀█       ██    ██  █ ██▀▀▀ ██▀▀▀ ██▀▀  ██▀█▀
;	  ▀▀▀▀▀  ▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀    ▀▀    ▀▀▀▀▀ ▀▀  ▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyBopper	proc

		mov	bl, al
		mov	bh, 2

blig:		xor	ecx, ecx
		xor	eax, eax
		mov	cx,word ptr[esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; xG

		sub	ecx, eax		; cx = DeltaX
		jc	sensdown
		inc	ecx

		mov	edi, ebp		; Debut Ligne ecran
		add	edi, eax		; Debut Line

		mov	al, bl
		mov	ah, bl

		test	edi, 1
		jz	aligne
		mov	byte ptr[edi], al
		inc	edi
		dec	ecx
aligne:		shr	ecx, 1			; By Word
		rep	stosw
		rcl	ecx, 1
		rep	stosb

		dec	bh
		jnz	sensdown
		mov	bh, 2
		inc	bl
		test	bl, 15
		jz	sensup

sensdown:	add	ebp, 640
		dec	edx
		jnz	blig

		ret

;-----------------------------------------------

blig2:		xor	ecx, ecx
		xor	eax, eax
		mov	cx,word ptr[esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; xG

		sub	ecx, eax		; cx = DeltaX
		jc	sensup
		inc	ecx

		mov	edi, ebp		; Debut Ligne ecran
		add	edi, eax		; Debut Line

		mov	al, bl
		mov	ah, bl

		test	edi, 1
		jz	aligne1
		mov	byte ptr[edi], al
		inc	edi
		dec	ecx
aligne1:	shr	ecx, 1			; By Word
		rep	stosw
		rcl	ecx, 1
		rep	stosb

		dec	bh
		jnz	plusloin
sensup:		mov	bh, 2
		dec	bl
		test	bl, 15
		jz	sensdown

plusloin:	add	ebp, 640
		dec	edx
		jnz	blig2

fin_poly:	ret

SVGAPolyBopper	endp

;*══════════════════════════════════════════════════════════════════════════*
;		      █▄ ▄█ █▀▀▀█ █▀▀▀█ █▀▀█  █▀▀▀█ █▀▀▀▀
;		      ██▀ █ ██▀▀█ ██▀█▀ ██▀▀█ ██▀█▀ ██▀▀
;		      ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyMarbre	proc

		; ax 2 couls: al start ah end

		mov	bx, ax
		shl	ebx, 16

		mov	[newloop], edx		; delta Y
blig0:
;		push	ecx

		xor	ecx, ecx
		xor	eax, eax
		mov	cx, word ptr[esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; xG

		sub	ecx, eax
		jz	justone
		jc	nextlig
		inc	ecx			; CX deltax

		shld	edi, ebx, 16
		mov	bx, di

		mov	edi, ebp
		add	edi, eax		; offset lig + x min

		mov	ah, bh			; AH = intensite 2
		shl	bx, 8			; BH = intensite 1 et BL = 0
		mov	al, bl			; Maz AL

		sub	ax, bx			; delta intensite

		inc	ax
		cwd
		idiv	cx			; AX step intensite

		xchg	al, ah
		mov	dx, ax			; DX step intensite

		mov	al, bh			; AL intensite start

		inc	ecx
		shr	ecx, 1
		jnc	xloop1
		clc				; init Carry
	EVEN
xloop:		mov	byte ptr[edi], al
		inc	edi
		adc	ax, dx			; cumul step
xloop1:		mov	byte ptr[edi], al
		inc	edi
		adc	ax, dx			; cumul step
		dec	ecx
		jnz	xloop

nextlig:
;		pop	ecx
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0

		ret

justone:	mov	edi, ebp
		add	edi, eax		; offset lig + x min
		mov	eax, ebx
		shr	eax, 16+8
		mov	byte ptr[edi], al
		inc	edi
;		pop	ecx
		add	ebp, 640

		dec	dword ptr[newloop]
		jnz	blig0

		ret

SVGAPolyMarbre	endp

;*══════════════════════════════════════════════════════════════════════════*
;		      █▀▀▀█  █    █▀▀▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀▀
;		      ██▀▀▀  ██   ██▀▀  ██▀█▀ ██▀█▀ ██▀▀
;		      ▀▀     ▀▀   ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyTele	proc

		mov	[newloop], edx		; ecx = delta
		mov	dl, al			; DL = couleur
		mov	dh, 3			; Pour AND
		mov	bx, 17371

	EVEN
blig:
;		push	ecx
		xor	ecx, ecx
		xor	eax, eax
		mov	cx, word ptr[esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; xG

		sub	ecx, eax		; cx = DeltaX
		jc	nextlig
		inc	ecx

		mov	edi, ebp	 	; Debut Ligne ecran
		add	edi, eax		; Debut Line

		inc	ecx
		shr	ecx, 1
		jnc	bcol1
	EVEN
bcol:           add	ax, bx
		and	al, dh
		add	al, dl
		mov	byte ptr[edi], al
		inc	edi
		rol	bx, 2
		inc	bx
bcol1:          add	ax, bx
		and	al, dh
		add	al, dl
		mov	byte ptr[edi], al
		inc	edi
		rol	bx, 2
		inc	bx
		dec	ecx
		jnz	bcol

nextlig:	add	ebp, 640		; lig++
;		pop	ecx
		dec	dword ptr[newloop]
		jnz	blig

		ret

SVGAPolyTele	endp

;*══════════════════════════════════════════════════════════════════════════*
;       ▀▀█▀▀ █▀▀▀█ █▀▀▀█ ██▄ █ ██▀▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀█ █▀▀▀▀ ██▄ █ ▀▀█▀▀
;         ██  ██▀█▀ ██▀▀█ ██▀██ ▀▀▀▀█ ██▀▀▀ ██▀▀█ ██▀█▀ ██▀▀  ██▀██   ██
;	  ▀▀  ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀    ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀   ▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyTrans	proc

		and	al, 0F0h		; AL = couleur & 0xF0
		mov	bl, al			; BL BH = newcouleur
		mov	bh, al
		xor	eax, eax

blig:		mov	cx, word ptr[esi+960]	; xD
		mov	ax, [esi]		; xG
		add	esi, 2

		sub	cx, ax			; cx = DeltaX
		jc	boul			; neg => next line
		inc	cx

		lea	edi, [ebp+eax]		; Debut Ligne ecran + Debut Line

		shr	cx, 1
		jnc	bcol

		mov	al, [edi]
		and	al, 0Fh
		or	al, bl
		mov	[edi], al

		or	cx, cx			; anymore ?
		jz	boul

		inc	edi

bcol:           mov	ax, [edi]
		and	ax, 0F0Fh
		or	ax, bx
		mov	[edi], ax
		add	edi, 2
		dec	cx
		jnz	bcol
boul:
		add	ebp, 640		; lig++
		dec	edx
		jnz	blig

		ret

SVGAPolyTrans	endp

;*══════════════════════════════════════════════════════════════════════════*
;			 ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █▄ ▄█ █▀▀▀▀
;			   ██  ██▀█▀ ██▀▀█ ██▀ █ ██▀▀
;			   ▀▀  ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*/
;*──────────────────────────────────────────────────────────────────────────*/

SVGAPolyTrame	proc

		mov	bl, al			; BL = couleur
		xor	bh, bh
		xor	eax, eax
	EVEN
blig:		mov	cx, word ptr[esi+960]	; xD
		mov	ax, [esi]		; xG
		add	esi, 2

		sub	cx, ax			; cx = DeltaX
		jc	nextlig
		inc	cx

		lea	edi, [ebp+eax]		; Debut Ligne ecran + Debut Line

		shr	cx, 1			; /2
		jz	nextlig

		xor	bh, 1			; paire/impair
		mov	ax, di
		and	ax, 1
		xor	al, bh
		jz	paire
		inc	edi
paire:
		mov	al, bl

		inc	cx
		shr	cx, 1
		jnc	bcol1
	EVEN
bcol:		mov	byte ptr[edi], al
		add	edi, 2
bcol1:		mov	byte ptr[edi], al
		add	edi, 2
		dec	cx
		jnz	bcol

nextlig:	add	ebp, 640		; lig++
		dec	edx
		jnz	blig

		ret

SVGAPolyTrame	endp


;*══════════════════════════════════════════════════════════════════════════*
;                   █▀▀▀▀ █▀▀▀█ █   █ █▀▀▀█ █▀▀▀█ █   █ █▀▀▀▄
;                   ██ ▀█ ██  █ ██  █ ██▀█▀ ██▀▀█ ██  █ ██  █
;                   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyTriche	proc

		xor	ecx, ecx
		xor	eax, eax

blig:		mov	cx, [esi+960]		; xD
		mov	ax, [esi]		; XG
		add	esi, 2

		sub	cx, ax			; cx = DeltaX
		jc	nextlig
		inc	ecx

		lea	edi, [ebp+eax]		; Debut Ligne ecran + Debut Line

		mov	al, [ebx+1]		; intensite 1 *256: F.f
		add	ebx, 2

		rep	stosb

nextlig:	add	ebp, 640
		dec	dx
		jnz	blig

		ret

SVGAPolyTriche	endp

;*──────────────────────────────────────────────────────────────────────────*

comment	@

SVGAPolyGouraudS	proc

		mov	[newloop], edx		; delta Y
	EVEN
blig0:
;		push	ecx

		xor	ecx, ecx
		xor	eax, eax
		mov	cx, word ptr [esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; xG

		mov	edi, ebp		; offset lig + x min
		add	edi, eax		; col start

		sub	ecx, eax		; ECX delta x
		jz	l0
		jc	nextlig

		mov	ax, [ebx+960]		; intensite 2 *256: F.f
		mov	dx, [ebx]		; intensite 1 *256: F.f
		sub	ax, dx			; delta intensite
		jc      GouraudInverse

gouraud:	xor	dx, dx
		div	cx			; AX step intensite

		xchg	dx, ax			; DX step intensite, AX reste
		xchg	dl, dh			; inverse pour addition
		shr	ax, 1
		add	ax, [ebx]		; intensite 1 start + reste
		xchg	al, ah			; inverse pour coul dans AL
		inc	ebx
		inc	ebx

		inc	ecx
		add	ah, dh			; init Carry
	EVEN
xloop:		mov	byte ptr[edi], al
		inc	edi
		adc	ax, dx			; cumul step
		dec	ecx
		jnz	xloop

nextlig:
;		pop	ecx
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0

		ret

GouraudNormal:	neg	ax
		jmp	short gouraud

l0:		mov	al, [ebx+961]
		add	ebx, 2
		mov	byte ptr[edi], al
		inc	edi
;		pop	ecx
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0
		ret

GouraudInverse:	neg	ax
		jmp	short igouraud

il0:		mov	al, [ebx+961]
		add	ebx, 2
		mov	byte ptr[edi], al
		inc	edi
;		pop	ecx
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	iblig0
		ret

	EVEN
iblig0:         ; push	ecx

		xor	ecx, ecx
		xor	eax, eax
		mov	cx, word ptr [esi+960]	; xD
		mov	ax, [esi]
		add	esi, 2			; xG

		mov	edi, ebp		; offset lig + x min
		add	edi, eax		; col start

		sub	ecx, eax		; ECX delta x
		jz      il0
		jc	inextlig

		mov	dx, [ebx+960]		; intensite 2 *256: F.f
		mov	ax, [ebx]		; intensite 1 *256: F.f
		sub	ax, dx			; delta intensite
		jc      GouraudNormal
igouraud:	xor	dx, dx
		div	cx			; AX step intensite

		xchg	dx, ax			; DX step intensite, AX reste
		xchg	dl, dh			; inverse pour addition
		shr	ax, 1                   ;
		neg	ax			; - reste / 2
		add	ax, [ebx]		; intensite 1 start + reste
		xchg	al, ah			; inverse pour coul dans AL
		add	ebx, 2

		inc	ecx
		sub	ah, dh			; init Carry
	EVEN
ixloop:		mov	byte ptr[edi], al
		inc	edi
		sbb	ax, dx			; cumul step
		dec	ecx
		jnz	ixloop

inextlig:	; pop	ecx
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	iblig0

		ret

SVGAPolyGouraudS	endp

	@

;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyGouraud	proc

		mov	[newloop], edx		; delta Y
		xor	eax, eax
		xor	ecx, ecx
		xor	edx, edx

blig0:		mov	cx, [esi+960]		; xD
		mov	ax, [esi]		; xG
		add	esi, 2

		lea	edi, [ebp+eax]		; offset lig + x min + col start

		sub	cx, ax			; CX delta x
		mov	ax, [ebx+960]		; intensite 2 *256: F.f
		mov	dx, [ebx]		; intensite 1 *256: F.f
		jz	l0
		jc	nextlig

		sub	ax, dx
		jc      GouraudInverse		; delta intensite

		cmp	cx, 2
		jbe	opt

gouraud:	xor	dx, dx
		div	cx			; AX step intensite

		mov	dx, [ebx]		; intensite 1 start
		add	ebx, 2

		inc	cx
		shr	cx, 1			; divise par 2
		jnc	start			; impair ?
		mov	byte ptr[edi], dh
		jz	nextlig
		inc	edi
		jmp	start2
	ALIGN 4
xloop:		add	edi, 2
start2:		add	dx, ax			; cumul step
start:		mov	[edi], dh
		add	dx, ax			; cumul step
		mov	[edi+1], dh
		dec	cx
		jnz	xloop

nextlig:	add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0

		ret

GouraudNormal:  cmp	cx, 2
		jbe	opt0
		neg	ax
		jmp	short gouraud

opt0:		mov	dx, [ebx]
opt:		mov	ax, [ebx+960]
		add	ebx, 2
		mov	[edi+ecx], ah
		dec	cx
		jz	opt2
		add	eax, edx
		shr	eax, 1
		mov	[edi+1], ah
opt2:		mov	[edi], dh
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0

		ret

l0:		add	eax, edx
		add	ebx, 2
		shr	eax, 1
		add	ebp, 640
		mov	[edi], ah
		dec	dword ptr[newloop]
		jnz	blig0

		ret

GouraudInverse: cmp	cx, 2
		jbe	iopt0
		neg	ax
		jmp	short igouraud

iopt0:		mov	dx, [ebx+960]
iopt:		mov	ax, [ebx]
		add	ebx, 2
		mov	[edi+ecx], dh
		dec	cx
		jz	iopt2
		add	edx, eax
		shr	edx, 1
		mov	[edi+1], dh
iopt2:		mov	[edi], ah
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	iblig0

		ret

il0:		add	eax, edx
		add	ebx, 2
		shr	eax, 1
		add	ebp, 640
		mov	[edi], ah
		dec	dword ptr[newloop]
		jnz	iblig0

		ret

	ALIGN 4
iblig0:
		mov	cx, [esi+960]		; xD
		mov	ax, [esi]		; xG
		add	esi, 2

		lea	edi, [ebp+eax]		; offset lig + x min + col start

		sub	cx, ax			; CX delta x
		mov	ax, [ebx]		; intensite 1 *256: F.f
		mov	dx, [ebx+960]		; intensite 2 *256: F.f
		jz      il0
		jc	inextlig

		sub	ax, dx
		jc      GouraudNormal		; delta intensite

		cmp	cx, 2
		jbe	iopt

igouraud:	xor	dx, dx
		div	cx			; AX step intensite

		mov	dx, [ebx]		; intensite 1 start
		add	ebx, 2

		inc	cx
		shr	cx, 1			; divise par 2
		jnc    	istart			; impair ?
		mov	byte ptr[edi], dh
		jz	nextlig
		inc	edi
		jmp	istart2
	ALIGN 4
ixloop:		add	edi, 2
istart2:	sub	dx, ax			; cumul step
istart:		mov	[edi], dh
		sub	dx, ax			; cumul step
		mov	[edi+1], dh
		dec	cx
		jnz	ixloop

inextlig:	add	ebp, 640
		dec	dword ptr[newloop]
		jnz	iblig0

		ret

SVGAPolyGouraud	endp

;*══════════════════════════════════════════════════════════════════════════*
;             █▀▀▀▄  █    ▀▀█▀▀ █   █ █▀▀▀▀ █▀▀▀█  █    ██▄ █ █▀▀▀▀
;             ██  █  ██     ██  ██▀▀█ ██▀▀  ██▀█▀  ██   ██▀██ ██ ▀█
;             ▀▀▀▀   ▀▀     ▀▀  ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

SVGAPolyDith	proc

		mov	[newloop], edx		; delta Y
		xor	eax, eax

blig0:          mov	cx, word ptr [esi+960]	; xD
		mov	ax, [esi]		; xG
		add	esi, 2


		sub	cx, ax			; cx = delta X
		jc	nextlig

		lea	edi, [ebp + eax]	; offset lig + x min + col start
		mov	[Save1], esi
		mov	ax, [ebx+960]		; intensite 2
		mov	si, [ebx]		; intensite 1

		jz      cx0

		add	ebx, 2
		sub	ax, si			; delta intensite

		cmp	cx, 2
		jbe	opt

		cwd
		idiv	cx			; AX step intensite

		mov	dx, si

		inc	cx
		shr	cx, 1
		jnc	start

		xor	dh, dh
		rol	dl, cl
		add	dx, si
		or	cx, cx
		mov	[edi], dh
		jz	endloop
		inc	edi
		jmp	start2

	ALIGN 4
xloop:		add	edi, 2
start2:		add	si, ax
start:		xor	dh, dh
		add	dx, si
		mov	[edi], dh		; start
		xor	dh, dh
		add	si, ax
		rol	dl, cl
		add	dx, si
		dec	cx
		mov	[edi+1], dh
		jnz	xloop
endloop:
		mov	esi, [Save1]
nextlig:
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0

		ret

cx0:		add	ax, si
		mov	esi, [Save1]
		rcr	ax, 1
		add	ebx, 2
		mov	[edi], ah
		add	ebp, 640
		dec	dword ptr[newloop]
		jnz	blig0

		ret

opt:		inc	cx
		mov	dx, si
		shr	cx, 1
		jnc	two

		xor	dh, dh
		sar	ax, 1
		rol	dl, cl
		inc	edi
		add	dx, si
		mov	[edi-1], dh
		add	si, ax
two:
		xor	dh, dh
		add	dx, si
		mov	[edi], dh
		xor	dh, dh
		add	si, ax
		rol	dl, cl
		add	ebp, 640
		add	dx, si
		mov	esi, [Save1]
		mov	[edi+1], dh
		dec	dword ptr[newloop]
		jnz	blig0

		ret

SVGAPolyDith	endp

;*──────────────────────────────────────────────────────────────────────────*
;				The
				End





