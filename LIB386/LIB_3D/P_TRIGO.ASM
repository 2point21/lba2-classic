
		.386p
		jumps

		.model SMALL, SYSCALL

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		   ÛÜ ÜÛ       ฿฿Û฿฿ Û฿฿฿Û  Û    Û฿฿฿฿ Û฿฿฿Û
;		   ÛÛ฿ Û         ÛÛ  ÛÛ฿Û฿  ÛÛ   ÛÛ ฿Û ÛÛ  Û
;		   ฿฿  ฿ ฿฿฿฿฿   ฿฿  ฿฿  ฿  ฿฿   ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.data

		include P_define.ash


		extrn	NoLanguage P_SinTab:WORD

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; locales

compteur	dw	00

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; globales


		public	NoLanguage TypeProj
TypeProj	dw	00


		public	NoLanguage XCentre
		public	NoLanguage YCentre
XCentre		dd	320
YCentre		dd	200

		public	NoLanguage Xp
		public	NoLanguage Yp
Xp		dw	00
Yp		dw	00

		public	NoLanguage IsoScale
IsoScale	dw	500

		public	NoLanguage Z_Min
		public	NoLanguage Z_Max
Z_Min		dw	00
Z_Max		dw	00


ALIGN 4

		public	NoLanguage KFactor
		public	NoLanguage LFactorX
		public	NoLanguage LFactorY
KFactor		dd	128
LFactorX	dd	1024
LFactorY	dd	840

		public	NoLanguage CameraX
		public	NoLanguage CameraY
		public	NoLanguage CameraZ
CameraX		dd	00
CameraY		dd	00
CameraZ		dd	00

		public	NoLanguage CameraXr
		public	NoLanguage CameraYr
		public	NoLanguage CameraZr
CameraXr	dd	00
CameraYr	dd	00
CameraZr	dd	00

		public	NoLanguage AlphaLight
		public	NoLanguage BetaLight
		public	NoLanguage GammaLight
AlphaLight	dd	0000
BetaLight	dd	0000
GammaLight	dd	0000

		public	NoLanguage Alpha
		public	NoLanguage Beta
		public	NoLanguage Gamma
Alpha		dd	0000
Beta		dd	0000
Gamma		dd	0000


		public	NoLanguage LMatriceWorld
		public	NoLanguage LMatriceRot
		public	NoLanguage LMatriceTempo

;		public	NoLanguage MatriceEntity

		public	NoLanguage LMat00
		public	NoLanguage LMat01
		public	NoLanguage LMat02
		public	NoLanguage LMat10
		public	NoLanguage LMat11
		public	NoLanguage LMat12
		public	NoLanguage LMat20
		public	NoLanguage LMat21
		public	NoLanguage LMat22

		public	NoLanguage compteur


		;	Je les ai bouge ici pour qu'il soit a cote des
		;	Matrices afin d'essayer d'optimiser le cache
		;	(on sait jamais....)

		public	NoLanguage lAlpha
		public	NoLanguage lBeta
		public	NoLanguage lGamma
lAlpha		dd	0000
lBeta		dd	0000
lGamma		dd	0000

		public	NoLanguage NormalXLight
		public	NoLanguage NormalYLight
		public	NoLanguage NormalZLight
NormalXLight	dd	0061
NormalYLight	dd	0000
NormalZLight	dd	0000

		public	NoLanguage X0
		public	NoLanguage Y0
		public	NoLanguage Z0
X0		dd	0000
Y0		dd	0000
Z0		dd	0000

LMatriceRot	label	DWORD
LMat00		dd	0000
LMat01		dd	0000
LMat02		dd	0000
LMat10		dd	0000
LMat11		dd	0000
LMat12		dd	0000
LMat20		dd	0000
LMat21		dd	0000
LMat22		dd	0000

LMatriceDummy	label	DWORD
LMatD00		dd	0000
LMatD01		dd	0000
LMatD02		dd	0000
LMatD10		dd	0000
LMatD11		dd	0000
LMatD12		dd	0000
LMatD20		dd	0000
LMatD21		dd	0000
LMatD22		dd	0000

Public	NoLanguage	LMatriceWorld
Public	NoLanguage	LMatW00
Public	NoLanguage	LMatW01
Public	NoLanguage	LMatW02
Public	NoLanguage	LMatW10
Public	NoLanguage	LMatW11
Public	NoLanguage	LMatW12
Public	NoLanguage	LMatW20
Public	NoLanguage	LMatW21
Public	NoLanguage	LMatW22

LMatriceWorld	label	DWORD
LMatW00		dd	0000
LMatW01		dd	0000
LMatW02		dd	0000
LMatW10		dd	0000
LMatW11		dd	0000
LMatW12		dd	0000
LMatW20		dd	0000
LMatW21		dd	0000
LMatW22		dd	0000


LMatriceTempo	label	DWORD
LMatT00		dd	0000
LMatT01		dd	0000
LMatT02		dd	0000
LMatT10		dd	0000
LMatT11		dd	0000
LMatT12		dd	0000
LMatT20		dd	0000
LMatT21		dd	0000
LMatT22		dd	0000

		public	NoLanguage TabMat	; 30 Matrix max
TabMat		dd	9*30 dup (?)

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

Save1		dd	0000

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

;h		dw	00

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		.code

		public	NoLanguage	Rotate
		public	NoLanguage	RotatePoint
		public	NoLanguage	LongInverseRotatePoint
		public	NoLanguage	RotateMatriceWorld
		public	NoLanguage	CopyMatrice
		public	NoLanguage	LongWorldRotatePoint
		public	NoLanguage	WorldRotatePoint
		public	NoLanguage	SetFollowCamera
		public	NoLanguage	SetPosCamera
		public	NoLanguage	SetAngleCamera
		public	NoLanguage	SetInverseAngleCamera
		public	NoLanguage	SetProjection
		public	NoLanguage	SetIsoProjection
		public	NoLanguage	ProjettePoint
		public	Nolanguage	LongProjettePoint
		public	NoLanguage	TestVuePoly

		public	NoLanguage	LongWorldRot
		public	NoLanguage	WorldRot
		public	NoLanguage	RotMatW
		public	NoLanguage	RotMatIndex2
		public	NoLanguage	Rot
