#include 	"c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

	LONG	MixMusic = 1 ;		// Flag Mixer for Music
extern	LONG	MaxVolume ;		// Max Music Volume if no Mixer

#define	HAUTEUR_STANDARD	50
#define	MENU_SPACE		6

#define	COUL_TEXT_MENU		15	// trouver blanc palette jeu
#define	COUL_SELECT_MENU	68
#define	COUL_INPUT_NAME		112
#define	COUL_LETTER_INPUT	15

WORD	LargeurMenu = 550 ;

UWORD	GameMainMenu[] = {
			0,	// selected
			4,	// nb entries
			200,	// y center
			0,	// .dia num
			0, 20,	// start a new game game
			0, 21,	// continue game
			0, 23,	// options
			0, 22	// ret dos
				}	;

UWORD	GameQuitMenu[] = {
			0,	// selected
			2,	// nb entries
			240,	// y center
			0,	// .dia num
			0, 28,	// continuer jeu
			0, 27	// abandonner partie
				} ;

UWORD	GameOptionMenu[] = {
			0,	// selected
			4,	// nb entries
			0,	// y from top.
			0,	// .dia num

			0, 24,  // annuler
			0, 30,	// reglages volumes
			0, 46,	// gestion saved game
			0, 47	// advanced options
				} ;

UWORD	GameAdvancedMenu[] = {
			0,	// selected
			5,	// nb entries
			0,	// y from top.
			0,	// .dia num

			0, 26,	// retour
			0, 4,	// agressif auto/manuel
			6, 31,	// niveau details
			7, 32,	// ombres on/off
			8, 33	// zoom on/off
				} ;

UWORD	GameVolumeMenu[] = {
			0,	// selected
			7,	// nb entries
			0,	// y from top.
			0,	// .dia num

			0, 26,  // retour prec
			1, 10,	// type musvol / num text
			2, 11,	// type sampvol
			3, 12,	// type cdvol
			4, 13,	// type linevol
			5, 14,	// type mastervol
			0, 16	// save settings

				}	;

UWORD	SavedGameMenu[] = {
			0,	// selected
			3,	// nb entries
			0,	// y from top.
			0,	// .dia num

			0, 26,  // retour menu prec
			0, 41,	// copier une sauvegarde
			0, 45	// détruire une sauvegarde

				} ;

UWORD	SavedConfirmMenu[] = {
			0,	// selected
			2,	// nb entries
			240,	// y center.
			0,	// .dia num

			0, 24,  // annuler
			0, 48	// détruire une sauvegarde

				} ;

UWORD	GameChoiceMenu[] = {
			0,	// selected
			7,	// nb entries
			0,	// y from top. byte 8(
			0,	// .dia num

			0, 0,
			0, 1,
			0, 2,
			0, 3,
			0, 4,
			0, 5,
			0, 6	}	;

ULONG	TimerSample ;

// save players games

#define	MAX_PLAYER	50

UBYTE	NumVersion = 0 ;

#define	NB_CAR_H	14
#define	NB_CAR_V	5

#define	MAX_SIZE_PLAYER_NAME	29

UBYTE	GamePathname[_MAX_PATH] ;

//UBYTE	PlayerGameFile[_MAX_PATH] ;

UBYTE	PlayerName[MAX_SIZE_PLAYER_NAME+1] ;

/*──────────────────────────────────────────────────────────────────────────*/
// UBYTE	TabInvOrder[] = {

WORD	ListBetaGame[MAX_FLAGS_GAME] ;

#define	SIZE_INV_OBJ_X	75
#define	SIZE_INV_OBJ_Y	65


WORD	InvSelect = 0 ;
WORD	InvCoul = COUL_SELECT_MENU ;

#define	INV_START_X	17
#define	INV_START_Y	10

/*──────────────────────────────────────────────────────────────────────────*/

#define	SIZE_FOUND_OBJ	130

/*──────────────────────────────────────────────────────────────────────────*/

T_REAL_VALUE	RealRot ;
UBYTE		*Ptr3do ;
WORD		TabAnim[4] ;
WORD		FrameAnim[4] ;

extern	WORD	ZoneGrm ;
extern	WORD	IndexGrm ;

#define	CTRL_X0	100
#define	CTRL_Y0	100
#define	CTRL_X1	550
#define	CTRL_Y1	290

/*══════════════════════════════════════════════════════════════════════════*
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	Init3DGame()
{
	SetIsoProjection( 320-8-1, 240, SIZE_BRICK_XZ ) ;
	SetPosCamera( 0,0,0 ) ;
	SetAngleCamera( 0,0,0 ) ;
	SetLightVector( AlphaLight, BetaLight, 0 ) ;
}

extern	UBYTE	*Imagen ;

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawFire( WORD x0, WORD y0, WORD x1, WORD y1, WORD coul )
{
	DoFire( y0, coul ) ;

	if( !Rnd(5) )
	{
		BufSpeak[ Rnd(320*10)+20*320 ] = 255 ;
	}
}

void	WaitReleaseSpace()
{
	SaveTimer() ;
	while( (Fire & F_SPACE)
	OR (Key == K_ESC) ) ;
	RestoreTimer() ;
}

/*══════════════════════════════════════════════════════════════════════════*
	     █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀       █▄ ▄█ █▀▀▀▀ ██▄ █ █   █
	     ██ ▀█ ██▀▀█ ██▀ █ ██▀▀        ██▀ █ ██▀▀  ██▀██ ██  █
	     ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/

/*══════════════════════════════════════════════════════════════════════════*/

