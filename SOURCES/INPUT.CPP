#include	"c_extern.h"

// Input
U32		WaitNoKey	= 0	;

#ifdef	LBA_EDITOR
U32		WaitNoClick	= 0	;
#endif

U32             LastInput       = 0     ;
U32		NbInput	= MAX_INPUT ;

T_DEF_KEY      	DefKeysDefault[MAX_INPUT+1] =
		{
			{K_GRAY_UP,K_NUMPAD_8},		// I_UP
			{K_GRAY_DOWN,K_NUMPAD_2},	// I_DOWN
			{K_GRAY_LEFT,K_NUMPAD_4},	// I_LEFT
			{K_GRAY_RIGHT,K_NUMPAD_6},	// I_RIGHT
			{K_ALT_LEFT,K_ALT_RIGHT},	// I_THROW
			{K_CTRL_LEFT,K_CTRL_RIGHT},	// I_COMPORTEMENT
			{K_SHIFT_LEFT,K_SHIFT_RIGHT},	// I_INVENTORY
			{K_SPACE,K_NUMPAD_5},		// I_ACTION_M
			{K_W,K_GRAY_END},		// I_ACTION_ALWAYS
			{K_ENTER,K_NUMPAD_ENTER},	// I_RETURN
			{K_F10,0},			// I_MENUS
			{K_H,K_TAB},			// I_HOLOMAP
			{K_P,0},			// I_PAUSE
			{K_X,0},			// I_ESQUIVE
			{K_F5,0},			// I_NORMAL
			{K_F6,0},			// I_SPORTIF
			{K_F7,0},			// I_AGRESSIF
			{K_F8,0},			// I_DISCRET
			{K_F1,0},			// I_HELP
			{K_F2,K_S},			// I_SAVE
			{K_F3,K_L},			// I_LOAD
			{K_F4,K_O},			// I_OPTIONS
			{K_BACKSPACE,K_CARRE},		// I_CAMERA
			{K_NUMPAD_PLUS,K_GRAY_PAGE_UP},	// I_CAMERA_LEVEL_PLUS
			{K_NUMPAD_MOINS,K_GRAY_PAGE_DOWN},// I_CAMERA_LEVEL_MOINS
			{K_1,0},			// I_WEAPON_1
			{K_2,0},			// I_WEAPON_2
			{K_3,0},			// I_WEAPON_3
			{K_4,0},			// I_WEAPON_4
			{K_5,0},			// I_WEAPON_5
			{K_6,0},			// I_WEAPON_6
			{K_7,0},			// I_WEAPON_7
			{K_N,0},			// I_PINGOUIN
			{K_J,0},			// I_JETPACK
			{K_C,0},			// I_PROTECTION
			{K_F,0},			// I_FOUDRE
		} ;

#ifdef	_WIN32
// for WIN95

T_DEF_KEY      	DefKeysDefault95[MAX_INPUT+1] =
		{
			{K_GRAY_UP,K_NUMPAD_8},		// I_UP
			{K_GRAY_DOWN,K_NUMPAD_2},	// I_DOWN
			{K_GRAY_LEFT,K_NUMPAD_4},	// I_LEFT
			{K_GRAY_RIGHT,K_NUMPAD_6},	// I_RIGHT
			{K_ALT,0},			// I_THROW
			{K_CTRL,0},			// I_COMPORTEMENT
			{K_SHIFT,0},			// I_INVENTORY
			{K_SPACE,K_NUMPAD_5},		// I_ACTION_M
			{K_W,K_GRAY_END},		// I_ACTION_ALWAYS
			{K_ENTER,K_NUMPAD_ENTER},	// I_RETURN
			{K_F10,0},			// I_MENUS
			{K_H,K_TAB},			// I_HOLOMAP
			{K_P,0},			// I_PAUSE
			{K_X,0},			// I_ESQUIVE
			{K_F5,0},			// I_NORMAL
			{K_F6,0},			// I_SPORTIF
			{K_F7,0},			// I_AGRESSIF
			{K_F8,0},			// I_DISCRET
			{K_F1,0},			// I_HELP
			{K_F2,K_S},			// I_SAVE
			{K_F3,K_L},			// I_LOAD
			{K_F4,K_O},			// I_OPTIONS
			{K_BACKSPACE,K_CARRE},		// I_CAMERA
			{K_NUMPAD_PLUS,K_GRAY_PAGE_UP},	// I_CAMERA_LEVEL_PLUS
			{K_NUMPAD_MOINS,K_GRAY_PAGE_DOWN},// I_CAMERA_LEVEL_MOINS
			{K_1,0},			// I_WEAPON_1
			{K_2,0},			// I_WEAPON_2
			{K_3,0},			// I_WEAPON_3
			{K_4,0},			// I_WEAPON_4
			{K_5,0},			// I_WEAPON_5
			{K_6,0},			// I_WEAPON_6
			{K_7,0},			// I_WEAPON_7
			{K_N,0},			// I_PINGOUIN
			{K_J,0},			// I_JETPACK
			{K_C,0},			// I_PROTECTION
			{K_F,0},			// I_FOUDRE
		} ;
#endif

T_DEF_KEY      	MemoDefKeys[MAX_INPUT+1] ;
T_DEF_KEY      	DefKeys[MAX_INPUT+1] ;

// tableau de configuration des touches utilisé au cours du jeux
U32		TabInputBit[MAX_INPUT*2+1]  ;


