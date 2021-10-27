;----------------------------------------------------------------------------
;				 Graph_A.ASM 386
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
				   Block:
				    - 00xxxxxxb  xxxxxx zeros to jump
				    - 01xxxxxxb  xxxxxx Copy Pix
				    - 10xxxxxxb  xxxxxx Repeat Pix
				    - BYTE datas[]
@

BufferClip	db	512	dup(?)
OffsetBegin	dd	0
NbPix		dd	0
;----------------------------------------------------------------------------
				.code

			public	NoLanguage	AffGraph
			public	NoLanguage	GetDxDyGraph

;----------------------------------------------------------------------------
AffGraph		proc	uses esi edi ebx ebp,\
			numbrick:DWORD, xbrick:DWORD, ybrick:DWORD, \
			bankbrick:DWORD

			mov	eax, numbrick
			mov	ebx, xbrick
			mov	ecx, ybrick
			mov	esi, bankbrick

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
			jl	ClippingGraph
			cmp	ecx, ClipYmin
			jl	ClippingGraph
			cmp	edx, ClipXmax
			jg	ClippingGraph
			cmp	eax, ClipYmax
			jg	ClippingGraph
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
SameLine:		lodsb   		; OpCode
			mov	cl, al          ; Sauve AL
			and	cl, 00111111b	; AH = Bit 0-5
			inc	cl		; One More Please...
			test	al, 11000000b	; AL = Bit 6-7
			je	JumpZero	; Incrust
			test	al, 01000000b	; AL = 01b
			jne	WriteDiffPix	; Pix Differents
;----------------------------------------------- Repeat Same Color
			lodsb			; Color to Repeat
			rep	stosb		; Fill
			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
			add	edi, edx	; EDI += Offset Screen
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret
;----------------------------------------------- Incrust
JumpZero:		add	edi, ecx	; Jump Zero (incrust)
			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
			add	edi, edx	; EDI += Offset Screen
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret
;----------------------------------------------- Copy Diff Pix
WriteDiffPix:		rep	movsb		; Copy Pix
			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
;----------------------------------------------- End of this Line ?
			add	edi, edx	; EDI += Offset Screen
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret
;-------------------------------------------------------------------- Clipping
;		Graph : ( EBX, ECX ) ( EDX, EAX )
ClippingGraph:
			cmp	ebx, ClipXmax
			jg      EndGraph
			cmp	ecx, ClipYmax
			jg	EndGraph
			cmp	edx, ClipXmin
			jl	EndGraph
			cmp	eax, ClipYmin
			jl	EndGraph

			cmp	ecx, ClipYmin
			jge	PasHaut

;---------------------- Clipping Haut, Saute ClipYmin-ECX Line(s)

			push	eax
			push	ebx


			mov	ebx, ClipYmin
			sub	ebx, ecx

			mov	bh, bl

NextH:			lodsb
			mov	bl, al
SameH:			lodsb
			test	al, 11000000b
			je	JumpzH
			mov	ecx, 1
			test	al, 01000000b
			je	AquiH
			mov	ecx, eax
			and	ecx, 00111111b
			inc	ecx
AquiH:			add	esi, ecx
JumpzH:			dec	bl
			jne	SameH
			dec	bh
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
			lodsb
			mov	bl, al
SameL:			lodsb
			mov	cl, al
			and	cl, 00111111b
			inc	cl
			test	al, 11000000b
			je	JumpL
			test	al, 01000000b
			jne	WriteDiffL
;----------------------
			lodsb
			rep	stosb
			jmp	nextBlock
;----------------------
JumpL:			xor	al, al
			rep	stosb
			jmp	NextBlock
;----------------------
WriteDiffL:             rep	movsb
;----------------------
NextBlock:             	dec	bl
			jne	SameL
;----------------------
			push	esi			; SAVE ESI
			lea	esi, BufferClip		; ESI = BUFFERCLIP
			mov	edi, ebp		; EDI = SCREEN
			mov	ecx, OffsetBegin
			add	esi, ecx
			add	edi, ecx
			mov	ecx, NbPix
Again:			lodsb
			or	al, al
			je	Incrust
			mov	[edi], al
Incrust:		inc	edi
			loop	Again
			add	ebp, 640
			pop	esi
			dec	bh
			jne	NextL
;----------------------
EndGraph:		ret
AffGraph		endp
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
GetDxDyGraph		proc	uses esi edi ,\
			num:DWORD, ptdx:DWORD, ptdy:DWORD, bankgraph:DWORD

			mov	esi, bankgraph
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
GetDxDyGraph		endp

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------




;----------------------------------------------------------------------------
;				The
				End






































