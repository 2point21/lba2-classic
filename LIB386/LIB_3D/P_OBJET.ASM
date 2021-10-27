		.386p
		jumps

		.model SMALL, C

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                  ÛÜ ÜÛ       Û฿฿฿Û Û฿฿Û      Û Û฿฿฿฿ ฿฿Û฿฿
;                  ÛÛ฿ Û       ÛÛ  Û ÛÛ฿฿Û ÜÜ  Û ÛÛ฿฿    ÛÛ
;                  ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿   ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		include		p_define.ash

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.data
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

		; extern M_TRIGO

		extrn	NoLanguage CameraXr:WORD
		extrn	NoLanguage CameraYr:WORD
		extrn	NoLanguage CameraZr:WORD
		extrn	NoLanguage X0:WORD
		extrn	NoLanguage Y0:WORD
		extrn	NoLanguage Z0:WORD
		extrn	NoLanguage XCentre:WORD
		extrn	NoLanguage YCentre:WORD

		extrn	NoLanguage lAlpha:WORD
		extrn	NoLanguage lBeta:WORD
		extrn	NoLanguage lGamma:WORD

		extrn	NoLanguage NormalXLight:WORD
		extrn	NoLanguage NormalYLight:WORD
		extrn	NoLanguage NormalZLight:WORD

		extrn	NoLanguage MatriceTempo:WORD
		extrn	NoLanguage MatriceWorld:WORD
		extrn	NoLanguage MatriceRot:WORD

		extrn	NoLanguage IsoScale:WORD
		extrn	NoLanguage TypeProj:WORD


		extrn	NoLanguage Mat00:WORD
		extrn	NoLanguage Mat01:WORD
		extrn	NoLanguage Mat02:WORD
		extrn	NoLanguage Mat10:WORD
		extrn	NoLanguage Mat11:WORD
		extrn	NoLanguage Mat12:WORD
		extrn	NoLanguage Mat20:WORD
		extrn	NoLanguage Mat21:WORD
		extrn	NoLanguage Mat22:WORD

		extrn	NoLanguage compteur:WORD

		; extern LIB_GRAF

		extrn	NoLanguage NbPolyPoints:WORD
		extrn	NoLanguage TabPoly:WORD
		extrn	NoLanguage TypePoly:WORD

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		public	NoLanguage FlagLight

		public	NoLanguage List_Point
		public	NoLanguage NbPoints

		public	NoLanguage List_Anim_Point
		public	NoLanguage List_Entity
		public	NoLanguage List_Tri

		public	NoLanguage ScreenXmin
		public	NoLanguage ScreenYmin
		public	NoLanguage ScreenXmax
		public	NoLanguage ScreenYmax

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

List_Point	dw	(500*3) dup(00)		; Xp Yp Zrot

List_Normal	dw	500	dup(00)		; surement plus

List_Anim_Point	dw	(500*3) dup(00)		; Xr Yr Zr

List_Entity	dw	5000 dup(00)		; TAILLE … determine

List_Tri	dw	(500*8) dup(00)		; entites

PosXWr		dw	00		; pos World rote de l'objet … afficher
PosYWr		dw	00
PosZWr		dw	00

ScreenXmin	dw	00
ScreenYmin	dw	00
ScreenXmax	dw	00
ScreenYmax	dw	00

NbPoints	dw	00
TotalEntite	dw	00

PointeurListTri	dd	00
StartDI		dd	00
NextDI		dd	00

ZMax		dw	00

NbGroupes	dw	00
ListGroupe	dd	00

Infos		dw	00
StartInfos	dd	00
OffsetDefPoint	dd	00
OffsetListNormal dd	00

FlagLight	dw	01

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

TabJump_2	dd	aff_E_LIGNE
		dd	aff_E_POLY
		dd	aff_E_SPHERE

;		dd	aff_E_POINT

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.code
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*


		public	NoLanguage	AffObjet


		extrn	NoLanguage	ComputePoly:PROC
		extrn	NoLanguage	ComputeSphere_A:PROC
		extrn	NoLanguage	FillVertic_A:PROC
		extrn	NoLanguage	Line_A:PROC

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

		extrn	NoLanguage	RotMatW:near
		extrn	NoLanguage	RotMat:near
		extrn	NoLanguage	WorldRot:near
		extrn	NoLanguage	Rot:near
		extrn	NoLanguage	Proj:near
		extrn	NoLanguage	RotList:near
		extrn	NoLanguage	TransRotList:near
		extrn	NoLanguage	RotListNormal:near

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿ Û฿฿฿Û Û฿฿Û      Û Û฿฿฿฿ ฿฿Û฿฿
;		ÛÛ฿฿Û ÛÛ฿฿  ÛÛ฿฿  ÛÛ  Û ÛÛ฿฿Û ÜÜ  Û ÛÛ฿฿    ÛÛ
;		฿฿  ฿ ฿฿    ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿   ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*


AffObjet	proc uses esi edi ebx ebp,\
		xwr:DWORD, ywr:DWORD, zwr:DWORD,\
		palpha:DWORD, pbeta:DWORD, pgamma:DWORD,\
		ptrobj:DWORD

		mov	eax, palpha
		mov	[lAlpha], ax
		mov	eax, pbeta
		mov	[lBeta], ax
		mov	eax, pgamma
		mov	[lGamma], ax

		mov	ax, 32767
		mov	[ScreenXmin], ax
		mov	[ScreenYmin], ax
		neg	ax
		mov	[ScreenXmax], ax
		mov	[ScreenYmax], ax

;*--------------------------------------------------------------------------
;* rotation world org obj

		mov	eax, xwr		; X World rot
		mov	ebx, ywr		; Y World
		mov	ecx, zwr		; Z World

		push	ebp
		call	WorldRot
		pop	ebp

		mov	ax, [CameraXr]
		sub	ax, [X0]
		mov	[PosXWr], ax

		mov	ax, [CameraYr]
		sub	ax, [Y0]
		mov	[PosYWr], ax

		mov	ax, [CameraZr]
		sub	ax, [Z0]