void	DrawObj3D( WORD x0, WORD y0, WORD x1, WORD y1,
	WORD y3d , WORD betaflag ,
	UBYTE *ptr3do )
{
	WORD	beta ;

	SetIsoProjection( (x1+x0)/2, (y1+y0)/2, 0 ) ;

	SetClip( x0,y0, x1,y1 ) ;

	if( betaflag == -1 )
	{
		beta = GetRealAngle( &RealRot ) ;

		if( RealRot.TimeValue == 0 )
		{
			InitRealAngle( beta, beta-256, 50, &RealRot ) ;
		}
	}
	else
	{
		beta = betaflag ;
	}

	AffObjetIso( 0,y3d,0, 0,beta,0, ptr3do ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	DrawCadre( WORD x0, WORD y0, WORD x1, WORD y1 )
{
	Line( x0  , y0  , x1, y0, 79 ) ;
	Line( x0  , y0  , x0, y1, 79 ) ;
	Line( x1  , y0+1, x1, y1, 73 ) ;
	Line( x0+1, y1  , x1, y1, 73 ) ;
}

/*══════════════════════════════════════════════════════════════════════════*
    █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀█ █▀▀▀█ █▀▀▀█ ▀▀█▀▀ █▀▀▀▀ █▄ ▄█ █▀▀▀▀ ██▄ █ ▀▀█▀▀
    ██    ██  █ ██▀ █ ██▀▀▀ ██  █ ██▀█▀   ██  ██▀▀  ██▀ █ ██▀▀  ██▀██   ██
    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀    ▀▀▀▀▀ ▀▀  ▀   ▀▀  ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀   ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DrawComportement( WORD comportement, WORD beta, WORD copyblock )
{
	WORD	x0, y0, x1, y1 ;
	UBYTE	*ptranim ;
	WORD	frameanim ;


	x0 = CTRL_X0+10 + comportement * 110 ;
	y0 = CTRL_Y0+10 ;
	x1 = x0+99 ;
	y1 = y0+119 ;

	ptranim = HQR_Get( HQR_Anims, TabAnim[comportement] ) ;
	frameanim = FrameAnim[comportement] ;

	if( SetInterAnimObjet( frameanim, ptranim, Ptr3do ) )
	{
		frameanim++ ;
		if( frameanim == GetNbFramesAnim( ptranim ) )
		{
			frameanim = GetBouclageAnim( ptranim ) ;
		}
		FrameAnim[comportement] = frameanim ;
	}

	if( !copyblock )	/* init */
	{
		DrawCadre( x0-1, y0-1, x1+1, y1+1 ) ;
	}

	MemoClip() ;
	UnSetClip() ;

	if( comportement != Comportement )
	{

		Box( x0,y0, x1,y1, 0 ) ;

/*		CopyBlock( x0,y0, x1,y1, Screen, x0, y0, Log ) ;
		ShadeBox( x0,y0, x1,y1, 3 ) ;
*/
	}
	else
	{

		Box( x0,y0, x1,y1, 68 ) ;

/*		CopyBlock( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40, Screen,
			   CTRL_X0+10, CTRL_Y0+139, Log ) ;
		ShadeBox( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40, 4 ) ;
*/
		Box( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40, 0 ) ;
		DrawCadre( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40 ) ;

		CoulFont( 15 ) ;
		if( Comportement == C_AGRESSIF
		AND CombatAuto == TRUE )
			GetMultiText( 4, String ) ;
		else
			GetMultiText( Comportement, String ) ;

		Font( (CTRL_X1+CTRL_X0-SizeFont( String ))/2, CTRL_Y0+140, String )		;
	}

	DrawObj3D(	x0, y0, x1, y1,
			-600, beta,
			Ptr3do ) ;

/*	if( comportement != Comportement )
	{
//		ShadeBox( x0,y0, x1,y1, 5 ) ;
	}
	else
	{
//		Rect( x0+1,y0+1, x1-1,y1-1, 12 ) ;
		Rect( x0+2,y0+2, x1-2,y1-2, 12 ) ;
	}
*/

	if( copyblock )
	{
		CopyBlockPhys( x0,y0,x1,y1 ) ;
		CopyBlockPhys( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40 ) ;
	}

	RestoreClip() ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	DrawInfoMenu( WORD x0, WORD y0 )
{
	WORD	x,x1 ;
	WORD	l ;
	WORD	lifepoint ;

	UnSetClip() ;

	DrawCadre( x0,y0, x0+450, y0+80 ) ;
	/*Shade*/Box( x0+1,y0+1, x0+450-1, y0+80-1, 0/*3*/ ) ;

	// Life Points	(max 50)

//	AffGraph( 4, x0+9, y0+13, PtrSpriteExtra ) ;
	AffGraph( 0, x0+9, y0+13, HQR_Get(HQRPtrSpriteExtra, 4) ) ;

	lifepoint = ListObjet[NUM_PERSO].LifePoint ;
	x = RegleTrois32( x0+25, x0+325, 50, lifepoint ) ;

	Box( x0+25, y0+10, x, y0+10+15, 91 ) ;
	Box( x, y0+10, x0+325, y0+10+15, 0 ) ;

	DrawCadre( x0+25, y0+10, x0+325, y0+10+15 ) ;

	// Magic level	(max 4)	MagicPoint (max 80)

	if( !ListFlagGame[FLAG_CONSIGNE]
	AND  ListFlagGame[FLAG_TUNIQUE] )
	{

//	AffGraph( 5, x0+9, y0+36, PtrSpriteExtra ) ;
	AffGraph( 0, x0+9, y0+36, HQR_Get(HQRPtrSpriteExtra, 5) ) ;

	if( MagicLevel )
	{
		x = RegleTrois32( x0+25, x0+325, 80, MagicPoint ) ;
		Box( x0+25, y0+35, x, y0+35+15, 75 ) ;

		for( l=0; l<MagicLevel; l++ )
		{
			x1 = RegleTrois32( x0+25, x0+325, 40, MagicLevel*10 ) ;
			Line( x1, y0+35+1, x1, y0+35+15-1, 0 ) ;
		}

		x1 = RegleTrois32( x0+25, x0+325, 40, MagicLevel*10 ) ;
		DrawCadre( x0+25, y0+35, x1, y0+35+15 ) ;
	}

	}

	// Nb Gold Pieces

//	AffGraph( 3, x0+340, y0+15, PtrSpriteExtra ) ;
	AffGraph( 0, x0+340, y0+15, HQR_Get(HQRPtrSpriteExtra, 3) ) ;
	CoulFont( 155 ) ;
	Font( x0+370, y0+5, Itoa(NbGoldPieces) ) ;

	// Nb Little Keys

//	AffGraph( 6, x0+340, y0+55, PtrSpriteExtra ) ;
	AffGraph( 0, x0+340, y0+55, HQR_Get(HQRPtrSpriteExtra, 6) ) ;
	CoulFont( 155 ) ;
	Font( x0+370, y0+40, Itoa(NbLittleKeys) ) ;

	// Nb CloverBox

	for( l=0; l<NbCloverBox; l++ )
	{
		x1 = RegleTrois32( x0+25, x0+325, MAX_CLOVER_BOX, l ) ;
//		AffGraph( 41, x1, y0+58, PtrSpriteExtra ) ;
		AffGraph( 0, x1, y0+58, HQR_Get(HQRPtrSpriteExtra, 41) ) ;
	}

	// Nb Four-leaf Clover

	for( l=0; l<NbFourLeafClover; l++ )
	{
		x1 = RegleTrois32( x0+25, x0+325, MAX_CLOVER_BOX, l ) ;
//		AffGraph( 7, x1+2, y0+60, PtrSpriteExtra ) ;
		AffGraph( 0, x1+2, y0+60, HQR_Get(HQRPtrSpriteExtra, 7) ) ;
	}

	CopyBlockPhys( x0,y0, x0+450, y0+80 ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	DrawMenuComportement( WORD beta )
{
	DrawCadre( CTRL_X0  , CTRL_Y0  , CTRL_X1, CTRL_Y1 ) ;

	ShadeBox( CTRL_X0+1,CTRL_Y0+1, CTRL_X1-1, CTRL_Y1-1, 2 ) ;

	SetAnimObjet(	FrameAnim[C_NORMAL],
			HQR_Get( HQR_Anims, TabAnim[C_NORMAL] ),
			Ptr3do ) ;
	DrawComportement( C_NORMAL, beta, FALSE ) ;

	SetAnimObjet(	FrameAnim[C_SPORTIF],
			HQR_Get( HQR_Anims, TabAnim[C_SPORTIF] ),
			Ptr3do ) ;
	DrawComportement( C_SPORTIF, beta, FALSE ) ;

	SetAnimObjet(	FrameAnim[C_AGRESSIF],
			HQR_Get( HQR_Anims, TabAnim[C_AGRESSIF] ),
			Ptr3do ) ;
	DrawComportement( C_AGRESSIF,beta, FALSE ) ;

	SetAnimObjet(	FrameAnim[C_DISCRET],
			HQR_Get( HQR_Anims, TabAnim[C_DISCRET] ),
			Ptr3do ) ;
	DrawComportement( C_DISCRET, beta, FALSE ) ;

	DrawInfoMenu( CTRL_X0, CTRL_Y1+10 ) ;

	CopyBlockPhys( CTRL_X0,CTRL_Y0, CTRL_X1, CTRL_Y1 ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	MenuComportement()
{
	T_OBJET	*ptrobj ;
	UBYTE	*memoptrfile3d ;
	WORD	oldcomp ;
	WORD	memoisland ;
	LONG	memoflagspeak ;

	SaveTimer() ;

	if( Comportement == C_PROTOPACK )
	{
		HQ_StopSample() ;
		SetComportement( C_NORMAL ) ;
	}

	ptrobj = &ListObjet[NUM_PERSO] ;

	memoptrfile3d = ptrobj->PtrFile3D ;

	Ptr3do = PtrBody[ptrobj->Body] ;

	TabAnim[0] = AnimRienNormal ;
	TabAnim[1] = AnimRienSportif ;
	TabAnim[2] = AnimRienAgressif ;
	TabAnim[3] = AnimRienDiscret ;

	InitRealAngle( ptrobj->Beta, ptrobj->Beta-256, 50, &RealRot ) ;

	CopyScreen( Log, Screen ) ;

	memoisland = Island ;
	Island = 255 ;

	memoflagspeak = FlagSpeak ;
	FlagSpeak = FALSE ;
	InitDial( 0 ) ;

	DrawMenuComportement( ptrobj->Beta ) ;

	oldcomp = Comportement ;
	SetAnimObjet(	FrameAnim[Comportement],
			HQR_Get( HQR_Anims, TabAnim[Comportement] ),
			Ptr3do ) ;

	while( (Fire & F_CTRL) OR ((Key>=K_F1) AND (Key<=K_F4)) )
	{
#ifdef	DEBUG_TOOLS
		CheckSavePcx() ;
#endif
		MyJoy = Joy ;

/*		if( Comportement == C_AGRESSIF )
		{
			if( MyJoy & J_UP )	CombatAuto = TRUE ;
			if( MyJoy & J_DOWN )	CombatAuto = FALSE ;
		}
*/
		if( MyJoy & J_RIGHT ) 	Comportement++ ;
		if( MyJoy & J_LEFT )	Comportement-- ;
		Comportement &= 3 ;

		if( Comportement != oldcomp )
		{
			DrawComportement( oldcomp, ptrobj->Beta, TRUE ) ;

			oldcomp = Comportement ;
			InitRealAngle(	ptrobj->Beta,
					ptrobj->Beta-256,
					50, &RealRot ) ;

			SetAnimObjet(	FrameAnim[Comportement],
					HQR_Get( HQR_Anims, TabAnim[Comportement] ),
					Ptr3do ) ;

			while( Joy )
				DrawComportement( Comportement, -1, TRUE ) ;
		}
		DrawComportement( Comportement, -1, TRUE ) ;
	}
	SetComportement( Comportement ) ;

	Init3DGame() ;

	Island = memoisland ;

	FlagSpeak = memoflagspeak ;
	InitDial( START_FILE_ISLAND+Island ) ;

	RestoreTimer() ;
}

/*══════════════════════════════════════════════════════════════════════════*
	      █    ██▄ █ █   █ █▀▀▀▀ ██▄ █ ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █  ▄▀
	      ██   ██▀██ ██ ▄▀ ██▀▀  ██▀██   ██  ██  █ ██▀█▀ ██▀
	      ▀▀   ▀▀  ▀ ▀▀▀   ▀▀▀▀▀ ▀▀  ▀   ▀▀  ▀▀▀▀▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

void	Draw3dObject( WORD x, WORD y, WORD *ptrobj, WORD beta, WORD zoom )
{
	SetProjection( x,y,128,200,200 ) ;

	SetFollowCamera( 0,0,0, 60,0,0, zoom ) ;

	if( !ptrobj )
	{
		Message( "ptrobj = 0", TRUE ) ;
	}
	else
	{
		AffObjetIso(	0,0,0, 0,beta,0, ptrobj ) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawOneInventory( WORD flagnum )
{
	WORD	x, y, x0, y0, x1, y1 ;
	WORD	*ptrobj ;

	x =	INV_START_X + 10 + SIZE_INV_OBJ_X/2
		+ (SIZE_INV_OBJ_X+10) * (flagnum/4) ;
	y = 	INV_START_Y + 10 + SIZE_INV_OBJ_Y/2
		+ (SIZE_INV_OBJ_Y+10) * (flagnum&3) ;

	x0 = x - SIZE_INV_OBJ_X/2 ;
	y0 = y - SIZE_INV_OBJ_Y/2 ;
	x1 = x + SIZE_INV_OBJ_X/2 ;
	y1 = y + SIZE_INV_OBJ_Y/2 ;

	if( flagnum == InvSelect )
	{
		Box( x0, y0, x1, y1, InvCoul ) ;
	}
	else
	{
		Box( x0, y0, x1, y1, 0 ) ;
//		CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
//		ShadeBox( x0, y0, x1, y1, 4 ) ;
	}

	if( (ListFlagGame[flagnum])
	AND ((ListFlagGame[FLAG_CONSIGNE]==0) OR (flagnum==FLAG_CLOVER)) )
	{
		ptrobj = HQR_Get( InventoryObj, flagnum ) ;
		if( HQR_Flag )
		{
			PatchObjet( ptrobj ) ;
		}

		Draw3dObject(	x, y, ptrobj,
				ListBetaGame[flagnum]+=8, 15000 ) ;

		if( flagnum == 15 )	// carburant
		{
			CoulFont( 15 ) ;
			Font( x0+3,y0+32, Itoa( Fuel ) ) ;
		}
	}

	DrawCadre( x0, y0, x1, y1 ) ;

	CopyBlockPhys( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/


void	DrawListInventory()
{
	WORD	x, y ;
	WORD	n = 0 ;
	WORD	x0, y0, x1, y1 ;

	x0 = INV_START_X ;
	y0 = INV_START_Y ;

	x1 = x0 + (SIZE_INV_OBJ_X+10)*7 + 10 ;
	y1 = y0 + (SIZE_INV_OBJ_Y+10)*4 + 10 ;

	ShadeBox( x0,y0, x1,y1, 4 ) ;
	DrawCadre( x0,y0, x1,y1 ) ;

/*	DrawCadre(	INV_START_X+ 10 - 2 + (SIZE_INV_OBJ_X+10) * 0,
			INV_START_Y+ 10 - 2 + (SIZE_INV_OBJ_Y+10) * 1,

			INV_START_X+ 1 + (SIZE_INV_OBJ_X+10) * 1,
			INV_START_Y+ 1 + (SIZE_INV_OBJ_Y+10) * 3  ) ;
*/
	Rect(	INV_START_X+ 10 - 2 + (SIZE_INV_OBJ_X+10) * 1,
		INV_START_Y+ 10 - 2 + (SIZE_INV_OBJ_Y+10) * 0,
		INV_START_X+ 1 + (SIZE_INV_OBJ_X+10) * 2,
		INV_START_Y+ 1 + (SIZE_INV_OBJ_Y+10) * 4,
		75  ) ;

	CopyBlockPhys( x0,y0, x1,y1 ) ;

	for( n=0; n<MAX_INVENTORY; n++ )
	{
		DrawOneInventory( n ) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void	Inventory()
{
	WORD	flag = 1 ;
	WORD	dialstat = 0 ;
	WORD	oldselect ;
	WORD	memoalphalight, memobetalight ;
	LONG	memoflagspeak ;

	CopyScreen( Log, Screen ) ;

	memoalphalight = AlphaLight ;
	memobetalight = BetaLight ;
	SetLightVector( 896, 950, 0 ) ;

	// magic NRJ
	if( NbFourLeafClover > 0 )	ListFlagGame[FLAG_CLOVER] = TRUE ;
	else				ListFlagGame[FLAG_CLOVER] = FALSE ;

	InvCoul = COUL_SELECT_MENU ;
	DrawListInventory() ;	// draw all

	memoflagspeak = FlagSpeak ;
	FlagSpeak = FALSE ;
	InitDial( 2 ) ;	// game divers txt

	dialstat = 3 ;
	TestCoulDial( 4 ) ;
	InitDialWindow() ;

	while( Key != K_ESC )
	{
#ifdef	DEBUG_TOOLS
		CheckSavePcx() ;
#endif
		oldselect = InvSelect ;

		if( !flag )
		{
			MyJoy = Joy ;
			MyFire = Fire ;
			MyKey = Key ;
			if( MyJoy OR Fire )	flag = 1 ;
		}
		else
		{
			MyFire = MyJoy = MyKey = 0 ;
			if( !Joy AND !Fire )	flag = 0 ;
		}

		if( (MyKey == K_ESC) OR (MyFire&F_SHIFT) )
		{
			break ;
		}

		if( MyJoy & J_DOWN )
		{
			InvSelect++ ;
			if( InvSelect >= MAX_INVENTORY )
				InvSelect = 0 ;
			DrawOneInventory( oldselect ) ;
			dialstat = 3 ;
		}

		if( MyJoy & J_UP )
		{
			InvSelect-- ;
			if( InvSelect < 0 )
				InvSelect = MAX_INVENTORY - 1 ;
			DrawOneInventory( oldselect ) ;
			dialstat = 3 ;
		}

		if( MyJoy & J_LEFT )
		{
			InvSelect -= 4 ;
			if( InvSelect < 0 )
				InvSelect = MAX_INVENTORY + InvSelect ;
			DrawOneInventory( oldselect ) ;
			dialstat = 3 ;
		}

		if( MyJoy & J_RIGHT )
		{
			InvSelect += 4 ;
			if( InvSelect >= MAX_INVENTORY )
				InvSelect = InvSelect - MAX_INVENTORY ;
			DrawOneInventory( oldselect ) ;
			dialstat = 3 ;
		}


		if( dialstat == 3 )
		{
			SecondInitDialWindow() ;

			if( (ListFlagGame[InvSelect]==1)
			AND ((ListFlagGame[FLAG_CONSIGNE]==0) OR (InvSelect==FLAG_CLOVER)) )
			{
				OpenDialNoWindow( InvSelect+100 ) ;
				dialstat = 0 ;
			}
			else
			{
				OpenDialNoWindow( 128 ) ;
				dialstat = 0 ;
			}
		}

		if( dialstat != 2 )
		{
			dialstat = NextDialCar() ;
		}
		if( dialstat != 1 )		// en cours lecture
		{
			Vsync() ;
		}

		if( MyFire & F_SPACE )
		{
			if( dialstat == 2 )
			{
				SecondInitDialWindow() ;
				dialstat = 0 ;
			}
			else
			{
				if( (ListFlagGame[InvSelect]==1)
				AND ((ListFlagGame[FLAG_CONSIGNE]==0) OR (InvSelect==FLAG_CLOVER)) )
				{
					SecondInitDialWindow() ;
					OpenDialNoWindow( InvSelect+100 ) ;
				}
			}
		}

		DrawOneInventory( InvSelect ) ;

		if( MyFire & F_RETURN )
		{
			if( (ListFlagGame[InvSelect]==1)
			AND ((ListFlagGame[FLAG_CONSIGNE]==0) OR (InvSelect==FLAG_CLOVER)) )
			{
				InventoryAction = InvSelect ;

				InvCoul = 91 ; // rouge
				DrawOneInventory( InvSelect ) ;

				break ;
			}
		}
	}
	CloseDial() ;

	AlphaLight = memoalphalight ;
	BetaLight  = memobetalight ;
	Init3DGame() ;

	FlagSpeak = memoflagspeak ;
	InitDial( START_FILE_ISLAND+Island ) ;

	while( Key OR Fire ) ;
}
/*══════════════════════════════════════════════════════════════════════════*
    █▀▀▀▀ █▀▀▀█ █   █ ██▄ █ █▀▀▀▄       █▀▀▀█ █▀▀█      █ █▀▀▀▀ █▀▀▀▀ ▀▀█▀▀
    ██▀▀  ██  █ ██  █ ██▀██ ██  █       ██  █ ██▀▀█ ▄▄  █ ██▀▀  ██      ██
    ▀▀    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DoFoundObj( WORD numobj )
{
	char	string[256] ;
	WORD	*ptr3do ;
	T_OBJET	*ptrobj ;
	WORD	xm, ym, zm, xw, yw, zw ;
	WORD	x, y, x0, y0, x1, y1 ;
	ULONG	timerend ;
	WORD	dialstat ;
	WORD	flag ;
	UBYTE	*ptranim ;
	WORD	numanim ;
	WORD	frameanim ;

	LONG	memoflagspeak ;
	LONG	speakfromcd  ;
	LONG	memotimersystem ;

	ptrobj = &ListObjet[NUM_PERSO] ;

	// reaff scene recentree sans twinkel

	StartXCube = ((ListObjet[NUM_PERSO].PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
	StartYCube = ((ListObjet[NUM_PERSO].PosObjY+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
	StartZCube = ((ListObjet[NUM_PERSO].PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;

	ptrobj->Flags |= INVISIBLE ;
	AffScene( TRUE ) ;
	ptrobj->Flags &= ~INVISIBLE ;

	CopyScreen( Log, Screen ) ;

	// reaff twinksen anim trouve

	xw = StartXCube*SIZE_BRICK_XZ ;
	yw = StartYCube*SIZE_BRICK_Y ;
	zw = StartZCube*SIZE_BRICK_XZ ;

	AffObjetIso(	ptrobj->PosObjX-xw, ptrobj->PosObjY-yw, ptrobj->PosObjZ-zw,
			0, 128/*ptrobj->Beta*/, 0,
			PtrBody[ptrobj->Body] ) ;

	SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;

	xm = ((ptrobj->PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
	ym = ((ptrobj->PosObjY)/SIZE_BRICK_Y) ;
	if( (ptrobj->Col&127) != 0 )	ym++ ;
	zm = ((ptrobj->PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;

	DrawOverBrick( xm, ym, zm ) ;

	Flip() ;

	ProjettePoint(	ptrobj->PosObjX-xw,
			ptrobj->PosObjY-yw,
			ptrobj->PosObjZ-zw ) ;

	Yp -= 150 ;

	x0 = Xp - SIZE_FOUND_OBJ/2 ;
	y0 = Yp - SIZE_FOUND_OBJ/2 ;
	x1 = Xp + SIZE_FOUND_OBJ/2 ;
	y1 = Yp + SIZE_FOUND_OBJ/2 ;

	HQ_MixSample(	41, 0x1000, 1, 128, 128 ) ;

	// aff text et objet

#ifdef	CDROM

	if( IsVoiceFileOnHD( 2 ) )
	{
		// voix sur hd touche à rien

		InitDial( 2 ) ;	// game divers txt
		speakfromcd = FALSE ;

	}
	else	// coupe music cd si on
	{
		StopMusicCD() ;
		memoflagspeak = FlagSpeak ;
		FlagSpeak = FALSE ;

		InitDial( 2 ) ;

		FlagSpeak = memoflagspeak ;
		speakfromcd = TRUE ;
	}

#else

	InitDial( 2 ) ;	// game divers txt

#endif

	UnSetClip() ;
	OpenDial( numobj ) ;
//	timerend = TimerRef + 50 * 30 ;
	dialstat = 1 ;
	flag = FALSE ;
	TestCoulDial( 4 ) ;	// coul twinsen
#ifdef	CDROM

	if( speakfromcd )
	{
		SpeakFromCD( 2, numobj ) ;
	}
	else
	{
		Speak( numobj ) ;
	}
#endif

	numanim = SearchAnim( GEN_ANIM_TROUVE, NUM_PERSO ) ;
	ptranim = HQR_Get( HQR_Anims, numanim ) ;
#ifdef	DEBUG_TOOLS
	if( numanim == -1 )
	{
		Message( "num anim 'trouve' = -1 in DoFoundObj", TRUE ) ;
		goto argexit ;
	}
	if( !ptranim )
	{
		Message( "ptranim = 0 in DoFoundObj", TRUE ) ;
		goto argexit ;
	}
#endif

	frameanim = 0 ;

	PtrBufferAnim += StockInterAnim( PtrBufferAnim, PtrBody[ptrobj->Body] ) ;
	if( PtrBufferAnim > BufferAnim + 5000 - 512 )	PtrBufferAnim = BufferAnim ;

	ptr3do = HQR_Get( InventoryObj, numobj ) ;
	if( HQR_Flag )
	{
		PatchObjet( ptr3do ) ;
	}

	NbOptPhysBox = 0 ;		// clear list cls boxes

	while( flag == FALSE )
	{
		UnSetClip() ;

		NbPhysBox = 0 ;
		ClsBoxes() ;

		ShadeBox( x0, y0, x1, y1, 4 ) ;
		SetClip(  x0, y0, x1, y1 ) ;

		Draw3dObject(	Xp, Yp, ptr3do,
				ListBetaGame[numobj]+=8, 10000 ) ;

		DrawCadre( x0, y0, x1, y1 ) ;
		AddPhysBox( x0,y0, x1,y1 ) ;

		UnSetClip() ;
		Init3DGame() ;

		// reaff twinkel anim trouve

		if( SetInterAnimObjet( frameanim, ptranim, PtrBody[ptrobj->Body] ) )
		{
			frameanim++ ;
			if( frameanim == GetNbFramesAnim( ptranim ) )
			{
				frameanim = GetBouclageAnim( ptranim ) ;
			}
		}

		AffObjetIso(	ptrobj->PosObjX-xw, ptrobj->PosObjY-yw, ptrobj->PosObjZ-zw,
				0, 128/*ptrobj->Beta*/, 0,
				PtrBody[ptrobj->Body] ) ;

		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
		DrawOverBrick( xm, ym, zm ) ;
		AddPhysBox( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;

		// message

		if( dialstat == 1 )
		{
			UnSetClip() ;
			dialstat = NextDialCar() ;
		}
		if( dialstat != 1 )		// pas en cours lecture
		{
			Vsync() ;
		}

		FlipBoxes() ;

		if( Fire )
		{
			if( dialstat == 0 )
			{
				flag = TRUE ;
			}
			if( dialstat == 2 )
			{
				dialstat = 1 ;	// continue
			}
		}
	}

#ifdef	DEBUG_TOOLS
argexit:
#endif

#ifdef	CDROM

	if( speakfromcd )
	{
		while( TestSpeakFromCD() AND (Key != K_ESC) ) ;
	}
	else
	{
		while( TestSpeak() AND (Key != K_ESC) ) ;
	}
	StopSpeak() ;
#endif
	Init3DGame() ;

	InitDial( START_FILE_ISLAND+Island ) ;

	while( Key == K_ESC ) ;

}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ExtInitMcga()
{
	int	i ;
	char	num[10] ;

	if (Wave_Driver_Enable)
		while( !WavePause() ) ;

	InitMcgaMode() ;
	if( FlagPalettePcx )	Palette( PalettePcx ) ;
	else			Palette( PtrPal ) ;
	WaveContinue() ;
	FlagMCGA = TRUE ;
}

void	ExtInitSvga()
{
	if (Wave_Driver_Enable)
		while( !WavePause() ) ;

	SimpleInitSvga() ;
	if( FlagPalettePcx )	Palette( PalettePcx ) ;
	else			Palette( PtrPal ) ;
	FlagMCGA = FALSE ;
	WaveContinue() ;
}

void	TestRestoreModeSVGA( LONG flag )
{
	if( FlagMCGA )
	{
		ExtInitSvga() ;
		if( flag )	AffScene(TRUE)	;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/

void	CopyBlockPhysMCGA( LONG x0, LONG y0, LONG x1, LONG y1 )
{
	LONG	x, y ;

/*	x = (x0+x1)/2 - 160 ;
	y = (y0+y1)/2 - 100 ;
*/
	x = x0 - 160 ;
	y = y0 - 140 ;

	if( x > 640-320 )	x = 640-320 ;
	if( x < 0 )		x = 0 ;
	if( y > 480-200 )	y = 480-200 ;
	if( y < 0 )		y = 0 ;

	CopyBlockMCGA( x,y, x+319, y+199, Log, 0,0,0xA0000 ) ;
}

/*══════════════════════════════════════════════════════════════════════════*
	     ██▀▀▀ █▀▀▀█ █   █ █▀▀▀▀       █     █▀▀▀█ █▀▀▀█ █▀▀▀▄
	     ▀▀▀▀█ ██▀▀█ ██ ▄▀ ██▀▀        ██    ██  █ ██▀▀█ ██  █
	     ▀▀▀▀▀ ▀▀  ▀ ▀▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

WORD	PlayerGameList( UBYTE **ptrlistname, UBYTE *listname )
{
	struct	find_t	fileinfo ;
	ULONG	rc ;
	UBYTE	wbyte ;
	UBYTE	pathname[_MAX_PATH] ;
	WORD	nb = 0 ;
	ULONG	handle ;

	strcpy( pathname, PATH_RESSOURCE"*.LBA" ) ;

	rc = _dos_findfirst( pathname, _A_NORMAL, &fileinfo ) ;
	while( !rc )
	{
		strcpy( pathname, PATH_RESSOURCE ) ;
		strcat( pathname,  fileinfo.name ) ;
		handle = OpenRead( pathname ) ;
		if( handle )
		{
			Read( handle, &wbyte, 1 ) ;	// num version

			*ptrlistname++ = listname ;
			do
			{
				Read( handle, &wbyte, 1 ) ;
				*listname++ = wbyte ;

			} while( wbyte != 0 ) ;

			Close( handle ) ;

			nb++ ;
			if( nb == MAX_PLAYER )	return nb ;
		}
		rc = _dos_findnext( &fileinfo ) ;
	}
	return nb ;
}

/*──────────────────────────────────────────────────────────────────────────*/

WORD	FindPlayerFile()
{
	struct	find_t	fileinfo ;
	ULONG	rc ;
	UBYTE	pathname[_MAX_PATH] ;
	UBYTE	playername[MAX_SIZE_PLAYER_NAME+1] ;
	UBYTE	*ptr ;
	ULONG	handle ;
	UBYTE	wbyte ;

	strcpy( pathname, PATH_RESSOURCE"*.LBA" ) ;

	rc = _dos_findfirst( pathname, _A_NORMAL, &fileinfo ) ;
	while( !rc )
	{
		strcpy( pathname, PATH_RESSOURCE ) ;
		strcat( pathname,  fileinfo.name ) ;

		handle = OpenRead( pathname ) ;

		Read( handle, &wbyte, 1 ) ;	// num version

		ptr = playername ;
		do
		{
			Read( handle, &wbyte, 1 ) ;
			*ptr++ = wbyte ;

		} while( wbyte != 0 ) ;

		Close( handle ) ;

		if( !stricmp( PlayerName, playername ) )
		{
			strcpy( GamePathname, PATH_RESSOURCE ) ;
			strcat( GamePathname, fileinfo.name ) ;
			return TRUE ;
		}

		rc = _dos_findnext( &fileinfo ) ;
	}
	return FALSE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	SaveGame()
{
	WORD	handle ;
	WORD	wword ;
	UBYTE	wbyte ;

	handle = OpenWrite( GamePathname ) ;
	if( !handle )
	{
		Message( "Error Writing Saved Game", TRUE ) ;
		return ;
	}

	NumVersion = 3 ;

	Write( handle, &NumVersion, 1 ) ;

	Write( handle, PlayerName, strlen(PlayerName)+1 ) ;

// list flag game
	wbyte = MAX_FLAGS_GAME ;
	Write( handle, &wbyte, 1 ) ;	// nb octets
	Write( handle, ListFlagGame, MAX_FLAGS_GAME ) ;

// infos jeu
	Write( handle, &NumCube, 1 ) ;
	Write( handle, &Chapitre, 1 ) ;
	Write( handle, &Comportement, 1 ) ;
	Write( handle, &ListObjet[NUM_PERSO].LifePoint, 1 ) ;

	Write( handle, &NbGoldPieces, 2 ) ;

	Write( handle, &MagicLevel, 1 ) ;
	Write( handle, &MagicPoint, 1 ) ;
	Write( handle, &NbCloverBox, 1 ) ;

	Write( handle, &SceneStartX, 2 ) ;
	Write( handle, &SceneStartY, 2 ) ;
	Write( handle, &SceneStartZ, 2 ) ;
	Write( handle, &ListObjet[NUM_PERSO].Beta, 2 ) ;

	Write( handle, &ListObjet[NUM_PERSO].GenBody, 1 ) ;

// tabholopos

	wbyte = MAX_HOLO_POS ;
	Write( handle, &wbyte, 1 ) ;
	Write( handle, TabHoloPos, wbyte ) ;

// others
	Write( handle, &Fuel, 1 ) ;

// used inventory
	wbyte = MAX_INVENTORY ;
	Write( handle, &wbyte, 1 ) ;	// nb octets
	Write( handle, ListFlagInventory, MAX_INVENTORY ) ;

// others
	Write( handle, &NbFourLeafClover, 1 ) ;
	Write( handle, &Weapon, 2 ) ;

	Close( handle ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	LoadGame()
{
	WORD	handle ;
	WORD	wword ;
	UBYTE	wbyte ;
	UBYTE	*ptr ;

	handle = OpenRead( GamePathname ) ;

	Read( handle, &wbyte, 1 ) ;	// num version

	ptr = PlayerName ;
	do
	{
		Read( handle, &wbyte, 1 ) ;
		*ptr++ = wbyte ;

	} while( wbyte != 0 ) ;

// list flag game
	Read( handle, &wbyte, 1 ) ;	// nb octets
	Read( handle, ListFlagGame, wbyte ) ;

	NewCube = 0 ;
	Read( handle, &NewCube, 1 ) ;

	Read( handle, &Chapitre, 1 ) ;

	Read( handle, &Comportement, 1 ) ;
	SaveComportement = Comportement ;
	Read( handle, &ListObjet[NUM_PERSO].LifePoint, 1 ) ;

	Read( handle, &NbGoldPieces, 2 ) ;

	Read( handle, &MagicLevel, 1 ) ;
	Read( handle, &MagicPoint, 1 ) ;
	Read( handle, &NbCloverBox, 1 ) ;

	Read( handle, &SceneStartX, 2 ) ;
	Read( handle, &SceneStartY, 2 ) ;
	Read( handle, &SceneStartZ, 2 ) ;
	Read( handle, &ListObjet[NUM_PERSO].Beta, 2 ) ;
	SaveBeta = ListObjet[NUM_PERSO].Beta ;

	Read( handle, &ListObjet[NUM_PERSO].GenBody, 1 ) ;

// tabholopos
	Read( handle, &wbyte, 1 ) ;
	Read( handle, TabHoloPos, wbyte ) ;

// others
	Read( handle, &Fuel, 1 ) ;

// used inventory
	Read( handle, &wbyte, 1 ) ;	// nb octets
	Read( handle, ListFlagInventory, wbyte ) ;

// others
	Read( handle, &NbFourLeafClover, 1 ) ;
	Read( handle, &Weapon, 2 ) ;


	Close( handle ) ;

	NumCube = -1 ;
	FlagChgCube = 3 ;
}

/*══════════════════════════════════════════════════════════════════════════*
	   █    ██▄ █ █▀▀▀█ █   █ ▀▀█▀▀       ██▄ █ █▀▀▀█ █▄ ▄█ █▀▀▀▀
	   ██   ██▀██ ██▀▀▀ ██  █   ██        ██▀██ ██▀▀█ ██▀ █ ██▀▀
	   ▀▀   ▀▀  ▀ ▀▀    ▀▀▀▀▀   ▀▀  ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawOneString( WORD x, WORD y, UBYTE *playername, WORD draw )
{
	WORD	x2, x0,y0,x1,y1 ;
//	UBYTE	string[256] ;

	x0 = x - LargeurMenu/2 ;
	x1 = x + LargeurMenu/2 ;

	y0 = y - HAUTEUR_STANDARD/2 ;
	y1 = y + HAUTEUR_STANDARD/2 ;

	// fond

	if( draw == 0 )
	{
		CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
		ShadeBox( x0, y0, x1, y1, 4 ) ;
	}
	if( draw == 1 )
	{
		DrawFire( x0, y0, x1, y1, COUL_INPUT_NAME&0xF0 ) ;
	}
	if( draw == 2 )
	{
		Box( x0, y0, x1, y1, 91 ) ;
	}

	// cadre
	DrawCadre( x0, y0, x1, y1 ) ;

	// text
	CoulFont( COUL_TEXT_MENU ) ;

//	if( playername[0] != 0 )
		Font( x - SizeFont( playername )/2, y-18, playername ) ;

	// flip

	CopyBlockPhys( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawSingleString( WORD x, WORD y, UBYTE *string )
{
	WORD	x2, x0,y0,x1,y1 ;
	WORD	sf ;

	sf = SizeFont( string ) ;

	x0 = x - sf/2 ;
	x1 = x + sf/2 ;

	y0 = y - HAUTEUR_STANDARD/2 ;
	y1 = y + HAUTEUR_STANDARD/2 ;

	// cadre
//	DrawCadre( x0, y0, x1, y1 ) ;

	// text
	CoulFont( 0 ) ;
	Font( x0 + 4, y-18 + 4, string ) ;
	CoulFont( COUL_TEXT_MENU ) ;
	Font( x0, y-18, string ) ;

	// flip
	CopyBlockPhys( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

UBYTE	TabLetter[NB_CAR_V][NB_CAR_H] =
{
	{ ' ','A','B','C','D','E','F','G','H','I','J','K','L','M' } ,
	{ '.','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' } ,
	{ '-','a','b','c','d','e','f','g','h','i','j','k','l','m' } ,
	{ '?','n','o','p','q','r','s','t','u','v','w','x','y','z' } ,
	{ '!','0','1','2','3','4','5','6','7','8','9',' ', 8 , 13 }   } ;

//	{ ' ','é','è','ç','à','ô','ê','â','î','8','9',' ',' ',' ' } ,

/*──────────────────────────────────────────────────────────────────────────*/

/*
WORD	SearchLetter( UBYTE car )
{
	WORD	x, y ;

	for( y=0; y<NB_CAR_H; y++ )
	{
		for( x=0; x<NB_CAR_H; x++ )
		{
			if( car == TabLetter[y][x] )
			{
				return TRUE ;
			}
		}
	}
	return FALSE ;
}
*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DrawOneLetter( WORD tabx, WORD taby, WORD draw )
{
	WORD	x2, x0,y0,x1,y1 ;
	UBYTE	letter[256] ;
	WORD	x, y ;

	letter[0] = TabLetter[taby][tabx] ;
	letter[1] = 0 ;

	x = 25 + (640/NB_CAR_H) * tabx ;
	y = 200 + (280/NB_CAR_V) * taby ;

	x0 = x - 40/2 ;
	x1 = x + 40/2 ;

	y0 = y - 50/2 ;
	y1 = y + 50/2 ;

	// fond

	if( draw )
	{
		Box( x0, y0, x1, y1, 91 ) ;
	}
	else
	{
		CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
		ShadeBox( x0, y0, x1, y1, 4 ) ;
	}

	// cadre
	DrawCadre( x0, y0, x1, y1 ) ;

	// text

	CoulFont( COUL_LETTER_INPUT ) ;

	Font( x - SizeFont( letter )/2, y-18, letter ) ;

	// flip

	CopyBlockPhys( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawAllLetters()
{
	WORD	x, y ;

	for( y=0; y<NB_CAR_V; y++ )
	{
		for( x=0; x<NB_CAR_H; x++ )
		{
			DrawOneLetter( x,y, FALSE ) ;
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

WORD	InputPlayerName( WORD nummess )
{
	WORD	flag = 1 ;
	UBYTE	string[256] ;
	WORD	poscarX = 0 , poscarY = 0 ;
	WORD	oldposcarX = -1 , oldposcarY ;
	WORD	len ;
	WORD	memoasciimode ;
	WORD	car = 0 ;
	WORD	retval = FALSE ;
	UBYTE	incar ;

try_again:

	CopyScreen( Screen, Log ) ;
	Flip() ;

	InitDial( 0 ) ;

	GetMultiText( nummess, string ) ;
	CoulFont( COUL_LETTER_INPUT ) ;
	Font( 320 - SizeFont(string)/2, 20, string ) ;
	CopyBlockPhys( 0,0,639,99 ) ;

	strcpy( PlayerName, "" ) ;
	DrawOneString( 320, 100, PlayerName, 1 ) ;
	DrawAllLetters() ;

	while( Key OR Fire ) ;

	memoasciimode = AsciiMode ;
	AsciiMode = TRUE ;

	while( GetAscii() != 0 ) ;

	while( Key != K_ESC )
	{
#ifdef	DEBUG_TOOLS
		CheckSavePcx() ;
#endif
		////////////////

		car = GetAscii() ;
		MyKey = Key ;
		MyJoy = Joy ;
		MyFire = Fire ;

		///////////////

		DrawOneString( 320, 100, PlayerName, 1 ) ;

		if( flag & 1 )
		{
			if( oldposcarX != -1 )
				DrawOneLetter( oldposcarX, oldposcarY, FALSE ) ;
			DrawOneLetter( poscarX, poscarY, TRUE ) ;
			oldposcarX = poscarX ;
			oldposcarY = poscarY ;
			flag &= ~1 ;
		}

		if( flag & 2 )
		{
			while( Fire ) ;
			flag &= ~2 ;
		}

		///////////////

		if( car == A_SPACE )
		{
			incar = TabLetter[poscarY][poscarX] ;
			if( (incar == 8) OR (incar == 13) )
			{
				if( incar == 8 )	car = A_BACKSPACE ;
				if( incar == 13 )	car = A_RETURN ;
			}
			else
			{
				len = strlen(PlayerName) ;
				if( len < MAX_SIZE_PLAYER_NAME )
				{
					PlayerName[len] = incar ;
					PlayerName[len+1] = 0 ;
					flag |= 2 ;
				}
			}
		}

		if( car == A_RIGHT )
		{
			poscarX++ ;
			if( poscarX == NB_CAR_H )	poscarX = 0 ;
			flag |= 1 ;
		}
		if( car == A_LEFT )
		{
			poscarX-- ;
			if( poscarX == -1 )	poscarX = NB_CAR_H - 1 ;
			flag |= 1 ;
		}
		if( car == A_DOWN )
		{
			poscarY++ ;
			if( poscarY == NB_CAR_V )	poscarY = 0 ;
			flag |= 1 ;
		}
		if( car == A_UP )
		{
			poscarY-- ;
			if( poscarY == -1 )	poscarY = NB_CAR_V - 1 ;
			flag |= 1 ;
		}

		///////////////

		len = strlen(PlayerName) ;

		if( car == A_BACKSPACE )
		{
			if( len > 0 )	PlayerName[len-1] = 0 ;
			else		car = 0 ;
		}

		if( car == A_RETURN )
		{
			if( len != 0 )	break ;
			else		car = 0 ;
		}

		if( (car != A_SPACE) AND (car != 0) )
		{
			car &= 255 ;

//			if( SearchLetter( car ) )
			if( (car>32) AND (car<255) )
			{
				if( len < MAX_SIZE_PLAYER_NAME )
				{
					PlayerName[len] = car ;
					PlayerName[len+1] = 0 ;
				}
			}
		}

		////////////////
	}

	if( car == A_RETURN )
	{
		if( FindPlayerFile() == TRUE )
		{
			nummess = 43 ;
			goto try_again ;
		}
		else	retval = TRUE ;
	}

#ifdef	TRACE
	AsciiMode = memoasciimode ;
#else
	AsciiMode = FALSE ;
#endif

	CopyScreen( Screen, Log ) ;
	Flip() ;

	return retval ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

#define	NB_GAME_CHOICE	6

WORD	ChoosePlayerName( WORD mess )
{
	WORD	flag = 1 ;
	UBYTE	*listplayername ;
	UBYTE	**ptrlist ;
	WORD	n, nb ;
	WORD	start = 0 ;
	WORD	select = 0 ;
	WORD	ys = 50 ;
	WORD	retval = 0 ;
	UBYTE	string[256] ;

	CopyScreen( Screen, Log ) ;
	Flip() ;

	listplayername = Malloc( MAX_PLAYER * (MAX_SIZE_PLAYER_NAME+1) ) ;
	ptrlist = (UBYTE**)Malloc( MAX_PLAYER * sizeof(UBYTE*) ) ;

	if( !listplayername OR !ptrlist )
	{
		TheEnd( NOT_ENOUGH_MEM, "Choose Player Name" ) ;
	}

	nb = PlayerGameList( ptrlist, listplayername ) ;

	if( !nb )	return FALSE ;

	InitDial( 0 ) ;

	DrawSingleString( 320, 40, (UBYTE*)GetMultiText( mess,string ) ) ;

	while( Key != K_ESC )
	{
		if( flag == 1 )
		{
			for( n=0; n<6; n++ )
			{
				if( n+start >= nb )	break ;
				if( (n+start) == select )
				{
					ys = 50+50+60*n ;
				}
				DrawOneString( 320, 50+50+60*n, ptrlist[n+start], (n+start) == select ) ;
			}
			while( Joy OR Fire )
				DrawOneString( 320, ys, ptrlist[select], 1 ) ;
			flag = 0 ;
		}

		DrawOneString( 320, ys, ptrlist[select], 1 ) ;

		if( Joy & J_DOWN )
		{
			if( select < (nb-1) )
			{
				DrawOneString( 320, ys, ptrlist[select], 0 ) ;
				select++ ;
				flag = 1 ;
			}

			if( select >= (start + 6) )
			{
				if( start < (nb-1) )
				{
					start++ ;
				}
			}
		}

		if( Joy & J_UP )
		{
			if( select > 0 )
			{
				DrawOneString( 320, ys, ptrlist[select], 0 ) ;
				select-- ;
				flag = 1 ;
			}
			if( select < start )
			{
				start-- ;
			}
		}

		if( Fire )
		{
			retval = 1 ;
			break ;
		}
	}

	strcpy( PlayerName, ptrlist[select] ) ;
	FindPlayerFile() ;

	Free( ptrlist ) ;
	Free( listplayername ) ;

	CopyScreen( Screen, Log ) ;
	Flip() ;

	return retval ;
}

/*══════════════════════════════════════════════════════════════════════════*
			    █▄ ▄█ █▀▀▀▀ ██▄ █ █   █
			    ██▀ █ ██▀▀  ██▀██ ██  █
			    ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DrawOneChoice( WORD x, WORD y, WORD type, WORD num, WORD select )
{
	WORD	x2, x0,y0,x1,y1 ;
	UBYTE	string[256] ;
	ULONG	volleft, volright ;

	x0 = x - LargeurMenu/2 ;
	x1 = x + LargeurMenu/2 ;

	y0 = y - HAUTEUR_STANDARD/2 ;
	y1 = y + HAUTEUR_STANDARD/2 ;

	// fond

	if( select )
	{
		switch( type )
		{
			case 1:	// music volume
				x2 = RegleTrois32( x0, x1, 255, MusicVolume ) ;
				DrawFire( x0, y0, x2, y1, 91&0xF0 ) ;
				Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;
				break ;

			case 2:	// sample volume
				x2 = RegleTrois32( x0, x1, 255, SampleVolume ) ;
				DrawFire( x0, y0, x2, y1, 91&0xF0 ) ;
				Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;
				break ;

			case 3:	// CD volume
				x2 = RegleTrois32( x0, x1, 255, CDVolume ) ;
				DrawFire( x0, y0, x2, y1, 91&0xF0 ) ;
				Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;
				break ;

			case 4:	// Line volume
				x2 = RegleTrois32( x0, x1, 255, LineVolume ) ;
				DrawFire( x0, y0, x2, y1, 91&0xF0 ) ;
				Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;
				break ;

			case 5:	// Master volume
				x2 = RegleTrois32( x0, x1, 255, MasterVolume ) ;
				DrawFire( x0, y0, x2, y1, 91&0xF0 ) ;
				Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;
				break ;
			default:
//				Box( x0, y0, x1, y1, COUL_SELECT_MENU ) ;
				DrawFire( x0, y0, x1, y1, COUL_SELECT_MENU&0xF0 ) ;
		}

		if( (type >= 1) AND (type<=5) )
		{
			// bruitons un peu la situation (violement)

			DoLoopMidi() ;

			if( (TimerRef-TimerSample) > 30 )
			{
				TimerSample = TimerRef ;
				Balance( rand()&127, 128, &volleft, &volright );
				HQ_MixSample( 34+((rand()&7)>0)*92, 0x1000, 1, volleft, volright ) ;
//				HQ_MixSample( Rnd( 50 ), 0x1000, 1, volleft, volright ) ;
			}
		}

	}
	else
	{
		CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
		ShadeBox( x0, y0, x1, y1, 4 ) ;
	}

	// cadre
	DrawCadre( x0, y0, x1, y1 ) ;

	// text

	CoulFont( COUL_TEXT_MENU ) ;
	GetMultiText( num, string ) ;

	Font( x - SizeFont( string )/2, y-18, string ) ;

	// flip

	CopyBlockPhys( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawGameMenu( UWORD *ptrmenu, WORD justone )
{
	WORD	n, y ;
	WORD	nb ;
	WORD	selected, type, num ;

	selected = *ptrmenu++ ;

	nb = *ptrmenu++ ;

	y = *ptrmenu++ ;

	ptrmenu++ ;	// dial num

	if( y == 0 )	// depuis le haut
	{
		y = HAUTEUR_STANDARD/2 + 10 ;
	}
	else		// centre sur y
	{
		y -= ( HAUTEUR_STANDARD*nb + (nb-1)*MENU_SPACE ) / 2 ;
	}

	for( n=0; n<nb; n++ )
	{
		type = *ptrmenu++ ;
		num = *ptrmenu++ ;

		if( justone )
		{
			if( n == selected )
			{
				DrawOneChoice( 320, y, type, num, TRUE ) ;
			}
		}
		else
		{
			DrawOneChoice( 320, y, type, num, (n==selected) ) ;
		}

		y += HAUTEUR_STANDARD + MENU_SPACE ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void	SetVolumes(LONG Music, LONG Sample, LONG Line, LONG CD, LONG Master )
{
	static	ULONG	lasttimer ;


	while (lasttimer == TimerRef ) ;
	lasttimer == TimerRef ;

	if (!MixMusic)
	{
		MaxVolume = Music ;
		VolumeMidi( 100 ) ;
	}

	MixerChangeVolume(	Sample,
				Music,
				CD,
				Line,
				Master ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

WORD	DoGameMenu( UWORD *ptrmenu )
{
	WORD	flag = 1 ;
	WORD	nb , selected ;
	LONG	flagVol = 0 ;
	UWORD	*ptr ;
	ULONG	chrono ;

	Load_HQR( PATH_RESSOURCE"ress.hqr", BufSpeak, RESS_INIT_PLASMA ) ;

	selected = ptrmenu[0] ;
	nb = ptrmenu[1] ;

	chrono = TimerRef ;

	while( TRUE )
	{

#ifdef	DEBUG_TOOLS
		CheckSavePcx() ;
#endif
/*	okazou
		if( Key == K_S )
		{
			Save( "f:\\projet\\lba\\datas\\initplas.ma", BufSpeak, 16*1024 ) ;
		}
*/

		// rustine credits ...
		if( ptrmenu == GameMainMenu )
		{
#ifdef	DEMO
			if( (TimerRef - chrono > 50 * 60)
			OR ((Key == K_C) AND (Fire & F_SHIFT)) )
#else
			if( (TimerRef - chrono > 50 * (60 * 3 + 53) )
			OR ((Key == K_C) AND (Fire & F_SHIFT)) )
#endif
			{
				chrono = TimerRef ;
				return 9999 ;
			}
		}

		MyJoy = Joy ;

		if( MyJoy & J_DOWN )
		{
			selected++ ;
			if( selected == nb )	selected = 0 ;
			flag = 1 ;
		}

		if( MyJoy & J_UP )
		{
			selected-- ;
			if( selected < 0 )	selected = nb - 1 ;
			flag = 1 ;
		}

		switch( ptrmenu[4 + 0 + selected*2] ) // type
		{
			case 0:	// text normal
				break ;

			case 1: // volume music

				if ( flag ) flagVol = 3 ;

				if( (MyJoy & J_LEFT) )
				{
					MusicVolume -= 4 ;
					flagVol = 3 ;
				}
				if( (MyJoy & J_RIGHT) )
				{
					MusicVolume += 4 ;
					flagVol = 3 ;
				}
				break ;

			case 2: // volume sample

				if ( flag ) flagVol = 3 ;

				if( (MyJoy & J_LEFT) )
				{
					SampleVolume -= 4 ;
					flagVol = 3 ;
				}
				if( (MyJoy & J_RIGHT) )
				{
					SampleVolume += 4 ;
					flagVol = 3 ;
				}
				break ;

			case 3: // volume cd

				if ( flag ) flagVol = 4 ;

				if( (MyJoy & J_LEFT) )
				{
					CDVolume -= 4 ;
					flagVol = 4 ;
				}
				if( (MyJoy & J_RIGHT) )
				{
					CDVolume += 4 ;
					flagVol = 4 ;
				}
				break ;

			case 4: // volume Line

				if ( flag ) flagVol = 5 ;

				if( (MyJoy & J_LEFT) )
				{
					LineVolume -= 4 ;
					flagVol = 5 ;
				}
				if( (MyJoy & J_RIGHT) )
				{
					LineVolume += 4 ;
					flagVol = 5 ;
				}
				break ;

			case 5: // volume master

				if ( flag ) flagVol = 2 ;

				if( (MyJoy & J_LEFT) )
				{
					MasterVolume -= 4 ;
					flagVol = 2 ;
				}
				if( (MyJoy & J_RIGHT) )
				{
					MasterVolume += 4 ;
					flagVol = 2 ;
				}
				break ;
		}

		if( flag == 1 )
		{
			ptrmenu[0] = selected ;
			DrawGameMenu( ptrmenu, FALSE ) ;
			while( Joy OR Fire OR Key )
				DrawGameMenu( ptrmenu, TRUE ) ;
			flag = 0 ;
		}
		else
		{
//			Vsync() ;
			if( flagVol )
			{
				if( MusicVolume > 255 )	MusicVolume = 255 ;
				if( SampleVolume > 255 )SampleVolume = 255 ;
				if( LineVolume > 255 )	LineVolume = 255 ;
				if( CDVolume > 255 )	CDVolume = 255 ;
				if( MasterVolume > 255 )MasterVolume = 255 ;

				if( MusicVolume < 0 )	MusicVolume = 0 ;
				if( SampleVolume < 0 )	SampleVolume = 0 ;
				if( LineVolume < 0 )	LineVolume = 0 ;
				if( CDVolume < 0 )	CDVolume = 0 ;
				if( MasterVolume < 0 )	MasterVolume = 0 ;

				switch ( flagVol )
				{
					case 3:
					SetVolumes(MusicVolume, SampleVolume, 0, 0, MasterVolume ) ;
					break;
					case 4:
					SetVolumes(0, SampleVolume, 0, CDVolume, MasterVolume ) ;
					break;
					case 5:
					SetVolumes(0, SampleVolume, LineVolume, 0, MasterVolume ) ;
					break;
					default:
					SetVolumes(MusicVolume, SampleVolume, LineVolume, CDVolume, MasterVolume ) ;
				}
			}
			DrawGameMenu( ptrmenu, TRUE ) ;
			flagVol = 0 ;
			flag = 0 ;
		}

		if( Fire & F_RETURN
		OR  Fire & F_SPACE )
		{
			return ptrmenu[4 + 1 + selected*2] ; // num mess
		}

//		if( Key == K_ESC )	break ;
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

LONG	OptionsMenu()
{
	WORD select ;
	WORD flag = 0 ;

	CopyScreen( Log, Screen ) ;

	HQ_StopSample() ;
#ifdef	CDROM
	PlayCdTrack( 9 ) ;
#else
	PlayMidiFile( 9 ) ;
#endif

	while( !flag )
	{
		select = DoGameMenu( GameOptionMenu ) ;

		switch( select )	// num mess
		{
/*			case -1: // esc
				flag = 1 ;
				break ;
*/
			case 15: // retour au jeu
			case 26: // menu prec.
				flag = 1 ;
				break ;

			case 47:
				CopyScreen( Screen, Log ) ;
				Flip() ;
				AdvancedOptions() ;
				break ;

			case 30:
				CopyScreen( Screen, Log ) ;
				Flip() ;
				VolumeOptions() ;
				break ;

			case 46:
				CopyScreen( Screen, Log ) ;
				Flip() ;
				SavedGameManagement() ;
				break ;
		}
	}
	CopyScreen( Screen, Log ) ;
	Flip() ;

	return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	VolumeOptions()
{
	WORD select ;
	WORD flag = 0 ;

	CopyScreen( Log, Screen ) ;

#ifdef	CDROM
	PlayAllMusic( 9 ) ;	// he oui on lance les 2
#else
	PlayMidiFile( 9 ) ;
#endif

	TimerSample = TimerRef ;

	while( !flag )
	{
		select = DoGameMenu( GameVolumeMenu ) ;

		switch( select )	// num mess
		{
/*			case -1: // esc
				flag = 1 ;
				break ;
*/
			case 26: // quitter
#ifdef	CDROM
				FadeMusicMidi( 1 ) ;
#endif
				SetVolumes(MusicVolume, SampleVolume, LineVolume, CDVolume, MasterVolume ) ;
				flag = 1 ;
				break ;

			case 16:
				SetVolumes(MusicVolume, SampleVolume, LineVolume, CDVolume, MasterVolume ) ;
				WriteVolumeSettings() ;
				GameVolumeMenu[0] = 0 ; // selected
				break ;
		}
	}
	CopyScreen( Screen, Log ) ;
	Flip() ;
}
/*──────────────────────────────────────────────────────────────────────────*/

void	SavedGameManagement()
{
	WORD	select ;
	WORD	flag = 0 ;
	UBYTE	memoplayername[MAX_SIZE_PLAYER_NAME] ;
	UBYTE	memogamepath[_MAX_PATH] ;

	CopyScreen( Log, Screen ) ;

	strcpy( memoplayername, PlayerName ) ;
	strcpy( memogamepath, GamePathname ) ;

	while( !flag )
	{
		select = DoGameMenu( SavedGameMenu ) ;

		switch( select )	// num mess
		{
			case 26: // quitter
				flag = 1 ;
				break ;

			case 41: // copier
				if( ChoosePlayerName( 41 ) )
				{
					UBYTE	*ptr,*ptrs ;
					LONG	size ;
					UBYTE	num ;
					UBYTE	n,c ;
					LONG	handle ;

					ptrs = ptr = LoadMalloc( GamePathname ) ;
					size = FileSize( GamePathname ) ;
					num = *ptr++ ;		// num version
					n = strlen( ptr ) + 1 ; // size player name

					if( InputPlayerName( 44 ) )
					{
						do
						{
							strcpy( GamePathname, PATH_RESSOURCE"S" ) ;
							strcat( GamePathname, Itoa( Rnd(10000) ) ) ;
							strcat( GamePathname, ".LBA" ) ;
						}
						while( FileSize( GamePathname ) != 0 ) ;

						handle = OpenWrite( GamePathname ) ;
						if( handle )
						{
							Write( handle, &num, 1 ) ;
							Write( handle, PlayerName, strlen(PlayerName)+1 ) ;
							Write( handle, ptrs+n+1, size-n-1 ) ;
							Close( handle ) ;
						}
					}
					Free( ptrs ) ;
				}
				CopyScreen( Screen, Log ) ;
				break ;

			case 45: // detruire
				if( ChoosePlayerName( 45 ) )
				{
					CopyScreen( Screen, Log ) ;

					DrawOneString( 320, 80, PlayerName, 2 ) ;

					SavedConfirmMenu[0] = 0 ;
					select = DoGameMenu( SavedConfirmMenu ) ;

					switch( select )	// num mess
					{
						case 48: // detruire
							Delete( GamePathname ) ;
							break ;
					}
				}
				SavedGameMenu[0] = 0 ;
				CopyScreen( Screen, Log ) ;
				Flip() ;
//				flag = 1 ;
				break ;
		}
	}
	CopyScreen( Screen, Log ) ;
	Flip() ;

	strcpy( GamePathname, memogamepath ) ;
	strcpy( PlayerName, memoplayername ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	AdvancedOptions()
{
	WORD select ;
	WORD flag = 0 ;

	CopyScreen( Log, Screen ) ;

	while( !flag )
	{
		select = DoGameMenu( GameAdvancedMenu ) ;

		switch( select )	// num mess
		{
			case 26:
				flag = 1 ;
				break ;

			case 4: // agressif auto
				GameAdvancedMenu[7] = 2 ;
				CombatAuto = FALSE ;
				break ;

			case 2: // agressif manuel
				GameAdvancedMenu[7] = 4 ;
				CombatAuto = TRUE ;
				break ;
			case 31: // poly high
				GameAdvancedMenu[9] = 131 ;
				SetFillDetails( 1 ) ;
				break ;
			case 131: // poly medium
				GameAdvancedMenu[9] = 231 ;
				SetFillDetails( 0 ) ;
				break ;
			case 231: // poly low
				GameAdvancedMenu[9] = 31 ;
				SetFillDetails( 2 ) ;
				break ;

			case 32: // ombres full
				GameAdvancedMenu[11] = 132 ;
				Shadow = 1 ;
				break ;
			case 132: // ombres obj
				GameAdvancedMenu[11] = 232 ;
				Shadow = 0 ;
				break ;
			case 232: // pas ombres
				GameAdvancedMenu[11] = 32 ;
				Shadow = 2 ;
				break ;

			case 33: // zoom hard
				GameAdvancedMenu[13] = 233 ;
				SceZoom = 0 ;
				break ;

			case 233: // pas zoom
				GameAdvancedMenu[13] = 33 ;
				SceZoom = 1 ;
				break ;

/*			case 233: // zoom soft
				GameAdvancedMenu[13] = 33 ;
				SceZoom = 2 ;
				break ;
*/
		}
	}
	CopyScreen( Screen, Log ) ;
	Flip() ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

LONG	MainGameMenu()
{
	WORD select ;
	WORD flag = 0 ;

	HQ_StopSample() ;


/*	for( flag=0; flag<50*320; flag++ )
	{
		BufSpeak[flag] = 0 ;
	}
	flag = 0;
*/

	CopyScreen( Log, Screen ) ;

	while( !flag )
	{
		InitDial( 0 )		;//	SYS

#ifdef	CDROM
		PlayCdTrack( 9 ) ;
#else
		PlayMidiFile( 9 ) ;
#endif
		HQ_StopSample() ;

		GetMultiText( 49, PleaseWait ) ;

		select = DoGameMenu( GameMainMenu ) ;
		switch( select )	// num mess
		{
			case 9999:	// menu demo

				MenuDemo() ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_MENU_PCR ) ;
				CopyScreen( Screen, Log ) ;
				SetBlackPal() ;
				Flip() ;
				FadeToPal( PtrPal ) ;
				break ;

//			case -1: // esc
			case 22: // quitter
				flag = 1 ;
				break ;

			case 20: // newgame

				if( !InputPlayerName( 42 ) )	break ;

				do
				{
					strcpy( GamePathname, PATH_RESSOURCE"S" ) ;
					strcat( GamePathname, Itoa( Rnd(10000) ) ) ;
					strcat( GamePathname, ".LBA" ) ;
				}
				while( FileSize( GamePathname ) != 0 ) ;

				InitGame( 1, 0 ) ;
				Introduction() ;
				if( MainLoop() )
				{
#ifdef	DEMO
					PlayMidiFile( 6 ) ;
					Credits() ;
					TheEnd( PROGRAM_OK, "* End of Demo version." ) ;
#else
					Credits() ;
					PlayAnimFla( "The_End" ) ;
					Cls() ;
					Flip() ;
					Palette( PtrPal ) ;
#endif
				}

				CopyScreen( Log, Screen ) ;
				while( Key OR Fire ) ; // provisoire
				break ;

			case 21: // load

				if( !ChoosePlayerName( 21 ) ) break ;

				InitGame( -1, 0 ) ;
				Introduction() ;
				if( MainLoop() )
				{
#ifdef	DEMO
					PlayMidiFile( 6 ) ;
					Credits() ;
					TheEnd( PROGRAM_OK, "* End of Demo version." ) ;
#else
					Credits() ;
					PlayAnimFla( "The_End" ) ;
					Cls() ;
					Flip() ;
					Palette( PtrPal ) ;
#endif
				}
				CopyScreen( Log, Screen ) ;
				while( Key OR Fire ) ; // provisoire
				break ;

			case 23:
				CopyScreen( Screen, Log ) ;
				Flip() ;
				GameOptionMenu[5] = 26 ; // retour prec
				OptionsMenu() ;
				break ;
		}
	}
	return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

LONG	QuitMenu()
{
	WORD select ;
	LONG memoflagspeak ;

	CopyScreen( Log, Screen ) ;
	HQ_StopSample() ;

	while( TRUE )
	{
		memoflagspeak = FlagSpeak ;
		FlagSpeak = FALSE ;
		InitDial( 0 )		;//	SYS
		FlagSpeak = memoflagspeak ;

		select = DoGameMenu( GameQuitMenu ) ;

		InitDial( START_FILE_ISLAND+Island )	;//	SYS

		switch( select )	// num mess
		{
			case 28: // continue
				return FALSE ;

			case 27: // abandonner
				return TRUE ;
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*
       █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀       █▀▀▀▀ █   █ █▀▀▀█  █    █▀▀▀▀ █▀▀▀▀
       ██ ▀█ ██▀▀█ ██▀ █ ██▀▀        ██    ██▀▀█ ██  █  ██   ██    ██▀▀
       ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀  ▀▀   ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
/*
UWORD	GameChoiceMenu[] = {
			0,	// selected
			7,	// nb entries
			0,	// y from top.
			0,	// .dia num

			0, 0,
			0, 1,
			0, 2,
			0, 3,
			0, 4,
			0, 5,
			0, 6	}	;
*/

/*──────────────────────────────────────────────────────────────────────────*/

void	GameAskChoice( WORD nummess )
{
	WORD n ;

	CopyScreen( Log, Screen ) ;

	GameChoiceMenu[0] = 0 ;
	GameChoiceMenu[1] = GameNbChoices ;
	GameChoiceMenu[2] = 0 ;
	GameChoiceMenu[3] = START_FILE_ISLAND+Island ;
	for(n=0; n<GameNbChoices; n++ )
	{
		GameChoiceMenu[4+n*2+0] = 0 ;			// type
		GameChoiceMenu[4+n*2+1] = GameListChoice[n] ;	// mess
	}

	MyDial( nummess ) ;
	DoGameMenu( GameChoiceMenu ) ;
	GameChoice = GameListChoice[GameChoiceMenu[0]] ; // ret num mess

#ifdef	CDROM
	NumObjSpeak = NUM_PERSO ;
	Speak( GameChoice ) ;
	while( TestSpeak() AND (Key!=K_ESC) )	;// Wait until silence
	StopSpeak() ;//	Security
#endif
}

/*══════════════════════════════════════════════════════════════════════════*
	     █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀       █▀▀▀█ █   █ █▀▀▀▀ █▀▀▀█
	     ██ ▀█ ██▀▀█ ██▀ █ ██▀▀        ██  █ ██ ▄▀ ██▀▀  ██▀█▀
	     ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀   ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	GameOver()
{
	WORD	*ptr3do ;
	LONG	x0,y0,x1,y1 ;
	LONG	zoom ;
	LONG	alpha, beta ;
	ULONG	chrono ;

	TestRestoreModeSVGA( FALSE ) ;
	Palette( PtrPal ) ;

	CopyScreen( Log, Screen ) ;

	ptr3do = LoadMalloc_HQR( PATH_RESSOURCE"ress.hqr", RESS_GAME_OVER ) ;
	if( !ptr3do )	return ;
	PatchObjet( ptr3do ) ;

	HQ_StopSample() ;
	FadeMusicMidi( 1 ) ;

	// lance un sample ou music de game over ??

	SetProjection( 320,240, 128,200,200 ) ;

	alpha = 60 ;
	beta = 0 ;
	zoom = 50000 ;

	x0 = 320-200 ;
	y0 = 240-120 ;
	x1 = 320+200-1 ;
	y1 = 240+120-1 ;

	chrono= TimerRef ;

	SetClip(  x0, y0, x1, y1 ) ;
	while( (Key != K_ESC) AND (TimerRef-chrono<=10*50) )
	{
		zoom = BoundRegleTrois( 40000, 3200, 10*50, TimerRef-chrono ) ;
		beta = RegleTrois32( 1, 1024, 2*50, (TimerRef-chrono)%(2*50) ) ;

		CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;

		SetFollowCamera( 0,0,0, 0, -beta,0, zoom ) ;
		AffObjetIso(	0,0,0, 0,0,0, ptr3do ) ;

		CopyBlockPhys( x0,y0, x1,y1 ) ;
	}
	HQ_MixSample( 37, 0x1000+Rnd(2000)-(2000/2), 1, 128, 128 ) ;
	CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
	SetFollowCamera( 0,0,0, 0,0,0, zoom ) ;
	AffObjetIso(	0,0,0, 0,0,0, ptr3do ) ;
	CopyBlockPhys( x0,y0, x1,y1 ) ;

	chrono = TimerRef + 50 * 3 ;
	while( (TimerRef < chrono) AND (Key!=K_ESC) ) ;

	UnSetClip() ;
	Free( ptr3do ) ;

	CopyScreen( Screen, Log ) ;
	Flip() ;

	Init3DGame() ;
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▀ █▀▀▀█ █▀▀▀▀ █▀▀▀▄  █    ▀▀█▀▀ ██▀▀▀
		   ██    ██▀█▀ ██▀▀  ██  █  ██     ██  ▀▀▀▀█
		   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀   ▀▀     ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	Credits()
{
	LONG	memoflagspeak ;
	LONG	memoshadow ;

	memoflagspeak = FlagSpeak ;
	memoshadow = Shadow ;

	FlagSpeak = FALSE ;
	FlagCredits = TRUE ;
	Shadow = 0 ;

	InitGame( 1, 0 ) ;

	NewCube = NumCube = 119 ;

	MainLoop() ;

	FlagCredits = FALSE ;
	FlagSpeak = memoflagspeak ;
	Shadow = memoshadow ;

	Cls() ;
	Flip() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	RessPict( WORD num )
{
	SetBlackPal() ;
	Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, num ) ;		// pcr
	CopyScreen( Screen, Log ) ;
	Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, num+1 ) ;	// pcp
	Flip() ;
	FadeToPal( PalettePcx ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	TimerPause( WORD nbsec )
{
	ULONG	memotimer ;

	memotimer = TimerRef ;
	while( TimerRef < (memotimer+50*nbsec) )
	{
		if( Key OR Fire OR Joy )	return ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void	AdelineLogo()
{
	PlayMidiFile( 31 ) ;

	Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_LOGO_PCR ) ;
	CopyScreen( Screen, Log ) ;
	Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_LOGO_PAL ) ;
	WhiteFade() ;
	Flip() ;
	FadeWhiteToPal( PalettePcx ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	MenuDemo()	// lancée toutes les 4 minutes
{
	Credits() ;
	if( Key OR Fire OR Joy )	return ;

	PlayAnimFla( "DRAGON3" ) ;
	if( Key OR Fire OR Joy )	return ;

	RessPict( RESS_TWINSUN_PCR ) ;
	TimerPause( 3 ) ;
	if( Key OR Fire OR Joy )	return ;
	FadeToBlack( PalettePcx ) ;

	RessPict( RESS_INTRO_2_PCR ) ;
	TimerPause( 3 ) ;
	if( Key OR Fire OR Joy )	return ;
	FadeToBlack( PalettePcx ) ;

	RessPict( RESS_INTRO_3_PCR ) ;
	TimerPause( 3 ) ;
	if( Key OR Fire OR Joy )	return ;
	FadeToBlack( PalettePcx ) ;

	PlayAnimFla( "BATEAU" ) ;
	if( Key OR Fire OR Joy )	return ;

	if( Version_US )
		RessPict( RESS_BUMPER_PCR ) ;
	else
		RessPict( RESS_BUMPER2_PCR ) ;

	TimerPause( 3 ) ;
	if( Key OR Fire OR Joy )	return ;
	FadeToBlack( PalettePcx ) ;

	AdelineLogo() ;
	TimerPause( 3 ) ;
	if( Key OR Fire OR Joy )	return ;
	FadeToBlack( PalettePcx ) ;
}

