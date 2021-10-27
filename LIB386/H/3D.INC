NORMAL_UNIT	equ	16384
LIB_NORMAL_UNIT	equ	15360		; INT(15*256*4)
;LIB_NORMAL_UNIT	equ	16381	; INT(4095.49*65536/16384)
					;       MAX  *shift/ colinearity case
					; 16382 is too big, bcoz we can get 4095.5
					; that's bounded up to 4096=>overflow
TYPE_3D		equ	0
TYPE_ISO	equ	1
