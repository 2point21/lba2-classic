;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;			      (c) Adeline 1993
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		.386P
		jumps
		.model SMALL, SYSCALL
		.data

	include	\projet\lib386\lib_svga\svga.ash

	extrn	NoLanguage	TabVerticG	:WORD
	extrn	NoLanguage	TabCoulG	:WORD
	extrn	NoLanguage	TabCoulD	:WORD
	extrn	NoLanguage	Ymin		:WORD
	extrn	NoLanguage	Ymax		:WORD

EVEN

		public	NoLanguage	TabText

TabText		dw	32*3	dup(00)

boucle		dw	00
loopy		dw	00

darkdec		dw	00

ALIGN 4

xstep           dd	0000
ystep		dd	0000

ptrlig		dd	0000
ptrmap		dd	0000
ptrtab		dd	0000

		public	NoLanguage	LYmin
		public	NoLanguage	LYmax

LYmin		dd 0000
LYmax		dd 0000

	extrn	NoLanguage	TabGauche:WORD
	extrn	NoLanguage	TabDroite:WORD
	extrn	NoLanguage	TabX0:WORD
	extrn	NoLanguage	TabY0:WORD
	extrn	NoLanguage	TabX1:WORD
	extrn	NoLanguage	TabY1:WORD

	extrn	NoLanguage	TabPoly:WORD

	extrn	NoLanguage	P_SinTab:WORD

TAB_GAUCHE	equ	960*0
TAB_DROITE	equ	960*1
TAB_X0		equ	960*2
TAB_Y0		equ	960*3
TAB_X1		equ	960*4
TAB_Y1		equ	960*5

		.code

		public	 NoLanguage	FillTextPoly
		public	 NoLanguage	FillTextPolyNoClip
;		public	 NoLanguage	M_FillTextPoly
		public	 NoLanguage	FillTextPolyShade

		public	 NoLanguage	AsmTexturedTriangleNoClip
		public	 NoLanguage	AsmGouraudTriangleNoClip

		public	 NoLanguage	AsmFillProp
		public	 NoLanguage	AsmFillPropNoClip
;		public	 NoLanguage	M_AsmFillProp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                        Û฿฿฿Û Û฿฿฿Û Û     Û  Ü฿ ÛÛ฿฿฿
;                        ÛÛ฿฿฿ ÛÛ  Û ÛÛ    ÛÛ฿   ฿฿฿฿Û
;                        ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿    ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

comment	#
void	FillProp( WORD *ptrdest, WORD x0, WORD y0, WORD x1, WORD y1 )
{
	WORD	y, t ;

	if( y0 > y1 )
	{
		t=y0; y0=y1; y1=t;
		t=x0; x0=x1; x1=t;
	}

	for( y=y0; y<=y1; y++ )
	{
		ptrdest[y] = RegleTrois32( x0, x1, y1-y0+1, y-y0 ) ;
	}
}
#

AsmFillProp	proc uses esi edi ebx ebp,\
		ptrdest:DWORD, x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD

		mov	edi, ptrdest

		mov	eax, y0
		mov	ecx, y1

		mov	ebx, x0
		mov	edx, x1

		cmp	eax, ecx		; y0 < y1
		jle	afp0

		xchg	eax, ecx		; y0 <> y1
		xchg	ebx, edx		; x0 <> x1

afp0:
		cmp	eax, [ClipYmax]
		jg	nofill

		cmp	ecx, [ClipYmin]
		jl	nofill

		cmp	eax, [ClipYmin]
		jl	clip_ymin

afp01:		cmp	ecx, [ClipYmax]
		jle	afp00

clip_ymax:

		push	edi
		push	eax

		mov	edi, ebx
		sub	edi, edx		; dx

		mov     esi, eax
		sub	esi, ecx		; dy
		jz	nofill2

		mov	ebp, edx

		mov	eax, ecx		; y1

		sub	eax, [ClipYmax]		; ymax - y1
		neg	eax
		imul	edi			; * dx
		idiv	esi			; / dy
		add     eax, ebp		; += x1
		mov	edx, eax
		mov	ecx, [ClipYmax]		; y1 = ymax

		pop	eax
		pop	edi

		jmp	afp00

clip_ymin:

		push	edi
		push	edx

		mov	edi, ebx
		sub	edi, edx		; dx

		mov     esi, eax
		sub	esi, ecx		; dy
		jz	nofill1

		mov	ebp, ebx

		sub	eax, [ClipYmin]		; ymin - y0
		neg	eax
		imul	edi			; * dx
		idiv	esi			; / dy
		add     eax, ebp		; += x0
		mov	ebx, eax
		mov	eax, [ClipYmin]		; y0 = ymin

		pop	edx
		pop	edi

		jmp	afp01

afp00:		sub	ecx, eax		; ecx deltay >= 0


		jz	nofill		; GASP mettre cas 1 ligne


		shl	eax, 1
		add	edi, eax		; (WORD*)ptrdest[y0]

		cmp	ebx, edx
		jg	afp1

		; x0 < x1

		mov	ax, dx			; x0
		sub	ax, bx			; x1 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		xchg	edx, eax
;'		shr	ax, 1
;'		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc

		mov	ax, bx

		inc	ecx
;'		add	ecx, 16
;'		shr	ecx, 4

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
lt0:
;'	REPT	16
		stosw
		adc	eax, edx
;'	ENDM
;'		loop	lt0			; pas touche C ?

		dec	ecx
		jnz	lt0

		ret

afp1:		; x0 > x1

		mov	ax, bx			; x1
		sub	ax, dx			; x0 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		xchg	edx, eax
;'		shr	ax, 1
;'		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc				; la 1ere valeur est bonne
		mov	ax, bx			; x0

		inc	ecx
;'		add	ecx, 16
;'		shr	ecx, 4

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
afp2:
;'	REPT	16
		stosw
		sbb	eax, edx
;'	ENDM
;'		loop	afp2

		dec	ecx
		jnz	afp2

nofill:		ret

nofill1:	pop	edx
		pop	edi
		ret

nofill2:	pop	eax
		pop	edi
		ret

AsmFillProp	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

AsmFillPropNoClip	proc uses esi edi ebx ebp,\
			ptrdest:DWORD, x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD

		mov	edi, ptrdest

		mov	eax, y0

		mov	ecx, y1

		mov	ebx, x0
		mov	edx, x1

		cmp	eax, ecx		; y0 < y1
		jle	afp0

		xchg	eax, ecx		; y0 <> y1
		xchg	ebx, edx		; x0 <> x1

afp0:		sub	ecx, eax		; ecx deltay > 0

		shl	eax, 1
		add	edi, eax		; (WORD*)ptrdest[y0]

		cmp	ebx, edx
		jg	afp1

		; x0 < x1

		mov	ax, dx			; x0
		sub	ax, bx			; x1 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		inc	ecx

		xchg	edx, eax
		shr	ax, 1
		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc

		mov	ax, bx

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
lt0:		stosw
		adc	eax, edx
		dec	ecx
		jnz	lt0

		ret

