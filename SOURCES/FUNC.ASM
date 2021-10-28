		.386p

		.model FLAT, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*
		.code

;*──────────────────────────────────────────────────────────────────────────*
ifndef	_WIN32
		public	SYSCALL	GetHDFreeSize
endif
		public	SYSCALL	SearchBoundColRGB

;----------------------------------------------------------------------------
ifndef	_WIN32

;*══════════════════════════════════════════════════════════════════════════*

GetHDFreeSize		proc

			xor	eax, eax	;' Cause IMUL
			xor	ebx, ebx	;' Cause IMUL
			xor	ecx, ecx	;' Cause IMUL

			mov	ah, 36h
			xor	edx, edx	;' Current Drive
			int	21h		;' Get Free Space

			imul	ebx
			imul	ecx		;' Bytes*Sectors*Clusters

			ret

GetHDFreeSize		endp

;*══════════════════════════════════════════════════════════════════════════*/
endif

;*══════════════════════════════════════════════════════════════════════════*
;----------------------------------------------------------------------------
;		U8 SearchBoundColRGB( U8 Rouge, U8 Vert, U8 Bleu, (U8*)Palette )
;
;//	ATTENTION:	Mettre un AllowPatch pour la version Windows

SearchBoundColRGB	PROC	Rouge: DWORD, Vert: DWORD, Bleu: DWORD, Palette: DWORD,\
				coulmin:DWORD, coulmax:DWORD

			push	ebx
			mov	ecx,coulmin

			mov	ebx,coulmax
			push	edi

			push	esi
			mov	edi,-1			; Norme de la meilleure couleur

			lea	esi,[ecx*2+ecx]
			push	ecx

			add	esi,[Palette]
			jmp	Search_Coul
Pas_La_Plus_Proche:
			cmp	ecx,ebx
			ja	exit
Search_Coul:
			xor	edx,edx
			xor	eax,eax
			mov	dl,[esi]
			mov	al,[esi+1]
			sub	edx,[Rouge]
			sub	eax,[Vert]
			imul	edx,edx
			imul	eax,eax
			add	edx,eax
			xor	eax,eax
			mov	al,[esi+2]
			add	esi,3
			sub	eax,[Bleu]
			imul	eax,eax
			inc	ecx
			add	edx,eax

			cmp	edx,edi
			jae	Pas_La_Plus_Proche

			mov	edi,edx
			mov	[esp],ecx

			cmp	ecx,ebx
			jbe	Search_Coul
exit:
			pop	eax
			pop	esi

			dec	eax
			pop	edi

			and	eax,0FFh
			pop	ebx

			ret

SearchBoundColRGB	ENDP

;*──────────────────────────────────────────────────────────────────────────*/
			END
