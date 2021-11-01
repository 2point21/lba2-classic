//
//	TEST VERSION ZBUFFER PAR 
//

#include	"c_extern.h"	// include LBA2

//────────────────────────────────────────────────────────────────────────────
//	Concerning BODY and ANIM
T_OBJ_3D ObjTwinsen	;

U8	*BufFleche	;
U8	*BufLoFleche	;
U8	*BufBuggy	;
U8	*BufDyno	;

U8	*BufBodyTempo	;//	Buffer Tempo Traffiquer Body ( scale & rot )
U8	*BufAnimTempo	;//	Buffer Tempo Traffiquer Body ( scale & rot )

U8	*PtCurrentBody	;
U8	*PtCurrentAnim	;

U32	SizeCurrentBody	;
U32	SizeCurrentAnim	;

//────────────────────────────────────────────────────────────────────────────
//	Concerning OBJECTIF
S32	NumObjectif=-1	;
S32	NextObjectif=-1	;
S32	OldObjectif=-2	;
U32	DistObjectif	;
S32	XpObjectif	;
S32	YpObjectif	;
S32	XpNextObjectif	;
S32	YpNextObjectif	;

//────────────────────────────────────────────────────────────────────────────
//	Divers

S32	ZBufYMin = 0		;
S32	ZBufYMax = 0		;

S32	HoloAlpha = 0		;
S32	HoloBeta = 0		;
S32	HoloGamma = 0		;

S32	DestAlpha 		;
S32	DestBeta		;
S32	TwinsenLinkArrow=0	;//	Twinsen Linked with an arrow ?

S32	ZoomedIsland=-1		;
U32	RayonPlanet		;
S32	FlagMap = 0		;
S32	FlagHoloEnd = 0		;


U16	*PtrMapping		;
U16	*PtrGlobe		;
U8	*PtrAlt			;
U8	*PtrTextMap		;
U16	*PtrCoorGlobe		;
U16	PtrTri[17*33*6]		;//	For 1 Planet
S32	ListTriPlanet[MAX_PLANET*2]	;//	For 6 Planets
S32	NbTri			;//	Nb Planet(s) in ListTriPlanet
S32	ModeAuto = 0		;
S32	XPos, YPos, ZPos	;
S32	XDest, YDest, ZDest	;
S32	ZoomDest=ZOOM_TWINSUN	;
S32	ZoomPlanet		;
S32	ZoomPlanetDest  	;
S32	Destination=P_TWINSUN	;
U32	MyTimerRef		;
S32	SizeRet = 24		;
S32	ColRet = 143		;
U32	MemoTimerRet		;
U32	MemoTimerRot		;
S32	DialStatHolo = -1	;
U8	RotPal[(32+31)*3] 	;
S16	RotPalPos = 0 		;
S32	NumOldMess=-1		;
S32	HoloMode=0		;//	0 Globe, 1 Plan
//────────────────────────────────────────────────────────────────────────────
/*		A R R O W

	Explanation about TabArrow[].FlagHolo

	Bit 0	Unactiv / Activ
	Bit 1	Never asked / asked
	Bit 2	Inside Mode / Outside Mode

*/
T_ARROW	TabArrow[MAX_OBJECTIF+MAX_CUBE]		;
U32	DecalTimerRef[MAX_OBJECTIF+MAX_CUBE] 	;
//────────────────────────────────────────────────────────────────────────────
//	Objects Scale factor by planet
S32	TabRayon[MAX_PLANET] = { 1000, 250, 500, 500 }	;
S32	TabScale[MAX_PLANET] = { 8, 2, 5, 5 }		;
S32	TabZoom[MAX_PLANET] = { 8000, 2000, 5000, 5000 };
//────────────────────────────────────────────────────────────────────────────
//		S T A R S
T_STARS	TabStars[MAX_STARS]	;//	3200 bytes

//────────────────────────────────────────────────────────────────────────────

T_PLANET	TabPlanet[MAX_PLANET]	;

//────────────────────────────────────────────────────────────────────────────
//		U F O

U8		*PtUfo		;
U8		*PtAnimUfo	;
T_OBJ_3D        Ufo		;

//────────────────────────────────────────────────────────────────────────────
S32	SetHoloPos( S32 num )
{
	if ( !(TabArrow[MAX_OBJECTIF+num].FlagHolo & 2 ))	 //	Bit 1=0 Never asked
	{
		TabArrow[MAX_OBJECTIF+num].FlagHolo |= 1	;//	Bit 0=1 (Activ) & 1=1 (Asked)

		if ( TabArrow[MAX_OBJECTIF+num].Island != Island )
		{
			HoloMode = 0	;//	Globe Next Time
		}

		return TRUE 	;
	}

	return FALSE ;
}
//────────────────────────────────────────────────────────────────────────────
void	ClrHoloPos( S32 num )
{
	TabArrow[MAX_OBJECTIF+num].FlagHolo &= ~(1) ;//	Clear bit 0 (Unactiv)
	TabArrow[MAX_OBJECTIF+num].FlagHolo |= 2    ;//	Set bit 1 (Already Asked)
}
//────────────────────────────────────────────────────────────────────────────
static void	ClearZBufMinMax( void )
{
	ZBufYMin = 479 	;
	ZBufYMax = 0 	;
}
//────────────────────────────────────────────────────────────────────────────
static void	AdjustZBufMinMax( void )
{
	if( ScreenYMin<ZBufYMin )	ZBufYMin = ScreenYMin 	;
	if( ScreenYMax>ZBufYMax )	ZBufYMax = ScreenYMax 	;
	if( ZBufYMin<0 )		ZBufYMin = 0 		;
	if( ZBufYMax>479 )		ZBufYMax = 479 		;
}

