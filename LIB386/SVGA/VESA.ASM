;----------------------------------------------------------------------------
			.486
			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
VESA_WINDOWED		EQU	01b
VESA_LINEAR             EQU     10b
VESA_LINEAR_OR_WINDOWED	EQU     11b
VESAMODE_LINEAR		EQU	(1 shl 14)

;----------------------------------------------------------------------------
Struc_VbeInfoBlock	STRUC

  VbeSignature		db	"VESA"
  VbeVersion		dw	0200h
  VbeOemStringPtr	dd	?
  VbeCapabilities	dd	?
  VbeVideoModePtr	dd	?
  VbeTotalMemory	dw	?

  VbeOemSoftwareRev	dw	?
  VbeOemVendorNamePtr	dd	?
  VbeOemProductNamePtr	dd	?
  VbeOemProductRevPtr	dd	?
  VbeReserved		db	222 dup (?)

  VbeOemData		db	256 dup (?)

Struc_VbeInfoBlock	ENDS

;----------------------------------------------------------------------------
Struc_ModeInfoBlock	STRUC

  ModeAttributes	dw	?
  ModeWinAAttributes	db	?
  ModeWinBAttributes	db	?
  ModeWinGranularity	dw	?
  ModeWinSize		dw	?
  ModeWinASegment	dw	?
  ModeWinBSegment	dw	?
  ModeWinFuncPtr	dd	?
  ModeBytesPerScanLine	dw	?

  ModeXResolution	dw	?
  ModeYResolution	dw	?
  ModeXCharSize		db	?
  ModeYCharSize		db	?
  ModeNumberOfPlanes	db	?
  ModeBitsPerPixel	db	?
  ModeNumberOfBanks	db	?
  ModeMemoryModel	db	?
  ModeBankSize		db	?
  ModeNumberOfImagePages db	?
			db	?

  ModeRedMaskSize	db	?
  ModeRedFieldPosition	db	?
  ModeGreenMaskSize	db	?
  ModeGreenFieldPosition db	?
  ModeBlueMaskSize	db	?
  ModeBlueFieldPosition	db	?
  ModeRsvdMaskSize	db	?
  ModeRsvdFieldPosition	db	?
  ModeDirectColorModeInfo db	?

  ModePhysBasePtr	dd	?
  ModeOffScreenMemOffset dd	?
  ModeOffScreenMemSize	dd	?

  ModeReserved		db	206 dup (?)

Struc_ModeInfoBlock	ENDS

;----------------------------------------------------------------------------
			ALIGN	4
R_EDI			dd	0
R_ESI			dd	0
R_EBP			dd	0
			dd	0
R_EBX			dd	0
R_EDX			dd	0
R_ECX			dd	0
R_EAX			dd	0
R_Flags			dw	0
R_ES			dw	0
R_DS			dw	0
R_FS			dw	0
R_GS			dw	0
R_IP			dw	0
R_CS			dw	0
R_SP			dw	0
R_SS			dw	0

;----------------------------------------------------------------------------
			Extrn	SYSCALL		PhysHard	:DWORD
			Extrn	SYSCALL		Phys		:DWORD

			Extrn	SYSCALL		ModeResX	:DWORD
			Extrn	SYSCALL		ModeResY	:DWORD
			Extrn	SYSCALL		ModeMemory	:DWORD

			Extrn	SYSCALL		ModePages	:DWORD

			Extrn	SYSCALL		ModeDesiredDepth:DWORD
			Extrn	SYSCALL		ModeDesiredX	:DWORD
			Extrn	SYSCALL		ModeDesiredY	:DWORD

			Extrn	SYSCALL		BytesScanLine	:DWORD

;----------------------------------------------------------------------------
			Extrn	SYSCALL		QuietLog	:DWORD

;----------------------------------------------------------------------------
			Public	SYSCALL		PtrVbeInfoBlock
			Public	SYSCALL		PtrModeInfoBlock

			Public	SYSCALL		VESAMode

			Public	SYSCALL		BankSize

			Public	SYSCALL		SetPaletteVESA
			Public	SYSCALL		SwitchBankVESA

			Public	SYSCALL		VirtualLinearFirst

;----------------------------------------------------------------------------
			ALIGN	4

SetPaletteVESA		dd	0	; Pointeur sur une fonction de palette

SwitchBankVESA		dd	0	; Pointeur sur une fonction de changement de bank
VESABank		dd	0

NB_OFFSET		equ	(6*3+2*3+6*3)

TabWrite                dd      offset CopyBoxVESAI1
                        dd      offset CopyBoxVESAI2
                        dd      offset CopyBoxVESAI3
                        dd      offset CopyBoxVESAI4
                        dd      offset CopyBoxVESAI5
                        dd      offset CopyBoxVESAI6

                        dd      offset CopyBoxVESAF1
                        dd      offset CopyBoxVESAF2
                        dd      offset CopyBoxVESAF3
                        dd      offset CopyBoxVESAF4
                        dd      offset CopyBoxVESAF5
                        dd      offset CopyBoxVESAF6

                        dd      offset CopyBoxVESAM1
                        dd      offset CopyBoxVESAM2
                        dd      offset CopyBoxVESAM3
                        dd      offset CopyBoxVESAM4
                        dd      offset CopyBoxVESAM5
                        dd      offset CopyBoxVESAM6

                        dd      offset BlitBoxVESAI1
                        dd      offset BlitBoxVESAI2

                        dd      offset BlitBoxVESAF1
                        dd      offset BlitBoxVESAF2

                        dd      offset BlitBoxVESAM1
                        dd      offset BlitBoxVESAM2

			dd      offset ClearBoxVESAI1
                        dd      offset ClearBoxVESAI2
                        dd      offset ClearBoxVESAI3
                        dd      offset ClearBoxVESAI4
                        dd      offset ClearBoxVESAI5
                        dd      offset ClearBoxVESAI6

                        dd      offset ClearBoxVESAF1
                        dd      offset ClearBoxVESAF2
                        dd      offset ClearBoxVESAF3
                        dd      offset ClearBoxVESAF4
                        dd      offset ClearBoxVESAF5
                        dd      offset ClearBoxVESAF6

                        dd      offset ClearBoxVESAM1
                        dd      offset ClearBoxVESAM2
                        dd      offset ClearBoxVESAM3
                        dd      offset ClearBoxVESAM4
                        dd      offset ClearBoxVESAM5
                        dd      offset ClearBoxVESAM6

