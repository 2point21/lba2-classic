;
; ╔════════════════════════════════════════════════════════════════════════════╗
; ║ ┌─────┐ ┬─────┐        ┌─────┐ ┌────┐  ─────┬─                             ║
; ║       │ │     │        │     │ │    │       │                              ║
; ║       │ │     │        │     │ │    │       │                              ║
; ║       │ │     │        │     │ │    │       │                              ║
; ║  ─────┤ │     │ ─────  │     │ ├────┴┐ ┌    │                              ║
; ║       │ │     │        │     │ │     │ │    │   ┌──────────────────────────╢
; ║       │ │     │        │     │ │     │ │    │   │                          ║
; ║       │ │     │        │     │ │     │ │    │   │  MAJ : Le 09/11/1995     ║
; ║ └─────┘ ┴─────┘        └─────┘ └─────┘ └────┘   │                          ║
; ╚═════════════════════════════════════════════════╧══════════════════════════╝
;

COMMENT \

** Optimiser le produit vectoriel dans le backface removing.

** virer le push ebx / pop ebx lors du remplissage de list_entity dans le cas
   d'un poly gouraud mappe.

\


Max_Nb_Polys		EQU	550
Max_Nb_Points		EQU	550
Max_Nb_Spheres		EQU	50
Max_Nb_Lignes		EQU	50



Struc_Entity		STRUC
  XE_Face		dw	0		; Coord_X a l'ecran
  YE_Face		dw	0		; Coord_Y a l'ecran
  U_Face		dw	0		; Map_U
  V_Face		dw	0		; Map_V
  Coul_Face		dw	0		; Couleur du point ou de la face
  ZO_Face		dd	0		; Z du point dans le repere de l'observateur
Struc_Entity		ENDS








			.486P
			.MODEL	FLAT, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
;                  █▄ ▄█       █▀▀▀█ █▀▀█      █ █▀▀▀▀ ▀▀█▀▀
;                  ██▀ █       ██  █ ██▀▀█ ▄▄  █ ██▀▀    ██
;                  ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀
;*══════════════════════════════════════════════════════════════════════════*

;*──────────────────────────────────────────────────────────────────────────*
			.DATA
;*──────────────────────────────────────────────────────────────────────────*
PUBLIC	SYSCALL	PosXWr
PUBLIC	SYSCALL	PosYWr
PUBLIC	SYSCALL	PosZWr
PUBLIC	SYSCALL	TotalEntite
PUBLIC	SYSCALL	OffsetDefPoint
PUBLIC	SYSCALL	PointeurListTri
PUBLIC	SYSCALL	ListGroupAnim
PUBLIC	SYSCALL	NbGroupes
PUBLIC	SYSCALL	ListGroupe
PUBLIC	SYSCALL	List_Tri
PUBLIC	SYSCALL	List_Normal
PUBLIC	SYSCALL	List_Entity
PUBLIC	SYSCALL	TabMat
PUBLIC	SYSCALL	ObjPtrMap
PUBLIC	SYSCALL	Line_In_Deg
PUBLIC	SYSCALL	Object_Flags
;*──────────────────────────────────────────────────────────────────────────*

INCLUDE			p_data.ash

			; extern LIB_SVGA

			extrn	SYSCALL RepMask:DWORD
			extrn	SYSCALL PtrMap:DWORD


			; extern P_ANIM

			extrn	SYSCALL TransFctBody:DWORD

;*──────────────────────────────────────────────────────────────────────────*

			public	SYSCALL List_Point
			public	SYSCALL List_Anim_Point
			public	SYSCALL NbPoints

			; Extrn SCMINMAX.ASM de LIB_SYSn
			Extrn	SYSCALL ScreenXmin			:	DWORD
			Extrn	SYSCALL ScreenYmin			:	DWORD
			Extrn	SYSCALL ScreenXmax			:	DWORD
			Extrn	SYSCALL ScreenYmax			:	DWORD

;*──────────────────────────────────────────────────────────────────────────*

Trans_X			equ	dword ptr[patch_TX+3]
Trans_Y			equ	dword ptr[patch_TY+3]
Trans_Z			equ	dword ptr[patch_TZ+3]

Mat_00			equ	dword ptr[patch_M00+2]
Mat_01			equ	dword ptr[patch_M01+2]
Mat_02			equ	dword ptr[patch_M02+2]
Mat_10			equ	dword ptr[patch_M10+2]
Mat_11			equ	dword ptr[patch_M11+2]
Mat_12			equ	dword ptr[patch_M12+2]
Mat_20			equ	dword ptr[patch_M20+2]
Mat_21			equ	dword ptr[patch_M21+1]
Mat_22			equ	dword ptr[patch_M22+2]

End_Loop		equ	dword ptr[patch_End+2]

Delta0			equ	dword ptr[patch_Delta0+3]
Delta1			equ	dword ptr[patch_Delta1+3]
Delta2			equ	dword ptr[patch_Delta2+3]

;*──────────────────────────────────────────────────────────────────────────*

; attention voir ANIMIT pour defines maxs


			ALIGN	4

Lbody			dd	0

List_Point		dd	(Max_Nb_Points*3)/2 dup(0)	; Xp Yp Zrot

List_Normal		dd	750/2	dup (0)			; surement plus

List_Anim_Point		dd	Max_Nb_Points*3/2 dup (0)	; Xr Yr Zr

			ALIGN	4
List_Entity		db	(Max_Nb_Polys*(2+4*(Size Struc_Entity))) dup(0)
						; Description des faces
						; En-tete:
						;   Type du poly BYTE
						;   Nb de pts    BYTE
						;   au pire:4* Struc_Entity

			ALIGN	4
List_Tri		db	(Max_Nb_Polys+Max_Nb_Lignes+Max_Nb_Spheres)*8 dup(0)
						; entites a trier

			ALIGN	4
TabMat			dd	9*30	dup (?)		; 30 Matrix max

ZMax			dd	0

NbGroupes		dd	0

TotalEntite		dd	0

Count1			dd	0

NbPoints		dd	0

compteur		dd	0

ObjPtrMap		dd	0

PointeurListTri		dd	0
NextDI			dd	0

ListGroupe		dd	0
ListGroupAnim		dd	0

StartInfos		dd	0
OffsetDefPoint		dd	0
OffsetListNormal 	dd	0

CoulTempo		dd	0

save1			dd	0
save2			dd	0
ptr1			dd	0
ptr2			dd	0

PosXWr			dd	0		; pos World rotée de l'objet à afficher
PosYWr			dd	0
PosZWr			dd	0

DummyObj		OBJ_3D	<>

;*──────────────────────────────────────────────────────────────────────────*

			ALIGN	4
TabJump			dd	aff_E_LIGNE
			dd	aff_E_POLY
			dd	aff_E_SPHERE


Object_Flags		dd	0		; Flags sur l'objet en cours (ie: Environment a gerer?)
;						INFO_NON_TRI	equ	1
;						INFO_ANIM	equ	2
;						INFO_ENV	equ	4

Off_List_Anim_Point	dd	0





Line_In_Deg		db	0			; No de ligne de depart dans un Gouraud Compatible




;*──────────────────────────────────────────────────────────────────────────*
			.CODE

;Seg_Code		SEGMENT USE32 PAGE PUBLIC

;*──────────────────────────────────────────────────────────────────────────*


public	SYSCALL	BodyDisplay
public	SYSCALL	BodyDisplay_AlphaBeta
public	SYSCALL	ObjectDisplay
public	SYSCALL	TestVuePoly
public	SYSCALL	TestVuePolyOld

public	SYSCALL		First_3DPatch
public	SYSCALL		Last_3DPatch

;*──────────────────────────────────────────────────────────────────────────*

extrn	SYSCALL		RotMatIndex_A		:	PROC
extrn	SYSCALL		LongWorldRotatePoint	:	PROC

					; extrn LIB_SVGA
extrn	SYSCALL	Line_A			:	PROC

					; FILLER.ASM
Extrn	SYSCALL	Fill_Poly		:	PROC
Extrn	SYSCALL	Fill_Sphere		:	PROC

					; P_FUNC.ASM
Extrn	SYSCALL	Sqr_A			:	PROC


;*══════════════════════════════════════════════════════════════════════════*
;*              █▀▀▀█ █▀▀▀▀ █▀▀▀▀ █▀▀▀█ █▀▀█      █ █▀▀▀▀ ▀▀█▀▀             *
;*              ██▀▀█ ██▀▀  ██▀▀  ██  █ ██▀▀█ ▄▄  █ ██▀▀    ██              *
;*              ▀▀  ▀ ▀▀    ▀▀    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀              *
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*
;*══════════════════════════════════════════════════════════════════════════*




;//***************************************************************************
BodyDisplay_AlphaBeta	PROC	USES ebp
;			lbody:DWORD
;			x     : eax,	y    : ebx,	z     : ecx
;			alpha : edx, 	beta : esi, 	gamma : edi
;*--------------------------------------------------------------------------
;* rotation world org obj
			mov	ebp,[esp+8]	; lbody
			mov	[lAlpha],edx
			xor	edi,edi			; lGamma = 0
			mov	[Lbody],ebp
			mov	edx,7FFFFFFFh
			mov	[lBeta],esi
			mov	[lGamma],edi

			mov	[ScreenXmin],edx
			mov	[ScreenYmin],edx

			inc	edx			; edx = 80000000h

			cmp	[TypeProj],TYPE_3D

			mov	[ScreenXmax],edx
			mov	[ScreenYmax],edx

			jne	iso0

			call	LongWorldRotatePoint

			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
iso0:
			sub	eax,[CameraXr]
			sub	ebx,[CameraYr]
			sub	ecx,[CameraZrClip]

;			jge	badfinafobj

			mov	[PosXWr],eax
			mov	[PosYWr],ebx
			mov	[PosZWr],ecx

;*--------------------------------------------------------------------------
;* recup infos

			mov	esi,[Lbody]

			mov	[TotalEntite],0
			mov	[PointeurListTri],offset List_Tri

			mov	eax,[esi+14]
			mov	ebx,[esi]
			and	eax,0FFFFh		; nb bytes to skip
			and	ebx,0FFFFh		; infos

			lea	esi,[esi+eax+16]	; saute zone info
			mov	[Object_Flags],ebx

;*--------------------------------------------------------------------------*
;* rotation nuage/normal face/normal point


			test	ebx,INFO_ANIM
			jne	badfinafobj

;*══════════════════════════════════════════════════════════════════════════*
;         rotate                        ██▄ █ █   █ █▀▀▀█ █▀▀▀▀ █▀▀▀▀
;                                       ██▀██ ██  █ ██▀▀█ ██ ▀█ ██▀▀
;                                 ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

			mov	eax,[esi]			; nb points
			add	esi,2
			and	eax,0FFFFh
			je	badfinafobj
;			mov	NbPoints,eax
			mov	[compteur],eax
			mov	[ptr1],esi

;*--------------------------------------------------------------------------*/
; rotations/translations/projection

			mov	esi,offset LMatriceWorld	; Matrice source
			mov	edi,offset TabMat+9*4		; Matrice dest

			push	[lAlpha]
			mov	[lAlpha],0
			call	RotMatIndex_A			; rot de esi-> vers edi->
								; compute group matrix
			pop	[lAlpha]

			mov	esi,offset TabMat+9*4
			mov	edi,Offset TabMat

			mov	[lBeta],0
			call	RotMatIndex_A

			jmp	Return_BodyDisplay
BodyDisplay_AlphaBeta	ENDP




;//***************************************************************************
BodyDisplay		PROC	USES ebp
;			lbody:DWORD
;			x     : eax,	y    : ebx,	z     : ecx
;			alpha : edx, 	beta : esi, 	gamma : edi
;*--------------------------------------------------------------------------
;* rotation world org obj
			mov	ebp,[esp+8]	; lbody
			mov	[lAlpha],edx
			mov	[Lbody],ebp
			mov	edx,7FFFFFFFh
			mov	[lBeta],esi
			mov	[lGamma],edi

			mov	[ScreenXmin],edx
			mov	[ScreenYmin],edx

			inc	edx			; edx = 80000000h

			cmp	[TypeProj],TYPE_3D

			mov	[ScreenXmax],edx
			mov	[ScreenYmax],edx

			jne	iso0

			call	LongWorldRotatePoint

			mov	eax,[X0]
			mov	ebx,[Y0]
			mov	ecx,[Z0]
