;----------------------------------------------------------------------------
;				 Grille_A.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------
			public	SYSCALL	CodeJeu

			public	SYSCALL	XMap
			public	SYSCALL	YMap
			public	SYSCALL	ZMap

			public	SYSCALL	XScreen
			public	SYSCALL	YScreen

			public	SYSCALL	BufCube
			public	SYSCALL	BufMap
			public	SYSCALL	TabBlock
			public	SYSCALL	BufferBrick

CodeJeu		db	0


XMap		dd	0
YMap		dd	0
ZMap		dd	0

XScreen		dd	0
YScreen		dd	0

comment	@ /*
UBYTE	*BufCube		;
UBYTE	*BufMap			;
UBYTE	*TabBlock		;
UBYTE	*BufferBrick		;
	*/ @

BufCube		dd	0
BufMap		dd	0
TabBlock	dd	0
BufferBrick	dd	0

HEADER_BLOCK	equ	3

SIZE_CUBE_X	equ	64
SIZE_CUBE_Y	equ	25
SIZE_CUBE_Z	equ	64

SIZE_BRICK_XZ	equ	512
SIZE_BRICK_Y	equ	256
DEMI_BRICK_XZ	equ	256
DEMI_BRICK_Y	equ	128

;----------------------------------------------------------------------------
				.code

			public	SYSCALL	DecompColonne
			public	SYSCALL	Map2Screen
;'			public	SYSCALL	MixteColonne
			public	SYSCALL	GetAdrBlock
;'			public	SYSCALL	GetColBrick
			public	SYSCALL	WorldColBrick
			public	SYSCALL	WorldColBrickFull
			public	SYSCALL	WorldCodeBrick
			public	SYSCALL	GetBlockBrick
			public	SYSCALL	GetWorldColBrickVisible

;----------------------------------------------------------------------------
;*--------------------------------------------------------------------------*

;// ATTENTION: passage de parametres par registres le 04/04/96
;
;	U8 *GetAdrBlock( S32 numblock)  ;
;			     ecx
;

GetAdrBlock	proc

		mov	eax, [TabBlock]		; pto = (ULONG*)TabBlock
		add	eax, [ eax + ecx*4 ]	; TabBlock + pto[numbloc]

		ret

GetAdrBlock	endp

;*--------------------------------------------------------------------------*

WorldColBrickFull 	proc	xw:DWORD, yw:DWORD, zw:DWORD, ymax:DWORD

	;' ptc = BufCube

		mov	ecx, [BufCube]

	;' + (xm*SIZE_CUBE_Y*2)

		mov	eax, xw

	;' xm = (xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512

		mov	[XMap], eax

	;' + ym*2

		mov	eax, yw

	;' ym = yw/SIZE_BRICK_Y

		sar	eax, 8			; /256
		mov	[YMap], eax

	;' + zm*3200 (SIZE_CUBE_X*SIZE_CUBE_Y*2)

		mov	eax, zw

	;' zm = (zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512

		mov	[ZMap], eax

		mov	eax, [XMap]
		or	eax, eax		; bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		lea	eax, [eax*2]		; *2
		mov	edx, eax		; =
		lea	eax, [eax*8]		; *16
		add	edx, eax		; 2  + 16 = 18
		lea	edx, [edx+eax*2]	; 18 + 32 = 50

		add	ecx, edx

		mov	eax, [YMap]

		cmp	eax, -1
		jle	col_1
		cmp	eax, 25
		jge	ovfw

		lea	ecx, [ecx+eax*2]

		mov	eax, [ZMap]

		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		shl	eax, 7			; *128
		mov	edx, eax		; = 128
		lea	eax, [eax*8]		; *1024
		add	edx, eax		; 128  + 1024 = 1152
		lea	edx, [edx+eax*2]	; 1152 + 2048 = 3200

		add	ecx, edx

		xor	eax, eax
		mov	al, [ecx]

		or	al, al
		jz	nocol

	;'	if( *ptc != 0 )
	;'	{
	;'		block = (*ptc++) - 1 ;
	;'		adr = GetAdrBlock( block )	;
	;'		adr += HEADER_BLOCK		;
	;'		adr += (*ptc)<<2 		;/* 4 Bytes to Jump	*/
	;'		return *adr ;
	;'	}

		mov	edx, [TabBlock]			; pto = (ULONG*)TabBlock
		add	edx, [ edx + eax*4 - 4]		; TabBlock + pto[numblock]

		add	edx, HEADER_BLOCK

		mov	al, byte ptr[ecx+1]
		lea	edx, [edx + eax*4 ]

		mov	al, [edx]

		; check full height
		mov	edx, ymax
		add	edx, 255
		shr	edx, 8				; / 256 hauteur brick

		push	ebx
		mov	ebx, [YMap]

