;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
comment @

'		    ÛÛÛÛÛÛ¿ ÛÛÛÛÛ¿  ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ Û¿   Û¿
		    ÛÛÚÄÄÄÙ ÛÛÚÄÛÛ¿ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÛÛ¿ ÛÛ³
'		    ÛÛ³     ÛÛ³ ÛÛ³ ÛÛÛÛÛÛ³ ÛÛ³ ÛÛ³ ÛÛÛÛÛÛ³
		    ÛÛ³     ÛÛ³ ÛÛ³ ÛÛÚÛÛÚÙ ÛÛ³ ÛÛ³ ÛÛÚÄÛÛ³
'		    ÛÛÛÛÛÛ¿ ÛÛÛÛÛÚÙ ÛÛ³ÀÛÛ¿ ÛÛÛÛÛÛ³ ÛÛ³ ÛÛ³
'		    ÀÄÄÄÄÄÙ ÀÄÄÄÄÙ  ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ
'
'		    25 Juillet 1994
'
		    (c) Adeline Software International 1994
@
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			JUMPS
			.386P
			.model SMALL, C

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			.data
		public	NoLanguage	LeadOut
		public	NoLanguage	LowTrack
		public	NoLanguage	HigTrack
		public	NoLanguage	BufferData
		public	NoLanguage	CptTimeOut
		public	NoLanguage	DriveCDR

                public	NoLanguage	FileCD_Start
		public	NoLanguage	FileCD_Sect
		public	NoLanguage	FileCD_Size

TIME_OUT	equ	10


FileCD_Start    dd	0
FileCD_Sect	dd	0
FileCD_Size	dd	0


LowTrack	db	0
HigTrack	db	0
LeadOut		dd	0
DriveCDR	dw	-2
BufferData	db	1000 dup(0)
CptTimeOut	dw	0

PtDosCD		dd	?	;' Pointeur Prot sur Dos Mem
OffDosCd	dw	?	;' Offset Real sur Dos Mem

;----------------------------------' PtDosCD : Structure IOCTL

I_LENHDR	equ	0	;'	Byte 1
I_SUBUNIT	equ	1	;'	Byte 1
I_COMMAND	equ	2	;'	Byte 1
I_STATUS	equ	3	;'	Word 1
I_RESERVED	equ	5	;'	Byte 8

I_MEDIA		equ	13	;'	Byte 1
I_TRANSPTR	equ	14	;'	Long 1
I_NBBYTE	equ	18	;'	Word 1
I_STARTING	equ	20	;'	Word 1
I_PTRREQUEST	equ	22	;'      Long 1
I_DUMM		equ	26	;'	Byte 500

;----------------------------------' PtDosCD : End IOCTL, Begin BUFFERDATA

I_BUFFERDATA	equ	540	;'	Byte 1000
I_SIZE		equ	1540

;----------------------------------' PtDosCD : End BUFFERDATA

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


;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			.code

			public	NoLanguage	InitCDR
			public	NoLanguage	ClearCDR
			public	NoLanguage	GetCopyrightName
			public	NoLanguage	Red2Hsg
			public	NoLanguage	GetLengthTrackCDR
			public	NoLanguage	StartTrackCDR
			public	NoLanguage	ResumeCDR
			public	NoLanguage	EjectCDR
			public	NoLanguage	CloseTrayCDR
			public	NoLanguage	LockCDR
			public	NoLanguage	UnLockCDR
			public	NoLanguage	StatusCDR
			public	NoLanguage	GetMusicCDR
			public	NoLanguage	ResetCDR
			public	NoLanguage	ControlCDR
			public	NoLanguage	LocateHeadCDR
			public	NoLanguage	PlayCDR
			public	NoLanguage	StopCDR
			public	NoLanguage	PlayTrackCDR
			public	NoLanguage	ReadLongCDR
			public	NoLanguage	GetBitCDR
			public	NoLanguage	WaitDisk
;'			public	NoLanguage	TestCDROM
			public	NoLanguage	GetFileCDR