iso0:
			sub	eax,[CameraXr]
			sub	ebx,[CameraYr]
			sub	ecx,[CameraZrClip]

;			jge	badfinafobj

			mov	[PosXWr],eax
			mov	[PosYWr],ebx
			mov	[PosZWr],ecx

;*--------------------------------------------------------------------------
;* recup infos

			mov	esi,[Lbody]

			mov	[TotalEntite],0
			mov	[PointeurListTri],offset List_Tri

			mov	eax,[esi+14]
			mov	ebx,[esi]
			and	eax,0FFFFh		; nb bytes to skip
			and	ebx,0FFFFh		; infos

			lea	esi,[esi+eax+16]	; saute zone info
			mov	[Object_Flags],ebx

;*--------------------------------------------------------------------------*
;* rotation nuage/normal face/normal point


			test	ebx,INFO_ANIM
			jne	badfinafobj

;*══════════════════════════════════════════════════════════════════════════*
;         rotate                        ██▄ █ █   █ █▀▀▀█ █▀▀▀▀ █▀▀▀▀
;                                       ██▀██ ██  █ ██▀▀█ ██ ▀█ ██▀▀
;                                 ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

			mov	eax,[esi]			; nb points
			add	esi,2
			and	eax,0FFFFh
			je	badfinafobj
;			mov	NbPoints,eax
			mov	[compteur],eax
			mov	[ptr1],esi

;*--------------------------------------------------------------------------*/
; rotations/translations/projection

			mov	esi,offset LMatriceWorld	; Matrice source
			mov	edi,offset TabMat		; Matrice dest

			call	RotMatIndex_A			; rot de esi-> vers edi->
								; compute group matrix
Return_BodyDisplay::
			mov	edi,offset List_Point		; edi sur List_Point
			test	[Object_Flags],INFO_ENV
			jne	@@Doit_Sauver_Pts

			cmp	[TypeProj],TYPE_3D
			je	rl13d
			jmp	rl1

@@Doit_Sauver_Pts:
			mov	[Off_List_Anim_Point],Offset List_Anim_Point
			cmp	[TypeProj],TYPE_3D
			je	@@Tourne_Sauve_3d
			jmp	@@Tourne_Sauve_Iso

						; *** ISO (Sauve les points tournes dans List_Anim_Point) ***
			ALIGN	4
Sauve_NegZ:
			mov	edi,[ptr2]
			mov	edx,[compteur]
			mov	dword ptr [edi],0
			mov	dword ptr [edi+4],32767
			add	edi,6
			dec	edx
			je	finloop
			ALIGN	4
Loop_Sauve_Iso:
			mov	[compteur],edx
@@Tourne_Sauve_Iso:
			mov	esi,[ptr1]			; ESI = PTR sur le point en cours avant transformation
			mov	[ptr2],edi			; EDI = PTR sur List_Point genere

			movsx	eax,word ptr[esi+0]		; EAX = PX
			movsx	ebx,word ptr[esi+2]		; EBX = PY
			movsx	edx,word ptr[esi+4]		; EDX = PZ
			add	esi,6
			mov	ebp,[TabMat+LT20]
			mov	[ptr1],esi
			mov	esi,[TabMat+LT21]
			imul	ebp,eax
			imul	esi,ebx
			mov	edi,[TabMat+LT22]
			add	ebp,esi
			imul	edi,edx
			mov	esi,[TabMat+LT10]
			add	ebp,edi
			mov	edi,[TabMat+LT11]
			sar	ebp,14
			imul	esi,eax
			imul	edi,ebx
			mov	ecx,ebp
			add	esi,edi
			mov	edi,[TabMat+LT12]
			add	ebp,PosZWr
			imul	edi,edx
			imul	eax,[TabMat+LT00]
			add	esi,edi
			imul	ebx,[TabMat+LT01]
			imul	edx,[TabMat+LT02]
			add	eax,ebx

	mov	ebx,[Off_List_Anim_Point]
			sar	esi,14
			add	eax,edx
	mov	[ebx+2],si
			add	esi,PosYWr
			sar	eax,14
	mov	[ebx],ax
	add	ebp,[PosZWr]
	mov	[ebx+4],bp
	add	ebx,6
	sub	ebp,[PosZWr]
	mov	[Off_List_Anim_Point],ebx

			add	ecx,eax
			add	eax,[PosXWr]
			neg	ecx	   		; - x - z tri
			mov	edi,eax
			sub	eax,ebp			; x - z
			add	ebp,edi			; x + z
			mov	ebx,esi
			shl	ebp,1
			lea	eax,[eax+eax*2]
			lea	ebp,[ebp+ebp*2]
			shl	esi,4
			add	ebp,ebx
			sar	eax,6			; (x-z)*3/64
			sub	ebp,esi
			add	eax,[XCentre]
			sar	ebp,8			; ((x+z)*6-y*15)/256
			mov	edi,[ptr2]
			add	ebp,[YCentre]

			cmp	eax,32767
			jg	Sauve_overX
			cmp	eax,-32768
			jl	Sauve_belowX
Sauve_fixedX:
			mov	[edi],ax		; stock Xp
			cmp	eax,[ScreenXmin]
			jl	Sauve_sxmin
Sauve_nosxmin:		cmp	eax,[ScreenXmax]
			jg	Sauve_sxmax
Sauve_nosxmax:
			cmp	ebp,32767
			jg	Sauve_overY
			cmp	ebp,-32768
			jl	Sauve_belowY
Sauve_fixedY:
			mov	[edi+2],bp		; stock Yp
			cmp	ebp,[ScreenYmin]
			jl	Sauve_symin
Sauve_nosymin:		cmp	ebp,[ScreenYmax]
			jg	Sauve_symax
Sauve_nosymax:
			cmp	ecx,32767
			jg	Sauve_overZ
			cmp	ecx,-32768
			jl	Sauve_belowZ
Sauve_fixedZ:
			mov	[edi+4],cx		; stock Zrot pour tri

			mov	edx,[compteur]
			add	edi,6
			dec	edx
			jnz	Loop_Sauve_Iso

			jmp	finloop

;*--------------------------------------------------------------------------*/

Sauve_sxmin:		mov	[ScreenXmin], eax
			jmp	Sauve_nosxmin
Sauve_sxmax:		mov	[ScreenXmax], eax
			jmp	Sauve_nosxmax
Sauve_symin:		mov	[ScreenYmin], ebp
			jmp	Sauve_nosymin
Sauve_symax:		mov	[ScreenYmax], ebp
			jmp	Sauve_nosymax

Sauve_overX:          	mov	eax, 32767
			jmp	Sauve_fixedX
Sauve_belowX:		mov	eax, -32768
			jmp	Sauve_fixedX

Sauve_overY:          	mov	ebp, 32767
			jmp	Sauve_fixedY
Sauve_belowY:		mov	ebp, -32768
			jmp	Sauve_fixedY

Sauve_overZ:          	mov	ecx, 32767
			jmp	Sauve_fixedZ
Sauve_belowZ:		mov	ecx, -32768
			jmp	Sauve_fixedZ


	; **********************************************************
	; *** 3D (Sauve les points tournes dans List_Anim_Point) ***
	; **********************************************************
			ALIGN	4
Sauve_negZ3d:
			mov	edi,[ptr2]
			mov	edx,[compteur]
			mov	dword ptr [edi],0
			mov	dword ptr [edi+4],32767
			add	edi,6
			dec	edx
			jz	finloop
			ALIGN	4
Loop_Sauve_3d:
			mov	[compteur],edx
@@Tourne_Sauve_3D:
			mov	esi,[ptr1]			; ESI = PTR sur le point qui va etre transforme
			mov	[ptr2],edi			; EDI = PTR sur List_Point genere

			movsx	eax,word ptr[esi+0]
			movsx	ebx,word ptr[esi+2]
			movsx	edx,word ptr[esi+4]
			add	esi,6
			mov	ebp,[TabMat+LT20]
			mov	[ptr1],esi
			mov	esi,[TabMat+LT21]
			imul	ebp,eax
			imul	esi,ebx
			mov	edi,[TabMat+LT22]
			add	ebp,esi
			imul	edi,edx
			mov	esi,[TabMat+LT10]
			add	ebp,edi
			mov	edi,[TabMat+LT11]
			sar	ebp,14
			imul	esi,eax
			neg     ebp
			imul	edi,ebx
			mov	ecx,ebp
			add	esi,edi
			mov	edi,[TabMat+LT12]
			sub	ebp,PosZWr
			jle	negZ3d
			imul	edi,edx
			imul	eax,[TabMat+LT00]
			add	esi,edi
			imul	ebx,[TabMat+LT01]
			imul	edx,[TabMat+LT02]
			add	eax,ebx
	mov	ebx,[Off_List_Anim_Point]
			sar	esi,14
			add	eax,edx
	mov	[ebx+2],si
			add	esi,PosYWr
			sar	eax,14
	mov	[ebx],ax
	add	ebp,[PosZWr]
	mov	[ebx+4],bp
	add	ebx,6
	sub	ebp,[PosZWr]
	mov	[Off_List_Anim_Point],ebx
			add	ebp,[KFactor]		; ....
			add	eax,PosXWr
			neg	esi			; -Y
			imul	LFactorX		; X
			idiv	ebp
			add	eax,XCentre
			mov	edi,[ptr2]
			cmp	eax,32767
			jg	Sauve_overX3d
			cmp	eax,-32768
			jl	Sauve_belowX3d
Sauve_fixedX3d:
			mov	[edi], ax		; stock Xp
			cmp	eax, ScreenXmin
			jl	Sauve_sxmin3d
Sauve_nosxmin3d:	cmp	eax, ScreenXmax
	  		jg	Sauve_sxmax3d
Sauve_nosxmax3d:
			mov	eax,esi
			imul	LFactorY		; -Y
			idiv	ebp
			add	eax,YCentre
			cmp	eax,32767
			jg	Sauve_overY3d
			cmp	eax,-32768
			jl	Sauve_belowY3d
Sauve_fixedY3d:
			mov	[edi+2],ax		; stock Yp
			cmp	eax, ScreenYmin
			jl	Sauve_symin3d
Sauve_nosymin3d:	cmp	eax, ScreenYmax
			jg	Sauve_symax3d
Sauve_nosymax3d:
			cmp	ecx, 32767
			jg	Sauve_overZ3d
			cmp	ecx, -32768
			jl	Sauve_belowZ3d
Sauve_fixedZ3d:
			mov	[edi+4], cx		; stock Zrot pour tri

			mov	edx,[compteur]
			add	edi,6
			dec	edx
			jnz	Loop_Sauve_3D

			jmp	finloop

;*--------------------------------------------------------------------------*

Sauve_sxmin3d:		mov	[ScreenXmin], eax
			jmp	Sauve_nosxmin3d
Sauve_sxmax3d:		mov	[ScreenXmax], eax
			jmp	Sauve_nosxmax3d
Sauve_symin3d:		mov	[ScreenYmin], eax
			jmp	Sauve_nosymin3d
Sauve_symax3d:		mov	[ScreenYmax], eax
			jmp	Sauve_nosymax3d

Sauve_overX3d:        	mov	eax, 32767
			jmp	Sauve_fixedX3d
Sauve_belowX3d:		mov	eax, -32768
			jmp	Sauve_fixedX3d

Sauve_overY3d:        	mov	eax, 32767
			jmp	Sauve_fixedY3d
Sauve_belowY3d:		mov	eax, -32768
			jmp	Sauve_fixedY3d

Sauve_overZ3d:        	mov	ecx, 32767
			jmp	Sauve_fixedZ3d
Sauve_belowZ3d:		mov	ecx, -32768
			jmp	Sauve_fixedZ3d