afp1:		; x0 > x1

		mov	ax, bx			; x1
		sub	ax, dx			; x0 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		inc	ecx

		xchg	edx, eax
		shr	ax, 1
		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc				; la 1ere valeur est bonne
		mov	ax, bx			; x0

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
afp2:		stosw
		sbb	eax, edx
		dec	ecx
		jnz	afp2

		ret

AsmFillPropNoClip	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

A_FillPropNoClip	proc

;		mov	edi, ptrdest

;		mov	eax, y0
;		mov	ecx, y1
;		mov	ebx, x0
;		mov	edx, x1

		cmp	eax, ecx		; y0 < y1
		jle	afp0

		xchg	eax, ecx		; y0 <> y1
		xchg	ebx, edx		; x0 <> x1

afp0:		sub	ecx, eax		; ecx deltay > 0

		shl	eax, 1
		add	edi, eax		; (WORD*)ptrdest[y0]

		cmp	ebx, edx
		jg	afp1

		; x0 < x1

		mov	ax, dx			; x0
		sub	ax, bx			; x1 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		inc	ecx

		xchg	edx, eax
		shr	ax, 1
		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc

		mov	ax, bx

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
lt0:		stosw
		adc	eax, edx
		dec	ecx
		jnz	lt0

		ret

afp1:		; x0 > x1

		mov	ax, bx			; x1
		sub	ax, dx			; x0 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		inc	ecx

		xchg	edx, eax
		shr	ax, 1
		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc				; la 1ere valeur est bonne
		mov	ax, bx			; x0

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
afp2:		stosw
		sbb	eax, edx
		dec	ecx
		jnz	afp2

		ret

A_FillPropNoClip	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

AsmTexturedTriangleNoClip	proc uses esi edi ebx ebp

PX0	TEXTEQU <word ptr[ TabPoly + 1*2 ]>
PY0	TEXTEQU <word ptr[ TabPoly + 2*2 ]>
PX1	TEXTEQU <word ptr[ TabPoly + 4*2 ]>
PY1	TEXTEQU <word ptr[ TabPoly + 5*2 ]>
PX2	TEXTEQU <word ptr[ TabPoly + 7*2 ]>
PY2	TEXTEQU <word ptr[ TabPoly + 8*2 ]>

TX0	TEXTEQU <word ptr[ TabText + 1*2 ]>
TY0	TEXTEQU <word ptr[ TabText + 2*2 ]>
TX1	TEXTEQU <word ptr[ TabText + 4*2 ]>
TY1	TEXTEQU <word ptr[ TabText + 5*2 ]>
TX2	TEXTEQU <word ptr[ TabText + 7*2 ]>
TY2	TEXTEQU <word ptr[ TabText + 8*2 ]>

	mov	LYmin, 32000
	mov	LYmax, -32000

	movzx	eax, PY0
	movzx	ecx, PY1

	cmp	eax, ecx
	jge	ttnc0

		cmp	eax, LYmin
		jg	ttnc1
		mov	LYmin, eax
ttnc1:
		cmp	ecx, LYmax
		jl	ttnc2
		mov	LYmax, ecx
ttnc2:
		mov	edi, offset TabGauche
		movzx	ebx, PX0
;		movzx	eax, PY0
		movzx	edx, PX1
;		movzx	ecx, PY1
		call	A_FillPropNoClip

		mov	edi, offset TabX0
		movzx	ebx, TX0
		movzx	eax, PY0
		movzx	edx, TX1
		movzx	ecx, PY1
		call	A_FillPropNoClip

		mov	edi, offset TabY0
		movzx	ebx, TY0
		movzx	eax, PY0
		movzx	edx, TY1
		movzx	ecx, PY1
		call	A_FillPropNoClip
ttnc0:
	movzx	eax, PY0
	movzx	ecx, PY1

	cmp	eax, ecx
	jle	ttnc0a

		cmp	ecx, LYmin
		jg	ttnc1a
		mov	LYmin, ecx
ttnc1a:
		cmp	eax, LYmax
		jl	ttnc2a
		mov	LYmax, eax
ttnc2a:
		mov	edi, offset TabDroite
		movzx	ebx, PX0
;		movzx	eax, PY0
		movzx	edx, PX1
;		movzx	ecx, PY1
		call	A_FillPropNoClip

		mov	edi, offset TabX1
		movzx	ebx, TX0
		movzx	eax, PY0
		movzx	edx, TX1
		movzx	ecx, PY1
		call	A_FillPropNoClip

		mov	edi, offset TabY1
		movzx	ebx, TY0
		movzx	eax, PY0
		movzx	edx, TY1
		movzx	ecx, PY1
		call	A_FillPropNoClip
ttnc0a:
	movzx	eax, PY1
	movzx	ecx, PY2

	cmp	eax, ecx
	jge	ttnc0b

		cmp	eax, LYmin
		jg	ttnc1b
		mov	LYmin, eax
ttnc1b:
		cmp	ecx, LYmax
		jl	ttnc2b
		mov	LYmax, ecx
ttnc2b:
		mov	edi, offset TabGauche
		movzx	ebx, PX1
;		movzx	eax, PY1
		movzx	edx, PX2
;		movzx	ecx, PY2
		call	A_FillPropNoClip

		mov	edi, offset TabX0
		movzx	ebx, TX1
		movzx	eax, PY1
		movzx	edx, TX2
		movzx	ecx, PY2
		call	A_FillPropNoClip

		mov	edi, offset TabY0
		movzx	ebx, TY1
		movzx	eax, PY1
		movzx	edx, TY2
		movzx	ecx, PY2
		call	A_FillPropNoClip
ttnc0b:
	movzx	eax, PY1
	movzx	ecx, PY2

	cmp	eax, ecx
	jle	ttnc0c

		cmp	ecx, LYmin
		jg	ttnc1c
		mov	LYmin, ecx
ttnc1c:
		cmp	eax, LYmax
		jl	ttnc2c
		mov	LYmax, eax
ttnc2c:

		mov	edi, offset TabDroite
		movzx	ebx, PX1
;		movzx	eax, PY1
		movzx	edx, PX2
;		movzx	ecx, PY2
		call	A_FillPropNoClip

		mov	edi, offset TabX1
		movzx	ebx, TX1
		movzx	eax, PY1
		movzx	edx, TX2
		movzx	ecx, PY2
		call	A_FillPropNoClip

		mov	edi, offset TabY1
		movzx	ebx, TY1
		movzx	eax, PY1
		movzx	edx, TY2
		movzx	ecx, PY2
		call	A_FillPropNoClip
ttnc0c:
	movzx	eax, PY2
	movzx	ecx, PY0

	cmp	eax, ecx
	jge	ttnc0d

		cmp	eax, LYmin
		jg	ttnc1d
		mov	LYmin, eax
ttnc1d:
		cmp	ecx, LYmax
		jl	ttnc2d
		mov	LYmax, ecx
