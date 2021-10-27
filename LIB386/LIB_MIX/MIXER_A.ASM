;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
;                              MIXER_A.ASM 386
;                             (c) Adeline 1993
;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*

		.386
		jumps

		.model  FLAT, SYSCALL

		.CODE

;*--------------------------------------------------------------------------*

IFDEF	SB16
		SBLASTER	equ	1
ENDIF

IFDEF	SBPRO
		SBLASTER	equ	1
ENDIF

IFDEF	SB2CD
		SBLASTER	equ	1
ENDIF

;*--------------------------------------------------------------------------*

		PUBLIC	driver_start

driver_start    dd 	OFFSET ListFuncs

IFDEF	SB16
		db	'Sound Blaster 16 Mixer (CT1745) Driver'
ENDIF

IFDEF	SBPRO
		db	'Sound Blaster Pro Mixer (CT1345) Driver'
ENDIF

IFDEF	SB2CD
		db	'Sound Blaster 2 CD Interface Mixer (CT1335) Driver'
ENDIF

IFDEF	MWSS
		db	'Microsoft Windows Sound System Mixer (AD1848 SoundPort) Driver'
ENDIF

IFDEF	GOLD
		db	'Adlib Gold Mixer (Yamaha GOLD) Driver'
ENDIF

IFDEF	PAS
		db	'Media Vision Pro Audio Spectrum/Spectrum Plus/Spectrum 16 Mixer Driver'
ENDIF
		db 	', Version 1.00,', 13, 10
		db 	'Copyright (c) Adeline Software International 1994, All Rights Reserved.', 13, 10, 10, 0


ListFuncs	dd	offset	AskVars

		dd	offset	SetMixerVolume
		dd	offset	GetMixerVolume
		dd	offset	GetMixerInfo

;*--------------------------------------------------------------------------*

MixerBase		db	'MixerBase'
Empty			db	0

IFDEF PAS

ListNames		dd	offset	Empty

MV_filter       dd ?
MV_xchannel     dd ?

MVP_name        db 'MVPROAS',0

MV_vl_txt       db 'SET OUTPUT MIXER LEFT '
MV_dl		db 'PCM '
		db 'TO '
MV_vl           db '0'
		db '0'
		db '0'
		db '%',13
MV_vl_len       equ ($-MV_vl_txt)

MV_vr_txt       db 'SET OUTPUT MIXER RIGHT '
MV_dr		db 'PCM '
		db 'TO '
MV_vr           db '0'
		db '0'
		db '0'
		db '%',13
MV_vr_len       equ ($-MV_vr_txt)

MV_ml_txt       db 'SET VOLUME LEFT LEVEL TO '
MV_ml           db '0'
		db '0'
		db '0'
		db '%',13
MV_ml_len       equ ($-MV_ml_txt)

MV_mr_txt       db 'SET VOLUME RIGHT LEVEL TO '
MV_mr           db '0'
		db '0'
		db '0'
		db '%',13
MV_mr_len       equ ($-MV_mr_txt)

ELSE

ListNames		dd	offset	MixerBase
			dd	offset	Empty

ENDIF

;*--------------------------------------------------------------------------*
;*--------------------------------------------------------------------------*

AskVars		PROC	,\
		pListNames:DWORD, pListVars:DWORD

		mov	eax, pListNames
		mov	dword ptr[eax], offset ListNames
		mov	eax, pListVars
		mov	dword ptr[eax], offset ListVars
		ret

AskVars		ENDP

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF PAS

ListVars	equ	$

;----------------------------------------------------------------------------

SVolVoice	dd	255
SVolMidi	dd	255
SVolCD		dd	255
SVolLine	dd	255
SVolMaster	dd	255

;----------------------------------------------------------------------------

decstr          PROC USES ebx esi edi es,\  ;Decimal ASCII to string buffer
		Buf,Num                 ;(no lz suppression)
		LOCAL accum

		cld

		push ds
		pop es

		mov edi,[Buf]
		mov eax,[Num]
		mov accum,eax

		mov ecx,100
__div_loop:     mov eax,accum
		mov edx,0
		div ecx
		mov accum,edx
		add eax,'0'

		stosb

		mov eax,ecx
		mov edx,0
		mov ebx,10
		div ebx
		mov ecx,eax
		cmp eax,0
		jne __div_loop

		ret
decstr          ENDP

;----------------------------------------------------------------------------

