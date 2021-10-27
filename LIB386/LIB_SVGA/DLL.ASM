;----------------------------------------------------------------------------
;				INITSVGA.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------

				.386p
				jumps
				.model FLAT, SYSCALL

;----------------------------------------------------------------------------
				.code

		PUBLIC	driver_start

driver_start    dd 	OFFSET ListFuncs

IFDEF	REALTEK
		db	'Realtek RTG 3105'
ENDIF

IFDEF	OAK
		db	'OAK OTI 067/077'
ENDIF

IFDEF	NCR
		db	'NCR 77C22'
ENDIF

IFDEF	COMPAQ
		db	'Compaq QVision/AVGA'
ENDIF

IFDEF	TSENG
		db	'Tseng Lab ET 3000/4000'
ENDIF

IFDEF	PARADISE
		db	'Paradise/Western Digital'
ENDIF

IFDEF	GENOA
		db	'Genoa'
ENDIF

IFDEF	S3
		db	'S3'
ENDIF

IFDEF	CIRRUS
		db	'Cirrus Logic'
ENDIF

IFDEF	TRIDENT
		db	'Trident'
ENDIF

IFDEF	ATI
		db	'ATI'
ENDIF

		db 	' Super VGA Driver, Version 1.00,', 13, 10
		db 	'Copyright (c) Adeline Software International 1994, All Rights Reserved.', 13, 10, 10, 0


Scanline	textequ	<dword ptr[eax]>
BankSize	textequ	<dword ptr[eax+4]>
BankOver	textequ	<dword ptr[eax+8]>
NonStdVESA	textequ	<byte ptr[eax+12]>
BankShift	textequ	<byte ptr[eax+13]>


;*ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ*
;      €€ﬂﬂﬂ €ﬂﬂﬂﬂ €ﬂﬂﬂ€ €ﬂﬂﬂﬂ €ﬂﬂﬂﬂ €€‹ €       €‹ ‹€ €ﬂﬂﬂ€ €ﬂﬂﬂ‹ €ﬂﬂﬂﬂ
;      ﬂﬂﬂﬂ€ €€    €€ﬂ€ﬂ €€ﬂﬂ  €€ﬂﬂ  €€ﬂ€€       €€ﬂ € €€  € €€  € €€ﬂﬂ
;      ﬂﬂﬂﬂﬂ ﬂﬂﬂﬂﬂ ﬂﬂ  ﬂ ﬂﬂﬂﬂﬂ ﬂﬂﬂﬂﬂ ﬂﬂ  ﬂ ﬂﬂﬂﬂﬂ ﬂﬂ  ﬂ ﬂﬂﬂﬂﬂ ﬂﬂﬂﬂ  ﬂﬂﬂﬂﬂ
;*ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ*
;*ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ*

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	REALTEK

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc

			mov	dx, 3D7h
			out	dx, al

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 26h
			int	10h

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	OAK

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc

			mov	ah, al
			shl	al, 4
			or	ah, al
			mov	dx, 3DEh
			mov	al, 11h
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 53h
			int	10h

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	NCR

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc

			shl	ax, 10
			mov	al, 18h
			mov	dx, 03C4h
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 5Fh
			int	10h

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	COMPAQ

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc

			shl	ax, 12
			mov	al, 45h
			mov	dx, 03CEh
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ScanLine, 1024
			mov	NonStdVESA, 1

			mov	ax, 32h
			int	10h

			mov	dx, 03CEh
			mov	ax, 050Fh
			out	dx, ax		; unlock extended regs

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	TSENG

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc				; ET3000, Should work
							; on the ET4000 also
							; as long as we don't
							; read the screen...
			mov     dx, 03CDh
			or      al, 40h
			out     dx, al

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc				; ET3000/ET4000

			mov	ax, 2Eh			; Basculer Mode Video
			int	10h			; Tseng 640*480*256

			mov	dx, 3BFh		; Enable access to extended registers
			mov	al, 3			; for ET4000
			out	dx, al
			mov	dl, 0D8h
			mov	al, 0A0h
			out	dx, al

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	PARADISE

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc				; Western Digital /
							; Paradise

			mov     dx, 03CEh
			shl	ax, 12			; 4K banks
			mov     al, 09h
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 5Fh			; Basculer Mode Video
			int	10h			; WD/Paradise 640*480*256

			mov     dx, 03CEh
			mov     al, 0Fh
			out     dx, al
			inc     dx			; 3CFh
			in	al, dx
			and	al, 11101000b
			or      al, 00000101b
			out     dx, al

			dec	dx			; 3CEh
			mov     al, 0Bh
			out     dx, al
			inc     dx			; 3CFh
			in      al, dx
			and     al, 11110111b
			out     dx, al

			mov     dl, 0C4h		; 3C4h
			mov     al, 11h
			out     dx, al
			inc     dx			; 3C5h
			in      al, dx
			and     al, 01111111b
			out     dx, al

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	GENOA

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc				; Genoa (not old ET3000 model)

			mov     dx, 03C4h
			mov     ah, al
			shl     ah, 3
			or      ah, al
			or      ah, 40h
			mov	al, 06h
			out     dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc				; Genoa (not old ET3000 model)

			mov	ax, 5Ch			; Basculer Mode Video
			int	10h			; Genoa 640*480*256

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	S3

