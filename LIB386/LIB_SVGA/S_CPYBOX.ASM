	.model	SMALL, SYSCALL


	.code	LIB_SVGA

	.386
	jumps

OSEG	equ	FS:

	extrn   curbk:WORD
	extrn   newbank:PROC


TestSVGA	proc uses es ds si di,\
		ptrlog:PTR

	lds	si, ptrlog

	xor	ax,ax
	call	newbank

	mov	ax,0A000h
	mov	es,ax

	xor	di,di
	mov	dx,100

b1:
	mov	ecx,80
	rep	movsd


	add	di,320
	dec	dx
	jnz	b1

	mov	ax,1
	call	newbank

	xor	di,di
	mov	dx,100

b2:
	mov	ecx,80
	rep	movsd


	add	di,320
	dec	dx
	jnz	b2

	ret

TestSVGA	endp

TestSVGA2	proc uses es ds si di,\
		ptrlog:PTR

	lds	si, ptrlog

	xor	ax,ax
	call	newbank

	mov	ax,0A000h
	mov	es,ax

	xor	di,di

	mov	dx,200

b1:
	mov	bx,di
	add	bx,320
	jnc	ok1

	mov	cx,320
	sub	cx,bx
	rep	movsb

	mov	ax, OSEG[curbk]
	inc	ax
	call	newbank

	mov	cx,bx
	rep	movsb

	jmp	nxt

ok1:
	mov	ecx,80
	rep	movsd


nxt:	mov	di,bx
	add	di,320

	dec	dx
	jnz	b1

	ret

TestSVGA2	endp

opt_stos_line	macro

	mov	ax,cx
	shr	ecx, 2
	rep	movsd
	and	ax,3
	mov	cx,ax

;shr	cx,1
;rep	movsw
;rcl	cx,1

	rep	movsb

		endm

; CopyBox de MCGA vers SVGA


TestSVGA3	proc uses es ds si di,\
		sox:WORD,soy:WORD,sol:WORD,soh:WORD,soptr:PTR,\
		dex:WORD,dey:WORD



	lds	si, soptr

	mov	ax, soy
	mov	bx,ax
	shl	ax,2		; *4
	add	ax,bx		; *5
	shl	ax,6		; *320
	add	ax, sox

	add	si,ax



	xor	eax,eax
	mov	ax, dey
	mov	ebx,eax
	shl	eax,2		; *4
	add	eax,ebx		; *5
	shl	eax,7		; *640

	mov	bx, dex
	add	eax,ebx

	mov	bx,ax
	shr	eax,16		; ax start bank
	call	newbank

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

b1:	mov	bx,di
patch_larg1	equ	$+2
	add	bx,1234h
	jnc	ok1

patch_larg2	equ	$+1
	mov	cx,1234h
	sub	cx,bx
	opt_stos_line

	mov	ax, OSEG[curbk]
	inc	ax
	call	newbank

	mov	cx,bx
	opt_stos_line

	jmp	nxt

ok1:    xor	ecx,ecx
patch_larg3	equ	$+1
	mov	cx,1234h
	opt_stos_line

nxt:	mov	di,bx
patch_deoff	equ	$+2
	add	di,1234h
	jnc	ok2

	mov	ax, OSEG[curbk]
	inc	ax
	call	newbank

ok2:
patch_sooff	equ	$+2
	add	si,1234h

	dec	dx
	jnz	b1



	ret

TestSVGA3	endp

; CopyBox de MCGA vers SVGA en incrustation coul 0

inc_line	macro
		local	lbl1,lbl2,lbl3

	jcxz	lbl3
lbl1:	lodsb
	or	al,al
	jz	lbl2
	stosb
	loop	lbl1
	jmp	short lbl3
lbl2:	inc	di
	loop	lbl1
lbl3:
		endm


TestSVGA4	proc uses es ds si di,\
		sox:WORD,soy:WORD,sol:WORD,soh:WORD,soptr:PTR,\
		dex:WORD,dey:WORD



	lds	si, soptr

	mov	ax, soy
	mov	bx,ax
	shl	ax,2		; *4
	add	ax,bx		; *5
	shl	ax,6		; *320
	add	ax, sox

	add	si,ax



	xor	eax,eax
	mov	ax, dey
	mov	ebx,eax
	shl	eax,2		; *4
	add	eax,ebx		; *5
	shl	eax,7		; *640

	mov	bx, dex
	add	eax,ebx

	mov	bx,ax
	shr	eax,16		; ax start bank
	call	newbank

	mov	di,bx
	xor	ebx,ebx

	mov	ax,0A000h
	mov	es,ax

	mov	ax, sol
	mov	cs:word ptr[patch_larg1b],ax
	mov	cs:word ptr[patch_larg2b],ax
	mov	cs:word ptr[patch_larg3b],ax
	neg	ax
	add	ax,640
	mov	cs:word ptr[patch_deoffb],ax

	mov	ax,320
	sub	ax, sol
	mov	cs:word ptr[patch_sooffb],ax

	jmp	$+2

	mov	dx, soh

b1:	mov	bx,di
patch_larg1b	equ	$+2
	add	bx,1234h
	jnc	ok1

patch_larg2b	equ	$+1
	mov	cx,1234h
	sub	cx,bx
	inc_line

	mov	ax, OSEG[curbk]
	inc	ax
	call	newbank

	mov	cx,bx
	inc_line

	jmp	nxt

ok1:    xor	ecx,ecx
patch_larg3b	equ	$+1
	mov	cx,1234h
	inc_line

nxt:	mov	di,bx
patch_deoffb	equ	$+2
	add	di,1234h
	jnc	ok2

	mov	ax, OSEG[curbk]
	inc	ax
	call	newbank

ok2:
patch_sooffb	equ	$+2
	add	si,1234h

	dec	dx
	jnz	b1



	ret

TestSVGA4	endp



	end
