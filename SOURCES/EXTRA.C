#include	"c_extern.h"

extern	WORD	YMap ;

extern	LONG	EX0, EY0 ;

WORD	BigStar[] = {	10,			/* nb points */
			0,-20, 4,-6, 19,-6, 7,2, 12,16,
			0,7, -12,16, -7,2, -19,-6, -4,-6	} ;
/*
WORD	Star[] = {	10,			// nb points
			0,-10, 2,-3, 9,-3, 3,1, 6,8,
			0,3, -6,8, -3,1, -9,-3, -2,-3	} ;
*/

WORD	Poff[] = {	18,			/* nb points */
			0,-20, 6,-16, 8,-10, 14,-12, 20,-4,
			18,4, 12,4, 16,8, 8,16, 2,12,
			-4,18, -10,16, -12,8, -16,10, -20,4,
			-12,-8, -6,-6, -10,-12 } ;

/*══════════════════════════════════════════════════════════════════════════*
			 █▀▀▀▀ ▀▄ ▄▀ ▀▀█▀▀ █▀▀▀█ █▀▀▀█
			 ██▀▀   ▄▀▄    ██  ██▀█▀ ██▀▀█
			 ▀▀▀▀▀ ▀   ▀   ▀▀  ▀▀  ▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/

void	InitFly( T_EXTRA *ptrextra, WORD alpha, WORD beta, WORD vitesse, WORD poids )
{
	ptrextra->Flags |= EXTRA_FLY ;

	ptrextra->OrgX = ptrextra->PosX ;
	ptrextra->OrgY = ptrextra->PosY ;
	ptrextra->OrgZ = ptrextra->PosZ ;

	Rotate( vitesse,0, alpha ) ;
	ptrextra->Vy = -Y0 ;

	Rotate( 0, X0, beta ) ;
	ptrextra->Vx = X0 ;
	ptrextra->Vz = Y0 ;

	ptrextra->Poids = poids ;

	ptrextra->Timer = TimerRef ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	BounceExtra( T_EXTRA *ptrextra, WORD oldx, WORD oldy, WORD oldz )
{
	WORD	vx, vy, vz ;

	vx = ptrextra->Vx ;
	vy = ptrextra->Vy ;
	vz = ptrextra->Vz ;

	// test under last pos
	if( WorldColBrick( oldx, ptrextra->PosY, oldz ) )
	{
		ptrextra->Vy = -ptrextra->Vy ;
	}

	// test left & right
	if( WorldColBrick( ptrextra->PosX, oldy, oldz ) )
	{
		ptrextra->Vx = -ptrextra->Vx ;
	}

	// test up & down
	if( WorldColBrick( oldx, oldy, ptrextra->PosZ ) )
	{
		ptrextra->Vz = -ptrextra->Vz ;
	}

/*	// recherche last pos ok
	do
	{
		ptrextra->PosX -= vx ;
		ptrextra->PosY -= vy ;
		ptrextra->PosZ -= vz ;

	}
	while( WorldColBrick( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ) ;
	ptrextra->PosY -= *(&PtrZvExtra[ ptrextra->Sprite*8 + 2 ]+3) ;
*/

	ptrextra->OrgX = ptrextra->PosX = oldx ;
	ptrextra->OrgY = ptrextra->PosY = oldy ;
	ptrextra->OrgZ = ptrextra->PosZ = oldz ;

	ptrextra->Timer = TimerRef ;
}

/*══════════════════════════════════════════════════════════════════════════*/

WORD	ExtraBonus(	WORD x, WORD y, WORD z,
			WORD alpha, WORD beta,
			WORD num, WORD nb )
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = num ;

		ptrextra->Flags = EXTRA_STOP_COL
//				+ EXTRA_WAIT_NO_COL
				+ EXTRA_WAIT_SOME_TIME
				+ EXTRA_TAKABLE	;

		/* Special for Key ( Beurk cause detect with sprite number ! ) */

		if ( num != 6 )	ptrextra->Flags += EXTRA_TIME_OUT
						 + EXTRA_FLASH 	;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

// ESSAI bonus qui coince moins ???
		InitFly( ptrextra, alpha, beta, 40, 15 ) ;
//		InitFly( ptrextra, alpha, beta, 40, 16 ) ;

		ptrextra->HitForce = 0 ;

		ptrextra->Timer = TimerRef ;
		ptrextra->TimeOut = 50 * 20 ;	/* 20 secondes */

		ptrextra->Divers = nb ;
		return n ;
	}
	return -1 ;
}
/*══════════════════════════════════════════════════════════════════════════*/

