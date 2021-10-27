

		.386p

		.model SMALL, SYSCALL

		.data

		extrn   SYSCALL 	Log		:DWORD
		extrn	SYSCALL 	TabOffLine	:DWORD

FireSeed	dd	1876351

		.code

		public	SYSCALL	Do_Fire

;ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
;                           Û฿฿฿฿  Û    Û฿฿฿Û Û฿฿฿฿
;                           ÛÛ฿฿   ÛÛ   ÛÛ฿Û฿ ÛÛ฿฿
;                           ฿฿     ฿฿   ฿฿  ฿ ฿฿฿฿฿
;ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

; grossse optimisations … faire ......

;ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

Do_Fire		proc uses esi edi ebx ebp ,\
		ptrwork1:dword, ptrwork2:dword, colortable:DWORD, ptrtex:dword

		; genere 2 lignes aleatoires

		mov	esi, ptrwork1
		add	esi, 32*34
		mov	edx, 32*2

loopg:		mov	eax, [FireSeed]
		mov	cl, ah
		rol	eax, cl
		imul	ax, dx
		add	[FireSeed], 5137827
		and	eax, 128
		mov	[esi], al
gena:
		inc	esi
		dec	edx
		jnz	loopg

		; map smooth avec bouclage

		mov	esi, ptrwork1
		mov	edi, ptrwork2

		add	esi, 32
		add	edi, 32

		xor	edx, edx		; line cmpt

		mov	ecx, 32
;		sub	ecx, 2			; pas en haut, pas en bas
		add	ecx, 2
;		dec	ecx

loopy:		push	ecx

		mov	ecx, 32
		xor	ebx, ebx

loopx:		xor	eax, eax

;		push	edx

;		xor	edx, edx		; mov dl, ...

		sub	ebx, 32
		mov	dl, [esi+ebx]
		add	eax, edx

		add	ebx, 32
		add	ebx, 32
		mov	dl, [esi+ebx]
		add	eax, edx

		dec	ebx
		and	ebx, 31
		mov	dl, [esi+ebx]
		add	eax, edx

		sub	ebx, 32
		mov	dl, [esi+ebx]
		add	eax, edx

		add	ebx, 32
		add	ebx, 32
		mov	dl, [esi+ebx]
		add	eax, edx

		add	ebx, 2
		and	ebx, 31
		mov	dl, [esi+ebx]
		add	eax, edx

		sub	ebx, 32
		mov	dl, [esi+ebx]
		add	eax, edx

		add	ebx, 32
		add	ebx, 32
		mov	dl, [esi+ebx]
		add	eax, edx

;		pop	edx

		ror	ax, 3			; moyenne /8

		test	ah, 60h			; user param ?
		jnz	nodec			; masturbe le feu

dodec:		or	al, al
		jz	nodec

		dec	eax
nodec:
		mov	[edi], al
		inc	edi

		and	ebx, 31			; col 0-31

		dec	ecx
		jnz	loopx

		add	esi, 32

;		inc	edx			; lig

		pop	ecx
		dec	ecx
		jnz	loopy

		; copy scroll results

		mov	esi, ptrwork2
		add	esi, 32
		mov	edi, ptrwork1
		mov	ecx, 32 * 34 / 4
		rep	movsd

		; genere texture

		mov	esi, ptrwork1
		mov	edi, ptrtex
		mov	ebx, colortable
		mov	ecx, 32			; Y
		xor	eax, eax
loopyb:
		push	ecx
		mov	ecx, 32			; X

loopxb:
		mov	al, [esi]

;		shr	al, 2
;		cmp	al, 15
;		jbe	coulok
;		mov	al, 15

		shr	al, 1
		cmp	al, 31
		jbe	coulok
		mov	al, 31
coulok:
		mov	al, [ebx+eax]
		mov	[edi], al

		inc	esi
		inc	edi

		dec	ecx
		jnz	loopxb

		add	edi, 256 - 32

		pop	ecx
		dec	ecx
		jnz	loopyb

		ret


