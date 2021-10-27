		.386p
                jumps

		.model SMALL, SYSCALL

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

		extrn	NoLanguage CameraXr:DWORD
		extrn	NoLanguage CameraYr:DWORD
		extrn	NoLanguage CameraZr:DWORD
		extrn	NoLanguage X0:DWORD
		extrn	NoLanguage Y0:DWORD
		extrn	NoLanguage Z0:DWORD
		extrn	NoLanguage XCentre:DWORD
		extrn	NoLanguage YCentre:DWORD

		extrn	NoLanguage lAlpha:DWORD
		extrn	NoLanguage lBeta:DWORD
		extrn	NoLanguage lGamma:DWORD

		extrn	NoLanguage NormalXLight:DWORD
		extrn	NoLanguage NormalYLight:DWORD
		extrn	NoLanguage NormalZLight:DWORD

		extrn	NoLanguage LMatriceWorld:DWORD

;		extrn	NoLanguage IsoScale:WORD
		extrn	NoLanguage TypeProj:WORD

		extrn	NoLanguage KFactor:DWORD
		extrn	NoLanguage LFactorX:DWORD
		extrn	NoLanguage LFactorY:DWORD

		extrn	NoLanguage LMat00:DWORD
		extrn	NoLanguage LMat01:DWORD
		extrn	NoLanguage LMat02:DWORD
		extrn	NoLanguage LMat10:DWORD
		extrn	NoLanguage LMat11:DWORD
		extrn	NoLanguage LMat12:DWORD
		extrn	NoLanguage LMat20:DWORD
		extrn	NoLanguage LMat21:DWORD
		extrn	NoLanguage LMat22:DWORD

		extrn	NoLanguage TabMat:DWORD

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

List_Entity	dw	5000	dup(00)		; TAILLE … determine

List_Tri	dw	(500*8) dup(00)		; entites

ScreenXmin	dw	00
ScreenYmin	dw	00
ScreenXmax	dw	00
ScreenYmax	dw	00

NbPoints	dw	00
TotalEntite	dw	00

ZMax		dw	00

NbGroupes	dw	00

Infos		dw	00

Count1		dw	00

FlagLight	dw	01

ALIGN 4

PointeurListTri	dd	00
StartDI		dd	00
NextDI		dd	00

ListGroupe	dd	00

StartInfos	dd	00
OffsetDefPoint	dd	00
OffsetListNormal dd	00

Save1		dd	00
Save2		dd	00
Save3		dd	00
Ptr1		dd	00
Ptr2		dd	00

PosXWr		dd	00		; pos World rote de l'objet … afficher
PosYWr		dd	00
PosZWr		dd	00

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

TabJump_2	dd	aff_E_LIGNE
		dd	aff_E_POLY
		dd	aff_E_SPHERE

;		dd	aff_E_POINT

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.code
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*


		public	NoLanguage	PatchObjet
		public	NoLanguage	AffObjetIso


		extrn	NoLanguage	ComputePoly_A:PROC
;		extrn	NoLanguage	ComputePoly:PROC
		extrn	NoLanguage	ComputeSphere_A:PROC
		extrn	NoLanguage	FillVertic_A:PROC
		extrn	NoLanguage	Line_A:PROC

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

		extrn	NoLanguage	RotMatW:near
		extrn	NoLanguage	RotMatIndex2:near
		extrn	NoLanguage	WorldRot:near
		extrn	NoLanguage	LongWorldRot:near
		extrn	NoLanguage	Rot:near
;		extrn	NoLanguage	Proj:near
		extrn	NoLanguage	Proj_3D:near
		extrn	NoLanguage	Proj_ISO:near
		extrn	NoLanguage	RotList:near
;		extrn	NoLanguage	RotList16:near
		extrn	NoLanguage	TransRotList:near
;		extrn	NoLanguage	RotListNormal:near

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿ Û฿฿฿Û Û฿฿Û      Û Û฿฿฿฿ ฿฿Û฿฿
;		ÛÛ฿฿Û ÛÛ฿฿  ÛÛ฿฿  ÛÛ  Û ÛÛ฿฿Û ÜÜ  Û ÛÛ฿฿    ÛÛ
;		฿฿  ฿ ฿฿    ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿   ฿฿ ISO
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*


PatchObjet	proc uses esi ebx ebp,\
		ptrobj:DWORD

		mov	esi, ptrobj

		test	word ptr[esi], INFO_ANIM
		jz	exit

		movzx	eax, word ptr[esi+14]	; nb bytes to skip
		lea	esi, [esi+eax+16]	; saute zone info

		movzx	eax, word ptr[esi]	; nb points
		add	esi, 2

		lea	eax, [eax+eax*2]	; *3
		lea	esi, [esi+eax*2]	; *6, esi saute defpoint

		mov	cx, [esi]
		add	esi, 2

		dec	cx			; - groupe 0
		jz	exit

		mov	bp, 36
		mov	bx, 38

ag0:            add	esi, 38			; size d'un groupe
		mov	ax, [esi+6]		; orggroupe * 38
		mul	bp
		div	bx			; orggroupe * 36
		mov	[esi+6], ax
		dec	cx
		jnz	ag0
exit:
		ret

PatchObjet	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