//────────────────────────────────────────────────────────────────────────────
//────────────────────────────────────────────────────────────────────────────
//────────────────────────────────────────────────────────────────────────────
//────────────────────────────────────────────────────────────────────────────
void	AffUpperLeft( S32 x, S32 y, S32 flag )
{
	Line( x, y, x+SX, y, ColRet )		;
	Line( x, y+1, x+SX-1, y+1, ColRet )	;
	Line( x, y, x, y+SY, ColRet )		;
	Line( x+1, y, x+1, y+SY-1, ColRet )	;

	if ( flag )	BoxMovingAdd( x, y, x+SX, y+SY )	;//	U L
}
//────────────────────────────────────────────────────────────────────────────
void	AffLowerLeft( S32 x, S32 y, S32 flag )
{
	Line( x, y, x+SX, y, ColRet )		;
	Line( x, y-1, x+SX-1, y-1, ColRet )	;
	Line( x, y, x, y-SY, ColRet )		;
	Line( x+1, y, x+1, y-(SY-1), ColRet)	;

	if ( flag )	BoxMovingAdd( x, y-SY, x+SX, y )	;//	L L
}
//────────────────────────────────────────────────────────────────────────────
void	AffUpperRight( S32 x, S32 y, S32 flag )
{
	Line( x, y, x-SX, y, ColRet )		;
	Line( x, y+1, x-(SX-1), y+1, ColRet)	;
	Line( x, y, x, y+SY, ColRet )		;
	Line( x-1, y, x-1, y+SY-1, ColRet )	;

	if ( flag )	BoxMovingAdd( x-SX, y, x, y+SY )	;//	U R
}
//────────────────────────────────────────────────────────────────────────────
void	AffLowerRight( S32 x, S32 y, S32 flag )
{
	Line( x, y, x-SX, y, ColRet )		;
	Line( x, y-1, x-(SX-1), y-1, ColRet)	;
	Line( x, y, x, y-SY, ColRet )		;
	Line( x-1, y, x-1, y-(SY-1), ColRet)	;

	if ( flag )	BoxMovingAdd( x-SX, y-SY, x, y )	;//	L R
}
//────────────────────────────────────────────────────────────────────────────

//	Pas gegene, je reve d'autre chose, mais quoi ?
void	AffReticule()
{
	S32	x0, y0	;
	S32	x1, y1	;

	if ( NumObjectif != OldObjectif )//	if -1 reinit ok
	{
		SizeRet = 128		;// Replace by Regle 3 Timer
		MemoTimerRet = TimerRefHR	;
	}

	if ( NumObjectif == -1 ) return	;//	Invalid

	if ( SizeRet > 16 )
	{
		SizeRet = BoundRegleTrois( 80, 16, 20*20, TimerRefHR-MemoTimerRet)		;
	}// No flash if FlagMap ( I like flashing 1/3  2/3 ;)
	else if (( !FlagMap ) AND ( ((TimerRefHR) & 0xFF ) < 5*20 ))	return	;

	x0 = XpObjectif-SizeRet	;
	y0 = YpObjectif-SizeRet	;
	x1 = XpObjectif+SizeRet	;
	y1 = YpObjectif+SizeRet	;

	UnsetClip()		;

	AffUpperLeft( x0, y0, 1 )	;
	AffLowerLeft( x0, y1, 1 )	;
	AffUpperRight( x1, y0, 1 )	;
	AffLowerRight( x1, y1, 1 )	;
}
//────────────────────────────────────────────────────────────────────────────

