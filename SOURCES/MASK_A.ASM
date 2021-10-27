;----------------------------------------------------------------------------
;				 Mask_A.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------
			include	\projet\lib386\lib_svga\svga.ash
comment @
		DWORD	TabOffset[]
			Brick:
			    - BYTE Delta X
			    - BYTE Delta Y
				Line(Delta Y):
				- BYTE NbBlock
				   Block 0:	Nb Zero to Jump
				   Block 1:	Nb Zero to Write
				   Block 2:	Nb Zero to Jump
					etc...
@

BufferClip	db	512	dup(?)
OffsetBegin	dd	0
NbPix		dd	0

extrn		NoLanguage	Screen:DWORD

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CopyMask
			public	NoLanguage	AffMask
			public	NoLanguage	GetDxDyMask

;----------------------------------------------------------------------------
CopyMask		proc	uses esi edi ebx ebp,\
			nummask:DWORD, xmask:DWORD, ymask:DWORD, \
			bankmask:DWORD, ptsrc:DWORD

			mov	eax, nummask
			mov	ebx, xmask
			mov	ecx, ymask
			mov	esi, bankmask

			add	esi, [esi+eax*4]; ESI = Begin Data


		movzx	eax, byte ptr[esi+2]	; Hot X
		add	ebx, eax
		movzx	eax, byte ptr[esi+3]	; Hot Y
		add	ecx, eax


;-----------------------------------------------
			lodsb			; Delta X
			movzx	edx, al         ;
			lodsb			; Nb Line ( Delta Y )
			movzx	eax, al


		add	esi, 2		; Jump Hot X & Hot Y


;----------------------------------------------- Test Clipping
			add	edx, ebx
			add	eax, ecx
			dec	edx
			dec	eax
			cmp	ebx, ClipXmin
			jl	ClippingMask
			cmp	ecx, ClipYmin
			jl	ClippingMask
			cmp	edx, ClipXmax
			jg	ClippingMask
			cmp	eax, ClipYmax
			jg	ClippingMask
			sub	edx, ebx
			sub 	eax, ecx
			inc	edx
			inc	eax
;----------------------------------------------- Calcul Offset Ecran
			add	ebx, TabOffLine[ecx*4]

			mov	edi, Log
			add	edi, ebx

			mov	ebp, ptsrc
			add	ebp, ebx

			mov	bh, al		; BH = NbLine
			sub	edx, Screen_X	; EDX = Offset Screen
			neg	edx		; EDX = Screen_X-edx
			xor	ecx, ecx	; Maz Compteur
			mov	eax, ecx
;----------------------------------------------- Init NbBlock for this line
NextLine:		lodsb			; Nb Block for this line
			mov	bl, al		; BL = NbBlock
;----------------------------------------------- Manage One Line
SameLine:		lodsb           	; Nb Zero to Jump
			add	edi, eax	; Incrust on Log
			add	ebp, eax	; And on PtSrc

			dec	bl
			je	EndBlock

			lodsb			; Nb Zero to Write
			mov	cl, al

;	xor	al, al
;	rep	stosb

		xchg	esi, ebp
		mov	al, cl
		shr	cl, 2
		rep	movsd 		; Write Datas From PtSrc
		mov	cl, al
		and	cl, 11b
		rep	movsb
		xchg	esi, ebp


			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
EndBlock:
			add	edi, edx	; EDI += Offset Screen
			add	ebp, edx
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
;----------------------------------------------- Incrust
			ret
;-------------------------------------------------------------------- Clipping
;		Graph : ( EBX, ECX ) ( EDX, EAX )
ClippingMask:
			cmp	ebx, ClipXmax
			jg      EndMask
			cmp	ecx, ClipYmax
			jg	EndMask
			cmp	edx, ClipXmin
			jl	EndMask
			cmp	eax, ClipYmin
			jl	EndMask

			cmp	ecx, ClipYmin
			jge	PasHaut

;---------------------- Clipping Haut, Saute ClipYmin-ECX Line(s)

			sub	ecx, ClipYMin
			neg	ecx
			mov	ebp, eax
			xor	eax, eax

NextH:			lodsb
			add	esi, eax
			loop	NextH

			mov	ecx, ClipYMin
			mov	eax, ebp

;---------------------- Clipping Bas
PasHaut:		cmp	eax, ClipYmax
			jle	PasBas
			mov	eax, ClipYmax
;---------------------- Clipping Gauche
Pasbas:                 mov	OffsetBegin, 0
			cmp     ebx, ClipXmin
			jge	PasGauche

			mov	ebp, ClipXmin
			sub	ebp, ebx
			mov	OffsetBegin, ebp

;----------------------	Clipping Droit
PasGauche:
			mov	ebp, edx
			sub	ebp, ebx
			sub	ebp, OffsetBegin
			inc	ebp
			mov	NbPix, ebp

			cmp	edx, ClipXmax
			jle	PasDroit

			sub	edx, ClipXmax
			sub	NbPix, edx
		mov	edx, ClipXmax
