; **************************************************************************
; **************************************************************************
; **************************************************************************
; ***                                                                    ***
; ***                         POLYGON LIBRARY                            ***
; ***                                                                    ***
; **************************************************************************
; ***                                                                    ***
; ***                              (Adeline Software Intl.)              ***
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
PUBLIC	SYSCALL		First_POLYGOURAUDPatch
PUBLIC	SYSCALL		Last_POLYGOURAUDPatch

PUBLIC	SYSCALL		Filler_Gouraud
PUBLIC	SYSCALL		Filler_Dither
PUBLIC	SYSCALL		Filler_GouraudTable
PUBLIC	SYSCALL		Filler_DitherTable
PUBLIC	SYSCALL		Filler_GouraudFog
PUBLIC	SYSCALL		Filler_DitherFog
PUBLIC	SYSCALL		Filler_GouraudZBuf
PUBLIC	SYSCALL		Filler_DitherZBuf
PUBLIC	SYSCALL		Filler_GouraudTableZBuf
PUBLIC	SYSCALL		Filler_DitherTableZBuf
PUBLIC	SYSCALL		Filler_GouraudFogZBuf
PUBLIC	SYSCALL		Filler_DitherFogZBuf
PUBLIC	SYSCALL		Filler_GouraudNZW
PUBLIC	SYSCALL		Filler_DitherNZW
PUBLIC	SYSCALL		Filler_GouraudTableNZW
PUBLIC	SYSCALL		Filler_DitherTableNZW
PUBLIC	SYSCALL		Filler_GouraudFogNZW
PUBLIC	SYSCALL		Filler_DitherFogNZW

;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***


;					*** MODULE DATA ***
PUBLIC	SYSCALL		Fill_VLGouraud1
PUBLIC	SYSCALL		Fill_VLGouraud2
PUBLIC	SYSCALL		Fill_VLGouraud3
PUBLIC	SYSCALL		Fill_VLGouraud4
PUBLIC	SYSCALL		Fill_VLGouraud5
PUBLIC	SYSCALL		Fill_VLGouraud6
PUBLIC	SYSCALL		Fill_VLGouraud7
PUBLIC	SYSCALL		Fill_VLGouraud8
PUBLIC	SYSCALL		Fill_VLGouraud9
PUBLIC	SYSCALL		Fill_VLGouraud10
PUBLIC	SYSCALL		Fill_VLGouraud11
PUBLIC	SYSCALL		Fill_VLGouraud12
PUBLIC	SYSCALL		Fill_VLGouraud13
PUBLIC	SYSCALL		Fill_VLGouraud14
PUBLIC	SYSCALL		Fill_VLGouraud15
PUBLIC	SYSCALL		Fill_VLGouraud16
PUBLIC	SYSCALL		Fill_VLGouraud17
PUBLIC	SYSCALL		Fill_VLGouraud18



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

Extrn	SYSCALL		Fill_LeftSlope		:	DWORD
Extrn	SYSCALL		Fill_RightSlope		:	DWORD

Extrn	SYSCALL		Fill_CurY		:	DWORD
Extrn	SYSCALL		Fill_CurXMin		:	DWORD
Extrn	SYSCALL		Fill_CurXMax		:	DWORD

Extrn	SYSCALL		PTR_TabOffLine		:	DWORD
Extrn	SYSCALL		Fill_CurOffLine		:	DWORD
Extrn	SYSCALL		Log			:	DWORD
Extrn	SYSCALL		ScreenPitch		:	DWORD
Extrn	SYSCALL		PtrZBuffer		:	DWORD

Extrn	SYSCALL		Fill_CurGouraudMin	:	DWORD
Extrn	SYSCALL		Fill_Gouraud_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_Gouraud_XSlope	:	DWORD

Extrn	SYSCALL		Fill_CurZBufMin		:	DWORD
Extrn	SYSCALL		Fill_ZBuf_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_ZBuf_XSlope	:	DWORD

Extrn	SYSCALL		Fill_Logical_Palette	:	BYTE



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

First_POLYGOURAUDPatch	Label	DWORD



; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_Gouraud		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud1::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line

			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	esi,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:		mov	ebp,ebx		; EBP = Gouraud_XSlope(Frac part)

			shl	ebp,16

			sar	ebx,16		; EBX = Gouraud_XSlope(Int part)
			mov	eax,esi

			shr	eax,16

			shl	esi,16

				; *** Gouraud loop ***
@@Each_X:
			mov	[edi],al
			add	esi,ebp

			adc	al,bl
			inc	edi

			dec	ecx
			jne	@@Each_X
@@End_X:

			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_Gouraud		ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_Dither		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud2::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi

			sar	edx,8
			mov	ebp,[Fill_Gouraud_XSlope]

			sar	ebp,8
			mov	eax,edx

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	[edi],ah
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	[edi+1],ah
			add	edi,2

			rol	al,cl

			dec	ecx
			jne	@@Each_X

@@Add_One:
			and	eax,0FFh
			add	eax,edx
			mov	[edi],ah
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	[edi],ah
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	[edi+1],ah
			add	edi,2

			rol	al,cl

			dec	ecx
			jne	@@Each_X2
@@End_X:
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_Dither		ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudTable	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud3::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			lea	ebp,[edi+ecx]

			or	eax,edx
			and	edi,not 31

			add	esi,eax
			and	ebp,not 31

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,ebp
			jbe	@@Preload

			pop	edi
