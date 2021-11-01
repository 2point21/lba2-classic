#ifndef	INPUT_H
#define	INPUT_H

// Touches clavier du jeu

// Input

#define	I_UP			(1<<0)	// 'UP'		Avancer
#define	I_DOWN			(1<<1)	// 'DOWN'	Reculer
#define	I_LEFT			(1<<2)	// 'LEFT'	Tourner à Gauche
#define	I_RIGHT			(1<<3)	// 'RIGHT'	Tourner à droite
#define	I_THROW			(1<<4)  // 'ALT'	Utilisation arme
#define	I_COMPORTEMENT		(1<<5)	// 'CTRL'	Comportement Menu
#define	I_INVENTORY		(1<<6)	// 'SHIFT', 'I' Inventaire
#define	I_ACTION_M		(1<<7)	// 'SPACE' 	Action du Comportement
#define	I_ACTION_ALWAYS		(1<<8)	// 'W'   	Dialogue/Fouille
#define	I_RETURN		(1<<9)	// 'RETURN'	Recentrage caméra
#define	I_MENUS			(1<<10)	// 'ESC'	Menus
#define	I_HOLOMAP		(1<<11)	// 'H' 		Holomap
#define	I_PAUSE			(1<<12)	// 'P' 		Pause
#define	I_ESQUIVE		(1<<13) // 'X' 		Esquive

#define	I_NORMAL		(1<<14)	// 'F5' 	Comportement Normal
#define	I_SPORTIF		(1<<15)	// 'F6' 	Comportement Sportif
#define	I_AGGRESSIF		(1<<16)	// 'F7' 	Comportement Aggressif
#define	I_DISCRET		(1<<17)	// 'F8' 	Comportement Discret

#define	I_HELP			(1<<18)	// 'F1' 	Aide en ligne
#define	I_SAVE			(1<<19) // 'F2' et 'S' 	Sauver
#define	I_LOAD			(1<<20) // 'F3' et 'L' 	Charger
#define	I_OPTIONS		(1<<21)	// 'F4' et 'O' 	Options Menu

#define	I_CAMERA		(1<<22)	// '²'		Tourner la camera
#define	I_CAMERA_LEVEL_PLUS	(1<<23)	// '+'		Niveau Caméra suivant
#define	I_CAMERA_LEVEL_MOINS	(1<<24)	// '-'		Niveau Caméra Precedent

#define	I_WEAPON_1		(1<<25)	// '1'		Arme 1 (Balle Magique)
#define	I_WEAPON_2		(1<<26)	// '2'		Arme 2 (Balle Magique)
#define	I_WEAPON_3		(1<<27)	// '3'		Arme 3 (Balle Magique)
#define	I_WEAPON_4		(1<<28)	// '4'		Arme 4 (Balle Magique)
#define	I_WEAPON_5		(1<<29)	// '5'		Arme 5 (Balle Magique)
#define	I_WEAPON_6		(1<<30)	// '6'		Arme 6 (Balle Magique)
#define	I_WEAPON_7		(1<<31)	// '7'		Arme 7 (Balle Magique)

// Input2

#define	I_PINGOUIN		(1<<0)	// 'N'		Lancement Pingouin
#define	I_JETPACK		(1<<1)	// 'J'		Jetpack On/Off
#define	I_PROTECTION		(1<<2)	// 'C'		Sort de Protection
#define	I_FOUDRE		(1<<3)	// 'F'		Sort de Foudre

#define	MAX_INPUT	36

#define	I_JOY		(I_UP|I_DOWN|I_LEFT|I_RIGHT)
#define	I_FIRE		(I_ACTION_M|I_INVENTORY|I_RETURN|I_THROW|I_COMPORTEMENT)
#define	I_ACTION	(I_ACTION_M|I_ACTION_ALWAYS)

#define	ALL_INPUTS	(0xFFFFFFFFL)

// Input
extern	U32     LastInput ;
extern	U32     NbInput   ;
extern	U32	WaitNoKey ;

#define	InitWaitNoInput(inputs)	GetInput(inputs) ;
#define	InitWaitNoKey()		{WaitNoKey = TRUE; MyKey=0;}

#ifdef	LBA_EDITOR
extern	U32	WaitNoClick ;

#define	InitWaitNoClick(click)	{WaitNoClick |= click;MyClick&=~(click);}
#endif

typedef struct {
	U32	Key1 ;
	U32	Key2 ;
	} T_DEF_KEY ;

extern	T_DEF_KEY      	DefKeys[MAX_INPUT+1] ;

// tableau de configuration des touches utilisé au cours du jeu
extern	U32		TabInputBit[MAX_INPUT+1]  ;



/*══════════════════════════════════════════════════════════════════════════*
			██████┐ ██┐ ██┐ ██████┐ ██████┐
			└─██┌─┘ ███┐██│ └─██┌─┘ └─██┌─┘
			  ██│   ██████│   ██│     ██│
			  ██│   ██┌███│   ██│     ██│
			██████┐ ██│└██│ ██████┐   ██│
			└─────┘ └─┘ └─┘ └─────┘   └─┘
 *══════════════════════════════════════════════════════════════════════════*/

//-------------------------------------------------------------------------
extern	void	InitInput( void ) ;
//-------------------------------------------------------------------------
extern	void	ReadInputConfig( void ) ;
//-------------------------------------------------------------------------
extern	void	WriteInputConfig( void ) ;
//-------------------------------------------------------------------------
extern	void    MyGetInput( void ) ;
//-------------------------------------------------------------------------
extern	void	WaitNoInput( void ) ;
//-------------------------------------------------------------------------
extern	void	WaitInput( void ) ;
//-------------------------------------------------------------------------

#endif	// INPUT_H