void	AffHoloMess()
{
	S32	mess=-1	;
	U32	dx	;

	if ( NumObjectif != OldObjectif )//	It's a new One!
	{
		if (( NumObjectif == 1000 )
		OR ( NumObjectif == -1 ))
		{
			mess = TabArrow[Island].Mess	;// Current Island
		}
		else
		{
			mess = TabArrow[NumObjectif].Mess;
		}
		if ( NumOldMess != mess )
		{
			DialStatHolo = -1	;// Run, Baby Run...
			NumOldMess = mess	;
		}
	}

	switch( DialStatHolo )
	{
	case -1://	First Time, go!
		if ( GetText(mess) )
		{
			UnsetClip()		;

			CommonOpenDial(mess)	;
			NextDialWindow()	;

			dx = SizeFont( PtDial )	;
			X_Dial = CENTERX-dx/2	;//	Center

			DialStatHolo = 1;
		}
		else	DialStatHolo = 0;
		break		;

	case 1://	Running
		UnsetClip()			;
		DialStatHolo = NextDialCar() 	;
		break				;

	default://	Stopped ( DialStatHolo == 0 or 2 )
		break	;
	}
}
//────────────────────────────────────────────────────────────────────────────
void	InitObjectifPlanet()
{
	S32	i	;
	T_ARROW	*pt	;

	NumObjectif = -1	;//	None by default
	OldObjectif = -2	;//	For AffReticule

	pt = &TabArrow[MAX_OBJECTIF]	;

	for ( i = MAX_OBJECTIF ; i < MAX_OBJECTIF+MAX_CUBE ; i++, pt++ )
	{
		if ( pt->FlagHolo & 1 )// If one or more, set the island
		{
			TabArrow[pt->Island].FlagHolo |= 1	;
		}
	}

	if ( TabArrow[Island].FlagHolo & 1 )//	Twinsen on Island with Arrow
	{
		TwinsenLinkArrow = Island	;// Togetheir
	}
	else
	{
		TwinsenLinkArrow = -1		;// Alone (in the space)
	}
}
//────────────────────────────────────────────────────────────────────────────
//	Pourrait etre fait dans InitObjectifPlanet() avant boucle mais bof...
void	ClearObjectifPlanet()
{
	S32	i	;
	T_ARROW	*pt	;

	pt = TabArrow	;

	for ( i = 0 ; i < MAX_OBJECTIF ; i++, pt++ )
	{
		pt->FlagHolo &= ~1	;
	}
}
//────────────────────────────────────────────────────────────────────────────
void	PrepareObjectifPlanet()
{
	NumObjectif = NextObjectif	;
	XpObjectif = XpNextObjectif	;
	YpObjectif = YpNextObjectif	;
	NextObjectif = -1		;
	DistObjectif = 100		;// Arbitrary (distance 2d ;)
}
//────────────────────────────────────────────────────────────────────────────
void	ComputeObjectifPlanet( S32 arrow )
{
	T_ARROW	*pt	;
	S32	dist	;
	S32	x, y, z	;

	pt = &TabArrow[arrow]	;

	SetAngleCamera( pt->Alpha, pt->Beta, 0 )	;

	LongWorldRotatePoint( 0, 0, pt->Alt )		;
	x = X0	;
	y = Y0	;
	z = Z0	;

	SetFollowCamera( 0, 0, 0, HoloAlpha, HoloBeta, HoloGamma, ZoomPlanet ) ;

	LongWorldRotatePoint( x, y, z )	;
	LongProjectPoint( X0, Y0, Z0 )	;

	dist = Distance2D( Xp, Yp, CENTERX, CENTERY )	;

	if ( dist < DistObjectif )
	{
		DistObjectif = dist	;
		NextObjectif = arrow	;
		XpNextObjectif = Xp	;
		YpNextObjectif = Yp	;
	}
}
//────────────────────────────────────────────────────────────────────────────
void	DrawTwinsenGlobe( T_SORT *ptt )
{
	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

	ObjTwinsen.X = ptt->Xw 		;
	ObjTwinsen.Y = ptt->Yw 		;//	Altitude + Twinsen Altitude
	ObjTwinsen.Z = ptt->Zw		;
	ObjTwinsen.Alpha = ptt->XMin	;
	ObjTwinsen.Beta = ptt->YMin	;
	ObjTwinsen.Gamma = 0		;

	ObjectDisplay( &ObjTwinsen )	;

	BoxMovingAdd( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax )	;
	AdjustZBufMinMax() ;
}
//────────────────────────────────────────────────────────────────────────────
void	DrawArrowGlobe( T_SORT *ptt )
{
	T_BODY_HEADER	*buf	;

	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;

	if ( ptt->Num == 1 )	buf = (T_BODY_HEADER*)BufFleche	;//	Big Arrow
	else			buf = (T_BODY_HEADER*)BufLoFleche	;//	Small Arrow

	BodyDisplay( ptt->Xw, ptt->Yw, ptt->Zw, ptt->XMin, ptt->YMin, 0, buf )	;

	BoxMovingAdd( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax )	;
	AdjustZBufMinMax() ;

	if ( ptt->Num == 2 )//	Twinsen is on me!
	{//	Recup Y ZV Obj Fleche & Rot to match Arrow Angles...
		RotateVector( buf->YMax+10, ptt->XMin, ptt->YMin, 0 )	;

		ptt->Xw += X0	;
		ptt->Yw += Y0	;
		ptt->Zw += Z0	;

		DrawTwinsenGlobe( ptt )	;// Twinsen is on me !
	}
}
//────────────────────────────────────────────────────────────────────────────
int 	z_sort( void const *a, void const *b)
{
	return( *(S16*)a - *(S16*)b ) ;
}
/*-------------------------------------------------------------------------*/
//	For Planet Sort
int	z_sort32( void const *a, void const *b)
{
	return( *(S32*)a - *(S32*)b ) ;
}
//────────────────────────────────────────────────────────────────────────────
void	DrawListHoloGlobe(S32 pos)
{
	S32	xm, ym, zm	;
	S32	talt, arrow	;
	S32	flag=0		;
	T_ARROW *pt		;
	T_SORT *ptt		;
	S32	nbtri=0		;
	U32	timer, angle	;

	timer = TimerRefHR*2	;

	if (( NumObjectif != OldObjectif )
	AND ( DecalTimerRef[NumObjectif] == 3072 ))
	{
		DecalTimerRef[NumObjectif] -= timer	;
	}

	ptt = ListTri		;

	pt = TabArrow		;

	for ( arrow = 0 ; arrow < MAX_OBJECTIF ; arrow++, pt++ )
	{
		if ((!(pt->FlagHolo & 1 ))//	Pas 	Valid
		OR ( pt->Planet != Planet ))	continue;
//------------- Altitude
		talt = pt->Alt		;

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

		talt += 64+((SinTab[ angle ]) >> 8 )	;
//------------- Rotation
		SetAngleCamera( pt->Alpha, pt->Beta, 0 );// Position Arrow

		LongWorldRotatePoint( 0, 0, talt )	;// Alt Arrow
		xm = X0					;
		ym = Y0					;
		zm = Z0					;

		SetFollowCamera( 0, 0, 0, HoloAlpha, HoloBeta, HoloGamma, ZoomPlanet )	;

		SetLightVector( HoloAlpha, HoloBeta, HoloGamma )	;

		LongWorldRotatePoint( xm, ym, zm )	;

		if ( !pos )
		{
			if ( CameraZr-Z0 < ZoomPlanet )		continue;
		}
		else
		{
			if ( CameraZr-Z0 >= ZoomPlanet )	continue;
		}
//-------------
		if ( pos )	ComputeObjectifPlanet( arrow )	;
//------------- Insertion
		if ( TwinsenLinkArrow == arrow ) ptt->Num = 2	;// Little Arrow + Twinsen
		else				 ptt->Num = 1	;// Big Arrow

//		ptt->Z = Z0 		;
		ptt->Z = CameraZr-Z0 	;// for zbuffer
		ptt->XMin = pt->Alpha	;
		ptt->YMin = pt->Beta	;
		ptt->Xw = xm 		;
		ptt->Yw = ym 		;
		ptt->Zw = zm 		;
		nbtri++ 		;
		ptt++			;
	}

//----- Twinsen Position
//	A priori, Twinsen est sur cette planet (on ne la visualise que si...)

	if ( TwinsenLinkArrow == -1 )//	Twinsen not linked with an arrow
	{
		pt = &TabArrow[Island]	;
//------------- Rotation
		SetAngleCamera( pt->Alpha, pt->Beta, 0 );// Position Arrow

		LongWorldRotatePoint( 0, 0, pt->Alt )	;// Alt Arrow
		xm = X0					;
		ym = Y0					;
		zm = Z0					;

		SetFollowCamera( 0, 0, 0, HoloAlpha, HoloBeta, HoloGamma, ZoomPlanet )	;

		SetLightVector( HoloAlpha, HoloBeta, HoloGamma )	;

		LongWorldRotatePoint( xm, ym, zm )	;

		flag = 0	;

		if ( !pos )
		{
			if ( CameraZr-Z0 > ZoomPlanet )		flag = 1	;
		}
		else
		{
			if ( CameraZr-Z0 <= ZoomPlanet )	flag = 1	;
		}
//------------- Insertion
		if ( flag )//	We must display Twinsen
		{
			if ( pos )	ComputeObjectifPlanet( Island )	;

			ptt->Num = 0		;// Twinsen, General Message Island
//			ptt->Z = Z0 		;
			ptt->Z = CameraZr-Z0 	;// for zbuffer
			ptt->XMin = pt->Alpha	;
			ptt->YMin = pt->Beta	;
			ptt->Xw = xm 		;
			ptt->Yw = ym 		;
			ptt->Zw = zm 		;
			nbtri++ 		;
		}
	}

//----- Now we sort all thoose stuffs

	qsort( ListTri, nbtri, sizeof( T_SORT ), z_sort32 );

	ptt = ListTri	;

	for( arrow = 0 ; arrow < nbtri ; arrow++, ptt++ )
	{
		if ( ptt->Num == 0 )	DrawTwinsenGlobe( ptt )	;
		else			DrawArrowGlobe( ptt )	;
	}
}
//────────────────────────────────────────────────────────────────────────────
//────────────────────────────────────────────────────────────────────────────
//
//		   ?  ?█▀▀▀??    █▀▀▀?█▄ ▄█ █▀▀▀?█▀▀▀?
//		   ██▀▀?██  ?██    ██  ?██??██▀▀?██▀▀?
//		   ▀▀  ?▀▀▀▀?▀▀▀▀?▀▀▀▀?▀▀  ?▀▀  ?▀▀ tm
//────────────────────────────────────────────────────────────────────────────
/*──────────────────────────────────────────────────────────────────────────*/
// precalcul coor globe avec coor polaires