ttnc2d:
		mov	edi, offset TabGauche
		movzx	ebx, PX2
;		movzx	eax, PY2
		movzx	edx, PX0
;		movzx	ecx, PY0
		call	A_FillPropNoClip

		mov	edi, offset TabX0
		movzx	ebx, TX2
		movzx	eax, PY2
		movzx	edx, TX0
		movzx	ecx, PY0
		call	A_FillPropNoClip

		mov	edi, offset TabY0
		movzx	ebx, TY2
		movzx	eax, PY2
		movzx	edx, TY0
		movzx	ecx, PY0
		call	A_FillPropNoClip
ttnc0d:
	movzx	eax, PY2
	movzx	ecx, PY0

	cmp	eax, ecx
	jle	ttnc0e

		cmp	ecx, LYmin
		jg	ttnc1e
		mov	LYmin, ecx
ttnc1e:
		cmp	eax, LYmax
		jl	ttnc2e
		mov	LYmax, eax
ttnc2e:
		mov	edi, offset TabDroite
		movzx	ebx, PX2
;		movzx	eax, PY2
		movzx	edx, PX0
;		movzx	ecx, PY0
		call	A_FillPropNoClip

		mov	edi, offset TabX1
		movzx	ebx, TX2
		movzx	eax, PY2
		movzx	edx, TX0
		movzx	ecx, PY0
		call	A_FillPropNoClip

		mov	edi, offset TabY1
		movzx	ebx, TY2
		movzx	eax, PY2
		movzx	edx, TY0
		movzx	ecx, PY0
		call	A_FillPropNoClip
ttnc0e:
		ret

AsmTexturedTriangleNoClip	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

AsmGouraudTriangleNoClip	proc uses esi edi ebx ebp

PI0	TEXTEQU <word ptr[ TabPoly + 0*2 ]>
PX0	TEXTEQU <word ptr[ TabPoly + 1*2 ]>
PY0	TEXTEQU <word ptr[ TabPoly + 2*2 ]>
PI1	TEXTEQU <word ptr[ TabPoly + 3*2 ]>
PX1	TEXTEQU <word ptr[ TabPoly + 4*2 ]>
PY1	TEXTEQU <word ptr[ TabPoly + 5*2 ]>
PI2	TEXTEQU <word ptr[ TabPoly + 6*2 ]>
PX2	TEXTEQU <word ptr[ TabPoly + 7*2 ]>
PY2	TEXTEQU <word ptr[ TabPoly + 8*2 ]>

	mov	LYmin, 32000
	mov	LYmax, -32000

	movzx	eax, PY0
	movzx	ecx, PY1

	cmp	eax, ecx
	jge	ttnc0

		cmp	eax, LYmin
		jg	ttnc1
		mov	LYmin, eax
ttnc1:
		cmp	ecx, LYmax
		jl	ttnc2
		mov	LYmax, ecx
ttnc2:
		mov	edi, offset TabGauche
		movzx	ebx, PX0
;		movzx	eax, PY0
		movzx	edx, PX1
;		movzx	ecx, PY1
		call	A_FillPropNoClip

		mov	edi, offset TabCoulG
		movzx	ebx, PI0
;		shl	ebx, 8
		movzx	eax, PY0
		movzx	edx, PI1
;		shl	edx, 8
		movzx	ecx, PY1
		call	A_FillPropNoClip
ttnc0:
	movzx	eax, PY0
	movzx	ecx, PY1

	cmp	eax, ecx
	jle	ttnc0a

		cmp	ecx, LYmin
		jg	ttnc1a
		mov	LYmin, ecx
ttnc1a:
		cmp	eax, LYmax
		jl	ttnc2a
		mov	LYmax, eax
ttnc2a:
		mov	edi, offset TabDroite
		movzx	ebx, PX0
;		movzx	eax, PY0
		movzx	edx, PX1
;		movzx	ecx, PY1
		call	A_FillPropNoClip

		mov	edi, offset TabCoulD
		movzx	ebx, PI0
;		shl	ebx, 8
		movzx	eax, PY0
		movzx	edx, PI1
;		shl	edx, 8
		movzx	ecx, PY1
		call	A_FillPropNoClip

ttnc0a:
	movzx	eax, PY1
	movzx	ecx, PY2

	cmp	eax, ecx
	jge	ttnc0b

		cmp	eax, LYmin
		jg	ttnc1b
		mov	LYmin, eax
ttnc1b:
		cmp	ecx, LYmax
		jl	ttnc2b
		mov	LYmax, ecx
ttnc2b:
		mov	edi, offset TabGauche
		movzx	ebx, PX1
;		movzx	eax, PY1
		movzx	edx, PX2
;		movzx	ecx, PY2
		call	A_FillPropNoClip

		mov	edi, offset TabCoulG
		movzx	ebx, PI1
;		shl	ebx, 8
		movzx	eax, PY1
		movzx	edx, PI2
;		shl	edx, 8
		movzx	ecx, PY2
		call	A_FillPropNoClip
ttnc0b:
	movzx	eax, PY1
	movzx	ecx, PY2

	cmp	eax, ecx
	jle	ttnc0c

		cmp	ecx, LYmin
		jg	ttnc1c
		mov	LYmin, ecx
ttnc1c:
		cmp	eax, LYmax
		jl	ttnc2c
		mov	LYmax, eax
ttnc2c:

		mov	edi, offset TabDroite
		movzx	ebx, PX1
;		movzx	eax, PY1
		movzx	edx, PX2
;		movzx	ecx, PY2
		call	A_FillPropNoClip

		mov	edi, offset TabCoulD
		movzx	ebx, PI1
;		shl	ebx, 8
		movzx	eax, PY1
		movzx	edx, PI2
;		shl	edx, 8
		movzx	ecx, PY2
		call	A_FillPropNoClip

ttnc0c:
	movzx	eax, PY2
	movzx	ecx, PY0

	cmp	eax, ecx
	jge	ttnc0d

		cmp	eax, LYmin
		jg	ttnc1d
		mov	LYmin, eax
ttnc1d:
		cmp	ecx, LYmax
		jl	ttnc2d
		mov	LYmax, ecx
ttnc2d:
		mov	edi, offset TabGauche
		movzx	ebx, PX2
;		movzx	eax, PY2
		movzx	edx, PX0
;		movzx	ecx, PY0
		call	A_FillPropNoClip

		mov	edi, offset TabCoulG
		movzx	ebx, PI2
;		shl	ebx, 8
		movzx	eax, PY2
		movzx	edx, PI0
;		shl	edx, 8
		movzx	ecx, PY0
		call	A_FillPropNoClip

ttnc0d:
	movzx	eax, PY2
	movzx	ecx, PY0

	cmp	eax, ecx
	jle	ttnc0e

		cmp	ecx, LYmin
		jg	ttnc1e
		mov	LYmin, ecx
ttnc1e:
		cmp	eax, LYmax
		jl	ttnc2e
		mov	LYmax, eax
