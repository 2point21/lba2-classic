
	.model	SMALL, SYSCALL

	include	vesa.inc
;
;	VGAKIT Version 5.2b
;
;	Copyright 1988,89,90,91,92 John Bridges
;	Free for use in commercial, shareware or freeware applications
;
;	BANKS.ASM
;
	.data

	public	curbk

curbk	dw	?

	public	vgamem,bksize

vgamem	dw	?		;amount of memory on VGA card in K
bksize	dw	?		;size of memory banks in K

	public	cirrus,everex,acumos,paradise,tseng,trident,t8900
	public	ativga,aheada,aheadb,oaktech,video7
	public	chipstech,tseng4,genoa,ncr,compaq,vesa,dactype

cirrus	dw	?
everex	dw	?
acumos	dw	?
paradise dw	?
tseng	dw	?
trident	dw	?
t8900	dw	?
ativga	dw	?
aheada	dw	?
aheadb	dw	?
oaktech	dw	?		;Oak Technology Inc OTI-067 and OTI-077
video7	dw	?
chipstech dw	?
tseng4	dw	?
genoa	dw	?
ncr	dw	?
compaq	dw	?
vesa	dw	?

dactype	dw	?		; 0 = Normal DAC
				; 1 = Sierra SC11481, SC11486, SC11488
				; 2 = Sierra SC11485, SC11487, SC11489
				; 3 = AT&T ATT20C491
				; 4 = ACUMOS ADAC1

first	dw	?		;flag so whichvga() is only called once
retval	dw	?		;first return value from whichvga()

	.data

	.386
OSEG	equ	FS:		;segment override for variable access

	public	bankadr

bankadr	dw	offset _nobank
	dw	seg _nobank

	public	vesafunc

vesafunc dw	0
	dw	0

vesamodes label word
	dw	100h,     640, 400, 8
	dw	101h,     640, 480, 8
	dw	102h,     800, 600, 4
	dw	103h,     800, 600, 8
	dw	104h,    1024, 768, 4
	dw	105h,    1024, 768, 8
	dw	106h,    1280,1024, 4
	dw	107h,    1280,1024, 8
	dw	10Dh,     320, 200,16
	dw	10Eh,     320, 200,16
	dw	10Fh,     320, 200,24
	dw	110h,     640, 480,16
	dw	111h,     640, 480,16
	dw	112h,     640, 480,24
	dw	113h,     800, 600,16
	dw	114h,     800, 600,16
	dw	115h,     800, 600,24
	dw	116h,    1024, 768,16
	dw	117h,    1024, 768,16
	dw	118h,    1024, 768,24
	dw	119h,    1280,1024,16
	dw	11Ah,    1280,1024,16
	dw	11Bh,    1280,1024,24
	dw	0

	.code	LIB_SVGA

	public	newbank
	public	whichvga

	.386

newbank	proc			;bank number is in AX
	jmp	dword ptr OSEG[bankadr]

_tseng::			;Tseng 3000 series
	push	ax
	push	dx
	cli
	mov	OSEG[curbk],ax
	and	al,7
	mov	ah,al
	shl	al,1
	shl	al,1
	shl	al,1
	or	al,ah
	or	al,01000000b
	mov	dx,3cdh
	out	dx,al
	sti
	pop	dx
	pop	ax
	ret


_tseng4::			;Tseng 4000 series
	push	ax
	push	dx
	cli
	mov	OSEG[curbk],ax
	mov	ah,al
	mov	dx,3bfh		;Enable access to extended registers
	mov	al,3
	out	dx,al
	mov	dl,0d8h
	mov	al,0a0h
	out	dx,al
	and	ah,15
	mov	al,ah
	shl	al,1
	shl	al,1
	shl	al,1
	shl	al,1
	or	al,ah
	mov	dl,0cdh
	out	dx,al
	sti
	pop	dx
	pop	ax
	ret


_trident::			;Trident
	push	ax
	push	dx
	push	ax
	cli
	mov	OSEG[curbk],ax
	mov	dx,3ceh		;set pagesize to 64k
	mov	al,6
	out	dx,al
	inc	dl
	in	al,dx
	dec	dl
	or	al,4
	mov	ah,al
	mov	al,6
	out	dx,ax

	mov	dl,0c4h		;switch to BPS mode
	mov	al,0bh
	out	dx,al
	inc	dl
	in	al,dx
	dec	dl

	pop	ax
	mov	ah,al
	xor	ah,2
	mov	dx,3c4h
	mov	al,0eh
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret


_video7::			;Video 7
	push	ax
	push	dx
	push	cx
	cli
	mov	OSEG[curbk],ax
	and	ax,15
	mov	ch,al
	mov	dx,3c4h
	mov	ax,0ea06h
	out	dx,ax
	mov	ah,ch
	and	ah,1
	mov	al,0f9h
	out	dx,ax
	mov	al,ch
	and	al,1100b
	mov	ah,al
	shr	ah,1
	shr	ah,1
	or	ah,al
	mov	al,0f6h
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	and	al,not 1111b
	or	ah,al
	mov	al,0f6h
	out	dx,ax
	mov	ah,ch
	mov	cl,4
	shl	ah,cl
	and	ah,100000b
	mov	dl,0cch
	in	al,dx
	mov	dl,0c2h
	and	al,not 100000b
	or	al,ah
	out	dx,al
	sti
	pop	cx
	pop	dx
	pop	ax
	ret


_paradise::			;Paradise
	push	ax
	push	dx
	push	ax
	cli
	mov	OSEG[curbk],ax
	mov	dx,3ceh
	mov	ax,50fh		;turn off write protect on VGA registers
	out	dx,ax
	pop	ax
	mov	ah,al
	mov	al,9
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret

_acumos::			;AcuMos
	push	ax
	push	dx
	push	ax
	cli
	mov	OSEG[curbk],ax
	mov	dx,3c4h		;Sequencer index 6
	mov	ax,1206h	;turn off write protect on VGA registers
	out	dx,ax
	mov	dx,3ceh
	pop	ax
	mov	ah,al
	mov	al,9
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret


_chipstech::			;Chips & Tech
	push	ax
	push	dx
	push	ax
	cli
	mov	OSEG[curbk],ax
	mov	dx,46e8h	;place chip in setup mode
	mov	ax,1eh
	out	dx,ax
	mov	dx,103h		;enable extended registers
	mov	ax,0080h
	out	dx,ax
	mov	dx,46e8h	;bring chip out of setup mode
	mov	ax,0eh
	out	dx,ax
	pop	ax
	mov	ah,al
	mov	al,10h
	mov	dx,3d6h
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret


_ativga::			;ATI VGA Wonder
	push	ax
	push	dx
	cli
	mov	OSEG[curbk],ax
	mov	ah,al
	mov	dx,1ceh
	mov	al,0b2h
	out	dx,al
	inc	dl
	in	al,dx
	shl	ah,1
	and	al,0e1h
	or	ah,al
	mov	al,0b2h
	dec	dl
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret


_everex::			;Everex
	push	ax
	push	dx
	push	cx
	cli
	mov	OSEG[curbk],ax
	mov	cl,al
	mov	dx,3c4h
	mov	al,8
	out	dx,al
	inc	dl
	in	al,dx
	dec	dl
	shl	al,1
	shr	cl,1
	rcr	al,1
	mov	ah,al
	mov	al,8
	out	dx,ax
	mov	dl,0cch
	in	al,dx
	mov	dl,0c2h
	and	al,0dfh
	shr	cl,1
	jc	nob2
	or	al,20h
nob2:	out	dx,al
	sti
	pop	cx
	pop	dx
	pop	ax
	ret


_aheada::			;Ahead Systems Ver A
	push	ax
	push	dx
	push	cx
	cli
	mov	OSEG[curbk],ax
	mov	ch,al
	mov	dx,3ceh		;Enable extended registers
	mov	ax,200fh
	out	dx,ax
	mov	dl,0cch		;bit 0
	in	al,dx
	mov	dl,0c2h
	and	al,11011111b
	shr	ch,1
	jnc	skpa
	or	al,00100000b
skpa:	out	dx,al
	mov	dl,0cfh		;bits 1,2,3
	mov	al,0
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	and	al,11111000b
	or	al,ch
	mov	ah,al
	mov	al,0
	out	dx,ax
	sti
	pop	cx
	pop	dx
	pop	ax
	ret


_aheadb::			;Ahead Systems Ver A
	push	ax
	push	dx
	push	cx
	cli
	mov	OSEG[curbk],ax
	mov	ch,al
	mov	dx,3ceh		;Enable extended registers
	mov	ax,200fh
	out	dx,ax
	mov	ah,ch
	mov	cl,4
	shl	ah,cl
	or	ah,ch
	mov	al,0dh
	out	dx,ax
	sti
	pop	cx
	pop	dx
	pop	ax
	ret


