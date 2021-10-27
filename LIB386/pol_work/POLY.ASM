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


;CORRECT_TEXTURE	EQU	0	; IF DEFINED: Correct the mapping coord
					; (add 1/2 on u and v)

;FORCE_FPU		EQU	0
;FORCE_INT		EQU	0


;			**************
;			*** PUBLIC ***
;			**************

;					******************
;					*** PROCEDURES ***
;					******************
PUBLIC	SYSCALL		Fill_Poly		; Polygon filler (Variable Pointer)
PUBLIC	SYSCALL		Fill_PolyFast		; Polygon filler (Direct call without EMMS)
PUBLIC	SYSCALL		SetScreenPitch		; Change the pitch of the screen
PUBLIC	SYSCALL		SetFog			; Initialize the fog
PUBLIC	SYSCALL		SetCLUT			; Set the CLUT pointer
PUBLIC	SYSCALL		Switch_Fillers		; To change the filling method


PUBLIC	SYSCALL		Fill_PolyClip
PUBLIC	SYSCALL		Fill_ClipXMin		; Clipping procedures
PUBLIC	SYSCALL		Fill_ClipXMax
PUBLIC	SYSCALL		Fill_ClipYMin
PUBLIC	SYSCALL		Fill_ClipYMax

PUBLIC	SYSCALL		Triangle_ReadNextEdge	; Entry point that reads the next point in the triangle



;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***
PUBLIC	SYSCALL		ScreenPitch		; Screen width in bytes
PUBLIC	SYSCALL		PTR_TabOffLine		; Ptr on a LUT containing a list of line-offsets
PUBLIC	SYSCALL		PtrZBuffer		; ZBuffer pointer
PUBLIC	SYSCALL		PtrCLUTGouraud		; Pointer to a Gouraud CLUT
PUBLIC	SYSCALL		PtrCLUTFog		; Pointer to a Fog CLUT
PUBLIC	SYSCALL		PtrTruePal		; Pointer to the palette CLUT
PUBLIC	SYSCALL		RepMask			; Repeat zone for texture mapping
PUBLIC	SYSCALL		PtrMap			; Pointer of the texture map
PUBLIC	SYSCALL		Fill_Z_Fog_Near		; Near Fog
PUBLIC	SYSCALL		Fill_Z_Fog_Far		; Far Fog
PUBLIC	SYSCALL		Fill_ZBuffer_Factor	; ZBuffer factor
PUBLIC	SYSCALL		Fill_Fog_Factor		; Fog factor
PUBLIC	SYSCALL		Fill_ScaledFogNear	; Scaled FogNear value
PUBLIC	SYSCALL		IsPolygonHidden		; Flag for Poly type 25


;					*** MODULE DATA ***
PUBLIC	SYSCALL		Fill_Restart		; Return entry point from clipping proc
PUBLIC	SYSCALL		Fill_Filler		; Chooser filler
PUBLIC	SYSCALL		Fill_Patch		; Flag 1 means we should patch the filler proc; 0 means nothing to do
PUBLIC	SYSCALL		Fill_Color		; The polygon color
PUBLIC	SYSCALL		Fill_Trame_Parity	; Trame parity for Filler_Trame

PUBLIC	SYSCALL		Fill_FirstPoint		; Pointer of the first point of the list
PUBLIC	SYSCALL		Fill_LastPoint		; Pointer of the last point of the list
PUBLIC	SYSCALL		Fill_LeftPoint		; Pointer of the current point of the left edge
PUBLIC	SYSCALL		Fill_RightPoint		; Pointer of the current point of the right edge

PUBLIC	SYSCALL		Fill_LeftSlope		; Left slope
PUBLIC	SYSCALL		Fill_RightSlope		; Right slope

PUBLIC	SYSCALL		Fill_CurY		; Current Y
PUBLIC	SYSCALL		Fill_CurXMin		; Current XMin
PUBLIC	SYSCALL		Fill_CurXMax		; Current XMax
PUBLIC	SYSCALL		Fill_CurOffLine		; Offset of the current video line

PUBLIC	SYSCALL		Fill_CurGouraudMin	; Left edge: gouraud value
PUBLIC	SYSCALL		Fill_Gouraud_LeftSlope	; Left slope for Gouraud shading
PUBLIC	SYSCALL		Fill_Gouraud_XSlope	; X Slope for Gouraud shading

PUBLIC	SYSCALL		Fill_CurMapUMin		; Left edge: MapU value
PUBLIC	SYSCALL		Fill_MapU_LeftSlope	; Left slope for MapU
PUBLIC	SYSCALL		Fill_MapU_XSlope	; X Slope for MapU

PUBLIC	SYSCALL		Fill_CurMapVMin		; Left edge: MapV value
PUBLIC	SYSCALL		Fill_MapV_LeftSlope	; Left slope for MapV
PUBLIC	SYSCALL		Fill_MapV_XSlope	; X Slope for MapV

PUBLIC	SYSCALL		Fill_CurWMin		; Left edge: W value
PUBLIC	SYSCALL		Fill_W_LeftSlope	; Left slope for W
PUBLIC	SYSCALL		Fill_W_XSlope		; X Slope for W

PUBLIC	SYSCALL		Fill_CurZBufMin		; Left edge: ZBuf value
PUBLIC	SYSCALL		Fill_ZBuf_LeftSlope	; Left slope for ZBuf
PUBLIC	SYSCALL		Fill_ZBuf_XSlope	; X Slope for ZBuf
PUBLIC	SYSCALL		Fill_CurZBuf		; Current value of ZBuf in the scanline

PUBLIC	SYSCALL		Fill_Cur_W
PUBLIC	SYSCALL		Fill_Next_W
PUBLIC	SYSCALL		Fill_Cur_MapU
PUBLIC	SYSCALL		Fill_Next_MapU
PUBLIC	SYSCALL		Fill_Cur_MapV
PUBLIC	SYSCALL		Fill_Next_MapV
PUBLIC	SYSCALL		Fill_Cur_MapUOverW
PUBLIC	SYSCALL		Fill_Next_MapUOverW
PUBLIC	SYSCALL		Fill_Cur_MapVOverW
PUBLIC	SYSCALL		Fill_Next_MapVOverW


PUBLIC	SYSCALL		Fill_ReadFlag		; Flag telling which point to read
PUBLIC	SYSCALL		Fill_ClipFlag		; Flag telling us what to clip (ie: edges, gouraud...)
PUBLIC	SYSCALL		Fill_Flag_Fog		; True if FOG is active
PUBLIC	SYSCALL		Fill_Flag_ZBuffer	; True if ZBUFFER is active
PUBLIC	SYSCALL		Fill_Flag_NZW		; True if NZW is active

PUBLIC	SYSCALL		Fill_Logical_Palette	; Logical palette

PUBLIC	SYSCALL		F_1			; 1 in float
PUBLIC	SYSCALL		F_256			; 256 in float
PUBLIC	SYSCALL		Fill_UseFPU		; Flag that tells us weither we should use the FPU or not

PUBLIC	SYSCALL		Status_Int		; FPU control word for CHOP rounding mode
PUBLIC	SYSCALL		Status_Float		; FPU control word for nearest rounding mode




;			***************
;			*** INCLUDE ***
;			***************

INCLUDE			FILLER.INC
INCLUDE			IAMMX.INC		; For MMx instr.


;			***************
;			*** EQUATES ***
;			***************

CLIP_XMIN		EQU	0
CLIP_XMAX		EQU	1
CLIP_YMIN		EQU	2
CLIP_YMAX		EQU	3



;			**************
;			*** MACROS ***
;			**************

; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Clipping : Generic macro                                                  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Call : [Fill_FirstPoint] = Ptr on the beginning of the list               บ
; บ        [Fill_LastPoint]  = Ptr on the end of the list                     บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Return: ESI = Ptr on the new points-list                                  บ
; บ         ECX = Nb of points in that new list                               บ
; บ                                                                           บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
M_FILL_CLIP		MACRO	Type

			mov	esi,[Fill_FirstPoint]	; ESI = Current point
	IF Type EQ CLIP_XMIN
			mov	edi,Offset List_Clip_XMin
	ELSEIF Type EQ CLIP_XMAX
			mov	edi,Offset List_Clip_XMax
	ELSEIF Type EQ CLIP_YMIN
			mov	edi,Offset List_Clip_YMin
	ELSEIF Type EQ CLIP_YMAX
			mov	edi,Offset List_Clip_YMax
	ENDIF

			mov	ebx,[Fill_LastPoint]	; EBX = Last point
			xor	ecx,ecx			; ECX = Nb of points in the new poly

			sub	esi,Size STRUC_POINT
							; ebp = Clipping axis
	IF Type EQ CLIP_XMIN
			mov	ebp,[ClipXMin]
	ELSEIF Type EQ CLIP_XMAX
			mov	ebp,[ClipXMax]
			inc	ebp
	ELSEIF Type EQ CLIP_YMIN
			mov	ebp,[ClipYMin]
	ELSEIF Type EQ CLIP_YMAX
			mov	ebp,[ClipYMax]
	ENDIF

			shl	ebp,16
			mov	eax,dword ptr [ebx].Pt_XE

	IF Type EQ CLIP_XMIN
			shl	eax,16
			mov	edx,1			; edx = 1 if point non-visible
			cmp	eax,ebp
			jl	@@Jump_First
	ELSEIF Type EQ CLIP_XMAX
			shl	eax,16
			mov	edx,1			; edx = 1 if point non-visible
			cmp	eax,ebp
			jg	@@Jump_First
	ELSEIF Type EQ CLIP_YMIN
			and	eax,(65535 shl 16)	; MASM BUG (65535 shl 16) instead of 0FFFF0000h
			mov	edx,1			; edx = 1 if point non-visible
			cmp	eax,ebp
			jl	@@Jump_First
	ELSEIF Type EQ CLIP_YMAX
			and	eax,(65535 shl 16)
			mov	edx,1			; edx = 1 if point non-visible
			cmp	eax,ebp
			jg	@@Jump_First
	ENDIF
			xor	edx,edx			; edx = 0 if visible
			jmp	@@Jump_First



			; *****************
			; *** Main loop ***
			; *****************
			ALIGN	4
@@Test_Next_Pt:						; Check next point
			mov	ebx,esi			; EBX = Old point
@@Jump_First:		mov	eax,[Fill_LastPoint]

			cmp	esi,eax
			je	@@End

			mov	eax,dword ptr [esi+Size STRUC_POINT].Pt_XE
			add	esi,Size STRUC_POINT	; ESI = Current point

	IF Type EQ CLIP_XMIN
			shl	eax,16
			cmp	eax,ebp
			jge	@@Visible
	ELSEIF Type EQ CLIP_XMAX
			shl	eax,16
			cmp	eax,ebp
			jle	@@Visible
	ELSEIF Type EQ CLIP_YMIN
			and	eax,(65535 SHL 16)	; 0FFFF0000h
			cmp	eax,ebp
			jge	@@Visible
	ELSEIF Type EQ CLIP_YMAX
			and	eax,(65535 SHL 16)	; 0FFFF0000h
			cmp	eax,ebp
			jle	@@Visible
	ENDIF

			; ***********************************
			; *** Current point is off-screen ***
			; ***********************************
			test	edx,edx			; Was last point visible ?
			jne	@@Test_Next_Pt

			call	[Clipper_Jmp+Type*4]

			add	edi,Size STRUC_POINT	; A point was created: registers it!
			inc	ecx
			mov	edx,1			; Last point not visible
			jmp	@@Test_Next_Pt

			ALIGN	4
@@Visible:
			test	edx,edx
			je	@@Pas_Inters

			call	[Clipper_Jmp+Type*4]

			add	edi,Size STRUC_POINT
			inc	ecx
@@Pas_Inters:
			mov	eax,dword ptr [esi].Pt_XE	; Visible
			mov	edx,dword ptr [esi].Pt_Light

			mov	dword ptr [edi].Pt_XE,eax
			mov	dword ptr [edi].Pt_Light,edx

			mov	eax,dword ptr [esi].Pt_MapU
			mov	edx,[esi].Pt_W

			mov	dword ptr [edi].Pt_MapU,eax
			mov	[edi].Pt_W,edx

			inc	ecx
			add	edi,Size STRUC_POINT

			xor	edx,edx
			jmp	@@Test_Next_Pt

			ALIGN	4
@@End:
	IF Type EQ CLIP_XMIN
			mov	esi,Offset List_Clip_XMin
	ELSEIF Type EQ CLIP_XMAX
			mov	esi,Offset List_Clip_XMax
	ELSEIF Type EQ CLIP_YMIN
			mov	esi,Offset List_Clip_YMin
	ELSEIF Type EQ CLIP_YMAX
			mov	esi,Offset List_Clip_YMax
	ENDIF
			jmp	[Fill_Restart]
			ENDM












;			**************************
;			*** SEGMENTATION MODEL ***
;			**************************

			.386
			.387
;			.MODEL	FLAT, SYSCALL



;			************
;			*** DATA ***
;			************
;			.DATA
;FLAT			GROUP	_DATA, _TEXT
_DATA			SEGMENT	USE32 PUBLIC PARA 'DATA'

;				******************
;				*** Extrn data ***
;				******************
Extrn	SYSCALL		ProcessorSignature	:	DWORD


Extrn	SYSCALL		TabOffLine		:	DWORD
Extrn	SYSCALL		Fill_N_Table_Jumps	:	DWORD
Extrn	SYSCALL		Fill_Fog_Table_Jumps	:	DWORD
Extrn	SYSCALL		Fill_ZBuf_Table_Jumps	:	DWORD
Extrn	SYSCALL		Fill_FogZBuf_Table_Jumps:	DWORD
Extrn	SYSCALL		Fill_NZW_Table_Jumps	:	DWORD
Extrn	SYSCALL		Fill_FogNZW_Table_Jumps	:	DWORD

Extrn	SYSCALL		ClipXMin		:	DWORD
Extrn	SYSCALL		ClipXMax		:	DWORD
Extrn	SYSCALL		ClipYMin		:	DWORD
Extrn	SYSCALL		ClipYMax		:	DWORD
Extrn	SYSCALL		Log			:	DWORD


Extrn	SYSCALL		Fill_VLDisc1		:	DWORD
Extrn	SYSCALL		Fill_VLDisc2		:	DWORD
Extrn	SYSCALL		Fill_VLDisc3		:	DWORD
Extrn	SYSCALL		Fill_VLDisc4		:	DWORD

Extrn	SYSCALL		Fill_VLFlat1		:	DWORD
Extrn	SYSCALL		Fill_VLFlat2		:	DWORD
Extrn	SYSCALL		Fill_VLFlat3		:	DWORD
Extrn	SYSCALL		Fill_VLFlat4		:	DWORD
Extrn	SYSCALL		Fill_VLFlat5		:	DWORD
Extrn	SYSCALL		Fill_VLFlat6		:	DWORD
Extrn	SYSCALL		Fill_VLFlat7		:	DWORD
Extrn	SYSCALL		Fill_VLFlat8		:	DWORD
Extrn	SYSCALL		Fill_VLFlat9		:	DWORD
Extrn	SYSCALL		Fill_VLFlat10		:	DWORD

Extrn	SYSCALL		Fill_VLGouraud1		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud2		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud3		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud4		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud5		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud6		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud7		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud8		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud9		:	DWORD
Extrn	SYSCALL		Fill_VLGouraud10	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud11	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud12	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud13	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud14	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud15	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud16	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud17	:	DWORD
Extrn	SYSCALL		Fill_VLGouraud18	:	DWORD

Extrn	SYSCALL		Fill_VLTexture1		:	DWORD
Extrn	SYSCALL		Fill_VLTexture2		:	DWORD
Extrn	SYSCALL		Fill_VLTexture3		:	DWORD
Extrn	SYSCALL		Fill_VLTexture4		:	DWORD
Extrn	SYSCALL		Fill_VLTexture5		:	DWORD
Extrn	SYSCALL		Fill_VLTexture6		:	DWORD
Extrn	SYSCALL		Fill_VLTexture7		:	DWORD
Extrn	SYSCALL		Fill_VLTexture8		:	DWORD
Extrn	SYSCALL		Fill_VLTexture9		:	DWORD
Extrn	SYSCALL		Fill_VLTexture10	:	DWORD
Extrn	SYSCALL		Fill_VLTexture11	:	DWORD
Extrn	SYSCALL		Fill_VLTexture12	:	DWORD
Extrn	SYSCALL		Fill_VLTexture13	:	DWORD
Extrn	SYSCALL		Fill_VLTexture14	:	DWORD
Extrn	SYSCALL		Fill_VLTexture15	:	DWORD
Extrn	SYSCALL		Fill_VLTexture16	:	DWORD
Extrn	SYSCALL		Fill_VLTexture17	:	DWORD
Extrn	SYSCALL		Fill_VLTexture18	:	DWORD

Extrn	SYSCALL		Fill_VLGouraudTexture1	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture2	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture3	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture4	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture5	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture6	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture7	:	DWORD
Extrn	SYSCALL		Fill_VLGouraudTexture8	:	DWORD

Extrn	SYSCALL		Fill_VLTextureZ1	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ2	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ3	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ4	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ5	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ6	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ7	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ8	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ9	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ10	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ11	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ12	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ13	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ14	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ15	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ16	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ17	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZ18	:	DWORD


Extrn	SYSCALL		Fill_VLTextureZGouraud1	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZGouraud2	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZGouraud3	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZGouraud4	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZGouraud5	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZGouraud6	:	DWORD

Extrn	SYSCALL		Fill_VLTextureZFog1	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZFog2	:	DWORD
Extrn	SYSCALL		Fill_VLTextureZFog3	:	DWORD


				; Chooser.asm
Extrn	SYSCALL		Chooser_RtnI	:	DWORD
Extrn	SYSCALL		Chooser_RtnF	:	DWORD
Extrn	SYSCALL		Chooser_RtnM	:	DWORD
Extrn	SYSCALL		Chooser_Result	:	DWORD





;				*******************
;				*** Global data ***
;				*******************

;					*******************
;					*** PUBLIC data ***
;					*******************
			ALIGN	4
Fill_Poly		dd	Fill_PolyInit	; Fill_Poly pointer
PtrZBuffer		dd	0		; ZBuffer pointer
ScreenPitch		dd	0		; Screen width in bytes
PTR_TabOffLine		dd	0		; Ptr on a LUT containing a list of line-offsets
PtrCLUTGouraud		dd	0		; Pointer to a Gouraud CLUT
PtrCLUTFog		dd	0		; Pointer to a Fog CLUT
PtrTruePal		dd	0		; Pointer of the palette CLUT for normal lighting
RepMask			dd	0		; Repeat zone for texture mapping
PtrMap			dd	0		; Pointer of the texture map
Fill_Z_Fog_Near		dd	0		; Near Fog
Fill_Z_Fog_Far		dd	0		; Far Fog
Fill_ZBuffer_Factor	dd	0		; ZBuffer factor
Fill_Fog_Factor		dd	0		; Fog factor
Fill_ScaledFogNear	dd	0		; Scaled Near Fog value
IsPolygonHidden		dd	0		; Flag set after poly type 25


;					*******************
;					*** MODULE data ***
;					*******************
			ALIGN	4
F_65536			REAL4	65536.0		; 65536 in float
F_256			REAL4	256.0		; 256 in float
F_1			REAL4	1.0		; 1 in float
FInv_65536		REAL4	0.0000152588	; 1/65536 in float
FInv_256		REAL4	0.00390625	; 1/256 in float
FInv_2			REAL4	0.5		; 0.5 in float



