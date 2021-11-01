#include	"c_extern.h"

#define	ZOOM_A	2048	//	Big Islands
#define	ZOOM_B	1024	//	Islands with 4 cubes
#define	ZOOM_C	768	//	Islands with 1 cube
#define	ZOOM_D	256
#define	ZOOM_E	128
#define	ZOOM_F	96

S32	BodyFactorScale[MAX_ISLAND] =
{
	ZOOM_A,	//  0	Citadelle
	ZOOM_A,	//  1	Puit Sendell
	ZOOM_A,	//  2	Desert
	ZOOM_B,	//  3	Lune
	ZOOM_A,	//  4	Otringal
	ZOOM_C,	//  5	Celebration
	ZOOM_B,	//  6	Blafard
	ZOOM_B,	//  7	Mosquibee
	ZOOM_B,	//  8	Knartas
	ZOOM_C,	//  9	Island CX
	ZOOM_C,	// 10	Elevator
	ZOOM_B	// 11	Under Celebration
} ;

S32	ArrowFactorScale[MAX_ISLAND] =
{
	ZOOM_D,	//  0	Citadelle
	ZOOM_D,	//  1	Puit Sendell
	ZOOM_D,	//  2	Desert
	ZOOM_E,	//  3	Lune
	ZOOM_D,	//  4	Otringal
	ZOOM_F,	//  5	Celebration
	ZOOM_E,	//  6	Blafard
	ZOOM_E,	//  7	Mosquibee
	ZOOM_E,	//  8	Knartas
	ZOOM_F,	//  9	Island CX
	ZOOM_F,	// 10	Elevator
	ZOOM_E	// 11	Under Celebration
} ;
//----------------------------------------------------------------------------
void	DrawTwinsenPlan( T_SORT *ptt )
{
	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

	ObjTwinsen.X = ptt->Xw		;
	ObjTwinsen.Y = ptt->Yw		;
	ObjTwinsen.Z = ptt->Zw		;
	ObjTwinsen.Alpha = 0		;
	ObjTwinsen.Beta = ListObjet[NUM_PERSO].Obj.Beta	;
	ObjTwinsen.Gamma = 0		;

	ObjectDisplay( &ObjTwinsen )	;

	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax )	;
}
//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
/*
void	DrawObjFix(T_SORT *ptt)
{
	S16	ymin	;
	S16	ymax	;
	S16	*buf	;

	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

//	buf = (S16*)HQR_Get( HQR_ObjFix, ptt->NumObj );
	buf = (S16*)GivePtrObjFix( ptt->NumObj );	//
	if ( !buf )	return			;

	ymin = *(buf+3)	;
	ymax = *(buf+4)	;

	ptt->Yw -= ymin	;

	BodyDisplay( ptt->Xw, ptt->Yw , ptt->Zw, 0, TimerRef*10, 0, buf);

	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax )	;

	ptt->Yw += ymax+1000	;//	Add ObjFix Size ZV YMax
}
*/
//----------------------------------------------------------------------------
void	DrawArrowPlan(T_SORT *ptt)
{
	T_BODY_HEADER	*buf	;
	S32	ymax	;

	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

	if ( ptt->Num == 1 )	buf = (T_BODY_HEADER*)BufFleche		;//	Big
	else			buf = (T_BODY_HEADER*)BufLoFleche	;//	Small

	ymax = buf->YMax	;

	BodyDisplay( ptt->Xw, ptt->Yw, ptt->Zw, 0, 0, 0, buf )	;

	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax )	;

	ptt->Yw += ymax + 1000	;//	Add Arrow Size (ZV YMax)