void	ComputeCoorGlobe()
{
	S32	alpha, beta 	;
	U16	*ptrc 		;
	U8	*mptrv, *ptrv 	;
	S32	x, y 		;
	S32	normal 		;

	ptrc = PtrCoorGlobe	;

	ptrv =  PtrAlt 		;

	SetAngleCamera( 0,0,0 ) ;

	for( alpha = -1024; alpha <= 1024; alpha += STEP_ANGLE )
	{
		mptrv = ptrv ; // memo 0

		for( beta = 0; beta < 4096 ; beta += STEP_ANGLE )
		{
			if( *ptrv < 255 )
			{
				normal = RayonPlanet + *ptrv++ * 2 	;
			}
			else
			{
				ptrv++ ;
				normal = RayonPlanet + 500 ;
			}
//			normal = RayonPlanet + *ptrv++ * 2 	;
			Rotate( normal , 0, alpha ) 	;
			x = X0 				;
			y = Z0 				;
			Rotate( x, 0, beta ) 		;
			WorldRotatePoint( X0, y, Z0 ) 	;
			*ptrc++ = (U16)X0 		;
			*ptrc++ = (U16)Y0 		;
			*ptrc++ = (U16)Z0 		;
		}

		Rotate( RayonPlanet + *mptrv * 2 , 0, alpha ) 	;
		x = X0 				;
		y = Z0 				;
		Rotate( x, 0, 0 ) 		;	//...
		WorldRotatePoint( X0, y, Z0 ) 	;

		*ptrc++ = (U16)X0 	;
		*ptrc++ = (U16)Y0 	;
		*ptrc++ = (U16)Z0 	;
	}
}
//────────────────────────────────────────────────────────────────────────────

