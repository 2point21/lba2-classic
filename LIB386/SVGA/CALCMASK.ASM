;----------------------------------------------------------------------------
			.386p
			.model SMALL, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
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
			.CODE

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	CalcGraphMsk

;----------------------------------------------------------------------------
;S32	CalcGraphMsk(S32 numbrick, void *bankbrick, void *ptmask)

;#pragma aux CalcGraphMsk	"*"	\
;	parm	[eax] [esi] [edi]	\
;	modify	[ebx ecx edx]

CalcGraphMsk		proc	uses ebp

			push	edi
			add	esi, [esi+eax*4]; ESI = Begin Data

;-----------------------------------------------
			mov	ebx, [esi]	; Get DX, DY, Hot X, Hot Y
			add	esi, 4		; bh = NbLine
			mov	[edi], ebx	; Stock DX, DY, Hot X, Hot Y
			add	edi, 4

			xor	ecx, ecx
;----------------------------------------------- Init NbBlock for this line
NextLine:
			mov	word ptr [edi],0000h
			mov	ebp, edi	; Sauve Pos NbBlockDst
			mov	bl, [esi]	; BL = Nb Block for this line
			inc	esi
			inc	edi		; Jump NbBlockDst
			xor	ah,ah		; AH = Flag :           si AH=0   => Code "Jump"
						;			si AH=1   => Code "Mask"
						; On commence toujours pas un "Jump"
SameLine:		mov	al, [esi]
			inc	esi   		; OpCode
			mov	cl, al          ; Sauve AL
			and	cl, 00111111b
			inc	cl		; One More Please...
;-----------------------------------------------
			test	al, 10000000b
			jne	RepeatCol
			test	al, 01000000b
			jne	CopyCol
;----------------------------------------------- 00 Jump CL Zero
			test	ah,ah		; Teste si AH = 1
			je	@@Same_Block1
			inc	edi
			xor	ah,ah
			inc	byte ptr [ebp]	; Incremente Nb de blocks (car chgt de block)
			mov	[edi],ah	; Ecrit zero a cause du ADD
@@Same_Block1:
			add	[edi], cl
			jmp	EndLoop
;----------------------------------------------- 01 Repeat Nb Col
RepeatCol:
			test	ah,ah
			jne	@@Same_Block2
			mov	byte ptr [edi+1],0	; Precaution avec le ADD
			inc	edi
			inc	byte ptr [ebp]
			mov	ah,1
@@Same_Block2:
			add	[edi], cl	; NbData += Nb
			inc	esi		; pts++
			jmp	EndLoop
;----------------------------------------------- 10 Copy Nb Col
CopyCol:
			test	ah,ah
			jne	@@Same_Block3
			mov	byte ptr [edi+1],0
			inc	edi
			inc	byte ptr [ebp]
			mov	ah,1
@@Same_Block3:
			add	[edi], cl	; NbData += Nb
			add	esi, ecx	; pts += nb
;----------------------------------------------- End Loop
EndLoop:		dec	bl		; Nb Block--
			jne	SameLine	; Continue Same Line
;----------------------------------------------- Cloture Eventuelle
			inc	edi
			inc	byte ptr [ebp]	; NbBlockDst++
;-----------------------
			dec	bh		; NbLine--
			jne	NextLine	; Next Line
;-----------------------
			pop	ebx
			mov	eax, edi

			sub	eax, ebx	; Size Mask
			ret
;-----------------------
CalcGraphMsk		endp

;----------------------------------------------------------------------------
;				The
				End