;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TalkCDR_O		proc	uses ebx es

			mov	ebx, [PtDosCD]
			mov	word ptr [ebx+I_STATUS], 0

			mov	RMI_EAX, 1510h

			movzx	eax, DriveCDR
			mov	RMI_ECX, eax

			mov	bx, [OffDosCD]
			mov	RMI_ES, bx
			mov	RMI_EBX, 0

			mov	ax, ds
			mov	es, ax
			mov	edi, offset RMI
			mov	ax, 300h
			mov	bx, 2fh
			int	31h

			mov	ebx, [PtDosCD]
			mov	eax, [ebx+I_STATUS]

			ret
TalkCDR_O		endp

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AudioInfoCDR		proc	uses ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command Input
			mov	word ptr [ebx+I_NBBYTE], 7	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	[ebx+I_TRANSPTR], ax		;' Offset
			mov	ax, [OffDosCD]
			mov	[ebx+I_TRANSPTR+2], ax		;' Segment

			mov	byte ptr [ebx+I_BUFFERDATA], 10 ;' Audio Info

Retry:			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
			xor	ax, ax
			ret
NoError:
			mov	ebx, [PtDosCD]

			mov	al, [ebx+I_BUFFERDATA+1]
			mov	ah, [ebx+I_BUFFERDATA+2]

			mov	[LowTrack], al
			mov	[HigTrack], ah

			mov	eax, [ebx+I_BUFFERDATA+3]
			mov	[LeadOut], eax
			mov	ax, 1
			ret
AudioInfoCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
WaitDisk		proc
			local	timeout:DWORD

			mov	[timeout], 256*3
Again:
			call	StatusCDR
			test	ax, 100000000000b	;' Disk Present ?
			je	OkDisk
			dec	[timeout]
			jne	Again
OkDisk:
			xor	ax, ax
			ret
WaitDisk		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
InitCDR			proc	uses es esi edi ebx,\
			nameid:DWORD

			local	nbcddrive:WORD
			local	listdrive[26]:BYTE
			local	lennameid:DWORD
			local	save_ecx:DWORD
			local	timeout:DWORD

			mov	[DriveCDR], -2		;' Default No CD Drive

			xor	bx, bx
			mov	ax, 1500h		;' Get Drive Number
			int	2Fh

			or	bx, bx
			je	NoCDRom

			mov	[nbcddrive], bx
			mov	[DriveCDR], cx

;----------------------'Init PtDosCD en Mem Dos par DPMI

			mov	eax, 0100h		;' Dmpi Alloc Mem Dos
			mov	ebx, 128		;' 2048 ( 128*16 )
			int	31h

			mov	[OffDosCD], ax
			shl	eax, 4			;' Con real->Prot
			mov	[PtDosCD], eax

;----------------------'Calcul Taille nameid

			mov	edi, nameid		;' Len of nameid

			mov	ecx, 0FFh
			xor	al, al
			repne	scasb			;' Size+1 of nameid
			sub	ecx, 0FFh
			neg	ecx
			dec	ecx			;' Without 0 for ""
			mov	[lennameid], ecx

;----------------------'Recup List CD Drive & Copy in listdrive

			mov	RMI_EAX, 150Dh		;' Get List CD Drive
			mov	bx, [OffDosCD]
			mov	RMI_ES, bx
			mov	RMI_EBX, 0

			mov	ax, ds
			mov	es, ax
			mov	edi, offset RMI
			mov	ax, 300h
			mov	bx, 2fh
			int	31h			;' ES:BX List Letter

			mov	cx, [nbcddrive]
			mov	esi, [PtDosCD]
			lea	edi, listdrive
			rep	movsb			;' Save List CD Drive

;----------------------'Scan des lecteurs CD pour trouver nameid

			lea	ebx, listdrive
			xor	ecx, ecx
NextCDDrive:
			mov	[save_ecx], ecx

			xor	eax, eax

			mov	al, [ebx+ecx] 		;' Recup Drive Letter
			mov	[DriveCDR], ax

			call	CloseTrayCDR		;' CLOSETRAYCDR
			jne	NotGoodOne		;

			call	WaitDisk		;' WAITDISK
			jne	NotGoodOne

			call	LockCDR			;' LOCKCDR
			call	StopCDR			;' STOPCDR

			call	StatusCDR       	;' STATUSCDR 1ere
			test	ax, 8000h		;' Error
			je	NoError

			call	ResetCDR                ;' RESETCDR

			call	StatusCDR		;' STATUSCDR 2eme
			test	ax, 8000h
			jne	NotGoodOne