;		public	NoLanguage	Proj
		public	NoLanguage	Proj_3D
		public	NoLanguage	Proj_ISO

		public	NoLanguage	RotList
		public	NoLanguage	TransRotList
;		public	NoLanguage	RotListNormal

		public	NoLanguage	SetLightVector

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                        Û฿฿฿Û Û฿฿฿Û  Û    ÛÛÜ Û ฿฿Û฿฿
;                        ÛÛ฿฿฿ ÛÛ  Û  ÛÛ   ÛÛ฿ÛÛ   ÛÛ
;                        ฿฿    ฿฿฿฿฿  ฿฿   ฿฿  ฿   ฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'une liste d'aprs MatriceRot (ASM)
; ESI source XYZ
; EDI dest XYZ
; EAX Matrice
; ECX nbpoints
; 			EBP clat

RotList		proc	near

		mov	[compteur], cx

		jmp	short start

rl0:		mov	esi, [Save1]
		add	edi, 6
		add	esi, 6
start:		mov	[Save1], esi

		mov	ebx, [esi]
		movsx	edx, bx
		sar	ebx, 16
		movsx	ecx, word ptr[esi+04]

		mov	esi, [ eax+LT00 ]
		mov	ebp, [ eax+LT01 ]
		imul	esi, edx
		imul	ebp, ebx
		add	esi, ebp
		mov	ebp, [ eax+LT02 ]
		imul	ebp, ecx
		add	esi, ebp
		sar	esi, 14
		add	si, word ptr[X0]
		mov	word ptr[edi], si

		mov	esi, [ eax+LT10 ]
		mov	ebp, [ eax+LT11 ]
		imul	esi, edx
		imul	ebp, ebx
		add	esi, ebp
		mov	ebp, [ eax+LT12 ]
		imul	ebp, ecx
		add	esi, ebp
		sar	esi, 14
		add	si, word ptr[Y0]
		mov	word ptr[edi+2], si

		imul	edx, [ eax+LT20 ]
		imul	ebx, [ eax+LT21 ]
		imul	ecx, [ eax+LT22 ]
		add	ecx, edx
		add	ecx, ebx
		sar	ecx, 14
		add	cx, word ptr[Z0]
		mov	word ptr[edi+4], cx

		dec	[compteur]
		jnz	rl0

		ret

RotList		endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation et Translation d'une liste d'aprs MatriceRot (ASM)
; ESI source XYZ
; EDI dest XYZ
; EAX Matrice
; ECX nbpoints
; 			EBP clat

TransRotList	proc	near

		mov	[compteur], cx

		jmp	short start

rl0:		mov	esi, [Save1]
		add	edi, 6
		add	esi, 6
start:		mov	[Save1], esi

		movsx	edx, word ptr[esi]
		movsx   ebx, word ptr[esi+02]
		movsx	ecx, word ptr[esi+04]

		add	edx, [lAlpha]
		add	ebx, [lBeta]
		add	ecx, [lGamma]

		mov	esi, [ eax+LT00 ]
		mov	ebp, [ eax+LT01 ]
		imul	esi, edx
		imul	ebp, ebx
		add	esi, ebp
		mov	ebp, [ eax+LT02 ]
		imul	ebp, ecx
		add	esi, ebp
		sar	esi, 14
		add	si, word ptr[X0]
		mov	word ptr[edi], si

		mov	esi, [ eax+LT10 ]
		mov	ebp, [ eax+LT11 ]
		imul	esi, edx
		imul	ebp, ebx
		add	esi, ebp
		mov	ebp, [ eax+LT12 ]
		imul	ebp, ecx
		add	esi, ebp
		sar	esi, 14
		add	si, word ptr[Y0]
		mov	word ptr[edi+2], si

		imul	edx, [ eax+LT20 ]
		imul	ebx, [ eax+LT21 ]
		imul	ecx, [ eax+LT22 ]
		add	ecx, edx
		add	ecx, ebx
		sar	ecx, 14
		add	cx, word ptr[Z0]
		mov	word ptr[edi+4], cx

		dec	[compteur]
		jnz	rl0

		ret

TransRotList	endp


comment	@

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'une liste d'aprs MatriceRot (ASM)
; ESI source XYZ
; EDI dest XYZ
; ECX nbpoints
; 			EBP clat

RotListNormal	proc	near

		mov	[compteur], cx

bouclerot:	push	edi

		movsx	ebp, word ptr[esi]
		movsx   ebx, word ptr[esi+2]
		movsx	ecx, word ptr[esi+4]
		add	esi, 6

		mov	edx, [ LMat00 ]
		mov	eax, [ LMat01 ]
		imul	edx, ebp
		imul	eax, ebx
		add	edx, eax
		mov	eax, [ LMat02 ]
		imul	eax, ecx
		add	eax, edx
		sar	eax, 14

		imul	eax, [NormalXLight]
		mov	edi, eax

		mov	edx, [ LMat10 ]
		mov	eax, [ LMat11 ]
		imul	edx, ebp
		imul	eax, ebx
		add	edx, eax
		mov	eax, [ LMat12 ]
		imul	eax, ecx
		add	eax, edx
		sar	eax, 14

		imul	eax, [NormalYLight]
		add	edi, eax

		mov	edx, [ LMat20 ]
		mov	eax, [ LMat21 ]
		imul	edx, ebp
		imul	eax, ebx
		add	edx, eax
		mov	eax, [ LMat22 ]
		imul	eax, ecx
		add	eax, edx
		sar	eax, 14

		imul	eax, [NormalZLight]
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

		dec	word ptr[compteur]
		jnz	bouclerot

		ret