ListFuncs	dd	0
		dd	offset	NewBank
		dd	offset	Enable

;----------------------------------------------------------------------------
NewBank			proc

			mov     dx, 3d4h		; Switch banks
			mov	ah, al
			mov     al, 35h
			out     dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
Enable			proc

			mov     dx, 3d4h		; Enable extended registers
			mov     al, 38h
			out     dx, al
			inc     dx
			mov     al, 48h
			out     dx, al

			dec     dx			; Enable write to bank registers
			mov     al, 31h
			out     dx, al
			inc     dx
			mov     al, 89h
			in      al, dx
			mov	ah, al
			dec     dx
			mov     al, 31h
			out     dx, al
			inc     dx
			mov	al, ah
			or      al, 9
			out     dx, al

			ret

Enable			endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	CIRRUS

ListFuncs	dd	0
		dd	offset	NewBank
		dd	offset	Enable

;----------------------------------------------------------------------------
NewBank			proc

			mov     dx, 03CEh
			shl     ax, 12
			mov	al, 09h
			out     dx, ax
			dec	al
			out     dx, al

			ret
 
NewBank			endp

;----------------------------------------------------------------------------
Enable			proc

comment	@
			mov	dx, 03D4h		; 3D4h
			mov	al, 0Ch			; Screen A start addr Hi
			out	dx, al
			inc	dx			; 3D5h
			in	al, dx
			mov	cl, al			; cl = start addr
			xor	al, al
			out	dx, al			; write 0 to gain
							; access to Eagle ID

			dec	dx			; 3D4h
			mov	al, 1Fh			; Eagle ID reg
			out	dx, al
			inc	dx    			; 3D5h
			in	al, dx			; get Eagle ID
			mov	ah, al			; ah = Eagle ID

			mov     dl, 0C4h		; 3C4h
			mov     al, 06h			; extension register
			mov	ah, 12h			; should not be there
			out     dx, ax			; write Eagle ID to
							; enable extended regs
							; Used to be 12h...

			mov	dl, 0D4h		; 3D4h
			mov	al, 0Ch			; Screen A start addr Hi
			mov	ah, cl			; old start addr
			out	dx, ax			; write old value

	@
			mov	dx, 3c4h
			mov	ax, 1206h
			out	dx, ax

			ret

Enable			endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	TRIDENT

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

;----------------------------------------------------------------------------
NewBank			proc

			mov	dx, 3C4h
			xor	al, 2			; (strange chip design)
			mov	ah, al
			mov	al, 0Eh
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 5Dh			; Basculer Mode Video
			int	10h			; Trident 640*480*256

			mov	dx, 3CEh		; set pagesize to 64k
			mov	al, 6
			out	dx, al
			inc	dx
			in	al, dx
			or	al, 4
			mov	ah, al
			mov	al, 6
			dec	dx
			out	dx, ax

			mov	dl, 0C4h		; switch to BPS mode
			mov	al, 0Bh
			out	dx, al
			inc	dx
			in	al, dx

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	ATI

ListFuncs	dd	offset	InitMode
		dd	offset	NewBank
		dd	0

comment	@

;----------------------------------------------------------------------------
NewBank			proc

			shl	ax, 9

			mov	dx, 1CEh
			mov	al, 0B2h
			out	dx, al
			inc	dx
			in	al, dx
			and	al, 0E1h
			or	ah, al
			mov	al, 0B2h
			dec	dx
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 62h			; Basculer Mode Video
			int	10h			; ATI 640*480*256
			ret

InitMode		endp

;----------------------------------------------------------------------------

	@

NewBank			proc

			shl	ax, 9

extended_reg		equ	$+4
			mov	dx, 1CEh
			mov	al, 0B2h
			out	dx, al
			inc	dx
			in	al, dx
			and	al, 0E1h
			or	ah, al
			mov	al, 0B2h
			dec	dx
			out	dx, ax

			ret

NewBank			endp

;----------------------------------------------------------------------------
InitMode		proc

			mov	ax, 62h			; Basculer Mode Video
			int	10h			; ATI 640*480*256

			mov	ax, [0C0010h]
			mov	word ptr[extended_reg], ax

			ret

InitMode		endp

;----------------------------------------------------------------------------

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

			End