th0:		cmp	ebx, 24
		jae	notesth

		add	ecx, 2
		inc	ebx

		cmp	word ptr[ecx], 0
		jnz	hcol
		dec	edx
		jnz	th0

notesth:	pop	ebx

		ret

nocol:	;' return *(ptc+1)

		mov	al, byte ptr[ecx+1]	; eventuelle col transparente

		; check full height
		mov	edx, ymax
		add	edx, 255
		shr	edx, 8			; / 256 hauteur brick

		push	ebx
		mov	ebx, [YMap]

th1:		cmp	ebx, 24
		jae	notsth

		add	ecx, 2
		inc	ebx
		cmp	word ptr[ecx], 0
		jnz	hcol
		dec	edx
		jnz	th1

notsth:		pop	ebx

		ret

col_1:		; mov	[YMap], 0		; test y-1
hcol:           pop	ebx
		mov	al, 1
		ret

ovfw:		mov	al, 0
		ret

WorldColBrickFull	endp

;*--------------------------------------------------------------------------*

WorldColBrick 	proc	xw:DWORD, yw:DWORD, zw:DWORD

	;' ptc = BufCube
		mov	ecx, [BufCube]

	;' + (xm*SIZE_CUBE_Y*2)
		mov	eax, xw

	;' xm = (xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512
		mov	[XMap], eax

	;' + ym*2
		mov	eax, yw
	;' ym = yw/SIZE_BRICK_Y

		sar	eax, 8			; /256
		mov	[YMap], eax

	;' + zm*3200 (SIZE_CUBE_X*SIZE_CUBE_Y*2)

		mov	eax, zw

	;' zm = (zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512
		mov	[ZMap], eax



		mov	eax, [XMap]

		or	eax, eax		; bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		lea	eax, [eax*2]		; *2
		mov	edx, eax		; =
		lea	eax, [eax*8]		; *16
		add	edx, eax		; 2  + 16 = 18
		lea	edx, [edx+eax*2]	; 18 + 32 = 50

		add	ecx, edx

		mov	eax, [YMap]

		cmp	eax, -1
		jle	col_1

		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 25
		jge	ovfw

		lea	ecx, [ecx+eax*2]

		mov	eax, [ZMap]

		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		shl	eax, 7			; *128
		mov	edx, eax		; = 128
		lea	eax, [eax*8]		; *1024
		add	edx, eax		; 128  + 1024 = 1152
		lea	edx, [edx+eax*2]	; 1152 + 2048 = 3200

		add	ecx, edx

		xor	eax, eax
		mov	al, [ecx]

		or	al, al
		jz	nocol

	;'	if( *ptc != 0 )
	;'	{
	;'		block = (*ptc++) - 1 ;
	;'		adr = GetAdrBlock( block )	;
	;'		adr += HEADER_BLOCK		;
	;'		adr += (*ptc)<<2 		;/* 4 Bytes to Jump	*/
	;'		return *adr ;
	;'	}

		mov	edx, [TabBlock]			; pto = (ULONG*)TabBlock
		add	edx, [ edx + eax*4 - 4]		; TabBlock + pto[numblock]

		add	edx, HEADER_BLOCK