ttnc2e:
		mov	edi, offset TabDroite
		movzx	ebx, PX2
;		movzx	eax, PY2
		movzx	edx, PX0
;		movzx	ecx, PY0
		call	A_FillPropNoClip

		mov	edi, offset TabCoulD
		movzx	ebx, PI2
;		shl	ebx, 8
		movzx	eax, PY2
		movzx	edx, PI0
;		shl	edx, 8
		movzx	ecx, PY0
		call	A_FillPropNoClip
ttnc0e:

		mov	ax, word ptr[LYmin]
		mov	Ymin, ax

		mov	ax, word ptr[LYmax]
		mov	Ymax, ax

		ret

AsmGouraudTriangleNoClip	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;               ฿฿Û฿฿ Û฿฿฿฿ ฿Ü Ü฿ ฿฿Û฿฿ Û   Û Û฿฿฿Û Û฿฿฿฿ ÛÛ฿฿฿
;                 ÛÛ  ÛÛ฿฿   Ü฿Ü    ÛÛ  ÛÛ  Û ÛÛ฿Û฿ ÛÛ฿฿  ฿฿฿฿Û
;                 ฿฿  ฿฿฿฿฿ ฿   ฿   ฿฿  ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

FillTextPolyShade	proc	uses esi edi ebx ebp,\
			pymin:DWORD, pymax:DWORD, dark:DWORD, source:DWORD

		mov	eax, dark
		mov	[darkdec], ax

		mov	edi, [Log]

		mov	ebx, pymin
		add	edi, TabOffLine[ ebx*4 ]

		mov	[ptrlig], edi

		mov	eax, source
		mov	[ptrmap], eax

		mov	ecx, pymax
		sub	ecx, ebx
		inc	ecx
		mov	[loopy], cx

		mov	esi, offset TabGauche
		shl	ebx, 1				; ymin * word
		add	esi, ebx

		mov	[ptrtab], esi

l3:		mov	edi, [ptrlig]
		mov	esi, [ptrtab]

		movsx	eax, word ptr[ esi + TAB_GAUCHE ]
;		add	edi, eax

		movsx	ecx, word ptr[ esi + TAB_DROITE ]
		sub	ecx, eax
		jle	l4			; prevoir cas 1 point

		movzx	eax, word ptr[ esi + TAB_Y0 ]
		movzx	ebp, word ptr[ esi + TAB_Y1 ]
		sub	eax, ebp
		neg	eax				; -(y0-y1)
		inc	eax
		cdq
		idiv	ecx
		mov	[ystep], eax			; step y source

		movzx	eax, word ptr[ esi + TAB_X1 ]	; delta x
		movzx	ebp, word ptr[ esi + TAB_X0 ]
		sub	eax, ebp
		inc	eax
		cdq
		idiv	ecx
		mov	[xstep], eax			; step x source

		xor	eax, eax
		mov	ax, word ptr[ esi + TAB_GAUCHE ]
		cmp	ax, word ptr[ ClipXmin ]
		jge	noclpleft

		; clip x min

		sub	ax, word ptr[ ClipXmin ]
		mov	bx, ax

		imul	word ptr[xstep]
		sub	word ptr[ esi + TAB_X0 ], ax

		mov	ax, bx
		imul	word ptr[ystep]
		sub	word ptr[ esi + TAB_Y0 ], ax

		mov	ax, word ptr[ ClipXmin ]

noclpleft:      add	edi, eax

		mov	cx, word ptr[ esi + TAB_DROITE ]
		cmp	cx, word ptr[ ClipXmax ]
		jle	noclpright

		; clip x max

		mov	cx, word ptr[ ClipXmax ]

noclpright:	sub	cx, ax
		inc	cx
		jle	l4

		mov	[boucle], cx

		mov	ecx, [ptrmap]

		mov	dx, word ptr[ esi + TAB_X0 ]
		xor	ebx, ebx
		mov	bx, word ptr[ esi + TAB_Y0 ]	; precision ?

		mov	esi, [xstep]
		mov	ebp, [ystep]

	ALIGN 4
l2:		xchg	bl,dh				; bl <- dh colonne
		mov	al,byte ptr[ecx+ebx]		; bh ligne * 256
		xchg	bl,dh

	mov	ah, al
	and	al, 15
	sub	al, byte ptr[darkdec]
	jc	ovfshd
	and	ah, 240
	or	al, ah

		mov	byte ptr[edi], al
		inc	edi

		add	dx, si
		add	bx, bp

		dec	word ptr[boucle]
		jnz	l2


l4:		add	[ptrlig], 640
		add	[ptrtab], 2

		dec	word ptr[loopy]
		jnz	l3

		ret

ovfshd:
	and	ah, 240
	mov	al, ah

		mov	byte ptr[edi], al
		inc	edi

		add	dx, si
		add	bx, bp

		dec	word ptr[boucle]
		jnz	l2

		add	[ptrlig], 640
		add	[ptrtab], 2

		dec	word ptr[loopy]
		jnz	l3

		ret

FillTextPolyShade	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

FillTextPoly	proc	uses esi edi ebx ebp,\
		pymin:DWORD, pymax:DWORD, source:DWORD

		mov	edi, [Log]

		mov	ebx, pymin
		add	edi, TabOffLine[ ebx*4 ]

		mov	[ptrlig], edi

		mov	eax, source
		mov	[ptrmap], eax

		mov	ecx, pymax
		sub	ecx, ebx
		inc	ecx
		mov	[loopy], cx

		mov	esi, offset TabGauche
		shl	ebx, 1				; ymin * word
		add	esi, ebx

		mov	[ptrtab], esi

l3:		mov	edi, [ptrlig]
		mov	esi, [ptrtab]

		movsx	eax, word ptr[ esi + TAB_GAUCHE ]
;		add	edi, eax

		movsx	ecx, word ptr[ esi + TAB_DROITE ]
		sub	ecx, eax
		jle	l4			; prevoir cas 1 point

		movzx	eax, word ptr[ esi + TAB_Y0 ]
		movzx	ebp, word ptr[ esi + TAB_Y1 ]
		sub	eax, ebp
		neg	eax				; -(y0-y1)
		inc	eax
		cdq
		idiv	ecx
		mov	[ystep], eax			; step y source

		movzx	eax, word ptr[ esi + TAB_X1 ]	; delta x
		movzx	ebp, word ptr[ esi + TAB_X0 ]
		sub	eax, ebp
		inc	eax
		cdq
		idiv	ecx
		mov	[xstep], eax			; step x source

		xor	eax, eax
		mov	ax, word ptr[ esi + TAB_GAUCHE ]
		cmp	ax, word ptr[ ClipXmin ]
		jge	noclpleft

		; clip x min

		sub	ax, word ptr[ ClipXmin ]
		mov	bx, ax

		imul	word ptr[xstep]
		sub	word ptr[ esi + TAB_X0 ], ax

		mov	ax, bx
		imul	word ptr[ystep]
		sub	word ptr[ esi + TAB_Y0 ], ax

		mov	ax, word ptr[ ClipXmin ]

