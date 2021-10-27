; **************************************************************************
; **************************************************************************
; **************************************************************************
; ***                                                                    ***
; ***                         POLYGON LIBRARY                            ***
; ***                                                                    ***
; **************************************************************************
; ***                                                                    ***
; ***                         (Adeline Software Intl.)                   ***
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
PUBLIC	SYSCALL		First_POLYGOURAUDTEXTUREPatch
PUBLIC	SYSCALL		Last_POLYGOURAUDTEXTUREPatch

PUBLIC	SYSCALL		Filler_TextureGouraud
PUBLIC	SYSCALL		Filler_TextureDither
PUBLIC	SYSCALL		Filler_TextureGouraudChromaKey
PUBLIC	SYSCALL		Filler_TextureDitherChromaKey
PUBLIC	SYSCALL		Filler_TextureGouraudZBuf
PUBLIC	SYSCALL		Filler_TextureGouraudChromaKeyZBuf
PUBLIC	SYSCALL		Filler_TextureGouraudNZW
PUBLIC	SYSCALL		Filler_TextureGouraudChromaKeyNZW



;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***
PUBLIC	SYSCALL		Fill_VLGouraudTexture1
PUBLIC	SYSCALL		Fill_VLGouraudTexture2
PUBLIC	SYSCALL		Fill_VLGouraudTexture3
PUBLIC	SYSCALL		Fill_VLGouraudTexture4
PUBLIC	SYSCALL		Fill_VLGouraudTexture5
PUBLIC	SYSCALL		Fill_VLGouraudTexture6
PUBLIC	SYSCALL		Fill_VLGouraudTexture7
PUBLIC	SYSCALL		Fill_VLGouraudTexture8


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

Extrn	SYSCALL		Fill_CurGouraudMin	:	DWORD
Extrn	SYSCALL		Fill_Gouraud_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_Gouraud_XSlope	:	DWORD

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
First_POLYGOURAUDTEXTUREPatch	Label	DWORD


; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureGouraud	PROC
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
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			push	ebx
			mov	ebx,eax

			shl	eax,16
			push	ecx

			shr	ebx,16
			mov	dword ptr [@@DecMapU+2],eax

			mov	byte ptr [@@IntMapU+1],bl
			mov	eax,[Fill_MapV_XSlope]

			mov	ecx,[Fill_Gouraud_XSlope]
			mov	ebx,eax

			shr	ecx,8

			shr	ebx,16
			and	ecx,0FFFFh

			shl	eax,16
			mov	byte ptr [@@IntMapV+2],bl

			or	eax,ecx
			pop	ecx

			mov	dword ptr [@@DecMapV_Gour+2],eax

			pop	ebx
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture1::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurGouraudMin],eax
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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_Gouraud_XSlope]

			push	edx

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx
			pop	edx

			add	edx,eax

			push	edx
			mov	eax,[Fill_MapU_XSlope]

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
			mov	ebx,esi
			mov	edx,ebp

			shr	edx,16

			shr	ebx,16
			mov	ah,dl

			shl	esi,16
			mov	al,bl

			shl	ebp,16
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			mov	ebx,edx
			and	edx,0FFFFh

			or	edx,ebp
			mov	ebp,[RepMask]

			and	ebx,0FFFFh
			and	eax,ebp

@@Each_X:
@@PtrMap:		mov	bl,[eax+12345678h]
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecMapV_Gour:		add	edx,12345678h

@@IntMapV:		adc	ah,12h
@@PtrCLUT:		mov	bl,[ebx+12345678h]

			mov	[edi],bl
			inc	edi

			mov	bh,dh
			and	eax,ebp

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
Filler_TextureGouraud	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureDither	PROC
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
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			push	ebx
			mov	ebx,eax

			shl	eax,16
			push	ecx

			shr	ebx,16
			mov	dword ptr [@@DecMapU+2],eax

			mov	byte ptr [@@IntMapU+1],bl
			mov	eax,[Fill_MapV_XSlope]

			mov	ecx,[Fill_Gouraud_XSlope]
			mov	ebx,eax

			shr	ecx,8

			shr	ebx,16
			and	ecx,0FFFFh

			shl	eax,16
			mov	byte ptr [@@IntMapV+2],bl

			or	eax,ecx
			pop	ecx

			mov	dword ptr [@@DecMapV_Gour+2],eax

			pop	ebx
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture2::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurGouraudMin],eax
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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_MapU_XSlope]

			push	edx

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
			mov	ebx,esi
			mov	edx,ebp

			shr	edx,16

			shr	ebx,16
			mov	ah,dl

			shl	esi,16
			mov	al,bl

			shl	ebp,16
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			mov	ebx,edx
			and	edx,0FFFFh

			or	ebp,edx

			and	ebx,0FFFFh
			and	eax,[RepMask]