nointensity:    add	esi, 2

		pop	edi
		mov	word ptr[edi], 0
		add	edi, 2

		dec	word ptr[compteur]
		jnz	bouclerot

		ret

RotListNormal	endp

	@

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

SetLightVector	proc uses esi edi ebx ebp,\
		pa:DWORD, pb:DWORD, pg:DWORD

		mov	eax, pa
		mov	[AlphaLight], eax
		mov	[lAlpha], eax
		mov	eax, pb
		mov	[BetaLight], eax
		mov	[lBeta], eax
		mov	eax, pg		; pfeu
		mov	[GammaLight], eax
		mov	[lGamma], eax

		call	RotMatW

		xor	eax, eax
		mov	ebx, eax
		mov	ecx, NORMAL_UNIT - 5	; je sais je sais...

		call	Rot

		mov	eax, [X0]
		mov	[NormalXLight], eax
		mov	eax, [Y0]
		mov	[NormalYLight], eax
		mov	eax, [Z0]
		mov	[NormalZLight], eax

		ret

SetLightVector	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'un point d'aprs MatriceRot (ASM)
; DX et SI inchangs BP clat

Rot		proc

		; AX=X BX=Y CX=Z

		movsx	ebp, ax
		movsx   ebx, bx
		movsx	ecx, cx

		mov	edi, [ LMat00 ]
		mov	eax, [ LMat01 ]
		imul	edi, ebp
		imul	eax, ebx
		add	edi, eax
		mov	eax, [ LMat02 ]
		imul	eax, ecx
		add	eax, edi
		sar	eax, 14
		mov	[ X0 ], eax

		mov	edi, [ LMat10 ]
		mov	eax, [ LMat11 ]
		imul	edi, ebp
		imul	eax, ebx
		add	edi, eax
		mov	eax, [ LMat12 ]
		imul	eax, ecx
		add	eax, edi
		sar	eax, 14
		mov	[ Y0 ], eax

		imul	ebp, [ LMat20 ]
		imul	ebx, [ LMat21 ]
		imul	ecx, [ LMat22 ]
		add	ebx, ebp
		add	ebx, ecx
		sar	ebx, 14
		mov	[ Z0 ], ebx

		; X0=X' Y0=Y' Z0=Z'

		ret

Rot		endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'un point d'aprs MatriceRot (C)


RotatePoint	proc	uses edi ebx ebp ,\
		X:DWORD, Y:DWORD, Z:DWORD

		mov	eax, X
		mov	ebx, Y
		mov	ecx, Z

		call	Rot

		ret

RotatePoint	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; Rotation d'un point d'aprs MatriceWorld (ASM)
; TOUS LES REGISTRES ECLATES

LongWorldRot	proc

		; EAX=X EBX=Y ECX=Z

		mov	ebp, eax

		mov     eax, [ LMatW00 ]
		imul    ebp
		mov     esi, eax
		mov     edi, edx
		mov	eax, [ LMatW01 ]
		imul	ebx
		add     esi, eax
		adc     edi, edx
		mov	eax, [ LMatW02 ]
		imul	ecx
		add     esi, eax
		adc     edi, edx
		shrd    esi, edi, 14
		mov	[ X0 ], esi

		mov     eax, [ LMatW10 ]
		imul    ebp
		mov     esi, eax
		mov     edi, edx
		mov	eax, [ LMatW11 ]
		imul	ebx
		add     esi, eax
		adc     edi, edx
		mov	eax, [ LMatW12 ]
		imul	ecx
		add     esi, eax
		adc     edi, edx
		shrd    esi, edi, 14
		mov	[ Y0 ], esi

		mov     eax, [ LMatW20 ]
		imul    ebp
		mov     esi, eax
		mov     edi, edx
		mov	eax, [ LMatW21 ]
		imul	ebx
		add     esi, eax
		adc     edi, edx
		mov	eax, [ LMatW22 ]
		imul	ecx
		add     esi, eax
		adc     edi, edx
		shrd    esi, edi, 14
		mov	[ Z0 ], esi

		; X0=X' Y0=Y' Z0=Z'

		ret

LongWorldRot	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'un point d'aprs MatriceWorld (C)

LongWorldRotatePoint	proc	uses esi edi ebx ebp,\
			X:DWORD, Y:DWORD, Z:DWORD

		mov	eax, X
		mov	ebx, Y
		mov	ecx, Z

		call	LongWorldRot

		ret

LongWorldRotatePoint	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; Rotation d'un point d'aprs MatriceWorld (ASM)
; DX SI inchang BP clat

WorldRot	proc

		; AX=X BX=Y CX=Z

		movsx	ebp, ax
		movsx   ebx, bx
		movsx	ecx, cx

		mov	edi, [ LMatW00 ]
		mov	eax, [ LMatW01 ]
		imul	edi, ebp
		imul	eax, ebx
		add	edi, eax
		mov	eax, [ LMatW02 ]
		imul	eax, ecx
		add	eax, edi
		sar	eax, 14
		mov	[ X0 ], eax

		mov	edi, [ LMatW10 ]
		mov	eax, [ LMatW11 ]
		imul	edi, ebp
		imul	eax, ebx
		add	edi, eax
		mov	eax, [ LMatW12 ]
		imul	eax, ecx
		add	eax, edi
		sar	eax, 14
		mov	[ Y0 ], eax

		mov	edi, [ LMatW20 ]
		mov	eax, [ LMatW21 ]
		imul	edi, ebp
		imul	eax, ebx
		add	edi, eax
		mov	eax, [ LMatW22 ]
		imul	eax, ecx
		add	eax, edi
		sar	eax, 14
		mov	[ Z0 ], eax

		; X0=X' Y0=Y' Z0=Z'

		ret