noclpleft:      add	edi, eax

		mov	cx, word ptr[ esi + TAB_DROITE ]
		cmp	cx, word ptr[ ClipXmax ]
		jle	noclpright

		; clip x max

		mov	cx, word ptr[ ClipXmax ]

noclpright:	sub	cx, ax
		inc	cx
		jle	l4

		mov	[boucle], cx

		mov	ecx, [ptrmap]

		mov	dx, word ptr[ esi + TAB_X0 ]
		xor	ebx, ebx
		mov	bx, word ptr[ esi + TAB_Y0 ]	; precision ?

		mov	esi, [xstep]
		mov	ebp, [ystep]

	ALIGN 4
l2:		xchg	bl,dh				; bl <- dh colonne
		mov	al,byte ptr[ecx+ebx]		; bh ligne * 256
		xchg	bl,dh

		mov	byte ptr[edi], al
		inc	edi

		add	dx, si
		add	bx, bp

		dec	word ptr[boucle]
		jnz	l2


l4:		add	[ptrlig], 640
		add	[ptrtab], 2

		dec	word ptr[loopy]
		jnz	l3


		ret

FillTextPoly	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

FillTextPolyNoClip	proc	uses esi edi ebx ebp,\
			pymin:DWORD, pymax:DWORD, source:DWORD

;	for( y=ymin; y<=ymax; y++ )
;	{
;		xg = TabGauche[y] ;
;		xd = TabDroite[y] ;
;
;		x0 = TabX0[y] ;
;		y0 = TabY0[y] ;
;
;		x1 = TabX1[y] ;
;		y1 = TabY1[y] ;
;
;		CopyLine( xg, y, xd-xg+1,  x0,y0, x1,y1, ptrmap ) ;
;	}

		mov	edi, [Log]

		mov	ebx, pymin
		add	edi, TabOffLine[ ebx*4 ]

		mov	[ptrlig], edi

		mov	eax, source
		mov	[ptrmap], eax

		mov	ecx, pymax
		sub	ecx, ebx
		inc	ecx
		mov	[loopy], cx

		mov	esi, offset TabGauche
		shl	ebx, 1				; ymin * word
		add	esi, ebx

		mov	[ptrtab], esi

l3:
;'		mov	ebx, [ptrmap]
		mov	edi, [ptrlig]
		mov	esi, [ptrtab]

		movsx	eax, word ptr[ esi + TAB_GAUCHE ]
		add	edi, eax

		movsx	ecx, word ptr[ esi + TAB_DROITE ]
		sub	ecx, eax
;'		inc	ecx
		jle	l4

		movzx	eax, word ptr[ esi + TAB_Y0 ]
;'		mov	edx, eax
;'		and	edx, 0FFFFFF00h

;'		add	ebx, edx			; + start buf

		movzx	ebp, word ptr[ esi + TAB_Y1 ]
		sub	eax, ebp
		neg	eax				; -(y0-y1)
		inc	eax
		cdq
		idiv	ecx
		mov	[ystep], eax			; step y source

		movzx	eax, word ptr[ esi + TAB_X1 ]	; delta x
		movzx	ebp, word ptr[ esi + TAB_X0 ]
		sub	eax, ebp
		inc	eax
		cdq
		idiv	ecx
		mov	[xstep], eax			; step x source
		mov	edx, ebp 			; word ptr[ esi + TAB_X0 ]


		mov	[boucle], cx

;'		mov	ecx, ebx			; ecx ptr source
		mov	ecx, [ptrmap]

		xor	ebx, ebx
		mov	bx, word ptr[ esi + TAB_Y0 ]	; precision ?
;'		and	ebx, 0FFh

		mov	esi, [xstep]
		mov	ebp, [ystep]

	ALIGN 4
l2:		xchg	bl,dh				; bl <- dh colonne

		mov	al,byte ptr[ecx+ebx]		; bh ligne * 256

		xchg	bl,dh

		mov	byte ptr[edi], al
		inc	edi

;'		add	edx, esi
;'		add	ebx, ebp

		add	dx, si
		add	bx, bp

		dec	word ptr[boucle]
		jnz	l2


l4:		add	[ptrlig], 640
		add	[ptrtab], 2

		dec	word ptr[loopy]
		jnz	l3

		ret

FillTextPolyNoClip	endp


;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		END

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

M_FillTextPoly	proc	uses esi esi ebx ebp,\
		ymin:DWORD, ymax:DWORD, source:DWORD

		mov	edi, [M_Log]

		mov	ebx, ymin

		mov	eax, ebx
		shr	eax, 2
		add	edi, eax	; TabOffLine[ ebx*4 ]
		add	edi, 239*80	; x = 0 = y = 239;

		mov	[ptrlig], edi

		mov	eax, ebx
		and	eax, 3
		mov	al, byte ptr[Mask_p+eax]
		mov	byte ptr[maskplane], al

		mov	eax, source
		mov	[ptrmap], eax

		mov	ecx, ymax
		sub	ecx, ebx
		inc	ecx
		mov	[loopy], cx

		mov	esi, offset TabGauche
		shl	ebx, 1				; ymin * word
		add	esi, ebx

		mov	[ptrtab], esi


l3:
		mov	ah, byte ptr[maskplane]

		mov     dx,3C4h
		mov     al,02h			; slectionne MAP MASK REGISTER
		out     dx,ax

		mov	edi, [ptrlig]
		mov	esi, [ptrtab]

		movsx	eax, word ptr[ esi + TAB_GAUCHE ]

		movsx	ecx, word ptr[ esi + TAB_DROITE ]
		sub	ecx, eax
		jle	l4			; prevoir cas 1 point

		movzx	eax, word ptr[ esi + TAB_Y0 ]
		movzx	ebp, word ptr[ esi + TAB_Y1 ]
		sub	eax, ebp
		neg	eax				; -(y0-y1)
		inc	eax
		cdq
		idiv	ecx
		mov	[ystep], eax			; step y source

		movzx	eax, word ptr[ esi + TAB_X1 ]	; delta x
		movzx	ebp, word ptr[ esi + TAB_X0 ]
		sub	eax, ebp
		inc	eax
		cdq
		idiv	ecx
		mov	[xstep], eax			; step x source

		xor	eax, eax
		mov	ax, word ptr[ esi + TAB_GAUCHE ]
		mov	bx, 239
		sub	bx, word ptr[ ClipYmax ]
		cmp	ax, bx
		jge	noclpleft

		; clip x min

		mov	bx, 239
		sub	bx, word ptr[ ClipYmax ]
		sub	ax, bx
		mov	bx, ax

		imul	word ptr[xstep]
		sub	word ptr[ esi + TAB_X0 ], ax

		mov	ax, bx
		imul	word ptr[ystep]
		sub	word ptr[ esi + TAB_Y0 ], ax

		mov	ax, 239
		sub	ax, word ptr[ ClipYmax ]

