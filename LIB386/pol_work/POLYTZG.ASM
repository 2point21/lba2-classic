; **************************************************************************
; **************************************************************************
; **************************************************************************
; ***                                                                    ***
; ***                         POLYGON LIBRARY                            ***
; ***                                                                    ***
; **************************************************************************
; ***                                                                    ***
; ***                    (Adeline Software Intl.)                        ***
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
PUBLIC	SYSCALL		First_POLYTEXTUREZGOURAUDPatch
PUBLIC	SYSCALL		Last_POLYTEXTUREZGOURAUDPatch

PUBLIC	SYSCALL		Filler_TextureZGouraud
PUBLIC	SYSCALL		Filler_TextureZGouraudChromaKey
PUBLIC	SYSCALL		Filler_TextureZGouraudZBuf
PUBLIC	SYSCALL		Filler_TextureZGouraudChromaKeyZBuf
PUBLIC	SYSCALL		Filler_TextureZGouraudNZW
PUBLIC	SYSCALL		Filler_TextureZGouraudChromaKeyNZW




;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***
PUBLIC	SYSCALL		Fill_VLTextureZGouraud1
PUBLIC	SYSCALL		Fill_VLTextureZGouraud2
PUBLIC	SYSCALL		Fill_VLTextureZGouraud3
PUBLIC	SYSCALL		Fill_VLTextureZGouraud4
PUBLIC	SYSCALL		Fill_VLTextureZGouraud5
PUBLIC	SYSCALL		Fill_VLTextureZGouraud6


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

Extrn	SYSCALL		Fill_CurWMin		:	DWORD
Extrn	SYSCALL		Fill_W_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_W_XSlope		:	DWORD

Extrn	SYSCALL		Fill_CurGouraudMin	:	DWORD
Extrn	SYSCALL		Fill_Gouraud_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_Gouraud_XSlope 	:	DWORD

Extrn	SYSCALL		Fill_CurZBufMin		:	DWORD
Extrn	SYSCALL		Fill_ZBuf_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_ZBuf_XSlope	:	DWORD
Extrn	SYSCALL		Fill_CurZBuf		:	DWORD

Extrn	SYSCALL		Fill_Cur_W		:	DWORD
Extrn	SYSCALL		Fill_Next_W		:	DWORD
Extrn	SYSCALL		Fill_Cur_MapU		:	DWORD
Extrn	SYSCALL		Fill_Next_MapU		:	DWORD
Extrn	SYSCALL		Fill_Cur_MapV		:	DWORD
Extrn	SYSCALL		Fill_Next_MapV		:	DWORD
Extrn	SYSCALL		Fill_Cur_MapUOverW	:	DWORD
Extrn	SYSCALL		Fill_Next_MapUOverW	:	DWORD
Extrn	SYSCALL		Fill_Cur_MapVOverW	:	DWORD
Extrn	SYSCALL		Fill_Next_MapVOverW	:	DWORD

Extrn	SYSCALL		Fill_Init_Perspective	:	DWORD
Extrn	SYSCALL		Fill_Loop_Perspective	:	DWORD





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
Cur_Gouraud		dd	0






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
First_POLYTEXTUREZGOURAUDPatch	Label	DWORD






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZGouraud	PROC
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
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			push	ebx

			mov	eax,[RepMask]
			mov	ebx,[Fill_Gouraud_XSlope]

			mov	dword ptr [@@RepMask+1],eax
			mov	eax,ebx

			shl	ebx,16

			shr	eax,16
			mov	dword ptr [@@DecGour+2],ebx

			mov	byte ptr [@@IntGour+2],al
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZGouraud1::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_W_LeftSlope]
			mov	eax,[Fill_CurWMin]

			add	eax,ebx
			mov	ebx,[Fill_Gouraud_LeftSlope]

			mov	[Fill_CurWMin],eax
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
			mov	eax,[Fill_CurGouraudMin]

			mov	[Cur_Gouraud],eax
			mov	edx,[Fill_CurWMin]

			mov	[Fill_Cur_W],edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_W_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx

			mov	edx,[Fill_CurWMin]

			add	edx,eax
			mov	eax,[Fill_MapU_XSlope]

			mov	[Fill_Cur_W],edx

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
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,PERSP_SHIFT			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,PERSP_SHIFT			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	eax,ebx

			shr	eax,16
			mov	edx,[RepMask]

			mov	al,bh
			push	ecx

			and	eax,edx
			cmp	ecx,PERSP_VALUE

			jle	@@LessPERSP_VALUE

			mov	edx,[Cur_Gouraud]
			mov	ecx,-PERSP_VALUE			; Rasterize PERSP_VALUE pix

			mov	esi,edx
			add	edi,PERSP_VALUE

			shl	esi,16

			shr	edx,8
			jmp	@@LoopX
