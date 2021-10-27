;----------------------------------------------------------------------------
;				GraphMsk.ASM 386
;			      	(c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------
			include	\projet\lib386\lib_svga\svga.ash

comment @
		DWORD	TabOffset[]
			Brick:
			    - BYTE Delta X
			    - BYTE Delta Y
				Line(Delta Y):
				- BYTE NbBlock
				   Block:
				    - 00xxxxxxb  xxxxxx zeros to jump
				    - 01xxxxxxb  xxxxxx Copy Pix
				    - 10xxxxxxb  xxxxxx Repeat Pix
				    - BYTE datas[]
@

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CalcGraphMsk

;----------------------------------------------------------------------------
CalcGraphMsk		proc	uses esi edi ebx ebp,\
			numbrick:DWORD, bankbrick:DWORD, ptmask:DWORD

			mov	eax, numbrick
			mov	esi, bankbrick
			mov	edi, ptmask
			push	edi
			add	esi, [esi+eax*4]; ESI = Begin Data

;-----------------------------------------------
			xor	edx, edx
			mov	ebx, [esi]	; Get DX, DY, Hot X, Hot Y
			mov	[edi], ebx	; Stock DX, DY, Hot X, Hot Y
			add	esi, 4		; bh = NbLine
			add	edi, 4
;-----------------------------------------------
			xor	ecx, ecx	; Maz Compteur
;----------------------------------------------- Init NbBlock for this line
NextLine:		xor	dl, dl		; NbBlockDst = 0
			xor	ah, ah		; NbData = 0
			mov	ebp, edi	; Sauve Pos NbBlockDst
			inc	edi		; Jump NbBlockDst
			mov	bl, [esi]	; BL = Nb Block for this line
			inc	esi
			test	byte ptr [esi], 11000000b; Jump Zero ?
			je	SameLine	; Line MUST Begin JumpZero
;----------------------------------------------- Premier Block En Jump Zero
			mov	byte ptr[edi], 0
			inc	edi
			inc	dl		; NbBlockDst++
;----------------------------------------------- Manage One Line
SameLine:		mov	al, [esi]
			inc	esi   		; OpCode
			mov	cl, al          ; Sauve AL
			and	cl, 00111111b	; AH = Bit 0-5
			inc	cl		; One More Please...
;-----------------------
			test	al, 10000000b
			jne	RepeatCol
			test	al, 01000000b
			jne	CopyCol
;----------------------------------------------- 00 Jump CL Zero
			or	ah, ah		; NbData
			je	RienEnCours
;-----------------------
			mov	[edi], ah
			inc	edi
			inc	dl		; NbBlockDst++
			xor	ah, ah		; NbData=0
RienEnCours:		mov	[edi], cl
			inc	edi
			inc	dl		; NbBlockDst++
			jmp	EndLoop
;----------------------------------------------- 01 Repeat Nb Col
RepeatCol:              add	ah, cl		; NbData += Nb
			inc	esi		; pts++
			jmp	EndLoop
;----------------------------------------------- 10 Copy Nb Col
CopyCol:		add	ah, cl		; NbData += Nb
			add	esi, ecx	; pts += nb
;----------------------------------------------- End Loop
EndLoop:		dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
;----------------------------------------------- Cloture Eventuelle
			or	ah, ah		; NbData
			je	PasDeCloture
;-----------------------
			mov	[edi], ah
			inc	edi
			inc	dl		; NbBlockDst++
;-----------------------
PasDeCloture:           mov	ds:[ebp], dl	; Write NbBlockDst
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
;-----------------------
			mov	eax, edi
			pop	edi
			sub	eax, edi	; Size Mask
			ret
;----------------------
CalcGraphMsk		endp
;----------------------------------------------------------------------------
;				The
				End






