noclpleft:	mov	ecx, eax
		shl	eax, 2
		add	eax, ecx
		shl	eax, 4			; *80
		sub	edi, eax		; sur bonne ligne
		mov	eax, ecx

		mov	cx, word ptr[ esi + TAB_DROITE ]
		mov	dx, 239
		sub	dx, word ptr[ ClipYmin ]
		cmp	cx, dx
		jle	noclpright

		; clip x max

		mov	cx, dx 	;	mov	cx, word ptr[ ClipXmax ]

noclpright:	sub	cx, ax
		inc	cx
		jle	l4

		mov	[boucle], cx

		mov	ecx, [ptrmap]

		mov	dx, word ptr[ esi + TAB_X0 ]
		xor	ebx, ebx
		mov	bx, word ptr[ esi + TAB_Y0 ]	; precision ?

		mov	esi, [xstep]
		mov	ebp, [ystep]

	ALIGN 4
l2:		xchg	bl,dh				; bl <- dh colonne
		mov	al,byte ptr[ecx+ebx]		; bh ligne * 256
		xchg	bl,dh

		mov	byte ptr[edi], al
		sub	edi, 80

		add	dx, si
		add	bx, bp

		dec	word ptr[boucle]
		jnz	l2


l4:
		shl	byte ptr[maskplane], 1
		cmp	byte ptr[maskplane], 16
		jnz	l4a

		mov	byte ptr[maskplane], 1
		add	[ptrlig], 1

l4a:
		add	[ptrtab], 2

		dec	word ptr[loopy]
		jnz	l3


		ret

M_FillTextPoly	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

M_AsmFillProp	proc uses esi edi ebx ebp,\
		ptrdest:DWORD, x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD

		mov	edi, ptrdest

		mov	eax, y0

		mov	ecx, y1

		mov	ebx, x0
		mov	edx, x1

		cmp	eax, ecx		; y0 < y1
		jle	afp0

		xchg	eax, ecx		; y0 <> y1
		xchg	ebx, edx		; x0 <> x1

afp0:
		cmp	eax, [ClipXmax]
		jg	nofill

		cmp	ecx, [ClipXmin]
		jl	nofill

		cmp	eax, [ClipXmin]
		jl	clip_ymin

afp01:		cmp	ecx, [ClipXmax]
		jle	afp00

clip_ymax:

		push	edi
		push	eax

		mov	edi, ebx
		sub	edi, edx		; dx

		mov     esi, eax
		sub	esi, ecx		; dy
		jz	nofill2

		mov	ebp, edx

		mov	eax, ecx		; y1

		sub	eax, [ClipXmax]		; ymax - y1
		neg	eax
		imul	edi			; * dx
		idiv	esi			; / dy
		add     eax, ebp		; += x1
		mov	edx, eax
		mov	ecx, [ClipXmax]		; y1 = ymax

		pop	eax
		pop	edi

		jmp	afp00

clip_ymin:

		push	edi
		push	edx

		mov	edi, ebx
		sub	edi, edx		; dx

		mov     esi, eax
		sub	esi, ecx		; dy
		jz	nofill1

		mov	ebp, ebx

		sub	eax, [ClipXmin]		; ymin - y0
		neg	eax
		imul	edi			; * dx
		idiv	esi			; / dy
		add     eax, ebp		; += x0
		mov	ebx, eax
		mov	eax, [ClipXmin]		; y0 = ymin

		pop	edx
		pop	edi

		jmp	afp01

afp00:		sub	ecx, eax		; ecx deltay >= 0


		jz	nofill		; GASP mettre cas 1 ligne


		shl	eax, 1
		add	edi, eax		; (WORD*)ptrdest[y0]

		cmp	ebx, edx
		jg	afp1

		; x0 < x1

		mov	ax, dx			; x0
		sub	ax, bx			; x1 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		xchg	edx, eax
;'		shr	ax, 1
;'		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc

		mov	ax, bx

		inc	ecx
;'		add	ecx, 16
;'		shr	ecx, 4

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
lt0:
;'	REPT	16
		stosw
		adc	eax, edx
;'	ENDM
		dec	ecx
		jnz	lt0			; pas touche C ?

;'		dec	ecx
;'		jnz	lt0

		ret

afp1:		; x0 > x1

		mov	ax, bx			; x1
		sub	ax, dx			; x0 delta X (>0)

		shl	eax, 16
		xor	edx, edx
		div	ecx

		xchg	edx, eax
;'		shr	ax, 1
;'		add	ax, 7FFFh
		rol	edx, 16
		shl	eax, 16

		clc				; la 1ere valeur est bonne
		mov	ax, bx			; x0

		inc	ecx
;'		add	ecx, 16
;'		shr	ecx, 4

		; EAX = .cumul:X
		; EDX = .DeltaX:DeltaX
	ALIGN 4
afp2:
;'	REPT	16
		stosw
		sbb	eax, edx
;'	ENDM
		dec	ecx
		jnz	afp2

;'		dec	ecx
;'		jnz	afp2

nofill:		ret

nofill1:	pop	edx
		pop	edi
		ret

nofill2:	pop	eax
		pop	edi
		ret

M_AsmFillProp	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;	si = shape list
;	bx = y list
;	bp = x list
;	ax = texture resource
;

disptexture	proc

	push	ds

	mov	cx,dgroup
	mov	ds,cx
	mov	ax, es
	mov	fs, ax				; data texture
	mov	es, Log

	add	si,cl_miny
	add	bx,cl_leftx
	add	bp,cl_leftx

	lodsw
	mov	di,ax
	lodsw
	sub	ax,di
	js	#dispret
	add	si,2
	mov	boucle,ax

	shl	di,1
	mov	di,scantab[di]

blig:

		lodsw				; xd1
		mov	cx, ax
		lodsw				; xd2
		sub	ax, cx
		inc	ax
		jle	h1
		mov	cx, ax			; delta xd

		mov	ax, [bx+2]		; y2
		sub	ax, [bx]		; y1
		cwd
		idiv	cx

		mov	stepy, ax

		mov	ax, ds:[bp+2]		; x2
		sub	ax, ds:[bp]		; x1
		cwd
		idiv	cx
		mov	stepx, ax

		mov	dx, [bx]

		mov	ax, [si-4]		; xd1
		cmp	ax, minx1
		jge	l0

		sub	ax, minx1
		push	ax
		imul	stepx
		sub	ds:[bp], ax
		pop	ax
		imul	stepy
		sub	[bx], ax
		mov	ax, minx1

l0:		mov	cx, [si-2]		; xd2
		cmp	cx, maxx1
		jle	l1
		mov	cx, maxx1
l1:		sub	cx, ax
		inc	cx
		jle	h1

		push	di
		push	bx
		push	bp
		push	si

		add	di, ax
		mov	dx, ds:[bp]
		mov	bx, ds:[bx]
		mov	si, xdda
		mov	bp, ydda

