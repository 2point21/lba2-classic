;*──────────────────────────────────────────────────────────────────────────*
;                               SAMP_A.ASM 386
;                             (c) Adeline 1993
;*──────────────────────────────────────────────────────────────────────────*

;*--------------------------------------------------------------------------*

include	wave.inc

;*--------------------------------------------------------------------------*

MIDPOINT	MACRO

IFDEF	SAMPLE16BIT
	xor	eax, eax
ELSE
	mov	eax, 80808080h
ENDIF

		ENDM

;*--------------------------------------------------------------------------*

		.386
		jumps

		.model  FLAT, SYSCALL

		.CODE


;*--------------------------------------------------------------------------*

		PUBLIC	driver_start

driver_start    dd 	OFFSET ListFuncs

IFDEF		SB16
		db	'Sound Blaster 16 (DSP 4.XX)'

ELSEIFDEF	SBPRO
		db	'Sound Blaster Pro (DSP 3.XX)'

ELSEIFDEF	SBLASTER1
		db	'Sound Blaster 2 (DSP 2.01+)'

ELSEIFDEF	SBLASTER
		db	'Sound Blaster (DSP 1.00-2.00)'

ELSEIFDEF	MWSS
		db	'Microsoft Windows Sound System (AD1848 SoundPort)'

ELSEIFDEF	GOLD
		db	'Adlib Gold (Yamaha GOLD)'

ELSEIFDEF	GUS
		db	'Advanced Gravis UltraSound'

ELSEIFDEF	PAS16
		db	'Media Vision Pro Audio Spectrum 16'

ELSEIFDEF	PAS
		db	'Media Vision Pro Audio Spectrum/Spectrum Plus'
ENDIF

		db	' Wave Driver,', 13, 10

IFDEF	SAMPLE16BIT
		db	'16 bit'
ELSE
		db	'8 bit'
ENDIF
IFDEF	STEREO
		db	' Stereo'
ELSE
		db	' Mono'
ENDIF
IFDEF	SURROUND
		db	' Surround'
ENDIF

		db 	' playback, Version 1.00.', 13, 10
IFDEF	GUS
		db	'Copyright (C) 1991,1992 Miles Design, Inc.', 0Dh, 0Ah
		db	'Copyright (C) 1993,1994 Advanced Gravis Computer '
		db 	'Technology Ltd. All rights reserved.', 0Dh, 0Ah
		db	'Copyright (C) 1992,1993,1994 Forte Technologies.', 0Dh, 0Ah
		db	'Copyright (C) 1994 Adeline Software International.', 0Dh, 0Ah
		db	'All rights reserved.', 0Dh, 0Ah
		db	'UltraSound conversion by Jayeson Lee-Steere.', 0Dh, 0Ah
		db	'Adeline Software conversion by Serge Plagnol.', 0Dh, 0Ah
		db	'MIDI and Digital sound library by Forte.', 0Dh, 0Ah, 0Ah, 0
ELSE
		db	'Copyright (c) Adeline Software International 1994, All Rights Reserved.', 13, 10, 10, 0
ENDIF

ListFuncs	dd	offset	InitCard
		dd	offset	ClearCard

		dd	offset	AskVars

		dd	offset	MixSample
		dd	offset	GiveSampleInfo0

		dd	offset	StopSample
		dd	offset	StopOneSample

		dd	offset	SampleInList
		dd	offset	GetSnapSample

		dd	offset	PauseSample
		dd	offset	ContinueSample

		dd	offset	SaveStateSample
		dd	offset	RestoreStateSample

		dd	offset	GetDMAAddr

		dd	offset	GetBufferSize

		dd	offset	ChangeVolume

		dd	offset	ShiftSamples

		dd	offset	StopOneSampleLong

;*--------------------------------------------------------------------------*

ADDRESS                 equ     0
FRACT                   equ     4
LEFT                    equ     8
C_REPEAT                equ     12
INCR                    equ     14
SON                     equ     16
HANDLE                  equ     20
START                   equ     24
DLENGTH                 equ     28
VOL_LEFT		equ	32
VOL_RIGHT		equ	34
INFO0			equ	36
INTERPOL		equ	40
LAST_SAMPLE		equ	41

;*--------------------------------------------------------------------------*

DSP_VO8S_CMD            equ     14h
DSP_VO8_CMD             equ     1Ch
DSP_TIME_CMD            equ     40h
DSP_RATE_CMD            equ     41h
DSP_BSIZE_CMD           equ     48h
DSP_VO8H_CMD            equ     90h
DSP_VO16S_CMD           equ     0B0h
DSP_VO16_CMD            equ     0B6h
DSP_VO8S_4_CMD		equ	0C0h
DSP_VO8_4_CMD		equ	0C6h
DSP_ONSPK_CMD           equ     0D1h
DSP_OFFSPK_CMD          equ     0D3h

DSP_16MONO_MODE         equ     10h
DSP_16STEREO_MODE       equ     30h
DSP_8MONO_MODE		equ	00h
DSP_8STEREO_MODE	equ	20h

RESET_TEST_CODE         equ     0AAh


;*--------------------------------------------------------------------------*

ifndef	NOIRQ

VOICE_OUT               equ     0       ; equ to access the 2 arrays below
AUTO_OUT                equ     1
MASK1                   equ     2
MASK2                   equ     3

ADDX_REG                equ     4
COUNT_REG               equ     6
MASK_REG                equ     8
MODE_REG                equ     10
FF_REG                  equ     12
PAGE_REG                equ     14


DMA0                    equ     $       ; Ports for DMA0

DMA0_VOICE_OUT          db      48H
DMA0_AUTO_OUT           db      58H
DMA0_MASK1              db      00H
DMA0_MASK2              db      04H

DMA0_ADDX_REG           dw      00H
DMA0_COUNT_REG          dw      01H
DMA0_MASK_REG           dw      0AH
DMA0_MODE_REG           dw      0BH
DMA0_FF_REG             dw      0CH
DMA0_PAGE_REG           dw      87H


DMA1                    equ     $       ; Ports for DMA1

DMA1_VOICE_OUT          db      49H
DMA1_AUTO_OUT           db      59H
DMA1_MASK1              db      01H
DMA1_MASK2              db      05H

DMA1_ADDX_REG           dw      02H
DMA1_COUNT_REG          dw      03H
DMA1_MASK_REG           dw      0AH
DMA1_MODE_REG           dw      0BH
DMA1_FF_REG             dw      0CH
DMA1_PAGE_REG           dw      83H


DMA3                    equ     $       ; Ports for DMA3

DMA3_VOICE_OUT          db      4BH
DMA3_AUTO_OUT           db      5BH
DMA3_MASK1              db      03H
DMA3_MASK2              db      07H

DMA3_ADDX_REG           dw      06H
DMA3_COUNT_REG          dw      07H
DMA3_MASK_REG           dw      0AH
DMA3_MODE_REG           dw      0BH
DMA3_FF_REG             dw      0CH
DMA3_PAGE_REG           dw      82H


DMA5                    equ     $       ; Ports for DMA5

DMA5_VOICE_OUT          db      49H
DMA5_AUTO_OUT           db      59H
DMA5_MASK1              db      01H
DMA5_MASK2              db      05H

DMA5_ADDX_REG           dw      0C4H
DMA5_COUNT_REG          dw      0C6H
DMA5_MASK_REG           dw      0D4H
DMA5_MODE_REG           dw      0D6H
DMA5_FF_REG             dw      0D8H
DMA5_PAGE_REG           dw      08BH

DMA6                    equ     $       ; Ports for DMA6

DMA6_VOICE_OUT          db      4AH
DMA6_AUTO_OUT           db      5AH
DMA6_MASK1              db      02H
DMA6_MASK2              db      06H

DMA6_ADDX_REG           dw      0C8H
DMA6_COUNT_REG          dw      0CAH
DMA6_MASK_REG           dw      0D4H
DMA6_MODE_REG           dw      0D6H
DMA6_FF_REG             dw      0D8H
DMA6_PAGE_REG           dw      089H

DMA7                    equ     $       ; Ports for DMA7

DMA7_VOICE_OUT          db      4BH
DMA7_AUTO_OUT           db      5BH
DMA7_MASK1              db      03H
DMA7_MASK2              db      07H

DMA7_ADDX_REG           dw      0CCH
DMA7_COUNT_REG          dw      0CEH
DMA7_MASK_REG           dw      0D4H
DMA7_MODE_REG           dw      0D6H
DMA7_FF_REG             dw      0D8H
DMA7_PAGE_REG           dw      08AH

ALIGN	4

TAB_DMA			dd	offset DMA0
			dd	offset DMA1
			dd	0
			dd	offset DMA3
			dd	0
			dd	offset DMA5
			dd	offset DMA6
			dd	offset DMA7

endif

;*--------------------------------------------------------------------------*

CurrentList             db      STRUCT_SIZE*LIST_SIZE+4 dup (?)
SonList                 db      STRUCT_SIZE*LIST_SIZE+4 dup (?)

BackCurrentList         db      STRUCT_SIZE*LIST_SIZE+4 dup (?)
BackSonList             db      STRUCT_SIZE*LIST_SIZE+4 dup (?)

SnapList		db	SNAP_SIZE*LIST_SIZE dup (?)

IRQ_mask                db      ?

FlagPause               db      0

WaveBase		db	'WaveBase',0
WaveIRQ			db	'WaveIRQ',0
WaveDMA			db	'WaveDMA',0
WaveRate		db	'WaveRate'
Empty			db	0

ifdef	SBPRO

	OkIRQ		db	0
	Filter		db	?

endif

ifdef	GOLD

	Mixer_13	db	?
	Mixer_14	db	?
	Gold_IRQ	db	3, 4, 5, 7, 10, 11, 12, 15
endif

ifdef	SB16
	SB16_IRQ	db	2, 5, 7, 10
endif

EVEN

ifdef   MWSS

MWSSFreq		dw	11025, 3, 16000, 2, 18900, 5, 22050, 7
			dw	27429, 4, 32000, 6, 33075, 0dh, 37800, 9
			dw	44100, 0Bh

MWSSIrq			db	0, 0, 010000b, 0, 0, 0, 0, 001000b, 0, 010000b
			db	011000b, 100000b, 0, 0, 0, 0

MWSSDma			db	01b, 10b, 0, 11b, 0, 0, 0, 0

endif

Critical                dw      ?
DoUpdate                dw      ?

Old_RIRQ_Seg		dw	?
Old_PIRQ_Sel		dw	?
Old_18_Sel		dw	?
Old_RIRQ_Off		dw	?

ALIGN 4

Old_PIRQ_Off		dd	?
Old_18_Off		dd	?

ifdef	GOLD

	ListNames	dd	offset	WaveBase
			dd	offset	Empty

elseifdef SB16

	ListNames	dd	offset	WaveBase
			dd	offset	WaveRate
			dd	offset	Empty

elseifdef GUS

	ListNames	dd	offset	WaveRate
			dd	offset	Empty

elseifdef PAS

	ListNames	dd	offset	WaveRate
			dd	offset	Empty

else

	ListNames	dd	offset	WaveBase
			dd	offset	WaveRate
			dd	offset	WaveIRQ
			dd	offset	WaveDMA
			dd	offset	Empty

endif

public	Nolanguage	PlayRate

ListVars		equ	$

ifdef	GOLD

	BASE_ADDR       dd      388h            ; By default 388h
	PlayRate        dd      22000           ; By default 22 Khz
	IRQ_number      dd      ?               ; no default (read card)
	DMA_number	dd      ?               ; no default (read card)


elseifdef SBLASTER

	BASE_ADDR       dd      220h            ; By default 220h
	PlayRate        dd      22000           ; By default 22 Khz
	IRQ_number	dd      5               ; By default 5
	DMA_number	dd      1               ; By default 1

elseifdef MWSS

	BASE_ADDR       dd      540h            ; By default 540h
	PlayRate        dd      22000           ; By default 22 Khz
	IRQ_number	dd      9               ; By default 9
	DMA_number	dd      1               ; By default 1

elseifdef GUS

	PlayRate        dd      22000           ; By default 22 Khz

elseifdef PAS

	PlayRate        dd      22000           ; By default 22 Khz
	IRQ_number	dd      0               ; no default
	DMA_number	dd      0               ; no default

else

	BASE_ADDR       dd      0		; no default
	PlayRate        dd      22000           ; By default 22 Khz
	IRQ_number	dd      0               ; no default
	DMA_number	dd      0               ; no default

endif

INT_number              dd      0Dh		; By default 0Dh

public	Nolanguage	BufferHalf

BufferHalf              dd      ?

follow                  dd      -1

backfollow              dd      -1

ifndef	NOIRQ

DMA			dd      offset DMA1     ; By default DMA1

