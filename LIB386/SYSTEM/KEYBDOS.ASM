;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		.386p
		.model  SMALL, SYSCALL

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		.DATA

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		public	SYSCALL		KeybDataStart	; for VMM_lock
		public	SYSCALL		KeybDataEnd

		public	SYSCALL		TabKeys

		public	SYSCALL		Stack

		public	SYSCALL		Key

		public	SYSCALL		KeyOff

		public	SYSCALL		AsciiMode

		public	SYSCALL		Old_PM09_Off
		public	SYSCALL		Old_PM09_Sel

		public	SYSCALL		FlagExtendScan


;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
KeybDataStart	label	dword

TabKeys		dd	8 DUP (0)
TabOthers	dd	12 DUP (0)

Stack		dd	00

Key		dd	00

KeyOff		dd	00

AsciiMode	dd	00

Old_PM09_Off	dd	00
Old_PM09_Sel    dd	00

FlagExtendScan	db	00

KeybDataEnd	label	dword

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
KB_DATA		equ     60H

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		.CODE

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		public	SYSCALL		KeybHandler
		public	SYSCALL		GetAscii
		public	SYSCALL		ClearAsciiBuffer

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		public	SYSCALL		KeybCodeStart	; for VMM_lock
		public	SYSCALL		KeybCodeEnd

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		public	SYSCALL		PatchExtended

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ifdef	REG_CALL
		extrn	SYSCALL		exit_:PROC
else ;	REG_CALL
		extrn	SYSCALL		exit:PROC
endif;	REG_CALL

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
KeybCodeStart	label	dword

KeybHandler	PROC    FAR

		push	eax			; save used registers
		push	ecx
		push	ebx
		push	edi
		push	ds

		cld				; reset direction

		mov	ecx, _DATA		; reset data segment
		xor	eax, eax
		mov     ds, ecx

		xor	edi, edi

once_again:
		in      al, KB_DATA		; get byte from keyboard

		mov	ebx, [KeyOff]
		mov     ch, al

		test	ebx, ebx		; check if keyboard enabled
		jnz	lexit2

		cmp	eax, 0E0h		; extended code
		je      doextend

		and	eax, 07Fh		; al scancode
		mov	cl, [FlagExtendScan]

		cmp	eax, 2Ah		; shift ?
		jnz	noshift

		test	cl, cl			; extended shift, bye
		jnz	lexit3
noshift:
PatchExtended::	and	cl, 0			; 0 if 128 scancodes map, 1 of 256 scancodes map
		or	al, cl
		mov	ebx, 1
		mov	cl, al
		mov	edi, eax
		shr	eax, 5-2
		and	cl, 31
		shl	ebx, cl
		and	eax, 111b SHL 2

		test	ch, 80h			; check for up or down
		jnz	up

		or	ebx, TabKeys[eax]	; update keyboard map (set a 1 at the right place)
		mov	[Key], edi		; last key pressed
lexit:
		mov	TabKeys[eax], ebx
lexit3:
		mov	[FlagExtendScan], 0	; clear extended flag

		test	TabKeys[8], 80000h	; SUPPR pressed ?
		jnz	reset

;		cmp	eax, 128/8		; check if key is extented
;		jb	lexit2			; if not then not Impr Ecran

;		and	ch, 7Fh

;		cmp	ch, 037h
;		je	ascii2
lexit2:
		in	al, 64h			; another byte
		test	al, 1
		jnz	once_again

		cmp	byte ptr[AsciiMode], 0	; bios enabled ?
		jnz	ascii

		mov     al, 20H			; enable interrupts
		out     20H, al

		pop	ds			; pop and return
		pop	edi
		pop	ebx
		pop	ecx
		pop	eax
		iretd
up:
		not	ebx
		mov     [Key], 0		; last key is up
		and	ebx, TabKeys[eax]	; update keyboard map (set a 1 at the right place)
		jmp	lexit

doextend:	mov     [FlagExtendScan], 080h	; next code is extended
		jmp	lexit2

reset:
		test	TabKeys[0], 20000000h	; CTRL ?
		jz	lexit2
		test	TabKeys[4], 01000000h	; ALT ?
		jz	lexit2

		mov	eax, _DATA		; ok try to call exit
		mov	ss, eax
		mov	esp, Stack		; reset the stack to a "safe" spot
		sti				; enable interrupts
		mov     al, 20H
		out     20H, al
ifdef	REG_CALL
		jmp	exit_			; jump to "C" exit function
else ;	REG_CALL
		jmp	exit
endif;	REG_CALL

ascii2:
		mov	[AsciiMode], 1
ascii:
		mov	ecx, [Old_PM09_Off]	; mess with the stack
		mov	eax, [Old_PM09_Sel]	; to reset all the register
				      		; and allow to do a retf
		xchg	ecx, [esp+3*4]		; to chain with the bios
		xchg	eax, [esp+4*4]		; keyboard handler

		pop	ds
		pop	edi
		pop	ebx
		retf				; jmp to bios keyboard handler

KeybHandler 	endp

KeybCodeEnd	label	dword

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
GetAscii	proc

		mov	ah, 1
		int	16h
		mov	eax, 0
		jz	nokey
		int	16h
		and	eax, 0FFFFh
nokey:
		ret

GetAscii	endp

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ClearAsciiBuffer proc

cab0:		mov	ah, 1
		int	16h
		mov	ah, 0
		jz	nokey1
		int	16h
		jmp	cab0
nokey1:
		ret

ClearAsciiBuffer endp

;//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;			The
       			End
