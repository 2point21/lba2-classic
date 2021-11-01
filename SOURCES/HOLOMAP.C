#include 	"c_extern.h"

#define	STEP_ANGLE	32
#define	SLIG  		(((1024/STEP_ANGLE)+1) * 4)
#define	SIZE_TEXT_GLOBE	(((1024/STEP_ANGLE)+1) * ((512/STEP_ANGLE)+1) * 4 * 2)
#define	SIZE_COOR_GLOBE	(((1024/STEP_ANGLE)+1) * ((512/STEP_ANGLE)+1) * 3 * 2)
#define	SIZE_MAP_ALT	( (1024/STEP_ANGLE) * ((512/STEP_ANGLE)+1) )


#define	X_CENTRE_BIG_HOLO	320
#define	Y_CENTRE_BIG_HOLO	190
#define	LARGEUR_BIG_HOLO	300
#define	HAUTEUR_BIG_HOLO	280
#define	ZOOM_BIG_HOLO		9500
#define	X_CENTRE_TRAJ_HOLO	320+80
#define	Y_CENTRE_TRAJ_HOLO	240

extern	WORD		NbPolyPoints ;
extern	T_REAL_VALUE	RealRot ;
extern	LONG		LYmin, LYmax ;

UBYTE		*PtrHoloBody ;
UBYTE		*PtrHoloFleche ;
UBYTE		*PtrHoloBodyFleche ;
UBYTE		*PtrCone ;
UBYTE		*LastPtr ;

WORD	CamAlpha, CamBeta, CamGamma ;


/*══════════════════════════════════════════════════════════════════════════*
		   █   █ █▀▀▀█ █     █▀▀▀█ █▄ ▄█ █▀▀▀█ █▀▀▀█
		   ██▀▀█ ██  █ ██    ██  █ ██▀ █ ██▀▀█ ██▀▀▀
		   ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

// calcule coor mapping. 256*256 -> polys

	/* ptrglobe 	WORD	Xp
			WORD	Yp
			WORD	Xt
			WORD	Yt	*/
UBYTE	*ListTraj ;
UWORD	*PtrGlobe ;
UWORD	*PtrCoorGlobe ;
UBYTE	*PtrAlt ;
UBYTE	*PtrMap ;
UWORD	*PtrTri ;
UBYTE	RotPal[(32+31)*3] ;
WORD	RotPalPos = 0 ;