TabJump	                dd      offset I1
                        dd      offset I2
                        dd      offset I3
                        dd      offset I4
                        dd      offset I5
                        dd      offset I6

                        dd      offset F1
                        dd      offset F2
                        dd      offset F3
                        dd      offset F4
                        dd      offset F5
                        dd      offset F6

                        dd      offset M1
                        dd      offset M2
                        dd      offset M3
                        dd      offset M4
                        dd      offset M5
                        dd      offset M6

                        dd	offset I7
                        dd	offset I8

                        dd	offset F7
                        dd	offset F8

                        dd	offset M7
                        dd	offset M8

	                dd      offset I1
                        dd      offset I2
                        dd      offset I3
                        dd      offset I4
                        dd      offset I5
                        dd      offset I6

                        dd      offset F1
                        dd      offset F2
                        dd      offset F3
                        dd      offset F4
                        dd      offset F5
                        dd      offset F6

                        dd      offset M1
                        dd      offset M2
                        dd      offset M3
                        dd      offset M4
                        dd      offset M5
                        dd      offset M6

;----------------------------------------------------------------------------
			ALIGN	4

LinearSel		dd	0

Old_GPF_Off		dd	0
Old_GPF_Sel		dd	0

SegVbeInfoBlock		dw	0			; Segment real du VbeInfoBlock
SelVbeInfoBlock		dw	0			; Son selecteur
PtrVbeInfoBlock		dd	0			; Ptr flat sur la VbeInfoBlock
PtrModeInfoBlock	dd	0			; Ptr flat sur la ModeInfoBlock

VESALastMode		dd	-1			;
VESALastSizeChar	dd	-1			;

VESAMode		dd	-1			; Mode VESA found

BankSize		dd	0

VESAModeMemory		dd	0

PtrLinearMemPort	dd	0			; FLAT memory-mapped ports address
PtrProtectedAPI		dd	0			; PTR sur les routines VESA protegees

ModeDesiredMemory	dd	0

PaletteMode		dd	0

;----------------------------------------------------------------------------
; Mode 256
Mode_256		label	DWORD

;----------------------------------------------------------------------------
;	Mode 13h
			dw	13h	; mode
			dw	320	; XRes
			dw	200	; YRes

;----------------------------------------------------------------------------
;	Mode 100h
			dw	100h	; mode
			dw	640	; XRes
			dw	400	; YRes

;----------------------------------------------------------------------------
;	Mode 101h
			dw	101h	; mode
			dw	640	; XRes
			dw	480	; YRes

;----------------------------------------------------------------------------
;	Mode 103h
			dw	103h	; mode
			dw	800	; XRes
			dw	600	; YRes

;----------------------------------------------------------------------------
;	Mode 105h
			dw	105h	; mode
			dw	1024	; XRes
			dw	768	; YRes

;----------------------------------------------------------------------------
;	Mode 107h
			dw	107h	; mode
			dw	1280	; XRes
			dw	1024	; YRes

;----------------------------------------------------------------------------
;	End Modes
			dw	0FFFFh	; end of list

