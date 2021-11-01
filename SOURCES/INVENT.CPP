#include 	"c_extern.h"

#define	SHADE_INVENT_LVL	8

extern	S32	FlagAnimWhoSpeak	;	// MESSAGE.CPP

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	      Û    ÛÛÜ Û Û   Û Ûßßßß ÛÛÜ Û ßßÛßß ÛßßßÛ ÛßßßÛ Û  Üß
	      ÛÛ   ÛÛßÛÛ ÛÛ Üß ÛÛßß  ÛÛßÛÛ   ÛÛ  ÛÛ  Û ÛÛßÛß ÛÛß
	      ßß   ßß  ß ßßß   ßßßßß ßß  ß   ßß  ßßßßß ßß  ß ßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// MyMagicLevel correspond to Twinsen current level of magie
// MaxMagicLevel correspond to Twinsen current capabilities
// MAX_MAGIC_LEVEL correspond to Maximum Magie in the game
// FlagZoomed :  0:No Zoom / 1:Wait for Zoom / 2: Zoom en cours / 3 Zoom / 4 Got to left corner before explain
//		 5: Fixed in up center / 6:return to his own place / 7:fixed at his place
// TIME_BEFORE_ZOOM : Time in 50eme to wait before zoom
// TIME_ZOOM : Time before completely zoomed
/*
S32	MyMagicLevel = 50	;// "My" tempo
S32	MaxMagicLevel = 75	;
#define	MAX_MAGIC_LEVEL	100
*/
void	DrawInventoryScreen(void);
void	ClearNearAllBox(void)	;
S32	FlagZoomed=0		;
U32	MemoTimerZoom		;
#define	TIME_BEFORE_ZOOM	(50*20)
#define TIME_ZOOM		(25*20)
U32	TimerRot		;
S32	OldX0=0, OldY0=0, OldX1=5, OldY1=5	;
S32	InvSelect = 0 			;
S32	InvCoul = COUL_SELECT_MENU 	;

U8	PingouinActif = FALSE ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
T_INV_OBJ	TabInv[MAX_INVENTORY] ;

T_INV_OBJ	InitTabInv[MAX_INVENTORY] =
{
{
//	0: FLAG_HOLOMAP,
	2,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 0, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	1: FLAG_BALLE_MAGIQUE,
	0,			//	S32 Box
	0,			//	S32 BetaObj
	4, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 1, 40, 41, 42 }	//	U8 TabObj3D[4]
},
{
//	2: FLAG_DART,
	5,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	FLAG_INV_FEWER,		//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 2, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	3: FLAG_BOULE_SENDELL
	6,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 3, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	4: FLAG_TUNIQUE
	1,			//	S32 Box
	0,			//	S32 BetaObj
	10, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 4, 47, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	5: FLAG_PERLE, PUIS FLAG_DISQUE_DE_ROUTE
	23,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 5, 24, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	6: FLAG_CLEF_PYRAMID,
	29,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 6, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	7: FLAG_VOLANT,
	17,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 7, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	8: FLAG_MONEY,	(Kashes ou Zlitos)
	27,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	FLAG_INV_FEWER,		//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 8, 43, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	9: FLAG_PISTOLASER,
	25,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 49, 9, 50, 0 }	//	U8 TabObj3D[4]
},
{
//	10: FLAG_SABRE,
	30,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 10, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	11: FLAG_GANT,
	20,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 11, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	12: FLAG_PROTOPACK,
	7,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 12, 48, 0, 0 }	//	U8 TabObj3D[4]
},
{
//	13: FLAG_TICKET_FERRY,
	24,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 13, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	14: FLAG_MECA_PINGOUIN,
	22,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	FLAG_INV_FEWER,		//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 14, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	15: FLAG_GAZOGEM,
	4,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 15, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	16: FLAG_DEMI_MEDAILLON,
	9,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 16, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	17: FLAG_ACIDE_GALLIQUE,
	28,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 17, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	18: FLAG_CHANSON	//
	3,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 18, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	19: FLAG_ANNEAU_FOUDRE,
	11,			//	S32 Box
	0,			//	S32 BetaObj
	25, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 19, 51, 52, 53 }	//	U8 TabObj3D[4]
},
{
//	20: FLAG_PARAPLUIE,
	14,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 20, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	21: FLAG_GEMME,
	18,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	FLAG_INV_FEWER,		//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 21, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	22: FLAG_CONQUE,
	15,			//	S32 Box
	0,			//	S32 BetaObj
	50, 			//	S32 PtMagie
	FLAG_INV_VALUE,		//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 22, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	23: FLAG_SARBACANE,
	10,			//	S32 Box
	0,			//	S32 BetaObj
	2, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 23, 46, 0, 0 }	//	U8 TabObj3D[4]
},
{
//	24: VISIONNEUSE
	19,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 45, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	25: FLAG_TART_LUCI,
	8,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 25, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	26: FLAG_RADIO,
	12,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 26, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	27: FLAG_FLEUR,
	13,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 27, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	28: FLAG_ARDOISE,
	21,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 28, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	29: FLAG_TRADUCTEUR,    //	(REMPLACE le VOLANT)
	17,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	FLAG_INV_AUTO_USE,	//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 29, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	30: FLAG_DIPLOME,
	26,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 30, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	31: FLAG_DMKEY_KNARTA, FLAG_DMKEY
	31,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 31, 44, 0, 0 }	//	U8 TabObj3D[4]
},
{
//	32: FLAG_DMKEY_SUP,
	32,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 32, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	33: FLAG_DMKEY_MOSQUI,
	33,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 33, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	34: FLAG_DMKEY_BLAFARD,
	34,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 34, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	35: FLAG_CLE_REINE (REMPLACE LA FLEUR)
	13,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 35, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	36: FLAG_PIOCHE (REMPLACE L'ACIDE GALLIQUE)
	28,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 36, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	37: FLAG_CLEF_BOURGMESTRE (REMPLACE Clef pyramidale )
	29,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 37, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	38: FLAG_NOTE_BOURGMESTRE (REMPLACE Clef Bourgmestre )
	29,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 38, 0, 0, 0 }		//	U8 TabObj3D[4]
},
{
//	39: FLAG_PROTECTION
	16,			//	S32 Box
	0,			//	S32 BetaObj
	0, 			//	S32 PtMagie
	0,			//	S32 FlagInv
	0,			//	S32 IdObj3D
	{ 39, 0, 0, 0 }		//	U8 TabObj3D[4]
}
} ;

