#include "defines.h"

extern	void	HPrintf( S32 x, S32 y, char *format, ... ) ;
extern	U8	AnimNumObj ;

extern	void	TestCameraPosObjets( void ) ;// ..\EXTFUNC.CPP

/*══════════════════════════════════════════════════════════════════════════*
	     █▄ ▄█ █▀▀▀█ █▀▀▀█       ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █     ██▀▀▀
	     ██▀ █ ██▀▀█ ██▀▀▀         ██  ██  █ ██  █ ██    ▀▀▀▀█
	     ▀▀  ▀ ▀▀  ▀ ▀▀    ▀▀▀▀▀   ▀▀  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
// tests collision/altitude du terrain

/*══════════════════════════════════════════════════════════════════════════*/
// retourne le Y d'apres une position x(0 à 32767), z(0 à 32767) sur la map

S32	CalculAltitudeObjet( S32 x, S32 z, S32 cj )
{
	T_HALF_POLY *mappoly ;
	S32	y0,y1,y2,y3 ;
	S32	dx,dz;
	S32	dz0, dz1 ;

	dx = x>>9; // div512
	dz = z>>9; // div512

	if( (dx < 0) OR (dx > 63) )	return -1 ;
	if( (dz < 0) OR (dz > 63) )	return -1 ;

	x &= 511;
	z &= 511;

	dz0 = dz*65 ;
	dz1 = dz0+65 ;

//---------------------------------------------------------------
	if( cj==-1 )
	{
		mappoly = &MapPolyGround[dz*64*2+dx*2] ;

		if( mappoly->Sens == 0 )	// poly séparé par ligne reliant point 0 et 2
		{
			if ( x >= z )	// poly de droite
			{
				mappoly++ ;
			}
		}
		else				// poly séparé par ligne reliant point 1 et 3
		{
			if( 511-x <= z ) // poly de droite
			{
				mappoly++ ;
			}
		}


		cj = (U8)mappoly->CodeJeu ;
	}
//--------------------------------------------------------------

	y0 = MapSommetY[dz0+dx] ;
	y1 = MapSommetY[dz1+dx] ;
	y2 = MapSommetY[dz1+(dx+1)] ;
	y3 = MapSommetY[dz0+(dx+1)] ;

	if( cj==CJ_FOOT_WATER
	OR  cj==CJ_WATER )
	{
		U8	*i = &MapIntensity[dz0+dx] 	;

		y0 += (i[0]   >>4)*-200	;
		y1 += (i[65]  >>4)*-200	;
		y2 += (i[65+1]>>4)*-200	;
		y3 += (i[1]   >>4)*-200	;
	}
/*	else
	{
		y0 = MapSommetY[dz0+dx] ;
		y1 = MapSommetY[dz1+dx] ;
		y2 = MapSommetY[dz1+(dx+1)] ;
		y3 = MapSommetY[dz0+(dx+1)] ;
	}
*/
	if( MapPolyGround[dz*64*2+dx*2].Sens == 0 )	// poly séparé par ligne reliant point 0 et 2
	{
		if ( x < z )	// poly de gauche
		{
			LastTriangle = 0 ;
			return( y0 + ( (y1-y0)*z + (y2-y1)*x )/512 );
		}
		else		// poly de droite
		{
			LastTriangle = 1 ;
			return( y0 + ( (y3-y0)*x + (y2-y3)*z )/512 );
		}
	}
	else				// poly séparé par ligne reliant point 1 et 3
	{
		if( 511-x > z )// poly de gauche
		{
			LastTriangle = 0 ;
//			return( y0 + (y3-y0)*x/512 + (y1-y0)*z/512 );
			return( y0 + ( (y3-y0)*x + (y1-y0)*z )/512 );
		}
		else		// poly de droite
		{
			LastTriangle = 1 ;
//			return( y1 + (y2-y1)*x/512 + (y3-y2)*(511-z)/512 );
			return( y1 + ( (y2-y1)*x + (y3-y2)*(511-z) )/512 );
		}
	}
}

//****************************************************************************

// Comme CalculAltitudeObjet() sauf qu'on tient compte de l'avancée de
// l'objet dans l'eau

/*
S32	CalculOffsetYWater( S32 x, S32 z )
{
	S32	y0,y1,y2,y3 ;
	S32	dx,dz;

	dx = x>>9; // div512
	dz = z>>9; // div512

	x &= 511;
	z &= 511;

	y0 = (MapIntensity[dz+0][dx+0]>>4)*200 ;
	y1 = (MapIntensity[dz+1][dx+0]>>4)*200 ;
	y2 = (MapIntensity[dz+1][dx+1]>>4)*200 ;
	y3 = (MapIntensity[dz+0][dx+1]>>4)*200 ;

	if( MapPolyGround[dz][dx][0].Sens == 0 )	// poly séparé par ligne reliant point 0 et 2
	{
		if ( x < z )	// poly de gauche
		{
			return( y0 + ( (y1-y0)*z + (y2-y1)*x )/512 );
		}
		else		// poly de droite
		{
			return( y0 + ( (y3-y0)*x + (y2-y3)*z )/512 );
		}
	}
	else				// poly séparé par ligne reliant point 1 et 3
	{
		if( 511-x > z )// poly de gauche
		{
			return( y0 + (y3-y0)*x/512 + (y1-y0)*z/512 );
		}
		else		// poly de droite
		{
			return( y1 + (y2-y1)*x/512 + (y3-y2)*(511-z)/512 );
		}
	}
}
*/

/*══════════════════════════════════════════════════════════════════════════*/
// permet de determiner sur quel triangle d'une brick se trouve un objet