_oaktech::			;Oak Technology Inc OTI-067 and OTI-077
	push	ax
	push	dx
	cli
	mov	OSEG[curbk],ax
	and	al,15
	mov	ah,al
	shl	al,1
	shl	al,1
	shl	al,1
	shl	al,1
	or	ah,al
	mov	al,11h
	mov	dx,3deh
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret

_genoa::			;GENOA GVGA
	push	ax
	push	dx
	cli
	mov	OSEG[curbk],ax
	mov	ah,al
	shl	al,1
	shl	al,1
	shl	al,1
	or	ah,al
	mov	al,6
	or	ah,40h
	mov	dx,3c4h
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret

_ncr::				;NCR 77C22E
	push	ax
	push	dx
	cli
	mov	OSEG[curbk],ax
	mov	ah,al
	mov	al,18h
	mov	dx,3c4h
	out	dx,ax
	mov	ax,19h
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret

_compaq::			;Compaq
	push	ax
	push	dx
	push	ax
	cli
	mov	OSEG[curbk],ax
	mov	dx,3ceh
	mov	ax,50fh		;unlock extended registers
	out	dx,ax
	pop	ax
	mov	ah,al
	mov	al,45h
	out	dx,ax
	sti
	pop	dx
	pop	ax
	ret

_vesa::				;Vesa SVGA interface
	push	ax
	push	bx
	push	dx
	cli
	mov	OSEG[curbk],ax
	mov	dx,ax
	xor	bx,bx
	call	dword ptr OSEG[vesafunc]
	mov	dx,OSEG[curbk]
	mov	bx,1
	call	dword ptr OSEG[vesafunc]
	sti
	pop	dx
	pop	bx
	pop	ax
	ret

_nobank::
	cli
	mov	OSEG[curbk],ax
	sti
	ret

newbank	endp

bkadr	macro	func
	mov	[func],1
	mov	word ptr [bankadr],offset _&func
	mov	word ptr [bankadr+2],seg _&func
	endm

nojmp	macro
	local	lbl
	jmp	lbl
lbl:
	endm

dummyvesa proc	far
	ret
dummyvesa endp











whichvga proc	uses si di es ds
	local	vesabuf[512]:byte	; extra large to make up for
					; bugs in some VESA VBE's

	cmp	[first],'FI'
	jnz	gotest
	mov	ax,[retval]
	ret







gotest:	mov	word ptr [bankadr],offset _nobank
	mov	word ptr [bankadr+2],seg _nobank


	mov	[vgamem],256
	mov	[bksize],64
	xor	ax,ax
	mov	[curbk],ax
	mov	[cirrus],ax
	mov	[everex],ax
	mov	[paradise],ax
	mov	[tseng],ax
	mov	[trident],ax
	mov	[t8900],ax
	mov	[ativga],ax
	mov	[aheada],ax
	mov	[aheadb],ax
	mov	[oaktech],ax
	mov	[video7],ax
	mov	[chipstech],ax
	mov	[tseng4],ax
	mov	[genoa],ax
	mov	[ncr],ax
	mov	[compaq],ax
	mov	[vesa],ax
	mov	[dactype],ax
	mov	[first],'FI'






	mov	si,1			;flag for SVGA found




	mov	ax,ss
	mov	es,ax
	lea	di,vesabuf[0]
	mov	ax,4f00h
	int	10h
	cmp	ax,4fh
;ù
	jnz	novesa

	mov	[bksize],64		; (for now)
	mov	[vesafunc],offset dummyvesa
	mov	[vesafunc+2],cs
	bkadr	vesa
	mov	si,offset vesamodes
	cld
vesalp:	lodsw
	mov	cx,ax
	jcxz	endvesa
	mov	ax,ss
	mov	es,ax
	lea	di,vesabuf[0]
	mov	ax,4f01h		;is mode available?
	int	10h
	or	ah,ah
	jnz	notmode
	test	byte ptr es:[di],1
	jz	notmode			;if mode not available skip mem size
	lodsw
	mov	cx,ax
	lodsw
	mov	bx,ax
	lodsw
	mul	cx			;multiple by bits per pixel
	shr	dx,1			;shift right 3 times to get # of bytes
	rcr	ax,1
	shr	dx,1
	rcr	ax,1
	shr	dx,1
	rcr	ax,1
	mul	bx			;multiply by number of scanlines
	mov	bx,1024
	add	ax,bx			;round up to nearest kilobyte
	dec	ax
	adc	dx,0
	div	bx
	cmp	ax,[vgamem]		;if mode requires more memory
	jbe	vesalp
	mov	[vgamem],ax		;increase amount of vga memory
	mov	ax,vesabuf.vesamode.WinSize
	mov	[bksize],ax
	jmp	short vesalp
