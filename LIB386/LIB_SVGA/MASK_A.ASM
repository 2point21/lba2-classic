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

ColMask		db	0	; Noir par Defaut

ALIGN 4

OffsetBegin	dd	0
NbPix		dd	0

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CoulMask
			public	NoLanguage	AffMask
			public	NoLanguage	AffMask_Asm
			public	NoLanguage	GetDxDyMask

;----------------------------------------------------------------------------
CoulMask		proc ,\
			coul:DWORD

			mov	eax, coul
			mov	byte ptr [ColMask], al
			ret

CoulMask		endp
;----------------------------------------------------------------------------
AffMask			proc	uses esi edi ebx ebp,\
			nummask:DWORD, xmask:DWORD, ymask:DWORD, \
			bankmask:DWORD

			mov	eax, nummask
			mov	ebx, xmask
			mov	ecx, ymask
			mov	esi, bankmask

			Call	AffMask_Asm

			ret

AffMask			endp
;----------------------------------------------------------------------------
AffMask_Asm		proc

			add	esi, [esi+eax*4]	; ESI = Begin Data

			xor	eax, eax
			mov	al, [esi+2]
			add	ebx, eax		; Hot X
			mov	al, [esi+3]
			add	ecx, eax		; Hot Y

;-----------------------------------------------

			xor	edx, edx
			mov	dl, [esi]	; Delta X
			mov	al, [esi+1]	; Nb Line ( Delta Y )
			add	esi, 4		; Jump HotX & HotY

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
			mov	edi, Log
			add	edi, TabOffLine[ecx*4]
			add	edi, ebx
			mov	bh, al		; BH = NbLine
			sub	edx, Screen_X	; EDX = Offset Screen
			neg	edx		; EDX = Screen_X-edx
			xor	ecx, ecx	; Maz Compteur
			mov	ebp, edx	; EBP Offset Next Line
			mov	al, byte ptr [ColMask]; Couleur du Masque
			mov	ah, al
;----------------------------------------------- Init NbBlock for this line
NextLine:		mov	bl, [esi]	; BL = Nb Block for this line
			inc	esi
;----------------------------------------------- Manage One Line
SameLine:		mov	cl, [esi]     	; Nb Zero to Jump
			add	edi, ecx	; Incrust
			dec	bl
			je	EndBlock
			mov	cl, [esi+1]	; Nb Zero to Write
			add	esi, 2
			shr	ecx, 1
			rep	stosw 		; Write Zero
			jnc	nobyte
			stosb
nobyte:			dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
			add	edi, ebp	; EDI += Offset Next Line
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
			ret
EndBlock:
			inc	esi
			add	edi, ebp	; EDI += Offset Next Line
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

			sub	ecx, ClipYmin
			neg	ecx

			mov	ebp, eax
;----------------------
			xor	eax, eax

NextH:			mov	al, [esi]		; NbBlock
			lea	esi, [esi+1+eax]	; Jump Data
			dec	ecx
			jnz	NextH

			mov	eax, ebp

			mov	ecx, ClipYmin		; New Y Haut

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
PasGauche:		mov	ebp, edx
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
			mov	dl, byte ptr [ColMask]
			mov	dh, dl
							; ESI = DATAS LINE
NextL:			lea	edi, BufferClip		; EDI = BUFFERCLIP
			mov	bl, [esi]		; NbBlock
			inc	esi
SameL:			mov	cl, [esi]
			xor	ax, ax
			shr	ecx, 1
			rep	stosw
			jnc	nobyte1
			stosb
nobyte1:		dec	bl
			je	EndLine
			mov	cl, [esi+1]
			add	esi, 2
			mov	ax, dx			; ColMask
			shr	ecx, 1
			rep	stosw
			jnc	nobyte2
			stosb
nobyte2:		dec	bl
			jne	SameL
			dec	esi
;----------------------
EndLine:		inc	esi
			push	esi			; SAVE ESI
			mov	ecx, OffsetBegin
			lea	edi, [ebp+ecx]		; EDI = SCREEN
			lea	esi, [BufferClip+ecx]	; ESI = BUFFERCLIP
			mov	ecx, NbPix
Again:			lodsb
			or	al, al
			je	Incrust
			mov	[edi], al
Incrust:		inc	edi
			dec	ecx
			jnz	Again
			add	ebp, 640
			pop	esi
			dec	bh
			jne	NextL

;----------------------
EndMask:		ret
AffMask_Asm		endp
;----------------------------------------------------------------------------
GetDxDyMask		proc	uses esi edi ebx,\
			num:DWORD, ptdx:DWORD, ptdy:DWORD, bankmask:DWORD

			mov	esi, bankmask
			mov	eax, num
			add	esi, [esi+eax*4]
			mov	edi, ptdx
			xor	eax, eax
			mov	al, [esi]
			mov	[edi], eax
			mov	edi, ptdy
			mov	al, [esi+1]
			mov	[edi], eax
			ret
GetDxDyMask		endp
;----------------------------------------------------------------------------
;				The
				End






