NoError:
			jne	NotGoodOne		;' Can t close

			cmp	[lennameid], 0
			je	TheGoodOne		;' No Copyright test

			call	GetCopyrightName	;' GETCOPYRIGHTNAME

			mov	esi, eax		;' Copyright Name
			mov	edi, nameid
			mov	ecx, [lennameid]

			rep	cmpsb			;' Compar with Copyright
			je	TheGoodOne
NotGoodOne:
			call	UnLockCDR
			mov	ecx, [save_ecx]
			inc	ecx
			cmp	cx, [nbcddrive]
			jne	NextCDDrive

			jmp	NoCDRom

;----------------------'GOTCHA :-)
TheGoodOne:
			call	StatusCDR
			test	ax, 2048		;' Pas de CD
			jne	NoCDRom
GetAudio:
			call	AudioInfoCDR
			or	ax, ax
			je	GetAudio		;' Ca doit passer!

			mov	eax, 1
			ret
NoCDRom:
			cmp	[DriveCDR], -2		;' No CD Drive
			je	ContError
			mov	[DriveCDR], -1          ;' No Adeline CD
ContError:
			call	UnLockCDR		;' UNLOCKCDR
			xor	eax, eax		;' No CDRom
			ret
InitCDR			endp

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ClearCDR		proc
			call	StopCDR
			call	UnLockCDR
			call	ResetCDR
			ret
ClearCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GetCopyrightName	proc	uses ebx

			mov	RMI_EAX, 1502h		;' Get List CD Drive
			mov	bx, [OffDosCD]
			mov	RMI_ES, bx
			mov	RMI_EBX, 0

			mov	ax, ds
			mov	es, ax
			mov	edi, offset RMI
			mov	ax, 300h
			mov	bx, 2fh
			int	31h          		;' ES:BX List Letter

			mov	eax, [PtDosCD]
			ret

GetCopyrightName	endp

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Red2Hsg			proc	,\
			value:DWORD

			mov	eax, value

			and	eax, 00FF0000h		;' Recup Min
			shr	eax, 14 		;' Normal 16 mais...
			mov	edx, eax		;' ebx=v*4

			shl	eax, 4			;' *64 ( eh oui! )
			sub	eax, edx		;' min*60

			mov	edx, value

			and	edx, 0000FF00h		;' Recup Sec
			shr	edx, 8  		;' >> 8
			add	eax, edx		;'min*60+sec

			mov	edx, 75			;' flemme decalage :-(
			imul	edx                     ;'(min*60+sec)*75

			mov	edx, value
			and	edx, 000000FFh		;' Recup frame (100e)

			add	eax, edx		;'(min*60+sec)*75+frame

			ret
Red2Hsg			endp

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

GetLengthTrackCDR	proc	uses ebx,\
			track:DWORD

			local	begin:DWORD

			mov	ecx, [track]

			push	ecx
			call	StartTrackCDR		;' b = Debut Track
			pop	ecx

			mov	[begin], eax

			mov	ebx, [LeadOut]
			cmp	cl, [HigTrack]
			jge	OkLastOne

			inc	ecx

			push	ecx
			call	StartTrackCDR		;' e = Debut Track+1
			add	esp, 4

			mov	ebx, eax
OkLastOne:
			push	ebx
			call	Red2Hsg
			mov	ebx, eax

			push	[begin]
			call	Red2Hsg
			add	esp, 8

			sub	ebx, eax		;' R2H(b)-R2H(e)
			mov	eax, ebx

			ret
GetLengthTrackCDR	endp

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

StartTrackCDR		proc	uses ebx,\
			Track:DWORD

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command Input
			mov	word ptr [ebx+I_NBBYTE], 7	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 11 ;' Audio Info
			mov	eax, Track
			mov	byte ptr [ebx+I_BUFFERDATA+1], al
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			mov	ebx, [PtDosCD]
			mov	eax, dword ptr [ebx+I_BUFFERDATA+2]

			ret
StartTrackCDR		endp

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ResumeCDR		proc	uses ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13
			mov	byte ptr [ebx+I_COMMAND], 136
