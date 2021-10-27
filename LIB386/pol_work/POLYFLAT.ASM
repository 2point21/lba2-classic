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
;PUBLIC	SYSCALL		Fill_PolyFlat

PUBLIC	SYSCALL		First_POLYFLATPatch
PUBLIC	SYSCALL		Last_POLYFLATPatch

PUBLIC	SYSCALL		Filler_Flat
PUBLIC	SYSCALL		Filler_Transparent
PUBLIC	SYSCALL		Filler_Trame
PUBLIC	SYSCALL		Filler_FlatZBuf
PUBLIC	SYSCALL		Filler_TransparentZBuf
PUBLIC	SYSCALL		Filler_TrameZBuf
PUBLIC	SYSCALL		Filler_FlatNZW
PUBLIC	SYSCALL		Filler_TransparentNZW
PUBLIC	SYSCALL		Filler_TrameNZW
PUBLIC	SYSCALL		Filler_FlagZBuffer


;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***


;					*** MODULE DATA ***
PUBLIC	SYSCALL		Fill_VLFlat1
PUBLIC	SYSCALL		Fill_VLFlat2
PUBLIC	SYSCALL		Fill_VLFlat3
PUBLIC	SYSCALL		Fill_VLFlat4
PUBLIC	SYSCALL		Fill_VLFlat5
PUBLIC	SYSCALL		Fill_VLFlat6
PUBLIC	SYSCALL		Fill_VLFlat7
PUBLIC	SYSCALL		Fill_VLFlat8
PUBLIC	SYSCALL		Fill_VLFlat9
PUBLIC	SYSCALL		Fill_VLFlat10



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
Extrn	SYSCALL		Fill_Filler		:	DWORD
Extrn	SYSCALL		Fill_Color		:	DWORD
Extrn	SYSCALL		Fill_Trame_Parity	:	DWORD
Extrn	SYSCALL		Fill_Patch		:	DWORD

Extrn	SYSCALL		Fill_FirstPoint		:	DWORD
Extrn	SYSCALL		Fill_LastPoint		:	DWORD
Extrn	SYSCALL		Fill_LeftPoint		:	DWORD
Extrn	SYSCALL		Fill_RightPoint		:	DWORD

Extrn	SYSCALL		Fill_LeftSlope		:	DWORD
Extrn	SYSCALL		Fill_RightSlope		:	DWORD

Extrn	SYSCALL		Fill_CurY		:	DWORD
Extrn	SYSCALL		Fill_CurXMin		:	DWORD
Extrn	SYSCALL		Fill_CurXMax		:	DWORD


Extrn	SYSCALL		Fill_ClipFlag		:	DWORD
Extrn	SYSCALL		Fill_ReadFlag		:	DWORD

Extrn	SYSCALL		PTR_TabOffLine		:	DWORD
Extrn	SYSCALL		Fill_CurOffLine		:	DWORD
Extrn	SYSCALL		Log			:	DWORD
Extrn	SYSCALL		PtrZBuffer		:	DWORD
Extrn	SYSCALL		ScreenPitch		:	DWORD


Extrn	SYSCALL		ClipXMin		:	DWORD
Extrn	SYSCALL		ClipXMax		:	DWORD
Extrn	SYSCALL		ClipYMin		:	DWORD
Extrn	SYSCALL		ClipYMax		:	DWORD

Extrn	SYSCALL		Fill_CurZBufMin		:	DWORD
Extrn	SYSCALL		Fill_ZBuf_LeftSlope	:	DWORD
Extrn	SYSCALL		Fill_ZBuf_XSlope	:	DWORD

Extrn	SYSCALL		IsPolygonHidden		:	DWORD




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
Extrn	SYSCALL		Fill_ClipXMin		:	PROC
Extrn	SYSCALL		Fill_ClipXMax		:	PROC
Extrn	SYSCALL		Fill_ClipYMin		:	PROC
Extrn	SYSCALL		Fill_ClipYMax		:	PROC

Extrn	SYSCALL		Triangle_ReadNextEdge	:	PROC