;*--------------------------------------------------------------------------*/
; ISO

			ALIGN	4
negZ:
			mov	edi,[ptr2]
			mov	edx,[compteur]
			mov	dword ptr [edi],0
			mov	dword ptr [edi+4],32767
			add	edi,6
			dec	edx
			jz	finloop
			ALIGN	4
rl0:
			mov	[compteur],edx
rl1:
			mov	esi,[ptr1]			; ESI = PTR sur le point en cours avant transformation
			mov	[ptr2],edi			; EDI = PTR sur List_Point genere

			movsx	eax,word ptr[esi+0]		; EAX = PX
			movsx	ebx,word ptr[esi+2]		; EBX = PY
			movsx	edx,word ptr[esi+4]		; EDX = PZ
			add	esi,6
			mov	ebp,[TabMat+LT20]
			mov	[ptr1],esi
			mov	esi,[TabMat+LT21]
			imul	ebp,eax
			imul	esi,ebx
			mov	edi,[TabMat+LT22]
			add	ebp,esi
			imul	edi,edx
			mov	esi,[TabMat+LT10]
			add	ebp,edi
			mov	edi,[TabMat+LT11]
			sar	ebp,14
			imul	esi,eax
			imul	edi,ebx
			mov	ecx,ebp
			add	esi,edi
			mov	edi,[TabMat+LT12]
			add	ebp,PosZWr
			imul	edi,edx
			imul	eax,[TabMat+LT00]
			add	esi,edi
			imul	ebx,[TabMat+LT01]
			imul	edx,[TabMat+LT02]
			add	eax,ebx
			sar	esi,14
			add	eax,edx
			add	esi,[PosYWr]
			sar	eax,14
			add	ecx,eax
			add	eax,[PosXWr]
			neg	ecx	   		; - x - z tri
			mov	edi,eax
			sub	eax,ebp			; x - z
			add	ebp,edi			; x + z
			mov	ebx,esi
			shl	ebp,1
			lea	eax,[eax+eax*2]
			lea	ebp,[ebp+ebp*2]
			shl	esi,4
			add	ebp,ebx
			sar	eax,6			; (x-z)*3/64
			sub	ebp,esi
			add	eax,[XCentre]
			sar	ebp,8			; ((x+z)*6-y*15)/256
			mov	edi,[ptr2]
			add	ebp,[YCentre]

			cmp	eax,32767
			jg	overX
			cmp	eax,-32768
			jl	belowX
fixedX:
			mov	[edi],ax		; stock Xp
			cmp	eax,[ScreenXmin]
			jl	sxmin
nosxmin:		cmp	eax,[ScreenXmax]
			jg	sxmax
nosxmax:
			cmp	ebp,32767
			jg	overY
			cmp	ebp,-32768
			jl	belowY
fixedY:
			mov	[edi+2],bp		; stock Yp
			cmp	ebp,[ScreenYmin]
			jl	symin
nosymin:		cmp	ebp,[ScreenYmax]
			jg	symax
nosymax:
			cmp	ecx,32767
			jg	overZ
			cmp	ecx,-32768
			jl	belowZ
fixedZ:
			mov	[edi+4],cx		; stock Zrot pour tri

			mov	edx,[compteur]
			add	edi,6
			dec	edx
			jnz	rl0

			jmp	finloop

;*--------------------------------------------------------------------------*/

sxmin:			mov	ScreenXmin, eax
			jmp	nosxmin
sxmax:			mov	ScreenXmax, eax
			jmp	nosxmax
symin:			mov	ScreenYmin, ebp
			jmp	nosymin
symax:			mov	ScreenYmax, ebp
			jmp	nosymax

overX:          	mov	eax, 32767
			jmp	fixedX
belowX:			mov	eax, -32768
			jmp	fixedX

overY:          	mov	ebp, 32767
			jmp	fixedY
belowY:			mov	ebp, -32768
			jmp	fixedY

overZ:          	mov	ecx, 32767
			jmp	fixedZ
belowZ:			mov	ecx, -32768
			jmp	fixedZ

;*--------------------------------------------------------------------------*/
; 3D

			ALIGN	4
negZ3d:
			mov	edi,[ptr2]
			mov	edx,[compteur]
			mov	dword ptr [edi],0
			mov	dword ptr [edi+4],32767
			add	edi,6
			dec	edx
			jz	finloop
			ALIGN	4
rl03d:
			mov	[compteur],edx
rl13d:
			mov	esi,[ptr1]			; ESI = PTR sur le point qui va etre transforme
			mov	[ptr2],edi			; EDI = PTR sur List_Point genere

			movsx	eax,word ptr[esi+0]
			movsx	ebx,word ptr[esi+2]
			movsx	edx,word ptr[esi+4]
			add	esi,6
			mov	ebp,[TabMat+LT20]
			mov	[ptr1],esi
			mov	esi,[TabMat+LT21]
			imul	ebp,eax
			imul	esi,ebx
			mov	edi,[TabMat+LT22]
			add	ebp,esi
			imul	edi,edx
			mov	esi,[TabMat+LT10]
			add	ebp,edi
			mov	edi,[TabMat+LT11]
			sar	ebp,14
			imul	esi,eax
			neg     ebp
			imul	edi,ebx
			mov	ecx,ebp
			add	esi,edi
			mov	edi,[TabMat+LT12]
			sub	ebp,PosZWr
			jle	negZ3d
			imul	edi,edx
			imul	eax,[TabMat+LT00]
			add	esi,edi
			imul	ebx,[TabMat+LT01]
			imul	edx,[TabMat+LT02]
			add	eax,ebx
			sar	esi,14
			add	eax,edx
			add	esi,PosYWr
			sar	eax,14
			add	ebp,[KFactor]		; ....
			add	eax,PosXWr
			neg	esi			; -Y
			imul	LFactorX		; X
			idiv	ebp
			add	eax,XCentre
			mov	edi,[ptr2]
			cmp	eax,32767
			jg	overX3d
			cmp	eax,-32768
			jl	belowX3d
fixedX3d:
			mov	[edi], ax		; stock Xp
			cmp	eax, ScreenXmin
			jl	sxmin3d
nosxmin3d:		cmp	eax, ScreenXmax
	  		jg	sxmax3d
nosxmax3d:
			mov	eax,esi
			imul	LFactorY		; -Y
			idiv	ebp
			add	eax,YCentre
			cmp	eax,32767
			jg	overY3d
			cmp	eax,-32768
			jl	belowY3d
fixedY3d:
			mov	[edi+2],ax		; stock Yp
			cmp	eax, ScreenYmin
			jl	symin3d
nosymin3d:		cmp	eax, ScreenYmax
			jg	symax3d
nosymax3d:
			cmp	ecx, 32767
			jg	overZ3d
			cmp	ecx, -32768
			jl	belowZ3d
fixedZ3d:
			mov	[edi+4],cx		; stock Zrot pour tri

			mov	edx,compteur
			add	edi,6
			dec	edx
			jnz	rl03d

			jmp	finloop

;*--------------------------------------------------------------------------*

sxmin3d:		mov	ScreenXmin, eax
			jmp	nosxmin3d
sxmax3d:		mov	ScreenXmax, eax
			jmp	nosxmax3d
symin3d:		mov	ScreenYmin, eax
			jmp	nosymin3d
symax3d:		mov	ScreenYmax, eax
			jmp	nosymax3d

overX3d:        	mov	eax, 32767
			jmp	fixedX3d
belowX3d:		mov	eax, -32768
			jmp	fixedX3d

overY3d:        	mov	eax, 32767
			jmp	fixedY3d
belowY3d:		mov	eax, -32768
			jmp	fixedY3d

overZ3d:        	mov	ecx, 32767
			jmp	fixedZ3d
belowZ3d:		mov	ecx, -32768
			jmp	fixedZ3d

;*--------------------------------------------------------------------------*/
;*--------------------------------------------------------------------------*/
;*--------------------------------------------------------------------------*

finloop:

;*══════════════════════════════════════════════════════════════════════════*
;		   ██▄ █ █▀▀▀█ █▀▀▀█ █▄ ▄█ █▀▀▀█ █     ██▀▀▀
;		   ██▀██ ██  █ ██▀█▀ ██▀ █ ██▀▀█ ██    ▀▀▀▀█
;		   ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*/
;*──────────────────────────────────────────────────────────────────────────*/

			mov	esi, [ptr1]

			mov	ebp, [esi]		; nbobj normal faces/points
			add	esi, 2
			and	ebp, 0FFFFh
			jz	FinNuage

			; t(Normallight) * GroupMatrice

			mov	eax, [NormalXlight]
			mov	ebx, [NormalYlight]
			mov	ecx, [NormalZlight]
			mov	edx, [TabMat+LT00]
			mov	edi, [TabMat+LT10]
			imul	edx, eax
			imul	edi, ebx
			add	edi, edx
			mov	edx, [TabMat+LT20]
			imul	edx, ecx
			add	edi, edx
			mov	edx, [TabMat+LT01]
			mov	[LMatD00], edi
			mov	edi, [TabMat+LT11]
			imul	edx, eax
			imul	eax, [TabMat+LT02]
			imul	edi, ebx
			imul	ebx, [TabMat+LT12]
			add	edi, edx
			mov	edx, [TabMat+LT21]
			add	ebx, eax
			imul	edx, ecx
			imul	ecx, [TabMat+LT22]
			add	edi, edx
			add	ebx, ecx
			mov	[LMatD01], edi
			mov	[LMatD02], ebx

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
			mov	edi, offset List_Normal
			ALIGN	4
bouclerot:
			movsx	eax, word ptr[esi+0]
			movsx	ebx, word ptr[esi+2]
			movsx	ecx, word ptr[esi+4]
			imul	eax, [LMatD00]
			imul	ebx, [LMatD01]
			imul	ecx, [LMatD02]
			add	eax, ebx
			xor	edx, edx		; 0 intensity for now
			add	eax, ecx
			js	nointensity		; saut si negatif

			shr	eax, 14
			idiv    word ptr[esi+6]		; prenormalized range
			mov	edx, eax
			ALIGN	4
nointensity:
			add	esi, 8
			mov	[edi], edx		; stock intensity
			add	edi, 2
			dec	ebp
			jnz	bouclerot

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
			jmp	FinNuage
BodyDisplay		ENDP











;*══════════════════════════════════════════════════════════════════════════*
;*══════════════════════════════════════════════════════════════════════════*
;*// OBJECT DISPLAY
;*══════════════════════════════════════════════════════════════════════════*
;*══════════════════════════════════════════════════════════════════════════*

ObjectDisplay		PROC	USES ebp
;			eax:POBJ_3D

			mov	ebp, eax
			mov	ebx, [eax].Texture
			mov	edx, 7FFFFFFFh

			cmp	ebx, -1
			je	skip

			mov	ObjPtrMap, ebx
skip:

			mov	eax, [ebp].ObjAlpha
			mov	ebx, [ebp].ObjBeta
			mov	ecx, [ebp].ObjGamma

			mov	ScreenXmin, edx
			mov	ScreenYmin, edx

			inc	edx			; edx = 80000000h

			mov	lAlpha, eax
			mov	lBeta,  ebx
			mov	lGamma, ecx

			mov	ScreenXmax, edx
			mov	ScreenYmax, edx

;*--------------------------------------------------------------------------
;* rotation world org obj

			mov	eax, [ebp].ObjX		; X World rot
			mov	ebx, [ebp].ObjY		; Y World
			mov	ecx, [ebp].ObjZ		; Z World

			cmp	[TypeProj], TYPE_3D
			jne	iso0

			call	LongWorldRotatePoint

			mov	eax, X0
			mov	ebx, Y0
			mov	ecx, Z0

iso0:
			sub	eax, [CameraXr]
			sub	ebx, [CameraYr]
			sub	ecx, [CameraZrClip]

;			jge	badfinafobj

			mov	[PosXWr], eax
			mov	[PosYWr], ebx
			mov	[PosZWr], ecx

