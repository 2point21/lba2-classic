;/*══════════════════════════════════════════════════════════════════════════*
;                █  ▄▀ █▀▀▀▀ █  ▄▀ █▀▀█  █▀▀▀█ █▀▀▀█ █▀▀▀█ █▀▀▀▄
;                ██▀▄  ██▀▀  ██▀   ██▀▀█ ██  █ ██▀▀█ ██▀█▀ ██  █
;                ▀▀  ▀ ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀
; *══════════════════════════════════════════════════════════════════════════*/
;
;/*──────────────────────────────────────────────────────────────────────────*/
				.386p

				.model  SMALL, SYSCALL

KB_DATA		equ     60H

;int15		equ	1

;/*───────────────────────────────────────────────────────────────────────*/
				.DATA

		public	NoLanguage	Key
		public	NoLanguage	_Key
		public	NoLanguage	Joy
		public	NoLanguage	Fire
		public	NoLanguage	FuncKey
		public	NoLanguage	AsciiMode
		public	NoLanguage	Old_PM09_Off
		public	NoLanguage	Old_PM09_Sel


NoAscii		db	0

Key		dw	00
_Key		dw	00
FuncKey		dw	00
Joy		dw	00
Fire		dw	00
AsciiMode	dw	00


NB_SPECIAL_KEY	equ	29

TabSpecialKey	db	72	; UP
		db	80	; DOWN
		db	75	; LEFT
		db	77	; RIGHT		Joy
		db	71	; UP LEFT
		db	73	; UP RIGHT
		db	81	; DOWN RIGHT
		db	79	; DOWN LEFT

		db	57	; space
		db	28	; return
		db	29	; CTRL
		db	56	; ALT
		db	83	; SUPPR
		db	42	; SHIFT_LEFT
		db	54	; SHIFT_RIGHT

		db	59	; F1
		db	60	; F2
		db	61	; F3
		db	62	; F4
		db	63	; F5
		db	64	; F6
		db	65	; F7
		db	66	; F8
		db	67	; F9
		db	68	; F10
		db	87	; F11
		db	88	; F12

		db	42
		db	0	; ??? code cursor

comment	@

TabSpecialCount	db	00	; UP
		db	00	; DOWN
		db	00 	; LEFT
		db	00	; RIGHT		Joy
		db	00	; UP LEFT
		db	00	; UP RIGHT
		db	00	; DOWN RIGHT
		db	00	; DOWN LEFT

		db	00	; space
		db	00	; return
		db	00	; CTRL
		db	00	; ALT
		db	00	; SUPPR
		db	00	; SHIFT_LEFT
		db	00	; SHIFT_RIGHT

		db	00	; F1
		db	00	; F2
		db	00	; F3
		db	00	; F4
		db	00	; F5
		db	00	; F6
		db	00	; F7
		db	00	; F8
		db	00	; F9
		db	00	; F10
		db	00	; F11
		db	00	; F12

		db	00
		db	00	; ??? code cursor

	@

TabSpecialFunc	db	0,1	; J_UP
		db	0,2	; J_DOWN
		db	0,4	; J_LEFT
		db	0,8	; J_RIGHT	Joy
		db	0,1+4	; UP LEFT
		db	0,1+8	; UP RIGHT
		db	0,2+8	; DOWN RIGHT
		db	0,2+4	; DOWN LEFT

		db	1,1	; Fire
		db	1,2	; return
		db	1,4	; ctrl
		db	1,8	; alt
		db	1,16	; suppr		; attention cas 224
		db	1,32	; shift left
		db	1,32	; shift right

		db	2,1	; F1
		db	2,2	; F2
		db	2,4	; F3
		db	2,8	; F4
		db	2,16	; F5
		db	2,32	; F6
		db	2,64	; F7
		db	2,128	; F8
		db	3,1	; F9
		db	3,2	; F10
		db	3,4	; F11
		db	3,8	; F12

		db	255,0	; oublie 42
		db	255,0	; oublie 0

Old_Int_09	dd	0000

Old_PM09_Off	dd	0000
Old_PM09_Sel    dw	00


FlagExtendScan	db	0

;/*══════════════════════════════════════════════════════════════════════════*/

		.CODE

		public	NoLanguage	NewIntPM09
		public	NoLanguage	GetAscii
		public	NoLanguage	ClearAsciiBuffer

;/*══════════════════════════════════════════════════════════════════════════*/

NewIntPM09	PROC    FAR

		push	eax
		push	ecx
		push	ebx
		push	edi
		push	ds
		push	es

ifdef	int15
		jnc	back15

		cmp	ah, 4Fh
		jne     back15
endif

		cld

		mov	cx, DGROUP
		mov     ds, cx
		mov	es, cx

comment	@
; peut etre une bonne idee mais plante le clavier sur plein de pc
		mov	al, 0ADh
		out	64h, al			; disable keyboard
	@

ifdef	int15

		mov	ah, al

else

once_again:
		in      al, KB_DATA
		mov     ah, al