@@LessPERSP_VALUE:
			lea	edi,[edi+ecx]		; Rasterize less than PERSP_VALUE pix
			mov	edx,[Cur_Gouraud]

			xor	ecx,-1
			mov	esi,edx

			shl	esi,16
			inc	ecx

			shr	edx,8
@@LoopX:
@@PtrMap:		mov	dl,[eax+12345678h]
			lea	eax,[ebx+ebp]

			shr	eax,16
			add	ebx,ebp

@@PtrCLUT:		mov	dl,[edx+12345678h]
			mov	al,bh

			mov	[edi+ecx],dl
@@DecGour:		add	esi,12345678h

@@IntGour:		adc	dh,12h
@@RepMask:		and	eax,12345678h

			inc	ecx
			jne	@@LoopX

			pop	ecx
			mov	eax,[Fill_Next_MapU]

			cmp	ecx,PERSP_VALUE
			jle	@@End_X

			shl	edx,8

			shr	esi,16
			and	edx,0FF0000h

			or	edx,esi
			mov	ebx,[Fill_Next_MapV]

			mov	[Cur_Gouraud],edx
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_Cur_MapV],ebx
			mov	eax,[Fill_Next_MapUOverW]

			mov	ebx,[Fill_Next_MapVOverW]
			mov	[Fill_Cur_MapUOverW],eax

			mov	eax,[Fill_Next_W]
			mov	[Fill_Cur_MapVOverW],ebx

			mov	[Fill_Cur_W],eax
			sub	ecx,PERSP_VALUE

			jg	@@FastFill

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
Filler_TextureZGouraud	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZGouraudChromaKey	PROC
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
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			push	ebx

			mov	eax,[RepMask]
			mov	ebx,[Fill_Gouraud_XSlope]

			mov	dword ptr [@@RepMask+1],eax
			mov	dword ptr [@@RepMask2+1],eax

			mov	eax,ebx

			shl	ebx,16

			shr	eax,16
			mov	dword ptr [@@DecGour+2],ebx

			mov	dword ptr [@@DecGour2+2],ebx
			mov	byte ptr [@@IntGour+2],al

			mov	byte ptr [@@IntGour2+2],al
			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	dword ptr [@@PtrMap2+2],eax

			pop	ebx
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZGouraud2::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_W_LeftSlope]
			mov	eax,[Fill_CurWMin]

			add	eax,ebx
			mov	ebx,[Fill_Gouraud_LeftSlope]

			mov	[Fill_CurWMin],eax
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
			mov	eax,[Fill_CurGouraudMin]

			mov	[Cur_Gouraud],eax
			mov	edx,[Fill_CurWMin]

			mov	[Fill_Cur_W],edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_W_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx

			mov	edx,[Fill_CurWMin]

			add	edx,eax
			mov	eax,[Fill_MapU_XSlope]

			mov	[Fill_Cur_W],edx

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
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,PERSP_SHIFT			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,PERSP_SHIFT			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	eax,ebx

			shr	eax,16
			mov	edx,[RepMask]

			mov	al,bh
			push	ecx

			and	eax,edx
			cmp	ecx,PERSP_VALUE

			jle	@@LessPERSP_VALUE

			mov	edx,[Cur_Gouraud]
			mov	ecx,-PERSP_VALUE			; Rasterize PERSP_VALUE pix

			mov	esi,edx
			add	edi,PERSP_VALUE

			shl	esi,16

			shr	edx,8
			jmp	@@LoopX
@@LessPERSP_VALUE:
			lea	edi,[edi+ecx]		; Rasterize less than PERSP_VALUE pix
			mov	edx,[Cur_Gouraud]

			xor	ecx,-1
			mov	esi,edx

			shl	esi,16
			inc	ecx

			shr	edx,8