Fill_Restart		dd	0		; Return entry point from clipping procedure
Fill_Filler		dd	0		; Choosen filler procedure
Fill_Patch		dd	0		; Flag 1 means we should patch the filler proc; 0 means nothing to do
Fill_Color		dd	0		; The polygon color
Fill_Trame_Parity	dd	0		; Trame parity for Filler_Trame
Fill_Type		dd	0		; Polygon type number

Fill_FirstPoint		dd	0		; Pointer to the first point of the list
Fill_LastPoint		dd	0		; Pointer to the last point of the list
Fill_LeftPoint		dd	0		; Pointer of the current point of the left edge
Fill_RightPoint		dd	0		; Pointer of the current point of the right edge

Fill_LeftSlope		dd	0		; Left slope
Fill_RightSlope		dd	0		; Right slope

Fill_Gouraud_LeftSlope	dd	0		; Left slope for Gouraud shading
Fill_Gouraud_XSlope	dd	0		; X Slope for Gouraud shading
Fill_CurGouraudMin	dd	0		; Current value of Gouraud on the left edge

Fill_MapU_LeftSlope	dd	0		; Left slope for MapU
Fill_MapU_XSlope	dd	0		; X Slope for MapU
Fill_CurMapUMin		dd	0		; Current value of MapU on the left edge

Fill_MapV_LeftSlope	dd	0		; Left slope for MapV
Fill_MapV_XSlope	dd	0		; X Slope for MapV
Fill_CurMapVMin		dd	0		; Current value of MapV on the left edge

Fill_W_LeftSlope	dd	0		; Left slope for W
Fill_W_XSlope		dd	0		; X Slope for W
Fill_CurWMin		dd	0		; Current value of W on the left edge

Fill_ZBuf_LeftSlope	dd	0		; Left slope for ZBuf
Fill_ZBuf_XSlope	dd	0		; X Slope for ZBuf
Fill_CurZBufMin		dd	0		; Current value of ZBuf on the left edge
Fill_CurZBuf		dd	0		; Current value of ZBuf in the scanline

Fill_CurY		dd	0		; Current Y
Fill_CurXMin		dd	0		; Current XMin
Fill_CurXMax		dd	0		; Current XMax
Fill_CurOffLine		dd	0		; Offset of the current video line

Fill_Cur_W		dd	0		; Current W
Fill_Next_W		dd	0		; Next W
Fill_Cur_MapU 		dd	0		; all those variables for Texture Z
Fill_Next_MapU		dd	0
Fill_Cur_MapV		dd	0
Fill_Next_MapV		dd	0
Fill_Cur_MapUOverW 	dd	0
Fill_Next_MapUOverW	dd	0
Fill_Cur_MapVOverW	dd	0
Fill_Next_MapVOverW	dd	0



Fill_ReadFlag		dd	0		; Flag telling which point to read
Fill_ClipFlag		dd	0		; Flag telling us what to clip
						; See in FILLER.INC : CLIP_xxxx
Fill_Flag_Fog		db	FALSE		; True if FOG is active
Fill_Flag_ZBuffer	db	FALSE		; True if ZBUFFER is active
Fill_Flag_NZW		db	FALSE		; True if NZW active
Fill_UseFPU		db	FALSE		; True if we should use the FPU


Fill_Logical_Palette	db	256 dup (?)	; Logical palette





;				******************
;				*** Local data ***
;				******************
			ALIGN	4
Clipper_Jmp		dd	Offset Generic_ClipXMin
			dd	Offset Generic_ClipXMax
			dd	Offset Generic_ClipYMin
			dd	Offset Generic_ClipYMax

List_Clip_XMin		db	(Size STRUC_POINT)*NB_MAX_EDGES	 dup (0)
List_Clip_XMax		db	(Size STRUC_POINT)*NB_MAX_EDGES	 dup (0)
List_Clip_YMin		db	(Size STRUC_POINT)*NB_MAX_EDGES	 dup (0)
List_Clip_YMax		db	(Size STRUC_POINT)*NB_MAX_EDGES	 dup (0)

			ALIGN	4
YB_YA			dd	0
YC_YA			dd	0
Denom			dd	0
Clip_Num		dd	0


			ALIGN	4
Jmp_XSlope		dd	Triangle_ReadNextEdge	; Solid
			dd	Triangle_ReadNextEdge	; Flat
			dd	Triangle_ReadNextEdge	; Transp.
			dd	Triangle_ReadNextEdge	; Trame
			dd	Calc_GouraudXSlope	; Gouraud
			dd	Calc_DitherXSlope	; Dither
			dd	Calc_GouraudXSlope	; Gouraud table
			dd	Calc_DitherXSlope	; Dither table
			dd	Calc_TextureXSlope	; Texture Solid
			dd	Calc_TextureXSlope	; Texture Flat
			dd	Calc_TextureGouraudXSlope; Texture Gouraud
			dd	Calc_TextureDitherXSlope; Texture Gouraud Dither
			dd	Calc_TextureXSlope	; Texture Solid Chroma Key
			dd	Calc_TextureXSlope	; Texture Flat Chroma Key
			dd	Calc_TextureGouraudXSlope; Texture Gouraud Chroma Key
			dd	Calc_TextureDitherXSlope; Texture Gouraud Dither Chroma Key
			dd	Calc_TextureZXSlope	; Texture Z Solid
			dd	Calc_TextureZXSlope	; Texture Z Flat
			dd	Calc_TextureZGouraudXSlope; Texture Z Gouraud
			dd	Calc_TextureZGouraudXSlope; Texture Z Dither
			dd	Calc_TextureZXSlope	; Texture Z Solid Chroma Key
			dd	Calc_TextureZXSlope	; Texture Z Flat Chroma Key
			dd	Calc_TextureZGouraudXSlope; Texture Z Gouraud Chroma Key
			dd	Calc_TextureZGouraudXSlope; Texture Z Dither Chroma Key
			dd	Calc_TextureZXSlopeZBuf	; Texture Z Solid
			dd	Calc_XSlopeZBufferFlag	; ZBuffer only




			ALIGN	4
Jmp_LeftSlope		dd	Read_Next_Right		; Solid
			dd	Read_Next_Right		; Flat
			dd	Read_Next_Right		; Transp.
			dd	Read_Next_Right		; Trame
			dd	Calc_GouraudLeftSlope	; Gouraud
			dd	Calc_GouraudLeftSlope	; Dither
			dd	Calc_GouraudTableLeftSlope ; Gouraud table
			dd	Calc_GouraudTableLeftSlope ; Dither table
			dd	Calc_TextureLeftSlope	; Texture Solid
			dd	Calc_TextureLeftSlope	; Texture Flat
			dd	Calc_TextureGouraudLeftSlope; Texture Gouraud
			dd	Calc_TextureGouraudLeftSlope; Texture Gouraud Dither
			dd	Calc_TextureLeftSlope	; Texture Solid Chroma Key
			dd	Calc_TextureLeftSlope	; Texture Flat Chroma Key
			dd	Calc_TextureGouraudLeftSlope; Texture Gouraud Chroma Key
			dd	Calc_TextureGouraudLeftSlope; Texture Gouraud Dither Chroma Key
			dd	Calc_TextureZLeftSlope	; Texture Z Solid
			dd	Calc_TextureZLeftSlope	; Texture Z Flat
			dd	Calc_TextureZGouraudLeftSlope; Texture Z Gouraud
			dd	Calc_TextureZGouraudLeftSlope; Texture Z Dither
			dd	Calc_TextureZLeftSlope	; Texture Z Solid Chroma Key
			dd	Calc_TextureZLeftSlope	; Texture Z Flat Chroma Key
			dd	Calc_TextureZGouraudLeftSlope; Texture Z Gouraud Chroma Key
			dd	Calc_TextureZGouraudLeftSlope; Texture Z Dither Chroma Key
			dd	Calc_TextureZLeftSlopeZBuf; Texture Z Solid
			dd	Calc_LeftSlopeZBufferFlag; ZBuffer only


			ALIGN	4
Jmp_XSlopeZBuf		dd	Calc_XSlopeZBuffer	; Solid
			dd	Calc_XSlopeZBuffer	; Flat
			dd	Calc_XSlopeZBuffer	; Transp.
			dd	Calc_XSlopeZBuffer	; Trame
			dd	Calc_XSlopeZBuffer	; Gouraud
			dd	Calc_XSlopeZBuffer	; Dither
			dd	Calc_XSlopeZBuffer	; Gouraud table
			dd	Calc_XSlopeZBuffer	; Dither table
			dd	Calc_XSlopeZBuffer	; Texture Solid
			dd	Calc_XSlopeZBuffer	; Texture Flat
			dd	Calc_XSlopeZBuffer	; Texture Gouraud
			dd	Calc_XSlopeZBuffer	; Texture Gouraud Dither
			dd	Calc_XSlopeZBuffer	; Texture Solid Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Flat Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Gouraud Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Gouraud Dither Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Z Solid
			dd	Calc_XSlopeZBuffer	; Texture Z Flat
			dd	Calc_XSlopeZBuffer	; Texture Z Gouraud
			dd	Calc_XSlopeZBuffer	; Texture Z Dither
			dd	Calc_XSlopeZBuffer	; Texture Z Solid Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Z Flat Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Z Gouraud Chroma Key
			dd	Calc_XSlopeZBuffer	; Texture Z Dither Chroma Key
			dd	Calc_TextureZXSlopeZBuf	; Texture Z Solid
			dd	Calc_XSlopeZBufferFlag	; ZBuffer only




			ALIGN	4
Jmp_LeftSlopeZBuf	dd	Calc_LeftSlopeZBuffer	; Solid
			dd	Calc_LeftSlopeZBuffer	; Flat
			dd	Calc_LeftSlopeZBuffer	; Transp.
			dd	Calc_LeftSlopeZBuffer	; Trame
			dd	Calc_LeftSlopeZBuffer	; Gouraud
			dd	Calc_LeftSlopeZBuffer	; Dither
			dd	Calc_LeftSlopeZBuffer	; Gouraud table
			dd	Calc_LeftSlopeZBuffer	; Dither table
			dd	Calc_LeftSlopeZBuffer	; Texture Solid
			dd	Calc_LeftSlopeZBuffer	; Texture Flat
			dd	Calc_LeftSlopeZBuffer	; Texture Gouraud
			dd	Calc_LeftSlopeZBuffer	; Texture Gouraud Dither
			dd	Calc_LeftSlopeZBuffer	; Texture Solid Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Flat Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Gouraud Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Gouraud Dither Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Z Solid
			dd	Calc_LeftSlopeZBuffer	; Texture Z Flat
			dd	Calc_LeftSlopeZBuffer	; Texture Z Gouraud
			dd	Calc_LeftSlopeZBuffer	; Texture Z Dither
			dd	Calc_LeftSlopeZBuffer	; Texture Z Solid Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Z Flat Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Z Gouraud Chroma Key
			dd	Calc_LeftSlopeZBuffer	; Texture Z Dither Chroma Key
			dd	Calc_TextureZLeftSlopeZBuf; Texture Z Solid
			dd	Calc_LeftSlopeZBufferFlag; ZBuffer only




			ALIGN	4
Jmp_XSlopeFPU		dd	Triangle_ReadNextEdgeFPU; Solid
			dd	Triangle_ReadNextEdgeFPU; Flat
			dd	Triangle_ReadNextEdgeFPU; Transp.
			dd	Triangle_ReadNextEdgeFPU; Trame
			dd	Calc_GouraudXSlopeFPU	; Gouraud
			dd	Calc_DitherXSlopeFPU	; Dither
			dd	Calc_GouraudXSlopeFPU	; Gouraud table
			dd	Calc_DitherXSlopeFPU	; Dither table
			dd	Calc_TextureXSlopeFPU	; Texture Solid
			dd	Calc_TextureXSlopeFPU	; Texture Flat
			dd	Calc_TextureGouraudXSlopeFPU; Texture Gouraud
			dd	Calc_TextureDitherXSlopeFPU; Texture Gouraud Dither
			dd	Calc_TextureXSlopeFPU	; Texture Solid Chroma Key
			dd	Calc_TextureXSlopeFPU	; Texture Flat Chroma Key
			dd	Calc_TextureGouraudXSlopeFPU; Texture Gouraud Chroma Key
			dd	Calc_TextureDitherXSlopeFPU; Texture Gouraud Dither Chroma Key
			dd	Calc_TextureZXSlopeFPU	; Texture Z Solid
			dd	Calc_TextureZXSlopeFPU	; Texture Z Flat
			dd	Calc_TextureZGouraudXSlopeFPU; Texture Z Gouraud
			dd	Calc_TextureZGouraudXSlopeFPU; Texture Z Dither
			dd	Calc_TextureZXSlopeFPU	; Texture Z Solid Chroma Key
			dd	Calc_TextureZXSlopeFPU	; Texture Z Flat Chroma Key
			dd	Calc_TextureZGouraudXSlopeFPU; Texture Z Gouraud Chroma Key
			dd	Calc_TextureZGouraudXSlopeFPU; Texture Z Dither Chroma Key
			dd	Calc_TextureZXSlopeZBufFPU; Texture Z Solid
			dd	Calc_XSlopeZBufferFlagFPU; ZBuffer only




			ALIGN	4
Jmp_LeftSlopeFPU	dd	Read_Next_RightFPU	; Solid
			dd	Read_Next_RightFPU	; Flat
			dd	Read_Next_RightFPU	; Transp.
			dd	Read_Next_RightFPU	; Trame
			dd	Calc_GouraudLeftSlopeFPU; Gouraud
			dd	Calc_GouraudLeftSlopeFPU; Dither
			dd	Calc_GouraudTableLeftSlopeFPU; Gouraud table
			dd	Calc_GouraudTableLeftSlopeFPU; Dither table
			dd	Calc_TextureLeftSlopeFPU; Texture Solid
			dd	Calc_TextureLeftSlopeFPU; Texture Flat
			dd	Calc_TextureGouraudLeftSlopeFPU; Texture Gouraud
			dd	Calc_TextureGouraudLeftSlopeFPU; Texture Gouraud Dither
			dd	Calc_TextureLeftSlopeFPU; Texture Solid Chroma Key
			dd	Calc_TextureLeftSlopeFPU; Texture Flat Chroma Key
			dd	Calc_TextureGouraudLeftSlopeFPU; Texture Gouraud Chroma Key
			dd	Calc_TextureGouraudLeftSlopeFPU; Texture Gouraud Dither Chroma Key
			dd	Calc_TextureZLeftSlopeFPU; Texture Z Solid
			dd	Calc_TextureZLeftSlopeFPU; Texture Z Flat
			dd	Calc_TextureZGouraudLeftSlopeFPU; Texture Z Gouraud
			dd	Calc_TextureZGouraudLeftSlopeFPU; Texture Z Dither
			dd	Calc_TextureZLeftSlopeFPU; Texture Z Solid Chroma Key
			dd	Calc_TextureZLeftSlopeFPU; Texture Z Flat Chroma Key
			dd	Calc_TextureZGouraudLeftSlopeFPU; Texture Z Gouraud Chroma Key
			dd	Calc_TextureZGouraudLeftSlopeFPU; Texture Z Dither Chroma Key
			dd	Calc_TextureZLeftSlopeZBufFPU; Texture Z Solid
			dd	Calc_LeftSlopeZBufferFlagFPU; ZBuffer only




			ALIGN	4
Jmp_XSlopeZBufFPU	dd	Calc_XSlopeZBufferFPU	; Solid
			dd	Calc_XSlopeZBufferFPU	; Flat
			dd	Calc_XSlopeZBufferFPU	; Transp.
			dd	Calc_XSlopeZBufferFPU	; Trame
			dd	Calc_XSlopeZBufferFPU	; Gouraud
			dd	Calc_XSlopeZBufferFPU	; Dither
			dd	Calc_XSlopeZBufferFPU	; Gouraud table
			dd	Calc_XSlopeZBufferFPU	; Dither table
			dd	Calc_XSlopeZBufferFPU	; Texture Solid
			dd	Calc_XSlopeZBufferFPU	; Texture Flat
			dd	Calc_XSlopeZBufferFPU	; Texture Gouraud
			dd	Calc_XSlopeZBufferFPU	; Texture Gouraud Dither
			dd	Calc_XSlopeZBufferFPU	; Texture Solid Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Flat Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Gouraud Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Gouraud Dither Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Z Solid
			dd	Calc_XSlopeZBufferFPU	; Texture Z Flat
			dd	Calc_XSlopeZBufferFPU	; Texture Z Gouraud
			dd	Calc_XSlopeZBufferFPU	; Texture Z Dither
			dd	Calc_XSlopeZBufferFPU	; Texture Z Solid Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Z Flat Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Z Gouraud Chroma Key
			dd	Calc_XSlopeZBufferFPU	; Texture Z Dither Chroma Key
			dd	Calc_TextureZXSlopeZBufFPU; Texture Z Solid
			dd	Calc_XSlopeZBufferFlagFPU; ZBuffer only





			ALIGN	4
Jmp_LeftSlopeZBufFPU	dd	Calc_LeftSlopeZBufferFPU; Solid
			dd	Calc_LeftSlopeZBufferFPU; Flat
			dd	Calc_LeftSlopeZBufferFPU; Transp.
			dd	Calc_LeftSlopeZBufferFPU; Trame
			dd	Calc_LeftSlopeZBufferFPU; Gouraud
			dd	Calc_LeftSlopeZBufferFPU; Dither
			dd	Calc_LeftSlopeZBufferFPU; Gouraud table
			dd	Calc_LeftSlopeZBufferFPU; Dither table
			dd	Calc_LeftSlopeZBufferFPU; Texture Solid
			dd	Calc_LeftSlopeZBufferFPU; Texture Flat
			dd	Calc_LeftSlopeZBufferFPU; Texture Gouraud
			dd	Calc_LeftSlopeZBufferFPU; Texture Gouraud Dither
			dd	Calc_LeftSlopeZBufferFPU; Texture Solid Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Flat Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Gouraud Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Gouraud Dither Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Solid
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Flat
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Gouraud
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Dither
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Solid Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Flat Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Gouraud Chroma Key
			dd	Calc_LeftSlopeZBufferFPU; Texture Z Dither Chroma Key
			dd	Calc_TextureZLeftSlopeZBufFPU; Texture Z Solid
			dd	Calc_LeftSlopeZBufferFlagFPU; ZBuffer only






			ALIGN	4
Table_Switch		dd	Switch_Normal
			dd	Switch_Normal
			dd	Switch_Fog
			dd	Switch_Normal
			dd	Switch_ZBuffer
			dd	Switch_FogZBuffer
			dd	Switch_NZW
			dd	Switch_FogNZW



Status_Int		dw	111101111111b
Status_Float		dw	001101111111b


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
IFDEF	_WIN32
Extrn	SYSCALL		AllowPatch		:	PROC
Extrn	SYSCALL		AllowPatchAndPatch	:	PROC
ENDIF	; _WIN32


					; *** POLYLINE.ASM ***
Extrn	SYSCALL		First_POLYLINEPatch	:	DWORD
Extrn	SYSCALL		Last_POLYLINEPatch	:	DWORD

					; *** POLYDISC.ASM ***
Extrn	SYSCALL		First_POLYDISCPatch	:	DWORD
Extrn	SYSCALL		Last_POLYDISCPatch	:	DWORD

					; *** POLYFLAT.ASM ***