//	if ( ptt->NumObj != -1 )	DrawObjFix( ptt )	;// An object on me!

	if ( ptt->Num == 2 )		DrawTwinsenPlan( ptt )	;// Twinsen is on me !
}
//----------------------------------------------------------------------------
void	DrawBuggyPlan(T_SORT *ptt)
{
	S32	a, b, g	;

	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

	a = ListBuggy[0].Alpha	;
	b = ListBuggy[0].Beta	;
	g = ListBuggy[0].Gamma	;

	BodyDisplay( ptt->Xw, ptt->Yw+(TimerRefHR&0xFF), ptt->Zw, a, b, g, BufBuggy )	;

	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax )	;
}
//----------------------------------------------------------------------------
void	DrawDynoPlan(T_SORT *ptt)
{
	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

	BodyDisplay( ptt->Xw, ptt->Yw+((TimerRefHR&0xF00)>>8), ptt->Zw, 0, 0, 0, BufDyno )	;

	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax )	;
}
//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
S32	RecupCoorTwinsen( S32 *xt, S32 *yt, S32 *zt )
{
	S32	mode=0	;

	if ( TabArrow[MAX_OBJECTIF+NumCube].FlagHolo & 4 )// Exterieur
	{
		mode = 1	;
	}

	if ( TwinsenLinkArrow != -1 )//	Twinsen link with an Arrow
	{
		*xt = TabArrow[TwinsenLinkArrow].X	;
		*yt = TabArrow[TwinsenLinkArrow].Y	;
		*zt = TabArrow[TwinsenLinkArrow].Z	;
		return(mode)				;
	}

	if ( mode == 1 )// Exterieur
	{//	Twinsen Outside
		*xt = ListObjet[NUM_PERSO].Obj.X+(CurrentCubeX*SCE)	; // TEMPO
		*yt = ListObjet[NUM_PERSO].Obj.Y		;
		*zt = ListObjet[NUM_PERSO].Obj.Z+(CurrentCubeY*SCE)	;

		return(1)		;//	or mode, same thing
	}

//	Twinsen Inside
	*xt = TabArrow[MAX_OBJECTIF+NumCube].X	;
	*yt = TabArrow[MAX_OBJECTIF+NumCube].Y	;
	*zt = TabArrow[MAX_OBJECTIF+NumCube].Z	;

	return(0)	;//	or mode...
}
//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
/*
typedef struct {

	S32	X ;
	S32	Y ;
	S32	Z ;
	S32	Cube ;
	S32	Beta	;
	S32	Alpha	;
	S32     Gamma	;

	S_ONE_WHEEL	Wheel[4] ;

	S32		BetaWheel ;

	S32		SpeedInc ;
	S32		SpeedRot ;

	S32		Speed ;

//	S32		YSpeed ;
//	S32		LastYs ;
//	S32		LastSpeedLevel ;

	S32		LastTimer ;

		} S_BUGGY ;
*/