void	AdjustScreenMax()
{
	if( Xp < ScreenXMin )	ScreenXMin = Xp ;
	if( Xp > ScreenXMax )	ScreenXMax = Xp ;
	if( Yp < ScreenYMin )	ScreenYMin = Yp ;
	if( Yp > ScreenYMax )	ScreenYMax = Yp ;
}
//────────────────────────────────────────────────────────────────────────────
void	ComputeGlobeProj()
{
	U16	*ptr, *ptrt	;
	S16	*ptrc 		;
	S32	alpha, beta	;


	ScreenXMin = 32000	;
	ScreenXMax = -32000	;
	ScreenYMin = 32000	;
	ScreenYMax = -32000	;

	ptr = PtrGlobe 		;
	ptrt = PtrTri 		;

	ptrc = (S16*)PtrCoorGlobe ;

	for( alpha = -1024; alpha <= 1024; alpha += STEP_ANGLE )
	{
		for( beta = 0; beta < 4096 ; beta += STEP_ANGLE )
		{
			X0 = *ptrc++ 	;
			Y0 = *ptrc++ 	;
			Z0 = *ptrc++ 	;

			WorldRotatePoint( X0, Y0, Z0 ) 	;

			if( alpha != 1024 )
			{
//				*ptrt++ = (U16)Z0		;
				*ptrt++ = (U16)(CameraZr-Z0)	;// for zbuffer
				*ptrt++ = (U16)(ptr-PtrGlobe)	;
			}

			LongProjectPoint( X0, Y0, Z0 ) ;

			AdjustScreenMax()	;
			*ptr++ = (U16)Xp 	;
			*ptr++ = (U16)Yp 	;
		}

		X0 = *ptrc++ 	;
		Y0 = *ptrc++ 	;
		Z0 = *ptrc++ 	;

		WorldRotatePoint( X0, Y0, Z0 )	;

		LongProjectPoint( X0, Y0, Z0 )	;
		AdjustScreenMax()		;

		*ptr++ = (U16)Xp 	;
		*ptr++ = (U16)Yp 	;
	}

//	qsort( PtrTri, 512, 4, z_sort ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	DrawHolomap()
{
	U16	*ptr, *ptrm	;
	S32	n, off		;
	S32	zr		;

	RepMask = 0xFFFF	;//	Ya besoin a chaque fois ?

	PtrMap = PtrTextMap	;

	SetClip( 0, 0, ModeDesiredX-1, Dial_Y0-1 )	;
	for( n=0; n<16*32; n++ )
	{
		// for zbuffer
		zr  = GET_ZO(PtrTri[n*2+0]) ;

		off = PtrTri[n*2+1]	;

		ptr = PtrGlobe+off 	;
		ptrm = PtrMapping+off	;

		Tab_Points[0].Pt_XE = ptr[0] 		;
		Tab_Points[0].Pt_YE = ptr[1] 		;
		Tab_Points[1].Pt_XE = ptr[0+SLIG]	;
		Tab_Points[1].Pt_YE = ptr[1+SLIG]	;
		Tab_Points[2].Pt_XE = ptr[2] 		;
		Tab_Points[2].Pt_YE = ptr[3] 		;

		if( TestVuePoly( Tab_Points ) )
		{
			Tab_Points[0].Pt_MapU = ptrm[0] 	;
			Tab_Points[0].Pt_MapV = ptrm[1] 	;
			Tab_Points[1].Pt_MapU = ptrm[0+SLIG]	;
			Tab_Points[1].Pt_MapV = ptrm[1+SLIG]	;
			Tab_Points[2].Pt_MapU = ptrm[2] 	;
			Tab_Points[2].Pt_MapV = ptrm[3] 	;

			// for zbuffer
			Tab_Points[0].Pt_ZO     = (U16)zr 	;
			Tab_Points[1].Pt_ZO     = (U16)zr 	;
			Tab_Points[2].Pt_ZO     = (U16)zr 	;

			Fill_Poly( 8, 0, 3, Tab_Points )	;
		}

		Tab_Points[0].Pt_XE = ptr[0+SLIG]	;
		Tab_Points[0].Pt_YE = ptr[1+SLIG]	;
		Tab_Points[1].Pt_XE = ptr[2+SLIG]	;
		Tab_Points[1].Pt_YE = ptr[3+SLIG]	;
		Tab_Points[2].Pt_XE = ptr[2] 		;
		Tab_Points[2].Pt_YE = ptr[3] 		;

		if( TestVuePoly( Tab_Points ) )
		{
			Tab_Points[0].Pt_MapU = ptrm[0+SLIG] 	;
			Tab_Points[0].Pt_MapV = ptrm[1+SLIG] 	;
			Tab_Points[1].Pt_MapU = ptrm[2+SLIG]	;
			Tab_Points[1].Pt_MapV = ptrm[3+SLIG]	;
			Tab_Points[2].Pt_MapU = ptrm[2] 	;
			Tab_Points[2].Pt_MapV = ptrm[3] 	;

			// for zbuffer
			Tab_Points[0].Pt_ZO     = (U16)zr	;
			Tab_Points[1].Pt_ZO     = (U16)zr 	;
			Tab_Points[2].Pt_ZO     = (U16)zr 	;

			Fill_Poly( 8, 0, 3, Tab_Points )	;
		}
	}
}

//────────────────────────────────────────────────────────────────────────────
#ifdef	DEBUG_TOOLS
S32	SizeHoloMalloc = 640L*480L+RECOVER_AREA	;
#endif
U8	*PtHoloMalloc				;
//────────────────────────────────────────────────────────────────────────────
void	InitHoloMalloc(void)
{
	PtHoloMalloc = ScreenAux			;
#ifdef	DEBUG_TOOLS
	SizeHoloMalloc = 640L*480L+RECOVER_AREA		;
#endif
}
//────────────────────────────────────────────────────────────────────────────
U8	*HoloMalloc( U32 size )
{
	U8	*ptret	;

#ifdef	DEBUG_TOOLS
	SizeHoloMalloc -= size		;
	if ( SizeHoloMalloc < 0 )	TheEnd(NOT_ENOUGH_MEM, "HoloMalloc" ) ;
#endif

	ptret = PtHoloMalloc	;

	PtHoloMalloc += size	;

	return(ptret)		;
}
//────────────────────────────────────────────────────────────────────────────
U8	*LoadHoloMalloc_HQR( char *name, S32 index )
{
	U8	*pt	;
	U32	size	;

	size = HQF_ResSize( name, index );

	pt = HoloMalloc( size )		;

	if( !Load_HQR( name, pt, index ) ) TheEndCheckFile( HOLO_HQR_NAME ) ;

	return(pt)	;
}
//────────────────────────────────────────────────────────────────────────────
void	InitHoloGlobe(S32 planet)
{
	S32	id	;

	Planet = planet	;


	// on passe en ZBuffer
	Switch_Fillers( FILL_POLY_ZBUFFER );

        InitHoloMalloc()	;

	BufBodyTempo = (U8*)HoloMalloc(SizeCurrentBody)	;
	BufAnimTempo = (U8*)HoloMalloc(SizeCurrentAnim)	;

	BufFleche = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_FLECHE )	;
	ScaleRotBody3D( BufFleche, SCALE_OBJ_GLOBE, TRUE )	;

	BufLoFleche = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_LOFLECHE );
	ScaleRotBody3D( BufLoFleche, SCALE_OBJ_GLOBE, TRUE )	;