;*--------------------------------------------------------------------------
;* recup infos

			mov	esi, [ebp].Body

			cmp	TransFctBody, 0
			je	skiptrans

			push	esi
			call	TransFctBody
			mov	esi, eax
			add	esp, 4
skiptrans:
			mov	TotalEntite, 0
			mov	[PointeurListTri], offset List_Tri

			movzx	eax, word ptr[esi+14]	; nb bytes to skip
			mov	ebx,dword ptr[esi]	; infos
							; word ptr [esi] contient un champ de bits
							; INFO_NON_TRI	equ	1
							; INFO_ANIM	equ	2
							; INFO_ENV	equ	4
			lea	esi, [esi+eax+16]	; saute zone info

			mov	[Object_Flags],ebx	; Flag de l'objet

;*--------------------------------------------------------------------------*
;* rotation nuage/normal face/normal point


			test	ebx, INFO_ANIM
			jz	badfinafobj

;*══════════════════════════════════════════════════════════════════════════*
;         █▀▀▀█ ██▄ █  █    █▄ ▄█       ██▄ █ █   █ █▀▀▀█ █▀▀▀▀ █▀▀▀▀
;         ██▀▀█ ██▀██  ██   ██▀ █       ██▀██ ██  █ ██▀▀█ ██ ▀█ ██▀▀
;         ▀▀  ▀ ▀▀  ▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*


			movzx	eax, word ptr[esi]	; nb points
			add	esi, 2
			mov	NbPoints, eax
			mov	OffsetDefPoint, esi

			lea	eax, [eax+eax*2]	; *3
			lea	ecx, [ebp].CurrentFrame
			lea	esi, [esi+eax*2]	; *6, esi saute defpoint

			movzx	eax, word ptr[esi]	; nb groups
			add	esi, 2
			mov	ListGroupAnim, ecx
			mov	NbGroupes, eax
			mov	ListGroupe, esi		; memo start list groupe

;*--------------------------------------------------------------------------*/
; start with group 0, always a TYPE_ROTATE

			mov	save1, esi
			mov	Count1, eax

			movzx	eax, word ptr[esi]
			mov	esi, OffsetDefPoint
			mov	edi, Offset List_Anim_Point
			lea	eax, [eax+eax*2]
			sub	esi, edi
			mov	save2, edi
			mov	Trans_X, 0			; XYZ0 ChgRepere: 0
			lea	eax, [edi+eax*2]
			mov	Delta0, esi
			add	esi, 2
			mov	Trans_Y, 0
			mov	Delta1, esi
			add	esi, 2
			mov	[End_Loop], eax
			mov	Delta2, esi
			mov	Trans_Z, 0

			mov	edi, offset TabMat		; Matrice dest
			mov	esi, offset LMatriceWorld	; Matrice source
			mov	ptr1, edi

			mov	eax, lAlpha
			mov	ebx, lBeta
			mov	ecx, lGamma

			jmp	rotateg

;*--------------------------------------------------------------------------*/
; rotations/translations des groupes

			ALIGN	4
ag0:
			mov	Count1, ecx

			mov	eax, ptr1
			add	eax, 36			; next matrix
			mov	ptr1, eax

			mov	edi, ListGroupAnim

			movzx	eax, word ptr[edi]	; type anim groupe
			movsx	ebx, word ptr[edi+2]	; Alpha ou stepX
			movsx	ecx, word ptr[edi+4]	; Beta  ou stepY
			movsx	edx, word ptr[edi+6]	; Gamma ou stepZ
			add	edi, 8
			mov	lAlpha, ebx
			mov	lBeta,  ecx
			mov	lGamma, edx

			mov	ListGroupAnim, edi


			movzx	ebx, word ptr[esi]	; nb points
			movzx	edx, word ptr[esi+2]	; Org Point index deja *
			mov	edi, save2
			lea	ebx, [ebx+ebx*2]
			movzx	esi, word ptr[esi+4]	; orgmatrice deja *
			lea	ebx, [edi+ebx*2]
			add	esi, offset TabMat			; esi Matrice du groupe org
			mov	[End_Loop], ebx
			movsx	ebx, word ptr[List_Anim_Point+edx+0]	; X0
			movsx	ecx, word ptr[List_Anim_Point+edx+2]	; Y0
			movsx	edx, word ptr[List_Anim_Point+edx+4]	; Z0
			shl	ebx, 14
			shl	ecx, 14
			mov	Trans_X, ebx
			shl	edx, 14
			mov	Trans_Y, ecx
			mov	Trans_Z, edx			; XYZ0 ChgRepere

			cmp	eax, TYPE_ROTATE
			mov	eax, lAlpha
			mov	ebx, lBeta
			mov	ecx, lGamma
			jne	translateg		; translation
rotateg:
			or	eax, ebx		; is there actually a rot?
			mov	edi, ptr1		; Matrice dest
			or	eax, ecx		; is there actually a rot?
			jz	copymat			; no so then do same as trans with 0 trans

			call	RotMatIndex_A		; rot de esi-> vers edi->
							; compute group matrix
			mov	esi, ptr1
init:
			mov	edi, [Trans_X]
			mov	eax, [esi].LT00
			mov	ebx, [esi].LT01
			mov	ebp, eax
			mov	ecx, [esi].LT02
			imul	ebp, ebx
			mov	Mat_00, eax
			sub	edi, ebp
			mov	Mat_01, ebx
			mov	[Trans_X], edi
			mov	Mat_02, ecx
			mov	edi, [Trans_Y]
			mov	eax, [esi].LT10
			mov	edx, [esi].LT11
			mov	ebx, [esi].LT21
			mov	ebp, eax
			mov	ecx, [esi].LT12
			imul	ebp, edx
			sub	edx, ebx
			mov	Mat_10, eax
			sub	edi, ebp
			mov	Mat_11, edx
			mov	[Trans_Y], edi
			mov	Mat_12, ecx
			mov	edi, [Trans_Z]
			mov	eax, [esi].LT20
			mov	ebp, eax
			mov	ecx, [esi].LT22
			imul	ebp, ebx
			mov	Mat_20, eax
			sub	edi, ebp
			mov	Mat_21, ebx
			mov	[Trans_Z], edi
			mov	edi, save2		; edi sur List_Point
			mov	Mat_22, ecx
			jmp	rl0
translateg:
			mov	edi, [ esi+LT00 ]	; rot translation vector
			mov	ebp, [ esi+LT01 ]	; add add to final trans
			imul	edi, eax
			imul	ebp, ebx
			mov	edx, [ esi+LT02 ]
			add	edi, ebp
			imul	edx, ecx
			add	edx, edi
			mov	edi, [ esi+LT10 ]
			add	edx, Trans_X
			mov	ebp, [ esi+LT11 ]
			mov	Trans_X, edx
			imul	edi, eax
			imul	ebp, ebx
			mov	edx, [ esi+LT12 ]
			add	edi, ebp
			imul	edx, ecx
			imul	eax, [ esi+LT20 ]
			imul	ebx, [ esi+LT21 ]
			imul	ecx, [ esi+LT22 ]
			add	ebx, eax
			add	edx, edi
			add	ebx, ecx
			add	edx, Trans_Y
			add	ebx, Trans_Z
			mov	Trans_Y, edx
			mov	Trans_Z, ebx
			mov	edi, ptr1		; Matrice dest
copymat:
			mov	ecx, 9
			rep	movsd			; rotation same as org group

			sub	esi, 9*4
			sub	edi, 9*4*2
			cmp	esi, edi
			jne	init
init0:
			mov	eax, [esi].LT00
			mov	ebx, [esi].LT10
			mov	ecx, [esi].LT20
			mov	edi, [Trans_X]
			mov	edx, [Trans_Y]
			mov	ebp, [Trans_Z]
			imul	eax, [esi].LT01
			imul	ebx, [esi].LT11
			imul	ecx, [esi].LT21
			sub	edi, eax
			sub	edx, ebx
			sub	ebp, ecx
			mov	[Trans_X], edi
			mov	edi, save2		; edi sur List_Point
			mov	[Trans_Y], edx
			mov	[Trans_Z], ebp
			jmp	rl0
			ALIGN	4
rl0:
First_3DPatch		label	DWORD

patch_Delta0:		movsx	eax, word ptr[edi+12345678h]	; cplx
patch_Delta1:		movsx	ebx, word ptr[edi+12345678h]	; cplx
patch_Delta2:		movsx	ecx, word ptr[edi+12345678h]	; cplx
			mov	ebp, eax			; U
patch_M01:		lea	edx, [eax+12345678h]		; 	V
patch_M00:		lea	esi, [ebx+12345678h]		; U
			add	edi, 6				;	V
			imul	ebp, ebx			; cplx
			imul	edx, esi			; cplx
patch_M02:		imul	esi, ecx, 12345678h		; cplx
			sub	edx, ebp			; U
patch_M21:		add	eax, 12345678h			; 	V
								; AGI 1 cycle
patch_TX:		lea	edx, [edx+esi+12345678h]	; U
patch_M10:		lea	esi, [ebx+12345678h]		;	V
			sar	edx, 14				; U
patch_M20:		add	ebx, 12345678h			;	V
			mov	[edi-6], dx			; cplx
patch_M11:		lea	edx, [eax+12345678h]		; U
								;
			imul	eax, ebx			; cplx
			imul	edx, esi			; cplx
patch_M12:		imul	esi, ecx, 12345678h		; cplx
			sub	edx, ebp			; U
			sub	eax, ebp			; 	V
								; AGI 1 cycle
patch_TY:		lea	edx, [edx+esi+12345678h]	; U
								;
patch_M22:		imul	esi, ecx, 12345678h		; cplx
			sar	edx, 14				; U
patch_TZ:		lea	ebp, [eax+esi+12345678h]	;	V
			mov	[edi-4], dx			; cplx
			sar	ebp, 14				; U
patch_End:		cmp	edi, 12345678h			;	V
			mov	[edi-2], bp			; U
			jne	rl0				; 	V

Last_3DPatch		label	DWORD

			mov	save2, edi

			mov	esi, save1
			mov	ecx, Count1
			add	esi, 8			; size d'un groupe
			dec	ecx
			mov	save1, esi
			jnz	ag0

;*--------------------------------------------------------------------------*/
; projette liste

project:     		mov	ecx, NbPoints
			mov	esi, offset List_Anim_Point
			mov	edi, offset List_Point

			cmp	[TypeProj], TYPE_3D
			jz	boucleproj3D
boucleproj:
			mov	Count1, ecx
			movsx	eax, word ptr[esi+0]	; eax X rot
			movsx	ebx, word ptr[esi+2]	; ebx Y rot
			movsx	ecx, word ptr[esi+4]	; ecx Z rot
			add	esi, 6

			mov	edx, eax
			add	eax, [PosXWr]
			add	edx, ecx
			add     ecx, [PosZWr]
			add	ebx, [PosYWr]

			lea	ebp, [eax+ecx]		; x + zrot
			sub	eax, ecx 		; x - zrot
			lea	ecx, [eax+eax*2]	; *3
			sar	ecx, 6			; /64 (= * 24 / 512 )
			add	ecx, XCentre

			cmp	ecx, 32767
			jg	overX
			cmp	ecx, -32768
			jl	belowX
fixedX:
			mov	[edi], ecx		; stock Xp

			cmp	ecx, ScreenXmin
			jl	sxmin
nosxmin:		cmp	ecx, ScreenXmax
			jg	sxmax
nosxmax:
			mov	ecx, ebx		; ecx  = ebx
			mov	eax, ebp
			shl	ebx, 4			; ebx *= 16
			lea	eax, [eax+eax*2]	; eax *= 3
			neg	edx			; - Zrot - X pour bon tri
			shl	eax, 1			; eax *= 2
							; eax = (x+zrot) * 6
			sub	ecx, ebx		; ecx = - y*15
			sar	edx, 1
			add	eax, ecx		; + (x-zrot) * 6
			sar	eax, 8			; /256 IsoScale
			add	eax, YCentre

			cmp	eax, 32767
			jg	overY
			cmp	eax, -32768
			jl	belowY