send_MV_cmd     PROC USES ebx esi edi,\ ;Send command string to MVPROAS
		String,Len              ;(device alias for MVSOUND.SYS)
		LOCAL handle

		mov eax,3d02h           ;open MVPROAS for writing
		mov edx,OFFSET MVP_name
		int 21h
		jc __exit
		mov handle,eax

		mov ebx,eax             ;write the command string
		mov ah,40h
		mov ecx,[Len]
		mov edx,[String]
		int 21h

		mov ah,3eh              ;close MVPROAS
		mov ebx,handle
		int 21h
__exit:
		ret
send_MV_cmd     ENDP

;----------------------------------------------------------------------------

SetMixerVolume	PROC	USES EDI\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov	dword ptr[MV_dr], 2043494Dh	; 'MIC '
		mov edi,OFFSET MV_vr
		invoke decstr,edi,0
		mov edi,OFFSET MV_vr_txt
		mov ecx,MV_vr_len
		invoke send_MV_cmd,edi,ecx

		mov	dword ptr[MV_dl], 2043494Dh
		mov edi,OFFSET MV_vl
		invoke decstr,edi,0
		mov edi,OFFSET MV_vl_txt
		mov ecx,MV_vl_len
		invoke send_MV_cmd,edi,ecx

		mov	eax, VolVoice
		cmp	eax, -1
		je	novoice

		mov	SVolVoice, eax

		imul	ax, 100
		xor	dx, dx
		mov	cx, 255
		div	cx

		push	eax

		mov	dword ptr[MV_dr], 204D4350h	; 'PCM '
		mov edi,OFFSET MV_vr
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vr_txt
		mov ecx,MV_vr_len
		invoke send_MV_cmd,edi,ecx

		pop	eax

		mov	dword ptr[MV_dl], 204D4350h
		mov edi,OFFSET MV_vl
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vl_txt
		mov ecx,MV_vl_len
		invoke send_MV_cmd,edi,ecx

novoice:
		mov	eax, VolMidi
		cmp	eax, -1
		je	nomidi

		mov	SVolMidi, eax

		imul	ax, 100
		xor	dx, dx
		mov	cx, 255
		div	cx

		push	eax

		mov	dword ptr[MV_dr], 20204D46h	; 'FM  '
		mov edi,OFFSET MV_vr
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vr_txt
		mov ecx,MV_vr_len
		invoke send_MV_cmd,edi,ecx

		pop	eax

		mov	dword ptr[MV_dl], 20204D46h
		mov edi,OFFSET MV_vl
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vl_txt
		mov ecx,MV_vl_len
		invoke send_MV_cmd,edi,ecx

nomidi:
		mov	eax, VolCD
		cmp	eax, -1
		je	nocd

		mov	SVolCD, eax

		imul	ax, 100
		xor	dx, dx
		mov	cx, 255
		div	cx

		push	eax

		mov	dword ptr[MV_dr], 20204443h	; 'CD  '
		mov edi,OFFSET MV_vr
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vr_txt
		mov ecx,MV_vr_len
		invoke send_MV_cmd,edi,ecx

		pop	eax

		mov	dword ptr[MV_dl], 20204443h
		mov edi,OFFSET MV_vl
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vl_txt
		mov ecx,MV_vl_len
		invoke send_MV_cmd,edi,ecx

nocd:
		mov	eax, VolLine
		cmp	eax, -1
		je	noline

		mov	SVolLine, eax

		imul	ax, 100
		xor	dx, dx
		mov	cx, 255
		div	cx

		push	eax

		mov	dword ptr[MV_dr], 20545845h	; 'EXT '
		mov edi,OFFSET MV_vr
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vr_txt
		mov ecx,MV_vr_len
		invoke send_MV_cmd,edi,ecx

		pop	eax

		mov	dword ptr[MV_dl], 20545845h
		mov edi,OFFSET MV_vl
		invoke decstr,edi,eax
		mov edi,OFFSET MV_vl_txt
		mov ecx,MV_vl_len
		invoke send_MV_cmd,edi,ecx

noline:
		mov	eax, VolMaster
		cmp	eax, -1
		je	nomaster

		mov	SVolMaster, eax

		imul	ax, 100
		xor	dx, dx
		mov	cx, 255
		div	cx

		push	eax

		mov edi,OFFSET MV_mr
		invoke decstr,edi,eax
		mov edi,OFFSET MV_mr_txt
		mov ecx,MV_mr_len
		invoke send_MV_cmd,edi,ecx

		pop	eax

		mov edi,OFFSET MV_ml
		invoke decstr,edi,eax
		mov edi,OFFSET MV_ml_txt
		mov ecx,MV_ml_len
		invoke send_MV_cmd,edi,ecx
