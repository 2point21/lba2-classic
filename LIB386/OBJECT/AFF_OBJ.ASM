; ╔════════════════════════════════════════════════════════════════════════════╗
; ║                                                                            ║
; ║  AFFICHAGE D'OBJETS 3D                                                     ║
; ║                                                                            ║
; ║                                                                            ║
; ║                                                                            ║
; ╟────────────────────────────────────────────────────────────────────────────╢
; ║  Version: Oct 1996 / Jan 1997                                              ║
; ╚════════════════════════════════════════════════════════════════════════════╝


; This flag must be defined if you want to bypass the standard polygon engine

ENABLE_INDIRECT_FILL_POLY EQU	0



JMP_FILL_POLY		MACRO
IFDEF	ENABLE_INDIRECT_FILL_POLY
			jmp	[Fill_Poly]
ELSE
			jmp	Fill_PolyFast
ENDIF
			ENDM



			.486P
;			.MODEL	FLAT, SYSCALL

			OPTION	PROC:PRIVATE
			OPTION	SCOPED
			OPTION	LANGUAGE:SYSCALL


; ╔════════════════════════════════════════════════════════════════════════════╗
; ║                                                                            ║
; ║  Les EQUATES, MACROS et STRUCTURES                                         ║
; ║                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════╝

INCLUDE			IAMMX.INC
INCLUDE			AFF_OBJ.INC
INCLUDE			FILLER.INC
INCLUDE			3D.INC
INCLUDE			MAT.INC
INCLUDE			CPU.INC



PUBLIC	SYSCALL		BodyDisplay
PUBLIC	SYSCALL		BodyDisplay_AlphaBeta
PUBLIC	SYSCALL		ObjectDisplay
PUBLIC	SYSCALL		ObjPtrMap
PUBLIC	SYSCALL		TransFctBody
PUBLIC	SYSCALL		Obj_ListRotatedPoints


W_NORM			EQU	(1 SHL 30)



; Call  : EBX=Ptr on 1st point
;         ECX=Ptr on 2nd point
;         EBP=Ptr on 3rd point
; Return:
;         AX =ZFar