fixedY:
			mov	[edi+2], eax		; stock Yp

			cmp	eax, ScreenYmin
			jl	symin
nosymin:		cmp	eax, ScreenYmax
			jg	symax
nosymax:
			cmp	edx, 32767
			jg	overZ
			cmp	edx, -32768
			jl	belowZ
fixedZ:
			mov	[edi+4], edx		; stock Zrot pour tri
			mov	ecx, Count1
			add	edi, 6
			dec	ecx
			jnz	boucleproj

;*--------------------------------------------------------------------------*

			jmp	ComputeAnimNormal

;*--------------------------------------------------------------------------*
;*--------------------------------------------------------------------------*
;*--------------------------------------------------------------------------*

sxmin:			mov	ScreenXmin, ecx
			jmp	nosxmin
sxmax:			mov	ScreenXmax, ecx
			jmp	nosxmax
symin:			mov	ScreenYmin, eax
			jmp	nosymin
symax:			mov	ScreenYmax, eax
			jmp	nosymax

overX:			mov	ecx, 32767
			jmp	fixedX
belowX:			mov	ecx, -32768
			jmp	fixedX

overY:			mov	eax, 32767
			jmp	fixedY
belowY:			mov	eax, -32768
			jmp	fixedY

overZ:			mov	edx, 32767
			jmp	fixedZ
belowZ:			mov	edx, -32768
			jmp	fixedZ

sxmin3d:		mov	ScreenXmin, eax
			jmp	nosxmin3d
sxmax3d:		mov	ScreenXmax, eax
			jmp	nosxmax3d
symin3d:		mov	ScreenYmin, eax
			jmp	nosymin3d
symax3d:		mov	ScreenYmax, eax
			jmp	nosymax3d

overX3d:		mov	eax, 32767
			jmp	fixedX3d
belowX3d:		mov	eax, -32768
			jmp	fixedX3d

overY3d:		mov	eax, 32767
			jmp	fixedY3d
belowY3d:		mov	eax, -32768
			jmp	fixedY3d

overZ3d:		mov	ecx, 32767
			jmp	fixedZ3d
belowZ3d:		mov	ecx, -32768
			jmp	fixedZ3d

negZ:           	mov	dword ptr[edi], 0
			mov	dword ptr[edi+4], 32767
			mov	ecx, Count1
			add	edi, 6
			dec	ecx
			jz	finloop
boucleproj3D:
			mov	[Count1],ecx
			movsx	eax,word ptr[esi+0]	; eax X rot
			movsx	ebx,word ptr[esi+2]	; ebx Y rot
			movsx	ebp,word ptr[esi+4]	; Z rot, bp = Zrot tri
			add	eax,[PosXWr]
			add	ebx,[PosYWr]
			neg     ebp

			add	esi,6
	 		mov	ecx,ebp

			sub	ebp,[PosZWr]
			jle	negZ

			add	ebp,[KFactor]		; ....

			imul	[LFactorX]		; X
			idiv	ebp
			add	eax,[XCentre]

			cmp	eax,32767
			jg	overX3d
			cmp	eax,-32768
			jl	belowX3d
fixedX3d:
			mov	[edi],eax		; stock Xp

			cmp	eax,[ScreenXmin]
			jl	sxmin3d
nosxmin3d:		cmp	eax,[ScreenXmax]
			jg	sxmax3d
nosxmax3d:

			mov	eax,ebx
			neg	eax
			imul	[LFactorY]		; -Y
			idiv	ebp
			add	eax,[YCentre]

			cmp	eax,32767
			jg	overY3d
			cmp	eax,-32768
			jl	belowY3d
fixedY3d:
			mov	[edi+2],eax		; stock Yp

			cmp	eax,[ScreenYmin]
			jl	symin3d
nosymin3d:		cmp	eax,[ScreenYmax]
			jg	symax3d
nosymax3d:
			cmp	ecx,32767
			jg	overZ3d
			cmp	ecx,-32768
			jl	belowZ3d
fixedZ3d:
			mov	[edi+4],ecx		; stock Zrot pour tri
			mov	ecx,Count1
			add	edi,6
			dec	ecx
			jnz	boucleproj3D

;*--------------------------------------------------------------------------*

finloop:

;*══════════════════════════════════════════════════════════════════════════*
;		   ██▄ █ █▀▀▀█ █▀▀▀█ █▄ ▄█ █▀▀▀█ █     ██▀▀▀
;		   ██▀██ ██  █ ██▀█▀ ██▀ █ ██▀▀█ ██    ▀▀▀▀█
;		   ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*/
;*──────────────────────────────────────────────────────────────────────────*/

ComputeAnimNormal:
			mov	esi,[save1]

			movzx	eax,word ptr[esi]	; nbobj normal faces/points
			add	esi,2
			test	eax,eax
			jz	FinNuage

			mov	ebx,ListGroupe
			mov	eax,offset TabMat
			mov	ecx,NbGroupes
			mov	edi,offset List_Normal
			add	ebx,6			; sur NbNormal
			mov	ptr2,eax
			mov	Count1,ecx
			movzx	ebp,word ptr[ebx]
			mov	save1,ebx
			test	ebp,ebp
			jz	nextgroup
			ALIGN	4
bouclegroup:
			mov	ptr1,edi
			mov	compteur,ebp

			; t(Normallight) * GroupMatrice

			mov	ebx,[NormalXlight]
			mov	ecx,[NormalYlight]
			mov	edx,[NormalZlight]
			mov	ebp,[eax+LT00]
			mov	edi,[eax+LT10]
			imul	ebp,ebx
			imul	edi,ecx
			add	edi,ebp
			mov	ebp,[eax+LT20]
			imul	ebp,edx
			add	edi,ebp
			mov	ebp,[eax+LT01]
			mov	[LMatD00],edi
			mov	edi,[eax+LT11]
			imul	ebp,ebx
			imul	ebx,[eax+LT02]
			imul	edi,ecx
			imul	ecx,[eax+LT12]
			add	edi,ebp
			mov	ebp,[eax+LT21]
			add	ecx,ebx
			imul	ebp,edx
			imul	edx,[eax+LT22]
			add	edi,ebp
			add	edx,ecx
			mov	[LMatD01],edi
			mov	[LMatD02],edx

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
			mov	edi,ptr1
			mov	ebp,compteur
			ALIGN	4
bouclerot:
			movsx	eax,word ptr[esi+0]
			movsx	ebx,word ptr[esi+2]
			movsx	ecx,word ptr[esi+4]
			imul	eax,[LMatD00]
			imul	ebx,[LMatD01]
			imul	ecx,[LMatD02]
			add	eax,ebx
			xor	edx,edx			; 0 intensity for now
			add	eax,ecx
			js	nointensity		; saut si negatif

			shr	eax,14
			idiv    word ptr[esi+6]		; prenormalized range
			mov	edx,eax
			ALIGN	4
nointensity:
			add	esi,8
			mov	[edi],edx		; stock intensity
			add	edi,2
			dec	ebp
			jnz	bouclerot

;''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
			ALIGN	4
nextgroup:
			mov	ebx,[save1]
			mov	eax,[ptr2]
			add	ebx,8
			add	eax,36
			mov	[save1],ebx
	 		mov	edx,Count1
			mov	[ptr2],eax
	 		dec	edx
			movzx	ebp, word ptr[ebx]
			jz	FinNuage

	 		mov	Count1,edx
			test	ebp,ebp
			jnz	bouclegroup
			jmp	nextgroup

;*══════════════════════════════════════════════════════════════════════════*
;            █▀▀▀█ █▀▀▀█ █     █  ▄▀ █▀▀▀▀ █▀▀▀█ ██▄ █ █▀▀▀▀ ██▀▀▀
;            ██▀▀▀ ██  █ ██    ██▀   ██ ▀█ ██  █ ██▀██ ██▀▀  ▀▀▀▀█
;            ▀▀    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*

FinNuage::
			mov	edi,offset List_Entity

			movzx	ecx,word ptr[esi]	; nb polys
			add	esi,2
			test	ecx,ecx			; 0000 => Ligne; si encore
			jz	lignes			; 0000 => Sphere

			ALIGN	4
b_poly:
			push	edi			; SAV du Nb de polys et du PTR
			mov	eax,[esi]
			push	ecx			; dans List_Entity
							; ECX apres car en fin de boucle,
							; on fait un POP ECX, POP EDI puis
							; DEC ECX: ca paire!!!

			mov	ecx,eax 		; matiere poly + nb point poly
			and	eax,0FFh		; (+ coul1/coul2)

			add	esi,4
			jmp	[@@Table_B4_TriZ+eax*4]

			ALIGN	4
@@Table_B4_TriZ		Label	DWORD
						; *** Triste ***
			dd	Poly_Triste		; 0 -> 6
			dd	Poly_Triste
			dd	Poly_Triste
			dd	Poly_Triste
			dd	Poly_Triste
			dd	Poly_Triste
			dd	Poly_Triste

						; *** Flat ***
			dd	PolyFlat		; 7 -> 8
			dd	PolyFlat

						; *** Gouraud ***
			dd	PolyGouraud		; 9 -> 10
			dd	PolyGouraud		; (+dithering)

						; *** Texture sans incruste ***
			dd	PolyTexture		; 11
			dd	PolyTexFlat		; 12

			dd	PolyTexGouraud		; 13 -> 14
			dd	PolyTexGouraud		; (+dithering)

						; *** Texture avec incruste ***
			dd	PolyTexture		; 15
			dd	PolyTexFlat		; 16

			dd	PolyTexGouraud		; 17-18
			dd	PolyTexGouraud		; (+dithering)

						; *** Texture perspective corrected ***
						; *** sans incruste ***
			dd	PolyTextureZ		; 19 (Texture Z)
			dd	PolyFlatTextureZ	; 20 (Texture Z)
			dd	PolyTexGouraud		; 21 (Texture Z)
			dd	PolyTexGouraud		; 22 (Texture Z)

						; *** avec incruste ***
			dd	PolyTextureZ		; 23 (Texture Z)
			dd	PolyFlatTextureZ	; 24 (Texture Z)
			dd	PolyTexGouraud		; 25 (Texture Z)
			dd	PolyTexGouraud		; 26 (Texture Z)

						; *** Gouraud avec lecture en table de conversion ***
			dd	PolyGouraudTable	; 27->28 (lecture en table pour le bon passage entre mapping et gouraud)
			dd	PolyGouraudTable	; (+dithering)

						; *** Environment mapping ***
						; *** sans incruste ***
			dd	PolyTexture_Env		; 29
			dd	PolyTexFlat_Env		; 30

			dd	PolyTexGouraud_Env	; 31
			dd	PolyTexGouraud_Env	; 32 (+dithering)

						; *** avec incruste ***
			dd	PolyTexture_Env		; 33
			dd	PolyTexFlat_Env		; 34

			dd	PolyTexGouraud_Env	; 35
			dd	PolyTexGouraud_Env	; 36 (+dithering)


			ALIGN	4
Poly_Triste:

					; *****************************
					; *** MAT_TRISTE->MAT_TRAME ***
					; *****************************

			mov	[edi],ecx		; stock type + nbp
							; stock coul1/coul2
			add	edi,4
			mov	ebp,80000000h		; ZMax
			mov	[ptr1],edi		; memo start poly

							; ** Copie les points dans la table
							; des polygones a afficher
							; (Heu: il reste le test de visibilite a faire :)
			ALIGN	4
p0:			movzx	eax,word ptr[esi]	; index point sur List_Point déjà *SIZE_LIST_POINT

			add	esi,2			; Vaut mieux eviter l'AGI plutot
							; que pairer...
			mov	edx,[List_Point+eax]
			mov	eax,[List_Point+eax+4-2]; Lit le Z du point dans le poids
							; fort de EAX; on se fout du LSB

			mov	dword ptr [edi].XE_Face,edx
			add	edi,Size Struc_Entity

			cmp	eax,ebp			; A-t-on un ZMax?
			jl	nozmax
			mov	ebp,eax
