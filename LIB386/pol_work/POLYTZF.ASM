; **************************************************************************
; **************************************************************************
; **************************************************************************
; ***                                                                    ***
; ***                         POLYGON LIBRARY                            ***
; ***                                                                    ***
; **************************************************************************
; ***                                                                    ***
; ***            (Adeline Software Intl.)                                ***
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
PUBLIC	SYSCALL		First_POLYTEXTUREZFOGPatch
PUBLIC	SYSCALL		Last_POLYTEXTUREZFOGPatch

PUBLIC	SYSCALL		Filler_TextureZFogSmooth
PUBLIC	SYSCALL		Filler_TextureZFogSmoothZBuf
PUBLIC	SYSCALL		Filler_TextureZFogSmoothNZW


;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***
PUBLIC	SYSCALL		Fill_VLTextureZFog1
PUBLIC	SYSCALL		Fill_VLTextureZFog2
PUBLIC	SYSCALL		Fill_VLTextureZFog3


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
Extrn	SYSCALL		PtrCLUTFog		:	DWORD
Extrn	SYSCALl		PtrZBuffer		:	DWORD

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


Extrn	SYSCALL		F_256			:	DWORD

Extrn	SYSCALL		Fill_UseFPU		:	BYTE

Extrn	SYSCALL		Fill_Z_Fog_Near		:	DWORD
Extrn	SYSCALL		Fill_Fog_Factor		:	DWORD

Extrn	SYSCALL		Fill_ScaledFogNear	:	DWORD




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
Fill_Cur_ZBufOverW	dd	0
Fill_Next_ZBufOverW	dd	0
Fill_Next_ZBuf		dd	0

			ALIGN	4
Fill_Init_Perspective	dd	Offset Choose_Init
Fill_Loop_Perspective	dd	Offset Choose_Loop


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
First_POLYTEXTUREZFOGPatch Label	DWORD





Choose_Init		PROC
			mov	[Fill_Init_Perspective],Offset Init_PerspINT

			cmp	[Fill_UseFPU],TRUE
			jne	@@End

			mov	[Fill_Init_Perspective],Offset Init_PerspFPU
@@End:
			jmp	[Fill_Init_Perspective]
Choose_Init		ENDP



Choose_Loop		PROC
			mov	[Fill_Loop_Perspective],Offset Loop_PerspINT

			cmp	[Fill_UseFPU],TRUE
			jne	@@End

			mov	[Fill_Loop_Perspective],Offset Loop_PerspFPU
@@End:
			jmp	[Fill_Loop_Perspective]
Choose_Loop		ENDP







; ╔═════════════════════════════════════════════════════════════╗
; ║ Initialize the gradiants of a scanline                      ║
; ╚═════════════════════════════════════════════════════════════╝

Init_PerspINT		PROC
			mov	ebx,[Fill_Cur_W]		; Current W

			test	ebx,ebx
			je	@@W_Nul1
			cmp	ebx,-1
			je	@@W_Nul1
			cmp	ebx,1
			je	@@W_Nul1
@@Return_Error1:
			mov	eax,-1
			xor	edx,edx

			idiv	ebx

			mov	ebx,eax				; EBX = 1/W
			mov	eax,esi

			imul	ebx

			shr	eax,24
			mov	[Fill_Cur_MapUOverW],esi

			shl	edx,8
			mov	[Fill_Cur_MapVOverW],ebp

			or	eax,edx

			mov	[Fill_Cur_MapU],eax		; Current U
			mov	eax,ebp

			imul	ebx

			shr	eax,24

			shl	edx,8

			or	edx,eax
			mov	eax,[Fill_CurZBufMin]

			mov	[Fill_Cur_MapV],edx		; Current V
			mov	[Fill_Cur_ZBufOverW],eax	; Cur ZBuf

			imul	ebx
			shr	eax,16
			shl	edx,16
			or	eax,edx
			mov	[Fill_CurZBuf],eax		; Cur ZBuf

			ret