//----- Recuperation Body Twinsen et Scale

	memcpy( BufBodyTempo, PtCurrentBody, SizeCurrentBody )	;
	memcpy( BufAnimTempo, PtCurrentAnim, SizeCurrentAnim )	;

	ScaleRotBody3D( BufBodyTempo, SCALE_BODY_GLOBE, TRUE )	;
	ScaleRotAnim3D( BufAnimTempo, SCALE_BODY_GLOBE, TRUE )	;

	ObjectClear( &ObjTwinsen )		;

	ObjectInitBody( &ObjTwinsen, NULL )	;
	ObjectInitAnim( &ObjTwinsen, NULL )	;

//----- For One Planet

	PtrGlobe = (U16*)HoloMalloc(SIZE_TEXT_GLOBE)	;

	PtrCoorGlobe = (U16*)HoloMalloc(SIZE_COOR_GLOBE)	;

	PtrMapping = (U16*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, HQR_COORMAPP_HMM );// CoorMapp.HMM

	id = HQR_TWINSUN_HMT+(Planet*2)			;

	PtrAlt = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, id ) 		;// .HMT

	PtrTextMap   = (U8*)LoadHoloMalloc_HQR( HOLO_HQR_NAME, id+1 ) 	;// .HMG

	RayonPlanet = RAYON_PLANET		;

	ComputeCoorGlobe()			;

//-----

	SetProjection( CENTERX, CENTERY, 128,1024,1024 ) ;
//	SetProjection( CENTERX, CENTERY, 128,700,700 ) ;
	SetLightVector( 0, 0, 0 )		;

	InitObjectifPlanet()	;

	ZoomPlanet = ZOOM_INIT_PLANET;

	if( Planet==3 )// sous-gaz
		ZoomPlanetDest = ZOOM_PLANET_SOUSGAZ 	;
	else	ZoomPlanetDest = ZOOM_PLANET 	;

	NumOldMess = -1		;

	Cls()				;//	Tempo : Chargement Screen joli fond hologlobe
	BoxUpdate() 			;

	CopyScreen(Log, Screen)		;

	HoloInitDialWindow()		;

}
//────────────────────────────────────────────────────────────────────────────
void	ClearHoloGlobe()
{
	ClearObjectifPlanet()	;

/*	Unused depuis HoloMalloc
	if( PtrTextMap )	Free( PtrTextMap)	;
	PtrTextMap = NULL	;

	if( PtrAlt )		Free( PtrAlt)		;
	PtrAlt = NULL ;

	if( PtrMapping )	Free( PtrMapping )	;
	PtrMapping = NULL ;

	if( PtrCoorGlobe )	Free( PtrCoorGlobe)	;
	PtrCoorGlobe = NULL ;

	if( PtrGlobe )		Free( PtrGlobe )	;
	PtrGlobe = NULL ;

	if( BufFleche )		Free( BufFleche )	;
	BufFleche = NULL ;
*/

	// Désactive mode ZBuffer
	Switch_Fillers( FILL_POLY_TEXTURES );
}
//────────────────────────────────────────────────────────────────────────────