nozmax:
			dec	ch
			jne	p0

			shr	ebp,16
			cmp	ebp,32767		; C'est un code d'erreur
			je	badpoly

			jmp	testpoly


					; ****************
					; *** MAT_FLAT ***
					; ****************
			ALIGN	4
polyflat:
			movzx	eax, word ptr[esi]	; Lit index de la normale
			sub	cl,7			; pour obtenir l'intensite
			add	esi,2			; evite l'AGI
			mov	ebp,[ List_Normal + eax*2 ]

			shl	ebp,16
			add	ebp,ecx

			mov	[edi],ebp		; stock coul + intensity
			add	edi,4



			mov	ebp,80000000h		; ZMax
			mov	[ptr1],edi		; memo start poly somm
			ALIGN	4
p0f:			movzx	eax,word ptr[esi]	; index point sur List_Point
			add	esi,2
			mov	edx,[eax+List_Point]	; index déjà *SIZE_LIST_POINT
			mov	eax,[eax+List_Point+4-2]

			mov	dword ptr [edi].XE_Face,edx
			add	edi,Size Struc_Entity

			cmp	eax,ebp			; Recherche du ZMax
			jl	nozmaxf
			mov	ebp,eax
nozmaxf:
			dec	ch
			jne	p0f

			shr	ebp,16
			cmp	ebp,32767		; code d'erreur
			je	badpoly

			jmp	testpoly



					; *******************
					; *** MAT_GOURAUD ***
					; *******************
			ALIGN	4
polygouraud:
			sub	cl,2			; attention voir reanim
							; ... merci pour l'indication :=]
			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			add	edi,4

			shr	ecx,8			; cl = nbp, ch = coul1
			xor	edx,edx
			mov	[ptr1],edi		; memo start poly somm

			ALIGN	4
p0g:
			mov	ebx,[esi]		; Index [Point][Normal]
			add	esi,4
			mov	eax,ebx
			shr	ebx,16
			and	eax,0FFFFh
			mov	dl,byte ptr[List_Normal+eax*2]
			mov	eax,[ebx+List_Point]
			mov	ebx,[ebx+List_Point+4-2]
			mov	[edi].Coul_Face,dx	 	; stock intensity point

			mov	dword ptr [edi].XE_Face,eax	; Xscr & Yscr dans List_Coor
			add	edi,Size Struc_Entity


			cmp	ebx,ebp			; Recherche du ZMax
			jl	nozmaxg
			mov	ebp,ebx
nozmaxg:
			dec	cl
			jne	p0g

			shr	ebp,16
			cmp	ebp,32767		; C'est tjrs un code d'erreur :)
			je	badpoly

			jmp	testpoly



					; *************************
					; *** MAT_GOURAUD_TABLE ***
					; *************************
			ALIGN	4
polygouraudTable:
			sub	cl,2			; attention voir reanim
							; ... merci pour l'indication :=]
			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			add	edi,4

			mov	ebx,ecx			; EBX : coul2.coul1.nbp.type
			shr	ecx,8			; cl = nbp, ch = coul1
			xor	edx,edx
			shr	ebx,16+4		; coul2 = Ligne start.Range
			mov	[ptr1],edi		; memo start poly somm
			mov	[Line_In_Deg],bh

			ALIGN	4
p0gtable:
			mov	ebx,[esi]		; Index [Point][Normal]
			add	esi,4
			mov	eax,ebx
			shr	ebx,16
			and	eax,0FFFFh
			mov	dl,byte ptr[List_Normal+eax*2]
			mov	eax,[ebx+List_Point]
			add	dl,[Line_In_Deg]
			mov	ebx,[ebx+List_Point+4-2]
			mov	[edi].Coul_Face,dx	 	; stock intensity point

			mov	dword ptr [edi].XE_Face,eax	; Xscr & Yscr dans List_Coor
			add	edi,Size Struc_Entity


			cmp	ebx,ebp			; Recherche du ZMax
			jl	nozmaxgtable
			mov	ebp,ebx
nozmaxgtable:
			dec	cl
			jne	p0gtable

			shr	ebp,16
			cmp	ebp,32767		; C'est tjrs un code d'erreur :)
			je	badpoly

			jmp	testpoly



					; ************************
					; *** MAT_TEXTURE_FLAT ***
					; ************************

			ALIGN	4
PolyTexFlat:
			movzx	eax,word ptr[esi]	; calcule l'intensite de la face
			add	esi,2
			mov	ebp,[List_Normal + eax*2 - 2]
			and	ebp, 0FFFF0000h
			add	ecx,ebp			; change coul + intensity



					; *******************
					; *** MAT_TEXTURE ***
					; *******************
			ALIGN	4
PolyTexture:
			sub	cl,2

			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			mov	bl,ch			; bl = nbp
			mov	cl,ch			; cl = nbp

			add	edi,4
			and	ecx,0FFh

			mov	[ptr1],edi		; memo start poly somm
			shl	ecx,2			; Deplacement pour pointer sur
							; les infos de texture
			ALIGN	4
p0t:
			movzx	eax,word ptr[esi+2]	; index point sur List_Point
			add	esi,4
			mov	edx,[eax+List_Point]
			mov	eax,[eax+List_Point+4-2]
			mov	dword ptr [edi].XE_Face,edx
			mov	edx,[esi+ecx]		; en fait [esi+ecx+4]
			mov	dword ptr [edi].U_Face,edx
			add	edi,Size Struc_Entity

			cmp	eax,ebp			; ZMin
			jl	nozmaxt
			mov	ebp,eax
nozmaxt:
			dec	bl
			jne	p0t


			mov	eax,[esi]		; Copie l'offset dans la feuille
			shr	ebp,16
	       		mov	[edi],eax		; de la texture et le RepMask
			add	edi,4

							; WORD relative org texture
							; WORD repmask X Y

							; WORD coor X0 (256.256)
							; WORD coor Y0 (256.256)

							; WORD coor X1 (256.256)
							; WORD coor Y1 (256.256)

							; WORD coor X2 (256.256)
							; WORD coor Y2 (256.256)

							; WORD coor X3 (256.256)
							; WORD coor Y3 (256.256)
			lea	esi,[esi+ecx+4]

			cmp	ebp,32767		; Code d'erreur
			je	badpoly

			jmp	testpoly



					; ***************************
					; *** MAT_TEXTURE_GOURAUD ***
					; ***************************
			ALIGN	4
polytexgouraud:
			sub	cl,2
			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			mov	bl,ch			; bl = nbp
			mov	cl,ch			; cl = nbp
			add	edi,4
			and	ecx,0FFh
			mov	[ptr1],edi		; memo start poly somm

			shl	ecx,2

			ALIGN	4
p0tg:
			push	ebx
			mov	ebx,[esi]		; normal point
			xor	edx,edx
			mov	eax,ebx
			shr	ebx,16
			and	eax,0FFFFh
			mov	dl,byte ptr [List_Normal+eax*2]
			mov	[edi].Coul_Face,dx	; stock intensity point

			add	esi,4
			mov	edx,[ebx+List_Point]
			mov	eax,[ebx+List_Point+4-2]
			mov	dword ptr [edi].XE_Face,edx
			mov	edx,[esi+ecx]
			mov	dword ptr [edi].U_Face,edx
			add	edi,Size Struc_Entity

			cmp	eax,ebp			; recherche du ZMax
			jl	nozmaxtg
			mov	ebp,eax
nozmaxtg:
			pop	ebx
			dec	bl
			jne	p0tg

			mov	eax,[esi]
			shr	ebp,16
			mov	[edi],eax
			add	edi,4


						; WORD relative org texture
						; WORD repmask X Y

						; WORD coor X0 (256.256)
						; WORD coor Y0 (256.256)

						; WORD coor X1 (256.256)
						; WORD coor Y1 (256.256)

						; WORD coor X2 (256.256)
						; WORD coor Y2 (256.256)

						; WORD coor X3 (256.256)
						; WORD coor Y3 (256.256)

			lea	esi,[esi+ecx+4]
			cmp	ebp,32767		; C'est tjrs et encore un code
			je	badpoly			; d'erreur :)

			jmp	testpoly




					; *************************
					; *** MAT_FLAT_TEXTUREZ ***
					; *************************

			ALIGN	4
PolyFlatTextureZ:
			movzx	eax, word ptr[esi]	; calcule l'intensite de la face
			add	esi,2
			mov	ebp,[List_Normal + eax*2 - 2]
			and	ebp, 0FFFF0000h
			add	ecx,ebp			; change coul + intensity


					; ********************
					; *** MAT_TEXTUREZ ***
					; ********************
			ALIGN	4
PolyTextureZ:
			sub	cl,2

			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			mov	bl,ch			; bl = nbp
			mov	cl,ch			; cl = nbp

			add	edi,4
			and	ecx,0FFh

			mov	[ptr1],edi		; memo start poly somm
			shl	ecx,2			; Deplacement pour pointer sur
							; les infos de texture

			ALIGN	4
p0tz:
			push	ebx
			movzx	eax,word ptr[esi+2]	; index point sur List_Point
			add	esi,4
			mov	edx,[eax+List_Point]
			mov	ebx,[eax+List_Point+4-2]	; Lire le Z dans le poids fort
			mov	dword ptr [edi].XE_Face,edx
			mov	edx,[esi+ecx]		; en fait [esi+ecx+4]
			mov	dword ptr [edi].U_Face,edx

			push	ebx
			sar	ebx,16
			sub	ebx,[PosZWr]		; Calcul de 1/Z, sur nn bits
			add	ebx,[KFactor]

			mov	eax,1 SHL 31
			or	ebx,1			; Pas de DIV par 0
			xor	edx,edx
			idiv	ebx

			pop	ebx
			mov	[edi].ZO_Face,eax

			add	edi,Size Struc_Entity

			cmp	ebx,ebp			; ZMin
			jl	nozmaxtz
			mov	ebp,ebx
nozmaxtz:
			pop	ebx
			dec	bl
			jne	p0tz


			mov	eax,[esi]		; Copie l'offset dans la feuille
			shr	ebp,16
	       		mov	[edi],eax		; de la texture et le RepMask
			add	edi,4

			lea	esi,[esi+ecx+4]

			cmp	ebp,32767		; Code d'erreur
			je	badpoly

			jmp	testpoly



					; ****************************
					; *** MAT_ENV_MAPPING_FLAT ***
					; ****************************
			ALIGN	4
PolyTexFlat_Env:
			movzx	eax,word ptr[esi]	; calcule l'intensite de la face
			add	esi,2
			mov	ebp,[List_Normal + eax*2 -2]
			and	ebp, 0FFFF0000h
			add	ecx,ebp			; change coul + intensity



					; ***********************
					; *** MAT_ENV_MAPPING ***
					; ***********************
			ALIGN	4
PolyTexture_Env:
			sub	cl,2 +(29-11)

			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			mov	bl,ch			; bl = nbp
			mov	cl,ch			; cl = nbp

			add	edi,4
			and	ecx,0FFh

			mov	[ptr1],edi		; memo start poly somm
			shl	ecx,2			; Deplacement pour pointer sur
							; les infos de texture
			ALIGN	4
