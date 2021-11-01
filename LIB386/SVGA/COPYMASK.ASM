;----------------------------------------------------------------------------
			.386p
			.model SMALL, SYSCALL

STRUC_MASK		STRUC
  Mask_DeltaX		db	0
  Mask_DeltaY		db	0
  Mask_HotX		db	0
  Mask_HotY		db	0
STRUC_MASK		ENDS





;----------------------------------------------------------------------------
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

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			EXTRN   SYSCALL      ClipXMin        :DWORD
			EXTRN   SYSCALL      ClipYMin        :DWORD
			EXTRN   SYSCALL      ClipXMax        :DWORD
			EXTRN   SYSCALL      ClipYMax        :DWORD
			EXTRN   SYSCALL      Log             :DWORD
			EXTRN   SYSCALL      TabOffLine      :DWORD

;----------------------------------------------------------------------------
			OffsetBegin	dd	0
			NbPix		dd	0

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			PUBLIC  SYSCALL      CopyMask

;----------------------------------------------------------------------------
;S32	CopyMask(S32 nummask, S32 x, S32 y, void *bankmask, void *src)	;

;#pragma aux CopyMask	"*"			\
;	parm	[eax] [ebx] [ecx] [esi] [edx]	\
;	modify	[edi]

CopyMask		proc	uses ebp

			add	esi, [esi+eax*4]	; ESI = Begin Data

			mov	ebp, edx
			xor	eax, eax

			xor	edx, edx
			mov	al, [esi].Mask_HotX	; Hot X

			mov	dl, [esi].Mask_HotY	; Hot Y
			add	ebx, eax

			add	ecx, edx
;-----------------------------------------------
			mov	dl, [esi].Mask_DeltaX	; Delta X

			mov	al, [esi].Mask_DeltaY	; Nb Line ( Delta Y )
			add	esi, Size STRUC_MASK	; Skip header

;----------------------------------------------- Test Clipping
			lea	edx, [edx+ebx-1]
			lea	eax, [eax+ecx-1]

			cmp	ebx, [ClipXMin]
			jl	ClippingMask
			cmp	ecx, [ClipYMin]
			jl	ClippingMask
			cmp	edx, [ClipXMax]
			jg	ClippingMask
			cmp	eax, [ClipYMax]
			jg	ClippingMask

			sub	edx, ebx
			sub 	eax, ecx

			inc	edx
			inc	eax

;----------------------------------------------- Calcul Offset Ecran
			add	ebx, TabOffLine[ecx*4]
			mov	edi, [Log]

			add	edi, ebx
			add	ebp, ebx

			mov	bh, al			; BH = NbLine
			sub	edx, TabOffLine[1*4]	; EDX = Offset Screen
			neg	edx			; EDX = Screen_X-edx
			xor	ecx, ecx		; Maz Compteur
			xor	eax,eax
;----------------------------------------------- Init NbBlock for this line
NextLine:		mov	bl, [esi]	; Nb Block for this line
			inc	esi


;----------------------------------------------- Manage One Line
SameLine:		mov	al, [esi]       ; Nb Zero to Jump
			inc	esi

			add	edi, eax	; Incrust on Log
			add	ebp, eax	; And on PtSrc

			dec	bl
			jz	EndBlock

			mov	cl, [esi]	; Nb Zero to Write
			inc	esi

			push	ecx
			push	edi
@@LoopLoadCache:
			mov	al,[edi]
			add	edi,32
			sub	ecx,32
			jg	@@LoopLoadCache
			pop	edi
			pop	ecx

loopb:			mov	al, [ebp]
			inc	ebp
			mov	[edi], al
			inc	edi
			dec	ecx
			jnz	loopb
nob:
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
			cmp	ebx, ClipXMax
			jg      EndMask
			cmp	ecx, ClipYMax
			jg	EndMask
			cmp	edx, ClipXMin
			jl	EndMask
			cmp	eax, ClipYMin
			jl	EndMask

			cmp	ecx, [ClipYMin]
			jge	PasHaut