//	Le buggy ne peut se trouver que dans le cube 49 sur la citadelle,
//	et dans les cubes 55 a 73 sur le desert ( dans ce cas, on l'affiche ).
S32	RecupCoorBuggy( S32 *xb, S32 *yb, S32 *zb )
{
	S32	cube, cx, cz	;

	if (( !NumBuggy )
	OR ( ListObjet[0].WorkFlags & MANUAL_INTER_FRAME ))
	{//	Never Init or Twinsen IN Buggy
		return(0)	;
	}

	cube = ListBuggy[0].Cube	;

	if (( cube < 55 ) OR ( cube > 73 ))	return(0)	;

	cx = (7+(cube-54)/5) * SCE	;
	cz = (7+(cube-54)%5) * SCE	;

	*xb = ListBuggy[0].X+cx	;
	*yb = ListBuggy[0].Y	;
	*zb = ListBuggy[0].Z+cz	;

	return(1)		;
}
//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#define	MAX_DYNO_POS	4
typedef	struct
{
	S32	Ile	;
	S32	X	;
	S32	Y	;
	S32	Z	;
	S32	Beta	;

} T_DYNO_POS	;
T_DYNO_POS	TabDynoPos[MAX_DYNO_POS] =
{
	0, 10784+( 8*SCE), 2000, 14640+(10*SCE), 1024,// Citadelle Twinsen, 49
	0, 16114+( 6*SCE), 7250, 26473+( 8*SCE), 3072,// Citadelle Dome, 44
	2, 9234 +( 7*SCE),  850,  7235+( 8*SCE), 3072,// Desert Oasis, 55
	2, 22203+(10*SCE), 4791, 13591+(11*SCE), 431  // Desert Ilot, 73
} ;
//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
S32	RecupCoorDyno( S32 *xd, S32 *yd, S32 *zd )
{
	S32	dyno	;
#ifdef	LBA_EDITOR
	dyno = ListVarGame[FLAG_DINO_VOYAGE].Value-1	;
#else
	dyno = ListVarGame[FLAG_DINO_VOYAGE]-1		;
#endif
	if (( dyno < 0 ) OR ( dyno >= MAX_DYNO_POS ))	return(0)	;
	if ( ZoomedIsland != TabDynoPos[dyno].Ile )	return(0)	;

	*xd = TabDynoPos[dyno].X	;
	*yd = TabDynoPos[dyno].Y	;
	*zd = TabDynoPos[dyno].Z	;

	return(1)	;//	TempO
}
//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
void	DrawListHoloPlan()
{
	S32	xt, yt, zt	;
	S32	xb, yb, zb	;
	S32	xd, yd, zd	;
	S32	alt	;
	T_ARROW *pt	;
	T_SORT	*ptt	;
	S32	arrow	;
	S32	nbtri=0	;
	U32	timer	;
	U32	angle	;

	timer = TimerRefHR*2	;

	if (( NumObjectif != OldObjectif )
	AND ( DecalTimerRef[NumObjectif] == 3072 ))
	{
		DecalTimerRef[NumObjectif] -= timer	;
	}

	ptt = ListTri			;

	pt = &TabArrow[MAX_OBJECTIF]	;

	for ( arrow = MAX_OBJECTIF ; arrow < MAX_OBJECTIF+MAX_CUBE ; arrow++, pt++ )
	{
		if (( !(pt->FlagHolo & 1))
		OR  ( pt->Island != ZoomedIsland )) continue ;

		alt = pt->Y	;

		if ( arrow == NumObjectif )
		{
			angle = (DecalTimerRef[arrow]+timer)&4095	;
		}
		else
		{
			angle = DecalTimerRef[arrow]&4095	;

			if ( angle != 3072 )
			{
				angle = (angle+timer)&4095	;

				if ( abs (angle-3072) < 64 )
					DecalTimerRef[arrow] = 3072	;
			}
		}

		alt += 2048+((SinTab[ angle ]) >> 3 )	;

		if ( TwinsenLinkArrow == arrow ) ptt->Num = 2	;// Small Arrow
		else				 ptt->Num = 1	;// Big Arrow

		LongWorldRotatePoint( pt->X, alt, pt->Z )	;

		ptt->NumObj = pt->ObjFix;
		ptt->Z = Z0		;
		ptt->Xw = pt->X		;
		ptt->Yw = alt		;
		ptt->Zw = pt->Z		;
		ptt++			;
		nbtri++			;
	}

//----- Here I must (eventually) add twinsen to ListTri...

	if (( TwinsenLinkArrow == -1 )//		Not on an arrow
	AND ( ZoomedIsland == Island ))//	Twinsen is here!
	{
		RecupCoorTwinsen( &xt, &yt, &zt )	;
		LongWorldRotatePoint( xt, yt, zt )	;
		ptt->Num = 0	;//	Type Twinsen
		ptt->Z = Z0	;
		ptt->Xw = xt	;
		ptt->Yw = yt	;
		ptt->Zw = zt	;
		ptt++		;
		nbtri++		;
	}

//----- Here I must (eventually) add the Buggy to ListTri...

	if (( ZoomedIsland == 2 )//	Separe cause Debug
	AND ( RecupCoorBuggy( &xb, &yb, &zb )))//	Desert + Buggy
	{
		LongWorldRotatePoint( xb, yb, zb )	;
		ptt->Num = 3	;//	Type Buggy
		ptt->Z = Z0	;
		ptt->Xw = xb	;
		ptt->Yw = yb	;
		ptt->Zw = zb	;
		ptt++		;
		nbtri++		;
	}

//----- Here I must (eventually) add the Dyno to ListTri...

	if ( RecupCoorDyno( &xd, &yd, &zd ))//	Desert ou Citadelle
	{
		LongWorldRotatePoint( xd, yd, zd )	;
		ptt->Num = 4	;//	Type Dyno
		ptt->Z = Z0	;
		ptt->Xw = xd	;
		ptt->Yw = yd	;
		ptt->Zw = zd	;
		ptt++		;
		nbtri++		;
	}

//----- Now we sort all thoose stuffs

	qsort( ListTri, nbtri, sizeof( T_SORT ), z_sort32 )	;

	ptt = ListTri	;

	for( arrow = 0 ; arrow < nbtri ; arrow++, ptt++ )
	{
		switch( ptt->Num )
		{
		case 0:
			DrawTwinsenPlan( ptt )	;
			break			;
		case 1:
		case 2:
			DrawArrowPlan( ptt )	;
			break			;
		case 3:
			DrawBuggyPlan( ptt )	;
			break			;
		case 4:
			DrawDynoPlan( ptt )	;
			break			;
		}
	}
}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
void	InitHoloPlan()
{
	S32	buffer[20]	;//	M3D  36 octets, 9 LONG
	S32	*pt		;
	S32	orgmx, orgmz 	;
	S32	offx, offz 	;
	S32	alpha, beta	;
	S32	lalpha, lbeta	;
	S32	distance	;
	S32	id		;

	InitHoloMalloc()	;

	BufBodyTempo = (U8*)HoloMalloc(SizeCurrentBody)	;
	BufAnimTempo = (U8*)HoloMalloc(SizeCurrentAnim)	;

	if (( ZoomedIsland == 0 ) AND ( TEMPETE_FINIE ))
	{
		id = HQR_BEGIN_MAP+(2*12)		;//	Citabau
	}
#ifdef	LBA_EDITOR
	else if( ( ZoomedIsland == 5 ) AND ( ListVarGame[FLAG_CELEBRATION].Value ) )
#else
	else if( ( ZoomedIsland == 5 ) AND ( ListVarGame[FLAG_CELEBRATION] ) )
#endif
	{
		id = HQR_BEGIN_MAP+(2*13)		;//	Celebra2
	}
	else	id = HQR_BEGIN_MAP+(2*ZoomedIsland)	;

	Load_HQR( HOLO_HQR_NAME, Log, id )	;

	Load_HQR( HOLO_HQR_NAME, buffer, id+1 )	;

	AffUpperLeft( 0, 0, 0 )		;
	AffLowerLeft( 0, 479, 0 )	;
	AffUpperRight( 639, 0, 0 )	;
	AffLowerRight( 639, 479, 0 )	;

	CopyScreen( Log, Screen )	;

	//	To avoid Zoom with Window
	if ( !FlagMap )	HoloInitDialWindow()	;

	pt = buffer		;

	orgmx = *pt++		;
	orgmz = *pt++		;
	offx = *pt++		;
	offz = *pt++		;
	alpha = *pt++		;
	beta = *pt++		;
	distance = *pt++	;
	lalpha = *pt++		;
	lbeta = *pt++		;

	orgmx = (orgmx*SCE)+offx;
	orgmz = (orgmz*SCE)+offz;

	BufFleche = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_FLECHE )	;
	BufLoFleche = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_LOFLECHE );
	BufBuggy = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_BUGGY )	;
	BufDyno = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_DYNO )	;

	ScaleRotBody3D( BufFleche, ArrowFactorScale[ZoomedIsland], FALSE )  ;
	ScaleRotBody3D( BufLoFleche, ArrowFactorScale[ZoomedIsland], FALSE );

	ScaleRotBody3D( BufBuggy, SCALE_BUGGY_PLAN, FALSE )		;
	ScaleRotBody3D( BufDyno, SCALE_DYNO_PLAN, FALSE )		;

