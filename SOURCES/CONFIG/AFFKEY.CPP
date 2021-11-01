#include	"c_extern.h"

char	ConfigTxt[MAX_INPUT][100] = {
	"Avancer",
	"Reculer",
	"Tourner à Gauche",
	"Tourner à droite",
	"Utilisation arme",
	"Comportement Menu",
	"Inventaire",
	"Action du Comportement",
	"Dialogue/Fouille",
	"Recentrage caméra",
	"Menus",
	"Holomap",
	"Pause",
	"Esquive",
	"Comportement Normal",
	"Comportement Sportif",
	"Comportement Aggressif",
	"Comportement Discret",
	"Aide en ligne",
	"Sauver",
	"Charger",
	"Options Menu",
	"Tourner la camera",
	"Niveau Caméra suivant",
	"Niveau Caméra Precedent",
	"Arme 1",
	"Arme 2",
	"Arme 3",
	"Arme 4",
	"Arme 5",
	"Arme 6",
	"Arme 7",
	"MécaPingouin",
	"Jetpack On/Off",
	"Sort de Protection",
	"Sort de Foudre",
	} ;

char	KeysTxt[89][13] = {
	"",		//		0
	"ESC",		// K_ESC	1
	"1",		// K_1		2
	"2",		// K_2		3
	"3",		// K_3		4
	"4",		// K_4		5
	"5",		// K_5		6
	"6",		// K_6		7
	"7",		// K_7		8
	"8",		// K_8		9
	"9",		// K_9		10
	"0",		// K_0		11
	")",		// K_PARENTHESE	12
	"=",		// K_EGAL	13
	"BackSpace",	// K_BACKSPACE	14
	"Tab",		// K_TAB	15
	"A",		// K_A		16
	"Z",		// K_Z		17
	"E",		// K_E		18
	"R",		// K_R		19
	"T",		// K_T		20
	"Y",		// K_Y		21
	"U",		// K_U		22
	"I",		// K_I		23
	"O",		// K_O		24
	"P",		// K_P		25
	"^",		// K_CHAPEAU	26
	"$",		// K_DOLLAR	27
	"Entrée",	// K_ENTER	28
	"CTRL",		// K_CTRL	29
	"Q",		// K_Q		30
	"S",		// K_S		31
	"D",		// K_D		32
	"F",		// K_F		33
	"G",		// K_G		34
	"H",		// K_H		35
	"J",		// K_J		36
	"K",		// K_K		37
	"L",		// K_L		38
	"M",		// K_M		39
	"ù",		// K_OU		40
	"²",		// K_CARRE	41
	"Shift Gauche",	// K_SHIFT_LEFT	42
	"*",		// K_MUL	43
	"W",		// K_W		44
	"X",		// K_X		45
	"C",		// K_C		46
	"V",		// K_V		47
	"B",		// K_B		48
	"N",		// K_N		49
	",",		// K_VIRGULE	50
	";",		// K_PT_VIRGULE	51
	":",		// K_2_PTS	52
	"!",		// K_EXC	53
	"Shift Droite",	// K_SHIFT_RIGHT 54
	"* (NumPad)",	// K_MUL_PAD	55
	"ALT",		// K_ALT	56
	"Espace",	// K_SPACE	57
	"Caps Lock",	// K_CAPS_LCK	58
	"F1",		// K_F1		59
	"F2",		// K_F2		60
	"F3",		// K_F3		61
	"F4",		// K_F4		62
	"F5",		// K_F5		63
	"F6",		// K_F6		64
	"F7",		// K_F7		65
	"F8",		// K_F8		66
	"F9",		// K_F9		67
	"F10",		// K_F10	68
	"Verr Num",	// K_NUMLOCK	69
	"Scroll Lock",	// K_SCROLL_LCK	70
	"Début",	// K_HOME	71
	"Haut",		// K_UP		72
	"PageUp",	// K_PAGE_UP	73
	"-",		// K_MOINS	74
	"Gauche",	// K_LEFT	75
	"Centre",	// K_CENTER	76
	"Droite",	// K_RIGHT	77
	"+",		// K_PLUS	78
	"Fin",		// K_END	79
	"Bas",		// K_DOWN       80
	"PageDown",	// K_PAGE_DOWN	81
	"Insert",	// K_INSER	82
	"Suppr",	// K_SUPPR	83
	"",		//		84
	"",		//		85
	"<",		// K_INF	86
	"F11",		// K_F11	87
	"F12",		// K_F12	88
	} ;

void	MenuConfig( void )
{
	FadeToPal( PtrPalNormal ) ;

	CoulText( 255, 0 ) ;

	WaitNoInput() ;

	do
	{
		GraphPrintf( FALSE, 10, 30, "Press a key (<ESC> to quit) ..." ) ;

		MyGetInput() ;

		if( MyKey )
		{
			if( MyKey<127 )
				GraphPrintf( FALSE, 10, 10, "%2d (0x%02x) : %s           ", MyKey, MyKey, KeysTxt[MyKey] ) ;
			else	GraphPrintf( FALSE, 10, 20, "%2d (0x%02x)", MyKey, MyKey ) ;
		}

		BoxUpdate() ;
	}
	while( MyKey!=K_ESC ) ;

	WaitNoInput() ;
	WaitInput() ;
}