Extrn	SYSCALL		First_POLYFLATPatch	:	DWORD
Extrn	SYSCALL		Last_POLYFLATPatch	:	DWORD

					; *** POLYGOUR.ASM ***
Extrn	SYSCALL		First_POLYGOURAUDPatch	:	DWORD
Extrn	SYSCALL		Last_POLYGOURAUDPatch	:	DWORD

					; *** POLYTEXT.ASM ***
Extrn	SYSCALL		First_POLYTEXTUREPatch	:	DWORD
Extrn	SYSCALL		Last_POLYTEXTUREPatch	:	DWORD

					; *** POLYGTEX.ASM ***
Extrn	SYSCALL		First_POLYGOURAUDTEXTUREPatch:	DWORD
Extrn	SYSCALL		Last_POLYGOURAUDTEXTUREPatch :	DWORD

					; *** POLYTEXZ.ASM ***
Extrn	SYSCALL		First_POLYTEXTUREZPatch	:	DWORD
Extrn	SYSCALL		Last_POLYTEXTUREZPatch	:	DWORD

					; *** POLYTZG.ASM ***
Extrn	SYSCALL		First_POLYTEXTUREZGOURAUDPatch:	DWORD
Extrn	SYSCALL		Last_POLYTEXTUREZGOURAUDPatch:	DWORD

					; *** POLYTZF.ASM ***
Extrn	SYSCALL		First_POLYTEXTUREZFOGPatch:	DWORD
Extrn	SYSCALL		Last_POLYTEXTUREZFOGPatch:	DWORD


					; *** Chooser.asm ***
Extrn	SYSCALL		Chooser			:	PROC




;				*******************
;				*** Global proc ***
;				*******************




Fill_PolyInit		PROC
			mov	[Chooser_RtnI], Offset Fill_PolyFast	; Integer
			mov	[Chooser_RtnM], Offset Fill_PolyMMX	; MMX
			mov	[Chooser_Result], Offset Fill_Poly	; Pointer
			jmp	Chooser
Fill_PolyInit		ENDP









; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Fill_Poly : Polygon filler                                               บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Call  : EAX = Polygon type (ie flat, gouraud, textured...)               บ
; บ         EBX = Polygon color (if needed by the type)                      บ
; บ         ECX = Nb of points in that polygon                               บ
; บ         ESI = Ptr on the point list of this polygon                      บ
; บ               (list of STRUC_POLY_POINT)                                 บ
; บ                                                                          บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Return: EAX = 0 if error (polygon not filled)                            บ
; บ                                                                          บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ ALL REGISTERS ARE MODIFIED (EXCEPT EBP)                                  บ
; บ                                                                          บ
; บ The value given to EAX is not check for validity, so be sure to fill it  บ
; บ with a correct poly type number                                          บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4
Fill_PolyMMX		PROC
			emms
Fill_PolyFast::
First_FillerPatch	Label	DWORD
			; ****** First_FillerPatch MUST STAY HERE ************

			push	ebp			; ** THERE MUST BE AT LEAST
			xor	ebp,ebp			; ** 5 BYTES B4 THE
			mov	[Fill_LeftSlope],ebp	; ** PATCH CODE
IFDEF	_WIN32
x:
			org	Fill_PolyFast
			jmp	NEAR PTR InitFillPoly_WIN32
			org	x
ELSE	; _WIN32
x:
			org	Fill_PolyFast
			jmp	NEAR PTR InitFillPoly_DOS
			org	x
ENDIF	; _WIN32

			mov	[Fill_Type],eax
			and	ebx,0FFh

Fill_Saut_Normal::	jmp	[Fill_N_Table_Jumps+eax*4]

Fill_PolyMMX		ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ SetScreenPitch : Setup a new pitch of the logical screen buffer                  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel: EBX = TabOffLine (new pointer)                                            บ
; บ                                                                                  บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

SetScreenPitch		PROC
			push	eax
			push	ebx
					; *** THINK B4 CHANGING THIS CODE!!!!
IFDEF	_WIN32
REPT 3
			NOP
ENDM
x:
			org	SetScreenPitch
			jmp	NEAR PTR Init_SetScreenPitch_WIN32
			org	x
ENDIF	; _WIN32
					; *** YOU CAN DO WHAT YOU WANT NOW!
			mov	eax,[ebx+4]		; Get screen-pitch

			mov	[ScreenPitch],eax
			mov	[PTR_TabOffLine],ebx


						; DISC
			mov	dword ptr [Fill_VLDisc1+3],ebx
			mov	dword ptr [Fill_VLDisc2+3],ebx
			mov	dword ptr [Fill_VLDisc3+3],ebx
			mov	dword ptr [Fill_VLDisc4+3],ebx

						; FLAT
			mov	dword ptr [Fill_VLFlat1+2],eax
			mov	dword ptr [Fill_VLFlat2+2],eax
			mov	dword ptr [Fill_VLFlat3+2],eax
			mov	dword ptr [Fill_VLFlat4+2],eax
			mov	dword ptr [Fill_VLFlat5+2],eax
			mov	dword ptr [Fill_VLFlat6+2],eax
			mov	dword ptr [Fill_VLFlat7+2],eax
			mov	dword ptr [Fill_VLFlat8+2],eax
			mov	dword ptr [Fill_VLFlat9+2],eax
			mov	dword ptr [Fill_VLFlat10+2],eax

						; GOURAUD
			mov	dword ptr [Fill_VLGouraud1+2],eax
			mov	dword ptr [Fill_VLGouraud2+2],eax
			mov	dword ptr [Fill_VLGouraud3+2],eax
			mov	dword ptr [Fill_VLGouraud4+2],eax
			mov	dword ptr [Fill_VLGouraud5+2],eax
			mov	dword ptr [Fill_VLGouraud6+2],eax
			mov	dword ptr [Fill_VLGouraud7+2],eax
			mov	dword ptr [Fill_VLGouraud8+2],eax
			mov	dword ptr [Fill_VLGouraud9+2],eax
			mov	dword ptr [Fill_VLGouraud10+2],eax
			mov	dword ptr [Fill_VLGouraud11+2],eax
			mov	dword ptr [Fill_VLGouraud12+2],eax
			mov	dword ptr [Fill_VLGouraud13+2],eax
			mov	dword ptr [Fill_VLGouraud14+2],eax
			mov	dword ptr [Fill_VLGouraud15+2],eax
			mov	dword ptr [Fill_VLGouraud16+2],eax
			mov	dword ptr [Fill_VLGouraud17+2],eax
			mov	dword ptr [Fill_VLGouraud18+2],eax


						; TEXTURE
			mov	dword ptr [Fill_VLTexture1+2],eax
			mov	dword ptr [Fill_VLTexture2+2],eax
			mov	dword ptr [Fill_VLTexture3+2],eax
			mov	dword ptr [Fill_VLTexture4+2],eax
			mov	dword ptr [Fill_VLTexture5+2],eax
			mov	dword ptr [Fill_VLTexture6+2],eax
			mov	dword ptr [Fill_VLTexture7+2],eax
			mov	dword ptr [Fill_VLTexture8+2],eax
			mov	dword ptr [Fill_VLTexture9+2],eax
			mov	dword ptr [Fill_VLTexture10+2],eax
			mov	dword ptr [Fill_VLTexture11+2],eax
			mov	dword ptr [Fill_VLTexture12+2],eax
			mov	dword ptr [Fill_VLTexture13+2],eax
			mov	dword ptr [Fill_VLTexture14+2],eax
			mov	dword ptr [Fill_VLTexture15+2],eax
			mov	dword ptr [Fill_VLTexture16+2],eax
			mov	dword ptr [Fill_VLTexture17+2],eax
			mov	dword ptr [Fill_VLTexture18+2],eax

						; GOURAUD TEXTURE
			mov	dword ptr [Fill_VLGouraudTexture1+2],eax
			mov	dword ptr [Fill_VLGouraudTexture2+2],eax
			mov	dword ptr [Fill_VLGouraudTexture3+2],eax
			mov	dword ptr [Fill_VLGouraudTexture4+2],eax
			mov	dword ptr [Fill_VLGouraudTexture5+2],eax
			mov	dword ptr [Fill_VLGouraudTexture6+2],eax
			mov	dword ptr [Fill_VLGouraudTexture7+2],eax
			mov	dword ptr [Fill_VLGouraudTexture8+2],eax

						; TEXTURE Z
			mov	dword ptr [Fill_VLTextureZ1+2],eax
			mov	dword ptr [Fill_VLTextureZ2+2],eax
			mov	dword ptr [Fill_VLTextureZ3+2],eax
			mov	dword ptr [Fill_VLTextureZ4+2],eax
			mov	dword ptr [Fill_VLTextureZ5+2],eax
			mov	dword ptr [Fill_VLTextureZ6+2],eax
			mov	dword ptr [Fill_VLTextureZ7+2],eax
			mov	dword ptr [Fill_VLTextureZ8+2],eax
			mov	dword ptr [Fill_VLTextureZ9+2],eax
			mov	dword ptr [Fill_VLTextureZ10+2],eax
			mov	dword ptr [Fill_VLTextureZ11+2],eax
			mov	dword ptr [Fill_VLTextureZ12+2],eax
			mov	dword ptr [Fill_VLTextureZ13+2],eax
			mov	dword ptr [Fill_VLTextureZ14+2],eax
			mov	dword ptr [Fill_VLTextureZ15+2],eax
			mov	dword ptr [Fill_VLTextureZ16+2],eax
			mov	dword ptr [Fill_VLTextureZ17+2],eax
			mov	dword ptr [Fill_VLTextureZ18+2],eax

						; TEXTURE Z GOURAUD
			mov	dword ptr [Fill_VLTextureZGouraud1+2],eax
			mov	dword ptr [Fill_VLTextureZGouraud2+2],eax
			mov	dword ptr [Fill_VLTextureZGouraud3+2],eax
			mov	dword ptr [Fill_VLTextureZGouraud4+2],eax
			mov	dword ptr [Fill_VLTextureZGouraud5+2],eax
			mov	dword ptr [Fill_VLTextureZGouraud6+2],eax

						; TEXTURE Z FOG
			mov	dword ptr [Fill_VLTextureZFog1+2],eax
			mov	dword ptr [Fill_VLTextureZFog2+2],eax
			mov	dword ptr [Fill_VLTextureZFog3+2],eax


						; Set the flag that tells us if we should use the FPU
			; use if FPU only if the current CPU has an FPU and this
			; one is made by Intel

			mov	ebx,[ProcessorSignature]

			shr	ebx,8

			and	ebx,0F4Fh

			cmp	bl,045h

			setae	bl			; BL = 1 if we should use FPU

			cmp	bh,1			; BH = 1 if Intel

			sete	bh

			and	bl,bh

			mov	[Fill_UseFPU],bl

IFDEF FORCE_FPU
			mov	[Fill_UseFPU],TRUE
ENDIF
IFDEF FORCE_INT
			mov	[Fill_UseFPU],FALSE
ENDIF


			pop	ebx
			pop	eax
			ret
SetScreenPitch		ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Set_Fog: Initialize the fog parameters                                           บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel: EAX = Z near (at which the fog starts)                                    บ
; บ        EBX = Z far (at which everything is under fog)                            บ
; บ                                                                                  บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

SetFog			PROC
			pushad

			test	ebx,ebx
			jne	@@Ok1
			mov	ebx,1
@@Ok1:

			mov	[Fill_Z_Fog_Near],eax
			mov	[Fill_Z_Fog_Far],ebx

			xor	eax,eax
			mov	edx,1
			div	ebx
			mov	[Fill_ZBuffer_Factor],eax	; Pt_ZO = (Z*Fill_ZBuffer_Factor) SHR 16


			imul	eax,[Fill_Z_Fog_Near]
			shr	eax,16
			mov	[Fill_ScaledFogNear],eax


			mov	ebx,65535
			sub	ebx,eax
			test	ebx,ebx
			jne	@@Ok2
			mov	ebx,1
@@Ok2:
			mov	eax,256*65536
			xor	edx,edx
			div	ebx
			mov	[Fill_Fog_Factor],eax		; Fog value= 0 if Zbuf<FogNear
								;	   = (Zbuf-Near)*FogFactor SHR 20 if ZBuf>FogNear
			popad
			ret
SetFog			ENDP



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ SetCLUT: Initialize some CLUT pointers                                           บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel: EAX = Line number in PtrCLUTFog that represents the palette CLUT.         บ
; บ                                                                                  บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

SetCLUT			PROC
			push	eax
			push	ebx
			push	ecx
			push	esi
			push	edi

			shl	eax,8
			mov	ecx,[PtrCLUTFog]

			mov	ebx,eax
			add	eax,ecx

			mov	ecx,[PtrTruePal]
			and	ebx,0F000h

			cmp	eax,ecx
			je	@@End

			mov	[PtrTruePal],eax
			mov	ecx,[PtrCLUTFog]

			add	ebx,ecx
			mov	esi,eax

			mov	[PtrCLUTGouraud],ebx
			mov	edi,Offset Fill_Logical_Palette

			mov	ecx,256/8
@@Copy:
			mov	eax,[esi]
			mov	ebx,[esi+4]
			mov	[edi],eax
			mov	[edi+4],ebx
			add	esi,8
			add	edi,8
			dec	ecx
			jne	@@Copy
@@End:
			pop	edi
			pop	esi
			pop	ecx
			pop	ebx
			pop	eax
			ret
SetCLUT			ENDP



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Switch_Fillers                                                                   บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ                                                                                  บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Switch_Fillers		PROC
			push	eax
			push	ebx
			push	ecx
			push	edx

					; *** THINK B4 CHANGING THIS CODE!!!!
IFDEF	_WIN32
			nop
x:
			org	Switch_Fillers
			jmp	NEAR PTR Init_SwitchFillers_WIN32
			org	x
ENDIF	; _WIN32
					; *** YOU CAN DO WHAT YOU WANT NOW!
			jmp	[Table_Switch+eax*4]

Switch_Normal::
			mov	eax,Offset Fill_N_Table_Jumps
			mov	bl,FALSE

			mov	dword ptr [Fill_Saut_Normal+3],eax
			mov	[Fill_Flag_Fog],bl

			mov	[Fill_Flag_ZBuffer],bl
			mov	eax,Offset Jmp_XSlope

			mov	[Fill_Flag_NZW],bl
			mov	ebx,Offset Jmp_LeftSlope

			mov	ecx,Offset Jmp_XSlopeFPU
			mov	edx,Offset Jmp_LeftSlopeFPU

			jmp	@@End

Switch_Fog::
			mov	eax,Offset Fill_Fog_Table_Jumps
			mov	bl,FALSE

			mov	dword ptr [Fill_Saut_Normal+3],eax
			mov	al,TRUE

			mov	[Fill_Flag_ZBuffer],bl
			mov	[Fill_Flag_NZW],bl

			mov	[Fill_Flag_Fog],al
			mov	eax,Offset Jmp_XSlope

			mov	ebx,Offset Jmp_LeftSlope
			mov	ecx,Offset Jmp_XSlopeFPU

			mov	edx,Offset Jmp_LeftSlopeFPU
			jmp	@@End

Switch_ZBuffer::
			mov	eax,Offset Fill_ZBuf_Table_Jumps
			mov	bl,FALSE

			mov	dword ptr [Fill_Saut_Normal+3],eax
			mov	al,TRUE

			mov	[Fill_Flag_Fog],bl
			mov	[Fill_Flag_NZW],bl

			mov	[Fill_Flag_ZBuffer],al
			mov	eax,Offset Jmp_XSlopeZBuf

			mov	ebx,Offset Jmp_LeftSlopeZBuf
			mov	ecx,Offset Jmp_XSlopeZBufFPU

			mov	edx,Offset Jmp_LeftSlopeZBufFPU
			jmp	@@End

Switch_FogZBuffer::
			mov	eax,Offset Fill_FogZBuf_Table_Jumps
			mov	bl,TRUE

			mov	dword ptr [Fill_Saut_Normal+3],eax
			mov	al,FALSE

			mov	[Fill_Flag_Fog],bl
			mov	[Fill_Flag_NZW],al

			mov	[Fill_Flag_ZBuffer],bl
			mov	eax,Offset Jmp_XSlopeZBuf

			mov	ebx,Offset Jmp_LeftSlopeZBuf
			mov	ecx,Offset Jmp_XSlopeZBufFPU

			mov	edx,Offset Jmp_LeftSlopeZBufFPU
			jmp	@@End

Switch_NZW::
			mov	eax,Offset Fill_NZW_Table_Jumps
			mov	bl,FALSE

			mov	dword ptr [Fill_Saut_Normal+3],eax
			mov	al,TRUE

			mov	[Fill_Flag_Fog],bl
			mov	[Fill_Flag_NZW],al

			mov	[Fill_Flag_ZBuffer],al
			mov	eax,Offset Jmp_XSlopeZBuf

			mov	ebx,Offset Jmp_LeftSlopeZBuf
			mov	ecx,Offset Jmp_XSlopeZBufFPU

			mov	edx,Offset Jmp_LeftSlopeZBufFPU
			jmp	@@End

Switch_FogNZW::
			mov	eax,Offset Fill_FogNZW_Table_Jumps
			mov	bl,TRUE

			mov	dword ptr [Fill_Saut_Normal+3],eax
			mov	[Fill_Flag_Fog],bl

			mov	[Fill_Flag_ZBuffer],bl
			mov	[Fill_Flag_NZW],bl

			mov	eax,Offset Jmp_XSlopeZBuf
			mov	ebx,Offset Jmp_LeftSlopeZBuf

			mov	ecx,Offset Jmp_XSlopeZBufFPU
			mov	edx,Offset Jmp_LeftSlopeZBufFPU

			jmp	@@End

@@End:
			cmp	[Fill_UseFPU],TRUE
			jne	@@NoZBuf

			mov	dword ptr [Patch_Jmp_XSlopeFPU+3],ecx
			mov	dword ptr [Patch_Jmp_LeftSlopeFPU+3],edx
			jmp	@@TrueEnd
@@NoZBuf:
			mov	dword ptr [Patch_Jmp_XSlope+3],eax
			mov	dword ptr [Patch_Jmp_LeftSlope+3],ebx
@@TrueEnd:
			pop	edx
			pop	ecx
			pop	ebx
			pop	eax
			ret
Switch_Fillers		ENDP




Fill_ClipXMin		PROC
			M_FILL_CLIP	CLIP_XMIN
Fill_ClipXMin		ENDP


Fill_ClipXMax		PROC
			M_FILL_CLIP	CLIP_XMAX
Fill_ClipXMax		ENDP


Fill_ClipYMin		PROC
			M_FILL_CLIP	CLIP_YMIN
Fill_ClipYMin		ENDP


Fill_ClipYMax		PROC
			M_FILL_CLIP	CLIP_YMAX
Fill_ClipYMax		ENDP






; ESI = PtA; EBX = Pt_B with PtA: point to clip; and PtB: valid point
; y=yA+[(XMin-xa)*(yB-yA)]/(xB-xA)

