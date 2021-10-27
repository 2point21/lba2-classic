; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ                                                                            บ
; บ  AFFICHAGE D'OBJETS 3D                                                     บ
; บ                                                                            บ
; บ                                                                            บ
; บ                                                                            บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ  Version: Oct 1996                                                         บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ                                                                            บ
; บ  Les EQUATES, MACROS et STRUCTURES                                         บ
; บ                                                                            บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ



				; *** Limitations du format ***
MAX_NB_POLYS		EQU	550
MAX_NB_POINTS		EQU	550
MAX_NB_SPHERES		EQU	50
MAX_NB_LIGNES		EQU	50
MAX_NB_GROUPES		EQU	30




				; *** Types des polygones ***
POLY_SOLID		EQU	0
POLY_FLAT		EQU	1
POLY_TRANSPARENT	EQU	2
POLY_TRAME		EQU	3
POLY_GOURAUD		EQU	4
POLY_DITHER		EQU	5
POLY_GOURAUD_TABLE	EQU	6
POLY_DITHER_TABLE	EQU	7

POLY_TEXTURE_SOLID	EQU	8
POLY_TEXTURE_FLAT	EQU	9
POLY_TEXTURE_GOURAUD	EQU	10
POLY_TEXTURE_DITHER	EQU	11
POLY_TEXTURE_SOLID_INC	EQU	12
POLY_TEXTURE_FLAT_INC	EQU	13
POLY_TEXTURE_GOURAUD_INC EQU	14
POLY_TEXTURE_DITHER_INC	EQU	15

POLY_TEXTUREZ_SOLID	EQU	16
POLY_TEXTUREZ_FLAT	EQU	17
POLY_TEXTUREZ_GOURAUD	EQU	18
POLY_TEXTUREZ_DITHER	EQU	19
POLY_TEXTUREZ_SOLID_INC	EQU	20
POLY_TEXTUREZ_FLAT_INC	EQU	21
POLY_TEXTUREZ_GOURAUD_INC EQU	22
POLY_TEXTUREZ_DITHER_INC EQU	23

MAX_POLYS_TYPE		EQU	24
MAX_LINES_TYPE		EQU	1

				; *** Bits de flag ***
MASK_QUADRILATERE	EQU	(1 SHL 15)	; Au niveau des faces
MASK_ENVIRONMENT	EQU	(1 SHL 14)


MASK_OBJECT_VERSION	EQU	(0FFh)		; Dans Obj_Info
MASK_OBJECT_ANIMATED	EQU	(1 SHL 8)
MASK_OBJECT_NO_SORT	EQU	(1 SHL 9)
MASK_OBJECT_TRANSPARENT	EQU	(1 SHL 10)



				; *** Structures des objets ***
STRUC_OBJ_HEADER	STRUC
  Obj_Info		dd	0
  Obj_SizeHeader	dw	0
  Obj_Dummy		dw	0
  Obj_ZV_Xmin		dd	0
  Obj_ZV_Xmax		dd	0
  Obj_ZV_Ymin		dd	0
  Obj_ZV_Ymax		dd	0
  Obj_ZV_Zmin		dd	0
  Obj_ZV_Zmax		dd	0
  Obj_Nb_Groupes	dd	0
  Obj_Off_Groupes	dd	0
  Obj_Nb_Points		dd	0
  Obj_Off_Points	dd	0
  Obj_Nb_Normales	dd	0
  Obj_Off_Normales	dd	0
  Obj_Nb_NormFaces	dd	0
  Obj_Off_NormFaces	dd	0
  Obj_Nb_Polys		dd	0
  Obj_Off_Polys		dd	0
  Obj_Nb_Lines		dd	0
  Obj_Off_Lines		dd	0
  Obj_Nb_Spheres	dd	0
  Obj_Off_Spheres	dd	0
  Obj_Nb_Textures	dd	0
  Obj_Off_Textures	dd	0
STRUC_OBJ_HEADER	ENDS


STRUC_OBJ_GROUPE	STRUC
  Obj_OrgGroupe		dw	0
  Obj_OrgPoint		dw	0
  Obj_NbPts		dw	0
  Obj_NbNorm		dw	0
STRUC_OBJ_GROUPE	ENDS


STRUC_OBJ_POINT		STRUC
  Obj_Pt_PX		dw	0
  Obj_Pt_PY		dw	0
  Obj_Pt_PZ		dw	0
  Obj_Pt_Groupe		dw	0
STRUC_OBJ_POINT		ENDS



STRUC_POLY_HEADER	STRUC
  Obj_TypePoly		dw	0
  Obj_NbPoly		dw	0
  Obj_OffNextType	dd	0
STRUC_POLY_HEADER	ENDS



STRUC_OBJ_LINE		STRUC
  Obj_LineType		dw	0
  Obj_LineCoul		dw	0
  Obj_Line_P1		dw	0
  Obj_Line_P2		dw	0
STRUC_OBJ_LINE		ENDS