@@LoopX:
@@PtrMap:		mov	dl,[eax+12345678h]
			lea	eax,[ebx+ebp]

			shr	eax,16
			add	ebx,ebp

			test	dl,dl
			je	@@NotVisible
@@Visible:
@@PtrCLUT:		mov	dl,[edx+12345678h]
			mov	al,bh

			mov	[edi+ecx],dl
@@DecGour:		add	esi,12345678h

@@IntGour:		adc	dh,12h
@@RepMask:		and	eax,12345678h

			inc	ecx
			jne	@@LoopX
			jmp	@@EndRaster


@@LoopX2:
@@PtrMap2:		mov	dl,[eax+12345678h]
			lea	eax,[ebx+ebp]

			shr	eax,16
			add	ebx,ebp

			test	dl,dl
			jne	@@Visible
@@NotVisible:
			mov	al,bh
@@DecGour2:		add	esi,12345678h

@@IntGour2:		adc	dh,12h
@@RepMask2:		and	eax,12345678h

			inc	ecx
			jne	@@LoopX2

@@EndRaster:


			pop	ecx
			mov	eax,[Fill_Next_MapU]

			cmp	ecx,PERSP_VALUE
			jle	@@End_X

			shl	edx,8

			shr	esi,16
			and	edx,0FF0000h

			or	edx,esi
			mov	ebx,[Fill_Next_MapV]

			mov	[Cur_Gouraud],edx
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_Cur_MapV],ebx
			mov	eax,[Fill_Next_MapUOverW]

			mov	ebx,[Fill_Next_MapVOverW]
			mov	[Fill_Cur_MapUOverW],eax

			mov	eax,[Fill_Next_W]
			mov	[Fill_Cur_MapVOverW],ebx

			mov	[Fill_Cur_W],eax
			sub	ecx,PERSP_VALUE

			jg	@@FastFill

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
Filler_TextureZGouraudChromaKey	ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZGouraudZBuf PROC
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
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			push	ebx

			mov	eax,[RepMask]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@RepMask+1],eax
			mov	dword ptr [@@RepMask2+1],eax

			mov	dword ptr [@@PtrMap+2],ebx
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	eax,[Fill_Gouraud_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			and	eax,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_AddGour+2],eax

			mov	dword ptr [@@DecZBuf_AddGour2+2],eax
			mov	dword ptr [@@IntZBuf+2],ebx

			mov	dword ptr [@@IntZBuf2+2],ebx
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZGouraud3::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_W_LeftSlope]
			mov	eax,[Fill_CurWMin]

			add	eax,ebx
			mov	ebx,[Fill_Gouraud_LeftSlope]

			mov	[Fill_CurWMin],eax
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
			mov	eax,[Fill_CurGouraudMin]

			mov	[Cur_Gouraud],eax
			mov	eax,[Fill_CurZBufMin]

			mov	[Fill_CurZBuf],eax
			mov	edx,[Fill_CurWMin]

			mov	[Fill_Cur_W],edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_W_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx

			mov	edx,[Fill_CurWMin]

			add	edx,eax
			mov	eax,[Fill_MapU_XSlope]

			mov	[Fill_Cur_W],edx

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
			mov	eax,[Log]

			sub	edi,eax
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,PERSP_SHIFT			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,PERSP_SHIFT			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	eax,ebx

			shr	eax,16
			mov	edx,[RepMask]

			mov	al,bh
			push	ecx

			and	eax,edx
			cmp	ecx,PERSP_VALUE

			jle	@@LessPERSP_VALUE

			mov	ecx,PERSP_VALUE			; Rasterize PERSP_VALUE pix
@@LessPERSP_VALUE:               				; Rasterize less than PERSP_VALUE pix
			mov	edx,[Cur_Gouraud]
			mov	esi,[Fill_CurZBuf]

			shr	edx,8

			shl	esi,32-8

			or	edx,esi
			mov	esi,[Fill_CurZBuf]

			shr	esi,8
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],si
			jb	@@NoDisp
@@Disp:
@@PtrMap:		mov	al,[eax+12345678h]
			add	ebx,ebp

			mov	ah,dh

@@PtrZBuffer1:		mov	[edi*2+12345678h],si
@@PtrCLUT:		mov	al,[eax+12345678h]

@@PtrLog:		mov	[edi+12345678h],al
			mov	eax,ebx

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf_AddGour:	add	edx,12345678h