p0te:
			movzx	eax,word ptr[esi+2]	; index point sur List_Point
			add	esi,4
			mov	edx,[eax+List_Point]
			mov	dword ptr [edi].XE_Face,edx
							; En [ESI+ECX] se trouve (U,V)

			push	eax
			push	ecx
			push	ebx
			push	ebp
			mov	ebx,eax			; Calcule la norme du vecteur entre le point courant et l'origine du repere
							; local
			mov	ebp,[esi+ecx]		; Lit l'abscisse de l'origine du repere local
			movsx 	edx,word ptr [List_Anim_Point+eax]
			and	ebp,0FF000000h
			sar	ebp,16
			add	edx,ebp
			imul	edx,edx

			mov	ebp,[esi+ecx]		; Lit l'ordonnee de l'origine du repere local
			movsx	eax,word ptr [List_Anim_Point+ebx+2]
			shl	ebp,8
			and	ebp,0FF000000h
			sar	ebp,16
			add	eax,ebp
			imul	eax,eax

			add	edx,eax
			movsx	eax,word ptr [List_Anim_Point+ebx+4]
			imul	eax,eax
			add	eax,edx
			push	ecx
			call	Sqr_A
			pop	ecx
			mov	ebp,eax
							; Maintenant que l'on a la norme, on peut calculer les coordonnees
							; de mapping
			mov	eax,[esi+ecx]
			shl	eax,8
			and	eax,0FF000000h
			add	eax,[List_Anim_Point+ebx+2-2]	; Lit Y dans le poids fort
			sar	eax,16
			neg	eax
			push	ecx
			mov	edx,eax
			mov	cl,[esi+ecx]
			add	cl,8
			shl	eax,cl
			neg	cl
			add	cl,32
			sar	edx,cl
			idiv	ebp
			pop	ecx
			add	eax,128*256
			shl	eax,16
			push	eax

			mov	eax,[esi+ecx]
			and	eax,0FF000000h
			add	eax,[List_Anim_Point+ebx+0-2]	; Lit X dans le poids fort
			sar	eax,16
			push	ecx
			mov	edx,eax
			mov	cl,[esi+ecx]
			add	cl,8
			shl	eax,cl
			neg	cl
			add	cl,32
			sar	edx,cl
			idiv	ebp
			pop	ecx
			add	eax,128*256

			pop	edx
			add	edx,eax
			pop	ebp
			pop	ebx
			pop	ecx
			pop	eax

			mov	dword ptr [edi].U_Face,edx
			mov	eax,[eax+List_Point+4-2]
			add	edi,Size Struc_Entity

			cmp	eax,ebp			; ZMin
			jl	nozmaxte
			mov	ebp,eax
nozmaxte:
			dec	bl
			jne	p0te


			mov	eax,[esi]		; Copie l'offset dans la feuille
			shr	ebp,16
	       		mov	[edi],eax		; de la texture et le RepMask
			add	edi,4

							; WORD relative org texture
							; WORD repmask X Y

							; WORD coor X0 (256.256)
							; WORD coor Y0 (256.256)

							; WORD coor X1 (256.256)
							; WORD coor Y1 (256.256)

							; WORD coor X2 (256.256)
							; WORD coor Y2 (256.256)

							; WORD coor X3 (256.256)
							; WORD coor Y3 (256.256)
			lea	esi,[esi+ecx+4]

			cmp	ebp,32767		; Code d'erreur
			je	badpoly

			jmp	testpoly


					; *******************************
					; *** MAT_ENV_MAPPING_GOURAUD ***
					; *******************************
			ALIGN	4
polytexgouraud_Env:
			sub	cl,2 + (29-11)
			mov	ebp,80000000h		; ZMax
			mov	[edi],ecx		; stock type translated + nbp
							; + stock coul1/coul2
			mov	bl,ch			; bl = nbp
			mov	cl,ch			; cl = nbp
			add	edi,4
			and	ecx,0FFh
			mov	[ptr1],edi		; memo start poly somm

			shl	ecx,2

			ALIGN	4
p0tge:
			push	ebx
			mov	ebx,[esi]		; normal point
			xor	edx,edx
			mov	eax,ebx
			shr	ebx,16
			and	eax,0FFFFh
			mov	dl,byte ptr [List_Normal+eax*2]
			mov	[edi].Coul_Face,dx	; stock intensity point

			add	esi,4
			mov	edx,[ebx+List_Point]
			mov	dword ptr [edi].XE_Face,edx
							; En [ESI+ECX] se trouve (U,V)

			push	eax
			push	ecx
			push	ebp			; Calcule la norme du vecteur entre le point courant et l'origine du repere
							; local
			mov	ebp,[esi+ecx]		; Lit l'abscisse de l'origine du repere local
			movsx 	edx,word ptr [List_Anim_Point+ebx]
			and	ebp,0FF000000h
			sar	ebp,16
			add	edx,ebp
			imul	edx,edx

			mov	ebp,[esi+ecx]		; Lit l'ordonnee de l'origine du repere local
			movsx	eax,word ptr [List_Anim_Point+ebx+2]
			shl	ebp,8
			and	ebp,0FF000000h
			sar	ebp,16
			add	eax,ebp
			imul	eax,eax

			add	edx,eax
			movsx	eax,word ptr [List_Anim_Point+ebx+4]
			imul	eax,eax
			add	eax,edx
			push	ecx
			call	Sqr_A
			pop	ecx
			mov	ebp,eax
							; Maintenant que l'on a la norme, on peut calculer les coordonnees
							; de mapping
			mov	eax,[esi+ecx]
			shl	eax,8
			and	eax,0FF000000h
			add	eax,[List_Anim_Point+ebx+2-2]	; Lit Y dans le poids fort
			sar	eax,16
			neg	eax
			push	ecx
			mov	edx,eax
			mov	cl,[esi+ecx]
			add	cl,8
			shl	eax,cl
			neg	cl
			add	cl,32
			sar	edx,cl
			idiv	ebp
			pop	ecx
			add	eax,128*256
			shl	eax,16
			push	eax

			mov	eax,[esi+ecx]
			and	eax,0FF000000h
			add	eax,[List_Anim_Point+ebx+0-2]	; Lit X dans le poids fort
			sar	eax,16
			push	ecx
			mov	edx,eax
			mov	cl,[esi+ecx]
			add	cl,8
			shl	eax,cl
			neg	cl
			add	cl,32
			sar	edx,cl
			idiv	ebp
			pop	ecx
			add	eax,128*256

			pop	edx
			add	edx,eax
			pop	ebp
			pop	ecx
			pop	eax

			mov	dword ptr [edi].U_Face,edx
			mov	eax,[ebx+List_Point+4-2]

			add	edi,Size Struc_Entity

			cmp	eax,ebp			; recherche du ZMax
			jl	nozmaxtge
			mov	ebp,eax
nozmaxtge:
			pop	ebx
			dec	bl
			jne	p0tge

			mov	eax,[esi]
			shr	ebp,16
			mov	[edi],eax
			add	edi,4


						; WORD relative org texture
						; WORD repmask X Y

						; WORD coor X0 (256.256)
						; WORD coor Y0 (256.256)

						; WORD coor X1 (256.256)
						; WORD coor Y1 (256.256)

						; WORD coor X2 (256.256)
						; WORD coor Y2 (256.256)

						; WORD coor X3 (256.256)
						; WORD coor Y3 (256.256)

			lea	esi,[esi+ecx+4]
			cmp	ebp,32767		; C'est tjrs et encore un code
			je	badpoly			; d'erreur :)

			jmp	testpoly

;*--------------------------------------------------------------------------*/

			ALIGN	4
badpoly:
			pop	ecx
			pop	edi
			dec	ecx
			jne	b_poly
			jmp	lignes

;-----------------------------------------------------------------------------
			ALIGN	4
testpoly:
			mov	[NextDI],edi		; memo pointeur List_Coor
			mov	[ZMax],ebp

			mov	edi,[ptr1]		; pointeur list sommets

			mov	eax,dword ptr [edi-2].YE_Face
			sar	eax,16

			mov	edx,dword ptr [edi+2*(Size Struc_Entity)-2].YE_Face
			sar	edx,16

			sub	edx,eax

			mov	ebx,dword ptr [edi+Size Struc_Entity-2].XE_Face
			sar	ebx,16

			mov	ebp,dword ptr [edi-2].XE_Face
			sar	ebp,16

			sub	ebx,ebp
			imul	ebx,edx


			mov	ecx,dword ptr [edi+Size Struc_Entity-2].YE_Face
			sar	ecx,16

			mov	edx,dword ptr [edi+2*(Size Struc_Entity)-2].XE_Face
			sar	edx,16

			sub	ecx,eax
			sub	ebp,edx
			imul	ebp,ecx

			add	ebp,ebx
			jnl	badpoly


			mov	ebx,[TotalEntite]
			mov	edi,[PointeurListTri]
			mov	eax,[ZMax]
			inc	ebx
			add	edi,8
			mov	[TotalEntite],ebx
			or	eax,E_POLY * 10000h
			pop	ecx
			pop	ebx

			mov	[edi-8],eax
			mov	[edi+4-8],ebx
			mov	[PointeurListTri],edi

			dec	ecx
			mov	edi,[NextDI]
			jne	b_poly


;*══════════════════════════════════════════════════════════════════════════*
;                     █      █    █▀▀▀▀ ██▄ █ █▀▀▀▀ ██▀▀▀
;                     ██     ██   ██ ▀█ ██▀██ ██▀▀  ▀▀▀▀█
;                     ▀▀▀▀▀  ▀▀   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

			ALIGN	4
lignes:
			movzx	ecx, word ptr[esi]	; nb lignes
			add	esi, 2
			test	ecx, ecx
			je	spheres

			add	TotalEntite,ecx

			mov	edx,[PointeurListTri]
			jmp	b_line
			ALIGN	4
lclipz:
			mov	eax,TotalEntite
			dec	eax
			dec	ecx
			mov	TotalEntite,eax
			jz	lexit
			ALIGN	4
b_line:
			mov	eax,[esi]		; stock matiere/coul
			movzx	ebx,word ptr[esi+4]	; index point 1 sur List_Point
			mov	[edi], eax		; coul2 / even
			mov	eax,[ebx+List_Point+0]
			mov	ebp,[ebx+List_Point+4-2]
			movzx	ebx,word ptr[esi+6]	; index point 2 sur List_Point
			mov	[edi+4],eax		; X1scr vers List_Coor
							; Y1scr
			mov	eax,[ebx+List_Point+0]
			mov	ebx,[ebx+List_Point+4-2]; ZMin ZMax
			mov	[edi+8], eax		; X2scr vers List_Coor
							; Y2scr
			cmp	ebx,ebp
			jl	zok0
			mov	ebp,ebx
zok0:
			shr	ebp,16

			add	esi,8

			cmp	ebp,32767
			je	lclipz

			or	ebp,E_LIGNE * 10000h
			mov	[edx+4],edi
			mov	[edx],ebp

			add	edx,8
			add	edi,12

			dec	ecx
			jnz	b_line
lexit:
			mov	[PointeurListTri],edx


;*══════════════════════════════════════════════════════════════════════════*
;                  ██▀▀▀ █▀▀▀█ █   █ █▀▀▀▀ █▀▀▀█ █▀▀▀▀ ██▀▀▀
;                  ▀▀▀▀█ ██▀▀▀ ██▀▀█ ██▀▀  ██▀█▀ ██▀▀  ▀▀▀▀█
;                  ▀▀▀▀▀ ▀▀    ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

			ALIGN	4
spheres:		movzx	ecx,word ptr[esi]	; nb spheres
			add	esi,2
			mov	eax,TotalEntite
			test	ecx, ecx
			jz	tri
			add	eax,ecx
			mov	edx,[PointeurListTri]
			mov	TotalEntite,eax
			jmp	b_sphere
			ALIGN	4
sclipz:
			mov	eax, TotalEntite
			dec	eax
			dec	ecx
			mov	TotalEntite, eax
			jz	sexit
			ALIGN	4
b_sphere:
			mov	ebx, [esi+4]		; rayon & index point 1 sur List_Point
			mov	eax, [esi]		; stock matiere/coul
			mov	[edi+4], ebx		; rayon
			shr	ebx, 16			; index point 1 sur List_Point
			mov	[edi], eax		; coul2 / even
			mov	eax, [ebx+List_Point]	; X1scr, Y1scr
			movzx	ebx, word ptr[ebx+List_Point+4]; Zrot ZMin/Max
			mov	[edi+6], eax
			add	esi, 8
			cmp	ebx, 32767
			je	sclipz
			mov	[edi+10], ebx

			or	ebx, E_SPHERE * 10000h
			mov	[edx+4], edi
			mov	[edx], ebx
			add	edi, 12
			add	edx, 8

			dec	ecx
			jnz	b_sphere
sexit:
			mov	[PointeurListTri], edx