Generic_ClipXMin	PROC
			push	ecx
			push	ebp

			mov	edx,dword ptr [esi].Pt_XE	; Current point
			mov	eax,dword ptr [ebx].Pt_XE	; Last point

			mov	ecx,edx
			mov	ebp,eax

			shl	ecx,16
			and	edx,0FFFF0000h			; EDX = Ya shl 16

			shl	ebp,16
			and	eax,0FFFF0000h			; EAX = Yb shl 16

			sar	ecx,16				; ECX = Xa
			sub	eax,edx				; EAX = (Yb-Ya) shl 16

			sar	ebp,16				; EBP = Xb
			push	edx

			sub	ebp,ecx				; EBP = (Xb-Xa)
			mov	edx,[ClipXMin]

			sub	edx,ecx				; EDX = (XMin-Xa)

			mov	[Clip_Num],edx

			imul	edx
			idiv	ebp

			pop	edx
			mov	ecx,[ClipXMin]

			add	eax,edx
			and	ecx,0FFFFh

			and	eax,0FFFF0000h

			or	ecx,eax

			mov	dword ptr [edi].Pt_XE,ecx
EnterClip::
			mov	eax,[Fill_ClipFlag]
			test	eax,CLIP_LIGHT
			je	@@No_ClipLight

			mov	eax,dword ptr [ebx].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light
			and	eax,0FFFFh
			and	ecx,0FFFFh
			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx
			mov	[edi].Pt_Light,ax
@@No_ClipLight:

			mov	eax,[Fill_ClipFlag]
			test	eax,CLIP_TEXTURE
			je	@@No_ClipTexture

					; ** MapU **
			mov	eax,dword ptr [ebx].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU
			and	eax,0FFFFh
			and	ecx,0FFFFh
			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx
			mov	[edi].Pt_MapU,ax

					; ** MapV **
			mov	eax,dword ptr [ebx].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU
			shr	eax,16
			shr	ecx,16
			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx
			mov	[edi].Pt_MapV,ax
@@No_ClipTexture:

			mov	eax,[Fill_ClipFlag]
			test	eax,CLIP_TEXTUREZ
			je	@@No_ClipTextureZ

					; ** W **
			mov	eax,dword ptr [ebx].Pt_W
			mov	ecx,dword ptr [esi].Pt_W
			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx
			mov	[edi].Pt_W,eax

					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			and	eax,0FFFFh
			imul	[esi].Pt_W
			shr	eax,8
			shl	edx,24
			or	eax,edx
			mov	ecx,eax

			mov	eax,dword ptr [ebx].Pt_MapU
			and	eax,0FFFFh
			imul	[ebx].Pt_W
			shr	eax,8
			shl	edx,24
			or	eax,edx

			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx

			mov	edx,eax
			shl	eax,8
			sar	edx,24
			idiv	[edi].Pt_W
			mov	[edi].Pt_MapU,ax

					; ** MapV **
			mov	eax,dword ptr [esi].Pt_MapU
			shr	eax,16
			imul	[esi].Pt_W
			shr	eax,8
			shl	edx,24
			or	eax,edx
			mov	ecx,eax

			mov	eax,dword ptr [ebx].Pt_MapU
			shr	eax,16
			imul	[ebx].Pt_W
			shr	eax,8
			shl	edx,24
			or	eax,edx

			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx

			mov	edx,eax
			shl	eax,8
			sar	edx,24
			idiv	[edi].Pt_W
			mov	[edi].Pt_MapV,ax

			mov	eax,[Fill_ClipFlag]
			test	eax,CLIP_ZBUFFER
			je	@@No_ClipZBuffer

					; ** ZBuf **
							; If persp correct and Zbuf,
							; then we clip the zbuf with a
							; perspective correction
			mov	eax,dword ptr [esi].Pt_Light
			shr	eax,16
			imul	[esi].Pt_W
			shr	eax,8
			shl	edx,24
			or	eax,edx
			mov	ecx,eax

			mov	eax,dword ptr [ebx].Pt_Light
			shr	eax,16
			imul	[ebx].Pt_W
			shr	eax,8
			shl	edx,24
			or	eax,edx

			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx

			mov	edx,eax
			shl	eax,8
			sar	edx,24
			idiv	[edi].Pt_W
			mov	[edi].Pt_ZO,ax

			jmp	@@No_ClipZBuffer

@@No_ClipTextureZ:
			mov	eax,[Fill_ClipFlag]
			test	eax,CLIP_ZBUFFER
			je	@@No_ClipZBuffer

			mov	eax,dword ptr [ebx].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx
			imul	[Clip_Num]
			idiv	ebp
			add	eax,ecx
			mov	[edi].Pt_ZO,ax
@@No_ClipZBuffer:

			pop	ebp
			pop	ecx

			ret
Generic_ClipXMin	ENDP





; ESI = PtA; EBX = Pt_B
; y=yA+[(XMax-xa)*(yB-yA)]/(xB-xA)

Generic_ClipXMax	PROC
			push	ecx
			push	ebp

			mov	edx,dword ptr [esi].Pt_XE	; Current point
			mov	eax,dword ptr [ebx].Pt_XE	; Last point

			mov	ecx,edx
			mov	ebp,eax

			shl	ecx,16
			and	edx,0FFFF0000h			; EDX = Ya shl 16

			shl	ebp,16
			and	eax,0FFFF0000h			; EAX = Yb shl 16

			sar	ecx,16				; ECX = Xa
			sub	eax,edx				; EAX = (Yb-Ya) shl 16

			sar	ebp,16				; EBP = Xb
			push	edx

			sub	ebp,ecx				; EBP = (Xb-Xa)
			mov	edx,[ClipXMax]

			inc	edx				; XMax++ bcoz we don't draw the last pixel on a scanline

			sub	edx,ecx				; EDX = (XMin-Xa)

			mov	[Clip_Num],edx

			imul	edx
			idiv	ebp

			pop	edx
			mov	ecx,[ClipXMax]

			add	eax,edx
			and	ecx,0FFFFh

			and	eax,0FFFF0000h
			inc	ecx				; XMax++ bcoz we don't draw the last pixel on a scanline

			or	eax,ecx

			mov	dword ptr [edi].Pt_XE,eax

			jmp	EnterClip
Generic_ClipXMax	ENDP




; ESI = PtA; EBX = Pt_B
; x=xA+[(YMin-ya)*(xB-xA)]/(yB-yA)

Generic_ClipYMin	PROC
			mov	edx,dword ptr [esi].Pt_XE	; A
			mov	eax,dword ptr [ebx].Pt_XE	; B

			push	ecx
			mov	ecx,edx

			shl	edx,16
			push	ebp

			sar	ecx,16				; ECX = Ya
			mov	ebp,eax

			shl	eax,16

			sar	ebp,16				; EBP = Yb
			sub	eax,edx				; EAX = (Xb-Xa) shl 16

			sar	edx,16				; EDX = Xa
			sub	ebp,ecx				; EBP = (Yb-Ya)

			push	edx
			mov	edx,[ClipYMin]

			sub	edx,ecx				; EDX = (YMin-Ya)

			mov	[Clip_Num],edx

			imul	edx				; EAX = (Ymin-Ya)(Xb-Xa)
			idiv	ebp

			sar	eax,16
			pop	edx

			add	eax,edx
			mov	ecx,[ClipYMin]

			shl	ecx,16
			and	eax,0FFFFh

			or	eax,ecx

			mov	dword ptr [edi].Pt_XE,eax

			jmp	EnterClip
Generic_ClipYMin	ENDP




; ESI = PtA; EBX = Pt_B
; x=xA+[(YMax-ya)*(xB-xA)]/(yB-yA)

Generic_ClipYMax	PROC
			mov	edx,dword ptr [esi].Pt_XE	; A
			mov	eax,dword ptr [ebx].Pt_XE	; B

			push	ecx
			mov	ecx,edx

			shl	edx,16
			push	ebp

			sar	ecx,16				; ECX = Ya
			mov	ebp,eax

			shl	eax,16

			sar	ebp,16				; EBP = Yb
			sub	eax,edx				; EAX = (Xb-Xa) shl 16

			sar	edx,16				; EDX = Xa
			sub	ebp,ecx				; EBP = (Yb-Ya)

			push	edx
			mov	edx,[ClipYMax]

			sub	edx,ecx				; EDX = (YMin-Ya)

			mov	[Clip_Num],edx

			imul	edx				; EAX = (Ymin-Ya)(Xb-Xa)
			idiv	ebp

			sar	eax,16
			pop	edx

			add	eax,edx
			mov	ecx,[ClipYMax]

			shl	ecx,16
			and	eax,0FFFFh

			or	eax,ecx

			mov	dword ptr [edi].Pt_XE,eax

			jmp	EnterClip
Generic_ClipYMax	ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Fill_PolyClip: Display a n-angle (provided that the poly is convex)      บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel : ECX = Nb points                                                  บ
; บ         ESI = PTR on the list of points                                  บ
; บ                                                                          บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Retour: EAX = 0 if error (non-visible poly)                              บ
; บ                                                                          บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ ALL REGISTERS MODIFIED                                                   บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4
Fill_PolyClip		PROC
			mov	ebx,Offset @@Fill_Restart
			mov	[Fill_Restart],ebx

@@Fill_Restart:
			; *** Get the poly-box so as to test the clipping ***

			lea	eax,[ecx-1]		; Loop counter in EAX
			push	ecx

			mov	ecx,dword ptr [esi].Pt_XE
			push	esi

			mov	edx,ecx
			mov	[Fill_FirstPoint],esi

			sar	edx,16
			add	esi,Size STRUC_POINT

			shl	ecx,16
			mov	ebp,edx

			sar	ecx,16

			mov	edi,ecx
							; (ECX, EDX, EDI, EBP)
							; (XMin, YMin, XMax, YMax)
@@Bounding_Box:
			push	eax			; What follows has a bad pairing... How can we pair when we have NOTHING to do ?
			mov	eax,dword ptr [esi].Pt_XE
			mov	ebx,eax

			shl	eax,16
			sar	ebx,16
			sar	eax,16

			cmp	edx,ebx
			jle	@@Not_YMin

			mov	edx,ebx
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
			jne	@@Bounding_Box

			cmp	ebp,edx
			jle	@@End			; Skip poly if DeltaY<=0

				; *** Is the polygon at least visible ? ***

			mov	eax,[ClipXMax]
			mov	ebx,[ClipXMin]

			inc	eax			; Bcoz we don't draw the last pixel on a scanline

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


				; *** Should we clip the polygon ? ***

			mov	eax,[ClipXMin]
			sub	esi,Size STRUC_POINT

			mov	ebx,[ClipXMax]
			mov	[Fill_LastPoint],esi

			cmp	ecx,eax
			pop	esi			; Restore List pointer

			pop	ecx			; Restore Nb Pts
			jl	Fill_ClipXMin

			inc	ebx			; Bcoz we don't draw the last pixel on a scanline

			cmp	edi,ebx
			jg	Fill_ClipXMax

			mov	eax,[ClipYMin]
			mov	ebx,[ClipYMax]

			cmp	edx,eax
			jl	Fill_ClipYMin

			cmp	ebp,ebx
			jg	Fill_ClipYMax


Fill_PolyNoClip::
			cmp	ecx,3			; Is it a triangle ?
			je	Draw_Triangle

			push	ecx
			push	Offset @@Ret_Triangle

			push	esi			; Will be poped by Draw_Triangle in EBP
			mov	ecx,3			; (BCOZ EBP is saved in Fill_Poly and restored in Draw_Triangle)

			jmp	Draw_Triangle
@@Ret_Triangle:
			pop	ecx
			lea	esi,[ebp+Size STRUC_POINT]; Next ListPoint pointer

			mov	eax,[ebp]		; Copy the first point in the new location (the following one)
			mov	ebx,[ebp+4]

			mov	[esi],eax
			mov	[esi+4],ebx

			mov	eax,[ebp+8]
			mov	ebx,[ebp+12]

			mov	[esi+8],eax
			mov	[esi+12],ebx

			dec	ecx
			jmp	Fill_PolyNoClip
@@End:
			pop	esi
			pop	ecx
			pop	ebp
			mov	eax,[Fill_LeftSlope]
			ret