bcol:		xchg	bl,dh
		mov	al,fs:sp_data[bx]
		xchg	bl,dh
		stosb
		add	dx,si
		add	bx,bp
		dec	ecx
		jnz	bcol

		pop	si
		pop	bp
		pop	bx
		pop	di

h1:		add	bx,4			; next xy
		add	bp,4			; next xy
		add	di,640			; next lig

		dec	boucle
		jns	#solidloop

#dispret:	pop	ds
		ret

disptexture	endp


;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

AsmTexturedTriangleNoClip	proc uses esi edi ebx ebp

;'		WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2,
;'		LONG tx0, LONG ty0, LONG tx1, LONG ty1, LONG tx2, LONG ty2,
;'		UBYTE *ptrmap )
;'{
;'	LONG	y, ymin, ymax, xd, xg ;
;'	WORD	tabpoly[3*4] ;


;'	tabpoly[1] = x0 ;
;'	tabpoly[2] = y0 ;
;'	tabpoly[4] = x1 ;
;'	tabpoly[5] = y1 ;
;'	tabpoly[7] = x2 ;
;'	tabpoly[8] = y2 ;
;'
;'	if( !TestVuePoly( tabpoly ) )	return ;

PX0	=	word ptr[ offset TabPoly + 1*2 ]
PY0	=	word ptr[ offset TabPoly + 2*2 ]
PX1	=	word ptr[ offset TabPoly + 4*2 ]
PY1	=	word ptr[ offset TabPoly + 5*2 ]
PX2	=	word ptr[ offset TabPoly + 7*2 ]
PY2	=	word ptr[ offset TabPoly + 8*2 ]

TX0	=	word ptr[ offset TabText + 1*2 ]
TY0	=	word ptr[ offset TabText + 2*2 ]
TX1	=	word ptr[ offset TabText + 4*2 ]
TY1	=	word ptr[ offset TabText + 5*2 ]
TX2	=	word ptr[ offset TabText + 7*2 ]
TY2	=	word ptr[ offset TabText + 8*2 ]

;'	ymin = 32000 ;
;'	ymax = -32000 ;

	mov	LYmin, 32000
	mov	LYmax, -32000

;'	if( y0 < y1 )
;'	{
;'		if( y0 < ymin ) ymin = y0 ;
;'		if( y1 > ymax )	ymax = y1 ;

	movzx	eax, PY0
	movzx	ecx, PY1

	cmp	eax, ecx
	jge	ttnc0

		cmp	eax, LYmin
		jg	ttnc1
		mov	LYmin, eax
ttnc1:
		cmp	ecx, LYmax
		jl	ttnc2
		mov	LYmax, ecx
ttnc2:

;'		AsmFillPropNoClip( TabGauche, x0,  y0, x1,   y1 ) ;

		mov	edi, offset TabGauche
		movzx	ebx, PX0
;		movzx	eax, PY0
		movzx	edx, PX1
;		movzx	ecx, PY1

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabX0,   tx0,   y0, tx1,  y1 ) ;

		mov	edi, offset TabX0
		movzx	ebx, TX0
		movzx	eax, PY0
		movzx	edx, TX1
		movzx	ecx, PY1

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabY0,   ty0,   y0, ty1,  y1 ) ;

		mov	edi, offset TabY0
		movzx	ebx, TY0
		movzx	eax, PY0
		movzx	edx, TY1
		movzx	ecx, PY1

		call	A_FillPropNoClip

;'	}
ttnc0:

;'	if( y0 > y1 )
;'	{
;'		if( y0 > ymax )	ymax = y0 ;
;'		if( y1 < ymin ) ymin = y1 ;

	movzx	eax, PY0
	movzx	ecx, PY1

	cmp	eax, ecx
	jle	ttnc0a

		cmp	ecx, LYmin
		jg	ttnc1a
		mov	LYmin, ecx
ttnc1a:
		cmp	eax, LYmax
		jl	ttnc2a
		mov	LYmax, eax
ttnc2a:



;'		AsmFillPropNoClip( TabDroite, x0,y0, x1,y1 ) ;

		mov	edi, offset TabDroite
		movzx	ebx, PX0
;		movzx	eax, PY0
		movzx	edx, PX1
;		movzx	ecx, PY1

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabX1, tx0, y0, tx1, y1 ) ;

		mov	edi, offset TabX1
		movzx	ebx, TX0
		movzx	eax, PY0
		movzx	edx, TX1
		movzx	ecx, PY1

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabY1, ty0, y0, ty1, y1 ) ;

		mov	edi, offset TabY1
		movzx	ebx, TY0
		movzx	eax, PY0
		movzx	edx, TY1
		movzx	ecx, PY1

		call	A_FillPropNoClip
;'	}
ttnc0a:

;'	if( y1 < y2 )
;'	{
;'		if( y1 < ymin ) ymin = y1 ;
;'		if( y2 > ymax )	ymax = y2 ;

	movzx	eax, PY1
	movzx	ecx, PY2

	cmp	eax, ecx
	jge	ttnc0b

		cmp	eax, LYmin
		jg	ttnc1b
		mov	LYmin, eax
ttnc1b:
		cmp	ecx, LYmax
		jl	ttnc2b
		mov	LYmax, ecx
ttnc2b:



;'		AsmFillPropNoClip( TabGauche, x1,y1, x2,y2 ) ;

		mov	edi, offset TabGauche
		movzx	ebx, PX1
;		movzx	eax, PY1
		movzx	edx, PX2
;		movzx	ecx, PY2

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabX0, tx1, y1, tx2, y2 ) ;

		mov	edi, offset TabX0
		movzx	ebx, TX1
		movzx	eax, PY1
		movzx	edx, TX2
		movzx	ecx, PY2

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabY0, ty1, y1, ty2, y2 ) ;

		mov	edi, offset TabY0
		movzx	ebx, TY1
		movzx	eax, PY1
		movzx	edx, TY2
		movzx	ecx, PY2

		call	A_FillPropNoClip
;'	}
ttnc0b:

;'	if( y1 > y2 )
;'	{
;'		if( y1 > ymax )	ymax = y1 ;
;'		if( y2 < ymin ) ymin = y2 ;

	movzx	eax, PY1
	movzx	ecx, PY2

	cmp	eax, ecx
	jle	ttnc0c

		cmp	ecx, LYmin
		jg	ttnc1c
		mov	LYmin, ecx
ttnc1c:
		cmp	eax, LYmax
		jl	ttnc2c
		mov	LYmax, eax
ttnc2c:

;'		AsmFillPropNoClip( TabDroite, x1,y1, x2,y2 ) ;

		mov	edi, offset TabDroite
		movzx	ebx, PX1
;		movzx	eax, PY1
		movzx	edx, PX2
