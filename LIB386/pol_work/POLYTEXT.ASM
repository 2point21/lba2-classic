; **************************************************************************
; **************************************************************************
; **************************************************************************
; ***                                                                    ***
; ***                         POLYGON LIBRARY                            ***
; ***                                                                    ***
; **************************************************************************
; ***                                                                    ***
; ***                           (Adeline Software Intl.)                 ***
; ***                           1995-96-97                               ***
; ***                                                                    ***
; **************************************************************************
; **************************************************************************
; **************************************************************************

			OPTION	PROC:PRIVATE
			OPTION	SCOPED
			OPTION	LANGUAGE:SYSCALL



;			**************
;			*** PUBLIC ***
;			**************

;					******************
;					*** PROCEDURES ***
;					******************
PUBLIC	SYSCALL		First_POLYTEXTUREPatch
PUBLIC	SYSCALL		Last_POLYTEXTUREPatch

PUBLIC	SYSCALL		Filler_Texture
PUBLIC	SYSCALL		Filler_TextureFlat
PUBLIC	SYSCALL		Filler_TextureChromaKey
PUBLIC	SYSCALL		Filler_TextureFlatChromaKey
PUBLIC	SYSCALL		Filler_TextureFog
PUBLIC	SYSCALL		Filler_TextureChromaKeyFog
PUBLIC	SYSCALL		Filler_TextureZBuf
PUBLIC	SYSCALL		Filler_TextureChromaKeyZBuf
PUBLIC	SYSCALL		Filler_TextureFlatZBuf
PUBLIC	SYSCALL		Filler_TextureFlatChromaKeyZBuf
PUBLIC	SYSCALL		Filler_TextureFogZBuf
PUBLIC	SYSCALL		Filler_TextureChromaKeyFogZBuf
PUBLIC	SYSCALL		Filler_TextureNZW
PUBLIC	SYSCALL		Filler_TextureChromaKeyNZW
PUBLIC	SYSCALL		Filler_TextureFlatNZW
PUBLIC	SYSCALL		Filler_TextureFlatChromaKeyNZW
PUBLIC	SYSCALL		Filler_TextureFogNZW
PUBLIC	SYSCALL		Filler_TextureChromaKeyFogNZW




;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***
PUBLIC	SYSCALL		Fill_VLTexture1
PUBLIC	SYSCALL		Fill_VLTexture2
PUBLIC	SYSCALL		Fill_VLTexture3
PUBLIC	SYSCALL		Fill_VLTexture4
PUBLIC	SYSCALL		Fill_VLTexture5
PUBLIC	SYSCALL		Fill_VLTexture6
PUBLIC	SYSCALL		Fill_VLTexture7
PUBLIC	SYSCALL		Fill_VLTexture8
PUBLIC	SYSCALL		Fill_VLTexture9
PUBLIC	SYSCALL		Fill_VLTexture10
PUBLIC	SYSCALL		Fill_VLTexture11
PUBLIC	SYSCALL		Fill_VLTexture12
PUBLIC	SYSCALL		Fill_VLTexture13
PUBLIC	SYSCALL		Fill_VLTexture14
PUBLIC	SYSCALL		Fill_VLTexture15
PUBLIC	SYSCALL		Fill_VLTexture16
PUBLIC	SYSCALL		Fill_VLTexture17
PUBLIC	SYSCALL		Fill_VLTexture18


;					*** MODULE DATA ***




;			***************
;			*** INCLUDE ***
;			***************

INCLUDE			F:\PROJET\LIB386\H\FILLER.INC


;			**************
;			*** MACROS ***
;			**************


;			***************
;			*** EQUATES ***
;			***************




;			**************************
;			*** SEGMENTATION MODEL ***
;			**************************

			.386
;			.MODEL	FLAT, SYSCALL



;			************
;			*** DATA ***
;			************
;			.DATA
_DATA			SEGMENT	USE32 PUBLIC PARA 'DATA'

;				******************
;				*** Extrn data ***
;				******************
Extrn	SYSCALL		Fill_Restart		:	DWORD
Extrn	SYSCALL		Fill_Patch		:	DWORD
Extrn	SYSCALL		Fill_Color		:	DWORD
Extrn	SYSCALL		Fill_Logical_Palette	:	BYTE

Extrn	SYSCALL		PtrMap			:	DWORD
Extrn	SYSCALL		RepMask			:	DWORD
Extrn	SYSCALL		PtrCLUTGouraud		:	DWORD
Extrn	SYSCALL		PtrZBuffer		:	DWORD

