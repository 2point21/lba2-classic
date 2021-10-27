#include	"c_extern.h"

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
     Û    ÛÛÜ Û Û฿฿฿฿ Û฿฿฿Û Û   Û ÛÛ฿฿฿ ฿฿Û฿฿       Û฿฿฿Ü  Û    ÛÛ฿฿฿ Û฿฿฿Û
     ÛÛ   ÛÛ฿ÛÛ ÛÛ    ÛÛ฿Û฿ ÛÛ  Û ฿฿฿฿Û   ÛÛ        ÛÛ  Û  ÛÛ   ฿฿฿฿Û ÛÛ฿฿฿
     ฿฿   ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿   ฿฿  ฿฿฿฿฿ ฿฿฿฿   ฿฿   ฿฿฿฿฿ ฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	InitIncrustDisp(	WORD type,
				WORD num,
				WORD x, WORD y,
				WORD info,
				WORD move,
				WORD timeout	)
{
	T_INCRUST_DISP	*ptrdisp ;
	WORD	n ;

	ptrdisp = ListIncrustDisp ;

	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		if( ptrdisp->Num == -1 )	/* slot libre */
		{
			ptrdisp->Num = num ;

			ptrdisp->X = x ;
			ptrdisp->Y = y ;

			ptrdisp->Type = type ;
			ptrdisp->Info = info ;

			ptrdisp->Move = move ;

			ptrdisp->TimerEnd = TimerRef + 50 * timeout ;

			return ;
		}
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ChangeIncrustPos( WORD xp0, WORD yp0, WORD xp1, WORD yp1 )
{
	T_INCRUST_DISP	*ptrdisp ;
	WORD	n ;
	WORD	dx, dy ;

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

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

WORD	LastNumBulle = -1 ;
WORD	SpriteBulle = 91 ;

void	DrawBulle( WORD numobj )
{
	LONG	dx, dy ;

	ProjettePoint(	ListObjet[numobj].PosObjX - WorldXCube,
			ListObjet[numobj].PosObjY + ListObjet[numobj].Ymax - WorldYCube,
			ListObjet[numobj].PosObjZ - WorldZCube ) ;

	if( LastNumBulle != numobj )
	{
		SpriteBulle ^= 1 ;
		LastNumBulle = numobj ;
	}

	GetDxDyGraph( 0, &dx, &dy, HQR_Get(HQRPtrSpriteExtra, SpriteBulle ) ) ;

	if( SpriteBulle == 90 )	ScreenXmin = Xp + 10 ;
	else			ScreenXmin = Xp - 10 - dx ;

	ScreenYmin = Yp - 20 ;


	ScreenXmax = ScreenXmin + dx -1 ;
	ScreenYmax = ScreenYmin + dy -1 ;

	SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;

	AffGraph( 0, ScreenXmin, ScreenYmin, HQR_Get(HQRPtrSpriteExtra, SpriteBulle) ) ;
	if( ClipXmin <= ClipXmax
	AND ClipYmin <= ClipYmax )
	{
		CopyBlockPhys( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
	}

	UnSetClip() ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
