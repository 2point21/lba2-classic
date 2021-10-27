#include	"c_extern.h"

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
     Û    ÛÛÜ Û Ûßßßß ÛßßßÛ Û   Û ÛÛßßß ßßÛßß       ÛßßßÜ  Û    ÛÛßßß ÛßßßÛ
     ÛÛ   ÛÛßÛÛ ÛÛ    ÛÛßÛß ÛÛ  Û ßßßßÛ   ÛÛ        ÛÛ  Û  ÛÛ   ßßßßÛ ÛÛßßß
     ßß   ßß  ß ßßßßß ßß  ß ßßßßß ßßßßß   ßß  ßßßßß ßßßß   ßß   ßßßßß ßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

S32	InitIncrustDisp(	S16 type,
				S16 num,
				S32 x, S32 y,
				S16 info,
				S16 move,
				S32 timeout	)
{
	T_INCRUST_DISP	*ptrdisp ;
	S32	n ;

	// Li‚s aux objets de l'inventaire changeant d'‚tat
	if( (type&0xFF)==INCRUST_INVENTORY )
	{
		num = TabInv[num].TabObj3D[ TabInv[num].IdObj3D ] ;
	}

	ptrdisp = ListIncrustDisp ;

	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		if( ptrdisp->Num == -1	/* slot libre */
		OR  (ptrdisp->Num==num AND ptrdisp->Type==type) )// same object
		{
			if( ptrdisp->Num==-1
			OR  ptrdisp->Type!=type )
			{
				ptrdisp->X = (S16)x ;
				ptrdisp->Y = (S16)y ;
			}

			if( (type&0xff)==INCRUST_CMPT )
			{
				// ATTENTION: Grosse Magouille: les compteurs sont tjs affich‚s
				// en y=20, ceci permet d'utiliser le Y pour memoriser la
				// valeur courante du compteur (cf FlagAnimWhoSpeak)
				ptrdisp->Y = num ;
			}

			ptrdisp->Type = type ;
			ptrdisp->Info = info ;

			ptrdisp->Move = move ;

			ptrdisp->TimerEnd = TimerRefHR + 1000 * timeout ;

			ptrdisp->Num = num ;
			PosObjIncrust( ptrdisp, n ) ;

			return n ;
		}
	}

	return -1 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	PosObjIncrust( T_INCRUST_DISP *ptrdisp, S32 num )
{
	// au cas ou on ait plusieurs objets 3D tournant en mˆme temps !
	S32	x = 10 ;
	S32	n ;
	S32	type ;
	T_INCRUST_DISP *listdisp = ListIncrustDisp ;

	type = (ptrdisp->Type&0xFF) ;

	if( type==INCRUST_INVENTORY
	OR  type==INCRUST_OBJ )
	{
		for( n=0; n<MAX_INCRUST_DISP; n++, listdisp++ )
		{
			if( n != num
			AND listdisp->Num != -1 )
			{
				type = (listdisp->Type&0xFF) ;

				if( type==INCRUST_INVENTORY
				OR  type==INCRUST_OBJ )
				{
					x += 70 ;
				}
			}
		}

		ptrdisp->Y = 10 	;
		ptrdisp->X = (S16)x 	;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	ChangeIncrustPos( S32 xp0, S32 yp0, S32 xp1, S32 yp1 )
{
	T_INCRUST_DISP	*ptrdisp ;
	S32	n ;
	S32	dx, dy ;

	dx = xp1 - xp0 ;
	dy = yp1 - yp0 ;

	ptrdisp = ListIncrustDisp ;

	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		if( ptrdisp->Type == INCRUST_NUM )
		{
			ptrdisp->X += dx ;
			ptrdisp->Y += dy ;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	ClearIncrusts( S32 type )
{
	T_INCRUST_DISP	*ptrdisp ;
	S32	n ;

	ptrdisp = ListIncrustDisp ;

	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		if( ptrdisp->Type==type )
		{
			ptrdisp->Num = -1 ;// libŠre slot
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

S32	LastNumBulle = -1 ;
S32	SpriteBulle = SPRITE_BULLE_RIGHT ;

#define	DX_BULLE	45	// Valeur saisie dans Volgraf

void	DrawBulle( S32 numobj )
{
	PtrProjectPoint(	ListObjet[numobj].Obj.X,
				ListObjet[numobj].Obj.Y + ListObjet[numobj].YMax,
				ListObjet[numobj].Obj.Z ) ;

	if( LastNumBulle != numobj )
	{
		SpriteBulle ^= 1 ;
		LastNumBulle = numobj ;
	}

	if( SpriteBulle == SPRITE_BULLE_LEFT )	SpriteX = Xp + 10 ;
	else			SpriteX = Xp - 10 - DX_BULLE ;

	SpriteY = Yp - 20 ;

	PtrAffGraph( SpriteX, SpriteY, SpriteBulle ) ;
	SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

	if( ClipXMin <= ClipXMax
	AND ClipYMin <= ClipYMax )
	{
		if ( FlagDisplayText )
			BoxStaticAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		else    BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
	}

	UnsetClip() ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