;----------------------------------------------------------------------------
; Other data 256 colors
Data_256		label	DWORD

			db	8	; char cell X (don't care)
			db	8	; char cell Y (don't care)
			db	1	; planes
			db	8	; bits per pixel
			db	1	; banks
			db	4	; memory model
			db	0	; bank size
			db	0	; number of image pages

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
IFDEF	REG_CALL
			extrn	SYSCALL		malloc_	:PROC
			extrn	SYSCALL		free_	:PROC
ELSE
			extrn	SYSCALL		malloc	:PROC
			extrn	SYSCALL		free	:PROC
ENDIF

;----------------------------------------------------------------------------
			public	SYSCALL		InitVESA
			public	SYSCALL		EndVESA
			public	SYSCALL		ScanVESAModes
			public	SYSCALL		DetectVESAMode
			public	SYSCALL		InitVESAMode
			public	SYSCALL		DetectInitVESAMode

			public	SYSCALL		SetPaletteOneVESA

			Public	SYSCALL		VblVESA
			Public	SYSCALL		PaletteVblVESA

;----------------------------------------------------------------------------
                        extrn   SYSCALL CopyBoxVESAM1:DWORD
                        extrn   SYSCALL CopyBoxVESAM2:DWORD
                        extrn   SYSCALL CopyBoxVESAM3:DWORD
                        extrn   SYSCALL CopyBoxVESAM4:DWORD
                        extrn   SYSCALL CopyBoxVESAM5:DWORD
                        extrn   SYSCALL CopyBoxVESAM6:DWORD

                        extrn   SYSCALL CopyBoxVESAF1:DWORD
                        extrn   SYSCALL CopyBoxVESAF2:DWORD
                        extrn   SYSCALL CopyBoxVESAF3:DWORD
                        extrn   SYSCALL CopyBoxVESAF4:DWORD
                        extrn   SYSCALL CopyBoxVESAF5:DWORD
                        extrn   SYSCALL CopyBoxVESAF6:DWORD

                        extrn   SYSCALL CopyBoxVESAI1:DWORD
                        extrn   SYSCALL CopyBoxVESAI2:DWORD
                        extrn   SYSCALL CopyBoxVESAI3:DWORD
                        extrn   SYSCALL CopyBoxVESAI4:DWORD
                        extrn   SYSCALL CopyBoxVESAI5:DWORD
                        extrn   SYSCALL CopyBoxVESAI6:DWORD

                        extrn   SYSCALL ClearBoxVESAM1:DWORD
                        extrn   SYSCALL ClearBoxVESAM2:DWORD
                        extrn   SYSCALL ClearBoxVESAM3:DWORD
                        extrn   SYSCALL ClearBoxVESAM4:DWORD
                        extrn   SYSCALL ClearBoxVESAM5:DWORD
                        extrn   SYSCALL ClearBoxVESAM6:DWORD

                        extrn   SYSCALL ClearBoxVESAF1:DWORD
                        extrn   SYSCALL ClearBoxVESAF2:DWORD
                        extrn   SYSCALL ClearBoxVESAF3:DWORD
                        extrn   SYSCALL ClearBoxVESAF4:DWORD
                        extrn   SYSCALL ClearBoxVESAF5:DWORD
                        extrn   SYSCALL ClearBoxVESAF6:DWORD

                        extrn   SYSCALL ClearBoxVESAI1:DWORD
                        extrn   SYSCALL ClearBoxVESAI2:DWORD
                        extrn   SYSCALL ClearBoxVESAI3:DWORD
                        extrn   SYSCALL ClearBoxVESAI4:DWORD
                        extrn   SYSCALL ClearBoxVESAI5:DWORD
                        extrn   SYSCALL ClearBoxVESAI6:DWORD

                        extrn   SYSCALL BlitBoxVESAM1:DWORD
                        extrn   SYSCALL BlitBoxVESAM2:DWORD

                        extrn   SYSCALL BlitBoxVESAF1:DWORD
                        extrn   SYSCALL BlitBoxVESAF2:DWORD

                        extrn   SYSCALL BlitBoxVESAI1:DWORD
                        extrn   SYSCALL BlitBoxVESAI2:DWORD

;----------------------------------------------------------------------------
GPFHandler		PROC

			push	eax
			push	ebx

			push	ecx
			push	edx

NB_PUSH		equ	4			; NUMBER of regs SAVED

			mov	edx, ds

			mov	ebx, _DATA
			mov	ecx, [esp+(4+NB_PUSH)*4]; CS

			cmp	edx, ebx	; right DS ?
			jne	chain

			mov	edx, cs

			and	ecx, 0FFFFh	; CS

			cmp	edx, ecx	; right CS ?
			jne	chain

			mov	ecx, -NB_OFFSET*4
			mov	edx, [esp+(3+NB_PUSH)*4]; EIP

again:			cmp	edx, TabWrite[ecx+NB_OFFSET*4]
			je	found
			add	ecx, 4
			jnz	again

			jmp	chain
found:
			push	esi
			jmp	TabJump[ecx+NB_OFFSET*4]
F5::
M5::
			lea	esi, [edi+eax+16]
			jmp	ok
F2::
F4::
F8::
M2::
M4::
M8::
			lea	esi, [edi+eax+8]
			jmp	ok
F1::
F3::
F7::
M1::
M3::
M7::
			lea	esi, [edi+eax]
			jmp	ok
I5::
			lea	esi, [edi+8]
			jmp	ok
I2::
I4::
I8::
			lea	esi, [edi+4]
			jmp	ok
I1::
I3::
I6::
I7::
F6::
M6::
			mov	esi, edi
ok:
GPFShift1::		sar	esi, 10h
			mov	eax, [VESABank]

			add	eax, esi

GPFShift2::		shl	esi, 10h

			sub	edi, esi

			push	edi
			call	[SwitchBankVESA]

			pop	edi
			pop	esi

			pop	edx
			pop	ecx

			pop	ebx
			pop	eax

			retf
chain:
			push	ds

			mov	ds, ebx

			mov	ebx, [Old_GPF_Off]
			mov	eax, [Old_GPF_Sel]

			pop	ds

			pop	edx
			pop	ecx

			xchg	ebx, [esp]

			xchg	eax, [esp+4]

			retf
GPFHandler		ENDP

;----------------------------------------------------------------------------
VirtualLinearFirst	PROC
			push	eax
			push	ebx

			push	ecx
			push	edx

			push	esi
			add	eax, edi

GPFShift3::		shr	eax, 10h

			mov	ebx, eax

GPFShift4::		shl	ebx, 10h

			sub	edi, ebx

			push	edi
			call	[SwitchBankVESA]

			pop	edi
			pop	esi

			pop	edx
			pop	ecx

			pop	ebx
			pop	eax

			ret
VirtualLinearFirst	ENDP

;----------------------------------------------------------------------------
FreeDPMIRsrc		PROC

			pushad

			mov	ebx, [Phys]

			test	ebx, ebx
			jz	ok_linear

			cmp	ebx, 0C0000h
			jb	ok_linear

			mov	ecx, ebx
			shr	ebx, 16
			mov	ax, 0801h
			int	31h			; Free Physical Address Mapping

			xor	eax, eax
			mov	[Phys], eax
ok_linear:
			cmp	[SwitchBankVESA], Offset SwitchBankVESA2Sel
			jne	ok_selector

			mov	ebx, dword ptr[Patch_Prot_Sel+1]
			mov	eax, 1			; free descriptor
			int	31h

			mov	ebx, [PtrLinearMemPort]
			mov	eax, 0801h		; free linear mapping
			mov	ecx, ebx
			shr	ebx, 16
			int	31h

ok_selector:		xor	eax, eax
			mov	[SwitchBankVESA], eax

			mov	fs, eax

			mov	ebx, [LinearSel]
			mov	[LinearSel], eax

			test	ebx, ebx
			jz	ok_LinearSel

			mov	eax, 1			; free descriptor
			int	31h
ok_LinearSel:
			popad
			ret

FreeDPMIRsrc		ENDP

;----------------------------------------------------------------------------
GetCheckVESAModeInfo	PROC

			pushad

			and	eax, not (1 shl 14)

			movzx	ecx, [SegVbeInfoBlock]
			mov	[R_EAX], 4F01h
			mov	[R_ECX], eax
			mov	[R_ES], cx
			mov	[R_EDI], 512

			mov	edx, eax

			mov	edi, [PtrModeInfoBlock]
			mov	ecx, 256/4
			xor	eax, eax
			rep	stosd			; clear InfoBlock for 1.0 compatibility

			mov	esi, offset Mode_256	; check if known mode for 1.0 compatibility

mode_scan:		movsx	eax, word ptr[esi]

			test	eax, eax
			js	exit_mode_scan

			cmp	eax, edx
			je	mode_found

			add	esi, 6
			jmp	mode_scan
mode_found:
			mov	edi, [PtrModeInfoBlock]

			mov	eax, [esi+2]		; X/Y Resolution
			mov	ebx, [Data_256]		; Rest of data
			mov	ecx, [Data_256+4]
			mov	dword ptr[edi+ModeXResolution], eax; fill buffer
			mov	dword ptr[edi+ModeXResolution+4], ebx
			mov	dword ptr[edi+ModeXResolution+8], ecx

			cmp	[R_ECX], 13h
			jne	exit_mode_scan

			mov	[edi].ModeAttributes, 10110001b
			mov	[edi].ModeNumberOfPlanes, 1
			mov	[edi].ModeNumberOfBanks, 1
			mov	[edi].ModeMemoryModel, 04h; Packed pixel

			mov	[edi].ModePhysBasePtr, 0A0000h
			mov	[edi].ModeBytesPerScanLine, ax

			jmp	Ok_Get
exit_mode_scan:
			mov	ebx, 10h		; Return Vbe Mode Info
			mov	eax, 0300h
			xor	ecx, ecx
			lea	edi, R_EDI
			int	31h
			jc	Unsupported_Mode	; DPMI error ?
			cmp	word ptr [R_EAX], 004Fh	; VESA error ?
			jne	Unsupported_Mode

			mov	edi, [PtrModeInfoBlock]

			movzx	ebp, [edi].ModeAttributes
			xor	ebp, (1 shl 5)+(1 shl 6); XOR to get all the bits in positive logic
							; Bit 7: 1=>Linear frame buffer available
							; Bit 6: 1=>Windowed memory mode available
							; Bit 5: 1=>VGA compatible
							; Bit 4: 1=>Graphic mode
							; Bit 0: 1=>Mode supported by hardware
			mov	[edi].ModeAttributes, bp

			test	ebp, 1
			jz	Unsupported_Mode	; if not supported exit

			test	ebp, 1 shl 4
			jz	Unsupported_Mode	; if not graphic mode then exit

			cmp	[edi].ModeNumberOfPlanes, 1
			jne	Unsupported_Mode

			cmp	[edi].ModeNumberOfBanks, 1
			jne	Unsupported_Mode

			cmp	[edi].ModeMemoryModel, 06h; Direct color?
			je	Ok_Get
Test_Packed:
			cmp	[edi].ModeMemoryModel, 04h; Packed pixel
			jne	Unsupported_Mode
Ok_Get:
			popad
			mov	eax, 1
			ret
Unsupported_Mode:
			popad
			xor	eax, eax
			ret

GetCheckVESAModeInfo	ENDP

;----------------------------------------------------------------------------
;void	SwitchBankVESA2(U32 bank)

;#pragma aux 	SwitchBankVESA2		\
;	parm	[eax]			\
;	modify	[ebx ecx edx esi edi]

SwitchBankVESA2		PROC

			cmp	eax, [VESABank]
			je	exit

			mov	[VESABank], eax

BankShift1::		shl	eax, 0
Patch_Window1::		mov	ebx, 1234h	; Window

			mov	edx, eax
Patch_Prot::		jmp	NEAR PTR Patch_Prot
exit:
			ret

SwitchBankVESA2		ENDP

;----------------------------------------------------------------------------
;void	SwitchBankVESA2Sel(U32 bank)

;#pragma aux 	SwitchBankVESA2Sel	\
;	parm	[eax]			\
;	modify	[ebx ecx edx esi edi]

SwitchBankVESA2Sel	PROC

			cmp	eax, [VESABank]
			je	exit

			mov	[VESABank], eax
			push	es

BankShift3::		shl	eax, 0
Patch_Sel::		mov	edx, 12345678h

			mov	es, edx

Patch_Window3::		mov	ebx, 1234h		; Window
			mov	edx, eax

Patch_Prot_Sel::	call	NEAR PTR Patch_Prot_Sel

			pop	es
exit:
			ret

SwitchBankVESA2Sel	ENDP

;----------------------------------------------------------------------------
;void	SwitchBankVESA1(U32 bank)

;#pragma aux 	SwitchBankVESA1		\
;	parm	[eax]			\
;	modify	[ebx ecx edx esi edi]

SwitchBankVESA1		PROC

			cmp	eax, [VESABank]
			je	exit

			mov	[VESABank], eax

BankShift2::		shl	eax, 0
Patch_Window2::		mov	ebx, 1234h		; Window

			mov	edx, 4F05h

			mov	[R_EBX], ebx
			mov	[R_EDX], eax
			mov	[R_EAX], edx

			mov	ebx, 10h		; Switch bank
			mov	eax, 0300h
			xor	ecx, ecx
			lea	edi, R_EDI
			int	31h
exit:
			ret

SwitchBankVESA1		ENDP

;----------------------------------------------------------------------------
;void	PaletteVblVESA(S32 flag)

;#pragma aux 	PaletteVblVESA		\
;	parm	[eax]

PaletteVblVESA		PROC

			test	eax, eax
			jz	ok

			mov	eax, 80h
ok:
			mov	[PaletteMode], eax

			ret

PaletteVblVESA		ENDP

;----------------------------------------------------------------------------
;void	SetPaletteVESA1(void *pal, S32 start, S32 n)

;#pragma aux 	SetPaletteVESA1		\
;	parm	[esi] [edx] [ecx]	\
;	modify	[eax ebx edi]

SetPaletteVESA1		PROC

			test	ecx, ecx
			jz	exit

			push	ecx
			mov	edi, [PtrVbeInfoBlock]

			lea	edi, [edi+1280+edx*4]

			push	edi

next:			mov	eax, [esi]

			and	eax, 00FCFCFCh

			shl	eax, 8-2
			add	esi, 3

			bswap	eax		; 486 only

			mov	[edi], eax
			add	edi, 4

			dec	ecx
			jnz	next

			pop	edi
			pop	ecx

			mov	eax, edi

			mov	[R_ECX], ecx
			mov	[R_EDX], edx

			shr	eax, 4
			and	edi, 15

			mov	[R_ES], ax
			mov	[R_EDI], edi

			mov	eax, 4F09h
	       		mov	ebx, [PaletteMode]

			mov	[R_EAX], eax
			mov	[R_EBX], ebx

			mov	ebx, 10h		; Set Pal
			mov	eax, 0300h
			xor	ecx, ecx
			lea	edi, R_EDI
			int	31h
exit:
			ret

SetPaletteVESA1		ENDP

;----------------------------------------------------------------------------
;void	SetPaletteVESA2(void *pal, S32 start, S32 n)

;#pragma aux 	SetPaletteVESA2		\
;	parm	[esi] [edx] [ecx]	\
;	modify	[eax ebx edi]

SetPaletteVESA2		PROC

			test	ecx, ecx
			jz	exit

			push	ecx
			mov	edi, [PtrVbeInfoBlock]

			lea	edi, [edi+1280+edx*4]

			push	edi

next:			mov	eax, [esi]

			and	eax, 00FCFCFCh

			shl	eax, 8-2
			add	esi, 3

			bswap	eax		; 486 only

			mov	[edi], eax
			add	edi, 4

			dec	ecx
			jnz	next

			pop	edi
			pop	ecx

			mov	eax, 4F09h
	       		mov	ebx, [PaletteMode]

Patch_Pal::		jmp	NEAR PTR Patch_Pal
exit:
			ret

SetPaletteVESA2		ENDP

;----------------------------------------------------------------------------
;void	SetPaletteDirect(void *pal, S32 start, S32 n)

;#pragma aux 	SetPaletteDirect	\
;	parm	[esi] [edx] [ecx]	\
;	modify	[eax ebx edi]

SetPaletteDirect	PROC

			test	ecx, ecx
			jz	exit

			mov	ebx, edx
			mov	eax, [PaletteMode]

			test	eax,eax
			jz	ok

			mov	edx, 3DAh
wait_end_vbl:
			in	al, dx
			test	al, 8
			jz	wait_end_vbl
wait_vbl:
			in	al, dx
			test	al, 8
			jnz	wait_vbl
ok:
			mov	eax, ebx
			mov	edx, 03C8h

			out	dx, al

			inc	edx
next:
			mov	eax, [esi]
			add	esi, 3

			and	eax, 00FCFCFCh

			shr	eax, 2

			out	dx, al		; R
			shr	eax, 8
			out	dx, al		; G
			shr	eax, 8
			out	dx, al		; B

			dec	ecx
			jnz	next
exit:
			ret

SetPaletteDirect	ENDP

;----------------------------------------------------------------------------
;void	VblVESA();

;#pragma aux 	VblVESA			\
;	 modify	[eax ebx ecx edx esi edi]

VblVESA			PROC	uses ebp


			mov	esi, [PtrVbeInfoBlock]
			xor	edx, edx

			add	esi, 1280
			xor	ecx, ecx

			mov	ebp, [PaletteMode]
			mov	cl, 1

			mov	ebx, 80h
			mov	[esi], dl

			mov	[PaletteMode], ebx
			mov	[esi+1], dl

			mov	[esi+2], dl
			call	[SetPaletteVESA]

			mov	[PaletteMode], ebp

			ret

VblVESA			ENDP

;----------------------------------------------------------------------------
;void	SetPaletteOneVESA(S32 col, S32 red, S32 green, S32 blue);

;#pragma aux SetPaletteOneVESA	"*"	\
;	parm	[edx] [eax] [ebx] [ecx]	\
;	modify	[esi edi]

SetPaletteOneVESA	PROC

			mov	esi, [PtrVbeInfoBlock]

			lea	esi, [esi+1280+edx*4]

			mov	[esi], al
			mov	[esi+1], bl
			mov	[esi+2], cl

			mov	ecx, 1
			jmp	[SetPaletteVESA]

SetPaletteOneVESA	ENDP

;----------------------------------------------------------------------------
;U32	InitVESA()

;#pragma aux 	InitVESA	\
;	modify	[]

InitVESA		PROC

			cmp	[SelVbeInfoBlock], 0
			jne	Exit

			pushad

			mov	eax, 0202h		; Get Old GPF handler
			mov	ebx, 13
			int	31h

			mov	word ptr[Old_GPF_Sel], cx
			mov	[Old_GPF_Off], edx

			mov	ecx, cs
			mov	edx, offset GPFHandler

			mov	eax, 0203h		; Set GPF handler
			mov	ebx, 13
			int	31h

			mov	eax, 0100h
			mov	ebx, (512+512+256+1024)/16; 512 for VBE info
			int	31h			; + 512 for real mode stack
			jc	Error			; + 256 for Mode info
							; + 1024 pal
			mov	[SegVbeInfoBlock], ax
			mov	[SelVbeInfoBlock], dx

			mov	[R_EDI], 0
			mov	[R_SS], ax
			mov	[R_ES], ax
			mov	[R_SP], 1280		; 512+512+256
			mov	[R_EAX], 4F00h		; Fonction 0: fill VbeInfoBlock

			and	eax, 0FFFFh
			shl	eax, 4
			mov	dword ptr [eax].VbeSignature, "2EBV"; 'VBE2' signature

			mov	[PtrVbeInfoBlock], eax

			lea	edi, [eax+4]
			mov	ecx, (512-4)/4
			xor	eax, eax
			rep	stosd				; security, clear VbeInfoBlock

			mov	[PtrModeInfoBlock], edi

			mov	ebx, 10h
			mov	eax, 0300h
			lea	edi, R_EDI
			xor	ecx, ecx
			int	31h				; real mode interrupt
			jc	Errorfree_

			mov	eax, [R_EAX]
			cmp	ax, 004Fh
			jne	Errorfree_

			mov	esi, [PtrVbeInfoBlock]

			cmp	dword ptr[esi].VbeSignature, "ASEV"; check 'VESA' signature
			jne	Errorfree_

			mov	eax, [esi].VbeOemStringPtr	; Conversion d'un ptr Real vers un Ptr flat
			mov	ebx, eax
			shr	eax, 12
			and	eax, not 15
			and	ebx, 0FFFFh
			add	eax, ebx
			mov	[esi].VbeOemStringPtr, eax


			mov	eax, [esi].VbeVideoModePtr	; Conversion d'un ptr Real vers un Ptr flat
			mov	ebx, eax
			shr	eax, 12
			and	eax, not 15
			and	ebx, 0FFFFh
			add	eax, ebx
			mov	[esi].VbeVideoModePtr, eax


			mov	eax, [esi].VbeOemVendorNamePtr	; Conversion d'un ptr Real vers un Ptr flat
			mov	ebx, eax
			shr	eax, 12
			and	eax, not 15
			and	ebx, 0FFFFh
			add	eax, ebx
			mov	[esi].VbeOemVendorNamePtr, eax

			mov	eax, [esi].VbeOemProductNamePtr	; Conversion d'un ptr Real vers un Ptr flat
			mov	ebx, eax
			shr	eax, 12
			and	eax, not 15
			and	ebx, 0FFFFh
			add	eax, ebx
			mov	[esi].VbeOemProductNamePtr, eax

			mov	eax, [esi].VbeOemProductRevPtr	; Conversion d'un ptr Real vers un Ptr flat
			mov	ebx, eax
			shr	eax, 12
			and	eax, not 15
			and	ebx, 0FFFFh
			add	eax, ebx
			mov	[esi].VbeOemProductRevPtr, eax
Fin_Ok:
			mov	[ModeDesiredX], 640
			mov	[ModeDesiredY], 480
			mov	[ModeDesiredDepth], 8
			mov	[ModeDesiredMemory], VESA_LINEAR_OR_WINDOWED

			popad
Exit:
			mov	eax, 1
			ret

Errorfree_:
			mov	dx, [SelVbeInfoBlock]
			mov	ax, 0101h
			int	31h				; Free allocated real mode memory block
			mov	[SelVbeInfoBlock], 0
Error:
			popad
			xor	eax, eax
			ret

InitVESA		ENDP

;----------------------------------------------------------------------------
;void	EndVESA()

;#pragma aux 	EndVESA	\
;	modify	[]

EndVESA			PROC

			pushad

			movzx	edx, [SelVbeInfoBlock]; already free ?
			test	edx, edx
			jz	exit

			call	FreeDPMIRsrc

			movzx	edx, [SelVbeInfoBlock]
			test	edx, edx
			je	exit
			mov	eax, 0101h
			int	31h			; Free allocated real mode memory block

			xor	ebx, ebx
			mov	[SegVbeInfoBlock], bx	; Reset
			mov	[SelVbeInfoBlock], bx

			mov	eax, [PtrProtectedAPI]

			test	eax, eax
			je	exit

			mov	[PtrProtectedAPI], ebx
IFDEF	REG_CALL
			call	free_
ELSE
			push	eax
			call	free
			pop	eax
ENDIF
exit:
			popad
			ret

EndVESA			ENDP

;----------------------------------------------------------------------------
;void	ScanVESAModes(MODE_CALLBACK *modeproc)

;#pragma aux ScanVESAModes	"*"		\
;	parm caller	[]			\
;	modify		[eax ebx ecx edx esi edi]

ScanVESAModes		PROC

			cmp	[SelVbeInfoBlock], 0	; VESA initialized
			jne	Can_Start
			call	InitVESA		; No, then do it
			test	eax, eax
			jz	exit
Can_Start:
			mov	esi, [PtrVbeInfoBlock]
			mov	esi, [esi].VbeVideoModePtr

			mov	eax, 13h
			jmp	EnterLoop
Next_Mode:
			movzx	eax, word ptr[esi]
			add	esi, 2

			cmp	eax, 0FFFFh
			je	exit
EnterLoop:
			call	GetCheckVESAModeInfo
			test	eax, eax
			jz	Next_Mode

			mov	edi, [PtrModeInfoBlock]
			xor	edx, edx

			xor	eax, eax
			xor	ebx, ebx

			xor	ecx, ecx
			mov	dx, [edi].ModeAttributes

			mov	ax, [edi].ModeXResolution
			mov	bx, [edi].ModeYResolution

			shr	edx, 6
			mov	cl, [edi].ModeBitsPerPixel

			movzx	edi, [edi].ModeNumberOfImagePages

			and	edx, 3
			inc	edi

			call	[esp+4]

			jmp	Next_Mode
exit:
			ret

ScanVESAModes		ENDP

;----------------------------------------------------------------------------
;U32	DetectVESAMode(U32 ResX, U32 ResY, U32 Depth, U32 Memory)

;#pragma aux 	DetectVESAMode		\
;	parm	[eax] [ebx] [ecx] [edx]	\
;	modify	[esi edi]

;Will find the best (lowest) resolution containing the required res
;with same Depth. Memory indicate if linear or windowed or don't care
;	for windowed	Memory = 1
;	for linear	Memory = 2
;	for don't care	Memory = 0 or 3

DetectVESAMode		PROC	uses ebp

			push	eax

			and	edx, 3
			jnz	ok_memory

			mov	edx, 3
ok_memory:
			mov	[ModeDesiredX], eax
			mov	[ModeDesiredY], ebx
			imul	eax, ebx
			mov	[ModeDesiredDepth], ecx
			mov	[ModeDesiredMemory], edx
			mov	[esp], eax

			mov	[VESAMode], -1		; Reset
			mov	[ModeResX], 7FFFh
			mov	[ModeResY], 7FFFh
			mov	[VESAModeMemory], 0

			cmp	[SelVbeInfoBlock], 0	; VESA initialized
			jne	Can_Start
			call	InitVESA		; No, then do it
			test	eax, eax
			jz	Check_End
Can_Start:
			mov	esi, [PtrVbeInfoBlock]
			mov	esi, [esi].VbeVideoModePtr

			mov	eax, 13h
			jmp	EnterLoop
Next_Mode:
			movzx	eax, word ptr[esi]
			add	esi, 2

			cmp	eax, 0FFFFh
			je	Check_End
EnterLoop:
			mov	ebp, eax
			call	GetCheckVESAModeInfo
			test	eax, eax
			jz	Next_Mode

			mov	ecx, [ModeDesiredDepth]

			mov	edi, [PtrModeInfoBlock]

			cmp	[edi].ModeBitsPerPixel, cl
			jne	Next_Mode

			movzx	eax, [edi].ModeXResolution
			movzx	ebx, [edi].ModeYResolution

			cmp	eax, [ModeDesiredX]
			jb	Next_Mode

			cmp	ebx, [ModeDesiredY]
			jb	Next_Mode

			movzx	edi, [edi].ModeAttributes
			shr	edi, 6
			and	edi, [ModeDesiredMemory]
			jz	Next_Mode

			mov	ecx, eax
			mov	edx, [ModeResX]

			imul	ecx, ebx		; area for new mode
			imul	edx, [ModeResY]		; area for last mode

			cmp	ecx, edx		; if new larger than last exit
			jg	Next_Mode

			je	Check_Memory
Ok_Mode:
			mov	[ModeResX], eax
			mov	[ModeResY], ebx
			mov	[VESAMode], ebp
			mov	[VESAModeMemory], edi

			sub	ecx, [esp]
			jz	exactres

			jmp	Next_Mode
Check_End:
			mov	edx, [VESAModeMemory]
			mov	eax, [VESAMode]
			and	edx, VESA_LINEAR
			shl	edx, 13
			or	eax, edx
			mov	[VESAMode], eax

			pop	edx

			ret
Check_Memory:
			test	edi, VESA_LINEAR
			jnz	Ok_Mode			; if linear then take this mode instead
			jmp	Next_Mode
exactres:
			cmp	ebp, 13h
			je	Next_Mode
			test	edi, 2
			jnz	Check_End
			cmp	[ModeDesiredMemory], 1
			je	Check_End
			jmp	Next_Mode

DetectVESAMode		ENDP

;----------------------------------------------------------------------------
;U32	InitVESAMode(U32 Mode)

;#pragma aux 	InitVESAMode		\
;	parm	[eax]			\
;	modify	[ebx ecx edx esi edi]

;If bit 14 in mode is set then will try to set a linear frame buffer
;if possible, else will default to windowed

;Return mode set with bit 14 correctly set

InitVESAMode		PROC

			cmp	eax, -1
			je	Init_Error

			cmp	[SelVbeInfoBlock], 0	; VESA initialised ?
			jne	Can_Start
			push	eax
			call	InitVESA		; then do it :)
			test	eax, eax
			pop	eax
			jz	Init_Error
Can_Start:
Retry:
			mov	[VESAMode], eax

			call	FreeDPMIRsrc

			; Get info on wanted mode
			call	GetCheckVESAModeInfo
			test	eax, eax
			jz	Init_Error

			mov	edi, [PtrModeInfoBlock]	; retrieve infos
			movzx	eax, [edi].ModeBytesPerScanLine
			movzx	ebx, [edi].ModeXResolution
			movzx	ecx, [edi].ModeYResolution
			mov	edx, [edi].ModePhysBasePtr
			movzx	edi, [edi].ModeNumberOfImagePages
			mov	[BytesScanLine], eax
			inc	edi
			mov	[ModeResX], ebx
			mov	[ModeResY], ecx
			mov	[ModePages], edi
			mov	[PhysHard], edx		; physical address
			mov	[Phys], edx		; physical address

			mov	[SetPaletteVESA], Offset SetPaletteVESA1

			mov	edi, [PtrVbeInfoBlock]
			cmp	[edi].VbeVersion, 0200h
			jb	No_Prot

			mov	[R_EAX], 4F0Ah
			mov	[R_EBX], 0
			mov	ebx, 10h		; Get 32bit interface
			mov	eax, 0300h
			xor	ecx, ecx
			lea	edi, R_EDI
			int	31h
			jc	No_Prot			; DPMI error ?
			cmp	word ptr [R_EAX], 004Fh	; VESA error ?
			jne	No_Prot

							; allocate buffer for bank switching
			cmp	[PtrProtectedAPI], 0	; and palette functions
			jne	NoAlloc

IFDEF	REG_CALL
			movzx	eax, word ptr[R_ECX]
 			call	malloc_
ELSE
			movzx	ecx, word ptr[R_ECX]
			push	ecx
 			call	malloc
			pop	ecx
ENDIF
			mov	[PtrProtectedAPI], eax	; set pointer

			test	eax, eax
			je	No_Prot
NoAlloc:
			movzx	esi, [R_ES]
			mov	eax, [R_EDI]
			shl	esi, 4
			and	eax, 0FFFFh
			add	esi, eax

			movzx	ecx, word ptr[R_ECX]
			mov	edi, [PtrProtectedAPI]
			mov	ebx, edi
			rep	movsb			; Copy byte by byte :)

			mov	edi, ebx
			movzx	ecx, word ptr[edi+4]	; Palette routine

			add	ecx, edi		; Palette routine
			mov	eax, ecx

			sub	ecx, (Offset Patch_Pal)+5
			mov	dword ptr [Patch_Pal+1], ecx; protected mode palette routine

			mov	[SetPaletteVESA], Offset SetPaletteVESA2