Extrn	SYSCALL		Fill_LeftSlope		:	DWORD
Extrn	SYSCALL		Fill_RightSlope		:	DWORD

Extrn	SYSCALL		Fill_CurY		:	DWORD
Extrn	SYSCALL		Fill_CurXMin		:	DWORD
Extrn	SYSCALL		Fill_CurXMax		:	DWORD

Extrn	SYSCALL		PTR_TabOffLine		:	DWORD
Extrn	SYSCALL		Fill_CurOffLine		:	DWORD
Extrn	SYSCALL		Log			:	DWORD
Extrn	SYSCALL		ScreenPitch		:	DWORD

Extrn	SYSCALL		Fill_CurMapUMin		:	DWORD
Extrn	SYSCALL		Fill_MapU_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_MapU_XSlope	:	DWORD

Extrn	SYSCALL		Fill_CurMapVMin		:	DWORD
Extrn	SYSCALL		Fill_MapV_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_MapV_XSlope	:	DWORD

Extrn	SYSCALL		Fill_CurZBufMin		:	DWORD
Extrn	SYSCALL		Fill_ZBuf_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_ZBuf_XSlope	:	DWORD




;				*******************
;				*** Global data ***
;				*******************

;					*******************
;					*** PUBLIC data ***
;					*******************



;					*******************
;					*** MODULE data ***
;					*******************
			ALIGN	4







;				******************
;				*** Local data ***
;				******************
			ALIGN	4
Precalc_Inc		dd	0		; Precalc value



_DATA			ENDS


;			************
;			*** CODE ***
;			************
;			.CODE
_TEXT			SEGMENT	USE32 PARA PUBLIC 'CODE'
			ASSUME	CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT


;				******************
;				*** Extrn proc ***
;				******************
Extrn	SYSCALL		Triangle_ReadNextEdge	:	PROC	; Entry point that reads the next point in the triangle


;				*******************
;				*** Global proc ***
;				*******************
First_POLYTEXTUREPatch	Label	DWORD


; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_Texture		PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			push	ebx

			mov	ebx,[Fill_MapV_XSlope]
			mov	dword ptr [@@DecMapU+2],eax

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			pop	ebx

			mov	[Precalc_Inc],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture1::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	eax,ebp
			mov	edx,esi

			shr	eax,8
			and	edx,0FF0000h

			shr	edx,16
			and	eax,0FF00h

			shl	ebp,16		; EBP = DecMapV
			or	eax,edx

			mov	edx,esi
			mov	esi,[RepMask]

			and	eax,esi
			mov	esi,[PtrMap]

			shl	edx,16		; EDX = DecMapU
			mov	ebx,[Precalc_Inc]
@@Each_X:
			mov	dl,[eax+esi]
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecMapU:		add	edx,12345678h

			adc	al,bl
			mov	[edi],dl

			inc	edi
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X
@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_Texture		ENDP




; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureChromaKey	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			push	ebx

			mov	ebx,[Fill_MapV_XSlope]
			mov	dword ptr [@@DecMapU+2],eax

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			pop	ebx

			mov	[Precalc_Inc],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture3::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	eax,ebp
			mov	edx,esi

			shr	eax,8
			and	edx,0FF0000h

			shr	edx,16
			and	eax,0FF00h

			shl	ebp,16		; EBP = DecMapV
			or	eax,edx

			mov	edx,esi
			mov	esi,[RepMask]

			and	eax,esi
			mov	esi,[PtrMap]

			shl	edx,16		; EDX = DecMapU
			mov	ebx,[Precalc_Inc]
@@Each_X:
			mov	dl,[eax+esi]
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecMapU:		add	edx,12345678h

			adc	al,bl
			inc	edi

			test	dl,dl
			je	@@NoDisp

			mov	[edi-1],dl
@@NoDisp:
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureChromaKey	ENDP









; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFlat	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrCLUTGouraud]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	dword ptr [@@PtrMap+2],ebx

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shl	eax,16

			mov	dword ptr [@@DecMapU+2],eax

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			pop	ebx

			mov	[Precalc_Inc],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture2::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	eax,ebp
			mov	edx,esi

			shr	eax,8
			and	edx,0FF0000h

			shr	edx,16
			and	eax,0FF00h

			shl	ebp,16		; EBP = DecMapV
			or	eax,edx

			shl	esi,16		; ESI = DecMapU
			mov	edx,[RepMask]

			and	eax,edx
			mov	ebx,[Precalc_Inc]

			mov	edx,[Fill_Color]