WorldRot	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'un point d'aprs MatriceWorld (C)

WorldRotatePoint	proc	uses edi ebx ebp,\
			X:DWORD, Y:DWORD, Z:DWORD

		mov	eax, X
		mov	ebx, Y
		mov	ecx, Z

		call	WorldRot

		ret

WorldRotatePoint	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; Rotation d'un point d'aprs MatriceInverse (ASM)
; DX SI inchang BP clat

LongInverseRot	proc

		; EAX=X EBX=Y ECX=Z

		mov	ebp, eax

		mov     eax, [ LMatW00 ]
		imul    ebp
		mov     esi, eax
		mov     edi, edx
		mov	eax, [ LMatW10 ]
		imul	ebx
		add     esi, eax
		adc     edi, edx
		mov	eax, [ LMatW20 ]
		imul	ecx
		add     esi, eax
		adc     edi, edx
		shrd    esi, edi, 14
		mov	[ X0 ], esi

		mov     eax, [ LMatW01 ]
		imul    ebp
		mov     esi, eax
		mov     edi, edx
		mov	eax, [ LMatW11 ]
		imul	ebx
		add     esi, eax
		adc     edi, edx
		mov	eax, [ LMatW21 ]
		imul	ecx
		add     esi, eax
		adc     edi, edx
		shrd    esi, edi, 14
		mov	[ Y0 ], esi

		mov     eax, [ LMatW02 ]
		imul    ebp
		mov     esi, eax
		mov     edi, edx
		mov	eax, [ LMatW12 ]
		imul	ebx
		add     esi, eax
		adc     edi, edx
		mov	eax, [ LMatW22 ]
		imul	ecx
		add     esi, eax
		adc     edi, edx
		shrd    esi, edi, 14
		mov	[ Z0 ], esi

		; X0=X' Y0=Y' Z0=Z'

		ret

LongInverseRot	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation d'un point d'aprs MatriceInverse (C)

LongInverseRotatePoint	proc	uses esi edi ebx ebp,\
			X:DWORD, Y:DWORD, Z:DWORD

		mov	eax, X
		mov	ebx, Y
		mov	ecx, Z

		call	LongInverseRot

		ret

LongInverseRotatePoint	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; fonctions de travail sur les
;                   ÛÜ ÜÛ Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿Û  Û    Û฿฿฿฿ Û฿฿฿฿
;                   ÛÛ฿ Û ÛÛ฿฿Û   ÛÛ  ÛÛ฿Û฿  ÛÛ   ÛÛ    ÛÛ฿฿
;                   ฿฿  ฿ ฿฿  ฿   ฿฿  ฿฿  ฿  ฿฿   ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; Rotation de MatriceWorld vers MatriceRot (ASM)


RotMatW		proc

		mov	ebp, offset LMatriceWorld
		mov	eax, offset LMatriceRot
		jmp	RotMatIndex2

RotMatW		endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
; Rotation de MatriceWorld vers MatriceRot (C)

RotateMatriceWorld	proc	uses esi edi ebp ebx,\
			palpha:DWORD, pbeta:DWORD, pgamma:DWORD

		mov	eax, palpha
		mov	lAlpha, eax
		mov	eax, pbeta
		mov	lBeta, eax
		mov	eax, pgamma
		mov	lGamma, eax

		mov	ebp, offset LMatriceWorld
		mov	eax, offset LMatriceRot

		call	RotMatIndex2

		ret

RotateMatriceWorld	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; rotation de Matrice pointe par EBP vers EAX (ASM)
; EBP -> LONG, EAX -> LONG
; EAX pas modifie !!!
; EBX pas utilise !!!

RotMatIndex2	proc

a:		mov	edx, [lAlpha]
		or	edx, edx
		jz	g

doalpha:	;			si s=TabSinus[a&1023];
		;			cx c=TabSinus[(a+256)&1023];
		and	edx, 1023
		movsx	esi, word ptr[ P_SinTab + edx*2 ]
		add	edx, 256
		and	edx, 1023
		movsx	ecx, word ptr[ P_SinTab + edx*2 ]

		;			LMat00 = LMatT00 ;
		;			LMat10 = LMatT10 ;
		;			LMat20 = LMatT20 ;
		mov	edi, [ebp+LT00]
		mov	edx, [ebp+LT10]
		mov	[eax+LT00], edi
		mov	edi, [ebp+LT20]
		mov	[eax+LT10], edx
		mov	[eax+LT20], edi
		;			LMat01 = ( c*LMatT01 +  s*LMatT02 )>>15;
		mov	edx, [ebp+LT01]
		mov	edi, [ebp+LT02]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	[eax+LT01], edi
		;			LMat02 = ( c*LMatT02 + -s*LMatT01 )>>15;
		mov	edx, [ebp+LT01]
		mov	edi, [ebp+LT02]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	[eax+LT02], edi
		;			LMat11 = ( c*LMatT11 +  s*LMatT12 )>>15;
		mov	edx, [ebp+LT11]
		mov	edi, [ebp+LT12]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	[eax+LT11], edi
		;			LMat12 = ( c*LMatT12 + -s*LMatT11 )>>15;
		mov	edx, [ebp+LT11]
		mov	edi, [ebp+LT12]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	[eax+LT12], edi
		;			LMat21 = ( c*LMatT21 +  s*LMatT22 )>>15;
		mov	edx, [ebp+LT21]
		mov	edi, [ebp+LT22]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	[eax+LT21], edi
		;			LMat22 = ( c*LMatT22 + -s*LMatT21 )>>15;
		mov	edx, [ebp+LT21]
		mov	edi, [ebp+LT22]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	[eax+LT22], edi

		mov	ebp, eax