TEST_ZFAR3		MACRO
Local			@@Sup1, @@Sup2
						; Find the furthest point
			mov	ax,[Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			cmp	ax,[Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
			jle	@@Sup1
			mov	ax,[Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
@@Sup1:
			cmp	ax,[Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ
			jle	@@Sup2
			mov	ax,[Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ
@@Sup2:
			ENDM






TEST_ZFAR4		MACRO
Local			@@Sup1, @@Sup2, @@Sup3
						; Find the furthest point
			mov	ax,[Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ
			mov	ebx,dword ptr [esi].Poly4_P3

			cmp	ax,[Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
			jle	@@Sup1
			mov	ax,[Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
@@Sup1:
			shr	ebx,16
			cmp	ax,[Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ
			jle	@@Sup2
			mov	ax,[Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ
@@Sup2:
			cmp	ax,[Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ
			jle	@@Sup3
			mov	ax,[Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ
@@Sup3:
			ENDM






INSERT_TRIANGLE		MACRO	Type
Local			@@Do_Triangle, @@Next_Triangle
						; *** Triangle ***
@@Do_Triangle:
			push	ecx
			call	[TestVisible]
			jc	@@Next_Triangle		; Carry = 0 if visible

			TEST_ZFAR3			; AX = ZFar

			shl	eax,16
			mov	ebx,[Off_Sort]		; offset of sort list

			mov	edx,[Nb_Sort]		; Number of entries in sort list
			mov	ecx,[esp+4]		; Poly type

			shl	ecx,26			; Poly type in bit 24-31
			or	eax,edx			; EAX = | ZFar | Poly Number |
IF Type EQ 2
			or	ecx,10b shl 24		; Bit environment
ENDIF
			mov	[ebx],eax
			or	ecx,edi			; ECX = | PolyType (8 bit) | Poly Offset (24 bit) |

			inc	edx
			mov	[ebx+MAX_NB_POLYS*4],ecx

			add	ebx,4
			mov	[Nb_Sort],edx

			mov	[Off_Sort],ebx
@@Next_Triangle:
			pop	ecx

IF Type EQ 0
			add	esi,Size STRUC_POLY3_LIGHT
			add	edi,Size STRUC_POLY3_LIGHT
ELSEIF Type EQ 1
			add	esi,Size STRUC_POLY3_TEXTURE
			add	edi,Size STRUC_POLY3_TEXTURE
ELSE
			add	esi,Size STRUC_POLY3_ENV
			add	edi,Size STRUC_POLY3_ENV
ENDIF
			dec	ecx
			jne	@@Do_Triangle

			pop	eax			; POP Poly type
			jmp	@@Loop_Polys
			ENDM




INSERT_QUAD		MACRO	Type
Local			@@Do_Quad, @@Next_Quad
						; *** Triangle ***
@@Do_Quad:
			push	ecx
			call	[TestVisible]
			jc	@@Next_Quad		; Carry = 0 if visible


			TEST_ZFAR4			; AX = ZFar
							; EBX = P4
			shl	eax,16
			mov	ebx,dword ptr [Obj_ListProjectedPoints+ebx*4].Pt_XE

			cmp	ebx,80008000h
			je	@@Next_Quad

			mov	ebx,[Off_Sort]		; offset of sort list
			mov	ecx,[esp+4]		; Poly type

			shl	ecx,26			; Poly type in bit 24-31
			mov	edx,[Nb_Sort]		; Number of entries in sort list

			or	eax,edx			; EAX = | ZFar | Poly Number |
IF Type EQ 2
			or	ecx,11b shl 24		; Bit quad+environment
ELSE
			or	ecx,01b shl 24		; Bit quad
ENDIF
			mov	[ebx],eax
			or	ecx,edi			; ECX = | PolyType (8 bit) | Poly Offset (24 bit) |

			inc	edx
			mov	[ebx+MAX_NB_POLYS*4],ecx

			add	ebx,4
			mov	[Nb_Sort],edx

			mov	[Off_Sort],ebx
@@Next_Quad:
			pop	ecx

IF Type EQ 0
			add	esi,Size STRUC_POLY4_LIGHT
			add	edi,Size STRUC_POLY4_LIGHT
ELSEIF Type EQ 1
			add	esi,Size STRUC_POLY4_TEXTURE
			add	edi,Size STRUC_POLY4_TEXTURE
ELSE
			add	esi,Size STRUC_POLY4_ENV
			add	edi,Size STRUC_POLY4_ENV
ENDIF
			dec	ecx
			jne	@@Do_Quad

			pop	eax			; POP Poly type
			jmp	@@Loop_Polys
			ENDM




; Calculate, if needed, the Zbuf value on each vertex of the poly
GET_ZO3			MACRO	Dst, P1, P2, P3
Local			@@NoZBuf
			cmp	[Fill_Flag_ZBuffer],TRUE
			jne	@@NoZBuf

			cmp	[TypeProj],TYPE_3D
			jne	@@NoZBuf

			movsx	eax,word ptr [Obj_ListRotatedPoints+P1*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst].Pt_ZO,ax

			movsx	eax,word ptr [Obj_ListRotatedPoints+P2*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst+Size STRUC_POINT].Pt_ZO,ax

			movsx	eax,word ptr [Obj_ListRotatedPoints+P3*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst+2*(Size STRUC_POINT)].Pt_ZO,ax
@@NoZBuf:
			ENDM


GET_ZO4			MACRO	Dst, P1, P2, P3, P4
Local			@@NoZBuf
			cmp	[Fill_Flag_ZBuffer],TRUE
			jne	@@NoZBuf

			cmp	[TypeProj],TYPE_3D
			jne	@@NoZBuf

			movsx	eax,word ptr [Obj_ListRotatedPoints+P1*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst].Pt_ZO,ax

			movsx	eax,word ptr [Obj_ListRotatedPoints+P2*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst+Size STRUC_POINT].Pt_ZO,ax

			movsx	eax,word ptr [Obj_ListRotatedPoints+P3*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst+2*(Size STRUC_POINT)].Pt_ZO,ax

			movsx	eax,word ptr [Obj_ListRotatedPoints+P4*8].Obj_Pt_PZ
			neg	eax
			sub	eax,[PosZWr]
			imul	eax,[Fill_ZBuffer_Factor]
			sar	eax,16
			mov	[Dst+3*(Size STRUC_POINT)].Pt_ZO,ax
@@NoZBuf:
			ENDM














; ╔════════════════════════════════════════════════════════════════════════════╗
; ║                                                                            ║
; ║  Les DATA                                                                  ║
; ║                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════╝

;			.DATA
DGROUP			GROUP	_DATA
_DATA			SEGMENT	USE32 PUBLIC PARA 'DATA'

Extrn	SYSCALL		ProcessorFeatureFlags:	DWORD	; Contains MMX flag


Extrn	SYSCALL		LongRotatePoint	:	DWORD	; Point rotation (32 bit)
Extrn	SYSCALL		RotatePoint	:	DWORD	; Point rotation (16 bit)
Extrn	SYSCALL		RotatePointNoMMX:	DWORD	; Point rotation (16 bit)
Extrn	SYSCALL		RotateMatrix	:	DWORD	; Matrix rotation
Extrn	SYSCALL		CopyMatrix	:	DWORD	; Matrix copy
Extrn	SYSCALL		RotTransList	:	DWORD	; Rotation of list of point (16 bit)
Extrn	SYSCALL		ProjectList	:	DWORD	; Projection of a list of points
							; The points are first translated:
							; XE = Lx*(Xlist+TX-CameraXr)/(Zlist+TZ-CameraZr)
							; YE = Ly*(Ylist+TY-CameraYr)/(Zlist+TZ-CameraZr)

Extrn	SYSCALL		LightList	:	DWORD	; Calculates the light at each point of the list

Extrn	SYSCALL		InitMatrixTrans	:	DWORD	; Initialize the translation scalars for a given matrix



Extrn	SYSCALL		ScreenXMin	:	DWORD	; Screen size
Extrn	SYSCALL		ScreenXMax	:	DWORD	; Screen size
Extrn	SYSCALL		ScreenYMin	:	DWORD	; Screen size
Extrn	SYSCALL		ScreenYMax	:	DWORD	; Screen size

Extrn	SYSCALL		ClipXMin	:	DWORD	; Clip coord
Extrn	SYSCALL		ClipXMax	:	DWORD	; Clip coord
Extrn	SYSCALL		ClipYMin	:	DWORD	; Clip coord
Extrn	SYSCALL		ClipYMax	:	DWORD	; Clip coord

Extrn	SYSCALL		TypeProj	:	DWORD	; Projection type
Extrn	SYSCALL		MatriceWorld	:	DWORD	; World matrix

Extrn	SYSCALL		X0		:	DWORD	; Return value of RotatePoint
Extrn	SYSCALL		Y0		:	DWORD	; Return value of RotatePoint
Extrn	SYSCALL		Z0		:	DWORD	; Return value of RotatePoint

				; poly.asm
IFDEF	ENABLE_INDIRECT_FILL_POLY
Extrn	SYSCALL		Fill_Poly	:	DWORD
ENDIF
Extrn	SYSCALL		RepMask		:	DWORD	; Repeat zone for texture mapping
Extrn	SYSCALL		PtrMap		:	DWORD	; Ptr of texture
Extrn	SYSCALL		Fill_Flag_ZBuffer:	BYTE	; TRUE if Zbuffer active
Extrn	SYSCALL		Fill_ZBuffer_Factor:	DWORD	; ZBuffer factor to have a 16 bit zbuf
Extrn	SYSCALL		Fill_Flag_NZW	:	BYTE	; TRUE if no Z Buffer updates


				; Camera.asm
Extrn	SYSCALL		CameraXr	:	DWORD	; Camera coord in observer space
Extrn	SYSCALL		CameraYr	:	DWORD
Extrn	SYSCALL		CameraZr	:	DWORD
Extrn	SYSCALL		NearClip	:	DWORD
Extrn	SYSCALL		CameraZrClip	:	DWORD

Extrn	SYSCALL		LFactorX	:	DWORD


				; Chooser.asm
Extrn	SYSCALL		Chooser_RtnI	:	DWORD
Extrn	SYSCALL		Chooser_RtnF	:	DWORD
Extrn	SYSCALL		Chooser_RtnM	:	DWORD
Extrn	SYSCALL		Chooser_Result	:	DWORD



			ALIGN	4
TestVisible		dd	Init_TestVisible	; Fastest method

PtrTextures		dd	0		; Ptr of texture handles
ObjPtrMap		dd	0		; Ptr of the texture page for the current object

lBody			dd	0		; Ptr of the object

Off_Sort		dd	0		; Current offset in sort list
Nb_Sort			dd	0		; Nb entries in sort list


PosXWr			dd	0		; Object position in world space
PosYWr			dd	0
PosZWr			dd	0

			ALIGN	8
				; Matrix list
TabMat			db	SIZE_MATRICE*MAX_NB_GROUPES dup (?)

			ALIGN	8
				; Rotated points list
Obj_ListRotatedPoints	STRUC_OBJ_POINT MAX_NB_POINTS dup (<>)

			ALIGN	8
				; Projected points list
Obj_ListProjectedPoints	STRUC_PT 	MAX_NB_POINTS dup (<>)

			ALIGN	8
				; 4 first are source normals; 4 last are rotated ones
				; (used for env. mapping)
ListNormEnv		dq	8 dup (?)


				; Light list
ListLights		dw	MAX_NB_POINTS+MAX_NB_POLYS dup (?)

			ALIGN	4
				; Sort list:
				;	* First half of list contain Z:Number
				;	* Second half: one pointer for each number
ListSort		dd	MAX_NB_POLYS*2 dup (?)

			ALIGN	8
				; List of vertex for Fill_Poly
ListFillPoly		STRUC_POINT 4 dup (<>)


			ALIGN	4
TransX			dd	0	; Temporary translation components
TransY			dd	0
TransZ			dd	0

ListGroupAnim		dd	0	; Current frame anim info
PtrPoints		dd	0	; Current point pointer
PtrNormales		dd	0	; Current normals pointer
PtrNormFaces		dd	0	; Current norm-faces pointer
PtrRotPoints		dd	0	; Current rotated points pointer
PtrListLights		dd	0	; Current lights pointer
PtrListLightsFaces	dd	0	; Current light-faces pointer
NbGroupes		dd	0	; Nb of groups in object
PtrGroupes		dd	0	; Pointer on groups definition
Current_Matrix		dd	0	; Pointer on current matrix
TransFctBody		dd	0	; Pointer to a conversion fct that converts a body number into its address






Table_Matrix		Label	DWORD
AA			=	0
REPT	MAX_GROUPS
			dd	Offset TabMat + AA
AA			=	AA+SIZE_MATRICE
ENDM



_DATA			ENDS


; ╔════════════════════════════════════════════════════════════════════════════╗
; ║                                                                            ║
; ║  Le CODE                                                                   ║
; ║                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════╝

_TEXT			SEGMENT	USE32 PARA PUBLIC 'CODE'
			ASSUME	CS:_TEXT, DS:DGROUP, ES:DGROUP, SS:DGROUP
;			.CODE


IFNDEF	ENABLE_INDIRECT_FILL_POLY
Extrn	SYSCALL		Fill_PolyFast	:	PROC
ENDIF
Extrn	SYSCALL		Fill_Sphere	:	PROC
Extrn	SYSCALL		Line_A		:	PROC
Extrn	SYSCALL		Chooser		:	PROC




; ╔════════════════════════════════════════════════════════════════════════════╗
; ║ BodyDisplay: Affiche un objet non animé                                    ║
; ╟────────────────────────────────────────────────────────────────────────────╢
; ║ Appel : (EAX, EBX, ECX) = (X, Y, Z)                                        ║
; ║         (EDX, ESI, EDI) = (Alpha, Beta, Gamma)                             ║
; ║         Stack           = Off_Body                                         ║
; ║                                                                            ║
; ║                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════╝

BodyDisplay		PROC
			push	ebp
			push	edx			; Push angles

			push	esi
			push	edi

			mov	ebp,[esp+4+4*4]		; EBP = PTR on object
			mov	edx,7FFFFFFFh

			mov	[lBody],ebp
			mov	[ScreenXMin],edx	; Init 2D box

			mov	[ScreenYMin],edx
			inc	edx			; edx = 80000000h

			mov	[ScreenXMax],edx
			mov	edi,[TypeProj]

			mov	[ScreenYMax],edx
			cmp	edi,TYPE_3D

			jne	@@Init_Iso		; Object position

			push	esi
			mov	esi,Offset MatriceWorld

			call	[LongRotatePoint]	; If perspective

			pop	esi
			mov	eax,[X0]

			mov	ebx,[Y0]
			mov	ecx,[Z0]
@@Init_Iso:
			mov	edi,[CameraXr]

			sub	eax,edi
			mov	edi,[CameraYr]

			sub	ebx,edi
			mov	edi,[CameraZr]

			sub	ecx,edi
			mov	[PosXWr],eax		; Object world position

			mov	[PosYWr],ebx
			mov	[PosZWr],ecx


; ┌─────────────────────────────────────────────────────────────────┐
; │                                                                 │
; │                          GEOMETRY                               │
; │                                                                 │
; └─────────────────────────────────────────────────────────────────┘

			; **************
			; *** MATRIX ***
			; **************

			mov	esi,Offset MatriceWorld

			mov	edi,Offset TabMat
			pop	ecx			; Get angles

			pop	ebx
			pop	eax
Return_BodyDisplay::
			call	[RotateMatrix]		; Mat_World*Mat_Obj

			; **************
			; *** POINTS ***
			; **************

			mov	ecx,[ebp].Obj_Nb_Points
			mov	eax,[ebp].Obj_Off_Points

			mov	edi,Offset Obj_ListRotatedPoints
			mov	esi,ebp

			add	esi,eax
			mov	ebx,Offset TabMat

			call	[RotTransList]		; Transformation



			xor	eax,eax
			mov	esi,[PosZWr]

			sub	eax,esi			; OrgZ=-PosZWr
			mov	esi,[PosYWr]

			push	eax			; Push OrgZ
			xor	eax,eax

			sub	eax,esi			; OrgY=-PosYWr
			mov	esi,[PosXWr]

			push	eax			; Push OrgY
			xor	eax,eax

			sub	eax,esi			; OrgX=-PosXWr
			mov	esi,Offset Obj_ListRotatedPoints

			push	eax			; Push OrgX
			mov	edi,Offset Obj_ListProjectedPoints

			mov	ecx,[ebp].Obj_Nb_Points
			call	[ProjectList]		; Projection

			add	esp,3*4			; Pop parameters



							; Is the object visible?
			mov	eax,[ScreenXMin]
			mov	ebx,[ClipXMax]

			cmp	eax,ebx
			jg	ByeBye

			mov	eax,[ScreenYMin]
			mov	ebx,[ClipYMax]

			cmp	eax,ebx
			jg	ByeBye

			mov	eax,[ScreenXMax]
			mov	ebx,[ClipXMin]

			cmp	eax,ebx
			jl	ByeBye

			mov	eax,[ScreenYMax]
			mov	ebx,[ClipYMin]

			cmp	eax,ebx
			jl	ByeBye



			; *****************
			; *** LIGHTING  ***
			; *****************

						; *** Calculate the intensities ***
			mov	ebx,Offset TabMat
			mov	esi,[ebp].Obj_Off_Normales	; First list of normals

			mov	edi,Offset ListLights
			mov	ecx,[ebp].Obj_Nb_Normales

			add	esi,ebp
			push	ecx

			call	[LightList]

			pop	ecx
			mov	esi,[ebp].Obj_Off_NormFaces

			mov	ebx,Offset TabMat
			add	esi,ebp

			lea	edi,[Offset ListLights+ecx*2]
			mov	ecx,[ebp].Obj_Nb_NormFaces

			call	[LightList]

; ┌─────────────────────────────────────────────────────────────────┐
; │                                                                 │
; │                     ENTITY LIST GENERATION                      │
; │                                                                 │
; └─────────────────────────────────────────────────────────────────┘
EntityCreation::
			CLEAN_MMX			; ObjectDisplay should
							; do the EMMS
			; *****************
			; *** POLYGONES ***
			; *****************

			mov	eax,Offset ListSort	; Init some var
			xor	ebx,ebx

			mov	[Off_Sort],eax
			mov	[Nb_Sort],ebx

			mov	edi,[ebp].Obj_Off_Polys ; EDI = Current offset in poly list
			mov	eax,[ebp].Obj_Off_Lines

			mov	esi,edi
			add	eax,ebp			; End source offset

			add	esi,ebp			; Start source offset
			push	eax			; Push it as a local variable
@@Loop_Polys:
			cmp	esi,[esp]		; Faster on a 486, too bad 4 the Pentium
			je	@@Do_Lines		; Is there at least a poly ?

			mov	ecx,dword ptr [esi].Obj_TypePoly
			add	esi,Size STRUC_POLY_HEADER

			mov	eax,ecx
			mov	edx,ecx

			shr	ecx,16			; ECX = Nb polys
			and	edx,0FFh		; EDX = Polys type
							; EAX = Type bit field

			push	edx			; Push current poly type
			add	edi,Size STRUC_POLY_HEADER


					; *** Type dispatch ***

			test	eax,MASK_QUADRILATERE	; Quad?
			jne	@@Do_Quad
							; Dispatch a triangle
			test	eax,MASK_ENVIRONMENT
			jne	@@Do_Triangle_Env

			cmp	edx,POLY_DITHER_TABLE
			jbe	@@Do_Triangle_Light
			jmp	@@Do_Triangle_Text
@@Do_Quad:
							; Dispatch a Quad
			test	eax,MASK_ENVIRONMENT
			jne	@@Do_Quad_Env

			cmp	edx,POLY_DITHER_TABLE
			jbe	@@Do_Quad_Light

						; *** Quad ***
@@Do_Quad_Text:
			INSERT_QUAD	1
@@Do_Quad_Light:
			INSERT_QUAD	0
@@Do_Quad_Env:
			INSERT_QUAD	2

						; *** Triangle ***
@@Do_Triangle_Light:
			INSERT_TRIANGLE	0
@@Do_Triangle_Text:
			INSERT_TRIANGLE	1
@@Do_Triangle_Env:
			INSERT_TRIANGLE	2




			; *************
			; *** LINES ***
			; *************
@@Do_Lines:
			mov	ebp,[lBody]
			pop	eax			; POP End loop pointer

			mov	ecx,[ebp].Obj_Nb_Lines
			test	ecx,ecx
			je	@@Do_Spheres		; Is there anything to do?
@@Loop_Lines:
			mov	ebx,dword ptr [esi].Obj_Line_P1
			mov	ebp,ebx

			shr	ebp,16			; EBP = P2
			and	ebx,0FFFFh		; EBX = P1

			mov	ax,[Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ
			push	ecx

			cmp	ax,[Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ
			jle	@@Line_Sup
			mov	ax,[Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ
@@Line_Sup:
			shl	eax,16
			mov	ebx,[Off_Sort]		; offset of sort list

			mov	edx,[Nb_Sort]		; Number of entries in sort list
			mov	ecx,dword ptr [esi].Obj_LineType ; Line type

			add	ecx,MAX_POLYS_TYPE*4	; Get correct pos for a line
			or	eax,edx			; EAX = | ZFar | Poly Number |

			shl	ecx,24			; Line type in bit 24-31
			mov	[ebx],eax

			or	ecx,edi			; ECX = | PolyType (8 bit) | Poly Offset (24 bit) |
			inc	edx

			mov	[ebx+MAX_NB_POLYS*4],ecx
			add	ebx,4

			mov	[Nb_Sort],edx
			mov	[Off_Sort],ebx

			pop	ecx
			add	esi,Size STRUC_OBJ_LINE
			add	edi,Size STRUC_OBJ_LINE

			dec	ecx
			jne	@@Loop_Lines


			; ***************
			; *** SPHERES ***
			; ***************
@@Do_Spheres:
			mov	ebp,[lBody]
				; AGI :(
			mov	ecx,[ebp].Obj_Nb_Spheres
			test	ecx,ecx
			je	@@Do_Sort		; Is there anything to do?

@@Loop_Spheres:
			push	ecx
			mov	ebx,dword ptr [esi].Obj_Sphere_P1

			and	ebx,0FFFFh		; EBX = P1
			mov	edx,[Nb_Sort]		; Number of entries in sort list

			mov	eax,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ
			mov	ebx,[Off_Sort]		; offset of sort list

			shl	eax,16
			mov	ecx,dword ptr [esi].Obj_SphereType ; Sphere type

							; Get correct pos for a sphere
			add	ecx,MAX_POLYS_TYPE*4+MAX_LINES_TYPE
			or	eax,edx			; EAX = | ZFar | Poly Number |

			shl	ecx,24			; Poly type in bit 24-31
			mov	[ebx],eax

			or	ecx,edi			; ECX = | PolyType (8 bit) | Poly Offset (24 bit) |
			inc	edx

			mov	[ebx+MAX_NB_POLYS*4],ecx
			add	ebx,4

			mov	[Nb_Sort],edx
			mov	[Off_Sort],ebx

			pop	ecx
			add	esi,Size STRUC_OBJ_SPHERE
			add	edi,Size STRUC_OBJ_SPHERE

			dec	ecx
			jne	@@Loop_Spheres


; ┌─────────────────────────────────────────────────────────────────┐
; │                                                                 │
; │                      ENTITIES SORTING                           │
; │                                                                 │
; └─────────────────────────────────────────────────────────────────┘

@@Do_Sort:
			mov	esi,[lBody]
			mov	ebx,Offset ListSort

			mov	ecx,[Off_Sort]
			cmp	ebx,ecx			; Anything to do ?

			mov	eax,[esi].Obj_Info
			je	ByeBye

			sub	ecx,4			; EBX = PTR on the fist element
			mov	dl,[Fill_Flag_ZBuffer]

			test	eax,MASK_OBJECT_NO_SORT
			jne	@@NoSort

			cmp	dl,TRUE			; If ZBuffer then sort from near to far
			je	@@SortZBuffer
@@SortNormal:
			call	QuickSort		; ECX = PTR on the last element
			jmp	@@NoSort
@@SortZBuffer:
			test	eax,MASK_OBJECT_TRANSPARENT; If ZBuffer and Transparent then sort from far to near
			jne	@@SortNormal

			mov	dl,[Fill_Flag_NZW]	; If ZBuffer and No Z-Write then
			cmp	dl,TRUE			; sort the object from far to near
			je	@@SortNormal
			call	QuickSortInv
@@NoSort:

; ┌─────────────────────────────────────────────────────────────────┐
; │                                                                 │
; │                             DISPLAY                             │
; │                                                                 │
; └─────────────────────────────────────────────────────────────────┘

			mov	ebp,[lBody]
			mov	esi,Offset ListSort

			mov	ecx,[Nb_Sort]
			mov	eax,[ebp].Obj_Off_Textures

			mov	ebx,esi
			add	eax,ebp

			add	ebx,MAX_NB_POLYS*4
			mov	[PtrTextures],eax
@@Do_Display:
			mov	eax,[esi]		; Get poly number
			push	ebx

			and	eax,0FFFFh
			push	ebp
			; AGI
			mov	eax,[ebx+eax*4]		; Get poly info
			add	esi,4

			push	esi
			mov	edx,eax

			shr	eax,24			; EAX = Poly type
			and	edx,0FFFFFFh		; EDX = Poly offset

			add	ebp,edx			; EBP = Poly address
			push	ecx

			jmp	[@@Poly_Jump+eax*4]
@@Return_Display:
			pop	ecx
			pop	esi

			pop	ebp
			pop	ebx

			dec	ecx
			jne	@@Do_Display

			pop	ebp
			mov	eax,1			; Object displayed
			ret

ByeBye::
			pop	ebp
			xor	eax,eax			; Object not displayed
			ret


			ALIGN	4
@@Poly_Jump		dd	@@Triangle_Solid		;  0
			dd	@@Quad_Solid			;  0
			dd	@@Triangle_Solid		;  0
			dd	@@Quad_Solid			;  0

			dd	@@Triangle_Flat			;  1
			dd	@@Quad_Flat			;  1
			dd	@@Triangle_Flat			;  1
			dd	@@Quad_Flat			;  1

			dd	@@Triangle_Solid		;  2
			dd	@@Quad_Solid			;  2
			dd	@@Triangle_Solid		;  2
			dd	@@Quad_Solid			;  2

			dd	@@Triangle_Solid		;  3
			dd	@@Quad_Solid			;  3
			dd	@@Triangle_Solid		;  3
			dd	@@Quad_Solid			;  3

			dd	@@Triangle_Gouraud		;  4
			dd	@@Quad_Gouraud			;  4
			dd	@@Triangle_Gouraud		;  4
			dd	@@Quad_Gouraud			;  4

			dd	@@Triangle_Gouraud		;  5
			dd	@@Quad_Gouraud			;  5
			dd	@@Triangle_Gouraud		;  5
			dd	@@Quad_Gouraud			;  5

			dd	@@Triangle_Gouraud		;  6
			dd	@@Quad_Gouraud			;  6
			dd	@@Triangle_Gouraud		;  6
			dd	@@Quad_Gouraud			;  6

			dd	@@Triangle_Gouraud		;  7
			dd	@@Quad_Gouraud			;  7
			dd	@@Triangle_Gouraud		;  7
			dd	@@Quad_Gouraud			;  7

			dd	@@Triangle_Texture_Solid	;  8
			dd	@@Quad_Texture_Solid		;  8
			dd	@@Triangle_EnvTexture_Solid	;  8
			dd	@@Quad_EnvTexture_Solid		;  8

			dd	@@Triangle_Texture_Flat		;  9
			dd	@@Quad_Texture_Flat		;  9
			dd	@@Triangle_EnvTexture_Flat	;  9
			dd	@@Quad_EnvTexture_Flat		;  9

			dd	@@Triangle_Texture_Gouraud	; 10
			dd	@@Quad_Texture_Gouraud		; 10
			dd	@@Triangle_EnvTexture_Gouraud	; 10
			dd	@@Quad_EnvTexture_Gouraud	; 10

			dd	@@Triangle_Texture_Gouraud	; 11
			dd	@@Quad_Texture_Gouraud		; 11
			dd	@@Triangle_EnvTexture_Gouraud	; 11
			dd	@@Quad_EnvTexture_Gouraud	; 11

			dd	@@Triangle_Texture_Solid	; 12
			dd	@@Quad_Texture_Solid		; 12
			dd	@@Triangle_EnvTexture_Solid	; 12
			dd	@@Quad_EnvTexture_Solid		; 12

			dd	@@Triangle_Texture_Flat		; 13
			dd	@@Quad_Texture_Flat		; 13
			dd	@@Triangle_EnvTexture_Flat	; 13
			dd	@@Quad_EnvTexture_Flat		; 13

			dd	@@Triangle_Texture_Gouraud	; 14
			dd	@@Quad_Texture_Gouraud		; 14
			dd	@@Triangle_EnvTexture_Gouraud	; 14
			dd	@@Quad_EnvTexture_Gouraud	; 14

			dd	@@Triangle_Texture_Gouraud	; 15
			dd	@@Quad_Texture_Gouraud		; 15
			dd	@@Triangle_EnvTexture_Gouraud	; 15
			dd	@@Quad_EnvTexture_Gouraud	; 15

			dd	@@Triangle_TextureZ_Solid	; 16
			dd	@@Quad_TextureZ_Solid		; 16
			dd	@@Triangle_EnvTexture_Solid	; 16
			dd	@@Quad_EnvTexture_Solid		; 16

			dd	@@Triangle_TextureZ_Flat	; 17
			dd	@@Quad_TextureZ_Flat		; 17
			dd	@@Triangle_EnvTexture_Flat	; 17
			dd	@@Quad_EnvTexture_Flat		; 17

			dd	@@Triangle_TextureZ_Gouraud	; 18
			dd	@@Quad_TextureZ_Gouraud		; 18
			dd	@@Triangle_EnvTexture_Gouraud	; 18
			dd	@@Quad_EnvTexture_Gouraud	; 18

			dd	@@Triangle_TextureZ_Gouraud	; 19
			dd	@@Quad_TextureZ_Gouraud		; 19
			dd	@@Triangle_EnvTexture_Gouraud	; 19
			dd	@@Quad_EnvTexture_Gouraud	; 19

			dd	@@Triangle_TextureZ_Solid	; 20
			dd	@@Quad_TextureZ_Solid		; 20
			dd	@@Triangle_EnvTexture_Solid	; 20
			dd	@@Quad_EnvTexture_Solid		; 20

			dd	@@Triangle_TextureZ_Flat	; 21
			dd	@@Quad_TextureZ_Flat		; 21
			dd	@@Triangle_EnvTexture_Flat	; 21
			dd	@@Quad_EnvTexture_Flat		; 21

			dd	@@Triangle_TextureZ_Gouraud	; 22
			dd	@@Quad_TextureZ_Gouraud		; 22
			dd	@@Triangle_EnvTexture_Gouraud	; 22
			dd	@@Quad_EnvTexture_Gouraud	; 22

			dd	@@Triangle_TextureZ_Gouraud	; 23
			dd	@@Quad_TextureZ_Gouraud		; 23
			dd	@@Triangle_EnvTexture_Gouraud	; 23
			dd	@@Quad_EnvTexture_Gouraud	; 23



			dd	@@Line				; Line
			dd	@@Sphere			; Sphere
			dd	@@Sphere_Transp			; Sphere transparent



			; *************
			; *** SOLID ***
			; *************
@@Triangle_Solid:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]
			mov	ebx,dword ptr [ebp].Poly3_Couleur

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	ecx,3

			pop	eax
			and	ebx,0FFh

			push	Offset @@Return_Display
			JMP_FILL_POLY

@@Quad_Solid:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]
			mov	ebx,dword ptr [ebp].Poly4_Couleur

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax
			mov	ecx,4

			pop	eax
			and	ebx,0FFh

			push	Offset @@Return_Display
			JMP_FILL_POLY




			; ************
			; *** FLAT ***
			; ************
@@Triangle_Flat:
			mov	ebx,dword ptr [ebp].Poly3_P1
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	ebx,dword ptr [ebp].Poly3_Couleur

			mov	edx,[Obj_ListProjectedPoints+edi*4]
			mov	eax,ebx

			shr	eax,16				; Face normal number
			push	Offset @@Return_Display

			mov	ax,[ListLights+eax*2]		; No AGI thanx to prefix ;)
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,edx

			shr	eax,8
			mov	ecx,3

			add	ebx,eax
			mov	eax,1

			and	ebx,0FFh
			JMP_FILL_POLY


@@Quad_Flat:
			mov	ebx,dword ptr [ebp].Poly4_P1
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	ebx,dword ptr [ebp].Poly4_Couleur

			mov	eax,ebx
			mov	ecx,[Obj_ListProjectedPoints+edx*4]

			shr	eax,16				; EAX = Face normal number
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,ecx

			mov	ax,[ListLights+eax*2]
			mov	ecx,4

			shr	eax,8
			push	Offset @@Return_Display

			add	ebx,eax
			mov	eax,1

			and	ebx,0FFh
			JMP_FILL_POLY




			; ***************
			; *** GOURAUD ***
			; ***************
@@Triangle_Gouraud:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dx,[ListLights+ebx*2]
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			mov	ax,[ListLights+ecx*2]
			mov	ecx,3

			mov	[esi].Pt_Light,dx
			mov	ebx,dword ptr [ebp].Poly3_Couleur

			mov	[esi+Size STRUC_POINT].Pt_Light,ax
			pop	eax

			mov	dx,[ListLights+edi*2]
			push	Offset @@Return_Display

			mov	[esi+2*(Size STRUC_POINT)].Pt_Light,dx
			JMP_FILL_POLY

@@Quad_Gouraud:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	bx,[ListLights+ebx*2]
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax

			mov	[esi].Pt_Light,bx
			pop	eax

			mov	cx,[ListLights+ecx*2]
			mov	ebx,dword ptr [ebp].Poly4_Couleur

			mov	[esi+Size STRUC_POINT].Pt_Light,cx
			and	ebx,0FFh

			mov	cx,[ListLights+edi*2]
			push	Offset @@Return_Display

			mov	[esi+2*(Size STRUC_POINT)].Pt_Light,cx
			mov	ecx,4

			mov	dx,[ListLights+edx*2]

			mov	[esi+3*(Size STRUC_POINT)].Pt_Light,dx
			JMP_FILL_POLY



			; *********************
			; *** TEXTURE SOLID ***
			; *********************
@@Triangle_Texture_Solid:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly3_P3	; Get texture handle

			shr	edx,16-2
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			and	edx,not 11b
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			add	edx,[PtrTextures]
			mov	eax,dword ptr [ebp].Poly3_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly3_U2

			mov	edx,[edx]			; Get texture infos
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			mov	ecx,edx
			mov	eax,dword ptr [ebp].Poly3_U3

			shr	ecx,16				; ECX = RepMask
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ecx
			add	edx,eax

			mov	[PtrMap],edx
			pop	eax

			mov	ecx,3
			push	Offset @@Return_Display

			JMP_FILL_POLY


@@Quad_Texture_Solid:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	ebx,dword ptr [ebp].Poly4_HandleText

			mov	eax,[Obj_ListProjectedPoints+ecx*4]
			and	ebx,0FFFFh

			shl	ebx,2
			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax

			add	ebx,[PtrTextures]
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	ebx,[ebx]
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax

			mov	ecx,ebx
			mov	eax,dword ptr [ebp].Poly4_U1

			shr	ebx,16				; EBX = RepMask
			mov	dword ptr [esi].Pt_MapU,eax

			and	ecx,0FFFFh			; ECX = OrgMap
			mov	eax,dword ptr [ebp].Poly4_U2

			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U3

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U4

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,eax
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebx
			add	ecx,eax

			mov	[PtrMap],ecx
			pop	eax

			mov	ecx,4
			push	Offset @@Return_Display

			JMP_FILL_POLY


			; ********************
			; *** TEXTURE FLAT ***
			; ********************
@@Triangle_Texture_Flat:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly3_P3	; Get texture handle in high word

			shr	edx,16-2
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			and	edx,not 11b
			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax

			add	edx,[PtrTextures]
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,dword ptr [ebp].Poly3_U1

			mov	edx,[edx]			; get texture infos
			mov	ebx,dword ptr [ebp].Poly3_Couleur

			mov	ecx,edx
			mov	dword ptr [esi].Pt_MapU,eax

			shr	ebx,16
			mov	eax,dword ptr [ebp].Poly3_U2

			shr	edx,16
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			and	ecx,0FFFFh
			mov	eax,dword ptr [ebp].Poly3_U3

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			mov	eax,[ObjPtrMap]

			mov	[RepMask],edx
			add	ecx,eax

			mov	[PtrMap],ecx
			mov	ecx,3

			mov	bx,[ListLights+ebx*2]
			pop	eax

			shr	ebx,8
			push	Offset @@Return_Display

			JMP_FILL_POLY


@@Quad_Texture_Flat:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly4_HandleText	; Get texture handle

			shl	edx,2
			mov	eax,dword ptr [ebp].Poly4_U1

			add	edx,[PtrTextures]
			mov	dword ptr [esi].Pt_MapU,eax

			mov	ebx,dword ptr [ebp].Poly4_Couleur; Get normal in high word
			mov	eax,dword ptr [ebp].Poly4_U2

			shr	ebx,16
			mov	edx,[edx]			; Get texture infos

			mov	edi,edx
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			shr	edx,16
			mov	eax,dword ptr [ebp].Poly4_U3

			and	edi,0FFFFh
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax

			mov	[RepMask],edx
			mov	eax,dword ptr [ebp].Poly4_U4

			mov	bx,[ListLights+ebx*2]
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,eax

			mov	ecx,4				; Nb pts
			mov	eax,[ObjPtrMap]

			add	edi,eax
			pop	eax				; Poly type

			mov	[PtrMap],edi
			push	Offset @@Return_Display

			shr	ebx,8
			JMP_FILL_POLY




			; ***********************
			; *** TEXTURE GOURAUD ***
			; ***********************
@@Triangle_Texture_Gouraud:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly3_P3	; Get texture handle

			shr	edx,16-2
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			and	edx,not 11b
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			add	edx,[PtrTextures]
			mov	eax,dword ptr [ebp].Poly3_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly3_U2

			mov	bx,[ListLights+ebx*2]
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			mov	[esi].Pt_Light,bx
			mov	edx,[edx]			; Get texture infos

			mov	bx,[ListLights+ecx*2]
			mov	ecx,edx

			mov	[esi+Size STRUC_POINT].Pt_Light,bx
			mov	eax,dword ptr [ebp].Poly3_U3

			mov	bx,[ListLights+edi*2]
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax

			mov	[esi+2*(Size STRUC_POINT)].Pt_Light,bx
			and	edx,0FFFFh			; EDX = OrgMap

			shr	ecx,16				; ECX = RepMask
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ecx
			add	edx,eax

			mov	[PtrMap],edx
			pop	eax

			mov	ecx,3
			push	Offset @@Return_Display

			JMP_FILL_POLY


@@Quad_Texture_Gouraud:
			shr	eax,2
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	bx,[ListLights+ebx*2]
			mov	dword ptr [esi].Pt_XE,eax

			mov	[esi].Pt_Light,bx
			mov	ebx,dword ptr [ebp].Poly4_HandleText

			mov	eax,[Obj_ListProjectedPoints+ecx*4]
			and	ebx,0FFFFh

			shl	ebx,2
			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax

			mov	cx,[ListLights+ecx*2]
			add	ebx,[PtrTextures]

			mov	[esi+Size STRUC_POINT].Pt_Light,cx
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	di,[ListLights+edi*2]
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			mov	[esi+2*(Size STRUC_POINT)].Pt_Light,di
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dx,[ListLights+edx*2]
			mov	ebx,[ebx]

			mov	[esi+3*(Size STRUC_POINT)].Pt_Light,dx
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax

			mov	ecx,ebx
			mov	eax,dword ptr [ebp].Poly4_U1

			shr	ebx,16				; EBX = RepMask
			mov	dword ptr [esi].Pt_MapU,eax

			and	ecx,0FFFFh			; ECX = OrgMap
			mov	eax,dword ptr [ebp].Poly4_U2

			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U3

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U4

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,eax
			mov	eax,[ObjPtrMap]

			add	ecx,eax
			mov	[RepMask],ebx

			mov	[PtrMap],ecx
			pop	eax

			mov	ecx,4
			push	Offset @@Return_Display

			JMP_FILL_POLY


			; ***********************
			; *** TEXTURE Z SOLID ***
			; ***********************
@@Triangle_TextureZ_Solid:
			shr	eax,2				; Get poly type
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly3_P3	; Get texture handle

			shr	edx,16-2
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			and	edx,not 11b
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			add	edx,[PtrTextures]
			mov	eax,dword ptr [ebp].Poly3_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly3_U2

			mov	edx,[edx]			; Get texture infos
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			mov	eax,dword ptr [ebp].Poly3_U3
			mov	ebp,edx

			shr	ebp,16				; EBP = RepMask
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebp
			add	edx,eax

			mov	[PtrMap],edx
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	ebp,[PosZWr]

			sar	ebx,16
			xor	edx,edx

			mov	ecx,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
			add	ebx,ebp

			mov	eax,W_NORM
			mov	edi,dword ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ

			idiv	ebx

			shl	ecx,16
			mov	[esi].Pt_W,eax

			sar	ecx,16
			xor	edx,edx

			mov	eax,W_NORM
			add	ecx,ebp

			idiv	ecx

			shl	edi,16
			mov	[esi+(Size STRUC_POINT)].Pt_W,eax

			sar	edi,16
			xor	edx,edx

			mov	eax,W_NORM
			add	edi,ebp

			idiv	edi

			mov	[esi+2*(Size STRUC_POINT)].Pt_W,eax
			pop	eax

			mov	ecx,3
			push	Offset @@Return_Display

			JMP_FILL_POLY


@@Quad_TextureZ_Solid:
			shr	eax,2				; Get poly type
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+1*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax
			push	edx

			mov	edx,dword ptr [ebp].Poly4_HandleText; Get texture handle
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			and	edx,0FFFFh

			shl	edx,2
			mov	eax,[PtrTextures]

			add	edx,eax
			mov	eax,dword ptr [ebp].Poly4_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U2

			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U3

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			mov	edx,[edx]			; Get texture infos

			mov	eax,dword ptr [ebp].Poly4_U4
			mov	ebp,edx

			shr	ebp,16				; EBP = RepMask
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebp
			add	edx,eax

			mov	[PtrMap],edx
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			pop	ebp			; P4 in EBP

			idiv	ebx

			mov	[esi].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+(Size STRUC_POINT)].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+2*(Size STRUC_POINT)].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+3*(Size STRUC_POINT)].Pt_W,eax
			pop	eax

			mov	ecx,4
			push	Offset @@Return_Display

			JMP_FILL_POLY




			; **********************
			; *** TEXTURE Z FLAT ***
			; **********************

@@Triangle_TextureZ_Flat:
			shr	eax,2				; Get poly type
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	eax,dword ptr [ebp].Poly3_Couleur

			shr	eax,16				; EAX = Normal vector number
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	ax,[ListLights+eax*2]		; Get intensity
			mov	edi,dword ptr [ebp].Poly3_P3

			shr	eax,8				; Shift intensity
			lea	esi,ListFillPoly

			push	eax				; Push intensity
			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly3_P3	; Get texture handle

			shr	edx,16-2
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			and	edx,not 11b
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			add	edx,[PtrTextures]
			mov	eax,dword ptr [ebp].Poly3_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly3_U2

			mov	edx,[edx]			; Get texture infos
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			mov	eax,dword ptr [ebp].Poly3_U3
			mov	ebp,edx

			shr	ebp,16				; EBP = RepMask
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebp
			add	edx,eax

			mov	[PtrMap],edx
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	ebp,[PosZWr]

			sar	ebx,16
			xor	edx,edx

			mov	ecx,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
			add	ebx,ebp

			mov	eax,W_NORM
			mov	edi,dword ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ

			idiv	ebx

			shl	ecx,16
			mov	[esi].Pt_W,eax

			sar	ecx,16
			xor	edx,edx

			mov	eax,W_NORM
			add	ecx,ebp

			idiv	ecx

			shl	edi,16
			mov	[esi+(Size STRUC_POINT)].Pt_W,eax

			sar	edi,16
			xor	edx,edx

			mov	eax,W_NORM
			add	edi,ebp

			idiv	edi

			mov	[esi+2*(Size STRUC_POINT)].Pt_W,eax
			pop	ebx			; Get light

			pop	eax			; Get poly number
			mov	ecx,3

			push	Offset @@Return_Display
			JMP_FILL_POLY


@@Quad_TextureZ_Flat:
			shr	eax,2				; Get poly type
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	eax,dword ptr [ebp].Poly4_Couleur

			shr	eax,16				; EAX = Normal vector number
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	ax,[ListLights+eax*2]		; Get intensity
			mov	edi,dword ptr [ebp].Poly4_P3

			shr	eax,8				; Shift intensity
			lea	esi,ListFillPoly

			push	eax				; Push intensity
			mov	edx,edi

			shr	edx,16				; EDX = P4
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			and	edi,0FFFFh			; EDI = P3
			mov	dword ptr [esi+1*(Size STRUC_POINT)].Pt_XE,eax

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+edx*4]
			push	edx

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax
			mov	edx,dword ptr [ebp].Poly4_HandleText; Get texture handle

			mov	eax,[Obj_ListProjectedPoints+edi*4]
			and	edx,0FFFFh

			shl	edx,2
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			mov	eax,[PtrTextures]

			add	edx,eax
			mov	eax,dword ptr [ebp].Poly4_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U2

			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U3

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			mov	edx,[edx]			; Get texture infos

			mov	eax,dword ptr [ebp].Poly4_U4
			mov	ebp,edx

			shr	ebp,16				; EBP = RepMask
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebp
			add	edx,eax

			mov	[PtrMap],edx
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			pop	ebp			; P4 in EBP

			idiv	ebx

			mov	[esi].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+(Size STRUC_POINT)].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+2*(Size STRUC_POINT)].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+3*(Size STRUC_POINT)].Pt_W,eax
			pop	ebx			; Get light

			pop	eax			; Get Poly number
			mov	ecx,4

			push	Offset @@Return_Display
			JMP_FILL_POLY




			; *************************
			; *** TEXTURE Z GOURAUD ***
			; *************************
@@Triangle_TextureZ_Gouraud:
			shr	eax,2				; Get poly type
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly3_P3
			lea	esi,ListFillPoly

			and	edi,0FFFFh			; EDI = P3

			GET_ZO3	esi, ebx, ecx, edi		; ZBuffer

			mov	dx,[ListLights+ebx*2]
			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	[esi].Pt_Light,dx
			mov	dword ptr [esi].Pt_XE,eax

			mov	dx,[ListLights+ecx*2]
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	[esi+Size STRUC_POINT].Pt_Light,dx
			mov	dword ptr [esi+Size STRUC_POINT].Pt_XE,eax

			mov	ax,[ListLights+edi*2]
			mov	edx,dword ptr [ebp].Poly3_P3	; Get texture handle

			mov	[esi+2*(Size STRUC_POINT)].Pt_Light,ax
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			shr	edx,16-2

			and	edx,not 11b
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax

			add	edx,[PtrTextures]
			mov	eax,dword ptr [ebp].Poly3_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly3_U2

			mov	edx,[edx]			; Get texture infos
			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax

			mov	eax,dword ptr [ebp].Poly3_U3
			mov	ebp,edx

			shr	ebp,16				; EBP = RepMask
			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebp
			add	edx,eax

			mov	[PtrMap],edx
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	ebp,[PosZWr]

			sar	ebx,16
			xor	edx,edx

			mov	ecx,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ
			add	ebx,ebp

			mov	eax,W_NORM
			mov	edi,dword ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ

			idiv	ebx

			shl	ecx,16
			mov	[esi].Pt_W,eax

			sar	ecx,16
			xor	edx,edx

			mov	eax,W_NORM
			add	ecx,ebp

			idiv	ecx

			shl	edi,16
			mov	[esi+(Size STRUC_POINT)].Pt_W,eax

			sar	edi,16
			xor	edx,edx

			mov	eax,W_NORM
			add	edi,ebp

			idiv	edi

			mov	[esi+2*(Size STRUC_POINT)].Pt_W,eax
			pop	eax

			mov	ecx,3
			push	Offset @@Return_Display

			JMP_FILL_POLY


@@Quad_TextureZ_Gouraud:
			shr	eax,2				; Get poly type
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edi,dword ptr [ebp].Poly4_P3
			lea	esi,ListFillPoly

			mov	edx,edi
			and	edi,0FFFFh			; EDI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	esi, ebx, ecx, edi, edx		; ZBuffer

			mov	ax,[ListLights+ebx*2]

			mov	[esi].Pt_Light,ax

			mov	ax,[ListLights+ecx*2]

			mov	[esi+Size STRUC_POINT].Pt_Light,ax

			mov	ax,[ListLights+edi*2]

			mov	[esi+2*(Size STRUC_POINT)].Pt_Light,ax

			mov	ax,[ListLights+edx*2]

			mov	[esi+3*(Size STRUC_POINT)].Pt_Light,ax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [esi+1*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_XE,eax
			push	edx

			mov	edx,dword ptr [ebp].Poly4_HandleText; Get texture handle
			mov	eax,[Obj_ListProjectedPoints+edi*4]

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [esi].Pt_XE,eax
			and	edx,0FFFFh

			shl	edx,2
			mov	eax,[PtrTextures]

			add	edx,eax
			mov	eax,dword ptr [ebp].Poly4_U1

			mov	dword ptr [esi].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U2

			mov	dword ptr [esi+Size STRUC_POINT].Pt_MapU,eax
			mov	eax,dword ptr [ebp].Poly4_U3

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			mov	edx,[edx]			; Get texture infos

			mov	eax,dword ptr [ebp].Poly4_U4
			mov	ebp,edx

			shr	ebp,16				; EBP = RepMask
			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,eax

			and	edx,0FFFFh			; EDX = OrgMap
			mov	eax,[ObjPtrMap]

			mov	[RepMask],ebp
			add	edx,eax

			mov	[PtrMap],edx
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			pop	ebp			; P4 in EBP

			idiv	ebx

			mov	[esi].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+(Size STRUC_POINT)].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+2*(Size STRUC_POINT)].Pt_W,eax
			mov	ebx,dword ptr [Obj_ListRotatedPoints+ebp*8].Obj_Pt_PZ

			shl	ebx,16
			mov	edx,[PosZWr]

			sar	ebx,16
			mov	eax,W_NORM

			add	ebx,edx
			xor	edx,edx

			idiv	ebx

			mov	[esi+3*(Size STRUC_POINT)].Pt_W,eax
			pop	eax

			mov	ecx,4
			push	Offset @@Return_Display

			JMP_FILL_POLY









			; *************************
			; *** TEXTURE ENV SOLID ***
			; *************************

@@Triangle_EnvTexture_Solid:
			shr	eax,2				; Get Poly type
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edx,dword ptr [ebp].Poly3_P3
			lea	edi,ListFillPoly

			and	edx,0FFFFh			; EDX = P3

			GET_ZO3	edi, ebx, ecx, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [edi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [edi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [edi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,dword ptr[ebp].Poly3_P3

			shr	eax,16-2
			mov	edi,[lBody]

			and	eax,not 11b
			mov	esi,[PtrTextures]

			add	eax,esi
			mov	esi,[edi].Obj_Off_Normales

			add	edi,esi
			push	ebp

			mov	esi,[eax]
			mov	eax,[edi+ebx*8]

			mov	ebp,esi
			mov	ebx,[edi+ebx*8+4]

			shr	esi,16				; ESI = RepMask
			and	ebp,0FFFFh 			; EBP = OrgMap

			mov	[RepMask],esi
			mov	esi,[ObjPtrMap]

			add	ebp,esi
			mov	esi,Offset ListNormEnv

			mov	[PtrMap],ebp
			pop	ebp

			mov	[esi],eax
			mov	[esi+4],ebx

			mov	eax,[edi+ecx*8]
			mov	ecx,[edi+ecx*8+4]

			mov	[esi+8],eax
			mov	ebx,[edi+edx*8+4]

			mov	eax,[edi+edx*8]
			mov	[esi+16+4],ebx

			shr	ebx,16
			mov	[esi+8+4],ecx

			mov	[esi+16],eax
			lea	edi,[esi+8*4]

			push	esi
			push	edi
			movsx	eax,[esi].VT_X
			movsx	ebx,[esi].VT_Y
			movsx	ecx,[esi].VT_Z
			movzx	esi,word ptr [esi+6]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi],eax
			mov	[edi+4],ecx

			push	esi
			push	edi
			movsx	eax,[esi+8].VT_X
			movsx	ebx,[esi+8].VT_Y
			movsx	ecx,[esi+8].VT_Z
			movzx	esi,word ptr [esi+6+8]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+8],eax
			mov	[edi+4+8],ecx

			push	esi
			push	edi
			movsx	eax,[esi+16].VT_X
			movsx	ebx,[esi+16].VT_Y
			movsx	ecx,[esi+16].VT_Z
			movzx	esi,word ptr [esi+6+16]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+16],eax
			mov	[edi+4+16],ecx


			mov	ecx,dword ptr [ebp].Poly3_Scale
			mov	edi,(Offset ListNormEnv)+8*4

			and	ecx,0FFFFh
			mov	edx,ecx

			shr	edx,8
			and	ecx,0FFh

			mov	esi,Offset ListFillPoly
			mov	eax,[edi]

			add	eax,40004000h
			mov	ebp,[edi+8]

			mov	ebx,eax
			add	ebp,40004000h

			shr	ebx,16
			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			shr	ebx,16
			mov	dword ptr [esi].Pt_MapU,eax

			and	ebp,0FFFFh
			mov	eax,[edi+16]

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	eax,40004000h

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ebx,eax

			shr	ebx,16
			mov	dword ptr [esi+(Size STRUC_POINT)].Pt_MapU,ebp

			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4
			mov	ecx,3

			shr	eax,4
			and	ebx,0FFFF0000h

			or	ebx,eax
			pop	eax

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,ebx
			push	Offset @@Return_Display

			JMP_FILL_POLY





@@Quad_EnvTexture_Solid:
			shr	eax,2				; Get Poly type
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	esi,dword ptr [ebp].Poly4_P3
			lea	edi,ListFillPoly

			mov	edx,esi
			and	esi,0FFFFh			; ESI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	edi, ebx, ecx, esi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [edi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [edi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+esi*4]

			mov	dword ptr [edi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [edi+3*(Size STRUC_POINT)].Pt_XE,eax
			mov	edi,[lBody]

			mov	eax,dword ptr[ebp].Poly4_Scale
			mov	ebp,[edi].Obj_Off_Normales	; AGI...

			add	edi,ebp
			mov	ebp,eax

			shr	eax,16-2
			and	ebp,0FFFFh

			and	eax,not 11b
			push	ebp			; Save scale

			push	eax			; Save Texture Index*4
			mov	ebp,Offset ListNormEnv

			mov	eax,[edi+ebx*8]
			mov	ebx,[edi+ebx*8+4]

			mov	[ebp],eax
			mov	[ebp+4],ebx

			mov	eax,[edi+ecx*8]
			mov	ebx,[edi+ecx*8+4]

			mov	[ebp+8],eax
			mov	[ebp+8+4],ebx

			mov	eax,[edi+esi*8]
			mov	ebx,[edi+esi*8+4]

			mov	[ebp+16],eax
			mov	[ebp+16+4],ebx

			mov	eax,[edi+edx*8]
			mov	ebx,[edi+edx*8+4]

			mov	[ebp+24],eax
			mov	[ebp+24+4],ebx

			shr	ebx,16
			mov	esi,ebp

			lea	edi,[ebp+8*4]

			push	esi
			push	edi
			movsx	eax,[esi].VT_X
			movsx	ebx,[esi].VT_Y
			movsx	ecx,[esi].VT_Z
			movzx	esi,word ptr [esi+6]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi],eax
			mov	[edi+4],ecx

			push	esi
			push	edi
			movsx	eax,[esi+8].VT_X
			movsx	ebx,[esi+8].VT_Y
			movsx	ecx,[esi+8].VT_Z
			movzx	esi,word ptr [esi+6+8]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+8],eax
			mov	[edi+4+8],ecx

			push	esi
			push	edi
			movsx	eax,[esi+16].VT_X
			movsx	ebx,[esi+16].VT_Y
			movsx	ecx,[esi+16].VT_Z
			movzx	esi,word ptr [esi+6+16]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+16],eax
			mov	[edi+4+16],ecx

			push	esi
			push	edi
			movsx	eax,[esi+24].VT_X
			movsx	ebx,[esi+24].VT_Y
			movsx	ecx,[esi+24].VT_Z
			movzx	esi,word ptr [esi+6+24]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+24],eax
			mov	[edi+4+24],ecx




			pop	eax			; Restore Text Index.
			mov	ebx,[PtrTextures]

			mov	edi,(Offset ListNormEnv)+8*4
			mov	esi,Offset ListFillPoly

			mov	eax,[eax+ebx]
			pop	ecx			; ECX = Scale

			mov	ebx,eax
			and	ecx,0FFFFh

			shr	eax,16			; EAX = RepMask
			and	ebx,0FFFFh 		; EBX = OrgMap

			mov	[RepMask],eax
			mov	eax,[ObjPtrMap]

			add	ebx,eax
			mov	edx,ecx

			shr	edx,8
			mov	[PtrMap],ebx

			and	ecx,0FFh
			mov	eax,[edi]

			mov	ebp,[edi+8]
			add	eax,40004000h

			mov	ebx,eax
			and	eax,0FFFFh

			shr	ebx,16
			add	ebp,40004000h

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			mov	dword ptr [esi].Pt_MapU,eax
			and	ebp,0FFFFh

			shr	ebx,16
			mov	eax,[edi+16]

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	eax,40004000h

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ebx,eax

			shr	ebx,16
			mov	dword ptr [esi+(Size STRUC_POINT)].Pt_MapU,ebp

			mov	ebp,[edi+24]
			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	ebp,40004000h

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			and	ebp,0FFFFh

			shr	ebx,16

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			pop	eax

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ecx,4

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,ebp
			push	Offset @@Return_Display

			JMP_FILL_POLY




			; ************************
			; *** TEXTURE ENV FLAT ***
			; ************************


@@Triangle_EnvTexture_Flat:
			shr	eax,2				; Get Poly type
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	eax,dword ptr [ebp].Poly3_Couleur

			shr	eax,16
			mov	ecx,ebx

			mov	ax,[ListLights+eax*2]		; No AGI bcoz 16 bit access
			and	ebx,0FFFFh			; EBX = P1

			shr	ecx,16				; ECX = P2
			push	eax				; Push Light

			mov	edx,dword ptr [ebp].Poly3_P3
			lea	edi,ListFillPoly

			and	edx,0FFFFh			; EDX = P3

			GET_ZO3	edi, ebx, ecx, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [edi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [edi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [edi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,dword ptr[ebp].Poly3_P3

			shr	eax,16-2
			mov	edi,[lBody]

			and	eax,not 11b
			mov	esi,[PtrTextures]

			add	eax,esi
			mov	esi,[edi].Obj_Off_Normales

			add	edi,esi
			push	ebp

			mov	esi,[eax]
			mov	eax,[edi+ebx*8]

			mov	ebp,esi
			mov	ebx,[edi+ebx*8+4]

			shr	esi,16				; ESI = RepMask
			and	ebp,0FFFFh 			; EBP = OrgMap

			mov	[RepMask],esi
			mov	esi,[ObjPtrMap]

			add	ebp,esi
			mov	esi,Offset ListNormEnv

			mov	[PtrMap],ebp
			pop	ebp

			mov	[esi],eax
			mov	[esi+4],ebx

			mov	eax,[edi+ecx*8]
			mov	ecx,[edi+ecx*8+4]

			mov	[esi+8],eax
			mov	ebx,[edi+edx*8+4]

			mov	eax,[edi+edx*8]
			mov	[esi+16+4],ebx

			shr	ebx,16
			mov	[esi+8+4],ecx

			mov	[esi+16],eax
			lea	edi,[esi+8*4]

			push	esi
			push	edi
			movsx	eax,[esi].VT_X
			movsx	ebx,[esi].VT_Y
			movsx	ecx,[esi].VT_Z
			movzx	esi,word ptr [esi+6]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi],eax
			mov	[edi+4],ecx

			push	esi
			push	edi
			movsx	eax,[esi+8].VT_X
			movsx	ebx,[esi+8].VT_Y
			movsx	ecx,[esi+8].VT_Z
			movzx	esi,word ptr [esi+6+8]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+8],eax
			mov	[edi+4+8],ecx

			push	esi
			push	edi
			movsx	eax,[esi+16].VT_X
			movsx	ebx,[esi+16].VT_Y
			movsx	ecx,[esi+16].VT_Z
			movzx	esi,word ptr [esi+6+16]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+16],eax
			mov	[edi+4+16],ecx



			mov	ecx,dword ptr [ebp].Poly3_Scale
			mov	edi,(Offset ListNormEnv)+8*4

			and	ecx,0FFFFh
			mov	edx,ecx

			shr	edx,8
			and	ecx,0FFh

			mov	esi,Offset ListFillPoly
			mov	eax,[edi]

			add	eax,40004000h
			mov	ebp,[edi+8]

			mov	ebx,eax
			add	ebp,40004000h

			shr	ebx,16
			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			shr	ebx,16
			mov	dword ptr [esi].Pt_MapU,eax

			and	ebp,0FFFFh
			mov	eax,[edi+16]

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	eax,40004000h

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ebx,eax

			shr	ebx,16
			mov	dword ptr [esi+(Size STRUC_POINT)].Pt_MapU,ebp

			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4
			mov	ecx,3

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			pop	ebx		; Get Light

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			pop	eax		; Get Polytype

			shr	ebx,8
			push	Offset @@Return_Display

			JMP_FILL_POLY



@@Quad_EnvTexture_Flat:
			shr	eax,2				; Get Poly type
			mov	edx,dword ptr [ebp].Poly4_Couleur

			shr	edx,16
			mov	ebx,dword ptr [ebp].Poly4_P1

			mov	dx,[ListLights+edx*2]
			mov	ecx,ebx

			push	edx				; Save Light
			push	eax				; Save poly type

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	esi,dword ptr [ebp].Poly4_P3
			lea	edi,ListFillPoly

			mov	edx,esi
			and	esi,0FFFFh			; ESI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	edi, ebx, ecx, esi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [edi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [edi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+esi*4]

			mov	dword ptr [edi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [edi+3*(Size STRUC_POINT)].Pt_XE,eax
			mov	edi,[lBody]

			mov	eax,dword ptr[ebp].Poly4_Scale
			mov	ebp,[edi].Obj_Off_Normales	; AGI...

			add	edi,ebp
			mov	ebp,eax

			shr	eax,16-2
			and	ebp,0FFFFh

			and	eax,not 11b
			push	ebp			; Save scale

			push	eax			; Save Texture Index*4
			mov	ebp,Offset ListNormEnv

			mov	eax,[edi+ebx*8]
			mov	ebx,[edi+ebx*8+4]

			mov	[ebp],eax
			mov	[ebp+4],ebx

			mov	eax,[edi+ecx*8]
			mov	ebx,[edi+ecx*8+4]

			mov	[ebp+8],eax
			mov	[ebp+8+4],ebx

			mov	eax,[edi+esi*8]
			mov	ebx,[edi+esi*8+4]

			mov	[ebp+16],eax
			mov	[ebp+16+4],ebx

			mov	eax,[edi+edx*8]
			mov	ebx,[edi+edx*8+4]

			mov	[ebp+24],eax
			mov	[ebp+24+4],ebx

			shr	ebx,16
			mov	esi,ebp

			lea	edi,[ebp+8*4]

			push	esi
			push	edi
			movsx	eax,[esi].VT_X
			movsx	ebx,[esi].VT_Y
			movsx	ecx,[esi].VT_Z
			movzx	esi,word ptr [esi+6]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi],eax
			mov	[edi+4],ecx

			push	esi
			push	edi
			movsx	eax,[esi+8].VT_X
			movsx	ebx,[esi+8].VT_Y
			movsx	ecx,[esi+8].VT_Z
			movzx	esi,word ptr [esi+6+8]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+8],eax
			mov	[edi+4+8],ecx

			push	esi
			push	edi
			movsx	eax,[esi+16].VT_X
			movsx	ebx,[esi+16].VT_Y
			movsx	ecx,[esi+16].VT_Z
			movzx	esi,word ptr [esi+6+16]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+16],eax
			mov	[edi+4+16],ecx

			push	esi
			push	edi
			movsx	eax,[esi+24].VT_X
			movsx	ebx,[esi+24].VT_Y
			movsx	ecx,[esi+24].VT_Z
			movzx	esi,word ptr [esi+6+24]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+24],eax
			mov	[edi+4+24],ecx


			pop	eax			; Restore Text Index.
			mov	ebx,[PtrTextures]

			mov	edi,(Offset ListNormEnv)+8*4
			mov	esi,Offset ListFillPoly

			mov	eax,[eax+ebx]
			pop	ecx			; ECX = Scale

			mov	ebx,eax
			and	ecx,0FFFFh

			shr	eax,16			; EAX = RepMask
			and	ebx,0FFFFh 		; EBX = OrgMap

			mov	[RepMask],eax
			mov	eax,[ObjPtrMap]

			add	ebx,eax
			mov	edx,ecx

			shr	edx,8
			mov	[PtrMap],ebx

			and	ecx,0FFh
			mov	eax,[edi]

			mov	ebp,[edi+8]
			add	eax,40004000h

			mov	ebx,eax
			and	eax,0FFFFh

			shr	ebx,16
			add	ebp,40004000h

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			mov	dword ptr [esi].Pt_MapU,eax
			and	ebp,0FFFFh

			shr	ebx,16
			mov	eax,[edi+16]

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	eax,40004000h

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ebx,eax

			shr	ebx,16
			mov	dword ptr [esi+(Size STRUC_POINT)].Pt_MapU,ebp

			mov	ebp,[edi+24]
			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	ebp,40004000h

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			and	ebp,0FFFFh

			shr	ebx,16

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			pop	eax			; Get poly type

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ecx,4

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,ebp
			pop	ebx			; Get Light

			shr	ebx,8
			push	Offset @@Return_Display

			JMP_FILL_POLY



			; ***************************
			; *** TEXTURE ENV GOURAUD ***
			; ***************************

@@Triangle_EnvTexture_Gouraud:
			shr	eax,2				; Get Poly type
			mov	ebx,dword ptr [ebp].Poly3_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	edx,dword ptr [ebp].Poly3_P3
			lea	edi,ListFillPoly

			and	edx,0FFFFh			; EDX = P3

			GET_ZO3	edi, ebx, ecx, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [edi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [edi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [edi+2*(Size STRUC_POINT)].Pt_XE,eax

			mov	ax,[ListLights+ebx*2]

			mov	[edi].Pt_Light,ax

			mov	ax,[ListLights+ecx*2]

			mov	[edi+Size STRUC_POINT].Pt_Light,ax

			mov	ax,[ListLights+edx*2]

			mov	[edi+2*(Size STRUC_POINT)].Pt_Light,ax
			mov	eax,dword ptr[ebp].Poly3_P3	; Get handle text.

			shr	eax,16-2
			mov	edi,[lBody]

			and	eax,not 11b
			mov	esi,[PtrTextures]

			add	eax,esi
			mov	esi,[edi].Obj_Off_Normales

			add	edi,esi
			push	ebp

			mov	esi,[eax]
			mov	eax,[edi+ebx*8]

			mov	ebp,esi
			mov	ebx,[edi+ebx*8+4]

			shr	esi,16				; ESI = RepMask
			and	ebp,0FFFFh 			; EBP = OrgMap

			mov	[RepMask],esi
			mov	esi,[ObjPtrMap]

			add	ebp,esi
			mov	esi,Offset ListNormEnv

			mov	[PtrMap],ebp
			pop	ebp

			mov	[esi],eax
			mov	[esi+4],ebx

			mov	eax,[edi+ecx*8]
			mov	ecx,[edi+ecx*8+4]

			mov	[esi+8],eax
			mov	ebx,[edi+edx*8+4]

			mov	[esi+8+4],ecx
			mov	[esi+16+4],ebx

			shr	ebx,16
			mov	eax,[edi+edx*8]

			mov	[esi+16],eax
			lea	edi,[esi+8*4]

			mov	ecx,3

			push	esi
			push	edi
			movsx	eax,[esi].VT_X
			movsx	ebx,[esi].VT_Y
			movsx	ecx,[esi].VT_Z
			movzx	esi,word ptr [esi+6]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi],eax
			mov	[edi+4],ecx

			push	esi
			push	edi
			movsx	eax,[esi+8].VT_X
			movsx	ebx,[esi+8].VT_Y
			movsx	ecx,[esi+8].VT_Z
			movzx	esi,word ptr [esi+6+8]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+8],eax
			mov	[edi+4+8],ecx

			push	esi
			push	edi
			movsx	eax,[esi+16].VT_X
			movsx	ebx,[esi+16].VT_Y
			movsx	ecx,[esi+16].VT_Z
			movzx	esi,word ptr [esi+6+16]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+16],eax
			mov	[edi+4+16],ecx


			mov	ecx,dword ptr [ebp].Poly3_Scale
			mov	edi,(Offset ListNormEnv)+8*4

			and	ecx,0FFFFh
			mov	edx,ecx

			shr	edx,8
			and	ecx,0FFh

			mov	esi,Offset ListFillPoly
			mov	eax,[edi]

			add	eax,40004000h
			mov	ebp,[edi+8]

			mov	ebx,eax
			add	ebp,40004000h

			shr	ebx,16
			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			shr	ebx,16
			mov	dword ptr [esi].Pt_MapU,eax

			and	ebp,0FFFFh
			mov	eax,[edi+16]

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	eax,40004000h

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ebx,eax

			shr	ebx,16
			mov	dword ptr [esi+(Size STRUC_POINT)].Pt_MapU,ebp

			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4
			mov	ecx,3

			shr	eax,4
			and	ebx,0FFFF0000h

			or	ebx,eax
			pop	eax

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,ebx
			push	Offset @@Return_Display

			JMP_FILL_POLY