nomaster:
		ret

SetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerVolume  PROC	uses EBX,\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov	edx, VolMaster
		or	edx, edx
		jz	nomaster

                mov	eax, SVolMaster
		mov	[edx], eax
nomaster:
		mov	edx, VolLine
		or	edx, edx
		jz	noline

		mov	eax, SVolLine
		mov	[edx], eax
noline:
		mov	edx, VolCD
		or	edx, edx
		jz	nocd

		mov	eax, SVolCD
		mov	[edx], ebx
nocd:
		mov	edx, VolMidi
		or	edx, edx
		jz	nomidi

		mov	eax, SVolMidi
		mov	[edx], eax
nomidi:
		mov	edx, VolVoice
		or	edx, edx
		jz	novoice

		mov	eax, SVolVoice
		mov	[edx], ecx
novoice:
		ret

GetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerInfo	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dword ptr[ecx], 1
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 1
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 1
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 1
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 1
nomaster:
		ret

GetMixerInfo	ENDP

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF   MWSS

ListVars		equ	$
BASE_ADDR               dd      530h		; By default 530h

;----------------------------------------------------------------------------

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

;----------------------------------------------------------------------------

SetMixerVolume	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov	dx, word ptr[BASE_ADDR]
		add	dx, 4

		mov	eax, VolVoice
		cmp	eax, -1
		je	novoice

		not	al
		shr	al, 2
		mov	ah, al
		WRITE_MIXER 06h, ah
		WRITE_MIXER 07h, ah
novoice:
		mov	eax, VolLine
		cmp	eax, -1
		je	noline

		not	al
		shr	al, 4
		mov	ah, al
		WRITE_MIXER 02h, ah
		WRITE_MIXER 03h, ah
noline:
		mov	ah, 000000b
		ret

SetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerVolume  PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dx, word ptr[BASE_ADDR]
		add	dx, 4
		READ_MIXER 06h
		not	al
		shl	al, 2
		and	eax, 255
		mov	dword ptr[ecx], eax
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		READ_MIXER 02h
		not	al
		shl	al, 4
		and	eax, 255
		mov	dword ptr[ecx], eax
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 255
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 0
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 255
nomaster:
		ret

GetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerInfo	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dword ptr[ecx], 1
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 1
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 0
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 0
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 0
nomaster:
		ret

GetMixerInfo	ENDP

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF   SBLASTER

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

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF	SB16

ListVars		equ	$
BASE_ADDR               dd      220h		; By default 220h

;----------------------------------------------------------------------------

SetMixerVolume	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]         ; MIXER
		add	dl, 4				; offset 4

		WRITE_MIXER 3Ch, 00011110b		; disable MIC
		WRITE_MIXER 3Bh, 0			; disable PC Speaker

		mov	ecx, VolVoice
		cmp	ecx, -1
		je	novoice

		WRITE_MIXER 32h, cl
		WRITE_MIXER 33h, cl
novoice:
		mov	ecx, VolMidi
		cmp	ecx, -1
		je	nomidi

		WRITE_MIXER 34h, cl
		WRITE_MIXER 35h, cl
nomidi:
		mov	ecx, VolCD
		cmp	ecx, -1
		je	nocd

		WRITE_MIXER 36h, cl
		WRITE_MIXER 37h, cl
nocd:
		mov	ecx, VolLine
		cmp	ecx, -1
		je	noline

		WRITE_MIXER 38h, cl
		WRITE_MIXER 39h, cl
noline:
		mov	ecx, VolMaster
		cmp	ecx, -1
		je	nomaster

		WRITE_MIXER 30h, cl
		WRITE_MIXER 31h, cl
nomaster:
		ret

SetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerVolume  PROC	uses EBX,\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]         ; MIXER
		add	dl, 4				; offset 4

		READ_MIXER 32h
		mov	cl, al
		READ_MIXER 34h
		mov	ch, cl
		READ_MIXER 36h
		mov	bl, al
		READ_MIXER 38h
		mov	bh, al
		READ_MIXER 30h

		mov	edx, VolMaster
		or	edx, edx
		jz	nomaster

		and	eax, 11111000b
		mov	[edx], eax
