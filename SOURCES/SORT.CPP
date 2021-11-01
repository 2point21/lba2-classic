#include	"c_extern.h"

S32	NbTriObj = 0	;

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*

	 Û    ÛÛÜ Û ßßÛßß	   ÛÛßßß ÛßßßÛ ÛßßßÛ ßßÛßß
	 ÛÛ   ÛÛßÛÛ   ÛÛ	   ßßßßÛ ÛÛ  Û ÛÛßÛß   ÛÛ
	 ßß   ßß  ß   ßß	   ßßßßß ßßßßß ßß  ß   ßß

			Tri Int‚rieur

 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#define	BitAfter(n)		(1<<(n&31))
#define	IndexAfter(n)		(n/32)

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#define	MAX_INDEX		((MAX_TRI+31)/32)

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef struct {
	U32	after[MAX_INDEX] ;
	S32	x0  ;
	S32	y0  ;
	S32	x1  ;
	S32	y1  ;
	S32	present ;
} Elem ;

typedef struct {
	Elem		aElem[MAX_TRI] ;
	S32		cElem ;
} tDatabase ;

tDatabase Base ;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
U32	Checked[MAX_INDEX]	;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
static inline S32 BaseDistance(S32 after, S32 before)
{
	T_SORT	*pt1, *pt2;
	S32 dxa, dya, dza ;
	S32 dxb, dyb, dzb ;
	S32 va ;

	pt1 = &ListTri[after]	;
	pt2 = &ListTri[before]	;

	dxb = pt1->XMin-pt2->XMax	;
	dyb = pt1->YMin-pt2->YMax	;
	dzb = pt1->ZMin-pt2->ZMax	;
	dxa = pt2->XMin-pt1->XMax	;
	dya = pt2->YMin-pt1->YMax	;
	dza = pt2->ZMin-pt1->ZMax	;

	va = __max(Max3(dxa, dya, dza), Max3(dxb, dyb, dzb)) ;

	if(va<0)	va = 0	;

	return va ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
static inline S32 BaseFindFirst(S32 after)
{
	S32	i	;
	S32	best	;
	S32	index	;
	S32	bafter	;
	S32	bi	;
	U32	bit	;

restartall:

	memset(Checked, 0, MAX_INDEX*sizeof(U32))	;

	best = -1	;

restart:

	bit   = BitAfter(after) ;
	index = IndexAfter(after) ;

	if(Checked[index]&bit)	// check Loop
	{
		Base.aElem[bi].after[IndexAfter(bafter)] &= ~BitAfter(bafter) ;
		after = bi	;
		goto restartall ;
	}

	Checked[index] |= bit ;

	for(i=0; i<Base.cElem; i++)
	{
		if(Base.aElem[i].after[index]&bit)
		{
			S32 temp ;

			temp = BaseDistance(after, i) ;

			if(temp>best)
			{
				best	= temp	;
				bafter	= after	;
				bi	= i	;
			}

			after = i ;

			goto restart ;
		}
	}

	return after ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
static inline S32 BaseDisplayFirst( void )
{
	S32 i ;

	for(i=0; i<Base.cElem; i++)
	{
		if(Base.aElem[i].present) break ;
	}

	i = BaseFindFirst(i);

	memset(&(Base.aElem[i]), 0, sizeof(Elem)) ;

	return( AffOneObject( &ListTri[i] ) ) ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
S32 BaseSort( void )
{
	S32 i ;

	for(i=0; i<Base.cElem; i++)
	{
		if( BaseDisplayFirst() )	return TRUE ;
	}

	return FALSE ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void BaseReset( void )
{
	Base.cElem = 0 ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#define	BaseAddRelation(a, b)	\
	Base.aElem[b].after[IndexAfter(a)] |= BitAfter(a)

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
S32	TreeInsert( S16 numtype, S32 posx, S32 posy, S32 posz,
		    S32 txmin, S32 tymin, S32 tzmin,
		    S32 txmax, S32 tymax, S32 tzmax )
{
	S32	x0, y0, x1, y1	;
	S32	i		;
	T_SORT	*pt		;
	Elem	*elem		;
	S32	zrot = 0	;

	if(CubeMode==CUBE_INTERIEUR)
	{
		// Serge: a optimiser
		LongProjectPoint(txmin, tymin, tzmax)	;
		x0 = Xp ;
		LongProjectPoint(txmax, tymin, tzmax)	;
		y1 = Yp ;
		LongProjectPoint(txmax, tymin, tzmin)	;
		x1 = Xp ;
		LongProjectPoint(txmin, tymax, tzmin)	;
		y0 = Yp ;
	}
	else
	{
		S32	d ;
		S32	mx, my, mz ;

		mx = (txmin + txmax)>>1 ;
		my = (tymin + tymax)>>1 ;
		mz = (tzmin + tzmax)>>1 ;

		d = Distance3D(mx, my, mz, txmax, tymax, tzmax) ;

		LongWorldRotatePoint(mx, my, mz) ;

		if(!LongProjectPoint(X0+d, Y0, Z0))
		{
			return FALSE ;
		}

		d = Xp ;

		if(!LongProjectPoint(X0, Y0, Z0))
		{
			return FALSE ;
		}

		d -= Xp ;

		x0 = Xp - d ;
		y0 = Yp - d ;
		x1 = Xp + d ;
		y1 = Yp + d ;

		LongWorldRotatePoint( posx, posy, posz ) ;
		zrot = CameraZrClip-Z0 ;// l'ecran
	}

	// PreClipping
	if( (x1<0) OR (x0>=640)
	OR  (y1<0) OR (y0>=480) )
	{
		if( !(numtype&SORT_NO_PRECLIP) )
		{
			return FALSE ;
		}
	}

	pt = &ListTri[NbTriObj]	;//	Create New Entry/List

	pt->Z = zrot		;// l'ecran
	pt->NumObj = numtype&~(SORT_NO_PRECLIP)	;
	pt->Xw = posx		;
	pt->Yw = posy		;
	pt->Zw = posz		;
	pt->XMin = txmin	;
	pt->YMin = tymin	;
	pt->ZMin = tzmin	;
	pt->XMax = txmax	;
	pt->YMax = tymax	;
	pt->ZMax = tzmax	;

	NbTriObj++ 		;

	Base.aElem[Base.cElem].present= TRUE;

	memset(Base.aElem[Base.cElem].after, 0, sizeof(U32)*MAX_INDEX) ;
	Base.aElem[Base.cElem].x0 = x0 ;
	Base.aElem[Base.cElem].y0 = y0 ;
	Base.aElem[Base.cElem].x1 = x1 ;
	Base.aElem[Base.cElem].y1 = y1 ;

	pt   = ListTri		;
	elem = Base.aElem	;

	for (i=0; i<Base.cElem; i++, pt++, elem++ )
	{
// test d'overlap
//		if((x0<=elem->x1)&&(x1>=elem->x0)&&(y0<=elem->y1)&&(y1>=elem->y0))
		{
			S32	dxa, dya, dza	;
			S32	dxb, dyb, dzb	;
			S32	vb		;

			dxa = txmin-pt->XMax	;
			dya = tymin-pt->YMax	;
			dza = tzmin-pt->ZMax	;
			dxb = pt->XMin-txmax	;
			dyb = pt->YMin-tymax	;
			dzb = pt->ZMin-tzmax	;

			if(CubeMode==CUBE_INTERIEUR)
			{
				vb = Max3( dxb, dyb, dzb ) - Max3( dxa, dya, dza ) ;
			}
			else
			{
				dxb -= dxa ;
				dzb -= dza ;
				dyb -= dya ;

				// new method
				dxb = ((-dxb * SinTab[CameraBeta]) >> 14) * CosTab[CameraAlpha] >> 14 ;
				dzb = ((dzb * CosTab[CameraBeta]) >> 14) * CosTab[CameraAlpha] >> 14  ;
				dyb = (dyb * SinTab[CameraAlpha]) >> 14 ;
/*
				// old method
				if(CameraBeta<2048)
				{
					dxb = - dxb ;
				}

				if(CameraAlpha>2048)
				{
					dyb = - dyb ;
				}

				if((CameraBeta>1024)&&(CameraBeta<3072))
				{
					dzb = - dzb ;
				}
*/
				if(abs(dxb)>abs(dyb))
				{
					vb = dxb ;
				}
				else
				{
					vb = dyb ;
				}

				if(abs(dzb)>abs(vb))
				{
					vb = dzb ;
				}

//				vb = Max3( dxb, dyb, dzb ) - Max3( dxa, dya, dza ) ;
			}

			if(vb)
			{
				if (vb>0)		// 	Before
				{
					BaseAddRelation(i, Base.cElem) ;
				}
				else
				{
					BaseAddRelation(Base.cElem, i) ;
				}
			}
		}
	}

	Base.cElem++ ;

	return TRUE ;
}