@@Each_X:
@@PtrMap:		mov	dl,[eax+12345678h]
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecMapU:		add	esi,12345678h

			adc	al,bl
@@PtrCLUT:		mov	dl,[edx+12345678h]

			mov	[edi],dl
			inc	edi

@@RepMask:		and	eax,12345678h
			dec	ecx

			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFlat	ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFlatChromaKey PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrCLUTGouraud]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	dword ptr [@@PtrMap+2],ebx

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shl	eax,16

			mov	dword ptr [@@DecMapU+2],eax

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			pop	ebx

			mov	[Precalc_Inc],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture4::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	eax,ebp
			mov	edx,esi

			shr	eax,8
			and	edx,0FF0000h

			shr	edx,16
			and	eax,0FF00h

			shl	ebp,16		; EBP = DecMapV
			or	eax,edx

			shl	esi,16		; ESI = DecMapU
			mov	edx,[RepMask]

			and	eax,edx
			mov	ebx,[Precalc_Inc]

			mov	edx,[Fill_Color]
@@Each_X:
@@PtrMap:		mov	dl,[eax+12345678h]
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecMapU:		add	esi,12345678h

			adc	al,bl
			inc	edi

			test	dl,dl
			je	@@NoDisp

@@PtrCLUT:		mov	dl,[edx+12345678h]

			mov	[edi-1],dl
@@NoDisp:
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFlatChromaKey ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFog	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			mov	ebx,[Fill_MapV_XSlope]
			mov	dword ptr [@@DecMapU+2],eax

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			pop	ebx

			mov	[Precalc_Inc],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture5::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	eax,ebp
			mov	edx,esi

			shr	eax,8
			and	edx,0FF0000h

			shr	edx,16
			and	eax,0FF00h

			shl	ebp,16		; EBP = DecMapV
			or	eax,edx

			shl	esi,16		; ESI = DecMapU
			mov	edx,[RepMask]

			and	eax,edx
			mov	ebx,[Precalc_Inc]

			xor	edx,edx
@@Each_X:
@@PtrMap:		mov	dl,[eax+12345678h]
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecMapU:		add	esi,12345678h

			adc	al,bl
			mov	dl,[edx+Fill_Logical_Palette]

			mov	[edi],dl
			inc	edi

@@RepMask:		and	eax,12345678h
			dec	ecx

			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFog	ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureChromaKeyFog PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			mov	ebx,[Fill_MapV_XSlope]
			mov	dword ptr [@@DecMapU+2],eax

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			pop	ebx

			mov	[Precalc_Inc],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture6::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	eax,ebp
			mov	edx,esi

			shr	eax,8
			and	edx,0FF0000h

			shr	edx,16
			and	eax,0FF00h

			shl	ebp,16		; EBP = DecMapV
			or	eax,edx

			shl	esi,16		; ESI = DecMapU
			mov	edx,[RepMask]

			and	eax,edx
			mov	ebx,[Precalc_Inc]

			xor	edx,edx
@@Each_X:
@@PtrMap:		mov	dl,[eax+12345678h]
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecMapU:		add	esi,12345678h

			adc	al,bl
			inc	edi

			test	dl,dl
			je	@@NoDisp

			mov	dl,[edx+Fill_Logical_Palette]

			mov	[edi-1],dl
@@NoDisp:
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureChromaKeyFog ENDP




; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZBuf	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			mov	dword ptr [@@DecMapU+2],eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			mov	ebx,[PtrZBuffer]

			mov	[Precalc_Inc],eax
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@PtrZBuffer1+4],ebx

			mov	ebx,[Fill_ZBuf_XSlope]
			mov	dword ptr [@@DecZBuf+2],eax

			shr	ebx,8
			mov	eax,[Log]

			mov	dword ptr [@@IntZBuf+2],ebx
			pop	ebx

			mov	dword ptr [@@PtrLog+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture7::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:

			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	eax,[Log]
			mov	ebx,[Precalc_Inc]

			shl	ebp,16
			sub	edi,eax

			or	ebx,esi
			mov	eax,[RepMask]

			and	eax,edx
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrZBuffer1:		mov	word ptr [edi*2+12345678h],dx
@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
			inc	edi
@@DecMapU:		add	ebx,12345678h

@@IntMapU:		adc	al,12
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecZBuf:		add	esi,12345678h

@@IntZBuf:		adc	edx,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureZBuf	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureChromaKeyZBuf PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			mov	dword ptr [@@DecMapU+2],eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			mov	ebx,[PtrZBuffer]

			mov	[Precalc_Inc],eax
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@PtrZBuffer1+4],ebx

			mov	ebx,[Fill_ZBuf_XSlope]
			mov	dword ptr [@@DecZBuf+2],eax

			shr	ebx,8
			mov	eax,[Log]

			mov	dword ptr [@@IntZBuf+2],ebx
			pop	ebx

			mov	dword ptr [@@PtrLog+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture8::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:

			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	eax,[Log]
			mov	ebx,[Precalc_Inc]

			shl	ebp,16
			sub	edi,eax

			or	ebx,esi
			mov	eax,[RepMask]

			and	eax,edx
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrZBuffer1:		mov	word ptr [edi*2+12345678h],dx
@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
			inc	edi
@@DecMapU:		add	ebx,12345678h

@@IntMapU:		adc	al,12
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecZBuf:		add	esi,12345678h

@@IntZBuf:		adc	edx,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureChromaKeyZBuf ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFlatZBuf	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[Fill_Color]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@PtrZBuffer1+4],ebx

			mov	ebx,[Fill_ZBuf_XSlope]
			mov	dword ptr [@@DecZBuf+2],eax

			shr	ebx,8
			mov	eax,[Log]

			and	ebx,0FFFFh
			mov	dword ptr [@@PtrLog+2],eax

			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture9::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrZBuffer1:		mov	word ptr [edi*2+12345678h],dx
@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFlatZBuf	ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFlatChromaKeyZBuf PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[Fill_Color]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@PtrZBuffer1+4],ebx

			mov	ebx,[Fill_ZBuf_XSlope]
			mov	dword ptr [@@DecZBuf+2],eax

			shr	ebx,8
			mov	eax,[Log]

			and	ebx,0FFFFh
			mov	dword ptr [@@PtrLog+2],eax

			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture10::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrZBuffer1:		mov	word ptr [edi*2+12345678h],dx
@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFlatChromaKeyZBuf ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFogZBuf	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@PtrZBuffer1+4],ebx

			mov	ebx,[Fill_ZBuf_XSlope]
			mov	dword ptr [@@DecZBuf+2],eax

			shr	ebx,8
			mov	eax,[Log]

			and	ebx,0FFFFh
			mov	dword ptr [@@PtrLog+2],eax

			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture11::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrZBuffer1:		mov	word ptr [edi*2+12345678h],dx
			mov	bl,[ebx+Fill_Logical_Palette]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFogZBuf	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureChromaKeyFogZBuf PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@PtrZBuffer1+4],ebx

			mov	ebx,[Fill_ZBuf_XSlope]
			mov	dword ptr [@@DecZBuf+2],eax

			shr	ebx,8
			mov	eax,[Log]

			and	ebx,0FFFFh
			mov	dword ptr [@@PtrLog+2],eax

			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture12::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrZBuffer1:		mov	word ptr [edi*2+12345678h],dx
			mov	bl,[ebx+Fill_Logical_Palette]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureChromaKeyFogZBuf ENDP











; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureNZW	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			mov	dword ptr [@@DecMapU+2],eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			mov	ebx,[PtrZBuffer]

			mov	[Precalc_Inc],eax
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf+2],eax

			mov	eax,[Log]
			mov	dword ptr [@@IntZBuf+2],ebx

			pop	ebx
			mov	dword ptr [@@PtrLog+2],eax

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture13::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:

			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	eax,[Log]
			mov	ebx,[Precalc_Inc]

			shl	ebp,16
			sub	edi,eax

			or	ebx,esi
			mov	eax,[RepMask]

			and	eax,edx
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
			inc	edi
@@DecMapU:		add	ebx,12345678h

@@IntMapU:		adc	al,12
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecZBuf:		add	esi,12345678h

