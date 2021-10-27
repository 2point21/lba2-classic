;----------------------------------------------------------------------------
;				S_PHYS.ASM 386
;   			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p
				jumps
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data

			include	svga.ash

			extrn	NoLanguage	NewBank : DWORD

			public	NoLanguage	ScanLine
			public	NoLanguage	BankSize
			public	NoLanguage	BankOver
			public	NoLanguage	BankShift
			public	NoLanguage	NonStdVESA

ALIGN 4

Diff		dd	?
SaveEAX		dd	?
SaveEBX		dd	?
SaveEDX		dd	?
DeltaLog	dd	?

ScanLine	dd	640
BankSize	dd	00000FFFFh
BankOver	dd	0FFFF0000h
NonStdVESA	db	0
BankShift	db	16


CptLine		dw	0


;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CopyBlockPhysClip
			public	NoLanguage	CopyBlockPhys
			public	NoLanguage	Flip
			public	NoLanguage	Vsync

;----------------------------------------------------------------------------
;			Vsync()
;
Vsync			proc
			mov	dx, 03DAh
NotReady:		in	al, dx
			jmp	$+2
			test	al, 08h
			jnz	short NotReady
Ready:			in	al, dx
			jmp	$+2
			test	al, 08h
			jz	short Ready
			ret
Vsync			endp
;----------------------------------------------------------------------------
SuperMovsD		macro

			shr	ecx, 2
			rep	movsd
			mov	ecx, ebx
			and	ecx, 11b
			rep	movsb

			endm


SwitchBank		macro
			local	noswitch

			cmp	eax, BankCurrent
			je	noswitch
			mov	BankCurrent, eax
			call	[NewBank]
noswitch:
			endm


SwitchBankNoTest	macro

			mov	BankCurrent, eax
			call	[NewBank]

			endm


SwitchBankDirect	macro

			call	[NewBank]

			endm


ChangeVideoBank		macro

			mov	[SaveEAX], eax
			mov	eax, BankCurrent
			mov	[SaveEDX], edx
			inc	eax
			mov	BankCurrent, eax
			call	[NewBank]
			mov	eax, [SaveEAX]
			mov	edx, [SaveEDX]

			endm

;----------------------------------------------------------------------------
;			CopyBlockClip( x0, y0, x1, y1 )
;
CopyBlockPhysClip	proc	uses esi edi ebx ebp,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD

			mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1

;----------------------	Clipping Source Part 1

			cmp	edx, ClipXmax
			jg	CopyBlockPhysClip_End
			cmp	ebx, ClipXmin
			jl	CopyBlockPhysClip_End
			cmp	ecx, ClipYmax
			jg      CopyBlockPhysClip_End
			cmp	eax, ClipYmin
			jl	CopyBlockPhysClip_End

;---------------------- Clipping Source Part 2

			cmp	edx, ClipXmin   	; X0 < ClipXmin ?
			jge	Ok_X0_L			; A plus tart
			mov	edx, ClipXmin		; X0 = ClipXmin
Ok_X0_L:		cmp	ebx, ClipXmax		; X1 > ClipXmax
			jle	Ok_X1_R			; Goodbye
			mov	ebx, ClipXmax		; X1 = ClipXmax
Ok_X1_R:		cmp	ecx, ClipYmin		; Y0 < ClipYmin
			jge	Ok_Y0_U			; On se casse
			mov	ecx, ClipYmin		; Y0 = ClipYmin
Ok_Y0_U:		cmp	eax, ClipYmax		; Y1 > ClipYMax
			jle	Ok_Y1_D			; A priori non
			mov	eax, ClipYmax		; Y1 = ClipYmax
Ok_Y1_D:
;---------------------- Compute Delta X & Y

			sub	ebx, edx		; EBX Delta X
			sub	eax, ecx		; EAX Delta Y
			inc	ebx 			; EBX = Delta X + 1
			inc	eax                     ; EAX = Delta Y + 1

			mov	esi, TabOffLine[ecx*4]	; y0
			add	esi, edx		; SI Offset Src
			mov	edi, esi

			mov	ebp, eax
			mov	eax, edi
			shr	eax, 16
			SwitchBank			; Change Bank 64Kb
			mov	eax, ebp

			xor	ebp, ebp
			add	esi, Log
			and	edi, 0FFFFh		; 64Kb bank
			or	edi, 0A0000h		; EDI = Phys
			mov	bp, di			; BP For Bank

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen
;----------------------
Again:			add	bp, bx
			jc	NewBankNear0c
			mov	ecx, ebx

			SuperMovsD			; Macro

			add	esi, edx
			add	edi, edx
			add	bp, dx
			jc	NewBankNear1c
			dec	eax
			jne	short Again