g:		mov	edx, [lGamma]
		or	edx, edx
		jz	b

dogamma:	;			si s=TabSinus[g&1023];
		;			cx c=TabSinus[(g+256)&1023];
		and	edx, 1023
		movsx	esi, word ptr[ P_SinTab + edx*2 ]
		add	edx, 256
		and	edx, 1023
		movsx	ecx, word ptr[ P_SinTab + edx*2 ]

		;			LMatD02 = LMat02;
		;			LMatD12 = LMat12;
		;			LMatD22 = LMat22;
		mov	edi, [ebp+LT02]
		mov	edx, [ebp+LT12]
		mov	LMatD02, edi
		mov	edi, [ebp+LT22]
		mov	LMatD12, edx
		mov	LMatD22, edi
		;			LMatD00 = ( c*LMat00 +  s*LMat01 )>>15;
		mov	edx, [ebp+LT00]
		mov	edi, [ebp+LT01]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	LMatD00, edi
		;			LMatD01 = ( c*LMat01 + -s*LMat00 )>>15;
		mov	edx, [ebp+LT00]
		mov	edi, [ebp+LT01]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	LMatD01, edi
		;			LMatD10 = ( c*LMat10 +  s*LMat11 )>>15;
		mov	edx, [ebp+LT10]
		mov	edi, [ebp+LT11]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	LMatD10, edi
		;			LMatD11 = ( c*LMat11 + -s*LMat10 )>>15;
		mov	edx, [ebp+LT10]
		mov	edi, [ebp+LT11]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	LMatD11, edi
		;			LMatD20 = ( c*LMat20 +  s*LMat21 )>>15;
		mov	edx, [ebp+LT20]
		mov	edi, [ebp+LT21]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	LMatD20, edi
		;			LMatD21 = ( c*LMat21 + -s*LMat20 )>>15;
		imul	esi, [ebp+LT20]
		imul	ecx, [ebp+LT21]
		sub	ecx, esi
		sar	ecx, 14
		mov	LMatD21, ecx

		mov	ebp, offset LMatriceDummy

b:		mov	edx, [lBeta]
		or	edx, edx
		jnz	dobeta

		cmp	ebp, eax
		je	short fin

		mov	esi, ebp
		mov	edi, eax
		mov	ecx, 9
		rep	movsd
fin:
		ret

dobeta:		cmp	ebp, eax
		jne	short nopb

		mov	esi, eax
		mov	ebp, offset LMatriceDummy
		mov	edi, ebp
		mov	ecx, 9
		rep	movsd
nopb:
		;			si s=TabSinus[g&1023];
		;			cx c=TabSinus[(g+256)&1023];
		and	edx, 1023
		movsx	esi, word ptr[ P_SinTab + edx*2 ]
		add	edx, 256
		and	edx, 1023
		movsx	ecx, word ptr[ P_SinTab + edx*2 ]

		;			LMat01 = LMatD01 ;
		;			LMat11 = LMatD11 ;
		;			LMat21 = LMatD21 ;
		mov	edi, [ebp+LT01]
		mov	edx, [ebp+LT11]
		mov	[eax+LT01], edi
		mov	edi, [ebp+LT21]
		mov	[eax+LT11], edx
		mov	[eax+LT21], edi

		;			LMat00 = ( c*LMatD00 + -s*LMatD02 )>>15;
		mov	edx, [ebp+LT02]
		mov	edi, [ebp+LT00]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	[eax+LT00], edi
		;			LMat02 = ( c*LMatD02 +  s*LMatD00 )>>15;
		mov	edx, [ebp+LT02]
		mov	edi, [ebp+LT00]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	[eax+LT02], edi
		;			LMat10 = ( c*LMatD10 + -s*LMatD12 )>>15;
		mov	edx, [ebp+LT12]
		mov	edi, [ebp+LT10]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	[eax+LT10], edi
		;			LMat12 = ( c*LMatD12 +  s*LMatD10 )>>15;
		mov	edx, [ebp+LT12]
		mov	edi, [ebp+LT10]
		imul	edx, ecx
		imul	edi, esi
		add	edi, edx
		sar	edi, 14
		mov	[eax+LT12], edi
		;			LMat20 = ( c*LMatD20 + -s*LMatD22 )>>15;
		mov	edx, [ebp+LT22]
		mov	edi, [ebp+LT20]
		imul	edx, esi
		imul	edi, ecx
		sub	edi, edx
		sar	edi, 14
		mov	[eax+LT20], edi
		;			LMat22 = ( c*LMatD22 +  s*LMatD20 )>>15;
		imul	ecx, [ebp+LT22]
		imul	esi, [ebp+LT20]
		add	ecx, esi
		sar	ecx, 14
		mov	[eax+LT22], ecx

		ret

RotMatIndex2	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		      Û฿฿฿฿ Û฿฿฿Û ÛÜ ÜÛ Û฿฿฿฿ Û฿฿฿Û Û฿฿฿Û
;		      ÛÛ    ÛÛ฿฿Û ÛÛ฿ Û ÛÛ฿฿  ÛÛ฿Û฿ ÛÛ฿฿Û
;		      ฿฿฿฿฿ ฿฿  ฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿  ฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

SetPosCamera	proc ,\
		poswx:DWORD, poswy:DWORD, poswz:DWORD

		mov	eax, poswx
		mov	[CameraX], eax
		mov	eax, poswy
		mov	[CameraY], eax
		mov	eax, poswz
		mov	[CameraZ], eax

		ret

SetPosCamera	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; initialisation de MatriceWorld (C)

