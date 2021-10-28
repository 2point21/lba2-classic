;*──────────────────────────────────────────────────────────────────────────*
;                               SAMP_A.ASM 386
;                             (c) Adeline 1993
;*──────────────────────────────────────────────────────────────────────────*

;*--------------------------------------------------------------------------*

		.386
		jumps

		.model  SMALL, SYSCALL

		.DATA

		PUBLIC	NoLanguage	listfcts

listfcts		dd	offset	Vesafcts

Vesafcts		dd	offset	SvgaInitVesa
			dd	offset	SvgaBankVesa

		.CODE

		PUBLIC	NoLanguage	SvgaInit
		PUBLIC	NoLanguage	SvgaBank

;----------------------------------------------------------------------------

RMI		equ	$
RMI_EDI		dd	0
RMI_ESI		dd	0
RMI_EBP		dd	0
RMI_RESERVED	dd	0
RMI_EBX		dd	0
RMI_EDX		dd	0
RMI_ECX		dd	0
RMI_EAX		dd	0
RMI_flags	dw	0
RMI_ES		dw	0
RMI_DS		dw	0
RMI_FS		dw	0
RMI_GS		dw	0
RMI_IP		dw	0
RMI_CS		dw	0
RMI_SP		dw	0
RMI_SS		dw	0

;----------------------------------------------------------------------------
NewBankVesa		proc

			push	ebx

Shift			equ	$+3
			shl	ax, 0
			mov	dx, ax
			mov	eax, 4F05h		; change bank VESA
			xor	ebx, ebx		; bh = 0, set bank
			int	10h

			pop	ebx

			ret

NewBankVesa		endp

;----------------------------------------------------------------------------
InitModeVesa		proc

			mov	eax, 0100h		; DMPI Allocate Dos Mem
			mov	ebx, 32			; 512 byte just in case
			int	31h			; return dos segment in ax
			push	edx			; save selector
			push	eax			; save real-mode segment

			push	es			; save es
			mov	RMI_ES, ax		; buffer segment
			xor	RMI_EDI, 0		; buffer offset 0
			mov	RMI_EAX, 4F01h		; Vesa information
			mov	RMI_ECX, 0101h		; mode 101h
			mov	ax, ds
			mov	es, ax			; es = ds (just in case)
			mov	edi, offset RMI		; registers
			mov	ax, 0300h		; DMPI generate real-mode int
			mov	bx, 10h			; int 10h
			int	31h
			pop	es			; restore es

			pop	eax			; restore real-mode segment
			and	eax, 0FFFFh		; only segment
			shl	eax, 4			; convert to linear
			mov	ax, [eax+4]		; get granularity
			bsr	ax, ax			; get higher bit position
			neg	al			; - position
			add	al, 6			; shift = 6 - position
			mov     byte ptr[Shift], al

			pop	edx			; restore selector
			mov	eax, 0101h		; DPMI return Dos memory
			int	31h

			mov	ax, 4F02h		; Switch to Video Mode
			mov	bx, 0101h		; VESA 640*480*256
			int	10h
			ret

InitModeVesa		endp

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

NewBank			PROC

			mov	eax, dword ptr[listfcts]
			jmp	dword ptr[eax]

NewBank			ENDP

;----------------------------------------------------------------------------

InitMode		PROC

			mov	eax, dword ptr[listfcts]
			jmp	dword ptr[eax+4]

InitMode		ENDP

;----------------------------------------------------------------------------

			END