void	ComputeCoorMapping()
{
	WORD	alpha, beta ;
	UWORD	*ptr ;

	ptr = PtrGlobe ;

	for( alpha = -256; alpha <= 256; alpha += STEP_ANGLE )
	{
		for( beta = 0; beta < 1024; beta += STEP_ANGLE )
		{
			ptr += 2 ;	/* Xp Yp */

			*ptr++ = (UWORD)RegleTrois32( 0,255*256+255, 1023, beta ) ;
			if( alpha == 256 )
				*ptr++ = 256*255 + 255 ;
			else
				*ptr++ = (UWORD) ( ((alpha+256) * 256) / 2) ;
		}

		ptr += 2 ; /* Xp Yp */
		*ptr++ = 255*256 + 255 ;
		if( alpha == 256 )
			*ptr++ = 256*255 + 255 ;
		else
			*ptr++ = (UWORD) ( ((alpha+256) * 256) / 2) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void	ComputeCoorGlobe()
{
	WORD	alpha, beta ;
	UWORD	*ptrc ;
	UBYTE	*mptrv, *ptrv ;
	WORD	x, y ;
	WORD	normal ;

// precalcul coor globe avec coor polaires

	ptrc = PtrCoorGlobe ;

	ptrv =  PtrAlt ;

	SetAngleCamera( 0,0,0 ) ;

	for( alpha = -256; alpha <= 256; alpha += STEP_ANGLE )
	{
		mptrv = ptrv ; // memo 0

		for( beta = 0; beta < 1024; beta += STEP_ANGLE )
		{
			normal = 1000 + *ptrv++ * 2 ;

			Rotate( normal , 0, alpha ) ;
			x = X0 ;
			y = Y0 ;
			Rotate( x, 0, beta ) ;
			WorldRotatePoint( X0, y, Y0 ) ;
			*ptrc++ = X0 ;
			*ptrc++ = Y0 ;
			*ptrc++ = Z0 ;
		}
		Rotate( 1000 + *mptrv * 2 , 0, alpha ) ;
		x = X0 ;
		y = Y0 ;
		Rotate( x, 0, 0 ) ;	//...
		WorldRotatePoint( X0, y, Y0 ) ;

		*ptrc++ = X0 ;
		*ptrc++ = Y0 ;
		*ptrc++ = Z0 ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
// compute projection sphere

int z_sort( WORD *a, WORD *b)
{
	return( *a - *b ) ;
}

void	ComputeGlobeProj()
{
	UWORD	*ptr, *ptrt ;
	WORD	*ptrc ;
	WORD	alpha, beta ;

	ptr = PtrGlobe ;
	ptrc = (UWORD*)PtrCoorGlobe ;
	ptrt = PtrTri ;

	for( alpha = -256; alpha <= 256; alpha += STEP_ANGLE )
	{
		for( beta = 0; beta < 1024; beta += STEP_ANGLE )
		{
			X0 = *ptrc++ ;
			Y0 = *ptrc++ ;
			Z0 = *ptrc++ ;
			WorldRotatePoint( X0, Y0, Z0 ) ;

			if( alpha != 256 )
			{
				*ptrt++ = Z0 ;
				*ptrt++ = ptr-PtrGlobe ;
			}

			ProjettePoint( X0, Y0, Z0 ) ;
			*ptr++ = Xp ;
			*ptr++ = Yp ;
			ptr += 2 ;	// coor text
		}

		X0 = *ptrc++ ;
		Y0 = *ptrc++ ;
		Z0 = *ptrc++ ;
		WorldRotatePoint( X0, Y0, Z0 ) ;
		ProjettePoint( X0, Y0, Z0 ) ;
		*ptr++ = Xp ;
		*ptr++ = Yp ;
		ptr += 2 ;	// coor text
	}


	qsort( PtrTri, 512, 4, z_sort ) ;

}

/*══════════════════════════════════════════════════════════════════════════*/

void	DrawHoloMap(	WORD calpha, WORD cbeta, WORD cgamma )
{
	WORD	alpha, beta, gamma ;
	LONG	offsetalpha ;
	UWORD	*ptr ;
	LONG	n ;

	ComputeGlobeProj() ;

	for( n=0; n<16*32; n++ )
	{
		ptr = PtrGlobe + PtrTri[n*2+1] ;

		TabPoly[1] = ptr[0] ;
		TabPoly[2] = ptr[1] ;
		TabPoly[4] = ptr[0+SLIG] ;
		TabPoly[5] = ptr[1+SLIG] ;
		TabPoly[7] = ptr[4] ;
		TabPoly[8] = ptr[5] ;

		if( TestVuePoly( TabPoly ) )
		{
			TabText[1] = ptr[2] ;
			TabText[2] = ptr[3] ;
			TabText[4] = ptr[2+SLIG] ;
			TabText[5] = ptr[3+SLIG] ;
			TabText[7] = ptr[6] ;
			TabText[8] = ptr[7] ;

			AsmTexturedTriangleNoClip() ;
			FillTextPolyNoClip( LYmin, LYmax, PtrMap ) ;
		}

		TabPoly[1] = ptr[0+SLIG] ;
		TabPoly[2] = ptr[1+SLIG] ;
		TabPoly[4] = ptr[4+SLIG] ;
		TabPoly[5] = ptr[5+SLIG] ;
		TabPoly[7] = ptr[4] ;
		TabPoly[8] = ptr[5] ;

		if( TestVuePoly( TabPoly ) )
		{
			TabText[1] = ptr[2+SLIG] ;
			TabText[2] = ptr[3+SLIG] ;
			TabText[4] = ptr[6+SLIG] ;
			TabText[5] = ptr[7+SLIG] ;
			TabText[7] = ptr[6] ;
			TabText[8] = ptr[7] ;

			AsmTexturedTriangleNoClip() ;
			FillTextPolyNoClip( LYmin, LYmax, PtrMap ) ;
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*
		█   █ █▀▀▀█ █     █▀▀▀█       █▀▀▀█ █▀▀█      █
		██▀▀█ ██  █ ██    ██  █       ██  █ ██▀▀█ ▄▄  █
		▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DrawListPos( WORD calpha, WORD cbeta, WORD cgamma, WORD pos )
{
	T_HOLO_POS	*ptrpos ;
	WORD		n, x,y,z ;
	WORD		x1,y1,z1 ;
	WORD		xm,ym,zm ;
	WORD		xm1,ym1,zm1 ;
	WORD		xp, yp ;
	WORD		normal ;
	UBYTE		*ptr3do ;
	UBYTE		t ;
	LONG		i, nbobjets ;

	ptrpos = ListHoloPos ;
	nbobjets = 0 ;

	for( n=0; n<MAX_HOLO_POS; n++, ptrpos++ )
	{
		// calcul Z vecteur pour savoir devant derriere

	if( !(TabHoloPos[n]&128)
	AND (NumCube != n))		continue ;

		SetAngleCamera(  ptrpos->Alpha, ptrpos->Beta, 0 ) ;
		WorldRotatePoint( 0,0, 1000+ptrpos->Size ) ;
		xm = X0 ;
		ym = Y0 ;
		zm = Z0 ;
		WorldRotatePoint( 0,0, 1500 ) ;
		xm1 = X0 ;
		ym1 = Y0 ;
		zm1 = Z0 ;

//		SetFollowCamera( 0,0,0, calpha,cbeta,cgamma, ZOOM_BIG_HOLO ) ;
		SetInverseAngleCamera( calpha,cbeta,cgamma ) ;
		CameraXr = 0 ;
		CameraYr = 0 ;
		CameraZr = ZOOM_BIG_HOLO ;


		WorldRotatePoint( xm, ym, zm ) ;
		x1 = X0 ;
		y1 = Y0 ;
		z1 = Z0 ;
		WorldRotatePoint( xm1, ym1, zm1 ) ;
		if( !pos )
		{
			if( Z0 > z1 )	continue ;
		}
		else
		{
			if( Z0 < z1 )	continue ;
		}

		// draw obj

		t = TabHoloPos[n] & 1 ;
//t = 1 ;
		if( NumCube == n )	t |= 2 ;

		ListTri[nbobjets].Z = z1 ;
		ListTri[nbobjets].NumObj = n ;
		ListTri[nbobjets].Num = t ;
		ListTri[nbobjets].Xw = xm ;
		ListTri[nbobjets].Yw = ym ;
		ListTri[nbobjets].Zw = zm ;
		nbobjets++ ;
	}

	SmallSort( ListTri, nbobjets, sizeof( T_SORT ) ) ;

	for( n=0; n<nbobjets; n++ )
	{
		i = ListTri[n].NumObj ;
		ptr3do = 0 ;
		switch( ListTri[n].Num )
		{
			case 1:
				ptr3do = PtrHoloFleche ;
				break;

			case 2:
				ptr3do = PtrHoloBody ;
				break ;

			case 3:
				ptr3do = PtrHoloBodyFleche ;
				break ;
		}
		if( ptr3do )
		{
			AffObjetIso(	ListTri[n].Xw, ListTri[n].Yw, ListTri[n].Zw,
					ListHoloPos[i].Alpha,ListHoloPos[i].Beta , 0,
					ptr3do ) ;
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

LONG	SearchNextArrow( LONG num )
{
	LONG		n ;

	for( n=num+1; n<MAX_HOLO_POS; n++ )
	{
		if( TabHoloPos[n] & (128+1) )
		{
			return n ;
		}
	}
	return -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/

LONG	SearchPrevArrow( LONG num )
{
	LONG		n ;

	if( num == -1 )	num = MAX_HOLO_POS ;

	for( n=num-1; n>=0; n-- )
	{
		if( TabHoloPos[n] & (128+1) )
		{
			return n ;
		}
	}
	return -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	SetHoloPos( UBYTE num )
{
//	if( !(TabHoloPos[num] & 64) )	// déjà visité
	{
		TabHoloPos[num] = 1 + 128 ;
	}
}
/*──────────────────────────────────────────────────────────────────────────*/

void	ClrHoloPos( UBYTE num )
{
	TabHoloPos[num] &= ~(1+128) ;
	TabHoloPos[num] |= 64 ;			// cube done
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawTitle( WORD x, WORD y, WORD num )
{
	WORD	x2, x0,y0,x1,y1 ;
	UBYTE	string[256] ;

	x0 = x - 630/2 ;
	x1 = x + 630/2 ;

	y0 = y - 40/2 ;
	y1 = y + 40/2 ;

//	Box( x0, y0, x1, y1, COUL_SELECT_MENU ) ;
//	DrawFire( x0, y0, x1, y1, COUL_SELECT_MENU&0xF0 ) ;
//	CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
//	ShadeBox( x0, y0, x1, y1, 4 ) ;

	// cadre
//	DrawCadre( x0, y0, x1, y1 ) ;

	// text
//	GetMultiText( num, string ) ;
	strcpy( string, "HoloMap" ) ;

	x -= SizeFont( string )/2 ;
	y -= 18 ;

	CoulFont( 12*16+10 ) ;
	Font( x-1, y-1, string ) ;
	Font( x  , y-1, string ) ;
	Font( x+1, y-1, string ) ;

	Font( x-1, y+1, string ) ;
	Font( x  , y+1, string ) ;
	Font( x+1, y+1, string ) ;

	Font( x-1, y , string ) ;
	Font( x+1, y , string ) ;

	CoulFont( 15 ) ;
	Font( x  , y, string ) ;

	// flip
	CopyBlockPhys( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

#define	SIZE_CURSOR	20

void	DrawCurseur()
{
	Rect(	X_CENTRE_BIG_HOLO - SIZE_CURSOR,
		Y_CENTRE_BIG_HOLO - SIZE_CURSOR,
		X_CENTRE_BIG_HOLO + SIZE_CURSOR,
		Y_CENTRE_BIG_HOLO + SIZE_CURSOR,
		15 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

void	InitHoloDatas()
{
	LONG	n ;

	// inits pointeurs

		PtrGlobe = (UWORD*)Screen ;
		PtrCoorGlobe = (UWORD*)(Screen + SIZE_TEXT_GLOBE) ;
		PtrAlt = Screen
			+ SIZE_TEXT_GLOBE
			+ SIZE_COOR_GLOBE ;
		PtrMap = Screen
			+ SIZE_TEXT_GLOBE
			+ SIZE_COOR_GLOBE
			+ SIZE_MAP_ALT ;
		PtrHoloBody = Screen
			+ SIZE_TEXT_GLOBE
			+ SIZE_COOR_GLOBE
			+ SIZE_MAP_ALT
			+ 65536 ;

	// chargements

		Load_HQR( PATH_RESSOURCE"ress.hqr", PtrAlt, RESS_HOLOMAP_HMT ) ;
		Load_HQR( PATH_RESSOURCE"ress.hqr", PtrMap, RESS_HOLOMAP_HMG ) ;

		n = Load_HQR( PATH_RESSOURCE"ress.hqr", PtrHoloBody, RESS_HOLO_TWINKEL ) ;
		PtrHoloFleche = PtrHoloBody + n ;
		n = Load_HQR( PATH_RESSOURCE"ress.hqr", PtrHoloFleche, RESS_HOLO_FLECHE ) ;
		PtrHoloBodyFleche = PtrHoloFleche + n ;
		n = Load_HQR( PATH_RESSOURCE"ress.hqr", PtrHoloBodyFleche, RESS_HOLO_BODYFLECHE ) ;
		PtrCone = PtrHoloBodyFleche + n ;
		n = Load_HQR( PATH_RESSOURCE"ress.hqr", PtrCone, RESS_HOLO_CONE ) ;

		PatchObjet( PtrHoloBody ) ;
		PatchObjet( PtrHoloFleche ) ;
		PatchObjet( PtrHoloBodyFleche ) ;
		PatchObjet( PtrCone ) ;

		PtrTri = (UWORD*)(PtrCone + n) ;

		ListHoloPos = (T_HOLO_POS*)( PtrTri + (17*33*4) ) ;
		n = Load_HQR( PATH_RESSOURCE"ress.hqr", ListHoloPos, RESS_HOLOMAP_POS ) ;
		//Load(	"F:\\PROJET\\LBA\\DATAS\\HOLOMAP.POS",	ListHoloPos ) ;

		ListTraj = (UBYTE*)(ListHoloPos) + n ;
		n = Load_HQR( PATH_RESSOURCE"ress.hqr", ListTraj, RESS_HOLOMAP_TRAJ ) ;
		//Load( "f:\\projet\\lba\\datas\\holotraj.pos", ListTraj ) ;

		LastPtr = (UBYTE*)(ListTraj) + n ;

	// recup couls flotte bank 12 et 13 32 couleurs

		Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_HOLOMAP_PAL ) ;
		for( n=0; n<32; n++ )
		{
			RotPal[n*3 + 0] = PalettePcx[(12*16+n)*3 + 0] ;
			RotPal[n*3 + 1] = PalettePcx[(12*16+n)*3 + 1] ;
			RotPal[n*3 + 2] = PalettePcx[(12*16+n)*3 + 2] ;
		}
		for( n=0; n<31; n++ )
		{
			RotPal[(32+n)*3 + 0] = PalettePcx[(12*16+n)*3 + 0] ;
			RotPal[(32+n)*3 + 1] = PalettePcx[(12*16+n)*3 + 1] ;
			RotPal[(32+n)*3 + 2] = PalettePcx[(12*16+n)*3 + 2] ;
		}

	// precalcul coordonnées diverses (stock dans screen)

		ComputeCoorMapping() ;
		ComputeCoorGlobe() ;
		RotPalPos = 0 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*══════════════════════════════════════════════════════════════════════════*
		   █   █ █▀▀▀█ █     █▀▀▀█ █▄ ▄█ █▀▀▀█ █▀▀▀█
		   ██▀▀█ ██  █ ██    ██  █ ██▀ █ ██▀▀█ ██▀▀▀
		   ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	HoloMap()
{
	WORD	calpha=-256, cbeta=0, cgamma=0 ;
	WORD	zoom = 22000 ;
	LONG	y, x = 15 ;
	UWORD	*ptr, *ptrc ;
	UBYTE	*mptrv, *ptrv ;
	WORD	c, n ;
	WORD	flagredraw ;
	WORD	flagpal = TRUE ;
	WORD	dialstat = 3 ;

	WORD	menumode = 0 ;
	WORD	flagrebond = FALSE ;
	WORD	redrawmenu = TRUE ;

	LONG	automove = FALSE ;
	LONG	otimer ;
	WORD	oalpha, obeta ;
	WORD	dalpha, dbeta ;
	LONG	current = -1 ;
	LONG	dialmess = -1 ;
	LONG	alphalight, betalight ;
	LONG	memoflagspeak ;

	UBYTE	savepalette[768] ;

	SaveTimer() ;

	for(n=0; n<768; n++)	savepalette[n] = PalettePcx[n] ;

	alphalight = AlphaLight ;
	betalight = BetaLight ;

	FadeToBlack( PtrPal ) ;
	HQ_StopSample() ;

	UnSetClip() ;
	Cls() ;
	Flip() ;
	CopyScreen( Log, Screen ) ;

	InitHoloDatas() ;

	DrawTitle( 320, 25, 0 ) ;

	SetProjection(	X_CENTRE_BIG_HOLO,
			Y_CENTRE_BIG_HOLO,
			128,1024,1024 ) ;

// boucle aff
	memoflagspeak = FlagSpeak ;
	FlagSpeak = FALSE ;
	InitDial( 2 ) 		;

	TestCoulDial( 9 ) ;

	dialmess = ListHoloPos[NumCube].Mess ;

//	FlagMessageShade = FALSE ;

	flagredraw = TRUE ;

	otimer = TimerRef ;
	dalpha = ListHoloPos[NumCube].Alpha & 1023 ;
	dbeta  = ListHoloPos[NumCube].Beta & 1023 ;

	calpha = oalpha = dalpha ;
	cbeta = obeta = dbeta ;
	automove = 0 ;

	current = NumCube ;

	while( (Key != K_ESC) AND (Key != K_H) AND !(Fire&F_RETURN) )
	{
	if( flagrebond )
	{
		if( !Joy AND !Fire )	flagrebond = FALSE ;
	}
	else
	{
		MyKey = Key ;
		MyFire = Fire ;
		MyJoy = Joy ;

		if( MyFire & F_CTRL )
		{
			if( (dialstat != 1) AND (!automove) )
			{
				if( MyJoy & J_UP )	calpha-=8 ;
				if( MyJoy & J_DOWN )	calpha+=8 ;
				if( MyJoy & J_LEFT )	cbeta-=8 ;
				if( MyJoy & J_RIGHT )	cbeta+=8 ;
			}

                        calpha &= 1023 ;
			cbeta  &= 1023 ;
		}
		else
		{
			// search & goto next arrow

			if( MyJoy & J_RIGHT )
			{
				current = SearchNextArrow( current ) ;
				if( current == -1 )
				{
					oalpha = calpha ;
					obeta = cbeta ;
					otimer = TimerRef ;
					dalpha = ListHoloPos[NumCube].Alpha & 1023 ;
					dbeta = ListHoloPos[NumCube].Beta & 1023 ;

					dialstat = 3 ;
					dialmess = ListHoloPos[NumCube].Mess ;
				}
				else
				{
					oalpha = calpha ;
					obeta = cbeta ;
					otimer = TimerRef ;
					dalpha = ListHoloPos[current].Alpha & 1023 ;
					dbeta  = ListHoloPos[current].Beta & 1023 ;

					dialstat = 3 ;
					dialmess = ListHoloPos[current].Mess ;
				}
				automove = TRUE ;
				flagrebond = TRUE ;
			}

			if( MyJoy & J_LEFT )
			{
				current = SearchPrevArrow( current ) ;
				if( current == -1 )
				{
					oalpha = calpha ;
					obeta = cbeta ;
					otimer = TimerRef ;
					dalpha = ListHoloPos[NumCube].Alpha & 1023 ;
					dbeta = ListHoloPos[NumCube].Beta & 1023 ;

					dialstat = 3 ;
					dialmess = ListHoloPos[NumCube].Mess ;
				}
				else
				{
					oalpha = calpha ;
					obeta = cbeta ;
					otimer = TimerRef ;
					dalpha = ListHoloPos[current].Alpha & 1023 ;
					dbeta  = ListHoloPos[current].Beta & 1023 ;

					dialstat = 3 ;
					dialmess = ListHoloPos[current].Mess ;
				}
				automove = TRUE ;
				flagrebond = TRUE ;
			}
		}


/*		if( MyJoy & J_UP )	menumode++, flagrebond = TRUE, redrawmenu = TRUE ;
		if( menumode > 3 )	menumode = 0 ;
		if( MyJoy & J_DOWN )	menumode--, flagrebond = TRUE, redrawmenu = TRUE  ;
		if( menumode < 0 )	menumode = 3 ;
*/
	} // flagrebond

	if( automove )
	{
		calpha = BoundRegleTrois( oalpha, dalpha, 75, TimerRef-otimer ) ;
		cbeta  = BoundRegleTrois( obeta,  dbeta,  75, TimerRef-otimer ) ;
		flagredraw = TRUE ;
	}

//	calpha &= 1023 ;
//	cbeta &= 1023 ;

/*
CoulText( 15, 0 ) ;
Text( 0,0, "%Falpha:%d beta:%d ", calpha, cbeta ) ;
Text( 0,10, "%Foalpha:%d obeta:%d ", oalpha, obeta ) ;
Text( 0,20, "%Fdalpha:%d dbeta:%d ", dalpha, dbeta ) ;
Text( 0,30, "%Fcurrent:%d mess:%d ", current, ListHoloPos[current].Mess ) ;
*/

		// cycle palette flotte
		if( !flagpal )
		{
			PalMulti( 12*16, 32, RotPal+(RotPalPos*3) ) ;
			RotPalPos++ ;
			if( RotPalPos == 32 )	RotPalPos = 0 ;
		}

// affiche big holo

		if( dialstat != 1 )	flagredraw = TRUE ;

		if( flagredraw == TRUE )
		{
			flagredraw = FALSE ;


			Box(	X_CENTRE_BIG_HOLO-LARGEUR_BIG_HOLO/2,
				Y_CENTRE_BIG_HOLO-HAUTEUR_BIG_HOLO/2,
				X_CENTRE_BIG_HOLO+LARGEUR_BIG_HOLO/2,
				Y_CENTRE_BIG_HOLO+HAUTEUR_BIG_HOLO/2, 0 ) ;

			SetInverseAngleCamera( calpha,cbeta,cgamma ) ;
			SetLightVector( calpha, cbeta, 0 ) ;

			// aff obj derriere

			DrawListPos( calpha, cbeta, cgamma, 0 ) ;

			// draw sphère
//			SetFollowCamera( 0,0,0, calpha,cbeta,cgamma, ZOOM_BIG_HOLO ) ;
			SetInverseAngleCamera( calpha,cbeta,cgamma ) ;
			CameraXr = 0 ;
			CameraYr = 0 ;
			CameraZr = ZOOM_BIG_HOLO ;

			DrawHoloMap(	calpha, -cbeta, cgamma ) ;

			// aff obj devant
			DrawListPos( calpha, cbeta, cgamma, 1 ) ;

			if( automove )	DrawCurseur() ;

			CopyBlockPhys(	X_CENTRE_BIG_HOLO-LARGEUR_BIG_HOLO/2,
					Y_CENTRE_BIG_HOLO-HAUTEUR_BIG_HOLO/2,
					X_CENTRE_BIG_HOLO+LARGEUR_BIG_HOLO/2,
					Y_CENTRE_BIG_HOLO+HAUTEUR_BIG_HOLO/2 ) ;
		}

		if( automove )
		{
			if( (dalpha == calpha)
			AND (dbeta == cbeta) )
			{
				automove = FALSE ;
			}
		}


// affiche message

		if( dialstat == 3 )
		{
			OpenDial( dialmess ) ;
			dialstat = 0 ;
		}

		if( dialstat != 2 )
		{
			dialstat = NextDialCar() ;
		}

		if( MyFire & F_SPACE )
		{
			if( dialstat == 2 )
			{
				dialstat = 0 ;
			}
			else
			{
				OpenDial( dialmess ) ;
			}
		}

// Text( 0,0, "Xmin: %d Ymin: %d Xmax: %d Ymax: %d Zoom: %d", ScreenXmin,ScreenYmin,ScreenXmax,ScreenYmax,zoom ) ;

// fade in 1ere fois
		if( flagpal )
		{
			flagpal = FALSE ;
			FadeToPal( PalettePcx ) ;
		}
	}

	FlagMessageShade = TRUE ;
	FadeToBlack( PalettePcx ) ;

/*	Cls() ;
	Flip() ;
*/
	AlphaLight = alphalight ;
	BetaLight = betalight ;
	Init3DGame() ;

	FlagSpeak = memoflagspeak ;
	InitDial( START_FILE_ISLAND+Island ) ;

	for(n=0; n<768; n++)	PalettePcx[n] = savepalette[n] ;

	RestoreTimer() ;
}

/*══════════════════════════════════════════════════════════════════════════*
		█   █ █▀▀▀█ █     █▀▀▀█ ▀▀█▀▀ █▀▀▀█ █▀▀▀█     █
		██▀▀█ ██  █ ██    ██  █   ██  ██▀█▀ ██▀▀█ ▄▄  █
		▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀  ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

	// 0 cube source
	// 1 cube dest
	// 2 vehicule
	// 3 alpha
	// 4 beta
	// 5 gamma
	// 6 nbpoints
	// {
	// 	alpha
	//	beta
	// }

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawHoloObj( UBYTE *ptr3do, WORD alpha, WORD beta, WORD size )
{
	LONG	xm, ym, zm ;
	LONG	x1, y1, z1 ;

	SetAngleCamera(  alpha, beta, 0 ) ;
	WorldRotatePoint( 0,0, 1000 + size ) ;
	xm = X0 ;
	ym = Y0 ;
	zm = Z0 ;

	SetFollowCamera( 0,0,0, CamAlpha,CamBeta,CamGamma, 5300 ) ;

	WorldRotatePoint( xm, ym, zm ) ;
	x1 = X0 ;
	y1 = Y0 ;
	z1 = Z0 ;

	UnSetClip() ;
	AffObjetIso(	xm,ym,zm,
			alpha, beta, 0,
			ptr3do ) ;

	CopyBlockPhys( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

WORD	*GiveTrajPtr( LONG numtraj )
{
	WORD	*ptr ;
	LONG	n ;
	WORD	nbp ;

	ptr = (WORD*)ListTraj ;

	for( n=0; n!=numtraj; n++ )
	{
		ptr += 6 ;
		nbp = *ptr++ ;
		ptr += nbp*2 ;
	}
	return ptr ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

void	HoloTraj( WORD numtraj )
{
	WORD	alpha=-256, beta=0, gamma=0 ;
	WORD	zoom = 22000 ;
	LONG	y, x = 15 ;
	UWORD	*ptr, *ptrc ;
	UBYTE	*mptrv, *ptrv ;
	WORD	c, n ;
	WORD	flagredraw ;
	WORD	flagpal = TRUE ;
	WORD	*ptrtraj, nbpoints, vehicule, cubestart, cubeend  ;

	UBYTE	*ptranim, *ptr3do ;
	WORD	frameanim, vbeta ;

	LONG	alphalight, betalight ;
	ULONG	memotimer ;

#ifndef	DEMO

	// inits

	SaveTimer() ;

	alphalight = AlphaLight ;
	betalight = BetaLight ;

	if( FlagPalettePcx )	FadeToBlack( PalettePcx ) ;
	else			FadeToBlack( PtrPal ) ;

	UnSetClip() ;
	Cls() ;
	Flip() ;

	InitHoloDatas() ;

	// recup infos traj

	ptrtraj = GiveTrajPtr( numtraj ) ;

	cubestart = ptrtraj[0] ;
	cubeend   = ptrtraj[1] ;
	vehicule  = ptrtraj[2] ;

	CamAlpha = ptrtraj[3] ;
	CamBeta  = ptrtraj[4] ;
	CamGamma = ptrtraj[5] ;

	nbpoints = ptrtraj[6] ;

	// vehicule

	ptr3do = LastPtr ;
	n = Load_HQR(	PATH_RESSOURCE"ress.hqr",
			ptr3do,
			RESS_HOLO_VEHICULE+vehicule*2 ) ;
	PatchObjet( ptr3do ) ;
	ptranim = ptr3do + n ;
	n = Load_HQR(	PATH_RESSOURCE"ress.hqr",
			ptranim,
			RESS_HOLO_VEHICULE+vehicule*2+1 ) ;
	frameanim = 0 ;
//	vbeta = rand()&1023 ;

	// draw holo

	SetProjection(	X_CENTRE_TRAJ_HOLO,
			Y_CENTRE_TRAJ_HOLO,
			128,1024,1024 ) ;

	SetFollowCamera( 0,0,0, CamAlpha,CamBeta,CamGamma, 5300 ) ;
	DrawHoloMap( CamAlpha, CamBeta, CamGamma ) ;
	Flip() ;

	// premier point

	DrawHoloObj(	PtrCone,
			ListHoloPos[cubestart].Alpha,
			ListHoloPos[cubestart].Beta,
			0 ) ;

	// boucle aff

	n = 0 ;
	memotimer = TimerRef ;
	while( Key != K_ESC )
	{
		// cycle palette flotte
		Vsync() ;
		if( !flagpal )
		{
			PalMulti( 12*16, 32, RotPal+(RotPalPos*3) ) ;
			RotPalPos++ ;
			if( RotPalPos == 32 )	RotPalPos = 0 ;
		}

		// animation vehicule
		vbeta = GetRealAngle( &RealRot ) ;
		if( RealRot.TimeValue == 0 )
		{
			InitRealAngle( vbeta, vbeta-256, 500, &RealRot ) ;
		}

		if( SetInterAnimObjet( frameanim, ptranim, ptr3do ) )
		{
			frameanim++ ;
			if( frameanim == GetNbFramesAnim( ptranim ) )
			{
				frameanim = GetBouclageAnim( ptranim ) ;
			}
		}

		SetProjection(	100, 100+300, 128,900,900 ) ;
		SetFollowCamera( 0,0,0, 60,128,0, 30000 ) ;
		SetLightVector( -60, 128, 0 ) ;
		Box( 0,0+300,199,179+300, 0 ) ;
		AffObjetIso( 0,0,0, 0,vbeta,0, ptr3do ) ;
		CopyBlockPhys( 0,0+300,199,179+300 ) ;

		SetProjection(	X_CENTRE_TRAJ_HOLO,
				Y_CENTRE_TRAJ_HOLO,
				128,1024,1024 ) ;
		SetFollowCamera( 0,0,0, CamAlpha,CamBeta,CamGamma, 5300 ) ;
		SetLightVector( CamAlpha, CamBeta, 0 ) ;

		// points de trajectoire

		if( TimerRef >= memotimer+40 )
		{
			memotimer = TimerRef ;

			if( n < nbpoints )
			{
				alpha = ptrtraj[7+n*2] ;
				beta  = ptrtraj[8+n*2] ;
			}
			else
			{
				if( n > nbpoints )	break ; // c'est fini

				alpha = ListHoloPos[cubeend].Alpha ;
				beta = ListHoloPos[cubeend].Beta ;
			}

			DrawHoloObj(	PtrCone,alpha,beta,0 ) ;
			n++ ;
		}

		// fade intro
		if( flagpal )
		{
			flagpal = FALSE ;
			FadeToPal( PalettePcx ) ;
		}
	}

	FadeToBlack( PalettePcx ) ;
	Cls() ;
	Flip() ;

	AlphaLight = alphalight ;
	BetaLight = betalight ;
	Init3DGame() ;

	RestoreTimer() ;
#endif
}

