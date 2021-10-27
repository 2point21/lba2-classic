#include	"defines.h"

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		Û฿฿฿฿ Û      Û    Û฿฿฿Û       Û฿฿Û  Û฿฿฿Û ฿Ü Ü฿
		ÛÛ฿฿  ÛÛ     ÛÛ   ÛÛ฿฿฿       ÛÛ฿฿Û ÛÛ  Û  Ü฿Ü
		฿฿    ฿฿฿฿฿  ฿฿   ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿   ฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

extern	UBYTE	*Screen ;		/* background clean */


typedef	struct
{
	WORD	x0 ;
	WORD 	y0 ;
	WORD	x1 ;
	WORD	y1 ;	}	T_PHYSBOX ;


T_PHYSBOX	NewListBox[MAX_OBJETS+MAX_EXTRAS] ;
T_PHYSBOX	OptListBox[(MAX_OBJETS+MAX_EXTRAS)*2] ;

WORD	NbPhysBox = 0 ;
WORD	NbOptPhysBox = 0 ;

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddOptBox( WORD x0, WORD y0, WORD x1, WORD y1 )
{
	T_PHYSBOX	*ptr ;
	WORD		cx0, cy0, cx1, cy1 ;
	WORD		n ;
	LONG		totalsurface ;
	LONG		surfacecommune ;
	LONG		surfacetest ;

	surfacetest = (x1-x0) * (y1-y0) ;
	ptr = OptListBox ;
	for( n=0; n<NbOptPhysBox; n++, ptr++ )
	{
		cx0 = min(ptr->x0,x0) ;
		cx1 = max(ptr->x1,x1) ;
		cy0 = min(ptr->y0,y0) ;
		cy1 = max(ptr->y1,y1) ;

		surfacecommune = (LONG)(cx1-cx0) * (cy1-cy0) ;
		totalsurface = surfacetest + (ptr->x1-ptr->x0) * (ptr->y1-ptr->y0) ;

		if( surfacecommune < totalsurface )
		{
			ptr->x0 = cx0 ;
			ptr->y0 = cy0 ;
			ptr->x1 = cx1 ;
			ptr->y1 = cy1 ;
			return ;
		}
	}
	ptr->x0 = x0 ;
	ptr->y0 = y0 ;
	ptr->x1 = x1 ;
	ptr->y1 = y1 ;
	NbOptPhysBox++ ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddPhysBox( WORD x0, WORD y0, WORD x1, WORD y1 )
{
	T_PHYSBOX	*ptr ;

	if( x0 < 0 )	x0 = 0 ;
	if( x1 > 639 )	x1 = 639 ;
	if( y0 < 0 )	y0 = 0 ;
	if( y1 > 479 )	y1 = 479 ;
	if( x1 < x0 )	return ;
	if( y1 < y0 )	return ;

	ptr = &NewListBox[NbPhysBox] ;
	ptr->x0 = x0 ;
	ptr->y0 = y0 ;
	ptr->x1 = x1 ;
	ptr->y1 = y1 ;
	NbPhysBox++ ;

	AddOptBox( x0, y0, x1, y1 ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	FlipOptList()
{
	WORD		n ;
	T_PHYSBOX	*ptr ;

	/* regenere optlistbox pour clear log
				puis phys a la prochaine boucle*/
	ptr = NewListBox ;
	NbOptPhysBox = 0 ;
	for( n=0; n<NbPhysBox; n++,ptr++ )
	{
		AddOptBox(	ptr->x0, ptr->y0, ptr->x1, ptr->y1 ) ;
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	FlipBoxes()
{
	WORD		n ;
	T_PHYSBOX	*ptr ;

	/* copie liste optimise vers phys */
	ptr = OptListBox ;
	for( n=0; n<NbOptPhysBox; n++,ptr++ )
	{
		CopyBlockPhys( ptr->x0, ptr->y0, ptr->x1, ptr->y1 ) ;
	}

	FlipOptList() ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ClsBoxes()
{
	WORD	n ;
	T_PHYSBOX	*ptr ;

	/* efface ecran logique (copy depuis Screen org) */
	ptr = OptListBox ;
	for( n=0; n<NbOptPhysBox; n++,ptr++ )
	{
		CopyBlock(	ptr->x0, ptr->y0, ptr->x1, ptr->y1, Screen,
				ptr->x0, ptr->y0, Log ) ;

/*		Box(	ptr->x0, ptr->y0, ptr->x1, ptr->y1, 0 ) ;	*/
	}
}