@@IntZBuf:		adc	edx,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureNZW	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureChromaKeyNZW PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			mov	dword ptr [@@DecMapU+2],eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	eax,0FFh

			or	eax,ebx
			mov	ebx,[PtrZBuffer]

			mov	[Precalc_Inc],eax
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf+2],eax

			mov	eax,[Log]
			mov	dword ptr [@@IntZBuf+2],ebx

			pop	ebx
			mov	dword ptr [@@PtrLog+2],eax

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture14::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:

			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	eax,[Log]
			mov	ebx,[Precalc_Inc]

			shl	ebp,16
			sub	edi,eax

			or	ebx,esi
			mov	eax,[RepMask]

			and	eax,edx
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
			inc	edi
@@DecMapU:		add	ebx,12345678h

@@IntMapU:		adc	al,12
@@DecMapV:		add	ebp,12345678h

			adc	ah,bh
@@DecZBuf:		add	esi,12345678h

@@IntZBuf:		adc	edx,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureChromaKeyNZW ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFlatNZW	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[Fill_Color]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf+2],eax

			mov	eax,[Log]
			and	ebx,0FFFFh

			mov	dword ptr [@@PtrLog+2],eax
			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture15::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFlatNZW	ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFlatChromaKeyNZW PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[Fill_Color]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf+2],eax

			mov	eax,[Log]
			and	ebx,0FFFFh

			mov	dword ptr [@@PtrLog+2],eax
			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture16::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFlatChromaKeyNZW ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureFogNZW	PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf+2],eax

			mov	eax,[Log]
			and	ebx,0FFFFh

			mov	dword ptr [@@PtrLog+2],eax
			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture17::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

			mov	bl,[ebx+Fill_Logical_Palette]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureFogNZW	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureChromaKeyFogNZW PROC
			inc	ecx
			mov	eax,[Fill_CurY]

			add	eax,ecx
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	esi,[Fill_CurMapUMin]	; ESI = MapU

			mov	eax,[Fill_Patch]
			mov	ebp,[Fill_CurMapVMin]	; EBP = MapV

			dec	eax
			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	ebx,[PtrMap]
			mov	eax,[Fill_MapU_XSlope]

			shl	eax,16
			mov	dword ptr [@@PtrMap+2],ebx

			push	eax
			mov	ebx,[Fill_MapV_XSlope]

			shl	ebx,16
			mov	eax,[RepMask]

			mov	dword ptr [@@DecMapV+2],ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	eax,[Fill_MapU_XSlope]
			mov	ebx,[Fill_MapV_XSlope]

			shr	eax,16

			shr	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	byte ptr [@@IntMapV+2],bl
			mov	ebx,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf+2],eax

			mov	eax,[Log]
			and	ebx,0FFFFh

			mov	dword ptr [@@PtrLog+2],eax
			pop	eax

			or	eax,ebx
			pop	ebx

			mov	dword ptr [@@DecMapUIntZBuf+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTexture18::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			push	ecx
			mov	eax,ebx

			shr	eax,16
			mov	ecx,edx

			shr	ecx,16
			push	edi

			sub	ecx,eax
			jle	@@EndScanLine

			add	edi,eax
			push	ebp

			push	esi
			push	ebx

			push	edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_MapU_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	edx,eax
			mov	eax,[Fill_MapV_XSlope]

			add	esi,edx

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	ebp,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***
@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi

@@FastInit:
			mov	edx,esi
			mov	ebx,ebp

			shr	edx,16
			and	ebx,0FF0000h

			shr	ebx,8
			and	edx,0FFh

			shl	esi,16
			or	edx,ebx

			mov	ebx,[Log]
			mov	eax,[RepMask]

			sub	edi,ebx
			and	eax,edx

			shl	ebp,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			xor	ebx,ebx

			or	edx,esi
			mov	esi,[Fill_CurZBufMin]

			shl	esi,32-8



@@Each_X:
@@PtrZBuffer:		cmp	word ptr [edi*2+12345678h],dx
			jb	@@NoDisp

@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

			mov	bl,[ebx+Fill_Logical_Palette]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecZBuf:		add	esi,12345678h

@@DecMapUIntZBuf:	adc	edx,12345678h
			inc	edi

@@IntMapU:		adc	al,12h
@@DecMapV:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h

@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X


@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi
			pop	ecx

			dec	ecx
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurMapUMin],esi

			mov	[Fill_CurMapVMin],ebp
			jmp	Triangle_ReadNextEdge
Filler_TextureChromaKeyFogNZW ENDP








;				*******************
;				*** Local proc ***
;				*******************


Last_POLYTEXTUREPatch	Label	DWORD

_TEXT			ENDS

;			The
			End