;----------------------
CopyBlockPhysClip_End:	ret
;-----------------------------------------------------------------------------
NewBankNear0c:		mov	ecx, ebx
			sub	ecx, ebp
			push	ebx			; Sauve EBX
			mov	ebx, ecx

			SuperMovsD			; Macro

			ChangeVideoBank			: Macro

			mov	edi, 0A0000h		; Begin Phys
			mov	ecx, ebp
			mov	ebx, ebp

			SuperMovsD			; Macro

			pop	ebx     		; Restore EBX

			add	esi, edx
			add	edi, edx
			add	bp, dx
			jc	NewBankNear1c
			dec	eax
			jne	Again
			ret
;----------------------
NewBankNear1c:		ChangeVideoBank			; Macro
			sub	edi, 010000h		; Stay in Phys
			dec	eax
			jne	Again
			ret
CopyBlockPhysClip	endp
;----------------------------------------------------------------------------
;			CopyBlockPhys( x0, y0, x1, y1 )
;
CopyBlockPhys		proc	uses esi edi ebx ebp,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD

			cmp	[NonStdVESA], 0
			jne	CopyBlockPhys2

			mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1
;---------------------- Compute Delta X & Y
			sub	ebx, edx		; EBX Delta X
			sub	eax, ecx		; EAX Delta Y
			inc	ebx 			; EBX = Delta X + 1
			inc	eax                     ; EAX = Delta Y + 1
;---------------------- Compute Adresse
			mov	esi, TabOffLine[ecx*4]
			add	esi, edx
			mov	edi, esi

			mov	ebp, eax
			mov	eax, edi
			shr	eax, 16
			SwitchBank			; Change Bank 64Kb
			mov	eax, ebp

			add	esi, Log

			xor	ebp, ebp
			and	edi, 0FFFFh		; 64Kb bank
			or	edi, 0A0000h		; EDI = Phys
			mov	bp, di			; BP For Bank

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen
;----------------------
Again:			add	bp, bx
			jc	NewBankNear0p
			mov	ecx, ebx

			SuperMovsD			; Macro

			add	esi, edx
			add	edi, edx
			add	bp, dx
			jc	NewBankNear1p
			dec	eax
			jne	short Again
;----------------------
			ret
;-----------------------------------------------------------------------------
NewBankNear0p:		mov	ecx, ebx
			sub	ecx, ebp
			mov	[SaveEBX], ebx		; Sauve EBX
			mov	ebx, ecx

			SuperMovsD			; Macro

			ChangeVideoBank			: Macro

			mov	edi, 0A0000h		; Begin Phys
			mov	ecx, ebp
			mov	ebx, ebp

			SuperMovsD			; Macro

			mov	ebx, [SaveEBX] 		; Restore EBX

			add	esi, edx
			add	edi, edx
			add	bp, dx
			jc	NewBankNear1p
			dec	eax
			jne	Again
			ret
;----------------------
NewBankNear1p:		ChangeVideoBank			; Macro
			sub	edi, 010000h		; Stay in Phys
			dec	eax
			jne	Again
			ret
;----------------------------------------------------------------------------
;			CopyBlockPhys( x0, y0, x1, y1 )
;
CopyBlockPhys2:		mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1

;---------------------- Compute Delta X & Y
			sub	ebx, edx		; EBX Delta X
			sub	eax, ecx		; EAX Delta Y
			inc	ebx 			; EBX = Delta X + 1
			inc	eax                     ; EAX = Delta Y + 1