SetInverseAngleCamera proc uses esi edi ebx ebp,\
		palpha:DWORD, pbeta:DWORD, pgamma:DWORD

		push	pgamma
		push	pbeta
		push	palpha
		call	SetAngleCamera
		add	esp, 12

		push	offset	LMatriceDummy
		push	offset	LMatriceWorld
		call	FlipMatrice
		add	esp, 8

		push	offset	LMatriceWorld
		push	offset	LMatriceDummy
		call	CopyMatrice
		add	esp, 8

		mov     eax, [CameraX]
		mov     ebx, [CameraY]
		mov     ecx, [CameraZ]

		call    LongWorldRot		; LONG for Ext mode

		mov     eax, [X0]
		mov     [CameraXr], eax
		mov     eax, [Y0]
		mov     [CameraYr], eax
		mov     eax, [Z0]
		mov     [CameraZr], eax

		ret

SetInverseAngleCamera endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; initialisation de MatriceWorld (C)

SetAngleCamera	proc	uses esi edi ebx ebp,\
		palpha:DWORD, pbeta:DWORD, pgamma:DWORD

		;			si	s=TabSinus[a&1023];
		;			cx	c=TabSinus[(a+256)&1023];
		mov	ebx, palpha
		and	ebx, 1023
		mov	[Alpha], ebx
		movsx	esi, word ptr[ P_SinTab + ebx*2 ]
		add	ebx, 256
		and	ebx, 1023
		movsx	ecx, word ptr[ P_SinTab + ebx*2 ]

		;			di	s2=TabSinus[g&1023];
		;			bx	c2=TabSinus[(g+256)&1023];
		mov	ebx, pgamma
		and	ebx, 1023
		mov	[Gamma], ebx
		movsx	edi, word ptr[ P_SinTab + ebx*2 ]
		add	ebx, 256
		and	ebx, 1023
		movsx	ebx, word ptr[ P_SinTab + ebx*2 ]

		;				LMatW00 =  c2;
		mov	LMatW00, ebx
		;				LMatW01 = -s2;
		mov	eax, edi
		neg	eax
		mov	LMatW01, eax
		;				LMatW10 = (s2*c)>>15;
		mov	eax, edi
		imul	eax, ecx
		sar	eax, 14
		mov	LMatW10, eax
		;				LMatW11 = (c2*c)>>15;
		mov	eax, ebx
		imul	eax, ecx
		sar	eax, 14
		mov	LMatW11, eax
		;				LMatW20 = (s2*s)>>15;
		mov	eax, edi
		imul	eax, esi
		sar	eax, 14
		mov	LMatW20, eax
		;				LMatW21 = (c2*s)>>15;
		mov	eax, ebx
		imul	eax, esi
		sar	eax, 14
		mov	LMatW21, eax
		;				di	s2=TabSinus[ b&1023];
		;				bx	c2=TabSinus[(b+256)&1023];
		mov	ebx, pbeta
		and	ebx, 1023
		mov	[Beta], ebx
		movsx	edi, word ptr[ P_SinTab + ebx*2 ]
		add	ebx, 256
		and	ebx, 1023
		movsx	ebx, word ptr[ P_SinTab + ebx*2 ]

		;				h=LMatW00;
		mov	ebp, [LMatW00]
		;				LMatW00 = ( c2*h )>>15;
		mov	eax, ebx
		imul    eax, ebp
		sar	eax, 14
		mov	LMatW00, eax
		;				LMatW02 = ( s2*h )>>15;
		mov	eax, edi
		imul	eax, ebp
		sar	eax, 14
		mov	LMatW02, eax
		;				h = LMatW10 = MatI01
		mov	ebp, [LMatW10]
		;				LMatW10 = ( c2*h + s2*s )>>15;
		mov	edx, ebx
		mov	eax, edi
		imul	edx, ebp
		imul	eax, esi
		add	eax, edx
		sar	eax, 14
		mov	LMatW10, eax
		;				LMatW12 = (-c2*s + s2*h )>>15
		mov	edx, ebx
		mov	eax, edi
		imul	edx, esi
		imul	eax, ebp
		sub	eax, edx
		sar	eax, 14
		mov	LMatW12, eax
		;				h=LMatW20 = MatI02
		mov	ebp, [LMatW20]
		;				LMatW20 = ( c2*h + -s2*c )>>15
		mov	edx, ecx
		mov	eax, ebx
		imul	edx, edi
		imul	eax, ebp
		sub	eax, edx
		sar	eax, 14
		mov	LMatW20, eax
		;				LMatW22 = ( c2*c + s2*h )>>15
		mov	edx, ecx
		mov	eax, edi
		imul	edx, ebx
		imul	eax, ebp
		add	eax, edx
		sar	eax, 14
		mov	LMatW22, eax

		mov     eax, [CameraX]
		mov     ebx, [CameraY]
		mov     ecx, [CameraZ]

		call    LongWorldRot		; LONG for Ext mode

		mov     eax, [X0]
		mov     [CameraXr], eax
		mov     eax, [Y0]
		mov     [CameraYr], eax
		mov     eax, [Z0]
		mov     [CameraZr], eax

		ret

SetAngleCamera	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;	    ฿฿฿ÛÛ Û฿฿฿Ü       Û฿฿฿Û Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿฿
;	    ÛÛ฿฿฿ ÛÛ  Û       ÛÛ฿Û฿ ÛÛ  Û   ÛÛ  ÛÛ฿฿Û   ÛÛ  ÛÛ฿฿
;	    ฿฿฿฿฿ ฿฿฿฿  ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿   ฿฿  ฿฿  ฿   ฿฿  ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; Rotation 2D autour d'un axe
; entre: bx=X  cx=Z  bp=T
;
; sortie: ax = X'=Z*sin(T)+X*cos(T)
;	  dx = Z'=Z*cos(T)-X*sin(T)
;
; ( bx,cx,di,si dtruits )
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

