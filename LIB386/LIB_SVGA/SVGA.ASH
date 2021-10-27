;-----------------------------------------------------------------------------
;			Extern lib_svga 386
;-----------------------------------------------------------------------------
				.data

		extrn	NoLanguage 	ClipXmin	:DWORD
		extrn	NoLanguage 	ClipYmin	:DWORD
		extrn	NoLanguage 	ClipXmax	:DWORD
		extrn	NoLanguage 	ClipYmax	:DWORD
		extrn	NoLanguage 	Phys		:DWORD
		extrn	NoLanguage 	Log		:DWORD
		extrn	NoLanguage 	MemoLog		:DWORD
		extrn	NoLanguage 	BankCurrent	:DWORD

		extrn   NoLanguage 	Screen_X	:DWORD
		extrn	NoLanguage 	Screen_Y	:DWORD
		extrn	NoLanguage 	TabOffLine	:DWORD

		extrn	NoLanguage 	SizeCar		:WORD

		extrn   NoLanguage 	Text_Ink	:BYTE
		extrn	NoLanguage 	Text_Paper	:BYTE
		extrn	NoLanguage 	OldVideo	:BYTE

POLY_TRISTE	equ	0
POLY_COPPER	equ	1
POLY_BOPPER	equ	2
POLY_MARBRE	equ	3
POLY_TELE	equ	4
POLY_TRANS	equ	5
POLY_TRAME	equ	6
POLY_GOURAUD	equ	7
POLY_DITHER	equ	8