WORD	ExtraExplo(	WORD x, WORD y, WORD z )
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = 97 ;	// explo

		ptrextra->Flags = EXTRA_TIME_OUT + EXTRA_EXPLO ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;
		ptrextra->HitForce = 0 ;

		ptrextra->Timer = TimerRef ;
		ptrextra->TimeOut = 40 ;

		return n ;
	}
	return -1 ;
}
/*══════════════════════════════════════════════════════════════════════════*/

WORD	SearchBonusKey()
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;
	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite == 6 )	// Key !!!!!
		{
			return n ;
		}
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/

WORD	ExtraSearch(	WORD owner, WORD x, WORD y, WORD z, WORD num,
			WORD numobj, WORD speed, WORD hitforce )
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		/* TimeOut: num obj throw
		/* WORD Timer: num obj search */
		/* &OrgX = RealValue
			(OrgY, OrgZ, Vx, Vy */
		/* Vz = speed */
/*
{
UBYTE	string[256] ;

strcpy( string, "ThrowSearch from " ) ;
strcat( string, itoa( owner, "         ",10 ) ) ;
Message( string, FALSE ) ;
}
*/
		ptrextra->Sprite = num ;

		ptrextra->Flags = EXTRA_SEARCH_OBJ ;
		ptrextra->Divers = 0 ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		ptrextra->TimeOut = owner ;
		ptrextra->Timer = (LONG)numobj ;

		ptrextra->Vz = speed ;
		ptrextra->HitForce = hitforce ;

		InitRealValue( 0, speed, 50, (T_REAL_VALUE*)&ptrextra->OrgX ) ;

		// memo last angle
		ptrextra->Poids = GetAngle(	x,z,
						ListObjet[numobj].PosObjX,
						ListObjet[numobj].PosObjZ ) ;
		return n ;
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/

WORD	ExtraSearchKey(	WORD owner, WORD x, WORD y, WORD z,
			WORD num, WORD numobj )
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		/* TimeOut: num obj throw
		/* WORD Timer: num obj search */
		/* &OrgX = RealValue
			(OrgY, OrgZ, Vx, Vy */
		/* Vz = speed */

		ptrextra->Sprite = num ;

		ptrextra->Flags = EXTRA_SEARCH_KEY ;
		ptrextra->Divers = 0 ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		ptrextra->TimeOut = owner ;
		ptrextra->Timer = (LONG)numobj ;

		ptrextra->Vz = 4000 ;
		ptrextra->HitForce = 0 ;

		InitRealValue( 0, 4000, 50, (T_REAL_VALUE*)&ptrextra->OrgX ) ;

		// memo last angle
		ptrextra->Poids = GetAngle(	x,z,
						ListExtra[numobj].PosX,
						ListExtra[numobj].PosZ ) ;

		return n ;
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

/*══════════════════════════════════════════════════════════════════════════*/