@@FastInit:
			mov	edx,[Fill_Color]	; PtrCLUTGouraud + Color index
			mov	eax,esi

			shr	eax,8
			mov	ebp,0FF00h

			and	eax,ebp

				; *** Gouraud loop ***
@@Each_X:
			mov	al,[eax+edx]
			add	esi,ebx

			mov	[edi],al
			mov	eax,esi

			shr	eax,8
			inc	edi

			and	eax,ebp

			dec	ecx
			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudTable	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherTable	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud4::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	esi
			push	ebx

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi

			sar	edx,8
			mov	ebp,[Fill_Gouraud_XSlope]

			sar	ebp,8
			mov	eax,edx

			mov	esi,[Fill_Color]
			and	eax,0FFh

			add	eax,edx

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			mov	ebx,eax
			add	edx,ebp

			and	eax,0FFh
			and	ebx,0FF00h

			add	eax,edx
			add	edx,ebp

			mov	bl,[esi+ebx]

			mov	[edi],bl
			mov	ebx,eax

			and	ebx,0FF00h
			and	eax,0FFh

			rol	al,cl

			mov	bl,[ebx+esi]
			add	eax,edx

			mov	[edi+1],bl
			add	edi,2

			dec	ecx
			jne	@@Each_X

@@Add_One:
			and	eax,0FF00h
			mov	al,[eax+esi]
			mov	[edi],al
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			mov	ebx,eax
			add	edx,ebp

			and	eax,0FFh
			and	ebx,0FF00h

			add	eax,edx
			add	edx,ebp

			mov	bl,[esi+ebx]

			mov	[edi],bl
			mov	ebx,eax

			and	ebx,0FF00h
			and	eax,0FFh

			rol	al,cl

			mov	bl,[ebx+esi]
			add	eax,edx

			mov	[edi+1],bl
			add	edi,2

			dec	ecx
			jne	@@Each_X2

@@End_X:
			pop	ebx
			pop	esi
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherTable	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudFog	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud5::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	esi,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:		mov	ebp,ebx		; EBP = Gouraud_XSlope(Frac part)

			shl	ebp,16
			add	edi,ecx

			sar	ebx,16		; EBX = Gouraud_XSlope(Int part)
			mov	eax,esi

			shr	eax,16
			xor	ecx,-1

			shl	esi,16
			and	eax,0FFh

			inc	ecx

				; *** Gouraud loop ***
@@Each_X:
			add	esi,ebp
			mov	dl,[eax+Fill_Logical_Palette]

			adc	al,bl
			mov	[edi+ecx],dl

			inc	ecx
			jne	@@Each_X
@@End_X:

			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudFog	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherFog	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud6::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	ebx
			push	edx
			push	ebp

			mov	dl,[edi]	; Load 1st dst cache line
			xor	ebx,ebx

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi

			sar	edx,8
			mov	ebp,[Fill_Gouraud_XSlope]

			sar	ebp,8
			mov	eax,edx

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	bl,ah
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	bl,[Fill_Logical_Palette+ebx]

			mov	[edi],bl
			mov	bl,ah

			rol	al,cl

			mov	bl,[Fill_Logical_Palette+ebx]

			mov	[edi+1],bl
			add	edi,2

			dec	ecx
			jne	@@Each_X

@@Add_One:
			and	eax,0FFh
			add	eax,edx
			mov	bl,ah
			mov	bl,[Fill_Logical_Palette+ebx]
			mov	[edi],bl
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	bl,ah
			and	eax,0FFh

			add	eax,edx
			add	edx,ebp

			mov	bl,[Fill_Logical_Palette+ebx]

			mov	[edi],bl
			mov	bl,ah

			rol	al,cl

			mov	bl,[Fill_Logical_Palette+ebx]

			mov	[edi+1],bl
			add	edi,2

			dec	ecx
			jne	@@Each_X2
@@End_X:
			pop	ebp
			pop	edx
			pop	ebx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherFog	ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[Fill_Gouraud_XSlope]

			shl	eax,16
			push	ebx

			mov	dword ptr [@@DecGour+2],eax
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	dword ptr [@@DecZBuf+2],ebx

			mov	dword ptr [@@IntZBuf+2],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	ebx,[Log]

			mov	dword ptr [@@PtrLog+2],ebx
			pop	ebx

			mov	dword ptr [@@PtrZBuffer1+4],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud7::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	esi,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:		sar	ebx,16		; EBX = Gouraud_XSlope(Int part)
			mov	ebp,[Fill_CurZBufMin]

			mov	eax,[Log]
			mov	edx,ebp

			shl	ebp,32-8
			sub	edi,eax

			shr	edx,8
			mov	eax,esi

			shr	eax,16

			shl	esi,16

				; *** Gouraud loop ***
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
@@PtrZBuffer1:		mov	[edi*2+12345678h],dx
@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:
@@DecGour:		add	esi,12345678h

			adc	al,bl
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	edx,12345678h
			inc	edi

			dec	ecx
			jne	@@Each_X
@@End_X:

			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdgeOk d