//	SetProjection( 320, 240, 1024, 1024, 1024 ) 			;
	SetProjection( 320, 240, 1024, 700, 700 ) 			;
	SetFollowCamera( orgmx, 0, orgmz, alpha, beta, 0, distance ) 	;
	SetLightVector( lalpha, lbeta, 0 ) 				;

//------------------------------
//----- Recuperation body current de Twinsen et size dans HQR
	memcpy( BufBodyTempo, PtCurrentBody, SizeCurrentBody )	;
	memcpy( BufAnimTempo, PtCurrentAnim, SizeCurrentAnim )	;

	ScaleRotBody3D( BufBodyTempo, BodyFactorScale[ZoomedIsland], FALSE )	;
	ScaleRotAnim3D( BufAnimTempo, BodyFactorScale[ZoomedIsland], FALSE )	;

	ObjectClear( &ObjTwinsen )		;

	ObjectInitBody( &ObjTwinsen, 0 )	;
	ObjectInitAnim( &ObjTwinsen, 0 )	;
//------------------------------

	NumOldMess = -1		;

}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
void	ClearHoloPlan()
{
/*      Unused depuis HoloMalloc

	if ( BufDyno )		Free(BufDyno)		;
	BufDyno = 0L					;

	if ( BufBuggy )		Free(BufBuggy)		;
	BufBuggy = 0L					;

	if( BufLoFleche )	Free(BufLoFleche)	;
	BufLoFleche = NULL	;

	if( BufFleche )		Free(BufFleche)		;
	BufFleche = NULL ;
*/
}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//	Oblige to found one, because user can zoom on this island
void	SearchFirstValidObjectifIsland()
{
	S32 	i	;
	T_ARROW	*pt	;

	pt = &TabArrow[MAX_OBJECTIF]	;

	for ( i = MAX_OBJECTIF ; i < MAX_OBJECTIF+MAX_CUBE ; i++, pt++ )
	{
		if (( pt->FlagHolo & 1 )
		AND ( pt->Island == ZoomedIsland ))
		{
			NumObjectif = i	;
			return		;
		}
	}
}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//	Oblige to found one, because user can zoom on this island
S32	FindNearestOutsideArrow( S32 xt, S32 yt, S32 zt )
{
	S32 	i	;
	T_ARROW	*pt	;
	S32	min=9999;
	S32	dist	;
	S32	arrow=-1;

	pt = &TabArrow[MAX_OBJECTIF]	;

	for ( i = MAX_OBJECTIF ; i < MAX_OBJECTIF+MAX_CUBE ; i++, pt++ )
	{
		if (( pt->FlagHolo & 1 )
		AND ( pt->Island == ZoomedIsland ))
		{
			dist = Distance3D( pt->X, pt->Y, pt->Z, xt, yt, zt ) ;
			if ( dist < min )
			{
				arrow = i	;
				dist = min	;
			}
		}
	}

	if (( arrow != -1 )
	AND ( dist <= DISTANCE_EXT ))	return(arrow)	;

	return(-1)	;
}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

