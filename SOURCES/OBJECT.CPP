#include 	"c_extern.h"

extern	S32	FlagAnimWhoSpeak	;	// MESSAGE.CPP

//#########################################################################
// Memorisation de l'anim et de la frame en cours au moment d'un changement
// de cube pour reprendre exactement a la même anim/frame dans le cube de
// destination pour un changement de cube plus smooth

U16	MemoAnim  ;	/* anim en cours */
U8	MemoFlagAnim ;
S32	MemoFrame ;
S32	FlagReinit = FALSE ;
S32	MemoMove ;	// pour le buggy

S32	TestNumObjFollow = TRUE ;

extern	S16 NumDecorsCol ;	// EXTFUNC.CPP

//#########################################################################

S32	Nxw, Nyw, Nzw ;
S32	SaveNxw, SaveNyw, SaveNzw ;
S32	OldX, OldY, OldZ ;
U8	AnimNumObj ;
T_OBJET	*APtObj ;

S32	LastJoyFlag = FALSE ;
S32	Jumping = FALSE ;
S32	Pushing = FALSE ;
S32	LastMyJoy = 0 ;
S32	LastMyFire = 0 ;

S32	Col1 ;

extern	S32	MagicBallFlags ;

S32	BetaUsedObj = 0 ;

S32	PersoFlashTimer = 0 ;

S32	ShadowYHero ;

T_COMPORTEMENT	ListComportement[MAX_COMPORTEMENTS] = {
	{ 0, -1, NULL, CF_STANDARD },		// Normal
	{ 0, -1, NULL, CF_STANDARD },		// Sportif
	{ 0, -1, NULL, CF_STANDARD },		// Agressif
	{ 0, -1, NULL, CF_STANDARD },		// Discret
	{ 1, -1, NULL, CF_COMPORTEMENT		// Protopack
		    | CF_INVENTORY
		    | CF_WALK_ON_CODES_JEU
		    | CF_WEAPON
		    | CF_HOLOMAP
		    | CF_SHIELD },
	{ 2, -1, NULL, CF_HOLOMAP },		// Double
	{ 3, -1, NULL, CF_COMPORTEMENT		// Conque
		    | CF_INVENTORY
		    | CF_WEAPON
		    | CF_HOLOMAP
		    | CF_SHIELD },
	{ 4, -1, NULL,CF_COMPORTEMENT 		// ScaphIntNorm
		    | CF_WEAPON
		    | CF_HOLOMAP },
	{ 1, -1, NULL, CF_COMPORTEMENT
		    | CF_INVENTORY
		    | CF_WALK_ON_CODES_JEU
		    | CF_WEAPON
		    | CF_HOLOMAP
		    | CF_SHIELD },
	{ 4, -1, NULL,CF_COMPORTEMENT		// ScaphIntSpor
		    | CF_WEAPON
		    | CF_HOLOMAP },
	{ 5, -1, NULL,CF_COMPORTEMENT		// ScapExtNorm
		    | CF_WEAPON
		    | CF_HOLOMAP },
	{ 5, -1, NULL,CF_COMPORTEMENT		// ScaphExtSpor
		    | CF_WEAPON
		    | CF_HOLOMAP },
	{ 6, -1, NULL, CF_COMPORTEMENT		// Buggy
		    | CF_HOLOMAP
		    | CF_WEAPON},
	{ 7, -1, NULL, CF_HOLOMAP},		// Squelette
} ;

char	DefSysText[4][20] = {
	"Normal",
	"Sportif",
	"Aggressif",
	"Discret"
	} ;

extern	S32	XMap, YMap, ZMap ;
extern	S16	M_XMin, M_XMax, M_YMin, M_YMax, M_ZMin, M_ZMax ;
extern	U8	*SearchPtrAnimAction ;

/*══════════════════════════════════════════════════════════════════════════*/
#ifdef	DEBUG_TOOLS

#define	HashLine( x0, y0, x1, y1, coul, ep )	Line( x0, y0, x1, y1, coul )

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
S32	DrawCube( 	S32 xw0, S32 yw0, S32 zw0,
			S32 xw1, S32 yw1, S32 zw1,
			S32 type	)

