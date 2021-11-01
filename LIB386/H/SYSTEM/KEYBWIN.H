//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_SYSTEM_KEYBWIN
#define LIB_SYSTEM_KEYBWIN

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
extern	S32		AsciiMode		;
extern	S32		Key			;

//──────────────────────────────────────────────────────────────────────────
extern	U8		TabKeys[]		;

//──────────────────────────────────────────────────────────────────────────
extern	WINDOW_PROC_PTR	OldKeybWindowProc	;

//──────────────────────────────────────────────────────────────────────────
extern	WINDOW_PROC	KeybWindowProc		;

//──────────────────────────────────────────────────────────────────────────
extern	void	ManageKeyboard()		;

//──────────────────────────────────────────────────────────────────────────
extern	S32	GetAscii()			;

//──────────────────────────────────────────────────────────────────────────
extern	void	ClearAsciiBuffer()		;

//──────────────────────────────────────────────────────────────────────────
extern	HANDLE	AsciiMutex			;

//──────────────────────────────────────────────────────────────────────────
#define	AllowExtendedKeyb()

//──────────────────────────────────────────────────────────────────────────
#define KeyIndex(x)     ((x)<256?(x):256-32+((x)>>3))
#define KeyBit(x)       ((x)<256?0x80:(1<<((x)&7)))
//#define CheckKey(x)     (TabKeys[KeyIndex(x)]&KeyBit(x))
#define CheckKey(x)     ((x)<256?TabKeys[(x)]&0x80:TabKeys[256-32+((x)>>3)]&(1<<((x)&7)))
#define FlagKey(x)      (CheckKey(x)>>((x)<256?7:(x)&7))
/*
#define KeyIndex(x)     ((x)/4)
#define KeyBit(x)       (128 << (((x)&3)*8))
#define CheckKey(x)     (TabKeys[KeyIndex(x)]&KeyBit(x))
#define FlagKey(x)      ((CheckKey(x)>>(((x)&3)*8))/128)
*/
//──────────────────────────────────────────────────────────────────────────
#define K_UP            VK_UP
#define K_DOWN          VK_DOWN
#define K_LEFT          VK_LEFT
#define K_RIGHT         VK_RIGHT
#define K_HOME          VK_HOME
#define K_PAGE_UP       VK_PRIOR
#define K_PAGE_DOWN     VK_NEXT
#define K_END           VK_END

#define K_SPACE         VK_SPACE
#define K_ENTER         VK_RETURN
#define K_SUPPR         VK_DELETE
#define K_INSER		VK_INSERT
#define K_CAPSLOCK      VK_CAPITAL
#define K_ALT           VK_MENU
#define K_CTRL          VK_CONTROL
#define K_CTRL_LEFT     VK_LCONTROL
#define K_CTRL_RIGHT    VK_RCONTROL
#define K_ALT_LEFT      VK_LMENU
#define K_ALT_RIGHT	VK_RMENU
#define K_SHIFT         VK_SHIFT
#define K_SHIFT_LEFT    VK_LSHIFT
#define K_SHIFT_RIGHT   VK_RSHIFT

#define K_ESC           VK_ESCAPE

#define K_F1            VK_F1
#define K_F2            VK_F2
#define K_F3            VK_F3
#define K_F4            VK_F4
#define K_F5            VK_F5
#define K_F6            VK_F6
#define K_F7            VK_F7
#define K_F8            VK_F8
#define K_F9            VK_F9
#define K_F10           VK_F10
#define K_F11           VK_F11
#define K_F12           VK_F12

#define K_PLUS          VK_ADD
#define K_MOINS         VK_SUBTRACT
#define K_BACKSPACE     VK_BACK
#define K_TAB           VK_TAB
#define K_NUMLOCK       VK_NUMLOCK

#define	K_PRINTSCR	VK_SNAPSHOT
#define	K_SCROLLLOCK	VK_SCROLL
#define	K_PAUSE		VK_PAUSE

#define K_NUMPAD_1      VK_NUMPAD1
#define K_NUMPAD_2      VK_NUMPAD2
#define K_NUMPAD_3      VK_NUMPAD3
#define K_NUMPAD_4      VK_NUMPAD4
#define K_NUMPAD_5      VK_NUMPAD5
#define K_NUMPAD_6      VK_NUMPAD6
#define K_NUMPAD_7      VK_NUMPAD7
#define K_NUMPAD_8      VK_NUMPAD8
#define K_NUMPAD_9      VK_NUMPAD9
#define K_NUMPAD_0      VK_NUMPAD0