notmode:
	add	si,6			;skip to next mode in list
	jmp	short vesalp
endvesa:
	mov	ax,[vgamem]
	inc	ah
	dec	ax
	mov	al,0			;round up to nearest 256k
	cmp	ax,1024
	jb	nomegs
	add	ax,1023			;round up to nearest megabyte
	and	ax,not 1023
nomegs:	mov	[vgamem],ax

;	jmp	fini			;uncomment to disable other tests if VESA VBE found











novesa:	mov	ax,0c000h		;ATI VGA detect (largely from ATI example code)
	mov	es,ax
	cmp	word ptr es:[40h],'13'	;ATI Signiture on the Video BIOS
	jnz	noati
	bkadr	ativga
	mov	[bksize],64		; 64k bank size
	mov	dx,es:[10h]		; Get value of ATI extended register
	mov	bl,es:[43h]		; Get value of ATI chip version
	cmp	bl,'3'
	jae	v6up			; Use different method to determine
	mov	al,0bbh 		; memory size of chip version is 3 or higher
	cli
	out	dx,al
	inc	dx
	in	al,dx			; Get ramsize byte for chip versions 1 & 2
	sti
	test	al,20h
	jz	no512
	mov	[vgamem],512
	jmp	short no512

v6up:	mov	al,0b0h			; Method used for newer ATI chip versions
	cli
	out	dx,al
	inc	dx
	in	al,dx			; Get ramsize byte for versions 3-5
	sti
	test	al,10h			; Check if ramsize byte indicates 256K or 512K bytes
	jz	v7up
	mov	[vgamem],512
v7up:	cmp	bl,'4'			; Check for ramsize for ATI chip versions 4 & 5
	jb	no512
	test	al,8			; Check if version 5 ATI chip has 1024K
	jz	no512
	mov	[vgamem],1024
no512:	jmp	fini

















noati:	mov	ax,7000h		;Test for Everex
	xor	bx,bx
	cld
	int	10h
	cmp	al,70h
	jnz	noev
	bkadr	everex
	mov	[bksize],64		; 64k bank size
	and	ch,11000000b		;how much memory on board
	jz	skp
	mov	[vgamem],512
skp:					;fall through for Everex boards using Trident or Tseng4000









noev:	mov	ax,0bf03h		;Test for Compaq
	xor	bx,bx
	mov	cx,bx
	int	10h
	cmp	ax,0bf03h
	jnz	nocp
	test	cl,40h			;is 640x480x256 available?
	jz	nocp
	bkadr	compaq
	mov	[bksize],4		; 4k bank size
	mov	[vgamem],512
	jmp	fini









nocp:	mov	dx,3c4h			;Test for NCR 77C22E
	mov	ax,0ff05h
	call	_isport2
	jnz	noncr
	mov	ax,5			;Disable extended registers
	out	dx,ax
	mov	ax,0ff10h		;Try to write to extended register 10
	call	_isport2		;If it writes then not NCR
	jz	noncr
	mov	ax,105h			;Enable extended registers
	out	dx,ax
	mov	ax,0ff10h
	call	_isport2
	jnz	noncr			;If it does NOT write then not NCR
	bkadr	ncr
	mov	[bksize],16		; 16k bank size
	mov	[vgamem],512
	jmp	fini
















noncr:	mov	dx,3c4h			;Test for Trident
	mov	al,0bh
	out	dx,al
	inc	dl
	in	al,dx
	and	al,0fh
	cmp	al,06h
	ja	notri
	cmp	al,2
	jb	notri
	bkadr	trident
	mov	[bksize],64		; 64k bank size
	cmp	al,3
	jb	no89
	mov	[t8900],1
	mov	dx,3d4h
	mov	al,1fh
	out	dx,al
	inc	dx
	in	al,dx
	and	ax,3
	inc	al
	xchg	al,ah
	mov	[vgamem],ax
	jmp	fini