No_Prot:
			xor	eax, eax		; try setting color 0 to black
			xor	ebx, ebx
			xor	ecx, ecx
			xor	edx, edx
			mov	[R_EAX], eax		; security
			call	SetPaletteOneVESA
			cmp	ax, 004Fh
			je	ok_pal

			mov	eax, [R_EAX]
			cmp	ax, 004Fh
			je	ok_pal

			; *** Bios color setup doesn't work... use direct port I/O

			mov	[SetPaletteVESA], Offset SetPaletteDirect
ok_pal:
			; *** Initialise le mode VESA ***
			mov	eax, [VESAMode]
			mov	ebx, eax
			xor	eax, 1 shl 14
			cmp	eax, 4013h
			je	Init_Error
			cmp	eax, 13h
			jne	okvesamode

			int	10h
			jmp	linearmode
okvesamode:
			mov	eax, 4F02h		; Init VESA mode
			push	ebp
			int	10h
			pop	ebp
			cmp	ax, 004Fh		; VESA error ?
			jne	Retry_Windowed

			test	[VESAMode], 1 shl 14
			je	Get_BankEntry
linearmode:
			; *** LINEAR ***

			mov	ModeMemory, 2
			mov	ebx, [Phys]
			mov	eax, 0800h
			test	ebx, ebx
			jz	Retry_Windowed
			cmp	ebx, 0C0000h
			jb	End_Ok

			mov	ecx, ebx
			mov	esi, [ModePages]
			shr	ebx, 16
			imul	esi, [BytesScanLine]
			imul	esi, [ModeResY]
			mov	edi, esi
			shr	esi, 16
			push	ebp
			int	31h			; Physical Address Mapping
			pop	ebp
			jc	Retry_Windowed		; failed try windowed

			shl	ebx, 16			; ok we have the logical address
			and	ecx, 0FFFFh
			or	ebx, ecx
			mov	[Phys], ebx


			jmp	End_Ok

			; *** If linear mode failed try windowed
