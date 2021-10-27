

	.model	SMALL, SYSCALL


	include	vesa.inc

;
;	VGAKIT Version 5.2b
;
;	Copyright 1988,89,90,91,92 John Bridges
;	Free for use in commercial, shareware or freeware applications
;
;	SVGAMODE.ASM
;
;
.data
	extrn	cirrus:byte,everex:byte,paradise:byte,tseng:byte,trident:byte
	extrn	t8900:byte,ativga:byte,aheada:byte,aheadb:byte
	extrn	oaktech:byte,video7:byte,chipstech:byte,tseng4:byte,genoa:byte
	extrn	ncr:byte,compaq:byte,acumos:byte,vesa:byte

	extrn	curbk:word,bksize:word

	extrn	vesafunc:dword

	public	maxx,maxy,scanline,ourseg

maxx	dw	?		;scanline of screen in pixels
maxy	dw	?		;height of screen in pixels
scanline dw	?		;actual width screen in bytes
ourseg	dw	?		;segment of screen buffer


	public	adrtbl

adrtbl	dd	1024 dup (?)	;offset and bank for each scanline

	.code	LIB_SVGA

	public	mkadrtbl
	public	svgamode
	public	txtmode
	public	setmany

mkadrtbl proc	uses di si
	mov	di,offset adrtbl
	mov	ax,ds
	mov	es,ax
	mov	bx,[maxy]

	mov	ax,[bksize]
	cmp	ax,64
	jz	nobnk
	mov	cl,10
	shl	ax,cl
	dec	ax
	mov	si,ax
	mov	cl,9
	mov	ax,[bksize]
shlp:	inc	cl
	shr	ax,1
	jnz	shlp
	xor	ax,ax
	xor	dx,dx
lp:	push	ax
	shr	ax,cl
	add	dx,ax
	pop	ax
	and	ax,si
	stosw
	xchg	ax,dx
	stosw
	xchg	ax,dx
	add	ax,[scanline]
	dec	bx
	jnz	lp
	ret

nobnk:	xor	ax,ax
	xor	dx,dx
	mov	si,[scanline]
	mov	cx,bx
nlp:	stosw
	xchg	ax,dx
	stosw
	xchg	ax,dx
	add	ax,si
	adc	dx,0
	loop	nlp
	ret

mkadrtbl endp

svgamode proc			;Set 640x480x256 on most SVGA cards
	mov	[ourseg],0a000h
	mov	ax,640
	mov	[maxx],ax		;default width to 640 for now
	mov	[scanline],ax

	cmp	[vesa],0
	jz	novs
	mov	bx,101h
	call	vesaset
	jmp	godo2
novs:	cmp	[compaq],0
	jz	nocp
	mov	[scanline],1024
	mov	ax,2eh
	jmp	godo
nocp:	cmp	[genoa],0
	jz	nogn
	mov	ax,5ch
	jmp	godo
nogn:	cmp	[ncr],0
	jz	noncr
	mov	ax,5fh
	jmp	godo
noncr:	cmp	[oaktech],0
	jz	noak
	mov	ax,53h
	jmp	short godo
noak:	cmp	[aheada],0
	jnz	@f
	cmp	[aheadb],0
	jz	noab
@@:	mov	ax,61h
	jmp	short godo
noab:	cmp	[everex],0
	jz	noev
	mov	ax,70h
	mov	bl,30h
	jmp	short godo
noev:	cmp	[ativga],0
	jz	noati
	mov	ax,62h
	jmp	short godo
noati:	cmp	[trident],0
	jz	notr
	mov	ax,5dh
	jmp	short godo
notr:	cmp	[video7],0
	jz	nov7
	mov	ax,6f05h
	mov	bl,67h
	jmp	short godo
nov7:	cmp	[chipstech],0
	jz	noct
	mov	ax,79h
	jmp	short godo
noct:	cmp	[acumos],0
	jnz	dopd
	cmp	[paradise],0
	jz	nopd
dopd:	mov	ax,5fh
	jmp	short godo
nopd:	cmp	[tseng],0
	jz	nots
	mov	ax,2eh
godo:	int	10h

godo2:	mov	[curbk],-1
	mov	ax,40h
	mov	es,ax
	mov	al,es:[84h]		;get height of screen
	inc	al
	mul	byte ptr es:[85h]
	mov	[maxy],ax
	call	mkadrtbl
nots:	ret
svgamode endp

vesaset	proc	near uses di
	local	modebuf[260]:byte	; extra large to make up for
					; bugs in some VESA VBE's

	push	bx
	mov	ax,4f02h		; set the VESA videomode
	int	10h
	pop	cx
	mov	ax,ss
	mov	es,ax
	lea	di,modebuf[0]		; get the mode information
	mov	ax,4f01h
	int	10h
	mov	ax,modebuf.vesamode.WinGranularity
	mov	[bksize],ax		; bank size from vesamode structure
	mov	ax,modebuf.vesamode.BytesPerLine
	mov	[scanline],ax		; scan line byte width from vesamode
	mov	ax,word ptr modebuf.vesamode.WinFuncPtr
	mov	word ptr [vesafunc],ax
	mov	ax,word ptr modebuf.vesamode.WinFuncPtr+2
	mov	word ptr [vesafunc+2],ax
	ret
vesaset	endp

txtmode	proc
	mov	ax,3
	int	10h
	ret
txtmode	endp

setmany proc	uses es,\
		palbuf:ptr byte,begcol:word,numcol:word

	les	dx,[palbuf]
	mov	bx,[begcol]
	mov	cx,[numcol]
	mov	ax,1012h
	int	10h
	ret

setmany endp


	end

