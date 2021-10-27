;----------------------------------------------------------------------------
;				S_PLOT.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p
				jumps
				.model SMALL, SYSCALL

				.data

			include	svga.ash

				.code

			public	 NoLanguage	Plot
			public	 NoLanguage	GetPlot

;----------------------------------------------------------------------------
;			Plot( x, y, coul )
;
Plot			proc	uses esi,\
			XX:DWORD, YY:DWORD, CC:DWORD
			mov	eax, XX
			mov	esi, YY
;----------------------
			cmp	eax, ClipXmin
			jl	Plot_End
			cmp	eax, ClipXmax
			jg	Plot_End
			cmp	esi, ClipYmin
			jl	Plot_End
			cmp	esi, ClipYmax
			jg	Plot_End
;----------------------
			mov	esi, TabOffLine[esi*4]

			add	esi, Log

			add	esi, eax

			mov	al, byte ptr [CC]

			mov	[esi], al
Plot_End:
			ret
Plot			endp
;----------------------------------------------------------------------------
;			GetPlot( x, y, coul )
;
GetPlot                 proc	uses esi,\
			XX:DWORD, YY:DWORD
			mov	eax, XX
			mov	esi, YY
;----------------------
			cmp	eax, ClipXmin
			jl	GetPlot_End
			cmp	eax, ClipXmax
			jg	GetPlot_End
			cmp	esi, ClipYmin
			jl	GetPlot_End
			cmp	esi, ClipYmax
			jg	GetPlot_End
;----------------------

			mov	esi, TabOffLine[esi*4]

			add	esi, Log

			add	esi, eax

			xor	eax, eax
			mov	al, [esi]
			ret
GetPlot_End:
			xor	eax, eax
			ret
GetPlot			endp
;----------------------------------------------------------------------------
;				The
				End





