RotXY		proc

		or	bp,bp
		jz	norot

		movsx	ebx, bx			; X
		movsx	ecx, cx			; Y

		movzx	eax, bp			; T
		and	eax, 03FFh
		mov	edi, offset P_SinTab
		movsx	ebp, word ptr[ edi+eax*2 ]	; ebp = Sin( T )
		add	eax, 256
		and	eax, 03FFh
		movsx	edx, word ptr[ edi+eax*2 ]	; edx = Cos( T )
; X*cos(T)
		mov	eax, ebx
		imul	eax, edx
		mov	edi, eax
; Z*sin(T)
		mov	eax, ecx
		imul	eax, ebp
		add	edi, eax
		sar	edi, 14			; di= X'= X*cos(T)+Z*sin(T)
; Z*cos(T)
		imul	edx, ecx
; X*sin(T)
		imul	ebx, ebp
		sub	edx, ebx		; Z' = -X*sin(T) + Y*cos(T)
		sar	edx, 14			; dx = Z'

		mov	eax, edi		; ax = X'
		ret

norot:		mov	eax, ebx		; ax = X' = X
		mov	edx, ecx		; dx = Z' = Z
		ret

RotXY		endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

Rotate		proc	uses esi edi ebx ebp,\
		coorx:DWORD,coory:DWORD,angle:DWORD

		mov	ebx, coorx
		mov	ecx, coory
		mov	ebp, angle
		call	RotXY
		mov	[X0], eax
		mov	[Y0], edx
		ret

Rotate		endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

SetFollowCamera	proc	uses esi edi ebx ebp,\
		targetx:DWORD, targety:DWORD, targetz:DWORD,\
		camalpha:DWORD,cambeta:DWORD,camgamma:DWORD,\
		camzoom:DWORD

		mov	eax, targetx
		mov     [CameraX], eax
		mov	eax, targety
		mov     [CameraY], eax
		mov	eax, targetz
		mov     [CameraZ], eax

		push	camgamma
		push	cambeta
		push	camalpha
		call	SetAngleCamera
		add	sp,12

		mov	eax, [CameraXr]
		mov	ebx, [CameraYr]
		mov	ecx, [CameraZr]
		add	ecx, camzoom
		mov	[CameraZr], ecx

		call	LongInverseRot

		mov	eax, [X0]
		mov	ebx, [Y0]
		mov	ecx, [Z0]
		mov	[CameraX], eax
		mov	[CameraY], ebx
		mov	[CameraZ], ecx

		ret

SetFollowCamera	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                     ÛÛ฿฿฿ Û฿฿฿฿ Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿ ÛÛÜ Û
;		      ฿฿฿฿Û ÛÛ    ÛÛ฿Û฿ ÛÛ฿฿  ÛÛ฿฿  ÛÛ฿ÛÛ
;		      ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

ifdef	0

Proj		proc		; ax:X bx:Y bp:Z

		cmp	word ptr[TypeProj], TYPE_ISO
		jz	projiso

		add	bp, word ptr[KFactor]
		js	overflow
fixed:		imul	word ptr[LFactorX]		; X
		idiv	bp
		add	ax, word ptr[XCentre]
		xchg	ax, bx
		neg	ax
		imul	word ptr[LFactorY]		; -Y
		idiv	bp
		add	ax, word ptr[YCentre]
		xchg	ax, bx
		ret					; AX:Xp BX:Yp

overflow:	mov	bp, 32767	; max value
		jmp	short fixed

projiso:        shl	ecx, 16			; save CX

		mov	cx, ax			; save x

;	add	ax, bp
;	mov	dx, 24
;	imul	dx
;	idiv	word ptr[IsoScale]
;	add	ax, [XCentre]

	add	ax, bp
	movzx	eax, ax
	imul	eax, 24		; mettre decalages
	sar	eax, 9
	add	ax, [XCentre]

		xchg	cx, ax			; mov ax, cx et cx, ax

		neg	bp
		add	ax, bp

;		shl	eax, 2		; *4
;		mov	edx, eax	; =4
;		shl	eax, 1		; *8
;		add	eax, edx	; *12
;		mov	edx, eax
;		shr	edx, 16		; dx high

		mov	dx, 12
		imul	dx

		idiv	word ptr[IsoScale]
		mov	bp, ax
		mov	ax, 30
		imul	bx
		idiv	word ptr[IsoScale]
		sub	ax, bp
		neg	ax
		add	ax, [YCentre]

		mov	bx, ax
		mov	ax, cx

		shr	ecx, 16			; restore CX

		ret

Proj		endp

endif

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

Proj_3D		proc		; ax:X bx:Y bp:Z

		add	bp, word ptr[KFactor]
		js	overflow
fixed:		imul	word ptr[LFactorX]		; X
		idiv	bp
		add	ax, word ptr[XCentre]
		xchg	ax, bx
		neg	ax
		imul	word ptr[LFactorY]		; -Y
		idiv	bp
		add	ax, word ptr[YCentre]
		xchg	ax, bx
		ret					; AX:Xp BX:Yp

overflow:	mov	bp, 32767	; max value
		jmp	short fixed

Proj_3D		endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

Proj_ISO	proc		; eax:X ebx:Y ebp:Z

		mov	ecx, eax		; save x

		sub	eax, ebp		; x + zrot
		add	ebp, ecx		; -(x - zrot)

		lea	eax, [eax*8]		; *8
		mov	edx, eax
		lea	eax, [edx+eax*2]	; *24

		sar	eax, 9			; /512 IsoScale

		add	ax, word ptr[XCentre]

		lea	edx, [ebp*4]		; *4
		lea	ebp, [edx+ebp*8]	; *12

		lea	ecx, [ebx+ebx]		; =*2
		shl	ebx, 5			; 32
		sub	ebx, ecx		; y*30

		sub	ebx, ebp		; - (x-zrot) * 12

		sar	ebx, 9			; /512 IsoScale

		neg	bx
		add	bx, word ptr[YCentre]

		ret