Retry:
			call	TalkCDR_O
			test	ax, 8000h		;' Bit 15
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
ResumeCDR			endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EjectCDR		proc	uses ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 12	;' Command IOCTLO
			mov	word ptr [ebx+I_NBBYTE], 1	;' Nb Byte Sub Funct

			mov	ax,  I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 0 	;' Command Eject
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
EjectCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CloseTrayCDR		proc	uses ebx
			local	timeout:DWORD

			call	StatusCDR
			test	ax, 1			;' Door
			je	OkRoule			;' Aready Closed!

;-------------------------------------------------------

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 12	;' Command IOCTLO
			mov	word ptr [ebx+I_NBBYTE], 1	;' Nb Byte Sub Funct

			mov	ax,  I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 5 	;' Command CloseTray
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
			ret
NoError:
;-------------------------------------------------------
			mov	[timeout], 512
Again_0:
			call	StatusCDR
			test	ax, 1b			;' Door
			je	OkRoule
			dec	[timeout]
			jne	Again_0
;-------------------------------------------------------
			ret
OkRoule:
			xor	eax, eax
			ret
CloseTrayCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LockCDR			proc

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 12	;' Command IOCTLO
			mov	word ptr [ebx+I_NBBYTE], 2	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 1 	;' Command Lock Unlock
			mov	byte ptr [ebx+I_BUFFERDATA+1], 1;' Lock
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
LockCDR			endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UnLockCDR		proc	uses ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 12	;' Command IOCTLO
			mov	word ptr [ebx+I_NBBYTE], 2	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 1 	;' Command Lock Unlock
			mov	byte ptr [ebx+I_BUFFERDATA+1], 0;' Lock
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
UnLockCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StatusCDR		proc	uses	ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command IOCTLI
			mov	word ptr [ebx+I_NBBYTE], 5	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 6 	;' Command Status
			mov	dword ptr [ebx+I_BUFFERDATA+1], 0;' Status
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
			xor	eax, eax
			ret
NoError:
			mov	ebx, [PtDosCD]
			movzx	eax, word ptr [ebx+I_BufferData+1]
			ret

StatusCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GetMusicCDR		proc	uses	ebx

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command IOCTLI
			mov	word ptr [ebx+I_NBBYTE], 11	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 0Ch;' Locate Head
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
NoPlay:
			xor	eax, eax
			ret
NoError:
			mov	ebx, [PtDosCD]
			test	ax, 200h			;' Buzy
			jz	NoPlay
			movzx	eax, byte ptr [ebx+I_BufferData+2];' Num Track
			ret

GetMusicCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GetBitCDR		proc	uses	ebx

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command IOCTLI
			mov	word ptr [ebx+I_NBBYTE], 5	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 6 	;' Command Status
			mov	dword ptr [ebx+I_BUFFERDATA+1], 0;' Status

			call	TalkCDR_O

			ret
GetBitCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ResetCDR		proc	uses	ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 26	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 12	;' Command IOCTLO
			mov	word ptr [ebx+I_NBBYTE], 1	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 2 	;' Command Reset
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
ResetCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ControlCDR		proc	uses ebx,\
			chan0:WORD, vol0:WORD, chan1:WORD, vol1:WORD


			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 12	;' Command IOCTLO
			mov	word ptr [ebx+I_NBBYTE], 9	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 3 	;' Command Control
			mov	ax, chan0
			mov	byte ptr [ebx+I_BUFFERDATA+1], al
			mov	ax, vol0
			mov	byte ptr [ebx+I_BUFFERDATA+2], al
			mov	ax, chan1
			mov	byte ptr [ebx+I_BUFFERDATA+3], al
			mov	ax, vol1
			mov	byte ptr [ebx+I_BUFFERDATA+4], al
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
ControlCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LocateHeadCDR		proc	uses ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command Input
			mov	word ptr [ebx+I_NBBYTE], 6	;' Nb Byte Sub Funct
			mov	byte ptr [ebx+I_MEDIA], 1	;' Mode Red Book

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 1 	;' Command Locate
			mov	byte ptr [ebx+I_BUFFERDATA+1], 0;' Mode Address
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			mov	ebx, [PtDosCD]
			mov	eax, dword ptr [ebx+I_BUFFERDATA+2]
			ret
LocateHeadCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PlayCDR			proc	uses ebx,\
			Debut:DWORD, Duree:DWORD

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]
Retry:
			mov	byte ptr [ebx+I_LENHDR], 26	;' Len
			mov	byte ptr [ebx+I_COMMAND], 132	;' Play
			mov	byte ptr [ebx+I_MEDIA], 1	;' Mode Red Book