;		movzx	ecx, PY2

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabX1, tx1, y1, tx2, y2 ) ;

		mov	edi, offset TabX1
		movzx	ebx, TX1
		movzx	eax, PY1
		movzx	edx, TX2
		movzx	ecx, PY2

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabY1, ty1, y1, ty2, y2 ) ;

		mov	edi, offset TabY1
		movzx	ebx, TY1
		movzx	eax, PY1
		movzx	edx, TY2
		movzx	ecx, PY2

		call	A_FillPropNoClip
;'	}
ttnc0c:

;'	if( y2 < y0 )
;'	{
;'		if( y2 < ymin ) ymin = y2 ;
;'		if( y0 > ymax )	ymax = y0 ;

	movzx	eax, PY2
	movzx	ecx, PY0

	cmp	eax, ecx
	jge	ttnc0d

		cmp	eax, LYmin
		jg	ttnc1d
		mov	LYmin, eax
ttnc1d:
		cmp	ecx, LYmax
		jl	ttnc2d
		mov	LYmax, ecx
ttnc2d:

;'		AsmFillPropNoClip( TabGauche, x2,y2, x0,y0 ) ;

		mov	edi, offset TabGauche
		movzx	ebx, PX2
;		movzx	eax, PY2
		movzx	edx, PX0
;		movzx	ecx, PY0

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabX0, tx2, y2, tx0, y0 ) ;

		mov	edi, offset TabX0
		movzx	ebx, TX2
		movzx	eax, PY2
		movzx	edx, TX0
		movzx	ecx, PY0

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabY0, ty2, y2, ty0, y0 ) ;

		mov	edi, offset TabY0
		movzx	ebx, TY2
		movzx	eax, PY2
		movzx	edx, TY0
		movzx	ecx, PY0

		call	A_FillPropNoClip
;'	}
ttnc0d:


;'	if( y2 > y0 )
;'	{
;'		if( y2 > ymax )	ymax = y2 ;
;'		if( y0 < ymin ) ymin = y0 ;

	movzx	eax, PY2
	movzx	ecx, PY0

	cmp	eax, ecx
	jle	ttnc0e

		cmp	ecx, LYmin
		jg	ttnc1e
		mov	LYmin, ecx
ttnc1e:
		cmp	eax, LYmax
		jl	ttnc2e
		mov	LYmax, eax
ttnc2e:



;'		AsmFillPropNoClip( TabDroite, x2,y2, x0,y0 ) ;

		mov	edi, offset TabDroite
		movzx	ebx, PX2
;		movzx	eax, PY2
		movzx	edx, PX0
;		movzx	ecx, PY0

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabX1, tx2, y2, tx0, y0 ) ;

		mov	edi, offset TabX1
		movzx	ebx, TX2
		movzx	eax, PY2
		movzx	edx, TX0
		movzx	ecx, PY0

		call	A_FillPropNoClip

;'		AsmFillPropNoClip( TabY1, ty2, y2, ty0, y0 ) ;

		mov	edi, offset TabY1
		movzx	ebx, TY2
		movzx	eax, PY2
		movzx	edx, TY0
		movzx	ecx, PY0

		call	A_FillPropNoClip
;'	}
ttnc0e:

;'	if( ymin < ClipYmin )	ymin = ClipYmin ;
;'	if( ymax > ClipYmax )	ymax = ClipYmax ;
;'	if( ymin > ClipYmax )	return ;
;'	if( ymax < ClipYmin )	return ;

;'	FillTextPolyNoClip( ymin, ymax, ptrmap ) ;
;'}
		ret

AsmTexturedTriangleNoClip	endp

comment	@
/*
void	TexturedTriangleNoClip(
		WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2,
		LONG tx0, LONG ty0, LONG tx1, LONG ty1, LONG tx2, LONG ty2,
		UBYTE *ptrmap )
{
	LONG	y, ymin, ymax, xd, xg ;
	WORD	tabpoly[3*4] ;


	tabpoly[1] = x0 ;
	tabpoly[2] = y0 ;
	tabpoly[4] = x1 ;
	tabpoly[5] = y1 ;
	tabpoly[7] = x2 ;
	tabpoly[8] = y2 ;

	if( !TestVuePoly( tabpoly ) )	return ;

	ymin = 32000 ;
	ymax = -32000 ;

	if( y0 < y1 )
	{
		if( y0 < ymin ) ymin = y0 ;
		if( y1 > ymax )	ymax = y1 ;

		AsmFillPropNoClip( TabGauche, x0,  y0, x1,   y1 ) ;
		AsmFillPropNoClip( TabX0,   tx0,   y0, tx1,  y1 ) ;
		AsmFillPropNoClip( TabY0,   ty0,   y0, ty1,  y1 ) ;

	}

	if( y0 > y1 )
	{
		if( y0 > ymax )	ymax = y0 ;
		if( y1 < ymin ) ymin = y1 ;

		AsmFillPropNoClip( TabDroite, x0,y0, x1,y1 ) ;

		AsmFillPropNoClip( TabX1, tx0, y0, tx1, y1 ) ;
		AsmFillPropNoClip( TabY1, ty0, y0, ty1, y1 ) ;
	}

	if( y1 < y2 )
	{
		if( y1 < ymin ) ymin = y1 ;
		if( y2 > ymax )	ymax = y2 ;
		AsmFillPropNoClip( TabGauche, x1,y1, x2,y2 ) ;

		AsmFillPropNoClip( TabX0, tx1, y1, tx2, y2 ) ;
		AsmFillPropNoClip( TabY0, ty1, y1, ty2, y2 ) ;
	}

	if( y1 > y2 )
	{
		if( y1 > ymax )	ymax = y1 ;
		if( y2 < ymin ) ymin = y2 ;
		AsmFillPropNoClip( TabDroite, x1,y1, x2,y2 ) ;

		AsmFillPropNoClip( TabX1, tx1, y1, tx2, y2 ) ;
		AsmFillPropNoClip( TabY1, ty1, y1, ty2, y2 ) ;
	}

	if( y2 < y0 )
	{
		if( y2 < ymin ) ymin = y2 ;
		if( y0 > ymax )	ymax = y0 ;

		AsmFillPropNoClip( TabGauche, x2,y2, x0,y0 ) ;

		AsmFillPropNoClip( TabX0, tx2, y2, tx0, y0 ) ;
		AsmFillPropNoClip( TabY0, ty2, y2, ty0, y0 ) ;
	}

	if( y2 > y0 )
	{
		if( y2 > ymax )	ymax = y2 ;
		if( y0 < ymin ) ymin = y0 ;

		AsmFillPropNoClip( TabDroite, x2,y2, x0,y0 ) ;

		AsmFillPropNoClip( TabX1, tx2, y2, tx0, y0 ) ;
		AsmFillPropNoClip( TabY1, ty2, y2, ty0, y0 ) ;
	}


	if( ymin < ClipYmin )	ymin = ClipYmin ;
	if( ymax > ClipYmax )	ymax = ClipYmax ;

	if( ymin > ClipYmax )	return ;
	if( ymax < ClipYmin )	return ;

	FillTextPolyNoClip( ymin, ymax, ptrmap ) ;
}
*/
	@