Do_Fire		endp









comment #/*
; ------------------------------ FIRE.ASM ------------------------------

		.386p

		.model SMALL, SYSCALL

		.data

		public	SYSCALL      Imagen

		extrn	SYSCALL	BufSpeak:DWORD

;Imagen  DB 320*50 DUP (0)
;Imagen2 DB 320*50 DUP (0)

Imagen		dd	0000
Imagen2		dd	0000

loopy	dw	00

		.code

		public	SYSCALL	DoFire
		public	SYSCALL	ComputeFire

ComputeFire	proc	uses esi edi ebx edx

		mov	eax, [BufSpeak]
		mov	[Imagen], eax
		add	eax, 320*50
		mov	[Imagen2], eax

		mov	esi, [Imagen]
		add	esi, 321
		mov	edi, [Imagen2]
		add	edi, 321

;		mov	esi, 321 + offset Imagen
;		mov	edi, 321 + offset Imagen2

		MOV     CX,48*320-2

		XOR     BH,BH
@@lp:
		XOR     AX,AX
		ADD     AL,-1[eSI]
		ADC     AH,BH
		ADD     AL,-320[eSI]
		ADC     AH,BH
		ADD     AL,-319[eSI]
		ADC     AH,BH
		ADD     AL,-321[eSI]
		ADC     AH,BH
		ADD     AL,1[eSI]
		ADC     AH,BH
		ADD     AL,320[eSI]
		ADC     AH,BH
		ADD     AL,319[eSI]
		ADC     AH,BH
		ADD     AL,321[eSI]
		ADC     AH,BH
		ROR     AX,3

		TEST    AH,65h                  ; Wanna know why 60h? Me too.
		JNZ     @@nx                    ; This is pure experience.
						; ok but it's better with 65

		mov	edx, [Imagen2]
		add	edx, 46*320

		CMP    edi, edx

;		cmp edi, 46*320 + offset Imagen2
		JNC    @@dec

		OR    AL,AL                   ; My dog coded here too.
		JZ    @@nx                    ; I helped my sister with this one.
@@dec:
		DEC  AL                      ; Yeah! Cool a bit, please.
@@nx:
		inc     esi
		mov	byte ptr[edi], al

		inc	edi

		dec	cx
		jnz	@@lp                    ; New image stored in Imagen2.

		mov	esi, [Imagen2]
		add	esi, 320
		MOV     edi, [Imagen]

;		mov	esi, 320 + offset Imagen2
;		mov	edi, offset Imagen

		MOV     ecx, 160*48/2
		rep	movsd

		mov	esi, [Imagen2]
		add	esi, 320*39

;		mov	esi, offset Imagen2 + 320*39

		mov	ecx, 5*320
		mov	ah, 11
@@rcl:
		mov	al,[esi]
		cmp	al, 15
		jnc	@@rcn
		SUB   AL,AH
		NEG   AL
		MOV   [eSI],AL
@@rcn:
		INC    eSI
		dec	cx
		jnz	@@rcl

		ret

ComputeFire	endp


DoFire		proc uses esi edi ebx ebp ,\
		lig:dword, coul:dword

		call	ComputeFire

		mov	esi, [Imagen]
		add	esi, 5*320

;		mov	esi, offset Imagen + 5 * 320

		mov	edi, Log

		mov	eax, lig
		add	edi, [TabOffLine + eax*4]

		mov	ebx, Coul
		mov	bh, bl
		add	bh, 15

		mov	dx, 25

copy0:		mov	cx, 320

copy:		mov	al, [esi]
		shr	al, 1
		add	al, bl
		cmp	al, bh
		ja	adcoul
adcrt:		mov	ah, al
		mov	[edi], ax
		mov	[edi+640], ax
		inc	esi
		inc	edi
		inc	edi

		dec	cx
		jnz	copy

		add	edi, 640

		dec	dx
		jnz	copy0

		ret

adcoul:		mov	al, bh
		jmp	adcrt

DoFire		endp

*/#
	END