;*══════════════════════════════════════════════════════════════════════════*
;         ▀▀█▀▀ █▀▀▀█  █          █▀▀▀▀ ██▄ █ ▀▀█▀▀  █    ▀▀█▀▀ █  ▄▀
;           ██  ██▀█▀  ██         ██▀▀  ██▀██   ██   ██     ██  ██▀
;           ▀▀  ▀▀  ▀  ▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀   ▀▀   ▀▀     ▀▀  ▀▀
;*══════════════════════════════════════════════════════════════════════════*
; tri les faces dans la table Listtri (ordre Z et offset sur list coors
; premachees.

			ALIGN	4
tri:
			mov	ecx,TotalEntite
			mov	esi,[Object_Flags]
			dec	ecx
			jl	badfinafobj
			jz	fintri

			test	esi,INFO_NON_TRI
			jne	FinTri
			mov	esi, offset List_Tri

			lea	ebp, [esi+ecx*8]

			; esi	bas
			; ebp	haut
			; cx	nbr d'elements

			; ax	pivot
			; edi	pospivot
			; ebx	h

			push	0			; flag de fin de pile !

			jmp	nocalccx

			ALIGN	4
plusgrand:    		mov	ebx, edi
			mov	eax, [edi]
			jmp	cont

			ALIGN	4
permut:			; cx nb entite - 1
			; esi start list sizeof 8 byte

			mov	ebp, ecx
tt0:
			mov	ecx, ebp

			mov	eax, [esi]		; lit Z
			lea	edi, [esi+8]
			xor	ebx, ebx

tt1:			cmp	[edi], ax
			jg	plusgrand
cont:			add	edi, 8
			dec	ecx
			jne	tt1

			test	ebx, ebx
			je	noexchg

			mov	ecx, [esi]		; permutte
			mov	[esi], eax
			mov	[ebx], ecx
			mov	eax, [esi+04]
			mov	ecx, [ebx+04]
			mov	[ebx+04], eax
			mov	[esi+04], ecx
noexchg:
			add	esi, 8
			dec	ebp
			jne	tt0

			pop	ebp
			test	ebp, ebp
			jnz	dopop
			jmp	fintri

			ALIGN	4
permut2:		mov	eax, [esi]
			mov	ebx, [ebp]

			cmp	ax, bx
			jge	skipswap

			mov	[esi], ebx
			mov	[ebp], eax
			mov	eax, [esi+04]
			mov	ebx, [ebp+04]
			mov	[ebp+04], eax
			mov	[esi+04], ebx
skipswap:
			pop	ebp
			test	ebp, ebp
			jz	fintri

dopop:			pop	esi
nopop:
			mov	ecx, ebp
			sub	ecx, esi
			shr	ecx, 3
nocalccx:
			cmp	ecx, 1			; select permut2 ?
			je	permut2

			cmp	ecx, 7			; select permut ou qsort
			jbe	permut

			mov	edi, esi		; pospivot = bas
			mov	eax, [esi]		; pivot = [pospivot] = [bas]
			mov	ebx, ebp		; h = haut
			add	esi, 8			; bas++

			jmp	w1
			ALIGN	4
w3:
		     	mov	edx, [esi]		; xchg [bas],[h]
			xchg	edx, [ebx]
			mov	[esi], edx
			mov	edx, [esi+4]
			xchg	edx, [ebx+4]
			mov	[esi+4], edx
			ALIGN	4
w1:
			cmp	[esi], ax
			jl	w2
			add	esi, 8			; si [bas] >= pivot
			dec	ecx
			jnz	w1

			jmp	w4
			ALIGN	4
w2:
			cmp	[ebx], ax
			jg	w3
			sub	ebx, 8			; si [h] <= pivot
			dec	ecx
			jnz	w2

w4:
			sub	esi, 8
			add	ebx, 8

			mov	ecx, [esi]
			mov	[esi], eax		; xchg [pospivot],[bas]
			mov	[edi], ecx
			mov	edx, [edi+4]
			mov	ecx, [esi+4]
			mov	[esi+4], edx
			mov	[edi+4], ecx

			cmp	ebx, ebp
			jae	nopush

			push	ebx			; h
			push	ebp			; haut
nopush:
			sub	esi, 8
			cmp	edi, esi
			jae	norecur

			mov	ebp, esi
			mov	esi, edi
			jmp	nopop
norecur:
			pop	ebp
			test	ebp, ebp
			jnz	dopop
fintri:

;*══════════════════════════════════════════════════════════════════════════*
;                 █▀▀▀▄  █    ██▀▀▀ █▀▀▀█ █     █▀▀▀█ █  ▄▀
;                 ██  █  ██   ▀▀▀▀█ ██▀▀▀ ██    ██▀▀█ ██▀
;                 ▀▀▀▀   ▀▀   ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀
;*══════════════════════════════════════════════════════════════════════════*
; affichage des entités

			mov	esi,offset List_Tri
			mov	ecx,[TotalEntite]
			mov	[ptr1],esi
ade0:
			mov	ebx,[esi]
			push	ecx
			shr	ebx,16			; type entité
			push	esi
			mov	esi,[esi+4]		; offset List_Coor

			jmp	[TabJump+ebx*4]		; jmp à aff de l'entité
nextaff:
			pop	esi
			pop	ecx
			add	esi,8
			dec	ecx
			jne	ade0

goodfinobj:
			xor	eax,eax			; OK affiché au moins 1 entité
			ret

;*--------------------------------------------------------------------------*/

badfinafobj::					; 1 si pas d'entite to aff
			mov	eax,7FFFFFFFh
			mov	[ScreenXmax],eax
			mov	[ScreenYmax],eax
			mov	[ScreenXmin],eax
			mov	[ScreenYmin],eax
			mov	eax,1
			ret

;*══════════════════════════════════════════════════════════════════════════*
;		      █▀▀▀▀ ██▄ █ ▀▀█▀▀  █    ▀▀█▀▀ █  ▄▀
;		      ██▀▀  ██▀██   ██   ██     ██  ██▀
;		      ▀▀▀▀▀ ▀▀  ▀   ▀▀   ▀▀     ▀▀  ▀▀
;
;		   █▀▀▀▄  █    ██▀▀▀ █▀▀▀█ █     █▀▀▀█ █  ▄▀
;		   ██  █  ██   ▀▀▀▀█ ██▀▀▀ ██    ██▀▀█ ██▀
;		   ▀▀▀▀   ▀▀   ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

;*──────────────────────────────────────────────────────────────────────────*
			ALIGN	4
aff_E_POLY::
			mov	eax,[esi]		; Lit [Coul1/Coul2][Nb Pts/Type]

			add	esi,4
			mov	ebx,eax
			xor	ecx,ecx
			shr	ebx,16			; EBX = Coul1/Coul2
			mov	cl,ah			; ECX = Nb Pts dans le poly


			cmp	al,9			; Si texture, alors on recupere
			jb	@@Not_A_Texture		; l'offset et le RepMask

			mov	edx,3*(Size Struc_Entity)
			cmp	cl,4
			jne	@@Not_4_Sides
			mov	edx,4*(Size Struc_Entity)
@@Not_4_Sides:
			mov	edx,[esi+edx]
			mov	edi,edx
			and	edx,0FFFFh
			shr	edi,16
			add	edx,[ObjPtrMap]

			mov	[RepMask],edi
			mov	[PtrMap],edx
@@Not_A_Texture:
			and	eax,0FFh

			push	Offset nextaff		; Equivalent call/jmp, mais plus
			jmp	Fill_Poly		; rapide!!! :)

;*──────────────────────────────────────────────────────────────────────────*
			ALIGN	4
aff_E_LIGNE::
			movzx	edx,word ptr[esi+00]
			mov	eax,[esi+04-2]
			mov	ebx,[esi+06-2]
			xchg	dl,dh
			sar	eax,16			; x0
			mov	ebp,edx
			sar	ebx,16			; y0
			mov	ecx,[esi+08-2]
			mov	edx,[esi+10-2]
			sar	ecx,16
			sar	edx,16			; y1

			call	Line_A
			jmp	nextaff

;*──────────────────────────────────────────────────────────────────────────*
			ALIGN	4
aff_E_SPHERE::

			mov	cl,[esi]		; type
			movzx	ebp,word ptr[esi+1]	; coul1/coul2
			and	ecx,0FFh
			movzx	eax,word ptr[esi+4]	; rayon
			mov	[save1], ecx
			mov	ecx,[esi+6-2]
			sar	ecx,16			; x
			mov	edi,[esi+8-2]
			mov	[save2], ebp
			sar	edi,16			; y

			cmp	[TypeProj],TYPE_3D
			jnz	isosph

			mov	ebp,[esi+10-2]
			imul	[LFactorX]
			sar	ebp,16			; Z
			sub	ebp,PosZwr
			add	ebp,KFactor
			idiv	ebp

			mov	ebp,eax
			jmp	affsph
isosph:
			mov	ebp,eax			; rayon

			shl	eax,5			; * 32
			add	ebp,ebp			; * 2
			add	ebp,eax	 		; * 34, racine sx²+sy²
			sar	ebp,9			; /512 IsoScale

			; ecx x
			; edi y -> esi pour computesphere
			; ebp rayon sur ecran

			; reajuste coordonnée box

affsph:
			mov	eax,ecx			; x + rayon
			add	eax,ebp
			cmp	eax,ScreenXmax
			jle	nosxa
			mov	ScreenXmax,eax
nosxa:			mov	eax,ecx       		; x - rayon
			sub	eax,ebp
			cmp	eax,ScreenXmin
			jge	nosxi
			mov	ScreenXmin,eax
nosxi:			mov	eax,edi
			add	eax,ebp			; y + rayon
			cmp	eax,ScreenYmax
			jle	nosya
			mov	ScreenYmax,eax
nosya:			mov	eax,edi       		; y - rayon
			sub	eax,ebp
			cmp	eax,ScreenYmin
			jge	nosyi
			mov	ScreenYmin,eax
nosyi:
			mov	eax,ecx			; C_X
			mov	ebx,edi			; C_Y
			mov	ecx,ebp			; rayon

			mov	edx,[save2]		; coul
			mov	esi,[save1]		; type

			call	Fill_Sphere
no_sphere:		jmp	nextaff

ObjectDisplay		ENDP






; --------------------------------------------------------------------------------
; Teste la visibilite pour les anciens types de faces (Light, X, Y)


TestVuePolyOld		PROC	USES edi ebx ebp,\
			ptrpoly:DWORD

			mov	edi, ptrpoly

			mov	bx,word ptr[edi+8]
			sub	bx,word ptr[edi+2]

			mov	ax,word ptr[edi+4]
			sub	ax,word ptr[edi+16]

			imul	bx

			mov	bp,ax
			mov	bx,dx

			mov	cx,word ptr[edi+10]
			sub	cx,word ptr[edi+4]

			mov	ax,word ptr[edi+2]
			sub	ax,word ptr[edi+14]

			imul	cx

			sub	ax,bp			; oublie face si bx:bp < dx:ax
			sbb	dx,bx
			jnl	nopoly

			mov	eax,1
			ret

nopoly:			xor	eax,eax
			ret
TestVuePolyOld		ENDP






TestVuePoly		PROC	USES edi ebx ebp,\
			ptrpoly:DWORD

			mov	edi,ptrpoly

			mov	bx,[edi+Size Struc_Entity].XE_Face
			sub	bx,[edi].XE_Face

			mov	ax,[edi].YE_Face
			sub	ax,[edi+2*(Size Struc_Entity)].YE_Face

			imul	bx

			mov	ebp,eax
			mov	ebx,edx

			mov	cx,[edi+Size Struc_Entity].YE_Face
			sub	cx,[edi].YE_Face

			mov	ax,[edi].XE_Face
			sub	ax,[edi+2*(Size Struc_Entity)].XE_Face

			imul	cx

			sub	ax,bp			; oublie face si bx:bp < dx:ax
			sbb	dx,bx
			jnl	nopoly

			mov	eax,1
			ret

nopoly:			xor	eax,eax
			ret
TestVuePoly		ENDP



;Seg_Code		ENDS



;			The
			End