;---------------------- Compute Adresse
			mov	edi, ecx
			imul	edi, [ScanLine]
			add	edi, edx

			mov	[SaveEDX], edx
			mov	ebp, eax
			mov	esi, ecx

			mov	eax, edi
			mov	cl, [BankShift]
			shr	eax, cl
			SwitchBank			; Change Bank

			mov	ecx, esi
			mov	eax, ebp
			mov	edx, [SaveEDX]

			mov	esi, TabOffLine[ecx*4]
			add	esi, edx
			add	esi, Log

			and	edi, [BankSize]		; bank size
			mov	ebp, edi		; BP For Bank
			or	edi, 0A0000h		; EDI = Phys

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Log
			mov	[DeltaLog], edx

			mov	edx, [ScanLine]
			sub	edx, ebx		; DX Delta Phys
;----------------------
Again2:			add	ebp, ebx
			test	ebp, [BankOver]
			jnz	NewBankNear0p2
			mov	ecx, ebx

			SuperMovsD			; Macro

			add	esi, [DeltaLog]
			add	edi, edx
			add	ebp, edx
			test	ebp, [BankOver]
			jnz	NewBankNear1p2
			dec	eax
			jne	Again2
;----------------------
			ret

;-----------------------------------------------------------------------------

NewBankNear0p2:		and	ebp, [BankSize]

			mov	ecx, ebx
			sub	ecx, ebp
			mov	[SaveEBX], ebx		; Sauve EBX
			mov	ebx, ecx

			SuperMovsD			; Macro

			ChangeVideoBank			: Macro

			mov	edi, 0A0000h		; Begin Phys
			mov	ecx, ebp
			mov	ebx, ebp

			SuperMovsD			; Macro

			mov	ebx, [SaveEBX] 		; Restore EBX

			add	esi, [DeltaLog]
			add	edi, edx
			add	ebp, edx
			test	ebp, [BankOver]
			jnz	NewBankNear1p2
			dec	eax
			jne	Again2
			ret
;----------------------
NewBankNear1p2:		ChangeVideoBank			; Macro
			sub	edi, [BankSize]		; Stay in Phys
			and	ebp, [BankSize]
			dec	edi
			dec	eax
			jne	Again2

			ret

CopyBlockPhys		endp

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;			Flip()
;
;	Attention: de Log (m‚moire) vers Phys (vid‚o ram) … modifier
;
Flip			proc

			cmp	[NonStdVESA], 0
			jne	flip2

			push	esi
			push	edi

			mov	esi, Log

			mov	eax, 0
			SwitchBank

			mov	edi, Phys
			mov	ecx, 04000h
			rep	movsd

			mov	eax, 1
			SwitchBankDirect

			mov	edi, Phys
			mov	ecx, 04000h
			rep	movsd

			mov	eax, 2
			SwitchBankDirect

			mov	edi, Phys
			mov	ecx, 04000h
			rep	movsd

			mov	eax, 3
			SwitchBankDirect

			mov	edi, Phys
			mov	ecx, 04000h
			rep	movsd

			mov	eax, 4
			SwitchBankNoTest

			mov	edi, Phys
			mov	ecx, 02C00h
			rep	movsd

			pop	edi
			pop	esi

			ret

flip2:			push	479
			push	639
			push	0
			push	0
			call	CopyBlockPhys
			add	esp, 16

			ret

Flip			endp

