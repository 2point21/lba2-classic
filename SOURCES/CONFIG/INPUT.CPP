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
			{K_UP,0},		// I_UP
			{K_DOWN,0},		// I_DOWN
			{K_LEFT,0},		// I_LEFT
			{K_RIGHT,0},		// I_RIGHT
			{K_ALT,0},		// I_THROW
			{K_CTRL,0},		// I_COMPORTEMENT
			{K_SHIFT_LEFT,K_I},	// I_INVENTORY
			{K_SPACE,0},		// I_ACTION_M
			{K_W,K_END},		// I_ACTION_ALWAYS
			{K_ENTER,0},		// I_RETURN
			{K_ESC,0},		// I_MENUS
			{K_H,0},		// I_HOLOMAP
			{K_P,0},		// I_PAUSE
			{K_X,0},		// I_ESQUIVE
			{K_F5,0},		// I_NORMAL
			{K_F6,0},		// I_SPORTIF
			{K_F7,0},		// I_AGGRESSIF
			{K_F8,0},		// I_DISCRET
			{K_F1,0},		// I_HELP
			{K_F2,K_S},		// I_SAVE
			{K_F3,K_L},		// I_LOAD
			{K_F4,K_O},		// I_OPTIONS
			{K_CARRE,0},		// I_CAMERA
			{K_PLUS,0},		// I_CAMERA_LEVEL_PLUS
			{K_MOINS,0},		// I_CAMERA_LEVEL_MOINS
			{K_1,0},		// I_WEAPON_1
			{K_2,0},		// I_WEAPON_2
			{K_3,0},		// I_WEAPON_3
			{K_4,0},		// I_WEAPON_4
			{K_5,0},		// I_WEAPON_5
			{K_6,0},		// I_WEAPON_6
			{K_7,0},		// I_WEAPON_7
			{K_N,0},		// I_PINGOUIN
			{K_J,0},		// I_JETPACK
			{K_C,0},		// I_PROTECTION
			{K_F,0},		// I_FOUDRE
		} ;

T_DEF_KEY      	DefKeys[MAX_INPUT+1] ;

// tableau de configuration des touches utilisé au cours du jeux
U32		TabInputBit[MAX_INPUT+1]  ;


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
	U32	key ;
	U32	shift = 0 ;
	U32	shiftbit = 0 ;

	for( NbInput=0; NbInput<MAX_INPUT; )
	{
		key = DefKeys[NbInput].Key1 ;

		if( key==K_SHIFT_LEFT
		OR  key==K_SHIFT_RIGHT )
		{
			shift = key ;
			shiftbit = (1<<NbInput) ;
		}

		TabInputBit[NbInput] = (1<<NbInput) ;

		key = DefKeys[NbInput].Key2 ;

		NbInput++ ;

		if( key==K_SHIFT_LEFT
		OR  key==K_SHIFT_RIGHT )
		{
			shift = key ;
			shiftbit = (1<<NbInput) ;
		}

		TabInputBit[NbInput] = (1<<NbInput) ;

		NbInput++ ;
	}

	switch( shift )	// on doit rajouter le second shift ?
	{
		case K_SHIFT_LEFT:
			TabInputBit[NbInput] = shiftbit ;
			DefKeys[NbInput].Key1 = K_SHIFT_RIGHT;
			NbInput++ ;
			break ;

		case K_SHIFT_RIGHT:
			TabInputBit[NbInput] = shiftbit ;
			DefKeys[NbInput].Key1 = K_SHIFT_LEFT ;
			NbInput++ ;
			break ;
	}

	DefineInputKeys( NbInput, (U32*)DefKeys, TabInputBit ) ;
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
		// reinit des input par défaut
		for( n=0; n<MAX_INPUT; n++ )
		{
			DefKeys[n].Key1 = DefKeysDefault[n].Key1 ;
			DefKeys[n].Key2 = DefKeysDefault[n].Key2 ;
		}
	}
	else
	{
		// On peut identifier chaque input avec un tableau de chaine,
		// mais bon, bof !!!

		for( n=0; n<MAX_INPUT; n++ )
		{
			sprintf( id, "Input%d_1", n ) ;
			DefKeys[n].Key1 = DefFileBufferReadValueDefault( id, DefKeysDefault[n].Key1 ) ;
			// Blindage si les Inputs sont configurés à 0
			if( !DefKeys[n].Key1 )	DefKeys[n].Key1 = DefKeysDefault[n].Key1 ;

			sprintf( id, "Input%d_2", n ) ;
			DefKeys[n].Key2 = DefFileBufferReadValueDefault( id, DefKeysDefault[n].Key2 ) ;
			// Blindage si les Inputs sont configurés à 0
			if( !DefKeys[n].Key2 )	DefKeys[n].Key2 = DefKeysDefault[n].Key2 ;
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

	while( MyKey
#ifdef	LBA_EDITOR
	OR MyClick
#endif
	)	MyGetInput() ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	WaitInput( void )
{
	MyGetInput() ;

	while( !MyKey
#ifdef	LBA_EDITOR
	AND    !MyClick
#endif
	)	MyGetInput() ;
}
//***************************************************************************