;---------------------- Clipping Haut, Saute ClipYMin-ECX Line(s)
			sub	ecx, [ClipYMin]
			neg	ecx
			mov	edi, eax
			xor	eax, eax

NextH:			mov	al, [esi]
			lea	esi, [esi+eax+1]
			dec	ecx
			jnz	NextH

			mov	ecx, [ClipYMin]
			mov	eax, edi

;---------------------- Clipping Bas
PasHaut:		cmp	eax, [ClipYMax]
			jle	PasBas
			mov	eax, [ClipYMax]

;---------------------- Clipping Gauche
PasBas:                 mov	[OffsetBegin], 0
			cmp     ebx, [ClipXMin]
			jge	PasGauche

			mov	edi, [ClipXMin]
			sub	edi, ebx
			mov	[OffsetBegin], edi

;----------------------	Clipping Droit
PasGauche:		mov	edi, edx
			sub	edi, ebx
			sub	edi, [OffsetBegin]
			inc	edi

			cmp	edx, [ClipXMax]
			jle	PasDroit

			sub	edx, [ClipXMax]
			sub	edi, edx
			mov	edx, [ClipXMax]
PasDroit:
			mov	[NbPix], edi
;----------------------

			; ESI debut data Y ok pas X
			; EBX not clipped X0
			; ECX good Y0
			; EAX clipped Y1
			; NbPix real X nb pix
			; OffsetBegin start X  ( !!!!! < 128 )

			mov	edi,esi
			mov	esi,ebp
			mov	ebp,edi
			mov	edi, TabOffLine[ecx*4]
			add	edi, ebx
			add	esi,edi

			add	edi, [Log]

			mov	edx, eax
			sub	edx, ecx
			inc	edx

			xor	ecx, ecx

l1:     		push	esi
			push	edi

			mov	bl, byte ptr[ebp]	; nb blocs
			inc	ebp

			mov	bh, byte ptr[NbPix]	; nb point à copier

			mov	ecx, [OffsetBegin]	; start x
			mov	ah, cl			; nb pix count
			add	edi, ecx		; new pos screen
			add	esi, ecx

l0:			mov	al, byte ptr[ebp]	; nb pix to jump
			inc	ebp
			dec	bl
			sub	ah, al
			jz	okdraw			; tombe pile
			jc	rptj			; reste pix to jump
			test	bl, bl
			jz	aplus

			mov	al, byte ptr[ebp]	; nb copy point
			inc	ebp
			dec	bl
			sub	ah, al
			jz	okjump
			jc	rgtc			; reste graph to copy

			test	bl, bl		; jmp l0
			jnz	l0
			jmp	aplus

rptj:			neg	ah			; ah nb pix to jump
			mov	cl, ah
			sub	bh, ah			; reste à ecrire
			jle	aplus
			add	edi, ecx
			add	esi, ecx
			jmp	okdraw

rgtc:			neg	ah			; al nb pix to copy
			mov	cl, ah
			sub	bh, ah			; reste à ecrire
			jc	cpt			; copy pas tout
			rep	movsb			; copy
			jz	aplus			; tombe pile
			jmp	okjump

cpt:			add	cl, bh
			rep	movsb
			jmp	aplus

okjump:			mov	cl, byte ptr[ebp]	; al nb pix to jump
			inc	ebp
			dec	bl
			jz	aplus
			sub	bh, cl			; reste à ecrire
			jle	aplus			; fini
			add	edi, ecx
			add	esi, ecx

okdraw:			mov	cl, byte ptr[ebp]	; nb pix to copy
			inc	ebp
			dec	bl
			sub	bh, cl			; reste à ecrire
			jc	cpt			; copy pas tout
			rep	movsb			; copy
			jz	aplus			; tombe pile
			test	bl,bl			; nb bloc--
			jz	aplus
			jmp	short okjump

aplus:			; ligne finie

			pop	edi
			pop	esi

			movzx	eax, bl

			add	edi, TabOffLine[1*4]
			add	ebp, eax

			add	esi, TabOffLine[1*4]

			dec	dl			; nb lig--
			jnz	l1

EndMask:		ret

CopyMask		endp

;----------------------------------------------------------------------------
;				The
				End