#define	K_NUMPAD_ENTER	VK_SEPARATOR

#define	K_NUMPAD_POINT	VK_DECIMAL

#define	K_NUMPAD_DIV	VK_DIVIDE
#define	K_NUMPAD_MUL	VK_MULTIPLY
#define K_NUMPAD_PLUS   VK_ADD
#define K_NUMPAD_MOINS  VK_SUBTRACT

#define K_WINDOWS_LEFT	VK_LWIN
#define K_WINDOWS_RIGHT	VK_RWIN
#define K_APPLICATION	VK_APPS

#define K_GRAY_UP	K_UP
#define K_GRAY_DOWN	K_DOWN
#define K_GRAY_LEFT	K_LEFT
#define K_GRAY_RIGHT 	K_RIGHT
#define K_GRAY_HOME	K_HOME
#define K_GRAY_END	K_END
#define K_GRAY_PAGE_UP	K_PAGE_UP
#define K_GRAY_PAGE_DOWN K_PAGE_DOWN
#define K_GRAY_INSER	K_INSER
#define K_GRAY_SUPPR	K_SUPPR

#define	K_CARRE		0xDE

#define K_A             'A'
#define K_B             'B'
#define K_C             'C'
#define K_D             'D'
#define K_E             'E'
#define K_F             'F'
#define K_G             'G'
#define K_H             'H'
#define K_I             'I'
#define K_J             'J'
#define K_K             'K'
#define K_L             'L'
#define K_M             'M'
#define K_N             'N'
#define K_O             'O'
#define K_P             'P'
#define K_Q             'Q'
#define K_R             'R'
#define K_S             'S'
#define K_T             'T'
#define K_U             'U'
#define K_V             'V'
#define K_W             'W'
#define K_X             'X'
#define K_Y             'Y'
#define K_Z             'Z'

#define K_1             '1'
#define K_2             '2'
#define K_3             '3'
#define K_4             '4'
#define K_5             '5'
#define K_6             '6'
#define K_7             '7'
#define K_8             '8'
#define K_9             '9'
#define K_0             '0'

//──────────────────────────────────────────────────────────────────────────
#define A_UP            VK_UP
#define A_DOWN          VK_DOWN
#define A_LEFT          VK_LEFT
#define A_RIGHT         VK_RIGHT

#define A_HOME          VK_HOME
#define A_END           VK_END
#define A_PGUP          VK_PRIOR
#define A_PGDN          VK_NEXT

#define A_SPACE         VK_SPACE
#define A_RETURN        VK_RETURN

#define A_SUPPR         VK_DELETE

#define A_ESC           VK_ESCAPE

#define A_F1            VK_F1
#define A_F2            VK_F2
#define A_F3            VK_F3
#define A_F4            VK_F4
#define A_F5            VK_F5
#define A_F6            VK_F6
#define A_F7            VK_F7
#define A_F8            VK_F8
#define A_F9            VK_F9
#define A_F10           VK_F10
#define A_F11           VK_F11
#define A_F12           VK_F12


#define A_N_PLUS        VK_ADD
#define A_N_MOINS       VK_SUBSTRACT
#define A_BACKSPACE     VK_BACK
#define A_TAB           VK_TAB
#define A_NUMLOCK       VK_NUMLOCK

#define A_A             'A'
#define A_B             'B'
#define A_C             'C'
#define A_D             'D'
#define A_E             'E'
#define A_F             'F'
#define A_G             'G'
#define A_H             'H'
#define A_I             'I'
#define A_J             'J'
#define A_K             'K'
#define A_L             'L'
#define A_M             'M'
#define A_N             'N'
#define A_O             'O'
#define A_P             'P'
#define A_Q             'Q'
#define A_R             'R'
#define A_S             'S'
#define A_T             'T'
#define A_U             'U'
#define A_V             'V'
#define A_W             'W'
#define A_X             'X'
#define A_Y             'Y'
#define A_Z             'Z'

#define A_1             '1'
#define A_2             '2'
#define A_3             '3'
#define A_4             '4'
#define A_5             '5'
#define A_6             '6'
#define A_7             '7'
#define A_8             '8'
#define A_9             '9'
#define A_0             '0'

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_SYSTEM_KEYBWIN

//──────────────────────────────────────────────────────────────────────────