Fill_PolyClip		ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ  Draw_Triangle : Remplissage d'un triangle                                     บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ  Appel : ESI = PTR sur une table du type Struc_Point (table des sommets)       บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ  Retour: Rien                                                                  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ  Remarque: les sommets doivent etre ordonnes dans le sens trigo. (anti-        บ
; บ            horlogique).                                                        บ
; บ                                                                                บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ  Principe: (Exemple sur le Gouraud)                                            บ
; บ            (En fait le commentaire etait sur une routine dediee Gouraud et     บ
; บ            j'ai la flemme de le reecrire ;)                                    บ
; บ                                                                                บ
; บ                                                                                บ
; บ   Dans un triangle, l'increment du gouraud est constant.. Sisi, voici la       บ
; บ  preuve:                                                                       บ
; บ                                                                                บ
; บ                                                                                บ
; บ  Soit ABC notre triangle de coordonnees A(xa,ya,ga); B(xb,yb,gb); C(xc,yc,gc)  บ
; บ                                                                                บ
; บ                                                                                บ
; บ                   [(ga+y*Pente_Gouraud_Droite) - (ga+y*Pente_Gouraud_Gauche)]  บ
; บ  Inc_Gouraud(y) = ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ  บ
; บ                           [(xa+y*Pente_Droite) - (xa+y*Pente_Gauche)]          บ
; บ                                                                                บ
; บ                                                                                บ
; บ                   Pente_Gouraud_Droite-Pente_Gouraud_Gauche                    บ
; บ                 = ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ                    บ
; บ                           Pente_Droite-Pente_Gauche                            บ
; บ                                                                                บ
; บ                                                                                บ
; บ                 = Cste; d'ou gain.... :)                                       บ
; บ                                                                                บ
; บ                                                                                บ
; บ                                                                                บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4
Draw_Triangle		PROC
			; *** Search the highest point in the list ***

			mov	eax,1
			mov	[Fill_FirstPoint],esi

			mov	[Fill_Patch],eax	;
			mov	eax,dword ptr [esi].Pt_XE

			sar	eax,16			; EAX = Ya
			mov	edi,esi			; EDI = Highest point

			mov	ebp,eax			; EBP = Highest Y (least value)
			mov	ebx,dword ptr [esi+Size STRUC_POINT].Pt_XE

			sar	ebx,16			; EBX = Yb
			add	esi,Size STRUC_POINT

			cmp	ebx,ebp
			jg	@@Not_Second

			mov	ebp,ebx
			mov	edi,esi
@@Not_Second:
			mov	ecx,dword ptr [esi+Size STRUC_POINT].Pt_XE

			sar	ecx,16			; ECX = Yc
			add	esi,Size STRUC_POINT

			cmp	ecx,ebp
			jg	@@Not_Third

			mov	ebp,ecx
			mov	edi,esi
@@Not_Third:
			mov	[Fill_LeftPoint],edi
			mov	[Fill_RightPoint],edi

			cmp	eax,ebx			; Test if the poly is flat
			jne	@@Not_Flat

			cmp	ebx,ecx
			je	@@End
@@Not_Flat:
			mov	[Fill_LastPoint],esi	; Last point pointer
			mov	ebx,[PTR_TabOffLine]

			mov	eax,[Log]
			mov	[Fill_CurY],ebp		; EBP = Highest point_Y

			mov	ebx,[ebx+ebp*4]
			mov	ecx,READ_NEXT_L + READ_NEXT_R

			add	ebx,eax
			mov	esi,[Fill_LeftPoint]

			mov	[Fill_ReadFlag],ecx
			mov	[Fill_CurOffLine],ebx	; Current offset in log screen

					; *** Calculate the common denominator
					; for interpolation calculation ***

			mov	eax,[Fill_LastPoint]
			lea	edi,[esi+Size STRUC_POINT]

			cmp	edi,eax
			jbe	@@No_Wrap1
			mov	edi,[Fill_FirstPoint]
@@No_Wrap1:
			lea	ebp,[edi+Size STRUC_POINT]
			cmp	ebp,eax
			jbe	@@No_Wrap2
			mov	ebp,[Fill_FirstPoint]
@@No_Wrap2:
							; ESI = PtA
							; EDI = PtB
							; EBP = PtC
			mov	al,[Fill_UseFPU]	; Should we use FPU or not?
			test	al,al
			je	@@DenomInt
					; *** FPU math. ***
			fldcw	[Status_Int]	; 7 cycles lost (not pipelined...)



			; K1 = (Yb-Ya)
			; K2 = (Xc-Xa)
			; K3 = (Yc-Ya)
			; K4 = (Xb-Xa)
			; Denom = K1*K2 - K3*K4
						; 0   1   2   3   4   5   6   7
			fild	[esi].Pt_XE	; Xa
			fild	[esi].Pt_YE	; Ya  Xa
			fild	[edi].Pt_XE	; Xb  Ya  Xa
			fild	[edi].Pt_YE	; Yb  Xb  Ya  Xa
			fild	[ebp].Pt_XE	; Xc  Yb  Xb  Ya  Xa
			fild	[ebp].Pt_YE	; Yc  Xc  Yb  Xb  Ya  Xa

			fxch	st(2)		; Yb  Xc  Yc  Xb  Ya  Xa

			fsub	st,st(4)	; K1  Xc  Yc  Xb  Ya  Xa
			fxch	st(2)		; Yc  Xc  K1  Xb  Ya  Xa

			fsubrp	st(4),st	; Xc  K1  Xb  K3  Xa

			fsub	st,st(4)	; K2  K1  Xb  K3  Xa
			fxch	st(2)		; Xb  K1  K2  K3  Xa

			fsubrp	st(4),st	; K1  K2  K3  K4

			fld	st(0)		; K1  K1  K2  K3  K4

			fmulp	st(2),st	; K1  K12 K3  K4

			fld	st(2)		; K3  K1  K12 K3  K4

			fmulp	st(4),st	; K1  K12 K3  K34
			fxch	st(1)		; K12 K1  K3  K34

						; 2 cycles penalty

			fsubrp	st(3),st	; K1  K3  Denom

			mov	edx,[Fill_Type]

			fld	[F_256]		; 256 K1  K3  Denom
			fxch	st(3)		; Denom K1  K3  256

						; 1 cycle penalty

			fst	[Denom]		; Denom K1  K3  256

			mov	eax,[Denom]

			test	eax,eax
			je	@@EndFPU

			fdivp	st(3),st	; K1  K3  D

Patch_Jmp_XSlopeFPU::	jmp	[Jmp_XSlopeFPU+edx*4]



					; *** Integer math. ***
@@DenomInt:
			mov	eax,dword ptr [edi].Pt_XE
			mov	edx,dword ptr [esi].Pt_XE

			sar	eax,16			; EAX = YB
			mov	ecx,edx

			sar	edx,16			; EDX = YA
			and	ecx,0FFFFh		; ECX = XA (always>0)

			sub	eax,edx			; EAX = YB-YA
			mov	ebx,dword ptr [ebp].Pt_XE

			mov	[YB_YA],eax
			and	ebx,0FFFFh		; EBX = XC

			sub	ebx,ecx			; EBX = XC-XA

			imul	eax,ebx			; EAX = (YB-YA)*(XC-XA)

			push	eax
			mov	eax,dword ptr [ebp].Pt_XE

			sar	eax,16			; EAX = YC
			mov	edx,dword ptr [esi].Pt_XE

			mov	ecx,edx
			mov	ebx,dword ptr [edi].Pt_XE

			sar	edx,16			; EDX = YA
			and	ecx,0FFFFh		; ECX = XA

			sub	eax,edx			; EAX = YC-YA
			and	ebx,0FFFFh		; EBX = XB

			mov	[YC_YA],eax
			sub	ebx,ecx			; EBX = XB-XA

			imul	eax,ebx			; EAX = (YC-YA)*(XB-XA)

			pop	ebx
			mov	edx,[Fill_Type]

			sub	ebx,eax			; EBX = Denom
			je	@@End

			mov	[Denom],ebx		; Never equal to zero
							; bcoz points and flat polys are rejected
							; Denom = (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
Patch_Jmp_XSlope::	jmp	[Jmp_XSlope+edx*4]



; **************************************************
; **************************************************


Triangle_ReadNextEdgeFPU::
						; *** POP FPU Stack ***
							; A  B  C
			faddp	st(0),st		; B  C
			faddp	st(0),st		; C
			faddp	st(0),st		;


;			*****************
;			*** LEFT EDGE ***
;			*****************

Triangle_ReadNextEdge::
			mov	eax,[Fill_ReadFlag]

			test	eax,READ_NEXT_L
			je	Read_Next_Right

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
			mov	dl,[Fill_UseFPU]

			cmp	dl,FALSE
			je	@@CalcLeftSlopeInt

			push	ecx
			mov	eax,dword ptr [esi].Pt_XE

			fild	dword ptr [esp]	; dY

			shl	eax,16
			mov	[Fill_CurXMin],ebx

			fdivr	[F_1]		; 1/dY

			sub	ebx,8000h
			mov	edx,[Fill_Type]

			sub	eax,ebx
			mov	[Fill_LeftPoint],esi

			push	eax		; Save dX
Patch_Jmp_LeftSlopeFPU::jmp	[Jmp_LeftSlopeFPU+edx*4]

			; here we have :
			; ** in the Stack(ESP) : esp+4 => ??
			;			 esp   => dX
			; ** in the Stack(FPU) : 1/dY


@@CalcLeftSlopeInt:
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
			mov	edx,[Fill_Type]

			mov	[Fill_LeftSlope],eax
Patch_Jmp_LeftSlope::	jmp	[Jmp_LeftSlope+edx*4]


;			******************
;			*** RIGHT EDGE ***
;			******************

Read_Next_RightFPU::
			fild	dword ptr [esp]	; dX  1/dY

			fmulp	st(1),st; dX/dY

			pop	eax
			pop	ebx

			fadd	[F_1]	; dX/dY+1

			fistp	[Fill_LeftSlope]
Read_Next_Right::
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
			mov	dl,[Fill_UseFPU]

			cmp	dl,FALSE
			je	@@CalcRightSlopeInt

			push	ecx
			mov	eax,dword ptr [esi].Pt_XE

			fild	dword ptr [esp]	; dY

			shl	eax,16
			mov	[Fill_CurXMax],ebx

			fdivr	[F_1]		; 1/dY

			sub	ebx,8000h

			sub	eax,ebx
			mov	[Fill_RightPoint],esi

			mov	[esp],eax

			fild	dword ptr [esp]	; dX  1/dY

			fmulp	st(1),st; dX/dY

			pop	eax

			fadd	[F_1]	; dX/dY+1

			fistp	[Fill_RightSlope]

			jmp	@@Test_Scan


@@CalcRightSlopeInt:
			mov	eax,dword ptr [esi].Pt_XE
			mov	[Fill_CurXMax],ebx

			shl	eax,16
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
			je	Triangle_ReadNextEdge

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
			je	Triangle_ReadNextEdge

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
			je	Triangle_ReadNextEdge

			mov	edx,[Fill_CurXMax]		; EDX = XMax SHL 16
			mov	ebx,[Fill_CurXMin]		; EBX = XMin SHL 16

			jmp	[Fill_Filler]

			ALIGN	4

@@EndFPU:
						; Denom K1  K3  65536
			faddp	st(0),st	; K1  K3  65536
			faddp	st(0),st	; K3  65536
			faddp	st(0),st	; 65536
			faddp	st(0),st
@@End:
			mov	dl,[Fill_UseFPU]
			mov	eax,[Fill_LeftSlope]

			cmp	dl,FALSE
			je	@@EndRet

			fldcw	[Status_Float]
@@EndRet:
			pop	ebp
			ret
Draw_Triangle		ENDP




Last_FillerPatch	Label	DWORD








; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Gouraud shading                                 บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_GouraudXSlope	PROC
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			ROUND_CHOP eax,edx

			mov	[Fill_Gouraud_XSlope],eax
			jmp	Triangle_ReadNextEdge
Calc_GouraudXSlope	ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Gouraud shading                                 บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_GouraudXSlopeFPU	PROC
						; ESI = PtA
						; EDI = PtB
						; EBP = PtC
					; (Yb-Ya)  (Yc-Ya)   D

			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_Light

			push	eax
			and	ebx,0FFFFh

			sub	ebx,ecx			; EBX = LC-LA

			push	ebx
Gouraud_XSlopeFPU::
			fild	dword ptr [esp+4]; (LB-LA)  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp]	; (LC-LA)  (LB-LA)  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(1)		; (LB-LA)  (LC-LA)  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; (LC-LA) (Yb-Ya)  M1  D'

			add	esp,2*4

			fmulp	st(1),st	; M1  M2  D'

			; ***
			; ***

			fsubrp	st(1),st	; F  D'

			; ***
			; ***

			fmulp	st(1),st	; GSlope

			; ***
			; ***
			; ***

			fistp	[Fill_Gouraud_XSlope]

			jmp	Triangle_ReadNextEdge
Calc_GouraudXSlopeFPU	ENDP







; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Dithered Gouraud shading (the intensity is      บ
; บ saturated)                                                               บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_DitherXSlope	PROC
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			cmp	eax,14*256
			jae	@@SatEDIUp
			cmp	eax,255
			jbe	@@SatEDILow
@@RetEDI:
			cmp	ecx,14*256
			jae	@@SatESIUp
			cmp	ecx,255
			jbe	@@SatESILow
@@RetESI:
			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			cmp	eax,14*256
			jae	@@SatEBPUp
			cmp	eax,255
			jbe	@@SatEBPLow
@@RetEBP:
			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			ROUND_CHOP eax,edx

			mov	[Fill_Gouraud_XSlope],eax
			jmp	Triangle_ReadNextEdge

@@SatEDIUp:
			mov	eax,14*256-1
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatEDILow:
			mov	eax,256
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatESIUp:
			mov	ecx,14*256-1
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatESILow:
			mov	ecx,256
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatEBPUp:
			mov	eax,14*256-1
			mov	[ebp].Pt_Light,ax
			jmp	@@RetEBP
@@SatEBPLow:
			mov	eax,256
			mov	[ebp].Pt_Light,ax
			jmp	@@RetEBP
Calc_DitherXSlope	ENDP







; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Dithered Gouraud shading (the intensity is      บ
; บ saturated)                                                               บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_DitherXSlopeFPU	PROC
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			cmp	eax,14*256
			jae	@@SatEDIUp
			cmp	eax,255
			jbe	@@SatEDILow
@@RetEDI:
			cmp	ecx,14*256
			jae	@@SatESIUp
			cmp	ecx,255
			jbe	@@SatESILow
@@RetESI:
			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_Light

			push	eax
			and	ebx,0FFFFh

			cmp	ebx,14*256
			jae	@@SatEBPUp
			cmp	ebx,255
			jbe	@@SatEBPLow
@@RetEBP:
			sub	ebx,ecx			; EAX = LC-LA

			push	ebx
			jmp	Gouraud_XSlopeFPU	; Go to Gouraud function
							; (don't lose any time bcoz
							; the div is still being evaluated)

@@SatEDIUp:
			mov	eax,14*256-1
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatEDILow:
			mov	eax,256
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatESIUp:
			mov	ecx,14*256-1
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatESILow:
			mov	ecx,256
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatEBPUp:
			mov	ebx,14*256-1
			mov	[ebp].Pt_Light,bx
			jmp	@@RetEBP
@@SatEBPLow:
			mov	ebx,256
			mov	[ebp].Pt_Light,bx
			jmp	@@RetEBP
Calc_DitherXSlopeFPU	ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Mapping                                 บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureXSlope	PROC
					; ** MapU **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			ROUND_CHOP eax,edx

			mov	[Fill_MapU_XSlope],eax

					; ** MapV **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx

			mov	[Fill_MapV_XSlope],eax


			jmp	Triangle_ReadNextEdge
Calc_TextureXSlope	ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Mapping                                 บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureXSlopeFPU	PROC
			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_MapU

			push	eax			; Save (LB-LA)U
			and	ebx,0FFFFh

			sub	ebx,ecx			; EBX = LC-LA
			mov	eax,dword ptr [edi].Pt_MapU

			shr	eax,16
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	ecx,16
			push	ebx			; Save (LC-LA)U

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_MapU

			shr	ebx,16
			push	eax			; Save (LB-LA)V

			sub	ebx,ecx			; EBX = LC-LA

			; Stack FPU		; (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+2*4]; (LB-LA)U  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+1*4]; (LC-LA)U  (LB-LA)U  (Yb-Ya)  (Yc-Ya)  D

			push	ebx			; Save (LC-LA)V

			; ***

			fmul	st,st(2)	; K1  (LB-LA)U  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+1*4]; (LB-LA)V  K1  (LB-LA)U  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+0*4]; (LC-LA)V  (LB-LA)V  K1  (LB-LA)U  (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(3)		; (LB-LA)U  (LB-LA)V  K1  (LC-LA)V  (Yb-Ya)  (Yc-Ya)  D'

			fmul	st,st(5)	; K2  (LB-LA)V  K1  (LC-LA)V  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (LB-LA)V  K2  K1  (LC-LA)V  (Yb-Ya)  (Yc-Ya)  D'

			add	esp,4*4

			fmulp	st(5),st	; K2  K1  (LC-LA)V  (Yb-Ya)  K3  D'

			fsubp	st(1),st	; M1  (LC-LA)V  (Yb-Ya)  K3  D'
			fxch	st(1)		; (LC-LA)V  M1  (Yb-Ya)  K3  D'

			fmulp	st(2),st	; M1  K4  K3  D'

			; ***

			fmul	st,st(3)	; SlopeU  K4  K3  D'
			fxch	st(2)		; K3  K4  SlopeU  D'

			fsubp	st(1),st	; M2  SlopeU  D'

			; ***
			; ***

			fmulp	st(2),st	; SlopeU  SlopeV


;		fadd	[FInv_2]
			fistp	[Fill_MapU_XSlope]
;		fadd	[FInv_2]
			fistp	[Fill_MapV_XSlope]

			jmp	Triangle_ReadNextEdge
Calc_TextureXSlopeFPU	ENDP









; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Gouraud Mapping                         บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureGouraudXSlope PROC
					; ** Gouraud **
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_Gouraud_XSlope],eax

					; ** MapU **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapU_XSlope],eax

					; ** MapV **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapV_XSlope],eax


			jmp	Triangle_ReadNextEdge
Calc_TextureGouraudXSlope ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Gouraud Mapping                         บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureGouraudXSlopeFPU PROC
			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_MapU

			push	eax			; Save (LB-LA)U
			and	ebx,0FFFFh

			sub	ebx,ecx			; EBX = LC-LA
			mov	eax,dword ptr [edi].Pt_MapU

			shr	eax,16
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	ecx,16
			push	ebx			; Save (LC-LA)U

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_MapU

			shr	ebx,16
			push	eax			; Save (LB-LA)V

			sub	ebx,ecx			; EBX = LC-LA
			mov	eax,dword ptr [edi].Pt_Light

			push	ebx			; Save (LC-LA)V
			and	eax,0FFFFh

			mov	ecx,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [ebp].Pt_Light

			and	ecx,0FFFFh
			and	ebx,0FFFFh
CalcGouraudTextureXFPU::
			sub	eax,ecx			; EAX = (LB-LA)L
			sub	ebx,ecx			; EBX = (LC-LA)L

			push	eax			; Save (LB-LA)L
			push	ebx			; Save (LC-LA)L

			; Stack(ESP)
			; 5*4ÿÿÿ(LB-LA)U
			; 4*4   (LC-LA)U
			; 3*4ÿÿÿ(LB-LA)V
			; 2*4ÿÿÿ(LC-LA)V
			; 1*4ÿÿÿ(LB-LA)L
			; 0*4   (LC-LA)L

			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)


			; Stack FPU		; (Yb-Ya)  (Yc-Ya)  D

				; Rq: it may go a bit faster if it could access the stack in the inverse order...
			fild	dword ptr [esp+5*4]; (LB-LA)U (Yb-Ya) (Yc-Ya) D

			fild	dword ptr [esp+4*4]; (LC-LA)U (LB-LA)U (Yb-Ya) (Yc-Ya) D

			fxch	st(1)		; (LB-LA)U (LC-LA)U  (Yb-Ya) (Yc-Ya) D

			fmul	st,st(3)	; KBU (LC-LA)U  (Yb-Ya) (Yc-Ya) D

			fild	dword ptr [esp+3*4]; (LB-LA)V K1 (LC-LA)U  (Yb-Ya) (Yc-Ya) D

			fild	dword ptr [esp+2*4]; (LC-LA)V (LB-LA)V K1 (LC-LA)U  (Yb-Ya) (Yc-Ya) D

			fxch	st(3)		; (LC-LA)U (LB-LA)V K1 (LC-LA)V  (Yb-Ya) (Yc-Ya) D

			fmul	st,st(4)	; KCU (LB-LA)V KBU (LC-LA)V  (Yb-Ya) (Yc-Ya) D
			fxch	st(1)		; (LB-LA)V KCU KBU (LC-LA)V  (Yb-Ya) (Yc-Ya) D

			; ***

			fmul	st,st(5)	; KBV KCU KBU (LC-LA)V  (Yb-Ya) (Yc-Ya) D
			fxch	st(1)		; KCU KBV KBU (LC-LA)V  (Yb-Ya) (Yc-Ya) D

			fsubrp	st(2),st	; KBV  MU (LC-LA)V  (Yb-Ya) (Yc-Ya) D
			fxch	st(2)		; (LC-LA)V MU KBV (Yb-Ya) (Yc-Ya) D

			fmul	st,st(3)	; KCV MU KBV (Yb-Ya) (Yc-Ya) D

			fild	dword ptr [esp+1*4]; (LB-LA)L KCV MU KBV (Yb-Ya) (Yc-Ya) D

			fild	dword ptr [esp+0*4]; (LC-LA)L (LB-LA)L KCV MU KBV (Yb-Ya) (Yc-Ya) D

			fxch	st(1)		; (LB-LA)L (LC-LA)L KCV MU KBV (Yb-Ya) (Yc-Ya) D

			fmulp	st(6),st	; (LC-LA)L KCV MU KBV (Yb-Ya) KBL  D
			fxch	st(1)		; KCV (LC-LA)L MU KBV (Yb-Ya) KBL  D

			fsubrp	st(3),st	; (LC-LA)L MU MV (Yb-Ya) KBL  D

			fmulp	st(3),st	; MU MV KCL KBL  D

			add	esp,6*4

			fmul	st,st(4)	; USlope MV KCL KBL  D
			fxch	st(2)		; KCL MV USlope KBL  D

			fsubrp	st(3),st	; MV USlope ML D

			fmul	st,st(3)	; VSlope USlope ML D
			fxch	st(2)		; ML USlope VSlope D

			; ***

			fmulp	st(3),st	; USlope VSlope LSlope

			fistp	[Fill_MapU_XSlope]
			fistp	[Fill_MapV_XSlope]
			fistp	[Fill_Gouraud_XSlope]

			jmp	Triangle_ReadNextEdge
Calc_TextureGouraudXSlopeFPU ENDP









; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Dither Mapping                          บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureDitherXSlope PROC
				; ** Gouraud **
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			cmp	eax,14*256
			ja	@@SatEDIUp
			cmp	eax,255
			jbe	@@SatEDILow
@@RetEDI:
			cmp	ecx,14*256
			jae	@@SatESIUp
			cmp	ecx,255
			jbe	@@SatESILow
@@RetESI:
			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			cmp	eax,14*256
			jae	@@SatEBPUp
			cmp	eax,255
			jbe	@@SatEBPLow
@@RetEBP:
			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_Gouraud_XSlope],eax


					; ** MapU **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapU_XSlope],eax

					; ** MapV **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapV_XSlope],eax


			jmp	Triangle_ReadNextEdge
@@SatEDIUp:
			mov	eax,14*256-1
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatEDILow:
			mov	eax,256
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatESIUp:
			mov	ecx,14*256-1
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatESILow:
			mov	ecx,256
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatEBPUp:
			mov	eax,14*256-1
			mov	[ebp].Pt_Light,ax
			jmp	@@RetEBP
@@SatEBPLow:
			mov	eax,256
			mov	[ebp].Pt_Light,ax
			jmp	@@RetEBP
Calc_TextureDitherXSlope ENDP





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Dither Mapping                          บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureDitherXSlopeFPU PROC
			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ecx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_MapU

			push	eax			; Save (LB-LA)U
			and	ebx,0FFFFh

			sub	ebx,ecx			; EBX = LC-LA
			mov	eax,dword ptr [edi].Pt_MapU

			shr	eax,16
			mov	ecx,dword ptr [esi].Pt_MapU

			shr	ecx,16
			push	ebx			; Save (LC-LA)U

			sub	eax,ecx			; EAX = LB-LA
			mov	ebx,dword ptr [ebp].Pt_MapU

			shr	ebx,16
			push	eax			; Save (LB-LA)V

			sub	ebx,ecx			; EBX = LC-LA
			mov	eax,dword ptr [edi].Pt_Light

			push	ebx			; Save (LC-LA)V
			and	eax,0FFFFh

			mov	ecx,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [ebp].Pt_Light

			and	ecx,0FFFFh
			and	ebx,0FFFFh

			cmp	eax,14*256
			ja	@@SatEDIUp
			cmp	eax,255
			jbe	@@SatEDILow
@@RetEDI:
			cmp	ecx,14*256
			jae	@@SatESIUp
			cmp	ecx,255
			jbe	@@SatESILow
@@RetESI:
			cmp	ebx,14*256
			jae	@@SatEBPUp
			cmp	ebx,255
			jnbe	CalcGouraudTextureXFPU
			jmp	@@SatEBPLow


@@SatEDIUp:
			mov	eax,14*256-1
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatEDILow:
			mov	eax,256
			mov	[edi].Pt_Light,ax
			jmp	@@RetEDI
@@SatESIUp:
			mov	ecx,14*256-1
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatESILow:
			mov	ecx,256
			mov	[esi].Pt_Light,cx
			jmp	@@RetESI
@@SatEBPUp:
			mov	ebx,14*256-1
			mov	[ebp].Pt_Light,bx
			jmp	CalcGouraudTextureXFPU
@@SatEBPLow:
			mov	ebx,256
			mov	[ebp].Pt_Light,bx
			jmp	CalcGouraudTextureXFPU
Calc_TextureDitherXSlopeFPU ENDP








; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Mapping with perspective correction     บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZXSlope	PROC

			mov	eax,[Denom]
			cmp	eax,16
			jge	@@Ok
			cmp	eax,-16
			jle	@@Ok
			mov	[Fill_MapU_XSlope],0
			mov	[Fill_MapV_XSlope],0
			mov	[Fill_W_XSlope],0
			jmp	Triangle_ReadNextEdge