AffObjetIso	proc uses esi edi ebx ebp,\
		xwr:DWORD, ywr:DWORD, zwr:DWORD,\
		palpha:DWORD, pbeta:DWORD, pgamma:DWORD,\
		ptrobj:DWORD

		mov	eax, palpha
		mov	[lAlpha], eax
		mov	eax, pbeta
		mov	[lBeta], eax
		mov	eax, pgamma
		mov	[lGamma], eax

		mov	ax, 32767
		mov	[ScreenXmin], ax
		mov	[ScreenYmin], ax
		neg	ax
		mov	[ScreenXmax], ax
		mov	[ScreenYmax], ax

;*--------------------------------------------------------------------------
;* rotation world org obj

		cmp	word ptr[TypeProj], TYPE_3D
		jnz	iso0

		mov	eax, xwr		; X World rot
		mov	ebx, ywr		; Y World
		mov	ecx, zwr		; Z World

		mov	[Save1], ebp
		call	LongWorldRot
		mov	ebp, [Save1]

		mov	eax, [X0]
		sub	eax, [CameraXr]
		mov	[PosXWr], eax

		mov	eax, [Y0]
		sub	eax, [CameraYr]
		mov	[PosYWr], eax

		mov	eax, [Z0]
		sub	eax, [CameraZr]
		mov	[PosZWr], eax

		jmp	iso1

iso0:
		mov	eax, xwr		; X World rot
		mov	[PosXWr], eax
		mov	eax, ywr		; Y World rot
		mov	[PosYWr], eax
		mov	eax, zwr		; Z World rot
		mov	[PosZWr], eax

iso1:
;*--------------------------------------------------------------------------
;* recup infos

		mov	[TotalEntite], 0
		mov	[PointeurListTri], offset List_Tri

		mov	esi, ptrobj

		mov	bx, [esi]		; infos
		mov	[Infos], bx

		movzx	eax, word ptr[esi+14]	; nb bytes to skip
		lea	esi, [esi+eax+16]	; saute zone info

;*--------------------------------------------------------------------------*
;* rotation nuage/normal face/normal point


		test	bx, INFO_ANIM
		jnz	AnimNuage
		jmp	RotateNuage

;		test	bx, INFO_ANIM
;		jz	normalrotate

;		call	AnimNuage		; Objet Anim
;		call	ComputeAnimNormal
;		jmp	short finnuage
;normalrotate:
;		call	RotateNuage		; Objet Normal
;		call	ComputeStaticNormal

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;            Û฿฿฿Û Û฿฿฿Û Û     Û  Ü฿ Û฿฿฿฿ Û฿฿฿Û ÛÛÜ Û Û฿฿฿฿ ÛÛ฿฿฿
;            ÛÛ฿฿฿ ÛÛ  Û ÛÛ    ÛÛ฿   ÛÛ ฿Û ÛÛ  Û ÛÛ฿ÛÛ ÛÛ฿฿  ฿฿฿฿Û
;            ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

finnuage::
		mov	edi, offset List_Entity

		mov	cx, word ptr[esi]	; nb polys
		add	esi, 2
		or	cx, cx
		jz	lignes

		mov	[Count1], cx
b_poly:
		mov	[StartDI], edi		; memo pointeur List_Coor

		mov	ecx, [esi]		; matiere poly + nb point poly
						; (+ coul1/coul2)
		add	esi, 2

		cmp	cl, 9			; >= MAT_GOURAUD
		jae	polygouraud

		cmp	cl, 7			; >= MAT_FLAT
		jae	polyflat

;-----------------------------------------------------------------------------
; MAT_TRISTE->MAT_TRAME

		mov	[edi], ecx		; stock type + nbp
						; stock coul1/coul2
		add	esi, 2
		add	edi, 4

		mov	bp, -32000		; ZMax

		mov	[Ptr1], edi		; memo start poly somm

		xor	eax, eax
	EVEN
p0:             lodsw				; index point sur List_Point
		mov	edx, [eax+offset List_Point]; index dj… *SIZE_LIST_POINT
		mov	dword ptr[edi+2], edx	; Xscr & Yscr	dans List_Coor
		add	edi, 6

		mov	dx, word ptr[eax+offset List_Point+4]; Zrot
		cmp	dx, bp			; ZMin
		jle	nozmax
		mov	bp, dx
nozmax:
		dec	ch
		jnz	p0

		jmp	testpoly

;-----------------------------------------------------------------------------
; MAT_FLAT
polyflat:	sub	cl, 7
		mov	[edi], cx		; stock mat translat + nbp

		mov	bp, [esi]		; coul1/coul2

		xor	eax, eax

		; 1st coul = normal face
		mov	ax, [esi+2]
		add	esi, 4
		add	bp, word ptr[ List_Normal + eax*2 ]
		mov	word ptr[edi+2], bp	; stock coul + intensity
		add	edi, 4

		mov	[ptr1], edi		; memo start poly somm

		mov	bp, -32000		; ZMax

	EVEN
p0f:		lodsw				; index point sur List_Point
		mov	edx, [eax+offset List_Point]; index dj… *SIZE_LIST_POINT
		mov	dword ptr[edi+2], edx	; Xscr & Yscr dans List_Coor
		add	edi, 6

		mov	dx, word ptr[eax+offset List_Point+4]; Zrot
		cmp	dx, bp			; ZMin
		jle	nozmaxf
		mov	bp, dx
nozmaxf:
		dec	ch
		jnz	p0f

		jmp	testpoly