@@Each_X:
@@PtrMap:		mov	bl,[eax+12345678h]
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecMapV_Gour:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h
@@PtrCLUT:		mov	bl,[ebx+12345678h]

			xor	dh,dh
			mov	[edi],bl

			add	edx,ebp
			inc	edi

			mov	bh,dh
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
Filler_TextureDither	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureGouraudChromaKey	PROC
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
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	dword ptr [@@PtrMap2+2],eax

			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			push	ebx
			mov	ebx,eax

			shl	eax,16
			push	ecx

			shr	ebx,16
			mov	dword ptr [@@DecMapU+2],eax

			mov	dword ptr [@@DecMapU2+2],eax
			mov	byte ptr [@@IntMapU+1],bl

			mov	byte ptr [@@IntMapU2+1],bl
			mov	eax,[Fill_MapV_XSlope]

			mov	ecx,[Fill_Gouraud_XSlope]
			mov	ebx,eax

			shr	ecx,8

			shr	ebx,16
			and	ecx,0FFFFh

			shl	eax,16
			mov	byte ptr [@@IntMapV+2],bl
			mov	byte ptr [@@IntMapV2+2],bl

			or	eax,ecx
			pop	ecx

			mov	dword ptr [@@DecMapV_Gour+2],eax
			mov	dword ptr [@@DecMapV_Gour2+2],eax

			pop	ebx
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture3::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurGouraudMin],eax
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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_Gouraud_XSlope]

			push	edx

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx
			pop	edx

			add	edx,eax

			push	edx
			mov	eax,[Fill_MapU_XSlope]

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
			mov	ebx,esi
			mov	edx,ebp

			shr	edx,16

			shr	ebx,16
			mov	ah,dl

			shl	esi,16
			mov	al,bl

			shl	ebp,16
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			mov	ebx,edx
			and	edx,0FFFFh

			or	edx,ebp
			mov	ebp,[RepMask]

			and	ebx,0FFFFh
			and	eax,ebp

@@Each_X:
@@PtrMap:		mov	bl,[eax+12345678h]
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecMapV_Gour:		add	edx,12345678h

@@IntMapV:		adc	ah,12h
			inc	edi

			test	bl,bl
			je	@@NotVisible
@@Visible:
@@PtrCLUT:		mov	bl,[ebx+12345678h]
			and	eax,ebp

			mov	[edi-1],bl
			mov	bh,dh

			dec	ecx
			jne	@@Each_X
			jmp	@@End_X

@@Each_X2:
@@PtrMap2:		mov	bl,[eax+12345678h]
@@DecMapU2:		add	esi,12345678h

@@IntMapU2:		adc	al,12h
@@DecMapV_Gour2:	add	edx,12345678h

@@IntMapV2:		adc	ah,12h
			inc	edi

			test	bl,bl
			jne	@@Visible
@@NotVisible:
			mov	bh,dh
			and	eax,ebp

			dec	ecx
			jne	@@Each_X2


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
Filler_TextureGouraudChromaKey	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureDitherChromaKey	PROC
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
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	dword ptr [@@PtrMap2+2],eax

			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[Fill_MapU_XSlope]

			push	ebx
			mov	ebx,eax

			shl	eax,16
			push	ecx

			shr	ebx,16
			mov	dword ptr [@@DecMapU+2],eax

			mov	dword ptr [@@DecMapU2+2],eax
			mov	byte ptr [@@IntMapU+1],bl

			mov	byte ptr [@@IntMapU2+1],bl
			mov	eax,[Fill_MapV_XSlope]

			mov	ecx,[Fill_Gouraud_XSlope]
			mov	ebx,eax

			shr	ecx,8

			shr	ebx,16
			and	ecx,0FFFFh

			shl	eax,16
			mov	byte ptr [@@IntMapV+2],bl

			mov	byte ptr [@@IntMapV2+2],bl
			or	eax,ecx

			pop	ecx
			mov	dword ptr [@@DecMapV_Gour+2],eax

			mov	dword ptr [@@DecMapV_Gour2+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	dword ptr [@@RepMask2+1],eax

			pop	ebx
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture4::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurGouraudMin],eax
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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_MapU_XSlope]

			push	edx

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
			mov	ebx,esi
			mov	edx,ebp

			shr	edx,16

			shr	ebx,16
			mov	ah,dl

			shl	esi,16
			mov	al,bl

			shl	ebp,16
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			mov	ebx,edx
			and	edx,0FFFFh

			or	ebp,edx

			and	ebx,0FFFFh
			and	eax,[RepMask]