@@Quad_EnvTexture_Gouraud:
			shr	eax,2				; Get Poly type
			mov	ebx,dword ptr [ebp].Poly4_P1

			push	eax				; Save poly type
			mov	ecx,ebx

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	esi,dword ptr [ebp].Poly4_P3
			lea	edi,ListFillPoly

			mov	edx,esi
			and	esi,0FFFFh			; ESI = P3

			shr	edx,16				; EDX = P4

			GET_ZO4	edi, ebx, ecx, esi, edx		; ZBuffer

			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	dword ptr [edi].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+ecx*4]

			mov	dword ptr [edi+Size STRUC_POINT].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+esi*4]

			mov	dword ptr [edi+2*(Size STRUC_POINT)].Pt_XE,eax
			mov	eax,[Obj_ListProjectedPoints+edx*4]

			mov	dword ptr [edi+3*(Size STRUC_POINT)].Pt_XE,eax

			mov	ax,[ListLights+ebx*2]

			mov	[edi].Pt_Light,ax

			mov	ax,[ListLights+ecx*2]

			mov	[edi+Size STRUC_POINT].Pt_Light,ax

			mov	ax,[ListLights+esi*2]

			mov	[edi+2*(Size STRUC_POINT)].Pt_Light,ax

			mov	ax,[ListLights+edx*2]

			mov	[edi+3*(Size STRUC_POINT)].Pt_Light,ax
			mov	edi,[lBody]

			mov	eax,dword ptr[ebp].Poly4_Scale
			mov	ebp,[edi].Obj_Off_Normales	; AGI...

			add	edi,ebp
			mov	ebp,eax

			shr	eax,16-2
			and	ebp,0FFFFh

			and	eax,not 11b
			push	ebp			; Save scale

			push	eax			; Save Texture Index*4
			mov	ebp,Offset ListNormEnv

			mov	eax,[edi+ebx*8]
			mov	ebx,[edi+ebx*8+4]

			mov	[ebp],eax
			mov	[ebp+4],ebx

			mov	eax,[edi+ecx*8]
			mov	ebx,[edi+ecx*8+4]

			mov	[ebp+8],eax
			mov	[ebp+8+4],ebx

			mov	eax,[edi+esi*8]
			mov	ebx,[edi+esi*8+4]

			mov	[ebp+16],eax
			mov	[ebp+16+4],ebx

			mov	eax,[edi+edx*8]
			mov	ebx,[edi+edx*8+4]

			mov	[ebp+24],eax
			mov	[ebp+24+4],ebx

			shr	ebx,16
			mov	esi,ebp

			lea	edi,[ebp+8*4]
			mov	ecx,4

			push	esi
			push	edi
			movsx	eax,[esi].VT_X
			movsx	ebx,[esi].VT_Y
			movsx	ecx,[esi].VT_Z
			movzx	esi,word ptr [esi+6]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi],eax
			mov	[edi+4],ecx

			push	esi
			push	edi
			movsx	eax,[esi+8].VT_X
			movsx	ebx,[esi+8].VT_Y
			movsx	ecx,[esi+8].VT_Z
			movzx	esi,word ptr [esi+6+8]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+8],eax
			mov	[edi+4+8],ecx

			push	esi
			push	edi
			movsx	eax,[esi+16].VT_X
			movsx	ebx,[esi+16].VT_Y
			movsx	ecx,[esi+16].VT_Z
			movzx	esi,word ptr [esi+6+16]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+16],eax
			mov	[edi+4+16],ecx

			push	esi
			push	edi
			movsx	eax,[esi+24].VT_X
			movsx	ebx,[esi+24].VT_Y
			movsx	ecx,[esi+24].VT_Z
			movzx	esi,word ptr [esi+6+24]
			mov	esi,[Table_Matrix+esi*4]
			call	[RotatePointNoMMX]
			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
			shl	ebx,16
			and	eax,0FFFFh
			or	eax,ebx
			and	ecx,0FFFFh
			pop	edi
			pop	esi
			mov	[edi+24],eax
			mov	[edi+4+24],ecx


			pop	eax			; Restore Text Index.
			mov	ebx,[PtrTextures]

			mov	edi,(Offset ListNormEnv)+8*4
			mov	esi,Offset ListFillPoly

			mov	eax,[eax+ebx]
			pop	ecx			; ECX = Scale

			mov	ebx,eax
			and	ecx,0FFFFh

			shr	eax,16			; EAX = RepMask
			and	ebx,0FFFFh 		; EBX = OrgMap

			mov	[RepMask],eax
			mov	eax,[ObjPtrMap]

			add	ebx,eax
			mov	edx,ecx

			shr	edx,8
			mov	[PtrMap],ebx

			and	ecx,0FFh
			mov	eax,[edi]

			mov	ebp,[edi+8]
			add	eax,40004000h

			mov	ebx,eax
			and	eax,0FFFFh

			shr	ebx,16
			add	ebp,40004000h

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			mov	dword ptr [esi].Pt_MapU,eax
			and	ebp,0FFFFh

			shr	ebx,16
			mov	eax,[edi+16]

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	eax,40004000h

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ebx,eax

			shr	ebx,16
			mov	dword ptr [esi+(Size STRUC_POINT)].Pt_MapU,ebp

			mov	ebp,[edi+24]
			and	eax,0FFFFh

			imul	eax,ecx
			imul	ebx,edx

			shl	ebx,16-4
			add	ebp,40004000h

			shr	eax,4
			and	ebx,0FFFF0000h

			or	eax,ebx
			mov	ebx,ebp

			mov	dword ptr [esi+2*(Size STRUC_POINT)].Pt_MapU,eax
			and	ebp,0FFFFh

			shr	ebx,16

			imul	ebp,ecx
			imul	ebx,edx

			shl	ebx,16-4
			pop	eax

			shr	ebp,4
			and	ebx,0FFFF0000h

			or	ebp,ebx
			mov	ecx,4

			mov	dword ptr [esi+3*(Size STRUC_POINT)].Pt_MapU,ebp
			push	Offset @@Return_Display

			JMP_FILL_POLY



			; ************
			; *** LINE ***
			; ************
