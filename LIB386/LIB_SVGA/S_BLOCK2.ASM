;----------------------------------------------------------------------------
;				 S_BLOCK2.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
				.model SMALL, SYSCALL

				.data
EVEN

CptPixl		dw	0
CptLine		dw	0

			include	f:\projet\lib386\lib_svga\svga.ash


;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CopyBlockOnBlack

;----------------------------------------------------------------------------
;			ptsrc -> ptdst
;			CopyBlockOnBlack( x0, y0, x1, y1, BufSrc, BufDst )
;
;			Copy datas from BufSrc to Bufdst, but only on Black
;
CopyBlockOnBlack	proc	uses esi edi ebx ebp,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD,\
			ptsrc:DWORD, ptdst:DWORD

			mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1
;---------------------- Compute Delta X & Y
			sub	ebx, edx		; EBX Delta X
			sub	eax, ecx		; EAX Delta Y
			inc	ebx 			; EBX = Delta X + 1
			inc	eax                     ; EAX = Delta Y + 1
			mov	word ptr [CptPixl], bx
			mov	word ptr [CptLine], ax
;---------------------- Compute Adresse
			mov	eax, TabOffLine[ecx*4]
			mov	esi, ptsrc
			mov	edi, ptdst
			add	eax, edx
			add	esi, eax
			add	edi, eax
			xor	eax, eax		; Je cherche != 0

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen
;----------------------
NewLine:
			xor	ebx, ebx
			mov	bx, [CptPixl]
;-----------------------------------------------------------------------------
ScanAgain:
			mov	ecx, ebx	; EBX Bytes to Scan
			repne	scasb		; Arret quand != 0
			jne	EndLine2	; c'est fini!

			dec	edi		; EDI un peu trop loin
			inc	ecx		; ECX trop Petit

			sub	ebx, ecx	; EBX = Bytes Parcourus
			add	esi, ebx	; Maj ESI

			mov	ebx, ecx	; EBX = Bytes Restant
			rep	scasb		; Combien != 0 ?
			je	short UntilEnd  ; Tous jusqu'a fin (Quick!

			inc	ecx		; Maj	ECX
			dec	edi		; Maj 	EDI
			sub	ebx, ecx	; EBX = Nb Same Bytes
			sub	edi, ebx	; Recule EDI

			xchg	ecx, ebx
;-----------------------
			mov	ebp, ecx
			shr	ecx, 2
			rep	movsd		; movsD Plus Tard!
			mov	ecx, ebp
			and	ecx, 11b
			rep	movsb
;-----------------------
			jmp	short ScanAgain
UntilEnd:
			sub	edi, ebx
			mov	ecx, ebx
			shr	ecx, 2
			rep	movsd
			mov	ecx, ebx
			and	ecx, 11b
			rep	movsb
EndLine:
;-----------------------------------------------------------------------------
			add	edi, edx
			add	esi, edx
			dec	word ptr [CptLine]
			jne	short NewLine
;----------------------
			ret
EndLine2:
			add	esi, ebx
			add	edi, edx
			add	esi, edx
			dec	word ptr [CptLine]
			jne	short NewLine
;----------------------
			ret

CopyBlockOnBlack	endp
;----------------------------------------------------------------------------
;				The
				End