STRUC_OBJ_SPHERE	STRUC
  Obj_SphereType	dw	0
  Obj_SphereCoul	dw	0
  Obj_Sphere_P1		dw	0
  Obj_Sphere_Rayon	dw	0
STRUC_OBJ_SPHERE	ENDS



STRUC_OBJ_TEXTURE	STRUC
  Obj_OrgMap		dw	0
  Obj_RepMask		dw	0
STRUC_OBJ_TEXTURE	ENDS



STRUC_POLY3_LIGHT	STRUC
  Poly3_P1		dw	0
  Poly3_P2		dw	0
  Poly3_P3		dw	0
			dw	0
  Poly3_Couleur		dw	0
  Poly3_Normale		dw	0
STRUC_POLY3_LIGHT	ENDS


STRUC_POLY4_LIGHT	STRUC
  Poly4_P1		dw	0
  Poly4_P2		dw	0
  Poly4_P3		dw	0
  Poly4_P4		dw	0
  Poly4_Couleur		dw	0
  Poly4_Normale		dw	0
STRUC_POLY4_LIGHT	ENDS



STRUC_POLY3_TEXTURE	STRUC
  Poly3_P1		dw	0
  Poly3_P2		dw	0
  Poly3_P3		dw	0
  Poly3_HandleText	dw	0
  Poly3_Couleur		dw	0
  Poly3_Normale		dw	0
  Poly3_U1		dw	0
  Poly3_V1		dw	0
  Poly3_U2		dw	0
  Poly3_V2		dw	0
  Poly3_U3		dw	0
  Poly3_V3		dw	0
STRUC_POLY3_TEXTURE	ENDS


STRUC_POLY4_TEXTURE	STRUC
  Poly4_P1		dw	0
  Poly4_P2		dw	0
  Poly4_P3		dw	0
  Poly4_P4		dw	0
  Poly4_Couleur		dw	0
  Poly4_Normale		dw	0
  Poly4_U1		dw	0
  Poly4_V1		dw	0
  Poly4_U2		dw	0
  Poly4_V2		dw	0
  Poly4_U3		dw	0
  Poly4_V3		dw	0
  Poly4_U4		dw	0
  Poly4_V4		dw	0
  Poly4_HandleText	dw	0
			dw	0
STRUC_POLY4_TEXTURE	ENDS




STRUC_POLY3_ENV		STRUC
  Poly3_P1		dw	0
  Poly3_P2		dw	0
  Poly3_P3		dw	0
  Poly3_HandleEnv	dw	0
  Poly3_Couleur		dw	0
  Poly3_Normale		dw	0
  Poly3_Scale		dw	0
			dw	0
STRUC_POLY3_ENV		ENDS


STRUC_POLY4_ENV		STRUC
  Poly4_P1		dw	0
  Poly4_P2		dw	0
  Poly4_P3		dw	0
  Poly4_P4		dw	0
  Poly4_Couleur		dw	0
  Poly4_Normale		dw	0
  Poly4_Scale		dw	0
  Poly4_HandleEnv	dw	0
STRUC_POLY4_ENV		ENDS



			; *** Animated objects ***

STATUS_FRAME		EQU	1
STATUS_LAST		EQU	2
STATUS_CHANGE		EQU	4
STATUS_BODY		EQU	8

TYPE_ROTATE		EQU	0
TYPE_TRANSLATE		EQU	1


MAX_GROUPS		EQU	30


GROUP_INFO		STRUCT
  GroupType		dw	0
  GroupAlpha		dw	0
  GroupBeta      	dw	0
  GroupGamma		dw	0
GROUP_INFO		ENDS


OBJ_3D			STRUCT
  ObjX			dd	0		; Object's position
  ObjY			dd	0
  ObjZ			dd	0

  ObjAlpha		dd	0		; Objects angles
  ObjBeta		dd	0
  ObjGamma		dd	0

  Body			dd	0		; Either Body's number or body's ptr
  NextBody		dd	0
  Anim			dd	0

  Texture		dd	0		; Object's texture (-1 if no changes)
  NextTexture		dd	0

  LastOfsIsPtr		dd	0

  LastFrame		dd	0
  LastOfsFrame		dd	0
  LastTimer		dd	0
  LastNbGroups		dd	0

  NextFrame		dd	0
  NextOfsFrame		dd	0
  NextTimer		dd	0
  NextNbGroups		dd	0

  LoopFrame		dd	0
  LoopOfsFrame		dd	0

  NbFrames		dd	0

  LastAnimStepX		dd	0
  LastAnimStepY		dd	0
  LastAnimStepZ		dd	0

  LastAnimStepAlpha	dd	0
  LastAnimStepBeta	dd	0
  LastAnimStepGamma	dd	0

  Interpolator		dd	0

  ObjTime		dd	0

  Status		dd	0

  Master		dd	0

  NbGroups		dd	MAX_GROUPS

  CurrentFrame		db	MAX_GROUPS*(size GROUP_INFO) DUP (<>)
OBJ_3D			ENDS