@@Ok:

					; ** MapU **

			mov	eax,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			mov	ecx,eax
			mov	eax,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU

			and	eax,0FFFFh

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapU_XSlope],eax



					; ** MapV **

			mov	eax,dword ptr [esi].Pt_MapU

			shr	eax,16

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			mov	ecx,eax

			mov	eax,dword ptr [edi].Pt_MapU

			shr	eax,16

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU

			shr	eax,16

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapV_XSlope],eax



					; ** W **
			mov	eax,[edi].Pt_W
			mov	ecx,[esi].Pt_W

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,[ebp].Pt_W

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)

			ROUND_CHOP eax,edx
			mov	[Fill_W_XSlope],eax

			jmp	Triangle_ReadNextEdge
Calc_TextureZXSlope	ENDP












; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Mapping with perspective correction     บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZXSlopeFPU	PROC
							; ESI = PtA
							; EDI = PtB
							; EBP = PtC
			; Stack FPU		; (Yb-Ya)  (Yc-Ya)  D


			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)


					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			shr	eax,16
			mov	ecx,dword ptr [ebp].Pt_MapU

			shr	ebx,16
			push	eax			; Save VA

			shr	ecx,16
			push	ebx			; Save VB

			push	ecx			; Save VC
			mov	eax,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			mov	ebx,dword ptr [edi].Pt_MapU

			mov	ecx,dword ptr [ebp].Pt_MapU
			and	ebx,0FFFFh

			and	ecx,0FFFFh
			push	eax			; Save UA

			push	ebx			; Save UB
			push	ecx			; Save UC

			fild	dword ptr [esp+2*4]; UA  (Yb-Ya)  (Yc-Ya)  D

			fild	[esi].Pt_W	; WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+1*4]; UB  WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fild	[edi].Pt_W	; WB  UB  WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; WA  UB  WB  UA  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+0*4]; UC  UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fild	[ebp].Pt_W	; WC  UC  UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; UB  UC  WC  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; UC  WC  UB'  UA'  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmulp	st(1),st	; UC'  UB'  UA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; UB'  UC'  UA'  (Yb-Ya)  (Yc-Ya)  D

			fsub	st,st(2)	; (UB'-UA')  UC'  UA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; UC'  (UB'-UA')  UA'  (Yb-Ya)  (Yc-Ya)  D

			fsubrp	st(2),st	; (UB'-UA')  (UC'-UA')  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (UC'-UA')  (UB'-UA')  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(2)	; MUC  (UB'-UA')  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (UB'-UA')  MUC  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(3)	; MUB  MUC  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(4)		; D  MUC  (Yb-Ya)  (Yc-Ya)  MUB

			; ***

			fmul	[FInv_65536]	; D' MUC  (Yb-Ya)  (Yc-Ya)  MUB
			fxch	st(4)		; MUB  MUC  (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(1),st	; (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+5*4]; VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	[esi].Pt_W	; WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+4*4]; VB  WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	[edi].Pt_W	; WB  VB  WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; WA  VB  WB  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(3),st	; VB  WB  VA' (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(3)		; (MUC-MUB)  WB  VA' VB  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(6)	; USlope  WB  VA' VB  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; WB  USlope  VA' VB  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmulp	st(3),st	; USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+3*4]; VC  USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fild	[ebp].Pt_W	; WC  VC  USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; USlope  VC  WC  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fistp	[Fill_MapU_XSlope]; VC  WC  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(1),st	; VC'  VA'  VB' (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; VB'  VA'  VC' (Yb-Ya)  (Yc-Ya)  D'

			fsub	st,st(1)	; (VB'-VA')  VA'  VC' (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; VC'  VA'  (VB'-VA') (Yb-Ya)  (Yc-Ya)  D'

			fsubrp	st(1),st	; (VC'-VA') (VB'-VA') (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (VB'-VA') (VC'-VA') (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(3)	; MVB  (VC'-VA') (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (VC'-VA')  MVB (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(2)	; MVC  MVB (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[esi].Pt_W	; WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[edi].Pt_W	; WB  WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[ebp].Pt_W	; WC  WB  WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(3)		; MVB  WB  WA  WC  MVC (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(4),st	; WB  WA  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'

			fsub	st,st(1)	; (WB-WA)  WA  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; WA  (WB-WA)  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(2),st	; (WB-WA) (WC-WA) (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; (MVC-MVB) (WC-WA) (WB-WA) (Yb-Ya)  (Yc-Ya)  D'

			fmul	st,st(5)	; VSlope  (WC-WA) (WB-WA) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; (WB-WA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmulp	st(4),st	; (WC-WA) VSlope  (Yb-Ya)  MWB  D'

			; ***

			fmulp	st(2),st	; VSlope  MWC  MWB  D'

			fistp	[Fill_MapV_XSlope]; MWC  MWB  D'

			fsubrp	st(1),st	; (MWC-MWB) D'

			; ***
			; ***

			fmulp	st(1),st	; WSlope

			; ***
			; ***

			fmul	[F_256]		; WSlope'

			add	esp,6*4
			; ***
			; ***

			fistp	[Fill_W_XSlope]

			jmp	Triangle_ReadNextEdge
Calc_TextureZXSlopeFPU	ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Mapping with perspective correction     บ
; บ and Gouraud shading                                                      บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZGouraudXSlope PROC

			mov	eax,[Denom]
			cmp	eax,16
			jge	@@Ok
			cmp	eax,-16
			jle	@@Ok
			mov	[Fill_MapU_XSlope],0
			mov	[Fill_MapV_XSlope],0
			mov	[Fill_W_XSlope],0
			mov	[Fill_Gouraud_XSlope],0
			jmp	Triangle_ReadNextEdge
@@Ok:

					; ** Gouraud **
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			and	eax,0FFFFh
			and	ecx,0FFFFh

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_Gouraud_XSlope],eax


					; ** MapU **

			mov	eax,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			mov	ecx,eax

			mov	eax,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU

			and	eax,0FFFFh

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapU_XSlope],eax



					; ** MapV **

			mov	eax,dword ptr [esi].Pt_MapU

			shr	eax,16

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			mov	ecx,eax

			mov	eax,dword ptr [edi].Pt_MapU

			shr	eax,16

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU

			shr	eax,16

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapV_XSlope],eax



					; ** W **
			mov	eax,[edi].Pt_W
			mov	ecx,[esi].Pt_W

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,[ebp].Pt_W

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_W_XSlope],eax
			jmp	Triangle_ReadNextEdge
Calc_TextureZGouraudXSlope ENDP



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for Texture Mapping with perspective correction     บ
; บ and Gouraud shading                                                      บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZGouraudXSlopeFPU PROC
							; ESI = PtA
							; EDI = PtB
							; EBP = PtC
			; Stack FPU		; (Yb-Ya)  (Yc-Ya)  D


			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)


					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			shr	eax,16
			mov	ecx,dword ptr [ebp].Pt_MapU

			shr	ebx,16
			push	eax			; Save VA

			shr	ecx,16
			push	ebx			; Save VB

			push	ecx			; Save VC
			mov	eax,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			mov	ebx,dword ptr [edi].Pt_MapU

			mov	ecx,dword ptr [ebp].Pt_MapU
			and	ebx,0FFFFh

			and	ecx,0FFFFh
			push	eax			; Save UA

			push	ebx			; Save UB
			push	ecx			; Save UC

			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			mov	ecx,dword ptr [ebp].Pt_Light
			and	eax,0FFFFh

			and	ebx,0FFFFh
			and	ecx,0FFFFh

			sub	ebx,eax			; EBX = LB-LA
			sub	ecx,eax			; ECX = LC-LA

			fild	dword ptr [esp+2*4]; UA  (Yb-Ya)  (Yc-Ya)  D

			fild	[esi].Pt_W	; WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+1*4]; UB  WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fild	[edi].Pt_W	; WB  UB  WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; WA  UB  WB  UA  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+0*4]; UC  UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fild	[ebp].Pt_W	; WC  UC  UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; UB  UC  WC  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; UC  WC  UB'  UA'  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(1),st	; UC'  UB'  UA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; UB'  UC'  UA'  (Yb-Ya)  (Yc-Ya)  D

			fsub	st,st(2)	; (UB'-UA')  UC'  UA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; UC'  (UB'-UA')  UA'  (Yb-Ya)  (Yc-Ya)  D

			fsubrp	st(2),st	; (UB'-UA')  (UC'-UA')  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (UC'-UA')  (UB'-UA')  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(2)	; MUC  (UB'-UA')  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (UB'-UA')  MUC  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(3)	; MUB  MUC  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(4)		; D  MUC  (Yb-Ya)  (Yc-Ya)  MUB

			; ***

			fmul	[FInv_65536]	; D' MUC  (Yb-Ya)  (Yc-Ya)  MUB
			fxch	st(4)		; MUB  MUC  (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(1),st	; (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+5*4]; VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	[esi].Pt_W	; WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+4*4]; VB  WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	[edi].Pt_W	; WB  VB  WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; WA  VB  WB  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(3),st	; VB  WB  VA' (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(3)		; (MUC-MUB)  WB  VA' VB  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(6)	; USlope  WB  VA' VB  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; WB  USlope  VA' VB  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmulp	st(3),st	; USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+3*4]; VC  USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fild	[ebp].Pt_W	; WC  VC  USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; USlope  VC  WC  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fistp	[Fill_MapU_XSlope]; VC  WC  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(1),st	; VC'  VA'  VB' (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; VB'  VA'  VC' (Yb-Ya)  (Yc-Ya)  D'

			fsub	st,st(1)	; (VB'-VA')  VA'  VC' (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; VC'  VA'  (VB'-VA') (Yb-Ya)  (Yc-Ya)  D'

			fsubrp	st(1),st	; (VC'-VA') (VB'-VA') (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (VB'-VA') (VC'-VA') (Yb-Ya)  (Yc-Ya)  D'

			mov	[esp],ebx	; Save LB-LA
			mov	[esp+4],ecx	; Save LC-LA

			fmul	st,st(3)	; MVB  (VC'-VA') (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (VC'-VA')  MVB (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(2)	; MVC  MVB (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[esi].Pt_W	; WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[edi].Pt_W	; WB  WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[ebp].Pt_W	; WC  WB  WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(3)		; MVB  WB  WA  WC  MVC (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(4),st	; WB  WA  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'

			fsub	st,st(1)	; (WB-WA)  WA  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; WA  (WB-WA)  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(2),st	; (WB-WA) (WC-WA) (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; (MVC-MVB) (WC-WA) (WB-WA) (Yb-Ya)  (Yc-Ya)  D'

			fmul	st,st(5)	; VSlope  (WC-WA) (WB-WA) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; (WB-WA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp]	; (LB-LA) (WB-WA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+4];(LC-LA) (LB-LA) (WB-WA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; (WB-WA) (LB-LA) (LC-LA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'

			fmul	st,st(6)	; MWB (LB-LA) (LC-LA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(4)		; VSlope (LB-LA) (LC-LA) (WC-WA) MWB  (Yb-Ya)  (Yc-Ya)  D'

			fistp	[Fill_MapV_XSlope]; (LB-LA) (LC-LA) (WC-WA) MWB  (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(5),st	; (LC-LA) (WC-WA) MWB  (Yb-Ya)  MLB  D'

			; ***

			fmul	st,st(3)	; MLC (WC-WA) MWB  (Yb-Ya)  MLB  D'
			fxch	st(1)		; (WC-WA) MLC MWB  (Yb-Ya)  MLB  D'

			; ***

			fmulp	st(3),st	; MLC  MWB  MWC  MLB  D'

			; ***

			fsubrp	st(3),st	; MWB  MWC  (MLC-MLB)  D'
			fxch	st(3)		; D'   MWC  (MLC-MLB)  MWB

			fmul	[F_65536]	; D   MWC  (MLC-MLB)  MWB
			fxch	st(3)		; MWB  MWC  (MLC-MLB)  D

			fsubp	st(1),st	; (MWC-MWB) (MLC-MLB)  D
			fxch	st(1)		; (MLC-MLB) (MWC-MWB)  D

			; ***

			fmul	st,st(2)	; LSlope  (MWC-MWB)  D
			fxch	st(2)		; D  (MWC-MWB)  LSlope

			; ***

			fmulp	st(1),st	; WSlope LSlope
			fxch	st(1)		; LSlope WSlope

			fistp	[Fill_Gouraud_XSlope]; WSlope

			fmul	[FInv_256]	; WSlope'

			add	esp,6*4
			; ***
			; ***

			fistp	[Fill_W_XSlope]

			jmp	Triangle_ReadNextEdge
Calc_TextureZGouraudXSlopeFPU ENDP







; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for ZBuffering                                      บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_ZBufXSlope		PROC
					; ** ZBuf **
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LB-LA

			shl	eax,8

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			shr	eax,16

			shr	ecx,16

			sub	eax,ecx			; EAX = LC-LA

			shl	eax,8

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_ZBuf_XSlope],eax
			ret
Calc_ZBufXSlope		ENDP


; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the XSlope for ZBuffering                                      บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_ZBufXSlopeFPU	PROC
					; ** ZBuf **
			mov	eax,dword ptr [edi].Pt_Light
			mov	ecx,dword ptr [esi].Pt_Light

			shr	eax,16
			mov	ebx,dword ptr [ebp].Pt_Light

			shr	ecx,16

			shr	ebx,16
			sub	eax,ecx			; EAX = LB-LA

			sub	ebx,ecx
			push	eax

			push	ebx

			; Stack FPU		; (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+4]; (LB-LA)  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp];(LC-LA)  (LB-LA)  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(1)		; (LB-LA)  (LC-LA)  (Yb-Ya)  (Yc-Ya)  D

			fmul	st,st(3)	; MZB  (LC-LA)  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (LC-LA)  MZB  (Yb-Ya)  (Yc-Ya)  D

			add	esp,2*4

			fmul	st,st(2)	; MZC  MZB  (Yb-Ya)  (Yc-Ya)  D

			; ***
			; ***

			fsubrp	st(1),st	; (MZC-MZB)  (Yb-Ya)  (Yc-Ya)  D

			; ***
			; ***

			fmul	st,st(3)	; ZSlope  (Yb-Ya)  (Yc-Ya)  D

			; ***
			; ***
			; ***

			fistp	[Fill_ZBuf_XSlope]; (Yb-Ya)  (Yc-Ya)  D

			ret
Calc_ZBufXSlopeFPU	ENDP






Calc_XSlopeZBuffer	PROC
			push	edx
			call	Calc_ZBufXSlope
			pop	eax
			jmp	[Jmp_XSlope+eax*4]	; AGI
Calc_XSlopeZBuffer	ENDP






Calc_XSlopeZBufferFPU	PROC
			push	edx
			call	Calc_ZBufXSlopeFPU
			pop	eax
			jmp	[Jmp_XSlopeFPU+eax*4]	; AGI
Calc_XSlopeZBufferFPU	ENDP




Calc_XSlopeZBufferFlag	PROC
			call	Calc_ZBufXSlope
			jmp	[Jmp_XSlope]
Calc_XSlopeZBufferFlag	ENDP






Calc_XSlopeZBufferFlagFPU PROC
			call	Calc_ZBufXSlopeFPU
			jmp	[Jmp_XSlopeFPU]
Calc_XSlopeZBufferFlagFPU ENDP







Calc_TextureZXSlopeZBuf	PROC

			mov	eax,[Denom]
			cmp	eax,16
			jge	@@Ok
			cmp	eax,-16
			jle	@@Ok
			mov	[Fill_MapU_XSlope],0
			mov	[Fill_MapV_XSlope],0
			mov	[Fill_W_XSlope],0
			mov	[Fill_ZBuf_XSlope],0
			jmp	Triangle_ReadNextEdge
@@Ok:


					; ** MapU **

			mov	eax,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			mov	ecx,eax

			mov	eax,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU

			and	eax,0FFFFh

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapU_XSlope],eax



					; ** MapV **

			mov	eax,dword ptr [esi].Pt_MapU

			shr	eax,16

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			mov	ecx,eax

			mov	eax,dword ptr [edi].Pt_MapU

			shr	eax,16

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_MapU

			shr	eax,16

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_MapV_XSlope],eax


					; ** ZBuf **

			mov	eax,dword ptr [esi].Pt_Light

			shr	eax,16

			imul	[esi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			mov	ecx,eax

			mov	eax,dword ptr [edi].Pt_Light

			shr	eax,16

			imul	[edi].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,dword ptr [ebp].Pt_Light

			shr	eax,16

			imul	[ebp].Pt_W

			shr	eax,8

			shl	edx,24

			or	eax,edx

			ROUND_CHOP eax,edx

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_ZBuf_XSlope],eax



					; ** W **
			mov	eax,[edi].Pt_W
			mov	ecx,[esi].Pt_W

			sub	eax,ecx			; EAX = LB-LA

			imul	[YC_YA]			; (Yc-Ya)(LB-LA)

			push	eax
			push	edx			; Save (Yc-Ya)(LB-LA)

			mov	eax,[ebp].Pt_W

			sub	eax,ecx			; EAX = LC-LA

			imul	[YB_YA]

			pop	ebx
			pop	ecx

			sub	eax,ecx

			sbb	edx,ebx

			idiv	[Denom]			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
							; -----------------------------
							; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)
			ROUND_CHOP eax,edx
			mov	[Fill_W_XSlope],eax
			jmp	Triangle_ReadNextEdge
Calc_TextureZXSlopeZBuf	ENDP



Calc_TextureZXSlopeZBufFPU PROC
							; ESI = PtA
							; EDI = PtB
							; EBP = PtC
			; Stack FPU		; (Yb-Ya)  (Yc-Ya)  D


			; (Yb-Ya)(LC-LA)-(Yc-Ya)(LB-LA)
			; -----------------------------
			; (Yb-Ya)(Xc-Xa)-(Yc-Ya)(Xb-Xa)


					; ** ZBuf **
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			shr	eax,16
			mov	ecx,dword ptr [ebp].Pt_Light

			shr	ebx,16
			push	eax			; Save ZA

			shr	ecx,16
			push	ebx			; Save ZB

			push	ecx			; Save ZX

					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			shr	eax,16
			mov	ecx,dword ptr [ebp].Pt_MapU

			shr	ebx,16
			push	eax			; Save VA

			shr	ecx,16
			push	ebx			; Save VB

			push	ecx			; Save VC
			mov	eax,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			mov	ebx,dword ptr [edi].Pt_MapU

			mov	ecx,dword ptr [ebp].Pt_MapU
			and	ebx,0FFFFh

			and	ecx,0FFFFh
			push	eax			; Save UA

			push	ebx			; Save UB
			push	ecx			; Save UC


			fild	dword ptr [esp+8*4]; ZA  (Yb-Ya)  (Yc-Ya)  D

			fild	[esi].Pt_W	; WA  ZA  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+7*4]; ZB  WA  ZA  (Yb-Ya)  (Yc-Ya)  D

			fild	[edi].Pt_W	; WB  ZB  WA  ZA  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; WA  ZB  WB  ZA  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; ZB  WB  ZA'  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+6*4]; ZC  ZB  WB  ZA'  (Yb-Ya)  (Yc-Ya)  D

			fild	[ebp].Pt_W	; WC  ZC  ZB  WB  ZA'  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; ZB  ZC  WC  WB  ZA'  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; ZC  WC  ZB' ZA'  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmulp	st(1),st	; ZC'  ZB'  ZA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; ZB'  ZC'  ZA'  (Yb-Ya)  (Yc-Ya)  D

			fsub	st,st(2)	; (ZB'-ZA') ZC'  ZA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(2)		; ZA'  ZC'  (ZB'-ZA') (Yb-Ya)  (Yc-Ya)  D

			fsubp	st(1),st	; (ZC'-ZA') (ZB'-ZA') (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (ZB'-ZA') (ZC'-ZA') (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(3)	; MZB (ZC'-ZA') (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (ZC'-ZA') MZB (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(2)	; MZC  MZB  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(4)		; D  MZC  (Yb-Ya)  (Yc-Ya)  MZB

			; ***

			fmul	[FInv_65536]	; D' MZC  (Yb-Ya)  (Yc-Ya)  MZB
			fxch	st(4)		; MZB  MZC  (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(1),st	; (MZC-MZB)  (Yb-Ya)  (Yc-Ya)  D'

			; ***
			; ***

			fmul	st,st(3)	; ZBufXSlope  (Yb-Ya)  (Yc-Ya)  D'

			fistp	[Fill_ZBuf_XSlope]; (Yb-Ya)  (Yc-Ya)  D'

;		mov	[Fill_ZBuf_XSlope],0

			fild	dword ptr [esp+2*4]; UA  (Yb-Ya)  (Yc-Ya)  D

			fild	[esi].Pt_W	; WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+1*4]; UB  WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fild	[edi].Pt_W	; WB  UB  WA  UA  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; WA  UB  WB  UA  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fild	dword ptr [esp+0*4]; UC  UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fild	[ebp].Pt_W	; WC  UC  UB  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fxch	st(2)		; UB  UC  WC  WB  UA'  (Yb-Ya)  (Yc-Ya)  D

			fmulp	st(3),st	; UC  WC  UB'  UA'  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmulp	st(1),st	; UC'  UB'  UA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; UB'  UC'  UA'  (Yb-Ya)  (Yc-Ya)  D

			fsub	st,st(2)	; (UB'-UA')  UC'  UA'  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; UC'  (UB'-UA')  UA'  (Yb-Ya)  (Yc-Ya)  D

			fsubrp	st(2),st	; (UB'-UA')  (UC'-UA')  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (UC'-UA')  (UB'-UA')  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(2)	; MUC  (UB'-UA')  (Yb-Ya)  (Yc-Ya)  D
			fxch	st(1)		; (UB'-UA')  MUC  (Yb-Ya)  (Yc-Ya)  D

			; ***

			fmul	st,st(3)	; MUB  MUC  (Yb-Ya)  (Yc-Ya)  D

			; ***
			; ***

			fsubp	st(1),st	; (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+5*4]; VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	[esi].Pt_W	; WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+4*4]; VB  WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fild	[edi].Pt_W	; WB  VB  WA  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; WA  VB  WB  VA  (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(3),st	; VB  WB  VA' (MUC-MUB)  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(3)		; (MUC-MUB)  WB  VA' VB  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(6)	; USlope  WB  VA' VB  (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; WB  USlope  VA' VB  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmulp	st(3),st	; USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fild	dword ptr [esp+3*4]; VC  USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fild	[ebp].Pt_W	; WC  VC  USlope  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(2)		; USlope  VC  WC  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fistp	[Fill_MapU_XSlope]; VC  WC  VA' VB' (Yb-Ya)  (Yc-Ya)  D'

			fmulp	st(1),st	; VC'  VA'  VB' (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; VB'  VA'  VC' (Yb-Ya)  (Yc-Ya)  D'

			fsub	st,st(1)	; (VB'-VA')  VA'  VC' (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; VC'  VA'  (VB'-VA') (Yb-Ya)  (Yc-Ya)  D'

			fsubrp	st(1),st	; (VC'-VA') (VB'-VA') (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (VB'-VA') (VC'-VA') (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(3)	; MVB  (VC'-VA') (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; (VC'-VA')  MVB (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmul	st,st(2)	; MVC  MVB (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[esi].Pt_W	; WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[edi].Pt_W	; WB  WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fild	[ebp].Pt_W	; WC  WB  WA  MVB  MVC (Yb-Ya)  (Yc-Ya)  D'

			fxch	st(3)		; MVB  WB  WA  WC  MVC (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(4),st	; WB  WA  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'

			fsub	st,st(1)	; (WB-WA)  WA  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(1)		; WA  (WB-WA)  WC (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'

			fsubp	st(2),st	; (WB-WA) (WC-WA) (MVC-MVB) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; (MVC-MVB) (WC-WA) (WB-WA) (Yb-Ya)  (Yc-Ya)  D'

			fmul	st,st(5)	; VSlope  (WC-WA) (WB-WA) (Yb-Ya)  (Yc-Ya)  D'
			fxch	st(2)		; (WB-WA) (WC-WA) VSlope  (Yb-Ya)  (Yc-Ya)  D'

			; ***

			fmulp	st(4),st	; (WC-WA) VSlope  (Yb-Ya)  MWB  D'

			; ***

			fmulp	st(2),st	; VSlope  MWC  MWB  D'

			fistp	[Fill_MapV_XSlope]; MWC  MWB  D'

			fsubrp	st(1),st	; (MWC-MWB) D'

			; ***
			; ***

			fmulp	st(1),st	; WSlope

			; ***
			; ***

			fmul	[F_256]		; WSlope'

			add	esp,9*4
			; ***
			; ***

			fistp	[Fill_W_XSlope]

			jmp	Triangle_ReadNextEdge
Calc_TextureZXSlopeZBufFPU ENDP










; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Gouraud shading                              บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_GouraudLeftSlope	PROC
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			add	eax,80h
			add	ebx,80h

			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	edx,[Fill_Color]

			mov	[Fill_Gouraud_LeftSlope],eax
			add	ebx,edx

			mov	[Fill_CurGouraudMin],ebx
			jmp	Read_Next_Right
Calc_GouraudLeftSlope	ENDP


; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Gouraud shading                              บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_GouraudLeftSlopeFPU PROC
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			shl	ebx,8
			add	eax,80h

			shl	eax,8
			add	ebx,8000h

			sub	eax,ebx
			mov	edx,[Fill_Color]

			add	ebx,edx
			mov	[esp+4],eax

			mov	[Fill_CurGouraudMin],ebx
CalcGouraudTableFPU::
			fild	dword ptr [esp]; dX  1/dY

			fild	dword ptr [esp+4]; dG  dX  1/dY

			fxch	st(1)		; dX  dG  1/dY

			fmul	st,st(2)	; dX/dY  dG  1/dY
			fxch	st(1)		; dG  dX/dY  1/dY

			pop	ebx
			pop	eax

			fmulp	st(2),st	; dX/dY  dG/dY

			fadd	[F_1]		; (dX/dY or 1)  dG/dY
			fxch	st(1)		; dG/dY  (dX/dY or 1)

			fistp	[Fill_Gouraud_LeftSlope]

			fistp	[Fill_LeftSlope]

			jmp	Read_Next_Right
Calc_GouraudLeftSlopeFPU ENDP







; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Gouraud shading                              บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_GouraudTableLeftSlope PROC
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			add	eax,80h
			add	ebx,80h

			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurGouraudMin],ebx

			mov	[Fill_Gouraud_LeftSlope],eax
			jmp	Read_Next_Right
Calc_GouraudTableLeftSlope ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Gouraud shading                              บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_GouraudTableLeftSlopeFPU PROC
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			shl	ebx,8
			add	eax,80h

			shl	eax,8
			add	ebx,8000h

			sub	eax,ebx
			mov	[Fill_CurGouraudMin],ebx

			mov	[esp+4],eax
			jmp	CalcGouraudTableFPU
Calc_GouraudTableLeftSlopeFPU ENDP










; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Mapping                              บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureLeftSlope	PROC
					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh

IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurMapUMin],ebx

			mov	[Fill_MapU_LeftSlope],eax

					; ** MapV **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF

			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF
			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurMapVMin],ebx

			mov	[Fill_MapV_LeftSlope],eax
			jmp	Read_Next_Right
Calc_TextureLeftSlope	ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Mapping                              บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureLeftSlopeFPU PROC
					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh
IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			sub	eax,ebx
			mov	ecx,dword ptr [esi].Pt_MapU

			shl	eax,8
			mov	edx,dword ptr [edi].Pt_MapU

			shl	ebx,8
			mov	[esp+4],eax	; Save dU

			and	ecx,0FFFF0000h
			and	edx,0FFFF0000h

			shr	ecx,8
IFDEF CORRECT_TEXTURE
			add	edx,80h shl 16
ENDIF
			shr	edx,8
IFDEF CORRECT_TEXTURE
			add	ecx,8000h
ENDIF
			sub	ecx,edx
			mov	[Fill_CurMapUMin],ebx

			fild	dword ptr [esp] ; dX  1/dY

			fild	dword ptr [esp+4]; dU  dX  1/dY

			fxch	st(1)		; dX  dU  1/dY

			fmul	st,st(2)	; XSlope dU  1/dY
			fxch	st(1)		; dU XSlope  1/dY

			mov	[esp],ecx		; Save dV

			fmul	st,st(2)	; USlope XSlope 1/dY
			fxch	st(1)		; XSlope USlope 1/dY

			fadd	[F_1]		; (XSlope+1) USlope 1/dY

			fild	dword ptr [esp]	; dV (XSlope+1) USlope 1/dY

			add	esp,2*4
			mov	[Fill_CurMapVMin],edx

			fxch	st(1)		; (XSlope+1)  dV  USlope 1/dY

			fistp	[Fill_LeftSlope]; dV  USlope 1/dY

			fmulp	st(2),st	; USlope VSlope

;		fadd	[FInv_2]
			fistp	[Fill_MapU_LeftSlope]
;		fadd	[FInv_2]
			fistp	[Fill_MapV_LeftSlope]

			jmp	Read_Next_Right
Calc_TextureLeftSlopeFPU ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Gouraud Mapping                      บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureGouraudLeftSlope PROC
				       ; ** Gouraud **
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			add	eax,80h
			add	ebx,80h

			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurGouraudMin],ebx

			mov	[Fill_Gouraud_LeftSlope],eax

					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh
IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurMapUMin],ebx

			mov	[Fill_MapU_LeftSlope],eax

					; ** MapV **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF

			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurMapVMin],ebx

			mov	[Fill_MapV_LeftSlope],eax
			jmp	Read_Next_Right
Calc_TextureGouraudLeftSlope ENDP





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Gouraud Mapping                      บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureGouraudLeftSlopeFPU PROC
					; ** MapU **
			mov	eax,dword ptr [esi].Pt_MapU
			mov	ebx,dword ptr [edi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh
IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			sub	eax,ebx
			mov	ecx,dword ptr [esi].Pt_MapU

			shl	eax,8
			mov	edx,dword ptr [edi].Pt_MapU

			shl	ebx,8
			mov	[esp+4],eax	; Save dU

			and	ecx,0FFFF0000h
			and	edx,0FFFF0000h

			shr	ecx,8
IFDEF CORRECT_TEXTURE
			add	edx,80h shl 16
ENDIF
			shr	edx,8
IFDEF CORRECT_TEXTURE
			add	ecx,8000h
ENDIF
			sub	ecx,edx
			mov	[Fill_CurMapUMin],ebx

			push	ecx		; Save dV
			mov	[Fill_CurMapVMin],edx

			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			shl	ebx,8
			add	eax,80h

			shl	eax,8
			add	ebx,8000h

			sub	eax,ebx
			mov	[Fill_CurGouraudMin],ebx

			push	eax

			; Stack (ESP)
			;   3*4 => dU
			;   2*4 => dX
			;   1*4 => dV
			;   0*4 => dG


			fild	dword ptr [esp+2*4]; dX  1/dY

			fild	dword ptr [esp+3*4]; dU  dX  1/dY

			fxch	st(1)		; dX  dU  1/dY

			fmul	st,st(2)	; dX/dY  dU  1/dY

			fild	dword ptr [esp+1*4]; dV  dX/dY  dU  1/dY

			fild	dword ptr [esp+0*4]; dG  dV  dX/dY  dU  1/dY

			fxch	st(3)		; dU  dV  dX/dY  dG  1/dY

			fmul	st,st(4)	; dU/dY  dV  dX/dY  dG  1/dY
			fxch	st(2)		; dX/dY  dV  dU/dY  dG  1/dY

			fadd	[F_1]		; XSlope dV  dU/dY  dG  1/dY
			fxch	st(1)		; dV  XSlope dU/dY  dG  1/dY

			fmul	st,st(4)	; dV/dY  XSlope dU/dY  dG  1/dY
			fxch	st(3)		; dG  XSlope dU/dY  dV/dY  1/dY

			add	esp,4*4

			fmulp	st(4),st	; XSlope dU/dY  dV/dY  dG/dY


			fistp	[Fill_LeftSlope]

			fistp	[Fill_MapU_LeftSlope]

			fistp	[Fill_MapV_LeftSlope]

			fistp	[Fill_Gouraud_LeftSlope]

			jmp	Read_Next_Right
Calc_TextureGouraudLeftSlopeFPU ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Mapping with perspective correction  บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZLeftSlope	PROC
					; ** MapU **

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh

IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF

			imul	[edi].Pt_W

			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W

			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurMapUMin],ebx

			mov	[Fill_MapU_LeftSlope],eax


					; ** MapV **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF

			imul	[edi].Pt_W
			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W
			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurMapVMin],ebx

			mov	[Fill_MapV_LeftSlope],eax


					; ** W **
			mov	eax,[esi].Pt_W
			mov	ebx,[edi].Pt_W

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31
			idiv	ecx
			ROUND_CHOP eax,edx
			mov	[Fill_CurWMin],ebx
			mov	[Fill_W_LeftSlope],eax

			jmp	Read_Next_Right
Calc_TextureZLeftSlope	ENDP





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Mapping with perspective correction  บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZLeftSlopeFPU PROC
			; ** in the Stack(ESP) : esp+4 => ??
			;			 esp   => dX
			; ** in the Stack(FPU) : 1/dY

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh

IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			mov	[esp+4],eax	; Save UA
			push	ebx		; Save UB

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF
			push	eax             ; Save VA
			mov	eax,[edi].Pt_W

			push	ebx		; Save VB
			mov	[Fill_CurWMin],eax


			; Stack(ESP)
			; 4*4	UA
			; 3*4	dX
			; 2*4	UB
			; 1*4	VA
			; 0*4ÿÿÿVB

			fild	dword ptr [esp+3*4]; dX  1/dY

			fild	dword ptr [esp+4*4]; UA  dX  1/dY

			fild	[edi].Pt_W	; WA  UA  dX  1/dY

			fxch	st(2)		; dX  UA  WA  1/dY

			fmul	st,st(3)	; XSlope  UA  WA  1/dY
			fxch	st(1)		; UA  XSlope  WA  1/dY

			; ***

			fmul	st,st(2)	; UA' XSlope  WA  1/dY
			fxch	st(1)		; XSlope  UA' WA  1/dY

			fadd	[F_1]		; XSlope' UA' WA  1/dY

			fild	dword ptr [esp+2*4]; UB XSlope' UA' WA  1/dY

			fild	[esi].Pt_W	; WB UB XSlope' UA' WA  1/dY

			fxch	st(2)		; XSlope' UB WB UA' WA  1/dY

			fistp	[Fill_LeftSlope]; UB WB UA' WA  1/dY

			fmul	st,st(1)	; UB' WB UA' WA  1/dY

			fild	dword ptr [esp+1*4]; VA UB' WB UA' WA  1/dY

			fild	dword ptr [esp+0*4]; VB VA UB' WB UA' WA  1/dY

			fxch	st(1)		; VA VB  UB' WB  UA' WA  1/dY

			fmul	st,st(5)	; VA' VB  UB' WB  UA' WA  1/dY
			fxch	st(1)		; VB  VA' UB' WB  UA' WA  1/dY

			; ***

			fmul	st,st(3)	; VB' VA' UB' WB  UA' WA  1/dY
			fxch	st(4)		; UA' VA' UB' WB  VB' WA  1/dY

			fsub	st(2),st	; UA' VA' (UB'-UA') WB  VB' WA  1/dY
			fxch	st(6)		; 1/dY VA' (UB'-UA') WB  VB' WA  UA'

			fmul	[FInv_256]	; 256/dY VA' (UB'-UA') WB  VB' WA  UA'
			fxch	st(3)		; WB  VA'  (UB'-UA')  256/dY  VB' WA  UA'

			fsubrp	st(5),st	; VA'  (UB'-UA')  256/dY  VB' (WB-WA)  UA'
			fxch	st(1)		; (UB'-UA')  VA'  256/dY  VB' (WB-WA)  UA'

			fmul	st,st(2)	; USlope   VA'  256/dY  VB' (WB-WA)  UA'
			fxch	st(1)		; VA'  USlope  256/dY  VB' (WB-WA)  UA'

			fsub	st(3),st	; VA'  USlope  256/dY  (VB'-VA')  (WB-WA)  UA'

			fmul	[FInv_256]	; VA'' USlope  256/dY  (VB'-VA')  (WB-WA)  UA'
			fxch	st(3)		; (VB'-VA')  USlope  256/dY  VA'' (WB-WA)  UA'

			; ***

			fmul	st,st(2)	; VSlope  USlope  256/dY  VA'' (WB-WA)  UA'
			fxch	st(5)		; UA'  USlope  256/dY  VA'' (WB-WA)  VSlope

			add	esp,5*4

			fmul	[FInv_256]	; UA'' USlope  256/dY  VA'' (WB-WA)  VSlope
			fxch	st(3)		; VA'' USlope  256/dY  UA'' (WB-WA)  VSlope

			fistp	[Fill_CurMapVMin]
			fistp	[Fill_MapU_LeftSlope]; 256/dY  UA'' (WB-WA)  VSlope

			fmulp	st(2),st	; UA''  WSlope  VSlope

			fistp	[Fill_CurMapUMin]

			fmul	[F_256]
			fxch	st(1)

			fistp	[Fill_MapV_LeftSlope]

			fistp	[Fill_W_LeftSlope]

			jmp	Read_Next_Right
Calc_TextureZLeftSlopeFPU ENDP





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Mapping with perspective correction  บ
; บ and Gouraud shading                                                      บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZGouraudLeftSlope PROC

				       ; ** Gouraud **
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			add	eax,80h
			add	ebx,80h

			mov	edx,eax
			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurGouraudMin],ebx

			mov	[Fill_Gouraud_LeftSlope],eax


					; ** MapU **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU
			and	eax,0FFFFh
			and	ebx,0FFFFh
IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			imul	[edi].Pt_W
			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W
			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurMapUMin],ebx

			mov	[Fill_MapU_LeftSlope],eax


					; ** MapV **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU
			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF
			imul	[edi].Pt_W
			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W
			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurMapVMin],ebx

			mov	[Fill_MapV_LeftSlope],eax


					; ** W **
			mov	eax,[esi].Pt_W
			mov	ebx,[edi].Pt_W

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31
			idiv	ecx
			ROUND_CHOP eax,edx
			mov	[Fill_CurWMin],ebx
			mov	[Fill_W_LeftSlope],eax

			jmp	Read_Next_Right
Calc_TextureZGouraudLeftSlope ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for Texture Mapping with perspective correction  บ
; บ and Gouraud shading                                                      บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_TextureZGouraudLeftSlopeFPU PROC
			; ** in the Stack(ESP) : esp+4 => ??
			;			 esp   => dX
			; ** in the Stack(FPU) : 1/dY

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh

IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			mov	[esp+4],eax	; Save UA
			push	ebx		; Save UB

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF
			push	eax             ; Save VA
			mov	eax,[edi].Pt_W

			push	ebx		; Save VB
			mov	[Fill_CurWMin],eax

			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			and	eax,0FFFFh
			and	ebx,0FFFFh

			shl	ebx,8
			add	eax,80h

			shl	eax,8
			add	ebx,8000h

			sub	eax,ebx
			mov	[Fill_CurGouraudMin],ebx

			push	eax

			; Stack(ESP)
			; 5*4	UA
			; 4*4	dX
			; 3*4	UB
			; 2*4	VA
			; 1*4ÿÿÿVB
			; 0*4   dL

			fild	dword ptr [esp+0*4]; dL  1/dY

			fild	dword ptr [esp+5*4]; UA  dL  1/dY

			fild	dword ptr [esp+4*4]; dX  UA  dL  1/dY

			fxch	st(2)		; dL  UA  dX  1/dY

			fmul	st,st(3)	; LSlope  UA  dX  1/dY

			fild	[edi].Pt_W	; WA  LSlope  UA  dX  1/dY

			fxch	st(1)

			fistp	[Fill_Gouraud_LeftSlope]; WA  UA  dX  1/dY

			fxch	st(2)		; dX  UA  WA  1/dY

			fmul	st,st(3)	; XSlope  UA  WA  1/dY
			fxch	st(1)		; UA  XSlope  WA  1/dY

			; ***

			fmul	st,st(2)	; UA' XSlope  WA  1/dY
			fxch	st(1)		; XSlope  UA' WA  1/dY

			fadd	[F_1]		; XSlope' UA' WA  1/dY

			fild	dword ptr [esp+3*4]; UB XSlope' UA' WA  1/dY

			fild	[esi].Pt_W	; WB UB XSlope' UA' WA  1/dY

			fxch	st(2)		; XSlope' UB WB UA' WA  1/dY

			fistp	[Fill_LeftSlope]; UB WB UA' WA  1/dY

			fmul	st,st(1)	; UB' WB UA' WA  1/dY

			fild	dword ptr [esp+2*4]; VA UB' WB UA' WA  1/dY

			fild	dword ptr [esp+1*4]; VB VA UB' WB UA' WA  1/dY

			fxch	st(1)		; VA VB  UB' WB  UA' WA  1/dY

			fmul	st,st(5)	; VA' VB  UB' WB  UA' WA  1/dY
			fxch	st(1)		; VB  VA' UB' WB  UA' WA  1/dY

			; ***

			fmul	st,st(3)	; VB' VA' UB' WB  UA' WA  1/dY
			fxch	st(4)		; UA' VA' UB' WB  VB' WA  1/dY

			fsub	st(2),st	; UA' VA' (UB'-UA') WB  VB' WA  1/dY
			fxch	st(6)		; 1/dY VA' (UB'-UA') WB  VB' WA  UA'

			fmul	[FInv_256]	; 256/dY VA' (UB'-UA') WB  VB' WA  UA'
			fxch	st(3)		; WB  VA'  (UB'-UA')  256/dY  VB' WA  UA'

			fsubrp	st(5),st	; VA'  (UB'-UA')  256/dY  VB' (WB-WA)  UA'
			fxch	st(1)		; (UB'-UA')  VA'  256/dY  VB' (WB-WA)  UA'

			fmul	st,st(2)	; USlope   VA'  256/dY  VB' (WB-WA)  UA'
			fxch	st(1)		; VA'  USlope  256/dY  VB' (WB-WA)  UA'

			fsub	st(3),st	; VA'  USlope  256/dY  (VB'-VA')  (WB-WA)  UA'

			fmul	[FInv_256]	; VA'' USlope  256/dY  (VB'-VA')  (WB-WA)  UA'
			fxch	st(3)		; (VB'-VA')  USlope  256/dY  VA'' (WB-WA)  UA'

			; ***

			fmul	st,st(2)	; VSlope  USlope  256/dY  VA'' (WB-WA)  UA'
			fxch	st(5)		; UA'  USlope  256/dY  VA'' (WB-WA)  VSlope

			; ***

			fmul	[FInv_256]	; UA'' USlope  256/dY  VA'' (WB-WA)  VSlope
			fxch	st(3)		; VA'' USlope  256/dY  UA'' (WB-WA)  VSlope

			fistp	[Fill_CurMapVMin]
			fistp	[Fill_MapU_LeftSlope]; 256/dY  UA'' (WB-WA)  VSlope

			fmulp	st(2),st	; UA''  WSlope  VSlope

			fistp	[Fill_CurMapUMin]

			fmul	[F_256]
			fxch	st(1)

			fistp	[Fill_MapV_LeftSlope]

			fistp	[Fill_W_LeftSlope]

			add	esp,6*4
			jmp	Read_Next_Right
Calc_TextureZGouraudLeftSlopeFPU ENDP





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for ZBuffer                                      บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_ZBufLeftSlope	PROC
			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			shr	eax,16

			shr	ebx,16
			mov	edx,eax

			sub	eax,ebx

			shl	eax,8
			sub	edx,ebx

			sar	edx,32-8

			idiv	ecx

			shl	ebx,8
			ROUND_CHOP eax,edx
			mov	[Fill_CurZBufMin],ebx

			mov	[Fill_ZBuf_LeftSlope],eax
			ret
Calc_ZBufLeftSlope	ENDP



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Calculate the LeftSlope for ZBuffer                                      บ
; บ                                                                          บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Calc_ZBufLeftSlopeFPU	PROC
			; WARNING : DON'T MODIFY EDX

			mov	eax,dword ptr [esi].Pt_Light
			mov	ebx,dword ptr [edi].Pt_Light

			shr	eax,8
			and	ebx,0FFFF0000h

			shr	ebx,8
			and	eax,0FFFF00h

			sub	eax,ebx
			mov	[Fill_CurZBufMin],ebx

			mov	[esp+8],eax

			fild	dword ptr [esp+8]	; dZ  1/dY

			fmul	st,st(1)

			fistp	[Fill_ZBuf_LeftSlope]

			ret
Calc_ZBufLeftSlopeFPU	ENDP





Calc_LeftSlopeZBuffer	PROC
			push	edx
			call	Calc_ZBufLeftSlope
			pop	edx
			jmp	[Jmp_LeftSlope+edx*4]	; AGI
Calc_LeftSlopeZBuffer	ENDP





Calc_LeftSlopeZBufferFPU PROC
			call	Calc_ZBufLeftSlopeFPU
			jmp	[Jmp_LeftSlopeFPU+edx*4]
Calc_LeftSlopeZBufferFPU ENDP




Calc_LeftSlopeZBufferFlag PROC
			call	Calc_ZBufLeftSlope
			jmp	[Jmp_LeftSlope]
Calc_LeftSlopeZBufferFlag ENDP





Calc_LeftSlopeZBufferFlagFPU PROC
			call	Calc_ZBufLeftSlopeFPU
			jmp	[Jmp_LeftSlopeFPU]
Calc_LeftSlopeZBufferFlagFPU ENDP







Calc_TextureZLeftSlopeZBuf PROC
					; ** MapU **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU
			and	eax,0FFFFh
			and	ebx,0FFFFh
IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			imul	[edi].Pt_W
			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W
			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurMapUMin],ebx

			mov	[Fill_MapU_LeftSlope],eax


					; ** MapV **
			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU
			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF
			imul	[edi].Pt_W
			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W
			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurMapVMin],ebx

			mov	[Fill_MapV_LeftSlope],eax


					; ** ZBuf **
			mov	ebx,dword ptr [esi].Pt_Light

			shr	ebx,16
			mov	eax,dword ptr [edi].Pt_Light

			shr	eax,16

			imul	[edi].Pt_W
			shl	edx,24
			shr	eax,8
			or	edx,eax
			ROUND_CHOP edx,eax

			mov	eax,ebx
			mov	ebx,edx

			imul	[esi].Pt_W
			shl	edx,24
			shr	eax,8
			or	eax,edx
			ROUND_CHOP eax,edx

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31

			idiv	ecx

			ROUND_CHOP eax,edx
			mov	[Fill_CurZBufMin],ebx

			mov	[Fill_ZBuf_LeftSlope],eax


					; ** W **
			mov	eax,[esi].Pt_W
			mov	ebx,[edi].Pt_W

			sub	eax,ebx
			mov	edx,eax
			sar	edx,31
			idiv	ecx
			ROUND_CHOP eax,edx
			mov	[Fill_CurWMin],ebx

			mov	[Fill_W_LeftSlope],eax

			jmp	Read_Next_Right
Calc_TextureZLeftSlopeZBuf ENDP









Calc_TextureZLeftSlopeZBufFPU PROC
			; ** in the Stack(ESP) : esp+4 => ??
			;			 esp   => dX
			; ** in the Stack(FPU) : 1/dY

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			and	eax,0FFFFh
			and	ebx,0FFFFh

IFDEF CORRECT_TEXTURE
			add	eax,80h
			add	ebx,80h
ENDIF
			mov	[esp+4],eax	; Save UA
			push	ebx		; Save UB

			mov	eax,dword ptr [edi].Pt_MapU
			mov	ebx,dword ptr [esi].Pt_MapU

			shr	eax,16
IFDEF CORRECT_TEXTURE
			add	ebx,80h shl 16
ENDIF
			shr	ebx,16
IFDEF CORRECT_TEXTURE
			add	eax,80h
ENDIF
			push	eax             ; Save VA
			mov	eax,[edi].Pt_W

			push	ebx		; Save VB
			mov	[Fill_CurWMin],eax


			mov	eax,dword ptr [edi].Pt_Light
			mov	ebx,dword ptr [esi].Pt_Light

			shr	eax,16

			shr	ebx,16
			push	eax             ; Save ZA

			push	ebx		; Save ZB


			; Stack(ESP)
			; 6*4	UA
			; 5*4	dX
			; 4*4	UB
			; 3*4	VA
			; 2*4ÿÿÿVB
			; 1*4	ZA
			; 0*4ÿÿÿZB

			fild	dword ptr [esp+1*4]; ZA  1/dY

			fild	dword ptr [esp+0*4]; ZB  ZA  1/dY

			fild	[edi].Pt_W	; WA  ZB  ZA  1/dY

			fild	[esi].Pt_W	; WB  WA  ZB  ZA  1/dY

			fxch	st(1)		; WA  WB  ZB  ZA  1/dY

			fmulp	st(3),st	; WB  ZB  ZA'  1/dY

			; ***

			fmulp	st(1),st	; ZB'  ZA'  1/dY
			; ***
			; ***
			fmul	[FInv_256]	; ZSlope'  1/dY

			fxch	st(1)		; ZA'  ZB'  1/dY
			; ***
			; ***
			fmul	[FInv_256]	; ZSlope'  1/dY

			; ***
			; ***
			; ***

			fist	[Fill_CurZBufMin]; ZA'  ZB'  1/dY

			fsubp	st(1),st	; (ZB'-ZA')  1/dY

			; ***
			; ***

			fmul	st,st(1)	; ZSlope  1/dY

			; ***
			; ***
			; ***

			fistp	[Fill_ZBuf_LeftSlope]


			fild	dword ptr [esp+5*4]; dX  1/dY

			fild	dword ptr [esp+6*4]; UA  dX  1/dY

			fild	[edi].Pt_W	; WA  UA  dX  1/dY

			fxch	st(2)		; dX  UA  WA  1/dY

			fmul	st,st(3)	; XSlope  UA  WA  1/dY
			fxch	st(1)		; UA  XSlope  WA  1/dY

			; ***

			fmul	st,st(2)	; UA' XSlope  WA  1/dY
			fxch	st(1)		; XSlope  UA' WA  1/dY

			fadd	[F_1]		; XSlope' UA' WA  1/dY

			fild	dword ptr [esp+4*4]; UB XSlope' UA' WA  1/dY

			fild	[esi].Pt_W	; WB UB XSlope' UA' WA  1/dY

			fxch	st(2)		; XSlope' UB WB UA' WA  1/dY

			fistp	[Fill_LeftSlope]; UB WB UA' WA  1/dY

			fmul	st,st(1)	; UB' WB UA' WA  1/dY

			fild	dword ptr [esp+3*4]; VA UB' WB UA' WA  1/dY

			fild	dword ptr [esp+2*4]; VB VA UB' WB UA' WA  1/dY

			fxch	st(1)		; VA VB  UB' WB  UA' WA  1/dY

			fmul	st,st(5)	; VA' VB  UB' WB  UA' WA  1/dY
			fxch	st(1)		; VB  VA' UB' WB  UA' WA  1/dY

			; ***

			fmul	st,st(3)	; VB' VA' UB' WB  UA' WA  1/dY
			fxch	st(4)		; UA' VA' UB' WB  VB' WA  1/dY

			fsub	st(2),st	; UA' VA' (UB'-UA') WB  VB' WA  1/dY
			fxch	st(6)		; 1/dY VA' (UB'-UA') WB  VB' WA  UA'

			fmul	[FInv_256]	; 256/dY VA' (UB'-UA') WB  VB' WA  UA'
			fxch	st(3)		; WB  VA'  (UB'-UA')  256/dY  VB' WA  UA'

			fsubrp	st(5),st	; VA'  (UB'-UA')  256/dY  VB' (WB-WA)  UA'
			fxch	st(1)		; (UB'-UA')  VA'  256/dY  VB' (WB-WA)  UA'

			fmul	st,st(2)	; USlope   VA'  256/dY  VB' (WB-WA)  UA'
			fxch	st(1)		; VA'  USlope  256/dY  VB' (WB-WA)  UA'

			fsub	st(3),st	; VA'  USlope  256/dY  (VB'-VA')  (WB-WA)  UA'

			fmul	[FInv_256]	; VA'' USlope  256/dY  (VB'-VA')  (WB-WA)  UA'
			fxch	st(3)		; (VB'-VA')  USlope  256/dY  VA'' (WB-WA)  UA'

			; ***

			fmul	st,st(2)	; VSlope  USlope  256/dY  VA'' (WB-WA)  UA'
			fxch	st(5)		; UA'  USlope  256/dY  VA'' (WB-WA)  VSlope

			add	esp,7*4

			fmul	[FInv_256]	; UA'' USlope  256/dY  VA'' (WB-WA)  VSlope
			fxch	st(3)		; VA'' USlope  256/dY  UA'' (WB-WA)  VSlope

			fistp	[Fill_CurMapVMin]
			fistp	[Fill_MapU_LeftSlope]; 256/dY  UA'' (WB-WA)  VSlope

			fmulp	st(2),st	; UA''  WSlope  VSlope

			fistp	[Fill_CurMapUMin]

			fmul	[F_256]
			fxch	st(1)

			fistp	[Fill_MapV_LeftSlope]

			fistp	[Fill_W_LeftSlope]

			jmp	Read_Next_Right
Calc_TextureZLeftSlopeZBufFPU ENDP





;				******************
;				*** Local proc ***
;				******************

IFDEF	_WIN32

; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Initialize 'Fill_Poly' under a WIN32 system                              บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
InitFillPoly_WIN32	PROC
			pushad
			mov	ebx,Offset TabOffLine
			call	SetScreenPitch
			popad

			push	Offset First_FillerPatch
			push	Offset Last_FillerPatch
			push	Offset Good_CodeFillPoly
			jmp	AllowPatchAndPatch
Good_CodeFillPoly:
			push	ebp			; ** THERE MUST BE AT LEAST
			xor	ebp,ebp			; ** 5 BYTES B4 THE
			mov	[Fill_LeftSlope],ebp	; ** PATCH CODE
InitFillPoly_WIN32	ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Initialize 'SetScreenPitch' under a WIN32 system                         บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
Init_SetScreenPitch_WIN32 PROC
			call	Init_Fillers_WIN32

			push	Offset SetScreenPitch
			push	Offset SetScreenPitch+5		; COZ we'll patch at least 5 bytes
			push	Offset @@Good_Code		;		   ^^^^exactly ;)
			jmp	AllowPatchAndPatch
@@Good_Code:
			push	eax
			push	ebx
REPT	3
			NOP
ENDM
Init_SetScreenPitch_WIN32 ENDP



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Initialize 'Switch_Fillers' under a WIN32 system                         บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
Init_SwitchFillers_WIN32 PROC
			call	Init_Fillers_WIN32

			push	Offset Switch_Fillers
			push	Offset Switch_Fillers+5		; COZ we'll patch at least 5 bytes
			push	Offset @@Good_Code		;		   ^^^^exactly ;)
			jmp	AllowPatchAndPatch
@@Good_Code:
			push	eax
			push	ebx
			push	ecx
			push	edx
			nop
Init_SwitchFillers_WIN32 ENDP





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Allow patches for all fillers                                            บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
Init_Fillers_WIN32	PROC
			pushad
					; *** Poly ***
			mov	eax,Offset First_FillerPatch
			mov	edx,Offset Last_FillerPatch
			call	AllowPatch

					; *** Flat ***
			mov	eax,Offset First_POLYFLATPatch
			mov	edx,Offset Last_POLYFLATPatch
			call	AllowPatch

					; *** Disc ***
			mov	eax,Offset First_POLYDISCPatch
			mov	edx,Offset Last_POLYDISCPatch
			call	AllowPatch

					; *** Line ***
			mov	eax,Offset First_POLYLINEPatch
			mov	edx,Offset Last_POLYLINEPatch
			call	AllowPatch

					; *** Gouraud ***
			mov	eax,Offset First_POLYGOURAUDPatch
			mov	edx,Offset Last_POLYGOURAUDPatch
			call	AllowPatch

					; *** Texture ***
			mov	eax,Offset First_POLYTEXTUREPatch
			mov	edx,Offset Last_POLYTEXTUREPatch
			call	AllowPatch

					; *** Gouraud Texture ***
			mov	eax,Offset First_POLYGOURAUDTEXTUREPatch
			mov	edx,Offset Last_POLYGOURAUDTEXTUREPatch
			call	AllowPatch

					; *** Texture Z ***
			mov	eax,Offset First_POLYTEXTUREZPatch
			mov	edx,Offset Last_POLYTEXTUREZPatch
			call	AllowPatch

					; *** Texture Z Gouraud ***
			mov	eax,Offset First_POLYTEXTUREZGOURAUDPatch
			mov	edx,Offset Last_POLYTEXTUREZGOURAUDPatch
			call	AllowPatch

					; *** Texture Z Fog ***
			mov	eax,Offset First_POLYTEXTUREZFOGPatch
			mov	edx,Offset Last_POLYTEXTUREZFOGPatch
			call	AllowPatch

			popad
			ret
Init_Fillers_WIN32	ENDP



ELSE	; _WIN32





; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Initialize 'Fill_Poly' under DOS                                         บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
InitFillPoly_DOS	PROC
			pushad
			mov	ebx,Offset TabOffLine
			call	SetScreenPitch

			mov	eax,dword ptr[Good_CodeFillPoly]
			mov	dword ptr [Fill_PolyFast],eax
			mov	al,byte ptr[Good_CodeFillPoly+4]
			mov	byte ptr [Fill_PolyFast+4],al

			popad
			jmp	Fill_PolyFast
Good_CodeFillPoly:
			push	ebp			; ** THERE MUST BE AT LEAST
			xor	ebp,ebp			; ** 5 BYTES B4 THE
			mov	[Fill_LeftSlope],ebp	; ** PATCH CODE
InitFillPoly_DOS	ENDP


ENDIF	; _WIN32



_TEXT			ENDS




;			The
			End