/*══════════════════════════════════════════════════════════════════════════*
			██████┐ ██┐ ██┐ ██████┐ ██████┐
			└─██┌─┘ ███┐██│ └─██┌─┘ └─██┌─┘
			  ██│   ██████│   ██│     ██│
			  ██│   ██┌███│   ██│     ██│
			██████┐ ██│└██│ ██████┐   ██│
			└─────┘ └─┘ └─┘ └─────┘   └─┘
 *══════════════════════════════════════════════════════════════════════════*/

//-------------------------------------------------------------------------
void	InitInput( void )
{
	U32	mask ;

	for( NbInput=0; NbInput<MAX_INPUT*2; )
	{
		mask = (1<<(NbInput/2)) ;

		TabInputBit[NbInput++] = mask ;
		TabInputBit[NbInput++] = mask ;
	}

	if( NbInput>64 )	NbInput = 64 ;

	DefineInputKeys( NbInput, (U32*)DefKeys, TabInputBit ) ;
}

//-------------------------------------------------------------------------
void	RestoreInput()
{
#ifdef	_WIN32
	T_DEF_KEY *DEFKEYSDEFAULT	;

	if(OSType == OS_WIN95)
	{
		DEFKEYSDEFAULT = DefKeysDefault95;
	}
	else
	{
		DEFKEYSDEFAULT = DefKeysDefault	;
	}
#else
	#define	DEFKEYSDEFAULT	DefKeysDefault
#endif

	memcpy( DefKeys, DEFKEYSDEFAULT, sizeof(T_DEF_KEY)*MAX_INPUT ) ;
}

//-------------------------------------------------------------------------
void	ReadInputConfig( void )
{
	S32	initkeys = FALSE ;
	S32	winmode ;
	S32	n ;
	char	id[50] ;

	winmode = DefFileBufferReadValueDefault( "WinMode", FALSE ) ;

#ifdef	_WIN32
	if( !winmode )
#else	// _WIN32
	if( winmode )
#endif	// _WIN32
	{
		RestoreInput()	;
	}
	else
	{
#ifdef	_WIN32
		T_DEF_KEY *DEFKEYSDEFAULT	;

		if(OSType == OS_WIN95)
		{
			DEFKEYSDEFAULT = DefKeysDefault95;
		}
		else
		{
			DEFKEYSDEFAULT = DefKeysDefault	;
		}
#else
		#define	DEFKEYSDEFAULT	DefKeysDefault
#endif
		// On peut identifier chaque input avec un tableau de chaine,
		// mais bon, bof !!!

		for( n=0; n<MAX_INPUT; n++ )
		{
			sprintf( id, "Input%d_1", n ) ;
			DefKeys[n].Key1 = DefFileBufferReadValueDefault( id, DEFKEYSDEFAULT[n].Key1 ) ;
			// Blindage si les Inputs sont configurés à 0
//			if( !DefKeys[n].Key1 )	DefKeys[n].Key1 = DEFKEYSDEFAULT[n].Key1 ;

			sprintf( id, "Input%d_2", n ) ;
			DefKeys[n].Key2 = DefFileBufferReadValueDefault( id, DEFKEYSDEFAULT[n].Key2 ) ;
			// Blindage si les Inputs sont configurés à 0
//			if( !DefKeys[n].Key2 )	DefKeys[n].Key2 = DEFKEYSDEFAULT[n].Key2 ;
		}
	}

	InitInput() ;
}

//-------------------------------------------------------------------------
void	WriteInputConfig( void )
{
	S32	n ;
	char	id[50] ;

	if( !FileSize(PathConfigFile ) )	return ;

#ifdef	_WIN32
	DefFileBufferWriteValue( "WinMode", TRUE ) ;
#else
	DefFileBufferWriteValue( "WinMode", FALSE ) ;
#endif

	// On peut identifier chaque input avec un tableau de chaine,
	// mais bon, bof !!!

	for( n=0; n<MAX_INPUT; n++ )
	{
		sprintf( id, "Input%d_1", n ) ;
		DefFileBufferWriteValue( id, DefKeys[n].Key1 ) ;

		sprintf( id, "Input%d_2", n ) ;
		DefFileBufferWriteValue( id, DefKeys[n].Key2 ) ;
	}
}

//-------------------------------------------------------------------------
void    MyGetInput()
{
#ifdef	_WIN32
	GetJoys((U32*)&TabKeys[256])	;
#endif
	GetInput( 0 ) ;	// no norepeat keys

	MyKey = Key ;

	// Attente d'événement de touche dans la boucle

	if( WaitNoKey )
	{
		if( MyKey )
		{
			MyKey = 0 ;
		}
		else	WaitNoKey = 0 ;
	}

#ifdef	LBA_EDITOR
	MyClick = Click ;

	if( WaitNoClick )
	{
		if( MyClick&WaitNoClick )
		{
			MyClick &= ~(WaitNoClick) ;
		}
		else	WaitNoClick = 0 ;
	}
#endif
}

//***************************************************************************
/*──────────────────────────────────────────────────────────────────────────*/
void	WaitNoInput( void )
{
	MyGetInput() ;

	while( MyKey OR Input
#ifdef	LBA_EDITOR
	OR MyClick
#endif
	)	MyGetInput() ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	WaitInput( void )
{
	MyGetInput() ;

	while( !MyKey AND !Input
#ifdef	LBA_EDITOR
	AND    !MyClick
#endif
	)	MyGetInput() ;
}
//***************************************************************************
