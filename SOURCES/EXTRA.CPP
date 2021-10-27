#include	"c_extern.h"

#define	CONE_GANT	2048
#define	RND_BETA_BONUS	300	// Angle alatoire rajouter

extern	S32	MagicBallFlags ;
extern	S32	InitMagicBall = FALSE ;	// Pour ne pas traiter la balle
					// magique (ou plutot son ombre de
					// retour) dans la boucle ou elle a
					// t initialise

// Tableaux de Kashes Maximum necessaire par chapitre scnarique

S32	NecessaryChapterMoney[MAX_CHAPTER] = {
	50,	// 0
	50,	// 1
	50,	// 2
	50,	// 3
	150,	// 4
	50,	// 5
	50,	// 6
	50,	// 7
	50,	// 8
	50,	// 9
	50,	// 10
	50,	// 11
	50,	// 12
	50,	// 13
	50,	// 14
	50,	// 15
	50,	// 16
	50,	// 17
	50,	// 18
	50	// 19
	} ;

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			 Û฿฿฿฿ ฿Ü Ü฿ ฿฿Û฿฿ Û฿฿฿Û Û฿฿฿Û
			 ÛÛ฿฿   Ü฿Ü    ÛÛ  ÛÛ฿Û฿ ÛÛ฿฿Û
			 ฿฿฿฿฿ ฿   ฿   ฿฿  ฿฿  ฿ ฿฿  ฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	InitFly( T_EXTRA *ptrextra, S32 alpha, S32 beta, S32 vitesse, U8 poids )
{
	ptrextra->Flags |= EXTRA_FLY ;

	ptrextra->U.Org.X = ptrextra->PosX ;
	ptrextra->U.Org.Y = ptrextra->PosY ;
	ptrextra->U.Org.Z = ptrextra->PosZ ;

	Rotate( vitesse,0, alpha ) ;
	ptrextra->Vy = (S16)(-Z0) ;

	Rotate( 0, X0, beta ) ;
	ptrextra->Vx = (S16)X0 ;
	ptrextra->Vz = (S16)Z0 ;

	ptrextra->Poids = poids ;

	ptrextra->Timer = TimerRefHR ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	BounceExtra( T_EXTRA *ptrextra, S32 oldx, S32 oldy, S32 oldz )
{
	S32	vx, vy, vz ;

	vx = ptrextra->Vx ;
	vy = ptrextra->Vy ;
	vz = ptrextra->Vz ;

	// test under last pos
	if( PtrWorldColBrick( oldx, ptrextra->PosY, oldz ) )
	{
		ptrextra->Vy = (S16)(-ptrextra->Vy) ;
	}

	// test left & right
	if( PtrWorldColBrick( ptrextra->PosX, oldy, oldz ) )
	{
		ptrextra->Vx = (S16)(-ptrextra->Vx) ;
	}

	// test up & down
	if( PtrWorldColBrick( oldx, oldy, ptrextra->PosZ ) )
	{
		ptrextra->Vz = (S16)(-ptrextra->Vz) ;
	}

	ptrextra->U.Org.X = ptrextra->PosX = oldx ;
	ptrextra->U.Org.Y = ptrextra->PosY = oldy ;
	ptrextra->U.Org.Z = ptrextra->PosZ = oldz ;

	ptrextra->Timer = TimerRefHR ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

S32	ExtraBonus(	S32 x, S32 y, S32 z,
			S32 alpha, S32 beta,
			S16 num, S16 nb )
{
	T_EXTRA	*ptrextra ;
	S32	n ;
	S32	flagkill = FALSE ;

searchextra:

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( flagkill )
		{
			if( (ptrextra->Sprite==SPRITE_CLE)
			OR  (ptrextra->Sprite&32768) )
			{
				continue ;
			}
		}
		else	if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = num ;
		ptrextra->Scale  = -1 ;

		ptrextra->Flags = EXTRA_STOP_COL
				| EXTRA_WAIT_NO_COL
				| EXTRA_BONUS
				| EXTRA_TAKABLE	;

		/* Special for Key ( Beurk cause detect with sprite number ! ) */

		if ( num != SPRITE_CLE ) ptrextra->Flags += EXTRA_TIME_OUT
							 +  EXTRA_FLASH 	;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		InitFly( ptrextra, alpha, beta, 40, 15 ) ;

		ptrextra->HitForce = 0 ;

		ptrextra->Timer = TimerRefHR ;
		ptrextra->TimeOut = 1000 * 20 ;	/* 20 secondes */

		if( !nb )	nb++ ;
		ptrextra->Divers = nb ;

		ptrextra->Body = -1 ;	// C'est un sprite !!!

		return n ;
	}

	if( num==SPRITE_CLE
	AND !flagkill )// vite bcl infinie
	{
		// on veut imperativement la donner !!!!!!!!!!!
		// du moins on essaie au moins une fois !
		flagkill = TRUE ;
		goto searchextra ;
	}

	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
S32	ExtraLabyrinthe( S32 x, S32 y, S32 z )
{
	T_EXTRA	*ptrextra ;
	S32	n ;
	S16	num = 4 ;

	if( !ModeLabyrinthe )	return -1 ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = (S16)(32768+num) ;	// explo
		ptrextra->Scale  = -1 ;

		ptrextra->Flags = EXTRA_TIME_OUT ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;
		ptrextra->HitForce = 0 ;

		ptrextra->Timer = TimerRefHR ;
		ptrextra->TimeOut = 50*20 ;
		ptrextra->Body = -1 ;

		return n ;
	}
	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	MagicBallExplo(	S32 x, S32 y, S32 z, S32 beta )
{
	y = ((y/SIZE_BRICK_Y)+1)*SIZE_BRICK_Y ;

	CreateParticleFlow( 0, 0, 0, x, y, z, beta, FLOW_MAGIC_BALL ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
S32	SearchBonusKey()
{
	T_EXTRA	*ptrextra ;
	S32	n ;

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

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

S32	ExtraSearch(	U8 owner, S32 x, S32 y, S32 z, S16 num,
			U8 numobj, S32 speed, U8 hitforce )
{
	T_EXTRA	*ptrextra ;
	S32	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		/* S16 Timer: num obj search */
		/* Vz = speed */

		ptrextra->Sprite = num ;
		ptrextra->Scale  = -1  ;

		ptrextra->Flags = EXTRA_SEARCH_OBJ ;
		ptrextra->Divers = 0 ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		ptrextra->Owner = owner ;
		ptrextra->Timer = (U32)numobj ;

		ptrextra->Vz = (S16)speed ;
		ptrextra->HitForce = hitforce ;

		InitMove( &ptrextra->U.Move, speed ) ;

		ptrextra->Body = -1 ;	// C'est un sprite !!!

		return n ;
	}
	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

S32	ExtraSearchKey(	U8 owner, S32 x, S32 y, S32 z,
			S16 num, U8 numobj )
{
	T_EXTRA	*ptrextra ;
	S32	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		// slot libre

		// S16 Timer: num obj search
		// Vz = speed

		ptrextra->Sprite = num ;
		ptrextra->Scale  = -1 ;

		ptrextra->Flags = EXTRA_SEARCH_KEY ;
		ptrextra->Divers = 0 ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		ptrextra->Owner = owner ;
		ptrextra->Timer = (U32)numobj ;

		ptrextra->Vz = 4000 ;
		ptrextra->HitForce = 0 ;

		InitMove( &ptrextra->U.Move, 4000 ) ;

		ptrextra->Body = -1 ;	// C'est un sprite !!!

		return n ;
	}
	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	GetExtraZV( T_EXTRA *ptrextra, S32 *xmin, S32 *ymin, S32 *zmin,
				       S32 *xmax, S32 *ymax, S32 *zmax )
{
	if( !(ptrextra->Sprite&32768) )	// c'est un sprite
	{
		S16	*ptr = &PtrZvExtraRaw[ ptrextra->Sprite*8 + 2 ] ;

		*xmin = *ptr++ ;
		*xmax = *ptr++ ;
		*ymin = *ptr++ ;
		*ymax = *ptr++ ;
		*zmin = *ptr++ ;
		*zmax = *ptr++ ;
	}
	else
	{
		switch( ptrextra->Sprite&32767 )
		{
			case 3:	// c'est un objet 3D
			{
				S32	x0,z0, x1,z1 ;
				S32	size ;
				T_BODY_HEADER	*ptr ;

				// recharger body ici pour etre s–r qu'il n'a pas boug
				// dans le buffer HQR_Get()
				ptrextra->PtrBody = (U8*)GivePtrObjFix( ptrextra->Body ) ;

				ptr = (T_BODY_HEADER *)ptrextra->PtrBody ;

				x0    = ptr->XMin ;
				x1    = ptr->XMax ;
				*ymin = ptr->YMin ;
				*ymax = ptr->YMax ;
				z0    = ptr->ZMin ;
				z1    = ptr->ZMax ;

				// moyenne
				size = ((x1 - x0) + (z1 - z0)) >> 2 ;	// /4

				*xmin = - size ;
				*xmax =   size ;
				*zmin = - size ;
				*zmax =   size ;
			}; break ;

			default:
				*xmin = 0 ;
				*xmax = 0 ;
				*ymin = 0 ;
				*ymax = 0 ;
				*zmin = 0 ;
				*zmax = 0 ;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

U8	ExtraCheckObjCol( T_EXTRA *ptrextra, U8 owner )
{
	T_OBJET	*ptrobjt = ListObjet;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;
	U8	n ;

	GetExtraZV( ptrextra, &x0, &y0, &z0, &x1, &y1, &z1 ) ;

	x0 += ptrextra->PosX ;
	y0 += ptrextra->PosY ;
	z0 += ptrextra->PosZ ;
	x1 += ptrextra->PosX ;
	y1 += ptrextra->PosY ;
	z1 += ptrextra->PosZ ;

	for( n=0; n<NbObjets; n++, ptrobjt++ )
	{
		if( (ptrobjt->Obj.Body.Num != -1)
		AND (!(ptrobjt->Flags&INVISIBLE) OR !(ptrextra->Flags&EXTRA_BONUS))
		AND ((n != owner) OR (ptrextra->Flags&EXTRA_HIT_ANY) ) )
		{
			xt0 = ptrobjt->Obj.X + ptrobjt->XMin ;
			xt1 = ptrobjt->Obj.X + ptrobjt->XMax ;
			yt0 = ptrobjt->Obj.Y + ptrobjt->YMin ;
			yt1 = ptrobjt->Obj.Y + ptrobjt->YMax ;
			zt0 = ptrobjt->Obj.Z + ptrobjt->ZMin ;
			zt1 = ptrobjt->Obj.Z + ptrobjt->ZMax ;

			if( x0 < xt1
			AND x1 > xt0
			AND y0 < yt1
			AND y1 > yt0
			AND z0 < zt1
			AND z1 > zt0 )
			{
				if( n==owner
				AND ptrextra->Flags&EXTRA_WAIT_NO_COL_OWNER )
				{
					continue ;
				}

				if( owner!=255 )
				{
					if( ptrextra->HitForce != 0 )
					{
						if( ptrextra->Flags&EXTRA_RENVOYABLE
						AND ptrobjt->WorkFlags&OK_RENVOIE )
						{
							S32 angle = GetAngle2D( ptrobjt->Obj.X, ptrobjt->Obj.Z,
									ptrextra->PosX, ptrextra->PosZ ) ;

							angle = (ptrobjt->Obj.Beta-angle+CONE_GANT/2) & (MAX_ANGLE-1) ;

							if( angle <= CONE_GANT )	// 2048: CONE_GANT
							{
								ptrextra->Vx = (S16)-ptrextra->Vx ;
								ptrextra->Vz = (S16)-ptrextra->Vz ;
								ptrextra->Vy = 10 ;
								ptrextra->Poids = 2 ;
								ptrextra->U.Org.X = ptrextra->PosX ;
								ptrextra->U.Org.Y = ptrextra->PosY ;
								ptrextra->U.Org.Z = ptrextra->PosZ ;
								ptrextra->Timer = TimerRefHR ;
								ptrextra->Owner = n ;	// new owner
								ptrextra->HitForce = ptrextra->NewForce ;
								ptrextra->Flags &= ~(EXTRA_NEW_IMPACT) ;

								return 255 ;	// on fait comme si l'extra n'avait
										// touch personne
							}
						}

						HitObj( owner, n, ptrextra->HitForce, -1 ) ;
					}
					else	ptrobjt->HitBy = owner ;

					if( ((ptrextra->Flags&EXTRA_MASK_WEAPON)==EXTRA_MAGIC_BALL)
					AND (ptrobjt->Flags&SPRITE_3D) )// les obj 3d ont leur propre sample
					{
						HQ_3D_MixSample( SAMPLE_EXPLO_MAGIC_BALL, 0x1000, 300, 1,
								 ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
					}
				}

				return n ;
			}
			else if( n==owner )
			{
				ptrextra->Flags &= ~(EXTRA_WAIT_NO_COL_OWNER) ;
			}
		}
	}
	return 255 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

U8	ExtraSearchCheckObjCol( T_EXTRA *ptrextra,
				S32 oldx, S32 oldy, S32 oldz,
				U8 owner )
{
	T_OBJET	*ptrobjt = ListObjet;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;
	S32	dx, dy, dz ;
	S32	zvinzv ;
	U8	n ;

	GetExtraZV( ptrextra, &x0, &y0, &z0, &x1, &y1, &z1 ) ;

	oldx += x0 ;
	oldy += y0 ;
	oldz += z0 ;

	dx = x1 - x0 ;
	dy = y1 - y0 ;
	dz = z1 - z0 ;

	x0 += ptrextra->PosX ;
	y0 += ptrextra->PosY ;
	z0 += ptrextra->PosZ ;
	x1 += ptrextra->PosX ;
	y1 += ptrextra->PosY ;
	z1 += ptrextra->PosZ ;

	for( n=0; n<NbObjets; n++, ptrobjt++ )
	{
		if( (ptrobjt->Obj.Body.Num != -1)
		AND !(ptrobjt->Flags&INVISIBLE)
		AND ((n != owner) OR (ptrextra->Flags&EXTRA_HIT_ANY) ) )
		{
			zvinzv = FALSE ;

			xt0 = ptrobjt->Obj.X + ptrobjt->XMin ;
			xt1 = ptrobjt->Obj.X + ptrobjt->XMax ;
			yt0 = ptrobjt->Obj.Y + ptrobjt->YMin ;
			yt1 = ptrobjt->Obj.Y + ptrobjt->YMax ;
			zt0 = ptrobjt->Obj.Z + ptrobjt->ZMin ;
			zt1 = ptrobjt->Obj.Z + ptrobjt->ZMax ;

			if( x0 < xt1
			AND x1 > xt0
			AND y0 < yt1
			AND y1 > yt0
			AND z0 < zt1
			AND z1 > zt0 )
			{
				zvinzv = TRUE ;
			}
			else
			{
				// appel routine
				zvinzv = IntersectZV( x0, y0, z0,
						      dx, dy, dz,
						      xt0, yt0, zt0,
						      ptrobjt->XMax - ptrobjt->XMin,
						      ptrobjt->YMax - ptrobjt->YMin,
						      ptrobjt->ZMax - ptrobjt->ZMin,
						      oldx, oldy, oldz ) ;
			}

			if( zvinzv )
			{
				if( n==owner
				AND ptrextra->Flags&EXTRA_WAIT_NO_COL_OWNER )
				{
					continue ;
				}

				if( owner!=255 )
				{
					if( ptrextra->HitForce != 0 )
					{
						if( ptrextra->Flags&EXTRA_RENVOYABLE
						AND ptrobjt->WorkFlags&OK_RENVOIE )
						{
							S32 angle = GetAngle2D( ptrobjt->Obj.X, ptrobjt->Obj.Z,
									ptrextra->PosX, ptrextra->PosZ ) ;

							angle = (ptrobjt->Obj.Beta-angle+CONE_GANT/2) & (MAX_ANGLE-1) ;

							if( angle <= CONE_GANT )	// 2048: CONE_GANT
							{
								ptrextra->Vx = (S16)-ptrextra->Vx ;
								ptrextra->Vz = (S16)-ptrextra->Vz ;
								ptrextra->Vy = 0 ;
								ptrextra->Poids = 1 ;
								ptrextra->U.Org.X = ptrextra->PosX ;
								ptrextra->U.Org.Y = ptrextra->PosY ;
								ptrextra->U.Org.Z = ptrextra->PosZ ;
								ptrextra->Timer = TimerRefHR ;
								ptrextra->Owner = n ;	// new owner
								ptrextra->HitForce = ptrextra->NewForce ;
								ptrextra->Flags &= ~(EXTRA_NEW_IMPACT) ;

								return 255 ;	// on fait comme si l'extra n'avait
										// touch personne
							}
						}

						HitObj( owner, n, ptrextra->HitForce, -1 ) ;
					}
					else	ptrobjt->HitBy = owner ;

					if( ((ptrextra->Flags&EXTRA_MASK_WEAPON)==EXTRA_MAGIC_BALL)
					AND (ptrobjt->Flags&SPRITE_3D) )// les obj 3d ont leur propre sample
					{
						HQ_3D_MixSample( SAMPLE_EXPLO_MAGIC_BALL, 0x1000, 300, 1,
								 ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
					}
				}

				return n ;
			}
			else if( n==owner )
			{
				ptrextra->Flags &= ~(EXTRA_WAIT_NO_COL_OWNER) ;
			}
		}
	}
	return 255 ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

U8	ExtraCheckExtraCol( 	T_EXTRA *ptrextra,
				S32 oldx, S32 oldy, S32 oldz,
				U8 owner )
{
	T_EXTRA	*ptrobjt ;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;
	S32	dx, dy ,dz ;
	S32	dxt, dyt, dzt ;
	U8	n ;

	GetExtraZV( ptrextra, &x0, &y0, &z0, &x1, &y1, &z1 ) ;

	oldx += x0 ;
	oldy += y0 ;
	oldz += z0 ;

	dx = x1 - x0 ;
	dy = y1 - y0 ;
	dz = z1 - z0 ;

	x0 += ptrextra->PosX ;
	y0 += ptrextra->PosY ;
	z0 += ptrextra->PosZ ;
	x1 += ptrextra->PosX ;
	y1 += ptrextra->PosY ;
	z1 += ptrextra->PosZ ;

	ptrobjt = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++ )
	{
		if( (ptrobjt->Sprite != -1)
		AND (n != owner) )
		{
			GetExtraZV( ptrobjt, &xt0, &yt0, &zt0, &xt1, &yt1, &zt1 ) ;

			dxt = xt1 - xt0 ;
			dyt = yt1 - yt0 ;
			dzt = zt1 - zt0 ;

			xt0 += ptrobjt->PosX ;
			yt0 += ptrobjt->PosY ;
			zt0 += ptrobjt->PosZ ;
			xt1 += ptrobjt->PosX ;
			yt1 += ptrobjt->PosY ;
			zt1 += ptrobjt->PosZ ;

			if( x0 < xt1
			AND x1 > xt0
			AND y0 < yt1
			AND y1 > yt0
			AND z0 < zt1
			AND z1 > zt0 )
			{
				return n ;
			}
			else if( IntersectZV( x0, y0, z0,
					      dx, dy, dz,
					      xt0, yt0, zt0,
					      dxt, dyt, dzt,
					      oldx, oldy, oldz ) )
			{
				return n ;
			}
		}
		ptrobjt++ ;
	}
	return 255 ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

U8	FullWorldColBrick(	S32 oldx, S32 oldy, S32 oldz,
				S32 newx, S32 newy, S32 newz )
{
	S32	x0, y0, z0 ;
	S32	x1, y1, z1 ;
	S32	x2, y2, z2 ;

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

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		   ÛÛ฿฿฿ Û฿฿฿Û Û฿฿฿฿ Û฿฿฿฿  Û    Û฿฿฿Û Û
		   ฿฿฿฿Û ÛÛ฿฿฿ ÛÛ฿฿  ÛÛ     ÛÛ   ÛÛ฿฿Û ÛÛ
		   ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿฿฿฿  ฿฿   ฿฿  ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ClearScreenMinMax()
{
	ScreenXMin = 32000 ;
	ScreenXMax = -32000 ;
	ScreenYMin = 32000 ;
	ScreenYMax = -32000 ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AdjustScreenMinMax()
{
	if( Xp < ScreenXMin )	ScreenXMin = Xp ;
	if( Xp > ScreenXMax )	ScreenXMax = Xp ;
	if( Yp < ScreenYMin )	ScreenYMin = Yp ;
	if( Yp > ScreenYMax )	ScreenYMax = Yp ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	Aff2DShape(	S16 *ptr,
			S32 xs, S32 ys,
			S32 coul, S32 angle, S32 zoom )
{
	S32	x0, y0, x1, y1 ;
	S32	x, y, n, nb ;

	ClearScreenMinMax() ;

	nb = *ptr++ ;

	x = (*ptr++) * zoom / 16 ;
	y = (*ptr++) * zoom / 16 ;
	Rotate( x, y, angle ) ;
	Xp = x0 = X0 + xs ;
	Yp = y0 = Z0 + ys ;
	AdjustScreenMinMax() ;

	for( n=1; n<nb; n++ )
	{
		x1 = Xp ;
		y1 = Yp ;

		x = (*ptr++) * zoom / 16 ;
		y = (*ptr++) * zoom / 16 ;
		Rotate( x, y, angle ) ;
		Xp = X0 + xs ;
		Yp = Z0 + ys ;
		AdjustScreenMinMax() ;

		Line( x1,y1, Xp, Yp, coul ) ;
	}
	Line( Xp,Yp, x0, y0, coul ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AffSpecial( S32 numextra )
{
	T_EXTRA	*ptrextra ;
	S32	angle ;
	S32	scale ;

	ptrextra = &ListExtra[numextra] ;

	switch( ptrextra->Sprite&32767 )
	{
		case 0: // Pof
			if( ptrextra->Divers!=0 )
			{
				// pas de gestion pendant les messages
				if( !FlagAnimWhoSpeak )
				{
					angle = (((TimerRefHR-ptrextra->Timer)*4096)/ptrextra->Divers)&4095 ;
					ptrextra->Impact = angle ;	// memo angle
				}
				else	angle = ptrextra->Impact ;
			}
			else	angle = 0 ;

			scale = ptrextra->Scale ;

			if( scale!=ptrextra->Beta )
			{
				if( !FlagAnimWhoSpeak )	// pas de gestion pendant les messages
				{
					scale = BoundRegleTrois( scale, ptrextra->Beta, (S32)(ptrextra->PtrBody), TimerRefHR-ptrextra->Timer ) ;

					if( scale==ptrextra->Beta )
					{
						// on a atteind la taille dsire, ciao !!!!
						ptrextra->Sprite = -1 ;
					}
				}
				else	scale = ptrextra->Info ;
			}

			ptrextra->Info = scale ;

			PofDisplay3DExt( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ,
					 ptrextra->Body, scale, angle ) ;
			break ;

		case 3: // Objet 3D
			ptrextra->PtrBody = (U8*)GivePtrObjFix( ptrextra->Body ) ;

			BodyDisplay_AlphaBeta( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ,
					       ptrextra->Scale, ptrextra->Beta, 0,
					       ptrextra->PtrBody );
			break ;

		case 4:	// Labyrinthe
			AffOneBrick( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
			break ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

S32	InitExtraPof( S32 x, S32 y, S32 z, S16 num, S32 alpha, S32 beta, S32 speed, S32 poids, S32 scaledeb, S32 scalefin, S32 duree, S32 rotation, S32 timeout )
{
	T_EXTRA	*ptrextra ;
	S32	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = (S16)(0+32768) ;

		ptrextra->Divers = (S16)rotation ;

		ptrextra->Body = num ;
		ptrextra->Scale = scaledeb ;
		ptrextra->Info  = scaledeb ;
		ptrextra->Beta  = (S16)scalefin ;

		ptrextra->PtrBody = (U8*)duree ;

		ptrextra->Flags = EXTRA_END_COL|EXTRA_NO_SHADOW ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		InitFly( ptrextra, alpha, beta, speed, (U8)poids ) ;

		ptrextra->HitForce = 0 ;

		ptrextra->Timer = TimerRefHR ;
		ptrextra->TimeOut = (S16)timeout ;

		if( timeout>0 )	ptrextra->Flags|=EXTRA_TIME_OUT ;

		return n ;
	}

	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

S32	InitExtraAnimSprite( 	U8 owner, S32 x, S32 y, S32 z,
				S16 deb, S16 fin, S32 tempo, S32 scale, S32 transparent,
				U8 hitforce )
{
	T_EXTRA	*ptrextra ;
	S32	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )	continue ;
		/* slot libre */

		ptrextra->Sprite = ptrextra->Body = deb ;
		ptrextra->Divers = (S16)(fin+1) ;	// +1 pour detecter la fin par BoundRegleTrois()

		ptrextra->Flags = EXTRA_ANIM_SPRITE ;
		if( transparent )	ptrextra->Flags |= EXTRA_TRANSPARENT ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		ptrextra->HitForce = hitforce ;

		if( hitforce>0 )
		{
			ptrextra->Flags |= EXTRA_END_OBJ
					|  EXTRA_END_COL
					|  EXTRA_WAIT_NO_COL
					|  EXTRA_IMPACT ;
		}

		ptrextra->Owner = owner ;

		ptrextra->Timer = TimerRefHR ;
		ptrextra->TimeOut = (S16)tempo ;	// tempo

		ptrextra->Scale = scale ;

		return n ;
	}

	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

S16	CoulRetourBalle( T_EXTRA *ptrextra )
{
	S16	sprite = SPRITE_BALLE_SHADOW ;	// jaune ou feu

	if( ptrextra->Sprite == SPRITE_BALLE_LVL_2 )	sprite = SPRITE_BALLE_SHADOW_2 ;	// vert
	if( ptrextra->Sprite == SPRITE_BALLE_LVL_3 )	sprite = SPRITE_BALLE_SHADOW_3 ;	// rouge

	return	sprite ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	  Û฿฿฿฿ Û฿฿฿Û ÛÜ ÜÛ Û฿฿฿฿       Û฿฿฿฿ Û฿฿฿Û Û     Û     ÛÛ฿฿฿
	  ÛÛ ฿Û ÛÛ฿฿Û ÛÛ฿ Û ÛÛ฿฿        ÛÛ    ÛÛ฿฿Û ÛÛ    ÛÛ    ฿฿฿฿Û
	  ฿฿฿฿฿ ฿฿  ฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ClearExtra()
{
	T_EXTRA	*ptrextra = ListExtra ;
	S32	n ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		ptrextra->Sprite = -1 ;
		ptrextra->Body   = -1 ;
		ptrextra->Divers = 1 ;
	}

}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
S32	ThrowExtra(	U8 owner,
			S32 x, S32 y, S32 z,
			S16 num,
			S32 alpha, S32 beta, S32 vitesse, U8 poids,
			U8 hitforce )
{
	T_EXTRA	*ptrextra ;
	S32	n ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 ) 	continue ;	/* slot libre */

		ptrextra->Sprite = num ;
		ptrextra->Scale  = -1 ;

		ptrextra->Flags = EXTRA_END_OBJ
				+ EXTRA_END_COL
//				+ EXTRA_WAIT_NO_COL
				+ EXTRA_IMPACT ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		InitFly( ptrextra, alpha, beta, vitesse, poids ) ;

		ptrextra->HitForce = hitforce ;

		ptrextra->Owner = owner ;

		ptrextra->Timer = TimerRefHR ;

		ptrextra->Divers = 0 ;

		ptrextra->PtrBody = 0 ;
		ptrextra->Body    = -1 ;	// c'est un sprite !!!

		return n ;
	}
	return -1 ;
}

// le 29/05/95 pour les projectiles 3D
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
S32	ThrowExtraObj(	U8 owner,
			S32 x, S32 y, S32 z,
			S32 indexfile3d,
			S32 alpha, S32 beta, S32 vitesse, S16 alpharot,
			U8 poids, U8 hitforce )
{
	T_EXTRA	*ptrextra ;
	S32	n ;
	S32	num = 3 ;

	ptrextra = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 ) 	continue ;	// slot libre

		ptrextra->Body    = (S16)indexfile3d ;
		ptrextra->PtrBody = (U8*)GivePtrObjFix( indexfile3d ) ;

		ptrextra->Sprite = (S16)(num+32768) ;
		ptrextra->Scale  = 0 ;

		ptrextra->Flags = EXTRA_END_OBJ
				+ EXTRA_END_COL
				+ EXTRA_IMPACT ;

		ptrextra->PosX = x ;
		ptrextra->PosY = y ;
		ptrextra->PosZ = z ;

		// Au-dessus de 1023, l'objet part dans l'autre sens
/*
		// ATTENTION: peut-etre dangereux sans ce test
		if( alpha>1023 )
		{
			alpha &= 1023 ;
			beta += 2048 ;
		}
*/
		beta &= 4095 ;

		InitFly( ptrextra, alpha, beta, vitesse, poids ) ;

		ptrextra->HitForce = hitforce ;

		ptrextra->Owner = owner ;

		ptrextra->Timer = TimerRefHR ;

		ptrextra->Divers = 0 ;

		ptrextra->Beta = (S16)beta ;

		if( alpharot>0 )	ptrextra->TimeOut = (S16)(alpharot/20) ;
		else			ptrextra->TimeOut = alpharot ;

		return n ;
	}

	return -1 ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	ThrowExtraFoudre( U8 owner )
{
	T_EXTRA	*ptrextra ;
	S32	n ;
	S32	m ;
	S32	t ;
	S32	a ;

	t = TimerRefHR ;

	// peut-etre faut-il obliger le sort de foudre a se lancer
	// (si ListExtra est plein)(cf scene finale !)

	for( m=0; m<8; m++ )
	{
		ptrextra = ListExtra ;

		for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
		{
			if( ptrextra->Sprite == -1 )	// slot libre
			{
				ptrextra->Info  = m  ; // num foudre
				ptrextra->Sprite= SPRITE_FOUDRE ; // numsprite

				ptrextra->Timer   = t - m*40 ;
				ptrextra->Flags = (EXTRA_FOUDRE_STEP1|EXTRA_TRANSPARENT) ;

				ptrextra->Owner = owner ;

				// Init pour la 1st frame
				ptrextra->Scale = 50 + (7*8) - 7 * m ;

				a = (t - ptrextra->Timer)*8 ;

				Rotate( 0, 600-(t - ptrextra->Timer)/2, a ) ;

				ptrextra->PosX = ListObjet[ptrextra->Owner].Obj.X + X0 ;
				ptrextra->PosY = ListObjet[ptrextra->Owner].Obj.Y + 1000 + (t - ptrextra->Timer)  ;
				ptrextra->PosZ = ListObjet[ptrextra->Owner].Obj.Z + Z0 ;
				break ;
			}
		}

		if( n==MAX_EXTRAS )	break ;
	}

	for( m=0; m<8; m++ )
	{
		ptrextra = ListExtra ;

		for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
		{
			if( ptrextra->Sprite == -1 )	// slot libre
			{
				ptrextra->Info  = m+8  		; // num foudre
				ptrextra->Sprite= SPRITE_FOUDRE ; // numsprite

				ptrextra->Timer = t - m*40 ;
				ptrextra->Flags = (EXTRA_FOUDRE_STEP1|EXTRA_TRANSPARENT) ;

				ptrextra->Owner = owner ;

				// Init pour la 1st frame
				ptrextra->Scale = 50 + (7*8) - 7 * m ;

				a = (t - ptrextra->Timer)*8 ;
				a += 2048 ;

				Rotate( 0, 600-(t - ptrextra->Timer)/2, a ) ;

				ptrextra->PosX = ListObjet[ptrextra->Owner].Obj.X + X0 ;
				ptrextra->PosY = ListObjet[ptrextra->Owner].Obj.Y + 1000 + (t - ptrextra->Timer)  ;
				ptrextra->PosZ = ListObjet[ptrextra->Owner].Obj.Z + Z0 ;
				break ;
			}
		}

		if( n==MAX_EXTRAS )	break ;
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	InvoqueFoudre( U8 owner, S32 x, S32 y, S32 z )
{
	T_EXTRA	*ptrextra = ListExtra ;
	T_OBJET	*ptrobj = ListObjet ;
	S32	extra ;
	S32	n ;
	U8	force ;

	// vire tous les extras de foudre de la liste
	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Flags&(EXTRA_FOUDRE_STEP1|EXTRA_FOUDRE_STEP2|EXTRA_FOUDRE_STEP3) )
		{
			ptrextra->Sprite = -1 ;
			ptrextra->Flags  = 0  ;
		}
	}

	// tonerre
	HQ_3D_MixSample( SAMPLE_FOUDRE_STEP3, 0x1000, 500, 1, x, ListObjet[owner].Obj.Y+1000, z ) ;

	n = InitIncrustDisp( INCRUST_ECLAIR, 0, 0, 0, 0, 0, 1 ) ;

	if( n!=-1 )
	{
		ListIncrustDisp[n].TimerEnd = TimerRefHR + 500 ;
	}

	for( n=0; n<NbObjets; n++, ptrobj++ )
	{
		if( (n!=owner)
		AND (ptrobj->WorkFlags & WAS_DRAWN)
		AND (ptrobj->Obj.Body.Num!=-1)
		AND (ptrobj->Armure<51)
		AND !(ptrobj->Flags&INVISIBLE) )
		{
			if( ptrobj->Armure<=MagicBallHitForce[MagicLevel] )
			{
				force = 255 ;

				if( (255-ptrobj->Armure)<ptrobj->LifePoint )
				{
					// grosse magouille pour etre sur de tuer l'objet
					ptrobj->LifePoint -= (255-ptrobj->Armure) ;
				}
			}
			else	force = MagicBallHitForce[MagicLevel] ;

			extra = ExtraSearch( owner, ptrobj->Obj.X, y, ptrobj->Obj.Z, SPRITE_FOUDRE,
					     (U8)n, 15000, force ) ;

			if( extra!=-1 )
			{
				ListExtra[extra].Flags |= EXTRA_TRANSPARENT ;
			}
		}
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	ThrowExtraProtection( U8 owner )
{
	T_EXTRA	*ptrextra ;
	S32	m, n ;
	S32	t ;

	t = TimerRefHR ;

	// peut-etre faut-il obliger le sort de foudre a se lancer
	// (si ListExtra est plein)(cf scene finale !)

	for( m=0; m<8; m++ )
	{
		ptrextra = ListExtra ;

		for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
		{
			if( ptrextra->Sprite == -1 ) // free slot
			{
				if( m==0 )
				{
					InitMove( &ptrextra->U.Move, PROTECT_SPEED_MAGIC ) ;
				}

				ptrextra->Info   = m ;
				ptrextra->Sprite = SPRITE_PROTECT ;// numsprite
				ptrextra->Timer  = t + m*40 ;
				ptrextra->Flags  = EXTRA_PROTECT|EXTRA_TRANSPARENT ;
				ptrextra->Scale  = 70 + 7 * m ;
				ptrextra->Owner  = owner ;
				break ;
			}
		}

		if( n==MAX_EXTRAS )	break ;
	}

	for( m=0; m<8; m++ )
	{
		ptrextra = ListExtra ;

		for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
		{
			if( ptrextra->Sprite == -1 ) // free slot
			{
				ptrextra->Info   = m+8 ;
				ptrextra->Sprite = SPRITE_PROTECT ;// numsprite
				ptrextra->Timer  = t + m*40 ;
				ptrextra->Flags  = EXTRA_PROTECT|EXTRA_TRANSPARENT ;
				ptrextra->Scale  = 70 + 7 * m ;
				ptrextra->Owner  = owner ;
				break ;
			}
		}

		if( n==MAX_EXTRAS )	break ;
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	AnnuleExtraProtection( void )
{
	T_EXTRA	*ptrextra = ListExtra ;
	S32	m ;

	// peut-etre faut-il obliger le sort de foudre a se lancer
	// (si ListExtra est plein)(cf scene finale !)

	for( m=0; m<MAX_EXTRAS; m++, ptrextra++ )
	{
		if( ptrextra->Flags&EXTRA_PROTECT )
		{
			ptrextra->Flags  =  0 ;
			ptrextra->Sprite = -1 ;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

/*
-----------------------------------------------------------
		Dcide quel bonus donner:
-----------------------------------------------------------

	0: Money
	1: Life
	2: Magic
	3: Key
	4: Clover

-----------------------------------------------------------
*/

U8	WhichBonus( S32 flags )	// types de bonus demands
{
	U8	n, m = 0 ;
	U8	tabbonus[5] ;
	S32	curr, max ;
	S32	num ;
	U8	besoin = 0 ;

	// cherche les bonus disponibles
	for( n=0; n<5; n++ )
	{
		if( flags & (1<<(n+4)) )
		{
//			if( n!=2 OR MagicLevel )
			{
				tabbonus[m] = n ;
				m++ ;
			}
		}
	}

	// si on n'a demand que de la magie avec MagicLevel==0 alors on
	// donne de la vie
/*	if( !m AND (flags & (1<<(2+4)) ) )
	{
		tabbonus[m] = 1 ;	// Life
		m++ ;
	}
*/
	if( !m )	return 255 ;	// aucun bonus

	max = 0 ;

	// Cherche de quel bonus on a le plus besoin
	for( n=0; n<m; n++ )
	{
		// calcul une moyenne en fonction du type de bonus
		switch( tabbonus[n] )
		{
			case 0:	// Money
				if( Planet>=2 )	num = NbZlitosPieces	;
				else		num = NbGoldPieces	;

				if( num>0 )
					curr = (NecessaryChapterMoney[ ListVarGame[FLAG_CHAPTER] ]<<8)
					      /num	 ;
				else	curr = NecessaryChapterMoney[ ListVarGame[FLAG_CHAPTER] ]<<8 ;
				break ;

			case 1:	// Life
				if( ListVarGame[FLAG_CONQUE] )
				{
					// Blindage
					if( ListObjet[NUM_PERSO].LifePoint+TabInv[FLAG_CONQUE].PtMagie > 15 )
					{
						curr = ((MAX_LIFE_POINTS+INV_MAX_VALUE)<<8)
						      /(ListObjet[NUM_PERSO].LifePoint+TabInv[FLAG_CONQUE].PtMagie) ;
					}
					else	curr = (MAX_LIFE_POINTS+INV_MAX_VALUE)<<8 ;
				}
				else
				{
					// Blindage
					if( ListObjet[NUM_PERSO].LifePoint>10 )
						curr = (MAX_LIFE_POINTS<<8)/ListObjet[NUM_PERSO].LifePoint ;
					else	curr = MAX_LIFE_POINTS<<8 ;
				}
				break ;

			case 2:	// Magic
				if( MagicLevel )
				{
					// Blindage
					if( MagicPoint>0 )
						curr = ((MagicLevel*20)<<8)/MagicPoint ;
					else	curr = (MagicLevel*20)<<8 ;
				}
				else
				{	// Normalement impossible ici
					curr = 0 ;
				}
				break ;

			default:	// Key or Clover
				curr = 0 ;	// Normalement, on donne des
						// cls ou des trfle sans
						// autre choix
		}

		if( curr>max )
		{
			max = curr ;
			besoin = n ;
		}
	}

	// dernire solution, si tous les compteurs (Life, Gold & Magic)
	// sont au max, on en choisit un au hasard
	if( max<=256 )
	{
		n = tabbonus[ MyRnd( m ) ] ;
	}
	else	n = tabbonus[ besoin ] ;

//	if( (MagicLevel == 0) AND (n==2) )	n = 1 ; // si pas magie -> coeur

	if( n==0 )	// Money demand
	{
		n = (U8)SPRITE_PIECE ;	// determine sprite (Kashes ou Zlitos)
	}
	else	n += 3 ;	// offset sprite bonus

	return( n ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	GiveExtraBonus( T_OBJET *ptrobj )
{
	S32	beta ;
	U8	n ;

	n = WhichBonus( ptrobj->OptionFlags ) ;
	if( n==255 )	return ;

	if( ptrobj->WorkFlags & OBJ_DEAD
	OR  ptrobj->LifePoint<=0 )	// obj mort
	{
		ExtraBonus(	ptrobj->Obj.X,
				ptrobj->Obj.Y,
				ptrobj->Obj.Z,
				(256*MUL_ANGLE), 0,
				(S16)(n/*+3*/),	/* 3 offset spr bonus */
				ptrobj->NbBonus ) ;

		HQ_3D_MixSample( SAMPLE_BONUS, 0x1000, 0, 1,
			ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;

	}
	else					// obj vivant
	{
		beta =	GetAngle2D( ptrobj->Obj.X, ptrobj->Obj.Z,
				  ListObjet[NUM_PERSO].Obj.X, ListObjet[NUM_PERSO].Obj.Z ) ;

		beta += MyRnd(RND_BETA_BONUS) - (RND_BETA_BONUS/2) ;

		ExtraBonus(	ptrobj->Obj.X,
				ptrobj->Obj.Y+ptrobj->YMax,
				ptrobj->Obj.Z,
				200*MUL_ANGLE,
				beta,
				(S16)(n/*+3*/),	/* 3 offset spr bonus */
				ptrobj->NbBonus ) ;

		HQ_3D_MixSample( SAMPLE_BONUS, 0x1000, 0, 1,
			ptrobj->Obj.X, ptrobj->Obj.Y+ptrobj->YMax, ptrobj->Obj.Z ) ;

	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	ZoneGiveExtraBonus( T_ZONE *ptrz )
{
	S32	p ;
	S32	x, z ;
	S32	beta ;
	U8	n ;

	if( ptrz->Info2 )	return ;	/* dj… pris */

	n = WhichBonus( ptrz->Info0 ) ;
	if( n==255 )	return ;

	x = (ptrz->X0+ptrz->X1)/2 ;
	z = (ptrz->Z0+ptrz->Z1)/2 ;

	beta = GetAngle2D( x, z,
			 ListObjet[NUM_PERSO].Obj.X, ListObjet[NUM_PERSO].Obj.Z ) ;

	beta += MyRnd(RND_BETA_BONUS) - (RND_BETA_BONUS/2) ;

	p = ExtraBonus(	x, ptrz->Y1, z,
			(180*MUL_ANGLE),
			beta,
			(S16)n,
			(S16)ptrz->Info1 ) ; /* nb */

	if( p != -1 )
	{
		ListExtra[p].Flags |= EXTRA_TIME_IN ;
		ptrz->Info2 = 1 ;		/* marque prise */
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// Retourne le numro d'extra le plus prs du point (x,z) (-1 si aucun extra)
S32	PlusProcheExtra( S32 x, S32 z, S32 flags )
{
	T_EXTRA	*ptrextra = ListExtra ;
	S32	n ;
	S32	dist = 32768 ;
	S32	num = -1 ;
	S32	newdist ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite==-1 )	continue ;
		if( !(ptrextra->Flags&flags) )	continue ;

		newdist = Distance2D( x, z, ptrextra->PosX, ptrextra->PosZ ) ;

		if( newdist<dist )
		{
			dist = newdist ;
			num = n ;
		}
	}

	return( num ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	ReajustPosExtra( T_EXTRA *ptrextra, U8 col, S16 *ptrymin )
{
	S32	xw, yw, zw ;
	S32	x, y, z ;
	S32	coly = 0 ;

	x = ptrextra->PosX ;
	y = ptrextra->PosY ;
	z = ptrextra->PosZ ;

//	xw = XMap * SIZE_BRICK_XZ ; // coin superieur gauche de la brick
//	yw = YMap * SIZE_BRICK_Y  ;
//	zw = ZMap * SIZE_BRICK_XZ ;

	xw = (x/SIZE_BRICK_XZ) * SIZE_BRICK_XZ ; // coin superieur gauche de la brick
	yw = (y/SIZE_BRICK_Y)  * SIZE_BRICK_Y  ;
	zw = (z/SIZE_BRICK_XZ) * SIZE_BRICK_XZ ;

	switch( col )	// collision complexes
	{
	case 6:
		if( x-xw < z-zw )	col = 3 ;
		else			col = 2 ;
		break ;

	case 7:
		if( x-xw < z-zw )	col = 5 ;
		else			col = 4 ;
		break ;

	case 10:
		if( x-xw < z-zw )	col = 2 ;
		else			col = 3 ;
		break ;

	case 11:
		if( x-xw < z-zw )	col = 4 ;
		else			col = 5 ;
		break ;

	case 8:
		if( SIZE_BRICK_XZ-(x-xw) > z-zw )	col = 4 ;
		else					col = 2 ;
		break ;

	case 9:
		if( SIZE_BRICK_XZ-(x-xw) > z-zw )	col = 5 ;
		else					col = 3 ;
		break ;

	case 12:
		if( SIZE_BRICK_XZ-(x-xw) > z-zw )	col = 2 ;
		else					col = 4 ;
		break ;

	case 13:
		if( SIZE_BRICK_XZ-(x-xw) > z-zw )	col = 3 ;
		else					col = 5 ;
		break ;
	}

	switch( col )	// collision de base
	{
	case 2:
		coly = yw + BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ, x - xw ) ;
		break ;

	case 3:
		coly = yw + BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ, z - zw ) ;
		break ;

	case 4:
		coly = yw + BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ, z - zw ) ;
		break ;

	case 5:
		coly = yw + BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ, x - xw ) ;
		break ;

	default:
		coly = yw + SIZE_BRICK_Y ;
		break ;
	}

	if( coly )
	{
		y = coly - *ptrymin ;
	}

	ptrextra->PosY = y ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
S32	NewMagicBallRebond = 0 ;

#define	BACK_EXPLO	1
#define	BACK_SOUND	2

void	InitBackMagicBall( T_EXTRA *ptrextra, S16 num, S32 flags )
{
	if( flags&BACK_SOUND )
	{
		HQ_3D_MixSample( SAMPLE_EXPLO_MAGIC_BALL, 0x1000, 300, 1,
			ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
	}

	if( flags&BACK_EXPLO )
	{
		MagicBallExplo( ptrextra->PosX,
				ptrextra->PosY,
				ptrextra->PosZ,
				GetAngle2D( ptrextra->PosX, ptrextra->PosZ,
					  ptrextra->PosX+ptrextra->Vx,
					  ptrextra->PosZ+ptrextra->Vz ) ) ;
	}

	// init retour balle
	MagicBall = ExtraSearch(255,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
				num,
				NUM_PERSO, 10000, 0 ) ;

	if( MagicBall!=-1 )
	{
		ListExtra[MagicBall].Flags |= EXTRA_MAGIC_BALL ;
		InitMagicBall = TRUE ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	GereExtras()
{
	T_EXTRA	*ptrextra ;
	S32	n ;
	S32	oldx, oldy, oldz ;
	S32	time ;
	S32	timer ;
	S16	*ptr ;
	S32	alpha, beta, s ;
	S32	flagcol ;
	S32	type ;	// type d'extra (arme,..)
	T_DART	*ptrd ;
	U8	col, owner, numobj, search ;
	S32     taken ;

	ptrextra = ListExtra ;

	timer = TimerRefHR ;

	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite == -1 )	continue ;
		if( InitMagicBall AND n==MagicBall )	continue ;
		if( ptrextra->Flags & EXTRA_WAIT )	continue ;
		if( ptrextra->Flags & EXTRA_TIME_IN )	continue ;

		type = ptrextra->Flags & EXTRA_MASK_WEAPON ;

		if( ptrextra->Flags & EXTRA_TIME_OUT )
		{
			if( TimerRefHR >= ptrextra->Timer + ptrextra->TimeOut )
			{
				/* bye bye */
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		//
		//	Type trainee de la balle magique
		//
		if( ptrextra->Flags & EXTRA_TRAINEE )
		{
			if( MagicBall!=-1
			AND !(ListExtra[MagicBall].Flags&EXTRA_SEARCH_OBJ) ) // on ne s'y interesse
									     // pas au retour
			{
				T_EXTRA *ptrball = &ListExtra[MagicBall] ;

				if( NewMagicBallRebond==(3-ptrextra->Body)
				AND (TimerRefHR>(ptrball->Timer+ptrextra->Body)) )
				{
					ptrextra->Vx    = ptrball->Vx ;
					ptrextra->Vy    = ptrball->Vy ;
					ptrextra->Vz	= ptrball->Vz ;
					ptrextra->U.Org.X  = ptrball->U.Org.X ;
					ptrextra->U.Org.Y  = ptrball->U.Org.Y ;
					ptrextra->U.Org.Z  = ptrball->U.Org.Z ;

					ptrextra->Timer = ptrball->Timer ;

					NewMagicBallRebond-- ;

					time = (TimerRefHR - (ptrball->Timer+(ptrextra->Body+1)*20))/20 ;
				}
				else
				{
					if( NewMagicBallRebond )
						time = (TimerRefHR - (ptrextra->Timer+(ptrextra->Body+1)*20))/20 ;
					else	time = (TimerRefHR - (ptrball->Timer+(ptrextra->Body+1)*20))/20 ;
				}

				ptrextra->PosX = ptrextra->Vx*time + ptrextra->U.Org.X ;
				ptrextra->PosY =  ptrextra->Vy*time
						+ ptrextra->U.Org.Y
						- (ptrextra->Poids*time*time)/16 ;
				ptrextra->PosZ = ptrextra->Vz*time + ptrextra->U.Org.Z ;
			}
			else	ptrextra->Sprite = -1 ;	// Ciao !!!!!!!!

			continue ; //	t'occupe pas du reste
		}

		if( ptrextra->Flags & EXTRA_FLY )
		{
			time = (TimerRefHR - ptrextra->Timer)/20 ;

			oldx = ptrextra->PosX ;
			oldy = ptrextra->PosY ;
			oldz = ptrextra->PosZ ;

			ptrextra->PosX = ptrextra->Vx*time + ptrextra->U.Org.X ;
			ptrextra->PosY =  ptrextra->Vy*time
					+ ptrextra->U.Org.Y
					- (ptrextra->Poids*time*time)/16 ;
			ptrextra->PosZ = ptrextra->Vz*time + ptrextra->U.Org.Z ;

			// Est-ce un extra Obj 3D
			if( (ptrextra->Sprite&32768)
			AND (ptrextra->Sprite&32767)==3 )
			{
				// Calcule Alpha de l'objet 3d
				// TimeOut:alpharot
				if( ptrextra->TimeOut>0 )
				{
					ptrextra->Scale = ((4096*time)/ptrextra->TimeOut)&4095 ;
				}
				else if( ptrextra->TimeOut<0 )
				{
					// alpha suivant la trajectoire de l'extra
					ptrextra->Scale = 1024 - GetAngleVector2D(
							Distance2D( 0,0,ptrextra->Vx * 5,ptrextra->Vz * 5 ),
							-ptrextra->Vy * 5 + (ptrextra->Poids*10*time+25)/16 ) ;
				}
				else	ptrextra->Scale = 0 ;
			}

			// Vrifie qu'on se dplace pas plus d'un pseudo, sinon
			// on passerait … travers
			if( abs(ptrextra->PosY-oldy) >= SIZE_BRICK_Y )
			{
				if( ptrextra->PosY>oldy )
				{
					ptrextra->PosY = oldy + (SIZE_BRICK_Y-1) ;
				}
				else
				{
					ptrextra->PosY = oldy - (SIZE_BRICK_Y-1) ;
				}
			}

			if( abs(ptrextra->PosX-oldx) >= SIZE_BRICK_XZ )
			{
				if( ptrextra->PosX>oldx )
				{
					ptrextra->PosX = oldx + (SIZE_BRICK_XZ-1) ;
				}
				else
				{
					ptrextra->PosX = oldx - (SIZE_BRICK_XZ-1) ;
				}
			}

			if( abs(ptrextra->PosZ-oldz) >= SIZE_BRICK_XZ )
			{
				if( ptrextra->PosZ>oldz )
				{
					ptrextra->PosZ = oldz + (SIZE_BRICK_XZ-1) ;
				}
				else
				{
					ptrextra->PosZ = oldz - (SIZE_BRICK_XZ-1) ;
				}
			}

			// securite
			if( (ptrextra->PosY < 0)
			OR  (ptrextra->PosX < 0)
			OR  (ptrextra->PosX > SIZE_BRICK_XZ * (SIZE_CUBE_X-1))
			OR  (ptrextra->PosZ < 0)
			OR  (ptrextra->PosZ > SIZE_BRICK_XZ * (SIZE_CUBE_Z-1))
			OR  (type==EXTRA_MAGIC_BALL AND (MagicBallFlags&MAGIC_BALL_RAPPELEE)) )
			{
				switch( type )
				{
					case EXTRA_MAGIC_BALL:
						// init retour balle
						InitBackMagicBall( ptrextra,
								   CoulRetourBalle( ptrextra ),
								   BACK_EXPLO|BACK_SOUND ) ;

						break ;
				}

				if( ptrextra->Flags & EXTRA_TAKABLE )
				{
					ptrextra->Flags &= ~(EXTRA_FLY|EXTRA_STOP_COL|EXTRA_FALLING) ;

					if( ptrextra->PosY < 0 )	ptrextra->PosY = 0 ;

					if( ptrextra->PosX < 0 )	ptrextra->PosX = 0 ;
					else if( ptrextra->PosX > SIZE_BRICK_XZ * (SIZE_CUBE_X-1) )
						ptrextra->PosX = SIZE_BRICK_XZ * (SIZE_CUBE_X-1) ;

					if( ptrextra->PosZ < 0 )	ptrextra->PosZ = 0 ;
					else if( ptrextra->PosZ > SIZE_BRICK_XZ * (SIZE_CUBE_Z-1) )
						ptrextra->PosZ = SIZE_BRICK_XZ * (SIZE_CUBE_Z-1) ;

					// faire un reajustement au sol ?
//					continue ;
				}
				else
				{
					if( !(ptrextra->Sprite&32768)
					OR   (ptrextra->Sprite&32767)!=0
					OR   (ptrextra->PosY<0) )
					{
						ptrextra->Sprite = -1 ;
						continue ;
					}
				}
			}
		}

		//------------------- Sort de foudre ----------------------

		if( ptrextra->Flags & EXTRA_FOUDRE_STEP1 )
		{
			S32	a ;

			if( timer > ptrextra->Timer+1000 )
			{
				ptrextra->Timer = timer - 8*40 + (ptrextra->Info&7) * 40 ;
				ptrextra->Flags = (EXTRA_FOUDRE_STEP2|EXTRA_TRANSPARENT) ;
				continue ;
			}

			a = (timer - ptrextra->Timer)*8 ;

			if( ptrextra->Info >= 8 )	a += 2048 ;

			Rotate( 0, 600-(timer - ptrextra->Timer)/2, a ) ;

			ptrextra->PosX = ListObjet[ptrextra->Owner].Obj.X + X0 ;
			ptrextra->PosY = ListObjet[ptrextra->Owner].Obj.Y + 1000 + (timer - ptrextra->Timer)  ;
			ptrextra->PosZ = ListObjet[ptrextra->Owner].Obj.Z + Z0 ;
		}

		if( ptrextra->Flags & EXTRA_FOUDRE_STEP2 )
		{
			if( timer > ptrextra->Timer+500 )
			{
				ptrextra->Timer = timer ;
				ptrextra->Flags = (EXTRA_FOUDRE_STEP3|EXTRA_TRANSPARENT) ;

				// Sample Throw
				if( ptrextra->Info == 0 )
				{
					// SAMPLE_FOUDRE_STEP2
					HQ_3D_MixSample( SAMPLE_FOUDRE_STEP2, 0x1000, 500, 1,
							 ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
				}
				continue ;
			}

			ptrextra->PosX = ListObjet[ptrextra->Owner].Obj.X + Rnd( 300 ) - 150 ;
			ptrextra->PosY = ListObjet[ptrextra->Owner].Obj.Y + 2000 ;
			ptrextra->PosZ = ListObjet[ptrextra->Owner].Obj.Z + Rnd( 300 ) - 150 ;
		}

		if( ptrextra->Flags & EXTRA_FOUDRE_STEP3 )
		{
			S32	y ;

			if( timer > ptrextra->Timer+500 )
			{
				ptrextra->Sprite = -1 ;

				if( ptrextra->Info == 0 )
				{
					// tue tout le monde
					InvoqueFoudre( 	ptrextra->Owner,
							ptrextra->PosX,
							ptrextra->PosY,
							ptrextra->PosZ ) ;
				}
				continue ;
			}

			y = 1000 + 1000
				+ (timer - ptrextra->Timer)*15
				+ (ptrextra->Info&7) * 100 ;

			ptrextra->PosX = ListObjet[ptrextra->Owner].Obj.X ;
			ptrextra->PosY = ListObjet[ptrextra->Owner].Obj.Y + y ;
			ptrextra->PosZ = ListObjet[ptrextra->Owner].Obj.Z ;
		}

		//--------------------------------------------------------

		//------------------- Sort de Protection -----------------

		if( ptrextra->Flags & EXTRA_PROTECT )
		{
			S32	a, y ;

			if( ptrextra->Info==0 )
			{
				// regarde si l'on doit consommer de la magie
				MagicPoint -= GetDeltaMove( &ptrextra->U.Move ) ;

				if( MagicPoint<=0 )
				{
					MagicPoint = 0 ;
					ToggleSortProtection() ;
					continue ;
				}

				ListObjet[ptrextra->Owner].Flags |= NO_CHOC ;

				PlayProtectSamples( ptrextra ) ;
			}

			if( MagicPoint<=PROTECT_ALARME_SEUIL )
			{
				ptrextra->Flags |= EXTRA_CLIGNOTE ;
			}
			else
			{
				if( (ptrextra->Info==0)
				AND (ptrextra->Flags&EXTRA_CLIGNOTE)
				AND IsSamplePlaying( SAMPLE_PROTECTION_ALARME ) )
				{
					// on vient de reprendre de la magie
					HQ_StopOneSample( SAMPLE_PROTECTION_ALARME ) ;
				}

				ptrextra->Flags &= ~EXTRA_CLIGNOTE ;
			}

			a = (timer - ptrextra->Timer)*5 ;
			if( ptrextra->Info >= 8 )
			{
				a = -a ;
				y = 400 + (SinTab[((timer - ptrextra->Timer)*13)&4095]>>6) ;
			}
			else
			{
				y = 900 + (SinTab[((timer - ptrextra->Timer)*13)&4095]>>6) ;
			}

			Rotate( 0, 500,	a ) ;

			ptrextra->PosX = ListObjet[ptrextra->Owner].Obj.X + X0 ;
			ptrextra->PosY = ListObjet[ptrextra->Owner].Obj.Y + y  ;
			ptrextra->PosZ = ListObjet[ptrextra->Owner].Obj.Z + Z0 ;
		}

		//--------------------------------------------------------

		if( ptrextra->Flags & EXTRA_SEARCH_OBJ )
		{
			T_OBJET	*ptrobj ;
			S32	ox, oy, oz ;

			/* (S16)Timer: numobj search */
			/* Vz = speed */
			/* PtrBody = last angle beta */

			taken = FALSE ;	// Objet touch

			search = (U8)ptrextra->Timer ;
			owner  = ptrextra->Owner ;

			oldx = ptrextra->PosX ;
			oldy = ptrextra->PosY ;
			oldz = ptrextra->PosZ ;

			ptrobj = &ListObjet[search] ;

			// On cherche le point central de la ZV
			ox = ptrobj->Obj.X+ptrobj->XMin+(ptrobj->XMax-ptrobj->XMin)/2 ;
			oy = ptrobj->Obj.Y+ptrobj->YMin+(ptrobj->YMax-ptrobj->YMin)/2 ;
			oz = ptrobj->Obj.Z+ptrobj->ZMin+(ptrobj->ZMax-ptrobj->ZMin)/2 ;

			beta =	GetAngle2D(	oldx, oldz,
						ox, oz ) ;

			alpha =	GetAngle2D( oldy, 0,
					    oy,
					    Distance2D( oldx, oldz,
							ox, oz ) ) ;

			s = GetDeltaMove( &ptrextra->U.Move ) ;

			Rotate( s,0, alpha ) ;
			ptrextra->PosY -= Z0 ;
			Rotate( 0, X0, beta ) ;
			ptrextra->PosX += X0 ;
			ptrextra->PosZ += Z0 ;

			if( ptrobj->LifePoint<=0// Au cas ou le mec meurre entre temps !
			OR  ExtraSearchCheckObjCol( ptrextra, oldx, oldy, oldz, owner ) == search )
			{
				taken = TRUE ;
			}

			if( taken )
			{
				switch( type )
				{
					case EXTRA_MAGIC_BALL:
						MagicBall = -1 ;
						break ;

					case EXTRA_CONQUE:
						TabInv[FLAG_CONQUE].PtMagie += ptrextra->Divers ;
						if( TabInv[FLAG_CONQUE].PtMagie>INV_MAX_VALUE )
							TabInv[FLAG_CONQUE].PtMagie = INV_MAX_VALUE ;

						ExtraConque = -1 ;
						break ;
				}
				/* bye bye */
				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_SEARCH_KEY )// QUE magic ball ONLY
		{
			T_EXTRA	*sextra ;
			S32	ox, oy, oz ;
			S32	xmin, ymin, zmin, xmax, ymax, zmax ;

			/* (S16)Timer: numobj search */
			/* Vz = speed */

			oldx = ptrextra->PosX ;
			oldy = ptrextra->PosY ;
			oldz = ptrextra->PosZ ;

			search = (U8)ptrextra->Timer ;

			sextra = &ListExtra[search] ;

			GetExtraZV( sextra, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax ) ;

			// On cherche le point central de la ZV
			ox = sextra->PosX+xmin+(xmax-xmin)/2 ;
			oy = sextra->PosY+ymin+(ymax-ymin)/2 ;
			oz = sextra->PosZ+zmin+(zmax-zmin)/2 ;

			beta =	GetAngle2D(	oldx, oldz,
						ox, oz ) ;

			alpha =	GetAngle2D(	oldy, 0,
						oy,
						Distance2D( oldx, oldz,
							    ox, oz ) ) ;

			s = GetDeltaMove( &ptrextra->U.Move ) ;

			Rotate( s,0, alpha ) ;
			ptrextra->PosY -= Z0 ;
			Rotate( 0, X0, beta ) ;
			ptrextra->PosX += X0 ;
			ptrextra->PosZ += Z0 ;

			if( ExtraCheckExtraCol( ptrextra, oldx, oldy, oldz, (U8)MagicBall ) == search )
			{
				// trouve clef
				HQ_3D_MixSample( SAMPLE_BONUS_TROUVE, 0x1000, 0, 1,
						 oldx, oldy, oldz ) ;

				// incruste pendant 2 sec le bonus trouve
				if( sextra->Divers > 1 )
				{
					PtrProjectPoint( oldx, oldy, oldz ) ;
					InitIncrustDisp( INCRUST_NUM,
							 sextra->Divers,
							 Xp, Yp,
							 158, 0, 2 ) ;
				}

				InitIncrustDisp( INCRUST_SPRITE,
						 SPRITE_CLE, // key
						 12, 30,
						 0, 0, 2 ) ;

				NbLittleKeys += (U8)sextra->Divers  ;
				sextra->Sprite = -1 ;

				// init retour balle
				InitBackMagicBall( ptrextra, SPRITE_CLE, 0 ) ;
				ptrextra->Sprite = -1 ;
				continue ;
			}

			if( sextra->Sprite == -1 ) // clef prise entre temps
			{
				// init retour balle
				InitBackMagicBall( ptrextra,
						   CoulRetourBalle( ptrextra ),
						   0 ) ;

				ptrextra->Sprite = -1 ;
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_END_OBJ )	/* hit force */
		{
			numobj = ExtraCheckObjCol( ptrextra, ptrextra->Owner ) ;

			if( numobj != 255 )
			{
				if( ptrextra->Flags & EXTRA_NEW_IMPACT )
				{
					DoImpact( ptrextra->Impact,
						  oldx, oldy, oldz,
						  ptrextra->Owner ) ;
				}

				switch( type )
				{
					case EXTRA_MAGIC_BALL:
						// sample choc balle sur obj ?
						// init retour balle
						InitBackMagicBall( ptrextra,
								   CoulRetourBalle( ptrextra ),
								   BACK_EXPLO ) ;

						ptrextra->Sprite = -1 ;
						continue ;

					case EXTRA_DART: // Fait tomber la flechette par terre
						if( !(ListObjet[numobj].Flags&INVISIBLE) )
						{
							ptrextra->Vx = 0 ;
							ptrextra->Poids <<= 1 ; // *2
							ptrextra->Vz = 0 ;
							ptrextra->Flags &= ~(EXTRA_END_OBJ) ;
							ptrextra->Timer = TimerRefHR ;

							ptrextra->U.Org.X = ptrextra->PosX ;
							ptrextra->U.Org.Y = ptrextra->PosY ;
							ptrextra->U.Org.Z = ptrextra->PosZ ;
						}
						break ;

					default:
						ptrextra->Sprite = -1 ;
						continue ;
				}
			}
		}

		if( ptrextra->Flags & EXTRA_END_COL )
		{
			flagcol = FALSE ;

			if( (col=PtrWorldColBrickVisible( oldx, oldy, oldz,
						ptrextra->PosX,
						ptrextra->PosY,
						ptrextra->PosZ )) != 0 )
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
			{
				if( ptrextra->Flags & EXTRA_IMPACT )
				{
					if( ptrextra->Flags & EXTRA_NEW_IMPACT )
					{
						DoImpact( ptrextra->Impact,
							  oldx, oldy, oldz,
							  ptrextra->Owner ) ;
					}
					else
					{
						// impact Pof
						InitExtraPof( oldx,oldy,oldz, 1,
							0, 0,	// alpha, beta
							0, 0, POF_COL_SCALE_DEB, POF_COL_SCALE_FIN, POF_COL_DUREE, 0, POF_COL_DUREE ) ;
					}
				}

				switch( type )
				{
					case EXTRA_MAGIC_BALL:
						switch( MagicBallType )
						{
						case 1:
							if( !MagicBallCount-- )
							{
								InitBackMagicBall( ptrextra,
										   CoulRetourBalle( ptrextra ),
										   BACK_SOUND|BACK_EXPLO ) ;

								ExtraLabyrinthe( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
								ptrextra->Sprite = -1 ;
								continue ;
							}
							else
							{
								HQ_3D_MixSample( SAMPLE_CHOC_MAGIC_BALL, 0x1000, 300, 1,
									ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;

								// forcement pass par EXTRA_FLY
								BounceExtra( ptrextra, oldx, oldy, oldz ) ;
								ExtraLabyrinthe( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
								NewMagicBallRebond = 3 ;
							}
							break ;

						default:
							InitBackMagicBall( ptrextra,
									   CoulRetourBalle( ptrextra ),
									   BACK_SOUND|BACK_EXPLO ) ;

							ExtraLabyrinthe( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
							ptrextra->Sprite = -1 ;
							continue ;
						}
						break ;

					case EXTRA_DART:
						if( ModeLabyrinthe
						AND !PtrWorldColBrick( oldx,ptrextra->PosY-1,oldz ) )
						{
							ptrextra->PosX = oldx ;
							ptrextra->PosZ = oldz ;

							ptrextra->Vx = 0 ;
							ptrextra->Poids <<= 1 ; // *2
							ptrextra->Vz = 0 ;
							ptrextra->Flags &= ~(EXTRA_END_OBJ) ;
							ptrextra->Timer = TimerRefHR ;

							ptrextra->U.Org.X = ptrextra->PosX ;
							ptrextra->U.Org.Y = ptrextra->PosY ;
							ptrextra->U.Org.Z = ptrextra->PosZ ;
						}
						else
						{
							ptrextra->Sprite = -1 ;
							s = GetDart() ;

							if( s!=-1 )
							{
								ptrd = &ListDart[s] ;

								ptr = (S16*)(ptrextra->PtrBody+2) ;
								ptr+=2 ;

								// On ne reajuste la flechette que si
								// elle atterit sur le sol, sinon elle
								// serait reajustee contre le mur au
								// pseudo prs
								if( PtrWorldColBrick( oldx,ptrextra->PosY-1,oldz ) )
									PtrReajustPosExtra( ptrextra, col, ptr ) ;

								ptrd->Flags &= ~(DART_TAKEN) ;
								ptrd->NumCube = NumCube ;
								ptrd->PosX = ptrextra->PosX ;
								ptrd->PosY = ptrextra->PosY ;
								ptrd->PosZ = ptrextra->PosZ ;
								ptrd->Beta = ptrextra->Beta ;
								ptrd->Alpha = ptrextra->Scale ;
							}
						}
						continue ;

					default:
						// bye bye
						ptrextra->Sprite = -1 ;
						continue ;
				}
			}
		}

		if( ptrextra->Flags & EXTRA_STOP_COL )
		{
			flagcol = FALSE ;

			if( (col=PtrWorldColBrickVisible( oldx, oldy, oldz,
						ptrextra->PosX,
						ptrextra->PosY,
						ptrextra->PosZ )) != 0 )
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
			{
				// si on cogne le mur, on tente de tomber au sol
				if( !(ptrextra->Flags&EXTRA_FALLING)
				AND PtrWorldColBrickVisible( oldx, oldy+SIZE_BRICK_Y, oldz,
							ptrextra->PosX,
							ptrextra->PosY+SIZE_BRICK_Y,
							ptrextra->PosZ ) != 0 )
				{
					ptrextra->Vx = 0 ;
					ptrextra->Poids <<= 1 ; // *2
					ptrextra->Vz = 0 ;
					ptrextra->Timer = TimerRefHR ;

					// forcement pass par EXTRA_FLY
					ptrextra->U.Org.X = oldx ;
					ptrextra->U.Org.Y = oldy ;
					ptrextra->U.Org.Z = oldz ;

					ptrextra->Flags |= EXTRA_FALLING ;

					// Bruitons la situation avec un petit
					// chboing...
					HQ_3D_MixSample( SAMPLE_BONUS, 6000, 500, 1,
						oldx, oldy, oldz ) ;
				}
				else
				{
					if( ptrextra->Body==-1 )
					{
						ptr = &PtrZvExtraRaw[ ptrextra->Sprite*8 + 2 ] ;
					}
					else
					{
						ptr = (S16*)(ptrextra->PtrBody+2) ;
					}
					ptr+=2 ;

					PtrReajustPosExtra( ptrextra, col, ptr ) ;
					ptrextra->Flags &= ~(EXTRA_FLY|EXTRA_STOP_COL|EXTRA_FALLING) ;
				}
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_TAKABLE )
		{
			if( !(ptrextra->Flags & EXTRA_FLY) )	/* no take while flying */
			{
				S32	numobj ;
				T_OBJET *ptrobj ;

				taken = FALSE ;

				if( Comportement==C_CONQUE
				AND ListObjet[NUM_PERSO].GenAnim==GEN_ANIM_MARCHE
				AND ptrextra->Sprite == SPRITE_COEUR
				AND ExtraConque==-1
				AND TabInv[FLAG_CONQUE].PtMagie<INV_MAX_VALUE
				AND (Distance2D( ptrextra->PosX,ptrextra->PosZ,
					      ListObjet[NUM_PERSO].Obj.X, ListObjet[NUM_PERSO].Obj.Z ) <= (3*SIZE_BRICK_XZ)) )
				{
					S32	angle ;

					angle = GetAngle2D( ListObjet[NUM_PERSO].Obj.X, ListObjet[NUM_PERSO].Obj.Z,
						 ptrextra->PosX, ptrextra->PosZ ) ;

					angle = (ListObjet[NUM_PERSO].Obj.Beta-angle+(256*MUL_ANGLE)) & (MAX_ANGLE-1) ;

					if( angle<=(384*MUL_ANGLE) )
					{
						// init retour coeur
						ExtraConque = ExtraSearch(255,ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ,
								SPRITE_COEUR,
								NUM_PERSO, 8000, 0 ) ;

						if( ExtraConque!=-1 )
						{
							ListExtra[ExtraConque].Flags |= EXTRA_CONQUE ;
							ListExtra[ExtraConque].Divers = ptrextra->Divers ;
						}

						taken = 1 ;
					}
				}
				else
				{
					numobj = ExtraCheckObjCol( ptrextra, 255 ) ;

					if( numobj!=255 )
					{
						ptrobj = &ListObjet[numobj] ;

						if( !(ptrobj->Flags&SPRITE_3D)
						AND ptrobj->LifePoint>0 )// pour ne pas prendre des coeurs !!!
						{
							/* give money/life/magic/key/clover to perso */
							switch( ptrextra->Sprite )
							{
								case SPRITE_KASHES: // Money
									if( numobj==NUM_PERSO )
									{
										NbGoldPieces += ptrextra->Divers ;
										if( NbGoldPieces > 999 )
											NbGoldPieces = 999 ;
									}
									taken = TRUE ;
									break ;

								case SPRITE_ZLITOS: // Money
									if( numobj==NUM_PERSO )
									{
										NbZlitosPieces += ptrextra->Divers ;
										if( NbZlitosPieces > 999 )
											NbZlitosPieces = 999 ;
									}
									taken = TRUE ;
									break ;

								case SPRITE_COEUR: /* Life points */
									if( ptrobj->LifePoint<MAX_LIFE_POINTS )
									{
										ptrobj->LifePoint += ptrextra->Divers*5  ;
										if( ptrobj->LifePoint > MAX_LIFE_POINTS )
										{
											ptrobj->LifePoint = MAX_LIFE_POINTS ;
										}
										taken = TRUE ;
									}
									else
									{
										if( PtrWorldColBrick( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) )
										{
											ptrextra->PosY = ((ptrextra->PosY/SIZE_BRICK_Y)*SIZE_BRICK_Y)+SIZE_BRICK_Y ;
										}

										InitFly( ptrextra, 200*MUL_ANGLE,
											 GetAngle2D( ptrextra->PosX, ptrextra->PosZ,
												   ptrobj->Obj.X, ptrobj->Obj.Z ),
												 40, 16 ) ;

										HQ_3D_MixSample( SAMPLE_BONUS, 4500, 0, 1,
											ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;

										ptrextra->Flags |= (EXTRA_STOP_COL/*|EXTRA_WAIT_SOME_TIME*/) ;
									}
									break ;

								case SPRITE_MAGIE: /* Magic points */
									if( numobj==NUM_PERSO
									AND MagicLevel
									AND MagicPoint<MagicLevel*20 ) // … voir ?
									{
										MagicPoint += ptrextra->Divers*2  ;
										if( MagicPoint > (MagicLevel*20) )
											MagicPoint = (U8)(MagicLevel*20) ;

										taken = TRUE ;
									}
									else
									{
										if( PtrWorldColBrick( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) )
										{
											ptrextra->PosY = ((ptrextra->PosY/SIZE_BRICK_Y)*SIZE_BRICK_Y)+SIZE_BRICK_Y ;
										}

										InitFly( ptrextra, 200*MUL_ANGLE,
											 GetAngle2D( ptrextra->PosX, ptrextra->PosZ,
												   ptrobj->Obj.X, ptrobj->Obj.Z ),
											 40, 16 ) ;

										HQ_3D_MixSample( SAMPLE_BONUS, 4500, 0, 1,
											ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;

										ptrextra->Flags |= (EXTRA_STOP_COL/*|EXTRA_WAIT_SOME_TIME*/) ;
									}
									break ;

								case SPRITE_CLE: /* Little Key */
									if( numobj==NUM_PERSO )
									{
										NbLittleKeys += (U8)ptrextra->Divers  ;
										taken = TRUE ;
									}
									break ;

								case SPRITE_CLOVER: /* four leaf clover */
									if( numobj==NUM_PERSO )
									{
										S32 sup ;	// supplement

										sup = (ListVarGame[FLAG_CLOVER]+ptrextra->Divers)-NbCloverBox ;

										ListVarGame[FLAG_CLOVER] += ptrextra->Divers  ;

										if( ListVarGame[FLAG_CLOVER] > NbCloverBox )
										{
											ListVarGame[FLAG_CLOVER] = NbCloverBox ;

										}

										// on en recupere plus que de boite ?
										if( sup>0 )
										{
											// ouuuuiii !!! alors on rempli les pdv et la magie
											if( ListObjet[NUM_PERSO].LifePoint < MAX_LIFE_POINTS
											OR  MagicPoint < (MagicLevel*20) )
											{
												ListObjet[NUM_PERSO].LifePoint = MAX_LIFE_POINTS ;
												MagicPoint = (U8)(MagicLevel*20) ;
												sup-- ;
											}
										}

										// il reste un petit trefle pour la conque ?
										if( sup>0
										AND ListVarGame[FLAG_CONQUE] )
										{
											TabInv[FLAG_CONQUE].PtMagie = INV_MAX_VALUE ;
										}

										taken = TRUE ;
									}
									break ;
							}
						}
					}
				}

				if( taken )
				{
					HQ_3D_MixSample( SAMPLE_BONUS_TROUVE, 0x1000, 0, 1,
						ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;

					// incruste pendant 2 sec le bonus trouve
					if( ptrextra->Divers > 1 )
					{
						PtrProjectPoint( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
						InitIncrustDisp( INCRUST_NUM,
								 ptrextra->Divers,
								 Xp, Yp,
								 158, 0, 2 ) ;
					}

					if( numobj==NUM_PERSO )
					{
						InitIncrustDisp( INCRUST_SPRITE|INCRUST_YCLIP,
								 ptrextra->Sprite,
								 12, 30,
								 0, 0, 2 ) ;
					}

					/* bye bye */
					ptrextra->Sprite = -1 ;
					continue ;
				}
			}
		}
	}

	// Ok, boucle des extras finie, donc si le retour de la balle magique
	// a t initialise dans cette boucle, elle sera traite … la boucle
	// suivante

	InitMagicBall = FALSE ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