S32	GiveTriangle( S32 x, S32 z )
{
	S32	dx,dz;

	dx = x>>9; // div512
	dz = z>>9; // div512

	x &= 511 ;
	z &= 511 ;

	// On pourrait eviter le calcul de l'index du poly car deja fait
	// dans GiveTerrainCol... Quoique GiveTriangle() peut etre appelé
	// ailleurs ?
	if( MapPolyGround[dz*64*2+dx*2].Sens == 0 )	// poly séparé par ligne reliant point 0 et 2
	{
		if ( x < z )	return 0 ; // poly de gauche
		else		return 1 ; // poly de droite
	}
	else				// poly séparé par ligne reliant point 1 et 3
	{
		if ( 511-x > z )return 0 ; // poly de gauche
		else		return 1 ; // poly de droite
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
// demande si une coor x,z est sur un poly interdit (collision)
// retourne true ou false

S32	GiveTerrainCol( S32 x, S32 z )
{
	S32	dx,dz;

	dx = x>>9; // div512
	dz = z>>9; // div512

	IndexMapPolyTested = dz*64*2+dx*2 ;	// pour le recuperer dans
						// perso pour glisser sur les
						// diagonales

	return( MapPolyGround[IndexMapPolyTested+GiveTriangle(x,z)].Col ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

U8	GiveTerrainCodeJeu( S32 x, S32 y, S32 z )
{
	S32	dx,dz;
	U8	codejeu ;
	T_HALF_POLY *mappoly ;

	if( CalculAltitudeObjet(x,z,0)<(y-1) )	return 0 ;

	// check ground poly codejeu/sample

	dx = x>>9; // div512
	dz = z>>9; // div512

	if( (dx < 0) OR (dx > 63) )	return 0 ;
	if( (dz < 0) OR (dz > 63) )	return 0 ;

	x &= 511;
	z &= 511;

	mappoly = &MapPolyGround[dz*64*2+dx*2] ;

	if( mappoly->Sens == 0 )	// poly séparé par ligne reliant point 0 et 2
	{
		if ( x >= z )	// poly de droite
		{
			mappoly++ ;
		}
	}
	else				// poly séparé par ligne reliant point 1 et 3
	{
		if( 511-x <= z ) // poly de droite
		{
			mappoly++ ;
		}
	}

	codejeu = (U8)((mappoly->CodeJeu<<4) | mappoly->SampleStep) ;

	return codejeu ;
}

//*══════════════════════════════════════════════════════════════════════════*


// camtargetx -> VueOffsetX
// CamPosX -> CameraX (lib)

S32	SearchCameraPos( S32 x, S32 y, S32 z, S32 objbeta, S32 mode )
{
	S32	n, gy, i ;
	S32	my ;
	S32	nbstep, step ;
	S32	searchbeta ;
	T_DECORS	*ptrdec ;
	S32	modified = FALSE ;

	if( mode == 0 )
	{
		searchbeta = objbeta ;

		BetaCam = (512*4) - searchbeta ;
		BetaCam &= (1024*4)-1;

		AlphaCam = 300 ;
	}
	else
	{
		// touche pas beta bien sur mais alpha ???

		searchbeta = 2048 - BetaCam ;
	}


	for( n=1000; n<VueDistance; n += 512 )
	{
		Rotate( n, 0, AlphaCam ) ;
		my = CamPosY = (y+512) - Z0 ;

		Rotate( 0, -X0, searchbeta ) ;

		CamPosX = x + X0 ;
		CamPosZ = z + Z0 ;

		if( CamPosX<0
		OR  CamPosX>32768
		OR  CamPosZ<0
		OR  CamPosZ>32768 )
		{
			// c crade mais ca évite de faire 1 CalculAltitudeObjet()
			// en dehors du cube !!!! et du coup ce n'est plus la
			// peine de continuer à tester
			break ;
		}

		gy = CalculAltitudeObjet( CamPosX, CamPosZ, 0 ) ;
		if( gy > CamPosY )
		{
			CamPosY = gy + 200 ;
		}

		// teste objets decors
		ptrdec = ListDecors ;

		for ( i=0; i<NbObjDecors; i++, ptrdec++ )
		{
			// pour passer par les tests !
// Pourquoi ??????????,
//			ptrdec->Body &= ~(DEC_INVISIBLE) ;

			if( TestZVDecors( CamPosX, CamPosY, CamPosZ,  ptrdec ) )
			{
				// cam dans decors
				CamPosY = ptrdec->YMax + 200 ;
			}
		}

		// teste objets scenariques
		TestCameraPosObjets() ;

		if( CamPosY != my )
		{
			AlphaCam = 1024-GetAngle2D( 0, 0, Distance2D( x, z, CamPosX, CamPosZ), CamPosY-(y+512) ) ;
			modified = TRUE ;
		}
	}

	if( modified )
	{
		mode = 0 ;// forcé pour test !

		switch( mode )
		{

		case 0: // behind twinsen
//			laisser les comments pour new lib
//			nbstep = P_SinTab[1024] - P_SinTab[300] ;
			nbstep = (1073741824>>16) - (476872522>>16) ;
//			step = P_SinTab[AlphaCam] - P_SinTab[300] ;
			step = SinTab[AlphaCam] - (476872522>>16) ;

			n = RegleTrois( DISTANCE_VISEE, 0, nbstep, step ) ;

			Rotate( 0, n, searchbeta ) ;

			VueOffsetX = x + X0 ;
			VueOffsetY = y ;		// ou calculaltitude
			VueOffsetZ = z + Z0 ;

			break ;

		case 1:	// shift screen

			Rotate( 0,1000 , objbeta ) ;

			VueOffsetX = x + X0 ;
			VueOffsetY = y ;		// ou calculaltitude
			VueOffsetZ = z + Z0 ;

			break ;
		}
	}

	return modified ;
}