;-----------------------
			mov	eax, Duree
			mov	dword ptr [ebx+I_NBBYTE], eax

			mov	eax, Debut
			mov	dword ptr [ebx+I_TRANSPTR], eax
;-----------------------
			call	TalkCDR_O
			test	ax, 200h			;' Buzy bit 9
			jnz	Ok

			dec	word ptr [CptTimeOut]
			jne	Retry

			mov	eax, 1
			ret
Ok:
			xor	eax, eax
			ret
PlayCDR 		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StopCDR			proc	uses ebx

			mov	[CptTimeOut], TIME_OUT

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13
			mov	byte ptr [ebx+I_COMMAND], 133
Retry:
			call	TalkCDR_O
			test	ax, 8000h		;' Bit 15
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
NoError:
			ret
StopCDR			endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PlayTrackCDR		proc	uses ebx,\
			Track:DWORD

			local	len:DWORD

			push	[track]
			call	GetLengthTrackCDR
			add	esp, 4
			mov	[len], eax

			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 13	;' Longueur Command
			mov	byte ptr [ebx+I_COMMAND], 3	;' Command Input
			mov	word ptr [ebx+I_NBBYTE], 7	;' Nb Byte Sub Funct

			mov	ax, I_BUFFERDATA
			mov	word ptr [ebx+I_TRANSPTR], ax
			mov	ax, [OffDosCD]
			mov     word ptr [ebx+I_TRANSPTR+2], ax

			mov	byte ptr [ebx+I_BUFFERDATA], 11 ;' Audio Info
			mov	eax, Track
			mov	byte ptr [ebx+I_BUFFERDATA+1], al
Retry:
			call	TalkCDR_O
			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Retry
			ret
NoError:
			push	[len]
			mov	eax, dword ptr [ebx+I_BUFFERDATA+2]
			push	eax
			call	PlayCDR
			add	esp, 8

			ret
PlayTrackCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
comment @
TestCDROM		proc	uses ebx

			xor	eax, eax
			mov	ebx, 1

			push    eax
			push	[PtDosCD]
			push	ebx
			push	eax
			call	ReadLongCDR
			add	esp, 4*4
			or	eax, eax
			je	NoError
			xor	eax, eax
			ret
NoError:
			mov	eax, 1
			ret
TestCDROM		endp
@
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ReadLongCDR		proc	uses es esi edi ebx,\
			Start:DWORD, \
			NbSect:DWORD, \
			BufferTrans:DWORD

			mov	[CptTimeOut], TIME_OUT
Error:
			mov	ebx, [PtDosCD]

			mov	byte ptr [ebx+I_LENHDR], 27 	;' 27 Len
			mov	byte ptr [ebx+I_SUBUNIT], 0	;' Drive
			mov	byte ptr [ebx+I_COMMAND], 128	;' Read_L
			mov	word ptr [ebx+I_STATUS], 0	;' Status
;---------------------------------------------------------------
			mov	byte ptr [ebx+I_MEDIA], 0	;' Mode HSG
;---------------------------------------------------------------
			mov	eax, BufferTrans		;' Addresse Dest
			shr	eax, 4				;' Prot->Real
			mov	word ptr [ebx+I_TRANSPTR], 0
			mov	word ptr [ebx+I_TRANSPTR+2], ax
;---------------------------------------------------------------
			mov	eax, NbSect			;' Nb Sector
			mov	word ptr [ebx+I_TRANSPTR+4], ax
;---------------------------------------------------------------
			mov	eax, Start         		;' Start Sector
			mov	dword ptr [ebx+I_TRANSPTR+6], eax
;---------------------------------------------------------------
			mov	byte ptr [ebx+I_TRANSPTR+10], 0	;' Mode Cooked
			mov	word ptr [ebx+I_TRANSPTR+11], 0	;' Interl Size+Skip
