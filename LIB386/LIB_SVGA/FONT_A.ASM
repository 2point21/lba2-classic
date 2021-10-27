;----------------------------------------------------------------------------
;
;'				 Text_A.ASM 386
;'			        (c) Adeline 1994
;'			   
;
;----------------------------------------------------------------------------
				.386P
				jumps
				.model SMALL, SYSCALL
;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------
			include	\projet\lib386\lib_svga\svga.ash

ALIGN 4

PtrFont		dd	0
DxFont		dd	?

InterLeave	dd	1
InterSpace	dd	10

;----------------------------------------------------------------------------
				.code

			public	NoLanguage	CoulFont
			public	NoLanguage	SetFont
			public	NoLanguage	SizeFont
			public	NoLanguage	CarFont
			public	NoLanguage	Font

			extrn	NoLanguage	CoulMask	:PROC
			extrn	NoLanguage	AffMask_Asm	:PROC

;----------------------------------------------------------------------------
;'
CoulFont		proc ,\
			coul:DWORD

			movzx	eax, byte ptr [coul]
			push	eax
			Call	CoulMask
			add	esp, 4
			ret

CoulFont		endp
;----------------------------------------------------------------------------
;'
SetFont			proc ,\
			ptfont:DWORD, inle:DWORD, insp:DWORD

			mov	eax, ptfont
			mov	[PtrFont], eax

			mov	eax, inle
			mov	[InterLeave], eax

			mov	eax, insp
			mov	[InterSpace], eax

			ret

SetFont			endp
;----------------------------------------------------------------------------
;'
SizeFont		proc	uses esi edi ,\
			chaine:DWORD

			mov	esi, [chaine]
			mov	edi, [PtrFont]
			xor	eax, eax
			xor	edx, edx
			mov	[DxFont], eax
;'			--------------------------------
NextCar:
			lodsb

			or	al, al
			je	short EndSizeFont

			cmp	al, 32
			jne	PlusLoin0

			mov	edx, [InterSpace]
			add	[DxFont], edx
			jmp	short NextCar

PlusLoin0:
			mov	edi, [PtrFont]
			add	edi, [edi+eax*4]


			mov	edx, [InterLeave]
			add	[DxFont], edx

			mov	dl, byte ptr[edi]

			add	[DxFont], edx

			jmp	short NextCar
;'			--------------------------------
EndSizeFont:
			mov	eax, [DxFont]
			ret
SizeFont		endp
;----------------------------------------------------------------------------
CarFont			proc	uses esi edi ebx ebp,\
			xcar:DWORD, ycar:DWORD, car:DWORD

			mov	eax, car
			cmp	eax, 32			;' SPACE
			je	Space

			mov	ebx, xcar
			mov	ecx, ycar
			mov	esi, [PtrFont]

			mov	edi, [PtrFont]
			add	edi, [edi+eax*4]

			xor	edx, edx
			mov	dl, [edi+0]
			add	edx, [InterLeave]

			push	edx

			Call	AffMask_Asm

			pop	eax

			ret
Space:
			mov	eax, [InterSpace]
			ret

CarFont			endp
;----------------------------------------------------------------------------
;'
Font			proc	uses esi edi ebx ebp,\
			xfont:DWORD, yfont:DWORD,chaine:DWORD

			cmp	dword ptr [PtrFont], 0
			je	short EndFont

;'			--------------------------------

			mov	esi, [chaine]
			mov	edi, [PtrFont]
			mov	ebx, [xfont]
			mov	ecx, [yfont]
			xor	eax, eax

			jmp	NextCar

;'			--------------------------------
Space:
			add	ebx, [InterSpace]
NextCar:
			xor	eax, eax
			lodsb

			or	al, al
			je	short EndFont

			cmp	al, 32
			je	Space

PasSpace:		mov	edi, [PtrFont]
			add	edi, [edi+eax*4]

			xor	edx, edx
			mov	dl, byte ptr[edi]
			mov	[DxFont], edx

			push	esi
			push	ebx
			push	ecx
			mov	esi, [PtrFont]

			Call	AffMask_Asm

			pop	ecx
			pop	ebx
			pop	esi

			add	ebx, [InterLeave]
			add	ebx, [DxFont]

			jmp	short NextCar
;'			--------------------------------
EndFont:
			ret
Font			endp
;----------------------------------------------------------------------------
;				The
				End



