@@W_Nul1:
			mov	ebx,2
			jmp	@@Return_Error1
Init_PerspINT		ENDP





; ╔═════════════════════════════════════════════════════════════╗
; ║ Initialize the gradiants of a section of PERSP_VALUE pixels ║
; ╚═════════════════════════════════════════════════════════════╝

Loop_PerspINT		PROC
			mov	edx,[Fill_W_XSlope]
			mov	ebx,[Fill_Cur_W]

			shl	edx,5
			mov	eax,-1

			add	ebx,edx
			je	@@W_Nul
			cmp	ebx,-1
			je	@@W_Nul
			cmp	ebx,1
			je	@@W_Nul
@@Return_Error:
			mov	[Fill_Next_W],ebx		; Next W
			xor	edx,edx

			idiv	ebx

			mov	ebx,eax				; EBX = 1/W
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,5
			mov	edx,[Fill_Cur_ZBufOverW]

			add	eax,edx

			mov	[Fill_Next_ZBufOverW],eax

			imul	ebx

			shr	eax,16
			shl	edx,16
			or	edx,eax
			mov	[Fill_Next_ZBuf],edx


			mov	eax,[Fill_MapU_XSlope]
			shl	eax,5
			mov	edx,[Fill_Cur_MapUOverW]

			add	eax,edx

			mov	[Fill_Next_MapUOverW],eax

			imul	ebx

			shr	eax,24
			mov	esi,[Fill_MapV_XSlope]

			shl	edx,8
			mov	ebp,[Fill_Cur_MapVOverW]

			shl	esi,5
			or	eax,edx

			mov	[Fill_Next_MapU],eax	; EBP = Next MapU
			add	esi,ebp

			mov	[Fill_Next_MapVOverW],esi
			mov	ebp,eax

			mov	eax,esi

			imul	ebx

			shr	eax,24

			shl	edx,8

			or	eax,edx

			mov	[Fill_Next_MapV],eax	; EAX = Next MapV

			ret
@@W_Nul:
			mov	ebx,2
			jmp	@@Return_Error
Loop_PerspINT		ENDP






; ╔═════════════════════════════════════════════════════════════╗
; ║ Initialize the gradiants of a scanline                      ║
; ╚═════════════════════════════════════════════════════════════╝

Init_PerspFPU		PROC
			mov	ebx,[Fill_Cur_W]		; Current W

			test	ebx,ebx
			je	@@W_Nul1
			cmp	ebx,-1
			je	@@W_Nul1
			cmp	ebx,1
			je	@@W_Nul1
@@Return_Error1:
			fild	[Fill_Cur_W]	; W

			mov	[Fill_Cur_MapUOverW],esi
			mov	edx,[Fill_W_XSlope]

			shl	edx,5
			mov	[Fill_Cur_MapVOverW],ebp

			fdivr	[F_256]		; 1/W

			; Here, we precalc an array in the stack. This array
			; contains valid Cur_W values

			pop	ebp		; EBP = Return address
			lea	eax,[ecx+32-1]

			shr	eax,5-2
			xor	esi,esi

			and	eax,not 11b

			sub	esp,eax
@@CreateStack:
			add	ebx,edx
			je	@@W_Nul
			cmp	ebx,-1
			je	@@W_Nul
			cmp	ebx,1
			je	@@W_Nul