;	iso	or	ax, ax
;		jle	finafobjshort		; objet Z <= 0
		mov	[PosZWr], ax

;*--------------------------------------------------------------------------
;* recup infos

		mov	[TotalEntite], 0
		mov	[PointeurListTri], offset List_Tri

		mov	esi, ptrobj

		lodsw				; infos
		mov	bx, ax
		mov	[Infos], ax

		add	esi, 12			; saute ZV

		lodsw				; nb bytes to skip
		movzx	eax, ax
		add	esi, eax		; saute zone info

;*--------------------------------------------------------------------------*
;* rotation nuage/normal face/normal point

		test	bx, INFO_ANIM
		jz	normalrotate

		call	AnimNuage		; Objet Anim
		call	ComputeAnimNormal
		jmp	short finnuage

normalrotate:	call	RotateNuage		; Objet Normal
		call	ComputeStaticNormal
		jmp	short finnuage

finafobjshort:	jmp	badfinafobj_2

finnuage:

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;            Û฿฿฿Û Û฿฿฿Û Û     Û  Ü฿ Û฿฿฿฿ Û฿฿฿Û ÛÛÜ Û Û฿฿฿฿ ÛÛ฿฿฿
;            ÛÛ฿฿฿ ÛÛ  Û ÛÛ    ÛÛ฿   ÛÛ ฿Û ÛÛ  Û ÛÛ฿ÛÛ ÛÛ฿฿  ฿฿฿฿Û
;            ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		mov	edi, offset List_Entity

		lodsw				; nb polys
		movzx	ecx, ax
		jecxz	short_lignes

b_poly:		push	ecx

		mov	[StartDI], edi		; memo pointeur List_Coor

		lodsb				; matiere poly

		cmp	al, 9			; >= MAT_GOURAUD
		jae	polygouraud

		cmp	al, 7			; >= MAT_FLAT
		jae	polyflat

;-----------------------------------------------------------------------------
; MAT_TRISTE->MAT_TRAME

		mov	byte ptr[edi], al	; stock type

		lodsb				; nb point poly
		mov	byte ptr[edi+1], al	; stock nbp
		add	edi, 2
		xor	ah, ah
		mov	cx, ax

		movsw				; stock coul1/coul2

		mov	bp, 32000		; ZMax

		push	edi			; memo start poly somm

	EVEN
p0:             lodsw				; index point sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		xchg	ebx, esi
		lodsw
		mov	word ptr[edi+2], ax	; Xscr

		lodsw
		mov	word ptr[edi+4], ax	; Yscr	dans List_Coor
		add	edi, 6

		lodsw				; Zrot
;	iso	or	ax, ax
;		js	skippoly		; <0 ?

		cmp	ax, bp			; ZMin
		jge	nozmax
		mov	bp, ax
nozmax:
		mov	esi, ebx
		dec	ecx
		jnz	p0

		jmp	testpoly

short_lignes:	jmp	lignes

;-----------------------------------------------------------------------------
; MAT_GOURAUD


polygouraud:    ;cmp	[FlagLight], 0
		;jz	nolight2

		dec	al
		dec	al			; attention voir reanim
		mov	byte ptr[edi], al	; stock type translated

		lodsb				; nb point poly
		mov	byte ptr[edi+1], al	; stock nbp
		xor	ah, ah
		movzx	ecx, ax

		lodsw
		mov	word ptr[edi+2], ax	; stock coul1/coul2
		add	edi, 4
		mov	dx, ax

		mov	bp, -32000		; ZMax

		push	edi			; memo start poly somm

	EVEN
p0g:
		lodsw				; normal point
		movzx	eax, ax
		mov	ax, word ptr[List_Normal + eax * 2]
		add	al, dl
		mov	word ptr[edi+0], ax	; stock intensity point

		lodsw				; index point sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		xchg	ebx, esi
		lodsw
		mov	word ptr[edi+2], ax	; Xscr
		lodsw
		mov	word ptr[edi+4], ax	; Yscr	dans List_Coor
		add	edi, 6

		lodsw				; Zrot

		; voir pour "depthcueing" vers sombre

;	iso	or	ax, ax
;		js	skippolyg

		cmp	ax, bp			; ZMin
		jle	nozmaxg
		mov	bp, ax
nozmaxg:
		mov	esi, ebx
		dec	ecx
		jnz	p0g

		jmp	testpoly

;-----------------------------------------------------------------------------
; gouraud --> MAT_TRISTE->MAT_TRAME


nolight2:	xor	al,al
		mov	byte ptr[edi], al	; stock type

		lodsb				; nb point poly
		mov	byte ptr[edi+1], al	; stock nbp
		add	edi, 2
		xor	ah, ah
		mov	cx, ax

		movsw				; stock coul1/coul2

		mov	bp, 32000		; ZMax

		push	edi			; memo start poly somm

	EVEN
p0l:            lodsw				; oublie normal

		lodsw				; index point sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		xchg	ebx, esi
		lodsw
		mov	word ptr[edi+2], ax	; Xscr

		lodsw
		mov	word ptr[edi+4], ax	; Yscr	dans List_Coor
		add	edi, 6

		lodsw				; Zrot
;	iso	or	ax, ax
;		js	skippoly		; <0 ?

		cmp	ax, bp			; ZMin
		jge	nozmaxl
		mov	bp, ax
nozmaxl:
		mov	esi, ebx
		dec	ecx
		jnz	p0l

		jmp	testpoly

;-----------------------------------------------------------------------------
; MAT_FLAT
polyflat:	sub	al, 7
		mov	byte ptr[edi], al	; stock mat translat

		lodsb				; nb point poly
		mov	byte ptr[edi+1], al	; stock nbp
		xor	ah, ah
		movzx	ecx, ax

		lodsw
		mov	bp, ax			; coul1/coul2

		; 1st coul = normal face
		lodsw
		movzx	eax, ax
		add	bp, word ptr[ List_Normal + eax*2 ]
		mov	ax, bp
		mov	word ptr[edi+2], ax	; stock coul + intensity
		add	edi, 4

		push	edi			; memo start poly somm

		mov	bp, -32000		; ZMax

	EVEN