void	DoAnimHolo()
{
	ObjectSetInterAnim( &ObjTwinsen )	;
}
//────────────────────────────────────────────────────────────────────────────
void	FindAngleIsland(S32 island)
{
	S32	xm, ym, zm	;
	S32	halpha, hbeta	;
	S32	alpha, beta, alt;
	S32	min, dist	;


	alpha = TabArrow[island].Alpha	;
	beta = TabArrow[island].Beta	;
	alt = TabArrow[island].Alt	;

	min = 32000		;

	for ( halpha = 0 ; halpha < 4096 ; halpha += 64 )//	32
	{
		for ( hbeta = 0 ; hbeta < 4096 ; hbeta += 64 )//	32
		{
			SetAngleCamera( alpha, beta, 0 );

			WorldRotatePoint( 0, 0, alt )	;
			xm = X0					;
			ym = Y0					;
			zm = Z0					;

			SetFollowCamera( 0, 0, 0, halpha&4095, hbeta&4095, 0, ZoomPlanet )	;

			WorldRotatePoint( xm, ym, zm )		;

			if ( CameraZr-Z0 > ZoomPlanet )	continue;

			LongProjectPoint( X0, Y0, Z0 )		;

			dist = Distance2D( Xp, Yp, CENTERX, CENTERY )	;
			if ( dist < min )
			{
				min = dist		;
				DestAlpha = halpha	;
				DestBeta = hbeta	;
			}
		}
	}
}
//────────────────────────────────────────────────────────────────────────────
S32	GoToArrow()
{
	U32	time	;

	time = TimerRefHR-MemoTimerRot	;

	HoloAlpha = BoundRegleTrois( HoloAlpha, DestAlpha, 25*20, time )	;
	HoloBeta = BoundRegleTrois( HoloBeta, DestBeta, 25*20, time )	;
//	ZoomPlanet = BoundRegleTrois( ZoomPlanet, 2000, 50, time )	;

	if (( HoloAlpha == DestAlpha )
	AND ( HoloBeta == DestBeta ))	return(1)	;

//	if ( ZoomPlanet == 2000 )	return(1)	;
	return(0)	;
}
//────────────────────────────────────────────────────────────────────────────
void	HoloGlobe(S32 planet)
{
	S32	testnofire = TRUE 	;
	U32	memotime		;

	InitHoloGlobe(planet)		;

	InitAngleArrow()		;

	HoloMode = 0			;

	HoloGamma = 0			;

	UnsetClip() 	;

	FindAngleIsland(Island)	;

	HoloAlpha = DestAlpha	;
	HoloBeta = DestBeta	;

	memotime = TimerRefHR	;

	ZBufYMin = 0		;
	ZBufYMax = 479		;

	while( !FlagHoloEnd )
	{
		ManageTime() ;
		MyGetInput() ;

		// Clear une bande du ZBuffer
		memset( (PtrZBuffer+TabOffLine[ZBufYMin]),// PtrZBuffer est un (U16 *)
			255,
			(ZBufYMax-ZBufYMin+1)*640*2 );	// Clear ZBuffer

		ClearZBufMinMax() ;

		if (( MyKey == K_ESC )
		OR ( Input&(I_MENUS|I_HOLOMAP) ))
		{
			FlagHoloEnd = TRUE	;
			break			;
		}

		if( testnofire )
		{
			if( !(Input&I_FIRE) )
				testnofire = FALSE ;
		}
		else
		{
			if ( FlagMap != 1 )
			{
				if( Input & I_LEFT )	HoloBeta  = (HoloBeta+32)&4095 	;
				if( Input & I_RIGHT )	HoloBeta  = (HoloBeta-32)&4095 	;
				if( Input & I_UP )	HoloAlpha = (HoloAlpha-32)&4095 ;
				if( Input & I_DOWN )	HoloAlpha =  (HoloAlpha+32)&4095;

				if ((( Input == I_ACTION_M ) OR ( Input == I_RETURN ))	// espace
				AND ( !FlagMap ))
				{
//					If the player can or cannot Zoom an other island...
					if (( NumObjectif == 1000 )//	Twinsen
					OR ( NumObjectif == Island )//	Ile de Twinsen
					OR ( NumObjectif == -1 ))//	Dans la Nature
					{
						FlagMap = 1			;
						ZoomedIsland = Island		;
						FindAngleIsland(ZoomedIsland)	;
						MemoTimerRot = TimerRefHR	;
					}
					else//				Sur une autre Ile...
					{
						PlayErrorSample()	;
					}
				}
			}
		}

		DoAnimHolo()	;

//----------------------------------------------------------------------------
		if ( FlagMap == 1 )
		{
			if ( GoToArrow())
			{
				ClearHoloGlobe()		;
				HoloPlan(ZoomedIsland)		;
				if ( FlagHoloEnd )	break	;
				HoloMode = 0			;
				InitHoloGlobe(Planet)		;
				Cls()				;
				BoxUpdate()			;
				FlagMap = 0			;
				InitWaitNoInput( I_JOY )	;
				memotime = TimerRefHR		;
			}
		}
//----------------------------------------------------------------------------
		DrawListHoloGlobe(0)	;
//-------------
		SetFollowCamera( 0, 0, 0, HoloAlpha, HoloBeta, HoloGamma, ZoomPlanet ) 	;

		SetLightVector( HoloAlpha, HoloBeta, HoloGamma )	;

		ComputeGlobeProj()	;

		DrawHolomap()	;
		BoxMovingAdd( ScreenXMin, ScreenYMin, ScreenXMax, Dial_Y0-1 );
		AdjustZBufMinMax() ;
//-------------
		PrepareObjectifPlanet()	;// Must be run before DrawListHoloGlobe(1)

		if ( ZoomPlanet == ZoomPlanetDest )	AffReticule()	;
//-------------
		DrawListHoloGlobe(1)	;
//-------------
		AffHoloMess()	;
//-------------
		UnsetClip()	;

#ifdef	LBA_EDITOR
UnsetClip()					;
CoulText( 255, 0 )				;
CmptFrame++ ;
GraphPrintf( FALSE, 0, 0, "%d     ", NbFramePerSecond )	;
#else
		CheckSaveLogPcx( PtrPal ) ;// F9: Sauve Screen dans PATH_PCX_SAVE
#endif

		BoxUpdate()	;

		OldObjectif = NumObjectif	;

		if( FlagFade )	// Fade après 1er affichage
		{
			MemoTimerRot = TimerRefHR	;
			FadeToPal( PtrPal ) 	;
			SetTimerHR( MemoTimerRot )	;
			FlagFade = FALSE 		;
		}

		if ( ZoomPlanet < ZoomPlanetDest )
		{
			ZoomPlanet = BoundRegleTrois( ZOOM_INIT_PLANET, ZoomPlanetDest, 500, TimerRefHR-memotime )	;
		}
	}

	ClearHoloGlobe()	;
}
//────────────────────────────────────────────────────────────────────────────
/*void	TheEnd()
{
	printf("Ok.\n")		;
}
*/
//────────────────────────────────────────────────────────────────────────────
/*══════════════════════════════════════════════════════════════════════════*/
Func_TransNumPtr	HoloGivePtrAnim	;