@@Line:
			mov	eax,dword ptr [ebp].Obj_Line_P1
			mov	ebp,dword ptr [ebp].Obj_LineType

			mov	ecx,eax
			and	eax,0FFFFh		; EAX = P1

			shr	ecx,16			; ECX = P2
			cmp	[Fill_Flag_ZBuffer],TRUE

			jne	@@NoZBufLine

			cmp	[TypeProj],TYPE_3D
			jne	@@NoZBufLine

			mov	esi,dword ptr [Obj_ListRotatedPoints+eax*8].Obj_Pt_PZ
			mov	edi,dword ptr [Obj_ListRotatedPoints+ecx*8].Obj_Pt_PZ

			shl	esi,16
			mov	ebx,[PosZWr]

			sar	esi,16
			dec	ebx			; BCOZ NEG  rr <=> XOR rr,-1 PLUS INC rr

			shl	edi,16
			xor	esi,-1			; NEG ESI (INC ESI is in SUB ESI,EBX)

			sar	edi,16
			sub	esi,ebx

			xor	edi,-1			; for NEG EDI

			imul	esi,[Fill_ZBuffer_Factor]

			sar	esi,16
			sub	edi,ebx

			imul	edi,[Fill_ZBuffer_Factor]

			sar	edi,16
@@NoZBufLine:
			shr	ebp,16
			mov	eax,[Obj_ListProjectedPoints+eax*4]

			mov	ecx,[Obj_ListProjectedPoints+ecx*4]
			mov	ebx,eax

			cmp	eax,80008000h		; Don't display something that is clipped
			je	@@Return_Display

			cmp	ecx,80008000h
			je	@@Return_Display

			shl	eax,16
			mov	edx,ecx

			shl	ecx,16

			sar	ebx,16

			sar	edx,16

			sar	eax,16
			push	Offset @@Return_Display

			sar	ecx,16
			jmp	Line_A




			; **************
			; *** SPHERE ***
			; **************