Retry_Windowed:
			mov	eax, [VESAMode]
			test	eax, 1 shl 14		; check if previous mode was linear
			je	Init_Error		; if not then give up
			and	eax, not (1 shl 14)	; ok retry windowed
			jmp	Retry

			; *** WINDOWED ***
Get_BankEntry:
			mov	ModeMemory, 1
			mov	edi, [PtrModeInfoBlock]

			movzx	edx, [edi].ModeWinSize
			shl	edx, 10
			mov	[BankSize], edx

			mov	ebx, [BankSize]
			bsr	ebx, ebx

			mov	byte ptr[GPFShift1+2], bl
			mov	byte ptr[GPFShift2+2], bl

			mov	byte ptr[GPFShift3+2], bl
			mov	byte ptr[GPFShift4+2], bl

			movzx	eax, [edi].ModeWinGranularity
			sub	ebx, 10

			bsr	eax, eax		; get higher bit position
			sub	ebx, eax		; shift = banksizebit - position

			mov     byte ptr[BankShift1+2], bl
			mov     byte ptr[BankShift2+2], bl
			mov     byte ptr[BankShift3+2], bl

			mov	edx, 1 + (1 shl 2)
			xor	ecx, ecx
			movzx	eax, [edi].ModeWinAAttributes
			movzx	ebx, [edi].ModeWinBAttributes

			and	eax, edx		; only keep movable & writable bits
			and	ebx, edx

			cmp	eax, edx		; A is movable & writable ?
			je	win_A

			cmp	eax, edx		; B is movable & writable ?
			je	win_B

			dec	edx

			test	eax, edx		; A is writable ?
			je	win_A

			test	ebx, edx		; B is writable ?
			je	win_B

			jmp	Init_Error