Proj_ISO	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

ProjettePoint	proc	uses ebx ebp ,\
			CoorX:DWORD,CoorY:DWORD,CoorZ:DWORD

		cmp	[TypeProj], TYPE_ISO
		jz	projiso

		mov	eax, CoorX
		sub	eax, [CameraXr]
		mov	ebx, CoorY
		sub	ebx, [CameraYr]
		mov	ecx, [CameraZr]
		sub	ecx, CoorZ
		mov	ebp, ecx

		or	cx, cx
		js	error			; si Z0 < 0 clip
;		cmp	ax, cx			; si X0 > Z0 clip
;		jg	error
;		cmp	bx, cx			; si Y0 > Z0 clip
;		jg	error
;		neg	cx
;		cmp	ax, cx
;		jl	error
;		cmp	bx, cx
;		jl	error

		call	Proj_3D

		mov	[Xp], ax
		mov	[Yp], bx

		mov	eax, -1
		ret

projiso:	mov	eax, CoorX
		mov	ebx, CoorY
		mov	ebp, CoorZ

		call	Proj_ISO

		mov	[Xp], ax
		mov	[Yp], bx

		mov	eax, -1
		ret

error:          xor	eax, eax
		mov	[Xp], ax
		mov	[Yp], ax
		ret

ProjettePoint	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

LongProjettePoint	proc	uses ebx,\
			CoorX:DWORD,CoorY:DWORD,CoorZ:DWORD

		mov	eax, CoorX
		sub	eax, [CameraXr]
		mov	ebx, CoorY
		sub	ebx, [CameraYr]
		mov	ecx, [CameraZr]
		sub	ecx, CoorZ

		or	ecx, ecx
		js	error			; si Z0 < 0 clip

		add	ecx, [KFactor]
	;		js	overflow
		js	error
fixed:		imul	[LFactorX]		; X
		idiv	ecx
		add	eax, [XCentre]
		xchg	eax, ebx
		neg	eax
		imul	[LFactorY]              ; -Y
		idiv	ecx
		add	eax, [YCentre]
		xchg	eax, ebx

		cmp	eax, 32767		; check if more than 16 bits
		jg	OverX
		cmp	eax, -32768
		jl	OverX
fixedX:
		cmp	ebx, 32767
		jg	OverY
		cmp	ebx, -32768
		jl	OverY
fixedY:

		mov	[Xp], ax		; result
		mov	[Yp], bx

		mov	eax, -1
		ret

overX:		shl	eax, 1
		mov	ax, 7FFFh
		adc	ax, 0
		jmp	fixedX

overY:		shl	ebx, 1
		mov	bx, 7FFFh
		adc	bx, 0
		jmp	fixedY

;overflow:	mov	ecx, 7FFFFFFFh		; max value
;		jmp	short fixed

error:          xor	eax, eax
		mov	[Xp], ax
		mov	[Yp], ax
		ret

LongProjettePoint	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

SetProjection	proc			,\
		xc:DWORD, yc:DWORD	,\
		kfact:DWORD		,\
		lfactx:DWORD,lfacty:DWORD

		mov	eax, xc
		mov	[XCentre], eax

		mov	eax, yc
		mov	[YCentre], eax

		mov	eax, kfact
		mov	[KFactor], eax

		mov	eax, lfactx
		mov	[LFactorX], eax

		mov	eax, lfacty
		mov	[LFactorY], eax

		mov	word ptr[TypeProj], TYPE_3D

		ret

SetProjection	endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

SetIsoProjection	proc			,\
			xc:DWORD, yc:DWORD	,\
			scale:DWORD

		mov	eax, xc
		mov	[XCentre], eax

		mov	eax, yc
		mov	[YCentre], eax

		mov	eax, scale
		mov	[IsoScale], ax

		mov	word ptr[TypeProj], TYPE_ISO

		ret

SetIsoProjection	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

TestVuePoly	proc	uses edi ebx ebp,\
		ptrpoly:DWORD

		mov	edi, ptrpoly

		mov	bx,word ptr[edi+8]
		sub	bx,word ptr[edi+2]

		mov	ax,word ptr[edi+4]
		sub	ax,word ptr[edi+16]

		imul	bx

		mov	bp,ax
		mov	bx,dx

		mov	cx,word ptr[edi+10]
		sub	cx,word ptr[edi+4]

		mov	ax,word ptr[edi+2]
		sub	ax,word ptr[edi+14]

		imul	cx

		sub	ax,bp			; oublie face si bx:bp < dx:ax
		sbb	dx,bx
		jnl	nopoly

		mov	eax,1
		ret

nopoly:		xor	eax,eax
		ret

TestVuePoly	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

CopyMatrice	proc	uses esi edi,\
		matsour:DWORD, matdest:DWORD

		mov	esi, matsour
		mov	edi, matdest
		mov	ecx, 9
		rep	movsd
		ret

CopyMatrice	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

FlipMatrice	proc	uses esi edi,\
		matsour:DWORD, matdest:DWORD

		mov	esi, matsour
		mov	edi, matdest

		mov	eax, [esi+LT00]
		mov	ecx, [esi+LT01]
		mov	edx, [esi+LT02]
		mov	[edi+LT00], eax
		mov	[edi+LT10], ecx
		mov	[edi+LT20], edx

		mov	eax, [esi+LT10]
		mov	ecx, [esi+LT11]
		mov	edx, [esi+LT12]
		mov	[edi+LT01], eax
		mov	[edi+LT11], ecx
		mov	[edi+LT21], edx

		mov	eax, [esi+LT20]
		mov	ecx, [esi+LT21]
		mov	edx, [esi+LT22]
		mov	[edi+LT02], eax
		mov	[edi+LT12], ecx
		mov	[edi+LT22], edx

		ret

FlipMatrice	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		END