;				*******************
;				*** Global proc ***
;				*******************
COMMENT \
; ************************************************************************
; ************************************************************************
; ***  Fill_PolyFlat : Polygon filler (solid/flat model)               ***
; ***                                                                  ***
; ************************************************************************
; ***  Call: ESI = Pointer to point-list (STRUC_POINT)                 ***
; ***        ECX = Nb of points                                        ***
; ***                                                                  ***
; ************************************************************************
; ************************************************************************
			ALIGN	4
Fill_PolyFlat		PROC
			mov	eax,CLIP_FLAT
			mov	ebx,Offset @@Fill_Restart

			mov	[Fill_ClipFlag],eax
			mov	eax,1

			mov	[Fill_Restart],ebx
			mov	[Fill_Patch],eax

@@Fill_Restart:
			; *** Search the highest point in the list ***
			; *** And get the poly-box so as to test the clipping ***

			lea	eax,[ecx-1]		; Loop counter in EAX
			mov	ecx,dword ptr [esi].Pt_XE

			mov	edx,ecx
			mov	[Fill_FirstPoint],esi

			shl	ecx,16
			push	esi			; Local variable: TopPoint

			sar	edx,16
			add	esi,Size STRUC_POINT

			sar	ecx,16
			mov	ebp,edx

			mov	edi,ecx
							; (ECX, EDX, EDI, EBP)
							; (XMin, YMin, XMax, YMax)
@@Search_TopPoint:
			push	eax			; What follows has a bad pairing... How can we pair when we have NOTHING to do ?
			mov	eax,dword ptr [esi].Pt_XE
			mov	ebx,eax

			shl	eax,16
			sar	ebx,16
			sar	eax,16

			cmp	edx,ebx
			jle	@@Not_YMin

			mov	edx,ebx
			mov	[esp+4],esi
@@Not_YMin:
			cmp	ebp,ebx
			jge	@@Not_YMax
			mov	ebp,ebx
@@Not_YMax:
			cmp	ecx,eax
			jle	@@Not_XMin
			mov	ecx,eax
@@Not_XMin:
			cmp	edi,eax
			jge	@@Not_XMax
			mov	edi,eax
@@Not_XMax:
			pop	eax
			add	esi,Size STRUC_POINT

			dec	eax
			jne	@@Search_TopPoint

			pop	eax			; Get highest point pointer
			sub	esi,Size STRUC_POINT

			mov	[Fill_LeftPoint],eax
			mov	[Fill_RightPoint],eax

			cmp	ebp,edx
			jle	@@End			; Skip poly if DeltaY<=0

				; *** Is the polygon at least visible ? ***

			mov	eax,[ClipXMax]
			mov	ebx,[ClipXMin]

			cmp	ecx,eax
			jg	@@End

			cmp	edi,ebx
			jl	@@End

			mov	eax,[ClipYMax]
			mov	ebx,[ClipYMin]

			cmp	edx,eax
			jg	@@End

			cmp	ebp,ebx
			jl	@@End

			mov	[Fill_LastPoint],esi	; Last point pointer
			mov	esi,[Fill_LeftPoint]

				; *** Should we clip the polygon ? ***

			mov	eax,[ClipXMin]
			mov	ebx,[ClipXMax]

			cmp	ecx,eax
			jl	Fill_ClipXMin

			cmp	edi,ebx
			jg	Fill_ClipXMax

			mov	eax,[ClipYMin]
			mov	ebx,[ClipYMax]

			cmp	edx,eax
			jl	Fill_ClipYMin

			cmp	ebp,ebx
			jg	Fill_ClipYMax


			mov	edi,esi
			mov	ebp,[PTR_TabOffLine]

			mov	[Fill_CurY],edx
			mov	eax,[Log]

			mov	ebx,[ebp+edx*4]
			mov	ecx,READ_NEXT_L + READ_NEXT_R

			add	ebx,eax
			mov	[Fill_ReadFlag],ecx

			mov	[Fill_CurOffLine],ebx	; Current offset in log screen


;			*****************
;			*** LEFT EDGE ***
;			*****************

