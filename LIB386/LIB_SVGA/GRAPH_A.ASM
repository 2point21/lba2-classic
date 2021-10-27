;----------------------------------------------------------------------------
;				 Graph_A.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------
			include	\projet\lib386\lib_svga\svga.ash


FULL_OPTIMIZE	equ	1

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

ALIGN 4

OffsetBegin	dd	0
NbPix		dd	0
BufferClip	db	512	dup(?)

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

			add	esi, [esi+eax*4]	; ESI = Begin Data

			xor	eax, eax
			xor	edx, edx

			mov	al, byte ptr[esi+2]	; Hot X
			add	ebx, eax
			mov	al, byte ptr[esi+3]	; Hot Y
			add	ecx, eax
			mov	dl, [esi]		; Delta X
			mov	al, [esi+1]		; Nb Line ( Delta Y )
			add	esi, 4			; Jump Hot X & Hot Y

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

			inc	edx
			inc	eax
			sub	edx, ebx
			sub 	eax, ecx


;----------------------------------------------- Calcul Offset Ecran

			mov	edi, Log
			add	edi, TabOffLine[ecx*4]
			add	edi, ebx
			mov	bh, al		; BH = NbLine
			sub	edx, Screen_X	; EDX = Offset Screen
			neg	edx		; EDX = Screen_X-edx
			xor	ecx, ecx	; Maz Compteur

;----------------------------------------------- Init NbBlock for this line

NextLine:		mov	bl, [esi]	; BL = Nb Block for this line
			inc	esi

;----------------------------------------------- Manage One Line

SameLine:		mov	al, [esi]	; OpCode
			mov	cl, al          ; Sauve AL
			and	cl, 00111111b	; AH = Bit 0-5
			test	al, 11000000b	; AL = Bit 6-7
			je	JumpZero	; Incrust
			inc	cl		; One More Please...
			test	al, 01000000b	; AL = 01b
			jne	WriteDiffPix	; Pix Differents

;----------------------------------------------- Repeat Same Color

			mov	al, [esi+1]	; Color to Repeat
			add	esi, 2
			shr	ecx, 1
			mov	ah, al
			rep	stosw		; Fill
			jnc	nobytec
			mov	[edi], al
			inc	edi
nobytec:		dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
			add	edi, edx	; EDI += Offset Screen
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret

;----------------------------------------------- Copy Diff Pix

WriteDiffPix:		inc	esi
			shr	ecx, 1
			rep	movsw		; Copy Pix
			jnc	nobyted
			mov	al, [esi]
			mov	[edi], al
			inc	esi
			inc	edi
nobyted:		dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
			add	edi, edx	; EDI += Offset Screen
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret

;----------------------------------------------- Incrust

JumpZero:		inc	esi
			lea	edi, [edi+ecx+1]; Jump Zero (incrust)
			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
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

			mov	ebp, eax
			mov	edi, ebx

			mov	ebx, ClipYmin
			sub	ebx, ecx

NextH:			mov	bh, [esi]
			inc	esi
SameH:			mov	al, [esi]
			inc	esi
			test	al, 11000000b
			je	JumpzH
			xor	ecx, ecx
			test	al, 01000000b
			je	AquiH
			and	eax, 00111111b
			mov	ecx, eax
AquiH:			lea	esi, [esi+ecx+1]
JumpzH:			dec	bh
			jne	SameH
			dec	bl
			jne	NextH

			mov	ecx, ClipYmin		; New Y Haut

			mov	eax, ebp
			mov	ebx, edi

;---------------------- Clipping Bas
PasHaut:		cmp	eax, ClipYmax
			jle	PasBas
			mov	eax, ClipYmax
;---------------------- Clipping Gauche
Pasbas:                 xor	edi, edi
			cmp     ebx, ClipXmin
			jge	PasGauche
			mov	edi, ClipXmin
			sub	edi, ebx
PasGauche:		mov	OffsetBegin, edi
;----------------------	Clipping Droit
			lea	edi, [edi+ebx-1]
			sub	edi, edx
			neg	edi
			mov	NbPix, edi

			sub	edx, ClipXmax
			jle	PasDroit
			sub	NbPix, edx
;----------------------
PasDroit:

			mov	edi, Log
			add	edi, TabOffLine[ecx*4]
			add	edi, ebx


			sub	eax, ecx
			inc	al
			mov	bh, al			; BH NbLine
			xor	ecx, ecx
			mov	edx, edi
;----------------------------------------------
							; ESI = DATAS LINE
NextL:			lea	edi, BufferClip		; EDI = BUFFERCLIP
			mov	bl, [esi]
			inc	esi
SameL:			mov	al, [esi]
			inc	esi
			mov	cl, al
			and	cl, 00111111b
			inc	cl
			test	al, 11000000b
			je	JumpL
			test	al, 01000000b
			jne	WriteDiffL
;----------------------
			mov	al, [esi]
			inc	esi
			shr	ecx, 1
			mov	ah, al
			rep	stosw
			jnc	nobyte
			mov	[edi], al
			inc	edi
			dec	bl
			jne	SameL
			jmp	NextLine2
;----------------------
JumpL:			xor	ax, ax
			shr	ecx, 1
			rep	stosw
			jnc	nobyte
			mov	[edi], al
			inc	edi
			dec	bl
			jne	SameL
			jmp	NextLine2
;----------------------
WriteDiffL:		shr	ecx, 1
			rep	movsw
			jnc	nobyte
			mov	al, [esi]
			mov	[edi], al
			inc	esi
			inc	edi
nobyte:			dec	bl
			jne	SameL
;----------------------
NextLine2:		mov	ebp, esi		; SAVE ESI
			mov	ecx, OffsetBegin
			lea	esi, [BufferClip+ecx]	; ESI = BUFFERCLIP
			lea	edi, [edx+ecx]		; EDI = SCREEN
			mov	ecx, NbPix
Again:			mov	al, [esi]
			inc	esi
			or	al, al
			je	Incrust
			mov	[edi], al
Incrust:		inc	edi
			dec	ecx
			jnz	Again
			add	edx, 640
			mov	esi, ebp
			dec	bh
			jne	NextL
;----------------------
EndGraph:		ret
AffGraph		endp
;----------------------------------------------------------------------------
GetDxDyGraph		proc	uses esi edi ebx,\
			num:DWORD, ptdx:DWORD, ptdy:DWORD, bankgraph:DWORD

			mov	esi, bankgraph
			mov	eax, num
			add	esi, [esi+eax*4]
			mov	edi, ptdx
			xor	eax, eax
			mov	al, [esi]
			mov	dword ptr[edi], eax
			mov	edi, ptdy
			mov	al, [esi+1]
			mov	dword ptr[edi], eax
			ret
GetDxDyGraph		endp
;----------------------------------------------------------------------------
;				The
				End






