no89:	mov	[vgamem],512
	jmp	fini



















notri:	mov	ax,6f00h		;Test for Video 7
	xor	bx,bx
	cld
	int	10h
	cmp	bx,'V7'
	jnz	nov7
	bkadr	video7
	mov	[bksize],64		; 64k bank size
	mov	ax,6f07h
	cld
	int	10h
	and	ah,7fh
	cmp	ah,1
	jbe	skp2
	mov	[vgamem],512
skp2:	cmp	ah,3
	jbe	skp3
	mov	[vgamem],1024
skp3:	jmp	fini























nov7:	mov	dx,3d4h			;Test for GENOA GVGA
	mov	al,2eh			;check for Herchi Register top 6 bits
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	test	al,11111100b		;top 6 bits should be zero
	jnz	nogn
	mov	ax,032eh		;check for Herchi Register
	call	_isport2
	jnz	nogn
	mov	dx,3c4h
	mov	al,7
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	test	al,10001000b
	jnz	nogn
	mov	al,10h
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	and	al,00110000b
	cmp	al,00100000b
	jnz	nogn
	mov	dx,3ceh
	mov	ax,0ff0bh
	call	_isport2
	jnz	nogn
	mov	dx,3c4h			;check for memory segment register
	mov	ax,3f06h
	call	_isport2
	jnz	nogn
	mov	dx,3ceh
	mov	ax,0ff0ah
	call	_isport2
	jnz	nogn
	bkadr	genoa
	mov	[bksize],64		; 64k bank size
	mov	[vgamem],512
	jmp	fini

















nogn:	call	_cirrus			;Test for Cirrus
	cmp	[cirrus],0
	je	noci
	jmp	fini


















noci:	mov	dx,3ceh			;Test for Paradise
	mov	al,9			;check Bank switch register
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	or	al,al
	jnz	nopd

	mov	ax,50fh			;turn off write protect on VGA registers
	out	dx,ax
	mov	dx,offset _paradise
	mov	cx,1
	call	_chkbk
	jc	nopd			;if bank 0 and 1 same not paradise
	bkadr	paradise
	mov	[bksize],4		; 4k bank size
	mov	dx,3ceh
	mov	al,0bh			;512k detect from Bob Berry
	out	dx,al
	inc	dx
	in	al,dx
	test	al,80h			;if top bit set then 512k
	jz	nop512
	test	al,40h
	jz	nop1024
	mov	[vgamem],1024
	jmp	fini
nop1024:
	mov	[vgamem],512
nop512:	jmp	fini





















nopd:	mov	ax,5f00h		;Test for Chips & Tech
	xor	bx,bx
	cld
	int	10h
	cmp	al,5fh
	jnz	noct
	bkadr	chipstech
	mov	[bksize],16		; 16k bank size
	cmp	bh,1
	jb	skp4
	mov	[vgamem],512
skp4:	jmp	fini
























noct:	mov	dx,3dah			;Test for Tseng 4000 & 3000
	in	al,dx			;bit 8 is opposite of bit 4
	mov	ah,al			;(vertical retrace bit)
	shr	ah,1
	shr	ah,1
	shr	ah,1
	shr	ah,1
	xor	al,ah
	test	al,00001000b
;ù
	jz	nots

	mov	dx,3d4h			;check for Tseng 4000 series
	mov	ax,0f33h
	call	_isport2
;ù
	jnz	not4

	mov	ax,0ff33h		;top 4 bits should not be there
	call	_isport2
;ù
	jz	nots


	mov	dx,3bfh			;Enable access to extended registers
	mov	al,3
	out	dx,al
	mov	dx,3d8h
	mov	al,0a0h
	out	dx,al
	mov	al,0ffh
	mov	dx,3cdh			;test bank switch register
	call	_isport1
	jnz	not4
	bkadr	tseng
	mov	[bksize],64		; 64k bank size
	mov	ax,10f1h		;Extended BIOS call to get DAC type on Tseng4000
	int	10h
	cmp	ax,10h
	jnz	nodac
	mov	bh,0
	mov	[dactype],bx