Flat_ReadNextEdge::
			mov	eax,[Fill_ReadFlag]

			test	eax,READ_NEXT_L
			je	@@Read_Next_Right

			mov	esi,[Fill_LeftPoint]
			and	eax,not READ_NEXT_L

			mov	[Fill_ReadFlag],eax
			mov	eax,[Fill_LastPoint]
@@Read_Next_L:
			mov	edi,esi
			add	esi,Size STRUC_POINT

			cmp	esi,eax
			jbe	@@No_LoopLeft

			mov	esi,[Fill_FirstPoint]
@@No_LoopLeft:
							; EDI = Old point
							; ESI = New point
			mov	edx,dword ptr [edi].Pt_XE
			mov	ecx,dword ptr [esi].Pt_XE

			mov	ebx,edx
			and	ecx,0FFFF0000h		; ECX = NewY shl 16

			shl	ebx,16			; EBX = OldXMin
			and	edx,0FFFF0000h		; EDX = OldY shl 16

			sub	ecx,edx			; If same Y then get next point
			je	@@Read_Next_L

			lea	ebx,[ebx+8000h]
			js	@@End			; If (NewY-OldY)<0 then we must end the rasterization

			shr	ecx,16
			mov	eax,dword ptr [esi].Pt_XE

			shl	eax,16
			mov	[Fill_CurXMin],ebx

			sub	ebx,8000h
			mov	edx,eax

			sub	eax,ebx
			sub	edx,ebx

			sar	edx,31
			mov	[Fill_LeftPoint],esi

			idiv	ecx

			or	eax,1

			mov	[Fill_LeftSlope],eax

;			******************
;			*** RIGHT EDGE ***
;			******************

@@Read_Next_Right:
			mov	eax,[Fill_ReadFlag]

			test	eax,READ_NEXT_R
			je	@@Test_Scan

			mov	esi,[Fill_RightPoint]
			and	eax,not READ_NEXT_R

			mov	[Fill_ReadFlag],eax
			mov	eax,[Fill_FirstPoint]
@@Read_Next_R:
			mov	edi,esi
			sub	esi,Size STRUC_POINT

			cmp	esi,eax
			jae	@@No_LoopRight

			mov	esi,[Fill_LastPoint]
@@No_LoopRight:
							; EDI = Old point
							; ESI = New point
			mov	ebx,dword ptr [edi].Pt_XE
			mov	ecx,dword ptr [esi].Pt_XE

			mov	edx,ebx
			and	ecx,0FFFF0000h		; ECX = NewY shl 16

			shl	ebx,16			; EBX = OldXMin
			and	edx,0FFFF0000h		; EDX = OldY shl 16

			sub	ecx,edx			; If same Y then get next point
			je	@@Read_Next_R

			lea	ebx,[ebx+8000h]
			js	@@End			; If (NewY-OldY)<0 then we must end the rasterization

			shr	ecx,16
			mov	eax,dword ptr [esi].Pt_XE

			shl	eax,16
			mov	[Fill_CurXMax],ebx

			sub	ebx,8000h
			mov	edx,eax

			sub	eax,ebx
			sub	edx,ebx

			sar	edx,31
			mov	[Fill_RightPoint],esi

			idiv	ecx

			or	eax,1

			mov	[Fill_RightSlope],eax


;			***************
;			*** DISPLAY ***
;			***************
@@Test_Scan:
			mov	esi,[Fill_LeftPoint]
			mov	edi,[Fill_RightPoint]

			; AGI
			mov	ecx,dword ptr [esi].Pt_XE

			sar	ecx,16
			mov	ebx,dword ptr [edi].Pt_XE

			sar	ebx,16

			cmp	ecx,ebx
			jl	@@Case_Left

			jg	@@Case_Right

;					*** Case 'both' ***
			mov	eax,READ_NEXT_L + READ_NEXT_R
			mov	ebx,[Fill_CurY]

			mov	[Fill_ReadFlag],eax
			sub	ecx,ebx				; DeltaY

			mov	ebx,[Fill_CurXMin]		; EBX = XMin SHL 16
			je	Flat_ReadNextEdge

			mov	edx,[Fill_CurXMax]		; EDX = XMax SHL 16
			jmp	[Fill_Filler]

			ALIGN	4