@@Each_X:
@@PtrMap:		mov	bl,[eax+12345678h]
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecMapV_Gour:		add	ebp,12345678h

@@IntMapV:		adc	ah,12h
			inc	edi

			test	bl,bl
			je	@@NotVisible
@@Visible:
@@PtrCLUT:		mov	bl,[ebx+12345678h]
			xor	dh,dh

			mov	[edi-1],bl
			add	edx,ebp

			mov	bh,dh
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@Each_X
			jmp	@@End_X

@@Each_X2:
@@PtrMap2:		mov	bl,[eax+12345678h]
@@DecMapU2:		add	esi,12345678h

@@IntMapU2:		adc	al,12h
@@DecMapV_Gour2:	add	ebp,12345678h

@@IntMapV2:		adc	ah,12h
			inc	edi

			test	bl,bl
			jne	@@Visible
@@NotVisible:
			xor	dh,dh
@@RepMask2:		and	eax,12345678h

			add	edx,ebp
			dec	ecx

			mov	bh,dh
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
Filler_TextureDitherChromaKey	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureGouraudZBuf PROC
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

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	eax,[Fill_MapU_XSlope]

			shr	eax,16
			mov	ebx,[Fill_MapU_XSlope]

			shl	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@DecMapU+2],ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			shr	ebx,8
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			and	ebx,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_Gour+2],eax

			mov	eax,[Fill_MapV_XSlope]

			shl	eax,16

			or	eax,ebx
			mov	ebx,[Fill_MapV_XSlope]

			shr	ebx,16
			mov	dword ptr [@@DecMapV_IntZBuf+2],eax

			mov	byte ptr [@@IntMapV+2],bl
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture5::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurGouraudMin],eax
			mov	eax,[Fill_CurZBufMin]

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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_Gouraud_XSlope]

			push	edx

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx
			pop	edx

			add	edx,eax

			push	edx
			mov	eax,[Fill_MapU_XSlope]

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
			mov	eax,esi
			mov	ebx,ebp

			shr	eax,16
			mov	edx,[Log]

			shr	ebx,16
			sub	edi,edx

			mov	ah,bl
			mov	ebx,[RepMask]

			shl	ebp,16
			and	eax,ebx

			shl	esi,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			mov	ebx,[Fill_CurZBufMin]

			or	ebp,edx
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			shl	ebx,32-8

			or	edx,ebx
			xor	ebx,ebx
			mov	bh,dh
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bp
			jb	@@NoDisp
@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrZBuffer1:		mov	[edi*2+12345678h],bp
@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecZBuf_Gour:		add	edx,12345678h

@@DecMapV_IntZBuf:	adc	ebp,12345678h
			inc	edi

@@IntMapV:		adc	ah,12h
			mov	bh,dh

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
Filler_TextureGouraudZBuf ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureGouraudChromaKeyZBuf PROC
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

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	eax,[Fill_MapU_XSlope]

			shr	eax,16
			mov	ebx,[Fill_MapU_XSlope]

			shl	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@DecMapU+2],ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			shr	ebx,8
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			and	ebx,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_Gour+2],eax

			mov	eax,[Fill_MapV_XSlope]

			shl	eax,16

			or	eax,ebx
			mov	ebx,[Fill_MapV_XSlope]

			shr	ebx,16
			mov	dword ptr [@@DecMapV_IntZBuf+2],eax

			mov	byte ptr [@@IntMapV+2],bl
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture6::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurGouraudMin],eax
			mov	eax,[Fill_CurZBufMin]

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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_Gouraud_XSlope]

			push	edx

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx
			pop	edx

			add	edx,eax

			push	edx
			mov	eax,[Fill_MapU_XSlope]

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
			mov	eax,esi
			mov	ebx,ebp

			shr	eax,16
			mov	edx,[Log]

			shr	ebx,16
			sub	edi,edx

			mov	ah,bl
			mov	ebx,[RepMask]

			shl	ebp,16
			and	eax,ebx

			shl	esi,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			mov	ebx,[Fill_CurZBufMin]

			or	ebp,edx
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			shl	ebx,32-8

			or	edx,ebx
			xor	ebx,ebx
			mov	bh,dh
@@Each_X:
@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrZBuffer:		cmp	[edi*2+12345678h],bp
			jb	@@NoDisp

@@PtrZBuffer1:		mov	[edi*2+12345678h],bp
@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecZBuf_Gour:		add	edx,12345678h

@@DecMapV_IntZBuf:	adc	ebp,12345678h
			inc	edi