@@Return_Error:
			mov	[esp+esi],ebx
			add	esi,4

			cmp	esi,eax
			jne	@@CreateStack

			pop	ebx
			mov	eax,[Fill_CurZBufMin]

			mov	[Fill_Cur_W],ebx
			mov	[Fill_Cur_ZBufOverW],eax	; Cur ZBuf

			fild	[Fill_Cur_MapUOverW]; MapU/Z  1/W

			fild	[Fill_Cur_MapVOverW]; MapV/Z  MapU/Z  1/W

			fild	[Fill_CurZBufMin]; ZBuf/Z  MapV/Z  MapU/Z  1/W

			fxch	st(2)		; MapU/Z  MapV/Z  ZBuf/Z  1/W

			fmul	st,st(3)	; MapU  MapV/Z  ZBuf/Z  1/W

			fild	[Fill_Cur_W]	; W' MapU  MapV/Z  ZBuf/Z  1/W

			fxch	st(2)		; MapV/Z  MapU  W'  ZBuf/Z  1/W

			fmul	st,st(4)	; MapV  MapU  W'  ZBuf/Z  1/W
			fxch	st(1)		; MapU  MapV  W'  ZBuf/Z  1/W

			fistp	[Fill_Cur_MapU]	; MapV  W'  ZBuf/Z  1/W

			fxch	st(2)		; ZBuf/Z  W'  MapV  1/W

			fmulp	st(3),st	; W'  MapV  ZBuf
			fxch	st(1)		; MapV  W'  ZBuf

			fistp	[Fill_Cur_MapV]	; W'  ZBuf

			fxch	st(1)		; ZBuf  W'

			fmul	[F_256]
			fistp	[Fill_CurZBuf]	; W'

			fdivr	[F_256]		; 1/W'

			jmp	ebp		; RET
@@W_Nul1:
			mov	ebx,2
			mov	[Fill_Cur_W],ebx
			jmp	@@Return_Error1
@@W_Nul:
			mov	ebx,2
			jmp	@@Return_Error
Init_PerspFPU		ENDP








; ╔═════════════════════════════════════════════════════════════╗
; ║ Initialize the gradiants of a section of PERSP_VALUE pixels ║
; ╚═════════════════════════════════════════════════════════════╝

Loop_PerspFPU		PROC
			mov	eax,[Fill_MapU_XSlope]
			mov	esi,[Fill_MapV_XSlope]

			shl	eax,5
			mov	edx,[Fill_Cur_MapUOverW]

			add	eax,edx
			mov	ebp,[Fill_Cur_MapVOverW]

			shl	esi,5
			mov	[Fill_Next_MapUOverW],eax

			add	esi,ebp
			mov	edx,[Fill_ZBuf_XSlope]

			shl	edx,5
			mov	eax,[Fill_Cur_ZBufOverW]

			pop	ebp		; EBP = Return address
			add	eax,edx

			mov	[Fill_Next_MapVOverW],esi
			mov	[Fill_Next_ZBufOverW],eax

			fild	[Fill_Next_MapUOverW]; U/Z  1/W

			fild	[Fill_Next_MapVOverW]; V/Z  U/Z  1/W

			fild	[Fill_Next_ZBufOverW]; ZBuf/Z  V/Z  U/Z  1/W

			fxch	st(2)		; U/Z  V/Z  ZBuf/Z  1/W

			fmul	st,st(3)	; U  V/Z  ZBuf/Z  1/W
			fxch	st(1)		; V/Z  U  ZBuf/Z  1/W

			; **

			fmul	st,st(3)	; V  U  ZBuf/Z  1/W
			fxch	st(1)		; U  V  ZBuf/Z  1/W

			fistp	[Fill_Next_MapU]; V  ZBuf/Z  1/W

			fxch	st(1)		; ZBuf/Z  V  1/W

			fmulp	st(2),st	; V  ZBuf

			fistp	[Fill_Next_MapV]

			fmul	[F_256]
			fistp	[Fill_Next_ZBuf]

			cmp	ecx,32
			jbe	@@Less32

			fild	dword ptr [esp]	; W

			pop	ebx

			mov	[Fill_Next_W],ebx	; Next W

			fdivr	[F_256]		; 1/W
@@Less32:
			jmp	ebp		; RET
Loop_PerspFPU		ENDP




















; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZFogSmooth PROC
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
			mov	eax,[PtrCLUTFog]

			add	eax,12*256

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZFog1::	add	edi,SIZE_VIDEOLINE

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
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurWMin],eax
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

			mov	dl,[edi]	; Load 1st dst cache line
			mov	eax,[Fill_CurWMin]

			xor	ebx,0FFFFh
			mov	[Fill_Cur_W],eax

			cmp	ecx,32
			jbe	@@FastInit

			mov	eax,[Fill_W_XSlope]

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
					; *** Init Scanline ***
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,5			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,5	       		; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	edx,[Fill_CurZBuf]

			shr	edx,8
			mov	eax,ebx

			shr	eax,16
			push	ecx

			cmp	ecx,32
			jb	@@Less32

			mov	ecx,32
@@Less32:
			mov	esi,[RepMask]
			mov	al,bh

			add	edi,ecx			; EDI = End on scanline
			xor	ecx,-1

			and	eax,esi
			mov	esi,[Fill_ScaledFogNear]

			sub	edx,esi
			js	@@CaseNull

			imul	edx,[Fill_Fog_Factor]

			shr	edx,8
			mov	esi,[PtrMap]

			and	edx,0F000h
			jmp	@@Go
@@CaseNull:
			mov	esi,[PtrMap]
			xor	edx,edx
@@Go:
			inc	ecx
@@LoopX:
			mov	dl,[eax+esi]
			lea	eax,[ebx+ebp]

			shr	eax,16
			add	ebx,ebp

@@PtrCLUT:		mov	dl,[edx+12345678h]
			mov	al,bh

			mov	[edi+ecx],dl
@@RepMask:		and	eax,12345678h

			inc	ecx
			jne	@@LoopX

			pop	ecx
			mov	eax,[Fill_Next_MapU]

			cmp	ecx,32
			jle	@@End_X

			mov	ebx,[Fill_Next_MapV]
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_Cur_MapV],ebx
			mov	eax,[Fill_Next_MapUOverW]

			mov	ebx,[Fill_Next_MapVOverW]
			mov	[Fill_Cur_MapUOverW],eax

			mov	[Fill_Cur_MapVOverW],ebx
			mov	eax,[Fill_Next_W]

			mov	[Fill_Cur_W],eax
			mov	ebx,[Fill_Next_ZBuf]

			mov	[Fill_CurZBuf],ebx
			mov	ebx,[Fill_Next_ZBufOverW]

			mov	[Fill_Cur_ZBufOverW],ebx
			sub	ecx,32

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
Filler_TextureZFogSmooth ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZFogSmoothZBuf PROC
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
			mov	eax,[PtrCLUTFog]

			add	eax,12*256

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	dword ptr [@@RepMask2+1],eax

			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer1+4],eax

			mov	dword ptr [@@PtrZBuffer2+4],eax
			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8

			mov	dword ptr [@@DecZBuf+2],eax
			mov	dword ptr [@@DecZBuf2+2],eax

			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8

			and	eax,0FFFFh

			mov	dword ptr [@@IntZBuf+2],eax
			mov	dword ptr [@@IntZBuf2+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZFog2::	add	edi,SIZE_VIDEOLINE

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
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurWMin],eax
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

			mov	dl,[edi]	; Load 1st dst cache line
			mov	eax,[Fill_CurWMin]

			xor	ebx,0FFFFh
			mov	[Fill_Cur_W],eax

			cmp	ecx,32
			jbe	@@FastInit

			mov	eax,[Fill_W_XSlope]

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
					; *** Init Scanline ***
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,5			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,5			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	edx,[Fill_CurZBuf]

			shr	edx,8
			mov	eax,ebx

			shr	eax,16
			mov	esi,[RepMask]

			mov	al,bh

			and	eax,esi
			mov	esi,[Log]

			sub	edi,esi
			push	ecx

			cmp	ecx,32
			jbe	@@Less32

			mov	ecx,32
@@Less32:
			mov	esi,[Fill_ScaledFogNear]

			sub	edx,esi
			js	@@CaseNull

			imul	edx,[Fill_Fog_Factor]

			shr	edx,8
			mov	esi,[Fill_CurZBuf]

			and	edx,0F000h
			jmp	@@Go
@@CaseNull:
			mov	esi,[Fill_CurZBuf]
			xor	edx,edx
