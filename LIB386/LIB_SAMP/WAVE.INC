;*--------------------------------------------------------------------------*

;DEBUG			equ	1

VOICE_HANDLE		equ	1234h

IFDEF   SB16

	SBLASTER        equ     1                       ; it is also a SBLASTER !

ENDIF

IFDEF	SBPRO

	SBLASTER        equ     1                       ; it is also a SBLASTER !
	STEREO		equ	1

ENDIF

IFDEF   SBLASTER1

	SBLASTER        equ     1                       ; it is also a SBLASTER !
	SINGLE_DMA      equ     1                       ; use DMA (no auto-reinit on the SB ver 1.00-1.99)

ELSEIFDEF SBLASTER

	AUTO_DMA        equ     1                       ; use auto_reinit DMA (faster and cleaner)

ENDIF

IFDEF	MWSS

	SAMPLE16BIT	equ	1
	STEREO		equ	1
	AUTO_DMA        equ     1                       ; use auto_reinit DMA (faster and cleaner)

ENDIF

IFDEF   GOLD

	SAMPLE16BIT	equ	1
	STEREO		equ	1
	SINGLE_DMA      equ     1                       ; use DMA (no counter on the card)

ENDIF

IFDEF   GUS

	AUTO_DMA        equ     1                       ; use auto_reinit DMA (faster and cleaner)
	SAMPLE16BIT	equ	1
	STEREO		equ	1
	NOIRQ		equ	1

ENDIF

IFDEF	PAS16

	PAS		equ	1
	SAMPLE16BIT	equ	1

ENDIF

IFDEF PAS

	STEREO		equ	1
	AUTO_DMA        equ     1                       ; use auto_reinit DMA (faster and cleaner)

	BI_OUTPUTMIXER  equ 00h				; PAS equates
	BI_L_PCM        equ 06h
	BI_R_PCM        equ 0dh
	INTRCTLRST      equ 0b89h
	AUDIOFILT       equ 0b8ah
	INTRCTLR        equ 0b8bh
	PCMDATA 	equ 0f88h
	CROSSCHANNEL    equ 0f8ah
	TMRCTLR 	equ 138bh
	SAMPLERATE	equ 1388h
	SAMPLECNT       equ 1389h
	SYSCONF		equ 8389h

ENDIF

;*--------------------------------------------------------------------------*

IFDEF	SAMPLE16BIT
	IFDEF	STEREO
		SLONG	equ	1
		SSIZE	equ	4
	ELSE
		SUWORD	equ	1
		SSIZE	equ	2
	ENDIF
ELSE
	IFDEF	STEREO
		SUWORD	equ	1
		SSIZE	equ	2
	ELSE
		SCHAR	equ	1
		SSIZE	equ	1
	ENDIF
ENDIF

;*--------------------------------------------------------------------------*

BUFFER_SIZE		equ	1024			; number of samples
							; in half-buffer
LIST_SIZE               equ     50
STRUCT_SIZE             equ     44
SNAP_SIZE		equ	8

SHIFT_SAMPLE		equ	3			; max number of
							; samples mixed

;*--------------------------------------------------------------------------*
