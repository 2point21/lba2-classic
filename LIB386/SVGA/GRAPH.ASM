;----------------------------------------------------------------------------
			.386p
			.model SMALL, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			EXTRN   SYSCALL      ClipXMin        :DWORD
			EXTRN   SYSCALL      ClipYMin        :DWORD
			EXTRN   SYSCALL      ClipXMax        :DWORD
			EXTRN   SYSCALL      ClipYMax        :DWORD
			EXTRN   SYSCALL      ScreenXMin      :DWORD
			EXTRN   SYSCALL      ScreenYMin      :DWORD
			EXTRN   SYSCALL      ScreenXMax      :DWORD
			EXTRN   SYSCALL      ScreenYMax      :DWORD
			EXTRN   SYSCALL      Log             :DWORD
			EXTRN   SYSCALL      TabOffLine      :DWORD

;----------------------------------------------------------------------------
;               DWORD   TabOffset[]     ( Un de plus pointe fin de fichier )
;
;                       Brick:
;
;                           - BYTE Delta X
;                           - BYTE Delta Y
;                           - BYTE HotX (Signed)
;                           - BYTE HotY (Signed)
;
;                               Pour chaque Ligne ( Delta Y ):
;
;                               - BYTE NbBlock
;
;                                  Par Block:
;                                   - 00xxxxxxb  xxxxxx zeros to jump
;                                   - 01xxxxxxb  xxxxxx Copy Pix
;                                   - 10xxxxxxb  xxxxxx Repeat Pix
;                                   - BYTE datas[]

;----------------------------------------------------------------------------
OffsetBegin	dd	0
NbPix		dd	0
BufferClip	db	512 dup (?)

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	AffGraph
			PUBLIC	SYSCALL	GetDxDyGraph
			PUBLIC	SYSCALL	GetBoxGraph

;----------------------------------------------------------------------------
;S32	AffGraph(S32 numgraph, S32 x, S32 y, void *bankgraph)	;

;#pragma aux AffGraph	"*"		\
;	parm	[eax] [ebx] [ecx] [esi]	\
;	modify	[edx edi]

AffGraph		proc	uses ebp

			add	esi, [esi+eax*4]	; ESI = Begin Data

			movsx	eax, byte ptr[esi+2]	; Hot X
			movsx	edx, byte ptr[esi+3]	; Hot Y

			add	ebx, eax
			add	ecx, edx

			xor	edx, edx
			xor	eax, eax

			mov	dl, [esi]		; Delta X
			mov	al, [esi+1]		; Nb Line ( Delta Y )

			add	esi, 4			; Jump Hot X & Hot Y

;----------------------------------------------- Test Clipping

			add	edx, ebx
			add	eax, ecx
			dec	edx
			dec	eax

			mov	[ScreenXMin], ebx
			mov	[ScreenYMin], ecx
			mov	[ScreenXMax], edx
			mov	[ScreenYMax], eax

			cmp	ebx, ClipXMin
			jl	ClippingGraph
			cmp	ecx, ClipYMin
			jl	ClippingGraph
			cmp	edx, ClipXMax
			jg	ClippingGraph
			cmp	eax, ClipYMax
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
			sub	edx, TabOffLine[1*4]; EDX = Offset Screen
			neg	edx		; EDX = ScreenWidth-edx
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
			cmp	ebx, ClipXMax
			jg      EndGraph
			cmp	ecx, ClipYMax
			jg	EndGraph
			cmp	edx, ClipXMin
			jl	EndGraph
			cmp	eax, ClipYMin
			jl	EndGraph

			cmp	ecx, ClipYMin
			jge	PasHaut

;---------------------- Clipping Haut, Saute ClipYMin-ECX Line(s)

			mov	ebp, eax
			mov	edi, ebx

			mov	ebx, ClipYMin
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

			mov	ecx, ClipYMin		; New Y Haut

			mov	eax, ebp
			mov	ebx, edi

;---------------------- Clipping Bas
PasHaut:		cmp	eax, ClipYMax
			jle	PasBas

			mov	eax, ClipYMax

;---------------------- Clipping Gauche
PasBas:                 xor	edi, edi

			cmp     ebx, ClipXMin
			jge	PasGauche

			mov	edi, ClipXMin

			sub	edi, ebx
PasGauche:		mov	OffsetBegin, edi
;----------------------	Clipping Droit
			lea	edi, [edi+ebx-1]
			sub	edi, edx
			neg	edi
			mov	NbPix, edi

			sub	edx, ClipXMax
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
			add	edx, TabOffLine[1*4]
			mov	esi, ebp
			dec	bh
			jne	NextL
;----------------------
EndGraph:		ret

AffGraph		endp

;----------------------------------------------------------------------------
;S32	GetDxDyGraph(S32 numgraph, S32 *x, S32 *y, void *bankgraph);

;#pragma aux GetDxDyGraph	"*"		\
;	parm		[eax] [ebx] [ecx] [esi]	\
;	modify exact	[eax edx esi]

GetDxDyGraph		proc

			add     esi, [esi+eax*4]
			xor     eax, eax
			xor     edx, edx
			mov     al, [esi]
			mov     dl, [esi+1]
			mov     [ebx], eax
			mov     [ecx], edx

			ret

GetDxDyGraph		endp

;----------------------------------------------------------------------------
;S32	GetBoxGraph(S32 numgraph, S32 *x0, S32 *y0, S32 *x1, S32 *y1, void *bankgraph);

;#pragma aux GetBoxGraph	"*"				\
;	parm		[eax] [ebx] [ecx] [edx] [edi] [esi]	\
;	modify exact	[eax ebx ecx esi]

GetBoxGraph		proc	uses	ebp

			add	esi, [esi+eax*4]

			movsx	eax, byte ptr[esi+2]		; Hot X

			movsx	ebp, byte ptr[esi+3]		; Hot Y

			mov	dword ptr[ebx], eax
			mov	dword ptr[ecx], ebp

			xor	ebx, ebx
			xor	ecx, ecx

			mov	bl, byte ptr[esi]		; Dx
			mov	cl, byte ptr[esi+1]		; Dy

			add	eax, ebx
			add	ebp, ecx

			mov	dword ptr[edx], eax
			mov	dword ptr[edi], ebp

			ret

GetBoxGraph		endp

;----------------------------------------------------------------------------
;				The
				End