p0f:		lodsw				; index point sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		xchg	ebx, esi
		lodsw
		mov	word ptr[edi+2], ax	; Xscr

		lodsw
		mov	word ptr[edi+4], ax	; Yscr	dans List_Coor
		add	edi, 6

		lodsw				; Zrot
;	iso	or	ax, ax
;		js	skippoly

		cmp	ax, bp			; ZMin
		jle	nozmaxf
		mov	bp, ax
nozmaxf:
		mov	esi, ebx
		dec	ecx
		jnz	p0f

		jmp	testpoly

skippoly:	mov	esi, ebx
		dec	ecx
		shl	ecx, 1
		add     esi, ecx		; skip points restant
		pop	edi
		jmp	badpoly

skippolyg:	mov	esi, ebx
		dec	ecx
		shl	ecx, 2
		add     esi, ecx		; skip points restant
		pop	edi
		jmp	short badpoly

;-----------------------------------------------------------------------------
testpoly:	mov	[NextDI], edi		; memo pointeur List_Coor

		pop	edi			; pointeur list sommets

		mov	[ZMax], bp

		mov	bx, [edi+8]
		sub	bx, [edi+2]
		mov	ax, [edi+4]
		sub	ax, [edi+16]
		imul	bx
		mov	bp,ax
		mov	bx,dx
		mov	cx, [edi+10]
		sub	cx, [edi+4]
		mov	ax, [edi+2]
		sub	ax, [edi+14]
		imul	cx

		sub	ax, bp			; oublie face si bx:bp < dx:ax
		sbb	dx, bx
		jnl	badpoly

okpoly:
		inc	[TotalEntite]
		mov	edi, [PointeurListTri]
		mov	ax, [ZMax]
		mov	word ptr[edi], ax
		mov	ax, E_POLY
		mov	word ptr[edi+2], ax
		mov	eax, [StartDI]
		mov	dword ptr[edi+4], eax
		add	edi, 8
		mov	[PointeurListTri], edi

;-----------------------------------------------------------------------------

		mov	edi, [NextDI]
nextpoly:	pop	ecx
		dec	ecx
		jz	lignes
		jmp	b_poly

badpoly:	mov	edi, [StartDI]
		jmp	nextpoly

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                     Û      Û    Û฿฿฿฿ ÛÛÜ Û Û฿฿฿฿ ÛÛ฿฿฿
;                     ÛÛ     ÛÛ   ÛÛ ฿Û ÛÛ฿ÛÛ ÛÛ฿฿  ฿฿฿฿Û
;                     ฿฿฿฿฿  ฿฿   ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

lignes:		lodsw				; nb lignes
		movzx	ecx, ax
		jecxz	spheres

b_line:		mov	[StartDI], edi		; memo pointeur List_Coor

		movsd				; stock matiere/coul
						; coul2 / even

		lodsw				; index point 1 sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		mov	eax, [ebx+00]
		mov	dword ptr[edi], eax	; X1scr vers List_Coor
						; Y1scr
		mov	dx, [ebx+04]		; Zrot ZMin/Max

		lodsw				; index point 2 sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		mov	eax, [ebx+00]
		mov	dword ptr[edi+4], eax	; X2scr vers List_Coor
		add	edi, 8
						; Y2scr
		mov	bp, [ebx+04]		; Zrot ZMin/Max

		mov	[NextDI], edi

		cmp	dx, bp			; ZMin ZMax
		jl	zok0
		xchg	dx, bp			; DX < BP
zok0:
;	iso	or	dx, dx			; ZMin
;		jle	badline

okline:		inc	[TotalEntite]
		mov	edi, [PointeurListTri]
		mov	ax, bp
		mov	word ptr[edi], ax
		mov	ax, E_LIGNE
		mov	word ptr[edi+2], ax
		mov	eax, [StartDI]
		mov	dword ptr[edi+4], eax
		add	edi, 8
		mov	[PointeurListTri], edi

		mov	edi, [NextDI]
		dec	ecx
		jnz	b_line
		jmp	short spheres

badline:	mov	edi, [StartDI]
		loope	b_line

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                  ÛÛ฿฿฿ Û฿฿฿Û Û   Û Û฿฿฿฿ Û฿฿฿Û Û฿฿฿฿ ÛÛ฿฿฿
;                  ฿฿฿฿Û ÛÛ฿฿฿ ÛÛ฿฿Û ÛÛ฿฿  ÛÛ฿Û฿ ÛÛ฿฿  ฿฿฿฿Û
;		   ฿฿฿฿฿ ฿฿    ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

spheres:	lodsw				; nb spheres
		movzx	ecx, ax
		jecxz	tri

b_sphere:	mov	[StartDI], edi		; memo pointeur List_Coor
		movsd				; stock matiere/coul
						; coul2 / even
		movsw				; rayon
		lodsw				; index point 1 sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		mov	eax, [ebx+00]		; X1scr vers List_Coor
		mov	dword ptr[edi], eax	; Y1scr
		mov	ax, [ebx+04]		; Zrot ZMin/Max
		mov	word ptr[edi+4], ax
		add	edi, 6

		mov	[NextDI], edi

;	iso	or	ax, ax			; ZMin
;		jle	badsphere

oksphere:	inc	[TotalEntite]
		mov	edi, [PointeurListTri]
		mov	word ptr[edi], ax
		mov	ax, E_SPHERE
		mov	word ptr[edi+2], ax
		mov	eax, [StartDI]
		mov	dword ptr[edi+4], eax
		add	edi, 8
		mov	[PointeurListTri], edi

		mov	edi, [NextDI]
		dec	ecx
		jnz	b_sphere
		jmp	short tri