;----------------------------------------------------------------------------
comment @
{'		EN COMMENTAIRE

SuperIncrust		MACRO
			local	Again, OkCopy, EndLine, UntilEnd

			push	ebx
			xor	eax, eax	; Je cherche != 0
Again:
			xchg	esi, edi
			rep	scasb		; Arret quand != 0
			jne	OkCopy		; Ok Data Rencontr‚e(s)
;-----------------------
			add	esi, ebx
			xchg	esi, edi
			jmp	EndLine		; c'est fini!
;-----------------------
OkCopy: 		dec	edi		; EDI un peu trop loin
			inc	ecx		; ECX trop Petit

			sub	ebx, ecx	; EBX = Bytes Parcourus
			add	esi, ebx	; Maj ESI

			mov	ebx, ecx	; EBX = Bytes Restant
			repne	scasb		; Combien != 0 ?
			jne	UntilEnd        ; Tous jusqu'a fin (Quick!

			inc	ecx		; Maj	ECX
			dec	edi		; Maj 	EDI
			sub	ebx, ecx	; EBX = Nb Same Bytes
			sub	edi, ebx	; Recule EDI
			xchg	esi, edi	; Log <=> Phys

			xchg	ecx, ebx
			rep	movsb		; movsD Plus Tard!

			mov	ecx, ebx
			jmp	Again
UntilEnd:
			sub	edi, ebx
			xchg	esi, edi
			mov	ecx, ebx
			rep	movsb
EndLine:
			pop	ebx

			endm
;----------------------------------------------------------------------------

;			Log -> Phys
;			CopyBlockIncrust( x0, y0, x1, y1 )
;
CopyBlockIncrust	proc	uses esi edi ebx ebp,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD

			mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1
;---------------------- Compute Delta X & Y
			sub	ebx, edx		; EBX Delta X
			sub	eax, ecx		; EAX Delta Y
			inc	ebx 			; EBX = Delta X + 1
			inc	eax                     ; EAX = Delta Y + 1
			mov	word ptr [CptLine], ax
;---------------------- Compute Adresse
			mov	esi, TabOffLine[ecx*4]
			add	esi, edx

			mov	edi, esi

			mov	eax, edi
			shr	eax, 16
			SwitchBank			; Change Bank 64Kb

			add	esi, Log

			xor	ebp, ebp
			and	edi, 0FFFFh		; 64Kb bank
			or	edi, 0A0000h		; EDI = Phys
			mov	bp, di			; BP For Bank

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen
;----------------------
Again:			add	bp, bx
			jc	NewBankNear0i
			mov	ecx, ebx

			SuperIncrust

			add	esi, edx
			add	edi, edx
			add	bp, dx
			jc	NewBankNear1i
			dec	word ptr [CptLine]
			jne	short Again
;----------------------
			ret
;----------------------
NewBankNear0i:		mov	ecx, ebx
			sub	ecx, ebp
			push	ebx			; Sauve EBX
			mov	ebx, ecx

			SuperIncrust 			; Macro

			ChangeVideoBank			: Macro

			mov	edi, 0A0000h		; Begin Phys
			mov	ecx, ebp
			mov	ebx, ecx

			SuperIncrust

			pop	ebx     		; Restore EBX

			add	esi, edx
			add	edi, edx
			add	bp, dx
			jc	NewBankNear1i
			dec	word ptr [CptLine]
			jne	Again
			ret
;----------------------
NewBankNear1i:		ChangeVideoBank			; Macro
			sub	edi, 010000h		; Stay in Phys
			dec	word ptr [CptLine]
			jne	Again
			ret
CopyBlockIncrust	endp
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
FlipComp		proc	uses esi edi ebx ebp,\
			log0:DWORD, log1:DWORD

			mov	esi, log0		; ESI = log0
			mov	edi, log1		; EDI = log1
			call	Vsync
;-------------------------------------------------------
Bank0:			mov	eax, 0			; Bank 0
			SwitchBank			; De l'ecran Phys
			mov	eax, esi		; EAX = log0
			sub	eax, 0A0000h		; EAX = log0-Phys
			mov	ecx, 04000h		; 64Kb
Cont0:
			rep	cmpsd			; Cherche Difference
			jne	Diff0			; Il y a Difference
;----------------------
Bank1:			mov	eax, 1			; Bank 1
			SwitchBank			; De l'ecran Phys
			mov	eax, esi		; Maj offset Phys
			sub	eax, 0A0000h		; Car Nouvelle Bank
			mov	ecx, 04000h		; 64Kb
Cont1:
			rep	cmpsd			; Cherche Difference
			jne	Diff1			; Il y a Difference
;----------------------
Bank2:			mov	eax, 2			; Bank 1
			SwitchBank			; De l'ecran Phys
			mov	eax, esi		; Maj offset Phys
			sub	eax, 0A0000h		; Car Nouvelle Bank
			mov	ecx, 04000h		; 64Kb
Cont2:
			rep	cmpsd			; Cherche Difference
			jne	Diff2			; Il y a Difference
;----------------------
Bank3:			mov	eax, 3			; Bank 1
			SwitchBank			; De l'ecran
			mov	eax, esi		; Maj offset Phys
			sub	eax, 0A0000h		; Car Nouvelle Bank
			mov	ecx, 04000h		; 64Kb
Cont3:
			rep	cmpsd			; Cherche Difference
			jne	Diff3			; Il y a Difference
;----------------------
Bank4:			mov	eax, 4			; Bank 1
			SwitchBank			; De l'ecran
			mov	eax, esi		; Maj offset Phys
			sub	eax, 0A0000h		; Car Nouvelle Bank
			mov	ecx, 02C00h		; 64Kb
Cont4:
			rep	cmpsd			; Cherche Difference
			jne	Diff4			; Il y a Difference
;----------------------
			ret
;-------------------------------------------------------
Diff0:			sub	esi, 4			; Recule ESI
			sub 	edi, 4			; Recule EDI
			inc	ecx			; One More
			mov	edx, esi		; Sauve ESI
			mov	ebx, ecx		; Sauve ECX

			repne	cmpsd			; Combien ?
			sub	edi, 4			; Recule EDI
			mov	esi, edx		; Restore ESI
			mov	edx, edi		; Sauve EDI
			inc	ecx			; One More
			sub	ebx, ecx		; Calcul Nb Diffs
			xchg	ecx, ebx		; Sauve ECX
			mov	edi, esi		; Calcul
			sub	edi, eax		; Adresse Phys
			rep	movsd			; Ecriture Diffs
			mov	edi, edx		; Restore EDI
			add	ecx, ebx		; Restore ECX
			je	Bank1			; Fin De cette Bank
			jmp	Cont0			; Again and Again...
;----------------------
Diff1:			sub	esi, 4			; Recule ESI
			sub 	edi, 4			; Recule EDI
			inc	ecx			; One More
			mov	edx, esi		; Sauve ESI
			mov	ebx, ecx		; Sauve ECX

			repne	cmpsd			; Combien ?
			sub	edi, 4			; Recule EDI
			mov	esi, edx		; Restore ESI
			mov	edx, edi		; Sauve EDI
			inc	ecx			; One More
			sub	ebx, ecx		; Calcul Nb Diffs
			xchg	ecx, ebx		; Sauve ECX
			mov	edi, esi		; Calcul
			sub	edi, eax		; Adresse Phys
			rep	movsd			; Ecriture Diffs
			mov	edi, edx		; Restore EDI
			add	ecx, ebx		; Restore ECX
			je	Bank2			; Fin De cette Bank
			jmp	Cont1			; Again and Again...
;----------------------
Diff2:			sub	esi, 4			; Recule ESI
			sub 	edi, 4			; Recule EDI
			inc	ecx			; One More
			mov	edx, esi		; Sauve ESI
			mov	ebx, ecx		; Sauve ECX

			repne	cmpsd			; Combien ?
			sub	edi, 4			; Recule EDI
			mov	esi, edx		; Restore ESI
			mov	edx, edi		; Sauve EDI
			inc	ecx			; One More
			sub	ebx, ecx		; Calcul Nb Diffs
			xchg	ecx, ebx		; Sauve ECX
			mov	edi, esi		; Calcul
			sub	edi, eax		; Adresse Phys
			rep	movsd			; Ecriture Diffs
			mov	edi, edx		; Restore EDI
			add	ecx, ebx		; Restore ECX
			je	Bank3			; Fin De cette Bank
			jmp	Cont2			; Again and Again...
;----------------------
Diff3:			sub	esi, 4			; Recule ESI
			sub 	edi, 4			; Recule EDI
			inc	ecx			; One More
			mov	edx, esi		; Sauve ESI
			mov	ebx, ecx		; Sauve ECX

			repne	cmpsd			; Combien ?
			sub	edi, 4			; Recule EDI
			mov	esi, edx		; Restore ESI
			mov	edx, edi		; Sauve EDI
			inc	ecx			; One More
			sub	ebx, ecx		; Calcul Nb Diffs
			xchg	ecx, ebx		; Sauve ECX
			mov	edi, esi		; Calcul
			sub	edi, eax		; Adresse Phys
			rep	movsd			; Ecriture Diffs
			mov	edi, edx		; Restore EDI
			add	ecx, ebx		; Restore ECX
			je	Bank4			; Fin De cette Bank
			jmp	Cont3			; Again and Again...
;----------------------
Diff4:			sub	esi, 4			; Recule ESI
			sub 	edi, 4			; Recule EDI
			inc	ecx			; One More
			mov	edx, esi		; Sauve ESI
			mov	ebx, ecx		; Sauve ECX

			repne	cmpsd			; Combien ?
			sub	edi, 4			; Recule EDI
			mov	esi, edx		; Restore ESI
			mov	edx, edi		; Sauve EDI
			inc	ecx			; One More
			sub	ebx, ecx		; Calcul Nb Diffs
			xchg	ecx, ebx		; Sauve ECX
			mov	edi, esi		; Calcul
			sub	edi, eax		; Adresse Phys
			rep	movsd			; Ecriture Diffs
			mov	edi, edx		; Restore EDI
			add	ecx, ebx		; Restore ECX
			jne	Cont4			; Again and Again
			ret
FlipComp		endp
;----------------------------------------------------------------------------

				.386
FlipComp386	proc	uses ds es si di, \
		scr0:DWORD, scr1:DWORD

		mov	dx, word ptr [scr0+2]	; dx = Log0
		mov	ds, word ptr [scr1+2]	; ds = Log1
		mov	ax, 0A000h		; Phys
		xor	si, si			; Maj Offset
		mov	di, si			; a zero
		mov	cx, 07D00h/2		; 100 Lines * 4 (LONG)
Aqui:
		mov	es, dx			; es = Log0
		rep	cmpsd			; Comp Log0 & Log1
		je	TheEnd			; No difference, End

		mov	bp, si			; Save offset in BP
		mov	bx, cx			; Save Nb Left
		repne	cmpsd			; How many difference(s) ?
		jne	UntilEnd		; All left are different...

		mov	es, ax
		sub	bp, 4			; Maj BP
		mov	si, bp			; Maj Offset SI
		mov 	di, bp			; & DI
		sub	bx, cx			; How much in BX
		xchg	cx, bx			; Now in CX
		rep	movsd			; Copy CX Bytes Log1->Phys

		mov	cx, bx			; Restore Bytes Left
		inc	cx
		inc	cx
		jmp	short Aqui		; Twist again
UntilEnd:
		mov	es, ax			; ES = Phys
		sub	bp, 4   		; Maj BP
		mov	si, bp			; Maj offset SI
		mov	di, bp			; & DI
		sub	bx, cx			; How much left in BX
		mov	cx, bx			; Now in CX ( ex: XCHG CX,BX )
		inc	cx
		inc	cx
		rep	movsd
TheEnd:         		.8086
		ret
FlipComp386	endp



;----------------------------------------------------------------------------
opt_stos_line	macro

			mov	eax, ecx
			shr	ecx, 2
			rep	movsd
			and	eax, 3
			mov	ecx, eax
			rep	movsb
		endm
;----------------------------------------------------------------------------
; Flip de Log vers Phys


BlockLogPhys		proc uses es ds si di,\
			sox:DWORD,soy:DWORD,sol:DWORD,soh:DWORD,\
			dex:DWORD,dey:DWORD


			mov	esi, soy
			mov	esi, TabOffLine[esi*4]
			add	esi, soy
			mov	edi, esi

			mov	eax, edi
			shr	eax, 16
			SwitchBank

			add	esi, Log
			add	edi, Phys



	mov	di,bx
	xor	ebx,ebx

	mov	ax,0A000h
	mov	es,ax

	mov	ax, sol
	mov	cs:word ptr[patch_larg1],ax
	mov	cs:word ptr[patch_larg2],ax
	mov	cs:word ptr[patch_larg3],ax
	neg	ax
	add	ax,640
	mov	cs:word ptr[patch_deoff],ax

	mov	ax,320
	sub	ax, sol
	mov	cs:word ptr[patch_sooff],ax

	jmp	$+2

	mov	dx, soh

Aqui:			mov	ebx, edi
			add	ebx, sol
			jnc	ok1

			mov	ecx, sol
			sub	ecx, ebx
			opt_stos_line

			mov	eax, BankCurrent
			inc	eax
			SwitchBank

			mov	ecx, ebx
			opt_stos_line

			jmp	Next

ok1:    		xor	ecx, ecx
			mov	ecx, sol
			opt_stos_line

Next:			mov	edi, ebx
patch_deoff	equ	$+2
			add	edi, 1234h
			jnc	ok2

			mov	eax, BankCurrent
			inc	eax
			push	eax
			SwitchBank

ok2:
patch_sooff	equ	$+2
			add	esi,1234h

			dec	edx
			jnz	Aqui

			ret

BlockLogPhys		endp

}
@
;----------------------------------------------------------------------------
;				The
				End






