;----------------------------------------------------------------------------
;				S_SCREEN.ASM 386
;   			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p
				jumps
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data

			include	svga.ash

ALIGN 4

Diff	dd	?

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	Cls
			public	NoLanguage	CopyScreen

;----------------------------------------------------------------------------
;			Cls()
;
Cls			proc	uses edi
			mov	edi, Log
			xor	eax, eax
			mov	ecx, 307200/4
			rep	stosd
			ret
Cls			endp

;----------------------------------------------------------------------------
;			CopyScreen( UBYTE *src, UBYTE *dst )
;
;	Attention:De mémoire à mémoire (pas sur vidéo ram)
;
CopyScreen		proc	uses esi edi,\
			src:DWORD, dst:DWORD

			mov	esi, src
			mov	edi, dst
			mov	ecx, 307200/4
			rep	movsd
			ret
CopyScreen		endp
;----------------------------------------------------------------------------

;
;				The
				End
