/*
	type zone :

	1 --> zone selectionnee (tracee en gras)
	2 --> zone giver
	4 --> zone echelle
	8 --> trace en pointilles (zones echelles)
      512 --> zone hit
     1024 --> zone message NORD
     2048 --> zone message SUD
     4096 --> zone message EST
     8192 --> zone message OUEST
*/
{
	S32	x0,y0,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7 ;
	S32	coul = 11 ;
	S32	coul1 = 13 ;

	ClearScreenMinMax() ;

	LongProjectPoint( xw0,yw0,zw0 ) ;
	AdjustScreenMinMax() ;
	x0 = Xp ;
	y0 = Yp ;
	LongProjectPoint( xw1,yw0,zw0 ) ;
	AdjustScreenMinMax() ;
	x1 = Xp ;
	y1 = Yp ;
	LongProjectPoint( xw1,yw0,zw1 ) ;
	AdjustScreenMinMax() ;
	x2 = Xp ;
	y2 = Yp ;
	LongProjectPoint( xw0,yw0,zw1 ) ;
	AdjustScreenMinMax() ;
	x3 = Xp ;
	y3 = Yp ;
	LongProjectPoint( xw0,yw1,zw0 ) ;
	AdjustScreenMinMax() ;
	x4 = Xp ;
	y4 = Yp ;
	LongProjectPoint( xw1,yw1,zw0 ) ;
	AdjustScreenMinMax() ;
	x5 = Xp ;
	y5 = Yp ;
	LongProjectPoint( xw1,yw1,zw1 ) ;
	AdjustScreenMinMax() ;
	x6 = Xp ;
	y6 = Yp ;
	LongProjectPoint( xw0,yw1,zw1 ) ;
	AdjustScreenMinMax() ;
	x7 = Xp ;
	y7 = Yp ;

	if( type&16 )
		coul = coul1 = LBAWHITE ;

	ScreenXMax++ ;	// pour afficher la zone selectionnee

	SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
	if( ClipXMin <= ClipXMax
	AND ClipYMin <= ClipYMax )
	{
		if( type&1 )	// selected : on epaissit le trait
		{
			coul  =
			coul1 = LBAWHITE ;

			if( type & 8 )
			{
				HashLine( x0+1,y0, x1+1,y1, (type&1024)?12:coul, 4 ) ;
				HashLine( x1+1,y1, x2+1,y2, (type&4096)?12:coul, 4 ) ;
				HashLine( x2+1,y2, x3+1,y3, (type&2048)?12:coul, 4 ) ;
				HashLine( x3+1,y3, x0+1,y0, (type&8192)?12:coul, 4 ) ;

				HashLine( x0+1,y0, x4+1,y4, (type&1024 OR type&8192)?12:coul1, 4 ) ;
				HashLine( x1+1,y1, x5+1,y5, (type&1024 OR type&4096)?12:coul1, 4 ) ;
				HashLine( x2+1,y2, x6+1,y6, (type&2048 OR type&4096)?12:coul1, 4 ) ;
				HashLine( x3+1,y3, x7+1,y7, (type&2048 OR type&8192)?12:coul1, 4 ) ;

				HashLine( x4+1,y4, x5+1,y5, (type&1024)?12:coul1, 4 ) ;
				HashLine( x5+1,y5, x6+1,y6, (type&4096)?12:coul1, 4 ) ;
				HashLine( x6+1,y6, x7+1,y7, (type&2048)?12:coul1, 4 ) ;
				HashLine( x7+1,y7, x4+1,y4, (type&8192)?12:coul1, 4 ) ;
			}
			else
			{
				Line( x0+1,y0, x1+1,y1, (type&1024)?12:coul ) ;
				Line( x1+1,y1, x2+1,y2, (type&4096)?12:coul ) ;
				Line( x2+1,y2, x3+1,y3, (type&2048)?12:coul ) ;
				Line( x3+1,y3, x0+1,y0, (type&8192)?12:coul ) ;

				Line( x0+1,y0, x4+1,y4, (type&1024 OR type&8192)?12:coul1 ) ;
				Line( x1+1,y1, x5+1,y5, (type&1024 OR type&4096)?12:coul1 ) ;
				Line( x2+1,y2, x6+1,y6, (type&2048 OR type&4096)?12:coul1 ) ;
				Line( x3+1,y3, x7+1,y7, (type&2048 OR type&8192)?12:coul1 ) ;

				Line( x4+1,y4, x5+1,y5, (type&1024)?12:coul1 ) ;
				Line( x5+1,y5, x6+1,y6, (type&4096)?12:coul1 ) ;
				Line( x6+1,y6, x7+1,y7, (type&2048)?12:coul1 ) ;
				Line( x7+1,y7, x4+1,y4, (type&8192)?12:coul1 ) ;
			}
		}

		if( type & 8 )
		{
			HashLine( x0,y0, x1,y1, (type&1024)?12:coul, 4 ) ;
			HashLine( x1,y1, x2,y2, (type&4096)?12:coul, 4 ) ;
			HashLine( x2,y2, x3,y3, (type&2048)?12:coul, 4 ) ;
			HashLine( x3,y3, x0,y0, (type&8192)?12:coul, 4 ) ;

			HashLine( x0,y0, x4,y4, (type&1024 OR type&8192)?12:coul1, 4 ) ;
			HashLine( x1,y1, x5,y5, (type&1024 OR type&4096)?12:coul1, 4 ) ;
			HashLine( x2,y2, x6,y6, (type&2048 OR type&4096)?12:coul1, 4 ) ;
			HashLine( x3,y3, x7,y7, (type&2048 OR type&8192)?12:coul1, 4 ) ;

			HashLine( x4,y4, x5,y5, (type&1024)?12:coul1, 4 ) ;
			HashLine( x5,y5, x6,y6, (type&4096)?12:coul1, 4 ) ;
			HashLine( x6,y6, x7,y7, (type&2048)?12:coul1, 4 ) ;
			HashLine( x7,y7, x4,y4, (type&8192)?12:coul1, 4 ) ;
		}
		else
		{
			Line( x0,y0, x1,y1, (type&1024)?12:coul ) ;
			Line( x1,y1, x2,y2, (type&4096)?12:coul ) ;
			Line( x2,y2, x3,y3, (type&2048)?12:coul ) ;
			Line( x3,y3, x0,y0, (type&8192)?12:coul ) ;

			Line( x0,y0, x4,y4, (type&1024 OR type&8192)?12:coul1 ) ;
			Line( x1,y1, x5,y5, (type&1024 OR type&4096)?12:coul1 ) ;
			Line( x2,y2, x6,y6, (type&2048 OR type&4096)?12:coul1 ) ;
			Line( x3,y3, x7,y7, (type&2048 OR type&8192)?12:coul1 ) ;

			Line( x4,y4, x5,y5, (type&1024)?12:coul1) ;
			Line( x5,y5, x6,y6, (type&4096)?12:coul1) ;
			Line( x6,y6, x7,y7, (type&2048)?12:coul1) ;
			Line( x7,y7, x4,y4, (type&8192)?12:coul1) ;
		}

		if( type & 2 )
		{
			if( type & 8 )
			{
				if( type & 1 ) // selected : on epaissit le trait
				{
					HashLine( x0+1,(y0+y4)/2, x1+1,(y1+y5)/2, coul, 4 ) ;
					HashLine( x1+1,(y1+y5)/2, x2+1,(y2+y6)/2, coul, 4 ) ;
					HashLine( x2+1,(y2+y6)/2, x3+1,(y3+y7)/2, coul, 4 ) ;
					HashLine( x3+1,(y3+y7)/2, x0+1,(y0+y4)/2, coul, 4 ) ;
				}

				HashLine( x0,(y0+y4)/2, x1,(y1+y5)/2, coul, 4 ) ;
				HashLine( x1,(y1+y5)/2, x2,(y2+y6)/2, coul, 4 ) ;
				HashLine( x2,(y2+y6)/2, x3,(y3+y7)/2, coul, 4 ) ;
				HashLine( x3,(y3+y7)/2, x0,(y0+y4)/2, coul, 4 ) ;
			}
			else
			{
				Line( x0,(y0+y4)/2, x1,(y1+y5)/2, coul ) ;
				Line( x1,(y1+y5)/2, x2,(y2+y6)/2, coul ) ;
				Line( x2,(y2+y6)/2, x3,(y3+y7)/2, coul ) ;
				Line( x3,(y3+y7)/2, x0,(y0+y4)/2, coul ) ;
			}
		}

		if( type & 4 )
		{
			Line( (x0+x1)/2, (y0+y1)/2, (x3+x2)/2,(y3+y2)/2, coul ) ;
			Line( (x0+x3)/2, (y0+y3)/2, (x1+x2)/2,(y1+y2)/2, coul ) ;
			Line( (x0+x2)/2, (y0+y2)/2, (x4+x6)/2,(y4+y6)/2, coul ) ;
		}

		if( type & 512 )	// zone Hit
		{
			coul = 14 ;

			if( type&1 )	// selected
			{
				if( type&8 )	// pointilles
				{
					HashLine( x0+1,y0, x6+1,y6, coul, 4 ) ;
					HashLine( x1+1,y1, x7+1,y7, coul, 4 ) ;
					HashLine( x3+1,y3, x5+1,y5, coul, 4 ) ;
					HashLine( x2+1,y2, x4+1,y4, coul, 4 ) ;
				}
				else
				{
					Line( x0+1,y0, x6+1,y6, coul ) ;
					Line( x1+1,y1, x7+1,y7, coul ) ;
					Line( x3+1,y3, x5+1,y5, coul ) ;
					Line( x2+1,y2, x4+1,y4, coul ) ;
				}
			}

			if( type&8 )	// pointilles
			{
				HashLine( x0,y0, x6,y6, coul, 4 ) ;
				HashLine( x1,y1, x7,y7, coul, 4 ) ;
				HashLine( x3,y3, x5,y5, coul, 4 ) ;
				HashLine( x2,y2, x4,y4, coul, 4 ) ;
			}
			else
			{
				Line( x0,y0, x6,y6, coul ) ;
				Line( x1,y1, x7,y7, coul ) ;
				Line( x3,y3, x5,y5, coul ) ;
				Line( x2,y2, x4,y4, coul ) ;
			}
		}

//		if( FlagInfos & INFO_VISU_BOX )
		{
			Rect( ScreenXMin,ScreenYMin,ScreenXMax,ScreenYMax, 11 ) ;
		}

		return 1 ;

	}

	return 0 ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/

void	DrawZonesDec()
{
	S32	n ;
	T_ZONE	*ptrz ;
	S32	xw, yw, zw ;
	S32	type ;

	xw = StartXCube*SIZE_BRICK_XZ ;
	yw = StartYCube*SIZE_BRICK_Y ;
	zw = StartZCube*SIZE_BRICK_XZ ;

	ptrz = ListZone ;
	for( n=0; n<NbZones; n++, ptrz++ )
	{
		switch( ptrz->Type )
		{
			case 4:	// zone giver
				type = 4 ;
				break ;

			case 5:	// zone message
				type = (ptrz->Info2<<10) ;
				break ;

			case 6:	// zone echelle
				type = 2 + (ptrz->Info1==0)*8 ;
				break ;

			case 8:	// zone hit
				type = 512 + (ptrz->Info1==0)*8 ;
				break ;

			default:
				type = 0 ;
		}

		if( DrawCube(ptrz->X0-xw,ptrz->Y0-yw,ptrz->Z0-zw,
				ptrz->X1-xw,ptrz->Y1-yw,ptrz->Z1-zw,
				 type ) )
		{
			CoulText( 11, -1 ) ;
			BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		}
	}

	UnsetClip() ;
}
#endif
/*══════════════════════════════════════════════════════════════════════════*/
void	InitSysText( void )
{
	S32	n ;

	InitDial( 0 ) ;

	// Charge les chaines des 4 premiers comportements (Normal...Discret)
	for( n=0; n<4; n++ )
	{
		GetMultiText( n+START_TEXT_COMPORTEMENT, DefSysText[n] ) ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀█ █▀▀█      █ █▀▀▀▀ █▀▀▀▀ ▀▀█▀▀ ██▀▀▀
		   ██  █ ██▀▀█ ▄▄  █ ██▀▀  ██      ██  ▀▀▀▀█
		   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

// Passer tout ca en DEFINE !!!!!!!!!! Impossible, ce sont des pointeurs
// de fonctions (sauf peut-etre GivePtrObjFix et GivePtrSample

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/
void	*GivePtrAnim(S32 index)
{
	return(HQR_Get( HQR_Anims, index ))	;
}
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/
void	*GivePtrBody(S32 index)
{
	return(HQR_Get( HQR_Bodys, index ) ) ;
}
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

/*══════════════════════════════════════════════════════════════════════════*/

void	InitObject( U8 numobj )
{
	T_OBJET *ptrobj ;

	ptrobj = &ListObjet[numobj] ;

	memset( ptrobj, 0, sizeof(T_OBJET))	;// Optimize Code

	ObjectClear( &(ptrobj->Obj) ) ;		// init structure 3D: TRES
						// IMPORTANT, A NE PAS VIRER !!!!

	ptrobj->GenBody = GEN_BODY_NORMAL ;
	ptrobj->GenAnim = GEN_ANIM_RIEN ;

	ptrobj->Obj.Y = SIZE_BRICK_Y ;

	ptrobj->SRot = 51200/40 ;

	ptrobj->Move = NO_MOVE ;

	ptrobj->ObjCol = -1 ;
	ptrobj->CarryBy = -1 ;

	ptrobj->ZoneSce = -1 ;

	ptrobj->LifePoint = MAX_LIFE_POINTS ;
	ptrobj->Armure = 0 ;
	ptrobj->HitBy = 255 ;

	ptrobj->Obj.Body.Num = -1 ;
	ptrobj->Obj.Anim.Num = -1 ;

	InitBoundAngleMove( &ptrobj->BoundAngle, 0, 0, 0 ) ;

	ptrobj->OffsetTrack = -1 ;

	ptrobj->AnimDial = GEN_ANIM_PARLE ;
	ptrobj->SampleAlways = 0 ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	StartInitObj( U8 numobj )
{
	T_OBJET	*ptrobj ;

	ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Flags & SPRITE_3D )
	{
		if( ptrobj->HitForce != 0 )
			ptrobj->WorkFlags |= OK_HIT ;

		ptrobj->Obj.Body.Num = -1 ;
		InitSprite( ptrobj->Sprite, ptrobj ) ;

		// vitesse de deplacement en mm/s
		InitMove( &ptrobj->BoundAngle.Move, 0 ) ;

		if( ptrobj->Flags & SPRITE_CLIP )
		{
			ptrobj->Obj.LastAnimStepX = ptrobj->Obj.X ;
			ptrobj->Obj.LastAnimStepY = ptrobj->Obj.Y ;
			ptrobj->Obj.LastAnimStepZ = ptrobj->Obj.Z ;
		}

		if( ptrobj->Flags & ANIM_3DS )
		{
			ptrobj->Coord.A3DS.Deb = ptrobj->Sprite ; // Frame Start
			ptrobj->Coord.A3DS.Fin = ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin ;	// Frame End
			ptrobj->SizeSHit = (S16)ptrobj->Info3 ;	// NbFps
		}
	}
	else
	{
		ptrobj->Obj.Body.Num = -1 ;
		InitBody( ptrobj->GenBody, numobj ) ;

		ptrobj->Obj.Anim.Num = -1 ;
		ptrobj->FlagAnim = 0 ;
		InitAnim( ptrobj->GenAnim, ANIM_REPEAT, numobj ) ;

		// vitesse de rotation
		InitBoundAngleMove( &ptrobj->BoundAngle,
				0,
				ptrobj->Obj.Beta,
				ptrobj->Obj.Beta ) ;
	}

/*	if( !(ptrobj->Flags&SPRITE_3D)
	AND ptrobj->Move!=MOVE_WAGON	// cf TM_SPEED
	AND ptrobj->SRot )
	{
		ptrobj->SRot = (S16)(51200 / (S32)ptrobj->SRot) ; // 1024*1000/20
	}
*/
	if( ptrobj->Move==MOVE_CIRCLE
	OR  ptrobj->Move==MOVE_CIRCLE2 )
	{
		S32	x, z ;

		x = ListBrickTrack[ptrobj->Info3].X ;
		z = ListBrickTrack[ptrobj->Info3].Z ;

		ptrobj->Info  = Distance2D( x,z,ptrobj->Obj.X,ptrobj->Obj.Z ) ;	// Distance entre le drapeau
										// et l'objet
		ptrobj->Info1 = GetAngle2D( x,z,ptrobj->Obj.X,ptrobj->Obj.Z ) ;	// Angle d'origine
		ptrobj->Info2 = 0 ;
	}

	if( ptrobj->Move==MOVE_WAGON
	OR  ptrobj->Move==MOVE_BUGGY_MANUAL )
	{
		ptrobj->WorkFlags |= MANUAL_INTER_FRAME ;
	}

/*	if( CubeMode==CUBE_EXTERIEUR )
	{
		ptrobj->Flags |= OBJ_ZBUFFER ;
	}
*/
	ptrobj->OffsetTrack = -1 ;
	ptrobj->LabelTrack = -1 ;
	ptrobj->OffsetLife = 0 ;

	ptrobj->AnimDial = GEN_ANIM_PARLE ;
	ptrobj->SampleAlways = 0 ;
}


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	StartInitAllObjs()
{
	U8	n ;

	// Faut-il le laisser ?
	// Cela sert a virer le compteur du pingouin si on meurt
	// pendant qu'il est actif
	if( ListObjet[NUM_PINGOUIN].Info1!=-1 )	// Num Incrust Diplay
	{
		ListIncrustDisp[ListObjet[NUM_PINGOUIN].Info1].Num = -1 ;
		ListObjet[NUM_PINGOUIN].Info1 = -1 ;
	}

	for( n=1; n<NbObjets; n++ )
	{
		StartInitObj( n ) ;
	}

	RestartMecaPingouin( ) ;

	AnimateTexture = FALSE ;        // Désactivé à chaque changement de cube
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	RestartMecaPingouin()
{
	T_OBJET *ptrobj = &ListObjet[NUM_PINGOUIN] ;

	PingouinActif = FALSE ;

	ptrobj->WorkFlags |= OBJ_DEAD	;
	ptrobj->Obj.Body.Num = -1		;
	ptrobj->ZoneSce = -1		;
	*(U32*)(&ptrobj->Info) = TimerRefHR + 30 * 1000 ;

	// init zv pingouin en dur (je sais, c un peu crade, mais c 1 rustine
	// de fin de projet !!!!!!)
	ptrobj->XMin = -143 ;
	ptrobj->YMin =    0 ;
	ptrobj->ZMin = -143 ;
	ptrobj->XMax =  143 ;
	ptrobj->YMax =  537 ;
	ptrobj->ZMax =  143 ;

	ptrobj->LifePoint = 0 ;
	ptrobj->Flags|=CHECK_CODE_JEU ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/

void	LoadFicPerso()
{
	S32	n ;
	T_COMPORTEMENT *ptrcomp = &ListComportement[MAX_COMPORTEMENTS-1] ;
	T_OBJET *ptrobj = &ListObjet[NUM_PERSO] ;

	// on parcours en decroissant pour que Twinsen se retrouve
	// initialisé en mode Normal

	for( n=MAX_COMPORTEMENTS-1; n>=0; n--, ptrcomp-- )
	{
		ptrobj->PtrFile3D = LoadFile3D( n ) ;

		ptrcomp->PtrFile3d = ptrobj->PtrFile3D ;
		ptrcomp->AnimRien  = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO ) ;
	}

	ptrobj->PtrAnimAction = SearchPtrAnimAction ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	InitPerso()
{
	T_OBJET *ptrobj ;

	InitObject( NUM_PERSO ) ;

	ptrobj = &ListObjet[NUM_PERSO] ;

	ptrobj->GenBody = GEN_BODY_NORMAL ;

	ptrobj->LifePoint = START_LIFE_POINTS ;

	ptrobj->CoulObj = COUL_PERSO ;	/* bleu twinsen */

	//NextAnim tjrs a "rien"
	ptrobj->NextGenAnim = GEN_ANIM_RIEN ;
	ptrobj->AnimDial = GEN_ANIM_PARLE ;

	NbGoldPieces = 0 ;
	NbLittleKeys = 0 ;
	MagicPoint = 0 ;
	MagicBall = -1 ;
	NbCloverBox = NB_CLOVER_BOX_DEFAULT ;
	ListVarGame[FLAG_CLOVER] = NB_CLOVER_DEFAULT ;
	Weapon = FLAG_BALLE_MAGIQUE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	UseOneClover( void )
{
	S32	x, y, z, beta ;
	S32	nu ;
	T_OBJET *ptrobj = &ListObjet[NUM_PERSO] ;
	S16	memoclover ;

	SaveTimer() ;
	// Penser a restarter les samples d'ambiance et les samples Always
	HQ_StopSample() ;
	RestartRainSample = TRUE ;

	memoclover = ListVarGame[FLAG_CLOVER] ;

	// Restore Valide Pos
	if( !ValidePos )
	{
		ClearScene() ;

		if( ValideCube!=NumCube )
		{
			NewCube = ValideCube ;
			ChangeCube() ;
		}

		RestartValidePos() ;
	}
	else
	{
		ClearFoudre() ;

		x = ptrobj->Obj.X ;
		y = ptrobj->Obj.Y ;
		z = ptrobj->Obj.Z ;
		beta = ptrobj->Obj.Beta ;

		SetAnim( NUM_PERSO, GEN_ANIM_RIEN ) ;

		ptrobj->Obj.X = x ;
		ptrobj->Obj.Y = y ;
		ptrobj->Obj.Z = z ;
		ptrobj->Obj.Beta = beta ;

		SetComportement( Comportement ) ;
	}

	ListVarGame[FLAG_CLOVER] = (S16)(memoclover-1) ;
	ptrobj->LifePoint = MAX_LIFE_POINTS ;
	MagicPoint = (U8)(MagicLevel * 20) ;

	TimerProto = 0 ;

	FlagWater = FALSE ;

	nu = ptrobj->CarryBy ;

	// on recentre Twinsen sur le pseudo sur lequel il se trouvait à la
	// dernière pos valide s'il n'était pas porté par un objet scénarique
	// ou un objet du decors pour qu'il ne reprenne pas en équilibre
	if( nu==-1 )
	{
		// se positionne au coin 0,0 de la brick
		x = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ ;
		z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ ;

		// regarde sur quel type de pseudo on se trouve
		if( CubeMode==CUBE_EXTERIEUR )
		{
			S32	col1, col2 ;
			S32	dx = x>>9; // div512
			S32	dz = z>>9; // div512

			IndexMapPolyTested = dz*64*2+dx*2 ;

			col1 = MapPolyGround[IndexMapPolyTested].CodeJeu ;
			if( col1==CJ_FOOT_WATER )	col1 = 0 ;
			col1 |= MapPolyGround[IndexMapPolyTested].Col ;

			col2 = MapPolyGround[IndexMapPolyTested+1].CodeJeu ;
			if( col2==CJ_FOOT_WATER )	col2 = 0 ;
			col2 |= MapPolyGround[IndexMapPolyTested+1].Col ;

			if( col1 OR col2 )
			{
				if( MapPolyGround[IndexMapPolyTested].Sens )
				{
					if( col1 )
					{
						x += (SIZE_BRICK_XZ-SIZE_BRICK_XZ/4) ;
						z += (SIZE_BRICK_XZ-SIZE_BRICK_XZ/4) ;
					}
					else
					{
						x += (SIZE_BRICK_XZ/4) ;
						z += (SIZE_BRICK_XZ/4) ;
					}
				}
				else
				{
					if( col1 )
					{
						x += (SIZE_BRICK_XZ-SIZE_BRICK_XZ/4) ;
						z += (SIZE_BRICK_XZ/4) ;
					}
					else
					{
						x += (SIZE_BRICK_XZ/4) ;
						z += (SIZE_BRICK_XZ-SIZE_BRICK_XZ/4) ;
					}
				}
			}
			else
			{
				x += (SIZE_BRICK_XZ/2) ;
				z += (SIZE_BRICK_XZ/2) ;
			}

			// reajuste altitude au cas ou l'on soit sur un penché
			y = CalculAltitudeObjet( x, z, -1 ) ;
		}
		else
		{
			x += (SIZE_BRICK_XZ/2) ;
			z += (SIZE_BRICK_XZ/2) ;

			// reajuste altitude au cas ou l'on soit sur un penché
			GetShadow( x, ptrobj->Obj.Y, z ) ;
			y = ShadowY ;

			// pour reajuster les recouvrements pendant le GamePaused()
			ptrobj->Col = WorldColBrick( x, y, z ) ;
		}

		Nxw = x ;
		Nyw = y ;
		Nzw = z  ;

		PtrReajustPos( PtrWorldColBrick( Nxw, Nyw, Nzw ) ) ;

		ptrobj->Obj.X = Nxw ;
		ptrobj->Obj.Y = Nyw ;
		ptrobj->Obj.Z = Nzw ;
	}
	else
	{
		if( nu&CARRY_BY_DECORS )
		{
			if( CubeMode==CUBE_EXTERIEUR )
			{
				// je suis porté par un objet du decors
				T_DECORS *decors ;

				nu &= ~(CARRY_BY_DECORS) ;
				decors = &ListDecors[nu] ;

				// recentre Twinsen au bord de l'objet
				if( ptrobj->Obj.X+ptrobj->XMin<decors->XMin )
				{
					ptrobj->Obj.X = decors->XMin - ptrobj->XMin ;
				}
				else if( ptrobj->Obj.X+ptrobj->XMax>decors->XMax )
				{
					ptrobj->Obj.X = decors->XMax - ptrobj->XMax ;
				}

				if( ptrobj->Obj.Z+ptrobj->ZMin<decors->ZMin )
				{
					ptrobj->Obj.Z = decors->ZMin - ptrobj->ZMin ;
				}
				else if( ptrobj->Obj.Z+ptrobj->ZMax>decors->ZMax )
				{
					ptrobj->Obj.Z = decors->ZMax - ptrobj->ZMax ;
				}

				ptrobj->Obj.Y = decors->YMax+1 ;
			}
		}
		else
		{
			// je suis porté par un objet scénarique
			T_OBJET *ptrobjt = &ListObjet[nu] ;

			// recentre Twinsen au milieu de l'objet
			ptrobj->Obj.X = ptrobjt->Obj.X ;
			ptrobj->Obj.Z = ptrobjt->Obj.Z ;
		}
	}

	ListBuggy[0].Speed = 0 ;
	MagicBall = -1 ;
	StartYFalling = 0 ;

//	CameraCenter( 1 ) ;
	AffScene( AFF_ALL_FLIP ) ;
	GamePaused( PAUSE_CLOVER ) ;
	RestoreTimer() ;

	// to reinit ?
	GetDeltaMove( &RealFalling ) ;
	GetDeltaMove( &RealShifting )    ;
	GetDeltaMove( &SampleAlwaysMove ) ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	SetComportement( U8 comportement )
{
	U8	memogenbody ;
	U16	memogenanim ;
	T_OBJET	*ptrobj = &ListObjet[ NUM_PERSO ] ;

	if( comportement >= MAX_COMPORTEMENTS )	return ;

	switch( Comportement )	// test ancien comportement
	{
		case C_PROTOPACK:
		case C_JETPACK:
			if( IsSamplePlaying( SAMPLE_PROTOPACK ) )
			{
				HQ_StopOneSample( SAMPLE_PROTOPACK ) ;
				ptrobj->SampleAlways = 0 ;
			}
			break ;

		case C_BUGGY:
			// modifie MenuComportement de la conque
			ListMenuComp[3].Next = 0 ;	// branche sur comp Normal
			// modifie MenuComportement Normal
			ListMenuComp[0].Previous = 3 ;	// branche sur comp Conque
			break ;
	}

	// Blindage Buggy lorsqu'appelée de MenuComportement
	if( ptrobj->Move==MOVE_BUGGY
	OR  ptrobj->Move==MOVE_BUGGY_MANUAL )
	{
		// modifie MenuComportement de la conque
		ListMenuComp[3].Next = 0 ;	// branche sur comp Normal
		// modifie MenuComportement Normal
		ListMenuComp[0].Previous = 3 ;	// branche sur comp Conque
	}

	// ATTENTION: Changement d'arme traité au cas par cas
	switch( comportement )
	{
		case C_NORMAL:
		case C_SPORTIF:
		case C_AGRESSIF:
		case C_DISCRET:
			switch( Weapon )
			{
				case FLAG_SCAPHANDRE:
				case FLAG_CONQUE:
					Weapon = FLAG_BALLE_MAGIQUE ;
					break ;
			}
			break ;

		case C_PROTOPACK:
		case C_JETPACK:
			ptrobj->GenBody = GEN_BODY_TUNIQUE ;
			// Toujours la Balle Magique en ProtoPack/JetPack
			Weapon = FLAG_BALLE_MAGIQUE ;
			break ;

		case C_SCAPH_INT_NORM:
		case C_SCAPH_INT_SPOR:
		case C_SCAPH_EXT_NORM:
		case C_SCAPH_EXT_SPOR:
			ptrobj->GenBody = GEN_BODY_TUNIQUE ;
			Weapon = FLAG_SCAPHANDRE ;
			break ;

		case C_CONQUE:
			ptrobj->GenBody = GEN_BODY_TUNIQUE ;
			Weapon = FLAG_CONQUE ;
			break ;

		case C_BUGGY:
			ptrobj->GenBody = GEN_BODY_TUNIQUE ;
			// modifie MenuComportement de la conque
			ListMenuComp[3].Next = 6 ;	// branche sur comp Buggy
			// modifie MenuComportement Normal
			ListMenuComp[0].Previous = 6 ;	// branche sur comp Buggy
			Weapon = FLAG_BALLE_MAGIQUE ;
			break ;

		case C_SKELETON:
			if( ProtectActif )
			{
				ToggleSortProtection() ;
			}
			break ;
	}

	Comportement = comportement ;
	PtrComportement = &ListComportement[comportement] ;

	ptrobj->PtrFile3D = PtrComportement->PtrFile3d ;

	memogenbody = ptrobj->GenBody ;
	ptrobj->GenBody = NO_BODY ;
	ptrobj->Obj.Body.Num = -1 ;

	// ATTENTION: répare un bug lorsqu'on change de cube en NO_BODY
	// mais cela peut avoir des effets secondaires, donc méfie Garçon !
	ptrobj->Obj.NextBody.Num = -1 ;

	InitBody( memogenbody, NUM_PERSO ) ;

	memogenanim = ptrobj->GenAnim ;

	ptrobj->GenAnim = NO_ANIM ;

	// Est-ce un changement  de cube ?
	if( FlagReinit==0 )
	{
		// Test seulement en version jeu pour ne pas ecraser une
		// anim en SET_DIR Free lors d'un chargement d'une
		// sauvegarde
		if( !FlagLoadGame /*OR ptrobj->Move==MOVE_MANUAL*/ )
		{
			ptrobj->FlagAnim = 0 ;
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
			ObjectSetFrame( &(ptrobj->Obj), 0 ) ;
		}
		else
		{
			InitAnim( memogenanim, ptrobj->FlagAnim, NUM_PERSO ) ;
		}
	}
	else
	{
		// oui, alors on reprend à la frame memorisée juste
		// avant le changement de cube
		ptrobj->FlagAnim = 0 ;
		InitAnim( MemoAnim, MemoFlagAnim, NUM_PERSO ) ;
		ClearExtra() ;
		ObjectSetFrame( &(ptrobj->Obj), MemoFrame ) ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
// reinitialise tout perso sans toucher à sa position

void	RestartPerso()
{
	T_OBJET	*ptrobj = &ListObjet[NUM_PERSO] ;

	ptrobj->Move = MOVE_MANUAL ;

	if( FlagReinit==0 )	ptrobj->WorkFlags = 0 ;

	ptrobj->Flags =	  OBJ_FALLABLE
			+ CHECK_ZONE
			+ CHECK_OBJ_COL
			+ CHECK_BRICK_COL
			+ CHECK_CODE_JEU ;

	ptrobj->Armure = 0 ;

	ptrobj->OffsetTrack = -1 ;
	ptrobj->LabelTrack = -1 ;
	ptrobj->OffsetLife = 0 ;
	ptrobj->ZoneSce = -1 ;

	ptrobj->Obj.LastOfsFrame = 0 ;	// Force init anim
	ptrobj->GenAnim = 255 ;	// Sure is not same !
	ptrobj->Obj.Anim.Num = 255 ;	// Sure is not same !

//	ptrobj->Obj.Beta = ValideBeta ;

	InitMove( &ptrobj->BoundAngle.Move, 0 ) ;

	SetComportement( SaveComportement ) ;
//	SetComportement( Comportement ) ;

	//NextAnim tjrs a "rien"
	ptrobj->NextGenAnim = GEN_ANIM_RIEN ;
	ptrobj->AnimDial = GEN_ANIM_PARLE ;

	if( MemoMove==MOVE_BUGGY_MANUAL
	OR  MemoMove==MOVE_BUGGY )
	{
		ptrobj->Move = (U8)MemoMove ;
		SetComportement( C_BUGGY ) ;

		if( MemoMove==MOVE_BUGGY_MANUAL )
		{
			ptrobj->WorkFlags |= MANUAL_INTER_FRAME ;
		}
	}
	else	ptrobj->SampleAlways = 0 ;

	ptrobj->CarryBy = -1 ;

	FlagWater = FALSE ;
	Pushing   = FALSE ;
	TimerProto = 0 ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
/*	Call Every Change Cube	    */
#define	ClearVarsCube()	memset( ListVarCube, 0, MAX_VARS_CUBE )

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █▀▀▀▀ █▀▀▀█ █▀▀▀▀       ██▀▀▀ █▀▀▀▀ █▀▀▀▀ ██▄ █ █▀▀▀▀
	  ██ ▀█ ██▀▀  ██▀█▀ ██▀▀        ▀▀▀▀█ ██    ██▀▀  ██▀██ ██▀▀
	  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	ClearFoudre( void )
{
	S32		n ;
	T_INCRUST_DISP	*ptrdisp = ListIncrustDisp ;

	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		// au cas où on serait en palette eclair
		if( ptrdisp->Num!=-1
		AND (ptrdisp->Type&0xFF)==INCRUST_ECLAIR )
		{
			ptrdisp->Num = -1 ;
			Palette( PtrPal ) ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	ClearScene( void )
{
	S32		n ;
	T_INCRUST_DISP	*ptrdisp = ListIncrustDisp ;

	ClearExtra() 	;
	ClearVarsCube() ;
	ClearAmbiance()	;

	ClearFoudre() ;
	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		ptrdisp->Num = -1 ;
	}

//	FlagPalettePcx = FALSE ;

	ClearDial() 	;

	FlagRain = FALSE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

// Clean HQR si 2/3 de la place occupée
#define	CleanHQR(hqr)	if(hqr&&(hqr->FreeSize<(hqr->MaxSize/3)))	HQR_Reset_Ressource(hqr)

void	ClearHQRs( void )
{
	CleanHQR( HQR_Anims  ) ;
	CleanHQR( HQR_Bodys  ) ;
	CleanHQR( HQR_ObjFix ) ;

	if( (Sample_Driver_Enabled)
	AND (HQR_Samples)
	AND ((HQR_Samples->FreeSize<(HQR_Samples->MaxSize/2)) OR (PLAY_THE_END))
	)
	{
		HQ_StopSample() ;
		HQR_Reset_Ressource( HQR_Samples ) ;
	}

	CleanHQR( HQRPtrAnim3DS ) ;
	CleanHQR( HQRPtrSprite ) ;
	CleanHQR( HQRPtrSpriteRaw ) ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/
void	InitLoadedGame( )
{
	S32	x, y, z ;
	U8	n, nu ;
	T_OBJ_3D	*ptr3d ;
	T_OBJET	*ptrobj = ListObjet ;

	if( ptrobj->GenBody==255 )
	{
		InitBody( NO_BODY, NUM_PERSO ) ;
	}
	else
	{
		S32	frame = ptrobj->Obj.LastFrame ;

		SetComportement( Comportement ) ;

		if( frame==-1 )	frame = 1 ;
		ObjectSetFrame( &(ptrobj->Obj), frame ) ;
	}

	PtrComportement = &ListComportement[Comportement] ;
	ptrobj->PtrFile3D = PtrComportement->PtrFile3d ;

	// ajuste CarryBy pour Twinsen
	if(  (ptrobj->CarryBy!=-1)
	AND !(ptrobj->CarryBy&CARRY_BY_DECORS) )
	{
		nu = (U8)ptrobj->CarryBy ;
		ptrobj->PtrZoneRail = (T_ZONE*)Distance2D( ListObjet[nu].Obj.X, ListObjet[nu].Obj.Z,
							   ptrobj->Obj.X, ptrobj->Obj.Z ) ;
	}

	ptrobj++ ;

	for( n=1; n<NbObjets; n++, ptrobj++ )
	{
		// ajuste CarryBy
		if(  (ptrobj->CarryBy!=-1)
		AND !(ptrobj->CarryBy&CARRY_BY_DECORS) )
		{
			nu = (U8)ptrobj->CarryBy ;
			ptrobj->PtrZoneRail = (T_ZONE*)Distance2D( ListObjet[nu].Obj.X, ListObjet[nu].Obj.Z,
								   ptrobj->Obj.X, ptrobj->Obj.Z ) ;
		}

		if( !(ptrobj->Flags & SPRITE_3D) )
		{
			ptr3d = &ptrobj->Obj ;

			ptrobj->PtrFile3D = LoadFile3D( ptrobj->IndexFile3D ) ;

			if( ptr3d->Body.Num!=-1 )
			{
				ptr3d->Body.Num = -1 ;	// pour forcer l'init
				InitBody( ptrobj->GenBody, n ) ;	// pour retrouver la ZV

				ptr3d->Anim.Num = SearchAnim( ptrobj->GenAnim, n ) ;
				if( ptr3d->Anim.Num==-1 )
				{
					ptr3d->Anim.Num = SearchAnim( GEN_ANIM_RIEN, n ) ;
				}
				ptrobj->PtrAnimAction = SearchPtrAnimAction ;

				// on se blinde les fesses pour etre sûr de recalculer la frame
				ptr3d->Status |= FLAG_BODY ;

				if( ptr3d->LastOfsIsPtr )
				{
					ptr3d->LastFrame = ptr3d->NextFrame ;
					ptr3d->LastOfsFrame = ptr3d->NextOfsFrame ;
					ptr3d->LastOfsIsPtr = FALSE ;
				}

				x = ptr3d->X ;
				y = ptr3d->Y ;
				z = ptr3d->Z ;

				// pour passer outre le moteur
				// (vu qu'on va d'abord passer par le AffScene)
				ObjectSetInterDep( ptr3d ) ;

				ptr3d->X = x ;
				ptr3d->Y = y ;
				ptr3d->Z = z ;
			}
		}
		else
		{
			// c'est un sprite
			if( ptrobj->Obj.Body.Num!=-1 )
			{
				ptrobj->Obj.Body.Num = -1 ;	// pour forcer l'init
				InitSprite( ptrobj->Sprite, ptrobj ) ;
			}
		}
	}

	switch( ListObjet[NUM_PERSO].Move )
	{
		case MOVE_BUGGY:
		case MOVE_BUGGY_MANUAL:
			PtrDoAnim( NUM_PERSO ) ;
			break ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	LoadFile3dObjects()
{
	U8	n ;
	T_OBJET	*ptrobj = &ListObjet[NUM_PERSO] ;

	// Init body Twinsen
	// Attention: Dangereux si Twinsen en body Special
	ptrobj->Obj.Body.Num = -1 ;
	InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;

	ptrobj->GenAnim = GEN_ANIM_RIEN ;
	SetComportement( Comportement ) ;

	ptrobj = &ListObjet[1] ;

	for( n=1; n<NbObjets; n++, ptrobj++ )
	{
		if( !(ptrobj->Flags&SPRITE_3D) )
		{
			ptrobj->PtrFile3D = LoadFile3D( ptrobj->IndexFile3D ) ;
		}

		StartInitObj( n ) ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/
// need: NewCube

// FlagChgCube == 1 : zone de chg cube recupere pos relative
// FlagChgCube == 2 : instruction chg cube positionne sur saved StartPos

void	ChangeCube()
{
	T_OBJET	*ptrobj = &ListObjet[NUM_PERSO] ;
	S32	oldcube ;
	S32	flagload ;

	APtObj = ptrobj ;

	MemoMove = ptrobj->Move ;
	LastCubeMode = CubeMode ;

	oldcube = NumCube ;
	NumCube = NewCube ;

	// reinitialisation du générateur de nombres aléatoires
	srand( TimerRefHR ) ;

	if( FlagChgCube )	SaveTimer() ;
//	if( !FlagLoadGame )	SaveTimer() ;

	// Indique que l'on doit reprendre l'anim à la frame où elle en est
	// après avoir changé de cube

// Pourquoi ce test ????????????
//	if( FlagReajustPosTwinsen )
		FlagReinit = FlagChgCube ;
//	else	FlagReinit = FALSE ;

	if( FlagReinit )
	{
		// Mémorise anim et frame à restaurer après le changement
		// de cube
		MemoAnim      = ptrobj->GenAnim ;
		MemoFlagAnim  = ptrobj->FlagAnim ;
		MemoFrame     = ptrobj->Obj.LastFrame ;

		if( MemoFrame<ptrobj->Obj.NbFrames-2 )	MemoFrame++ ;

		if( MemoFrame>=ptrobj->Obj.NbFrames-1 )
		{
			MemoFrame = ptrobj->Obj.LoopFrame ;
		}
	}

// clear
//	HQ_StopSample() ;

//	FreeGrille() ;
	ClearScene() ;

// perso reinit

	ptrobj->Move = MOVE_MANUAL ;
	ptrobj->ZoneSce = -1 ;
	ptrobj->OffsetLife = 0 ;
	ptrobj->OffsetTrack = -17 ;
	ptrobj->LabelTrack = -1 ;

	ManageSystem() ;// for streaming

// lance usine
	LoadScene( NewCube ) ;

	ManageSystem() ;// for streaming

	if( LastCubeMode!=CubeMode
	OR  PLAY_THE_END )
	{
		ClearHQRs() ;
	}

	InitDial( START_FILE_ISLAND+Island ) ;// Pas Mini

	ManageSystem() ;// for streaming

// load gri

	PtrInitGrille( NewCube ) 		;/*	HQR	*/

	ManageSystem() ;// for streaming

	if( !FlagLoadGame )
	{
		if( FlagChgCube == 1 )
		{
			SceneStartX = NewPosX ;
			SceneStartY = NewPosY ;
			SceneStartZ = NewPosZ ;
		}

		if( FlagChgCube == 2
		OR  FlagChgCube == 0 )
		{
			SceneStartX = CubeStartX ;
			SceneStartY = CubeStartY ;
			SceneStartZ = CubeStartZ ;
		}

		if( FlagChgCube!=3 )
		{
			ptrobj->Obj.X = SceneStartX ;
			ptrobj->Obj.Y = SceneStartY ;
			ptrobj->Obj.Z = SceneStartZ ;
		}

		StartYFalling = 0 ;	// init falling Y when fall
		FlagClimbing = FALSE ;
		PtrZoneClimb = NULL  ;
	}

	SetLightVector( AlphaLight, BetaLight, 0 ) ;

	SaveComportement = Comportement ;
//	ValideBeta = ptrobj->Obj.Beta ;

	if( FlagReajustPosTwinsen
	AND !(ptrobj->WorkFlags & ANIM_MASTER_GRAVITY)
	AND (ptrobj->CarryBy==-1 OR !(ptrobj->CarryBy&CARRY_BY_DECORS)) )
	{
		// Recale Twinsen pour eviter de rentrer dans une collision
		if( CubeMode==CUBE_EXTERIEUR )
		{
			ShadowX = SceneStartX ;
			ShadowY = CalculAltitudeObjet(SceneStartX,SceneStartZ,0) ;
			ShadowZ = SceneStartZ ;
		}
		else	GetShadow( SceneStartX, SceneStartY, SceneStartZ ) ;

		Nxw = ShadowX ;
		Nyw = ShadowY ;
		Nzw = ShadowZ ;

		PtrReajustPos( PtrWorldColBrick( Nxw, Nyw, Nzw ) ) ;

		ptrobj->Obj.X = Nxw ;
		ptrobj->Obj.Y = Nyw ;
		ptrobj->Obj.Z = Nzw ;
	}
	else
	{
		ptrobj->Obj.X = SceneStartX ;
		ptrobj->Obj.Y = SceneStartY ;
		ptrobj->Obj.Z = SceneStartZ ;
	}

	RestartPerso() ;

	if( !FlagLoadGame )
	{
		StartInitAllObjs() ;

		if( !FlagDrawHorizon
		OR  CubeMode+LastCubeMode!=2 )
		{
			FlagFade = TRUE  ;
		}
		else if( FlagBlackPal AND !FlagFade )
		{
			FlagPal = TRUE ;
		}
	}

	ManageSystem() ;// for streaming

	NbLittleKeys = 0 ;
	MagicBall = -1 ;
	LastJoyFlag = TRUE ;
	FirstTime = AFF_ALL_FLIP ;
	CameraZone = FALSE ;
	NewCube = -1 ;

	SamplePlayed = 2+4+8 ;	// joue le premier sample en 1er
	TimerNextAmbiance = 0 ;

	if( FlagChgCube == 2
	OR  FlagChgCube == 0 )
	{
		CameraCenter( 1 ) ;
	}
	else
	{
		StartXCube = ListObjet[NumObjFollow].Obj.X/SIZE_BRICK_XZ ;
		StartYCube = (ListObjet[NumObjFollow].Obj.Y+SIZE_BRICK_Y)/SIZE_BRICK_Y ;
		StartZCube = ListObjet[NumObjFollow].Obj.Z/SIZE_BRICK_XZ ;

		CameraCenter( 0 ) ;
	}


	SetLightVector( AlphaLight, BetaLight, 0 ) ;

	if( StopLastMusic )	StopMusic() ;

	if( CubeJingle == 255 )
	{
		NextMusic = -1 ;// Coupe Empilage du cube précédent
	}

	RazListPartFlow( ) ;

	// determine quelle est la monnaie courante
	if( Island>3 )	TabInv[FLAG_MONEY].IdObj3D = 1 ;
	else		TabInv[FLAG_MONEY].IdObj3D = 0 ;

	NumBuggy &= ~(BUGGY_PRESENT) ;

	if( FlagLoadGame )
	{
#if defined(DEBUG_TOOLS) || defined(TEST_TOOLS)
		if( (NumVersion&MASK_NUM_VERSION)!=NUM_VERSION )
		{
			Message( "Warning: Sauvegarde trop ancienne. Je vais tenter de la charger, mais méfie !!", TRUE ) ;

			flagload = LoadGameOldVersion() ;
//			flagload = FALSE ;	// en essai
		}
		else	flagload = LoadGame() ;
#else
		flagload = LoadGame() ;
#endif

		if( !flagload )	InitLoadedGame( ) ;
		else		LoadFile3dObjects() ;

		CheckProtoPack() ;

		NewCube = -1 ;
	}
	else
	{
		RestoreTimer() ;

		// Si le Sort de protection était actif, on le réactive
		if( ProtectActif )
		{
			ProtectActif = 0 ;// vu que la fonction s'appelle Toggle....
			ToggleSortProtection() ;
		}
	}

	if( MemoMove==MOVE_BUGGY_MANUAL
	OR  MemoMove==MOVE_BUGGY )
	{
		ptrobj->Move = (U8)MemoMove ;
		SetComportement( C_BUGGY ) ;

		if( MemoMove==MOVE_BUGGY_MANUAL )
		{
			ptrobj->WorkFlags |= MANUAL_INTER_FRAME ;
		}
	}

	ManageSystem() ;// for streaming

	// Ok, changement de cube terminé
	FlagReinit = 0 ;

	LastValidePos = ValidePos = FALSE ;
	ptrobj->CarryBy = -1 ;
}

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █▀▀▀█ █     █      █    ██▀▀▀  █    █▀▀▀█ ██▄ █ ██▀▀▀
	  ██    ██  █ ██    ██     ██   ▀▀▀▀█  ██   ██  █ ██▀██ ▀▀▀▀█
	  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀  ▀▀   ▀▀▀▀▀  ▀▀   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	InitHitExtraPof( S32 x, S32 y, S32 z, S32 hitforce )
{
	S32	nb = 0 ;

	while( hitforce>=1
	AND nb<MAX_HIT_POFS )	// on limite à 5 étoiles
	{
		InitExtraPof( x, y, z, 0,
			MyRnd((256*MUL_ANGLE))+(128*MUL_ANGLE),
			MyRnd((1024*MUL_ANGLE)),
			50, 20,
			POF_CHOC_SCALE, POF_CHOC_SCALE, 0, POF_CHOC_TIMER, POF_CHOC_DUREE ) ;

		hitforce-=5 ;
		nb++ ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	HitObj( U8 numhitter, U8 num, S32 hitforce, S32 beta )
{
	S32	memo ;
	T_OBJET *ptrobjt ;

	ptrobjt = &ListObjet[ num ] ;

	if( ptrobjt->Flags&NO_CHOC )	return ;
	if( ptrobjt->LifePoint <= 0 )	return ;
	if( ptrobjt->WorkFlags&FRAME_SHIELD )	return ;

	// Blindage MécaPingouin lorsque Twinsen est en automatique
	if( num==NUM_PERSO
	AND numhitter==NUM_PERSO
	AND ptrobjt->Move==NO_MOVE )
	{
		return ;
	}

	ptrobjt->HitBy = numhitter ;

	if( ptrobjt->Armure <= hitforce
	AND ptrobjt->Armure < 51 )
	{
		hitforce -= ptrobjt->Armure ;

		if( (ptrobjt->GenAnim == GEN_ANIM_CHOC)
		OR  (ptrobjt->GenAnim == GEN_ANIM_CHOC2) )
		{
			memo = ptrobjt->Obj.LastFrame ;
			ptrobjt->Obj.LastFrame = 1-1 ;

			if( ptrobjt->PtrAnimAction != 0 )
			{
				GereAnimAction( ptrobjt, num ) ;
			}
			ptrobjt->Obj.LastFrame = memo ;
		}
		else
		{
			if( (beta!=-1)
			AND !(ptrobjt->WorkFlags&TRACK_MASTER_ROT) )
			{
				InitBoundAngleMove( &ptrobjt->BoundAngle,
					0,
					beta,
					beta ) ;
			}

			if( !(rand()&1) )
				InitAnim( GEN_ANIM_CHOC, ANIM_ALL_THEN, num ) ;
			else
				InitAnim( GEN_ANIM_CHOC2, ANIM_ALL_THEN, num ) ;
		}

		/* effet special choc */
		/* etoile */
		InitHitExtraPof(ptrobjt->Obj.X,
				ptrobjt->Obj.Y+ptrobjt->YMax+1,
				ptrobjt->Obj.Z,
				hitforce ) ;

		if( num == NUM_PERSO )
		{
			LastJoyFlag = TRUE ;
		}

		ptrobjt->LifePoint -= hitforce ;

		/* attention dans l'outil LifePoint est un word
		si ca devient un U8 pas de signe */
		if( ptrobjt->LifePoint < 0 )	ptrobjt->LifePoint = 0 ;
	}
	else
	{
		if( ptrobjt->GenAnim == GEN_ANIM_ENCAISSE )
		{
			memo = ptrobjt->Obj.LastFrame ;
			ptrobjt->Obj.LastFrame = 1-1 ;

			if( ptrobjt->PtrAnimAction != 0 )
			{
				GereAnimAction( ptrobjt, num ) ;
			}
			ptrobjt->Obj.LastFrame = memo ;
		}
		else
		{
			InitAnim( GEN_ANIM_ENCAISSE, ANIM_ALL_THEN, num ) ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	FoudroieObj( U8 numhitter, U8 num, S32 armure )
{
	S32	memo ;
	T_OBJET *ptrobjt ;

	ptrobjt = &ListObjet[ num ] ;

	if( ptrobjt->Flags&NO_CHOC )	return ;
	if( ptrobjt->LifePoint <= 0 )	return ;

	ptrobjt->HitBy = numhitter ;

	if( ptrobjt->Armure <= armure
	AND ptrobjt->Armure < 51 )
	{
		/* effet special choc */
		/* etoile */
		InitHitExtraPof(ptrobjt->Obj.X,
				ptrobjt->Obj.Y+ptrobjt->YMax+1,
				ptrobjt->Obj.Z,
				armure ) ;

		ptrobjt->LifePoint = 0 ;
	}
	else
	{
		if( ptrobjt->GenAnim == GEN_ANIM_ENCAISSE )
		{
			memo = ptrobjt->Obj.LastFrame ;
			ptrobjt->Obj.LastFrame = 1-1 ;
			if( ptrobjt->PtrAnimAction != 0 )
			{
				GereAnimAction( ptrobjt, num ) ;
			}
			ptrobjt->Obj.LastFrame = memo ;
		}
		else
		{
			InitAnim( GEN_ANIM_ENCAISSE, ANIM_ALL_THEN, num ) ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

S32	CheckZvOnZv( U8 numobj, U8 numobjt )
{
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;

	ptrobj = &ListObjet[numobj] ;

	x0 = Nxw + ptrobj->XMin ;
	x1 = Nxw + ptrobj->XMax ;
	y0 = Nyw + ptrobj->YMin ;
	y1 = Nyw + ptrobj->YMax ;
	z0 = Nzw + ptrobj->ZMin ;
	z1 = Nzw + ptrobj->ZMax ;

	ptrobjt = &ListObjet[numobjt] ;

	xt0 = ptrobjt->Obj.X + ptrobjt->XMin ;
	xt1 = ptrobjt->Obj.X + ptrobjt->XMax ;
	yt0 = ptrobjt->Obj.Y + ptrobjt->YMin ;
	yt1 = ptrobjt->Obj.Y + ptrobjt->YMax ;
	zt0 = ptrobjt->Obj.Z + ptrobjt->ZMin ;
	zt1 = ptrobjt->Obj.Z + ptrobjt->ZMax ;

	if( x0 < xt1
	AND x1 > xt0
	AND y0 <= (yt1+1)
	AND y0 > (yt1 - SIZE_BRICK_Y)
	AND y1 > yt0
	AND z0 < zt1
	AND z1 > zt0 )
	{
		return TRUE ;
	}

	return FALSE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

S32	CheckObjCol( U8 numobj )
{
	S32	flagy ;
	S32	angle, angle2 ;
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;
	S32	dx, dz ;
	U8	n ;

	ptrobj = &ListObjet[numobj] ;
/*
	x0 = Nxw + ptrobj->XMin ;
	x1 = Nxw + ptrobj->XMax ;
*/
	y0 = Nyw + ptrobj->YMin ;
	y1 = Nyw + ptrobj->YMax ;

/*	z0 = Nzw + ptrobj->ZMin ;
	z1 = Nzw + ptrobj->ZMax ;
*/
	ptrobjt = ListObjet ;

	ptrobj->ObjCol = 255 ;

	for( n=0; n<NbObjets; n++,ptrobjt++ )
	{
		// Je sais c'est pas optimisé mais c'est necessaire si l'une ou
		// l'autre des coordonnees a ete modifiée dans la boucle pour
		// un objet précedent !
test_obj_zv:
		flagy = FALSE ;

		x0 = Nxw + ptrobj->XMin ;
		x1 = Nxw + ptrobj->XMax ;
		z0 = Nzw + ptrobj->ZMin ;
		z1 = Nzw + ptrobj->ZMax ;

		if( (n != numobj)
		AND (ptrobjt->Obj.Body.Num != -1)
//		AND (!(ptrobj->Flags&INVISIBLE))
		AND (ptrobjt->CarryBy != numobj) )
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
			ptrobj->ObjCol = n ;

			// On ne veut pas ejecter un wagon des rails
			// Mais on veut connaitre ses collisions !!!
			// (je sais, je suis capricieux)
			if( ptrobj->Move==MOVE_WAGON
			AND !(ListObjet[n].Flags&SPRITE_3D) )	return n ;

			if( ptrobjt->Flags & OBJ_CARRIER )
			{
				if (( y0 <= (yt1+1)) // CheckZvOnZv( numobj, n )
				AND ( y0 > (yt1-SIZE_BRICK_Y/*/2*/)))
				{
					// je marche sur un transporteur */
					Nyw = yt1 - ptrobj->YMin + 1 	;

					if( ptrobj->CarryBy!=n
					AND ptrobj->Move!=MOVE_WAGON )	// Ne peut pas être porté !!!!
									// ne devrait jamais arriver ici
					{
						// Attention, on mémorise le beta de l'objet qui nous porte
						ptrobj->OldBeta = (GetAngle2D( ptrobjt->Obj.X, ptrobjt->Obj.Z, Nxw, Nzw )-ptrobjt->Obj.Beta)&4095 ;

						ptrobj->PtrZoneRail = (T_ZONE*)Distance2D( ptrobjt->Obj.X, ptrobjt->Obj.Z, Nxw, Nzw ) ;
						ptrobj->CarryBy = n 		;
					}
					continue 			;
				}
			}
			else
			{	// je marche sur quelqu'un
				if( ( y0 <= (yt1+1)) // CheckZvOnZv( numobj, n )
				AND ( y0 > (yt1-SIZE_BRICK_Y))
				AND !(ptrobjt->Flags&SPRITE_3D)
				AND (ptrobjt->GenAnim!=GEN_ANIM_CHOC)
				AND (ptrobjt->GenAnim!=GEN_ANIM_CHOC2)
				AND (ptrobjt->GenAnim!=GEN_ANIM_ENCAISSE) )
				{
					HitObj( numobj, n, 1, -1 ) ;
					flagy = TRUE ;
				}
			}

			angle2 = angle = GetAngle2D( Nxw,Nzw,
					  ptrobjt->Obj.X,ptrobjt->Obj.Z ) ;

			// test obj pushable
			if( (ptrobjt->Flags & PUSHABLE)
			AND !(ptrobj->Flags & PUSHABLE) // protect reaction en chaine
			AND (!IsBuggyPresent() OR n!=NUM_BUGGY OR numobj!=NUM_PERSO) ) // Twinsen ne peut pas pousser le buggy
			{
				if( numobj == NUM_PERSO )
				{
					if( abs(ptrobj->Obj.Beta-angle2) > 2048 )
					{
						angle2 -= MAX_ANGLE ;
					}

					if( abs( ptrobj->Obj.Beta-angle2 ) <= 1024 )
					{
						Pushing = n ;
					}
				}

				ptrobjt->Coord.Push.PushY = 0 ;

				if( ptrobjt->Flags & MINI_ZV )
				{ // magouille boxxle align pos
					if( angle >= (128*MUL_ANGLE) AND angle < (384*MUL_ANGLE) )
						if( ptrobj->Obj.Beta >= (128*MUL_ANGLE) AND ptrobj->Obj.Beta < (384*MUL_ANGLE) )
							ptrobjt->Coord.Push.PushX = SIZE_BRICK_XZ/8 ;
					if( angle >= (384*MUL_ANGLE) AND angle < (640*MUL_ANGLE) )
						if( ptrobj->Obj.Beta >= (384*MUL_ANGLE) AND ptrobj->Obj.Beta < (640*MUL_ANGLE) )
							ptrobjt->Coord.Push.PushZ = -SIZE_BRICK_XZ/8 ;
					if( angle >= (640*MUL_ANGLE) AND angle < (896*MUL_ANGLE) )
						if( ptrobj->Obj.Beta >= (640*MUL_ANGLE) AND ptrobj->Obj.Beta < (896*MUL_ANGLE) )
							ptrobjt->Coord.Push.PushX = -SIZE_BRICK_XZ/8 ;
					if( angle >= (896*MUL_ANGLE) OR  angle < (128*MUL_ANGLE) )
						if( ptrobj->Obj.Beta >= (896*MUL_ANGLE) OR ptrobj->Obj.Beta < (128*MUL_ANGLE) )
							ptrobjt->Coord.Push.PushZ = SIZE_BRICK_XZ/8 ;
				}
				else
				{ // induit deplacement avant reajustement ?

					ptrobjt->Coord.Push.PushX = Nxw - ptrobj->OldPosX ;
					ptrobjt->Coord.Push.PushZ = Nzw - ptrobj->OldPosZ ;
				}
			}

			// joli mais ne fonctionne qu'avec
			// des ZV carres

			// donc rustine tempo
//			if( ptrobjt->XMax-ptrobjt->XMin ==
//			    ptrobjt->ZMax-ptrobjt->ZMin )

/*
			if( (ptrobjt->XMax-ptrobjt->XMin ==
			    ptrobjt->ZMax-ptrobjt->ZMin )
			    AND
			    (ptrobj->XMax-ptrobj->XMin ==
			    ptrobj->ZMax-ptrobj->ZMin) )
			{   // si ZV carre


				if( angle >= 128 AND angle < 384 )
					Nxw = xt0 - ptrobj->XMax ;
				if( angle >= 384 AND angle < 640 )
					Nzw = zt1 - ptrobj->ZMin ;
				if( angle >= 640 AND angle < 896 )
					Nxw = xt1 - ptrobj->XMin ;
				if( angle >= 896 OR  angle < 128 )
					Nzw = zt0 - ptrobj->ZMax ;
			}
			else
			{
				if( !(ptrobj->WorkFlags & FALLING) ) // gloups
				{
					// refuse pos
					Nxw = OldX ;
					Nyw = OldY ;
					Nzw = OldZ ;
				}
			}
*/
	//  --> pour les collisions avec des objets dont la ZV n'est pas
	//	   carree.

			// on ne recale pas si l'objet est un carrier
			// (cf bug des plaques qui tombent sur Twinsen)
			if( ptrobj->Flags & OBJ_CARRIER AND flagy )
			{
				Nxw = OldX ;
				Nyw = OldY ;
				Nzw = OldZ ;
			}
			else
			{
				S32	oldx, oldz ;
				S32	nbrecal = 0 ;
				S32	coin ;

				oldx = Nxw ;
				oldz = Nzw ;

				dx = __min(abs(xt1-x0),abs(x1-xt0)) ;
				dz = __min(abs(zt1-z0),abs(z1-zt0)) ;

				if( dx<dz )
				{
					if( x0<xt0 OR Nxw<=ptrobjt->Obj.X )
						coin = 0  ;
					else	coin = 1  ;
				}
				else
				{
					if( z0<zt0 OR Nzw<=ptrobjt->Obj.Z )
						coin = 2  ;
					else	coin = 3  ;
				}

test_next_coin:
				switch( coin )
				{
					case 0:
						Nxw -= dx ;
						break ;

					case 1:
						Nxw += dx ;
						break ;

					case 2:
						Nzw -= dz ;
						break ;

					case 3:
						Nzw += dz ;
						break ;
				}

				if( flagy )// je suis rentré par dessus
				{
					if( PtrWorldColBrick( Nxw, Nyw, Nzw )!=0 )
					{
						if( nbrecal<3 )
						{
							Nxw = oldx ;
							Nzw = oldz ;

							coin++ 	  ;
							if( coin>3 )	coin = 0 ;
							nbrecal++ ;

							goto test_next_coin ;
						}
					}
					else	goto test_obj_zv ;// pos acceptée, on reteste cet objet
				}
			}
		}
		}

		if( numobj==NUM_PERSO AND Pushing AND Pushing==n )
		{
			angle = GetAngle2D( Nxw, Nzw,
					  ptrobjt->Obj.X,ptrobjt->Obj.Z ) ;

			if( abs(ptrobj->Obj.Beta-angle) > 2048 )	angle -= MAX_ANGLE ;

/*	old way
			if( abs(ptrobj->Obj.Beta-angle) > (1024+512)
			OR (Distance2D(Nxw, Nzw, ptrobjt->Obj.X, ptrobjt->Obj.Z)>(ptrobjt->XMax+ptrobj->XMax+256) )
			) // on n'est plus face a l'objet
			{
				Pushing = 1000 ;	// passe en anim rien
			}
*/
			if( abs(ptrobj->Obj.Beta-angle) > 1024 ) // on n'est plus face a l'objet
			{
				Pushing = 1000 ;	// passe en anim rien
			}
		}
	}

	// test deplace la ZV plus en avant si frappe

	if( ptrobj->WorkFlags & OK_HIT )
	{
		Rotate( 0, 200, ptrobj->Obj.Beta ) ;

		x0 = Nxw + ptrobj->XMin + X0 ;
		x1 = Nxw + ptrobj->XMax + X0 ;
// deja fait
//		y0 = Nyw + ptrobj->YMin ;
//		y1 = Nyw + ptrobj->YMax ;
		z0 = Nzw + ptrobj->ZMin + Z0 ;
		z1 = Nzw + ptrobj->ZMax + Z0 ;

		ptrobjt = ListObjet ;

		for( n=0; n<NbObjets; n++,ptrobjt++ )
		{
			if( (n != numobj)
			AND (ptrobjt->Obj.Body.Num != -1)
			AND (!(ptrobjt->Flags&INVISIBLE))
			AND (ptrobjt->CarryBy != numobj) )
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
					HitObj( numobj, n, ptrobj->HitForce, ptrobj->Obj.Beta+(512*MUL_ANGLE) ) ;
					ptrobj->WorkFlags &= ~OK_HIT ;
				}
			}
		}
	}

	if( ptrobj->WorkFlags & OK_SUPER_HIT )	// le 09/01/95
	{
		x0 = ptrobj->Coord.SHit.SHitX-ptrobj->SizeSHit ;
		x1 = ptrobj->Coord.SHit.SHitX+ptrobj->SizeSHit ;
		y0 = ptrobj->Coord.SHit.SHitY-ptrobj->SizeSHit ;
		y1 = ptrobj->Coord.SHit.SHitY+ptrobj->SizeSHit ;
		z0 = ptrobj->Coord.SHit.SHitZ-ptrobj->SizeSHit ;
		z1 = ptrobj->Coord.SHit.SHitZ+ptrobj->SizeSHit ;

		ptrobjt = ListObjet ;

		for( n=0; n<NbObjets; n++,ptrobjt++ )
		{
			if( (n != numobj)
			AND (ptrobjt->Obj.Body.Num != -1)
			AND (!(ptrobjt->Flags&INVISIBLE))
			AND (ptrobjt->CarryBy != numobj) )
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
					HitObj( numobj, n, ptrobj->HitForce, ptrobj->Obj.Beta+(512*MUL_ANGLE) ) ;
					ptrobj->WorkFlags &= ~OK_SUPER_HIT ;
					ptrobj->WorkFlags |= WAIT_SUPER_HIT ;
				}
			}
		}
	}

	return ptrobj->ObjCol ;
}

/*──────────────────────────────────────────────────────────────────────────*/

U8	IsObjCol( U8 numobj )
{
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;
	U8	n ;

	ptrobj = &ListObjet[numobj] ;

	x0 = Nxw + ptrobj->XMin ;
	x1 = Nxw + ptrobj->XMax ;
	y0 = Nyw + ptrobj->YMin ;
	y1 = Nyw + ptrobj->YMax ;
	z0 = Nzw + ptrobj->ZMin ;
	z1 = Nzw + ptrobj->ZMax ;

	ptrobjt = ListObjet ;

	for( n=0; n<NbObjets; n++, ptrobjt++ )
	{
		if( (n != numobj)
		AND (ptrobjt->Obj.Body.Num != -1)
		AND (ptrobjt->CarryBy != numobj) )
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
				return TRUE ;
			}
		}
	}

	return FALSE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	CheckCarrier( U8 numobj )
{
	U8	n ;

	if( ListObjet[numobj].Flags & OBJ_CARRIER )
	{
		T_OBJET	*ptrobj = ListObjet ;

		for( n=0; n<NbObjets; n++, ptrobj++ )
		{
			if( ptrobj->CarryBy == numobj )
			{
				ptrobj->CarryBy = -1 ;
			}
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

S32	CheckValidObjPos( U8 numobj, U8 owner )
{
	T_ZONE	*ptrz ;
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;
	U8	n, cj ;

	ptrobj = &ListObjet[numobj] ;

	if( (ptrobj->Obj.X<DEMI_BRICK_XZ) OR (ptrobj->Obj.X>SIZE_BRICK_XZ*64-DEMI_BRICK_XZ-1) )	return FALSE ;
//	if( (ptrobj->Obj.Y<DEMI_BRICK_Y)  OR (ptrobj->Obj.Y>SIZE_BRICK_Y*64-DEMI_BRICK_Y-1) )	return FALSE ;
	if( (ptrobj->Obj.Z<DEMI_BRICK_XZ) OR (ptrobj->Obj.Z>SIZE_BRICK_XZ*64-DEMI_BRICK_XZ-1) )	return FALSE ;

	x0 = ptrobj->Obj.X + ptrobj->XMin ;
	x1 = ptrobj->Obj.X + ptrobj->XMax ;
	y0 = ptrobj->Obj.Y + ptrobj->YMin ;
	y1 = ptrobj->Obj.Y + ptrobj->YMax ;
	z0 = ptrobj->Obj.Z + ptrobj->ZMin ;
	z1 = ptrobj->Obj.Z + ptrobj->ZMax ;

	// test decors + terrain

	if( CubeMode==CUBE_INTERIEUR )
	{
		// check point chaud
		if( WorldColBrickFull( ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z, ptrobj->YMax ) )	return FALSE ;

		// check zv
		if( WorldColBrickFull( x0, y0, z0, ptrobj->YMax ) )	return FALSE ;
		if( WorldColBrickFull( x1, y0, z0, ptrobj->YMax ) )	return FALSE ;
		if( WorldColBrickFull( x1, y0, z1, ptrobj->YMax ) )	return FALSE ;
		if( WorldColBrickFull( x0, y0, z1, ptrobj->YMax ) )	return FALSE ;
	}
	else
	{
		S32	col ;
		S32	dx = ptrobj->Obj.X>>9; // div512
		S32	dz = ptrobj->Obj.Z>>9; // div512

		// check point chaud
		if( WorldColBrickExt( ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z )==1 )	return FALSE ;

		// check zv
		if( WorldColBrickExt( x0, y0, z0 )==1 )	return FALSE ;
		if( WorldColBrickExt( x1, y0, z0 )==1 )	return FALSE ;
		if( WorldColBrickExt( x1, y0, z1 )==1 )	return FALSE ;
		if( WorldColBrickExt( x0, y0, z1 )==1 )	return FALSE ;

		// teste ZV dans ZV des objets du decors
		if( WorldColBrickDecors( x0, y0, z0, x1, y1, z1 )==1 )	return FALSE ;

		NumDecorsCol = -1 ;

		ReajustPosDecors( x0, z0, x1, z1,
				  CalculAltitudeObjet(ptrobj->Obj.X,ptrobj->Obj.Z,0),
				  y0 ) ;

		if( NumDecorsCol==-1 )
		{
			IndexMapPolyTested = dz*64*2+dx*2 + GiveTriangle(ptrobj->Obj.X,ptrobj->Obj.Z) ;

			col = MapPolyGround[IndexMapPolyTested].CodeJeu ;
			if( col==CJ_FOOT_WATER )	col = 0 ;
			col |= MapPolyGround[IndexMapPolyTested].Col ;

			if( col )	return FALSE ;
		}
	}

	// test liste des objets

	ptrobjt = ListObjet ;
	for( n=0; n<NbObjets; n++,ptrobjt++ )
	{
		if( (n != numobj) AND (n != owner)
		AND (ptrobjt->Obj.Body.Num != -1)
		AND (!(ptrobjt->Flags&INVISIBLE))
		AND (ptrobjt->CarryBy != numobj) )
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
			{	// collision pos refusée
				return FALSE ;
			}
		}
	}

	x0 = ptrobj->Obj.X ;
	y0 = ptrobj->Obj.Y ;
	z0 = ptrobj->Obj.Z ;

	PtrGetShadow( x0, y0, z0 ) ;

	// teste code jeu
	cj = PtrWorldCodeBrick( x0, ShadowY-1, z0 ) ;

	switch( cj>>4 )
	{
		case CJ_LAVE:
		case CJ_GAZ:
		case CJ_WATER:
		case CJ_ANIMATED_GAZ:
		case CJ_ANIMATED_LAVE:
		case CJ_ELECTRIC:
		case CJ_POS_INVALID:
			return FALSE ;
	}

	// check diff altitude
//	PtrGetShadow( x0, y0, z0 ) ;

	if( ShadowY-y0 > 256 )	return FALSE ;

	// test zones
	ptrz = ListZone ;

	for( n=0; n<NbZones; n++, ptrz++ )
	{
		if( x0 >= ptrz->X0
		AND x0 <  ptrz->X1
		AND y0 >= ptrz->Y0
		AND y0 <= ptrz->Y1
		AND z0 >= ptrz->Z0
		AND z0 <  ptrz->Z1 )
		{
			switch( ptrz->Type )
			{
				case 0:	// ChangeCube
					if( numobj==NUM_PERSO )
					{
						return FALSE ;
					}
					break ;

				case 8:	// Hit
					return FALSE ;// ce serait salaud...
			}
		}
	}

	return TRUE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#define	START_AROUND_BETA	1024
#define	END_AROUND_BETA		3072
#define	STEP_AROUND_BETA	128	// 16 pos testees

// positionne l'objet objtopos autour de l'objet objsrc

void	PosObjetAroundAnother( U8 numsrc, U8 numtopos )
{
	T_OBJET *objsrc	;
	T_OBJET *objtopos ;
	S32	xb, zb ;
	S32	beta, dist, dist2 ;
	S32	step ;

	objsrc   = &ListObjet[numsrc] ;
	objtopos = &ListObjet[numtopos] ;

	xb = objsrc->Obj.X	;
	zb = objsrc->Obj.Z	;

	objtopos->Obj.Y = objsrc->Obj.Y ;

	dist = __max(objsrc->XMin,objsrc->XMax) ;
	dist = __max(dist,objsrc->ZMin) ;
	dist = __max(dist,objsrc->ZMax) ;

	dist2 = __max(objtopos->XMin,objtopos->XMax) ;
	dist2 = __max(dist2,objtopos->ZMin) ;
	dist2 = __max(dist2,objtopos->ZMax) ;

	dist += dist/2 + dist2 + dist2/2 ;

	beta = (objsrc->Obj.Beta+START_AROUND_BETA)&4095 ;

	for( step=0; step<(4096/STEP_AROUND_BETA); step++, beta+=STEP_AROUND_BETA )
	{
		beta &= 4095 ;
		Rotate( 0, dist, beta ) ;

		objtopos->Obj.X = xb + X0 ;
		objtopos->Obj.Z = zb + Z0 ;

		if( CheckValidObjPos( numtopos, numsrc ) )
		{
			// accepte position
			break ;
		}
	}

	objtopos->Obj.Beta = GetAngle2D(xb,zb,objtopos->Obj.X,objtopos->Obj.Z)&4095 ;
}

/*══════════════════════════════════════════════════════════════════════════*
			 █▀▀█  █▀▀▀█ █▀▀▀▄ █  ▄▀
			 ██▀▀█ ██  █ ██  █ ██▀
			 ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀  ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
U8	ChoiceHeroBody( U8 genbody )
{
/* peut-etre dangereux d'enlever ca
	if( genbody>8 )	// c'est un body special
	{
		return genbody ;
	}
*/

	if( genbody!=GEN_BODY_TUNIQUE )	return genbody ;

	if( ListVarGame[FLAG_TUNIQUE]==0 )
	{
		return GEN_BODY_NORMAL	;
	}
	else
	{
		// est-on en mage ?
		if( TabInv[FLAG_TUNIQUE].IdObj3D==0 )
		{
			switch( Weapon )
			{
				case FLAG_SARBACANE:
					// est-on en Sarbatron ?
					if( TabInv[FLAG_SARBACANE].IdObj3D )
						return GEN_BODY_SARBATRON	;
					else	return GEN_BODY_SARBACANE	;

				case FLAG_PISTOLASER:
					return GEN_BODY_PISTOLASER	;

				case FLAG_GANT:
					return GEN_BODY_GANT	;

				case FLAG_SABRE:
					return GEN_BODY_SABRE	;

				default:
					return GEN_BODY_TUNIQUE	;
			}
		}
		else
		{
			switch( Weapon )
			{
				case FLAG_SARBACANE:
					return GEN_BODY_MAGE_SARBACANE	;

				default:
					return GEN_BODY_MAGE	;
			}
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	InitBody(	U8 gennewbody,
			U8 numobj )
{
	S32	newbody ;
	T_OBJET *ptrobj ;

	ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Flags & SPRITE_3D )	return ;

	if( numobj==NUM_PERSO )	gennewbody = ChoiceHeroBody( gennewbody ) ;

	if( gennewbody != NO_BODY )
	{
		newbody = SearchBody( gennewbody, numobj ) ;

		if( newbody==-1 )
		{
			// protect au cas où ...
			newbody = SearchBody( GEN_BODY_NORMAL, numobj ) ;
		}
	}
	else
	{
		newbody = -1 ;
	}

	if( newbody != -1 )
	{
		if( newbody != ptrobj->Obj.Body.Num )
		{
			// force init si c'est la premiere boucle scenarique
			if( FirstLoop )	ptrobj->Obj.Body.Num = -1 ;

			ObjectInitBody( &ptrobj->Obj, (void*)newbody )	;

			ptrobj->GenBody = gennewbody ;
		}

		// Pour pouvoir changer de zv avec un même body
		if( M_XMin == -32000 )
		{
			InitBodyZV( ptrobj ) ;
		}
		else
		{
			ptrobj->XMin =  M_XMin ;
			ptrobj->XMax =  M_XMax ;
			ptrobj->YMin =  M_YMin ;
			ptrobj->YMax =  M_YMax ;
			ptrobj->ZMin =  M_ZMin ;
			ptrobj->ZMax =  M_ZMax ;
		}
	}
	else
	{

		ptrobj->GenBody = NO_BODY ;

		// Pour forcer le champ NextBody à -1
		ObjectInitBody( &ptrobj->Obj, (void*)-1 )	;
		ptrobj->Obj.Body.Num = -1 ;

		ptrobj->YMin =  0 ;
		ptrobj->YMax =  0 ;
		ptrobj->XMin =  0 ;
		ptrobj->XMax =  0 ;
		ptrobj->ZMin =  0 ;
		ptrobj->ZMax =  0 ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	InitSprite(	S32 newsprite,
			T_OBJET *ptrobj )
{
	S16	*ptr ;

	ptrobj->Sprite = (S16)newsprite ;

	if( !(ptrobj->Flags & SPRITE_3D) )	return ;

	if( newsprite != -1 AND newsprite != ptrobj->Obj.Body.Num )
	{
		ptrobj->Obj.Body.Num = newsprite ;

		if( ptrobj->Flags&ANIM_3DS )	ptr = &PtrZvAnim3DS[ newsprite*8 + 2 ] ;
		else
		{
			if( newsprite>=100 )
				ptr = &PtrZvExtra[ newsprite*8 + 2 ] ;
			else	ptr = &PtrZvExtraRaw[ newsprite*8 + 2 ] ;
		}

		ptrobj->XMin =  *ptr++ ;
		ptrobj->XMax =  *ptr++ ;
		ptrobj->YMin =  *ptr++ ;
		ptrobj->YMax =  *ptr++ ;
		ptrobj->ZMin =  *ptr++ ;
		ptrobj->ZMax =  *ptr++ ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
			 █▀▀▀█ ██▄ █  █    █▄ ▄█ ██▀▀▀
			 ██▀▀█ ██▀██  ██   ██▀ █ ▀▀▀▀█
			 ▀▀  ▀ ▀▀  ▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	SetAnim( U8 numobj, U16 anim )
{
	T_OBJ_3D	*obj = &ListObjet[numobj].Obj ;
	S32	x, y, z, beta ;
	U8   	body ;

	body = ListObjet[numobj].GenBody ;
	ListObjet[numobj].GenBody = 255 ;
	ListObjet[numobj].GenAnim = 255 ;
	ListObjet[numobj].FlagAnim = 0 ;

	x = obj->X ;
	y = obj->Y ;
	z = obj->Z ;
	beta = obj->Beta ;

	ObjectClear( obj ) ;
	InitBody( body, numobj ) ;
	InitAnim( anim, ANIM_REPEAT, numobj ) ;

	obj->X = x ;
	obj->Y = y ;
	obj->Z = z ;
	obj->Beta = beta ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
S32	InitAnim(	U16 gennewanim,
			U8 flag,
			U8 numobj )
{
	S32	newanim ;
	T_OBJET *ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Flags & SPRITE_3D )	return FALSE ; // -1 ;

	newanim = SearchAnim( gennewanim, numobj ) ;
	if( newanim == -1 )
	{
		newanim = SearchAnim( GEN_ANIM_RIEN, numobj ) ;
		gennewanim = GEN_ANIM_RIEN ;
	}

	// Anim a ne jamais interrompre, même par une ANIM_ALL_THEN
	// Sert ici uniquement pour la mort de Twinsen (PERSO.C)
	if( ptrobj->FlagAnim==ANIM_FINAL )	return FALSE ;

	if( (gennewanim == ptrobj->GenAnim)
	AND (ptrobj->Obj.Anim.Num != -1) )
	{
		return TRUE ;
	}

	switch( flag )
	{
		case ANIM_REPEAT:
			if( numobj!=NUM_PERSO )
			{
				ptrobj->NextGenAnim = gennewanim ;
			}
			else	ptrobj->NextGenAnim = GEN_ANIM_RIEN ;
			// y'a pas de break, c'est normal !!!
		case ANIM_THEN:
			if( ptrobj->FlagAnim!=ANIM_ALL_THEN )
			{
				ptrobj->FlagAnim = flag ;
			}
			else	return( FALSE ) ;
			break ;

		case ANIM_FINAL:
		case ANIM_ALL_THEN:
			ptrobj->FlagAnim = flag ;
			break ;

		case ANIM_TEMPO:
			if( ptrobj->FlagAnim!=ANIM_ALL_THEN )
			{
				ptrobj->FlagAnim = ANIM_REPEAT ;
			}
			else	return( FALSE ) ;
			break ;
	}

	ObjectInitAnim( &ptrobj->Obj, (void*)newanim )	;

	ptrobj->GenAnim = gennewanim ;

	ptrobj->PtrAnimAction = SearchPtrAnimAction ;

	ptrobj->WorkFlags &= ~(OK_HIT | OK_SUPER_HIT | ANIM_END
			      | FRAME_SHIELD | OK_RENVOIE
			      | LEFT_JUMP | RIGHT_JUMP
			      | WAIT_SUPER_HIT ) ;

	ptrobj->WorkFlags |= NEW_FRAME ;

	// gere actions premiere frame
	if( ptrobj->PtrAnimAction != 0 )
	{
		S32	frame = ptrobj->Obj.LastFrame ;
		ptrobj->Obj.LastFrame = -1 ;

		GereAnimAction( ptrobj, numobj ) ;

		ptrobj->Obj.LastFrame = frame ;
	}

	return TRUE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/
/* gestion anim */

void	ReajustPos( U8 col )
{
	S32	xw, yw, zw ;
	S32	coly = 0 ;

	if( col<=1 )	return ;	//  - optimisation puisque on ne
					// se sert pas de la collision 1 ici

	xw = XMap * SIZE_BRICK_XZ ; /* coin superieur gauche de la brick */
	yw = YMap * SIZE_BRICK_Y  ;
	zw = ZMap * SIZE_BRICK_XZ ;

	switch( col )	/* collision complexes */
	{
	case 6:
		if( Nxw-xw < Nzw-zw )	col = 3 ;
		else			col = 2 ;
		break ;

	case 7:
		if( Nxw-xw < Nzw-zw )	col = 5 ;
		else			col = 4 ;
		break ;

	case 10:
		if( Nxw-xw < Nzw-zw )	col = 2 ;
		else			col = 3 ;
		break ;

	case 11:
		if( Nxw-xw < Nzw-zw )	col = 4 ;
		else			col = 5 ;
		break ;

	case 8:
		if( SIZE_BRICK_XZ-(Nxw-xw) > Nzw-zw )	col = 4 ;
		else					col = 2 ;
		break ;

	case 9:
		if( SIZE_BRICK_XZ-(Nxw-xw) > Nzw-zw )	col = 5 ;
		else					col = 3 ;
		break ;

	case 12:
		if( SIZE_BRICK_XZ-(Nxw-xw) > Nzw-zw )	col = 2 ;
		else					col = 4 ;
		break ;

	case 13:
		if( SIZE_BRICK_XZ-(Nxw-xw) > Nzw-zw )	col = 3 ;
		else					col = 5 ;
		break ;
	}

	switch( col )	/* collision de base */
	{
	case 2:
		coly = yw + BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ, Nxw - xw ) ;
		break ;

	case 3:
		coly = yw + BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ, Nzw - zw ) ;
		break ;

	case 4:
		coly = yw + BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ, Nzw - zw ) ;
		break ;

	case 5:
		coly = yw + BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ, Nxw - xw ) ;
		break ;
	}

	if( coly )	// le 09/02/95
	{
		if( Nyw>coly ) // on se trouve au-dessus de la pente de la brick
		{
			if( !(APtObj->WorkFlags & ANIM_MASTER_GRAVITY) )
			{
				Nyw = coly ;
			}
		}
		else
		{
			Nyw = coly ;
		}
	}

}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	ReceptionObj()
{
	if( AnimNumObj == NUM_PERSO )
	{
		if( APtObj->WorkFlags&SKATING )	return ;

		if( (CubeMode==CUBE_EXTERIEUR)
		AND GiveTerrainCol(Nxw,Nzw)
		AND (APtObj->CarryBy==-1) )
		{
			InitSkating() ;
			StartYFalling = 0 ;
			APtObj->WorkFlags &= ~FALLING ;
			return ;
		}

		if( StartYFalling - Nyw >= 16 * SIZE_BRICK_Y )
		{ // trop haut mort directe
			InitHitExtraPof(APtObj->Obj.X,
					APtObj->Obj.Y+APtObj->YMax+1,
					APtObj->Obj.Z,
					APtObj->LifePoint ) ;

			APtObj->LifePoint = 0 ;
			InitAnim( GEN_ANIM_RECEPTION_2, ANIM_ALL_THEN, NUM_PERSO ) ;
		}
		else
		if( StartYFalling - Nyw >= 8 * SIZE_BRICK_Y )
		{ // se fait mal
			InitHitExtraPof(APtObj->Obj.X,
					APtObj->Obj.Y+APtObj->YMax+1,
					APtObj->Obj.Z,
					5 ) ;

			APtObj->LifePoint -= 5 ;
			InitAnim( GEN_ANIM_RECEPTION_2, ANIM_ALL_THEN, NUM_PERSO ) ;
		}
		else
		if( StartYFalling - Nyw > 1 )
		{ // reception normale
			InitAnim( GEN_ANIM_RECEPTION, ANIM_ALL_THEN, NUM_PERSO ) ;
		}
		else
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
		}

		StartYFalling = 0 ;
	}
	else
	{
		InitAnim( GEN_ANIM_RECEPTION, ANIM_ALL_THEN, AnimNumObj ) ;
	}
	APtObj->WorkFlags &= ~FALLING ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void    DoCornerReajust( S32 nx, S32 ny, S32 nz )
{
	S32	recalx = FALSE ;
	S32	recalz = FALSE ;
	U8	orgcol  ;

	orgcol = WorldColBrick( Nxw, Nyw, Nzw ) ;

	Nxw += nx ;
	Nyw += ny ;
	Nzw += nz ;

	if (( Nxw >= 0 ) AND ( Nzw >= 0 )
	AND ( Nxw <= 64*SIZE_BRICK_XZ-1 ) AND ( Nzw <= 64*SIZE_BRICK_XZ-1 ))
	{
//		if( orgcol!=1 )	// fait dans ReajustPos
		{
			ReajustPos( orgcol ) ;
		}

		if( WorldColBrick( Nxw, Nyw, Nzw ) == 1 )
		{
			Col1 = TRUE ;

			// choix de la méthode de recalage pour faire glisser l'Objet

			if( WorldColBrick( Nxw, Nyw, OldZ+nz ) == 1 )
			{
				if( WorldColBrick( OldX+nx, Nyw, Nzw ) != 1 )
				{
					recalx = TRUE ;
				}
				else
				{
					if( WorldColBrick( OldX+nx, Nyw, OldZ+nz ) != 1 )
					{
						recalx = TRUE ;
						recalz = TRUE ;
					}
				}
			}
			else
			{
				if( WorldColBrick( OldX+nx, Nyw, Nzw ) != 1 )
				{
					if( WorldColBrick( OldX+nx, Nyw, OldZ+nz ) == 1 )
					{
						recalx = TRUE ;
						recalz = TRUE ;
					}
				}
				else
				{
					recalz = TRUE ;
				}
			}

			// fait glisser l'Objet en le recalant en X ou/et en Z suivant
			// le cas. On le recale désormais par rapport à Nxw,Nzw et non
			// plus par rapport à OldX,OldZ pour le cas d'un Body ayant une
			// demi ZV plus grande qu'un pseudo

			if( recalx )
			{
				// Recale l'Objet contre le mur en X
				if( OldX>Nxw )
				{
//					SaveNxw = (OldX/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->XMin ;
					SaveNxw = (Nxw/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->XMin ;
				}
				else
				{
//					SaveNxw = (OldX/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->XMax - 1 ;
					SaveNxw = (Nxw/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->XMax - 1 ;
				}
			}

			if( recalz )
			{
				// Recale l'Objet contre le mur en Z
				if( OldZ>Nzw )
				{
//					SaveNzw = (OldZ/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->ZMin ;
					SaveNzw = (Nzw/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->ZMin ;
				}
				else
				{
//					SaveNzw = (OldZ/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->ZMax - 1 ;
					SaveNzw = (Nzw/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->ZMax - 1 ;
				}
			}
		}
	}

	Nxw = SaveNxw ;
	Nyw = SaveNyw ;
	Nzw = SaveNzw ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
// Pour Twinsen, on reajuste par rapport à OldX,OldY,OldZ car on est sûr que
// sa demi-ZV ne dépasse jamais la taille d'un pseudo
S32    DoCornerReajustTwinsen( S32 nx, S32 ny, S32 nz )
{
	S32	validepos = TRUE ;
	U8	orgcol  ;

	orgcol = WorldColBrick( Nxw, Nyw, Nzw ) ;

	Nxw += nx ;
	Nyw += ny ;
	Nzw += nz ;

	if (( Nxw >= 0 ) AND ( Nzw >= 0 )
	AND ( Nxw <= 64*SIZE_BRICK_XZ-1 ) AND ( Nzw <= 64*SIZE_BRICK_XZ-1 ))
	{
		ReajustPos( orgcol ) ;

		if( WorldColBrickFull( Nxw, Nyw, Nzw, APtObj->YMax ) == 1 )
		{
			Col1 = TRUE ;

			if( WorldColBrickFull( Nxw, Nyw, OldZ+nz, APtObj->YMax  ) == 1 )
			{
				if( WorldColBrickFull( OldX+nx, Nyw, Nzw, APtObj->YMax ) != 1 )
				{
					// Recale Twinsen contre le mur en X
					if( OldX>Nxw )
					{
						SaveNxw = (OldX/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->XMin ;
					}
					else
					{
						SaveNxw = (OldX/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->XMax - 1 ;
					}
				}
				else
				{
					if( WorldColBrickFull( OldX+nx, Nyw, OldZ+nz, APtObj->YMax ) != 1 )
					{
						// Recale Twinsen contre le mur en X
						if( OldX>Nxw )
						{
							SaveNxw = (OldX/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->XMin ;
						}
						else
						{
							SaveNxw = (OldX/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->XMax - 1 ;
						}

						// Recale Twinsen contre le mur en Z
						if( OldZ>Nzw )
						{
							SaveNzw = (OldZ/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->ZMin ;
						}
						else
						{
							SaveNzw = (OldZ/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->ZMax - 1 ;
						}
					}
					else	validepos = FALSE ;
				}
			}
			else
			{
				// Recale Twinsen contre le mur en Z
				if( OldZ>Nzw )
				{
					SaveNzw = (OldZ/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->ZMin ;
				}
				else
				{
					SaveNzw = (OldZ/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->ZMax - 1 ;
				}
			}
		}
	}

	Nxw = SaveNxw ;
	Nyw = SaveNyw ;
	Nzw = SaveNzw ;

	return validepos ;
}

/*
void    DoCornerReajustTwinsen( S32 nx, S32 ny, S32 nz )
{
	S32	recalx = FALSE ;
	S32	recalz = FALSE ;
	U8	orgcol  ;

	orgcol = WorldColBrick( Nxw, Nyw, Nzw ) ;

	Nxw += nx ;
	Nyw += ny ;
	Nzw += nz ;

	if (( Nxw >= 0 ) AND ( Nzw >= 0 )
	AND ( Nxw <= 64*SIZE_BRICK_XZ-1 ) AND ( Nzw <= 64*SIZE_BRICK_XZ-1 ))
	{
		ReajustPos( orgcol ) ;

		if( WorldColBrickFull( Nxw, Nyw, Nzw, APtObj->YMax ) == 1 )
		{
			Col1 = TRUE ;

			// choix de la méthode de recalage pour faire glisser l'Objet

			if( WorldColBrickFull( Nxw, Nyw, OldZ+nz, APtObj->YMax  ) == 1 )
			{
				if( WorldColBrickFull( OldX+nx, Nyw, Nzw, APtObj->YMax ) != 1 )
				{
					recalx = TRUE ;
				}
				else
				{
					if( WorldColBrickFull( OldX+nx, Nyw, OldZ+nz, APtObj->YMax ) != 1 )
					{
						recalx = TRUE ;
						recalz = TRUE ;
					}
				}
			}
			else
			{
				if( WorldColBrickFull( OldX+nx, Nyw, Nzw, APtObj->YMax ) != 1 )
				{
					if( WorldColBrickFull( OldX+nx, Nyw, OldZ+nz, APtObj->YMax ) == 1 )
					{
						recalx = TRUE ;
						recalz = TRUE ;
					}
				}
				else
				{
					recalz = TRUE ;
				}
			}

			// fait glisser l'Objet en le recalant en X ou/et en Z suivant
			// le cas. On le recale désormais par rapport à Nxw,Nzw et non
			// plus par rapport à OldX,OldZ pour le cas d'un Body ayant une
			// demi ZV plus grande qu'un pseudo

			if( recalx )
			{
				// Recale Twinsen contre le mur en X
				if( OldX>Nxw )
				{
//					SaveNxw = (OldX/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->XMin ;
					SaveNxw = (Nxw/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->XMin ;
				}
				else
				{
//					SaveNxw = (OldX/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->XMax - 1 ;
					SaveNxw = (Nxw/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->XMax - 1 ;
				}
			}

			if( recalz )
			{
				// Recale Twinsen contre le mur en Z
				if( OldZ>Nzw )
				{
//					SaveNzw = (OldZ/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->ZMin ;
					SaveNzw = (Nzw/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->ZMin ;
				}
				else
				{
// 					SaveNzw = (OldZ/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - APtObj->ZMax - 1 ;
					SaveNzw = (Nzw/SIZE_BRICK_XZ)*SIZE_BRICK_XZ - APtObj->ZMax - 1 ;
				}
			}
		}
	}

	Nxw = SaveNxw ;
	Nyw = SaveNyw ;
	Nzw = SaveNzw ;
}
*/
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
S32	CheckCarryBy( U8 numobj )
{
	S32	validepos = TRUE ;
	S16	nu = APtObj->CarryBy ;

	if( nu != -1 )
	{
		if( !(nu&CARRY_BY_DECORS) )
		{
			S32	flagrot = FALSE ;
			T_OBJET	*ptrobjc = &ListObjet[nu] ;

			if( (CubeMode==CUBE_EXTERIEUR)
			AND CalculAltitudeObjet(Nxw,Nzw,APtObj->CodeJeu>>4) > (ptrobjc->Obj.Y+ptrobjc->YMax) )
			{
				APtObj->CarryBy = -1 ;
				return TRUE ;
			}

			// ATTENTION, on remet a jour l'angle et la distance
			// d'origine de l'objet qui nous porte gentillement

			if( Nxw!=APtObj->Obj.X
			OR  Nzw!=APtObj->Obj.Z )
			{
				if( APtObj->Move!=MOVE_WAGON )	// Ne peut pas être porté !!!!
								// car on se sert du champ PtrZoneRail
				{
					// Attention, on mémorise le beta de l'objet qui nous porte
					APtObj->OldBeta = (GetAngle2D( ptrobjc->Obj.X, ptrobjc->Obj.Z, Nxw, Nzw )-ptrobjc->Obj.Beta)&4095 ;

					APtObj->PtrZoneRail = (T_ZONE*)Distance2D( ptrobjc->Obj.X, ptrobjc->Obj.Z, Nxw, Nzw ) ;
				}
			}

			Nxw -= ptrobjc->OldPosX ;
			Nyw -= ptrobjc->OldPosY ;
			Nzw -= ptrobjc->OldPosZ ;

			if( !(ptrobjc->Flags&SPRITE_3D)
			    AND ptrobjc->Obj.Beta!=ptrobjc->OldBeta
			    AND abs(ptrobjc->Obj.Beta-ptrobjc->OldBeta)<=(256*MUL_ANGLE)  )	// l'objet porteur tourne
			{
				S32	dangle ;

				dangle = (ptrobjc->Obj.Beta+APtObj->OldBeta) ;

				// Est-ce plus rapide qu'un Rotate() ?
				LongRotate( 0, (S32)APtObj->PtrZoneRail, dangle ) ;

				Nxw = X0 ;
				Nzw = Z0 ;

				flagrot = TRUE ;

				APtObj->Obj.Beta += (ptrobjc->Obj.Beta-ptrobjc->OldBeta) ;
				APtObj->Obj.Beta &= 4095 ;

				if( APtObj->Move!=MOVE_MANUAL
				AND APtObj->Move!=MOVE_WAGON )
				{
//					RestartMove( &APtObj->BoundAngle.Move ) ;
					ClearRealAngle( APtObj ) ;// cf T sur Wagon en DemoSlide
				}
			}

			Nxw += ptrobjc->Obj.X ;
			Nyw += ptrobjc->Obj.Y ;
			Nzw += ptrobjc->Obj.Z ;

			if( flagrot )
			{
				// Protection pour ne pas sortir de la ZV de l'objet transporteur
				if( (Nxw+APtObj->XMax)<=(ptrobjc->Obj.X+ptrobjc->XMin) )
				{
					Nxw = ptrobjc->Obj.X+ptrobjc->XMin+1-APtObj->XMax ;
				}

				if( (Nxw+APtObj->XMin)>=(ptrobjc->Obj.X+ptrobjc->XMax) )
				{
					Nxw = ptrobjc->Obj.X+ptrobjc->XMax-1-APtObj->XMin ;
				}

				if( (Nzw+APtObj->ZMax)<=(ptrobjc->Obj.Z+ptrobjc->ZMin) )
				{
					Nzw = ptrobjc->Obj.Z+ptrobjc->ZMin+1-APtObj->ZMax ;
				}

				if( (Nzw+APtObj->ZMin)>=(ptrobjc->Obj.Z+ptrobjc->ZMax) )
				{
					Nzw = ptrobjc->Obj.Z+ptrobjc->ZMax-1-APtObj->ZMin ;
				}

				if( ptrobjc->Flags&POS_INVALIDE )// POS_INVALID
				{
					validepos = FALSE ;
				}
			}
			else if( !CheckZvOnZv( numobj, (U8)nu ) )
			{
				APtObj->CarryBy = -1 ;
			}
			else	if( ptrobjc->Flags&POS_INVALIDE )// POS_INVALID
			{
				validepos = FALSE ;
			}
		}
		else
		{
			T_DECORS *ptrd = &ListDecors[nu&(~CARRY_BY_DECORS)] ;

			if( !TestZVDecorsZV( ptrd,
					     Nxw+APtObj->XMin, Nyw+APtObj->YMin-1, Nzw+APtObj->ZMin,
					     Nxw+APtObj->XMax, Nyw+APtObj->YMax-1, Nzw+APtObj->ZMax ) )
			{
				APtObj->CarryBy = -1 ;
			}
		}
	}

	return validepos ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

S32	GereCodesJeu( U8 numobj )
{
	S32	validepos = TRUE ;

	// ATTENTION : le flag OPT_CODES_JEU lorsqu'il est
	// positionne ne gere plus les codes jeux

	if( (APtObj->Flags & CHECK_CODE_JEU)
	AND ( (numobj!=NUM_PERSO)
	      OR !(PtrComportement->Flags&CF_WALK_ON_CODES_JEU)
	      OR (APtObj->GenAnim != GEN_ANIM_MARCHE) ) )
	{
		if( APtObj->WorkFlags&DONT_PICK_CODE_JEU )
		{
			// Magouille Tapis Roulant
			CodeJeu = APtObj->CodeJeu ;
		}
		else
		{
			CodeJeu = PtrWorldCodeBrick(Nxw, Nyw-1, Nzw ) ;

			if( (APtObj->Flags&SPRITE_3D)
			AND !(CodeJeu>>4)
			AND (APtObj->CodeJeu>>4) )
			{
				S32	oldcodejeu = APtObj->CodeJeu>>4 ;

				if( (PtrWorldCodeBrick( Nxw+APtObj->XMin, Nyw-1, Nzw+APtObj->ZMin )>>4)!=oldcodejeu
				AND (PtrWorldCodeBrick( Nxw+APtObj->XMin, Nyw-1, Nzw+APtObj->ZMax )>>4)!=oldcodejeu
				AND (PtrWorldCodeBrick( Nxw+APtObj->XMax, Nyw-1, Nzw+APtObj->ZMin )>>4)!=oldcodejeu
				AND (PtrWorldCodeBrick( Nxw+APtObj->XMax, Nyw-1, Nzw+APtObj->ZMax )>>4)!=oldcodejeu )
				// si aucun coin == old code jeu
				{
					APtObj->CodeJeu = CodeJeu ;
				}
				else
				{
					// on met à jour seulement le sample
					APtObj->CodeJeu &= 0xF0	;	// ecrase ancien sample
					APtObj->CodeJeu |= (U8)(CodeJeu&0x0F) ;
				}
			}
			else
			{
				APtObj->CodeJeu = CodeJeu ;
			}
		}

		APtObj->Flags &= ~(OBJ_IN_WATER) ;

		switch( APtObj->CodeJeu>>4 )
		{
			case CJ_LAVE:	// Rajouter un effet ?
			case CJ_GAZ:	// Rajouter un effet ?
			case CJ_WATER: 	// eau
			case CJ_ANIMATED_GAZ:
			case CJ_ANIMATED_WATER:
			case CJ_ANIMATED_LAVE:
				switch( numobj )
				{
					case NUM_PERSO:
						validepos = FALSE ;

						if( !FlagWater )
						{
							switch( APtObj->CodeJeu>>4 )
							{
								case CJ_LAVE:
								case CJ_ANIMATED_LAVE:
									InitBody( GEN_BODY_FEU, NUM_PERSO ) ;
									InitAnim( GEN_ANIM_FEU, ANIM_FINAL, NUM_PERSO ) ;
									break ;

								case CJ_GAZ:
								case CJ_ANIMATED_GAZ:
									InitAnim( GEN_ANIM_GAZ, ANIM_FINAL, NUM_PERSO ) ;
									break ;

								default:
									if( ModeLabyrinthe )
									{
										InitBody( GEN_BODY_LABYRINTHE, NUM_PERSO ) ;
										InitAnim( GEN_ANIM_LABYRINTHE, ANIM_FINAL, NUM_PERSO ) ;
									}
									else	InitAnim( GEN_ANIM_NOYADE, ANIM_FINAL, NUM_PERSO ) ;
							}

							ClearRealAngle( APtObj ) ;

							// init clipping Y noyade
							if( CubeMode==CUBE_INTERIEUR )
							{
								LongProjectPoint( Nxw, Nyw, Nzw ) ;
								FlagWater = Yp ;
							}
							else
							{
								APtObj->Flags |= OBJ_IN_WATER ;
								FlagWater = TRUE ;
							}
						}
						else if( CubeMode==CUBE_EXTERIEUR )
						{
							APtObj->Flags |= OBJ_IN_WATER ;
						}

						APtObj->Move = NO_MOVE ;
						APtObj->LifePoint = -1 ;
						APtObj->Flags |= NO_SHADOW ;
						break ;

					case NUM_PINGOUIN:
						ExplodeMecaPingouin() ;
						break ;

					default:// tout objet meurt dans l'eau
						HQ_3D_MixSample( SAMPLE_DEAD, 0x1000, 2000, 1,
								Nxw, Nyw, Nzw ) ;
						// 'explosion' de l'objet

						if( (APtObj->OptionFlags & EXTRA_MASK)
						AND !(APtObj->OptionFlags & EXTRA_GIVE_NOTHING) )
						{
							GiveExtraBonus( APtObj ) ;
						}
						APtObj->LifePoint = 0 ;
				}
				break ;

			case CJ_FOOT_WATER:
				if( Nyw<-10 )
				{
					APtObj->Flags |= OBJ_IN_WATER ;
				}
				break ;

			case CJ_ELECTRIC:	// mort (rails en montee par ex.)
				// correction bug rail penchée touchée même sur charriot
				if( APtObj->CarryBy != -1 ) break ;

				if( numobj == NUM_PERSO )
				{
					validepos = FALSE ;

					if( APtObj->GenAnim==GEN_ANIM_CHOC
					OR  APtObj->GenAnim==GEN_ANIM_CHOC2
					OR  APtObj->GenAnim==GEN_ANIM_SAUTE
					OR  APtObj->GenAnim==GEN_ANIM_SAUTE_DROIT
					OR  APtObj->GenAnim==GEN_ANIM_SAUTE_GAUCHE )
						break ;

					HQ_3D_MixSample( SAMPLE_DEAD, 0x1000, 2000, 1,
							Nxw, Nyw, Nzw ) ;

					HitObj( numobj, numobj, 1, APtObj->Obj.Beta ) ;
				}
				else	// tout objet
				{
					HQ_3D_MixSample( SAMPLE_DEAD, 0x1000, 2000, 1,
							Nxw, Nyw, Nzw ) ;
					// 'explosion' de l'objet
					HitObj( numobj, numobj, 1, APtObj->Obj.Beta ) ;
				}
				break ;

			case CJ_ESCALATOR_OUEST:	// Tapis roulant OUEST
				if( !IsSamplePlaying(SAMPLE_ESCALATOR) )
				{
					HQ_MixSample( 	SAMPLE_ESCALATOR,
							0x1000, 0, 1,
							64, VOLUME_ESCALATOR ) ;
				}

				Nxw -= StepShifting ;

				// ATTENTION: Le Beta des sprites est forcé !!!
				if( APtObj->Flags&SPRITE_3D )	APtObj->Obj.Beta = 3072 ;
				break ;

			case CJ_ESCALATOR_EST:	// Tapis roulant EST
				if( !IsSamplePlaying(SAMPLE_ESCALATOR) )
				{
					HQ_MixSample( 	SAMPLE_ESCALATOR,
							0x1000, 0, 1,
							64, VOLUME_ESCALATOR ) ;
				}

				Nxw += StepShifting ;

				// ATTENTION: Le Beta des sprites est forcé !!!
				if( APtObj->Flags&SPRITE_3D )	APtObj->Obj.Beta = 1024 ;
				break ;

			case CJ_ESCALATOR_NORD:	// Tapis roulant NORD
				if( !IsSamplePlaying(SAMPLE_ESCALATOR) )
				{
					HQ_MixSample( 	SAMPLE_ESCALATOR,
							0x1000, 0, 1,
							64, VOLUME_ESCALATOR ) ;
				}

				Nzw -= StepShifting ;

				// ATTENTION: Le Beta des sprites est forcé !!!
				if( APtObj->Flags&SPRITE_3D )	APtObj->Obj.Beta = 2048 ;
				break ;

			case CJ_ESCALATOR_SUD:	// Tapis roulant SUD
				if( !IsSamplePlaying(SAMPLE_ESCALATOR) )
				{
					HQ_MixSample( 	SAMPLE_ESCALATOR,
							0x1000, 0, 1,
							64, VOLUME_ESCALATOR ) ;
				}

				Nzw += StepShifting ;

				// ATTENTION: Le Beta des sprites est forcé !!!
				if( APtObj->Flags&SPRITE_3D )	APtObj->Obj.Beta = 0 ;
				break ;

			case CJ_POS_INVALID:	// Position non valide pour la mort
				// si on est dans un penché sur un objet CARRIER
				if( numobj == NUM_PERSO
				AND APtObj->CarryBy==-1 )
				{
					validepos = FALSE ;
				}
				break ;
		}
	}
	else	APtObj->CodeJeu = 0 ;

	return validepos ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	GereObjAnim( U8 numobj )
{
	S32	numanim ;

	if( APtObj->Obj.Anim.Num != -1 )
	{
		S32 flag = ObjectSetInterDep( &APtObj->Obj )	;

		if( APtObj->Obj.Master&1 )
			APtObj->WorkFlags |= ANIM_MASTER_ROT ;
		else	APtObj->WorkFlags &= ~ANIM_MASTER_ROT ;

		if( APtObj->Obj.Master&2 )
			APtObj->WorkFlags |= ANIM_MASTER_GRAVITY ;
		else	APtObj->WorkFlags &= ~ANIM_MASTER_GRAVITY ;

		Nxw = APtObj->Obj.X ;
		Nyw = APtObj->Obj.Y ;
		Nzw = APtObj->Obj.Z ;

		APtObj->Obj.X = OldX;
		APtObj->Obj.Y = OldY;
		APtObj->Obj.Z = OldZ;

		APtObj->WorkFlags &= ~(ANIM_END+NEW_FRAME) ;

		// Grosse Rustine pour les echelles
		if( (numobj==NUM_PERSO)
		AND PtrZoneClimb
		AND (APtObj->GenAnim==GEN_ANIM_ECHELLE)
		AND (Nyw>APtObj->Info3) )
		{
			// Info3: Haut de la zone echelle
			Nyw = APtObj->Info3 ;
		}

		if( flag & FLAG_FRAME )// Nouvelle frame
		{
/*-----------------------------------------------------------------*/
/* si nouvelle frame */
			APtObj->WorkFlags &= ~(FRAME_SHIELD) ;
			APtObj->WorkFlags |= NEW_FRAME ;

/*-----------------------------------------------------------------*/
/* gestion des actions déclenchées par des anims/frames */

			if( APtObj->PtrAnimAction != 0 )
			{
				GereAnimAction( APtObj, numobj ) ;
			}

/*-----------------------------------------------------------------*/
/* frame suivante test fin anim */

			if ( flag & FLAG_LAST_FRAME )
			{
				APtObj->WorkFlags &= ~(OK_HIT | OK_SUPER_HIT
						      | OK_RENVOIE
						      | LEFT_JUMP | RIGHT_JUMP
						      | WAIT_SUPER_HIT ) ;

				if( APtObj->FlagAnim != ANIM_REPEAT )
				{
					Jumping = FALSE ;

					APtObj->GenAnim = APtObj->NextGenAnim		;
					numanim = SearchAnim( APtObj->GenAnim, numobj ) ;
					if ( numanim ==-1 )
					{
						APtObj->GenAnim = GEN_ANIM_RIEN ;
						numanim = SearchAnim( GEN_ANIM_RIEN, numobj ) ;
					}
					ObjectInitAnim( &APtObj->Obj, (void*)numanim ) ;

					APtObj->PtrAnimAction = SearchPtrAnimAction ;

					APtObj->FlagAnim = ANIM_REPEAT ;
					APtObj->HitForce = 0 ;
				}

				// gere actions premiere frame
				if( APtObj->PtrAnimAction != 0 )
				{
					S32	frame = APtObj->Obj.LastFrame ;
					APtObj->Obj.LastFrame = -1 ;

					GereAnimAction( APtObj, numobj ) ;

					APtObj->Obj.LastFrame = frame ;
				}

				APtObj->WorkFlags |= ANIM_END ;
			}
/*-----------------------------------------------------------------*/
		}/* new frame */
	}/* if anim != -1 */
	else
	{
		// Au cas ou ce soit un objet avec body mais sans anim,
		// comme par exemple (mais vraiment un exemple au hasard,
		// hein) les WAGONS !!!!! (ca faisait un petit bug)
		Nxw = APtObj->Obj.X ;
		Nyw = APtObj->Obj.Y ;
		Nzw = APtObj->Obj.Z ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
static void	DoAnimSprite3D()
{
	S32	flag, n	;

	if( APtObj->HitForce != 0 )	APtObj->WorkFlags |= OK_HIT	;

	/* gestion anim pour sprite */

	Nxw = APtObj->Obj.X ;
	Nyw = APtObj->Obj.Y ;
	Nzw = APtObj->Obj.Z ;

	if( !(APtObj->WorkFlags & FALLING) )
	{
		if( APtObj->SRot != 0 )	/* vitesse deplacement */
		{
			n = GetDeltaMove( &APtObj->BoundAngle.Move ) ;

			Rotate( n,0, APtObj->Obj.Alpha ) ; /* alpha */
			Nyw = APtObj->Obj.Y - Z0 ;
			Rotate( 0, X0, APtObj->Obj.Beta ) ;
			Nxw = APtObj->Obj.X + X0 ;
			Nzw = APtObj->Obj.Z + Z0 ;

			if( APtObj->WorkFlags & AUTO_STOP_DOOR )
			{
				// c'est obligatoirement un sprite_clip
				if( APtObj->DoorWidth )
				{
				// distance ouverture max
					if( Distance2D( Nxw, Nzw,
							APtObj->Obj.LastAnimStepX,
							APtObj->Obj.LastAnimStepZ )
						    >= APtObj->DoorWidth )
					{
						// recalage violent
						switch( APtObj->Obj.Beta )
						{
						case (768*MUL_ANGLE): // left
							Nxw = APtObj->Obj.LastAnimStepX - APtObj->DoorWidth ;
							break ;
						case (256*MUL_ANGLE): // right
							Nxw = APtObj->Obj.LastAnimStepX + APtObj->DoorWidth ;
							break ;
						case (512*MUL_ANGLE): // up
							Nzw = APtObj->Obj.LastAnimStepZ - APtObj->DoorWidth ;
							break ;
						case (0*MUL_ANGLE): // down
							Nzw = APtObj->Obj.LastAnimStepZ + APtObj->DoorWidth ;
							break ;
						}
						APtObj->WorkFlags &= ~AUTO_STOP_DOOR ;
						APtObj->SRot = 0 ;
					}
				} //	DoorWidth
				else
				{ // 	je me ferme
					flag = FALSE ;

					switch( APtObj->Obj.Beta )
					{
					case (768*MUL_ANGLE): // left
						if( Nxw >= APtObj->Obj.LastAnimStepX ) flag = TRUE ;
						break ;
					case (256*MUL_ANGLE): // right
						if( Nxw <= APtObj->Obj.LastAnimStepX ) flag = TRUE ;
						break ;
					case (512*MUL_ANGLE): // up
						if( Nzw >= APtObj->Obj.LastAnimStepZ ) flag = TRUE ;
						break ;
					case (0*MUL_ANGLE): // down
						if( Nzw <= APtObj->Obj.LastAnimStepZ ) flag = TRUE ;
						break ;
					}

					// attention si porte decalée
					if( flag )
					{
						// position d'origine

						Nxw = APtObj->Obj.LastAnimStepX ;
						Nyw = APtObj->Obj.LastAnimStepY ;
						Nzw = APtObj->Obj.LastAnimStepZ ;

						APtObj->WorkFlags &= ~AUTO_STOP_DOOR ;
						APtObj->SRot = 0 ;
					}
				}
			} // 	auto stop door
		} // 	SRot != 0

		if( APtObj->Flags & PUSHABLE )
		{
			Nxw += APtObj->Coord.Push.PushX ;
			Nyw += APtObj->Coord.Push.PushY ;
			Nzw += APtObj->Coord.Push.PushZ ;

			APtObj->Coord.Push.PushX = 0 ;
			APtObj->Coord.Push.PushY = 0 ;
			APtObj->Coord.Push.PushZ = 0 ;
		}
	} //	!Falling

	// Gestion des Animations 3DS
	//
	// ptrobj->Coord.A3DS.Num : Num Anim 3DS dans ListAnim3DS
	// ptrobj->Coord.A3DS.Deb : Frame Start (dans le .HQR)
	// ptrobj->Coord.A3DS.Fin : Frame End   (dans le .HQR)
	// ptrobj->SizeSHit : NbFps
	// ptrobj->PtrFile3D: (U32)TimerRefHR de début d'anim

	if( (APtObj->Flags & ANIM_3DS)
	AND (APtObj->SizeSHit>0) )
	{
		S32	newsprite ;

		if( APtObj->Coord.A3DS.Fin>=APtObj->Coord.A3DS.Deb )
		{
			// on joue l'anim en avant
			newsprite = BoundRegleTrois(
					APtObj->Coord.A3DS.Deb,	// start
					APtObj->Coord.A3DS.Fin+1,// end
					((APtObj->Coord.A3DS.Fin-APtObj->Coord.A3DS.Deb+1)*1000)/APtObj->SizeSHit,	// TotalTime
					TimerRefHR-(U32)APtObj->PtrFile3D );

			if( newsprite>APtObj->Coord.A3DS.Fin )
			{
				newsprite = APtObj->Coord.A3DS.Deb ;
				APtObj->PtrFile3D = (U8*)TimerRefHR ;
			}
		}
		else
		{
			// on joue l'anim en arriere
			newsprite = BoundRegleTrois(
					APtObj->Coord.A3DS.Deb,	// start
					APtObj->Coord.A3DS.Fin-1,// end
					((APtObj->Coord.A3DS.Deb-APtObj->Coord.A3DS.Fin+1)*1000)/APtObj->SizeSHit,	// TotalTime
					TimerRefHR-(U32)APtObj->PtrFile3D );

			if( newsprite<APtObj->Coord.A3DS.Fin )
			{
				newsprite = APtObj->Coord.A3DS.Deb ;
				APtObj->PtrFile3D = (U8*)TimerRefHR ;
			}
		}

		if( newsprite!=APtObj->Sprite )
		{
			InitSprite( newsprite, APtObj ) ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	DoAnim( U8 numobj  )
{
	T_OBJET	*ptrobj ;
	U8	col ;
	S32	validepos = TRUE ;

	APtObj = ptrobj = &ListObjet[(AnimNumObj = numobj)] ;

	if( APtObj->Obj.Body.Num == -1 )	return ;

	OldX = APtObj->OldPosX ;
	OldY = APtObj->OldPosY ;
	OldZ = APtObj->OldPosZ ;

	if( numobj==NUM_PERSO )
	{
		LastValidePos = ValidePos ;
		ValidePos     = TRUE	;	// considère par défaut qu'on
	}
	else	validepos = FALSE ;

/*──────────────────────────────────────────────────────────────────────────*/
/* gestion DEP OBJET SPRITE */

	if( APtObj->Flags & SPRITE_3D )	DoAnimSprite3D()	;

/*──────────────────────────────────────────────────────────────────────────*/
/* gestion anim/DEP OBJET 3D */

	else	/* pas SPRITE_3D donc obj articulés (Avez Vous Donc Une Ame? ;)*/
		GereObjAnim( numobj ) ;

/*──────────────────────────────────────────────────────────────────────────*/
// Gestion des WAGONS

	if( APtObj->Move==MOVE_WAGON )	DoAnimWagon()	;

/*──────────────────────────────────────────────────────────────────────────*/
// pour tous obj/sprite 3d

	// je suis porte par un CARRIER / par quelqu'un ?
	if( !CheckCarryBy( numobj ) )
	{
		validepos = FALSE ;
	}

/*──────────────────────────────────────────────────────────────────────────*/
// je tombe

	if( APtObj->WorkFlags & FALLING )
	{
		/* gestion chute avec proportion sur y */
		Nxw = OldX ;
		Nyw = OldY + StepFalling ;
		Nzw = OldZ ;

		validepos = FALSE ;
	}

/*──────────────────────────────────────────────────────────────────────────*/
/* test CodeJeu Water, Electric And Roller  */


//------------- Gestion des CodeJeu

	if( !GereCodesJeu( numobj ) )	validepos = FALSE ;

/*──────────────────────────────────────────────────────────────────────────*/
/* test/reajuste pos */
// ?	APtObj->ObjCol = -1 ;

	if( APtObj->Flags & CHECK_BRICK_COL )
	{
		S32	memox, memoy, memoz ;

		YMap = 0 ;

		/* reajuste le nyw (nouvelle pos) par rapport à la col en cours	*/

		if( (col = WorldColBrick( OldX, OldY, OldZ )) != 0 )
		{
			if( col == 1 )
			{
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
				if( !(APtObj->Flags&CHECK_ONLY_FLOOR) )
				{
					Message( "currentpos dans col 1 ?", FALSE ) 	;
					GraphPrintf( TRUE, 0, 0, "NumObj:%d", numobj )		;
				}
#endif

				Nyw = (Nyw/SIZE_BRICK_Y)*SIZE_BRICK_Y+SIZE_BRICK_Y ;

				APtObj->Obj.Y = Nyw ; // essai rejette perso
			}
			else
			{
				if( APtObj->Move!=MOVE_WAGON OR !APtObj->Info )
					ReajustPos( col ) ;
			}
		}

		if( APtObj->Flags & CHECK_OBJ_COL )
		{
			CheckObjCol( numobj ) ;
		}

/*		if( numobj==NUM_PERSO )
		{
			CheckDartCol( NUM_PERSO ) ;
		}
*/
		//  - Optimisation en inversant l'ordre des 2 tests
		if( APtObj->WorkFlags&FALLING )
		{
			if( APtObj->CarryBy != -1 )
			{
				ReceptionObj() ;
			}
		}

// ■■■■■■■■■ 4 coins test la col de la nouvelle pos + size zv

		memox = SaveNxw = Nxw 	;
		memoy = SaveNyw = Nyw 	;
		memoz = SaveNzw = Nzw 	;

		Col1 = 0 	;

		if( (AnimNumObj == NUM_PERSO) AND ((ptrobj->Flags&COL_BASSE) == 0) )
		{
			if( !DoCornerReajustTwinsen( APtObj->XMin, APtObj->YMin, APtObj->ZMin) )	validepos = FALSE ;
			if( !DoCornerReajustTwinsen( APtObj->XMax, APtObj->YMin, APtObj->ZMin) )	validepos = FALSE ;
			if( !DoCornerReajustTwinsen( APtObj->XMax, APtObj->YMin, APtObj->ZMax) )	validepos = FALSE ;
			if( !DoCornerReajustTwinsen( APtObj->XMin, APtObj->YMin, APtObj->ZMax) )	validepos = FALSE ;

//			col = WorldColBrickFull( Nxw, Nyw, Nzw, APtObj->YMax ) ;
		}
		else
		{
			// Pour que des objets ayant une ZV de plus d'un
			// pseudo en X ou en Z puissent monter dans les
			// penches
			if( !(APtObj->Flags&CHECK_ONLY_FLOOR) )
			{
				DoCornerReajust( APtObj->XMin, APtObj->YMin, APtObj->ZMin) ;
				DoCornerReajust( APtObj->XMax, APtObj->YMin, APtObj->ZMin) ;
				DoCornerReajust( APtObj->XMax, APtObj->YMin, APtObj->ZMax) ;
				DoCornerReajust( APtObj->XMin, APtObj->YMin, APtObj->ZMax) ;
			}

//			col = WorldColBrick( Nxw, Nyw, Nzw ) ;
		}

		//************** ATTENTION: test de blindage collis avec objs
		if( memox!=Nxw
		OR  memoy!=Nyw
		OR  memoz!=Nzw )
		{
			// houullla !!!!!!!!!
			// on s'est fait recaler par le decors, vérifie qu'on
			// n'a pas été éjecter dans un objet sce

			if( IsObjCol( numobj ) )
			{
				// tant pis, on restore l'ancienne pos, et toc!
				Nxw = OldX ;
				Nyw = OldY ;
				Nzw = OldZ ;
			}
		}
		//**************

		col = WorldColBrick( Nxw, Nyw, Nzw ) ;

// ■■■■■■■■■	test la col de la nouvelle pos reelle


		APtObj->Col = col ;

		if( col != 0 )
		{
			if( col == 1 )
			{
				if( APtObj->WorkFlags & FALLING )
				{
					ReceptionObj() ;
					Nyw = YMap*SIZE_BRICK_Y+SIZE_BRICK_Y ;
				}
				else
				{
			// fait glisser
//■■■■■■■■		// regarde de dep X puis Z par rapport à old pos

					// WorldColBrickFull() pour Twinsen ?
					if( WorldColBrick( Nxw, Nyw, OldZ ) != 0 )
					{
						if( WorldColBrick( OldX, Nyw, Nzw ) != 0 )
						{
							// Cas particulier
							// Pour virer bug peintre dans 0_EGOUT2.SCE
							// Objet etant en ANIM_MASTER_GRAVITY, ayant le flag
							// BRICK_COL mais pas le flag OBJ_FALLABLE alors
							// que son anim boucle sur une frame ayant une
							// translation en Y (plonge)
							if( !(ptrobj->Flags&OBJ_FALLABLE) AND ptrobj->WorkFlags&ANIM_MASTER_GRAVITY )
							{
								// ReceptionObj( ) ; // Faut-il le mettre ?
								Nyw = YMap*SIZE_BRICK_Y+SIZE_BRICK_Y ;
							}
							else
							{
								validepos = FALSE ;
								goto findoanim ;	/* pos pas acceptee */
							}
						}
						else
						{
							Nxw = OldX ;
						}
					}
					else
					{
						Nzw = OldZ ;
					}
				}
			}
			else
			{
				if( APtObj->WorkFlags & FALLING )
				{
					ReceptionObj() ;
				}

// correction bug chariot dans descente twinsen touche col penchée
				if( APtObj->CarryBy == -1 )
				{
					if( APtObj->Move!=MOVE_WAGON OR !APtObj->Info )
						ReajustPos( col ) ;
				}
			}

			APtObj->WorkFlags &= ~FALLING ;
		}
		else    /* col == 0 */
		{
/* si sous les pieds != cube plein: susceptible de tomber ou reajuste y */

			if( (APtObj->Flags & OBJ_FALLABLE) AND (APtObj->CarryBy == -1) )
			{
				col = WorldColBrick( Nxw, Nyw-1, Nzw ) ;
				if( col )
				{
					if( APtObj->WorkFlags & FALLING )
					{
						ReceptionObj() ;
					}

					ReajustPos( col ) ;
				}
				else
				{
		// Nouvelle methode a remettre en place (le 30/05/95)
					validepos = FALSE ;

					if( !(APtObj->WorkFlags & ANIM_MASTER_GRAVITY) )
					{
					if( !(APtObj->WorkFlags&FALLING) )
					{
						Nyw -= DEMI_BRICK_Y ;// Magouille pour checkobjcol
						CheckObjCol(numobj) ;
						if( ptrobj->ObjCol==255 )	Nyw += DEMI_BRICK_Y ;// Magouille pour checkobjcol
					}

					if( ptrobj->ObjCol==255 )
					{
						if( APtObj->Move==MOVE_WAGON )
						{
							if( !APtObj->Info )
								APtObj->WorkFlags |= FALLING ;
						}
						else
						{
							if( APtObj->Flags & SPRITE_3D )
							{
								if( WorldColBrick( Nxw+APtObj->XMin, Nyw-1, Nzw+APtObj->ZMin )!=1
								AND WorldColBrick( Nxw+APtObj->XMin, Nyw-1, Nzw+APtObj->ZMax )!=1
								AND WorldColBrick( Nxw+APtObj->XMax, Nyw-1, Nzw+APtObj->ZMin )!=1
								AND WorldColBrick( Nxw+APtObj->XMax, Nyw-1, Nzw+APtObj->ZMax )!=1 )
								{
									APtObj->WorkFlags |= FALLING ;
								}
							}
							else	// obj 3D
							{
								if( !WorldColBrick( Nxw, Nyw-SIZE_BRICK_Y/2, Nzw ) )
								{
									if( numobj!=NUM_PERSO OR
//									!FlagClimbing )
									!FlagClimbing )
									{
										if( APtObj->GenAnim!=GEN_ANIM_ARRIMAGE )
										{
											InitAnim( GEN_ANIM_TOMBE, ANIM_TEMPO, numobj ) ;
											APtObj->WorkFlags |= FALLING ;
											PtrZoneClimb = NULL ;

											if( (numobj==NUM_PERSO)
											AND (!StartYFalling) )
											{
												StartYFalling = Nyw ;
											}
										}
									}
								}
								else
								{
									Nyw = YMap*SIZE_BRICK_Y+SIZE_BRICK_Y ;

									// Bug Plaque ejectee quand on saute
									// dessus
									CheckObjCol( numobj ) ;
								}
							}
						}
					}
					else if( APtObj->WorkFlags & FALLING )
					{
						ReceptionObj() ;
					}
					}
				}
			}
		}

		if( YMap == -1 )
		{
			APtObj->LifePoint = 0 ;	// bye bye
			validepos = FALSE ;
		}
//-------------
	}
	else	/* ! Flags & CHECK_BRICK_COL */
	{
		if( APtObj->Flags & CHECK_OBJ_COL )
		{
			CheckObjCol( numobj ) ;
		}
	}

	if( Col1 )	APtObj->Col |= 128 ;


/* protege sortie du cube */

	if( Nxw < DEMI_BRICK_XZ ) Nxw = DEMI_BRICK_XZ ;
	if( Nzw < DEMI_BRICK_XZ ) Nzw = DEMI_BRICK_XZ ;

	// mettre a jour test a 0

// Grosse Magouille Cage de la fin
	if( Nyw < 0 AND !(APtObj->Flags&OBJ_ZBUFFER) ) Nyw = 0 ;

	// a mettre en commentaire ????
	// peut-etre pour les objets sortants du cube
	if( Nxw > 64*SIZE_BRICK_XZ-DEMI_BRICK_XZ )	Nxw = 64*SIZE_BRICK_XZ-DEMI_BRICK_XZ ;
	if( Nzw > 64*SIZE_BRICK_XZ-DEMI_BRICK_XZ )	Nzw = 64*SIZE_BRICK_XZ-DEMI_BRICK_XZ ;

/* ok enregistre position */

	APtObj->Obj.X = Nxw ;
	APtObj->Obj.Y = Nyw ;
	APtObj->Obj.Z = Nzw ;

findoanim:

	if( numobj==NUM_PERSO )
	{
		S16	nu = APtObj->CarryBy ;

		if( !LastValidePos AND APtObj->LifePoint<=0 )
		{
			validepos = FALSE ;
		}

		// pour le protopack
		if( Comportement==C_PROTOPACK
		AND APtObj->GenAnim==GEN_ANIM_MARCHE )
		{
			validepos = FALSE ;
		}

		// blindage Carriers
		if(  (nu!=-1)
		AND !(nu&CARRY_BY_DECORS)
		AND  (ListObjet[nu].Flags&POS_INVALIDE) )// POS_INVALID
		{
			validepos = FALSE ;
		}

		if( validepos )
		{
#ifdef	DEBUG_TOOLS
			if( CheckKey(K_U) )	Message( "ValidePos", FALSE ) ;
#endif
			// sauve etat heros pour le prochain SaveValidePos()
			memcpy( &ValideHero, ptrobj, sizeof(T_OBJET) ) ;

			ValideHero.Obj.X = OldX ;
			ValideHero.Obj.Y = OldY ;
			ValideHero.Obj.Z = OldZ ;

			if( FirstSave )// on vient de changer de cube
			{
				SaveValidePos() ;
			}
		}
		else
		{
			ValidePos = FALSE ;

			if( LastValidePos
			AND !FirstSave		// on vient de changer de cube
			AND !FlagChgCube )	// on ne sauve pas le contexte
						// si l'autosave n'a pas encore
						// été faite
			{
				SaveValidePos() ;
			}
		}

		FirstSave = FALSE ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
			    █▄ ▄█ █▀▀▀█ █   █ █▀▀▀▀
			    ██▀ █ ██  █ ██ ▄▀ ██▀▀
			    ▀▀  ▀ ▀▀▀▀▀ ▀▀▀   ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	ManualRealAngle( T_OBJET *ptrobj )
{
	if( 	 Input & I_LEFT  )	ChangeSpeedMove( &ptrobj->BoundAngle.Move, 1024 ) ;
	else if( Input & I_RIGHT )	ChangeSpeedMove( &ptrobj->BoundAngle.Move, -1024 ) ;
	else				ChangeSpeedMove( &ptrobj->BoundAngle.Move, 0 ) ;

	if( (Jumping&1023)
	OR  (ptrobj->WorkFlags&ANIM_MASTER_ROT) )
	{
		RestartMove( &ptrobj->BoundAngle.Move ) ;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	ClearRealAngle( T_OBJET *ptrobj )
{
	InitBoundAngleMove( 	&ptrobj->BoundAngle,
				0,
				ptrobj->Obj.Beta,
				ptrobj->Obj.Beta ) ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*══════════════════════════════════════════════════════════════════════════*/
void	ThrowMecaPingouin( void )
{
	T_OBJET	*ptrobj = &ListObjet[NUM_PINGOUIN] ;

	if( PingouinActif )	return ;

	ptrobj->ObjCol = 255 ;
	ptrobj->LifePoint = MAX_LIFE_POINTS ;

	Rotate( 0, MECA_PINGOUIN_DISTANCE, ListObjet[NUM_PERSO].Obj.Beta ) ;

	ptrobj->Obj.X = ListObjet[NUM_PERSO].Obj.X + X0 ;
	ptrobj->Obj.Y = ListObjet[NUM_PERSO].Obj.Y + 1  ;
	ptrobj->Obj.Z = ListObjet[NUM_PERSO].Obj.Z + Z0 ;

	ptrobj->CodeJeu = 0 ;

	ptrobj->Obj.Beta = ListObjet[NUM_PERSO].Obj.Beta ;

	if( CheckValidObjPos( NUM_PINGOUIN, NUM_PERSO ) )
	{
/*		if( CubeMode==CUBE_EXTERIEUR )
		{
			GetShadowExt( ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
			ptrobj->Obj.Y = ShadowY+1 ;
		}
*/
		ptrobj->WorkFlags &= ~OBJ_DEAD ;
                ptrobj->CarryBy = 0 ;// pour le forcer à tomber si nécessaire

		ptrobj->GenBody = NO_BODY ;
		InitBody( GEN_BODY_NORMAL, NUM_PINGOUIN ) ;
		InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NUM_PINGOUIN ) ;

		ClearRealAngle( ptrobj ) ;

		PtrProjectPoint( ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
		RestoreTimer() ;
		*(U32*)(&ptrobj->Info) = TimerRefHR + 5 * 1000 ;

		ptrobj->Info1 = InitIncrustDisp( INCRUST_NUM,
						5,
						Xp, Yp,
						158, (NUM_PINGOUIN<<8)+2, 1 ) ;

		SaveTimer() ;
		ListVarGame[FLAG_MECA_PINGOUIN]-- ; // un de moins
		PingouinActif = TRUE ;
	}
	else
	{
		// sample bip-bip
		PingouinActif = FALSE ;

		if( !IsSamplePlaying(SAMPLE_PINGOUIN_ACTIF) )
		{
			HQ_3D_MixSample( SAMPLE_PINGOUIN_ACTIF, 0x1000, 0, 1,
					 ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
void	ExplodeMecaPingouin( void )
{
	S32	x, y, z ;
	S32	beta ;
	T_OBJET	*ptrobj = &ListObjet[NUM_PINGOUIN] ;
	U8	n ;
	U8	hitforce ;

	x = ptrobj->Obj.X ;
	y = ptrobj->Obj.Y ;
	z = ptrobj->Obj.Z ;

	beta = ptrobj->Obj.Beta ;

	if( ptrobj->Info1!=-1 )	// Num Incrust Diplay
	{
		ListIncrustDisp[ ptrobj->Info1 ].Num = -1 ;
		ptrobj->Info1 = -1 ;
	}

	ptrobj->LifePoint = 0 ;
	ptrobj->Obj.Body.Num = -1 ;
	ptrobj->ObjCol = 255 ;
	PingouinActif = FALSE ;

	DoImpact( IMPACT_PINGOUIN,
		  ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z,
		  NUM_PERSO ) ;

	ptrobj = ListObjet ;

	for( n=0; n<NbObjets; n++, ptrobj++ )
	{
		if( n!=NUM_PINGOUIN AND ptrobj->LifePoint>0 AND ptrobj->Obj.Body.Num!=-1 )
		{
			if( Distance3D( x, y, z, ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) < 2000 )
			{
				hitforce = MagicBallHitForce[MagicLevel] ;

				if( hitforce>ptrobj->Armure )
				{
					hitforce = (U8)(ptrobj->Armure+(hitforce-ptrobj->Armure)*3) ;
				}

				HitObj( NUM_PERSO, n, hitforce, beta ) ;
			}
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/* T_OBJET.	Info		Info1		Info2		Info3

FOLLOW								num obj
FOLLOW_2	etat		label1/label2	distance	num obj


		NumTrack	OffsetTrack

TRACK
TRACK_ATTACK

*/
/*══════════════════════════════════════════════════════════════════════════*/

// Valeur récupérée dans Animit pour l'anim marche du SuperJetPack
#define	HEIGHT_JETPACK	543

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	DoDir( U8 numobj  )
{
	T_OBJET	*ptrobj ;
	S32	info3 ;
	S32	angle ;

	ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Obj.Body.Num == -1 )	return ;

/*-------------------------------------------------------------------------*/
	if( ptrobj->WorkFlags & FALLING
	AND ptrobj->GenAnim!=GEN_ANIM_ARRIMAGE )
	{
		if( ptrobj->Move == MOVE_MANUAL )
		{
			ManualRealAngle( ptrobj ) ;
			LastMyJoy = (Input&I_JOY) ;
		}
		return ;
	}

	if( ptrobj->WorkFlags & SKATING )
	{
		ptrobj->Obj.Beta = GetBoundAngleMove( &ptrobj->BoundAngle )&4095 ;
	}

// a dupliquer dans les case ??? (pour eviter les tests !)
	if( !(ptrobj->Flags & SPRITE_3D) AND ptrobj->Move!=MOVE_WAGON
	AND ptrobj->Move != MOVE_MANUAL
	AND ptrobj->Move != MOVE_BUGGY_MANUAL )
	{
		if( ptrobj->WorkFlags & ANIM_MASTER_ROT )
		{
			RestartMove( &ptrobj->BoundAngle.Move ) ;
		}
		else
		{
			ptrobj->Obj.Beta = GetBoundAngleMove( &ptrobj->BoundAngle )&4095 ;
		}
	}

	switch( ptrobj->Move )
	{
/*-------------------------------------------------------------------------*/
	case MOVE_MANUAL:

	if( (Input & I_ACTION_ALWAYS)
	AND !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY)
	AND (ptrobj->FlagAnim!=ANIM_ALL_THEN) )
	{
		ActionNormal = TRUE ;
	}
	else	if( !DemoSlide )	ActionNormal = FALSE ;

	if( numobj == NUM_PERSO )	/* protection anim speciales */
	{
		switch( Comportement )
		{
		case C_SPORTIF:
		case C_SCAPH_INT_SPOR:	// Scaphandre Sportif Interieur
		case C_SCAPH_EXT_SPOR:	// Scaphandre Sportif Exterieur
			if( !Jumping
			AND ptrobj->GenAnim==GEN_ANIM_MARCHE
			AND ptrobj->Obj.LastFrame==1-1 )
			{
				Jumping	= 1024 ;
			}

			if( Input&I_ACTION_M
			AND !Pushing
			AND !FlagClimbing )
			{
				LastJoyFlag = FALSE ;

				if( Jumping&1024 )
				{
					if( ptrobj->GenAnim==GEN_ANIM_MARCHE )
					{
						// determine pied d'appel
						if( ptrobj->WorkFlags&LEFT_JUMP )
							Jumping = DO_LEFT_JUMP ;
						else if( ptrobj->WorkFlags&RIGHT_JUMP )
							Jumping = DO_RIGHT_JUMP ;
						else	Jumping = DO_NORMAL_JUMP ;
					}
				}

				if( !Jumping )
				{
					Jumping = DO_NORMAL_JUMP ;
				}

				switch( Jumping )
				{
					case DO_NORMAL_JUMP:
						InitAnim( GEN_ANIM_SAUTE, ANIM_THEN, numobj ) ;
						break ;

					case DO_LEFT_JUMP:
						InitAnim( GEN_ANIM_SAUTE_GAUCHE, ANIM_THEN, numobj ) ;
						break ;

					case DO_RIGHT_JUMP:
						InitAnim( GEN_ANIM_SAUTE_DROIT, ANIM_THEN, numobj ) ;
						break ;
				}
			}
			break ;

		case C_AGRESSIF:
			if( Input&I_ACTION_M )
			{
				LastJoyFlag = TRUE ;
				/* essai control direction pendant combat */
				ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
				ptrobj->Obj.Beta &= 4095 ;
//	Ici, il y a un bug, le IF qui suit empeche de retirer au hasard une
//	nouvelle anim. Resultat, Twinsen combat toujours avec le meme coup...
				if(  (LastInput&I_ACTION_M)
				AND( ptrobj->GenAnim != GEN_ANIM_RIEN ) )
					break ;

				switch( MyRnd(3) )
				{
				case 0:
					InitAnim( GEN_ANIM_COUP_1, ANIM_THEN, numobj ) ;
					break ;
				case 1:
					InitAnim( GEN_ANIM_COUP_2, ANIM_THEN, numobj ) ;
					break ;
				case 2:
					InitAnim( GEN_ANIM_COUP_3, ANIM_THEN, numobj ) ;
					break ;
				}
			}
			break ;

		case C_DISCRET:
			if( Input&I_ACTION_M )
			{
				InitAnim( GEN_ANIM_CACHE, ANIM_TEMPO, numobj ) ;
			}
			break ;

		// ATTENTION: laisser le 'case C_JETPACK:' juste avant le 'default:'
		case C_JETPACK:
//			if( Input&I_UP )
			{
				S32 ydec = ShadowYHero ;

				if( CubeMode==CUBE_EXTERIEUR )
				{
					ydec = ReajustPosDecors( ptrobj->Obj.X+ptrobj->XMin,
								 ptrobj->Obj.Z+ptrobj->ZMin,
								 ptrobj->Obj.X+ptrobj->XMax,
								 ptrobj->Obj.Z+ptrobj->ZMax,
								 ydec,
								 ptrobj->Obj.Y ) ;
				}

				ptrobj->WorkFlags |= FLY_JETPACK ;

				if( TimerRefHR<TimerProto )
				{
					Input |= I_UP ;
				}

				// en train de voler
				if( Input&I_UP )
				{
					if( ptrobj->GenAnim==GEN_ANIM_MARCHE
					    AND ptrobj->Obj.LastFrame+1>4-1 )
					{
						if( ydec-ShadowYHero<=HEIGHT_JETPACK
						AND !ptrobj->Col )
						{
							// tester point de derrière
							if( (ptrobj->Obj.Y-ShadowYHero) > (HEIGHT_JETPACK+SIZE_BRICK_Y/2) )
							{
								InitAnim( GEN_ANIM_ARRIMAGE, ANIM_REPEAT, numobj ) ;
							}
							else if( (ptrobj->Obj.Y-ShadowYHero) > (HEIGHT_JETPACK-SIZE_BRICK_Y/2) )
							{
								ptrobj->Obj.Y = ShadowYHero+HEIGHT_JETPACK ;
							}
						}
					}
					else if( ptrobj->GenAnim==GEN_ANIM_ARRIMAGE )
					{
						if( (ptrobj->Obj.Y-ShadowYHero) < (HEIGHT_JETPACK/*-SIZE_BRICK_Y/2*/)
						OR  ptrobj->Col )
						{
							if( !ptrobj->Col )	ptrobj->Obj.Y = ShadowYHero+HEIGHT_JETPACK ;
							InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, numobj ) ;
							ObjectSetFrame( &(ptrobj->Obj), 3 ) ;
						}
					}
				}
				else
				{
					ptrobj->WorkFlags |= CHECK_FALLING ;

					if( ptrobj->GenAnim==GEN_ANIM_ARRIMAGE )
					{
						InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
					}
				}
			}

		case C_PROTOPACK:
			if( TimerRefHR<TimerProto )
			{
				Input |= I_UP ;
			}

			// On peut pas démarrer en jetpack
			// si on est dans l'eau
			if( /*ptrobj->GenAnim!=GEN_ANIM_MARCHE
			AND*/ (ptrobj->CodeJeu>>4)==CJ_FOOT_WATER
			AND ptrobj->Obj.Y<=-300 )
			{
				Input &= ~(I_UP) ;
			}

		default:
			if(  (Input&I_ACTION_M)
			AND !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY)
			AND (ptrobj->FlagAnim!=ANIM_ALL_THEN)
			AND ActionNormal!=2 )	// Anti-rebond avec Buggy
			{
				ActionNormal = TRUE ;
			}
			break ;
		}

		// lance balle magique ou autre arme

		if( (Comportement!=C_PROTOPACK AND Comportement!=C_JETPACK)
		OR  ptrobj->GenAnim!=GEN_ANIM_MARCHE )
		{
			if( (Input & I_THROW)
			AND (PtrComportement->Flags & CF_WEAPON) )
			{
				S32	flagattack = FALSE ;

				switch( Weapon )
				{
					case FLAG_BALLE_MAGIQUE:	// balle magique
						if( MagicBallFlags&MAGIC_BALL_LACHEE )
						{
							// Si la balle est deja lancee et qu'on
							// appuie sur la touche <ALT> apres l'avoir
							// lachee, cela fait revenir la balle
							MagicBallFlags = MAGIC_BALL_RAPPELEE ;
						}
						else
						{
							if( ListVarGame[FLAG_BALLE_MAGIQUE] == 1 )
							{
								if( MagicBall == -1 )
								{
									InitAnim( GEN_ANIM_LANCE, ANIM_THEN, numobj ) ;
									MagicBallFlags = MAGIC_BALL_LANCEE ;
								}
								LastJoyFlag = TRUE ;
								flagattack  = TRUE ;
								/* control direction pendant visée */
								if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
								{
									ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
									ptrobj->Obj.Beta &= 4095 ;
								}
							}
						}
						break ;

					case FLAG_SABRE:
						if( ptrobj->GenBody != GEN_BODY_SABRE )
						{
							InitBody( GEN_BODY_SABRE, NUM_PERSO ) ;
						}

						// anim frappe rapide
						InitAnim( GEN_ANIM_SABRE, ANIM_THEN, NUM_PERSO ) ;

						LastJoyFlag = TRUE ;
						flagattack  = TRUE ;
						// control direction pendant visée
						if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
						{
							ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
							ptrobj->Obj.Beta &= 4095 ;
						}
						break ;

					case FLAG_DART:	// Flechettes
						if( ListVarGame[FLAG_DART] > 0 )
						{
							InitAnim( GEN_ANIM_DART, ANIM_THEN, numobj ) ;
							LastJoyFlag = TRUE ;
							flagattack  = TRUE ;
							/* control direction pendant visée */
							if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
							{
								ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
								ptrobj->Obj.Beta &= 4095 ;
							}
						}
						else	if( ptrobj->GenAnim==GEN_ANIM_RIEN )
						{
							// Repasse en balle magique
							if( ListVarGame[FLAG_BALLE_MAGIQUE] )
							{
								Weapon = FLAG_BALLE_MAGIQUE ;
							}
						}
						break ;

					case FLAG_CONQUE: // Lancement de coeurs
						InitAnim( GEN_ANIM_LANCE, ANIM_REPEAT, numobj ) ;

						flagattack = TRUE ;

						if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
						{
							ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
							ptrobj->Obj.Beta &= 4095 ;
						}
						break ;

					case FLAG_SARBACANE:
						// A-t-on le Sarbatron ?
						if( TabInv[FLAG_SARBACANE].IdObj3D==0 )// non
							InitAnim( GEN_ANIM_SARBACANE, ANIM_REPEAT, numobj ) ;
						else	InitAnim( GEN_ANIM_SARBATRON, ANIM_REPEAT, numobj ) ;
						flagattack = TRUE ;

						if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
						{
							ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
							ptrobj->Obj.Beta &= 4095 ;
						}
						break ;

					case FLAG_GANT:
						InitAnim( ChoiceAnimGant(), ANIM_THEN, numobj ) ;

						flagattack = TRUE ;

						if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
						{
							ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
							ptrobj->Obj.Beta &= 4095 ;
						}
						break ;

					case FLAG_PISTOLASER:
						InitAnim( GEN_ANIM_PISTOLASER, ANIM_THEN, numobj ) ;

						flagattack = TRUE ;

						if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
						{
							ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
							ptrobj->Obj.Beta &= 4095 ;
						}
						break ;

					case FLAG_SCAPHANDRE: // Rayons laser de la combinaison
						InitAnim( GEN_ANIM_LANCE, ANIM_THEN, numobj ) ;

						flagattack = TRUE ;

						if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
						{
							ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
							ptrobj->Obj.Beta &= 4095 ;
						}
						break ;
				} // fin switch

				if( !flagattack )
				{
					InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
				}
			}
			else
			{
				if( MagicBallFlags&MAGIC_BALL_LANCEE )
				{
					// On a laché la touche <ALT>, se tenir pret a
					// ramener la balle si le joueur reappuie sur la
					// touche <ALT>.
					MagicBallFlags = MAGIC_BALL_LACHEE ;
				}

				if( LastInput&I_THROW )
				{
					InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
				}

//				AnimGant = FALSE ;
			}
		}// fin test protopack
	} /* endif numperso */
	else
	{
		if(  (Input&I_ACTION_M)
		AND !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY) )
		{
			ActionNormal = TRUE ;
		}
	}

	// SuperJetPack
	if( ptrobj->GenAnim==GEN_ANIM_ARRIMAGE
	OR  ptrobj->WorkFlags&SKATING )
	{
		return ;
	}

	if( Jumping
	AND (ptrobj->GenAnim==GEN_ANIM_RIEN
	OR   ptrobj->GenAnim==GEN_ANIM_RECEPTION
	OR   ptrobj->WorkFlags&ANIM_MASTER_ROT) )
	{
		Jumping = FALSE ;
	}

	if( (Input&I_ESQUIVE)
	AND (Input&(I_LEFT|I_RIGHT)) )
	{
		// Magouille pour que les esquives interrompent les armes
		Input &= ~(I_FIRE) ;
	}

	if( !(Input&I_FIRE) OR ActionNormal )
	{
		S32	test_down ;

		if( Input&(I_UP|I_DOWN) )
		{
			if( Jumping&1023
			AND LastMyJoy&(I_UP|I_DOWN)
//			AND !PtrZoneClimb )
			AND !FlagClimbing )
			{
				// skip pour ne pas interrompre un saut
				// si le joueur n'a pas relacher Up et Down
				// avant de reappuyer dessus

				ManualRealAngle( ptrobj ) ;
				LastMyJoy  = (Input&I_JOY) ;
				LastMyFire = (Input&I_FIRE) ;
				break ;
			}
			else
			{
				LastJoyFlag = FALSE ;

				if( !(Jumping&1024) )
				{
					// Seulement si on n'est pas en amorce
					// de saut en course
					Jumping = FALSE ;
				}
			}
		}

		if( (LastJoyFlag)
		AND ( ((Input&I_JOY)  != LastMyJoy)
		OR    ((Input&I_FIRE) != LastMyFire) ) )
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
			Pushing = FALSE ;
		}

		if( (FlagClimbing)
		AND ((Input&I_JOY) != LastMyJoy) )
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
			Pushing = FALSE ;
		}

		LastJoyFlag = FALSE ;

		// Pushing contient le numero de l'objet que l'on pousse
		// Donc 1000 est une valeur impossible qui sert de flag d'init
		// anim rien
		if( Pushing==1000 )
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
			Pushing = FALSE ;
		}

		if( Input&I_UP )
		{
			test_down = FALSE ;

			if( Pushing )
			{
				InitAnim( GEN_ANIM_POUSSE, ANIM_TEMPO, numobj ) ;
				LastJoyFlag = TRUE ;
			}
			else if( FlagClimbing!=CLIMBING_UP )
			{
				if( Input&I_ESQUIVE )
				{
					if( ptrobj->FlagAnim!=ANIM_ALL_THEN )
					{
						InitAnim( GEN_ANIM_ESQUIVE_AVANT, ANIM_ALL_THEN, numobj ) ;
					}
				}
				else	InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, numobj ) ;

				// on était en train de descendre
				if( PtrZoneClimb )
				{
					FlagClimbing = CLIMBING_UP ;
					test_down = TRUE ;
				}
				LastJoyFlag = TRUE ;
			}
		}
		else	test_down = TRUE ;

		if( test_down AND (Input&I_DOWN) )
		{
			if( FlagClimbing!=CLIMBING_DOWN
			AND ptrobj->GenAnim!=GEN_ANIM_ECHDESC )	// Descente d'échelle ?
			{
				if( Input&I_ESQUIVE )
				{
					if( ptrobj->FlagAnim!=ANIM_ALL_THEN )
					{
						InitAnim( GEN_ANIM_ESQUIVE_ARRIERE, ANIM_ALL_THEN, numobj ) ;
					}
				}
				else	InitAnim( GEN_ANIM_RECULE, ANIM_REPEAT, numobj ) ;

				// on etait en train de monter
				if( PtrZoneClimb )	FlagClimbing = CLIMBING_DOWN ;
				LastJoyFlag = TRUE ;
			}
		}

		if( (Input & I_LEFT) AND !(Jumping&1023) )
		{
			LastJoyFlag = TRUE ;

			if( ptrobj->GenAnim == GEN_ANIM_RIEN )
			{
				if( Input&I_ESQUIVE )
				{
					if( ptrobj->FlagAnim!=ANIM_ALL_THEN )
					{
						InitAnim( GEN_ANIM_ESQUIVE_GAUCHE, ANIM_ALL_THEN, numobj ) ;
					}
				}
				else	InitAnim( GEN_ANIM_GAUCHE, ANIM_REPEAT, numobj ) ;
			}
			else
			{
				if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
				{
					ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
					ptrobj->Obj.Beta &= 4095 ;
				}
			}
		}
		else if( (Input & I_RIGHT) AND !(Jumping&1023) )
		{
			LastJoyFlag = TRUE ;

			if( ptrobj->GenAnim == GEN_ANIM_RIEN )
			{
				if( Input&I_ESQUIVE )
				{
					if( ptrobj->FlagAnim!=ANIM_ALL_THEN )
					{
						InitAnim( GEN_ANIM_ESQUIVE_DROITE, ANIM_ALL_THEN, numobj ) ;
					}
				}
				else	InitAnim( GEN_ANIM_DROITE, ANIM_REPEAT, numobj ) ;
			}
			else
			{
				if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
				{
					ptrobj->Obj.Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
					ptrobj->Obj.Beta &= 4095 ;
				}
			}
		}
	}

	ManualRealAngle( ptrobj ) ;
	LastInput  = Input ;
	LastMyJoy  = (Input&I_JOY) ;
	LastMyFire = (Input&I_FIRE) ;
	break ;

/*-------------------------------------------------------------------------*/
	case MOVE_BUGGY_MANUAL:
		if(  (Input&I_ACTION_M)
		AND !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY) )
		{
			ActionNormal = 2 ;// Action Buggy pour en descendre
			InitWaitNoInput( I_ACTION ) ;
		}
		else
		{
			if( (Input & I_ACTION_ALWAYS)
			AND !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY)
			AND ActionNormal!=2 )	// Anti-rebond avec Buggy
			{
				ActionNormal = TRUE ;
			}
			else	ActionNormal = FALSE ;
		}

		MoveBuggy( ptrobj ) ;
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_BUGGY:
		ListBuggy[0].LastTimer = TimerRefHR ;
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_FOLLOW:
		info3 = ptrobj->Info3 ;	/* num obj to follow */

		angle = GetAngle2D(	ptrobj->Obj.X,
					ptrobj->Obj.Z,
					ListObjet[info3].Obj.X,
					ListObjet[info3].Obj.Z ) ;

		angle &= 4095 ;

		if( ptrobj->Flags&SPRITE_3D
		OR  !ptrobj->SRot )	// Pour qu'un objet s'oriente
					// violement vers un autre
		{
			ptrobj->Obj.Beta = angle ;
		}
		else
		{
			if( !(ptrobj->WorkFlags&TRACK_MASTER_ROT) )// cf TM_ANGLE_RND
			{
				ChangeSpeedBoundAngleMove( &ptrobj->BoundAngle,
							   ptrobj->SRot,
							   angle ) ;
			}
		}
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_PINGOUIN:
		// pour etre sur de ne pas etre interrompu par le moteur
		InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NUM_PINGOUIN ) ;

		if( !(ptrobj->WorkFlags & ANIM_MASTER_GRAVITY) )
		{
			if( ptrobj->Col & 128	 // collision decors
			OR  ptrobj->ObjCol!=255 )// collision objet
			{
				ptrobj->Col = 0 ;
				ExplodeMecaPingouin() ;
			}

			if( TimerRefHR>=ptrobj->Info )
			{
				ExplodeMecaPingouin() ;
			}
		}
		break ;

/*-------------------------------------------------------------------------*/
/*	case MOVE_FOLLOW_2:
		break ;

	case MOVE_TRACK_ATTACK:
		break ;
*/
/*-------------------------------------------------------------------------*/
	case MOVE_TRACK:
		if( ptrobj->OffsetTrack == -1 )	ptrobj->OffsetTrack = 0 ;
		break ;


/*-------------------------------------------------------------------------*/
	case MOVE_SAME_XZ:
		info3 = ptrobj->Info3 ;	/* num obj to follow */
		ptrobj->Obj.X = ListObjet[info3].Obj.X ;
		ptrobj->Obj.Z = ListObjet[info3].Obj.Z ;
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_SAME_XZ_BETA:
		info3 = ptrobj->Info3 ;	/* num obj to follow */
		ptrobj->Obj.X = ListObjet[info3].Obj.X ;
		ptrobj->Obj.Z = ListObjet[info3].Obj.Z ;
		ptrobj->Obj.Beta = ListObjet[info3].Obj.Beta ;
		ClearRealAngle( ptrobj ) ;
		break ;

/*-------------------------------------------------------------------------*/
	// dedié aux wagons
	case MOVE_WAGON:
		DoDirWagon( ptrobj ) ;
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_CIRCLE:	// Rotation autour d'un drapeau
				// en orientant le beta de l'objet sur la
				// tangeante du cercle
		if( ptrobj->SRot )
		{
			S32	angle ;
			S32	x, z ;

			// init du timer
			if( !ptrobj->Info2 )	ptrobj->Info2 = TimerRefHR ;

			x = ListBrickTrack[ptrobj->Info3].X ;
			z = ListBrickTrack[ptrobj->Info3].Z ;

			angle = (((TimerRefHR-ptrobj->Info2)*320)/ptrobj->SRot)+ptrobj->Info1 ;
			Rotate( 0, ptrobj->Info, angle ) ;
			ptrobj->Obj.X = x + X0 ;
			ptrobj->Obj.Z = z + Z0 ;
			ptrobj->Obj.Beta = (1024+angle)&4095 ;
		}
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_CIRCLE2:	// Rotation autour d'un drapeau
				// en orientant le beta de l'objet face
				// au drapeau
		if( ptrobj->SRot )
		{
			S32	angle ;
			S32	x, z ;

			// init du timer
			if( !ptrobj->Info2 )	ptrobj->Info2 = TimerRefHR ;

			x = ListBrickTrack[ptrobj->Info3].X ;
			z = ListBrickTrack[ptrobj->Info3].Z ;

			angle = (((TimerRefHR-ptrobj->Info2)*320)/ptrobj->SRot)+ptrobj->Info1 ;

			Rotate( 0, ptrobj->Info, angle ) ;
			ptrobj->Obj.X = x + X0 ;
			ptrobj->Obj.Z = z + Z0 ;
			ptrobj->Obj.Beta = (2048+angle)&4095 ;
		}
		break ;
	}
/*──────────────────────────────────────────────────────────────────────────*/
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▄  █    ██▀▀▀ █▀▀▀█ █     █▀▀▀█ █  ▄▀
		   ██  █  ██   ▀▀▀▀█ ██▀▀▀ ██    ██▀▀█ ██▀
		   ▀▀▀▀   ▀▀   ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
#ifdef	DEBUG_TOOLS
void	DrawZV( T_OBJET *ptrobj )
{
	if( DrawCube(	ptrobj->XMin + ptrobj->Obj.X,
			ptrobj->YMin + ptrobj->Obj.Y,
			ptrobj->ZMin + ptrobj->Obj.Z,
			ptrobj->XMax + ptrobj->Obj.X,
			ptrobj->YMax + ptrobj->Obj.Y,
			ptrobj->ZMax + ptrobj->Obj.Z,
			(ptrobj->WorkFlags&FRAME_SHIELD)?8:0 ) )
	{
		BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
	}

	UnsetClip() ;
}

#endif


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
// appelée si le joueur appuie sur <ESC> pendant une scene automatique
// si CinemaMode>1 (Cube de destination)
void	ResetCinemaMode( void )
{
	ListVarGame[FLAG_ESC] = 1 ;	// previent scenar

	if( CinemaMode>1 )	// Cube de destination
	{
		NumCube	   = -1 ;	// pour forcer le changement de cube
		NewCube	   = CinemaMode	;
		CinemaMode = FALSE	;

		if( !FlagFade )	FadeToBlackAndSamples( PtrPal ) ;
		FlagFade = TRUE 	;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
void	FixeCinemaMode( S32 flagflip )
{
	if( CinemaMode )
	{
		if( ClipWindowYMin<40 )
		{
			// Scrolle vers l'interieur
			if( flagflip )
			{
				S32	ymin = BoundRegleTrois( DebCycleCinema, 40, DureeCycleCinema, TimerRefHR-TimerCinema ) ;

				SetClipWindow( ClipWindowXMin,
					       ymin,
					       ClipWindowXMax,
					       479-ymin ) ;
			}

			if( ClipWindowYMin>0 )
			{
				MemoClipWindow() ;
				UnsetClipWindow() ;

				Box( 0, 0, 639, MemoClipWindowYMin-1, COUL_CINEMA ) ;
				Box( 0, MemoClipWindowYMax+1, 639, 479, COUL_CINEMA ) ;

				if( flagflip  )
				{
					BoxStaticAdd( 0, 0, 639, MemoClipWindowYMin-1 ) ;
					BoxStaticAdd( 0, MemoClipWindowYMax+1, 639, 479 ) ;
				}

				RestoreClipWindow() ;
			}
		}
		else if( !flagflip )	// changement de caméra
		{
			MemoClipWindow() ;
			UnsetClipWindow() ;

			Box( 0, 0, 639, MemoClipWindowYMin-1, COUL_CINEMA ) ;
			Box( 0, MemoClipWindowYMax+1, 639, 479, COUL_CINEMA ) ;

			RestoreClipWindow() ;
		}
	}
	else
	{
		// Scrolle vers l'exterieur
		if( ClipWindowYMin>0 )
		{
			LastYCinema = ClipWindowYMin ;

			if( flagflip )
			{
				S32	ymin = BoundRegleTrois( DebCycleCinema, 0, DureeCycleCinema, TimerRefHR-TimerCinema ) ;

				SetClipWindow( ClipWindowXMin,
					       ymin,
					       ClipWindowXMax,
					       479-ymin ) ;
			}

			MemoClipWindow() ;
			UnsetClipWindow() ;

			if( MemoClipWindowYMin>0 )
			{
				Box( 0, 0, 639, MemoClipWindowYMin-1, COUL_CINEMA ) ;
				Box( 0, MemoClipWindowYMax+1, 639, 479, COUL_CINEMA ) ;
			}

			if( flagflip )
			{
				BoxStaticAdd( 0, 0, 639, 39 ) ;
				BoxStaticAdd( 0, 439, 639, 479 ) ;
			}

			RestoreClipWindow() ;
		}
		else if( ClipWindowYMin!=LastYCinema )
		{
			// derniere bande à effacer
			if( flagflip )
			{
				MemoClipWindow() ;
				UnsetClipWindow() ;

				BoxStaticAdd( 0, 0, 639, 39 ) ;
				BoxStaticAdd( 0, 439, 639, 479 ) ;

				RestoreClipWindow() ;
			}

			LastYCinema = ClipWindowYMin ;
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
void	RestoreCinemaMode( void )
{
	if( !CinemaMode
	AND (ClipWindowYMin>0 OR ClipWindowYMin!=LastYCinema) )
	{
		// restore Log
		CopyBlock( 0, ClipWindowYMin, 639, 39, Screen, 0, ClipWindowYMin, Log ) ;
		CopyBlock( 0, 439, 639, ClipWindowYMax, Screen, 0, 439, Log ) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
static  S32	FlagFlip = 0	;
/*──────────────────────────────────────────────────────────────────────────*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
void	AfficheShadow( T_SORT *pt, S32 beta )
{
	if( PtrProjectShadow( pt, beta ) )
	{
		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			S32	xmin, xmax, zmin, zmax ;
			S32	yw /*= pt->Yw*/ ;

			DrawShadow( ) ;

			yw = pt->Yw ;

			// Magouille pour eviter de se faire recouvrir
			// dans les penchés en Interieur
/*			if( CubeMode==CUBE_INTERIEUR )
			{
				if( WorldColBrick( pt->Xw, yw, pt->Zw ) > 1 )
					yw += SIZE_BRICK_Y ;
			}
*/
			xmin = pt->XMin-pt->Xw ;
			xmax = pt->XMax-pt->Xw ;
			zmin = pt->ZMin-pt->Zw ;
			zmax = pt->ZMax-pt->Zw ;

			DrawRecover( 	pt->Xw, yw, pt->Zw,
					pt->YMax-yw,
					__max( (xmax-xmin)/2, (zmax-zmin)/2 ) ) ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/
S32	AffOneObject(T_SORT *pt)
{
	T_OBJET	*ptrobj 	;
	T_EXTRA	*ptrextra 	;
	T_DART	*ptrd		;
	S32	aff		;
	S32	numobj, typeobj ;
	S32	num		;
	S32	memoy		;
	S32	memoymax	;

	numobj  = pt->NumObj ;
	typeobj = numobj & ~1023 ;
	numobj &= 1023 ;

	ptrobj = &ListObjet[ numobj ] ;

	UnsetClip() ;

	switch( typeobj )
	{
	/*------------------------------------------------------------------------*/
	case TYPE_OBJ_3D:
		if( CubeMode==CUBE_EXTERIEUR )
		{
			if( ptrobj->Flags&(OBJ_ZBUFFER|OBJ_IN_WATER) )
				Switch_Fillers( FILL_POLY_FOG_NZW );

			SetCLUT( PalLevel +
				 16 * BoundRegleTrois(0,15,
						ClipZFar - StartZFog,
						pt->Z - StartZFog )
				) ;
		}

		if( ptrobj->WorkFlags&DRAW_SHADOW )
		{
			S32	nu = ptrobj->CarryBy ;

			memoymax = pt->YMax ;
			memoy = pt->Yw ;
			pt->YMax = memoy ;

			if(  (nu!=-1)
			AND !(nu&CARRY_BY_DECORS) )
			{
				pt->Yw = ListObjet[nu].Obj.Y+ListObjet[nu].YMax ;
			}

			AfficheShadow( pt, ptrobj->Obj.Beta ) ;
			ptrobj->WorkFlags &= ~(DRAW_SHADOW) ;

			pt->Yw = memoy ;
			pt->YMax = memoymax ;

			UnsetClip() ;
		}

		if( !(ptrobj->WorkFlags&MANUAL_INTER_FRAME) )
		{
			ObjectSetInterFrame( &ptrobj->Obj )	;
		}

		if( FlagWater AND (numobj == NUM_PERSO)
		AND CubeMode==CUBE_INTERIEUR
		AND !ModeLabyrinthe )
		{
			SetClip( 0, 0, ModeDesiredX-1, FlagWater ) ;
		}

		aff = ObjectDisplay( &ptrobj->Obj )	;

		if( aff )	/* objet pas tout clippé */
		{
			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				S32	yw = ptrobj->Obj.Y ;

				// Magouille pour eviter de se faire recouvrir
				// dans les penchés en Interieur
/*				if( CubeMode==CUBE_INTERIEUR )
				{
//					AND (ptrobj->Col&127) > 1 )
					if( WorldColBrick(ptrobj->Obj.X,yw,ptrobj->Obj.Z) > 1 )
					{
						yw+=SIZE_BRICK_Y ;
					}
				}
*/
				ptrobj->WorkFlags |= WAS_DRAWN ;

				// ATTENTION: un objet en OBJ_ZBUFFER ne peut faire
				// recaler la camera automatiquement !!!!!!

				if( ptrobj->Flags&(OBJ_ZBUFFER|OBJ_IN_WATER) )
				{
					// grosse magouille cage de la fin
					if( CubeMode==CUBE_INTERIEUR )
					{
						DrawOverBrickCage( 	(ptrobj->Obj.X+ptrobj->XMax)/SIZE_BRICK_XZ,
									(ptrobj->Obj.Y)/SIZE_BRICK_Y,
									(ptrobj->Obj.Z+ptrobj->ZMax)/SIZE_BRICK_XZ ) ;
					}

					BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
				}
				else
				{
					if( DrawRecover( ptrobj->Obj.X, yw, ptrobj->Obj.Z, ptrobj->YMax,
							 __max( (ptrobj->XMax-ptrobj->XMin)/2, (ptrobj->ZMax-ptrobj->ZMin)/2 )) )
					{
						// Objet complètement masqué
						if( numobj==NumObjFollow
						AND TestNumObjFollow	// protec boucle infinie
						AND !CameraZone )
						{
							TestNumObjFollow = FALSE ;
							CameraCenter( 1 ) ;
							return TRUE ;
						}
					}
					else	if( numobj==NumObjFollow )
							TestNumObjFollow = TRUE ;
				}

				if( (ptrobj->Flags & OBJ_BACKGROUND)
				AND ((FlagFlip == AFF_ALL_FLIP)
				OR   (FlagFlip == AFF_ALL_NO_FLIP)) )
				{
					CopyBlock( ClipXMin, ClipYMin, ClipXMax, ClipYMax, Log,
						   ClipXMin, ClipYMin, Screen ) ;
				}

#ifdef DEBUG_TOOLS
				if( DrawZVObjets )
				{
					DrawZV( ptrobj ) ;
				}
#endif
			}
		}

		// attend d'avoir été affiché pour connaitre les coordonnées
		// d'un de ses points
		if( ptrobj->WorkFlags&WAIT_COORD )
		{
			// nécessaire 1 seule frame
			ptrobj->WorkFlags &= ~(WAIT_COORD) ;

			num = ptrobj->Coord.A3DS.Num ;// num extra

			if( num!=-1 )
			{
				ptrextra = &ListExtra[num] ;

				num = ptrextra->PosX ;// ptrextra->PosX contient le numéro de point

				// recupere coord dans anim
				LongWorldInverseRotatePoint( Obj_ListRotatedPoints[num].X,
							     Obj_ListRotatedPoints[num].Y,
							     Obj_ListRotatedPoints[num].Z );

				ptrextra->U.Org.X = ptrextra->PosX = ptrobj->Obj.X + X0 ;
				ptrextra->U.Org.Y = ptrextra->PosY = ptrobj->Obj.Y + Y0 ;
				ptrextra->U.Org.Z = ptrextra->PosZ = ptrobj->Obj.Z + Z0 ;
			}
		}

		if( CubeMode==CUBE_EXTERIEUR
		AND ptrobj->Flags&(OBJ_ZBUFFER|OBJ_IN_WATER) )
		{
			Switch_Fillers( FILL_POLY_FOG ) ;
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_OBJ_SPRITE:
		num = ptrobj->Obj.Body.Num ;

		if( ptrobj->WorkFlags&DRAW_SHADOW )
		{
			S32	nu = ptrobj->CarryBy ;

			memoymax = pt->YMax ;
			memoy = pt->Yw ;
			pt->YMax = memoy ;

			if(  (nu!=-1)
			AND !(nu&CARRY_BY_DECORS) )
			{
				pt->Yw = ListObjet[nu].Obj.Y+ListObjet[nu].YMax ;
			}

			if( CubeMode==CUBE_EXTERIEUR )
			{
				SetCLUT( PalLevel +
					 16 * BoundRegleTrois(0,15,
							ClipZFar - StartZFog,
							pt->Z - StartZFog )
					) ;
			}

			AfficheShadow( pt, ptrobj->Obj.Beta ) ;
			ptrobj->WorkFlags &= ~(DRAW_SHADOW) ;

			pt->Yw = memoy ;
			pt->YMax = memoymax ;

			UnsetClip() ;
		}

		ClearScreenMinMax() ;

		PtrProjectSprite(  ptrobj->Obj.X,
				   ptrobj->Obj.Y,
				   ptrobj->Obj.Z, num ) ;

		if( ptrobj->Flags & SPRITE_CLIP )
		{
			SetClip(ptrobj->Info+XpOrgw, ptrobj->Info1+YpOrgw,
				ptrobj->Info2+XpOrgw, ptrobj->Info3+YpOrgw ) ;

			// A remplacer par une fonction de preClipping des
			// sprites
			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				PtrAffGraph( SpriteX, SpriteY, num ) ;
				UnsetClip() ;
				SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
			}
		}
		else
		{
			PtrAffGraph( SpriteX, SpriteY, num ) ;
			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
		}

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			ptrobj->WorkFlags |= WAS_DRAWN ;

			if( ptrobj->Flags & SPRITE_CLIP )
			{
				DrawRecover3(	ptrobj->Obj.LastAnimStepX,
						ptrobj->Obj.LastAnimStepY,
						ptrobj->Obj.LastAnimStepZ,
						ptrobj->YMax,
						__max( (ptrobj->XMax-ptrobj->XMin)/2, (ptrobj->ZMax-ptrobj->ZMin)/2 ) ) ;
			}
			else
			{
				// grosse magouille cage de la fin
				if( ptrobj->Flags&(OBJ_ZBUFFER|OBJ_IN_WATER) )
				{
					DrawOverBrickCage( 	(ptrobj->Obj.X+ptrobj->XMax)/SIZE_BRICK_XZ,
								(ptrobj->Obj.Y)/SIZE_BRICK_Y,
								(ptrobj->Obj.Z+ptrobj->ZMax)/SIZE_BRICK_XZ ) ;

					BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
				}
				else
				{
					DrawRecover3(	ptrobj->Obj.X+ptrobj->XMax,
							ptrobj->Obj.Y,
							ptrobj->Obj.Z+ptrobj->ZMax,
							ptrobj->YMax,
							__max( (ptrobj->XMax-ptrobj->XMin)/2, (ptrobj->ZMax-ptrobj->ZMin)/2 ) ) ;
				}
			}

			if( (ptrobj->Flags & OBJ_BACKGROUND)
			AND ((FlagFlip == AFF_ALL_FLIP)
			OR   (FlagFlip == AFF_ALL_NO_FLIP)) )
			{
				CopyBlock( ClipXMin, ClipYMin, ClipXMax, ClipYMax, Log,
					   ClipXMin, ClipYMin, Screen ) ;
			}
#ifdef DEBUG_TOOLS
			if( DrawZVObjets )
			{
				DrawZV( ptrobj ) ;
			}
#endif
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_OBJ_ANIM_3DS:
		num = ptrobj->Obj.Body.Num ;

		if( ptrobj->WorkFlags&DRAW_SHADOW )
		{
			S32	nu = ptrobj->CarryBy ;

			memoymax = pt->YMax ;
			memoy = pt->Yw ;
			pt->YMax = memoy ;

			if(  (nu!=-1)
			AND !(nu&CARRY_BY_DECORS) )
			{
				pt->Yw = ListObjet[nu].Obj.Y+ListObjet[nu].YMax ;
			}

			if( CubeMode==CUBE_EXTERIEUR )
			{
				SetCLUT( PalLevel +
					 16 * BoundRegleTrois(0,15,
							ClipZFar - StartZFog,
							pt->Z - StartZFog )
					) ;
			}

			AfficheShadow( pt, ptrobj->Obj.Beta ) ;
			ptrobj->WorkFlags &= ~(DRAW_SHADOW) ;

			pt->Yw = memoy ;
			pt->YMax = memoymax ;

			UnsetClip() ;
		}

		PtrProjectPoint(  ptrobj->Obj.X,
				   ptrobj->Obj.Y,
				   ptrobj->Obj.Z ) ;

		ScaleFactorSprite = DEF_SCALE_FACTOR ;

		SpriteX = Xp + PtrZvAnim3DS[ num*8 + 0 ] ;
		SpriteY = Yp + PtrZvAnim3DS[ num*8 + 1 ] ;

		if( ptrobj->Flags & SPRITE_CLIP )
		{
			SetClip(ptrobj->Info+XpOrgw, ptrobj->Info1+YpOrgw,
				ptrobj->Info2+XpOrgw, ptrobj->Info3+YpOrgw ) ;

			// A remplacer par une fonction de preClipping des
			// sprites
			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				AffGraph( 0, SpriteX, SpriteY, GetPtrAnim3DS(num) ) ;
			}
		}
		else
		{
			AffGraph( 0, SpriteX, SpriteY, GetPtrAnim3DS(num) ) ;
			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
		}

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			ptrobj->WorkFlags |= WAS_DRAWN ;

			if( ptrobj->Flags & SPRITE_CLIP )
			{
				DrawRecover3(	ptrobj->Obj.LastAnimStepX,
						ptrobj->Obj.LastAnimStepY,
						ptrobj->Obj.LastAnimStepZ,
						ptrobj->YMax,
						__max( (ptrobj->XMax-ptrobj->XMin)/2, (ptrobj->ZMax-ptrobj->ZMin)/2 ) ) ;
			}
			else
			{
				DrawRecover3(	ptrobj->Obj.X+ptrobj->XMax,
						ptrobj->Obj.Y,
						ptrobj->Obj.Z+ptrobj->ZMax,
						ptrobj->YMax,
						__max( (ptrobj->XMax-ptrobj->XMin)/2, (ptrobj->ZMax-ptrobj->ZMin)/2 ) ) ;
			}

			if( (ptrobj->Flags & OBJ_BACKGROUND)
			AND ((FlagFlip == AFF_ALL_FLIP)
			OR   (FlagFlip == AFF_ALL_NO_FLIP)) )
			{
				CopyBlock( ClipXMin, ClipYMin, ClipXMax, ClipYMax, Log,
					   ClipXMin, ClipYMin, Screen ) ;
			}
#ifdef DEBUG_TOOLS
			if( DrawZVObjets )
			{
				DrawZV( ptrobj ) ;
			}
#endif
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_SHADOW:
		if( CubeMode==CUBE_EXTERIEUR )
		{
			SetCLUT( PalLevel +
				 16 * BoundRegleTrois(0,15,
						ClipZFar - StartZFog,
						pt->Z - StartZFog )
				) ;
		}

		AfficheShadow( pt, numobj<<2 ) ;
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_EXTRA:
		ptrextra = &ListExtra[ numobj ] ;

		PtrProjectPoint( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;

		num = ptrextra->Sprite ;
		if( num & 32768 )	/// effet spécial
		{
			AffSpecial( numobj ) ;
		}
		else			// sprite
		{
			SpriteX = Xp + PtrZvExtraRaw[ num*8 + 0 ] ; // Add Hot Spot
			SpriteY = Yp + PtrZvExtraRaw[ num*8 + 1 ] ;

			// Toujours des sprites Raw
			CalculeScaleFactorSprite( ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ, ptrextra->Scale ) ;

			if( ptrextra->Flags&EXTRA_TRANSPARENT )
			{
				ScaleSpriteTransp( 0, SpriteX, SpriteY,
					     ScaleFactorSprite, ScaleFactorSprite,
					     HQR_Get( HQRPtrSpriteRaw, num ),
					     PtrTransPal ) ;
			}
			else
			{
				ScaleSprite( 0, SpriteX, SpriteY,
					     ScaleFactorSprite, ScaleFactorSprite,
					     HQR_Get( HQRPtrSpriteRaw, num ) ) ;
			}
		}

		SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			S32	xmin, ymin, zmin, xmax, ymax, zmax ;
			S32	yw = ptrextra->PosY ;

			// Magouille pour eviter de se faire recouvrir
			// dans les penchés en Interieur
			if( CubeMode==CUBE_INTERIEUR )
			{
				if( WorldColBrick( ptrextra->PosX, yw, ptrextra->PosZ ) >  1 )
					yw += SIZE_BRICK_Y ;
			}

			GetExtraZV( ptrextra, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax ) ;

			DrawRecover3(	ptrextra->PosX,
					yw,
					ptrextra->PosZ,
					ymax,
					__max( (xmax-xmin)/2, (zmax-zmin)/2 ) ) ;
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_FLOW:
		{
		S_PART_FLOW *ptrf = &ListPartFlow[numobj] ;

		if( AffParticleFlow( ptrf ) )
		{
			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				S32	xw = ptrf->OrgX ;
				S32	yw = ptrf->OrgY ;
				S32	zw = ptrf->OrgZ ;

				// Magouille pour eviter de se faire recouvrir
				// dans les penchés en Interieur
				if( CubeMode==CUBE_INTERIEUR )
				{
					xw += ptrf->XMax ;
					zw += ptrf->ZMax ;
					yw += ptrf->YMin ;

					if( WorldColBrick( pt->Xw, yw, pt->Zw ) >  1 )
						yw += SIZE_BRICK_Y ;
				}

				DrawRecover( xw, yw, zw, ptrf->YMax,
					     __max( (ptrf->XMax-ptrf->XMin)/2, (ptrf->ZMax-ptrf->ZMin)/2 ) ) ;
			}
		}
		}
		break ;

	/*------------------------------------------------------------------------*/
	// Flechettes
	case TYPE_DART:
		ptrd = &ListDart[numobj] ;

		aff = BodyDisplay_AlphaBeta(	ptrd->PosX, ptrd->PosY, ptrd->PosZ,
					ptrd->Alpha, ptrd->Beta, 0,
					(U8*)GivePtrObjFix( ptrd->Body ) ) ;

		if( aff )
		{
			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				S32	yw = ptrd->PosY ;

				// Magouille pour eviter de se faire recouvrir
				// dans les penchés en Interieur
				if( CubeMode==CUBE_INTERIEUR )
				{
					if( WorldColBrick( ptrd->PosX, yw, ptrd->PosZ ) > 1 )
						yw += SIZE_BRICK_Y ;
				}

				DrawRecover( ptrd->PosX+ptrd->XMax, yw, ptrd->PosZ+ptrd->ZMax, ptrd->YMax,
					     __max( (ptrd->XMax-ptrd->XMin)/2, (ptrd->ZMax-ptrd->ZMin)/2 ) ) ;
			}
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_LABY:
		AffOneBrick( (S16)pt->Xw, (S16)pt->Yw, (S16)pt->Zw ) ;
		break ;

	/*------------------------------------------------------------------------*/
	}	/* end switch typeobj */
	UnsetClip() ;

	return FALSE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

void	AffNbFps( void )
{
	if( DebugFps )
	{
		MemoClipWindow() 	;// au cas ou il y ait le cinema Mode
		UnsetClipWindow() 	;

		CoulText( LBAWHITE, 0 ) ;
		GraphPrintf( FALSE, 0,472, "%d ", NbFramePerSecond ) ;

		RestoreClipWindow()	;
		UnsetClip()		;
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
/*──────────────────────────────────────────────────────────────────────────*/

void	AffScene( S32 flagflip )
{
	S32	n, c, cmpt ;
	S32	dx, dy ;
	T_OBJET		*ptrobj ;
	T_EXTRA		*ptrextra ;
	T_INCRUST_DISP	*ptrdisp ;
	S_PART_FLOW	*ptrflow;
	T_DART		*ptrd ;
	U8		*changepal = NULL ;

	S32	x,y,z ;
	S32	num ;
	S32	oldxporg ;
	S32	oldyporg ;

	S32	txmin, tymin, tzmin, txmax, tymax, tzmax ;
	S32	typeobj ;

	char	string[256] ;
/*------------------------------------------------------------------------*/
startaffscene:

	FlagFlip = flagflip	;//	Beurk, cf AffOneObject()

	oldxporg = XpOrgw ;
	oldyporg = YpOrgw ;

/*------------------------------------------------------------------------*/

	UnsetClip() ;

	switch( flagflip )
	{
		case AFF_OBJETS_FLIP:
//			BoxClean() ;
		case AFF_OBJETS_NO_FLIP:
			BoxClean() ;// ATTENTION: rajouté le 30/05/97
			if( !FlagAnimWhoSpeak )
			{
				RestoreCinemaMode() ;	// Scrolle les bandes noires
				if( CubeMode==CUBE_EXTERIEUR )	DrawAnimatedPolys() ;
			}
			break ;

		case AFF_ALL_FLIP:
			// Pour ecraser MemoClipWindowYMin qui est
			// testé plus loin
			if( !FlagAnimWhoSpeak )	MemoClipWindow() ;

			if( !CinemaMode AND ClipWindowYMin>0 )
			{
				MemoClipWindow() ;
				UnsetClipWindow() ;
			}

			ClearImpactRain()  ;

		case AFF_ALL_NO_FLIP:
			LockTimer() ;
			BoxReset() ;
			Cls() ;

			RefreshGrille() ;

			ChangeIncrustPos( oldxporg,oldyporg, XpOrgw, YpOrgw ) ;
//			HQ_ChangeBalanceSamples( oldxporg, oldyporg ) ;

			CopyScreen( Log, Screen ) ;

			if( flagflip==AFF_ALL_FLIP
			AND !CinemaMode AND MemoClipWindowYMin>0 )
			{
				RestoreClipWindow() ;
				UnsetClip() ;
			}
			break ;
	}

/*------------------------------------------------------------------------*/

	// Peut-etre faut-il le mettre avant l'affichage de la scene s'il y
	// a des textures animées dans le decors (Ouppppps! J'espère que non !)
	if( AnimateTexture OR ProtectActif )	DoTextureAnimation() ;

/*------------------------------------------------------------------------*/
/* objets scenarique 3D ou sprite */

	BaseReset() ;

	if( CubeMode==CUBE_EXTERIEUR )
	{
		Switch_Fillers( FILL_POLY_FOG );
		PtrCLUTGouraud = PtrCLUTFog ;
	}

	NbTriObj = 0 	;

/*------------------------------------------------------------------------*/

	ptrobj = ListObjet ;

/*------------------------------------------------------------------------*/
	for( n=0; n<NbObjets; n++, ptrobj++ )
	{
		ptrobj->WorkFlags &= ~WAS_DRAWN ;

		if( (ptrobj->Flags & OBJ_BACKGROUND)
		AND ((flagflip == AFF_OBJETS_FLIP)
		OR   (flagflip == AFF_OBJETS_NO_FLIP)) )
		{
			// rustine: renseigne qd meme moteur presence sprite
			// pour obj_back si dans l'écran
			PtrProjectPoint( ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
			if( (Xp>VIEW_X0) AND (Xp<VIEW_X1) AND (Yp>VIEW_Y0) AND (Yp<VIEW_Y1) )
			{
				ptrobj->WorkFlags |= WAS_DRAWN ;
			}
			continue ;
		}

		if( (ptrobj->Obj.Body.Num == -1)
		OR (ptrobj->Flags&INVISIBLE))	continue	;

		x = ptrobj->Obj.X	;
		y = ptrobj->Obj.Y	;
		z = ptrobj->Obj.Z	;

		if( ptrobj->Flags&CHECK_CODE_JEU
		AND y>256 )
		{
			if( ModeLabyrinthe )
			{
				txmin = (x/SIZE_BRICK_XZ)*SIZE_BRICK_XZ + DEMI_BRICK_XZ - 768 ;
				txmax = txmin + 1536 ;
				tymin = (y/SIZE_BRICK_Y)*SIZE_BRICK_Y - 1 ;
				tymax = tymin + 1 ;
				tzmin = (z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ + DEMI_BRICK_XZ - 768 ;
				tzmax = tzmin + 1536 ;

				TreeInsert( (S16)(n|TYPE_LABY),
					    x, y, z,
					    txmin, tymin, tzmin,
					    txmax, tymax, tzmax ) ;
//					    x-1024, y-768, z-1024,
//					    x+1024, y, z+1024 ) ;
			}
		}

		// Perso Clignotant (Pause ou Mort)
		if( n==NUM_PERSO AND PersoFlashTimer )
		{
			if( (TimerRefHR&0xFF) < 85 )	// aff 2/3 eff 1/3
			{
				continue ;
			}
			else
			{
				if( !IsSamplePlaying(SAMPLE_FLASH_PERSO) )
				{
					HQ_MixSample( SAMPLE_FLASH_PERSO,
						0x1000, 0,	// frequence, decalage
						1, 64, VOLUME_FLASH_PERSO ) ;
				}
			}
		}

		// preclip derriere camera (toujours fait !)
		if( !PtrProjectPoint( x, y, z ) )
			continue ;

		txmin = x + ptrobj->XMin ;
		tymin = y + ptrobj->YMin ;
		tzmin = z + ptrobj->ZMin ;
		txmax = x + ptrobj->XMax ;
		tymax = y + ptrobj->YMax ;
		tzmax = z + ptrobj->ZMax ;

		if( ptrobj->Flags & SPRITE_3D )
		{
			if( ptrobj->Flags & ANIM_3DS )
				typeobj = TYPE_OBJ_ANIM_3DS 	;
			else	typeobj = TYPE_OBJ_SPRITE 	;
		}
		else	typeobj = TYPE_OBJ_3D	  ;

		if( ptrobj->Flags&NO_PRE_CLIP )
		{
			typeobj |= SORT_NO_PRECLIP	;
		}

		TreeInsert( (S16)(n|typeobj), x, y, z,
			    txmin, tymin, tzmin, txmax, tymax, tzmax ) ;

		if( Shadow )
		{
			if( !(ptrobj->Flags & NO_SHADOW) )
			{
				if( (ptrobj->CarryBy!=-1)
				OR  ((ptrobj->CodeJeu>>4)==CJ_FOOT_WATER) )
				{
					ptrobj->WorkFlags |= DRAW_SHADOW ;
					continue ;
				}
				else
				{
					PtrGetShadow( x, y, z )	;

					if( n==NUM_PERSO ) ShadowYHero = ShadowY ;

					if( x==ShadowX AND y==ShadowY AND z==ShadowZ )
					{
						ptrobj->WorkFlags |= DRAW_SHADOW ;
						continue ;
					}
				}

				typeobj = ((ptrobj->Obj.Beta&4095)>>2)|TYPE_SHADOW ;

				if( ptrobj->Flags&NO_PRE_CLIP )
				{
					typeobj |= SORT_NO_PRECLIP	;
				}

				TreeInsert( (S16)typeobj,
					    ShadowX, ShadowY, ShadowZ,
					    txmin, ShadowY, tzmin, txmax, ShadowY, tzmax ) ;
			}
		}
	}

/*------------------------------------------------------------------------*/
/* objet temporaires (throw, bonus...) */

	ptrextra = ListExtra ;
	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite == -1 )	continue	;

		if( ptrextra->Flags & EXTRA_TIME_IN )
		{
			S32	timein = 35*20 ;

			// pas de gestion pendant un message
			if( FlagAnimWhoSpeak )	continue ;

			if( ListObjet[NUM_PERSO].Move==MOVE_BUGGY_MANUAL )
			{
				timein = 70*20 ;
			}

			if( (TimerRefHR - ptrextra->Timer) > timein )
			{
				ptrextra->Timer = TimerRefHR ;
				ptrextra->Flags &= ~EXTRA_TIME_IN ;

				HQ_3D_MixSample( SAMPLE_BONUS, 0x1000, 0, 1,
					ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
			}
			continue ;
		}

		if( ptrextra->Flags & EXTRA_WAIT )
		{
			// pas de gestion pendant un message
			if( FlagAnimWhoSpeak )	continue ;

			if( TimerRefHR > ptrextra->Timer )
			{
				if( ptrextra->Flags&EXTRA_TRAINEE
				AND MagicBall!=-1 )
					ptrextra->Timer = ListExtra[MagicBall].Timer ;
				else	ptrextra->Timer = TimerRefHR ;

				ptrextra->Flags &= ~(EXTRA_WAIT) ;
			}
			continue ;
		}

		if( ptrextra->Flags & EXTRA_TIME_OUT )
		{
			if( !FlagAnimWhoSpeak )
			{
				/* fait clignoter aff */
				if( ptrextra->Flags & EXTRA_FLASH )
				{
					if( TimerRefHR >= ptrextra->Timer + ptrextra->TimeOut - 50*20 * 3 )
					{
						if( ((TimerRefHR+ptrextra->Timer)/20) & 8 )
							continue ;
					}
				}
			}
			else if( TimerRefHR >= ptrextra->Timer + ptrextra->TimeOut - 50*20 * 3 )
				continue ;
		}

		if( ptrextra->Flags & EXTRA_ANIM_SPRITE )
		{
			// pas de gestion pendant un message
			if( FlagAnimWhoSpeak )	continue ;

			ptrextra->Sprite = (S16)BoundRegleTrois(
						ptrextra->Body,		// start
						ptrextra->Divers,	// end
						ptrextra->TimeOut,	// TotalTime
						TimerRefHR-ptrextra->Timer );

			if( ptrextra->Sprite>=ptrextra->Divers )
			{
				ptrextra->Sprite = -1 ;	// fini
				continue ;
			}
		}

		if( ptrextra->Flags & EXTRA_CLIGNOTE )
		{
			// pas de gestion pendant un message
			if( FlagAnimWhoSpeak )	continue ;
			if( (TimerRefHR&0xFF)<100 )	continue ;
		}

		x = ptrextra->PosX	;
		y = ptrextra->PosY	;
		z = ptrextra->PosZ	;

		// preclip
		if( !PtrProjectPoint( x, y, z ) )	continue ;

		if( ptrextra->Sprite==0x8004 )	// Dalle labyrinthe
		{
			TreeInsert( (S16)(n|TYPE_LABY),
				    x, y, z,
				    x-1024, y-768, z-1024,
				    x+1024, y, z+1024 ) ;
		}
		else
		{
			GetExtraZV( ptrextra, &txmin, &tymin, &tzmin,
					      &txmax, &tymax, &tzmax ) ;

			txmin += ptrextra->PosX ;
			tymin += ptrextra->PosY ;
			tzmin += ptrextra->PosZ ;
			txmax += ptrextra->PosX ;
			tymax += ptrextra->PosY ;
			tzmax += ptrextra->PosZ ;

			TreeInsert( (S16)(n|TYPE_EXTRA),
				    x, y, z,
				    txmin, tymin, tzmin,
				    txmax, tymax, tzmax ) ;

		}

		/* les effets n'ont pas d'ombres */
		if( (Shadow>=2) AND (!(ptrextra->Sprite&32768) OR ((ptrextra->Sprite&32767)==3))
		AND !(ptrextra->Flags&EXTRA_WAIT_NO_COL)
		AND (ptrextra->Flags&EXTRA_FLY)
		AND !(ptrextra->Flags&EXTRA_NO_SHADOW) )
		{
			PtrGetShadow( x, y, z ) ;

			if( (x!=ShadowX OR y!=ShadowY OR z!=ShadowZ) )
			{
				TreeInsert( (S16)(0|TYPE_SHADOW), ShadowX, ShadowY, ShadowZ,
					    txmin, ShadowY, tzmin, txmax, ShadowY+1, tzmax ) ;
			}
		}
	}

/*------------------------------------------------------------------------*/
/* Flechettes */
	ptrd = ListDart ;

	for( n=0; n<MAX_DARTS; n++, ptrd++ )
	{
		if( (ptrd->NumCube!=NumCube)
		OR (ptrd->Flags&DART_TAKEN) )	continue	;

		x = ptrd->PosX	;
		y = ptrd->PosY	;
		z = ptrd->PosZ	;

		// preclip
		if( !PtrProjectPoint( x, y, z ) )	continue ;

		txmin = x + ptrd->XMin	;
		tymin = y + ptrd->YMin 	;
		tzmin = z + ptrd->ZMin	;
		txmax = x + ptrd->XMax	;
		tymax = y + ptrd->YMax	;
		tzmax = z + ptrd->ZMax	;

		TreeInsert( (S16)(n|TYPE_DART),
			    x, y, z,
			    txmin, tymin, tzmin, txmax, tymax, tzmax ) ;

		if( ModeLabyrinthe )
		{
			TreeInsert( (S16)(n|TYPE_LABY), x, y, z,
				    x-1024, y-768, z-1024, x+1024, y, z+1024 ) ;
		}
	}

/*------------------------------------------------------------------------*/
	// Insertion des flows de pixels
	ptrflow = ListPartFlow ;
	for( n=0; n<MAX_FLOWS; n++, ptrflow++ )
	{
		if( !ptrflow->NbDot )	continue	;

		x = ptrflow->OrgX ;
		y = ptrflow->OrgY ;
		z = ptrflow->OrgZ ;

		// preclip
		if( !PtrProjectPoint( x, y, z ) )	continue ;

		TreeInsert( (S16)(n|TYPE_FLOW),
			    x, y, z,
			    x+ptrflow->XMin, y+ptrflow->YMin, z+ptrflow->ZMin,
			    x+ptrflow->XMax, y+ptrflow->YMax/2, z+ptrflow->ZMax ) ;
	}
/*------------------------------------------------------------------------*/
//				 boucle aff all objs
//----------------------------------------------------------------------------

	// Init Textures
	PtrMap = ObjPtrMap = BufferTexture ;
	RepMask = 0xFFFF ;

	if( BaseSort() )
	{
		// NumObjFollow masqué on
		// doit redessiner la camera
		if( flagflip==AFF_ALL_FLIP
		OR  flagflip==AFF_ALL_NO_FLIP )
		{
			// Car il y a un LockTimer() just après
			UnlockTimer() ;
		}

		flagflip = AFF_ALL_FLIP ;
		goto startaffscene ;
	}

/*------------------------------------------------------------------------*/
	if( CubeMode==CUBE_EXTERIEUR  )
	{
		if( TEMPETE_ACTIVE AND RainEnable )	AffRain() ;

		SetCLUT( PalLevel ) ;
		Switch_Fillers( FILL_POLY_TEXTURES );
	}

/*------------------------------------------------------------------------*/
// special incrust disp

	ptrdisp = ListIncrustDisp ;
	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
	if( ptrdisp->Num == -1 )	continue ;

	switch( ptrdisp->Move&0xFF )
	{
	case 0:	// wait nb sec and die
		// pas de gestion pendant un message
		if( !FlagAnimWhoSpeak )
		{
			if( TimerRefHR > ptrdisp->TimerEnd )
			{
				if( (ptrdisp->Type&0xFF)==INCRUST_ECLAIR
				AND ptrdisp->Info )
				{
					changepal = PtrPal ;
				}
				else if( (ptrdisp->Type&0xFF)==INCRUST_PLUIE )
				{
					FlagRain = FALSE ;

					if( CubeMode==CUBE_INTERIEUR OR TEMPETE_FINIE )
					{
						if( IsSamplePlaying(SAMPLE_RAIN) )      HQ_StopOneSample( SAMPLE_RAIN ) ;
					}
				}

				ptrdisp->Num = -1 ;
				continue ;
			}
		}
		break ;

	case 1:	// follow obj coor for nb sec and die
		num = ptrdisp->Info&0xFF ;	// skip couleur

		PtrProjectPoint(	ListObjet[num].Obj.X,
					ListObjet[num].Obj.Y + ListObjet[num].YMax,
					ListObjet[num].Obj.Z ) ;

		ptrdisp->X = (S16)Xp ;
		ptrdisp->Y = (S16)Yp ;

		// pas de gestion pendant un message
		if( !FlagAnimWhoSpeak )
		{
			if( TimerRefHR > ptrdisp->TimerEnd )
			{
				ptrdisp->Num = -1 ;
				continue ;
			}
		}
		break ;

	case 2:	// decremente compteur, follow obj coor for nb sec and die
		num = (ptrdisp->Move&0xFF00)>>8 ;

		PtrProjectPoint(	ListObjet[num].Obj.X,
					ListObjet[num].Obj.Y + ListObjet[num].YMax,
					ListObjet[num].Obj.Z ) ;

		ptrdisp->X = (S16)Xp ;
		ptrdisp->Y = (S16)Yp ;

		// pas de gestion pendant un message
		if( !FlagAnimWhoSpeak )
		{
			if( TimerRefHR>=ptrdisp->TimerEnd )
			{
				ptrdisp->Num-- ;
				ptrdisp->TimerEnd = TimerRefHR + 1000 ;
				continue ;
			}
		}
		break ;
	}

	UnsetClip() ;

	switch( ptrdisp->Type&0xFF )
	{
	case INCRUST_NUM:

		strcpy( string, Itoa( ptrdisp->Num ) ) ;
		dx = SizeFont( string ) ;
		dy = 48 ;

		ScreenXMin = ptrdisp->X - dx/2 ;
		ScreenYMin = ptrdisp->Y - dy/2 ;

		if( ptrdisp->Type&INCRUST_YCLIP )
		{
			ScreenYMin += ClipWindowYMin ;
		}

		ScreenXMax = ScreenXMin + dx ;
		ScreenYMax = ScreenYMin + dy ;

		SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
		ColorFont( ptrdisp->Info ) ;
		Font( ScreenXMin, ScreenYMin, string ) ;

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		}
		break ;

	case INCRUST_TEXT:	// SAY_MESSAGE

		if( FlagDisplayText )
		{
			GetMultiText( ptrdisp->Num, string ) ;
			dx = SizeFont( string ) + 1 ;	// because /2
			dy = 48 ;

			ScreenXMin = ptrdisp->X - dx/2 ;

			// tient compte du cinema mode
			ScreenYMin = ptrdisp->Y - dy/2 ;
			if( ScreenYMin<ClipWindowYMin )	ScreenYMin = ClipWindowYMin ;

			ScreenXMax = ScreenXMin + dx ;
			ScreenYMax = ScreenYMin + dy ;

			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				S32	x, y ;

				x = ScreenXMin ;
				y = ScreenYMin ;

				ColorFont( 0 ) ;
				Font( x+2, y+4, string ) ;
				ColorFont( ((ptrdisp->Info&0xFF00)>>8)*16 + 14 ) ;
				Font( x, y, string ) ;
				BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
			}
		}
		else
		{	// Speak Only
			DrawBulle( num ) ;// num positionné plus haut dans
					  // gestion deplacement incrust
		}
		break ;

	case INCRUST_SYS_TEXT:	// Changement de Comportement

		strcpy( string, DefSysText[ptrdisp->Num] ) ;
		dx = SizeFont( string ) + 1 ;	// because /2
		dy = 48 ;

		ScreenXMin = ptrdisp->X ;
		ScreenYMin = ptrdisp->Y ;
		ScreenXMax = ScreenXMin + dx ;
		ScreenYMax = ScreenYMin + dy ;

		SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			S32	x, y ;

			x = ScreenXMin ;
			y = ScreenYMin ;

			ColorFont( BLACK ) ;
			Font( x+2, y+4, string ) ;
			ColorFont( ptrdisp->Info ) ;
			Font( x, y, string ) ;
			BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		}
		break ;

	case INCRUST_SPRITE:
		if( ptrdisp->Num>=100 )
		{
			SpriteX = ptrdisp->X + PtrZvExtra[ ptrdisp->Num*8 + 0 ] ;
			SpriteY = ptrdisp->Y + PtrZvExtra[ ptrdisp->Num*8 + 1 ] ;
		}
		else
		{
			SpriteX = ptrdisp->X + PtrZvExtraRaw[ ptrdisp->Num*8 + 0 ] ;
			SpriteY = ptrdisp->Y + PtrZvExtraRaw[ ptrdisp->Num*8 + 1 ] ;
		}

		if( ptrdisp->Type&INCRUST_YCLIP )
		{
			SpriteY += ClipWindowYMin ;
		}

		ScaleFactorSprite = DEF_SCALE_FACTOR ;
		PtrAffGraph( SpriteX, SpriteY, ptrdisp->Num ) ;
		SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		}
		break ;

	case INCRUST_CMPT:
		// pas de gestion pendant un message
		if( !FlagAnimWhoSpeak )
		{
			cmpt = ptrdisp->TimerEnd-TimerRefHR ;
			c = BoundRegleTrois( ptrdisp->Info, ptrdisp->Num, 1000*2, cmpt-1000 ) ;
			ptrdisp->Y = (S16)c ;
		}
		else	c = ptrdisp->Y ;

		strcpy( string, Itoa( c ) ) ;
		dx = SizeFont( string ) ;
		dy = 48 ;

		ScreenXMin = ptrdisp->X ;

		// ATTENTION: Grosse Magouille: les compteurs sont tjs affichés
		// en y=20, ceci permet d'utiliser le Y pour memoriser la
		// valeur courante du compteur (cf FlagAnimWhoSpeak)
		ScreenYMin = 20 - dy/2 ;

		if( ptrdisp->Type&INCRUST_YCLIP )
		{
			ScreenYMin += ClipWindowYMin ;
		}

		ScreenXMax = ScreenXMin + dx ;
		ScreenYMax = ScreenYMin + dy ;

		SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
		ColorFont( 155 ) ;
		Font( ScreenXMin, ScreenYMin, string ) ;

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		}
		break ;

	case INCRUST_INVENTORY:
	case INCRUST_OBJ:
		{
		U8	*ptr3do ;

		ScreenXMin = ptrdisp->X ;
		ScreenYMin = ptrdisp->Y ;

		if( ptrdisp->Type&INCRUST_YCLIP )
		{
			ScreenYMin += ClipWindowYMin ;
		}

		ScreenXMax = ScreenXMin + 59 ;
		ScreenYMax = ScreenYMin + 59 ;

		SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
		BackupAngles( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		Box( ClipXMin, ClipYMin, ClipXMax, ClipYMax, 0 ) ;

		ptr3do = (U8 *)GivePtrObjFix( ptrdisp->Num ) ;

		// Anim les textures si elles ne le sont pas dans la boucle
		if( !AnimateTexture )	DoTextureAnimation() ;

		SaveCamera() ;

		SetProjection( 30+ClipXMin, 30+ClipYMin, 128,200,200 ) ;
		SetLightVector( OBJ_ALPHA_LIGHT, OBJ_BETA_LIGHT, 0 ) 	;
		SetFollowCamera( 0,0,0, 60*MUL_ANGLE,0,0, 16000 ) ;
		BetaUsedObj = TimerRefHR*80/20 ;	// environ 1 tr/s (4096/50 = 81)
		BodyDisplay( 0,0,0, 0,BetaUsedObj,0, ptr3do ) ;

		DrawCadre( ClipXMin, ClipYMin, ClipXMax, ClipYMax, ALL_ANGLES ) ;
		RestoreAngles( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;

		PtrInit3DView() ;
		if( CubeMode==CUBE_INTERIEUR )	CameraCenter( 0 ) 		;

		RestoreCamera() ;
		SetLightVector( AlphaLight, BetaLight, 0 ) 	;
		}
		break ;

	case INCRUST_ECLAIR:
		// pas de gestion pendant un message
		if( !FlagAnimWhoSpeak )
		{
			if( (TimerRefHR&0xFF)>100 )
			{
				if( ptrdisp->Info )
				{
					changepal = PtrPal ;
				}
				else
				{
					changepal = PtrPalEclair ;
				}

				ptrdisp->Info ^= 1 ;
			}
		}
		else
		{
			// on est dans un message, c'est plus joli de remettre
			// la palette normale
			changepal = PtrPal ;
		}
		break ;
	}
	}

/*------------------------------------------------------------------------*/
//DrawZonesDec()	;

//	if ( FlagNoFlip )	return	;//	Cf DoFoundObj()

/* flip boxes */

	UnsetClip() ;

	// incrust logo demo
#ifndef	DEMO
	if( DemoSlide )
#endif
	{
		S32	num = 11 ;
		S32	x = 639-103 ;

#ifndef	DEMO
		switch( DistribVersion )
		{
			case ACTIVISION_VERSION:
			case ACTIVISION_SUD_VERSION:
			case VIRGIN_VERSION:
			case VIRGIN_ASIA_VERSION:
				num = 16 ;// Logo Americain
				x   = 639-110 ;
				break ;
		}
#endif
		if( CubeMode==CUBE_EXTERIEUR AND TEMPETE_ACTIVE )	num++ ;

		MemoClipWindow() ;
		UnsetClipWindow() ;

		AffGraph( 0, x, 11, HQR_Get(HQRPtrSprite,num) ) ;
		BoxStaticAdd( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

#ifdef	DEMO
		num = 16 ;
		if( CubeMode==CUBE_EXTERIEUR AND TEMPETE_ACTIVE )	num++ ;

		AffGraph( 0, 639-110, 75, HQR_Get(HQRPtrSprite,num) ) ;
#else
		// a cause du CinemaMode
		BoxStaticAdd( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
#endif


		RestoreClipWindow() ;
	}

/*-------------------------------------------------------------------------*/

	UnsetClip() ;

	switch( flagflip )
	{
		case AFF_OBJETS_FLIP:
			if( !FlagAnimWhoSpeak )	FixeCinemaMode(TRUE) ;	// Scrolle les bandes noires
			AffNbFps() ;
			BoxBlit() ;
			break ;

		case AFF_ALL_FLIP:
			FixeCinemaMode(FALSE) ;	// Scrolle les bandes noires
			AffNbFps() ;
			BoxBlit() ;
			UnlockTimer() ;
			break ;

		case AFF_ALL_NO_FLIP:
			AffNbFps() ;
			UnlockTimer() ;
			break ;
	}

	if( FlagChgCube )
	{
		if( NumCube!=NUM_CUBE_PHANTOM )
		{
			AutoSaveGame() ;
		}

		FlagChgCube = 0 ;
	}

	if( changepal )	PaletteSync( changepal ) ;

	if( FlagPal )
	{
		PaletteSync( PtrPal ) ;
		FlagPal = FALSE ;
	}

	if( FlagFade )
	{
		FadeToPalAndSamples( PtrPal ) ;
		FlagFade = FALSE ;
	}
	else	if( FlagLoadGame )	PaletteSync( PtrPal ) ;

	if( !FlagAnimWhoSpeak )
	{
		switch( FlagRestoreCD )
		{
			case 3:// fadein but wait one bcl
			case 4:// resume but wait one bcl
				FlagRestoreCD -= 2 ;
				break ;

			case 1:// fadein
				FadeInVolumeMusic() ;
				FlagRestoreCD = 0 ;
				break ;

			case 2:// resume
				ResumeMusic(TRUE) ;
				FlagRestoreCD = 0 ;
				break ;
		}
	}
}
/*══════════════════════════════════════════════════════════════════════════*
		▀▀▀█▀ █▀▀▀█ ██▄ █ █▀▀▀▀       █▀▀▀▄ █▀▀▀▀ █▀▀▀▀
		 ▄█   ██  █ ██▀██ ██▀▀        ██  █ ██▀▀  ██
		▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀  ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
/*══════════════════════════════════════════════════════════════════════════*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	SetZoneCamera( T_ZONE *ptrz )
{
	S32	info0, info1, info2 ;

	info0 = ptrz->Info0 ;
	info1 = ptrz->Info1 ;
	info2 = ptrz->Info2 ;

	// signale pos forcée
	CameraZone = TRUE ;
	FlagCameraForcee = TRUE ;

	if( CubeMode==CUBE_INTERIEUR )
	{
		if( StartXCube != info0
		OR  StartYCube != info1
		OR  StartZCube != info2 )
		{
			StartXCube = info0 ;
			StartYCube = info1 ;
			StartZCube = info2 ;

			CameraCenter( 0 ) ;
			FirstTime = AFF_ALL_FLIP ;
		}
	}
	else
	{
		if( StartXCube != info0
		OR  StartYCube != info1
		OR  StartZCube != info2
		OR  SaveAlphaCam != AlphaCam
		OR  SaveBetaCam  != BetaCam
		OR  SaveGammaCam != GammaCam
		OR  SaveVueDistance != VueDistance )
		{
			StartXCube = info0 ;
			StartYCube = info1 ;
			StartZCube = info2 ;

			AlphaCam = ptrz->Info3 ;
			BetaCam  = ptrz->Info4 ;
			GammaCam = ptrz->Info5 ;
			VueDistance = ptrz->Info6 ;

			SaveCamera() ;

			CameraCenter( 0 ) ;
			FirstTime = AFF_ALL_FLIP ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	GereZoneMessage( T_OBJET *ptrobj, T_ZONE *ptrz )
{
	if( ActionNormal==1 )
	{
		S32	angle  ;
		S32	angle1 ;
		S32	flagmess = 0 ;
		S32	x, z ;

		x = ptrobj->Obj.X ;
		z = ptrobj->Obj.Z ;

		switch( ptrz->Info2 )
		{
			case 1:	// zone message NORD
				angle = GetAngle2D( x, z, ptrz->X0, ptrz->Z0 ) ;
				angle1 = GetAngle2D( x, z, ptrz->X1, ptrz->Z0 ) ;

				if( ptrobj->Obj.Beta>=angle1 AND ptrobj->Obj.Beta<=angle )
					flagmess = 1 ;

				break ;

			case 2: // zone message SUD
				angle = GetAngle2D( x, z, ptrz->X0, ptrz->Z1 ) ;
				angle1 = GetAngle2D( x, z, ptrz->X1, ptrz->Z1 ) ;

				if( ptrobj->Obj.Beta>=angle OR ptrobj->Obj.Beta<=angle1 )
					flagmess = 1 ;

				break ;

			case 4: // zone message EST
				angle = GetAngle2D( x, z, ptrz->X1, ptrz->Z0 ) ;
				angle1 = GetAngle2D( x, z, ptrz->X1, ptrz->Z1 ) ;

				if( ptrobj->Obj.Beta>=angle1 AND ptrobj->Obj.Beta<=angle )
					flagmess = 1 ;

				break ;

			case 8: // zone message OUEST
				angle1 = GetAngle2D( x, z, ptrz->X0, ptrz->Z0 ) ;
				angle = GetAngle2D( x, z, ptrz->X0, ptrz->Z1 ) ;

				if( ptrobj->Obj.Beta>=angle1 AND ptrobj->Obj.Beta<=angle )
					flagmess = 1 ;

				break ;
		}

		if( flagmess )
		{
			if( ptrz->Info1 )	// camera associee
			{
				S32	i ;
				T_ZONE 	*ptrzc = ListZone ;	// zone camera

				for( i=0; i<NbZones; i++, ptrzc++ )
				{
					if( ptrzc->Type==1 AND ptrzc->Num==ptrz->Info1 )	break ;
				}

				if( i<NbZones )
				{
					SetZoneCamera( ptrzc ) ;
					FlagCameraForcee = TRUE ;
					AffScene( AFF_ALL_FLIP ) ;
				}
			}

#ifdef	CDROM
			NumObjSpeak = NUM_PERSO ;
#endif
			NumObjDial = NUM_PERSO ;

			// au cas ou l'on soit appelé pendant un eclair
			Palette( PtrPal ) ;

			Dial( ptrz->Num, TRUE ) ;
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
S32	GereZoneChangeCube( T_OBJET *ptrobj, T_ZONE *ptrz )
{
	if( ((ptrobj->Move!=MOVE_BUGGY_MANUAL AND ptrobj->Move!=MOVE_BUGGY)
	     OR (TabArrow[MAX_OBJECTIF+ptrz->Num].FlagHolo&4))// Exterieur
	AND (ptrz->Info7&ZONE_ON) )
	{
		// protection meurt dans un autre cube
		if( ptrobj->LifePoint > 0 )
		{
			S32	newbeta ;
			S32	flagx = TRUE ;
			S32	flagz = TRUE ;
			S32	x, y, z ;

			x = ptrobj->Obj.X ;
			y = ptrobj->Obj.Y ;
			z = ptrobj->Obj.Z ;

			// Pour les zones ChangeCube en extérieur
			// on attend de rentrer en collision avec
			// la porte

			if( ptrz->Info5&ZONE_TEST_BRICK )
			{
				Rotate( ptrobj->XMin, ptrobj->ZMin, ptrobj->Obj.Beta + (896*MUL_ANGLE) + (512*MUL_ANGLE) ) ;
				X0 += x ;
				Z0 += z ;

				NumDecorsCol = -1 ;// cf WorldColBrickDecors()

				if( (X0 < 0) OR (Z0 < 0)
				OR (X0 > 64*SIZE_BRICK_XZ-1) OR (Z0 > 64*SIZE_BRICK_XZ-1)
				OR (WorldColBrickDecors( X0+ptrobj->XMin, y+ptrobj->YMin, Z0+ptrobj->ZMin,
							 X0+ptrobj->XMax, y+ptrobj->YMax, Z0+ptrobj->ZMax ) == 0) )
				{
					return FALSE ;
				}
			}
			else if( CubeMode==CUBE_EXTERIEUR
			AND  (TabArrow[MAX_OBJECTIF+ptrz->Num].FlagHolo&4) )
			{
				if( ptrz->Info2==(32768-1024) )// ZONE_NORD
				{
					if( !FlagHeroOutZ )	return FALSE ;
					NewPosZ = 32768-DEMI_BRICK_XZ ;
					flagz = FALSE ;
					FlagHeroOutZ = FALSE ;
				}

				if( ptrz->Info2==512 )// ZONE_SUD
				{
					if( !FlagHeroOutZ )	return FALSE ;
					NewPosZ = DEMI_BRICK_XZ ;
					flagz = FALSE ;
					FlagHeroOutZ = FALSE ;
				}

				if( ptrz->Info0==(32768-1024) )// ZONE_OUEST
				{
					if( !FlagHeroOutX )	return FALSE ;
					NewPosX = 32768-DEMI_BRICK_XZ ;
					flagx = FALSE ;
					FlagHeroOutX = FALSE ;
				}

				if( ptrz->Info0==512 )// ZONE_EST
				{
					if( !FlagHeroOutX )	return FALSE ;
					NewPosX = DEMI_BRICK_XZ ;
					flagx = FALSE ;
					FlagHeroOutX = FALSE ;
				}
			}

			NewCube = ptrz->Num ;
			NewPosY = y - ptrz->Y0 + ptrz->Info1 ;
			newbeta = (-ptrz->Info3*1024)&4095 ;

			LongRotate( x-ptrz->X0, z-ptrz->Z0, newbeta ) ;

			if( flagx )	NewPosX = ptrz->Info0 + X0 ;
			if( flagz )	NewPosZ = ptrz->Info2 + Z0 ;

			ptrobj->Obj.Beta += newbeta ;
			ptrobj->Obj.Beta &= 4095 ;

			if( ptrz->Info6&ZONE_DONT_REAJUST_POS_TWINSEN )	FlagReajustPosTwinsen = FALSE ;
			else						FlagReajustPosTwinsen = TRUE ;

			FlagChgCube = 1 ;
			return TRUE ;
		}
	}

	return FALSE ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	CheckZoneSce( T_OBJET *ptrobj, U8 numobj )
{
	S32	n ;
	T_ZONE	*ptrz ;
	S32	x,y,z ;
	S32	foundcamera = FALSE ;

	x = ptrobj->Obj.X ;
	y = ptrobj->Obj.Y ;
	z = ptrobj->Obj.Z ;

	if( y<0 )	y = 0 ;	// on ne teste pas les zones sous l'eau !

	ptrz = ListZone ;

	ptrobj->ZoneSce = -1 ;
	ptrobj->WorkFlags &= ~(DONT_PICK_CODE_JEU) ;

	if( ptrobj->Move==MOVE_WAGON )	ptrobj->PtrZoneRail = 0 ;

	if( numobj == NUM_PERSO )
	{
		FlagClimbing = FALSE ;
		PtrZoneClimb = NULL  ;
	}

	for( n=0; n<NbZones; n++, ptrz++ )
	{
		// bug resolu en testant les x1 et z1 avec '<' et non
		// '<=' car on se retrouvait dans 2 zones en même temps !!!
		if( x >= ptrz->X0
		AND x <  ptrz->X1
		AND y >= ptrz->Y0
		AND y <= ptrz->Y1
		AND z >= ptrz->Z0
		AND z <  ptrz->Z1 )
		{
			// si type test angle

			switch( ptrz->Type )
			{
			case 0:	// change cube perso
				if( numobj==NUM_PERSO AND !FirstLoop )
				{
					if( GereZoneChangeCube( ptrobj, ptrz ) )
					{
#ifdef	DEMO
						// Grosse Rustine pour la demo
						switch( NumCube )
						{
							case 183:
							case 184:
								if( NewCube<183 OR NewCube>184 )
								{
									NewDemoSave++ ;
								}
								break ;

							case 125:
								NewDemoSave++ ;
								break ;

							default:
								if( NewCube<42 OR NewCube>50 )
								{
									NewDemoSave++ ;
								}
						}
#endif
						return ;
					}
				}
				break ;

			case 1:	// set pos camera
				if( !foundcamera )
				{
					if( (numobj == NumObjFollow)
					AND (ptrz->Info7&ZONE_ON) )
					{
						if( ((ptrz->Info7&ZONE_OBLIGATOIRE)
						OR (CubeMode==CUBE_INTERIEUR)
						OR (!(ptrz->Info7&ZONE_ACTIVE) AND AllCameras)) )
						{
							SetZoneCamera( ptrz ) ;
							ptrz->Info7 |= ZONE_ACTIVE ;
						}

						foundcamera = TRUE ;
					}
				}
				break ;

			case 2:	// zone scenarique
				ptrobj->ZoneSce = ptrz->Num ;
				break ;

			case 4:	// zone giver
				if( numobj == NUM_PERSO )
				{
					if( ActionNormal==1 )
					{
						InitAnim( GEN_ANIM_ACTION, ANIM_ALL_THEN, NUM_PERSO ) ;
						ZoneGiveExtraBonus( ptrz ) ;
						ActionNormal = FALSE ;
					}
				}
				break ;

			case 5:	// zone message
				if( numobj == NUM_PERSO )
				{
					GereZoneMessage( ptrobj, ptrz ) ;
				}
				break ;

			case 6: // echelle
				if( ptrz->Info1 AND (numobj==NUM_PERSO)
				AND (PtrComportement->Flags&CF_CLIMB) )
				{
				PtrZoneClimb = ptrz ;

				if(    (Input&I_UP
				    OR (ptrobj->GenAnim == GEN_ANIM_MARCHE)
				    OR (ptrobj->GenAnim == GEN_ANIM_ECHELLE)
				    OR (ptrobj->GenAnim == GEN_ANIM_MONTE) )
				    AND ptrobj->FlagAnim!=ANIM_ALL_THEN )
				{
					// Blindage pour ne pas pouvoir monter sur
					// les montants en haut des echelles
					if( y<=ptrz->Y1-256 )
					{
						Rotate( ptrobj->XMin, ptrobj->ZMin, ptrobj->Obj.Beta + (896*MUL_ANGLE) + (512*MUL_ANGLE) ) ;
						X0 += Nxw ;
						Z0 += Nzw ;

						if( (X0 >= 0) AND (Z0 >= 0)
						AND (X0 <= 64*SIZE_BRICK_XZ-1) AND (Z0 <= 64*SIZE_BRICK_XZ-1)
						AND (PtrWorldColBrick( X0, y+256, Z0 ) != 0) )
						{
							FlagClimbing = CLIMBING_UP ;
							ptrobj->WorkFlags &= ~(FALLING) ;
							StartYFalling = 0 ;

							if( y >= ptrz->Y1-DELTAY_ANIM_ECHELLE )
							{
								ptrobj->Info3 = ptrz->Y1 ;// limite à ne pas depasser
								InitAnim( GEN_ANIM_ECHELLE, ANIM_ALL_THEN, NUM_PERSO ) ;
							}
							else
							{
								InitAnim( GEN_ANIM_MONTE, ANIM_REPEAT, NUM_PERSO ) ;
							}
						}
						else if( !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY)
						AND (PtrWorldColBrick( x, y-2, z ) == 0)
						AND ptrobj->CarryBy==-1 )
						{
							InitAnim( GEN_ANIM_TOMBE, ANIM_TEMPO, NUM_PERSO ) ;
							ptrobj->WorkFlags |= FALLING ;
							if( !StartYFalling )	StartYFalling = Nyw ;
						}
					}
					else if( !(ptrobj->WorkFlags&ANIM_MASTER_GRAVITY)
					AND (PtrWorldColBrick( x, y-2, z ) == 0)
					AND ptrobj->CarryBy==-1 )
					{
						InitAnim( GEN_ANIM_TOMBE, ANIM_TEMPO, NUM_PERSO ) ;
						ptrobj->WorkFlags |= FALLING ;
						if( !StartYFalling )	StartYFalling = Nyw ;
					}
				}
				else if( (Input&I_DOWN)
				    OR	 ( ptrobj->GenAnim==GEN_ANIM_RECULE )
				    OR   ( ptrobj->GenAnim==GEN_ANIM_ECHDESC)
				    OR   ( ptrobj->GenAnim==GEN_ANIM_DESCEND) )
				{
					Rotate( 10, 10, ptrobj->Obj.Beta + (896*MUL_ANGLE) + (512*MUL_ANGLE) ) ;
					X0 += Nxw ;
					Z0 += Nzw ;

					if( (X0 >= 0) AND (Z0 >= 0)
					AND (X0 <= 64*SIZE_BRICK_XZ-1) AND (Z0 <= 64*SIZE_BRICK_XZ-1) )
					{
						S32 col ;

						if( CubeMode==CUBE_INTERIEUR )
							col = PtrWorldColBrick( x, y-128, z ) ;
						else
						{
							// Trouver une autre méthode car c'est un pneu
							// crade !

							if( PtrWorldColBrick( X0, y, Z0 ) )
								col = PtrWorldColBrick( x, y-128, z ) ;
							else	col = PtrWorldColBrick( X0, y-128, Z0 ) ;
						}

						if( col==0 )
						{
							// voir si faut le mettre aussi en exterieur
							if( CubeMode==CUBE_INTERIEUR )
							{
								Rotate( ptrobj->XMin+1, ptrobj->ZMin+1, ptrobj->Obj.Beta + (896*MUL_ANGLE) + (512*MUL_ANGLE) ) ;
								X0 += Nxw ;
								Z0 += Nzw ;
								col = PtrWorldColBrick( X0, y-128, Z0 ) ;
							}
							else	col = 1 ;

							if( col )
							{
								ptrobj->WorkFlags &= ~(FALLING) ;
								FlagClimbing = CLIMBING_DOWN ;
								StartYFalling = 0 ;

								if( y <= ptrz->Y1-DELTAY_ANIM_ECHELLE )
								{
									InitAnim( GEN_ANIM_DESCEND, ANIM_REPEAT, NUM_PERSO ) ;
								}
								else
								{
									InitAnim( GEN_ANIM_ECHDESC, ANIM_ALL_THEN, NUM_PERSO ) ;
								}
							}
						}
						else
						{
							if( ptrobj->GenAnim==GEN_ANIM_DESCEND
							OR  ptrobj->GenAnim==GEN_ANIM_ECHDESC )
							{
								InitAnim( GEN_ANIM_TOMBE, ANIM_TEMPO, NUM_PERSO ) ;
								ptrobj->WorkFlags |= FALLING ;
								if( !StartYFalling )	StartYFalling = Nyw ;
							}
						}
					}
				}
				}
				break ;

			case 7: // Escalator
				if( ptrz->Info1
				AND ptrobj->CarryBy==-1 )
				{
					switch( ptrz->Info2 )
					{
						case 1:	// Tapis roulant NORD
							ptrobj->CodeJeu = CJ_ESCALATOR_NORD<<4 ;
							ptrobj->WorkFlags |= DONT_PICK_CODE_JEU ;
							break ;

						case 2:	// Tapis roulant SUD
							ptrobj->CodeJeu = CJ_ESCALATOR_SUD<<4 ;
							ptrobj->WorkFlags |= DONT_PICK_CODE_JEU ;
							break ;

						case 4:	// Tapis roulant EST
							ptrobj->CodeJeu = CJ_ESCALATOR_EST<<4 ;
							ptrobj->WorkFlags |= DONT_PICK_CODE_JEU ;
							break ;

						case 8:	// Tapis roulant OUEST
							ptrobj->CodeJeu = CJ_ESCALATOR_OUEST<<4 ;
							ptrobj->WorkFlags |= DONT_PICK_CODE_JEU ;
							break ;
					}
				}
				break ;

			case 8: // zone hit
				if( !ptrz->Info3 AND ptrz->Info1 )
				{
					if( ptrobj->LifePoint>0 )
						HitObj( numobj, numobj, ptrz->Info1, ptrobj->Obj.Beta ) ;

					ptrz->Info3 = ptrz->Info2*5*20 + TimerRefHR ;
				}
				else
				{
					if( TimerRefHR>=ptrz->Info3 )
						ptrz->Info3 = 0 ;
				}
				break ;

			case 9:	// zone Rail
				if( ptrobj->Move==MOVE_WAGON )
					ptrobj->PtrZoneRail = ptrz ;
				break ;
			}
		}
		else
		{
			switch( ptrz->Type )
			{
				case 1:	// zone camera
					if( numobj==NumObjFollow )
					{
						ptrz->Info7 &= ~(ZONE_ACTIVE) ;
					}
					break ;
			}
		}
	}
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	CheckBuggyZoneGiver( T_OBJET *ptrobj )
{
	S32	n ;
	T_ZONE	*ptrz ;
	S32	x,y,z ;

	// teste les zones giver pour le buggy au point devant le buggy
	Rotate( 0, ptrobj->ZMax, ptrobj->Obj.Beta ) ;

	x = ptrobj->Obj.X + X0 ;
	y = ptrobj->Obj.Y ;
	z = ptrobj->Obj.Z + Z0 ;

	ptrz = ListZone ;

	for( n=0; n<NbZones; n++, ptrz++ )
	{
		// on ne test que les zones giver et les zones messages
		if( ptrz->Type!=4
		AND ptrz->Type!=5 )	continue ;

		// bug resolu en testant les x1 et z1 avec '<' et non
		// '<=' car on se retrouvait dans 2 zones en même temps !!!
		if( x >= ptrz->X0
		AND x <  ptrz->X1
		AND y >= ptrz->Y0
		AND y <= ptrz->Y1
		AND z >= ptrz->Z0
		AND z <  ptrz->Z1 )
		{
			switch( ptrz->Type )
			{
				case 4:	// Giver
					if( ActionNormal==1 /*AND !ptrz->Info2*/ )
					{
						InitAnim( GEN_ANIM_ACTION, ANIM_ALL_THEN, NUM_PERSO ) ;
						ZoneGiveExtraBonus( ptrz ) ;
						ActionNormal = FALSE ;
					}
					break ;

				case 5:	// Message
					GereZoneMessage( ptrobj, ptrz ) ;
					break ;
			}
		}
	}
}