@@IntZBuf:		adc	esi,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@LoopX

			jmp	@@EndPERSP_VALUE

@@LoopX2:
@@PtrZBuffer2:		cmp	[edi*2+12345678h],si
			jae	@@Disp
@@NoDisp:
			lea	eax,[ebx+ebp]
			add	ebx,ebp

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf_AddGour2:	add	edx,12345678h

@@IntZBuf2:		adc	esi,12345678h
@@RepMask2:		and	eax,12345678h

			dec	ecx
			jne	@@LoopX2



@@EndPERSP_VALUE:
			pop	ecx
			mov	eax,edx

			cmp	ecx,PERSP_VALUE
			jle	@@End_X

			shl	eax,8
			mov	ebx,[Fill_Next_MapV]

			shr	edx,32-8
			mov	[Cur_Gouraud],eax

			shl	esi,8
			mov	eax,[Fill_Next_MapU]

			or	edx,esi
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_CurZBuf],edx
			mov	[Fill_Cur_MapV],ebx

			mov	eax,[Fill_Next_MapUOverW]
			mov	ebx,[Fill_Next_MapVOverW]

			mov	[Fill_Cur_MapUOverW],eax
			mov	eax,[Fill_Next_W]

			mov	[Fill_Cur_MapVOverW],ebx
			mov	[Fill_Cur_W],eax

			sub	ecx,PERSP_VALUE
			jg	@@FastFill

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
Filler_TextureZGouraudZBuf ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZGouraudChromaKeyZBuf PROC
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
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			push	ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	ebx,[PtrMap]
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrMap+2],ebx
			mov	dword ptr [@@PtrZBuffer+4],eax

			mov	dword ptr [@@PtrZBuffer1+4],eax
			mov	eax,[Fill_Gouraud_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			and	eax,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_AddGour+2],eax

			mov	dword ptr [@@IntZBuf+2],ebx
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZGouraud4::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_W_LeftSlope]
			mov	eax,[Fill_CurWMin]

			add	eax,ebx
			mov	ebx,[Fill_Gouraud_LeftSlope]

			mov	[Fill_CurWMin],eax
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
			mov	eax,[Fill_CurGouraudMin]

			mov	[Cur_Gouraud],eax
			mov	eax,[Fill_CurZBufMin]

			mov	[Fill_CurZBuf],eax
			mov	edx,[Fill_CurWMin]

			mov	[Fill_Cur_W],edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_W_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx

			mov	edx,[Fill_CurWMin]

			add	edx,eax
			mov	eax,[Fill_MapU_XSlope]

			mov	[Fill_Cur_W],edx

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
			mov	eax,[Log]

			sub	edi,eax
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,PERSP_SHIFT			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,PERSP_SHIFT			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	eax,ebx

			shr	eax,16
			mov	edx,[RepMask]

			mov	al,bh
			push	ecx

			and	eax,edx
			cmp	ecx,PERSP_VALUE

			jle	@@LessPERSP_VALUE

			mov	ecx,PERSP_VALUE			; Rasterize PERSP_VALUE pix
@@LessPERSP_VALUE:               				; Rasterize less than PERSP_VALUE pix
			mov	edx,[Cur_Gouraud]
			mov	esi,[Fill_CurZBuf]

			shr	edx,8

			shl	esi,32-8

			or	edx,esi
			mov	esi,[Fill_CurZBuf]

			shr	esi,8
@@LoopX:
@@PtrMap:		mov	al,[eax+12345678h]
			add	ebx,ebp

			test	al,al
			je	@@NoDisp

@@PtrZBuffer:		cmp	[edi*2+12345678h],si
			jb	@@NoDisp

			mov	ah,dh

@@PtrZBuffer1:		mov	[edi*2+12345678h],si
@@PtrCLUT:		mov	al,[eax+12345678h]

@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:		mov	eax,ebx

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf_AddGour:	add	edx,12345678h

