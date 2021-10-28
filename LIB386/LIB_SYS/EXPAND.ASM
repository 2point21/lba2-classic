	.386P
	.MODEL	SMALL, SYSCALL

	.CODE

	public	NoLanguage	Expand

;*──────────────────────────────────────────────────────────────────────────*

Expand	PROC USES ESI EDI EBX EBP,\
	src:DWORD, dest:DWORD, count:DWORD   ; COUNT devrait etre un DWORD !!!

	mov	esi, src
	mov	edi, dest
	mov	ebp, count		; compteur pour savoir si on a fini...
					; et puis j'ai plus besoin de lire
					; d'autre parametres... d'ou BP !
					; BP = decomp size
boucle:
	mov	dx, 8			; 1 octet info pour 8 data de 8 ou
					; 16 bits (ca depend du bit...)
	lodsb
	mov	bx, ax			; octet d'info dans bl
next:
	shr	bx, 1			; ca va plus vite de décaler bx ou
					; bl ? (Vrai question, chef)
	jnc	copie			; chaque bit indique la nature de
					; ce qui suit

	movsb				; ici un octet de data a recopier
	dec	ebp			; 1 de moins
	jz	termine			; si c'est fini, c'est fini
	dec	dx			; 1 bit de moins
	jnz	next			; s'il en reste on continu
	jmp	boucle			; sinon on lit un octet d'info de plus

copie:					; ici on a 16 bits formant un offset
					; (sur 12 bits) et une longueur					; (sur 4 bits)
	lodsw				; on lit donc cette donnee
	movzx	ecx, ax
	mov	eax, ecx
	and	ecx, 0Fh		; on recupere la longueur dans cx
	inc	ecx			; on ajuste car pas de copie de
	inc	ecx			; bloc de longeur < 2 (d'ou gain...)
	sub	ebp, ecx		; il restera ca en moins a faire
	shr	eax, 4			;
					; on recupere l'offset
	not	eax			; en fait c'est un decalage en
					; arrière - 1 (not ax = -ax - 1)
	add	eax, edi		; par rapport à di (ax = di - ax - 1)
	xchg	eax, esi		; on sauve si tout en lui donnant
					; l'ofset calcule
	rep	movsb			; on fait la recopie

	mov	esi, eax		; restore si
	or	ebp, ebp		; il en reste ?
	jz	termine			; ...
	dec	dx			; il reste des bits d'info ?
	jnz	next                    ; ...
	jmp	boucle                  ; ...
termine:				; c'est fini... Quoi ?!!! deja ?!!!
					; c'est con cette methode de
					; compression ! Mais ca marche !
	ret

Expand	ENDP


	end
