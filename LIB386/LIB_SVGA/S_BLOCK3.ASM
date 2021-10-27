;----------------------------------------------------------------------------
;				 S_BLOCK.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
				.model SMALL, SYSCALL

				.data

			include	f:\projet\lib386\lib_svga\svga.ash

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CopyBlockIncrust

;----------------------------------------------------------------------------
;			CopyBlockIncrust( x0, y0, x1, y1, *src, xd, yd, *dst )
;
CopyBlockIncrust	proc	uses esi edi ebx ebp,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD, src:DWORD,\
			xd:DWORD, yd:DWORD, dst:DWORD

			mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1
;----------------------

			mov	esi, TabOffLine[ecx*4]	; y0
			add	esi, src
			add	esi, edx		; SI Offset Src

			mov	edi, yd
			mov	edi, TabOffLine[edi*4]
			add	edi, dst
			add	edi, xd			; DI Offset Dst

			sub	ebx, edx
			inc	ebx 			; BX Delta Y

			sub	eax, ecx
			inc	eax                     ; AX Delta X
                        mov	ebp, eax

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen
Again:
			mov	ecx, ebx

loopx:			lodsb
			or	al, al
			jz	incrust
			mov	byte ptr[edi], al
incrust:		inc	edi
			dec	ecx
			jnz	loopx

			add	esi, edx
			add	edi, edx
			dec	ebp
			jne	short Again
CopyBlock_End:		ret
CopyBlockIncrust	endp

;----------------------------------------------------------------------------
;				The
				End