@@IntZBuf:		adc	esi,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@LoopX

			pop	ecx
			mov	eax,edx

			cmp	ecx,PERSP_VALUE
			jle	@@End_X

			shl	eax,8
			mov	ebx,[Fill_Next_MapV]

			shr	edx,32-8
			mov	[Cur_Gouraud],eax

			shl	esi,8
			mov	eax,[Fill_Next_MapU]

			or	edx,esi
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_CurZBuf],edx
			mov	[Fill_Cur_MapV],ebx

			mov	eax,[Fill_Next_MapUOverW]
			mov	ebx,[Fill_Next_MapVOverW]

			mov	[Fill_Cur_MapUOverW],eax
			mov	eax,[Fill_Next_W]

			mov	[Fill_Cur_MapVOverW],ebx
			mov	[Fill_Cur_W],eax

			sub	ecx,PERSP_VALUE
			jg	@@FastFill

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
Filler_TextureZGouraudChromaKeyZBuf ENDP











; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZGouraudNZW PROC
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
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			push	ebx

			mov	eax,[RepMask]
			mov	ebx,[PtrMap]

			mov	dword ptr [@@RepMask+1],eax
			mov	dword ptr [@@RepMask2+1],eax

			mov	dword ptr [@@PtrMap+2],ebx
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	eax,[Fill_Gouraud_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			and	eax,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_AddGour+2],eax

			mov	dword ptr [@@DecZBuf_AddGour2+2],eax
			mov	dword ptr [@@IntZBuf+2],ebx

			mov	dword ptr [@@IntZBuf2+2],ebx
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZGouraud5::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_W_LeftSlope]
			mov	eax,[Fill_CurWMin]

			add	eax,ebx
			mov	ebx,[Fill_Gouraud_LeftSlope]

			mov	[Fill_CurWMin],eax
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
			mov	eax,[Fill_CurGouraudMin]

			mov	[Cur_Gouraud],eax
			mov	eax,[Fill_CurZBufMin]

			mov	[Fill_CurZBuf],eax
			mov	edx,[Fill_CurWMin]

			mov	[Fill_Cur_W],edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_W_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx

			mov	edx,[Fill_CurWMin]

			add	edx,eax
			mov	eax,[Fill_MapU_XSlope]

			mov	[Fill_Cur_W],edx

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
			mov	eax,[Log]

			sub	edi,eax
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,PERSP_SHIFT			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,PERSP_SHIFT			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	eax,ebx

			shr	eax,16
			mov	edx,[RepMask]

			mov	al,bh
			push	ecx

			and	eax,edx
			cmp	ecx,PERSP_VALUE

			jle	@@LessPERSP_VALUE

			mov	ecx,PERSP_VALUE			; Rasterize PERSP_VALUE pix
@@LessPERSP_VALUE:               				; Rasterize less than PERSP_VALUE pix
			mov	edx,[Cur_Gouraud]
			mov	esi,[Fill_CurZBuf]

			shr	edx,8

			shl	esi,32-8

			or	edx,esi
			mov	esi,[Fill_CurZBuf]

			shr	esi,8
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],si
			jb	@@NoDisp
@@Disp:
@@PtrMap:		mov	al,[eax+12345678h]
			add	ebx,ebp

			mov	ah,dh

@@PtrCLUT:		mov	al,[eax+12345678h]

@@PtrLog:		mov	[edi+12345678h],al
			mov	eax,ebx

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf_AddGour:	add	edx,12345678h

@@IntZBuf:		adc	esi,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@LoopX

			jmp	@@EndPERSP_VALUE

@@LoopX2:
@@PtrZBuffer2:		cmp	[edi*2+12345678h],si
			jae	@@Disp
@@NoDisp:
			lea	eax,[ebx+ebp]
			add	ebx,ebp

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf_AddGour2:	add	edx,12345678h

@@IntZBuf2:		adc	esi,12345678h
@@RepMask2:		and	eax,12345678h

			dec	ecx
			jne	@@LoopX2