;					*** Case 'left' ***
@@Case_Left:
			mov	eax,READ_NEXT_L
			mov	ebx,[Fill_CurY]

			sub	ecx,ebx				; DeltaY
			mov	[Fill_ReadFlag],eax

			mov	ebx,[Fill_CurXMin]		; EBX = XMin SHL 16
			je	Flat_ReadNextEdge

			mov	edx,[Fill_CurXMax]		; EDX = XMax SHL 16
			jmp	[Fill_Filler]


			ALIGN	4
;					*** Case 'right' ***
@@Case_Right:
			mov	eax,READ_NEXT_R
			mov	ecx,ebx

			mov	ebx,[Fill_CurY]
			mov	[Fill_ReadFlag],eax

			sub	ecx,ebx				; DeltaY
			je	Flat_ReadNextEdge

			mov	edx,[Fill_CurXMax]		; EDX = XMax SHL 16
			mov	ebx,[Fill_CurXMin]		; EBX = XMin SHL 16

			jmp	[Fill_Filler]


			ALIGN	4
@@End:
			pop	ebp
			mov	eax,[Fill_LeftSlope]
			ret
Fill_PolyFlat		ENDP


\


;				******************
;				*** Local proc ***
;				******************

First_POLYFLATPatch	Label	DWORD



; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_Flat		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	esi,[Fill_LeftSlope]
			mov	eax,[Fill_RightSlope]

			add	ebx,esi
			add	edx,eax

Fill_VLFlat1::		add	edi,SIZE_VIDEOLINE
@@EnterLoop:
			mov	esi,ebx
			mov	ecx,edx

			shr	esi,16
			push	edi

			shr	ecx,16
			add	edi,esi

			sub	ecx,esi
			jle	@@EndScanLine

			mov	al,[edi]	; Load 1st dst cache line
			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache

			jb	@@SmallScanLine

				; *** Load dst in WB cache ***
			push	edi
			push	edx

			lea	edx,[edi+ecx]
			and	edi,not 31

			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edx
			pop	edi

			mov	eax,[Fill_Color]	; Color
			test	edi,1b			; alignment test
			je	@@ALIGN_Ok

			mov	[edi],al
			inc	edi
			dec	ecx
@@ALIGN_Ok:
			shr	ecx,1
			rep	stosw			; Should we make a rep stosd (and making the code more complicated for small polys ?)
			jnc	@@EndScanLine
			mov	[edi],al
			jmp	@@EndScanLine

				; *** Flat loop ***
@@SmallScanLine:
			mov	eax,[Fill_Color]	; Color
@@LoopX:
			mov	[edi],al
			inc	edi

			dec	ecx
			jne	@@LoopX

@@EndScanLine:
			pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			jmp	Triangle_ReadNextEdge
Filler_Flat		ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_Transparent	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	esi,[Fill_LeftSlope]
			mov	eax,[Fill_RightSlope]

			add	ebx,esi
			add	edx,eax

Fill_VLFlat2::		add	edi,SIZE_VIDEOLINE
@@EnterLoop:
			mov	esi,ebx
			mov	ecx,edx

			shr	esi,16
			push	edi

			shr	ecx,16
			add	edi,esi

			sub	ecx,esi
			jle	@@EndScanLine

			mov	ah,byte ptr [Fill_Color]; Color
@@LoopX:
			mov	al,[edi]

			and	al,0Fh

			or	al,ah

			mov	[edi],al
			inc	edi

			dec	ecx
			jne	@@LoopX

@@EndScanLine:
			pop	edi
			dec	ebp

			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			jmp	Triangle_ReadNextEdge
Filler_Transparent	ENDP



; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_Trame		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_Trame_Parity]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat3::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
@@EnterLoop:
			mov	eax,ebx
			mov	ecx,edx

			shr	eax,16
			push	edi

			shr	ecx,16
			add	edi,eax

			sub	ecx,eax
			jle	@@EndScanLine


			mov	al,[edi]	; Load 1st dst cache line
			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache

			jb	@@SmallScanLine

				; *** Load dst in WB cache ***
			push	edi
			push	edx

			lea	edx,[edi+ecx]
			and	edi,not 31

			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edx
			pop	edi