void	*HoloGivePtrAnim(S32 index)
{
	index = index		;// Pour virer Warning
	return( BufAnimTempo )	;//	Surement plus simple a faire
}
/*══════════════════════════════════════════════════════════════════════════*/
Func_TransNumPtr	HoloGivePtrBody	;

void	*HoloGivePtrBody(S32 index)
{
	index = index		;// Pour virer Warning
	return( BufBodyTempo )	;//	Voir au dessus...
}
/*══════════════════════════════════════════════════════════════════════════*/
void	InitHoloMap()
{
	if( !Load_HQR( HOLO_HQR_NAME, TabArrow, HQR_ARROWBIN ) )
		TheEndCheckFile( HOLO_HQR_NAME ) ;
}
/*══════════════════════════════════════════════════════════════════════════*/
void	HoloMap()
{
	S32	flagproto = FALSE ;


//----- DEBUG
//	TabArrow[MAX_OBJECTIF+67].FlagHolo = 1	;//	Desert Globe
//-----

	if( Comportement==C_PROTOPACK
	OR  Comportement==C_JETPACK )
	{
		// pour pouvoir restaurer anim marche protopack
#ifdef	LBA_EDITOR
		if( !stricmp(ListObjet[NUM_PERSO].StringAnim,GEN_ANIM_MARCHE) )
#else
		if( ListObjet[NUM_PERSO].GenAnim==GEN_ANIM_MARCHE )
#endif
		{
			flagproto = TRUE ;
		}
	}

	MemoClipWindow()		;//	CinemaMode
	UnsetClipWindow()		;
	SaveCamera()		;

	TestCoulDial( COUL_PERSO ) 	;
	InitDial( 2 )		;//	FR_GAM.DIA

	PtCurrentBody = (U8*)GivePtrBody( ListObjet[NUM_PERSO].Obj.Body.Num )	;
	PtCurrentAnim = (U8*)GivePtrAnim( ListObjet[NUM_PERSO].Obj.Anim.Num )	;

	SizeCurrentBody = GetSizeBody3D( PtCurrentBody );
	SizeCurrentAnim = GetSizeAnim3D( PtCurrentAnim );

	InitObjects( NULL, NULL, HoloGivePtrBody, HoloGivePtrAnim )	;

	FlagMap = 0		;
	FlagHoloEnd = FALSE	;
//-----
	FadeToBlackAndSamples( PtrPal ) ;
	HQ_StopSample()			;

#ifndef	LBA_EDITOR
	RestartRainSample = TRUE	;
#endif

	FlagFade = TRUE 		;	// ( pour HoloPlan() )
//-----

	BoxReset() 	;

	SizeRet = 128		;// Replace by Regle 3 Timer
	MemoTimerRet = TimerRefHR	;

	if ( HoloMode == 1 )	HoloPlan(Island)	;

	if ( !FlagHoloEnd )
	{
		while( !FlagHoloEnd )	HoloGlobe(Planet)	;
	}

//	Free( BufAnimTempo )	;
//	Free( BufBodyTempo )	;

	InitDial( START_FILE_ISLAND+Island ) ;

	InitObjects( NULL, NULL, GivePtrBody, GivePtrAnim )	;

	FadeToBlack( PtrPal ) ;	// Restauration + Fade
	FlagFade = TRUE ;
	RestoreCamera() ;
	PtrInit3DGame() ;

	if( CubeMode==CUBE_INTERIEUR )
	{
		CameraCenter( 200 ) ;// 200: parametre inutilisé

#ifndef	LBA_EDITOR
		InitGrille( NumCube ) ;// pour recharger dans PtrZBuffer !
#endif
	}

	FirstTime = AFF_ALL_FLIP ;

	RestoreClipWindow()	;//	CinemaMode

	InitWaitNoInput( I_HOLOMAP|I_MENUS ) ;
	InitWaitNoKey() ;

	if( flagproto )
	{
		RestoreTimer() ;

		// restore marche proto en ANIM_ALL_THEN pour que le
		// joueur ait le temps de se reprendre
		InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NUM_PERSO ) ;
		ObjectSetFrame( &(ListObjet[NUM_PERSO].Obj), 5 ) ;
		TimerProto = TimerRefHR+TEMPO_PROTO_AUTO ;// 1/2 s

		SaveTimer() ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/