@@EndPERSP_VALUE:
			pop	ecx
			mov	eax,edx

			cmp	ecx,PERSP_VALUE
			jle	@@End_X

			shl	eax,8
			mov	ebx,[Fill_Next_MapV]

			shr	edx,32-8
			mov	[Cur_Gouraud],eax

			shl	esi,8
			mov	eax,[Fill_Next_MapU]

			or	edx,esi
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_CurZBuf],edx
			mov	[Fill_Cur_MapV],ebx

			mov	eax,[Fill_Next_MapUOverW]
			mov	ebx,[Fill_Next_MapVOverW]

			mov	[Fill_Cur_MapUOverW],eax
			mov	eax,[Fill_Next_W]

			mov	[Fill_Cur_MapVOverW],ebx
			mov	[Fill_Cur_W],eax

			sub	ecx,PERSP_VALUE
			jg	@@FastFill

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
Filler_TextureZGouraudNZW ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZGouraudChromaKeyNZW PROC
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
			mov	eax,[PtrCLUTGouraud]

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			push	ebx
			mov	dword ptr [@@RepMask+1],eax

			mov	ebx,[PtrMap]
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrMap+2],ebx
			mov	dword ptr [@@PtrZBuffer+4],eax

			mov	eax,[Fill_Gouraud_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			and	eax,0FFFFh

			or	eax,ebx
			mov	ebx,[Fill_ZBuf_XSlope]

			shr	ebx,8
			mov	dword ptr [@@DecZBuf_AddGour+2],eax

			mov	dword ptr [@@IntZBuf+2],ebx
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZGouraud6::add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_MapU_LeftSlope]

			add	esi,eax
			mov	eax,[Fill_MapV_LeftSlope]

			add	ebp,eax
			push	ebx

			mov	ebx,[Fill_W_LeftSlope]
			mov	eax,[Fill_CurWMin]

			add	eax,ebx
			mov	ebx,[Fill_Gouraud_LeftSlope]

			mov	[Fill_CurWMin],eax
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
			mov	eax,[Fill_CurGouraudMin]

			mov	[Cur_Gouraud],eax
			mov	eax,[Fill_CurZBufMin]

			mov	[Fill_CurZBuf],eax
			mov	edx,[Fill_CurWMin]

			mov	[Fill_Cur_W],edx
			and	ebx,0FFFFh

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,0FFFFh

			mov	eax,[Fill_W_XSlope]

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16

			shr	eax,16

			or	eax,edx

			mov	edx,[Fill_CurWMin]

			add	edx,eax
			mov	eax,[Fill_MapU_XSlope]

			mov	[Fill_Cur_W],edx

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
			mov	eax,[Log]

			sub	edi,eax
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,PERSP_SHIFT		; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,PERSP_SHIFT		; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	eax,ebx

			shr	eax,16
			mov	edx,[RepMask]

			mov	al,bh
			push	ecx

			and	eax,edx
			cmp	ecx,PERSP_VALUE

			jle	@@LessPERSP_VALUE

			mov	ecx,PERSP_VALUE			; Rasterize PERSP_VALUE pix
@@LessPERSP_VALUE:               				; Rasterize less than PERSP_VALUE pix
			mov	edx,[Cur_Gouraud]
			mov	esi,[Fill_CurZBuf]

			shr	edx,8

			shl	esi,32-8

			or	edx,esi
			mov	esi,[Fill_CurZBuf]

			shr	esi,8
@@LoopX:
@@PtrMap:		mov	al,[eax+12345678h]
			add	ebx,ebp

			test	al,al
			je	@@NoDisp

@@PtrZBuffer:		cmp	[edi*2+12345678h],si
			jb	@@NoDisp

			mov	ah,dh

@@PtrCLUT:		mov	al,[eax+12345678h]

@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:		mov	eax,ebx

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf_AddGour:	add	edx,12345678h

@@IntZBuf:		adc	esi,12345678h
@@RepMask:		and	eax,12345678h

			dec	ecx
			jne	@@LoopX

			pop	ecx
			mov	eax,edx

			cmp	ecx,PERSP_VALUE
			jle	@@End_X

			shl	eax,8
			mov	ebx,[Fill_Next_MapV]

			shr	edx,32-8
			mov	[Cur_Gouraud],eax

			shl	esi,8
			mov	eax,[Fill_Next_MapU]

			or	edx,esi
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_CurZBuf],edx
			mov	[Fill_Cur_MapV],ebx

			mov	eax,[Fill_Next_MapUOverW]
			mov	ebx,[Fill_Next_MapVOverW]

			mov	[Fill_Cur_MapUOverW],eax
			mov	eax,[Fill_Next_W]

			mov	[Fill_Cur_MapVOverW],ebx
			mov	[Fill_Cur_W],eax

			sub	ecx,PERSP_VALUE
			jg	@@FastFill

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
Filler_TextureZGouraudChromaKeyNZW ENDP





;				*******************
;				*** Local proc ***
;				*******************





Last_POLYTEXTUREZGOURAUDPatch	Label	DWORD

_TEXT			ENDS

;			The
			End