Filler_GouraudZBuf	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[Fill_Gouraud_XSlope]

			sar	eax,8
			push	ebx

			mov	dword ptr [@@AddGour+2],eax
			mov	dword ptr [@@AddGour1+2],eax

			mov	dword ptr [@@AddGour2+2],eax
			mov	dword ptr [@@AddGour3+2],eax

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	dword ptr [@@PtrZBuffer3+4],eax

			mov	dword ptr [@@PtrZBuffer4+4],eax
			mov	dword ptr [@@PtrZBuffer5+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	dword ptr [@@PtrLog2+2],ebx

			mov	dword ptr [@@PtrLog4+2],ebx
			add	eax,2

			inc	ebx
			mov	dword ptr [@@PtrZBuffer_1+4],eax

			mov	dword ptr [@@PtrZBuffer1_1+4],eax
			mov	dword ptr [@@PtrZBuffer2_1+4],eax

			mov	dword ptr [@@PtrZBuffer3_1+4],eax
			mov	dword ptr [@@PtrLog_1+2],ebx

			mov	dword ptr [@@PtrLog2_1+2],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	dword ptr [@@IntZBuf+2],eax

			mov	dword ptr [@@IntZBuf1+2],eax
			mov	dword ptr [@@IntZBuf2+2],eax

			mov	dword ptr [@@IntZBuf3+2],eax
			mov	dword ptr [@@DecZBuf+2],ebx

			mov	dword ptr [@@DecZBuf1+2],ebx
			mov	dword ptr [@@DecZBuf2+2],ebx

			mov	dword ptr [@@DecZBuf3+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud8::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	ebx

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi
			mov	ebx,[Fill_CurZBufMin]

			shr	ebx,8
			mov	ebp,[Fill_CurZBufMin]

			shl	ebp,32-8
			mov	eax,[Log]

			sub	edi,eax

			sar	edx,8
			mov	eax,edx

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			and	eax,0FFh

			add	eax,edx
@@AddGour:		add	edx,12345678h

@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp0
@@PtrZBuffer1:		mov	[edi*2+12345678h],bx
@@PtrLog:		mov	[edi+12345678h],ah
@@NoDisp0:
			and	eax,0FFh
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			add	eax,edx

@@PtrZBuffer_1:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp1
@@PtrZBuffer1_1:	mov	[edi*2+12345678h],bx
@@PtrLog_1:		mov	[edi+12345678h],ah
@@NoDisp1:
@@AddGour1:		add	edx,12345678h
@@DecZBuf1:		add	ebp,12345678h

@@IntZBuf1:		adc	ebx,12345678h
			add	edi,2

			rol	al,cl

			dec	ecx
			jne	@@Each_X

@@Add_One:
@@PtrZBuffer4:		cmp	[edi*2+12345678h],bx
			jb	@@End_X
@@PtrZBuffer5:		mov	[edi*2+12345678h],bx
			and	eax,0FFh
			add	eax,edx
@@PtrLog4:		mov	[edi+12345678h],ah
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			and	eax,0FFh

			add	eax,edx
@@AddGour2:		add	edx,12345678h

@@PtrZBuffer2:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp2
@@PtrZBuffer3:		mov	[edi*2+12345678h],bx
@@PtrLog2:		mov	[edi+12345678h],ah
@@NoDisp2:
			and	eax,0FFh
@@DecZBuf2:		add	ebp,12345678h

@@IntZBuf2:		adc	ebx,12345678h
			add	eax,edx

@@PtrZBuffer2_1:	cmp	[edi*2+12345678h],bx
			jb	@@NoDisp3
@@PtrZBuffer3_1:	mov	[edi*2+12345678h],bx
@@PtrLog2_1:		mov	[edi+12345678h],ah
@@NoDisp3:
@@AddGour3:		add	edx,12345678h
@@DecZBuf3:		add	ebp,12345678h

@@IntZBuf3:		adc	ebx,12345678h
			add	edi,2

			rol	al,cl

			dec	ecx
			jne	@@Each_X2
@@End_X:
			pop	ebx
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherZBuf	ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudTableZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx
			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[Fill_Gouraud_XSlope]

			mov	dword ptr [@@AddGour+2],eax
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	dword ptr [@@IntZBuf+2],eax

			mov	dword ptr [@@DecZBuf+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud9::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			lea	ebp,[edi+ecx]

			or	eax,edx
			and	edi,not 31

			add	esi,eax
			and	ebp,not 31

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,ebp
			jbe	@@Preload

			pop	edi
@@FastInit:
			mov	edx,[Fill_Color]	; PtrCLUTGouraud + Color index
			mov	ebx,[Fill_CurZBufMin]

			mov	eax,[Log]
			mov	ebp,ebx

			shr	ebx,8
			sub	edi,eax

			shl	ebp,32-8
			mov	eax,esi

			shr	eax,8

			and	eax,0FF00h

				; *** Gouraud loop ***
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp
@@PtrZBuffer1:		mov	[edi*2+12345678h],bx
			mov	al,[eax+edx]

@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:
@@AddGour:		add	esi,12345678h
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			mov	eax,esi

			shr	eax,8
			inc	edi

			and	eax,0FF00h

			dec	ecx
			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudTableZBuf	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherTableZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	dword ptr [@@PtrZBuffer3+4],eax

			mov	dword ptr [@@PtrZBuffer4+4],eax
			mov	dword ptr [@@PtrZBuffer5+4],eax

			add	eax,2
			mov	dword ptr [@@PtrLog+2],ebx

			mov	dword ptr [@@PtrZBuffer_1+4],eax
			mov	dword ptr [@@PtrZBuffer1_1+4],eax

			mov	dword ptr [@@PtrZBuffer2_1+4],eax
			mov	dword ptr [@@PtrLog2+2],ebx

			mov	dword ptr [@@PtrLog4+2],ebx
			inc	ebx

			mov	dword ptr [@@PtrLog_1+2],ebx
			mov	dword ptr [@@PtrZBuffer3_1+4],eax

			mov	dword ptr [@@PtrLog2_1+2],ebx
			mov	eax,[Fill_Gouraud_XSlope]

			sar	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			mov	dword ptr [@@AddGour+2],eax
			mov	dword ptr [@@AddGour1+2],eax

			mov	dword ptr [@@AddGour2+2],eax
			mov	dword ptr [@@AddGour3+2],eax

			shr	ebx,8
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@IntZBuf+2],ebx

			mov	dword ptr [@@IntZBuf1+2],ebx
			mov	dword ptr [@@IntZBuf2+2],ebx

			mov	dword ptr [@@IntZBuf3+2],ebx
			mov	dword ptr [@@DecZBuf+2],eax

			mov	dword ptr [@@DecZBuf1+2],eax
			mov	dword ptr [@@DecZBuf2+2],eax

			mov	dword ptr [@@DecZBuf3+2],eax
			mov	eax,[Fill_Color]

			mov	dword ptr [@@PtrColor+2],eax
			mov	dword ptr [@@PtrColor1+2],eax

			mov	dword ptr [@@PtrColor2+2],eax
			mov	dword ptr [@@PtrColor3+2],eax

			mov	dword ptr [@@PtrColor4+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud10::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	esi
			push	ebx

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi

			sar	edx,8
			mov	eax,[Log]

			sub	edi,eax
			mov	eax,edx

			and	eax,0FFh
			mov	ebx,[Fill_CurZBufMin]

			shr	ebx,8
			add	eax,edx

			mov	ebp,[Fill_CurZBufMin]
			mov	esi,eax

			shl	ebp,32-8
			and	esi,0FF00h

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl

@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp
@@PtrZBuffer1:		mov	[edi*2+12345678h],bx
@@PtrColor:		mov	ah,[esi+12345678h]

@@PtrLog:		mov	[edi+12345678h],ah
@@NoDisp:
			and	eax,0FFh
@@AddGour:		add	edx,12345678h

			add	eax,edx
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			mov	esi,eax

			and	esi,0FF00h

@@PtrZBuffer_1:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp1
@@PtrZBuffer1_1:	mov	[edi*2+12345678h],bx
@@PtrColor1:		mov	ah,[esi+12345678h]

@@PtrLog_1:		mov	[edi+12345678h],ah
@@NoDisp1:
@@AddGour1:		add	edx,12345678h
			and	eax,0FFh

			add	eax,edx
@@DecZBuf1:		add	ebp,12345678h

@@IntZBuf1:		adc	ebx,12345678h
			mov	esi,eax

			add	edi,2
			and	esi,0FF00h

			rol	al,cl

			dec	ecx
			jne	@@Each_X


@@Add_One:
@@PtrZBuffer4:		cmp	[edi*2+12345678h],bx
			jb	@@End_X
@@PtrZBuffer5:		mov	[edi*2+12345678h],bx
@@PtrColor4:		mov	ah,[esi+12345678h]

@@PtrLog4:		mov	[edi+12345678h],ah
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
@@PtrZBuffer2:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp2
@@PtrZBuffer3:		mov	[edi*2+12345678h],bx
@@PtrColor2:		mov	ah,[esi+12345678h]

@@PtrLog2:		mov	[edi+12345678h],ah
@@NoDisp2:
			and	eax,0FFh
@@AddGour2:		add	edx,12345678h

			add	eax,edx
@@DecZBuf2:		add	ebp,12345678h

@@IntZBuf2:		adc	ebx,12345678h
			mov	esi,eax

			and	esi,0FF00h

@@PtrZBuffer2_1:	cmp	[edi*2+12345678h],bx
			jb	@@NoDisp3
@@PtrZBuffer3_1:	mov	[edi*2+12345678h],bx
@@PtrColor3:		mov	ah,[esi+12345678h]

@@PtrLog2_1:		mov	[edi+12345678h],ah
@@NoDisp3:
@@AddGour3:		add	edx,12345678h
			and	eax,0FFh

			add	eax,edx
@@DecZBuf3:		add	ebp,12345678h

@@IntZBuf3:		adc	ebx,12345678h
			mov	esi,eax

			add	edi,2
			and	esi,0FF00h

			rol	al,cl

			dec	ecx
			jne	@@Each_X2

@@End_X:
			pop	ebx
			pop	esi
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherTableZBuf	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudFogZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[Fill_Gouraud_XSlope]

			shr	eax,16
			mov	ebx,[Fill_Gouraud_XSlope]

			shl	ebx,16
			mov	byte ptr [@@IntGour+1],al

			mov	dword ptr [@@DecGour+2],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	dword ptr [@@IntZBuf+2],eax

			mov	dword ptr [@@DecZBuf+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud11::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	esi,eax
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
			mov	ebp,[Fill_CurZBufMin]

			shl	ebp,32-8
			sub	edi,eax

			mov	ebx,[Fill_CurZBufMin]
			mov	eax,esi

			shr	ebx,8

			shr	eax,16

			shl	esi,16
			and	eax,0FFh

				; *** Gouraud loop ***
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp
@@PtrZBuffer1:		mov	[edi*2+12345678h],bx
			mov	dl,[eax+Fill_Logical_Palette]
@@PtrLog:		mov	[edi+12345678h],dl
@@NoDisp:
			inc	edi
@@DecGour:		add	esi,12345678h

@@IntGour:		adc	al,12h
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			dec	ecx

			jne	@@Each_X
@@End_X:

			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudFogZBuf	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherFogZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[Fill_Gouraud_XSlope]
			mov	ebx,[Fill_ZBuf_XSlope]

			sar	eax,8

			shl	ebx,32-8
			and	eax,0FFFFh

			or	eax,ebx

			mov	dword ptr [@@DecZBuf_AddGour+2],eax
			mov	dword ptr [@@DecZBuf_AddGour1+2],eax

			mov	dword ptr [@@DecZBuf_AddGour2+2],eax
			mov	dword ptr [@@DecZBuf_AddGour3+2],eax

			mov	ebx,[Log]
			mov	eax,[Fill_ZBuf_XSlope]

			mov	dword ptr [@@PtrLog+2],ebx
			mov	dword ptr [@@PtrLog2+2],ebx

			mov	dword ptr [@@PtrLog4+2],ebx
			inc	ebx

			mov	dword ptr [@@PtrLog_1+2],ebx
			mov	dword ptr [@@PtrLog2_1+2],ebx

			shr	eax,8

			mov	dword ptr [@@IntZBuf+2],eax
			mov	dword ptr [@@IntZBuf1+2],eax

			mov	dword ptr [@@IntZBuf2+2],eax
			mov	dword ptr [@@IntZBuf3+2],eax

			pop	ebx
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud12::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	ebx
			push	esi

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:
			shr	esi,8
			mov	ebx,[Fill_CurZBufMin]

			and	esi,0FFFFh
			mov	ebp,ebx

			shr	ebx,8
			mov	eax,[Log]

			shl	ebp,32-8
			sub	edi,eax

			mov	eax,esi
			or	esi,ebp

			mov	ebp,[PtrZBuffer]
			and	eax,0FFh

			add	eax,esi
			xor	edx,edx

			mov	dl,ah

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			cmp	word ptr [edi*2+ebp],bx
			jb	@@NoDisp0
			mov	word ptr [edi*2+ebp],bx
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog:		mov	[edi+12345678h],dl
@@NoDisp0:
			and	eax,0FFh
@@DecZBuf_AddGour:	add	esi,12345678h

@@IntZBuf:		adc	ebx,12345678h
			add	eax,esi

			mov	dl,ah

			cmp	word ptr [edi*2+ebp+2],bx
			jb	@@NoDisp1
			mov	word ptr [edi*2+ebp+2],bx
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog_1:		mov	[edi+12345678h],dl
@@NoDisp1:
			and	eax,0FFh
@@DecZBuf_AddGour1:	add	esi,12345678h

@@IntZBuf1:		adc	ebx,12345678h
			add	eax,esi

			add	edi,2
			mov	dl,ah

			rol	al,cl

			dec	ecx
			jne	@@Each_X
@@Add_One:
			cmp	word ptr [edi*2+ebp],bx
			jb	@@End_X
			mov	word ptr [edi*2+ebp],bx
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog4:		mov	[edi+12345678h],dl
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			cmp	word ptr [edi*2+ebp],bx
			jb	@@NoDisp2
			mov	word ptr [edi*2+ebp],bx
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog2:		mov	[edi+12345678h],dl
@@NoDisp2:
			and	eax,0FFh
@@DecZBuf_AddGour2:	add	esi,12345678h

@@IntZBuf2:		adc	ebx,12345678h
			add	eax,esi

			mov	dl,ah

			cmp	word ptr [edi*2+ebp+2],bx
			jb	@@NoDisp3
			mov	word ptr [edi*2+ebp+2],bx
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog2_1:		mov	[edi+12345678h],dl
@@NoDisp3:
			and	eax,0FFh
@@DecZBuf_AddGour3:	add	esi,12345678h

@@IntZBuf3:		adc	ebx,12345678h
			add	eax,esi

			add	edi,2
			mov	dl,ah

			rol	al,cl

			dec	ecx
			jne	@@Each_X2
@@End_X:
			pop	esi
			pop	ebx
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherFogZBuf	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[Fill_Gouraud_XSlope]

			shl	eax,16
			push	ebx

			mov	dword ptr [@@DecGour+2],eax
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	dword ptr [@@DecZBuf+2],ebx

			mov	dword ptr [@@IntZBuf+2],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	ebx,[Log]

			mov	dword ptr [@@PtrLog+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud13::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	esi,eax
			and	edx,not 31	; AGI on EDI on first loop

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@FastInit:		sar	ebx,16		; EBX = Gouraud_XSlope(Int part)
			mov	ebp,[Fill_CurZBufMin]

			mov	eax,[Log]
			mov	edx,ebp

			shl	ebp,32-8
			sub	edi,eax

			shr	edx,8
			mov	eax,esi

			shr	eax,16

			shl	esi,16

				; *** Gouraud loop ***
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:
@@DecGour:		add	esi,12345678h

			adc	al,bl
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	edx,12345678h
			inc	edi

			dec	ecx
			jne	@@Each_X
@@End_X:

			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudNZW	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[Fill_Gouraud_XSlope]

			sar	eax,8
			push	ebx

			mov	dword ptr [@@AddGour+2],eax
			mov	dword ptr [@@AddGour1+2],eax

			mov	dword ptr [@@AddGour2+2],eax
			mov	dword ptr [@@AddGour3+2],eax

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	dword ptr [@@PtrZBuffer3+4],eax

			mov	dword ptr [@@PtrZBuffer4+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	dword ptr [@@PtrLog2+2],ebx

			mov	dword ptr [@@PtrLog4+2],ebx
			add	eax,2

			inc	ebx
			mov	dword ptr [@@PtrLog_1+2],ebx

			mov	dword ptr [@@PtrLog2_1+2],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	dword ptr [@@IntZBuf+2],eax

			mov	dword ptr [@@IntZBuf1+2],eax
			mov	dword ptr [@@IntZBuf2+2],eax

			mov	dword ptr [@@IntZBuf3+2],eax
			mov	dword ptr [@@DecZBuf+2],ebx

			mov	dword ptr [@@DecZBuf1+2],ebx
			mov	dword ptr [@@DecZBuf2+2],ebx

			mov	dword ptr [@@DecZBuf3+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud14::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	ebx

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi
			mov	ebx,[Fill_CurZBufMin]

			shr	ebx,8
			mov	ebp,[Fill_CurZBufMin]

			shl	ebp,32-8
			mov	eax,[Log]

			sub	edi,eax

			sar	edx,8
			mov	eax,edx

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			and	eax,0FFh

			add	eax,edx
@@AddGour:		add	edx,12345678h

@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp0
@@PtrLog:		mov	[edi+12345678h],ah
@@NoDisp0:
			and	eax,0FFh
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			add	eax,edx

@@PtrZBuffer1:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp1
@@PtrLog_1:		mov	[edi+12345678h],ah
@@NoDisp1:
@@AddGour1:		add	edx,12345678h
@@DecZBuf1:		add	ebp,12345678h

@@IntZBuf1:		adc	ebx,12345678h
			add	edi,2

			rol	al,cl

			dec	ecx
			jne	@@Each_X

@@Add_One:
@@PtrZBuffer4:		cmp	[edi*2+12345678h],bx
			jb	@@End_X
			and	eax,0FFh
			add	eax,edx
@@PtrLog4:		mov	[edi+12345678h],ah
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			and	eax,0FFh

			add	eax,edx
@@AddGour2:		add	edx,12345678h

@@PtrZBuffer2:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp2
@@PtrLog2:		mov	[edi+12345678h],ah
@@NoDisp2:
			and	eax,0FFh
@@DecZBuf2:		add	ebp,12345678h

@@IntZBuf2:		adc	ebx,12345678h
			add	eax,edx

@@PtrZBuffer3:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp3
@@PtrLog2_1:		mov	[edi+12345678h],ah
@@NoDisp3:
@@AddGour3:		add	edx,12345678h
@@DecZBuf3:		add	ebp,12345678h

@@IntZBuf3:		adc	ebx,12345678h
			add	edi,2

			rol	al,cl

			dec	ecx
			jne	@@Each_X2
@@End_X:
			pop	ebx
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherNZW	ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudTableNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrLog+2],ebx

			mov	eax,[Fill_Gouraud_XSlope]

			mov	dword ptr [@@AddGour+2],eax
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	dword ptr [@@IntZBuf+2],eax

			mov	dword ptr [@@DecZBuf+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud15::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			lea	ebp,[edi+ecx]

			or	eax,edx
			and	edi,not 31

			add	esi,eax
			and	ebp,not 31

				; *** Load dst in WB cache ***

@@Preload:
			mov	al,[edi]
			add	edi,32

			cmp	edi,ebp
			jbe	@@Preload

			pop	edi
@@FastInit:
			mov	edx,[Fill_Color]	; PtrCLUTGouraud + Color index
			mov	ebx,[Fill_CurZBufMin]

			mov	eax,[Log]
			mov	ebp,ebx

			shr	ebx,8
			sub	edi,eax

			shl	ebp,32-8
			mov	eax,esi

			shr	eax,8

			and	eax,0FF00h

				; *** Gouraud loop ***
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp
			mov	al,[eax+edx]

@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:
@@AddGour:		add	esi,12345678h
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			mov	eax,esi

			shr	eax,8
			inc	edi

			and	eax,0FF00h

			dec	ecx
			jne	@@Each_X

@@End_X:
			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudTableNZW	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherTableNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	dword ptr [@@PtrZBuffer3+4],eax

			mov	dword ptr [@@PtrZBuffer4+4],eax

			add	eax,2
			mov	dword ptr [@@PtrLog+2],ebx

			mov	dword ptr [@@PtrLog2+2],ebx
			mov	dword ptr [@@PtrLog4+2],ebx

			inc	ebx
			mov	eax,[Fill_Gouraud_XSlope]

			mov	dword ptr [@@PtrLog_1+2],ebx
			mov	dword ptr [@@PtrLog2_1+2],ebx

			sar	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			mov	dword ptr [@@AddGour+2],eax
			mov	dword ptr [@@AddGour1+2],eax

			mov	dword ptr [@@AddGour2+2],eax
			mov	dword ptr [@@AddGour3+2],eax

			shr	ebx,8
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8
			mov	dword ptr [@@IntZBuf+2],ebx

			mov	dword ptr [@@IntZBuf1+2],ebx
			mov	dword ptr [@@IntZBuf2+2],ebx

			mov	dword ptr [@@IntZBuf3+2],ebx
			mov	dword ptr [@@DecZBuf+2],eax

			mov	dword ptr [@@DecZBuf1+2],eax
			mov	dword ptr [@@DecZBuf2+2],eax

			mov	dword ptr [@@DecZBuf3+2],eax
			mov	eax,[Fill_Color]

			mov	dword ptr [@@PtrColor+2],eax
			mov	dword ptr [@@PtrColor1+2],eax

			mov	dword ptr [@@PtrColor2+2],eax
			mov	dword ptr [@@PtrColor3+2],eax

			mov	dword ptr [@@PtrColor4+2],eax
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud16::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	esi
			push	ebx

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:		mov	edx,esi

			sar	edx,8
			mov	eax,[Log]

			sub	edi,eax
			mov	eax,edx

			and	eax,0FFh
			mov	ebx,[Fill_CurZBufMin]

			shr	ebx,8
			add	eax,edx

			mov	ebp,[Fill_CurZBufMin]
			mov	esi,eax

			shl	ebp,32-8
			and	esi,0FF00h

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl

@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp
@@PtrColor:		mov	ah,[esi+12345678h]

@@PtrLog:		mov	[edi+12345678h],ah
@@NoDisp:
			and	eax,0FFh
@@AddGour:		add	edx,12345678h

			add	eax,edx
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			mov	esi,eax

			and	esi,0FF00h

@@PtrZBuffer1:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp1
@@PtrColor1:		mov	ah,[esi+12345678h]

@@PtrLog_1:		mov	[edi+12345678h],ah
@@NoDisp1:
@@AddGour1:		add	edx,12345678h
			and	eax,0FFh

			add	eax,edx
@@DecZBuf1:		add	ebp,12345678h

@@IntZBuf1:		adc	ebx,12345678h
			mov	esi,eax

			add	edi,2
			and	esi,0FF00h

			rol	al,cl

			dec	ecx
			jne	@@Each_X


@@Add_One:
@@PtrZBuffer4:		cmp	[edi*2+12345678h],bx
			jb	@@End_X
@@PtrColor4:		mov	ah,[esi+12345678h]

@@PtrLog4:		mov	[edi+12345678h],ah
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
@@PtrZBuffer2:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp2
@@PtrColor2:		mov	ah,[esi+12345678h]

@@PtrLog2:		mov	[edi+12345678h],ah
@@NoDisp2:
			and	eax,0FFh
@@AddGour2:		add	edx,12345678h

			add	eax,edx
@@DecZBuf2:		add	ebp,12345678h

@@IntZBuf2:		adc	ebx,12345678h
			mov	esi,eax

			and	esi,0FF00h

@@PtrZBuffer3:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp3
@@PtrColor3:		mov	ah,[esi+12345678h]

@@PtrLog2_1:		mov	[edi+12345678h],ah
@@NoDisp3:
@@AddGour3:		add	edx,12345678h
			and	eax,0FFh

			add	eax,edx
@@DecZBuf3:		add	ebp,12345678h

@@IntZBuf3:		adc	ebx,12345678h
			mov	esi,eax

			add	edi,2
			and	esi,0FF00h

			rol	al,cl

			dec	ecx
			jne	@@Each_X2

@@End_X:
			pop	ebx
			pop	esi
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherTableNZW	ENDP







; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_GouraudFogNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[PtrZBuffer]
			mov	ebx,[Log]

			mov	dword ptr [@@PtrZBuffer+4],eax

			mov	dword ptr [@@PtrLog+2],ebx
			mov	eax,[Fill_Gouraud_XSlope]

			shr	eax,16
			mov	ebx,[Fill_Gouraud_XSlope]

			shl	ebx,16
			mov	byte ptr [@@IntGour+1],al

			mov	dword ptr [@@DecGour+2],ebx
			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8
			mov	ebx,[Fill_ZBuf_XSlope]

			shl	ebx,32-8
			mov	dword ptr [@@IntZBuf+2],eax

			mov	dword ptr [@@DecZBuf+2],ebx
			pop	ebx

			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud17::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine

			push	ebp
			push	esi

			push	ebx
			push	edx

			mov	eax,ebx
			mov	ebx,[Fill_Gouraud_XSlope]

			and	eax,0FFFFh
			mov	dl,[edi]	; Load 1st dst cache line
			xor	eax,0FFFFh

			cmp	ecx,16
			jbe	@@FastInit

			imul	ebx

			shl	edx,16
			push	edi

			shr	eax,16
			and	edi,not 31

			or	eax,edx
			lea	edx,[edi+ecx]

			add	esi,eax
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
			mov	ebp,[Fill_CurZBufMin]

			shl	ebp,32-8
			sub	edi,eax

			mov	ebx,[Fill_CurZBufMin]
			mov	eax,esi

			shr	ebx,8

			shr	eax,16

			shl	esi,16
			and	eax,0FFh

				; *** Gouraud loop ***
@@Each_X:
@@PtrZBuffer:		cmp	[edi*2+12345678h],bx
			jb	@@NoDisp
			mov	dl,[eax+Fill_Logical_Palette]
@@PtrLog:		mov	[edi+12345678h],dl
@@NoDisp:
			inc	edi
@@DecGour:		add	esi,12345678h

@@IntGour:		adc	al,12h
@@DecZBuf:		add	ebp,12345678h

@@IntZBuf:		adc	ebx,12345678h
			dec	ecx

			jne	@@Each_X
@@End_X:

			pop	edx
			pop	ebx

			pop	esi
			pop	ebp

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_GouraudFogNZW	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_DitherFogNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			add	eax,ebp
			mov	edi,[Fill_CurOffLine]

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurGouraudMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			push	ebx

			mov	eax,[Fill_Gouraud_XSlope]
			mov	ebx,[Fill_ZBuf_XSlope]

			sar	eax,8

			shl	ebx,32-8
			and	eax,0FFFFh

			or	eax,ebx

			mov	dword ptr [@@DecZBuf_AddGour+2],eax
			mov	dword ptr [@@DecZBuf_AddGour1+2],eax

			mov	dword ptr [@@DecZBuf_AddGour2+2],eax
			mov	dword ptr [@@DecZBuf_AddGour3+2],eax

			mov	ebx,[Log]
			mov	eax,[Fill_ZBuf_XSlope]

			mov	dword ptr [@@PtrLog+2],ebx
			mov	dword ptr [@@PtrLog2+2],ebx

			mov	dword ptr [@@PtrLog4+2],ebx
			inc	ebx

			mov	dword ptr [@@PtrLog_1+2],ebx
			mov	dword ptr [@@PtrLog2_1+2],ebx

			shr	eax,8

			mov	dword ptr [@@IntZBuf+2],eax
			mov	dword ptr [@@IntZBuf1+2],eax

			mov	dword ptr [@@IntZBuf2+2],eax
			mov	dword ptr [@@IntZBuf3+2],eax

			pop	ebx
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLGouraud18::	add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_Gouraud_LeftSlope]

			add	esi,eax
			push	ebx

			mov	eax,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_LeftSlope]

			add	eax,ebx
			pop	ebx

			mov	[Fill_CurZBufMin],eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


				; *** Gouraud dither loop ***
			push	edx
			push	ebp
			push	ebx
			push	esi

			mov	dl,[edi]	; Load 1st dst cache line

			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache
			jb	@@NoPreload

				; *** Load dst in WB cache ***
			push	edi
			lea	edx,[edi+ecx]

			and	edi,not 31
			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edi
@@NoPreload:
			shr	esi,8
			mov	ebx,[Fill_CurZBufMin]

			and	esi,0FFFFh
			mov	ebp,ebx

			shr	ebx,8
			mov	eax,[Log]

			shl	ebp,32-8
			sub	edi,eax

			mov	eax,esi
			or	esi,ebp

			mov	ebp,[PtrZBuffer]
			and	eax,0FFh

			add	eax,esi
			xor	edx,edx

			mov	dl,ah

			shr	ecx,1
			jnc	@@Pas_Add1
			je	@@Add_One

			rol	al,cl
@@Each_X:
			cmp	word ptr [edi*2+ebp],bx
			jb	@@NoDisp0
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog:		mov	[edi+12345678h],dl
@@NoDisp0:
			and	eax,0FFh
@@DecZBuf_AddGour:	add	esi,12345678h

@@IntZBuf:		adc	ebx,12345678h
			add	eax,esi

			mov	dl,ah

			cmp	word ptr [edi*2+ebp+2],bx
			jb	@@NoDisp1
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog_1:		mov	[edi+12345678h],dl
@@NoDisp1:
			and	eax,0FFh
@@DecZBuf_AddGour1:	add	esi,12345678h

@@IntZBuf1:		adc	ebx,12345678h
			add	eax,esi

			add	edi,2
			mov	dl,ah

			rol	al,cl

			dec	ecx
			jne	@@Each_X
@@Add_One:
			cmp	word ptr [edi*2+ebp],bx
			jb	@@End_X
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog4:		mov	[edi+12345678h],dl
			jmp	@@End_X
@@Pas_Add1:
			je	@@End_X
@@Each_X2:
			cmp	word ptr [edi*2+ebp],bx
			jb	@@NoDisp2
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog2:		mov	[edi+12345678h],dl
@@NoDisp2:
			and	eax,0FFh
@@DecZBuf_AddGour2:	add	esi,12345678h

@@IntZBuf2:		adc	ebx,12345678h
			add	eax,esi

			mov	dl,ah

			cmp	word ptr [edi*2+ebp+2],bx
			jb	@@NoDisp3
			mov	dl,[edx+Fill_Logical_Palette]
@@PtrLog2_1:		mov	[edi+12345678h],dl
@@NoDisp3:
			and	eax,0FFh
@@DecZBuf_AddGour3:	add	esi,12345678h

@@IntZBuf3:		adc	ebx,12345678h
			add	eax,esi

			add	edi,2
			mov	dl,ah

			rol	al,cl

			dec	ecx
			jne	@@Each_X2
@@End_X:
			pop	esi
			pop	ebx
			pop	ebp
			pop	edx

@@EndScanLine:		pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurGouraudMin],esi

			jmp	Triangle_ReadNextEdge
Filler_DitherFogNZW	ENDP















Last_POLYGOURAUDPatch	Label	DWORD




_TEXT			ENDS

;			The
			End