;----------------------









PasDroit:	; ESI debut data Y ok pas X
		; EBX not clipped X0
		; ECX good Y0
		; EAX clipped Y1
		; NbPix real X nb pix
		; OffsetBegin start X  ( !!!!! < 128 )

		mov	ebp, esi

		mov	edi, TabOffLine[ecx*4]
		add	edi, ebx
		mov	esi, edi		; good Y offset screen
		add	edi, Log
		add	esi, Screen		; Screen

		mov	edx, eax
		sub	edx, ecx
		inc	edx

		xor	ecx, ecx

l1:     	push	esi
		push	edi

		mov	bl, byte ptr[ebp]	; nb blocs
		inc	ebp

		mov	bh, byte ptr[NbPix]	; nb point … copier

		mov	ecx, [OffsetBegin]	; start x
		mov	ah, cl			; nb pix count
		add	edi, ecx		; new pos screen
		add	esi, ecx

l0:		mov	al, byte ptr[ebp]	; nb pix to jump
		inc	ebp
		dec	bl
		sub	ah, al
		jz	okdraw			; tombe pile
		jc	rptj			; reste pix to jump
		or	bl, bl
		jz	aplus

		mov	al, byte ptr[ebp]	; nb copy point
		inc	ebp
		dec	bl
		sub	ah, al
		jz	okjump
		jc	rgtc			; reste graph to copy

	or	bl, bl		; jmp l0
	jnz	l0
	jmp	aplus

rptj:		neg	ah			; ah nb pix to jump
		mov	cl, ah
		sub	bh, ah			; reste … ecrire
		jle	aplus
		add	edi, ecx
		add	esi, ecx
		jmp	okdraw

rgtc:		neg	ah			; al nb pix to copy
		mov	cl, ah
		sub	bh, ah			; reste … ecrire
		jc	cpt			; copy pas tout
		rep	movsb			; copy
		jz	aplus			; tombe pile
		jmp	okjump

cpt:		add	cl, bh
		rep	movsb
		jmp	aplus

	EVEN
okjump:		mov	cl, byte ptr[ebp]	; al nb pix to jump
		inc	ebp
		dec	bl
		jz	aplus
		sub	bh, cl			; reste … ecrire
		jle	aplus			; fini
		add	edi, ecx
		add	esi, ecx

okdraw:		mov	cl, byte ptr[ebp]	; nb pix to copy
		inc	ebp
		dec	bl
		sub	bh, cl			; reste … ecrire
		jc	cpt			; copy pas tout
		rep	movsb			; copy
		jz	aplus			; tombe pile
		or	bl,bl			; nb bloc--
		jz	aplus
		jmp	short okjump

aplus:		; ligne finie

		movzx	eax, bl
		add	ebp, eax

		pop	edi
		pop	esi
		add	edi, 640
		add	esi, 640


		dec	dl			; nb lig--
		jnz	l1




comment	#
			add	ebx, TabOffLine[ecx*4]
			mov	edi, Log
			add	edi, ebx

			sub	eax, ecx
			inc	al
			mov	bh, al			; BH NbLine
			xor	ecx, ecx
			mov	ebp, edi
;----------------------------------------------
							; ESI = DATAS LINE
NextL:			lea	edi, BufferClip		; EDI = BUFFERCLIP
			lodsb				; NbBlock
			mov	bl, al
SameL:			lodsb
			mov	cl, al
			mov	al, 1
			rep	stosb
			dec	bl
			je	EndLine
			lodsb
			mov	cl, al
			xor	al, al
			rep	stosb
			dec	bl
			jne	SameL
;----------------------
EndLine:	push	esi			; SAVE ESI
			lea	esi, BufferClip		; ESI = BUFFERCLIP
			mov	edi, ebp		; EDI = SCREEN
			mov	ecx, OffsetBegin
			add	esi, ecx
			add	edi, ecx
			mov	ecx, NbPix
;----------------------
Again:
			lodsb
			or	al, al
			jne	Incrust
			mov	[edi], al
Incrust:		inc	edi
			loop	Again

			add	ebp, 640
		pop	esi
			dec	bh
			jne	NextL
;----------------------
	#

EndMask:		ret

CopyMask		endp

;----------------------------------------------------------------------------
AffMask			proc	uses esi edi ebx ebp,\
			nummask:DWORD, xmask:DWORD, ymask:DWORD, \
			bankmask:DWORD

			mov	eax, nummask
			mov	ebx, xmask
			mov	ecx, ymask
			mov	esi, bankmask

			add	esi, [esi+eax*4]; ESI = Begin Data


;-----------------------------------------------
			lodsb			; Delta X
			movzx	edx, al         ;
			lodsb			; Nb Line ( Delta Y )
			movzx	eax, al