U8	TabIndir[MAX_BOX_INVENTORY]	;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	InitInventory()
{
//		Ouais, c'est vide, mais a mon avis pas pour longtemps...

#ifdef	LBA_EDITOR	// dans le jeu, c'est fait par InitGameList()
	T_INV_OBJ *pt	;

//		Voila, j'avais raison...

	S32	i	;


	pt = TabInv	;

	for ( i = 0 ; i < MAX_INVENTORY ; i++, pt++ )
	{
		ListVarGame[i].Value = 0	;//	Security ( kill the cat... )
	}
#endif

	// init TabInv with initial values
	memcpy( TabInv, InitTabInv, sizeof(T_INV_OBJ)*MAX_INVENTORY ) ;

//		Je m'ecoute jamais, pourtant, je devrais ;)
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	InitTabIndir()//	NEW
{
	T_INV_OBJ *pt	;
	S32	i	;
	S32	num	;

	if( Planet>=2 )
	{
		num = NbGoldPieces	;
		TabInv[FLAG_MONEY].IdObj3D = 0 ;	// Kashes
	}
	else
	{
		num = NbZlitosPieces	;
		TabInv[FLAG_MONEY].IdObj3D = 1 ;	// Zlitos
	}

#ifdef	LBA_EDITOR
	ListVarGame[FLAG_MONEY].Value = (S16)num ;
#else
	ListVarGame[FLAG_MONEY] = (S16)num ;
#endif

	// positionne balle de la bonne couleur dans
	// l'inventaire
	if( MagicLevel>0 )
	{
		TabInv[FLAG_BALLE_MAGIQUE].IdObj3D = (S16)(MagicLevel-1) ;
		TabInv[FLAG_ANNEAU_FOUDRE].IdObj3D = (S16)(MagicLevel-1) ;

		// Sort de foudre
		if( MagicPoint>=(MagicLevel*20) )
			TabInv[FLAG_ANNEAU_FOUDRE].FlagInv &= ~(FLAG_INV_UNUSABLE) ;
		else	TabInv[FLAG_ANNEAU_FOUDRE].FlagInv |= FLAG_INV_UNUSABLE ;

		// Sort de protection
		if( MagicPoint>0 )
			TabInv[FLAG_PROTECTION].FlagInv &= ~(FLAG_INV_UNUSABLE) ;
		else	TabInv[FLAG_PROTECTION].FlagInv |= FLAG_INV_UNUSABLE ;
	}
	else
	{
		TabInv[FLAG_BALLE_MAGIQUE].IdObj3D = 0 ;
		TabInv[FLAG_ANNEAU_FOUDRE].IdObj3D = 0 ;
		TabInv[FLAG_ANNEAU_FOUDRE].FlagInv |= FLAG_INV_UNUSABLE ;
		TabInv[FLAG_PROTECTION].FlagInv |= FLAG_INV_UNUSABLE ;
	}

	if( PingouinActif )
		TabInv[FLAG_MECA_PINGOUIN].FlagInv |= FLAG_INV_UNUSABLE ;
	else    TabInv[FLAG_MECA_PINGOUIN].FlagInv &= ~(FLAG_INV_UNUSABLE) ;

	if( TabInv[FLAG_PISTOLASER].IdObj3D!=2 )
		TabInv[FLAG_PISTOLASER].FlagInv |= FLAG_INV_UNUSABLE ;
	else    TabInv[FLAG_PISTOLASER].FlagInv &= ~(FLAG_INV_UNUSABLE) ;

	pt = TabInv	;

	for ( i = 0 ; i < MAX_BOX_INVENTORY ; i++ )
	{
		TabIndir[i] = FLAG_DONT_USE	;
	}


	for ( i = 0 ; i < MAX_INVENTORY ; i++, pt++ )
	{
#ifdef	LBA_EDITOR
		if ( ListVarGame[i].Value )
#else
		if ( ListVarGame[i] )
#endif
		{
			TabIndir[pt->Box] = (U8)i	;//	Table d'indirection
		}
	}
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#ifdef	DEBUG_TOOLS
S32	TricheInventory( S32 ret )
{
	U32	i	;

	if ( Key == K_A )//	for All objects
	{
		for ( i = 0 ; i < MAX_BOX_INVENTORY ; i++ )
		{
#ifdef	LBA_EDITOR
			ListVarGame[i].Value = 1	;
#else
			ListVarGame[i] = 1		;
#endif
		}

		InitTabIndir()		;//	NEW

		ListArdoise[0] = 5 	;
		ListArdoise[1] = 7 	;
		ListArdoise[2] = 9 	;

		NbArdoise = 3 ;

		TakeAllDarts() ;
#ifdef	LBA_EDITOR
		ListVarGame[FLAG_DART].Value = 3		;
		ListVarGame[FLAG_MECA_PINGOUIN].Value = 9	;
#else
		ListVarGame[FLAG_DART] = 3		;
		ListVarGame[FLAG_MECA_PINGOUIN] = 9	;
#endif
		ret = 1			;
	}

	if ( Key == K_F5 )	{ TabInv[TabIndir[InvSelect]].IdObj3D = 0 ; ret=1 ; }
	if ( Key == K_F6 )	{ TabInv[TabIndir[InvSelect]].IdObj3D = 1 ; ret=1 ; }
	if ( Key == K_F7 )	{ TabInv[TabIndir[InvSelect]].IdObj3D = 2 ; ret=1 ; }
	if ( Key == K_F8 )	{ TabInv[TabIndir[InvSelect]].IdObj3D = 3 ; ret=1 ; }

	return(ret)	;
}
#endif
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	DrawObjInventory( S32 x, S32 y, S16 *ptrobj, S32 beta, S32 lfactor, S32 zoom )
{
	SetProjection( x,y,128,lfactor,lfactor );

	SetFollowCamera( 0,0,0, (60*MUL_ANGLE),0,0, zoom ) 	;

	BodyDisplay(	0,0,0, 0,beta,0, ptrobj ) ;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*
void	AffMagicBar( S32 x0, S32 y0, S32 x1, S32 y1, S32 magie )
{
	S32	startx, starty	;
	S32	x, dx, dy	;

	dx = x1-x0		;
	dy = y1-y0		;

	startx = dx>>4		;	// 	1/16eme de border en X
	starty = dy>>4		;	//      1/16eme de border en y

	x = RegleTrois( x0+startx, x1-startx, MAX_MAGIC_LEVEL, MaxMagicLevel ) ;

	Rect( x0+startx, y1-starty-starty, x, y1-starty, LBAWHITE )		;

	x = RegleTrois( x0+startx, x1-startx, MAX_MAGIC_LEVEL, MyMagicLevel ) ;

	Box( x0+startx+1, y1-starty-starty+1, x, y1-starty-1, 75 )	;

	x = RegleTrois( x0+startx, x1-startx, MAX_MAGIC_LEVEL, magie );

	Line( x, y1-starty-starty-5, x, y1-starty+5, LBAWHITE )	;//Tempo
}
*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	AffValueBar( S32 x0, S32 y0, S32 x1, S32 y1, S32 value )
{
	S32	startx, starty	;
	S32	x, dx, dy	;

	dx = x1-x0		;
	dy = y1-y0		;

	startx = dx>>4		;	// 	1/16eme de border en X
	starty = dy>>4		;	//      1/16eme de border en y

	x = RegleTrois( x0+startx, x1-startx, INV_MAX_VALUE, value ) ;

	Box( x0+startx+1, y1-starty-starty+1, x, y1-starty-1, 73 ) ; // rouge

	Rect( x0+startx, y1-starty-starty, x1-startx, y1-starty, LBAWHITE )		;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
S32	S_X0, S_Y0, S_X1, S_Y1	;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	SaveBackground( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	S_X0 = x0	;
	S_Y0 = y0	;
	S_X1 = x1	;
	S_Y1 = y1	;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	RestoreBackground()
{
	CopyBlock( S_X0, S_Y0, S_X1, S_Y1, Screen, S_X0, S_Y0, Log )	   ;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
S32	XObj0, YObj0, XObj1, YObj1, LFactor, MemoLFactor	;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/


U32	DrawOneInventory( S32 numbox )
{
	S32	x, y, x0, y0, x1, y1 	;
	S16	*ptrobj 		;
	S32	coul=0, step		;
	S32	beta			;
	S32	flagshade = FALSE	;
	S32	numinv			;
	U32	flags = ALL_ANGLES	;

	numinv = TabIndir[numbox]	;

//----- Position de depart dans la grille

	x0 =	INV_START_X + INV_INTER_X
		+ (INV_SIZEO_X+INV_INTER_X) * (numbox/INV_NBOBJ_Y) ;
	y0 = 	INV_START_Y + INV_INTER_Y
		+ (INV_SIZEO_Y+INV_INTER_Y) * (numbox%INV_NBOBJ_Y) ;

	x1 = x0 + INV_SIZEO_X 	;
	y1 = y0 + INV_SIZEO_Y 	;

	LFactor = 200		;

//----- Zoom en cours...
	if ( FlagZoomed == 2 )	//	En cours
	{
		step = TimerRefHR-MemoTimerZoom	;

		if ( step >= TIME_ZOOM )	//	Bound
		{
			FlagZoomed = 3		;// Avoid size computing
		}
		else
		{
			x0 = RegleTrois( x0, INV_ZOOM_X0, TIME_ZOOM, step );
			y0 = RegleTrois( y0, INV_ZOOM_Y0, TIME_ZOOM, step );

			x1 = RegleTrois( x1, INV_ZOOM_X1, TIME_ZOOM, step );
			y1 = RegleTrois( y1, INV_ZOOM_Y1, TIME_ZOOM, step );

			LFactor = RegleTrois( 200, 580, TIME_ZOOM, step )  ;
		}
	}
//----- Explain Zoom retour a ta place...
	if ( FlagZoomed == 6 )
	{
		step = TimerRefHR-MemoTimerZoom	;

		if ( step > TIME_ZOOM )	//	Bound
		{
			FlagZoomed = 7		;
		}
		else
		{
			x0 = RegleTrois( XObj0, x0, TIME_ZOOM, step );
			y0 = RegleTrois( YObj0, y0, TIME_ZOOM, step );
			x1 = RegleTrois( XObj1, x1, TIME_ZOOM, step );
			y1 = RegleTrois( YObj1, y1, TIME_ZOOM, step );
			LFactor = RegleTrois( MemoLFactor, 200, TIME_ZOOM, step )  ;
		}
	}
//----- Explain Zoom en cours...
	if ( FlagZoomed == 4 )	//	En cours ou Stop
	{//	Compilo put Direct Value

		x0 =	INV_START_X + INV_INTER_X
			+ (INV_SIZEO_X+INV_INTER_X) * (15/INV_NBOBJ_Y) ;
		y0 = 	INV_START_Y + INV_INTER_Y
			+ (INV_SIZEO_Y+INV_INTER_Y) * (15%INV_NBOBJ_Y) ;

		x1 = x0 + INV_SIZEO_X 	;
		y1 = y0 + INV_SIZEO_Y 	;

		step = TimerRefHR-MemoTimerZoom	;

		if ( step > TIME_ZOOM )	//	Bound
		{
			XObj0 = x0		;
			YObj0 = y0		;
			XObj1 = x1		;
			YObj1 = y1		;
			MemoLFactor = LFactor	;
			FlagZoomed = 5		;
		}
		else
		{
			x0 = RegleTrois( XObj0, x0, TIME_ZOOM, step );
			y0 = RegleTrois( YObj0, y0, TIME_ZOOM, step );
			x1 = RegleTrois( XObj1, x1, TIME_ZOOM, step );
			y1 = RegleTrois( YObj1, y1, TIME_ZOOM, step );
			LFactor = RegleTrois( MemoLFactor, 200, TIME_ZOOM, step )  ;
		}

	}
	if ( FlagZoomed == 5 )	//	En cours ou Stop
	{//	Compilo put Direct Value

		x0 =	INV_START_X + INV_INTER_X
			+ (INV_SIZEO_X+INV_INTER_X) * (15/INV_NBOBJ_Y) ;
		y0 = 	INV_START_Y + INV_INTER_Y
			+ (INV_SIZEO_Y+INV_INTER_Y) * (15%INV_NBOBJ_Y) ;

		x1 = x0 + INV_SIZEO_X 	;
		y1 = y0 + INV_SIZEO_Y 	;
	}

//----- No else please...
	if ( FlagZoomed == 3 )	//	Zoomed fix
	{
		x0 = INV_ZOOM_X0	;
		y0 = INV_ZOOM_Y0	;
		x1 = INV_ZOOM_X1	;
		y1 = INV_ZOOM_Y1	;
		LFactor = 580		;
	}

	x = (x0+x1)>>1	;
	y = (y0+y1)>>1	;


	if( numbox == InvSelect )	coul = InvCoul		;

	if ( FlagZoomed >= 2 )
	{
		SaveBackground( x0, y0, x1, y1 )	;
	}

	SetClip( x0, y0, x1, y1 )	;

//	if( !flagshade )
	{
		DrawCadreBox( x0, y0, x1, y1, coul ) 	;
	}

#ifdef	LBA_EDITOR
	if( ListVarGame[numinv].Value )
#else
	if( ListVarGame[numinv] )
#endif
	{
		// Definit si la box est shad‚e      *
		if( (FlagZoomed < 4 ) AND (numinv!=FLAG_DONT_USE)	// protection
		AND (TabInv[numinv].FlagInv & FLAG_INV_UNUSABLE) )
		{
			flagshade = TRUE ;
		}

		ptrobj = (S16 *)GivePtrObjFix( TabInv[numinv].TabObj3D[TabInv[numinv].IdObj3D] ) 	;

		beta = TabInv[numinv].BetaObj + (TimerRefHR-TimerRot);

		DrawObjInventory( x, y, ptrobj, beta, LFactor, 15000 ) 	;

		if (( TabInv[numinv].FlagInv & FLAG_INV_FEWER )// Fuel, etc.
		AND ( FlagZoomed < 4 ))
		{
			S32	xt ;
			char	string[10] ;

			ColorFont( LBAWHITE ) ;
#ifdef	LBA_EDITOR
			strcpy( string, Itoa( ListVarGame[numinv].Value ) ) ;
#else
			strcpy( string, Itoa( ListVarGame[numinv] ) ) ;
#endif
			xt = x1-SizeFont(string) ;

			Font( xt,y1-31, string ) ;
			ColorFont( COUL_PERSO*16+12 )	;//	64+12
		}

/*		if ( TabInv[numinv].PtMagie )//	It's Magic !
		{
			AffMagicBar( x0, y0, x1, y1, TabInv[numinv].PtMagie);
		}
*/
		if (( TabInv[numinv].FlagInv & FLAG_INV_VALUE )
		AND ( FlagZoomed <4 ))
		{
			AffValueBar( x0, y0, x1, y1, TabInv[numinv].PtMagie);
		}

	}

	if( flagshade ) // Objet Inutilisable
	{
//		DrawCadreShade( x0, y0, x1, y1, SHADE_INVENT_LVL ) 	;

		// Affiche 1 sprite barrant l'objet inventaire
		AffGraph( 0, x0+(x1-x0)/2, y0+(y1-y0)/2, HQR_Get(HQRPtrSprite,SYS_SPRITE_INV) ) ;
	}

	if ( numinv == Weapon ) flags |= CADRE_RED 	;

	flags |= BACKUP_ANGLES ;

	DrawCadre( x0, y0, x1, y1, flags ) ;

	if (( numinv == Weapon ) AND ( FlagZoomed < 2 ))
	{
		SetClip( x0-3, y0-3, x1+3, y1+3 )		;

		DrawCadre( x0-0, y0-1, x1+0, y1+1, ALL_ANGLES|CADRE_RED ) ;
		DrawCadre( x0-1, y0-1, x1+1, y1+1, ALL_ANGLES|CADRE_RED ) ;
		DrawCadre( x0-2, y0-1, x1+2, y1+1, ALL_ANGLES|CADRE_RED ) ;
		DrawCadre( x0-2, y0-2, x1+2, y1+2, ALL_ANGLES|CADRE_RED ) ;
		DrawCadre( x0-3, y0-2, x1+3, y1+2, ALL_ANGLES|CADRE_RED ) ;
		DrawCadre( x0-3, y0-3, x1+3, y1+3, ALL_ANGLES|CADRE_RED ) ;

		DrawCadre( x0, y0, x1, y1, flags ) 		;

		x0 -= 3	;
		y0 -= 3	;
		x1 += 3	;
		y1 += 3 ;
	}
	else	DrawCadre( x0, y0, x1, y1, flags ) ;

	if ( FlagZoomed >= 2 )
	{
		if ( x0 < OldX0 )	OldX0 = x0	;
		if ( y0 < OldY0 )	OldY0 = y0	;
		if ( x1 > OldX1 )	OldX1 = x1	;
		if ( y1 > OldY1 )	OldY1 = y1	;
	}

	BoxStaticAdd( OldX0, OldY0, OldX1, OldY1 )	;
	BoxUpdate() ;

	if ( FlagZoomed >= 2 )
	{
		RestoreBackground()	;
	}

	OldX0 = x0		;
	OldY0 = y0		;
	OldX1 = x1		;
	OldY1 = y1		;

	UnsetClip()	;

#ifdef	LBA_EDITOR
	return( ListVarGame[numinv].Value )	;
#else
	return( ListVarGame[numinv] )	;
#endif
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	DrawInventoryScreen()
{
	S32	n	;

//----- Avoid Flash ( compilator put direct value )
	OldX0 =	INV_START_X + INV_INTER_X
		+ (INV_SIZEO_X+INV_INTER_X) * (0/INV_NBOBJ_Y) ;
	OldY0 = 	INV_START_Y + INV_INTER_Y
		+ (INV_SIZEO_Y+INV_INTER_Y) * (0%INV_NBOBJ_Y) ;

	OldX1 = OldX0 + INV_SIZEO_X 	;
	OldY1 = OldY0 + INV_SIZEO_Y 	;
//-----


	for( n=0; n<MAX_BOX_INVENTORY; n++ )
	{
		TimerRot = TimerRefHR	;

		DrawOneInventory( n ) 	;
	}

}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	OpenInventory()
{
	S32	x0, y0, x1, y1 	;
	S32	savetimer, step	;

	x0 = INV_START_X	;
	y0 = INV_START_Y	;
	x1 = INV_START_X+16	;
	y1 = INV_START_Y+16	;

	ManageTime() ;
	savetimer = TimerRefHR	;

	while( x1 != INV_END_X )// Boucle Optimisable Si Necessaire...
	{
		ManageTime() ;
		step = TimerRefHR-savetimer	;

		x1 = BoundRegleTrois( INV_START_X+16, INV_END_X, 15*20, step ) ;
		y1 = BoundRegleTrois( INV_START_Y+16, INV_END_Y, 15*20, step ) ;

		DrawCadre( x0, y0, x1, y1, ALL_ANGLES|BACKUP_ANGLES )	    ;

//------------- Display new position on Phys
		BoxMovingAdd( x0, y0, x1, y0+6 )	;
		BoxMovingAdd( x0, y1-6, x1, y1 )	;
		BoxMovingAdd( x0, y0+7, x0, y1-7 )	;
		BoxMovingAdd( x1, y0+7, x1, y1-7 )	;

		BoxUpdate() ;
	}

	CopyScreen( Screen, Log )	;

//	Shade All Inventory
	BackupAngles( INV_START_X,INV_START_Y, INV_END_X,INV_END_Y )  ;
	ShadeBoxBlk( INV_START_X+1,INV_START_Y+1, INV_END_X-1,INV_END_Y-1, SHADE_INVENT_LVL );
	DrawCadre( INV_START_X,INV_START_Y, INV_END_X,INV_END_Y, ALL_ANGLES ) 	   ;
	RestoreAngles( INV_START_X,INV_START_Y, INV_END_X,INV_END_Y )  ;

//	Shade Message
	BackupAngles( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1 )	;
	ShadeBoxBlk ( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1, SHADE_INVENT_LVL )	;
	DrawCadre( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1, ALL_ANGLES )	;
	RestoreAngles( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1 )	;

	CopyScreen( Log, Screen )	;// Before Draw
	CopyScreen( Log, BufSpeak )	;

	BoxStaticFullflip();

	DrawInventoryScreen()	;

	CopyScreen( Log, Screen )	;// After Draw
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	EmptyBlockInventory()
{
	S32	x0, y0, x1, y1	;


	x0 =	INV_START_X + INV_INTER_X
		+ (INV_SIZEO_X+INV_INTER_X) * (InvSelect/INV_NBOBJ_Y) ;
	y0 = 	INV_START_Y + INV_INTER_Y
		+ (INV_SIZEO_Y+INV_INTER_Y) * (InvSelect%INV_NBOBJ_Y) ;

	x1 = x0 + INV_SIZEO_X 	;
	y1 = y0 + INV_SIZEO_Y 	;

	if ( TabIndir[InvSelect] == Weapon )
	{
		x0 -= 3	;
		y0 -= 3	;
		x1 += 3	;
		y1 += 3 ;

		OldX0 = x0	;
		OldY0 = y0	;
		OldX1 = x1	;
		OldY1 = y1	;
	}

	CopyBlock( x0, y0, x1, y1, BufSpeak, x0, y0, Screen )	;

	CopyBlock( x0, y0, x1, y1, BufSpeak, x0, y0, Log )	;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	RestoreInventoryScreen()
{
	CopyScreen( BufSpeak, Log )	;
	CopyScreen( BufSpeak, Screen )	;

	DrawOneInventory(InvSelect)	;

	BoxStaticAdd( INV_START_X, INV_START_Y, INV_END_X, INV_END_Y )	;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	AffObjName( S32 num )
{
	S32	x, dx, t, i	;

	CopyBlock( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1,
		       BufSpeak, INV_NAME_X0, INV_NAME_Y0, Screen ) 	;
	CopyBlock( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1,
		       BufSpeak, INV_NAME_X0, INV_NAME_Y0, Log ) 	;

	if ( num != -1 )
	{
		i = TabIndir[num]				;// Indirection
		t = TabInv[i].TabObj3D[TabInv[i].IdObj3D]	;
		GetMultiText( 100+t, String )			;
		dx = SizeFont( String )				;
		x = ((INV_NAME_X0+8+INV_NAME_X1-8)/2)-dx/2	;
		ColorFont( BLACK )      ;
		Font( x+2, INV_NAME_Y0+2+4, String )		;
		ColorFont( COUL_PERSO*16+12)			;
		Font( x, INV_NAME_Y0+2, String )		;
	}

	BoxStaticAdd( INV_NAME_X0, INV_NAME_Y0, INV_NAME_X1, INV_NAME_Y1 );
	BoxUpdate() ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	CheckProtoPack( void )
{
	T_MENU_COMP *ptrmcomp = &ListMenuComp[ListComportement[C_PROTOPACK].Menu] ;

	// Magouille ProtoPack/SuperJetPack
	if( TabInv[FLAG_PROTOPACK].IdObj3D==1 )
	{
		ptrmcomp->NbComps = 2 ;	// On a le SuperJetPack
	}
	else
	{
		ptrmcomp->NbComps = 1 ;	// On n'a pas le SuperJetPack
	}
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
U32	NextBox = 0	;

void	MenuInventory( U32 tempo )
{
	S32	flag = 1, t, i 	;
	S32	flagname = 1 	;
	S32	oldselect 	;
	S32	memoalphalight, memobetalight ;
	U32	timerzoom	;
	U32	timerslide	;	// Demo Slide Show
	U32	myinput	= Input	;
	S32	flagproto = FALSE ;
	U8	memobulle	;

	// au cas ou l'on soit appel‚ pendant un eclair
	Palette( PtrPal ) ;

	if( Comportement==C_PROTOPACK
	OR  Comportement==C_JETPACK )
	{
#ifdef	LBA_EDITOR
		HQ_StopOneSample( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTOPACK) ) ;
#else
		HQ_StopOneSample( SAMPLE_PROTOPACK ) ;
#endif
		ListObjet[NUM_PERSO].SampleAlways = 0 ;

		// pour pouvoir restaurer anim marche protopack
#ifdef	LBA_EDITOR
		if( !stricmp(ListObjet[NUM_PERSO].StringAnim,GEN_ANIM_MARCHE) )
#else
		if( ListObjet[NUM_PERSO].GenAnim==GEN_ANIM_MARCHE )
#endif
		{
			flagproto = TRUE ;
		}
	}

	InitTabIndir()		;

	timerslide = TimerRefHR + tempo ;

	MemoClipWindow() 		;
	UnsetClipWindow() 		;

	SetPosCamera( 0,0,0 ) 	;
	SetAngleCamera( 0,0,0 ) ;

	FlagZoomed = 0		;
	timerzoom = TimerRefHR	;

	SaveCamera() ;

	memoalphalight = AlphaLight				;
	memobetalight = BetaLight 				;

	// Valeurs choisies arbitrairement
	SetLightVector( OBJ_ALPHA_LIGHT, OBJ_BETA_LIGHT, 0 ) 	;

	InvCoul = COUL_SELECT_MENU 	;

	OpenInventory() 		;	// draw all

	InitDial( 2 ) 			;	// game divers txt

	ColorFont( COUL_PERSO*16+12 )	;//	64+12

	// Init Textures
	PtrMap = ObjPtrMap = BufferTexture ;
	RepMask = 0xFFFF ;

	DoTextureAnimation() ;	// pour initialiser l'affaire

	InitWaitNoInput( I_INVENTORY|I_RETURN|I_MENUS ) ;// fait dans la boucle du jeu
	InitWaitNoKey() ;

	while( MyKey != K_ESC AND !(Input&I_MENUS)
	AND  ( !tempo OR TimerRefHR<=timerslide ) )
	{
		ManageTime() ;

#ifdef	DEBUG_TOOLS
		CheckSaveLogPcx( PtrPalNormal ) ;
#endif

		DoTextureAnimation() ;

		oldselect = InvSelect ;

		if( !flag )
		{
			MyGetInput() ;

			myinput = Input ;

			if( myinput&(I_JOY|I_FIRE) )
			{
				flag = 1 ;
			}
		}
		else
		{
			myinput = 0 ;
			MyGetInput() ;
			if( !(Input & (I_JOY|I_FIRE)) )	flag = 0 ;
		}

		if( (MyKey == K_ESC) OR (myinput&(I_INVENTORY|I_MENUS)) )
		{
			break ;
		}


		if (( FlagZoomed != 4 ) AND ( FlagZoomed != 6 ))
		{
			if( myinput & I_DOWN )
			{
				InvSelect++ 			;
				if( InvSelect >= MAX_BOX_INVENTORY )
					InvSelect = 0 		;
				flagname = 1 			;
			}

			if( myinput & I_UP )
			{
				InvSelect-- 			;
				if( InvSelect < 0 )
					InvSelect = MAX_BOX_INVENTORY - 1 ;
				flagname = 1			;
			}

			if( myinput & I_LEFT )
			{
				InvSelect -= INV_NBOBJ_Y	;
				if( InvSelect < 0 )
					InvSelect = MAX_BOX_INVENTORY + InvSelect ;
				flagname = 1			;
			}

			if( myinput & I_RIGHT )
			{
				InvSelect += INV_NBOBJ_Y	;
				if( InvSelect >= MAX_BOX_INVENTORY )
					InvSelect = InvSelect - MAX_BOX_INVENTORY ;
				flagname = 1			;
			}
		}
//-------------
		if (( Input & I_HELP )
		AND ( FlagZoomed < 4 ))
		{

			InitWaitNoInput( I_HELP ) ;
#ifdef	LBA_EDITOR
			if (ListVarGame[TabIndir[InvSelect]].Value <= 0)
#else
			if (ListVarGame[TabIndir[InvSelect]] <= 0)
#endif
			{
				PlayErrorSample() ;
			}
			else
			{

				RestoreInventoryScreen();
				XObj0 = OldX0		;
				YObj0 = OldY0		;
				XObj1 = OldX1		;
				YObj1 = OldY1		;
				MemoLFactor = LFactor	;

				FlagZoomed = 4			;// Go to left corner
				MemoTimerZoom = TimerRefHR	;
				timerzoom = TimerRefHR		;
			}
		}

		if ( FlagZoomed == 5 ) // Fix in up center
		{
			AffObjName(InvSelect)		;

			CopyScreen(ScreenAux, Screen)	;// Because Dial do a BackupScreen
			i = TabIndir[InvSelect]		;// Indirection
			t = TabInv[i].TabObj3D[TabInv[i].IdObj3D];

			NumObjDial = 0			;//	Twinsen
			BoxReset()			;
			memobulle = Bulle		;
			Bulle = 0			;
			Dial(200+t, 0)			;
			Bulle = memobulle		;
			CopyScreen( Log, Screen )	;
			MemoTimerZoom = TimerRefHR	;
			timerzoom = TimerRefHR		;
			FlagZoomed = 6			;// Return panier
		}

		if ( FlagZoomed == 7 ) // Fix at is own place
		{
			TabInv[TabIndir[InvSelect]].BetaObj += (TimerRefHR-TimerRot);
			TimerRot = TimerRefHR		;
			FlagZoomed = 0			;// for DrawInvScr()
			BoxReset()			;

			RestoreInventoryScreen()	;

			DrawInventoryScreen()		;

			CopyScreen( Log, Screen )	;
			MemoTimerZoom = TimerRefHR	;
			timerzoom = TimerRefHR		;
			flagname = 1			;
		}
//-------------
#ifdef	DEBUG_TOOLS
		flagname = TricheInventory( flagname )	;
#endif

		if( flagname )	//	New object selected
		{
			FlagZoomed = 0			;// Alt Zoom
			if ( oldselect != InvSelect )
				DrawOneInventory( oldselect ) 	;

			CopyScreen( Log, Screen )	;

			if ( oldselect != InvSelect )
			{
#ifdef	LBA_EDITOR
				if( ListVarGame[TabIndir[oldselect]].Value > 0)
#else
				if( ListVarGame[TabIndir[oldselect]] > 0)
#endif
				{
					TabInv[TabIndir[oldselect]].BetaObj += (TimerRefHR-TimerRot);
				}
			}
#ifdef	LBA_EDITOR
			if( (ListVarGame[TabIndir[InvSelect]].Value > 0) )
#else
			if( (ListVarGame[TabIndir[InvSelect]] > 0) )
#endif
			{
				AffObjName( InvSelect )		;

				if( !(TabInv[TabIndir[InvSelect]].FlagInv&FLAG_INV_UNUSABLE) )
				{
					timerzoom = TimerRefHR		;
					FlagZoomed = 1			;// Wait Zoom
				}
			}
			else
			{
				AffObjName( -1 )		;
			}

			flagname = 0 			;
			TimerRot = TimerRefHR		;
		}


		if (( FlagZoomed == 1 )	//	En Attente de zoom
		AND ( TimerRefHR > timerzoom+TIME_BEFORE_ZOOM ))
		{
			EmptyBlockInventory()	;
			FlagZoomed = 2		;//	Ok, Zoom Now!
			MemoTimerZoom = TimerRefHR;
		}

		DrawOneInventory( InvSelect )	;


		if( myinput & I_RETURN )
		{
#ifdef	LBA_EDITOR
			if( (ListVarGame[TabIndir[InvSelect]].Value > 0)
#else
			if( (ListVarGame[TabIndir[InvSelect]] > 0)
#endif
			AND !(TabInv[TabIndir[InvSelect]].FlagInv&FLAG_INV_UNUSABLE) )
			{
				InventoryAction = TabIndir[InvSelect] 	;

/*				if( InventoryAction==FLAG_MECA_PINGOUIN )
				{
					// peut etre remplac‚ par un test sur son body ?
					PingouinActif = TRUE ;
				}
*/
				InvCoul = 72			; // ex 91 rouge
				DrawOneInventory( InvSelect ) 	;

				break ;
			}
			else	PlayErrorSample() ;
		}
	}

	CloseDial() ;

	FlagZoomed = 0	;// Keep this place clean like you find it.... ;)

	AlphaLight = memoalphalight 	;
	BetaLight  = memobetalight 	;
	PtrInit3DView() 		;
	if( CubeMode==CUBE_INTERIEUR )	CameraCenter( 0 ) 		;

	RestoreClipWindow() 	;

	RestoreCamera() ;

	RestoreTimer() ;

	InitDial( START_FILE_ISLAND+Island ) 	;

	CheckProtoPack() ;

	if( flagproto )
	{
		// restore marche proto en ANIM_ALL_THEN pour que le
		// joueur ait le temps de se reprendre
		InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NUM_PERSO ) ;
		ObjectSetFrame( &(ListObjet[NUM_PERSO].Obj), 5 ) ;
		TimerProto = TimerRefHR+TEMPO_PROTO_AUTO ;// 1/2 s
	}

	InitWaitNoInput( I_INVENTORY|I_RETURN|I_MENUS ) ;// fait dans la boucle du jeu
	InitWaitNoKey() ;
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
    Ûßßßß ÛßßßÛ Û   Û ÛÛÜ Û ÛßßßÜ       ÛßßßÛ ÛßßÛ      Û Ûßßßß Ûßßßß ßßÛßß
    ÛÛßß  ÛÛ  Û ÛÛ  Û ÛÛßÛÛ ÛÛ  Û       ÛÛ  Û ÛÛßßÛ ÜÜ  Û ÛÛßß  ÛÛ      ÛÛ
    ßß    ßßßßß ßßßßß ßß  ß ßßßß  ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß   ßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	FOUND_X0	256
#define	FOUND_Y0	48
#define	FOUND_X1	384
#define	FOUND_Y1	178
#define	FOUND_XO	((256+384)/2)
#define	FOUND_YO	((48+178)/2)

extern	S32	Dial_Y0	;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	DoFoundObj( S32 numvar )
{
#ifndef	COMPILATOR
	S16	*ptr3do 	;
	T_OBJET	*ptrobj 	;
	S32	x0, y0, x1, y1 ;
	S32	dialstat 	;
	S32	flag 		;
	S32	numanim 	;
/*
#ifdef	CDROM
	S32	memoflagspeak 	;
	S32	speakfromcd  	;
#endif
*/
	S32	sb		;
	U32	savetimer, organi;
	S32	end		;
#ifdef	LBA_EDITOR
	char	genanim[50]	;
#else
	U8	genanim	;
#endif
	U32	workflag	;
	S32	xo, yo, step	;
	S32	zoom	;
	S32	endtimer ;
	S32	keypress = FALSE ;
	S32	keyreleased = FALSE ;
	S32	numobj 		;
	S32	saveyclip 	;
	U8	memobulle = Bulle ;
//	U8	esc = FALSE ;

#ifndef	LBA_EDITOR
	ClearIncrusts( INCRUST_SYS_TEXT ) ;
	ClearIncrusts( INCRUST_TEXT ) ;
#endif

	BoxClean() ;// Clean Log from last AffScene

	Bulle = FALSE ;

	numobj = TabInv[numvar].TabObj3D[ TabInv[numvar].IdObj3D ] ;

	ptrobj = &ListObjet[NUM_PERSO] ;

#ifdef	LBA_EDITOR
	VueDistance = DEFAULT_DISTANCE_CAM ;
#else
	VueDistance = DefVueDistance[0] ;
#endif

	StartXCube = ((ptrobj->Obj.X)/SIZE_BRICK_XZ)		;
	StartYCube = ((ptrobj->Obj.Y+SIZE_BRICK_Y)/SIZE_BRICK_Y);
	StartZCube = ((ptrobj->Obj.Z)/SIZE_BRICK_XZ)		;

	CameraCenter( 0 ) 	;

	AffScene(AFF_ALL_NO_FLIP)	;// Centring Hero
	FixeCinemaMode(FALSE) 		;// Scrolle les bandes noires

	CopyScreen( Screen, ScreenAux ) ;
//	CopyScreen( Log, Screen ) ;

//	BoxUpdate() ;
	BoxBlit()  ;
	BoxReset() ;

//	BackupScreen( FALSE )	;

	xo = x0 = x1 = FOUND_XO	;
	yo = y0 = y1 = FOUND_YO	;

#ifdef	LBA_EDITOR
	HQ_MixSample( COMMON_NUM_DEF, SAMPLE_FOUND_OBJ, 0x1000, 0, 1, 64, 127 ) ;
#else
	HQ_MixSample( SAMPLE_FOUND_OBJ, 0x1000, 0, 1, 64, 127 ) ;
#endif

	FlagAnimWhoSpeak = TRUE ;	// magouille pour le CinemaMode

	// aff text et objet
/*
#ifdef	CDROM
	if( IsVoiceFileOnHD( 2 ) )// FR_GAM.VOX on HD ?
	{
		// voix sur hd touche … rien

		InitDial( 2 ) ;	// game divers txt
		speakfromcd = FALSE ;

	}
	else	// coupe music cd si on
	{
		StopMusic() ;
		memoflagspeak = FlagSpeak ;
		FlagSpeak = FALSE ;

		InitDial( 2 ) ;

		FlagSpeak = memoflagspeak ;
		speakfromcd = TRUE ;
	}

#else
*/
	InitDial( 2 ) ;	// game divers txt
//#endif

	UnsetClip() 			;

	if( GetText( numobj ) )	GereFlagDial(numobj) 	;

	if ( FlagDisplayText )	OpenDial( numobj )	;

	dialstat = FlagDisplayText	;// 0 or 1, Still or Run

	flag = 0			;

	TestCoulDial( COUL_PERSO )	;	// coul twinsen

#ifdef	CDROM
 /*	if( speakfromcd )
	{
		SpeakFromCD( 2, numobj ) ;
	}
	else
	{
*/
		Speak( numobj ) ;
//	}
#endif

	ptr3do = (S16 *)GivePtrObjFix( numobj ) ;

#ifdef	LBA_EDITOR
	strcpy( genanim, GEN_ANIM_GAUCHE ) ;
#else
	genanim = GEN_ANIM_GAUCHE;
#endif

	if( CubeMode == CUBE_INTERIEUR )
	{
		if ( ptrobj->Obj.Beta > 2560 )	end = 512+4096	;
		else
		{
			end = 512			;
			if ( ptrobj->Obj.Beta > 512 )
#ifdef	LBA_EDITOR
				strcpy( genanim, GEN_ANIM_DROITE ) ;
#else
				genanim = GEN_ANIM_DROITE;
#endif
		}
	}
	else
	{
		S32	diff ;

		end = GetAngle2D( ptrobj->Obj.X, ptrobj->Obj.Z,
				CameraX, CameraZ ) ;

		diff = (ptrobj->Obj.Beta + 4096 - (end+4096)) & (MAX_ANGLE-1) ;

		if( diff > 2048 )
		{
			if( ptrobj->Obj.Beta>end )	end += 4096	;
		}
		else
		{
			if( ptrobj->Obj.Beta<end )	end -= 4096	;
#ifdef	LBA_EDITOR
			strcpy( genanim, GEN_ANIM_DROITE ) ;
#else
			genanim = GEN_ANIM_DROITE;
#endif
		}
	}

	organi = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO )		;//ptrobj->Obj.Anim;
	numanim = SearchAnim( genanim, NUM_PERSO ) 		;
	ObjectInitAnim( &ptrobj->Obj, (void*)numanim )		;
	numanim = SearchAnim( GEN_ANIM_TROUVE, NUM_PERSO ) 	;

	if( numanim==-1 )	numanim = organi ;	// Anim Rien

	sb = ptrobj->Obj.Beta		;

	// Init Textures
	PtrMap = ObjPtrMap = BufferTexture ;
	RepMask = 0xFFFF ;

//	FlagNoFlip = 1			;// No Flip in AffScene()

	DialNbLine = 0 ;

	savetimer = TimerRefHR		;
	endtimer = TimerRefHR + 4*1000 ;

	workflag = ptrobj->WorkFlags		;
	ptrobj->WorkFlags &= ~MANUAL_INTER_FRAME;

	while( flag != 2 )
	{
		ManageTime() ;
		DoTextureAnimation() ;

//		ModeDesiredY = Dial_Y0	;// Nothing under Text Window
		saveyclip = ClipWindowYMax	;
//		ClipWindowYMax = ModeDesiredY-1	;

		if( FlagDisplayText )
			SetClipWindow( ClipWindowXMin, ClipWindowYMin, ClipWindowXMax, Dial_Y0-1 ) ;
		else	SetClipWindow( ClipWindowXMin, ClipWindowYMin, ClipWindowXMax, ClipWindowYMax ) ;

		UnsetClip() 		;// Tiens compte du ModeDesiredY

		AffScene( AFF_OBJETS_NO_FLIP )			;// With Boxes & no BoxUpdate

/*		ModeDesiredY = 480 ;// To gain some time...
		ClipWindowYMax = saveyclip	;
*/
		SetClipWindow( ClipWindowXMin, ClipWindowYMin, ClipWindowXMax, saveyclip ) ;

		step = TimerRefHR-savetimer	;

//------------- Message

		UnsetClip() ;

		if( dialstat == 1 )
		{
			dialstat = NextDialCar()	;
			if( dialstat==2 )	endtimer = TimerRefHR+DialNbLine*1000 ;
		}

//------------- Founding Object

		if ( !flag )
		{
			x0 = BoundRegleTrois( xo, FOUND_X0, 20*20, step )	;
			y0 = BoundRegleTrois( yo, FOUND_Y0, 20*20, step )	;
			x1 = BoundRegleTrois( xo, FOUND_X1, 20*20, step )	;
			y1 = BoundRegleTrois( yo, FOUND_Y1, 20*20, step )	;
			zoom = BoundRegleTrois( 1, 200, 20*20, step )	;
		}
		else
		{
			x0 = BoundRegleTrois( FOUND_X0, 16, 15*20, step )	;
			y0 = BoundRegleTrois( FOUND_Y0, 16, 15*20, step )	;
			x1 = BoundRegleTrois( FOUND_X1, 16, 15*20, step )	;
			y1 = BoundRegleTrois( FOUND_Y1, 16, 15*20, step )	;
			zoom = BoundRegleTrois( 200, 1, 15*20, step )	;
		}

		xo = (x0+x1)/2	;
		yo = (y0+y1)/2	;

		BackupAngles( x0, y0, x1, y1 ) 	;
		ShadeBoxBlk( x0, y0, x1, y1, SHADE_INVENT_LVL )	;
		SetClip(  x0, y0, x1, y1 ) 	;

		// Valeurs choisies arbitrairement
		SaveCamera() ;
		SetLightVector( OBJ_ALPHA_LIGHT, OBJ_BETA_LIGHT, 0 ) 	;

		DrawObjInventory( xo, yo, ptr3do,
			      TimerRefHR*40/20, zoom, 10000 ) ;

		RestoreCamera() ;
		SetLightVector( AlphaLight, BetaLight, 0 ) 	;

		DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) 	;
		RestoreAngles( x0, y0, x1, y1 ) ;
		BoxMovingAdd( x0, y0, x1, y1 )	;


		PtrInit3DView() 	;
		CameraCenter( 200 ) 	;// 200: parametre inutilis‚

//------------- Our Hero!

		ObjectSetInterDep( &ptrobj->Obj );

		if ( !flag )
		{
			ptrobj->Obj.Beta = BoundRegleTrois( sb, end, 25*20, TimerRefHR-savetimer );
			if ( ptrobj->Obj.Beta == end)
				ObjectInitAnim( &ptrobj->Obj, (void*)numanim )			;
		}
		else
		{
			ptrobj->Obj.Beta = BoundRegleTrois( end, sb, 20*20, TimerRefHR-savetimer )	;
			if ( ptrobj->Obj.Beta == sb )
				flag = 2	;
		}

//-------------

		UnsetClip()			;

		BoxUpdate() 	;

		MyGetInput()	;

		if( !DemoSlide AND (MyKey==K_ESC OR Input&I_MENUS) AND !flag )
		{
//			esc  = TRUE 				;
			StopSpeak() 				;
			flag = 1				;
			savetimer = TimerRefHR			;
			ObjectInitAnim( &ptrobj->Obj, (void*)organi )	;
		}

		if( DemoSlide )
		{
			if( TimerRefHR>endtimer AND !TestSpeak() )	keypress = TRUE ;
		}

//		else
		{
			if( Input OR MyKey )
			{
				if( keyreleased )	keypress = TRUE ;
			}
			else	keyreleased = TRUE ;
		}

		if( keypress )
		{
			if (( dialstat == 0 ) AND ( !flag ))
			{
//				esc  = TRUE 				;
				StopSpeak() 				;
				flag = 1				;
				savetimer = TimerRefHR			;
				ObjectInitAnim( &ptrobj->Obj, (void*)organi )	;
//				InitWaitNoInput( I_ACTION | I_MENUS ) ;
//				InitWaitNoKey() ;
			}
			if ( dialstat == 2 )
			{
				endtimer = TimerRefHR + DialNbLine*1000 ;
				keypress = FALSE ;
				DialNbLine = 0 ;
				dialstat = 1 ;	// continue
				keyreleased = FALSE ;
			}
		}

	}

	ptrobj->WorkFlags = workflag	;

//	FlagNoFlip = 0			;

#ifdef	CDROM
/*	if( speakfromcd )
	{
		while( TestSpeakFromCD() AND (MyKey != K_ESC AND !(Input&I_MENUS)) ) MyGetInput() ;
	}
	else
	{
*/
//		if( !esc )
		{
			while( TestSpeak() AND (MyKey != K_ESC AND !(Input&I_MENUS)) )  MyGetInput() ;
		}
//	}
	StopSpeak() ;
#endif

	InitWaitNoInput( I_ACTION | I_MENUS ) ;
	InitWaitNoKey() ;


//	PtrInit3DView() ;
//	CameraCenter( 0 ) ;

	RestoreScreen()	;

	InitDial( START_FILE_ISLAND+Island ) ;

	FlagAnimWhoSpeak = FALSE ;	// magouille pour le CinemaMode

	// Traitement special pour certains objets
	switch( numobj )
	{
		case FLAG_DART:	// Flechettes
			TakeAllDarts( ) ;

		case FLAG_BALLE_MAGIQUE:
		case FLAG_SARBACANE:
		case FLAG_GANT:
#ifdef	LBA_EDITOR
			if( ListVarGame[Weapon].Value==0 )
#else
			if( ListVarGame[Weapon]==0 )
#endif
			{
				Weapon = (U8)numobj ;
			}
			break ;

		case FLAG_PROTOPACK:
			CheckProtoPack() ;
			break ;
	}
#endif	// COMPILATOR

	Bulle = memobulle ;
	BoxReset() ;
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	    ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛ¿  ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿
	    ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ¿ ÛÛÚÄÛÛ³ ÀÄÛÛÚÄÙ ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ
	    ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³ ÛÛ³ ÛÛ³ ÛÛ³ ÛÛ³   ÛÛ³   ÛÛÛÛÛÛ¿ ÛÛÛÛÛ¿
	    ÛÛÚÄÛÛ³ ÛÛÚÛÛÚÙ ÛÛ³ ÛÛ³ ÛÛ³ ÛÛ³   ÛÛ³   ÀÄÄÄÛÛ³ ÛÛÚÄÄÙ
	    ÛÛ³ ÛÛ³ ÛÛ³ÀÛÛ¿ ÛÛÛÛÛÚÙ ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ¿
	    ÀÄÙ ÀÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÙ  ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#ifndef	COMPILATOR

// Coordonnees de la boite de la fleche gauche
#define	L_X0	78
#define	L_Y0	436
#define	L_X1	(L_X0+50)
#define	L_Y1	(L_Y0+29)

// Coordonnees de la boite de la fleche droite
#define	R_X0	495
#define	R_Y0	436
#define	R_X1	(R_X0+50)
#define	R_Y1	(R_Y0+29)

// Coordonnees de la boite des plans se trouvant dans l'ardoise
#define	PLAN_X0	78
#define	PLAN_Y0	61
#define	PLAN_X1	(PLAN_X0+479)
#define	PLAN_Y1	(PLAN_Y0+360)

#define	ARROW_UP	0
#define	ARROW_DN	2

//#define	DrawLeftArdoiseArrow(on)	AffGraph( 0, L_X0, L_Y0, HQR_Get(HQRPtrSprite,SPRITE_ARDOISE_LEFT_UP+on) )
//#define	DrawRightArdoiseArrow(on)	AffGraph( 0, R_X0, R_Y0, HQR_Get(HQRPtrSprite,SPRITE_ARDOISE_RIGHT_UP+on) )

#define	DrawLeftArdoiseArrow(on)	PtrAffGraph( L_X0, L_Y0, SPRITE_ARDOISE_LEFT_UP+on )
#define	DrawRightArdoiseArrow(on)	PtrAffGraph( R_X0, R_Y0, SPRITE_ARDOISE_RIGHT_UP+on )

void	DrawArdoiseArrows( )
{
	if( CurrentArdoise>0 )
	{
		DrawLeftArdoiseArrow( ARROW_UP ) ;
	}
//	else	RestoreBlock( Log, BufSpeak, L_X0, L_Y0, L_X1, L_Y1 ) ;
	else	CopyBlock( L_X0, L_Y0, L_X1, L_Y1, BufSpeak, L_X0, L_Y0, Log ) ;

	BoxStaticAdd( L_X0, L_Y0, L_X1, L_Y1 ) ;

	if( CurrentArdoise<NbArdoise-1 )
	{
		DrawRightArdoiseArrow( ARROW_UP ) ;
	}
//	else	RestoreBlock( Log, BufSpeak+5000, R_X0, R_Y0, R_X1, R_Y1 ) ;
	else	CopyBlock( R_X0, R_Y0, R_X1, R_Y1, BufSpeak, R_X0, R_Y0, Log ) ;

	BoxStaticAdd( R_X0, R_Y0, R_X1, R_Y1 ) ;
	BoxBlit() ;
	BoxReset() ;
}

//***************************************************************************
#define	ROULEAU_WIDTH	48

#define	MI_SHADE	(ROULEAU_WIDTH/4)
#define	TI_SHADE	((ROULEAU_WIDTH-MI_SHADE)/2)

void    RouleauRight()
{
	S32     x, xold,l;
	S32     min;
	S32	dep_timer ;
	S32	col ;
	S32	time = 2*1000 ;
	S32	larg = PLAN_X1-PLAN_X0+ROULEAU_WIDTH-1 ;
	S32	start = PLAN_X1+ROULEAU_WIDTH-1 ;
	S32	key = FALSE ;
	S32	cshade ;
	S32	shade[ROULEAU_WIDTH] ;

	MemoClip() ;
	SetClip( PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1 ) ;

//	CopyScreen( Log, Screen ) ;

	for( x=0; x<TI_SHADE; x++ )  shade[x]        = 12-(x*12/(TI_SHADE-1));
	for( x=0; x<MI_SHADE; x++ )  shade[TI_SHADE+x]     = 0;
	for( x=0; x<TI_SHADE; x++ )  shade[ROULEAU_WIDTH-TI_SHADE+x]  = x*12/(TI_SHADE-1);

	x = start ;
	xold = PLAN_X1 ;

	ManageTime() ;
	dep_timer = TimerRefHR;

	while( x > PLAN_X0 )
	{
		ManageTime() ;
		x = start - ( (TimerRefHR-dep_timer) * (larg) / time );

		if(     ( x > PLAN_X0 )
		AND     ( x != xold )   )
		{
			min = x - ROULEAU_WIDTH + 1;
			if( min < PLAN_X0 ) min = PLAN_X0;

			if( x > PLAN_X1 )
			{
				x = PLAN_X1 ;
			}

			cshade = ROULEAU_WIDTH-(x-min)-1 ;

			for( l=min; l<=x; l++ )
			{
				MyGetInput() ;

				col = min-(l-min) ;

				if( !key AND (MyKey OR Input) )
				{
					if( x>(PLAN_X0+(PLAN_X1-PLAN_X0)/10 ) )
						time = 10 ;
					else	time = 5 ;

					start = x ;
					larg = x-PLAN_X0 ;
					dep_timer = TimerRefHR ;
					key = TRUE ;
					InitWaitNoInput( Input ) ;
					InitWaitNoKey() ;
				}

				if( col>=PLAN_X0 )
				{
					CopyBlockShade( col, PLAN_Y0,
						   col, PLAN_Y1, (U8*)Screen,
						   l, PLAN_Y0, (U8*)Log,
						   shade[cshade] );

				}
				else	Line( l, PLAN_Y0, l, PLAN_Y1, 48 ) ;

				cshade++ ;
			}

			if( x!=xold )
			{
				CopyBlock( x+1, PLAN_Y0, xold, PLAN_Y1, Screen,
					   x+1, PLAN_Y0, Log );
			}

			BoxStaticAdd( min, PLAN_Y0, xold, PLAN_Y1 );
			BoxBlit() ;
			BoxReset() ;

			xold = x;
		}
	}

	// s'assure de copier la derniere portion
	if( xold>x )
	{
		CopyBlockIncrust( PLAN_X0, PLAN_Y0, xold, PLAN_Y1, Screen,
				  PLAN_X0, PLAN_Y0, Log ) ;
		BoxStaticAdd( PLAN_X0, PLAN_Y0, xold, PLAN_Y1 ) ;
		BoxBlit() ;
		BoxReset() ;
	}

	RestoreClip();
}

void    RouleauLeft()
{
	S32     x, xold,l;
	S32     max;
	S32	dep_timer ;
	S32	col ;
	S32	cshade ;
	S32	time = 2*1000 ;
	S32     shade[ROULEAU_WIDTH];
	S32	start = PLAN_X0-ROULEAU_WIDTH+1 ;
	S32	larg = PLAN_X1-PLAN_X0 ;
	S32	key = 0 ;

	MemoClip() ;
	SetClip( PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1 ) ;

//	CopyScreen( Log, Screen ) ;

	for( x=0; x<TI_SHADE; x++ )  shade[x]        = 12-(x*12/(TI_SHADE-1));
	for( x=0; x<MI_SHADE; x++ )  shade[TI_SHADE+x]     = 0;
	for( x=0; x<TI_SHADE; x++ )  shade[ROULEAU_WIDTH-TI_SHADE+x]  = x*12/(TI_SHADE-1);

	x = start ;
	xold = PLAN_X0 ;

	ManageTime() ;
	dep_timer = TimerRefHR;

	while( x < PLAN_X1 )
	{
		ManageTime() ;
		x = start + ( (TimerRefHR-dep_timer) * larg / time );

		if(     ( x < PLAN_X1 )
		AND     ( x != xold ) )
		{
			max = x + ROULEAU_WIDTH - 1 ;
			if( max > PLAN_X1 ) max = PLAN_X1;

			if( x < PLAN_X0 )
			{
				x = PLAN_X0 ;
			}

			cshade = ROULEAU_WIDTH-(max-x)-1;

			for( l=x; l<=max; l++ )
			{
				MyGetInput() ;

				col = max+(max-l) ;

				if( !key AND (MyKey OR Input) )
				{
					if( x<(PLAN_X1-(PLAN_X1-PLAN_X0)/10 ) )
						time = 10 ;
					else	time = 5 ;

					start = x ;
					larg = PLAN_X1-x ;
					dep_timer = TimerRefHR ;
					key = TRUE ;
					InitWaitNoInput( Input ) ;
					InitWaitNoKey() ;
				}

				if( col<=PLAN_X1 )
				{
					CopyBlockShade( col, PLAN_Y0,
						   col, PLAN_Y1, (U8*)Screen,
						   l, PLAN_Y0, (U8*)Log,
						   shade[cshade] ) ;
				}
				else	Line( l, PLAN_Y0, l, PLAN_Y1, 48 ) ;

				cshade++ ;
			}

			if( x!=xold )
			{
				CopyBlock( xold, PLAN_Y0, x-1, PLAN_Y1, Screen,
						  xold, PLAN_Y0, Log );
			}

			BoxStaticAdd( xold, PLAN_Y0, max, PLAN_Y1 );
			BoxBlit() ;
			BoxReset() ;

			xold = x;
		}
	}

	// s'assure de copier la derniere portion
	if( xold<x )
	{
		CopyBlockIncrust( xold, PLAN_Y0, PLAN_X1, PLAN_Y1, Screen,
				  xold, PLAN_Y0, Log ) ;
		BoxStaticAdd( xold, PLAN_Y0, PLAN_X1, PLAN_Y1 ) ;
		BoxBlit() ;
		BoxReset() ;
	}

	RestoreClip();
}

void	ChangeArdoise( S32 left )	// 0:right, 1:left
{
	S32 savetimer ;

	// Chargement pcx
	if( !Load_HQR( SCREEN_HQR_NAME, Screen, ListArdoise[CurrentArdoise]*2 ) )// *2 pour compter les palettes
		TheEndCheckFile( SCREEN_HQR_NAME ) ;

	CopyBlock( PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1, BufSpeak,
		   PLAN_X0, PLAN_Y0, Log ) ;//	Restore Ardoise

	CopyBlockIncrust( PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1, Screen,
			  PLAN_X0, PLAN_Y0, Log ) ;//	Add New Plan

	CopyScreen( Log, Screen ) ;

	CopyBlock( PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1, BufSpeak,
		   PLAN_X0, PLAN_Y0, Log ) ;//	Restore Ardoise

	savetimer = TimerRefHR ;

	if( !left )
	{
		RouleauRight() ;
	}
	else
	{
		RouleauLeft() ;
	}
}

void	DrawArdoise( )
{
	if( !Load_HQR( SCREEN_HQR_NAME, Log, PCR_ARDOISE ) )
		TheEndCheckFile( SCREEN_HQR_NAME ) ;

	CopyScreen( Log, BufSpeak )	;//	Ardoise Vierge
/*
	SaveBlock( Log, BufSpeak, L_X0, L_Y0, L_X1, L_Y1 );
	SaveBlock( Log, BufSpeak+5000, R_X0, R_Y0, R_X1, R_Y1 );
	SaveBlock( Log, BufSpeak+10000, PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1 ) ;
*/
	if( CurrentArdoise>=0 AND CurrentArdoise<NbArdoise )
	{
		// Chargement pcx
		if( !Load_HQR( SCREEN_HQR_NAME, Screen, ListArdoise[CurrentArdoise]*2 ) )// *2 pour compter les palettes
			TheEndCheckFile( SCREEN_HQR_NAME ) ;

		CopyBlockIncrust( PLAN_X0, PLAN_Y0, PLAN_X1, PLAN_Y1, Screen,
				  PLAN_X0, PLAN_Y0, Log ) ;
	}

	FadeToBlackAndSamples( PtrPal ) ;

	if( CurrentArdoise>0 )			DrawLeftArdoiseArrow( ARROW_UP ) ;
	if( CurrentArdoise<NbArdoise-1 )	DrawRightArdoiseArrow( ARROW_UP ) ;

	BoxStaticFullflip();
	FadeToPal( PtrPalNormal ) ;
}

void	ResetArdoise( void )
{
	S32	n ;

	for( n=0; n<MAX_ARDOISE; n++ )
	{
		ListArdoise[n] = -1 ;
	}

	CurrentArdoise = NbArdoise = 0 ;
}
#endif	// COMPILATOR

void	GereArdoise()
{
#ifndef	COMPILATOR
	S32	savetimer ;

	InitWaitNoInput( ALL_INPUTS ) ;	// fait dans la boucle
	InitWaitNoKey() ;

	DrawArdoise( ) ;

	BoxReset() ;

	while( !(Input&(I_RETURN|I_ACTION|I_MENUS)) AND Key!=K_ESC )
	{
		ManageTime() ;
		MyGetInput() ;

		if( Input&I_LEFT AND CurrentArdoise>0 )
		{
			savetimer = TimerRefHR ;

			DrawLeftArdoiseArrow( ARROW_DN ) ;
			BoxStaticAdd( L_X0, L_Y0, L_X1, L_Y1 ) ;
			BoxBlit() ;
			BoxReset() ;

			while( TimerRefHR<savetimer+10*20 ) ManageTime() ;

			InitWaitNoInput( Input ) ;

			CurrentArdoise-- ;
			DrawArdoiseArrows() ;
			ChangeArdoise( 1 ) ;
		}

		if( Input&I_RIGHT AND CurrentArdoise<NbArdoise-1 )
		{
			savetimer = TimerRefHR ;

			DrawRightArdoiseArrow( ARROW_DN ) ;
			BoxStaticAdd( R_X0, R_Y0, R_X1, R_Y1 ) ;
			BoxBlit() ;
			BoxReset() ;

			while( TimerRefHR<savetimer+10*20 ) ManageTime() ;

			InitWaitNoInput( Input ) ;

			CurrentArdoise++ ;
			DrawArdoiseArrows() ;
			ChangeArdoise( 0 ) ;
		}

		BoxUpdate()	;
	}

	BoxReset() ;
	FadeToBlack( PtrPalNormal ) ;
	FlagFade = TRUE	;
#endif	// COMPILATOR
}


/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛ¿  ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿
		ÛÛÚÄÄÄÙ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ¿ ÛÛÚÄÛÛ³ ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ
		ÛÛ³     ÛÛÛÛÛÛ³ ÛÛ³ ÛÛ³ ÛÛÛÛÛÛ³ ÛÛÛÛÛ¿  ÛÛÛÛÛÛ¿
		ÛÛ³     ÛÛÚÄÛÛ³ ÛÛ³ ÛÛ³ ÛÛÚÛÛÚÙ ÛÛÚÄÄÙ  ÀÄÄÄÛÛ³
		ÛÛÛÛÛÛ¿ ÛÛ³ ÛÛ³ ÛÛÛÛÛÚÙ ÛÛ³ÀÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ³
		ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÙ  ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

// On se place sur la RECOVER_AREA de ScreenAux car on a besoin de 48 octets (4*12)
#define	PtrBackupAngles	(ScreenAux+640*480)

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void	BackupAngles( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	U8	*dest = PtrBackupAngles ;
	U8	*src ;

//----- Angle Superieur Gauche
	src = (U8*)Log + TabOffLine[y0] + x0 ;

	*(U32*)dest = *(U32*)src;		// 1ere ligne
	dest[4] = src[4] 	;

	dest[5] = src[640]		;	// 2nd ligne
	*(S16*)(dest+6) = *(S16*)(src+640+1);

	*(S16*)(dest+8) = *(S16*)(src+640*2);	// 3eme ligne

	dest[10] = src[640*3] ;			// 4eme ligne
	dest[11] = src[640*4] ;			// 5eme ligne

	dest += 12 ;

//----- Angle Inferieur Gauche
	src = (U8*)Log + TabOffLine[y1-4] + x0 ;

	dest[0] = src[0]   ;			// 1ere ligne
	dest[1] = src[640] ;			// 2eme ligne

	*(S16*)(dest+2) = *(S16*)(src+640*2);	// 3eme ligne

	*(S16*)(dest+4) = *(S16*)(src+640*3);	// 4eme ligne
	dest[6] = src[640*3+2]		;

	dest[7] = src[640*4] ;			// 5eme ligne
	*(U32*)(dest+8) = *(U32*)(src+640*4+1);

	dest += 12 ;

//----- Angle Superieur Droit
	src = (U8*)Log + TabOffLine[y0] + x1 ;

	*(U32*)dest = *(U32*)(src-4);		// 1ere ligne
	dest[4] = src[0] 	;

	dest[5] = src[640-2]		;	// 2nd ligne
	*(S16*)(dest+6) = *(S16*)(src+640-1);

	*(S16*)(dest+8) = *(S16*)(src+640*2-1);	// 3eme ligne

	dest[10] = src[640*3] ;			// 4eme ligne
	dest[11] = src[640*4] ;			// 5eme ligne

	dest += 12 ;

//----- Angle Inferieur Droit
	src = (U8*)Log + TabOffLine[y1-4] + x1 ;

	dest[0] = src[0]   ;			// 1ere ligne
	dest[1] = src[640] ;			// 2eme ligne

	*(S16*)(dest+2) = *(S16*)(src+640*2-1);	// 3eme ligne

	*(S16*)(dest+4) = *(S16*)(src+640*3-1);	// 4eme ligne
	dest[6] = src[640*3-2]		;

	dest[7] = src[640*4-4] ;		// 5eme ligne
	*(U32*)(dest+8) = *(U32*)(src+640*4-3);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void	RestoreAngles( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	U8	*src = PtrBackupAngles ;
	U8	*dest ;

//----- Angle Superieur Gauche
	dest = (U8*)Log + TabOffLine[y0] + x0 ;

	*(U32*)dest = *(U32*)src;		// 1ere ligne
	dest[4] = src[4] 	;

	dest[640] = src[5]		;	// 2nd ligne
	*(S16*)(dest+640+1) = *(S16*)(src+6);

	*(S16*)(dest+640*2) = *(S16*)(src+8);	// 3eme ligne

	dest[640*3] = src[10] ;			// 4eme ligne
	dest[640*4] = src[11] ;			// 5eme ligne

	src += 12 ;

//----- Angle Inferieur Gauche
	dest = (U8*)Log + TabOffLine[y1-4] + x0 ;

	dest[0] = src[0]   ;			// 1ere ligne
	dest[640] = src[1] ;			// 2eme ligne

	*(S16*)(dest+640*2) = *(S16*)(src+2);	// 3eme ligne

	*(S16*)(dest+640*3) = *(S16*)(src+4);	// 4eme ligne
	dest[640*3+2] = src[6]		;

	dest[640*4] = src[7] ;			// 5eme ligne
	*(U32*)(dest+640*4+1) = *(U32*)(src+8);

	src += 12 ;

//----- Angle Superieur Droit
	dest = (U8*)Log + TabOffLine[y0] + x1 ;

	*(U32*)(dest-4) = *(U32*)src;		// 1ere ligne
	dest[0] = src[4] 	;

	dest[640-2] = src[5]		;	// 2nd ligne
	*(S16*)(dest+640-1) = *(S16*)(src+6);

	*(S16*)(dest+640*2-1) = *(S16*)(src+8);	// 3eme ligne

	dest[640*3] = src[10] ;			// 4eme ligne
	dest[640*4] = src[11] ;			// 5eme ligne

	src += 12 ;

//----- Angle Inferieur Droit
	dest = (U8*)Log + TabOffLine[y1-4] + x1 ;

	dest[0] = src[0]   ;			// 1ere ligne
	dest[640] = src[1] ;			// 2eme ligne

	*(S16*)(dest+640*2-1) = *(S16*)(src+2);	// 3eme ligne

	*(S16*)(dest+640*3-1) = *(S16*)(src+4);	// 4eme ligne
	dest[640*3-2] = src[6]		;

	dest[640*4-4] = src[7] ;		// 5eme ligne
	*(U32*)(dest+640*4-3) = *(U32*)(src+8);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Trace les angles arrondis des cadres de menus et de messages

#define	COUL_DOWN_BLUE	((10*16)+9)	// 10 --> Banque bleue
#define	COUL_UP_BLUE	((10*16)+15)	// 10 --> Banque bleue
#define	COUL_DOWN_RED	((4*16)+9)	//  4 --> Banque rouge
#define	COUL_UP_RED     ((4*16)+15)	//  4 --> Banque rouge

U8	CadreCoulDown = COUL_DOWN_BLUE	;
U8	CadreCoulUp   = COUL_UP_BLUE	;

void	DrawAngles( S32 x0, S32 y0, S32 x1, S32 y1, U32 type )
{
	if( type&BACKUP_ANGLES )
	{
		BackupAngles( x0, y0, x1, y1 ) ;
	}

	// Angle Sup‚rieur Gauche
	if( type&DRAW_SG )
	{
		Plot( x0+5, y0, CadreCoulUp ) ;
		Plot( x0+3, y0+1, CadreCoulUp ) ;
		Plot( x0+4, y0+1, CadreCoulUp ) ;
		Plot( x0+2, y0+2, CadreCoulUp ) ;
		Plot( x0+1, y0+3, CadreCoulUp ) ;
		Plot( x0+1, y0+4, CadreCoulUp ) ;
		Plot( x0, y0+5, CadreCoulUp ) ;
	}

	// Angle Inf‚rieur Gauche
	if( type&DRAW_IG )
	{
		Plot( x0, y1-5, CadreCoulUp ) ;
		Plot( x0+1, y1-4, CadreCoulUp ) ;
		Plot( x0+1, y1-3, CadreCoulUp ) ;
		Plot( x0+2, y1-2, CadreCoulUp ) ;
		Plot( x0+3, y1-1, CadreCoulDown ) ;
		Plot( x0+4, y1-1, CadreCoulDown ) ;
		Plot( x0+5, y1, CadreCoulDown ) ;
	}

	// Angle Inf‚rieur Droit
	if( type&DRAW_ID )
	{
		Plot( x1, y1-5, CadreCoulDown ) ;
		Plot( x1-1, y1-4, CadreCoulDown ) ;
		Plot( x1-1, y1-3, CadreCoulDown ) ;
		Plot( x1-2, y1-2, CadreCoulDown ) ;
		Plot( x1-3, y1-1, CadreCoulDown ) ;
		Plot( x1-4, y1-1, CadreCoulDown ) ;
		Plot( x1-5, y1, CadreCoulDown ) ;
	}

	// Angle Sup‚rieur Droit
	if( type&DRAW_SD )
	{
		Plot( x1-5, y0, CadreCoulUp ) ;
		Plot( x1-4, y0+1, CadreCoulUp ) ;
		Plot( x1-3, y0+1, CadreCoulUp ) ;
		Plot( x1-2, y0+2, CadreCoulUp ) ;
		Plot( x1-1, y0+3, CadreCoulDown ) ;
		Plot( x1-1, y0+4, CadreCoulDown ) ;
		Plot( x1, y0+5, CadreCoulDown ) ;
	}

	if( type&BACKUP_ANGLES )
	{
		RestoreAngles( x0, y0, x1, y1 ) ;
	}

	// Trace les flŠches si n‚cessaire avec la couleur param

	// Fl‚che sup‚rieure gauche
	if( type&ARROW_SG )
	{
		AffGraph( 0, x0+2, y0+2, HQR_Get(HQRPtrSprite,4) ) ;
		BoxStaticAdd( x0+2, y0+2, x0+27, y0+27 ) ;
	}

	// Fl‚che inf‚rieure gauche
	if( type&ARROW_IG )
	{
		AffGraph( 0, x0+2, y1-27, HQR_Get(HQRPtrSprite,5) ) ;
		BoxStaticAdd( x0+2, y1-27, x0+27, y1-2 ) ;
	}

	// Fl‚che inf‚rieure droite
	if( type&ARROW_ID )
	{
		AffGraph( 0, x1-27, y1-27, HQR_Get(HQRPtrSprite,6) ) ;
		BoxStaticAdd( x1-27, y1-27, x1-2, y1-2 ) ;
	}

	// Fl‚che sup‚rieure droite
	if( type&ARROW_SD )
	{
		AffGraph( 0, x1-27, y0+2, HQR_Get(HQRPtrSprite,7) ) ;
		BoxStaticAdd( x1-27, y0+2, x1-2, y0+27 ) ;
	}

	if( type&A_FLIP )	BoxUpdate() ;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void	DrawCadre( S32 x0, S32 y0, S32 x1, S32 y1, U32 type )
{
	if( type&CADRE_RED )
	{
		CadreCoulDown	= COUL_DOWN_RED	;
		CadreCoulUp	= COUL_UP_RED	;
	}

	if( type&ALL_ANGLES )
	{
		Line( x0+5  , y0  , x1-5, y0, CadreCoulUp ) ;
		Line( x0  , y0+5  , x0, y1-5, CadreCoulUp ) ;
		Line( x1  , y0+5, x1, y1-5, CadreCoulDown ) ;
		Line( x0+5, y1  , x1-5, y1, CadreCoulDown ) ;
	}
	else
	{
		Line( x0  , y0  , x1, y0, CadreCoulUp ) ;
		Line( x0  , y0  , x0, y1, CadreCoulUp ) ;
		Line( x1  , y0+1, x1, y1, CadreCoulDown ) ;
		Line( x0+1, y1  , x1, y1, CadreCoulDown ) ;
	}

	DrawAngles( x0, y0, x1, y1, type ) ;

	// Restore couleur d'origine en sortant
	CadreCoulDown	= COUL_DOWN_BLUE	;
	CadreCoulUp	= COUL_UP_BLUE		;
}
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void	DrawCadreBox( S32 x0, S32 y0, S32 x1, S32 y1, S32 color )
{
	Line( x0+5, y0+1, x1-5, y0+1, color )	;
	Line( x0+3, y0+2, x1-3, y0+2, color )	;
	Box( x0+2, y0+3, x1-2, y0+4, color )	;

	Box( x0+1, y0+5, x1-1, y1-5, color )	;

	Box( x0+2, y1-4, x1-2, y1-3, color )	;
	Line( x0+3, y1-2, x1-3, y1-2, color )	;
	Line( x0+5, y1-1, x1-5, y1-1, color )	;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void	DrawCadreShade( S32 x0, S32 y0, S32 x1, S32 y1, S32 color )
{
	ShadeBoxBlk( x0+5, y0+1, x1-5, y0+1, color )	;
	ShadeBoxBlk( x0+3, y0+2, x1-3, y0+2, color )	;
	ShadeBoxBlk( x0+2, y0+3, x1-2, y0+4, color )	;

	ShadeBoxBlk( x0+1, y0+5, x1-1, y1-5, color )	;

	ShadeBoxBlk( x0+2, y1-4, x1-2, y1-3, color )	;
	ShadeBoxBlk( x0+3, y1-2, x1-3, y1-2, color )	;
	ShadeBoxBlk( x0+5, y1-1, x1-5, y1-1, color )	;
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	    ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿
	    ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ ÛÛÚÄÛÛ³ ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ ÛÛÛ¿ÛÛ³ ÛÛÚÄÄÄÙ
	    ÛÛÛÛÛÛ¿ ÛÛ³     ÛÛÛÛÛÛ³ ÛÛÛÛÛ¿  ÛÛÛÛÛ¿  ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ¿
	    ÀÄÄÄÛÛ³ ÛÛ³     ÛÛÚÛÛÚÙ ÛÛÚÄÄÙ  ÛÛÚÄÄÙ  ÛÛÚÛÛÛ³ ÀÄÄÄÛÛ³
	    ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ¿ ÛÛ³ÀÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ³ÀÛÛ³ ÛÛÛÛÛÛ³
	    ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void	BackupScreen( S32 flagscreen )
{
	CopyScreen( Screen, ScreenAux ) ;	// decors only

	if( flagscreen )
	{
//		AffScene( AFF_OBJETS_NO_FLIP )	;
		CopyScreen( Log, Screen ) ;		// decors + persos
//		BoxUpdate() ;
		BoxReset() ;
	}
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void	RestoreScreen( void )
{
	S32	saveflagfade	;

	CopyScreen( ScreenAux, Screen )	;
	CopyScreen( ScreenAux, Log ) 	;

	RestoreTimer() 			;
	SaveTimer() 			;

	saveflagfade = FlagFade		;
	FlagFade = FALSE		;

	BoxReset() ;
	BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;
	AffScene( AFF_OBJETS_NO_FLIP ) 	;
	FixeCinemaMode( FALSE )		;
	BoxBlit()			;

	FlagFade = saveflagfade		;
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	    ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿
	    ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÀÄÛÛÚÄÙ ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ ÀÄÛÛÚÄÙ
	    ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³ ÛÛ³ ÛÛ³   ÛÛ³   ÛÛÛÛÛ¿  ÛÛ³       ÛÛ³
	    ÛÛÚÄÄÄÙ ÛÛÚÛÛÚÙ ÛÛ³ ÛÛ³   ÛÛ³   ÛÛÚÄÄÙ  ÛÛ³       ÛÛ³
	    ÛÛ³     ÛÛ³ÀÛÛ¿ ÛÛÛÛÛÛ³   ÛÛ³   ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿   ÛÛ³
	    ÀÄÙ     ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ   ÀÄÙ   ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ   ÀÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

S32		ProtectActif	= FALSE ;

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void	ToggleSortProtection( void )
{
	T_OBJET		*ptrobj = &ListObjet[NUM_PERSO] ;

	ProtectActif ^= 1 ;

	if( ProtectActif&1 )
	{
		ptrobj->Flags |= NO_CHOC ;
		ThrowExtraProtection( NUM_PERSO ) ;
	}
	else
	{
		ptrobj->Flags &= ~(NO_CHOC) 	;
		StopProtectSamples() 		;
		AnnuleExtraProtection() 	;
	}
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void	PlayProtectSamples( T_EXTRA *ptrextra )
{
	S32 x, y, z ;

	x = ptrextra->PosX ;
	y = ptrextra->PosY ;
	z = ptrextra->PosZ ;

	ParmSampleVolume = 50 ;

#ifdef	LBA_EDITOR
	if( !IsSamplePlaying( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTECTION) ) )
	{
		HQ_3D_MixSample( COMMON_NUM_DEF, SAMPLE_PROTECTION,
				 0x1000, 0, 0,
				 x, y, z ) ;
	}
	else
	{
		HQ_3D_ChangePanSample( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTECTION), x, y, z ) ;
	}

	if( ptrextra->Flags&EXTRA_CLIGNOTE )
	{
		ParmSampleVolume = 50 ;

		if( !IsSamplePlaying( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTECTION_ALARME) ) )
		{
			HQ_3D_MixSample( COMMON_NUM_DEF, SAMPLE_PROTECTION_ALARME,
					 0x1000, 0, 0,
					 x, y, z ) ;
		}
		else
		{
			HQ_3D_ChangePanSample( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTECTION_ALARME), x, y, z ) ;
		}
	}
#else
	if( !IsSamplePlaying( SAMPLE_PROTECTION ) )
	{
		HQ_3D_MixSample( SAMPLE_PROTECTION,
				 0x1000, 0, 0,
				 x, y, z ) ;
	}
	else
	{
		HQ_3D_ChangePanSample( SAMPLE_PROTECTION, x, y, z ) ;
	}

	if( ptrextra->Flags&EXTRA_CLIGNOTE )
	{
		ParmSampleVolume = 50 ;

		if( !IsSamplePlaying( SAMPLE_PROTECTION_ALARME ) )
		{
			HQ_3D_MixSample( SAMPLE_PROTECTION_ALARME,
					 0x1000, 0, 0,
					 x, y, z ) ;
		}
		else
		{
			HQ_3D_ChangePanSample( SAMPLE_PROTECTION_ALARME, x, y, z ) ;
		}
	}
#endif
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void 	StopProtectSamples( void )
{
#ifdef	LBA_EDITOR
	HQ_StopOneSample( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTECTION) ) ;
	HQ_StopOneSample( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTECTION_ALARME) ) ;
#else
	HQ_StopOneSample( SAMPLE_PROTECTION ) ;
	HQ_StopOneSample( SAMPLE_PROTECTION_ALARME ) ;
#endif
}