@@SmallScanLine:
			shr	ecx,1
			je	@@EndScanLine

			xor	esi,1
			mov	eax,edi

			and	eax,1
			inc	ecx

			xor	eax,esi

			mov	eax,[Fill_Color]
			je	@@Pair

			inc	edi
@@Pair:
			shr	ecx,1
			jnc	@@Odd
@@Each_X:
			mov	[edi],al
			add	edi,2
@@Odd:
			mov	[edi],al
			add	edi,2

			dec	ecx
			jne	@@Each_X


@@EndScanLine:
			pop	edi
			dec	ebp

			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			xor	esi,1

			mov	[Fill_Trame_Parity],esi
			jmp	Triangle_ReadNextEdge
Filler_Trame		ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_FlatZBuf		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurZBufMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat4::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_ZBuf_LeftSlope]

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

			push	edx

			mov	al,[edi]	; Load 1st dst cache line
			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache

			jb	@@SmallScanLine

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
@@SmallScanLine:
				; *** Flat loop ***
			push	esi
			push	ebx

			mov	ebx,[Fill_ZBuf_XSlope]

			push	ebp
			mov	edx,esi

			shl	esi,32-8
			mov	ebp,ebx

			shl	ebp,32-8
			mov	eax,[Log]

			shr	ebx,8
			sub	edi,eax

			shr	edx,8
			mov	eax,[Fill_Color]	; Color
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
@@PtrZBuffer1:		mov	[edi*2+12345678h],dx
@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:
			inc	edi
			add	esi,ebp

			adc	edx,ebx
			dec	ecx

			jne	@@LoopX

			pop	ebp
			pop	ebx

			pop	esi
			pop	edx
@@EndScanLine:
			pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurZBufMin],esi
			mov	[Fill_CurXMin],ebx

			mov	[Fill_CurXMax],edx
			mov	[Fill_CurOffLine],edi

			jmp	Triangle_ReadNextEdge
Filler_FlatZBuf		ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TransparentZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			dec	eax
			mov	esi,[Fill_CurZBufMin]

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			mov	dword ptr [@@PtrLog1+2],eax

			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat5::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_ZBuf_LeftSlope]

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

			push	ebx
			push	edx

			push	esi
			push	ebp

			mov	eax,[Log]
			mov	edx,esi

			shr	edx,8
			sub	edi,eax

			shl	esi,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			mov	ebp,ebx
			mov	ah,byte ptr [Fill_Color]; Color

			shr	ebx,8

			shl	ebp,32-8
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
@@PtrZBuffer1:		mov	[edi*2+12345678h],dx
@@PtrLog:		mov	al,[edi+12345678h]

			and	al,0Fh

			or	al,ah

@@PtrLog1:		mov	[edi+12345678h],al
@@NoDisp:		inc	edi
			add	esi,ebp

			adc	edx,ebx
			dec	ecx

			jne	@@LoopX

			pop	ebp
			pop	esi

			pop	edx
			pop	ebx
@@EndScanLine:
			pop	edi
			dec	ebp

			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurZBufMin],esi
			jmp	Triangle_ReadNextEdge
Filler_TransparentZBuf	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TrameZBuf	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_Trame_Parity]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	dword ptr [@@PtrZBuffer3+4],eax

			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			mov	dword ptr [@@PtrLog2+2],eax

			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat6::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			push	ebx

			mov	ebx,[Fill_CurZBufMin]
			mov	eax,[Fill_ZBuf_LeftSlope]

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


			mov	al,[edi]	; Load 1st dst cache line
			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache

			jb	@@SmallScanLine

				; *** Load dst in WB cache ***
			push	edi
			push	edx

			lea	edx,[edi+ecx]
			and	edi,not 31

			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edx
			pop	edi
@@SmallScanLine:
			shr	ecx,1
			je	@@EndScanLine

			xor	esi,1
			mov	eax,edi

			and	eax,1
			inc	ecx

			xor	eax,esi
			je	@@Pair

			inc	edi