@@IntMapV:		adc	ah,12h
			mov	bh,dh

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
Filler_TextureGouraudChromaKeyZBuf ENDP











; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureGouraudNZW PROC
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

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	eax,[Fill_MapU_XSlope]

			shr	eax,16
			mov	ebx,[Fill_MapU_XSlope]

			shl	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@DecMapU+2],ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			shr	ebx,8
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			and	ebx,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_Gour+2],eax

			mov	eax,[Fill_MapV_XSlope]

			shl	eax,16

			or	eax,ebx
			mov	ebx,[Fill_MapV_XSlope]

			shr	ebx,16
			mov	dword ptr [@@DecMapV_IntZBuf+2],eax

			mov	byte ptr [@@IntMapV+2],bl
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture7::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurGouraudMin],eax
			mov	eax,[Fill_CurZBufMin]

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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_Gouraud_XSlope]

			push	edx

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx
			pop	edx

			add	edx,eax

			push	edx
			mov	eax,[Fill_MapU_XSlope]

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
			mov	eax,esi
			mov	ebx,ebp

			shr	eax,16
			mov	edx,[Log]

			shr	ebx,16
			sub	edi,edx

			mov	ah,bl
			mov	ebx,[RepMask]

			shl	ebp,16
			and	eax,ebx

			shl	esi,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			mov	ebx,[Fill_CurZBufMin]

			or	ebp,edx
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			shl	ebx,32-8

			or	edx,ebx
			xor	ebx,ebx
			mov	bh,dh
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bp
			jb	@@NoDisp
@@PtrMap:		mov	bl,[eax+12345678h]

@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecZBuf_Gour:		add	edx,12345678h

@@DecMapV_IntZBuf:	adc	ebp,12345678h
			inc	edi

@@IntMapV:		adc	ah,12h
			mov	bh,dh

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
Filler_TextureGouraudNZW ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureGouraudChromaKeyNZW PROC
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

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	eax,[Fill_MapU_XSlope]

			shr	eax,16
			mov	ebx,[Fill_MapU_XSlope]

			shl	ebx,16
			mov	byte ptr [@@IntMapU+1],al

			mov	dword ptr [@@DecMapU+2],ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			shr	ebx,8
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			and	ebx,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_Gour+2],eax

			mov	eax,[Fill_MapV_XSlope]

			shl	eax,16

			or	eax,ebx
			mov	ebx,[Fill_MapV_XSlope]

			shr	ebx,16
			mov	dword ptr [@@DecMapV_IntZBuf+2],eax

			mov	byte ptr [@@IntMapV+2],bl
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraudTexture8::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_Gouraud_LeftSlope]
			mov	eax,[Fill_CurGouraudMin]

			add	eax,ebx
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurGouraudMin],eax
			mov	eax,[Fill_CurZBufMin]

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

			mov	al,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	edx,[Fill_CurGouraudMin]
			mov	eax,[Fill_Gouraud_XSlope]

			push	edx

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx
			pop	edx

			add	edx,eax

			push	edx
			mov	eax,[Fill_MapU_XSlope]

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
			mov	eax,esi
			mov	ebx,ebp

			shr	eax,16
			mov	edx,[Log]

			shr	ebx,16
			sub	edi,edx

			mov	ah,bl
			mov	ebx,[RepMask]

			shl	ebp,16
			and	eax,ebx

			shl	esi,16
			mov	edx,[Fill_CurZBufMin]

			shr	edx,8
			mov	ebx,[Fill_CurZBufMin]

			or	ebp,edx
			pop	edx		; Fill_CurGouraudMin

			shr	edx,8

			shl	ebx,32-8

			or	edx,ebx
			xor	ebx,ebx
			mov	bh,dh
@@Each_X:
@@PtrMap:		mov	bl,[eax+12345678h]

			test	bl,bl
			je	@@NoDisp

@@PtrZBuffer:		cmp	[edi*2+12345678h],bp
			jb	@@NoDisp

@@PtrCLUT:		mov	bl,[ebx+12345678h]

@@PtrLog:		mov	[edi+12345678h],bl
@@NoDisp:
@@DecMapU:		add	esi,12345678h

@@IntMapU:		adc	al,12h
@@DecZBuf_Gour:		add	edx,12345678h

@@DecMapV_IntZBuf:	adc	ebp,12345678h
			inc	edi

@@IntMapV:		adc	ah,12h
			mov	bh,dh

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
Filler_TextureGouraudChromaKeyNZW ENDP














;				*******************
;				*** Local proc ***
;				*******************


Last_POLYGOURAUDTEXTUREPatch Label	DWORD

_TEXT			ENDS

;			The
			End