@@Go:
			ror	esi,8
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],si
			jb	@@NoDisp
@@Disp:
@@PtrZBuffer1:		mov	[edi*2+12345678h],si
@@PtrMap:		mov	dl,[eax+12345678h]

			lea	eax,[ebx+ebp]
			add	ebx,ebp

			shr	eax,16
@@PtrCLUT:		mov	dl,[edx+12345678h]

@@PtrLog:		mov	[edi+12345678h],dl
			mov	al,bh

@@RepMask:		and	eax,12345678h
@@DecZBuf:		add	esi,12345678h

@@IntZBuf:		adc	esi,12345678h
			inc	edi

			dec	ecx
			jne	@@LoopX

			jmp	@@End16

@@NoDisp:
@@PtrZBuffer2:		cmp	[edi*2+12345678h],si
			jae	@@Disp

			lea	eax,[ebx+ebp]
			add	ebx,ebp

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf2:		add	esi,12345678h

@@IntZBuf2:		adc	esi,12345678h
@@RepMask2:		and	eax,12345678h

			dec	ecx
			jne	@@NoDisp
@@End16:
			pop	ecx
			mov	eax,[Fill_Next_MapU]

			cmp	ecx,32
			jle	@@End_X

			mov	ebx,[Fill_Next_MapV]
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_Cur_MapV],ebx
			mov	eax,[Fill_Next_MapUOverW]

			mov	ebx,[Fill_Next_MapVOverW]
			mov	[Fill_Cur_MapUOverW],eax

			mov	eax,[Fill_Next_W]
			mov	[Fill_Cur_MapVOverW],ebx

			mov	[Fill_Cur_W],eax
			mov	ebx,[Fill_Next_ZBuf]

			mov	eax,[Log]
			mov	[Fill_CurZBuf],ebx

			add	edi,eax
			mov	ebx,[Fill_Next_ZBufOverW]

			mov	[Fill_Cur_ZBufOverW],ebx
			sub	ecx,32

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
Filler_TextureZFogSmoothZBuf ENDP








; *** CALL: 	ECX = Nb lines to draw
;		EBX = Fill_CurXMin
;		EDX = Fill_CurXMax
			ALIGN	4
Filler_TextureZFogSmoothNZW PROC
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
			mov	eax,[PtrCLUTFog]

			add	eax,12*256

			mov	dword ptr [@@PtrCLUT+2],eax
			mov	eax,[RepMask]

			mov	dword ptr [@@RepMask+1],eax
			mov	dword ptr [@@RepMask2+1],eax

			mov	eax,[PtrMap]

			mov	dword ptr [@@PtrMap+2],eax
			mov	eax,[Log]

			mov	dword ptr [@@PtrLog+2],eax
			mov	eax,[PtrZBuffer]

			mov	dword ptr [@@PtrZBuffer+4],eax
			mov	dword ptr [@@PtrZBuffer2+4],eax

			mov	eax,[Fill_ZBuf_XSlope]

			shl	eax,32-8

			mov	dword ptr [@@DecZBuf+2],eax
			mov	dword ptr [@@DecZBuf2+2],eax

			mov	eax,[Fill_ZBuf_XSlope]

			shr	eax,8

			and	eax,0FFFFh

			mov	dword ptr [@@IntZBuf+2],eax
			mov	dword ptr [@@IntZBuf2+2],eax
			jmp	@@EnterLoop
@@Each_Line:
			mov	eax,[Fill_LeftSlope]
Fill_VLTextureZFog3::	add	edi,SIZE_VIDEOLINE

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
			mov	ebx,[Fill_ZBuf_LeftSlope]

			mov	[Fill_CurWMin],eax
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

			mov	dl,[edi]	; Load 1st dst cache line
			mov	eax,[Fill_CurWMin]

			xor	ebx,0FFFFh
			mov	[Fill_Cur_W],eax

			cmp	ecx,32
			jbe	@@FastInit

			mov	eax,[Fill_W_XSlope]

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
					; *** Init Scanline ***
			call	[Fill_Init_Perspective]