endif

weirdcount              dd      ?
backweirdcount          dd      ?

TheVolumeR		dd	?
TheVolumeL		dd	?

save_1                  dd      ?
save_2                  dd      ?
save_3                  dd      ?
save_4                  dd      ?


public	Nolanguage	BUFFER_DMA
public	Nolanguage	CURRENT_BUFFER


ifdef   RAM_CARD

	BUFFER_DMA              dd      ?       ; adress of the buffer

	R_BUFFER_CARD           dd      ?       ; adress of the buffer on the card for Right channel
	MID_R_BUFFER_CARD       dd      ?       ; adress of the mid buffer for Right channel

	CURRENT_R_BUFFER_CARD   dd      ?       ; current half for Right channel

	L_BUFFER_CARD           dd      ?       ; adress of the buffer on the card for Left channel
	MID_L_BUFFER_CARD       dd      ?       ; adress of the mid buffer for Left channel

	CURRENT_L_BUFFER_CARD   dd      ?       ; current half for Left channel
else

	BUFFER_DMA              dd      ?       ; adress of the buffer
	MID_BUFFER_DMA          dd      ?       ; adress of the mid buffer

	CURRENT_BUFFER          dd      ?       ; current half
endif

ifdef   INTERRRUPT

	DMA_COUNT               dd      ?
	DMA_PTR                 dd      ?

endif


;----------------------------------------------------------------------------

IFDEF PAS

	MV_filter       	dd	?
	MV_xchannel     	dd	?

	DPMI_real_int   LABEL BYTE		;DPMI real-mode interrupt structure

	int_DI			dw	?	;*** MUST REMAIN CONTIGUOUS ***
				dw	0
	int_SI          	dw	?
				dw	0
	int_BP          	dw	?
				dw	0
				dd	0
	int_BX          	dw	?
				dw	0
	int_DX          	dw	?
				dw	0
	int_CX          	dw	?
				dw	0
	int_AX          	dw	?
				dw	0
	int_flags       	dw	?
	int_ES          	dw	?
	int_DS          	dw	?
				dw	0
				dw	0
				dw	0
				dw	0
				dw	0
				dw	0

ENDIF

;*--------------------------------------------------------------------------*

Redirector	PROC

		int	18h
		iretd

Redirector	ENDP

;*--------------------------------------------------------------------------*

setalc		MACRO

		db	0D6h

		ENDM


lve		MACRO Reg, Exp

		lea	Reg, [Exp]

		ENDM


;*--------------------------------------------------------------------------*

GET_REAL_VECT	MACRO

		mov	ax, 0200h
		int	31h

		ENDM


SET_REAL_VECT	MACRO

		mov	ax, 0201h
		int	31h

		ENDM

GET_PROT_VECT	MACRO

		mov	ax, 0204h
		int	31h

		ENDM


SET_PROT_VECT	MACRO

		mov	ax, 0205h
		int	31h

		ENDM

CRIT_SECT       MACRO                           ; Enter Critical Section ;-)

		mov     word ptr[Critical], 1   ; tell the IRQ not to update the buffer
						; we'll take care of it if nescessary O:-)
		ENDM


