;----------------------------------------------------------------------------
;				 S_BLOCK.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386P
				jumps
				.model FLAT, SYSCALL

				.data

			include	f:\projet\lib386\lib_svga\svga.ash

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CopyBlock
			public	NoLanguage	SaveBlock
			public	NoLanguage	RestoreBlock

;----------------------------------------------------------------------------
;			CopyBlock( x0, y0, x1, y1, *src, xd, yd, *dst )
;
CopyBlock		proc	uses esi edi ebx ebp,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD, src:DWORD,\
			xd:DWORD, yd:DWORD, dst:DWORD

			mov	edx, x0
			mov	ecx, y0
			mov	ebx, x1
			mov	eax, y1
;----------------------

			mov	esi, TabOffLine[ecx*4]
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

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen

			mov	ebp, ebx
			inc	eax
			shr	ebx, 2
			and	ebp, 11b

			shr	eax, 1
			jnc	short odd
Again:
			mov	ecx, ebx
			rep	movsd
			mov	ecx, ebp
			rep	movsb

			add	esi, edx
			add	edi, edx
odd:
			mov	ecx, ebx
			rep	movsd
			mov	ecx, ebp
			rep	movsb

			add	esi, edx
			add	edi, edx

			dec	eax
			jnz	short Again

CopyBlock_End:		ret

CopyBlock		endp

comment	@

CopyBlock		proc	uses esi edi ebx,\
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

			mov	edx, Screen_X
			sub	edx, ebx		; DX Delta Screen
Again:
			mov	ecx, ebx
			shr	ecx, 2
			rep	movsd
			mov	ecx, ebx
			and	ecx, 11b
			rep	movsb

			add	esi, edx
			add	edi, edx
			dec	eax
			jne	short Again
CopyBlock_End:		ret
CopyBlock		endp

	@

;----------------------------------------------------------------------------
;			SaveBlock( *screen, *buffer, x0, y0, dx, dy )
;
;			DS:SI	*screen
;			ES:DI	*buffer
;			AX	x0
;			BX	y0
;			CX	dx
;			dx	dy
;

SaveBlock		proc	uses esi edi ebx ebp,\
			screen:DWORD, buffer:DWORD,\
			x0:DWORD, y0:DWORD, deltax:DWORD, deltay:DWORD

			mov	edi, buffer
			mov	eax, x0
			mov	ebx, y0
			mov	ecx, deltax
			mov	edx, deltay

			sub	ecx, eax
			sub	edx, ebx		; DX = DeltaY
			inc	ecx
			inc	edx
			mov	esi, TabOffLine[ebx*4]
			add	esi, eax
			add	esi, screen		; SI Offset Src

			mov	ebx, ecx		; BX = DeltaX

			mov	eax, Screen_X
			sub	eax, ebx		; bp Delta Screen

			mov	ebp, ebx
			inc	edx
			shr	ebx, 2
			and	ebp, 11b

			shr	edx, 1
			jnc	short odd
Again:
			mov	ecx, ebx		; Delta X
			rep	movsd
			mov	ecx, ebp
			rep	movsb

			add	esi, eax
odd:
			mov	ecx, ebx		; Delta X
			rep	movsd
			mov	ecx, ebp
			rep	movsb

			add	esi, eax

			dec	edx
			jne	Again

			ret
SaveBlock		endp

comment	@

SaveBlock		proc	uses esi edi ebx ebp,\
			screen:DWORD, buffer:DWORD,\
			x0:DWORD, y0:DWORD, deltax:DWORD, deltay:DWORD,\

			mov	edi, buffer
			mov	eax, x0
			mov	ebx, y0
			mov	ecx, deltax
			mov	edx, deltay

			sub	ecx, eax
			sub	edx, ebx		; DX = DeltaY
			inc	ecx
			inc	edx
			mov	esi, TabOffLine[ebx*4]
			add	esi, eax		; SI Offset Src
			add	esi, screen

			mov	ebx, ecx		; BX = DeltaX

			mov	ebp, Screen_X
			sub	ebp, ebx		; bp Delta Screen
Again:
			mov	ecx, ebx		; Delta X
			shr	ecx, 2
			rep	movsd
			mov	ecx, ebx
			and	ecx, 11b
			rep	movsb

			add	esi, ebp
			dec	edx
			jne	Again

			ret
SaveBlock		endp

	@

;----------------------------------------------------------------------------
;			RestoreBlock( *buffer, *screen, x0, y0, dx, dy )
;
;			DS:SI	*buffer
;			ES:DI	*screen
;			AX	x0
;			BX	y0
;			CX	x1
;			dx	y1
;			Already Clipped
;
RestoreBlock		proc	uses esi edi ebx ebp,\
			screen:DWORD, buffer:DWORD,\
			x0:DWORD, y0:DWORD, deltax:DWORD, deltay:DWORD

			mov	esi, buffer
			mov	eax, x0
			mov	ebx, y0
			mov	ecx, deltax
			mov	edx, deltay

			sub	ecx, eax
			sub	edx, ebx		; DX = DeltaY
			inc	ecx
			inc	edx
			mov	edi, TabOffLine[ebx*4]
			add	edi, eax		; SI Offset Src
			add	edi, screen
			mov	ebx, ecx		; BX = DeltaX

			mov	ebp, Screen_X
			sub	ebp, ebx		; bp Delta Screen
Again:
			mov	ecx, ebx		; Delta X
			shr	ecx, 2
			rep	movsd
			mov	ecx, ebx
			and	ecx, 11b
			rep	movsb

			add	edi, ebp
			dec	edx
			jne	Again

			ret
RestoreBlock		endp
;----------------------------------------------------------------------------
;				The
				End