;---------------------------------------------------------------

			mov	RMI_EAX, 1510h
			movzx	eax, DriveCDR
			mov	RMI_ECX, eax
			mov	bx, [OffDosCD]
			mov	RMI_ES, bx
			mov	RMI_EBX, 0
			mov	ax, ds
			mov	es, ax
			mov	edi, offset RMI
			mov	ax, 300h
			mov	bx, 2fh
			int	31h
			mov	ebx, [PtDosCD]
			mov	ax, word ptr [ebx+I_STATUS]

			test	ax, 8000h
			jz	NoError
			dec	[CptTimeOut]
			jne	Error
			ret
NoError:
			xor	eax, eax
			ret
ReadLongCDR 		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GetFileCDR		proc	uses esi edi ebx,\
			NameF:DWORD

;--------------------------------------------
			mov	edi, NameF
			mov	ecx, 0FFh
			xor	al, al
			repne	scasb			;' Size+1 of nameid
			sub	ecx, 0FFh
			neg	ecx
			mov	esi, NameF
			mov	edi, [PtDosCD]
			rep	movsb			;' Copy NameF low mem
;--------------------------------------------

			mov	RMI_EAX, 150Fh
			movzx	eax, DriveCDR
			mov	ah, 1			;'
			mov	RMI_ECX, eax
			movzx	ebx, [OffDosCD]
			mov	RMI_ES, bx		;' ES:BX File Name
			mov	RMI_EBX, 0

			mov	RMI_ESI, ebx		;' Segment
			mov	RMI_EDI, 1024		;' Offset

			mov	edi, offset RMI
			mov	ax, 300h
			mov	bx, 2fh
			int	31h
;--------------------------------------------------------

			mov	eax, RMI_EAX

			test	ax, 8000h
			jnz	Error


			mov	ebx, [PtDosCD]
			add	ebx, 1024

			inc	ebx
			mov	eax, [ebx]
			mov	[FileCD_Start], eax
			add	ebx, 6
			mov	eax, [ebx]
			mov	[FileCD_Size], eax

			add	eax, 2048
			shr	eax, 11			; (Size+2048)/2048
			mov	[FileCD_Sect], eax

			ret
Error:
			xor	eax, eax
			ret

GetFileCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ifdef	DEBUG
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OpenCDR			proc
			mov	byte ptr LenHDR, 13
			mov	byte ptr Command, 13
Retry:			call	TalkCDR_O
			test	ax, 8000h		; Bit 15
			jz	NoError
			jmp	Retry
NoError:		ret
OpenCDR			endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CloseCDR		proc
			mov	byte ptr LenHDR, 13
			mov	byte ptr Command, 14
Retry:			call	TalkCDR_O
			test	ax, 8000h		; Bit 15
			jz	NoError
			jmp	Retry
NoError:		ret
CloseCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SeekCDR			proc ,\
			DebutL:WORD, DebutH:WORD

			mov	byte ptr LenHdr, 24
			mov	byte ptr Command, 131
			mov	byte ptr Media, 1
			mov	ax, DebutL
			mov	word ptr [Starting], ax
			mov	ax, DebutH
			mov	word ptr [Starting+2], ax
Retry:			call	TalkCDR_O
			test	ax, 8000h		; Bit 15
			jz	NoError
			jmp	Retry
NoError:		ret
SeekCDR			endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GetFileCDR		proc	uses esi edi ebx,\
			NameF:DWORD, LowBuffer:DWORD

;--------------------------------------------
			mov	edi, NameF
			mov	ecx, 0FFh
			xor	al, al
			repne	scasb			;' Size+1 of nameid
			sub	ecx, 0FFh
			neg	ecx
			mov	esi, NameF
			mov	edi, [PtDosCD]
			rep	movsb			;' Copy NameF low mem
;--------------------------------------------

			mov	RMI_EAX, 150Fh
			movzx	eax, DriveCDR
			mov	ah, 1			;'
			mov	RMI_ECX, eax
			mov	bx, [OffDosCD]
			mov	RMI_ES, bx		;' ES:BX File Name
			mov	RMI_EBX, 0

			mov	eax, LowBuffer
			shr	eax, 4			;' Prot -> Real

			mov	RMI_ESI, eax		;' Segment
			shr	eax, 16			;' Zero, i know :-[
			mov	RMI_EDI, eax		;' Offset

			mov	edi, offset RMI
			mov	ax, 300h
			mov	bx, 2fh
			int	31h
;--------------------------------------------------------

			mov	eax, RMI_EAX
			ret

GetFileCDR		endp
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
endif
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


;				The
				End