@@Sphere:
			push	Offset @@Return_Display
			push	0
@@DispSph:
			mov	ebx,dword ptr [ebp].Obj_Sphere_P1
			mov	edi,dword ptr [ebp].Obj_SphereType

			mov	eax,ebx
			and	ebx,0FFFFh

			sar	eax,16			; EAX = Radius
			mov	edx,[TypeProj]

			cmp	edx,TYPE_3D
			je	@@Radius_Proj

			mov	edx,eax			; Magic formula: *34/512
			add	eax,eax			; 34 = SQRT(sx²+sy²)

			shl	edx,5			; *32
			push	ebx			; Save point index

			add	eax,edx

			sar	eax,9

			neg	eax
			jmp	@@Return_Radius
@@Radius_Proj:
			mov	ecx,[PosZWr]

			mov	ebp,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ
			push	ebx			; Save point index

			shl	ebp,16

			sar	ebp,16

			add	ebp,ecx
			je	@@SkipSphere

			imul	[LFactorX]
			idiv	ebp
@@Return_Radius:
			mov	ecx,eax
			mov	eax,[Obj_ListProjectedPoints+ebx*4]

			mov	edx,edi
			mov	ebx,eax

			shr	edx,16			; EDX = Color
			xor	ecx,-1

			sar	ebx,16
			inc	ecx

			shl	eax,16
			mov	esi,ebx

			sar	eax,16
			sub	esi,ecx

			mov	edi,[ScreenYMin]

			cmp	esi,edi
			jl	@@MAJ_YMin