nomaster:
		mov	edx, VolLine
		or	edx, edx
		jz	noline

		mov	al, bh
		and	eax, 11111000b
		mov	[edx], eax
noline:
		mov	edx, VolCD
		or	edx, edx
		jz	nocd

		and	ebx, 11111000b
		mov	[edx], ebx
nocd:
		mov	edx, VolMidi
		or	edx, edx
		jz	nomidi

		mov	al, ch
		and	eax, 11111000b
		mov	[edx], eax
nomidi:
		mov	edx, VolVoice
		or	edx, edx
		jz	novoice

		and	ecx, 11111000b
		mov	[edx], ecx
novoice:
		ret

GetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerInfo	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dword ptr[ecx], 1
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 1
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 1
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 1
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 1
nomaster:
		ret

GetMixerInfo	ENDP

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF SBPRO

ListVars		equ	$
BASE_ADDR               dd      220h		; By default 220h

;----------------------------------------------------------------------------

SetMixerVolume	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]         ; MIXER
		add	dl, 4				; offset 4

		WRITE_MIXER 0Ah , 0			; MIC off

		mov	eax, VolVoice
		cmp	eax, -1
		je	novoice

		mov	ah, al
		shr	ah, 4
		and	al, 11110000b
		or	ah, al
		and	ah, 11101110b
		WRITE_MIXER 04h, ah
novoice:
		mov	eax, VolMidi
		cmp	eax, -1
		je	nomidi

		mov	ah, al
		shr	ah, 4
		and	al, 11110000b
		or	ah, al
		and	ah, 11101110b
		WRITE_MIXER 26h, ah
nomidi:
		mov	eax, VolCD
		cmp	eax, -1
		je	nocd

		mov	ah, al
		shr	ah, 4
		and	al, 11110000b
		or	ah, al
		and	ah, 11101110b
		WRITE_MIXER 28h, ah
nocd:
		mov	eax, VolLine
		cmp	eax, -1
		je	noline

		mov	ah, al
		shr	ah, 4
		and	al, 11110000b
		or	ah, al
		and	ah, 11101110b
		WRITE_MIXER 2Eh, ah
noline:
		mov	eax, VolMaster
		cmp	eax, -1
		je	nomaster

		mov	ah, al
		shr	ah, 4
		and	al, 11110000b
		or	ah, al
		and	ah, 11101110b
		WRITE_MIXER 22h, ah
nomaster:
		ret

SetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerVolume  PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]         ; MIXER
		add	dl, 4				; offset 4

		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		READ_MIXER 04h
		and	eax, 11100000b
		mov	[ecx], eax
novoice:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		READ_MIXER 26h
		and	eax, 11100000b
		mov	[ecx], eax
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		READ_MIXER 28h
		and	eax, 11100000b
		mov	[ecx], eax
nocd:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		READ_MIXER 2Eh
		and	eax, 11100000b
		mov	[ecx], eax
noline:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		READ_MIXER 22h
		and	eax, 11100000b
		mov	[ecx], eax
nomaster:
		ret

GetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerInfo	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dword ptr[ecx], 1
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 1
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 1
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 1
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 1
nomaster:
		ret

GetMixerInfo	ENDP

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF SB2CD

ListVars		equ	$
BASE_ADDR               dd      220h		; By default 220h

;----------------------------------------------------------------------------

SetMixerVolume	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]         ; MIXER
		add	dl, 4				; offset 4

		mov	eax, VolVoice
		cmp	eax, -1
		je	novoice

		shl	ax, 3
		and	ah, 00000110b
		WRITE_MIXER 0Ah, ah
novoice:
		mov	eax, VolMidi
		cmp	eax, -1
		je	nomidi

		shl	ax, 4
		and	ah, 00001110b
		WRITE_MIXER 06h, ah
nomidi:
		mov	eax, VolCD
		cmp	eax, -1
		je	nocd

		shl	ax, 4
		and	ah, 00001110b
		WRITE_MIXER 08h, ah
nocd:
		mov	eax, VolMaster
		cmp	eax, -1
		je	nomaster

		shl	ax, 4
		and	ah, 00001110b
		WRITE_MIXER 02h, ah
nomaster:
		ret

SetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerVolume  PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]         ; MIXER
		add	dl, 4				; offset 4

		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		READ_MIXER 0Ah
		and	eax, 00000110b
		shl	eax, 5
		mov	[ecx], eax