badsphere:	mov	edi, [StartDI]
		dec	ecx
		jnz	b_sphere

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;         ฿฿Û฿฿ Û฿฿฿Û  Û          Û฿฿฿฿ ÛÛÜ Û ฿฿Û฿฿  Û    ฿฿Û฿฿ Û  Ü฿
;           ÛÛ  ÛÛ฿Û฿  ÛÛ         ÛÛ฿฿  ÛÛ฿ÛÛ   ÛÛ   ÛÛ     ÛÛ  ÛÛ฿
;           ฿฿  ฿฿  ฿  ฿฿   ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿   ฿฿   ฿฿     ฿฿  ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; tri les faces dans la table Listtri (ordre Z et offset sur list coors
; premachees.

tri:		movzx	ecx, word ptr[TotalEntite]
		dec	ecx
		jz	fintri

		mov	esi, offset List_Tri
		lea	ebp, List_Tri + ecx*8

SergeSort::	; esi	bas
		; ebp	haut
		; ecx	nbr d'elements

		; ax	pivot
		; edi	pospivot
		; ebx	h

		push	0			; flag de fin de pile !

		jmp	nocalccx

plusgrand:    	mov	ebx, edi
		mov	eax, [edi]
		jmp	short cont

permut:		; ecx nb entite - 1
		; esi start list sizeof 8 byte

tt0:            mov	ebp, ecx
		mov	eax, [esi]		; lit Z

		mov	edi, esi
		add	edi, 8
		xor	ebx, ebx

tt1:		cmp	[edi], ax
		jg	plusgrand
cont:		add	edi, 8
		dec	ecx
		jnz	tt1

		or	ebx, ebx
		jz	short noexchg

		xchg	[esi], eax		; permutte
		mov	[ebx], eax
		mov	eax, [esi+04]
		xchg	[ebx+04], eax
		mov	[esi+04], eax

noexchg:	add	esi, 8
		mov	ecx, ebp
		dec	ecx
		jnz	tt0

		pop	ebp
		or	ebp, ebp
		jnz	short dopop
		jmp	fintri

permut2:	mov	eax, [esi]
		mov	ebx, [ebp]

		cmp	ax, bx
		jge	short skipswap

		mov	[esi], ebx
		mov	[ebp], eax
		mov	eax, [esi+04]
		xchg	[ebp+04], eax
		mov	[esi+04], eax
skipswap:
		pop	ebp
		or	ebp, ebp
		jz	short fintri

dopop:		pop	esi
nopop:
		mov	ecx, ebp
		sub	ecx, esi
		shr	ecx, 3
nocalccx:
		cmp	ecx, 1			; select permut2 ?
		je	short permut2

		cmp	ecx, 7			; select permut ou qsort
		jbe	short permut

		mov	edi, esi		; pospivot = bas

		mov	eax, [edi]		; pivot = [pospivot] = [bas]

		add	esi, 8			; bas++

		mov	ebx, ebp		; h = haut

w1:		cmp	word ptr[esi], ax
		jl	short w2
		add	esi, 8			; si [bas] >= pivot
		dec	ecx
		jnz	w1

		jmp	short w4

w2:		cmp	word ptr[ebx], ax
		jg	short w3
		sub	ebx, 8			; si [h] <= pivot
		dec	ecx
		jnz	w2

		jmp	short w4

w3:     	mov	edx, [esi]		; xchg [bas],[h]
		xchg	edx, [ebx]
		mov	[esi], edx
		mov	edx, [esi+4]
		xchg	edx, [ebx+4]
		mov	[esi+4], edx

		jmp	short w1
w4:
		xchg	esi, ebx		; idem que esi-=8 et ebx+=8

		xchg	[esi], eax		; xchg [pospivot],[bas]
		mov	[edi], eax
		mov	edx, [edi+4]
		xchg	edx, [esi+4]
		mov	[edi+4], edx

		cmp	ebx, ebp
		jae	short nopush

		push	ebx			; h
		push	ebp			; haut
nopush:
		sub	esi, 8
		cmp	edi, esi
		jae	short norecur

		mov	ebp, esi
		mov	esi, edi
		jmp	nopop
norecur:
		pop	ebp
		or	ebp, ebp
		jnz	dopop
fintri:

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                  Û฿฿฿Ü  Û    ÛÛ฿฿฿ Û฿฿฿Û Û     Û฿฿฿Û Û  Ü฿
;        	   ÛÛ  Û  ÛÛ   ฿฿฿฿Û ÛÛ฿฿฿ ÛÛ    ÛÛ฿฿Û ÛÛ฿
;                  ฿฿฿฿   ฿฿   ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿  ฿ ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; affichage des entits

		mov	esi,offset List_Tri

		movzx	ecx, [TotalEntite]
		jcxz	badfinafobj_1

ade0:           push	ecx
		inc	esi
		inc	esi			; saute Z max

		lodsw				; type entit
		movzx	ebx, ax
		lodsd				; offset List_Coor
		push	esi
		mov	esi, eax

		jmp	[ TabJump_2 + ebx*4 ]	; jmp … aff de l'entit

nextaff:        pop	esi
		pop	ecx
		dec	ecx
		jnz	ade0

goodfinobj:	xor	eax, eax		; OK affich au moins 1 entit
		ret

;*--------------------------------------------------------------------------*/

badfinafobj_1:					; 1 si pas d'entite to aff
		mov	ax,-1
		mov	[ScreenXmax], ax
		mov	[ScreenYmax], ax
		mov	[ScreenXmin], ax
		mov	[ScreenYmin], ax
		mov	eax, 1
		ret

badfinafobj_2:	mov	ax, -1
		mov	[ScreenXmax], ax
		mov	[ScreenYmax], ax
		mov	[ScreenXmin], ax
		mov	[ScreenYmin], ax

		mov	eax, 2			; 2 si objet clipp en Z <= 0
		ret

AffObjet	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		      Û฿฿฿฿ ÛÛÜ Û ฿฿Û฿฿  Û    ฿฿Û฿฿ Û  Ü฿
;		      ÛÛ฿฿  ÛÛ฿ÛÛ   ÛÛ   ÛÛ     ÛÛ  ÛÛ฿
;		      ฿฿฿฿฿ ฿฿  ฿   ฿฿   ฿฿     ฿฿  ฿฿
;
;		   Û฿฿฿Ü  Û    ÛÛ฿฿฿ Û฿฿฿Û Û     Û฿฿฿Û Û  Ü฿
;		   ÛÛ  Û  ÛÛ   ฿฿฿฿Û ÛÛ฿฿฿ ÛÛ    ÛÛ฿฿Û ÛÛ฿
;		   ฿฿฿฿   ฿฿   ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿  ฿ ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
aff_E_POLY::
		lodsb				; type (ATTENTION GOURAUD)
		xor	ah, ah
		mov	bx, ax
		mov	[TypePoly], ax

		lodsb				; nb points
		movzx	ecx, ax
		mov	[NbPolyPoints], ax

		lodsw				; coul1/coul2
		mov	dx, ax

		mov	edi, ecx
		shl	ecx, 1
		add	ecx, edi		; *3 word
		mov	edi, offset TabPoly
		rep	movsw			; … changer

		mov	si, bx			; type
		mov	di, dx			; coul

		call	ComputePoly
		or	ax, ax
		jz	no_poly

		mov	cx, si			; type
		call	FillVertic_A

no_poly:	jmp	nextaff

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
aff_E_LIGNE::
		lodsw
		xchg	al, ah
		mov	bp, ax
		inc	esi
		inc	esi

		lodsw				; x0
		movsx	edx, ax
		lodsw				; y0
		movsx	ebx, ax
		lodsw				; x1
		movsx	ecx, ax
		lodsw				; y1
		movsx	eax, ax
		xchg	eax, edx

		call	Line_A

		jmp	nextaff

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
aff_E_SPHERE::
		xor	eax, eax
		lodsb				; type
		push	eax			; mov	[TypePoly], ax

		lodsw				; coul1/coul2
		push	eax

		inc	esi			; even

		lodsw				; rayon
		movzx	ebp, ax
		lodsw				; x
		movzx	ecx, ax
		lodsw				; y
		movzx	edi, ax
		lodsw				; Zr

;	iso	xchg	eax, ebp
;		imul	word ptr[LFactorX]
;		add	bp, [KFactor]
;		idiv	bp
;		movzx	ebp, ax				pfeu

		mov	ax, 34			; racine sxý+syý
		imul	bp
		idiv	word ptr[IsoScale]
		movzx	ebp, ax

		; ecx x
		; edi y -> esi pour computesphere
		; ebp rayon sur ecran

		; reajuste coordonne box

		mov	ax, cx			; x + rayon
		add	ax, bp
		cmp	ax, [ScreenXmax]
		jle	nosxa
		mov	[ScreenXmax], ax
nosxa:		mov	ax, cx			; x - rayon
		sub	ax, bp
		cmp	ax, [ScreenXmin]
		jge	nosxi
		mov	[ScreenXmin], ax
nosxi:		mov	ax, di
		add	ax, bp			; y + rayon
		cmp	ax, [ScreenYmax]
		jle	nosya
		mov	[ScreenYmax], ax
nosya:		mov	ax, di			; y - rayon
		sub	ax, bp
		cmp	ax, [ScreenYmin]
		jge	nosyi
		mov	[ScreenYmin], ax
nosyi:

		mov	esi, edi
		call	ComputeSphere_A

		pop	edi			; coul
		pop	ecx			; type

		or	eax, eax
		jz	no_sphere

		call	FillVertic_A

no_sphere:	jmp	nextaff

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; sous programmes
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*


;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		Û฿฿฿Û ÛÛÜ Û  Û    ÛÜ ÜÛ       Û฿฿฿Û Û฿฿฿Û ฿฿Û฿฿
;		ÛÛ฿฿Û ÛÛ฿ÛÛ  ÛÛ   ÛÛ฿ Û       ÛÛ฿Û฿ ÛÛ  Û   ÛÛ
;		฿฿  ฿ ฿฿  ฿  ฿฿   ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿   ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

RotateGroupe	proc	near

		mov	[lAlpha], bx
		mov	[lBeta],  cx
		mov	[lGamma], dx

		push	esi			; save group en cours


		movzx	ebx, word ptr[esi+6]	; orggroupe deja * 38
		cmp	bx, -1
		jnz	pasgroupe0

		mov	esi, offset MatriceWorld
		mov	edi, offset MatriceTempo
		mov	ecx, 4
		rep	movsd
		movsw

		mov	dword ptr[X0], ecx	; XYZ0 ChgRepere
		mov	[Z0], cx

		jmp	short groupe0

pasgroupe0:


		movzx	eax, word ptr[esi+04]		; Org Point index deja *6
		lea	edi, List_Anim_Point + eax	; di sur List_Anim_Point + Org
		mov	eax, dword ptr[edi]		; X0 et Y0
		mov	dword ptr[X0], eax
		mov	ax, [edi+4]			; Z0
		mov	[Z0], ax			; XYZ0 ChgRepere

		add	ebx, 20
		mov	esi, [ListGroupe]
		add	esi, ebx		; Matrice du groupe org
		mov	edi, offset MatriceTempo
		mov	ecx, 4
		rep	movsd
		movsw				; copy Mgroup org vers Mtempo

groupe0:	call	RotMat			; rot de Mtempo vers Mrot

		pop	edi			;
		add	edi, 20			; Mgroup en cours
		mov	esi, offset MatriceRot
		mov	ecx, 4
		rep	movsd			; copy de Mrot vers Mgroup
		movsw
		mov	esi, edi
		sub	esi, 38			; group en cours

		movzx	eax, word ptr[esi+0]	; Start point deja *6
		movzx	ecx, word ptr[esi+2]	; nb points

		mov	esi, [OffsetDefPoint]
		add	esi, eax		; si sur Def_Point + Start

		lea	edi, List_Anim_Point + eax	; di sur List_Point + Start

		call	RotList

		ret

RotateGroupe	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

TranslateGroupe	proc	near

		mov	[lAlpha], bx		; attention !
		mov	[lBeta],  cx		; ici step de translation
		mov	[lGamma], dx

		mov	edi, esi		; save group en cours

		movzx	ebx, word ptr[esi+6]	; orggroupe deja * 38
		cmp	bx, -1
		jnz	pasgroupe0

		add	edi, 20			; Mgroup en cours
		mov	esi, offset MatriceWorld
		mov	ecx, 4
		rep	movsd
		movsw

		mov	dword ptr[X0], ecx
		mov	[Z0], cx		; ChgRepere

		jmp	short groupe0

pasgroupe0:

		movzx	ecx, word ptr[esi+4]			; Org Point index deja *6
		mov	eax, dword ptr[List_Anim_Point + ecx ]	; X0 et Y0
		mov	dword ptr[X0], eax
		mov	ax, word ptr[List_Anim_Point + ecx + 4 ]; Z0
		mov	[Z0], ax				; ChgRepere

		mov	esi, [ListGroupe]
		add	ebx, 20
		add	esi, ebx		; Mgroup org
		add	edi, 20			; Mgroup en cours
		mov	ecx, 4
		rep	movsd			; copy de Mgroup org vers Mgroup
		movsw

groupe0:	mov	esi, edi
		sub	esi, 38			; group en cours

		movzx	eax, word ptr[esi+0]	; Start point deja *6
		mov	dx, [esi+2]		; nb points

		mov	edi, offset MatriceRot
		add	esi, 20
		mov	ecx, 4
		rep	movsd			; Mgroup vers Mrot
		movsw

		mov	esi, [OffsetDefPoint]
		add	esi, eax			; si sur List_Point + Start

		lea	edi, List_Anim_Point + eax

		movzx	ecx, dx
		call	TransRotList

		ret

TranslateGroupe	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

ifdef	Poubelle
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

ZoomGroupe	proc	near

;		bx zoomX cx zoomY dx zoomZ

		inc	bh			; ZoomX + 256
		inc	ch			; ZoomY + 256
		inc	dh			; ZoomZ + 256
		mov	bp, dx			; bp zoomZ

		mov	di, [si+0]		; Start point deja *6
		mov	dx, [si+2]		; nb points

		mov	si, offset List_Anim_Point
		add	si, di			; si sur List_Point + Start

		push	ds

		mov	di, es
		mov	ds, di			; ds sur dgroup
	EVEN
rg0:		push	dx			; beurk

		mov	ax, [si]		; Delta X
		imul	bx
		mov	al, ah			; /256
		mov	ah, dl
		mov	[si], ax
		inc	si
		inc	si

		mov	ax, [si]		; Delta Y
		imul	cx
		mov	al, ah			; /256
		mov	ah, dl
		mov	[si], ax
		inc	si
		inc	si

		mov	ax, [si]		; Delta Z
		imul	bp
		mov	al, ah			; /256
		mov	ah, dl
		mov	[si], ax
		inc	si
		inc	si

		pop	dx
		dec	dx
		jnz	rg0

		pop	ds
		ret

ZoomGroupe	endp

endif

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;         Û฿฿฿Û ÛÛÜ Û  Û    ÛÜ ÜÛ       ÛÛÜ Û Û   Û Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿
;         ÛÛ฿฿Û ÛÛ฿ÛÛ  ÛÛ   ÛÛ฿ Û       ÛÛ฿ÛÛ ÛÛ  Û ÛÛ฿฿Û ÛÛ ฿Û ÛÛ฿฿
;         ฿฿  ฿ ฿฿  ฿  ฿฿   ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

AnimNuage	proc	near

		lodsw				; nb points
		movsx	eax, ax
		mov	[NbPoints], ax

		mov	[OffsetDefPoint], esi
		mov	ecx, eax
		shl	ecx, 1			; *2
		add	ecx, eax		; *3
		shl	ecx, 1			; *6
		add	esi, ecx		; esi saute defpoint

		lodsw				; Nb groupes anims
		mov	[NbGroupes], ax
		movzx	ecx, ax

		mov	[ListGroupe], esi	; memo start list groupe

;*--------------------------------------------------------------------------*/
; rotations du groupe 0

		mov	bx, [lAlpha]		; SI sur groupe 0
		mov	cx, [lBeta]		; init aux valeurs de rot locale
		mov	dx, [lGamma]

		push	esi
		call	RotateGroupe
		pop	esi
		add	esi, 38			; size d'un groupe

;*--------------------------------------------------------------------------*/
; rotations/translations/zooms des groupes

		movzx	ecx, [NbGroupes]
		dec	ecx			; - groupe 0
		jcxz	ag0f

ag0:            push	ecx
		push	esi

		mov	bx, [esi+10]		; Alpha ou stepX
		mov	cx, [esi+12]		; Beta ou stepY
		mov	dx, [esi+14]		; Gamma ou stepZ

		mov	ax, [esi+8]		; type anim groupe

		cmp	ax, TYPE_ROTATE
		jz	rotateg
		cmp	ax, TYPE_TRANSLATE
		jz	translateg
		cmp	ax, TYPE_ZOOM
		jnz	nolocalrot

zoomg:		;call	ZoomGroupe
		jmp	short nolocalrot

translateg:	call	TranslateGroupe
		jmp	short nolocalrot

rotateg:	call	RotateGroupe

nolocalrot:     pop	esi
		add	esi, 38			; size d'un groupe
		pop	ecx
		dec	ecx
		jnz	ag0

ag0f:
;*--------------------------------------------------------------------------*/
; projette liste

		push	esi

project:     	movzx	ecx, [NbPoints]

		mov	esi, offset List_Anim_Point
		mov	edi, offset List_Point

boucleproj:	push	ecx

		lodsw				; eax X rot
		sub	ax, [PosXWr]
		movsx	ecx, ax

		lodsw				; ebx Y rot
		sub	ax, [PosYWr]
		movsx	ebx, ax

		lodsw				; ecx Z rot
		movsx	eax, ax			; ou cwde ?
		xchg	eax, ecx
		movsx	ebp, [PosZWr]
		sub	ebp, ecx		; bp = Zrot tri

;--------------------------------------------
;		cmp	word ptr[TypeProj], PROJ_ISO
;		jz	projiso

;	iso	js	nopoint			; si Z0 < 0 clip
;		cmp	ax, cx			; si X0 > Z0 clip
;		jg	nopoint
;		cmp	bx, cx			; si Y0 > Z0 clip
;		jg	nopoint

;		neg	cx
;		cmp	ax, cx
;		jl	nopoint
;		cmp	bx, cx
;		jl	nopoint
;		neg	cx


projiso:	mov	ecx, eax		; save x

		add	eax, ebp		; x + zrot

		sal	eax, 3			; *8
		mov	edx, eax
		sal	eax, 1			; *16
		add	eax, edx		; *24

		mov	edx, eax
		sar	edx, 16			;
		idiv	word ptr[IsoScale]	; (x+zrot)/IsoScale

		add	ax, [XCentre]
		mov	word ptr[edi], ax
		add	edi, 2			; stock Xp

		cmp	ax, [ScreenXmin]
		jl	sxmin
nosxmin:	cmp	ax, [ScreenXmax]
		jg	sxmax
nosxmax:
		mov	eax, ecx		; restore x
		sub	eax, ebp		; x - zrot
		sub	ebp, ecx		; Zrot - X pour bon tri

		sal	eax, 2			; *4
		mov	edx, eax
		sal	eax, 1			; *8
		add	eax, edx		; *12

		mov	ecx, eax		; ecx = (x-zrot) * 12

		sal	ebx, 1			; *2
		mov	eax, ebx		; = 2
		sal	eax, 4			; *32
		sub	eax, ebx

		sub	eax, ecx		; - (x-zrot) * 12

		mov	edx, eax
		sar	edx, 16
		idiv	word ptr[IsoScale]

		neg	ax
		add	ax, [YCentre]
		mov	word ptr[edi], ax
		add	edi, 2				; stock Yp

		cmp	ax, [ScreenYmin]
		jl	symin
nosymin:	cmp	ax, [ScreenYmax]
		jg	symax
nosymax:

		mov	ax, bp
		mov	word ptr[edi], ax
		add	edi, 2				; stock Zrot pour tri

;--------------------------------------------

pl0:		pop	ecx
		dec	ecx
		jnz	boucleproj

		pop	esi

;*--------------------------------------------------------------------------*

finanim:        mov	eax, -1
		ret

sxmin:		mov	[ScreenXmin], ax
		jmp	nosxmin
sxmax:		mov	[ScreenXmax],ax
		jmp	nosxmax
symin:		mov	[ScreenYmin], ax
		jmp	nosymin
symax:		mov	[ScreenYmax],ax
		jmp	nosymax

nopoint:	xor	eax, eax
		mov	word ptr[edi], ax
		mov	word ptr[edi+2], ax
		mov	ax, -1
		mov	word ptr[edi+6], ax
		add	edi, 8
		jmp     pl0

AnimNuage	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;   Û฿฿฿Û Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿฿       ÛÛÜ Û Û   Û Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿
;   ÛÛ฿Û฿ ÛÛ  Û   ÛÛ  ÛÛ฿฿Û   ÛÛ  ÛÛ฿฿        ÛÛ฿ÛÛ ÛÛ  Û ÛÛ฿฿Û ÛÛ ฿Û ÛÛ฿฿
;   ฿฿  ฿ ฿฿฿฿฿   ฿฿  ฿฿  ฿   ฿฿  ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

RotateNuage	proc	near

		push	esi

		call	RotMatW			; Rotate MWorld vers Mrot

		pop	esi

	; rotationS nuage de points

		lodsw				; nbobj points
		mov	[NbPoints], ax
		movzx	ecx, ax

		mov	edi,offset List_Point

bouclerot:	push	ecx
		push	edi

	; rotation totale de l'objet

		lodsw
		mov	cx, ax			; AX X reel
		lodsw
		mov	bx, ax			; BX Y reel
		lodsw
		xchg	ax, cx			; CX Z reel

		call	Rot			; voir pour Coor rotes
						; en LONG * 2^15
		pop	edi

	; change repere (pos camera reelle & pos objet reelle)

		mov	ax, [X0]
		sub	ax, [PosXWr]

		mov	bx, [Y0]
		sub	bx, [PosYWr]

		mov	cx, [PosZWr]
		sub	cx, [Z0]
		mov	bp, cx

		js	nopoint			; si Z0 < 0 clip
		cmp	ax, cx			; si X0 > Z0 clip
		jg	nopoint
		cmp	bx, cx			; si Y0 > Z0 clip
		jg	nopoint
		neg	cx
		cmp	ax, cx
		jl	nopoint
		cmp	bx, cx
		jl	nopoint
		neg	cx

	; projection point

		call	Proj

		mov	word ptr[edi], ax	; stock Xp

		cmp	ax,[ScreenXmin]
		jge	nosxmin
		mov	[ScreenXmin],ax
nosxmin:	cmp	ax,[ScreenXmax]
		jle	nosxmax
		mov	[ScreenXmax],ax
nosxmax:
		mov	ax,bx
		mov	word ptr[edi+2], ax	; stock Yp

		cmp	ax,[ScreenYmin]
		jge	nosymin
		mov	[ScreenYmin],ax
nosymin:	cmp	ax,[ScreenYmax]
		jle	nosymax
		mov	[ScreenYmax],ax
nosymax:
		mov	ax,cx
		mov	word ptr[edi+4], ax	; stock Zrot
		add	edi, 6

rn0:		pop	ecx
		dec	ecx
		jnz	bouclerot

		ret

nopoint:	xor	ax, ax
		mov	word ptr[edi], ax
		mov	word ptr[edi+2], ax
		mov	ax, -1
		mov	word ptr[edi+4], ax
		add	edi, 6
		jmp     rn0

RotateNuage	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		   ÛÛÜ Û Û฿฿฿Û Û฿฿฿Û ÛÜ ÜÛ Û฿฿฿Û Û     ÛÛ฿฿฿
;		   ÛÛ฿ÛÛ ÛÛ  Û ÛÛ฿Û฿ ÛÛ฿ Û ÛÛ฿฿Û ÛÛ    ฿฿฿฿Û
;		   ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿  ฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

ComputeStaticNormal	proc	near

		lodsw				; nbobj normal faces/points
		mov	cx, ax
		jcxz	nonp

		mov	edi, offset List_Normal

bouclerot:	push	ecx
		push	edi

		lodsw
		mov	dx, ax			; AX X
		lodsw
		mov	bx, ax			; BX Y
		lodsw
		mov	cx, ax			; CX Z
		lodsw
		xchg	dx, ax			; DX prenormalized range

		call	Rot			; DX inchang

		pop	edi

		mov	bp, dx

		mov	ax, [X0]

		imul	word ptr[NormalXLight]
		mov	cx, ax
		mov	bx, dx

		mov	ax, [Y0]
		imul	word ptr[NormalYLight]
		add	cx, ax
		adc	bx, dx

		mov	ax, [Z0]
		imul	word ptr[NormalZLight]
		add	cx, ax
		adc	bx, dx

		mov	ax, cx
		mov	dx, bx			; intensity

		or	dx,dx
		js	nointensity		; saut si negatif

		idiv	bp
		mov	word ptr[edi], ax
		add	edi, 2			; stock intensity

		pop	ecx
		dec	ecx
		jnz	bouclerot

nonp:		ret

nointensity:	xor	ax, ax
		mov	word ptr[edi], ax
		add	edi, 2
		pop	ecx
		dec	ecx
		jnz	bouclerot

		ret

ComputeStaticNormal	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

nextgroup2:	pop	ebx
		add	ebx, 38
		pop	ecx
		dec	ecx
		jnz	bouclegroup
		jmp	nonp

ComputeAnimNormal	proc	near

		lodsw				; nbobj normal faces/points
		or	ax, ax
		jz	nonp

		mov	edi,offset List_Normal

		movzx	ecx, [NbGroupes]

		mov	ebx, [ListGroupe]
		add	ebx, 18			; sur NbNormal

bouclegroup:    push	ecx
		push	ebx

		mov	cx, [ebx]
		jcxz	nextgroup2

		; copy Mgroup vers Mrot

		mov	edx, esi
		mov	ebp, edi
		mov	esi, ebx
		inc	esi
		inc	esi			; sur Mgroup

		mov	edi, offset MatriceRot
		mov	ecx, 4
		rep	movsd
		movsw
		mov	edi, ebp
		mov	esi, edx

		movzx	ecx, word ptr[ebx]

;		cmp	[FlagLight], 0
;		jz	nolight1

;		call	RotListNormal

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
		mov	[compteur], cx

bouclerot:	;push	ecx
		push	edi

		movsx	ebp, word ptr[esi+0]
		movsx   ebx, word ptr[esi+2]
		movsx	ecx, word ptr[esi+4]
		add	esi, 6

		movsx	eax, word ptr[ Mat00 ]
		imul	eax, ebp
		mov	edx, eax
		movsx	eax, word ptr[ Mat01 ]
		imul	eax, ebx
		add	edx, eax
		movsx	eax, word ptr[ Mat02 ]
		imul	eax, ecx
		add	eax, edx
		sar	eax, 15

		movsx	edx, word ptr[NormalXLight]
		imul	eax, edx
		mov	edi, eax

		movsx	eax, word ptr[ Mat10 ]
		imul	eax, ebp
		mov	edx, eax
		movsx	eax, word ptr[ Mat11 ]
		imul	eax, ebx
		add	edx, eax
		movsx	eax, word ptr[ Mat12 ]
		imul	eax, ecx
		add	eax, edx
		sar	eax, 15

		movsx	edx, word ptr[NormalYLight]
		imul	eax, edx
		add	edi, eax

		movsx	eax, word ptr[ Mat20 ]
		imul	eax, ebp
		mov	edx, eax
		movsx	eax, word ptr[ Mat21 ]
		imul	eax, ebx
		add	edx, eax
		movsx	eax, word ptr[ Mat22 ]
		imul	eax, ecx
		add	eax, edx
		sar	eax, 15

		movsx	edx, word ptr[NormalZLight]
		imul	eax, edx
		add	edi, eax

		or	edi, edi
		js	nointensity		; saut si negatif

		lodsw				; prenormalized range
		xchg	di, ax
		cwd
		idiv	di

		pop	edi
		mov	word ptr[edi], ax
		add	edi, 2			; stock intensity

;		pop	ecx
;		dec	ecx
		dec	word ptr[compteur]
		jnz	bouclerot

		jmp	nextgroup

nointensity:    lodsw
		xor	ax, ax
		pop	edi
		mov	word ptr[edi], ax
		add	edi, 2

;		pop	ecx
;		dec	ecx
		dec	word ptr[compteur]
		jnz	bouclerot

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

nextgroup:	pop	ebx
		add	ebx, 38

		pop	ecx
		dec	ecx
		jnz	bouclegroup
;		dec	ecx
		jnz	bouclegroup

nonp:		ret

nolight1:	mov	ebx, ecx
		xor	ax, ax
		rep	stosw
		shl	ebx, 3
		add	esi, ebx
		jmp	short nextgroup

ComputeAnimNormal	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

		END