nodac:	mov	dx,3d4h			;Tseng 4000 memory detect 1meg
	mov	al,37h
	out	dx,al
	inc	dx
	in	al,dx
	test	al,1000b		;if using 64kx4 RAMs then no more than 256k
	jz	nomem
	and	al,3
	cmp	al,1			;if 8 bit wide bus then only two 256kx4 RAMs
	jbe	nomem
	mov	[vgamem],512
	cmp	al,2			;if 16 bit wide bus then four 256kx4 RAMs
	je	nomem
	mov	[vgamem],1024		;full meg with eight 256kx4 RAMs
nomem:	bkadr	tseng4
	mov	[bksize],64		; 64k bank size
	jmp	fini






























not4:	mov	dx,3d4h			;Test for Tseng 3000
	mov	ax,1f25h		;is the Overflow High register there?
	call	_isport2
	jnz	nots
	mov	al,03fh			;bottom six bits only
	mov	dx,3cdh			;test bank switch register
	call	_isport1
	jnz	nots
	bkadr	tseng
	mov	[bksize],64		; 64k bank size
	call	_t3memchk
	jmp	fini



















nots:	mov	dx,3ceh			;Test for Ahead A or B chipsets
	mov	ax,0ff0fh		;register should not be fully available
	call	_isport2
	jz	noab
	mov	ax,200fh
	out	dx,ax
	inc	dx
	nojmp
	in	al,dx
	cmp	al,21h
	jz	verb
	cmp	al,20h
	jnz	noab
	bkadr	aheada
	mov	[bksize],64		; 64k bank size
	mov	[vgamem],512
	jmp	fini

verb:	bkadr	aheadb
	mov	[bksize],64		; 64k bank size
	mov	[vgamem],512
	jmp	fini


























noab:	mov	dx,3c4h			;Test for AcuMos chipsets
	mov	ax,0006h		;disable extended registers
	out	dx,ax
	mov	ax,0ff09h
	call	_isport2		;is scratchpad at index 9 writeable ?
	jz	noacu
	mov	ax,0ff0ah
	call	_isport2		;is scratchpad at index 10 writable ?
	jz	noacu
	mov	ax,1206h
	out	dx,ax
	mov	ax,0ff09h
	call	_isport2
	jnz	noacu
	mov	ax,0ff0ah
	call	_isport2
	jnz	noacu
	mov	dx,offset _acumos
	mov	cx,1
	call	_chkbk
	jc	noacu			;if bank 0 and 1 same not acumos
	bkadr	acumos
	mov	[bksize],4		; 4k bank size
	mov	dx,3c4h
	mov	al,0ah
	out	dx,al
	inc	dx
	in	al,dx			;get scratchpad index 10
	and	al,3
	cmp	al,1
	jb	noamem
	mov	[vgamem],512
	cmp	al,2
	jb	noamem
	mov	[vgamem],1024
	cmp	al,3
	jb	noamem
	mov	[vgamem],2048
noamem:	jmp	short fini


































noacu:	mov	dx,3deh			;Test for Oak Technology Inc OTI-067 and OTI-077
	mov	ax,0ff11h		;look for bank switch register
	call	_isport2
	jnz	nooak
	bkadr	oaktech
	mov	[bksize],64		; 64k bank size
	mov	al,0dh
	out	dx,al
	inc	dx
	nojmp
	in	al,dx
	test	al,11000000b
	jz	no4ram
	mov	[vgamem],512
	test	al,01000000b
	jz	no4ram
	mov	[vgamem],1024
no4ram:	jmp	short fini

nooak:





















	mov	si,0

fini:	cmp	[vesa],0
	jz	novbank
	bkadr	vesa			; Always use VESA bank switch
					; even if we have detected chipset
novbank:
	mov	ax,si
	mov	[retval],ax
	ret



whichvga endp


;Segment to access video buffer (based on GR[6])
buftbl	dw	0A000h,0A000h,0B000h,0B800h

_t3memchk proc				;[Charles Marslett -- ET3000 memory ck]
	mov	dx,3dah
	in	al,dx			;Reset the attribute flop (read 0x3DA)
	mov	dx,03c0h
	mov	al,36h
	out	dx,al
	inc	dx
	in	al,dx			;Save contents of ATTR[0x16]
	push	ax
	or	al,10h
	dec	dx
	out	dx,al
	mov	dx,3ceh			;Find the RAM buffer...
	mov	al,6
	out	dx,al
	inc	dx
	in	al,dx
	and	ax,000Ch
	shr	ax,1

	mov	bx,ax
	push	es
	mov	es,cs:buftbl[bx]
	mov	ax,09C65h
	mov	bx,1
	mov	es:[bx],ax
	mov	es:[bx+2],ax
	inc	bx
	mov	ax,es:[bx]
	pop	es
	cmp	ax,0659Ch
	jne	et3k_256
	mov	[vgamem],512