win_A:
			movzx	ebx, [edi].ModeWinASegment
			jmp	patch_window
win_B:
			inc	ecx
			movzx	ebx, [edi].ModeWinBSegment
patch_window:
			shl	ebx, 4			; adress of window

			mov	dword ptr[Patch_Window1+1], ecx
			mov	dword ptr[Patch_Window2+1], ecx
			mov	dword ptr[Patch_Window3+1], ecx

			mov	[PhysHard], ebx
			mov	[Phys], ebx

			test	edx, 1			; window movable
			jnz	ok_bank

			or	[VESAMode], 1 shl 14
			jmp	End_Ok
ok_bank:
			xor	eax, eax
			mov	ecx, 1
			int	31h			; Allocate one descriptor

			mov	[LinearSel], eax

			mov	ecx, [PhysHard]
			mov	ebx, eax
			mov	edx, ecx
			shr	ecx, 16
			mov	eax, 7
			int	31h			; set base to window

			mov	ecx, [BankSize]
			mov	eax, 8
			dec	ecx
			mov	ebx, [LinearSel]
			mov	edx, ecx
			shr	ecx, 16
			int	31h			; set segment limit to window size - 1

			mov	eax, [LinearSel]
			mov	fs, eax

			mov	[Phys], 0

			mov	edi, [PtrProtectedAPI]
			test	edi, edi
			jz	No_VESA2

			movzx	ebx, word ptr[edi]	; switching routine
			movzx	eax, word ptr[edi+6]	; Port / Memory locations

			add	ebx, edi		; switching routine
			add	edi, eax		; Port / Memory locations

