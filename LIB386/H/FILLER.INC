
; ษอออออออออออออออออออออออออออออออออออออออออออป
; บ ฺฤฤฤฤฤฟ ฺฤฤฤฤฤฟ ณ      ณ    ณ             บ
; บ ณ     ณ ณ     ณ ณ      ณ    ณ             บ
; บ ณ     ณ ณ     ณ ณ      ณ    ณ             ศอออออออออออออออออออออออัอออออออออออป
; บ ณ     ณ ณ     ณ ณ      ณ    ณ    ฺฤฤฤฤ ฤ                          ณDbut:     บ
; บ รฤฤฤฤฤู ณ     ณ ณ      ภฤฤฤฤด    ณ       ณ    ณ    ฺฤฤฤ รฤฤฟ      ณle 02/08/95บ
; บ ณ       ณ     ณ ณ           ณ    รฤฤ   ณ ณ    ณ    ณ    ณ         ณ           บ
; บ ณ       ณ     ณ ณ           ณ /  ณ     ณ ณ    ณ    รฤ   ณ         ณ           บ
; บ ณ       ณ     ณ ณ           ณ \  ณ     ณ ณ    ณ    ณ    ณ         ณ           บ
; บ ณ       ภฤฤฤฤฤู ภฤฤฤฤฤ      ณ /  ณ     ณ ภฤฤฤ ภฤฤฤ ภฤฤฤ ณ         ณ           บ
; บ                                                                   ณ           บ
; บ                                                                   ณ           บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ                          ณ           บ
; บModule: CONSTANTES ET MACROS            ณ                          ณ           บ
; ศออออออออออออออออออออออออออออออออออออออออฯป                         ณ MAJ:      บ
;                                           บ                         ณ 09/11/95  บ
;                                           ศอออออออออออออออออออออออออฯอออออออออออผ





; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ                                                                           บ
; บ  Structures, macros et constantes                                         บ
; บ                                                                           บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Struc_EntryPoint	EQU	STRUC_POINT
Pt_Coord		EQU	0

						; Structure d'un point tel
STRUC_POINT		STRUC			; qu'on peut le trouver dans
  Pt_XE			dw	0		; la table passee en parametre
  Pt_YE			dw	0		; aux routines de remplissage
  Pt_MapU		dw	0		; Coord de mapping
  Pt_MapV		dw	0
  Pt_Light		dw	0		; Lumiere
  Pt_ZO			dw	0		; Z norme sur 16 bits
  Pt_W			dd	0		; 1/Z norme sur 32 bits
STRUC_POINT		ENDS



STRUC_CLIPVERTEX	STRUC
  V_X0			dd	0
  V_Y0			dd	0
  V_Z0			dd	0
  V_MapU		dw	0
  V_MapV		dw	0
  V_Light		dw	0
  V_Dummy		dw	0
STRUC_CLIPVERTEX	ENDS


OUI			EQU	1
NON			EQU	0
TRUE			EQU	1
FALSE			EQU	0


SIZE_VIDEOLINE		EQU	640

NB_MAX_EDGES		EQU	NB_MAX_COTES
NB_MAX_COTES		EQU	16		; Nb de cotes max apres clipping


PERSP_SHIFT		EQU	4		; Shift value (log2(PERSP_VALUE))
PERSP_VALUE		EQU	(1 SHL PERSP_SHIFT); Nb of pixels rendered between two corrections

						; Pour le Flag_Display
						; Champ de bits
READ_NEXT_G		EQU	1		; Bit 0 : Lire le prochain sommet a gauche?
READ_NEXT_D		EQU	2		; Bit 1	: idem a droite ?
END_READ		EQU	4		; Bit 2	: quitter ?
READ_NEXT_L		EQU	1		; Bit 0 : Read next left point
READ_NEXT_R		EQU	2		; Bit 1	: Read next right point


CLIP_FLAT		EQU	1		; Clipper les bords des polys
CLIP_LIGHT		EQU	2		; Clipper la couleur
CLIP_TEXTURE		EQU	4		; Clipper la texture
CLIP_TEXTUREZ		EQU	8		; Clipper la texture plus le Z (cas d'une perspective corrected)
CLIP_ZBUFFER		EQU	16		; Heu, juste pour dire ZBuffer On
CLIP_ZFOG		EQU	32		; Flag de Fog pour le cas du TextureZ Fog
CLIP_DITHER		EQU	64		; If dithering On


FOG_DECAL		EQU	16+4		; Decalage "a la mano" pour le fog
						; dans notre cas: 16+4 pour 16 bits
						; de virgule et 4 bits de partie entiere.


ROUND_CHOP		MACRO	R1:req, R2:req
;	IFNB	R2
;			add	R1,R1
;			adc	R1,1
;			sar	R1,1
;	ELSE
			mov	R2,R1
			shr	R1,31
			add	R1,R2
;	ENDIF
			ENDM
