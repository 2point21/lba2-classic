;----------------------------------------------------------------------------
;
;'				 ADFLI.ASM 386
;'			        (c) Adeline 1994
;
;----------------------------------------------------------------------------
				.386P
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------
			include	\projet\lib386\lib_svga\svga.ash
;----------------------------------------------------------------------------
				.code

			public	NoLanguage	DrawFrame
			public	NoLanguage	UpdateFrame
			public	NoLanguage	BlackFrame
			public	NoLanguage	CopyFrame

;			public	NoLanguage	SetPal
;			public	NoLanguage	CpyPal

;----------------------------------------------------------------------------
DrawFrame		proc	uses	esi edi ebx,\
			ptframe:DWORD, deltax:DWORD, deltay:DWORD

			mov	esi, ptframe
			mov	edi, Log
			mov	edx, edi
			xor	eax, eax
NewLine:
			lodsb                   	; Nb Block
			or	al, al
			je	EndLine
			mov	bl, al
SameLine:
			lodsb
			test	al, al
			js	short CopyPix		;
			mov	ecx, eax
;----------------------
			lodsb
			rep	stosb
;----------------------
			dec	bl
			jne	short SameLine
			jmp	short EndLine
CopyPix:
;----------------------
			neg	al
			mov	ecx, eax
			shr	ecx, 2
			rep	movsd
			mov	cl, al
			and	cl, 11b
			rep	movsb
;----------------------
			dec	bl
			jne	short SameLine

EndLine:
			add	edx, [deltax]
			mov	edi, edx
			dec	dword ptr [deltay]
			jne	short NewLine
EndDrawFrame:
			ret
DrawFrame		endp
;----------------------------------------------------------------------------
UpdateFrame		proc	uses	esi edi ebx,\
			ptframe:DWORD, deltax:DWORD

			local	cptline	:WORD

			mov	esi, ptframe
			mov	edi, Log
			xor	eax, eax

			lodsw
			mov	edx, deltax
			imul	edx
			add	edi, eax
			mov	edx, edi
			lodsw
			mov	[cptline], ax
			xor	eax, eax
NewLine:
			lodsb                   	; Nb Block
			or	al, al
			je	EndLine
			mov	bl, al
SameLine:
			lodsb
			add	edi, eax

			lodsb
			test	al, al
			js	short RepeatPix		;

			mov	ecx, eax
			shr	ecx, 2
			rep	movsd
			mov	cl, al
			and	cl, 11b
			rep	movsb

			dec	bl
			jne	short SameLine
			jmp	short EndLine
RepeatPix:
			neg	al
			mov	ecx, eax
			lodsb
			rep	stosb

			dec	bl
			jne	short SameLine
EndLine:
			add	edx, deltax
			mov	edi, edx
			dec	word ptr [cptline]
			jne	short NewLine
EndUpdateFrame:
			ret
UpdateFrame		endp
;----------------------------------------------------------------------------
BlackFrame		proc	uses	edi

			mov	edi, Log
			xor	eax, eax
			mov	edx, 200
NextLine:
			mov	ecx, 320/4
			rep	stosd
			add	edi, 320
			dec	edx
			jne	NextLine
			ret
BlackFrame		endp
;----------------------------------------------------------------------------
CopyFrame		proc	uses esi edi,\
			ptsrc:DWORD

			mov	esi, ptsrc
			mov	edi, Log
			mov	edx, 200
NextLine:
			mov	ecx, 320/4
			rep	movsd
			add	edi, 320
			dec	edx
			jne	NextLine

			ret
CopyFrame		endp
;----------------------------------------------------------------------------

comment	#
SetPal			proc	uses esi edi ebx,\
			ptsrc:DWORD

			mov	esi, ptsrc
			xor	edi, edi

			lodsw
			mov	bx, ax
			test	bx, bx
			je	EndSetPal
Again:
			xor	eax, eax
			lodsb
			add	di, ax
			lodsb
			mov	ecx, eax
			or	ecx, ecx
			jne	PlusLoin
			mov	ecx, 256
PlusLoin:
			mov	dx, 3C8h
			mov	ax, di
			out	dx, al
			inc	di
			inc	dx
			lodsb
			out	dx, al
			lodsb
			out	dx, al
			lodsb
			out	dx, al
			loop	PlusLoin

			dec	bx
			jne	Again
EndSetPal:
			ret
SetPal			endp

;----------------------------------------------------------------------------
CpyPal			proc	uses	esi edi ebx,\
			ptsrc:DWORD, ptdst:DWORD

			mov	esi, ptsrc
			mov	edi, ptdst
			lodsw
			mov	bx, ax
			test 	bx, bx
			je	EndCpyPal
Again:
			xor	eax, eax
			lodsb
			add	di, ax
			add	di, ax
			add	di, ax
			lodsb
			or	al, al
			jne	PlusLoin
			mov     eax, 256
PlusLoin:
			mov	ecx, eax
			add	ecx, eax
			add	ecx, eax
mfp0:			lodsb
			shl	al, 2
			stosb
			loop	mfp0
;						rep	movsb
			dec	bx
			jne	Again
EndCpyPal:
			ret
CpyPal			endp

	#

;----------------------------------------------------------------------------
;				The
				End