@@FastFill:
			call	[Fill_Loop_Perspective]

			mov	ebx,[Fill_Cur_MapU]	; EBX = CurMapU
			mov	ebp,[Fill_Next_MapU]	; EBP = NextMapU

			sub	ebp,ebx
			mov	edx,[Fill_Cur_MapV]	; EDX = CurMapV

			sar	ebp,5			; EBP = IncMapU
			mov	eax,[Fill_Next_MapV]	; EAX = Next MapV

			sub	eax,edx
			and	ebp,0FFFFh

			sar	eax,5			; EAX = IncMapV
			and	ebx,0FFFFh

			shl	eax,16

			shl	edx,16
			or	ebp,eax			; EBP = IncV | IncU

			or	ebx,edx			; EBX = MapV | MapU
			mov	edx,[Fill_CurZBuf]

			shr	edx,8
			mov	eax,ebx

			shr	eax,16
			mov	esi,[RepMask]

			mov	al,bh

			and	eax,esi
			mov	esi,[Log]

			sub	edi,esi
			push	ecx

			cmp	ecx,32
			jbe	@@Less32

			mov	ecx,32
@@Less32:
			mov	esi,[Fill_ScaledFogNear]

			sub	edx,esi
			js	@@CaseNull

			imul	edx,[Fill_Fog_Factor]

			shr	edx,8
			mov	esi,[Fill_CurZBuf]

			and	edx,0F000h
			jmp	@@Go
@@CaseNull:
			mov	esi,[Fill_CurZBuf]
			xor	edx,edx
@@Go:
			ror	esi,8
@@LoopX:
@@PtrZBuffer:		cmp	[edi*2+12345678h],si
			jb	@@NoDispFast
@@Disp:
@@PtrMap:		mov	dl,[eax+12345678h]
			lea	eax,[ebx+ebp]

			shr	eax,16
			add	ebx,ebp

@@PtrCLUT:		mov	dl,[edx+12345678h]
			mov	al,bh

@@PtrLog:		mov	[edi+12345678h],dl
@@RepMask:		and	eax,12345678h

@@DecZBuf:		add	esi,12345678h

@@IntZBuf:		adc	esi,12345678h
			inc	edi

			dec	ecx
			jne	@@LoopX

			jmp	@@End16

@@NoDisp:
@@PtrZBuffer2:		cmp	[edi*2+12345678h],si
			jae	@@Disp
@@NoDispFast:
			lea	eax,[ebx+ebp]
			add	ebx,ebp

			shr	eax,16
			inc	edi

			mov	al,bh
@@DecZBuf2:		add	esi,12345678h

@@IntZBuf2:		adc	esi,12345678h
@@RepMask2:		and	eax,12345678h

			dec	ecx
			jne	@@NoDisp

@@End16:
			pop	ecx
			mov	eax,[Fill_Next_MapU]

			cmp	ecx,32
			jle	@@End_X

			mov	ebx,[Fill_Next_MapV]
			mov	[Fill_Cur_MapU],eax

			mov	[Fill_Cur_MapV],ebx
			mov	eax,[Fill_Next_MapUOverW]

			mov	ebx,[Fill_Next_MapVOverW]
			mov	[Fill_Cur_MapUOverW],eax

			mov	eax,[Fill_Next_W]
			mov	[Fill_Cur_MapVOverW],ebx

			mov	[Fill_Cur_W],eax
			mov	eax,[Log]

			add	edi,eax
			mov	ebx,[Fill_Next_ZBuf]

			mov	[Fill_CurZBuf],ebx
			mov	ebx,[Fill_Next_ZBufOverW]

			mov	[Fill_Cur_ZBufOverW],ebx
			sub	ecx,32

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
Filler_TextureZFogSmoothNZW ENDP






;				*******************
;				*** Local proc ***
;				*******************





Last_POLYTEXTUREZFOGPatch Label	DWORD

_TEXT			ENDS

;			The
			End