WORD	ExtraCheckObjCol( T_EXTRA *ptrextra, WORD owner )
{
	WORD	n ;
	T_OBJET	*ptrobjt ;
	WORD	xw, yw, zw ;
	WORD	x0,y0,z0, x1,y1,z1 ;
	WORD	xt0,yt0,zt0, xt1,yt1,zt1 ;
	WORD	*ptr ;

	xw = ptrextra->PosX ;
	yw = ptrextra->PosY ;
	zw = ptrextra->PosZ ;

	ptr = &PtrZvExtra[ ptrextra->Sprite*8 + 2 ] ;

	x0 = *ptr++ + xw ;
	x1 = *ptr++ + xw ;
	y0 = *ptr++ + yw ;
	y1 = *ptr++ + yw ;
	z0 = *ptr++ + zw ;
	z1 = *ptr++ + zw ;

	ptrobjt = ListObjet ;

	for( n=0; n<NbObjets; n++ )
	{
		if( (ptrobjt->Body != -1)
//		AND (!(ptrobjt->Flags&INVISIBLE))
		AND (n != owner) )
		{

		xt0 = ptrobjt->PosObjX + ptrobjt->Xmin ;
		xt1 = ptrobjt->PosObjX + ptrobjt->Xmax ;
		yt0 = ptrobjt->PosObjY + ptrobjt->Ymin ;
		yt1 = ptrobjt->PosObjY + ptrobjt->Ymax ;
		zt0 = ptrobjt->PosObjZ + ptrobjt->Zmin ;
		zt1 = ptrobjt->PosObjZ + ptrobjt->Zmax ;

		if( x0 < xt1
		AND x1 > xt0
		AND y0 < yt1
		AND y1 > yt0
		AND z0 < zt1
		AND z1 > zt0 )
		{
			if( ptrextra->HitForce != 0 )
			{
				HitObj( owner, n, ptrextra->HitForce, -1 ) ;
			}
			return n ;
		}
		}
		ptrobjt++ ;
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*
WORD	ExtraFullCheckObjCol( T_EXTRA *ptrextra, WORD owner )
{
	WORD	n ;
	T_OBJET	*ptrobjt ;
	WORD	xw, yw, zw ;
	WORD	x0,y0,z0, x1,y1,z1 ;
	WORD	xt0,yt0,zt0, xt1,yt1,zt1 ;
	WORD	*ptr ;

	xw = ptrextra->PosX ;
	yw = ptrextra->PosY ;
	zw = ptrextra->PosZ ;

	ptr = &PtrZvExtra[ ptrextra->Sprite*8 + 2 ] ;

	x0 = *ptr++ + xw ;
	x1 = *ptr++ + xw ;
	y0 = *ptr++ + yw ;
	y1 = *ptr++ + yw ;
	z0 = *ptr++ + zw ;
	z1 = *ptr++ + zw ;

	ptrobjt = ListObjet ;

	for( n=0; n<NbObjets; n++ )
	{
		if( (ptrobjt->Body != -1)
//		AND (!(ptrobjt->Flags&INVISIBLE))
		AND (n != owner) )
		{

		xt0 = ptrobjt->PosObjX + ptrobjt->Xmin ;
		xt1 = ptrobjt->PosObjX + ptrobjt->Xmax ;
		yt0 = ptrobjt->PosObjY + ptrobjt->Ymin ;
		yt1 = ptrobjt->PosObjY + ptrobjt->Ymax ;
		zt0 = ptrobjt->PosObjZ + ptrobjt->Zmin ;
		zt1 = ptrobjt->PosObjZ + ptrobjt->Zmax ;

		if( x0 < xt1
		AND x1 > xt0
		AND y0 < yt1
		AND y1 > yt0
		AND z0 < zt1
		AND z1 > zt0 )
		{
			if( ptrextra->HitForce != 0 )
			{
				HitObj( owner, n, ptrextra->HitForce, -1 ) ;
			}
			return n ;
		}
		}
		ptrobjt++ ;
	}
	return -1 ;
}
*/
/*──────────────────────────────────────────────────────────────────────────*/

WORD	ExtraCheckExtraCol( T_EXTRA *ptrextra, WORD owner )
{
	WORD	n ;
	T_EXTRA	*ptrobjt ;
	WORD	xw, yw, zw ;
	WORD	xwt, ywt, zwt ;
	WORD	x0,y0,z0, x1,y1,z1 ;
	WORD	xt0,yt0,zt0, xt1,yt1,zt1 ;
	WORD	*ptr ;

	xw = ptrextra->PosX ;
	yw = ptrextra->PosY ;
	zw = ptrextra->PosZ ;

	ptr = &PtrZvExtra[ ptrextra->Sprite*8 + 2 ] ;

	x0 = *ptr++ + xw ;
	x1 = *ptr++ + xw ;
	y0 = *ptr++ + yw ;
	y1 = *ptr++ + yw ;
	z0 = *ptr++ + zw ;
	z1 = *ptr++ + zw ;

	ptrobjt = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++ )
	{
		if( (ptrobjt->Sprite != -1)
		AND (n != owner) )
		{
			xwt = ptrobjt->PosX ;
			ywt = ptrobjt->PosY ;
			zwt = ptrobjt->PosZ ;

			ptr = &PtrZvExtra[ ptrobjt->Sprite*8 + 2 ] ;

			xt0 = *ptr++ + xwt ;
			xt1 = *ptr++ + xwt ;
			yt0 = *ptr++ + ywt ;
			yt1 = *ptr++ + ywt ;
			zt0 = *ptr++ + zwt ;
			zt1 = *ptr++ + zwt ;

			if( x0 < xt1
			AND x1 > xt0
			AND y0 < yt1
			AND y1 > yt0
			AND z0 < zt1
			AND z1 > zt0 )
			{
				return n ;
			}
		}
		ptrobjt++ ;
	}
	return -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/

UBYTE	FullWorldColBrick(	WORD oldx, WORD oldy, WORD oldz,
				WORD newx, WORD newy, WORD newz )
{
	WORD	x0, y0, z0 ;
	WORD	x1, y1, z1 ;
	WORD	x2, y2, z2 ;

	if( WorldColBrick( newx,newy,newz ) )	return 1 ;

	x0 = (newx+oldx)/2 ;
	y0 = (newy+oldy)/2 ;
	z0 = (newz+oldz)/2 ;

	if( WorldColBrick( x0,y0,z0 ) )		return 1 ;

	x1 = (newx+x0)/2 ;
	y1 = (newy+y0)/2 ;
	z1 = (newz+z0)/2 ;

	if( WorldColBrick( x1,y1,z1 ) )		return 1 ;

	x2 = (x0+oldx)/2 ;
	y2 = (y0+oldy)/2 ;
	z2 = (z0+oldz)/2 ;

	if( WorldColBrick( x2,y2,z2 ) )		return 1 ;

	return 0 ;
}

/*══════════════════════════════════════════════════════════════════════════*
		   ██▀▀▀ █▀▀▀█ █▀▀▀▀ █▀▀▀▀  █    █▀▀▀█ █
		   ▀▀▀▀█ ██▀▀▀ ██▀▀  ██     ██   ██▀▀█ ██
		   ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀▀▀▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/

/*──────────────────────────────────────────────────────────────────────────*/

void	ClearScreenMinMax()
{
	ScreenXmin = 32000 ;
	ScreenXmax = -32000 ;
	ScreenYmin = 32000 ;
	ScreenYmax = -32000 ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	AdjustScreenMax()
{
	if( Xp < ScreenXmin )	ScreenXmin = Xp ;
	if( Xp > ScreenXmax )	ScreenXmax = Xp ;
	if( Yp < ScreenYmin )	ScreenYmin = Yp ;
	if( Yp > ScreenYmax )	ScreenYmax = Yp ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

void	Aff2DShape(	WORD *ptr,
			WORD xs, WORD ys,
			WORD coul, WORD angle, WORD zoom )
{
	WORD	x0, y0, x1, y1, x2, y2 ;
	WORD	x, y, n, nb ;

	ClearScreenMinMax() ;

	nb = *ptr++ ;

	x = (*ptr++) * zoom / 16 ;
	y = (*ptr++) * zoom / 16 ;
	Rotate( x, y, angle ) ;
	Xp = x0 = X0 + xs ;
	Yp = y0 = Y0 + ys ;
	AdjustScreenMax() ;

	for( n=1; n<nb; n++ )
	{
		x1 = Xp ;
		y1 = Yp ;

		x = (*ptr++) * zoom / 16 ;
		y = (*ptr++) * zoom / 16 ;
		Rotate( x, y, angle ) ;
		Xp = X0 + xs ;
		Yp = Y0 + ys ;
		AdjustScreenMax() ;

		Line( x1,y1, Xp, Yp, coul ) ;
	}
	Line( Xp,Yp, x0, y0, coul ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	AffSpecial( WORD numextra, WORD xs, WORD ys )
{
	T_EXTRA	*ptrextra ;
	WORD	zoom ;

	ptrextra = &ListExtra[numextra] ;

	switch( ptrextra->Sprite & 32767 )
	{
		case 0:	// hit star
			Aff2DShape( BigStar, xs, ys,
				    15, (WORD)((TimerRef<<5) & 1023), 4 ) ;
			break ;

		case 1:	// impact
			zoom = 1+(WORD)(TimerRef - ptrextra->Timer) ;
			if( zoom > 32 )	zoom = 32 ; // peut etre moins
			Aff2DShape( Poff, xs, ys,
				    15, 0, zoom ) ;
			break ;

		case 2:	// fontaine
			break ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/

void	InitSpecial(	WORD x, WORD y, WORD z, WORD num )
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = num + 32768 ;

		ptrextra->Divers = 0 ;

		/* star */
		switch( num )
		{
			case 0:	/* small star */

				ptrextra->Flags = EXTRA_TIME_OUT
						+ EXTRA_END_COL ;

				ptrextra->PosX = x ;
				ptrextra->PosY = y ;
				ptrextra->PosZ = z ;

				InitFly( ptrextra, Rnd(256)+128, Rnd(1024), 50, 20 ) ;

				ptrextra->HitForce = 0 ;

				ptrextra->Timer = TimerRef ;
				ptrextra->TimeOut = 50 * 2 ;	/* 2 secondes */

				return ;

			case 1:	/* impact */

				ptrextra->Flags = EXTRA_TIME_OUT ;

				ptrextra->PosX = x ;
				ptrextra->PosY = y ;
				ptrextra->PosZ = z ;

				ptrextra->HitForce = 0 ;

				ptrextra->Timer = TimerRef ;
				ptrextra->TimeOut = 5 ;

				return ;

		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/

LONG	CoulRetourBalle( T_EXTRA *ptrextra )
{
	LONG	sprite = 44 ;				// jaune ou feu

	if( ptrextra->Sprite == 42 )	sprite = 109 ;	// vert
	if( ptrextra->Sprite == 43 )	sprite = 110 ;	// rouge

	return	sprite ;
}

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀       █▀▀▀▀ █▀▀▀█ █     █     ██▀▀▀
	  ██ ▀█ ██▀▀█ ██▀ █ ██▀▀        ██    ██▀▀█ ██    ██    ▀▀▀▀█
	  ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	ClearExtra()
{
	WORD	n ;

	for( n=0; n<MAX_EXTRAS; n++ )
	{
		ListExtra[n].Sprite = -1 ;
		ListExtra[n].Divers = 1 ;
	}

}

/*──────────────────────────────────────────────────────────────────────────*/
WORD	ThrowExtra(	WORD owner,
			WORD x, WORD y, WORD z,
			WORD num,
			WORD alpha, WORD beta, WORD vitesse, WORD poids,
			WORD hitforce )
{
	T_EXTRA	*ptrextra ;
	WORD	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 ) 	continue ;	/* slot libre */

		ptrextra->Sprite = num ;

		ptrextra->Flags = EXTRA_END_OBJ
				+ EXTRA_END_COL
				+ EXTRA_WAIT_NO_COL
				+ EXTRA_IMPACT ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		InitFly( ptrextra, alpha, beta, vitesse, poids ) ;

		ptrextra->HitForce = hitforce ;

		ptrextra->TimeOut = owner ;

		ptrextra->Timer = TimerRef ;

		ptrextra->Divers = 0 ;

		return n ;
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	GiveExtraBonus( T_OBJET *ptrobj )
{
	WORD	n, m ;
	UBYTE	tabbonus[5] ;

	m = 0 ;
	for( n=0; n<5; n++ )
	{
		if( ptrobj->OptionFlags & (1<<(n+4)) )
		{
			tabbonus[m] = n ;
			m++ ;
		}
	}

	if( !m )	return ;

	n = tabbonus[ Rnd( m ) ] ;

	if( (MagicLevel == 0) AND (n==2) )	n = 1 ; // si pas magie -> coeur

	if( ptrobj->WorkFlags & OBJ_DEAD )	// obj mort
	{
		ExtraBonus(	ptrobj->PosObjX,
				ptrobj->PosObjY,
				ptrobj->PosObjZ,
				256, 0,
				n + 3,	/* 3 offset spr bonus */
				ptrobj->NbBonus ) ;

		HQ_3D_MixSample( 11, 0x1000, 1,
			ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;

	}
	else					// obj vivant
	{
		ExtraBonus(	ptrobj->PosObjX,
				ptrobj->PosObjY+ptrobj->Ymax,
				ptrobj->PosObjZ,
				200,
				GetAngle(       ptrobj->PosObjX,
						ptrobj->PosObjZ,
						ListObjet[NUM_PERSO].PosObjX,
						ListObjet[NUM_PERSO].PosObjZ ) ,
				n + 3,	/* 3 offset spr bonus */
				ptrobj->NbBonus ) ;

		HQ_3D_MixSample( 11, 0x1000, 1,
			ptrobj->PosObjX, ptrobj->PosObjY+ptrobj->Ymax, ptrobj->PosObjZ ) ;

	}
}

/*══════════════════════════════════════════════════════════════════════════*/

void	ZoneGiveExtraBonus( T_ZONE *ptrz )
{
	WORD	n, m, x,y,z, p  ;
	UBYTE	tabbonus[5] ;

	if( ptrz->Info2 )	return ;	/* déjà pris */

	m = 0 ;
	for( n=0; n<5; n++ )
	{
		if( ptrz->Info0 & (1<<(n+4)) )
		{
			tabbonus[m] = n ;
			m++ ;
		}
	}

	if( !m )	return ;

	n = tabbonus[ Rnd( m ) ] ;

	if( (MagicLevel == 0) AND (n==2) )	n = 1 ; // si pas magie -> coeur

	x = (ptrz->X0+ptrz->X1)/2 ;
	z = (ptrz->Z0+ptrz->Z1)/2 ;

	p = ExtraBonus(	x, ptrz->Y1, z,
			180,
			GetAngle(       x, z,
					ListObjet[NUM_PERSO].PosObjX,
					ListObjet[NUM_PERSO].PosObjZ ) ,
			n + 3,		/* 3 offset spr bonus */
			ptrz->Info1 ) ; /* nb */

	if( p != -1 )
	{
		ListExtra[p].Flags |= EXTRA_TIME_IN ;
		ptrz->Info2 = 1 ;		/* marque prise */
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	GereExtras()
{
	T_EXTRA	*ptrextra ;
	WORD	n ;
	WORD	oldx, oldy, oldz ;
	LONG	time ;
	WORD	*ptr ;
	WORD	alpha, beta, angle, s, owner, search ;
	LONG	flagcol ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite == -1 )	continue ;

		if( ptrextra->Flags & EXTRA_TIME_OUT )
		{
			if( TimerRef >= ptrextra->Timer + ptrextra->TimeOut )
			{
				/* bye bye */
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_ONE_FRAME )
		{
			ptrextra->Sprite = -1 ;
			continue ;
		}

		if( ptrextra->Flags & EXTRA_EXPLO )
		{
			ptrextra->Sprite = BoundRegleTrois( 97,100,30,TimerRef - ptrextra->Timer ) ;
			continue ;
		}

		if( ptrextra->Flags & EXTRA_FLY )
		{
			time = TimerRef - ptrextra->Timer ;

			oldx = ptrextra->PosX ;
			oldy = ptrextra->PosY ;
			oldz = ptrextra->PosZ ;

			ptrextra->PosX = ptrextra->Vx*time + ptrextra->OrgX ;
			ptrextra->PosY =  ptrextra->Vy*time
					+ ptrextra->OrgY
					- (ptrextra->Poids*time*time)/16 ;
			ptrextra->PosZ = ptrextra->Vz*time + ptrextra->OrgZ ;

			/* securite */
			if( (ptrextra->PosY < 0)
			OR  (ptrextra->PosX < 0)
			OR  (ptrextra->PosX > SIZE_BRICK_XZ * (SIZE_CUBE_X-1))
			OR  (ptrextra->PosZ < 0)
			OR  (ptrextra->PosZ > SIZE_BRICK_XZ * (SIZE_CUBE_Z-1)) )
			{
				if( n == MagicBall )
				{
				/* init retour balle */
				MagicBall = ExtraSearch(	-1,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
								CoulRetourBalle( ptrextra ),
								NUM_PERSO, 10000, 0 ) ;
				}
				if( ptrextra->Flags & EXTRA_TAKABLE )
				{
					ptrextra->Flags &= ~(EXTRA_FLY+EXTRA_STOP_COL) ;
				}
				else
				{
					ptrextra->Sprite = -1 ;
				}
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_WAIT_SOME_TIME )
		{
			if( (TimerRef - ptrextra->Timer) > 40 )
			{
				ptrextra->Flags &= ~EXTRA_WAIT_SOME_TIME ;
			}
			continue ;
		}

		if( ptrextra->Flags & EXTRA_SEARCH_OBJ )
		{
			/* TimeOut: owner */
			/* (WORD)Timer: numobj search */
			/* &OrgX = RealValue
				(OrgY, OrgZ, Vx, Vy */
			/* Vz = speed */
			/* Poids = last angle beta */

			search = (WORD)ptrextra->Timer ;
			owner = ptrextra->TimeOut ;

			oldx = ListObjet[search].PosObjX ;
			oldy = ListObjet[search].PosObjY+1000 ;
			oldz = ListObjet[search].PosObjZ ;

			beta =	GetAngle(	ptrextra->PosX,
						ptrextra->PosZ,
						oldx, oldz ) ;

			angle = (beta - ptrextra->Poids)&1023 ;
			if( (angle < 600) AND (angle > 400) )
			{
				// si angle retourné obj considéré comme touché
				if( ptrextra->HitForce != 0 )
				{
					HitObj( owner, search, ptrextra->HitForce, -1 ) ;
				}
				if( n == MagicBall )
				{
					MagicBall = -1 ;
				}
				/* bye bye */
				ptrextra->Sprite = -1 ;
				continue ;
			}

			alpha =	GetAngle(	ptrextra->PosY,0,
						oldy,Distance  ) ;

			s = GetRealValue( (T_REAL_VALUE*)&ptrextra->OrgX ) ;
			if( !s )	s = 1 ;
			/* s = vitesse (distance/temps) */

			Rotate( s,0, alpha ) ;
			ptrextra->PosY -= Y0 ;
			Rotate( 0, X0, beta ) ;
			ptrextra->PosX += X0 ;
			ptrextra->PosZ += Y0 ;

			InitRealValue( 0, ptrextra->Vz,
					50, (T_REAL_VALUE*)&ptrextra->OrgX ) ;

//			if( ExtraFullCheckObjCol( ptrextra, owner ) == search )
			if( ExtraCheckObjCol( ptrextra, owner ) == search )
			{
				if( n == MagicBall )
				{
					MagicBall = -1 ;
				}
				/* bye bye */
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_SEARCH_KEY )// QUE magic ball ONLY
		{
			/* TimeOut: owner */
			/* (WORD)Timer: numobj search */
			/* &OrgX = RealValue
				(OrgY, OrgZ, Vx, Vy */
			/* Vz = speed */

			search = (WORD)ptrextra->Timer ;
			owner = ptrextra->TimeOut ;

			oldx = ListExtra[search].PosX ;
			oldy = ListExtra[search].PosY ;
			oldz = ListExtra[search].PosZ ;

			beta =	GetAngle(	ptrextra->PosX,
						ptrextra->PosZ,
						oldx, oldz ) ;

			angle = (beta - ptrextra->Poids)&1023 ;
			if( (angle < 600) AND (angle > 400) )
			{
				goto key_found ;
			}

			alpha =	GetAngle(	ptrextra->PosY,0,
						oldy,Distance  ) ;

			s = GetRealValue( (T_REAL_VALUE*)&ptrextra->OrgX ) ;
			if( !s )	s = 1 ;
			/* s = vitesse (distance/temps) */

			Rotate( s,0, alpha ) ;
			ptrextra->PosY -= Y0 ;
			Rotate( 0, X0, beta ) ;
			ptrextra->PosX += X0 ;
			ptrextra->PosZ += Y0 ;

			InitRealValue( 0, ptrextra->Vz,
					50, (T_REAL_VALUE*)&ptrextra->OrgX ) ;

			if( ExtraCheckExtraCol( ptrextra, MagicBall ) == search )
			{
key_found:
				// trouve clef
				HQ_3D_MixSample( 97, 0x1000, 1,
					ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;

				// incruste pendant 2 sec le bonus trouve
				if( ListExtra[search].Divers > 1 )
				{
					ProjettePoint( ptrextra->PosX-WorldXCube, ptrextra->PosY-WorldYCube, ptrextra->PosZ-WorldZCube ) ;
					InitIncrustDisp( INCRUST_NUM,
							 ListExtra[search].Divers,
							 Xp, Yp,
							 158, 0, 2 ) ;
				}
				InitIncrustDisp( INCRUST_SPRITE,
						 6, // key
						 10, 30,
						 0, 0, 2 ) ;

				NbLittleKeys += ListExtra[search].Divers  ;
				ListExtra[search].Sprite = -1 ;

				// init retour balle
				MagicBall = ExtraSearch(-1,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
						6, // key
						NUM_PERSO, 8000, 0 ) ;
				ptrextra->Sprite = -1 ;
				continue ;
			}

			if( ListExtra[search].Sprite == -1 ) // clef prise entre temps
			{
				// init retour balle
				MagicBall = ExtraSearch(-1,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
						CoulRetourBalle( ptrextra ),
						NUM_PERSO, 8000, 0 ) ;
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_END_OBJ )	/* hit force */
		{
			if( ExtraCheckObjCol( ptrextra, ptrextra->TimeOut/*owner*/ ) != -1 )
			{
				if( n == MagicBall )
				{
					// sample choc balle sur obj ?

					/* init retour balle */
					MagicBall = ExtraSearch(	-1,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
									CoulRetourBalle( ptrextra ),
									NUM_PERSO, 10000, 0 ) ;
				}
				/* bye bye */
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_END_COL )
		{

		flagcol = FALSE ;
		if( FullWorldColBrick(	oldx,
					oldy,
					oldz,
					ptrextra->PosX,
					ptrextra->PosY,
					ptrextra->PosZ ) )
		{
			if( !(ptrextra->Flags & EXTRA_WAIT_NO_COL) )
			{
				flagcol = TRUE ;
			}
		}
		else
		{
			if( ptrextra->Flags & EXTRA_WAIT_NO_COL )
			{
				ptrextra->Flags &= ~EXTRA_WAIT_NO_COL ;
			}
		}

		if( flagcol )
//		if( WorldColBrick( ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ ) )
		{
			if( ptrextra->Flags & EXTRA_IMPACT )
			{
				// impact
				InitSpecial( oldx,oldy,oldz, 1 ) ;
			}
			if( n == MagicBall )
			{
				// sample rebond retour balle
				HQ_3D_MixSample( 86, 0x1000+Rnd(300)-150, 1,
					ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;

				switch( MagicBallType )
				{
				case 0:
					// init retour balle
					MagicBall = ExtraSearch(-1,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
								CoulRetourBalle( ptrextra ),
								NUM_PERSO, 10000, 0 ) ;
					ptrextra->Sprite = -1 ;
					continue ;
					break ;

				case 1:
					if( !MagicBallCount-- )
					{
						// init retour balle
						MagicBall = ExtraSearch(-1,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
								CoulRetourBalle( ptrextra ),
								NUM_PERSO, 10000, 0 ) ;
						ptrextra->Sprite = -1 ;
						continue ;
					}
					else
					{
						// forcement passé par EXTRA_FLY
						BounceExtra( ptrextra, oldx, oldy, oldz ) ;
					}
					break ;
				}
			}
			else	// pas magic ball
			{
				// bye bye
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}
		}

		if( ptrextra->Flags & EXTRA_STOP_COL )
		{
			flagcol = FALSE ;
			if( FullWorldColBrick(	oldx,
						oldy,
						oldz,
						ptrextra->PosX,
						ptrextra->PosY,
						ptrextra->PosZ ) )
			{
				if( !(ptrextra->Flags & EXTRA_WAIT_NO_COL) )
				{
//Message( "bloque", FALSE ) ;
					flagcol = TRUE ;
				}
			}
			else
			{
				if( ptrextra->Flags & EXTRA_WAIT_NO_COL )
				{
/*
Message( "supprime flag", FALSE ) ;
Text( 0, 0, "%Foldx:%d oldy:%d oldz:%d ", oldx, oldy, oldz ) ;
Text( 0, 9, "%Fnewx:%d newy:%d newz:%d ", ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
Text( 0,18, "%Forgx:%d orgy:%d orgz:%d ", ptrextra->OrgX, ptrextra->OrgY, ptrextra->OrgZ ) ;
*/
					ptrextra->Flags &= ~EXTRA_WAIT_NO_COL ;
				}
			}

			if( flagcol )
			{
				ptr = &PtrZvExtra[ ptrextra->Sprite*8 + 2 ] ;
				ptr+=2 ;
				ptrextra->PosY = YMap * SIZE_BRICK_Y + SIZE_BRICK_Y - *ptr ;
				ptrextra->Flags &= ~(EXTRA_FLY+EXTRA_STOP_COL) ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_TAKABLE )
		{
			if( !(ptrextra->Flags & EXTRA_FLY) )	/* no take while flying */
			{
				if( ExtraCheckObjCol( ptrextra, -1 ) == NUM_PERSO )
				{
					HQ_3D_MixSample( 97, 0x1000, 1,
						ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;

					// incruste pendant 2 sec le bonus trouve
					if( ptrextra->Divers > 1 )
					{
						ProjettePoint( ptrextra->PosX-WorldXCube, ptrextra->PosY-WorldYCube, ptrextra->PosZ-WorldZCube ) ;
						InitIncrustDisp( INCRUST_NUM,
								 ptrextra->Divers,
								 Xp, Yp,
								 158, 0, 2 ) ;
					}
					InitIncrustDisp( INCRUST_SPRITE,
							 ptrextra->Sprite,
							 10, 30,
							 0, 0, 2 ) ;

					/* give money/life/magic/key/clover to perso */

					if( ptrextra->Sprite == 3 ) /* Money */
					{
						NbGoldPieces += ptrextra->Divers ;
						if( NbGoldPieces > 999 )
							NbGoldPieces = 999 ;
					}


					if( ptrextra->Sprite == 4 ) /* Life points */
					{
						ListObjet[NUM_PERSO].LifePoint += ptrextra->Divers  ;
						if( ListObjet[NUM_PERSO].LifePoint > 50 )
						{
							ListObjet[NUM_PERSO].LifePoint = 50 ;
						}
					}

					if( ptrextra->Sprite == 5 ) /* Magic points */
					{
						if( MagicLevel ) // à voir ?
						{
							MagicPoint += ptrextra->Divers*2  ;
							if( MagicPoint > (MagicLevel*20) )
								MagicPoint = (MagicLevel*20) ;
						}
					}

					if( ptrextra->Sprite == 6 ) /* Little Key */
						NbLittleKeys += ptrextra->Divers  ;

					if( ptrextra->Sprite == 7 ) /* four leaf clover */
					{
						NbFourLeafClover += ptrextra->Divers  ;

						if( NbFourLeafClover > NbCloverBox )
						{
							NbFourLeafClover = NbCloverBox ;
						}
					}

					/* bye bye */
					ptrextra->Sprite = -1 ;
					continue ;
				}
			}
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