void	InitObjectifIsland()
{
	S32	xt,yt,zt;
	S32	arrow	;
	T_ARROW	*pt	;

	NumObjectif = -1	;//	None by default
	OldObjectif = -2	;//	For AffReticule
	TwinsenLinkArrow = -1	;//	Alone by default

	if ( ZoomedIsland == Island )//	Twinsen is here = Objectif
	{
		if ( RecupCoorTwinsen( &xt, &yt, &zt ))//	Outside
		{
			arrow = FindNearestOutsideArrow( xt, yt, zt )	;

			if ( arrow != -1 )
			{
				NumObjectif = arrow	;
				TwinsenLinkArrow = arrow;
			}
			else	NumObjectif = 1000	;// Twinsen
		}
		else					// 	Inside
		{
			if ( TabArrow[MAX_OBJECTIF+NumCube].FlagHolo & 1 )
			{
				NumObjectif = MAX_OBJECTIF+NumCube	;
				TwinsenLinkArrow = NumObjectif		;
			}
			else
			{
				NumObjectif = 1000			;// Twinsen
			}
		}


	}
	else	SearchFirstValidObjectifIsland()	;

	if ( NumObjectif != 1000 )
	{
		pt = &TabArrow[NumObjectif]	;
		xt = pt->X			;
		yt = pt->Y			;
		zt = pt->Z			;
	}

	LongWorldRotatePoint( xt, yt, zt )	;
	LongProjectPoint( X0, Y0, Z0 )		;

	XpObjectif = Xp	;
	YpObjectif = Yp	;

	SizeRet = 16			;//	Size Reticule First Time

}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
void	InitAngleArrow()
{
	S32	arrow	;

	for ( arrow = 0 ; arrow < MAX_OBJECTIF+MAX_ARROW ; arrow++ )
	{
		DecalTimerRef[arrow] = 3072	;
	}
}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//	0:Left  1:Up  2:Right  3:Down
void	SearchObjectifIsland(S32 dir)
{
	T_ARROW	*pt	;
	S32	i, next	;
	S32	x, y, dd;
//	S32	dx, dy	;
	S32	min=9999;
	S32	xt,yt,zt;
	S32	flag=0	;

	next = -1	;

	pt = &TabArrow[MAX_OBJECTIF]	;

//----- Scan Objectif Arrow

	for ( i = MAX_OBJECTIF ; i < MAX_OBJECTIF+MAX_CUBE ; i++, pt++ )
	{
		if (( !(pt->FlagHolo & 1))
		OR ( i == NumObjectif )
		OR ( pt->Island != ZoomedIsland ))	continue;

		LongWorldRotatePoint( pt->X, pt->Y, pt->Z )	;
		LongProjectPoint( X0, Y0, Z0 )			;

//		dx = abs(Xp-XpObjectif)	;
//		dy = abs(Yp-YpObjectif)	;

		dd = Distance2D( Xp, Yp, XpObjectif, YpObjectif );

		if ( dir & 1 )//	Left
		{
			if ( Xp > XpObjectif )	continue	;
//			if ( dy > dx )		continue	;
		}
		if ( dir & 2 )//	Up
		{
			if ( Yp > YpObjectif )	continue	;
//			if ( dx > dy )		continue	;
		}
		if ( dir & 4 )//	Right
		{
			if ( Xp < XpObjectif )	continue	;
//			if ( dy > dx )		continue	;
		}
		if ( dir & 8 )//	Down
		{
			if ( Yp < YpObjectif )	continue	;
//			if ( dx > dy )		continue	;
		}

		if ( dd < min )
		{
			min = dd	;
			next = i	;
			x = Xp		;
			y = Yp		;
		}
	}


	if (( ZoomedIsland == Island )//	Twinsen is here!
	AND ( NumObjectif != 1000 )//	He is not the current objectif
	AND ( TwinsenLinkArrow == -1 ))//	And he isn't linked with an arrow
	{
		RecupCoorTwinsen( &xt, &yt, &zt )	;

		LongWorldRotatePoint( xt, yt, zt )	;
		LongProjectPoint( X0, Y0, Z0 )		;

//		dx = abs(Xp-XpObjectif)	;
//		dy = abs(Yp-YpObjectif)	;

		dd = Distance2D( Xp, Yp, XpObjectif, YpObjectif );

		if ( dir & 1 )//	Left
		{
			if ( Xp > XpObjectif )	flag = 1	;
//			if ( dy > dx )		flag = 1	;
		}
		if ( dir & 2 )//	Up
		{
			if ( Yp > YpObjectif )	flag = 1	;
//			if ( dx > dy )		flag = 1	;
		}
		if ( dir & 4 )//	Right
		{
			if ( Xp < XpObjectif )	flag = 1	;
//			if ( dy > dx )		flag = 1	;
		}
		if ( dir & 8 )//	Down
		{
			if ( Yp < YpObjectif )	flag = 1	;
//			if ( dx > dy )		flag = 1	;
		}

		if (( !flag ) AND ( dd < min ))
		{
			min = dd	;
			next = 1000	;//	Twinsen
			x = Xp		;
			y = Yp		;
		}
	}

	if ( next == -1 )	return	;//	Keep the same

	NumObjectif = next		;
	XpObjectif = x			;
	YpObjectif = y			;
}
//ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
void	HoloPlan(S32 numplan)
{
	S32	x0, y0, x1, y1	;
	S32	testrelease=0	;
	S32	testnofire=TRUE	;
//	U32	mkey=0, mfire=0	;
//	U32	mjoy=0, dir	;
	U32	dir, timer	;

	UnsetClip()		;

	ZoomedIsland = numplan	;

	HoloMode = 1		;

	InitHoloPlan()		;

	InitObjectifIsland()	;

	InitAngleArrow()	;

	if ( !FlagMap ) BoxStaticAdd( 0, 0, 639, 479 )	;//	Full Screen

	while( !FlagHoloEnd )
	{
		ManageTime() ;
		MyGetInput() ;

		if (( MyKey == K_ESC )
		OR ( Input&(I_MENUS|I_HOLOMAP) ))
		{
			FlagHoloEnd = TRUE	;
			break			;
		}

		if( testrelease )
		{
			if ( !(Input&I_JOY) )
			{
				testrelease = 0	;
			}
		}
		else if( testnofire )
		{
			if( !(Input&I_FIRE) )
				testnofire = FALSE ;
		}
		else if ( !FlagMap )
		{
			if (( Input & I_ACTION_M ) OR ( Input & I_RETURN ))
			{
				break	;
			}

			if ( Input&I_JOY )
			{
				dir = 0	;

				if( Input & I_LEFT )	dir |= 1	;
				if( Input & I_UP )	dir |= 2	;
				if( Input & I_RIGHT )	dir |= 4	;
				if( Input & I_DOWN )	dir |= 8	;

				if ( dir )
				{
					SearchObjectifIsland(dir)	;
					testrelease = 1			;
				}
			}
		}

		DoAnimHolo()		;

		DrawListHoloPlan()	;

		if ( FlagMap )
		{
			CopyScreen( Log, BufSpeak )	;
			UnsetClip()			;
			MemoTimerRot = TimerRefHR	;

			// pour virer rsidus non restaurs venants de
			//      HoloGlobe()
			BoxReset()	;

			while(TRUE)
			{
				ManageTime() ;
				timer = TimerRefHR-MemoTimerRot	;
				x0 = BoundRegleTrois( CENTERX-8, 0, 1000, timer )	;
				y0 = BoundRegleTrois( CENTERY-8, 0, 1000, timer )	;
				x1 = BoundRegleTrois( CENTERX+8, 639, 1000, timer )	;
				y1 = BoundRegleTrois( CENTERY+8, 479, 1000, timer )	;

				if ( x0 == 0 )
				{
					break		;// Before Crash!
				}

				ScaleBox( 0, 0, 639, 479, BufSpeak, x0, y0, x1, y1, Log )	;

				AffUpperLeft( x0, y0, 0 )	;
				AffLowerLeft( x0, y1, 0 )	;
				AffUpperRight( x1, y0, 0 )	;
				AffLowerRight( x1, y1, 0 )	;

				BoxStaticAdd( x0, y0, x1, y1 )	;// Optimisable
				BoxUpdate()			;
			}
			CopyScreen( BufSpeak, Log )	;
			HoloInitDialWindow()		;
			BoxStaticFullflip();
			FlagMap = 0	;
			//	To avoid Jumping Arrow
			SetTimerHR( MemoTimerRot )	;
			InitWaitNoInput( I_FIRE )	;
		}


		UnsetClip()	;

		AffReticule()	;

		AffHoloMess()	;

#ifndef	LBA_EDITOR
		CheckSaveLogPcx( PtrPal ) ;// F9: Sauve Screen dans PATH_PCX_SAVE
#endif

		BoxUpdate()	;

		OldObjectif = NumObjectif	;

		if( FlagFade )	// Fade aprs 1er affichage
		{
			MemoTimerRot = TimerRefHR	;
			FadeToPal( PtrPal ) 	;
			SetTimerHR(MemoTimerRot )	;
			FlagFade = FALSE 		;
		}

//-------------
/*UnsetClip()					;
CoulText( 255, 0 )				;
Text( 0, 0, "%F%D     ", NbFramePerSecond )	;
CmptFrame++					;
*/
	}

	ClearHoloPlan()	;

	InitWaitNoInput( I_RETURN|I_FIRE|I_MENUS|I_HOLOMAP )	;
}
//----------------------------------------------------------------------------