next_port:		cmp	word ptr[edi], 0FFFFh	; go through the port list
			lea	edi, [edi+2]
			jne	next_port

			cmp	word ptr[edi], 0FFFFh	; check if a memory selector is required
			jne	need_selector

			sub	ebx, (Offset Patch_Prot)+5
			mov	dword ptr [Patch_Prot+1], ebx	; protected mode switching routine

			mov	[SwitchBankVESA], Offset SwitchBankVESA2
			jmp	End_Ok
need_selector:
			sub	ebx, (Offset Patch_Prot_Sel)+5
			mov	dword ptr [Patch_Prot_Sel+1], ebx; protected mode switching routine

			push	ebp

			push	edi
			mov	ebx, [edi]
			movzx	edi, word ptr[edi+4]
			mov	ecx, ebx
			xor	esi, edi		; si:di = length
			shr	ebx, 16			; bx:cx = phys adress
			mov	eax, 0800h		; physical to linear
			int	31h
			shl	ebx, 16
			pop	edi
			and	ecx, 0FFFFh
			or	ebx, ecx		; ebx = linear
			mov	[PtrLinearMemPort], ebx

			xor	eax, eax		; allocate descriptor
			mov	ecx, 1			; one descriptor
			push	edi
			int	31h
			and	eax, 0FFFFh		; descriptor
			pop	edi
			mov	dword ptr[Patch_Sel+1], eax

			mov	ecx, [PtrLinearMemPort]
			mov	edx, ecx
			push	eax
			shr	ecx, 16			; cx:dx = base
			mov	ebx, eax		; selector
			mov	eax, 7			; set segment base
			push	edi
			int	31h
			pop	edi
			movzx	ecx, word ptr[edi+4]
			dec	ecx
			pop	ebx
			mov	edx, ecx
			mov	eax, 0800h		; set segment limit
			shr	ecx, 16			; cx:dx = limit
			int	31h

			pop	ebp

			mov	[SwitchBankVESA], Offset SwitchBankVESA2Sel
			jmp	End_Ok

			; *** Get real bank mode switch entry point ***
