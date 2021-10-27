;				S_BOX.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p
				jumps
				.model SMALL, SYSCALL

				.data

			include	svga.ash

				.code

			public	NoLanguage Box

;----------------------------------------------------------------------------
;			Box( x0, y0, x1, y1, col )
;
;
Box			proc	uses edi ebp ebx,\
			X0:DWORD, Y0:DWORD, X1:DWORD, Y1:DWORD, Col:DWORD

;----------------------
			mov	eax, X0
			mov	ebx, Y0
			mov	ecx, X1
			mov	edx, Y1

			cmp	eax, ClipXmax
			jg	Box_End
			cmp     ecx, ClipXmin
			jl	Box_End
			cmp	ebx, ClipYmax
			jg	Box_End
			cmp	edx, ClipYmin
			jl	Box_End
TestLeft:
			cmp	eax, ClipXmin
			jge	TestRight
			mov	eax, ClipXmin
TestRight:
			cmp	ecx, ClipXmax
			jle	TestUp
			mov	ecx, ClipXmax
TestUp:
			cmp	ebx, ClipYmin
			jge	TestDown
			mov	ebx, ClipYmin
TestDown:
			cmp	edx, ClipYmax
			jle	OkNoClip
			mov	edx, ClipYmax
OkNoClip:
;----------------------
			mov	edi, ebx		; Y0
			mov	edi, TabOffLine[edi*4]
			add	edi, Log
			add	edi, eax		; EDI Offset Src
;----------------------
			sub	edx, ebx		; Init Delta Y
			mov	ebx, edx
			inc	ebx

			mov	edx, ecx		; Init Delta X
			sub	edx, eax
			inc	edx
;----------------------
			mov	al, byte ptr [Col]
			mov	ah, al			; Init Color
			mov	cx, ax
			shl	eax, 16
			mov	ax, cx
;----------------------
			mov	ebp, Screen_X
			sub	ebp, edx		; ebp Delta Screen
Again:
			mov	ecx, edx
			shr	ecx, 2
			rep	stosd
			mov	ecx, edx
			and	ecx, 11b
			rep	stosb

			add	edi, ebp
			dec	ebx
			jne	Again
Box_End:
			ret
Box			endp
;----------------------------------------------------------------------------
;				The
				End