comment	@

skippoly:	mov	esi, ebx
		dec	ecx
		shl	ecx, 1
		add     esi, ecx		; skip points restant
		mov	edi, [Ptr1]
		jmp	badpoly

skippolyg:	mov	esi, ebx
		dec	ecx
		shl	ecx, 2
		add     esi, ecx		; skip points restant
		mov	edi, [Ptr1]
		jmp	badpoly

	@

comment	@

;-----------------------------------------------------------------------------
; gouraud --> MAT_TRISTE->MAT_TRAME


nolight2:	xor	cl, cl
		mov	[edi], cx		; stock type + nbp
		add	edi, 2			; stock nbp

		movsw				; stock coul1/coul2

		mov	bp, 32000		; ZMax

		mov	[ptr1], edi		; memo start poly somm

	EVEN
p0l:            add	esi, 2			; oublie normal

		lodsw				; index point sur List_Point
		movzx	ebx, ax			; index dj… *SIZE_LIST_POINT
		add	ebx, offset List_Point

		xchg	ebx, esi
		lodsd
		mov	dword ptr[edi+2], eax	; Xscr & Yscr dans List_Coor
		add	edi, 6

		lodsw				; Zrot
;	iso	or	ax, ax
;		js	skippoly		; <0 ?

		cmp	ax, bp			; ZMin
		jge	nozmaxl
		mov	bp, ax
nozmaxl:
		mov	esi, ebx
		dec	ch
		jnz	p0l

		jmp	testpoly

	@

;-----------------------------------------------------------------------------
; MAT_GOURAUD


polygouraud:    ;cmp	[FlagLight], 0
		;jz	nolight2

		sub	cl, 2			; attention voir reanim
		mov	[edi], ecx		; stock type translated + nbp
						; + stock coul1/coul2
		add	esi, 2
		add	edi, 4

		shr	ecx, 8			; cl = nbp, ch = coul1

		mov	bp, -32000		; ZMax

		mov	[Ptr1], edi		; memo start poly somm

		xor	eax, eax
	EVEN
p0g:
		mov	ax, [esi]		; normal point
		mov	dx, word ptr[List_Normal+eax*2]
		add	dl, ch
		mov	word ptr[edi], dx	; stock intensity point

		mov	ax, [esi+2]		; index point sur List_Point
		add	esi, 4
		mov	edx, [eax+offset List_Point]; index dj… *SIZE_LIST_POINT
		mov	dword ptr[edi+2], edx	; Xscr & Yscr dans List_Coor
		add	edi, 6

		mov	dx, word ptr[eax+offset List_Point+4]; Zrot

		; voir pour "depthcueing" vers sombre

		cmp	dx, bp			; ZMin
		jle	nozmaxg
		mov	bp, dx
nozmaxg:
		dec	cl
		jnz	p0g

;-----------------------------------------------------------------------------
testpoly:	mov	[NextDI], edi		; memo pointeur List_Coor

		mov	edi, [Ptr1]		; pointeur list sommets

		mov	[ZMax], bp

		mov	ax, [edi+4]
		mov	bx, [edi+8]
		sub	ax, [edi+16]
		sub	bx, [edi+2]
		imul	bx
		mov	bp,ax
		mov	bx,dx
		mov	ax, [edi+2]
		mov	cx, [edi+10]
		sub	ax, [edi+14]
		sub	cx, [edi+4]
		imul	cx

		sub	ax, bp			; oublie face si bx:bp < dx:ax
		sbb	dx, bx
		jnl	badpoly

okpoly:
		inc	[TotalEntite]
		mov	edi, [PointeurListTri]
		mov	ax, [ZMax]
		mov	word ptr[edi], ax
		mov	word ptr[edi+2], E_POLY
		mov	eax, [StartDI]
		mov	dword ptr[edi+4], eax
		add	edi, 8
		mov	[PointeurListTri], edi

;-----------------------------------------------------------------------------

		mov	edi, [NextDI]
nextpoly:	dec	word ptr[Count1]
		jnz	b_poly

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                     Û      Û    Û฿฿฿฿ ÛÛÜ Û Û฿฿฿฿ ÛÛ฿฿฿
;                     ÛÛ     ÛÛ   ÛÛ ฿Û ÛÛ฿ÛÛ ÛÛ฿฿  ฿฿฿฿Û
;                     ฿฿฿฿฿  ฿฿   ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

lignes:		mov	cx, [esi]		; nb lignes
		add	esi, 2
		or	cx, cx
		jz	spheres

		add	[TotalEntite], cx

		mov	edx, [PointeurListTri]

		xor	ebx, ebx

b_line:		mov	eax, [esi]		; stock matiere/coul
		mov	[edi], eax		; coul2 / even


		mov	bx, [esi+4]		; index point 1 sur List_Point

		mov	eax, [ebx+offset List_Point+00]
		mov	dword ptr[edi+4], eax	; X1scr vers List_Coor
						; Y1scr
		mov	bp, word ptr[ebx+offset List_Point+04]; Zrot ZMin/Max


		mov	bx, [esi+6]		; index point 2 sur List_Point
		add	esi, 8

		mov	eax, [ebx+offset List_Point+00]
		mov	dword ptr[edi+8], eax	; X2scr vers List_Coor
						; Y2scr

		cmp	word ptr[ebx+offset List_Point+04], bp; ZMin ZMax
		jl	zok0
		mov	bp, word ptr[ebx+offset List_Point+04]