@@Pair:
			push	esi
			push	ebx

			push	edx
			push	ebp

			mov	esi,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_XSlope]

			add	ebx,ebx
			mov	edx,esi

			shl	esi,32-8
			mov	ebp,ebx

			shl	ebp,32-8
			mov	eax,[Log]

			shr	edx,8
			sub	edi,eax

			shr	ebx,8
			mov	eax,[Fill_Color]

			shr	ecx,1
			jnc	@@Odd
@@Each_X:
@@PtrZBuffer:  		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp0
@@PtrZBuffer1:  	mov	[edi*2+12345678h],dx
@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp0:		add	edi,2
			add	esi,ebp

			adc	edx,ebx
@@Odd:
@@PtrZBuffer2:  	cmp	[edi*2+12345678h],dx
			jb	@@NoDisp2
@@PtrZBuffer3:  	mov	[edi*2+12345678h],dx
@@PtrLog2:		mov	[edi+12345678h],al
@@NoDisp2:		add	edi,2

			dec	ecx
			jne	@@Each_X

			pop	ebp
			pop	edx

			pop	ebx
			pop	esi

@@EndScanLine:
			pop	edi
			dec	ebp

			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			xor	esi,1

			mov	[Fill_Trame_Parity],esi
			jmp	Triangle_ReadNextEdge
Filler_TrameZBuf	ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_FlatNZW		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurZBufMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat7::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_ZBuf_LeftSlope]

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

			push	edx

			mov	al,[edi]	; Load 1st dst cache line
			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache

			jb	@@SmallScanLine

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
@@SmallScanLine:
				; *** Flat loop ***
			push	esi
			push	ebx

			mov	ebx,[Fill_ZBuf_XSlope]

			push	ebp
			mov	edx,esi

			shl	esi,32-8
			mov	ebp,ebx

			shl	ebp,32-8
			mov	eax,[Log]

			shr	ebx,8
			sub	edi,eax

			shr	edx,8
			mov	eax,[Fill_Color]	; Color
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp:
			inc	edi
			add	esi,ebp

			adc	edx,ebx
			dec	ecx

			jne	@@LoopX

			pop	ebp
			pop	ebx

			pop	esi
			pop	edx
@@EndScanLine:
			pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurZBufMin],esi
			mov	[Fill_CurXMin],ebx

			mov	[Fill_CurXMax],edx
			mov	[Fill_CurOffLine],edi

			jmp	Triangle_ReadNextEdge
Filler_FlatNZW		ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TransparentNZW	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			dec	eax
			mov	esi,[Fill_CurZBufMin]

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			mov	dword ptr [@@PtrLog1+2],eax

			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat8::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_ZBuf_LeftSlope]

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

			push	ebx
			push	edx

			push	esi
			push	ebp

			mov	eax,[Log]
			mov	edx,esi

			shr	edx,8
			sub	edi,eax

			shl	esi,32-8
			mov	ebx,[Fill_ZBuf_XSlope]

			mov	ebp,ebx
			mov	ah,byte ptr [Fill_Color]; Color

			shr	ebx,8

			shl	ebp,32-8
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
@@PtrLog:		mov	al,[edi+12345678h]

			and	al,0Fh

			or	al,ah

@@PtrLog1:		mov	[edi+12345678h],al
@@NoDisp:		inc	edi
			add	esi,ebp

			adc	edx,ebx
			dec	ecx

			jne	@@LoopX

			pop	ebp
			pop	esi

			pop	edx
			pop	ebx
@@EndScanLine:
			pop	edi
			dec	ebp

			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			mov	[Fill_CurZBufMin],esi
			jmp	Triangle_ReadNextEdge
Filler_TransparentNZW	ENDP





; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TrameNZW		PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_Trame_Parity]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer2+4],eax

			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			mov	dword ptr [@@PtrLog2+2],eax

			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat9::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			push	ebx

			mov	ebx,[Fill_CurZBufMin]
			mov	eax,[Fill_ZBuf_LeftSlope]

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


			mov	al,[edi]	; Load 1st dst cache line
			cmp	ecx,32		; Is the scanline big enough to load the dst lines in cache

			jb	@@SmallScanLine

				; *** Load dst in WB cache ***
			push	edi
			push	edx

			lea	edx,[edi+ecx]
			and	edi,not 31

			and	edx,not 31	; AGI on EDI on first loop
