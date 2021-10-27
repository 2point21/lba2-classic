
Struc_MatriceMMX	STRUCT
  MMX_M11		dw	?
  MMX_M12		dw	?
  MMX_M13		dw	?
  MMX_M14		dw	?

  MMX_M21		dw	?
  MMX_M22		dw	?
  MMX_M23		dw	?
  MMX_M24		dw	?

  MMX_M31		dw	?
  MMX_M32		dw	?
  MMX_M33		dw	?
  MMX_M34		dw	?

  MMX_MTX		dw	?
  MMX_MTY		dw	?
  MMX_MTZ		dw	?
			dw	0

  MMX_LTX		dd	?
  MMX_LTY		dd	?
  MMX_LTZ		dd	?
			dd	0

Struc_MatriceMMX	ENDS




Struc_MatriceMAT	STRUCT
  MAT_M11		dd	?
  MAT_M12		dd	?
  MAT_M13		dd	?

  MAT_M21		dd	?
  MAT_M22		dd	?
  MAT_M23		dd	?

  MAT_M31		dd	?
  MAT_M32		dd	?
  MAT_M33		dd	?

  MAT_MTX		dd	?
  MAT_MTY		dd	?
  MAT_MTZ		dd	?
Struc_MatriceMAT	ENDS


;STRUC_MATRICE		UNION
;			Struc_MatriceMMX <>
;			Struc_MatriceMAT <>
;STRUC_MATRICE		ENDS


SIZE_MATRICE		EQU	Size Struc_MatriceMAT


STRUC_VERTEX		STRUCT
  VT_X			dw	?
  VT_Y			dw	?
  VT_Z			dw	?
  			dw	?
STRUC_VERTEX		ENDS


STRUC_PT		STRUCT
  PT_X			dw	?
  PT_Y			dw	?
STRUC_PT		ENDS
