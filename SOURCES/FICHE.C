#include	"c_extern.h"
#include	<dos.h>

#ifdef	COMPILATOR
extern	UBYTE	*PtrCompil ;
extern	WORD	FlagCompilator ;
#endif

/*══════════════════════════════════════════════════════════════════════════*
		      ██▀▀▀ █▀▀▀▀ █▀▀▀█  █    █▀▀▀█ ▀▀█▀▀
		      ▀▀▀▀█ ██    ██▀█▀  ██   ██▀▀▀   ██
		      ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀  ▀▀   ▀▀      ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

WORD	SearchBonusKey() ;

UBYTE	*SearchPtrAnimAction ;

WORD	M_Xmin = -32000 ;
WORD	M_Xmax, M_Ymin, M_Ymax, M_Zmin, M_Zmax ;

UBYTE	MagicHitForce[] = { 2, 3, 4, 6, 8 } ;

#define	BASE_STEP_SOUND	126

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ThrowMagicBall(	WORD x, WORD y, WORD z,
			WORD alpha, WORD beta,
			WORD vitesse, UBYTE poids )
{
	UBYTE	sprite ;
	UBYTE	force ;
	WORD	search ;

	/* ici on determine le type de tir et la force */

	switch( MagicLevel )
	{
	case 0:
	case 1:
		sprite = 1 ;
		force = 4 ;
		break ;

	case 2:
		sprite = 42 ;
		force = 6 ;
		break ;

	case 3:
		sprite = 43 ;
		force = 8 ;
		break ;

	case 4:
		sprite = 13 ;
		force = 10 ;
		break ;
	}

	MagicBallType = ((MagicPoint-1) / 20) + 1 ;
	if( MagicPoint == 0 )	MagicBallType  = 0 ;

	// si clef trouvée
	if( (search = SearchBonusKey()) != -1 )
	{
		MagicBallType = 5 ;
	}

	switch( MagicBallType )
	{
	case 0:	// balle normale directe
		MagicBall =
		ThrowExtra(	NUM_PERSO, x,y,z,
				sprite,
				alpha, beta,
				vitesse, poids,
				force ) ;
		break ;

	case 1: // balle qui rebondit
		MagicBallCount = 4 ;
		MagicBall =
		ThrowExtra(	NUM_PERSO, x,y,z,
				sprite,
				alpha, beta,
				vitesse, poids,
				force ) ;
		break ;

	case 2: // balle qui rebondit
		MagicBallType = 1 ;
		MagicBallCount = 4 ;
		MagicBall =
		ThrowExtra(	NUM_PERSO, x,y,z,
				sprite,
				alpha, beta,
				vitesse, poids,
				force ) ;
		break ;

	case 3: // balle qui rebondit
		MagicBallType = 1 ;
		MagicBallCount = 4 ;
		MagicBall =
		ThrowExtra(	NUM_PERSO, x,y,z,
				sprite,
				alpha, beta,
				vitesse, poids,
				force ) ;
		break ;

	case 4: // balle qui rebondit
		MagicBallType = 1 ;
		MagicBallCount = 4 ;
		MagicBall =
		ThrowExtra(	NUM_PERSO, x,y,z,
				sprite,
				alpha, beta,
				vitesse, poids,
				force ) ;
		break ;

	case 5:	// cherche Key
		MagicBall = ExtraSearchKey(	NUM_PERSO, x,y,z,
						sprite, search ) ;
		break ;
	}

	// la balle magique coute 1 point de magie
	if( MagicPoint > 0 )	MagicPoint-- ;
}

/*══════════════════════════════════════════════════════════════════════════*/

/*----------------------------------------------------------------------*/
/* recherche des options d'anims */

#define	GET_BYTE	(*ptrc++)
#define	GET_WORD	(*(WORD*)ptrc) ; ptrc+=2