;	essai	mov	al, byte ptr[edx+1]		; code jeu
;		mov	[CodeJeu], al


		mov	al, byte ptr[ecx+1]
		lea	edx, [edx + eax*4 ]

		mov	al, [edx]

		ret

nocol:	;' return *(ptc+1)

		mov	al, byte ptr[ecx+1]			; eventuelle col transparente
		ret

col_1:		; mov	[YMap], 0		; test col-1
		mov	al, 1
		ret

ovfw:		mov	al, 0
		ret

WorldColBrick	endp

;*--------------------------------------------------------------------------*

;----------------------------------------------------------------------------

; SEBER

;// ATTENTION: passage de parametres par registres le 09/04/96
;
;	U8 WorldCodeBrick( S32 xm, S32 ym, S32 zm )  ;
;                           eax     ebx     ecx
;
;----------------------------------------------------------------------------
WorldCodeBrick	proc

		shr	eax, 9			; xm/512

		mov	[XMap], eax

		lea	eax, [eax*2]		; *2

		mov	edx, eax		; =
		lea	eax, [eax*8]		; *16
		add	edx, eax		; 2  + 16 = 18
		lea	edx, [edx+eax*2]	; 18 + 32 = 50

		add	edx, [BufCube]		; edx --> BufCube

	;' + ym*2

		cmp	ebx, -1			; yw
		jle	nocode

	;' ym = yw/SIZE_BRICK_Y

		shr	ebx, 8			; /256

		mov	[YMap], ebx

		lea	edx, [edx+ebx*2]

	;' + zm*3200 (SIZE_CUBE_X*SIZE_CUBE_Y*2)
	;' zm = (zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

		shr	ecx, 9			; /512

		mov	[ZMap], ecx

		shl	ecx, 7			; *128
		mov	eax, ecx		; = 128
		lea	ecx, [ecx*8]		; *1024
		add	eax, ecx		; 128  + 1024 = 1152
		lea	eax, [eax+ecx*2]	; 1152 + 2048 = 3200

		add	edx, eax

		xor	eax, eax
		mov	al, [edx]

		or	al, al
		jz	nocode

	;'	if( *ptc != 0 )
	;'	{
	;'		block = (*ptc++) - 1 ;
	;'		adr = GetAdrBlock( block )	;
	;'		adr += HEADER_BLOCK		;
	;'		adr += (*ptc)<<2 		;/* 4 Bytes to Jump	*/
	;'		return *adr ;
	;'	}

		mov	edx, [TabBlock]			; pto = (ULONG*)TabBlock
		add	edx, [ edx + eax*4 - 4]		; TabBlock + pto[numblock]

		mov	al, byte ptr[edx+1+HEADER_BLOCK]
		ret

nocode:	;' return *(ptc+1)

		mov	al, 00h
		ret

WorldCodeBrick	endp

;----------------------------------------------------------------------------

;----------------------------------------------------------------------------

;// ATTENTION: passage de parametres par registres le 04/04/96
;
;	void Map2Screen( S32 xm, S32 ym, S32 zm )  ;
;                          eax     ecx     edx
;
; optimisé le 09/04/96 (esi + 1 instruction économisés)
;
;----------------------------------------------------------------------------

Map2Screen		proc

			mov	ebx, eax	; save xm
			sub	ebx, edx	; xm -= zm

			add	edx, eax	; xm += zm

			lea	eax, [edx*8]	; *8
			lea	eax, [eax+edx*4]; *12

			mov	edx, ecx	; ym
			shl	edx, 4		; *16
			sub	edx, ecx	; *15
			sub	eax, edx
			add	eax, 240-14	; YCentre proj
			mov	YScreen, eax

			mov	eax, ebx	; restore xm-zm
			sal	ebx, 4		;	*16
			lea	ebx, [ebx + eax*8 + 320-8  -23-1]; *24 + XCentre proj
			mov	XScreen, ebx

			ret