@@Preload:
			mov	ah,[edi]
			add	edi,32

			cmp	edi,edx
			jbe	@@Preload

			pop	edx
			pop	edi
@@SmallScanLine:
			shr	ecx,1
			je	@@EndScanLine

			xor	esi,1
			mov	eax,edi

			and	eax,1
			inc	ecx

			xor	eax,esi
			je	@@Pair

			inc	edi
@@Pair:
			push	esi
			push	ebx

			push	edx
			push	ebp

			mov	esi,[Fill_CurZBufMin]
			mov	ebx,[Fill_ZBuf_XSlope]

			add	ebx,ebx
			mov	edx,esi

			shl	esi,32-8
			mov	ebp,ebx

			shl	ebp,32-8
			mov	eax,[Log]

			shr	edx,8
			sub	edi,eax

			shr	ebx,8
			mov	eax,[Fill_Color]

			shr	ecx,1
			jnc	@@Odd
@@Each_X:
@@PtrZBuffer:  		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp0
@@PtrLog:		mov	[edi+12345678h],al
@@NoDisp0:		add	edi,2
			add	esi,ebp

			adc	edx,ebx
@@Odd:
@@PtrZBuffer2:  	cmp	[edi*2+12345678h],dx
			jb	@@NoDisp2
@@PtrLog2:		mov	[edi+12345678h],al
@@NoDisp2:		add	edi,2

			dec	ecx
			jne	@@Each_X

			pop	ebp
			pop	edx

			pop	ebx
			pop	esi

@@EndScanLine:
			pop	edi
			dec	ebp

			jne	@@Each_Line

			mov	[Fill_CurXMin],ebx
			mov	[Fill_CurXMax],edx

			mov	[Fill_CurOffLine],edi
			xor	esi,1

			mov	[Fill_Trame_Parity],esi
			jmp	Triangle_ReadNextEdge
Filler_TrameNZW		ENDP






; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_FlagZBuffer	PROC
			lea	ebp,[ecx+1]
			mov	eax,[Fill_CurY]

			mov	edi,[Fill_CurOffLine]
			add	eax,ebp

			mov	[Fill_CurY],eax
			mov	eax,[Fill_Patch]

			mov	esi,[Fill_CurZBufMin]
			dec	eax

			jne	@@Each_Line

			mov	[Fill_Patch],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			jmp	@@EnterLoop
			ALIGN	4
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLFlat10::		add	edi,SIZE_VIDEOLINE

			add	ebx,eax
			mov	eax,[Fill_RightSlope]

			add	edx,eax
			mov	eax,[Fill_ZBuf_LeftSlope]

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

			push	edx


				; *** Flat loop ***
			push	esi
			push	ebx

			mov	ebx,[Fill_ZBuf_XSlope]

			push	ebp
			mov	edx,esi

			shl	esi,32-8
			mov	ebp,ebx

			shl	ebp,32-8
			mov	eax,[Log]

			shr	ebx,8
			sub	edi,eax

			shr	edx,8
			xor	eax,eax
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],dx
			jb	@@NoDisp
			mov	[IsPolygonHidden],eax
@@NoDisp:
			inc	edi
			add	esi,ebp

			adc	edx,ebx
			dec	ecx

			jne	@@LoopX

			pop	ebp
			pop	ebx

			pop	esi
			pop	edx
@@EndScanLine:
			pop	edi

			dec	ebp
			jne	@@Each_Line

			mov	[Fill_CurZBufMin],esi
			mov	[Fill_CurXMin],ebx

			mov	[Fill_CurXMax],edx
			mov	[Fill_CurOffLine],edi

			jmp	Triangle_ReadNextEdge
Filler_FlagZBuffer	ENDP
















Last_POLYFLATPatch	Label	DWORD



_TEXT			ENDS

;			The
			End