@@Ret_YMin:
			lea	esi,[esi+ecx*2]
			mov	edi,[ScreenYMax]
			cmp	esi,edi
			jg	@@MAJ_YMax
@@Ret_YMax:
			mov	esi,eax
			sub	esi,ecx
			mov	edi,[ScreenXMin]
			cmp	esi,edi
			jl	@@MAJ_XMin
@@Ret_XMin:
			lea	esi,[esi+ecx*2]
			mov	edi,[ScreenXMax]
			cmp	esi,edi
			jg	@@MAJ_XMax
@@Ret_XMax:
			pop	edi
			cmp	[Fill_Flag_ZBuffer],TRUE
			jne	@@NoZBufSphere

			cmp	[TypeProj],TYPE_3D
			jne	@@NoZBufSphere

			movsx	edi,word ptr [Obj_ListRotatedPoints+edi*8].Obj_Pt_PZ
			neg	edi
			sub	edi,[PosZWr]
			imul	edi,[Fill_ZBuffer_Factor]
			sar	edi,16
@@NoZBufSphere:
			pop	esi			; POP disc type
			jmp	Fill_Sphere

@@SkipSphere:						; Skip this sphere (behind the camera)
			pop	edi
			pop	esi
			ret
@@MAJ_YMin:
			mov	[ScreenYMin],esi
			jmp	@@Ret_YMin