Map2Screen		endp


;----------------------------------------------------------------------------

; SERGER


;// ATTENTION: passage de parametres par registres le 04/04/96
;
;	void DecompColonne( U8 *pts, U8 *ptd )  ;
;			     	esi      edi
;
; optimisé (ebx économisé) le 09/04/96
;
DecompColonne	proc
			xor	ecx, ecx
;----------------------------------------------- Get Nb Entity
			mov	dl, [esi]	; BL = NbEntity
			inc	esi
;----------------------------------------------- For Every Entity
NewEntity:		lodsb			; OpCode
			mov	cl, al          ; CL = AL
			and	cl, 00111111b	; CL = Repeat
			inc	cl		; One More ( 1 -> 64 )
			test	al, 11000000b	; Incrust ?
			je	JumpZero	; Yes, go...
			test	al, 01000000b	; Diff Brick ?
			jne	WriteDiffBrick	; Yes, go...
;----------------------------------------------- Write Same Brick
			lodsw			; Get NumBlock & PosBlock
			rep	stosw		; Fill Brick
			dec	dl		; Entity--
			jne	NewEntity	; Again
			ret			; End
;----------------------------------------------- Incrust ( Full off Nothing )
JumpZero:		xor	eax, eax        ; NumBlock & PosBlock = 0
			rep	stosw		; Fill
			dec	dl		; Entity--
			jne	NewEntity	; Again
			ret			; End
;----------------------------------------------- Write Diff Brick
WriteDiffBrick:		rep	movsw		; Copy NumBlock & PosBlock
			dec	dl		; Entity--
			jne	NewEntity	; Again
			ret			; end
DecompColonne		endp
;----------------------------------------------------------------------------
comment @/*
MixteColonne		proc	uses esi edi ebx,\
			pts:DWORD, ptd:DWORD

			mov	esi, pts
			mov	edi, ptd
			xor	ecx, ecx
;----------------------------------------------- Get Nb Entity
			mov	bl, [esi]	; BL = NbEntity
			inc	esi
;----------------------------------------------- For Every Entity
NewEntity:		lodsb			; OpCode
			mov	cl, al          ; CL = AL
			and	cl, 00111111b	; CL = Repeat
			inc	cl		; One More ( 1 -> 64 )
			test	al, 11000000b	; Incrust ?
			je	JumpZero	; Yes, go...
			test	al, 01000000b	; Diff Brick ?
			jne	WriteDiffBrick	; Yes, go...
;----------------------------------------------- Write Same Brick
			lodsw			; Get NumBlock & PosBlock
			rep	stosw		; Fill Brick
			dec	bl		; Entity--
			jne	NewEntity	; Again
			ret			; End
;----------------------------------------------- Incrust ( Full off Nothing )
JumpZero:		lea	edi, [edi+ecx*2]; must add ecx WORD!
			dec	bl		; Entity--
			jne	NewEntity	; Again
			ret			; End
;----------------------------------------------- Write Diff Brick
WriteDiffBrick:		rep	movsw		; Copy NumBlock & PosBlock
			dec	bl		; Entity--
			jne	NewEntity	; Again
			ret			; end
MixteColonne		endp
*/@
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
GetBlockBrick 	proc	xw:DWORD, yw:DWORD, zw:DWORD

	;' ptc = BufCube
		mov	ecx, [BufCube]

	;' + (xm*SIZE_CUBE_Y*2)
		mov	eax, xw

	;' xm = (xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512
		mov	[XMap], eax

	;' + ym*2
		mov	eax, yw