void	GereAnimAction( T_OBJET *ptrobj, WORD numobj )
{
	WORD	sample ;
	WORD	decal ;
	WORD	anim ;
	WORD	point, x,y,z ;
	WORD	sprite ;
	WORD	alpha ;
	WORD	beta ;
	WORD	vitesse ;
	WORD	poids ;
	WORD	force ;
	WORD	repeat ;
	WORD	search ;
	WORD	dist ;
	WORD	balltype ;
	UBYTE	*ptrc ;
	WORD	n, nb ;

	anim = ptrobj->Anim ;

	ptrc = ptrobj->PtrAnimAction ;

	nb = *ptrc++ ;
	for( n=0; n<nb; n++ )
	{
		switch( *ptrc++ )
		{
		case 5:		/* hit */
			if( ptrobj->Frame == *ptrc++ - 1 )
			{
				ptrobj->HitForce = GET_BYTE ;
				ptrobj->WorkFlags |= OK_HIT ;
			}
			else	ptrc++ ;
			break ;

		case 6:		/* sample */

//		Message( "Sample Action", FALSE ) ;

			if( ptrobj->Frame == *ptrc++ )
			{
				HQ_3D_MixSample( *(WORD*)ptrc, 0x1000, 1,
					ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
			}
			ptrc += 2 ;
			break ;

		case 7:		/* sample_rnd */
			if( ptrobj->Frame == *ptrc++ )
			{
				sample = GET_WORD ;
				decal = GET_WORD ;
				HQ_3D_MixSample( sample, 0x1000+Rnd(decal)-(decal/2), 1,
					ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
			}
			else	ptrc += 4 ;
			break ;

		case 8:		/* throw */



			if( ptrobj->Frame == *ptrc++ )
			{
				point = GET_WORD ;
				sprite = GET_BYTE ;
				alpha = GET_WORD ;
				beta = ptrobj->Beta + GET_WORD ;
				vitesse = GET_WORD ;
				poids = GET_BYTE ;
				force = GET_BYTE ;

				ThrowExtra( 	numobj,
						ptrobj->PosObjX,ptrobj->PosObjY+point,ptrobj->PosObjZ,
						sprite,
						alpha, beta, vitesse, poids,
						force ) ;
			}
			else	ptrc += 11 ;
			break ;

		case 9:		/* throw_magic */
			if( MagicBall == -1 )	// pas de balle en cours
			{

			if( ptrobj->Frame == *ptrc++ )	// frame ok ?
			{
				point = GET_WORD ;
				alpha = GET_WORD ;
				vitesse = GET_WORD ;
				poids = GET_BYTE ;

				ThrowMagicBall(	ptrobj->PosObjX,
						ptrobj->PosObjY + point,
						ptrobj->PosObjZ,
						alpha,
						ptrobj->Beta,
						vitesse,
						poids ) ;
			}
			else	ptrc += 7 ;

			}
			else	ptrc += 8 ;

			break ;

		case 10:	/* sample_repeat */
			if( ptrobj->Frame == *ptrc++ )
			{
				sample = GET_WORD ;
				repeat = GET_WORD ;
				HQ_3D_MixSample( sample, 0x1000, repeat,
					ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
			}
			else	ptrc += 4 ;
			break ;

		case 11:		/* throw_search */
			if( ptrobj->Frame == *ptrc++ )
			{
				point = GET_WORD ;
				sprite = GET_BYTE ;
				search = GET_BYTE ;
				vitesse = GET_WORD ;
				force = GET_BYTE ;

				ExtraSearch( 	numobj, ptrobj->PosObjX,ptrobj->PosObjY+point,ptrobj->PosObjZ,
						sprite,
						search, vitesse, force ) ;
			}
			else	ptrc += 6 ;
			break ;

		case 12:		/* throw_alpha */
			if( ptrobj->Frame == *ptrc++ )
			{
				dist = Distance2D(ptrobj->PosObjX,
						ptrobj->PosObjZ,
						ListObjet[NUM_PERSO].PosObjX,
						ListObjet[NUM_PERSO].PosObjZ ) ;

				alpha =	GetAngle(ptrobj->PosObjY,0,
						ListObjet[NUM_PERSO].PosObjY,
						dist  ) ;

				point = GET_WORD ;
				sprite = GET_BYTE ;
				alpha += GET_WORD ;
				beta = ptrobj->Beta + GET_WORD ;
				vitesse = GET_WORD ;
				poids = GET_BYTE ;
				force = GET_BYTE ;

				ThrowExtra( 	numobj,
						ptrobj->PosObjX,ptrobj->PosObjY+point,ptrobj->PosObjZ,
						sprite,
						alpha, beta, vitesse, poids,
						force ) ;
			}
			else	ptrc += 11 ;
			break ;

		case 13:		/* sample_stop */

			if( ptrobj->Frame == *ptrc++ )
			{
				sample = GET_WORD ;
				HQ_StopOneSample( sample ) 	;
			}
			else	ptrc += 2 ;
			break ;

		case 15:	// left_step
			if( ptrobj->Frame == *ptrc++ )	/* frame */
			{
/*				sample = WorldCodeBrick( ptrobj->PosObjX,
							 ptrobj->PosObjY-1,
							 ptrobj->PosObjZ ) ;
*/
				sample = ptrobj->CodeJeu ;

				if( sample != 0xF0 )	// rien
				{

					if( (sample & 0xF0) == 0xF0 ) // code jeu
					{
					}
					else
					{
						HQ_3D_MixSample( (sample>>4) + BASE_STEP_SOUND,
								0x1000+Rnd(1000)-500,
								1,
								ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
					}
				}
			}
			break ;

		case 16:	// right_step
			if( ptrobj->Frame == *ptrc++ )
			{
/*				sample = WorldCodeBrick( ptrobj->PosObjX,
							 ptrobj->PosObjY-1,
							 ptrobj->PosObjZ ) ;
*/
				sample = ptrobj->CodeJeu ;

				if( sample != 0xF0 )	// rien
				{
					if( (sample & 0xF0) == 0xF0 ) // code jeu
					{
					}
					else
					{
						HQ_3D_MixSample(
							(sample&0x0F) + BASE_STEP_SOUND + 15,
							0x1000+Rnd(1000)-500,
							1,
							ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
					}
				}
			}
			break ;

		case 17:		/* hit_twinkel */
			if( ptrobj->Frame == *ptrc++ - 1 )
			{
				ptrobj->HitForce = MagicHitForce[ MagicLevel ] ;
				ptrobj->WorkFlags |= OK_HIT ;

			}
			break ;

		case 18:		/* throw_3D */
			if( ptrobj->Frame == *ptrc++ )
			{
				x = GET_WORD ;
				y = GET_WORD ;
				z = GET_WORD ;

				Rotate( x,z, ptrobj->Beta ) ;

				x = X0 + ptrobj->PosObjX ;
				y += ptrobj->PosObjY ;
				z = Y0 + ptrobj->PosObjZ ;

				sprite = GET_BYTE ;
				alpha = GET_WORD ;
				beta = ptrobj->Beta + GET_WORD ;
				vitesse = GET_WORD ;
				poids = GET_BYTE ;
				force = GET_BYTE ;

				ThrowExtra( 	numobj,
						x,y,z,
						sprite,
						alpha, beta, vitesse, poids,
						force ) ;
			}
			else	ptrc += 15 ;
			break ;

		case 19:		/* throw_3D_alpha */
			if( ptrobj->Frame == *ptrc++ )
			{
				dist = Distance2D(ptrobj->PosObjX,
						ptrobj->PosObjZ,
						ListObjet[NUM_PERSO].PosObjX,
						ListObjet[NUM_PERSO].PosObjZ ) ;

				alpha =	GetAngle(ptrobj->PosObjY,0,
						ListObjet[NUM_PERSO].PosObjY,
						dist  ) ;

				x = GET_WORD ;
				y = GET_WORD ;
				z = GET_WORD ;

				Rotate( x,z, ptrobj->Beta ) ;

				x = X0 + ptrobj->PosObjX ;
				y += ptrobj->PosObjY ;
				z = Y0 + ptrobj->PosObjZ ;

				sprite = GET_BYTE ;
				alpha += GET_WORD ;
				beta = ptrobj->Beta + GET_WORD ;
				vitesse = GET_WORD ;
				poids = GET_BYTE ;
				force = GET_BYTE ;

				ThrowExtra( 	numobj,
						x,y,z,
						sprite,
						alpha, beta, vitesse, poids,
						force ) ;
			}
			else	ptrc += 15 ;
			break ;

		case 20:		/* throw_3D_search */
			if( ptrobj->Frame == *ptrc++ )
			{
				x = GET_WORD ;
				y = GET_WORD ;
				z = GET_WORD ;

				Rotate( x,z, ptrobj->Beta ) ;

				x = X0 + ptrobj->PosObjX ;
				y += ptrobj->PosObjY ;
				z = Y0 + ptrobj->PosObjZ ;

				sprite = GET_BYTE ;
				search = GET_BYTE ;
				vitesse = GET_WORD ;
				force = GET_BYTE ;

				ExtraSearch( 	numobj,
						x,y,z,
						sprite,
						search, vitesse, force ) ;
			}
			else	ptrc += 11 ;
			break ;

		case 21:		/* throw_3D_magic */
			if( MagicBall == -1 )	// pas de balle en cours
			{

			if( ptrobj->Frame == *ptrc++ )
			{
				x = GET_WORD ;
				y = GET_WORD ;
				z = GET_WORD ;

				Rotate( x,z, ptrobj->Beta ) ;

				x = X0 + ptrobj->PosObjX ;
				y += ptrobj->PosObjY ;
				z = Y0 + ptrobj->PosObjZ ;

				alpha = GET_WORD ;
				vitesse = GET_WORD ;
				poids = GET_BYTE ;

				ThrowMagicBall(	x,y,z,
						alpha,
						ptrobj->Beta,
						vitesse,
						poids ) ;
			}
			else	ptrc += 11 ;

			}
			else	ptrc += 12 ;

			break ;

		default:
			return ;
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

WORD	SearchAnim( UBYTE numanim, WORD numobj )
{
	UBYTE	command ;
	T_OBJET	*ptrobj ;
	WORD	hqranim ;
	UBYTE	*ptrc	;
	UBYTE	numgene ;

	ptrobj = &ListObjet[numobj] ;
	ptrc = ptrobj->PtrFile3D ;

	SearchPtrAnimAction = 0 ;

	while( (command = *ptrc++) != 255 )
	{
		switch( command )
		{
		case 3:	/* ANIM */
			numgene = *ptrc++ ; 	// lit num name generique

			if( numanim == numgene )	// ok trouve
			{
				ptrc++ ; // saute size anim

				hqranim = *(WORD*)ptrc ;

/*if( numobj==2)
{
CoulText( 15, 0 ) ;
Text( 10,110, "%FSearchAnim GenAnim:%d HqrAnim:%d  ", numanim, hqranim ) ;
}*/


				ptrc += 2 ;

				if( *ptrc++ ) // anim possede action ?
				{
					SearchPtrAnimAction = ptrc-1 ;
				}

				return hqranim ;
			}
			else
			{
				ptrc += *ptrc ;	// next anim
			}
			break ;
		default:
			ptrc++ ;	// gen
			ptrc += *ptrc ;
		}
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/

WORD	SearchBody( UBYTE numbody, WORD numobj )
{
	UBYTE	command ;
	T_OBJET	*ptrobj ;
	UWORD	hqrbody ;
	UBYTE	*ptrc	;
	UBYTE	numgene ;

	ptrobj = &ListObjet[numobj] ;
	ptrc = ptrobj->PtrFile3D ;

/*	if( (numobj == NUM_PERSO)
	AND (numbody == GEN_BODY_NORMAL)
	AND (Weapon == 1) )
	{
		numbody = GEN_BODY_SABRE ;
	}
*/
	while( (command = *ptrc++) != 255 )
	{
		switch( command )
		{
		case 1:	/* BODY */
			numgene = *ptrc++ ; 	// lit num name generique

			if( numbody == numgene )	// ok trouve
			{
				ptrc++ ; // saute size body

				hqrbody = *(UWORD*)ptrc ;

				if( hqrbody & (1<<15) )	// already loaded
				{
					hqrbody &= ~(1<<15) ;
//CoulText( 15, 0 ) ;
//Text( 10,110, "%FSearchBody déjà chargé %d", hqrbody ) ;
				}
				else
				{

//Text( 10,120, "%FSearchBody new %d", hqrbody ) ;
		HQRM_Load( PATH_RESSOURCE"body.hqr", hqrbody, &PtrBody[NbBodys] ) ;
		CHECK_MEMORY

/*					PtrBody[NbBodys]
						= LoadMalloc_HQR(
							PATH_RESSOURCE"body.hqr",
							hqrbody ) ;
*/
		if( !PtrBody[NbBodys] )	TheEnd( NOT_ENOUGH_MEM, "Body.HQR in HQ_Mem" ) ;

					PatchObjet( PtrBody[NbBodys] ) ;
					hqrbody = NbBodys ;
					NbBodys++ ;
//Text( 10,130, "%FNbBodys %d", NbBodys ) ;

					*(UWORD*)ptrc = hqrbody + (1<<15) ;
				}
				ptrc += 2 ;

				M_Xmin = -32000 ;

				if( *ptrc++  ) // body possede action ?
				{
					if( *ptrc++ == 14 ) // ZV
					{
						M_Xmin = *(WORD*)ptrc ;
						ptrc += 2 ;
						M_Ymin = *(WORD*)ptrc ;
						ptrc += 2 ;
						M_Zmin = *(WORD*)ptrc ;
						ptrc += 2 ;
						M_Xmax = *(WORD*)ptrc ;
						ptrc += 2 ;
						M_Ymax = *(WORD*)ptrc ;
						ptrc += 2 ;
						M_Zmax = *(WORD*)ptrc ;
						ptrc += 2 ;
					}
				}

				return hqrbody ;
			}
			else
			{
				ptrc += *ptrc ;	// next body
			}
			break ;

		default:
			ptrc++ ;	// gen
			ptrc += *ptrc ;
		}
	}
	return -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

/*
void	LoadFic( UBYTE indexfileobjet, WORD numobj )
{
	T_OBJET	*ptrobj ;
	LONG	size ;

	ptrobj = &ListObjet[numobj] ;

	ptrobj->PtrFile3D = LoadMalloc_HQR(
				PATH_RESSOURCE"FILE3D.HQR",
				indexfileobjet ) ;
}
*/

/*══════════════════════════════════════════════════════════════════════════*/