novoice:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		READ_MIXER 06h
		and	eax, 00001110b
		shl	eax, 4
		mov	[ecx], eax
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		READ_MIXER 08h
		and	eax, 00001110b
		shl	eax, 4
		mov	[ecx], eax
nocd:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 0
noline:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		READ_MIXER 02h
		and	eax, 00001110b
		shl	eax, 4
		mov	[ecx], eax
nomaster:
		ret

GetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerInfo	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dword ptr[ecx], 1
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 0
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 1
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 1
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 1
nomaster:
		ret

GetMixerInfo	ENDP

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

IFDEF GOLD

ListVars		equ	$
BASE_ADDR               dd      388h		; By default 388h

;----------------------------------------------------------------------------

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

		ENDM

READ_MIXER      MACRO   PORT
		local   wait1, wait2

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

		ENDM

;----------------------------------------------------------------------------

SetMixerVolume	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]
		add	dx, 2				; MIXER

		SELECT_MIXER

		WRITE_MIXER 0Fh, 10000000b		; MIC off
		WRITE_MIXER 10h, 10000000b		; TEL off

		mov	ecx, VolVoice
		cmp	ecx, -1
		je	novoice

		shr	cl, 1
		or	cl, 10000000b
		WRITE_MIXER 0Bh, cl
		WRITE_MIXER 0Ch, cl
novoice:
		mov	ecx, VolMidi
		cmp	ecx, -1
		je	nomidi

		shr	cl, 1
		or	cl, 10000000b
		WRITE_MIXER 09h, cl
		WRITE_MIXER 0Ah, cl
nomidi:
		mov	ecx, VolCD
		cmp	ecx, -1
		je	noCD

		shr	cl, 1
		or	cl, 10000000b
		WRITE_MIXER 0Dh, cl
		WRITE_MIXER 0Eh, cl
noCD:
		mov	ecx, VolMaster
		cmp	ecx, -1
		je	nomaster

		shr	cl, 2
		or	cl, 11000000b
		WRITE_MIXER 04h, cl
		WRITE_MIXER 05h, cl
nomaster:
		LEAVE_MIXER

		ret

SetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerVolume  PROC	uses EBX, \
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD

		mov     dx, word ptr[BASE_ADDR]
		add	dx, 2				; MIXER

		SELECT_MIXER

		READ_MIXER 0Bh
		mov	cl, al
		READ_MIXER 09h
		mov	ch, al
		READ_MIXER 0Dh
		mov	bl, al
		READ_MIXER 04h
		mov	bh, al

		LEAVE_MIXER

		xor	eax, eax

		mov	edx, VolVoice
		or	edx, edx
		jz	novoice

		shl	cl, 1
		mov	al, cl
		mov	[edx], eax
novoice:
		mov	edx, VolMidi
		or	edx, edx
		jz	nomidi

		shl	ch, 1
		mov	al, ch
		mov	[edx], eax
nomidi:
		mov	edx, VolCD
		or	edx, edx
		jz	nocd

		shl	bl, 1
		mov	al, bl
		mov	[edx], eax
nocd:
		mov	edx, VolLine
		or	edx, edx
		jz	noline

		mov	dword ptr[edx], 0
noline:
		mov	edx, VolMaster
		or	edx, edx
		jz	nomaster

		shl	bh, 2
		mov	al, bh
		mov	[edx], eax
nomaster:
		ret

GetMixerVolume  ENDP

;----------------------------------------------------------------------------

GetMixerInfo	PROC	\
		VolVoice:DWORD, VolMidi:DWORD, VolCD:DWORD, \
		VolLine:DWORD, VolMaster:DWORD


		mov	ecx, VolVoice
		or	ecx, ecx
		jz	novoice

		mov	dword ptr[ecx], 1
novoice:
		mov	ecx, VolLine
		or	ecx, ecx
		jz	noline

		mov	dword ptr[ecx], 0
noline:
		mov	ecx, VolMidi
		or	ecx, ecx
		jz	nomidi

		mov	dword ptr[ecx], 1
nomidi:
		mov	ecx, VolCD
		or	ecx, ecx
		jz	nocd

		mov	dword ptr[ecx], 1
nocd:
		mov	ecx, VolMaster
		or	ecx, ecx
		jz	nomaster

		mov	dword ptr[ecx], 1
nomaster:
		ret

GetMixerInfo	ENDP

ENDIF

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

		END