@@MAJ_YMax:
			mov	[ScreenYMax],esi
			jmp	@@Ret_YMax
@@MAJ_XMin:
			mov	[ScreenXMin],esi
			jmp	@@Ret_XMin
@@MAJ_XMax:
			mov	[ScreenXMax],esi
			jmp	@@Ret_XMax




			; *********************
			; *** SPHERE TRANSP ***
			; *********************
@@Sphere_Transp:
			push	Offset @@Return_Display
			push	2
			jmp	@@DispSph



BodyDisplay		ENDP







; ╔════════════════════════════════════════════════════════════════════════════╗
; ║ ObjectDisplay: Affiche un objet animé                                      ║
; ╟────────────────────────────────────────────────────────────────────────────╢
; ║ Call: EAX = PTR on the object                                              ║
; ║                                                                            ║
; ║                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════╝

ObjectDisplay		PROC
			push	ebp
			mov	ebp,eax			; EBP = PTR on object

			mov	ebx,[eax].Texture	; Must we change the texture pointer ?
			mov	edx,7FFFFFFFh

			test	ebx,ebx
			je	@@NoTextureChange
			cmp	ebx,-1
			jne	@@TextureChange
@@NoTextureChange:
			mov	[ScreenXMin],edx	; Init 2D box
			mov	[ScreenYMin],edx

			inc	edx			; edx = 80000000h
			mov	edi,[TypeProj]

			mov	[ScreenXMax],edx
			mov	[ScreenYMax],edx

			cmp	edi,TYPE_3D
			mov	eax,[ebp].ObjX		; Get object's position

			mov	ebx,[ebp].ObjY		; in world space
			mov	ecx,[ebp].ObjZ

			jne	@@Init_Iso		; Calc object position
							; in camera space
			mov	esi,Offset MatriceWorld
			call	[LongRotatePoint]	; If perspective

			mov	eax,[X0]
			mov	ebx,[Y0]

			mov	ecx,[Z0]
@@Init_Iso:
			mov	edi,[CameraXr]

			sub	eax,edi
			mov	edi,[CameraYr]

			sub	ebx,edi
			mov	edi,[CameraZr]

			sub	ecx,edi
			mov	[PosXWr],eax		; Object world position

			mov	[PosYWr],ebx
			mov	[PosZWr],ecx

			mov	esi,[ebp].Body		; Get either body's number or body's ptr
			mov	eax,[TransFctBody]

			test	eax,eax			; Convert it to a ptr
			je	@@NoConversion

			call	eax			; ESI = Pointer
@@NoConversion:

; ┌─────────────────────────────────────────────────────────────────┐
; │                                                                 │
; │                          GEOMETRY                               │
; │                                                                 │
; └─────────────────────────────────────────────────────────────────┘
							; ESI = Body's Ptr
			mov	[lBody],esi
			lea	ecx,[ebp-Size GROUP_INFO].CurrentFrame

			mov	[ListGroupAnim],ecx
			mov	eax,[esi].Obj_Off_Points

			add	eax,esi
			mov	ebx,[esi].Obj_Off_Normales

			mov	[PtrPoints],eax
			add	ebx,esi

			mov	[PtrNormales],ebx
			mov	eax,[esi].Obj_Off_NormFaces

			add	eax,esi
			mov	ebx,Offset Obj_ListRotatedPoints

			mov	[PtrNormFaces],eax
			mov	[PtrRotPoints],ebx

			mov	eax,Offset ListLights
			mov	ecx,[esi].Obj_Nb_Points

			mov	[PtrListLights],eax
			add	ecx,ecx				; Skip vertex light

			add	eax,ecx				; to calculate the offset of face-lights
			mov	ebx,[esi].Obj_Nb_Groupes

			mov	[PtrListLightsFaces],eax
			mov	[NbGroupes],ebx

			mov	ebx,[esi].Obj_Off_Groupes
			mov	edi,offset TabMat		; Target Matrix

			add	ebx,esi
			mov	esi,offset MatriceWorld		; Source Matrix

			mov	[PtrGroupes],ebx
			xor	eax,eax

			mov	[Current_Matrix],edi
			mov	[TransX],eax

			mov	[TransY],eax
			mov	[TransZ],eax

			mov	eax,[ebp].ObjAlpha
			mov	ebx,[ebp].ObjBeta

			mov	ecx,[ebp].ObjGamma
			jmp	@@Apply_Rotation

			ALIGN	4
@@Next_Group:
			add	ebx,Size STRUC_OBJ_GROUPE
			mov	[NbGroupes],eax

			mov	[PtrGroupes],ebx
			mov	ebp,[ListGroupAnim]

			mov	esi,dword ptr [ebx].Obj_OrgGroupe; OrgGroup | OrgPt
			add	ebp,Size GROUP_INFO

			mov	ebx,esi
			and	esi,0FFFFh		; OrgMatrix

			shr	ebx,16			; EBX = OrgPt
			mov	[ListGroupAnim],ebp

			mov	esi,[Table_Matrix+esi*4]
			mov	edi,[Current_Matrix]

			mov	eax,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PX
			mov	edx,dword ptr [Obj_ListRotatedPoints+ebx*8].Obj_Pt_PZ

			mov	ebx,eax			; Get father's position
			add	edi,SIZE_MATRICE

			shl	eax,16
			mov	[Current_Matrix],edi

			sar	eax,16
			mov	ecx,dword ptr [ebp].GroupBeta

			sar	ebx,16
			mov	[TransX],eax

			shl	edx,16
			mov	eax,dword ptr [ebp].GroupType

			sar	edx,16
			mov	[TransY],ebx

			mov	ebx,ecx

			shl	ebx,16
			mov	[TransZ],edx

			sar	ebx,16
			mov	ebp,eax

			sar	eax,16

			sar	ecx,16

			test	ebp,TYPE_TRANSLATE
			je	@@Apply_Rotation
@@Apply_Translation:
			push	esi
			push	edi

			call	[RotatePoint]

			mov	edi,[esp]
			mov	esi,[esp+4]

			call	[CopyMatrix]

			mov	eax,[X0]
			mov	edx,[TransX]

			add	eax,edx
			mov	ebx,[Y0]

			mov	edx,[TransY]
			mov	ecx,[Z0]

			add	ebx,edx
			pop	edi

			mov	edx,[TransZ]
			pop	esi

			add	ecx,edx
			push	Offset @@Transform

			mov	ebp,[PtrGroupes]
			jmp	[InitMatrixTrans]

@@Apply_Rotation:
			call	[RotateMatrix]		; Target=Source*Rot

			mov	eax,[TransX]
			mov	ebx,[TransY]

			mov	ecx,[TransZ]
			mov	edi,[Current_Matrix]	; EDI = Current Matrix

			mov	ebp,[PtrGroupes]
			call	[InitMatrixTrans]
@@Transform:
			; *****************
			; *** TRANSFORM ***
			; *****************

			mov	ecx,dword ptr [ebp].Obj_NbPts
			mov	esi,[PtrPoints]

			push	ecx			; Save NbNFaces || NbPts
			mov	edi,[PtrRotPoints]

			and	ecx,0FFFFh		; ECX = Nb Pts in group
			mov	ebx,[Current_Matrix]	; EBX = Current Matrix

			call	[RotTransList]		; Transformation

			mov	ecx,[esp]		; This piece of code
			mov	edi,[PtrRotPoints]	; just to adjust esi and edi

			and	ecx,0FFFFh
			mov	esi,[PtrPoints]

			shl	ecx,3
			mov	ebx,[Current_Matrix]	; EBX = Current Matrix

			add	esi,ecx
			add	edi,ecx

			mov	[PtrRotPoints],edi
			mov	[PtrPoints],esi



			; *****************
			; *** LIGHTING  ***
			; *****************

						; *** Calculate the intensities ***

			mov	ecx,[esp]		; Restore Nb Pts
			mov	esi,[PtrNormales]

			and	ecx,0FFFFh
			mov	edi,[PtrListLights]

			call	[LightList]

			mov	edx,[esp]		; Pointer adjustment...
			mov	esi,[PtrNormales]

			and	edx,0FFFFh
			mov	edi,[PtrListLights]

			pop	ecx

			shr	ecx,16			; Get Nb Norm Faces
			lea	esi,[esi+edx*8]

			lea	edi,[edi+edx*2]
			mov	[PtrNormales],esi

			mov	[PtrListLights],edi
			je	@@No_NormFaces		; Skip this part if nothing to do ;)

			mov	ebx,[Current_Matrix]	; EBX = Current Matrix
			mov	esi,[PtrNormFaces]

			mov	edi,[PtrListLightsFaces]
			push	ecx

			call	[LightList]

			pop	ecx
			mov	edi,[PtrListLightsFaces]

			mov	esi,[PtrNormFaces]
			add	edi,ecx

			lea	esi,[esi+ecx*8]		; AGI
			add	edi,ecx

			mov	[PtrNormFaces],esi
			mov	[PtrListLightsFaces],edi