No_VESA2:
			mov	[SwitchBankVESA], Offset SwitchBankVESA1
End_Ok:
			mov	[VESABank], -1
			mov	[QuietLog], 1
			mov	eax, [VESAMode]
			ret
Init_Error:
			mov	eax, -1
			ret

InitVESAMode		ENDP

;----------------------------------------------------------------------------
;U32	DetectInitVESAMode(U32 ResX, U32 ResY, U32 Depth, U32 Memory)

;#pragma aux 	DetectInitVESAMode	\
;	parm	[eax] [ebx] [ecx] [edx]	\
;	modify	[esi edi]

;Will find the best (lowest) resolution containing the required res
;with same Depth. Memory indicate if linear or windowed or don't care
;	for windowed	Memory = 1
;	for linear	Memory = 2
;	for don't care	Memory = 0 or 3

;Will then set the mode

DetectInitVESAMode	PROC

			call	DetectVESAMode
			jmp	InitVESAMode

DetectInitVESAMode	ENDP

;----------------------------------------------------------------------------
;void	GetLastVESAMode()

;#pragma aux 	GetLastVESAMode	\
;	modify	[]

GetLastVESAMode		PROC

			pushad

			cmp	[SelVbeInfoBlock], 0	; VESA initialised ?
			jne	Ok_Get

			call	InitVESA		; then do it :)
			test	eax, eax
			jz	Unsupported_Mode	; VESA error ?
Ok_Get:
			mov	eax, 4F03h		; Current VBE Mode
			int	10h
			cmp	ax, 4Fh
			je	Unsupported_Mode	; VESA error ?

			and	ebx, 7FFFh

			push	ebx
			movzx	ecx, [SegVbeInfoBlock]
			mov	[R_EAX], 4F01h
			mov	[R_ECX], ebx
			mov	[R_ES], cx
			mov	[R_EDI], 512
			mov	ebx, 10h		; Return Vbe Mode Info
			mov	eax, 0300h
			xor	ecx, ecx
			lea	edi, R_EDI
			int	31h
			pop	ebx
			jc	Unsupported_Mode	; DPMI error ?
			cmp	word ptr [R_EAX], 004Fh	; VESA error ?
			jne	Unsupported_Mode

			mov	eax, [PtrModeInfoBlock]

			movzx	eax, [eax].ModeAttributes

			test	eax, 1
			jnz	Unsupported_Mode	; Supported

			test	eax, 1 shl 4
			jnz	Unsupported_Mode	; text mode ?

			or	ebx, 8000h		; mark as VESA Mode

			mov	[VESALastMode], ebx

			popad
			ret
Unsupported_Mode:
			mov	ah, 0Fh
			int	10h
			and	eax, 0FFh
			mov	[VESALastMode], eax

			push	es

			mov	eax, 1130h
			xor	ebx, ebx
			int	10h			; change ES and EBP !

			pop	es

			and	ecx, 0FFh

			mov	[VESALastSizeChar], ecx

			popad
			ret

GetLastVESAMode		ENDP

;----------------------------------------------------------------------------
;void	ResetLastVESAMode()

;#pragma aux 	ResetLastVESAMode	\
;	modify	[]

ResetLastVESAMode	PROC

			pushad

			mov	eax, [VESALastMode]
			test	eax, 8000h
			jnz	vesa_mode

			int	10h		; Init old VGA mode

			xor	ebx, ebx
			mov	eax, [VESALastSizeChar]

			cmp	al, 8		; reset last char size
			je	size8

			cmp	al, 14
			je	size14

size16:			mov	eax, 1114h
			jmp	exit

size14:			mov	eax, 1111h
			jmp	exit

size8:			mov	eax, 1112h
			jmp	exit
vesa_mode:
			mov	ebx, eax
			mov	eax, 4F02h	; Init old VESA mode
			and	ebx, 7FFFh
exit:
			int	10h

			xor	eax, eax
			mov	QuietLog, eax

			popad
			ret

ResetLastVESAMode	ENDP

;----------------------------------------------------------------------------
;			The
			End