END_CRIT_SECT   MACRO                           ; Exit Critical Section  O:-)
		local   NoUpdate                ; Update buffer if necessary

		; Dealing with the critical section flags................
		;                                       DO NOT TOUCH !!!!

		mov     word ptr[Critical], 0   ; exit crit. sect.
						; this way DoUpdate can't change to 1 anymore
		cmp     word ptr[DoUpdate], 0   ; IRQ happened ?
		je      NoUpdate                ; if DopUpdate change to 0 now, we are in deep
						; shit anyway because we missed one round!
						; The program won't crash but we'll ear the old
						; content of half a buffer and also miss half a buffer
						; of new data... :-(
		mov     word ptr[Critical], 1   ; yes, crit. again, so we don't update twice
						; because UpdateBuffer is not reentrant
						; and we don't want to take a chance on crashing!
		pushad                          ; if DoUpdate change to 1 HERE
						; it means again that we missed one round!
						; So we play the buffer we are updating! (funny noise :-( )
						; and then will ear half buffer of old data!
		call    UpdateBuffer            ; do the update
		popad
		mov     word ptr[Critical], 0   ; exit crit. sect
						; this way DoUpdate can't change to 1 anymore
		mov     word ptr[DoUpdate], 0   ; Update done, so DoUpdate = 0
NoUpDate:

						; ALL THOSE NASTY THING SHOULD NOT HAPPEN :-) !!!!
						; BECAUSE IT WOULD MEAN THAT THE UPDATE OF THE BUFFER
						; TAKES MORE THAN 46000 microsec!
						; (at 33 Mhz one cycle = .03 microsec so 4600 microsec = 1.5 million cycle)
		ENDM


;*--------------------------------------------------------------------------*

GetDMAAddr      PROC    USES EBX

		xor     eax, eax

ifndef NOIRQ

		mov     ebx, dword ptr[DMA]

		cli

		mov	dx, word ptr[ebx + PAGE_REG]
		in	al, dx
		shl	eax, 16

		mov     dx, word ptr[ebx + FF_REG]
		out     dx, al                          ; Flip-Flop

		mov     dx, word ptr[ebx + ADDX_REG]    ; to get offset

		in      al, dx
		mov     ah, al
		in      al, dx

		sti

		xchg    al, ah

		cmp	word ptr[DMA_number], 3
		jbe	noadjust
		lve	eax, eax*2

noadjust:
endif
		ret

GetDMAAddr      ENDP

;*--------------------------------------------------------------------------*

ifndef NOIRQ

NewIRQ          PROC

		pushad
		push    ds
		push    es
		cld                                     ; DAMN IT !! :-o

local_DS	equ	$+2
		mov	ax, 1234h
		mov     ds, ax				; restore DS
		mov     es, ax                          ; and ES !!!

		call    AckIrq
		pushf

ifdef   SINGLE_DMA
		jc	notransfert
		call    BlockTransfert
;		call	Ackirq				; security
notransfert:
endif
		mov     al, 20h                         ; allows for new int
		cmp	byte ptr[IRQ_number], 7
		jbe	short NoSecondCtrl
		out	0A0h, al
NoSecondCtrl:	out     20h, al

		popf
		jc	short FinIRQ			; not a DMA IRQ

		mov     eax, dword ptr[BufferHalf]
		xor     al, 4                           ; switch half
		mov     byte ptr[BufferHalf], al

ifdef   RAM_CARD
		mov     eax, dword ptr[R_BUFFER_CARD+eax]; get current card buffer address
		mov     dword ptr[CURRENT_R_BUFFER_CARD], eax; update CURRENT_BUFFER_CARD
else
		mov     eax, dword ptr[BUFFER_DMA+eax]  ; get current buffer address
		mov     dword ptr[CURRENT_BUFFER], eax  ; update CURRENT_BUFFER
endif

		mov     word ptr[DoUpdate], 1           ; Time to do the update

		cmp     word ptr[Critical], 0           ; crit. sect ?
		jne     FinIRQ                          ; yes, don't update yet!

		mov     word ptr[Critical], 1           ; just to make jure it case the interrupt
		call    UpdateBuffer                    ; happen again before its finished
		mov     word ptr[Critical], 0

		mov     word ptr[DoUpdate], 0           ; Update done
FinIRQ:
		pop     es
		pop     ds
		popad

		iretd

NewIRQ          ENDP

endif

;*--------------------------------------------------------------------------*

ifdef	GUS
		PUBLIC	Nolanguage	UpdateBuffer
		PUBLIC	Nolanguage	DoUpdate
		PUBLIC	Nolanguage	Critical
endif

;*--------------------------------------------------------------------------*

UpdateBuffer    PROC

ifdef	GUS
		push	es
		pushf

		cld
		mov	ax, ds
		mov	es, ax
endif

		mov     eax, BUFFER_SIZE
ifdef   RAM_CARD
		mov     edi, dword ptr[BUFFER_DMA]      ; Buffer
else
		mov     edi, dword ptr[CURRENT_BUFFER]  ; Buffer
endif
		cmp     byte ptr[FlagPause], 1
		je	short dopause

		lea     ebx, CurrentList                ; Point to List of samples

		mov     esi, dword ptr[ebx]             ; Current Source
		or      esi, esi                        ; if 0 then empty list
		jnz     process_1st
dopause:
		mov     ecx, (BUFFER_SIZE * SSIZE) / 4
		MIDPOINT
		rep     stosd

		mov	al, byte ptr[FlagPause]
		and	al, 1
		mov	byte ptr[FlagPause], al
ifdef	GUS
		popf
		pop	es
endif
		ret
exit:
		mov	al, byte ptr[FlagPause]
		dec	al
		js	reallyexit

ifdef   RAM_CARD
		mov     edi, dword ptr[BUFFER_DMA]      ; Buffer
else
		mov     edi, dword ptr[CURRENT_BUFFER]  ; Buffer
endif

		mov	esi, ( BUFFER_SIZE * SSIZE ) / 4
		bsr	cx, si

		dec	si				; no need to fade first/last sample
							; since it will be multiplied by 1
		dec	al
		jnz	short fadeout

		mov	ebx, 1				; skip 0

ifdef	SAMPLE16BIT

loopfadein:	mov	edx, dword ptr[edi]
		movsx	eax, dx
		imul	eax, ebx
		sar	eax, cl
		sar	edx, 16
		imul	edx, ebx
		sar	edx, cl
		shl	edx, 16
		mov	dx, ax
		mov	dword ptr[edi], edx
		add	edi, 4
		inc	bx
		dec	si
		jnz	short loopfadein

else

loopfadein:     mov	edx, dword ptr[edi]
		xor	edx, 80808080h                  ; convert to signed
	REPT	2
		movsx	eax, dl
		imul	eax, ebx
		sar	eax, cl
		mov	dl, al
		movsx	eax, dh
		imul	eax, ebx
		sar	eax, cl
		mov	dh, al
		rol	edx, 16
	ENDM
		xor	edx, 80808080h			; back to unsigned
		mov	dword ptr[edi], edx
		add	edi, 4
		inc	bx
		dec	si
		jnz	short loopfadein

endif
		xor	al, al
		jmp	short endfade

fadeout:
		add	edi, 4				; skip first

ifdef	SAMPLE16BIT

loopfadeout:	mov	edx, dword ptr[edi]
		movsx	eax, dx
		imul	eax, esi
		sar	eax, cl
		sar	edx, 16
		imul	edx, esi
		sar	edx, cl
		shl	edx, 16
		mov	dx, ax
		mov	dword ptr[edi], edx
		add	edi, 4
		dec	si
		jnz	short loopfadeout

else

loopfadeout:	mov	edx, dword ptr[edi]
		xor	edx, 80808080h                  ; convert to signed
	REPT	2
		movsx	eax, dl
		imul	eax, esi
		sar	eax, cl
		mov	dl, al
		movsx	eax, dh
		imul	eax, esi
		sar	eax, cl
		mov	dh, al
		rol	edx, 16
	ENDM
		xor	edx, 80808080h			; back to unsigned
		mov	dword ptr[edi], edx
		add	edi, 4
		dec	si
		jnz	short loopfadeout
endif
		mov	al, 1
endfade:
		mov	byte ptr[FlagPause], al
reallyexit:
ifdef	GUS
		popf
		pop	es
endif
		ret

FinishFill0:
		or      ax, ax                          ; still some room in the buffer?
		jz      Finish2
process_1st:
		mov     cx, ax
		shl     eax, 16
		mov     ax, cx                          ; eax = BUFFER_SIZE:BUFFER_SIZE
		mov     dx, word ptr[ebx+FRACT]         ; fractionnal Source
		mov     bp, word ptr[ebx+INCR]          ; fractionnal inc
		mov     ecx, dword ptr[ebx+LEFT]        ; length left

		cmp     ecx, 0FFFFh                     ; length left to fill after sample
		ja      Longer0                         ; 64K or +, then more than the buffer
		sub     ax, cx
		jae     short NotLonger0                ; some left?
Longer0:
		shr	eax, 16
		mov     ecx, eax			; length = buffer_size
		sub     dword ptr[ebx+LEFT], ecx        ; reduce length left
		mov	word ptr[save_4], 0		; no room left
		jmp     short StartMix0
NotLonger0:
		inc     ax                              ; length + 1 (in case left = 0)
		mov	word ptr[save_4], ax
StartMix0:
		shl	edx, 16
		or	ecx, edx			; hecx = FRACT
		shl	ebp, 16				; hebp = INCR

ifdef   SAMPLE16BIT

		mov	edx, dword ptr[ebx+VOL_LEFT]	; read left & right volume

	ifdef	STEREO

		cmp     byte ptr[ebx+INTERPOL], 0
		je	nofilter

		mov	dword ptr[save_3], ebx

		movsx	eax, dx
		mov	[TheVolumeL], eax
		sar	edx, 16
		mov	[TheVolumeR], edx

		shr	ebp, 16				; BP = fract inc
		mov	edx, ecx
		shr	edx, 16				; DX = fract

		rol	ecx, 8
		mov	cl, [ebx+LAST_SAMPLE]
		ror	ecx, 8

		jmp	short next0

start0:         rol	ecx, 8
		mov	cl, [esi]
		inc	esi
		ror	ecx, 8
next0:		mov	eax, ecx
		mov	al, [esi]			; read data
		rol	eax, 8
		xor     ax, 8080h			; 8 bit signed
		movsx	ebx, al
		movsx	eax, ah				; sign extension to 32 bit

		not	dx
		inc	edx
		imul	ebx, edx
		dec	edx
		not	dx
		imul	eax, edx
		add	ebx, eax

		mov	eax, ebx
		imul	ebx, [TheVolumeR]		; Right
		imul	eax, [TheVolumeL]		; Left
		sar	eax, 16
		mov	bx, ax

		mov     [edi], ebx			; store buffer content
		add     edi, 4				; next location
		add     dx, bp				; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next0			; if (not C) and (not Z) next
		jnz     short start0			; if some left, read a new one

		mov	ebx, dword ptr[save_3]

		pushf

		rol	ecx, 8
		mov     [ebx+LAST_SAMPLE], cl

		shl	edx, 16
		mov	ecx, edx
		inc	esi

		popf

		jmp	short end16
nofilter:
		mov	dword ptr[save_3], ebx

start01:        lodsb                                   ; read new data
		xor     al, 80h				; 8 bit signed
		movsx	eax, al				; sign extension to 32 bit
		mov	ebx, eax			; copy into ebx
		imul	ax, dx				; ax = left
		imul	ebx, edx
		mov	bx, ax
next01:         mov     dword ptr[edi], ebx             ; write data
		add     edi, 4                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next01                    ; if (not C) and (not Z) next
		jnz     short start01                   ; if some left, read a new one

		mov	ebx, dword ptr[save_3]
end16:
	else

start0:         lodsb                                   ; read new data
		xor     al, 80h				; 8 bit signed
		movsx	ax, al				; sign extension to 16 bit
		imul	ax, dx				; ax = "volumed" sample
next0:          mov     word ptr[edi], ax               ; write data
		add     edi, 2                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next0                     ; if (not C) and (not Z) next
		jnz     short start0                    ; if some left, read a new one

	endif
else

	ifdef	STEREO

		mov	dl, byte ptr[ebx+VOL_LEFT]
		or	dl, dl
		jz	short middle0
		dec	dl
		jz	short left0

		mov	al, 80h				; "0"  -> left
r_start0:       mov     ah, byte ptr[esi]               ; read new data -> right
		inc	esi
r_next0:        mov     word ptr[edi], ax               ; write data
		add     edi, 2                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short r_next0                   ; if (not C) and (not Z) next
		jnz     short r_start0                  ; if some left, read a new one
		jmp	short end0

left0:

		mov	ah, 80h				; "0"  -> right
l_start0:       lodsb                                   ; read new data -> left
l_next0:        mov     word ptr[edi], ax               ; write data
		add     edi, 2                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short l_next0                   ; if (not C) and (not Z) next
		jnz     short l_start0                  ; if some left, read a new one
		jmp	short end0

middle0:

m_start0:       lodsb                                   ; read new data -> left
		mov	ah, al				; data -> right
m_next0:        mov     word ptr[edi], ax               ; write data
		add     edi, 2                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short m_next0                   ; if (not C) and (not Z) next
		jnz     short m_start0                  ; if some left, read a new one

end0:
	else

start0:         lodsb                                   ; read new data
next0:          mov     byte ptr[edi], al               ; write data
		inc     edi                             ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next0                     ; if (not C) and (not Z) next
		jnz     short start0                    ; if some left, read a new one

	endif
endif

		jc      noadjust0                       ; was a new data going to be read?
		dec     esi                             ; no, last data will be read again
noadjust0:
		mov	ax, word ptr[save_4]		; left to copy
		or	ax, ax
		jz      Finish                          ; 0 normal end

		dec     ax                              ; readjust size

		dec     word ptr[ebx+C_REPEAT]          ; repeat again?
		jnz     Reset0				; yes, then reset

		mov     edx, dword ptr[ebx+SON]         ; no, got a son?
		or      edx, edx
		js      short NoSon0                    ; no, remove the sample

		lea     esi, SonList                    ; yes, then find it
KeepLooking0:
		cmp     dword ptr[esi], 0               ; the end ?
		je      short NoSon0                    ; yes, then no son
		cmp     edx, dword ptr[esi+HANDLE]      ; this one?
		je      short FoundSon0                 ; yes, found!
		add     esi, STRUCT_SIZE                ; no, keep looking
		jmp     short KeepLooking0
NoSon0:
		or	ax, ax
		jz	noclear

		xor	ecx, ecx
		mov     cx, ax                          ; clear rest of the buffer
		MIDPOINT
IFDEF	  SCHAR
		mov     edx, ecx
		and     ecx, 3                          ; up to 3 by byte
		rep     stosb
		mov     ecx, edx
		shr     ecx, 2                          ; the rest by dword
ELSEIFDEF SUWORD
		shr     ecx, 1
		jnc     short NoAdjustByOne
		stosw
NoAdjustByOne:
ENDIF
		rep     stosd
noclear:
		mov     esi, ebx                        ; point where it is
		mov     edi, ebx                        ; idem
		add     esi, STRUCT_SIZE                ; source one further

LoopRemove0:    cmp     dword ptr[esi], 0               ; end ?
		je      short EndRemove0                ; yes, exit
		mov     ecx, STRUCT_SIZE / 4            ; trasnfert one struct
		rep     movsd                           ; transfer
		jmp     short LoopRemove0
EndRemove0:
		mov     dword ptr[edi], 0               ; write 0 to mark the end
		jmp     NextSample
FoundSon0:
		mov     ebp, esi                        ; save esi
		mov     edx, edi                        ; save edi

		mov     edi, ebx                        ; transfert at the location of the father
		mov     ecx, STRUCT_SIZE / 4            ; count in dword
		rep     movsd                           ; transfert

		mov     esi, ebp                        ; remove son from SonList
		mov     edi, ebp                        ; so, point to it
		add     esi, STRUCT_SIZE                ; source one further
LoopSon0:       cmp     dword ptr[esi], 0
		je      short EndSon0
		mov     ecx, STRUCT_SIZE / 4
		rep     movsd                           ; transfer while not 0
		jmp     short LoopSon0
EndSon0:
		mov     dword ptr[edi], 0               ; write 0 to mark the end

		mov     edi, edx                        ; restore edi in the buffer
		mov     esi, dword ptr[ebx+START]       ; restore source, no need to write it
		jmp     FinishFill0
Reset0:
		mov     esi, dword ptr[ebx+START]       ; restore source, no need to write it
		mov     ebp, dword ptr[ebx+DLENGTH]     ; restore length
		mov     dword ptr[ebx+LEFT], ebp
		mov     word ptr[ebx+FRACT], 0
		jmp     FinishFill0

NextSample:
		mov     esi, dword ptr[ebx]             ; Current Source
		or      esi, esi                        ; if 0 then end of the list
		jz      exit

ifdef   RAM_CARD
		mov     edi, dword ptr[BUFFER_DMA]      ; Buffer
else
		mov     edi, dword ptr[CURRENT_BUFFER]  ; Buffer
endif
		mov     ax, BUFFER_SIZE
FinishFill:
		or      ax, ax                          ; still some room in the buffer?
		jz      Finish2

		mov     cx, ax
		shl     eax, 16
		mov     ax, cx                          ; eax = BUFFER_SIZE:BUFFER_SIZE
		mov     dx, word ptr[ebx+FRACT]         ; fractionnal Source
		mov     bp, word ptr[ebx+INCR]          ; fractionnal inc
		mov     ecx, dword ptr[ebx+LEFT]        ; length left

		cmp     ecx, 0FFFFh                     ; length left to fill after sample
		ja      Longer                          ; 64K or +, then more than the buffer
		sub     ax, cx
		jae     short NotLonger                 ; some left?
Longer:
		shr     eax, 16                         ; no, restore buffer_size in ax
		mov     ecx, eax                        ; length = buffer_size
		sub     dword ptr[ebx+LEFT], ecx        ; reduce length left
		mov	word ptr[save_4], 0		; no room left
		jmp     short startMix00
NotLonger:
		inc     ax                              ; length + 1 (in case left = 0)
		mov	word ptr[save_4], ax
StartMix00:
		shl	edx, 16
		or	ecx, edx			; hecx = FRACT
		shl	ebp, 16				; hebp = INCR

ifdef   SAMPLE16BIT

		mov	edx, dword ptr[ebx+VOL_LEFT]

	ifdef	STEREO

		cmp     byte ptr[ebx+INTERPOL], 0
		je	nofilter00

		mov	dword ptr[save_3], ebx

		movsx	eax, dx
		mov	[TheVolumeL], eax
		sar	edx, 16
		mov	[TheVolumeR], edx

		shr	ebp, 16				; BP = fract inc
		mov	edx, ecx
		shr	edx, 16				; DX = fract

		rol	ecx, 8
		mov	cl, [ebx+LAST_SAMPLE]
		ror	ecx, 8

		jmp	short next00

start00:        rol	ecx, 8
		mov	cl, [esi]
		inc	esi
		ror	ecx, 8
next00:		mov	eax, ecx
		mov	al, [esi]			; read data
		rol	eax, 8
		xor     ax, 8080h			; 8 bit signed
		movsx	ebx, al
		movsx	eax, ah				; sign extension to 32 bit

		not	dx
		inc	edx
		imul	ebx, edx
		dec	edx
		not	dx
		imul	eax, edx
		add	ebx, eax

		mov	eax, ebx
		imul	ebx, [TheVolumeR]		; Right
		imul	eax, [TheVolumeL]		; Left
		sar	eax, 16
		mov	bx, ax

		add     [edi], ebx			; store buffer content
		add     edi, 4				; next location
		add     dx, bp				; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next00			; if (not C) and (not Z) next
		jnz     short start00			; if some left, read a new one

		mov	ebx, dword ptr[save_3]

		pushf

		rol	ecx, 8
		mov     [ebx+LAST_SAMPLE], cl

		shl	edx, 16
		mov	ecx, edx
		inc	esi

		popf

		jmp	short end016
nofilter00:
		mov	dword ptr[save_3], ebx

start001:       lodsb                                   ; read new data
		xor     al, 80h				; 8 bit signed
		movsx	eax, al				; sign extension to 32 bit
		mov	ebx, eax			; copy into ebx
		imul	ax, dx				; ax = left
		imul	ebx, edx
		mov	bx, ax
next001:        add     dword ptr[edi], ebx             ; write data
		add     edi, 4                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next001                   ; if (not C) and (not Z) next
		jnz     short start001                  ; if some left, read a new one

		mov	ebx, dword ptr[save_3]
end016:

	else

start00:        lodsb                                   ; read new data
		xor     al, 80h				; 8 bit signed
		movsx	ax, al				; sign extension to 16 bit
		imul	ax, dx				; ax = "volumed" sample
next00:         add     word ptr[edi], ax               ; add with buffer content
		add     edi, 2				; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next00                    ; if (not C) and (not Z) next
		jnz     short start00                   ; if some left, read a new one

	endif
else

	ifdef	STEREO

		mov	dl, byte ptr[ebx+VOL_LEFT]
		or	dl, dl
		jz	short middle00
		dec	dl
		jz	short left00

		inc	edi				; go right
r_start00:      lodsb                                   ; read new data
		mov     ah, al                          ; save it in ah
r_next00:       add     al, byte ptr[edi]               ; add with buffer content
		rcr     al, 1                           ; average
		mov     byte ptr[edi], al               ; write result back
		add     edi, 2                          ; next location
		mov     al, ah                          ; restore data
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short r_next00                  ; if (not C) and (not Z) next
		jnz     short r_start00                 ; if some left, read a new one
		dec	edi				; go back left
		jmp	short end00

left00:

l_start00:      lodsb                                   ; read new data
		mov     ah, al                          ; save it in ah
l_next00:       add     al, byte ptr[edi]               ; add with buffer content
		rcr     al, 1                           ; average
		mov     byte ptr[edi], al               ; write result back
		add     edi, 2                          ; next location
		mov     al, ah                          ; restore data
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short l_next00                  ; if (not C) and (not Z) next
		jnz     short l_start00                 ; if some left, read a new one
		jmp	short end00

middle00:

m_start00:      lodsb                                   ; read new data
m_next00:	mov	dx, word ptr[edi]		; read buffer, left & right
		add	dl, al				; average with left
		rcr	dl, 1
		add	dh, al				; average with right
		rcr	dh, 1
		mov	word ptr[edi], dx		; write result back
		add     edi, 2                          ; next location
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short m_next00                  ; if (not C) and (not Z) next
		jnz     short m_start00                 ; if some left, read a new one

end00:

	else

start00:        lodsb                                   ; read new data
		mov     ah, al                          ; save it in ah
next00:         add     al, byte ptr[edi]               ; add with buffer content
		rcr     al, 1                           ; average
		adc	al, 0
		mov     byte ptr[edi], al               ; write result back
		inc     edi                             ; next location
		mov     al, ah                          ; restore data
		add     ecx, ebp                        ; update fractional part of address
		dec     cx                              ; length-1, doesn't touch C but set Z ;-)
		ja      short next00                    ; if (not C) and (not Z) next
		jnz     short start00                   ; if some left, read a new one

	endif
endif
		jc      noadjust                        ; was a new data going to be read?
		dec     esi                             ; no, last data will be read again
noadjust:
		mov	ax, word ptr[save_4]            ; left to copy
		or	ax, ax
		jz      short Finish                    ; 0 normal end

		dec     ax                              ; readjust size

		dec     word ptr[ebx+C_REPEAT]            ; repeat again?
		jnz     short Reset                     ; yes, then reset

		mov     edx, dword ptr[ebx+SON]         ; no, got a son?
		or      edx, edx
		js      short NoSon                     ; no, remove the sample

		lea     esi, SonList                    ; yes, then find it
KeepLooking:
		cmp     dword ptr[esi], 0               ; the end ?
		je      short NoSon                     ; yes, then no son
		cmp     edx, dword ptr[esi+HANDLE]      ; this one?
		je      short FoundSon                  ; yes, found!
		add     esi, STRUCT_SIZE                ; no, keep looking
		jmp     short KeepLooking
NoSon:                                                  ; remove it
		mov     esi, ebx                        ; point where it is
		mov     edi, ebx                        ; idem
		add     esi, STRUCT_SIZE                ; source one further

LoopRemove:     cmp     dword ptr[esi], 0               ; end ?
		je      short EndRemove                 ; yes, exit
		mov     ecx, STRUCT_SIZE / 4            ; trasnfert one struct
		rep     movsd                           ; transfer
		jmp     short LoopRemove
EndRemove:
		mov     dword ptr[edi], 0               ; write 0 to mark the end
		jmp     NextSample
FoundSon:
		mov     ebp, esi                        ; save esi
		mov     edx, edi                        ; save edi

		mov     edi, ebx                        ; transfert at the location of the father
		mov     ecx, STRUCT_SIZE / 4            ; count in dword
		rep     movsd                           ; transfert

		mov     esi, ebp                        ; remove son from SonList
		mov     edi, ebp                        ; so, point to it
		add     esi, STRUCT_SIZE                ; source one further
LoopSon:        cmp     dword ptr[esi], 0
		je      short EndSon
		mov     ecx, STRUCT_SIZE / 4
		rep     movsd                           ; transfer while not 0
		jmp     short LoopSon
EndSon:
		mov     dword ptr[edi], 0               ; write 0 to mark the end

		mov     edi, edx                        ; restore edi in the buffer
		mov     esi, dword ptr[ebx+START]       ; restore source, no need to write it
		jmp     FinishFill
Finish:
		shr	ecx, 16
		mov     word ptr[ebx+FRACT], cx         ; save fractional part
Finish2:	mov     dword ptr[ebx], esi             ; save current address
		add     ebx, STRUCT_SIZE                ; point to next sample
		jmp     NextSample
Reset:
		mov     esi, dword ptr[ebx+START]       ; restore source, no need to write it
		mov     ebp, dword ptr[ebx+DLENGTH]     ; restore length
		mov     dword ptr[ebx+LEFT], ebp
		mov     word ptr[ebx+FRACT], 0
		jmp     FinishFill

UpdateBuffer    ENDP

;*--------------------------------------------------------------------------*

StopSample      PROC

	CRIT_SECT

		xor     eax, eax

		mov     dword ptr[CurrentList], eax
		mov     dword ptr[SonList], eax
		mov     byte ptr[FlagPause], al
		dec     eax
		mov     dword ptr[follow], eax          ; -1

		mov     dword ptr[weirdcount], 10000h

	END_CRIT_SECT

		ret

StopSample      ENDP

;*--------------------------------------------------------------------------*

StopOneSample   PROC USES EBX ESI EDI,\
		thehandle:DWORD


		push    -1                              ; end of recursion
		mov     eax, thehandle
		lea     edx, CurrentList
		mov     ebx, edx

	CRIT_SECT

keeplooking:    cmp     dword ptr[ebx], 0
		je      short exit
		cmp     ax, word ptr[ebx+HANDLE]
		je      short found
		add     ebx, STRUCT_SIZE
		jmp     short keeplooking
exit:
		lea     ebx, SonList
		cmp     edx, ebx
		mov	edx, ebx
		jne     short keeplooking

		pop     eax                             ; get "son"
		or      eax, eax
		jns     short keeplooking               ; less than #2 billions samples !

	END_CRIT_SECT

		ret
found:
		mov     ecx, dword ptr[ebx+SON]
		or      ecx, ecx
		js      short noson

		push    ecx                             ; "son" to be removed
noson:
		mov     esi, ebx
		mov     edi, ebx
		add     esi, STRUCT_SIZE
LoopRemove:     cmp     dword ptr[esi], 0
		je      short EndRemove
		mov     ecx, STRUCT_SIZE / 4
		rep     movsd
		jmp     short LoopRemove
EndRemove:
		mov     dword ptr[edi], 0
		jmp     short keeplooking

StopOneSample   ENDP

;----------------------------------------------------------------------------

ShiftSamples	PROC	USES ESI EDI EBX,\
		DestAddr:DWORD, SrcAddr:DWORD, SizeByte:DWORD

		lea     edx, CurrentList

		mov	esi, SrcAddr		; Source
		mov	eax, esi		; save Source for comparison
		mov	edi, DestAddr		; Destination
		mov	ecx, SizeByte		; number of bytes to move

		mov	bl, cl			; compute counters
		shr	ecx, 2
		and	bl, 3

	CRIT_SECT

		rep	movsd			; move the data
		mov	cl, bl
		rep	movsb

		sub	edi, esi		; shift (delta addr)

		mov     ebx, edx

keeplooking:    cmp     dword ptr[ebx], 0
		je      short exit
		cmp     eax, [ebx+START]
		ja	short notfound
		add	[ebx+START], edi
		add	[ebx], edi
notfound:
		add     ebx, STRUCT_SIZE
		jmp     short keeplooking
exit:
		lea     ebx, SonList
		cmp     edx, ebx
		mov	edx, ebx
		jne     short keeplooking

	END_CRIT_SECT

		ret

ShiftSamples	ENDP

;----------------------------------------------------------------------------

StopOneSampleLong PROC	USES ESI EDI,\
		LongHandle:DWORD

		mov	eax, LongHandle

	CRIT_SECT

		call	SearchLongHandle
		or	edx, edx
		jz	short notfound

		mov	esi, edx
		mov	edi, edx
		add     esi, STRUCT_SIZE
LoopRemove:     cmp     dword ptr[esi], 0
		je      short EndRemove
		mov     ecx, STRUCT_SIZE / 4
		rep     movsd
		jmp     short LoopRemove
EndRemove:
		mov	dword ptr[edi], 0
notfound:
	END_CRIT_SECT

		ret

StopOneSampleLong ENDP

;*--------------------------------------------------------------------------*

SampleInList    PROC USES EBX,\
		thehandle:DWORD

	CRIT_SECT

		mov     edx, thehandle
		mov     eax, 1                          ; True

		lea     ecx, CurrentList
		mov     ebx, ecx

keeplooking:    cmp     dword ptr[ebx], 0
		je      short exit
		cmp     dx, word ptr[ebx+HANDLE]
		je      short found
		add     ebx, STRUCT_SIZE
		jmp     short keeplooking
exit:
		lea     ebx, SonList
		cmp     ecx, ebx
		mov     ecx, ebx
		jne     keeplooking

		xor     eax, eax                        ; False
found:
	END_CRIT_SECT

		ret

SampleInList    ENDP


;*--------------------------------------------------------------------------*

SearchLongHandle PROC

		lea     edx, CurrentList
		lea	ecx, SonList

keeplooking:    cmp     dword ptr[edx], 0
		je      short exit
		cmp     eax, dword ptr[edx+HANDLE]
		je      short found
		add     edx, STRUCT_SIZE
		jmp     short keeplooking
exit:
		cmp     edx, ecx
		mov	edx, ecx
		jne     keeplooking

		xor	edx, edx
found:
		ret

SearchLongHandle ENDP


;*--------------------------------------------------------------------------*

ChangeVolume	PROC	\
		LongHandle:DWORD, volleft:DWORD, volright:DWORD

ifdef	STEREO
	ifdef	SAMPLE16BIT

		mov	eax, LongHandle

	CRIT_SECT

		call	SearchLongHandle
		or	edx, edx
		jz	short notfound

		mov	eax, volleft
		shr	eax, SHIFT_SAMPLE - 1		; Preshift according to number of channels
		mov     word ptr[edx+VOL_LEFT], ax
		mov	eax, volright
		shr	eax, SHIFT_SAMPLE - 1		; Preshift according to number of channels
ifdef	SURROUND
		neg	eax
endif
		mov     word ptr[edx+VOL_RIGHT], ax

notfound:

	END_CRIT_SECT

	else

		mov	eax, LongHandle

	CRIT_SECT

		call	SearchLongHandle
		or	edx, edx
		jz	short notfound

		push	edi
		mov	edi, edx

		mov	cx, word ptr[volleft]
		or	cx, cx
		jnz	short okleft
		mov	cx, 2				; if volleft = 0 then right
		jmp	short storepos
okleft:		mov     ax, word ptr[volright]
		shl	ax, 7				; * 128
		xor	dx, dx
		div	cx
		mov	cx, 2				; right
		cmp	ax, 74				; .577  (= tan 30) * 128
		jb	short storepos
		dec	cx				; left
		cmp	ax, 222				; 1.732 (= tan 60) * 128
		jae	short storepos
		dec	cx				; middle
storepos:	mov	word ptr[edi+VOL_LEFT], cx	; Store position (0:middle, 1:left, 2:right)

		pop	edi
notfound:

	END_CRIT_SECT

	endif
else
	ifdef	SAMPLE16BIT

		mov	eax, LongHandle

	CRIT_SECT

		call	SearchLongHandle
		or	edx, edx
		jz	short notfound

		push	edi
		push	ebx
		mov	edi, edx

		mov	eax, volleft
		imul	eax, eax
		mov	ecx, volright
		imul	ecx, ecx
		add	eax, ecx
		call	sqr2
		shr	eax, SHIFT_SAMPLE - 1		; Preshift according to number of channels
		mov	word ptr[edi+VOL_LEFT], ax	; VOL_LEFT = sqr(volleft^2 + volright^2)

		pop	ebx
		pop	esi

notfound:

	END_CRIT_SECT

	endif
endif

		ret

ChangeVolume	ENDP

ifdef	SAMPLE16BIT
ifndef	STEREO

;*--------------------------------------------------------------------------*

;          EAX = Sqr(EAX)

Sqr2            PROC

		cmp	eax, 3			; if eax <= 3 then
		jbe	short asqr_0_1		; square root is 0 or 1

		xor     edx, edx		; clear edx
		mov	ebx, eax		; copy eax into ebx so that
						; edx:ebx are used "like" a 64 bit
						; register

		bsr	eax, ebx		; position of last 1 of ebx in eax

		mov	cl, 33			; compute how many left shift
		sub	cl, al			; are needed in order to have the
		and	cl, 11111110b		; most significant 2 bit pair at the
						; leftmost position of edx

		shld	edx, ebx, cl		; shift edx:ebx by the number
		shl	ebx, cl			; computed above

		mov	ecx, eax                ; compute in ecx how many pairs of
		shr	ecx, 1			; 2 bit are left to be processed

		mov     eax, 1			; eax = 1
		dec     edx			; edx = edx - 1

asqr_loop:      shld    edx, ebx, 2		; edx:ebx << 2
		shl     ebx, 2			;
		lve     eax, eax*2		; eax * 2
		cmp     edx, eax		; compare edx and eax
		jb      short asqr_neg		; if edx<eax then go asqr_neg

		inc	eax			; else eax = eax + 1
		sub     edx, eax		; edx = edx - eax
		jnc     short asqr_1		; if edx was greater than eax
						; then no pb and go asqr_1
		add     edx, eax		; else undo, edx = edx + eax
asqr_neg:	shr	eax, 1			; shift eax right by one
		dec	ecx			; one less pair to process
		jnz     short asqr_loop		; if there is some left then go asqr_loop
		ret				; return to caller

asqr_1:         inc	eax			; eax = eax + 1
		shr	eax, 1			; eax >> 1
		dec	ecx			; one less pair to process
		jnz     short asqr_loop		; if there is some left then go asqr_loop
		ret				; return to caller

asqr_0_1:	or	eax, eax		; if eax = 0
		jz	short asqr_00		; then return to caller
		mov	eax, 1			; else eax = 1
asqr_00:	ret				; return to caller

Sqr2            ENDP

endif
endif

;*--------------------------------------------------------------------------*

MixSample       PROC USES ESI EDI EBX,\
		thehandle:DWORD, pitchbend:DWORD, therepeat:DWORD,\
		plug:DWORD, volleft:DWORD, volright:DWORD,\
		buffer:DWORD; buffer with .VOC

		mov     esi, buffer

		xor	eax, eax
		mov	al, [esi]
		inc	al
		cmp	al, 10
		jb	okfilter

		xor	eax, eax

okfilter:	push	eax

		movzx   eax, word ptr[esi+14h]
		add     esi, eax                        ; skip header

		cmp     byte ptr[esi], 1                ; only 1 bloc type allowed
		jne     typeunknown

		cmp     byte ptr[esi + 5], 0            ; pack method
		jne     typeunknown

	; This crap should be removed by using a file format containing the
	; real sampling frequency instead of that stupid sr number in .VOC !
	; :-(

		mov     dx, 0Fh
		mov     ax, 4240h                       ; dx:ax = 1000000

		xor     ebx, ebx
		mov     bl, byte ptr[esi + 4]           ; bl = sr
		neg     bl                              ; bl = 256 - sr

		div     bx

		shl     dx, 1
		adc     ax, 0                           ; round to nearest

		mov     bx, ax

	; ebx = real sampling rate
	; up to here ! and then avoid this ^ div !


		mov     eax, dword ptr[esi + 1]         ; size sample
		and     eax, 0FFFFFFh                   ; 24 significant bits
		sub     eax, 2                          ; -2 header


		add     esi, 6                          ; esi->data
		mov     dword ptr[save_1], esi          ; save it

		mov     edx, pitchbend
		shl     edx, 4

		imul    ebx, edx
		shr     ebx, 16
		adc     bx, 0                           ; ebx = scaled sampling rate

		xor     edx, edx
		xchg    eax, ebx                        ; edx = 0, eax = sr, ebx = size
		xchg    edx, eax                        ; edx = sr, eax = 0, ebx = size
		div     word ptr[PlayRate]              ; dx:ax / PlayRate; ax = INCR

		cmp	eax, 0FFFFh
		jbe	okincr
		mov	eax, 0FFFFh
okincr:		mov     dword ptr[save_2], eax          ; save_2 = INCR

		mov     ecx, ebx                        ; ecx = size
		mov     esi, ebx
		shr     esi, 16                         ; esi = hi(size)

		mov     edx, esi
		xchg    eax, ebx                        ; eax = size, ebx = INCR

		dec     eax                             ; A TRY !!!! (will make loop around next longer...)

		shl     eax, 16
		div     ebx                             ; div 64 bits !!!
		sub     eax, esi                        ; sub max roundoff error
		mov     esi, eax                        ; esi = eax = new size

		mul     ebx                             ; remultiply
		mov     bx, ax                          ; FRACT in bx
		shrd    eax, edx, 16                    ; size in eax
		mov     dx, word ptr[save_2]            ; INCR in dx

		sub     ecx, eax                        ; how much am I off?
		jbe     typeunknown                     ; security, should not happen!

next:           inc     esi                             ; length + 1
		add     bx, dx                          ; update fractional part of address
		jnc     short next                      ; if (not C) next
		dec     cx
		jnz     short next                      ; not right yet, loop

		mov     eax, dword ptr[follow]
		lea     ebx, CurrentList
		mov     edi, ebx

		mov     cx, word ptr[thehandle]

IFNDEF SAMPLE16BIT

		cmp	cx, VOICE_HANDLE
		jne	ok_handle

		push	eax
		push	ecx
		push	edx
		call	StopSample
		pop	edx
		pop	ecx
		pop	eax
ok_handle:

ENDIF
		or      ecx, dword ptr[weirdcount]      ; why not ?
		inc     word ptr[weirdcount+2]

	CRIT_SECT

		cmp     plug, 0
		je      NoPlug

		lea     edi, SonList

keeplooking0:   cmp     dword ptr[ebx], 0
		je      short SearchSonList
		cmp     eax, dword ptr[ebx+HANDLE]      ; look for the "father" sample
		je      short found
		add     ebx, STRUCT_SIZE
		jmp     short keeplooking0
SearchSonList:
		lea     ebx, SonList
keeplooking1:   cmp     dword ptr[ebx], 0
		je      short Exit
		cmp     eax, dword ptr[ebx+HANDLE]      ; look for the "father" sample
		je      short found
		add     ebx, STRUCT_SIZE
		jmp     short keeplooking1
exit:
		xor	eax, eax
		jmp	endcritical
found:
		mov     dword ptr[ebx+SON], ecx         ; connect it to ist "son"
NoPlug:
		lea	ebx, [edi+STRUCT_SIZE*LIST_SIZE]

		xor     eax, eax
		sub	edi, STRUCT_SIZE

SearchEnd:	add	edi, STRUCT_SIZE
		cmp	eax, dword ptr[edi]
		jne	short SearchEnd

		cmp	edi, ebx
		jae	typeunknown

		mov     dword ptr[follow], ecx          ; update follow
		mov     dword ptr[edi+HANDLE], ecx      ; sample handle
		mov     cx, word ptr[therepeat]
		mov     word ptr[edi+C_REPEAT], cx        ; Repeat
		mov     dword ptr[edi+LEFT], esi        ; length
		mov     dword ptr[edi+DLENGTH], esi     ; length
		mov     eax, dword ptr[save_1]
		mov     dword ptr[edi], eax             ; Start
		mov     dword ptr[edi+START], eax       ; Start
		mov     eax, dword ptr[save_2]
		mov     word ptr[edi+INCR], ax          ; increment
		mov     dword ptr[edi+FRACT], 10000h    ; fractional (with 1 above so != 0)
		mov     dword ptr[edi+SON], -1          ; son
		mov     dword ptr[edi+INFO0], -1	; info0
		pop	eax
		mov	[edi+INTERPOL], al		; filter on/off
		mov	byte ptr[edi+LAST_SAMPLE], 80h	; 80h ( 0 )
ifdef	STEREO
	ifdef	SAMPLE16BIT
		mov	eax, volleft
		shr	eax, SHIFT_SAMPLE - 1		; Preshift according to number of channels
		mov     word ptr[edi+VOL_LEFT], ax
		mov	eax, volright
		shr	eax, SHIFT_SAMPLE - 1		; Preshift according to number of channels

			ifdef	SURROUND
				neg	eax
			endif

		mov     word ptr[edi+VOL_RIGHT], ax
	else
		mov	si, 2				; right
		mov	ecx, volleft
		or	ecx, ecx
		jz	short storepos
okleft:		mov     eax, volright
		shl	eax, 7				; * 128
		xor	edx, edx
		div	ecx
		cmp	eax, 74				; 1.732 (= tan 60) * 128
		jb	short storepos
		dec	si				; left
		cmp	eax, 222			; .577  (= tan 30) * 128
		jae	short storepos
		dec	si				; middle
storepos:	mov	word ptr[edi+VOL_LEFT], si	; Store position (0:middle, 1:left, 2:right)
		mov	word ptr[edi+VOL_RIGHT], 1	; to fill with non 0
	endif
else
	ifdef	SAMPLE16BIT
		mov	eax, volleft
		imul	eax, eax
		mov	ecx, volright
		imul	ecx, ecx
		add	eax, ecx
		call	sqr2
		shr	eax, SHIFT_SAMPLE - 1		; Preshift according to number of channels
		mov	word ptr[edi+VOL_LEFT], ax	; VOL_LEFT = sqr(volleft^2 + volright^2)
		mov	word ptr[edi+VOL_RIGHT], 1	; to fill with non 0
	else
		mov	dword ptr[edi+VOL_LEFT], 1	; to fill with non 0
	endif
endif
		mov     dword ptr[edi+STRUCT_SIZE], 0   ; mark end of List

		mov	eax, dword ptr[follow]
endcritical:
	END_CRIT_SECT

		ret
typeunknown:
		pop	eax
		xor	eax, eax
		ret

MixSample       ENDP

;*--------------------------------------------------------------------------*

ifdef   SINGLE_DMA

BlockTransfert  PROC

		push    ebx

		mov	ecx, BUFFER_SIZE * SSIZE
		cmp     word ptr[DMA_number], 3
		jbe     noLengthAdj
		shr     ecx, 1
NoLengthAdj:
		dec     ecx                             ; buffer size - 1

		mov     ebx, dword ptr[DMA]             ; Point to DMAx

		mov     dx, word ptr[ebx + MASK_REG]
		mov     al, byte ptr[ebx + MASK2]       ; mask channel
		out     dx, al

		mov     dx, word ptr[ebx + FF_REG]
		out     dx, al                          ; flip-flop


		mov     dx, word ptr[ebx + COUNT_REG]
		mov     al, cl                          ; buffer size - 1
		out     dx, al
		mov     al, ch
		out     dx, al

		mov     dx, word ptr[ebx + ADDX_REG]
		mov     eax, dword ptr[BufferHalf]
		mov     eax, dword ptr[BUFFER_DMA+eax]  ; start offset
		cmp     word ptr[DMA_number], 3
		jbe     noAddrAdj
		shr     eax, 1
NoAddrAdj:
		out     dx, al
		shr     eax, 8
		out     dx, al

		mov     dx, word ptr[ebx + PAGE_REG]
		shr     eax, 8                          ; page of DMA transfert
		out     dx, al

		mov     dx, word ptr[ebx + MODE_REG]
		mov     al, byte ptr[ebx + VOICE_OUT]   ; output sample
		out     dx, al

		mov     dx, word ptr[ebx + MASK_REG]
		mov     al, byte ptr[ebx + MASK1]       ; channel OK
		out     dx, al

		pop     ebx

		jmp	StartDMACard

BlockTransfert  ENDP

endif

;*--------------------------------------------------------------------------*

PauseSample     PROC

		xor	eax, eax

	CRIT_SECT

		mov	al, byte ptr[FlagPause]
		dec	al
		and	al, 10b
		or	al, 01b				; 0->3, 1->1, 2->1, 3->3
		mov     byte ptr[FlagPause], al
		shr	eax, 1
		xor	al, 1

	END_CRIT_SECT

		ret

PauseSample     ENDP

;*--------------------------------------------------------------------------*

ContinueSample  PROC

		xor	eax, eax

	CRIT_SECT

		mov	al, byte ptr[FlagPause]
		inc	al
		and	al, 10b				; 0->0, 1->2, 2->2, 3->0
		mov     byte ptr[FlagPause], al
		shr	eax, 1
		xor	al, 1

	END_CRIT_SECT

		ret

ContinueSample  ENDP

;----------------------------------------------------------------------------

ifndef	NOIRQ

InstallISR	PROC

		push	edx

		mov	bl, byte ptr[INT_number]	; plug ISR on user INT
		mov	cx, cs
		SET_PROT_VECT

		pop	edx

		cmp	dword ptr[IRQ_number], 7	; if IRQ > 7
		jbe	nobi

		mov	bl, 18h				; plug as well on int 18h
		mov	cx, cs
		SET_PROT_VECT

		mov	bl, byte ptr[INT_number]	; plug into real usr INT
		mov	ecx, offset Redirector
		mov	dx, cx				; compute real-mode
		xor	cx, cx				; seg:ofs
		shr	ecx, 4
		SET_REAL_VECT
nobi:
		ret

InstallISR	ENDP

endif

;----------------------------------------------------------------------------

InitCard        PROC    USES EBX EDI ESI EBP,\
		Buffer:DWORD

ifndef	NOIRQ
		mov	word ptr[local_DS], ds
endif
		mov	eax, Buffer
		mov	dword ptr[BUFFER_DMA], eax

ifndef	SBPRO
		call    ResetCard
endif

ifndef	NOIRQ

		xor	eax, eax
		mov	ax, word ptr[DMA_number]
		cmp	ax, 7
		ja	short ErrorDMA
		mov	eax, dword ptr[TAB_DMA+eax*4]
		or	eax, eax
		jnz	short DMAFound
ErrorDMA:
		xor	eax, eax
		ret
DMAFound:
		mov	dword ptr[DMA], eax

		mov	eax, dword ptr[IRQ_number]
		cmp	al, 7
		ja	short Second
		add	al, 8
		jmp	short gotvect
Second:
		add	al, 70h - 8
gotvect:
		mov	dword ptr[INT_number], eax	; save user int

		mov	bl, al
		GET_PROT_VECT
		mov	word ptr[Old_PIRQ_Sel], cx
		mov	dword ptr[Old_PIRQ_Off], edx

		cmp	dword ptr[IRQ_number], 7
		jbe	nobi

		mov	bl, byte ptr[INT_number]	; if IRQ>7 save real vect
		GET_REAL_VECT
		mov	word ptr[Old_RIRQ_Seg], cx
		mov	word ptr[Old_RIRQ_Off], dx

		mov	bl, 18h				; if IRQ>7 save int 18h
		GET_PROT_VECT
		mov	word ptr[Old_18_Sel], cx
		mov	dword ptr[Old_18_Off], edx
nobi:

		mov     dx, 21h                         ; IRQ mask reg
		mov     cx, word ptr[IRQ_number]
		cmp	cx, 7
		jbe	short Ok21
		mov	dx, 0A1h			; reg A1h, 2nd ctrl
Ok21:		in      al, dx
		mov     byte ptr[IRQ_mask], al          ; save mask
		and	cl, 7
		mov     bl, 1
		shl     bl, cl
		not     bl
		and     al, bl                          ; unmask IRQ
		out     dx, al                          ; write new mask

ifdef	SBPRO
		call    ResetCard
endif

		mov	edx, offset NewIRQ
		call	InstallISR
endif
		call    StopSample                      ; to reset everything

		xor	eax, eax
		mov	dword ptr[BackCurrentList], eax
		mov	dword ptr[BackSonList], eax

		mov     edi, dword ptr[BUFFER_DMA]

		mov     eax, edi
		add	eax, BUFFER_SIZE * SSIZE
		mov     dword ptr[MID_BUFFER_DMA], eax  ; init pointer
		mov     dword ptr[CURRENT_BUFFER], eax  ; init pointer

		mov     dword ptr[BufferHalf], 4        ; point on second half

		mov	ecx, (BUFFER_SIZE * 2 * SSIZE) / 4; clear all buffer
		MIDPOINT
		rep     stosd

ifndef	NOIRQ

ifdef   AUTO_DMA

		mov	ecx, BUFFER_SIZE * 2 * SSIZE
		cmp     word ptr[DMA_number], 3
		jbe     noLengthAdj
		shr     ecx, 1
NoLengthAdj:
		dec     ecx

		mov     ebx, dword ptr[DMA]           ; Point to DMAx

		mov     dx, word ptr[ebx + MASK_REG]
		mov     al, byte ptr[ebx + MASK2]       ; mask channel
		out     dx, al

		mov     dx, word ptr[ebx + FF_REG]
		out     dx, al                          ; flip-flop

		mov     dx, word ptr[ebx + COUNT_REG]
		mov     al, cl                          ; buffer size - 1
		out     dx, al
		mov     al, ch
		out     dx, al

		mov     dx, word ptr[ebx + ADDX_REG]
		mov     eax, dword ptr[BUFFER_DMA]      ; start offset
		cmp     word ptr[DMA_number], 3
		jbe     noAddrAdj
		shr	eax, 1
		mov	di, ax
		xor	ax, ax
		shl	eax, 1
		mov	ax, di
NoAddrAdj:
		out     dx, al
		shr     eax, 8
		out     dx, al

		mov     dx, word ptr[ebx + PAGE_REG]
		shr     eax, 8                          ; page of DMA transfert
		out     dx, al

		mov     dx, word ptr[ebx + MODE_REG]
		mov     al, byte ptr[ebx + AUTO_OUT]    ; output sample
		out     dx, al

		mov     dx, word ptr[ebx + MASK_REG]
		mov     al, byte ptr[ebx + MASK1]       ; channel OK
		out     dx, al
endif

endif

ifdef   AUTO_DMA
		call    StartDMACard
elseifdef SINGLE_DMA
		call    BlockTransfert                  ; Start DMA transfert
endif
		mov	eax, 1
		ret

InitCard        ENDP

;----------------------------------------------------------------------------

ClearCard       PROC	USES EBX ESI EDI EBP

		call    StopSample                      ; just in case something was playing...
		call    CloseCard

ifndef	NOIRQ
		mov     dx, 21h                         ; read IRQ mask
		cmp     word ptr[IRQ_number], 7
		jbe	short Ok21
		add	dx, 80h				; reg 0A1h
Ok21:		mov     al, byte ptr[IRQ_mask]          ; get old mask
		out     dx, al                          ; write old IRQ mask

		mov	bl, byte ptr[INT_number]
		mov	cx, word ptr[Old_PIRQ_sel]
		mov	edx, dword ptr[Old_PIRQ_off]
		SET_PROT_VECT

		cmp	dword ptr[IRQ_number], 7
		jbe	nobi

		mov	bl, byte ptr[INT_number]
		mov	cx, word ptr[Old_RIRQ_seg]
		mov	dx, word ptr[Old_RIRQ_off]
		SET_REAL_VECT

		mov	bl, 18h
		mov	cx, word ptr[Old_18_sel]
		mov	edx, dword ptr[Old_18_off]
		SET_PROT_VECT
nobi:

endif

		ret

ClearCard       ENDP

;----------------------------------------------------------------------------

AskVars		PROC	,\
		pListNames:DWORD, pListVars:DWORD

		mov	eax, offset ListNames
		mov	ecx, pListNames
		mov	dword ptr[ecx], eax
		mov	eax, offset ListVars
		mov	ecx, pListVars
		mov	dword ptr[ecx], eax
		ret

AskVars		ENDP


;----------------------------------------------------------------------------

GetBufferSize	PROC

		mov	eax, BUFFER_SIZE * 2 * SSIZE

		ret

GetBufferSize	ENDP

;----------------------------------------------------------------------------

GiveSampleInfo0	PROC	\
		LongHandle:DWORD, Info:DWORD

		mov	eax, LongHandle

	CRIT_SECT

		call	SearchLongHandle
		or	edx, edx
		jz	short notfound

		mov	eax, Info
		mov	dword ptr[edx+INFO0], eax
notfound:
	END_CRIT_SECT

		ret

GiveSampleInfo0	ENDP

;----------------------------------------------------------------------------

GetSnapSample	PROC	USES EBX,\
		pList:DWORD

		lea	eax, SnapList
		mov	edx, pList
		mov	dword ptr[edx], eax

		lea	edx, CurrentList
		lea	ecx, SonList

	CRIT_SECT

keeplooking:    cmp     dword ptr[edx], 0
		je      short exit

		mov	ebx, dword ptr[edx+HANDLE]
		mov	dword ptr[eax], ebx
		mov	ebx, dword ptr[edx+INFO0]
		mov	dword ptr[eax+4], ebx
		mov	ebx, dword ptr[edx]
		sub	ebx, dword ptr[edx+START]
		mov	dword ptr[eax+8], ebx

		add	eax, 12
		add     edx, STRUCT_SIZE
		jmp     short keeplooking
exit:
		cmp     ecx, edx
		mov	edx, ecx
		jne     keeplooking

	END_CRIT_SECT

		sub	eax, offset SnapList
		shr	eax, 3

		ret

GetSnapSample	ENDP


;----------------------------------------------------------------------------

CopyList	PROC

keepcopying:	cmp	dword ptr[esi], 0
		je	short endcopy
		mov	ecx, STRUCT_SIZE / 4
		rep	movsd
		jmp	short keepcopying
endcopy:	mov	dword ptr[edi], 0
		ret

CopyList	ENDP

;----------------------------------------------------------------------------

SAveStateSample	PROC	USES ESI EDI

		call	PauseSample

waitpause:	cmp	byte ptr[FlagPause], 1
		jne	short waitpause

	CRIT_SECT

		lea	esi, CurrentList		; backup CurrentList
		lea	edi, BackCurrentList
		call	CopyList
		lea	esi, SonList			; backup SonList
		lea	edi, BackSonList
		call	CopyList
		mov	eax, dword ptr[follow]		; Backup follow
		mov	dword ptr[backfollow], eax
		mov	eax, dword ptr[weirdcount]	; Backup weirdcount
		mov	dword ptr[backweirdcount], eax

		xor     eax, eax			; empty both lists
		mov     dword ptr[CurrentList], eax
		mov     dword ptr[SonList], eax

		mov	byte ptr[FlagPause], 0

	END_CRIT_SECT

		ret

SAveStateSample	ENDP

;----------------------------------------------------------------------------

RestoreStateSample PROC	USES ESI EDI

		call	PauseSample

waitpause:	cmp	byte ptr[FlagPause], 1
		jne	short waitpause

	CRIT_SECT

		lea	esi, BackCurrentList		; restore CurrentList
		lea	edi, CurrentList
		call	CopyList
		lea	edi, BackSonList		; restore SonList
		lea	edi, SonList
		call	CopyList
		mov	eax, dword ptr[backfollow]	; restore follow
		mov	dword ptr[follow], eax
		mov	eax, dword ptr[backweirdcount]	; restore weirdcount
		mov	eax, dword ptr[weirdcount]

	END_CRIT_SECT

		call	ContinueSample

waitcontinue:	cmp	byte ptr[FlagPause], 0
		jne	short waitcontinue

		ret

RestoreStateSample ENDP

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	PAS

INT2F           PROC\                   ;Perform DPMI call to real-mode INT 2F
		USES esi edi\           ;for Pro Audio Spectrum driver access
		,regAX,regBX

		mov ax,WORD PTR [regAX]
		mov int_AX,ax

		mov ax,WORD PTR [regBX]
		mov int_BX,ax

		mov int_DS,0
		mov int_ES,0

		pushf
		pop ax
		mov int_flags,ax

		mov eax,0300h
		mov ebx,002fh
		mov ecx,0
		mov edi,OFFSET DPMI_real_int
		int 31h

		mov ax,int_AX
		mov bx,int_BX
		mov cx,int_CX
		mov dx,int_DX
		ret

INT2F           ENDP

;----------------------------------------------------------------------------

ResetCard       PROC	USES EBX

		mov eax,0bc04h          ;get current DMA and IRQ settings
		invoke INT2F,eax,eax
		and ebx,0ffh
		and ecx,0ffh
		mov [DMA_number],ebx
		mov [IRQ_number],ecx

		mov eax,0bc07h          ;get state table entries
		invoke INT2F,eax,eax

		xor eax,eax             ;update local state table
		mov al,bh
		mov MV_xchannel,eax
		mov al,ch
		mov MV_filter,eax

		mov eax, MV_xchannel
		mov edx, CROSSCHANNEL	;disable DRQs from PAS
IFDEF	SAMPLE16BIT
		and eax, 00110000b	;and disable PCM state machine
		or  eax, 00001001b
ELSE
		and eax, 00110000b	;and disable PCM state machine
		or  eax, 00000110b
ENDIF
		out dx, al
		mov MV_xchannel, eax

		mov	edx, PCMDATA		;silence PCM output
		mov	al, 80h
		out	dx, al


		mov	edx, SYSCONF
		in	al, dx

IFDEF	SAMPLE16BIT
		or	al, 100b
ELSE
		and	al, 11111011b
ENDIF

		out	dx, al


		mov	ecx, [PlayRate]
		shl	ecx, 1
		xor	edx, edx
		mov	eax, 1193180
		div	ecx
		mov	ecx, eax

		mov	al, 00110110b		;timer 0, square wave, binary mode
		mov	edx, TMRCTLR
		out	dx, al
		mov	edx, SAMPLERATE
		mov	al, cl
		out	dx, al
		jmp	$+2
		mov	al, ch
		out	dx, al

		mov edx,INTRCTLRST      ;enable IRQs on sample buffer empty
		out dx,al
		jmp $+2
		in al,dx
		mov edx,INTRCTLR
		in al,dx
		or eax,00001000b
		out dx,al

		ret

ResetCard	ENDP

;----------------------------------------------------------------------------

CloseCard	PROC

		mov eax,MV_xchannel
		mov edx,CROSSCHANNEL     ;disable DRQs from PAS
		and eax,00111111b        ;and disable PCM state machine
		out dx,al
		mov MV_xchannel,eax

		ret

CloseCard	ENDP

;----------------------------------------------------------------------------

StartDMACard    PROC

		mov eax,MV_xchannel
		mov edx,CROSSCHANNEL
		or eax,10000000b        ;secure the DMA channel
		out dx,al
		mov MV_xchannel,eax

		mov cx, BUFFER_SIZE * SSIZE

		mov eax,01110100b       ;program sample buffer counter
		mov edx,TMRCTLR
		out dx,al
		mov edx,SAMPLECNT
		mov al,cl
		out dx,al
		jmp $+2
		mov al,ch
		out dx,al

		mov al,BYTE PTR MV_xchannel
		and al,00001111b        ;reset PCM state machine
		or  al,10010000b
		mov edx,CROSSCHANNEL
		out dx,al
		jmp $+2
		or al,01000000b
		out dx,al
		mov BYTE PTR MV_xchannel,al

;		mov eax,MV_filter
;		or  eax,11000000b
		mov eax,11100000b
		mov edx,AUDIOFILT
		out dx,al               ;start the transfer
		mov MV_filter,eax

		ret

StartDMACard	ENDP

;----------------------------------------------------------------------------

AckIrq          PROC

		mov edx,INTRCTLRST
		in al,dx                ;acknowledge the interrupt
		test eax,00001000b
		jz error                ;it wasn't caused by our board, exit
		out dx,al
		clc
		ret
error:
		stc
		ret

AckIrq          ENDP

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef   MWSS

ResetCard       PROC	USES EBX


		mov	ax, word ptr[PlayRate]
		cmp	ax, 44100
		jae	short Freq44
		lea	edx, MWSSFreq
SearchFreq:	cmp	ax, word ptr[edx]
		jb	short FoundFreq
		add	edx, 4
		jmp	short SearchFreq
Freq44:
		mov	ax, 44100
		mov	cl, 0Bh
		jmp	short WBack
FoundFreq:
		mov	ax, word ptr[edx]		; closest faster speed
		mov	cl, byte ptr[edx+2]		; clock select
WBack:
		mov	word ptr[PlayRate], ax
		or	cl, 01010000b			; 16 bit linear, Stereo

		mov	edx, [IRQ_number]		; IRQ
		mov	al, [MWSSIrq+edx]

		mov	edx, [DMA_number]		; DMA
		or	al, [MWSSDma+edx]

		mov	dx, word ptr[BASE_ADDR]

		add	dl, 3				; offset 3
		out	dx, al				; program IRQ & DMA

		inc	dl				; offset 4
		mov	al, 6				; left vol
		out	dx, al
		inc	dl				; offset 5
		mov	al, 80h				; mute
		out	dx, al
		dec	dl				; offset 4
		mov	al, 7				; right vol
		out	dx, al
		inc	dl				; offset 5
		mov	al, 80h				; mute
		out	dx, al

		dec	dl				; offset 4
		mov	al, 13				; digital mix
		out	dx, al
		inc	dl				; offset 5
		xor	al, al				; mute
		out	dx, al

		dec	dl				; offset 4
		mov	al, 2				; left aux1
		out	dx, al
		inc	dl				; offset 5
		mov	al, 80h				; mute
		out	dx, al
		dec	dl				; offset 4
		mov	al, 3				; right aux1
		out	dx, al
		inc	dl				; offset 5
		mov	al, 80h				; mute
		out	dx, al

		dec	dl				; offset 4
		mov	al, 4				; left aux2
		out	dx, al
		inc	dl				; offset 5
		mov	al, 80h				; mute
		out	dx, al
		dec	dl				; offset 4
		mov	al, 5				; right aux2
		out	dx, al
		inc	dl				; offset 5
		mov	al, 80h				; mute
		out	dx, al

		dec	dl				; offset 4
		mov	al, 49h				; enter MCE, Interface Ctrl
		out	dx, al
		inc	dl				; offset 5
		mov	al, 08h				; auto-calibrate
		out	dx, al

		dec	dl				; offset 4
		mov	al, 48h				; MCE, Interface Ctrl
		out	dx, al
		inc	dl				; offset 5
		mov	al, cl				; 16 bit linear, stereo, speed
		out	dx, al

		dec	dl				; offset 4
wait_init:	in	al, dx
		and	al, 80h
		jnz	wait_init


		xor	al, al
		out	dx, al				; clear MCE

		xor	bx, bx				; at least 6 millisec.
wait0:		in	al, dx
		dec	bx
		jnz	short wait0


wait_cal:	mov	al, 0Bh				; Test & Init Reg
		out	dx, al
		inc	dl				; offset 5
		in	al, dx
		dec	dl				; offset 4
		and	al, 20h
		jnz	wait_cal

		mov	al, 6				; left vol
		out	dx, al
		inc	dl				; offset 5
		xor	al, al				; max vol
		out	dx, al
		dec	dl				; offset 4
		mov	al, 7				; right vol
		out	dx, al
		inc	dl				; offset 5
		xor	al, al				; max vol
		out	dx, al

		ret

ResetCard       ENDP

;*--------------------------------------------------------------------------*

CloseCard       PROC

		mov     dx, word ptr[BASE_ADDR]         ; DSP

		add	dl, 4				; offset 4
		mov	al, 0Ah				; Pin Ctrl reg
		out	dx, al
		inc	dx				; offset 5
		xor	al, al				; turn off interrupts
		out	dx, al

		inc	dx				; offset 6
		out	dx, al				; Ack outstanding interrupts

		sub	dl, 2				; offset 4
		mov	al, 9				; Interface Config reg
		out	dx, al
		inc	dx				; offset 5
		xor	al, al				; turn off codec DMA
		out	dx, al

		sub	dl, 2
		out	dx, al				; deselect IRQ and DMA

		ret

CloseCard       ENDP

;----------------------------------------------------------------------------

AckIrq          PROC

		mov     dx, word ptr[BASE_ADDR]         ; DSP
		add	dl, 6
		in	al, dx
		shr	al, 1				; get int flag in C
		cmc					; complement C in order to return error if 0
		out	dx, al				; Ack anyway
		ret

AckIrq          ENDP

;----------------------------------------------------------------------------

StartDMACard    PROC

		mov     cx, BUFFER_SIZE - 1

		mov     dx, word ptr[BASE_ADDR]         ; DSP

		add	dl, 6
		out	dx, al				; clear any pending IRQ

		sub	dl, 2				; offset 4
		mov	al, 15				; DMA Count Lower
		out	dx, al
		inc	dl				; offset 5
		mov	al, cl
		out	dx, al
		dec	dl				; offset 4
		mov	al, 14				; DMA Count Upper
		out	dx, al
		inc	dl				; offset 5
		mov	al, ch
		out	dx, al

		dec	dl				; offset 4
		mov	al, 9				; Interface config
		out	dx, al
		inc	dl				; offset 5
		mov	al, 00000101b			; DMA Playback
		out	dx, al
		dec	dl				; offset 4
		mov	al, 0Ah				; Pin Ctrl Reg
		out	dx, al
		inc	dx				; offset 5
		mov	al, 00000010b			; Turn on Interrupts
		out	dx, al

		ret

StartDMACard    ENDP

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef   SBLASTER

WRITE_DSP       MACRO   VAL                     ; write to DSP whithout timeout
		local   wait_dsp

wait_dsp:       in      al, dx
		or      al, al
		js      wait_dsp

		mov     al, VAL
		out     dx, al

		ENDM

WRITE_MIXER     MACRO   REG, VAL		; write to MIXER chip

		mov	al, REG
		out	dx, al
		inc	dx
		mov	al, VAL
		out	dx, al
		dec	dx

		ENDM

READ_MIXER      MACRO   REG			; read from MIXER chip

		mov	al, REG
		out	dx, al
		inc	dx
		in	al, dx
		dec	dx

		ENDM

ifdef	SBPRO

InitIRQ		PROC

		pushad
		push    ds

		mov     ax, cs:word ptr[local_DS]	; restore DS
		mov     ds, ax

		mov     al, 20h                         ; allows for new int
		cmp	byte ptr[IRQ_number], 7
		jbe	short NoSecondCtrl
		out	0A0h, al
NoSecondCtrl:	out     20h, al

		call    AckIrq
		jc	short FinIRQ			; not a DMA IRQ

		mov	byte ptr[OkIRQ], 1
FinIRQ:
		pop     ds
		popad

		iretd

InitIRQ		ENDP

endif


ResetCard       PROC

		cmp	dword ptr[PlayRate], 12000
		jae	OkRateLow
		mov	dword ptr[PlayRate], 12000
OkRateLow:

		cmp	dword ptr[PlayRate], 22000
		jbe	OkRateHigh
		mov	dword ptr[PlayRate], 22000
OkRateHigh:

ifndef  SB16
	ifdef	SBPRO
		mov     dx, 07A1h
		mov     ax, 2000h                       ; dx:ax = 128000000
	else
		mov     dx, 0F42h
		mov     ax, 4000h                       ; dx:ax = 256000000
	endif
		div     word ptr[PlayRate]              ; divide by PlayRate
		neg	ax
		shr	ax, 8
		mov	cx, ax				; cl = "Magic"

		push	ecx

	ifdef	SBPRO
		mov     dx, 07h
		mov     ax, 0A120h                      ; dx:ax = 500000
	else
		mov     dx, 0Fh
		mov     ax, 4240h                       ; dx:ax = 1000000
	endif
		neg	cl
		div     cx                              ; divide by "-Magic"
		mov     word ptr[PlayRate], ax          ; write back PlayRate
endif

		mov     dx, word ptr[BASE_ADDR]
		add     dl, 6                           ; reset port (offset 6)
		mov     al, 1                           ; write 1
		out     dx, al

		mov	ah, 8 * 3			; 4 * "in al, dx" = 1 microsec on std ISA bus (8 Mhz)
wait_io:	in      al, dx                          ; so  8 to be sure if 16 MHz !
		dec	ah				; kill the cat !
		jnz	short wait_io

		xor     al, al
		out     dx, al                          ; write 0
							; now reset should be done..
		add     dl, 4                           ; offset 0Ah

Next:           add     dl, 4                           ; offset 0Eh

Wait_s:         in      al, dx
		or      al, al
		jns     Wait_s

		sub     dl, 4                           ; offset 0Ah
		in      al, dx                          ; read if reset terminated

		cmp     al, RESET_TEST_CODE             ; should be...
		jne     Next

ifdef	SB16
		sub	dl, 6				; offset 04h
		mov	al, 80h
		out	dx, al
		inc	dx				; offset 05h
		xor	eax, eax
		in	al, dx
		and	al, 0Fh				; mask reserved bit
		bsf	ax, ax
		mov	al, byte ptr[SB16_IRQ+eax]
		mov	dword ptr[IRQ_number], eax
		dec	dx				; offset 04h
		mov	al, 81h
		out	dx, al
		inc	dx				; offset 05h
		in	al, dx
		and	al, 11101011b			; mask reserved bit
	ifdef	SAMPLE16BIT
		bsr	ax, ax
	else
		bsf	ax, ax
	endif
		mov	dword ptr[DMA_number], eax

		add	dl, 7				; offset 0Ch
		WRITE_DSP DSP_RATE_CMD                  ; rate
		mov     cx, word ptr[PlayRate]
		WRITE_DSP ch                            ;
		WRITE_DSP cl                            ;
else
		add     dl, 02h                         ; offset 0Ch
		WRITE_DSP DSP_ONSPK_CMD                 ; Turn speaker on

	ifdef	SBPRO

		mov	edx, offset InitIRQ		; Temporary IRQ handler
		call	InstallISR

		mov	dx, word ptr[BASE_ADDR]

		add     dl, 4				; offset 4
		mov	al, 0Eh				; stereo switch & filter
		out	dx, al
		inc	dx				; offset 5
		in	al, dx
		or	al, 2				; enable stereo
		out	dx, al

		mov     ecx, dword ptr[DMA]             ; Point to DMAx

		mov     dx, word ptr[ecx + MASK_REG]
		mov     al, byte ptr[ecx + MASK2]       ; mask channel
		out     dx, al

		mov     dx, word ptr[ecx + FF_REG]
		out     dx, al                          ; flip-flop


		mov     dx, word ptr[ecx + COUNT_REG]
		xor	al, al
		out     dx, al
		out     dx, al

		mov     eax, dword ptr[BUFFER_DMA]	; start offset
		mov	byte ptr[eax], 80h		; write a "0" into the buffer

		mov     dx, word ptr[ecx + ADDX_REG]
		out     dx, al
		shr     eax, 8
		out     dx, al

		mov     dx, word ptr[ecx + PAGE_REG]
		shr     eax, 8                          ; page of DMA transfert
		out     dx, al

		mov     dx, word ptr[ecx + MODE_REG]
		mov     al, byte ptr[ecx + VOICE_OUT]   ; output sample
		out     dx, al

		mov     dx, word ptr[ecx + MASK_REG]
		mov     al, byte ptr[ecx + MASK1]       ; channel OK
		out     dx, al

		mov     dx, word ptr[BASE_ADDR]
		add	dl, 0Ch                         ; offset 0Ch

		mov	byte ptr[OkIRQ], 0

		WRITE_DSP DSP_VO8S_CMD
		WRITE_DSP 0
		WRITE_DSP 0

WaitIRQ:	cmp	byte ptr[OkIRQ], 1
		jne	short WaitIRQ

		sub     dl, 8				; offset 4
		mov	al, 0Eh				; stereo switch & filter
		out	dx, al
		inc	dx				; offset 5
		in	al, dx
		mov	byte ptr[Filter], al
		or	al, 20h				; disable filter
		out	dx, al
		add	dl, 7				; offset 0Ch
	endif

		pop	ecx

		WRITE_DSP DSP_TIME_CMD                  ; "rate"
		WRITE_DSP cl                            ; magic number !

endif
		ret

ResetCard       ENDP

;*--------------------------------------------------------------------------*

CloseCard       PROC

		mov     dx, word ptr[BASE_ADDR]         ; point to DSP

ifdef   SB16
		add     dl, 0CH
ifdef	SAMPLE16BIT
		WRITE_DSP DSP_VO16S_CMD                 ; single-cycle 16 bit DMA
	ifdef	STEREO
		WRITE_DSP DSP_16STEREO_MODE		; 16 bit stereo
	else
		WRITE_DSP DSP_16MONO_MODE		; 16 bit mono
	endif
else
		WRITE_DSP DSP_VO8S_4_CMD                ; single-cycle 8 bit DMA
	ifdef	STEREO
		WRITE_DSP DSP_8STEREO_MODE		; 8 bit stereo
	else
		WRITE_DSP DSP_8MONO_MODE		; 8 bit mono
	endif
endif
		WRITE_DSP 0                             ; length of 1 to finish
		WRITE_DSP 0
else
	ifdef	SBPRO
		add     dl, 6                           ; reset port (offset 6)
		mov     al, 1                           ; write 1
		out     dx, al

		mov	ah, 8 * 3			; 4 * "in al, dx" = 1 microsec on std ISA bus (8 Mhz)
wait_io:	in      al, dx                          ; so  8 to be sure if 16 MHz !
		dec	ah				; kill the cat !
		jnz	wait_io

		xor     al, al
		out     dx, al                          ; write 0
							; now reset should be done..
		add     dl, 4                           ; offset 0Ah

Next:           add     dl, 4                           ; offset 0Eh

Wait_s:         in      al, dx
		or      al, al
		jns     Wait_s

		sub     dl, 4                           ; offset 0Ah
		in      al, dx                          ; read if reset terminated

		cmp     al, RESET_TEST_CODE             ; should be...
		jne     Next

		sub	dl, 6				; offset 4
		mov	al, 0Eh
		out	dx, al
		inc	dx				; offset 5
		mov	al, byte ptr[Filter]		; restore filter
		out	dx, al

		dec	dx
		mov	al, 0Eh
		out	dx, al
		inc	dx				; offset 5
		in	al, dx
		and	al, 11111101b			; turn off stereo
		out	dx, al

		add	dl, 7				; offset 0Ch
		WRITE_DSP DSP_OFFSPK_CMD                ; Turn speaker off

	else

		add     dl, 0CH
		WRITE_DSP DSP_OFFSPK_CMD                ; Turn speaker off
		WRITE_DSP DSP_VO8S_CMD                  ; single-cycle 8 bit DMA
		WRITE_DSP 0                             ; length of 1 to finish
		WRITE_DSP 0

	endif
endif

		ret

CloseCard       ENDP

;----------------------------------------------------------------------------

AckIrq          PROC

		mov     dx, word ptr[BASE_ADDR]         ; DSP IRQ ACK
ifdef	SB16
		add	dl, 4				; offset 4
		mov	al, 82h
		out	dx, al
		inc	dx				; offset 5
		in	al, dx
	ifdef	SAMPLE16BIT
		and	al, 2
		jz	NoDMAIRQ
		add	dl, 10				; offset Fh
	else
		and	al, 1
		jz	NoDMAIRQ
		add	dl, 9				; offset Eh
	endif
		in	al, dx
		ret
NoDMAIRQ:
		stc
		ret
else

irq_ok:		add     dl, 0Eh                         ; 8 bit Samples ACK
		in      al, dx
not_real:
		ret
endif

AckIrq          ENDP

;----------------------------------------------------------------------------

StartDMACard    PROC

		mov     dx, word ptr[BASE_ADDR]         ; DSP
		add     dl, 0Ch

ifdef	STEREO
		mov     cx, BUFFER_SIZE * 2 - 1		; half-buffer size - 1 for DMA
else
		mov     cx, BUFFER_SIZE - 1		; half-buffer size - 1 for DMA
endif

ifdef   SB16


ifdef	SAMPLE16BIT
		WRITE_DSP DSP_VO16_CMD			; auto-init 16 bit DMA
	ifdef	STEREO
		WRITE_DSP DSP_16STEREO_MODE		; 16 bit stereo
	else
		WRITE_DSP DSP_16MONO_MODE		; 16 bit mono
	endif
else
		WRITE_DSP DSP_VO8_4_CMD			; auto-init 8 bit DMA
	ifdef	STEREO
		WRITE_DSP DSP_8STEREO_MODE		; 8 bit stereo
	else
		WRITE_DSP DSP_8MONO_MODE		; 8 bit mono
	endif
endif
		WRITE_DSP cl
		WRITE_DSP ch

elseifdef SBLASTER1

		WRITE_DSP DSP_VO8S_CMD			; single-cycle 8 bit DMA
		WRITE_DSP cl
		WRITE_DSP ch

else

		WRITE_DSP DSP_BSIZE_CMD                 ; Set block size
		WRITE_DSP cl
		WRITE_DSP ch

	ifdef	SBPRO
		WRITE_DSP DSP_VO8H_CMD			; auto-init 8 bit high-speed DMA
	else
		WRITE_DSP DSP_VO8_CMD			; auto-init 8 bit DMA
	endif
endif
		ret

StartDMACard    ENDP

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef   GOLD

SELECT_MIXER    MACRO
		local	wait

		cli

		mov	al, 0FFh
		out	dx, al
		out	dx, al
wait:           in      al, dx
		and	al, 11000000b
		jnz     wait

		ENDM

LEAVE_MIXER     MACRO
		local	wait

wait:           in      al, dx
		and	al, 11000000b
		jnz     wait
		mov	al, 0FEh
		out	dx, al
		out	dx, al

		sti

		ENDM

WRITE_MIXER     MACRO   PORT, VAL
		local   wait1, wait2

		cli

		mov	al, PORT
		out	dx, al
wait1:          in      al, dx
		and	al, 11000000b
		jnz     wait1
		inc	dx
		mov     al, VAL
		out     dx, al
		dec	dx
wait2:          in      al, dx
		and	al, 11000000b
		jnz     wait2

		sti

		ENDM

READ_MIXER      MACRO   PORT
		local   wait1, wait2

		cli

		mov	al, PORT
		out	dx, al
wait1:          in      al, dx
		and	al, 11000000b
		jnz     wait1
		inc	dx
		in	al, dx
		dec	dx
		mov	ah, al
wait2:          in      al, dx
		and	al, 11000000b
		jnz     wait2
		mov	al, ah

		sti

		ENDM

WRITE_MMA0	MACRO   PORT, VAL

		cli

		mov	al, PORT
		out	dx, al
		inc	dx
		mov     al, VAL
		out     dx, al
		dec	dx
		in	al, dx
		in	al, dx

		sti

		ENDM

READ_MMA0	MACRO   PORT

		cli

		mov	al, PORT
		out	dx, al
		inc	dx
		in	al, dx
		dec	dx
		mov	ah, al
		in	al, dx
		in	al, dx
		mov	al, ah

		sti

		ENDM

WRITE_MMA1	MACRO   PORT, VAL

		cli

		mov	al, PORT
		out	dx, al
		add	dx, 3
		mov     al, VAL
		out     dx, al
		sub	dx, 3
		in	al, dx
		in	al, dx

		sti

		ENDM

READ_MMA1	MACRO   PORT

		cli

		mov	al, PORT
		out	dx, al
		add	dx, 3
		in	al, dx
		sub	dx, 3
		mov	ah, al
		in	al, dx
		in	al, dx
		mov	al, ah

		sti

		ENDM


;*--------------------------------------------------------------------------*

ResetCard       PROC

		mov	dx, word ptr[BASE_ADDR]
		add	dx, 2				; MIXER

		SELECT_MIXER

		WRITE_MIXER 8h, 11001110b		; STEREO mode
		WRITE_MIXER 11h, 00001000b		; filter on output, no mic input, no PC-speaker input

		READ_MIXER  13h

		mov	byte ptr[Mixer_13], al

		mov	cl, al

		and	eax, 3
		lea	ebx, Gold_IRQ
		mov	al, byte ptr[ebx+eax]
		mov	dword ptr[IRQ_number], eax

		mov	al, cl
		shr	al, 4
		and	al, 3
		mov	dword ptr[DMA_number], eax

		or	cl, 10001000b			; enable DMA0, enable IRQ
		WRITE_MIXER 13h, cl

		READ_MIXER  14h

		mov	byte ptr[Mixer_14], al

		mov	cl, al
		and	cl, 01111111b
		WRITE_MIXER 14h, cl

		LEAVE_MIXER

		add	dx, 2				; MMA

		WRITE_MMA0  9, 80h			; reset channel 0
		WRITE_MMA1  9, 80h			; reset	channel 1

		mov	cx, 4
fillFIFO0:	WRITE_MMA0  0Bh, 0
		dec	cx
		jnz	FillFIFO0

		mov	cx, 4
fillFIFO1:	WRITE_MMA1  0Bh, 0
		dec	cx
		jnz	FillFIFO1

		WRITE_MMA0  9, 00101110b		; 22.05 Khz, PCM, L, Play
		WRITE_MMA1  9, 01001110b		; 22.05 Khz, PCM, R, Play

		mov     word ptr[PlayRate], 22050	; 22.05 Khz

		WRITE_MMA0  0Ch, 11000101b		; Interleave, 16 bit, 96 byte FIFO, DMA
		WRITE_MMA1  0Ch, 01000011b		; 16 bit, no FIFO (use other channel), DMA


		WRITE_MMA0  0Ah, 0FFh			; Maximum volume (volume will be controlled by mixer)
		WRITE_MMA1  0Ah, 0FFh

		mov	cx, 100
fillFIFO:	WRITE_MMA0  0Bh, 0
		dec	cx
		jnz	FillFIFO

		ret

ResetCard       ENDP

;*--------------------------------------------------------------------------*

CloseCard       PROC

		mov     dx, word ptr[BASE_ADDR]
		add	dx, 4				; MMA

		WRITE_MMA1  0Ch, 01000010b		; 16 bit, no FIFO, no DMA
		WRITE_MMA0  0Ch, 01000010b		; 16 bit, no FIFO, no DMA

		WRITE_MMA0  9, 80h			; reset channel 0
		WRITE_MMA1  9, 80h			; reset	channel 1

		sub	dx, 2				; MIXER

		SELECT_MIXER

		WRITE_MIXER 13h, Mixer_13
		WRITE_MIXER 14h, Mixer_14

		LEAVE_MIXER

		ret

CloseCard       ENDP

;----------------------------------------------------------------------------

AckIrq          PROC

		mov     dx, word ptr[BASE_ADDR]
		add	dx, 4				; MMA

		in	al, dx
		and	al, 1
		jz	short noDMA

		mov     ebx, dword ptr[DMA]		; Point to DMAx

		mov     dx, word ptr[ebx + FF_REG]
		out     dx, al				; flip-flop

		mov     dx, word ptr[ebx + COUNT_REG]
		in	al, dx
		mov	ah, al
		in	al, dx
		xchg	al, ah

		or	ax, ax
		jz	short OkDMA
		inc	ax
		jnz	short noDMA
OkDMA:
		clc
		ret
NoDMA:
		stc
not_real:
		ret

AckIrq          ENDP

;----------------------------------------------------------------------------

StartDMACard    PROC

		mov     dx, word ptr[BASE_ADDR]
		add     dx, 4				; MMA

		WRITE_MMA0  9, 00101111b		; 22.05 Khz, PCM, L, Play, GO

		ret

StartDMACard    ENDP

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

ifdef	GUS

extrn	Nolanguage	resetcard:PROC

;*--------------------------------------------------------------------------*

extrn	Nolanguage	CloseCard:PROC

;*--------------------------------------------------------------------------*

extrn	Nolanguage	StartDMACard:PROC

endif

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

		END
