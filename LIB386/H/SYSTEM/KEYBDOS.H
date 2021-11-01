//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#ifndef	LIB_SYSTEM_KEYBDOS
#define LIB_SYSTEM_KEYBDOS

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#ifdef	__cplusplus
extern	"C"	{
#endif

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern	U32	Old_PM09_Off	;
extern	U32	Old_PM09_Sel	;

extern  U32	KeybDataStart	;
extern  U32	KeybDataEnd	;

extern  U32	KeybCodeStart	;
extern  U32	KeybCodeEnd	;

extern  U32	Stack		;

extern  U8	PatchExtended	;

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define	AllowExtendedKeyb()	(*(&PatchExtended+2)=0x80)

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern volatile	S32	TabKeys[8]	;

#pragma aux TabKeys	"*"

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern volatile	S32	Key		;

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern  	S32	AsciiMode	;

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern	void	__interrupt	KeybHandler(void)	;

#pragma	aux	KeybHandler	"*"

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define		ManageKeyboard()	ManageEvents()

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern	S32	GetAscii()		;

#pragma	aux	GetAscii		"*"

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern	S32	ClearAsciiBuffer()	;

#pragma	aux	ClearAsciiBuffer	"*"

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define KeyIndex(x)     ((x)/32)
#define KeyBit(x)       (1<<((x)&31))
#define CheckKey(x)     (TabKeys[KeyIndex(x)]&KeyBit(x))
#define FlagKey(x)      (CheckKey(x)>>((x)&31))

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define K_EXTENDED      0x80

#define K_UP            0x48
#define K_DOWN          0x50
#define K_LEFT          0x4B
#define K_RIGHT         0x4D
#define K_HOME          0x47
#define K_END           0x4F
#define K_PAGE_UP       0x49
#define K_PAGE_DOWN     0x51
#define K_CENTER        0x4C

#define K_SPACE         0x39
#define K_ENTER         0x1C
#define K_SHIFT		0x2A
#define K_CTRL          0x1D
#define K_ALT           0x38
#define K_SUPPR         0x53
#define K_INSER         0x52

#define K_SHIFT_LEFT    K_SHIFT
#define K_SHIFT_RIGHT   0x36
#define K_CTRL_LEFT	K_CTRL
#define K_CTRL_RIGHT	(K_CTRL_LEFT+K_EXTENDED)
#define K_ALT_LEFT	K_ALT
#define K_ALT_RIGHT	(K_ALT_LEFT+K_EXTENDED)

#define K_F1            0x3B
#define K_F2            0x3C
#define K_F3            0x3D
#define K_F4            0x3E
#define K_F5            0x3F
#define K_F6            0x40
#define K_F7            0x41
#define K_F8            0x42
#define K_F9            0x43
#define K_F10           0x44
#define K_F11           0x57
#define K_F12           0x58

#define K_ESC           0x01

#define K_PLUS          0x4E
#define K_MOINS         0x4A
#define K_BACKSPACE     0x0E
#define K_TAB           0x0F
#define K_NUMLOCK       0x45
#define K_CAPSLOCK      0x3A

#define	K_PRINTSCR	(0x37+K_EXTENDED)
#define	K_SCROLLLOCK	0x46
#define	K_PAUSE		0x77

#define K_A             0x10
#define K_B             0x30
#define K_C             0x2E
#define K_D             0x20
#define K_E             0x12
#define K_F             0x21
#define K_G             0x22
#define K_H             0x23
#define K_I             0x17
#define K_J             0x24
#define K_K             0x25
#define K_L             0x26
#define K_M             0x27
#define K_N             0x31
#define K_O             0x18
#define K_P             0x19
#define K_Q             0x1E
#define K_R             0x13
#define K_S             0x1F
#define K_T             0x14
#define K_U             0x16
#define K_V             0x2F
#define K_W             0x2C
#define K_X             0x2D
#define K_Y             0x15
#define K_Z             0x11

#define K_1             0x02
#define K_2             0x03
#define K_3             0x04
#define K_4             0x05
#define K_5             0x06
#define K_6             0x07
#define K_7             0x08
#define K_8             0x09
#define K_9             0x0A
#define K_0             0x0B

#define	K_NUMPAD_1	0x4F
#define	K_NUMPAD_2	0x50
#define	K_NUMPAD_3	0x51
#define	K_NUMPAD_4	0x4B
#define	K_NUMPAD_5	0x4C
#define	K_NUMPAD_6	0x4D
#define	K_NUMPAD_7	0x47
#define	K_NUMPAD_8	0x48
#define	K_NUMPAD_9	0x49
#define	K_NUMPAD_0	0x52

#define	K_NUMPAD_POINT	0x53

#define	K_NUMPAD_ENTER	(K_ENTER+K_EXTENDED)

#define	K_NUMPAD_DIV	(0x35+K_EXTENDED)
#define	K_NUMPAD_MUL	0x37
#define K_NUMPAD_PLUS   0x4E
#define K_NUMPAD_MOINS  0x4A

#define K_GRAY_UP	(K_UP+K_EXTENDED)
#define K_GRAY_DOWN	(K_DOWN+K_EXTENDED)
#define K_GRAY_LEFT	(K_LEFT+K_EXTENDED)
#define K_GRAY_RIGHT 	(K_RIGHT+K_EXTENDED)
#define K_GRAY_HOME	(K_HOME+K_EXTENDED)
#define K_GRAY_END	(K_END+K_EXTENDED)
#define K_GRAY_PAGE_UP	(K_PAGE_UP+K_EXTENDED)
#define K_GRAY_PAGE_DOWN (K_PAGE_DOWN+K_EXTENDED)
#define K_GRAY_INSER	(K_INSER+K_EXTENDED)
#define K_GRAY_SUPPR	(K_SUPPR+K_EXTENDED)

#define K_WINDOWS_LEFT	(0x5B+K_EXTENDED)
#define K_WINDOWS_RIGHT	(0x5C+K_EXTENDED)
#define K_APPLICATION	(0x5D+K_EXTENDED)

#define	K_CARRE		0x29	// 'ı'
#define	K_INF		0x56	// '<'
#define	K_EXC		0x35	// '!'
#define	K_PARENTHESE	0x0C	// ')'
#define	K_EGAL		0x0D	// '='
#define	K_CHAPEAU	0x1A	// '^'
#define	K_DOLLAR	0x1B	// '$'
#define	K_OU		0x28	// '—'
#define	K_MUL		0x2B	// '*'
#define	K_VIRGULE	0x32	// ','
#define	K_PT_VIRGULE	0x33	// ';'
#define	K_2_PTS		0x34	// ':'

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
/*
#define	K_SCROLL_LCK	0x46	// 'Scroll Lock'
#define	K_CAPS_LCK	0x3A	// 'Caps Lock'
#define	K_MUL_PAD	0x37	// '* NumPad'

*/
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define A_ESC           0x011B

#define A_F1            0x3B00
#define A_F2            0x3C00
#define A_F3            0x3D00
#define A_F4            0x3E00
#define A_F5            0x3F00
#define A_F6            0x4000
#define A_F7            0x4100
#define A_F8            0x4200
#define A_F9            0x4300
#define A_F10           0x4400

#define A_UP            0x4800
#define A_DOWN          0x5000
#define A_LEFT          0x4B00
#define A_RIGHT         0x4D00

#define A_HOME          0x4700
#define A_END           0x4F00
#define A_PGUP          0x4900
#define A_PGDN          0x5100

#define A_SPACE         0x3920
#define A_RETURN        0x1C0D

#define A_N_PLUS        0x4E2B
#define A_N_MOINS       0x4A2D

#define A_BACKSPACE     0x0E08
#define A_SUPPR         0x5300

#define A_CARRE         0x29FD

#define A_TAB           0x0F09
#define A_INS           0x5200

#define A_1             0x226
#define A_2             0x382
#define A_3             0x422
#define A_4             0x527
#define A_5             0x628
#define A_6             0x77D
#define A_7             0x88A
#define A_8             0x95F
#define A_9             0xA87
#define A_0             0xB85

#define A_A             0x1061
#define A_B             0x3062
#define A_C             0x2E63
#define A_D             0x2064
#define A_E             0x1265
#define A_F             0x2166
#define A_G             0x2267
#define A_H             0x2368
#define A_I             0x1769
#define A_J             0x246A
#define A_K             0x256B
#define A_L             0x266C
#define A_M             0x276D
#define A_N             0x316E
#define A_O             0x186F
#define A_P             0x1970
#define A_Q             0x1E71
#define A_R             0x1372
#define A_S             0x1F73
#define A_T             0x1474
#define A_U             0x1675
#define A_V             0x2F76
#define A_W             0x2C77
#define A_X             0x2D78
#define A_Y             0x1579
#define A_Z             0x117A

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#ifdef	__cplusplus
}
#endif

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#endif//LIB_SYSTEM_KEYBDOS

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