endif

		mov	byte ptr[_Key], ah

		cmp	al, 224			; code etendu
		jz      doextend

		and	al, 127			; al scancode

		cmp	al, 2Ah			; shift ?
		jnz	noshift

		cmp	[FlagExtendScan], 1	; extended shift say goodbye
		jz	exit

noshift:	mov	ecx, NB_SPECIAL_KEY
		mov	edi, offset TabSpecialKey
		repne	scasb
		jnz	nospecial

		mov	ebx, NB_SPECIAL_KEY-1
		sub	ebx, ecx

comment	@
		lea	ecx, [TabSpecialCount + ebx]
		mov	al, [ecx]

		test	ah, 128
		jnz	decr

		inc	al
		jmp	ok1

decr:		or	al, al
		jz	keyup
		dec	al
		jz	keyup

keydown:	and	ah, 127
		jmp	ok1
keyup:		or	ah, 128
ok1:		mov	[ecx], al
		mov	al, ah
	@

		mov	bx, word ptr[ TabSpecialFunc + ebx*2 ]

ni0:		or	bl, bl
		jnz	ni1

	; code touche pour Joy			CODE BL 0 : Joy

		test	ah, 128
		jnz	relache0

		or	byte ptr[ Joy ], bh
		jmp	exit

relache0:	not	bh
		and	byte ptr[ Joy ], bh
		jmp	exit

doextend:	mov     [FlagExtendScan], 1
		jmp	exit2

	; code touche pour Fire			CODE BL 1 : Fire

ni1:		dec	bl
		jnz	ni2

		test	ah, 128
		jnz	relache1

		or	byte ptr[ Fire ], bh

		cmp	byte ptr[ Fire ], 4 + 8 + 16	; ctrl alt suppr
		jnz	exit

reset:		mov	eax, 472h
		mov	word ptr[eax], 1234h		; don'check mem but reset it

		push	0FFFFh
		push	00000h
		retf

relache1:	not	bh
		and	byte ptr[ Fire ], bh
		jmp	short exit

ni2:		dec	bl
		jnz	ni3

	; code touche pour FuncKey		CODE BL 2 : FuncKey 1-8

		test	ah, 128
		jnz	relache2

		or	byte ptr[ FuncKey ], bh
		jmp	short nospecial

relache2:	not	bh
		and	byte ptr[ FuncKey ], bh
		jmp	short nospecial

ni3:		dec	bl
		jnz	ni4

	; code touche pour FuncKey		CODE BL 3 : FuncKey 9-12

		test	ah, 128
		jnz	relache3

		or	byte ptr[ FuncKey+1 ], bh
		jmp	short nospecial

relache3:	not	bh
		and	byte ptr[ FuncKey+1 ],bh
		jmp	short nospecial

ni4:		jmp	short exit		; CODE BL 255 : oublie token

nospecial: 	test	ah, 128
		jnz	relache5

		mov	byte ptr[ Key ], al
		jmp	short exit

relache5:	mov     byte ptr[ Key ], 0

exit:		mov	[FlagExtendScan], 0

exit2:

ifdef	int15

		cmp	byte ptr[ AsciiMode ], 0
		jne	back15

		and	dword ptr[esp+8*4], 0FFFFFFFEh
back15:
		pop	es
		pop	ds
		pop	edi
		pop	ebx
		pop	ecx
		pop	eax
		iret

else

		in	al, 64h			
		test	al, 1
		jnz	once_again

		cmp	byte ptr[ AsciiMode ], 0
		jnz	ascii

;--------------
		mov     al, 20H
		out     20H, al

comment	@
; peut etre une bonne idee mais plante le clavier sur plein de pc
		mov	al, 0AEh
		out	64h, al			; enable keyboard
	@
		pop	es
		pop	ds
		pop	edi
		pop	ebx
		pop	ecx
		pop	eax
		iretd

ascii:
comment	@
; peut etre une bonne idee mais plante le clavier sur plein de pc
		mov	al, 0AEh
		out	64h, al			; enable keyboard
	@
		xor	eax, eax
		mov	ecx, [Old_PM09_Off]
		mov	ax,  [Old_PM09_Sel]

		xchg	ecx, [esp+4*4]
		xchg	eax, [esp+5*4]

		pop	es
		pop	ds
		pop	edi
		pop	ebx
		retf

endif

NewIntPM09 	endp

;/*══════════════════════════════════════════════════════════════════════════*/

GetAscii	proc

		mov	ah, 1
		int	16h
		jz	nokey
		mov	ah, 0
		int	16h
		ret

nokey:		xor	ax, ax
		ret

GetAscii	endp

;/*───────────────────────────────────────────────────────────────────────*/

ClearAsciiBuffer	proc

cab0:		mov	ah, 1
		int	16h
		jz	nokey1
		mov	ah, 0
		int	16h
		jmp	cab0
nokey1:		ret

ClearAsciiBuffer	endp

;/*───────────────────────────────────────────────────────────────────────*/
;				The
				End