;----------------------------------------------- Test Clipping
			add	edx, ebx
			add	eax, ecx
			dec	edx
			dec	eax
			cmp	ebx, ClipXmin
			jl	ClippingMask
			cmp	ecx, ClipYmin
			jl	ClippingMask
			cmp	edx, ClipXmax
			jg	ClippingMask
			cmp	eax, ClipYmax
			jg	ClippingMask
			sub	edx, ebx
			sub 	eax, ecx
			inc	edx
			inc	eax
;----------------------------------------------- Calcul Offset Ecran
			mov	edi, Log
			add	edi, TabOffLine[ecx*4]
			add	edi, ebx
			mov	bh, al		; BH = NbLine
			sub	edx, Screen_X	; EDX = Offset Screen
			neg	edx		; EDX = Screen_X-edx
			xor	ecx, ecx	; Maz Compteur
;----------------------------------------------- Init NbBlock for this line
NextLine:		lodsb			; Nb Block for this line
			mov	bl, al		; BL = NbBlock
;----------------------------------------------- Manage One Line
SameLine:		lodsb           	; Nb Zero to Jump
			add	edi, eax	; Incrust
			dec	bl
			je	EndBlock
			lodsb			; Nb Zero to Write
			mov	cl, al
			xor	al, al
			rep	stosb 		; Write Zero
			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
EndBlock:
			add	edi, edx	; EDI += Offset Screen
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
;----------------------------------------------- Incrust
			ret
;-------------------------------------------------------------------- Clipping
;		Graph : ( EBX, ECX ) ( EDX, EAX )
ClippingMask:
			cmp	ebx, ClipXmax
			jg      EndMask
			cmp	ecx, ClipYmax
			jg	EndMask
			cmp	edx, ClipXmin
			jl	EndMask
			cmp	eax, ClipYmin
			jl	EndMask

			cmp	ecx, ClipYmin
			jge	PasHaut

;---------------------- Clipping Haut, Saute ClipYmin-ECX Line(s)

			push	eax
			push	ebx

			mov	ebx, ClipYmin
			sub	ebx, ecx

;----------------------
			xor	eax, eax
NextH:			lodsb			; NbBlock
			add	esi, eax	; Jump Data
			dec	bl
			jne	NextH

			mov	ecx, ClipYmin		; New Y Haut

			pop	ebx
			pop	eax
;---------------------- Clipping Bas
PasHaut:		cmp	eax, ClipYmax
			jle	PasBas
			mov	eax, ClipYmax
;---------------------- Clipping Gauche
Pasbas:                 mov	OffsetBegin, 0
			cmp     ebx, ClipXmin
			jge	PasGauche
			push	eax
			mov	eax, ClipXmin
			sub	eax, ebx
			mov	OffsetBegin, eax
			pop	eax
;----------------------	Clipping Droit
PasGauche:		push	eax
			mov	eax, edx
			sub	eax, ebx
			sub	eax, OffsetBegin
			inc	eax
			mov	NbPix, eax
			pop	eax

			cmp	edx, ClipXmax
			jle	PasDroit

			sub	edx, ClipXmax
			sub	NbPix, edx
			mov	edx, ClipXmax
;----------------------
PasDroit:

			mov	edi, Log
			add	edi, TabOffLine[ecx*4]
			add	edi, ebx


			sub	eax, ecx
			inc	al
			mov	bh, al			; BH NbLine
			xor	ecx, ecx
			mov	ebp, edi
;----------------------------------------------
							; ESI = DATAS LINE
NextL:			lea	edi, BufferClip		; EDI = BUFFERCLIP
			lodsb				; NbBlock
			mov	bl, al
SameL:			lodsb
			mov	cl, al
			mov	al, 1
			rep	stosb
			dec	bl
			je	EndLine
			lodsb
			mov	cl, al
			xor	al, al
			rep	stosb
			dec	bl
			jne	SameL
;----------------------
EndLine:		push	esi			; SAVE ESI
			lea	esi, BufferClip		; ESI = BUFFERCLIP
			mov	edi, ebp		; EDI = SCREEN
			mov	ecx, OffsetBegin
			add	esi, ecx
			add	edi, ecx
			mov	ecx, NbPix
Again:			lodsb
			or	al, al
			jne	Incrust
			mov	[edi], al
Incrust:		inc	edi
			loop	Again
			add	ebp, 640
			pop	esi
			dec	bh
			jne	NextL

;----------------------
EndMask:		ret
AffMask			endp
;----------------------------------------------------------------------------
GetDxDyMask		proc	uses esi edi ebx,\
			num:DWORD, ptdx:DWORD, ptdy:DWORD, bankmask:DWORD

			mov	esi, bankmask
			mov	eax, num
			add	esi, [esi+eax*4]
			mov	edi, ptdx
			lodsb
			movzx	eax, al
			stosd
			mov	edi, ptdy
			lodsb
			movzx	eax, al
			stosd
			ret
GetDxDyMask		endp
;----------------------------------------------------------------------------
;				The
				End






