@@No_NormFaces:
			mov	eax,[NbGroupes]
			mov	ebx,[PtrGroupes]

			dec	eax
			jne	@@Next_Group


			; ******************
			; *** PROJECTION ***
			; ******************

			mov	ebp,[lBody]

			xor	eax,eax
			mov	esi,[PosZWr]

			sub	eax,esi			; OrgZ=-PosZWr
			mov	esi,[PosYWr]

			push	eax			; Push OrgZ
			xor	eax,eax

			sub	eax,esi			; OrgY=-PosYWr
			mov	esi,[PosXWr]

			push	eax			; Push OrgY
			xor	eax,eax

			sub	eax,esi			; OrgX=-PosXWr
			mov	esi,Offset Obj_ListRotatedPoints

			push	eax			; Push OrgX
			mov	edi,Offset Obj_ListProjectedPoints

			mov	ecx,[ebp].Obj_Nb_Points
			call	[ProjectList]		; Projection

			add	esp,3*4			; Pop parameters


							; Is the object visible?
			mov	eax,[ScreenXMin]
			mov	ebx,[ClipXMax]

			cmp	eax,ebx
			jg	ByeBye

			mov	eax,[ScreenYMin]
			mov	ebx,[ClipYMax]

			cmp	eax,ebx
			jg	ByeBye

			mov	eax,[ScreenXMax]
			mov	ebx,[ClipXMin]

			cmp	eax,ebx
			jl	ByeBye

			mov	eax,[ScreenYMax]
			mov	ebx,[ClipYMin]

			cmp	eax,ebx
			jnl	EntityCreation

			jmp	ByeBye

			ALIGN	4
@@TextureChange:
			mov	[ObjPtrMap],ebx
			jmp	@@NoTextureChange
ObjectDisplay		ENDP




; ╔════════════════════════════════════════════════════════════════════════════╗
; ║ BodyDisplay_AlphaBeta: Display a non-animated object (rotation Alp*Bet*Gam)║
; ╟────────────────────────────────────────────────────────────────────────────╢
; ║ Appel : (EAX, EBX, ECX) = (X, Y, Z)                                        ║
; ║         (EDX, ESI, EDI) = (Alpha, Beta, Gamma)                             ║
; ║         Stack           = Off_Body                                         ║
; ║                                                                            ║
; ║                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════╝

BodyDisplay_AlphaBeta	PROC
			push	ebp
			push	edx			; Push angles

			push	esi
			push	edi

			mov	ebp,[esp+4+4*4]		; EBP = PTR on object
			mov	edx,7FFFFFFFh

			mov	[lBody],ebp
			mov	[ScreenXMin],edx	; Init 2D box

			mov	[ScreenYMin],edx
			inc	edx			; edx = 80000000h

			mov	[ScreenXMax],edx
			mov	edi,[TypeProj]

			mov	[ScreenYMax],edx
			cmp	edi,TYPE_3D

			jne	@@Init_Iso		; Object position

			mov	esi,Offset MatriceWorld
			call	[LongRotatePoint]	; If perspective

			mov	eax,[X0]
			mov	ebx,[Y0]

			mov	ecx,[Z0]
@@Init_Iso:		mov	edi,[CameraXr]

			sub	eax,edi
			mov	edi,[CameraYr]

			sub	ebx,edi
			mov	edi,[CameraZr]

			sub	ecx,edi
			mov	[PosXWr],eax		; Object world position

			mov	[PosYWr],ebx
			mov	[PosZWr],ecx


; ┌─────────────────────────────────────────────────────────────────┐
; │                                                                 │
; │                          GEOMETRY                               │
; │                                                                 │
; └─────────────────────────────────────────────────────────────────┘

			; **************
			; *** MATRIX ***
			; **************

			mov	esi,Offset MatriceWorld
			pop	ecx			; Get angles

			mov	edi,Offset TabMat+SIZE_MATRICE
			pop	ebx

			xor	eax,eax			; Alpha = 0
			call	[RotateMatrix]		; Mat_World*Mat_Obj

			pop	eax			; Restore Alpha
			mov	esi,Offset TabMat+SIZE_MATRICE

			xor	ebx,ebx			; Beta = 0
			mov	edi,Offset TabMat

			xor	ecx,ecx
			jmp	Return_BodyDisplay

BodyDisplay_AlphaBeta	ENDP






Init_TestVisible	PROC
			mov	[Chooser_RtnI], Offset TestVisibleI	; Integer
			mov	[Chooser_RtnF], Offset TestVisibleF	; FPU
			mov	[Chooser_Result], Offset TestVisible	; Pointer
			jmp	Chooser
Init_TestVisible	ENDP






; ╔═════════════════════════════════════════════════════════════════╗
; ║ TestVisible : Tests the visibility of a poly                    ║
; ╟─────────────────────────────────────────────────────────────────╢
; ║ Call: ESI = Pointer on poly structure (P1 P2 P3)                ║
; ╟─────────────────────────────────────────────────────────────────╢
; ║ Return: Carry = 0 if the poly is visible                        ║
; ║         EBX = Pointer on P1                                     ║
; ║         ECX = Pointer on P2                                     ║
; ║         EBP = Pointer on P3                                     ║
; ║                                                                 ║
; ╚═════════════════════════════════════════════════════════════════╝

TestVisibleI		PROC
			mov	ebx,dword ptr [esi].Poly3_P1
			mov	ebp,dword ptr [esi].Poly3_P3

			mov	ecx,ebx
			and	ebp,0FFFFh			; EBP = P3

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	eax,dword ptr [Obj_ListProjectedPoints+ebp*4].Pt_XE

			cmp	eax,80008000h
			je	@@Eject

			mov	eax,dword ptr [Obj_ListProjectedPoints+ecx*4].Pt_XE
			mov	edx,dword ptr [Obj_ListProjectedPoints+ebx*4].Pt_XE

			cmp	eax,80008000h
			je	@@Eject

			cmp	edx,80008000h
			je	@@Eject1


;(x2-x1)*(y1-y3)-(y2-y1)*(x1-x3)
;   P1  *  P2   -   P3  *  P4

			push	ebx
			mov	ebx,dword ptr [Obj_ListProjectedPoints+ebx*4].Pt_XE

	       		push	ecx
			mov	edx,ebx

			shl	edx,16
			mov	ecx,dword ptr [Obj_ListProjectedPoints+ecx*4].Pt_XE

			sar	edx,16
			mov	eax,ecx

			shl	eax,16
			push	esi

			sar	eax,16
			mov	esi,dword ptr [Obj_ListProjectedPoints+ebp*4].Pt_XE

			sub	eax,edx
			mov	edx,ebx

			sar	edx,16
			push	edi

			sar	ecx,16
			mov	edi,esi

			sar	edi,16

			sub	edx,edi
			mov	edi,ebx

			imul	eax,edx

			sar	ebx,16

			shl	edi,16
			sub	ecx,ebx

			sar	edi,16

			shl	esi,16

			sar	esi,16

			sub	edi,esi

			imul	ecx,edi

			sub	eax,ecx
			pop	edi

			add	eax,eax			; Copy sign in carry
			pop	esi

			pop	ecx
			pop	ebx

			ret

@@Eject:
			add	eax,-1			; Set Carry Flag
			ret
@@Eject1:
			add	edx,-1
			ret
TestVisibleI		ENDP





; ╔═════════════════════════════════════════════════════════════════╗
; ║ TestVisible : Tests the visibility of a poly                    ║
; ╟─────────────────────────────────────────────────────────────────╢
; ║ Call: ESI = Pointer on poly structure (P1 P2 P3)                ║
; ╟─────────────────────────────────────────────────────────────────╢
; ║ Return: Carry = 0 if the poly is visible                        ║
; ║         EBX = Pointer on P1                                     ║
; ║         ECX = Pointer on P2                                     ║
; ║         EBP = Pointer on P3                                     ║
; ║                                                                 ║
; ╚═════════════════════════════════════════════════════════════════╝

TestVisibleF		PROC
			mov	ebx,dword ptr [esi].Poly3_P1
			mov	ebp,dword ptr [esi].Poly3_P3

			mov	ecx,ebx
			and	ebp,0FFFFh			; EBP = P3

			shr	ecx,16				; ECX = P2
			and	ebx,0FFFFh			; EBX = P1

			mov	eax,dword ptr [Obj_ListProjectedPoints+ebp*4].Pt_XE

			cmp	eax,80008000h
			je	@@Eject

			mov	eax,dword ptr [Obj_ListProjectedPoints+ebx*4].Pt_XE
			mov	edx,dword ptr [Obj_ListProjectedPoints+ecx*4].Pt_XE

			cmp	eax,80008000h
			je	@@Eject

			cmp	edx,80008000h
			je	@@Eject1


;(x2-x1)*(y1-y3)-(y2-y1)*(x1-x3)
;   P1  *  P2   -   P3  *  P4
							; 0   1   2   3   4   5   6   7
	fild	[Obj_ListProjectedPoints+ebx*4].Pt_XE	; x1
	fild	[Obj_ListProjectedPoints+ebx*4].Pt_YE	; y1  x1
	fild	[Obj_ListProjectedPoints+ecx*4].Pt_XE	; x2  y1  x1
	fild	[Obj_ListProjectedPoints+ebp*4].Pt_YE	; y3  x2  y1  x1
	fild	[Obj_ListProjectedPoints+ecx*4].Pt_YE	; y2  y3  x2  y1  x1
	fild	[Obj_ListProjectedPoints+ebp*4].Pt_XE	; x3  y2  y3  x2  y1  x1
	fld	st(5)					; x1  x3  y2  y3  x2  y1  x1
	fsubp	st(4),st				; x3  y2  y3  P1  y1  x1
	fld	st(4)					; y1  x3  y2  y3  P1  y1  x1
	fsubrp	st(3),st				; x3  y2  P2  P1  y1  x1
	fsubp	st(5),st				; y2  P2  P1  y1  P4
	fsubrp	st(3),st				; P2  P1  P3  P4
	fmulp	st(1),st				; PP1 P3  P4
	fxch	st(1)					; P3  PP1 P4
	; *
	fmulp	st(2),st				; PP1 PP2
	push	eax
	; 1
	fsubrp	st(1),st
	; 2+1
	fstp	dword ptr [esp]

	pop	eax

	add	eax,eax

			ret
@@Eject:
			add	eax,-1			; Set Carry Flag
			ret
@@Eject1:
			add	edx,-1			; Set Carry Flag
			ret
TestVisibleF		ENDP







; ╔════════════════════════════════════════════════════════════════════════╗
; ║  Le Quick-Sort                                                         ║
; ╟────────────────────────────────────────────────────────────────────────╢
; ║  Appel :                                                               ║
; ║    EBX  = PTR sur le debut de la table de Tri (1er element)            ║
; ║    ECX  = PTR sur la fin de la table de Tri (Dernier element)          ║
; ║                                                                        ║
; ║    La table de tri est decomposee en DWORD:                            ║
; ║       DW Parametre auxiliere (ie Offset ou numero de la face)          ║
; ║       DW Parametre de tri (ie Z)                                       ║
; ║    (Dans cet ordre, pour des raisons d'optim)                          ║
; ║                                                                        ║
; ╟────────────────────────────────────────────────────────────────────────╢
; ║  Remarque: Il s'agit d'une methode recursive: il faut donc prevoir     ║
; ║            la pile en consequence!!                                    ║
; ║  Au premier abord, c'est incomprehensible, mais avec un schemas sur    ║
; ║  papier, tout s'eclaircit!                                             ║
; ║                                                                        ║
; ║   Le premier element pointe par EBX est compare avec tous les autres   ║
; ║  jusqu'au rang ECX. Si le critere de comparaison n'est pas valide,     ║
; ║  on place le mauvais element a gauche du pivot, on decale ce pivot, et ║
; ║  l'element qui vient d'etre ecrase par le pivot est ecrit a la place   ║
; ║  du mauvais element.                                                   ║
; ║  Ainsi, pour le pivot en cours, la condition fait que les elements     ║
; ║  "mauvais" sont a sa gauche, et les "bons" a sa droite.                ║
; ║  On recommence le processus en divisant le tableau au niveau du pivot  ║
; ║   - un tableau a gauche : du debut jusqu'au pivot                      ║
; ║   - un tableau a droite : du pivot jusqu'au dernier element            ║
; ║  Ainsi, tous les elements sont bien positionnes les uns par rapport aux║
; ║  autres et donc le tableau est trie.                                   ║
; ║                                                                        ║
; ║                                                                        ║
; ╚════════════════════════════════════════════════════════════════════════╝
; Sort from far to near
			ALIGN	4
QuickSort		PROC
			cmp	ecx,ebx			; End test
			jbe	EndQSort

			lea	esi,[ebx+4]
			mov	edx,[ebx]
			mov	edi,ebx

@@Next:			mov	eax,[esi]
			add	esi,4

			cmp	edx,eax
			jl	@@Ordre

			mov	[edi],eax
			mov	eax,[edi+4]
			mov	[esi-4],eax
			mov	[edi+4],edx
			add	edi,4

@@Ordre:		cmp	esi,ecx
			jbe	@@Next

			push	ecx
			push	edi
			mov	ecx,edi
			call	QuickSort

			pop	ebx
			pop	ecx
			add	ebx,4
			jmp	QuickSort
			ALIGN	4
EndQSort::		ret
QuickSort		ENDP



; Sort from near to far
			ALIGN	4
QuickSortInv		PROC
			cmp	ecx,ebx			; End test
			jbe	EndQSort
			lea	esi,[ebx+4]
			mov	edi,ebx
			mov	edx,[ebx]
@@Next:			mov	eax,[esi]
			add	esi,4
			cmp	edx,eax
			jg	@@Ordre
			mov	[edi],eax
			mov	eax,[edi+4]
			mov	[esi-4],eax
			mov	[edi+4],edx
			add	edi,4
@@Ordre:		cmp	esi,ecx
			jbe	@@Next
			push	ecx
			push	edi
			mov	ecx,edi
			call	QuickSortInv
			pop	ebx
			pop	ecx
			add	ebx,4
			jmp	QuickSortInv
QuickSortInv		ENDP



_TEXT			ENDS


;			The
			End