zok0:
		mov	word ptr[edx], bp
		mov	word ptr[edx+2], E_LIGNE
		mov	dword ptr[edx+4], edi
		add	edx, 8
		add	edi, 12

		dec	cx
		jnz	b_line

		mov	[PointeurListTri], edx

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                  ÛÛ฿฿฿ Û฿฿฿Û Û   Û Û฿฿฿฿ Û฿฿฿Û Û฿฿฿฿ ÛÛ฿฿฿
;                  ฿฿฿฿Û ÛÛ฿฿฿ ÛÛ฿฿Û ÛÛ฿฿  ÛÛ฿Û฿ ÛÛ฿฿  ฿฿฿฿Û
;		   ฿฿฿฿฿ ฿฿    ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

spheres:	mov	cx, [esi]		; nb spheres
		add	esi, 2
		or	cx, cx
		jz	tri

		add	[TotalEntite], cx

		mov	edx, [PointeurListTri]

b_sphere:	mov	eax, [esi]		; stock matiere/coul
		mov	ebx, [esi+4]		; rayon & index point 1 sur List_Point
		mov	[edi], eax		; coul2 / even
		mov	[edi+4], bx		; rayon
		add	esi, 8

		shr	ebx, 16				; index point 1 sur List_Point
		mov	eax, [ebx+offset List_Point+00]	; X1scr vers List_Coor
		mov	dword ptr[edi+6], eax		; Y1scr
		mov	ax, word ptr[ebx+offset List_Point+04]	; Zrot ZMin/Max
		mov	word ptr[edi+10], ax

		mov	word ptr[edx], ax
		mov	word ptr[edx+2], E_SPHERE
		mov	dword ptr[edx+4], edi
		add	edi, 12
		add	edx, 8

		dec	cx
		jnz	b_sphere

		mov	[PointeurListTri], edx

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;         ฿฿Û฿฿ Û฿฿฿Û  Û          Û฿฿฿฿ ÛÛÜ Û ฿฿Û฿฿  Û    ฿฿Û฿฿ Û  Ü฿
;           ÛÛ  ÛÛ฿Û฿  ÛÛ         ÛÛ฿฿  ÛÛ฿ÛÛ   ÛÛ   ÛÛ     ÛÛ  ÛÛ฿
;           ฿฿  ฿฿  ฿  ฿฿   ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿   ฿฿   ฿฿     ฿฿  ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; tri les faces dans la table Listtri (ordre Z et offset sur list coors
; premachees.

tri:		movzx	ecx, word ptr[TotalEntite]
		dec	ecx
		jle	fintri

		mov	esi, offset List_Tri
		lea	ebp, [esi+ecx*8]

SergeSort::	; esi	bas
		; ebp	haut
		; cx	nbr d'elements

		; ax	pivot
		; edi	pospivot
		; ebx	h

		push	0			; flag de fin de pile !

		jmp	nocalccx

plusgrand:    	mov	ebx, edi
		mov	eax, [edi]
		jmp	short cont

permut:		; cx nb entite - 1
		; esi start list sizeof 8 byte

		mov	bp, cx

tt0:            mov	eax, [esi]		; lit Z

		lea	edi, [esi+8]
		xor	ebx, ebx

tt1:		cmp	[edi], ax
		jg	plusgrand
cont:		add	edi, 8
		dec	cx
		jnz	tt1

		or	ebx, ebx
		jz	short noexchg

		xchg	[esi], eax		; permutte
		mov	[ebx], eax
		mov	eax, [esi+04]
		xchg	[ebx+04], eax
		mov	[esi+04], eax

noexchg:	add	esi, 8
		dec	bp
		mov	cx, bp
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
		cmp	cx, 1			; select permut2 ?
		je	short permut2

		cmp	cx, 7			; select permut ou qsort
		jbe	short permut

		mov	edi, esi		; pospivot = bas

		mov	eax, [esi]		; pivot = [pospivot] = [bas]

		add	esi, 8			; bas++

		mov	ebx, ebp		; h = haut

w1:		cmp	word ptr[esi], ax
		jl	short w2
		add	esi, 8			; si [bas] >= pivot
		dec	cx
		jnz	w1

		jmp	short w4

w2:		cmp	word ptr[ebx], ax
		jg	short w3
		sub	ebx, 8			; si [h] <= pivot
		dec	cx
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

		mov	cx, [TotalEntite]
		or	cx, cx
		jz	badfinafobj_1

		mov	[Count1], cx
		mov	[Ptr1], esi

ade0:           movzx	ebx, word ptr[esi+2]	; type entit
		mov	esi, [esi+4]		; offset List_Coor
		add	[Ptr1], 8

		jmp	[ TabJump_2 + ebx*4 ]	; jmp … aff de l'entit

nextaff::       mov	esi, [Ptr1]
		dec	word ptr[Count1]
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

;*--------------------------------------------------------------------------*/

badpoly:	mov	edi, [StartDI]
		jmp	nextpoly

AffObjetIso	endp

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
		xor	ecx, ecx
		mov	ebx, ecx

		lodsd
		mov	bl, al			; type
		mov	cl, ah			; nb points
		shr	eax, 16			; coul1/coul2

		mov	[TypePoly], bx
		mov	[NbPolyPoints], cx

		lea	ecx, [ecx+ecx*2]	; *3 word
		mov	edi, offset TabPoly
		shr	ecx, 1
		jnc	dod
		movsw
dod:		rep	movsd


		mov	di, ax			; coul pour fillv

		call	ComputePoly_A
		or	ax, ax
		jz	nextaff

		mov	cx, bx			; type
		call	FillVertic_A

		jmp	nextaff

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
aff_E_LIGNE::
		mov	ax, [esi]
		xchg	al, ah
		mov	bp, ax

		mov	ebx, [esi+4]
		movsx	eax, bx			; x0
		sar	ebx, 16			; y0
		mov	edx, [esi+8]
		movsx	ecx, dx			; x1
		sar	edx, 16			; y1

		call	Line_A

		jmp	nextaff

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
aff_E_SPHERE::
		movzx	eax, byte ptr[esi]	; type
		mov	[save1], eax		; mov	[TypePoly], ax

		mov	ax, [esi+1]		; coul1/coul2
		mov	[save2], eax

		mov	ecx, [esi+4]
		movzx	ebp, cx			; rayon
		shr	ecx, 16			; x
		movzx	edi, word ptr[esi+8]	; y

		cmp	word ptr[TypeProj], TYPE_3D
		jnz	isosph

		mov	ax, bp			; rayon
		imul	word ptr[LFactorX]
		mov	bp, word ptr[esi+10]	; Z
		add	bp, word ptr[KFactor]
		idiv	bp
		movzx	ebp, ax
		jmp	short affsph

		;		mov	ax, 34			; racine sxý+syý
		;		imul	bp
		;		idiv	word ptr[IsoScale]
		;		movzx	ebp, ax
isosph:		imul	ebp, 34			; racine sxý+syý
		sar	ebp, 9			; /512 IsoScale

		; ecx x
		; edi y -> esi pour computesphere
		; ebp rayon sur ecran

		; reajuste coordonne box

affsph:		mov	ax, cx			; x + rayon
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

		or	eax, eax
		jz	nextaff

		mov	edi, [save2]		; coul
		mov	ecx, [save1]		; type

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

		mov	[lAlpha], ebx
		mov	[lBeta],  ecx
		mov	[lGamma], edx

		movzx	eax, word ptr[esi]	; Start point deja *6
		mov	[Save2], eax
		movzx	eax, word ptr[esi+2]	; nb points
		mov	[Save3], eax

		movzx	ebp, word ptr[esi+6]	; orggroupe deja * 36
		cmp	bp, -1
		jne	pasgroupe0

		mov	ebp, offset LMatriceWorld

		mov	[X0], 0			; XYZ0 ChgRepere
		mov	[Y0], 0
		mov	[Z0], 0

		jmp	short groupe0

pasgroupe0:	add	ebp, offset TabMat			; EBP Matrice du groupe org

		movzx	edx, word ptr[esi+04]			; Org Point index deja *6
		mov	eax, dword ptr[List_Anim_Point+edx]	; X0 & Y0
		movsx	edi, ax
		sar	eax, 16
		mov	[X0], edi
		mov	[Y0], eax
		movsx	eax, word ptr[List_Anim_Point+edx+4]	; Z0
		mov	[Z0], eax				; XYZ0 ChgRepere
groupe0:
		mov	eax, [Ptr1]		; Mgroup

		call	RotMatIndex2		; rot de Mtempo vers Mrot & Mgroup

		mov	ecx, [Save2]

		mov	esi, [OffsetDefPoint]
		add	esi, ecx		; si sur Def_Point + Start

		lea	edi, [List_Anim_Point + ecx]; di sur List_Point + Start

		mov	ecx, [Save3]

		jmp	RotList

RotateGroupe	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

TranslateGroupe	proc	near

		mov	[lAlpha], ebx		; attention !
		mov	[lBeta],  ecx		; ici step de translation
		mov	[lGamma], edx

		mov	edx, esi		; save group en cours

		movzx	esi, word ptr[esi+6]	; orggroupe deja * 38
		cmp	si, -1
		jne	pasgroupe0

		mov	[X0], 0
		mov	[Y0], 0
		mov	[Z0], 0			; ChgRepere

		mov	esi, offset LMatriceWorld

		jmp	short groupe0

pasgroupe0:	add	esi, offset TabMat	; Mgroup org

		movzx	ecx, word ptr[edx+4]			; Org Point index deja *6
		mov	eax, dword ptr[List_Anim_Point+ecx]	; X0 & Y0
		movsx	edi, ax
		sar	eax, 16
		mov	[X0], edi
		mov	[Y0], eax
		movsx	eax, word ptr[List_Anim_Point + ecx + 4]; Z0
		mov	[Z0], eax				; XYZ0 ChgRepere

groupe0:	mov	eax, [Ptr1]

		mov	edi, eax
		mov	ecx, 9
		rep	movsd

		mov	ecx, [edx]
		movzx	edx, cx			; Start point deja *6

		mov	esi, [OffsetDefPoint]
		add	esi, edx		; si sur List_Point + Start

		lea	edi, [List_Anim_Point + edx]

		shr	ecx, 16			; nb points

		jmp	TransRotList

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

		movzx	eax, word ptr[esi]	; nb points
		add	esi, 2
		mov	[NbPoints], ax

		mov	[OffsetDefPoint], esi
		lea	eax, [eax+eax*2]	; *3
		lea	esi, [esi+eax*2]	; *6, esi saute defpoint

		lodsw
		mov	[NbGroupes], ax

		mov	[ListGroupe], esi	; memo start list groupe

;*--------------------------------------------------------------------------*/
; rotations du groupe 0

		mov	ebx, [lAlpha]		; SI sur groupe 0
		mov	ecx, [lBeta]		; init aux valeurs de rot locale
		mov	edx, [lGamma]

		mov	[Ptr1], offset TabMat

		mov	[Save1], esi

		call	RotateGroupe

		mov	esi, [Save1]
		add	esi, 38			; size d'un groupe
		mov	[Save1], esi

;*--------------------------------------------------------------------------*/
; rotations/translations/zooms des groupes

		mov	cx, [NbGroupes]
		dec	cx			; - groupe 0
		jz	ag0f

		mov	[Count1], cx

		mov	[Ptr1], offset TabMat + 36

ag0:            mov	ebx, [esi+8]
		mov	ax, bx			; type anim groupe
		sar	ebx, 16			; Alpha ou stepX
		mov	edx, [esi+12]
		movsx	ecx, dx			; Beta ou stepY
		sar	edx, 16			; Gamma ou stepZ

		cmp	ax, TYPE_ROTATE
		jz	rotateg
		cmp	ax, TYPE_TRANSLATE
		jnz	nolocalrot

;		jz	translateg
;		cmp	ax, TYPE_ZOOM
;		jnz	nolocalrot

;zoomg:		call	ZoomGroupe
;		jmp	short nolocalrot

translateg:	call	TranslateGroupe
		jmp	short nolocalrot

rotateg:	call	RotateGroupe

nolocalrot:	add	[Ptr1], 36		; next matrix

		mov	esi, [Save1]
		add	esi, 38			; size d'un groupe
		mov	[Save1], esi

		dec	word ptr[Count1]
		jnz	ag0

ag0f:

;*--------------------------------------------------------------------------*/
; projette liste

project:     	mov	cx, [NbPoints]
		mov	[Count1], cx

		mov	esi, offset List_Anim_Point
		mov	edi, offset List_Point

		cmp	word ptr[TypeProj], TYPE_3D
		jz	boucleproj3d



boucleproj:	movsx	eax, word ptr[esi]	; eax X rot
		add	eax, [PosXWr]

		movsx	ebx, word ptr[esi+2]	; ebx Y rot
		add	ebx, [PosYWr]

		movsx   ebp, word ptr[esi+4]
		add     ebp, [PosZWr]
		neg     ebp             	; Z rot, bp = Zrot tri

		add	esi, 6

projiso:	lea	ecx, [ebp+eax]		; x + zrot
		lea	ecx, [ecx*8]		; *8
		lea	ecx, [ecx+ecx*2]	; *24

		sar	ecx, 9			; /512 IsoScale

		add	cx, word ptr[XCentre]
		mov	word ptr[edi], cx	; stock Xp

		cmp	cx, [ScreenXmin]
		jl	sxmin
nosxmin:	cmp	cx, [ScreenXmax]
		jg	sxmax
nosxmax:
		mov	ecx, eax
		sub	eax, ebp		; x - zrot
		sub	ebp, ecx		; Zrot - X pour bon tri

		lea	eax, [eax*4]		; *4
		lea	eax, [eax+eax*2]	; *12
						; eax = (x-zrot) * 12

; essai de correction tri par influence du y
		sub	bp, bx

		lea	ecx, [ebx+ebx]		; *2
		shl	ebx, 5			; 32
		sub	ecx, ebx		; - y*30

		add	eax, ecx		; + (x-zrot) * 12

		;	mov	edx, eax
		;	sar	edx, 16
		;	idiv	word ptr[IsoScale]

		sar	eax, 9			; /512 IsoScale

		add	ax, word ptr[YCentre]
		mov	word ptr[edi+2], ax	; stock Yp

		cmp	ax, [ScreenYmin]
		jl	symin
nosymin:	cmp	ax, [ScreenYmax]
		jg	symax
nosymax:

		mov	word ptr[edi+4], bp	; stock Zrot pour tri
		add	edi, 6

;--------------------------------------------

pl0:		dec	word ptr[Count1]
		jnz	boucleproj


		mov	esi, [Save1]
		mov	eax, -1
		jmp	ComputeAnimNormal
;		ret

;*--------------------------------------------------------------------------*
;*--------------------------------------------------------------------------*
;*--------------------------------------------------------------------------*


sxmin:		mov	[ScreenXmin], cx
		jmp	nosxmin
sxmax:		mov	[ScreenXmax], cx
		jmp	nosxmax
symin:		mov	[ScreenYmin], ax
		jmp	nosymin
symax:		mov	[ScreenYmax], ax
		jmp	nosymax

sxmin3d:	mov	[ScreenXmin], ax
		jmp	nosxmin3d
sxmax3d:	mov	[ScreenXmax], ax
		jmp	nosxmax3d
symin3d:	mov	[ScreenYmin], ax
		jmp	nosymin3d
symax3d:	mov	[ScreenYmax], ax
		jmp	nosymax3d

overflow:	mov	ebp, 7FFFFFFFh	; max value
		jmp	fixed

overX:		shr	eax, 16
		or	ax, 7FFFh
		jmp	fixedX

overY:		shr	eax, 16
		or	ax, 7FFFh
		jmp	fixedY

overZ:		shr	ebp, 16
		or	bp, 7FFFh
		jmp	fixedZ


boucleproj3D:	movsx	eax, word ptr[esi]	; eax X rot
		add	eax, [PosXWr]

		movsx	ebx, word ptr[esi+2]	; ebx Y rot
		add	ebx, [PosYWr]

		movsx	ebp, word ptr[esi+4]	; Z rot, bp = Zrot tri
		add	ebp, [PosZWr]
		neg     ebp

		add	esi, 6

		add	ebp, [KFactor]
		jle	overflow

fixed:		imul	[LFactorX]		; X
		idiv	ebp
		add	eax, [XCentre]

		cmp	eax, 0FFFFh
		ja	overX
fixedX:
		mov	word ptr[edi], ax	; stock Xp

		cmp	ax, [ScreenXmin]
		jl	sxmin3d
nosxmin3d:	cmp	ax, [ScreenXmax]
		jg	sxmax3d
nosxmax3d:

		mov	eax, ebx
		neg	eax
		imul	[LFactorY]		; -Y
		idiv	ebp
		add	eax, [YCentre]

		cmp	eax, 0FFFFh
		ja	overY
fixedY:
		mov	word ptr[edi+2], ax	; stock Yp

		cmp	ax, [ScreenYmin]
		jl	symin3d
nosymin3d:	cmp	ax, [ScreenYmax]
		jg	symax3d
nosymax3d:
		cmp	ebp, 0FFFFh
		ja	overZ
fixedZ:
		mov	word ptr[edi+4], bp	; stock Zrot pour tri
		add	edi, 6

;--------------------------------------------

pl03d:		dec	word ptr[Count1]
		jnz	boucleproj3d

;*--------------------------------------------------------------------------*

		mov	esi, [Save1]

finanim:
;	        mov	eax, -1
;		ret

;AnimNuage	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		   ÛÛÜ Û Û฿฿฿Û Û฿฿฿Û ÛÜ ÜÛ Û฿฿฿Û Û     ÛÛ฿฿฿
;		   ÛÛ฿ÛÛ ÛÛ  Û ÛÛ฿Û฿ ÛÛ฿ Û ÛÛ฿฿Û ÛÛ    ฿฿฿฿Û
;		   ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿  ฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

;ComputeAnimNormal	proc	near

ComputeAnimNormal:

		lodsw				; nbobj normal faces/points
		or	ax, ax
		jz	nonp

		mov	edi, offset List_Normal
		mov	[Ptr1],	edi

		mov	eax, offset TabMat
		mov	[Ptr2], eax

		mov	cx, [NbGroupes]
		mov	word ptr[Count1], cx

		mov	ebx, [ListGroupe]
		add	ebx, 18			; sur NbNormal
		mov	[Save1], ebx

bouclegroup:    mov	cx, [ebx]
		or	cx, cx
		jz	nextgroup

		mov	[compteur], cx

		; copy Mgroup vers Mrot & Premultiply par Normallight

		mov	ecx, [NormalXlight]
		mov	edx, [eax+LT00]
		mov	ebx, [eax+LT01]
		mov	ebp, [eax+LT02]
		imul	edx, ecx
		imul	ebx, ecx
		imul	ebp, ecx
		mov	[LMat00], edx
		mov	[LMat01], ebx
		mov	[LMat02], ebp

		mov	ecx, [NormalYlight]
		mov	edx, [eax+LT10]
		mov	ebx, [eax+LT11]
		mov	ebp, [eax+LT12]
		imul	edx, ecx
		imul	ebx, ecx
		imul	ebp, ecx
		mov	[LMat10], edx
		mov	[LMat11], ebx
		mov	[LMat12], ebp

		mov	ecx, [NormalZlight]
		mov	edx, [eax+LT20]
		mov	ebx, [eax+LT21]
		mov	ebp, [eax+LT22]
		imul	edx, ecx
		imul	ebx, ecx
		imul	ebp, ecx
		mov	[LMat20], edx
		mov	[LMat21], ebx
		mov	[LMat22], ebp

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
bouclerot:
		mov	ebx, [esi]
		movsx	ebp, bx
		sar	ebx, 16
		movsx	ecx, word ptr[esi+4]

		mov	eax, [ LMat00 ]
		mov	edx, [ LMat01 ]
		imul	eax, ebp
		imul	edx, ebx
		add	eax, edx
		mov	edx, [ LMat02 ]
		imul	edx, ecx
		add	eax, edx

		mov	edi, [ LMat10 ]
		mov	edx, [ LMat11 ]
		imul	edi, ebp
		imul	edx, ebx
		add	eax, edi
		mov	edi, [ LMat12 ]
		imul	edi, ecx
		add	eax, edx
		add	eax, edi

		xor	di, di			; 0 intensity for now

		imul	ebp, [ LMat20 ]
		imul	ebx, [ LMat21 ]
		add     eax, ebp
		imul	ecx, [ LMat22 ]
		add	eax, ebx
		add	eax, ecx

		js	nointensity		; saut si negatif

		sar	eax, 14
		cwd
		idiv    word ptr[esi+6]		; prenormalized range
		mov	di, ax

nointensity:	mov	eax, [Ptr1]
		mov	word ptr[eax], di	; stock intensity
		add	eax, 2
		mov	[Ptr1], eax

		add	esi, 8

		dec	word ptr[compteur]
		jnz	bouclerot

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

nextgroup:
		mov	ebx, [Save1]
		mov	eax, [Ptr2]
		add	ebx, 38
		add	eax, 36
		mov	[Save1], ebx
		mov	[Ptr2], eax

		dec	word ptr[Count1]
		jnz	bouclegroup
nonp:
		jmp	FinNuage
;		ret

;ComputeAnimNormal	endp

AnimNuage	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;   Û฿฿฿Û Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿฿       ÛÛÜ Û Û   Û Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿
;   ÛÛ฿Û฿ ÛÛ  Û   ÛÛ  ÛÛ฿฿Û   ÛÛ  ÛÛ฿฿        ÛÛ฿ÛÛ ÛÛ  Û ÛÛ฿฿Û ÛÛ ฿Û ÛÛ฿฿
;   ฿฿  ฿ ฿฿฿฿฿   ฿฿  ฿฿  ฿   ฿฿  ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

RotateNuage	proc	near

		mov     [Save1], esi
		call	RotMatW			; Rotate MWorld vers Mrot
		mov     esi, [Save1]

	; rotationS nuage de points

		mov	cx, word ptr[esi]	; nbobj points
		add	esi, 2
		mov	[NbPoints], cx

		mov	edi,offset List_Point

		mov	[Count1], cx
		mov	[Ptr1], edi

bouclerot2:

	; rotation totale de l'objet

		mov	ax, [esi]		; AX X reel
		mov	bx, [esi+2]		; BX Y reel
		mov	cx, [esi+4]		; CX Z reel
		add	esi, 6

		call	Rot			; voir pour Coor rotes
						; en LONG * 2^15

	; change repere (pos camera reelle & pos objet reelle)

		mov	ax, word ptr[X0]
		sub	ax, word ptr[PosXWr]

		mov	bx, word ptr[Y0]
		sub	bx, word ptr[PosYWr]

		mov	bp, word ptr[Z0]
		add	bp, word ptr[PosZWr]
		neg     bp

	; projection point

; passer les param en long !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		mov	word ptr[Save2], bp
		call	Proj_ISO
		mov	bp, word ptr[Save2]

		mov	edi, [Ptr1]

		mov	word ptr[edi], ax	; stock Xp

		cmp	ax,[ScreenXmin]
		jge	nosxmin
		mov	[ScreenXmin],ax
nosxmin:	cmp	ax,[ScreenXmax]
		jle	nosxmax
		mov	[ScreenXmax],ax
nosxmax:
		mov	word ptr[edi+2], bx	; stock Yp

		cmp	bx,[ScreenYmin]
		jge	nosymin
		mov	[ScreenYmin],bx
nosymin:	cmp	bx,[ScreenYmax]
		jle	nosymax
		mov	[ScreenYmax],bx
nosymax:
		mov	word ptr[edi+4], bp	; stock Zrot

		add	dword ptr[Ptr1], 6

rn0:		dec	word ptr[Count1]
		jnz	bouclerot2

;		jmp	ComputeStaticNormal
;		ret

;RotateNuage	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		   ÛÛÜ Û Û฿฿฿Û Û฿฿฿Û ÛÜ ÜÛ Û฿฿฿Û Û     ÛÛ฿฿฿
;		   ÛÛ฿ÛÛ ÛÛ  Û ÛÛ฿Û฿ ÛÛ฿ Û ÛÛ฿฿Û ÛÛ    ฿฿฿฿Û
;		   ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿  ฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

;ComputeStaticNormal	proc	near

		mov	cx, [esi]		; nbobj normal faces/points
		add	esi, 2
		or	cx, cx
		jz	nonp

		mov	edi, offset List_Normal

		mov	[Count1], cx
		mov	[Ptr1], edi

bouclerot:	mov	ax, [esi]		; AX X
		mov	bx, [esi+2]		; BX Y
		mov	cx, [esi+4]		; CX Z
		mov	dx, [esi+6]		; DX prenormalized range
		add	esi, 8

		call	Rot			; DX inchang

		mov	bp, dx

		mov	ax, word ptr[X0]

		imul	word ptr[NormalXLight]
		mov	cx, ax
		mov	bx, dx

		mov	ax, word ptr[Y0]
		imul	word ptr[NormalYLight]
		add	cx, ax
		adc	bx, dx

		mov	ax, word ptr[Z0]
		imul	word ptr[NormalZLight]
		add	ax, cx
		adc	dx, bx			; intensity

		mov	edi, [Ptr1]

		or	dx,dx
		js	nointensity		; saut si negatif

		idiv	bp

		mov	word ptr[edi], ax
		add	edi, 2
		mov	[Ptr1], edi		; stock intensity

		dec	word ptr[Count1]
		jnz	bouclerot
nonp:
                jmp	FinNuage
;		ret

nointensity:	mov	word ptr[edi], 0
		add	edi, 2
		mov	[Ptr1], edi		; stock intensity

		dec	word ptr[Count1]
		jnz	bouclerot

		jmp	FinNuage
;		ret

;ComputeStaticNormal	endp

RotateNuage	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

		END