et3k_256:
	mov	dx,3c0h
	mov	al,36h
	out	dx,al
	pop	ax
	out	dx,al			;Restore ATTR[16h]
	ret
_t3memchk endp

_cirrus	proc	near
	mov	dx,3d4h		; assume 3dx addressing
	mov	al,0ch		; screen a start address hi
	out	dx,al		; select index
	inc	dx		; point to data
	mov	ah,al		; save index in ah
	in	al,dx		; get screen a start address hi
	xchg	ah,al		; swap index and data
	push	ax		; save old value
	push	dx		; save crtc address
	xor	al,al		; clear crc
	out	dx,al		; and out to the crtc

	mov	al,1fh		; Eagle ID register
	dec	dx		; back to index
	out	dx,al		; select index
	inc	dx		; point to data
	in	al,dx		; read the id register
	mov	bh,al		; and save it in bh

	mov	cl,4		; nibble swap rotate count
	mov	dx,3c4h		; sequencer/extensions
	mov	bl,6		; extensions enable register

	ror	bh,cl		; compute extensions disable value
	mov	ax,bx		; extensions disable
	out	dx,ax		; disable extensions
	inc	dx		; point to data
	in	al,dx		; read enable flag
	or	al,al		; disabled ?
	jnz	exit		; nope, not an cirrus

	ror	bh,cl		; compute extensions enable value
	dec	dx		; point to index
	mov	ax,bx		; extensions enable
	out	dx,ax		; enable extensions
	inc	dx		; point to data
	in	al,dx		; read enable flag
	cmp	al,1		; enabled ?
	jne	exit		; nope, not an cirrus
	mov	[cirrus],1
	mov	word ptr [bankadr],offset _nobank
	mov	word ptr [bankadr+2],seg _nobank
exit:	pop	dx		; restore crtc address
	dec	dx		; point to index
	pop	ax		; recover crc index and data
	out	dx,ax		; restore crc value
	ret
_cirrus	endp

_chkbk	proc	near		;bank switch check routine
	mov	di,0b800h
	mov	es,di
	xor	di,di
	mov	bx,1234h
	call	_gochk
	jnz	badchk
	mov	bx,4321h
	call	_gochk
	jnz	badchk
	clc
	ret
badchk:	stc
	ret
_chkbk	endp

calldx	macro
	push	cs
	call	dx
	endm

_gochk	proc	near
	push	si
	mov	si,bx

	mov	al,cl
	calldx
	xchg	bl,es:[di]
	mov	al,ch
	calldx
	xchg	bh,es:[di]

	xchg	si,bx

	mov	al,cl
	calldx
	xor	bl,es:[di]
	mov	al,ch
	calldx
	xor	bh,es:[di]

	xchg	si,bx

	mov	al,ch
	calldx
	mov	es:[di],bh
	mov	al,cl
	calldx
	mov	es:[di],bl

	mov	al,0
	calldx
	or	si,si
	pop	si
	ret
_gochk	endp

_isport2 proc	near		;check for valid indexed i/o port, al is index, ah is bit mask
	push	bx
	mov	bx,ax
	out	dx,al
	mov	ah,al
	inc	dx
	in	al,dx
	dec	dx
	xchg	al,ah
	push	ax
	mov	ax,bx
	out	dx,ax
	out	dx,al
	mov	ah,al
	inc	dx
	in	al,dx
	dec	dx
	and	al,bh
	cmp	al,bh
	jnz	noport
	mov	al,ah
	mov	ah,0
	out	dx,ax
	out	dx,al
	mov	ah,al
	inc	dx
	in	al,dx
	dec	dx
	and	al,bh
	cmp	al,0
noport:	pop	ax
	out	dx,ax
	pop	bx
	ret
_isport2 endp

_isport1 proc	near		;check for valid i/o port, al is bit mask
	mov	ah,al
	in	al,dx
	push	ax
	mov	al,ah
	out	dx,al
	in	al,dx
	and	al,ah
	cmp	al,ah
	jnz	noport
	mov	al,0
	out	dx,al
	in	al,dx
	and	al,ah
	cmp	al,0
noport:	pop	ax
	out	dx,al
	ret
_isport1 endp

	end

