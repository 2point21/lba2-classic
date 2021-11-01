;----------------------------------------------------------------------------
;				 Mask_A.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
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

save_esi	dd	?

extrn		NoLanguage	Screen:DWORD

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CopyMask

;----------------------------------------------------------------------------
CopyMask		proc	uses esi edi ebx ebp,\
			nummask:DWORD, xmask:DWORD, ymask:DWORD, \
			bankmask:DWORD, ptsrc:DWORD

			mov	esi, ptsrc

			mov	eax, nummask
			mov	ebx, xmask
			mov	ecx, ymask
			mov	ebp, bankmask

			add	ebp, [ebp+eax*4]	; EBP = Begin Data

			xor	eax, eax
			mov	al, [ebp+2]		; Hot X
			add	ebx, eax
			mov	al, [ebp+3]		; Hot Y
			add	ecx, eax

;-----------------------------------------------

			xor	edx, edx
			mov	dl, [ebp]	; Delta X
			mov	al, [ebp+1]	; Nb Line ( Delta Y )
			add	ebp, 4		; Jump Hot X & Hot Y

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

			inc	edx
			inc	eax
			sub	edx, ebx
			sub 	eax, ecx

;----------------------------------------------- Calcul Offset Ecran
			add	ebx, TabOffLine[ecx*4]

			mov	edi, Log

			add	esi, ebx
			add	edi, ebx

			mov	bh, al		; BH = NbLine
			sub	edx, Screen_X	; EDX = Offset Screen
			neg	edx		; EDX = Screen_X-edx
			xor	ecx, ecx	; Maz Compteur
;----------------------------------------------- Init NbBlock for this line
NextLine:		mov	bl, [ebp]	; BL = Nb Block for this line
			inc	ebp
;----------------------------------------------- Manage One Line
SameLine:		mov	cl, [ebp]      	; Nb Zero to Jump
			add	edi, ecx	; Incrust on Log
			add	esi, ecx	; And on PtSrc

			dec	bl
			je	EndBlock

			mov	cl, [ebp+1]	; Nb Zero to Write
			add	ebp, 2

			mov	al, cl
			shr	cl, 2
			and	al, 11b
			rep	movsd 		; Write Datas From PtSrc
			mov	cl, al
			rep	movsb

			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
			add	edi, edx	; EDI += Offset Screen
			add	esi, edx
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret
EndBlock:
			inc	ebp
			add	edi, edx	; EDI += Offset Screen
			add	esi, edx
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
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

			mov	esi, eax

			xor	eax, eax
NextH:			mov	al, [ebp]
			lea	ebp, [ebp+1+eax]
			dec	ecx
			jnz	NextH

			mov	eax, esi

			mov	ecx, ClipYMin

;---------------------- Clipping Bas
PasHaut:		cmp	eax, ClipYmax
			jle	PasBas
			mov	eax, ClipYmax
;---------------------- Clipping Gauche
Pasbas:                 mov	OffsetBegin, 0
			cmp     ebx, ClipXmin
			jge	PasGauche

			mov	esi, ClipXmin
			sub	esi, ebx
			mov	OffsetBegin, esi

;----------------------	Clipping Droit
PasGauche:
			mov	esi, edx
			sub	esi, ebx
			sub	esi, OffsetBegin
			inc	esi
			mov	NbPix, esi

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

		mov	edx, TabOffLine[ecx*4]
		add	edx, ebx
		add	edx, [OffsetBegin]

		lea	ebx, [eax+1]

		mov	eax, edx		; good Y offset screen
		add	edx, Log
		add	eax, Screen		; Screen

		sub	ebx, ecx

		xor	ecx, ecx

		jmp	start



aplus:		mov	cl, bl
		add	ebp, ecx

aplus2:		shr	ebx, 16
		dec	bl
		jz	EndMask

l1:		inc	ebp

		add	eax, 640
		add	edx, 640
start:
		shl	ebx, 16

		mov	cl, byte ptr[OffsetBegin]; start x
						; cl = nb pix count

		mov	bh, byte ptr[NbPix]	; nb point à copier

		mov	esi, eax
		mov	edi, edx		; new pos screen

		mov	bl, [ebp]		; nb blocs

l0:		inc	ebp
		dec	bl
		jz	aplus2
		sub	cl, [ebp]		; nb pix to jump
		jc	rptj			; reste pix to jump
		jz	okdraw			; tombe pile

		inc	ebp
		dec	bl
		sub	cl, [ebp]		; nb copy point
		jc	rgtc			; reste graph to copy
		jz	okjump2

		or	bl, bl
		jnz	l0

		shr	ebx, 16
		dec	bl			; nb lig--
		jnz	l1
		ret
rptj:
						; cl = - nb pix to jump
		add	bh, cl			; reste à ecrire
		jle	aplus
		neg	cl
		jmp	okdraw2

rgtc:
		neg	cl			; cl nb pix to copy
		sub	bh, cl			; reste à ecrire
		jc	cpt			; copy pas tout
		rep	movsb			; copy
		jz	aplus

okjump2:	or	bl, bl
		jz	aplus2

okjump:		inc	ebp
		dec	bl
		jz	aplus2
		mov	cl, [ebp]		; nb pix to jump
		sub	bh, cl			; reste à ecrire
		jle	aplus			; fini
okdraw2:
		add	edi, ecx
		add	esi, ecx

okdraw:		inc	ebp
		dec	bl			; nb bloc--
		mov	cl, [ebp]		; nb pix to copy
		sub	bh, cl			; reste à ecrire
		jc	cpt			; copy pas tout
		rep	movsb			; copy
		jz	aplus			; tombe pile
		or	bl, bl
		jnz	okjump

		shr	ebx, 16
		dec	bl			; nb lig--
		jnz	l1
		ret

cpt:		add	cl, bh
		rep	movsb

		; ligne finie

		mov	cl, bl
		add	ebp, ecx

		shr	ebx, 16
		dec	bl			; nb lig--
		jnz	l1
EndMask:	ret

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

CopyMask		endp

;----------------------------------------------------------------------------
;				The
				End