;// a l'origine, pour les wagons
;//		dec	eax

	;' ym = yw/SIZE_BRICK_Y

		shr	eax, 8			; /256
		mov	[YMap], eax

	;' + zm*3200 (SIZE_CUBE_X*SIZE_CUBE_Y*2)

		mov	eax, zw

	;' zm = (zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512
		mov	[ZMap], eax



		mov	eax, [XMap]

		or	eax, eax		; bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		lea	eax, [eax*2]		; *2
		mov	edx, eax		; =
		lea	eax, [eax*8]		; *16
		add	edx, eax		; 2  + 16 = 18
		lea	edx, [edx+eax*2]	; 18 + 32 = 50

		add	ecx, edx

		mov	eax, [YMap]
		cmp	eax, -1
		jle	col_1
		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 25
		jge	ovfw

		lea	ecx, [ecx+eax*2]

		mov	eax, [ZMap]

		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		shl	eax, 7			; *128
		mov	edx, eax		; = 128
		lea	eax, [eax*8]		; *1024
		add	edx, eax		; 128  + 1024 = 1152
		lea	edx, [edx+eax*2]	; 1152 + 2048 = 3200

		add	ecx, edx

		xor	eax, eax
		mov	al, [ecx]

		ret

col_1:		mov	[YMap], 0
		mov	al, 1
		ret

ovfw:		mov	al, 0
		ret


GetBlockBrick	endp

;*--------------------------------------------------------------------------*

GetWorldColBrickVisible 	proc	xw:DWORD, yw:DWORD, zw:DWORD

	;' ptc = BufCube
		mov	ecx, [BufCube]

	;' + (xm*SIZE_CUBE_Y*2)
		mov	eax, xw

	;' xm = (xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512
		mov	[XMap], eax

	;' + ym*2
		mov	eax, yw
	;' ym = yw/SIZE_BRICK_Y

		shr	eax, 8			; /256
		mov	[YMap], eax

	;' + zm*3200 (SIZE_CUBE_X*SIZE_CUBE_Y*2)

		mov	eax, zw

	;' zm = (zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ

;'		add	eax, DEMI_BRICK_XZ	; +256
		shr	eax, 9			; /512
		mov	[ZMap], eax



		mov	eax, [XMap]

		or	eax, eax		; bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		lea	eax, [eax*2]		; *2
		mov	edx, eax		; =
		lea	eax, [eax*8]		; *16
		add	edx, eax		; 2  + 16 = 18
		lea	edx, [edx+eax*2]	; 18 + 32 = 50

		add	ecx, edx

		mov	eax, [YMap]
		cmp	eax, -1
		jle	col_1
		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 25
		jge	ovfw

		lea	ecx, [ecx+eax*2]

		mov	eax, [ZMap]

		or	eax, eax		; check bound
		jl	ovfw
		cmp	eax, 64
		jge	ovfw

		shl	eax, 7			; *128
		mov	edx, eax		; = 128
		lea	eax, [eax*8]		; *1024
		add	edx, eax		; 128  + 1024 = 1152
		lea	edx, [edx+eax*2]	; 1152 + 2048 = 3200

		add	ecx, edx

		xor	eax, eax
		mov	al, [ecx]

		or	al, al
		jz	nocol

	;'	if( *ptc != 0 )
	;'	{
	;'		block = (*ptc++) - 1 ;
	;'		adr = GetAdrBlock( block )	;
	;'		adr += HEADER_BLOCK		;
	;'		adr += (*ptc)<<2 		;/* 4 Bytes to Jump	*/
	;'		return *adr ;
	;'	}

		mov	edx, [TabBlock]			; pto = (ULONG*)TabBlock
		add	edx, [ edx + eax*4 - 4]		; TabBlock + pto[numblock]

		add	edx, HEADER_BLOCK


;	essai	mov	al, byte ptr[edx+1]		; code jeu
;		mov	[CodeJeu], al


		mov	al, byte ptr[ecx+1]
		lea	edx, [edx + eax*4 ]

		mov	al, [edx]

		ret

col_1:		mov	[YMap], 0
		mov	eax, 1
		ret

nocol:
ovfw:		mov	eax, 0
		ret

GetWorldColBrickVisible	endp

;----------------------------------------------------------------------------
;				The
				End




















