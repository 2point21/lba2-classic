/*══════════════════════════════════════════════════════════════════════════*
			 █▀▀█  █   █ █▀▀▀▀ █▀▀▀▀ █  ▄▀
			 ██▀▀█ ██  █ ██ ▀█ ██ ▀█ ██▀
                         ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/

#include 	"c_extern.h"

#define	MAX_SAMPLE_PITCH	11000
#define	MIN_SAMPLE_PITCH2	5000
#define	MAX_SAMPLE_PITCH2	8500
#define	MAX_SPEED		3800
#define	TEMPO_GEAR		1200	// tempo changement de vitesse

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

S_BUGGY	ListBuggy[MAX_BUGGYS] ;

S32	Gear = 0 ;
S32	TimerGear ;

U8	NumBuggy = 0	;

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// Initialise Pos et Cube Buggy Scenarique pris par Twinsen
void	InitBuggy( U8 numobj, U32 flaginit )
{
	S_BUGGY	*ptb = &ListBuggy[0] ;
	T_OBJET	*ptrobj = &ListObjet[numobj] ;

	// Pour que les objets suivent leurs tracks sans etre interrompus
	// par le buggy (tant pis, il sera poussé)
	ptrobj->Flags |= (PUSHABLE|CHECK_CODE_JEU) ;

	if( (flaginit==2)// on force le repositionnement du buggy
	OR  (flaginit
	    AND !NumBuggy) )	// premiere initialisation
			// car le buggy vide ne peut pas etre Twinsen
	{
		ptb->Cube = NumCube 	;	// Port-Ludo (Désert)

		ptb->X = ptrobj->Obj.X	;
		ptb->Y = ptrobj->Obj.Y	;
		ptb->Z = ptrobj->Obj.Z	;

		ptb->Beta = ptrobj->Obj.Beta ;

		InitBody( GEN_BODY_NORMAL, numobj ) ;

		NumBuggy = (U8)(numobj|BUGGY_PRESENT) ;
	}
	else if( NumBuggy )
	{
		if( ListObjet[NUM_PERSO].Move!=MOVE_BUGGY_MANUAL
		AND ListObjet[NUM_PERSO].Move!=MOVE_BUGGY )
		{
#ifndef	LBA_EDITOR
			S32	x, y ;
#endif

			if( NumCube==ptb->Cube )
			{
				ptrobj->Obj.X = ptb->X ;
				ptrobj->Obj.Y = ptb->Y ;
				ptrobj->Obj.Z = ptb->Z ;

				ptrobj->Obj.Beta = ptb->Beta ;

				InitBody( GEN_BODY_NORMAL, numobj ) ;
			}
#ifndef	LBA_EDITOR
			else	if( LoadSceneCubeXY( ptb->Cube, &x, &y ) )
			{
				x -= CurrentCubeX ;
				y -= CurrentCubeY ;

				ptrobj->Obj.X = ptb->X + x*32768 ;
				ptrobj->Obj.Y = ptb->Y ;
				ptrobj->Obj.Z = ptb->Z + y*32768 ;

				ptrobj->Obj.Beta = ptb->Beta ;

				ptrobj->Flags = NO_SHADOW|OBJ_BACKGROUND|NO_CHOC|OBJ_ZBUFFER ;

				InitBody( GEN_BODY_NORMAL, numobj ) ;
			}
#endif
			else	InitBody( NO_BODY, numobj ) ;

			ClearRealAngle( ptrobj ) ;
		}
		else	InitBody( NO_BODY, numobj ) ;

		NumBuggy = (U8)(numobj|BUGGY_PRESENT) ;
	}
	else	InitBody( NO_BODY, numobj ) ;
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// A Appeler en début de partie

void	ResetBuggy( void )
{
	S_BUGGY	*ptb    = &ListBuggy[0] ;

	NumBuggy  = 0 ;
	ptb->Cube = 0 ;
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void	TakeBuggy( void )
{
	S32	sample ;
	T_OBJET	*ptrobj = &ListObjet[NUM_PERSO] ;
	S_BUGGY	*ptb    = &ListBuggy[0] ;

	ptb->SpeedRot  = 1024  ;
	ptb->LastTimer = TimerRefHR ;

	ObjectClear( &ptrobj->Obj ) ;

	// Blindage au cas où le Buggy aurait bougé (en étant poussé par ex)
	ptb->X = ListObjet[NUM_BUGGY].Obj.X ;
	ptb->Y = ListObjet[NUM_BUGGY].Obj.Y ;
	ptb->Z = ListObjet[NUM_BUGGY].Obj.Z ;
	///////////////////////////////////////////

	ptrobj->Obj.X = ptb->X ;
	ptrobj->Obj.Y = ptb->Y ;
	ptrobj->Obj.Z = ptb->Z ;
	ptrobj->Obj.Beta = ptb->Beta ;
	ClearRealAngle( ptrobj ) ;	// pour ne pas ecraser le beta

	ptrobj->WorkFlags |= MANUAL_INTER_FRAME ;
	ptrobj->Flags |= OBJ_ZBUFFER ;

	SetComportement( C_BUGGY ) ;

	// Passe Buggy Scenarique en NoBody
	InitBody( NO_BODY, NUM_BUGGY ) ;

	if( ptrobj->SampleAlways )
	{
		HQ_StopOneSample( ptrobj->SampleAlways ) ;
		ptrobj->SampleAlways = 0 ;
	}

#ifdef	LBA_EDITOR
	sample = NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_BUGGY) ;
	ptrobj->NumDef = COMMON_NUM_DEF ;
#else
	sample = SAMPLE_BUGGY ;
#endif

	if( IsSamplePlaying( sample ) )
	{
		HQ_StopOneSample( sample ) ;
	}

	ptrobj->SampleVolume = 20 ;

	ParmSampleVolume = ptrobj->SampleVolume ;

	Gear = 0 ;
	TimerGear = 0 ;

#ifdef	LBA_EDITOR
	ptrobj->SampleAlways = HQ_3D_MixSample( COMMON_NUM_DEF, SAMPLE_BUGGY, 4096, 0, 0,
						ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
#else
	ptrobj->SampleAlways = HQ_3D_MixSample( SAMPLE_BUGGY, 4096, 0, 0,
						ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
#endif
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void	LeaveBuggy( U8 newcomportement )
{
	S32	sample ;
	T_OBJET	*ptrobj = &ListObjet[NUM_PERSO] ;
	S_BUGGY	*ptb    = &ListBuggy[0] ;

#ifdef	LBA_EDITOR
	sample = NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_BUGGY) ;
#else
	sample = SAMPLE_BUGGY ;
#endif

	if( IsSamplePlaying( sample ) )
	{
		HQ_StopOneSample( sample ) ;
		ptrobj->SampleAlways = 0 ;
	}

	ptb->X = ptrobj->Obj.X ;
	ptb->Y = ptrobj->Obj.Y ;
	ptb->Z = ptrobj->Obj.Z ;
	ptb->Beta = ptrobj->Obj.Beta ;
	ptb->Cube = NumCube ;

	ObjectClear( &ptrobj->Obj ) ;

	ptrobj->WorkFlags &= ~(MANUAL_INTER_FRAME) ;
	ptrobj->Flags &= ~(OBJ_ZBUFFER) ;

	InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;

	SetComportement( newcomportement ) ;

//----- Restaure buggy scénarique

	ptrobj = &ListObjet[NUM_BUGGY] ;

	ptrobj->Obj.X = ptb->X ;
	ptrobj->Obj.Y = ptb->Y ;
	ptrobj->Obj.Z = ptb->Z ;
	ptrobj->Obj.Beta = ptb->Beta ;

	ptrobj->CodeJeu = ListObjet[NUM_PERSO].CodeJeu ;

	ClearRealAngle( ptrobj ) ;	// pour ne pas ecraser le beta

	InitBody( GEN_BODY_NORMAL, NUM_BUGGY ) ;

	// recherche pos libre pour Twinsen autour
	PosObjetAroundAnother( NUM_BUGGY, NUM_PERSO ) ;
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void	MoveBuggy( T_OBJET *ptrobj )
{
	S_BUGGY		*ptb   = &ListBuggy[0] ;
	T_OBJ_3D	*ptb3d = &ptrobj->Obj ;
	S32		pitch  = 0 ;
	S32	flagattack = FALSE ;

//	S32	speedlevel ;
	S32	speedinc ;
	S32	rotlevel ;
	S32	timerhr, deltatimer ;

	timerhr = TimerRefHR ;
	deltatimer = timerhr - ptb->LastTimer ;

	if( (Input & I_THROW)
	AND (PtrComportement->Flags & CF_WEAPON) )
	{
		// est-on en mage ?
		if( TabInv[FLAG_TUNIQUE].IdObj3D==0 )
		{
			InitBody( GEN_BODY_TUNIQUE_TIR, NUM_PERSO ) ;
		}
		else
		{
			InitBody( GEN_BODY_MAGE_TIR, NUM_PERSO ) ;
		}

		InitAnim( GEN_ANIM_LANCE, ANIM_REPEAT, NUM_PERSO ) ;

		/* control direction pendant visée */
		if( !(ptrobj->WorkFlags&ANIM_MASTER_ROT) )
		{
			ptb3d->Beta += GetDeltaMove( &ptrobj->BoundAngle.Move ) ;
			ptb3d->Beta &= 4095 ;

			ManualRealAngle( ptrobj ) ;
		}

//		MagicBall = TRUE ;
		LastJoyFlag = TRUE ;
		flagattack  = TRUE ;
	}
	else
	{
		if( LastInput&I_THROW )
		{
			// on a fini de tirer avec le buggy,
			// on referme le capot
			InitBody( GEN_BODY_TUNIQUE, NUM_PERSO ) ;
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
		}
	}

	if( !flagattack
	AND !(ptrobj->WorkFlags&(FALLING|ANIM_MASTER_GRAVITY)) )
	{
		ClearRealAngle( ptrobj ) ;

		if( LastJoyFlag
		AND (((Input&I_JOY)!=LastMyJoy) OR ((Input&I_FIRE)!=LastMyFire)) )
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
			Pushing = FALSE ;
		}

		LastJoyFlag = FALSE ;

		// Pushing contient le numero de l'objet que l'on pousse
		// Donc 1000 est une valeur impossible qui sert de flag d'init
		// anim rien
		if( Pushing==1000 )
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
			Pushing = FALSE ;
		}

		if( Input&I_UP )
		{
			if( Pushing )
			{
				InitAnim( GEN_ANIM_POUSSE, ANIM_TEMPO, NUM_PERSO ) ;
				LastJoyFlag = TRUE ;
			}
			else
			{
				InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NUM_PERSO ) ;

				LastJoyFlag = TRUE ;
			}
		}
		else if( Input&I_DOWN )
		{
			InitAnim( GEN_ANIM_RECULE, ANIM_REPEAT, NUM_PERSO ) ;
			LastJoyFlag = TRUE ;
		}
	}

	if( !(ptrobj->WorkFlags&(FALLING|ANIM_MASTER_GRAVITY)) )
	{
		// check speed command
		if( (Input & I_UP)	// j'accelere
		AND !flagattack )
		{
			ptb->Speed += deltatimer * 4 ;

			if( !TimerGear )	TimerGear = TimerRefHR+TEMPO_GEAR ;
			else
			{
				if( Gear<0 )	Gear = 0 ;

				if( TimerRefHR>TimerGear
				AND Gear<2 )
				{
					Gear++ ;
					TimerGear = TimerRefHR+TEMPO_GEAR ;
				}
			}
		}
		else if( (Input & I_DOWN)	// je freine / recule
		AND !flagattack )
		{
			ptb->Speed -= deltatimer * 12 ;
			Gear = -1 ;
			TimerGear = 0 ;
		}
		else	// je laisse ralentir
		{
			if( ptb->Speed > 0 )
			{
				ptb->Speed -= deltatimer * 7 ;
				if( ptb->Speed < 0 )
				{
					ptb->Speed = 0 ;
				}
			}
			if( ptb->Speed < 0 )
			{
				ptb->Speed += deltatimer * 7 ;
				if( ptb->Speed > 0 )
				{
					ptb->Speed = 0 ;
				}
			}
			Gear = 0 ;
			TimerGear = 0 ;
		}

		if( ptb->Speed < -2000 )	ptb->Speed = -2000 ;
		if( ptb->Speed > MAX_SPEED )	ptb->Speed = MAX_SPEED ;

		speedinc = ptb->Speed * deltatimer / 1000 ;
	}
	else
	{
		speedinc = 0 ;
	}

	// check dir

	if( !flagattack )
	{
		if( Input & I_RIGHT )
		{
			ptb->BetaWheel = -300 ;
			if( ptb->Speed )
			{
				rotlevel = -ptb->SpeedRot * speedinc / ptb->Speed ;
//				ptb->BetaWheel = rotlevel*1000 ;
			}
			else
			{
				rotlevel = 0 ;
			}
		}
		else if( Input & I_LEFT )
		{
			ptb->BetaWheel = 300 ;
			if( ptb->Speed )
			{
				rotlevel = ptb->SpeedRot * speedinc / ptb->Speed ;
//				ptb->BetaWheel = rotlevel*1000 ;
			}
			else
			{
				rotlevel = 0 ;
			}
		}
		else
		{
			ptb->BetaWheel = 0 ;
			rotlevel = 0 ;
		}

		if( ptrobj->Flags & SKATING )
		{
			ptb->Speed = 3000 ;
			speedinc = ptb->Speed * deltatimer / 1000 ;
		}
		else
		{
			if( ptb->Speed >= 0 )
			{
				ptb3d->Beta += rotlevel ;
			}
			else
			{
				ptb3d->Beta -= rotlevel ;
			}

			ptb3d->Beta &= 4095 ;
		}
	}
	else
	{
		ptb->BetaWheel = 0 ;
	}

	LastMyJoy  = Input&I_JOY  ;
	LastMyFire = Input&I_FIRE ;
	LastInput  = Input ;

	ptb->LastTimer = timerhr ;
	ptb->SpeedInc = speedinc ;

	if( ptrobj->SampleAlways
	AND IsSamplePlaying( ptrobj->SampleAlways ) )
	{
		S32	pitch ;

		switch( Gear )
		{
			case -1:
				pitch = BoundRegleTrois( 3000, MAX_SAMPLE_PITCH2, MAX_SPEED, abs(ptb->Speed) ) ;
				break ;

			case 0:
				pitch = BoundRegleTrois( 3000, MAX_SAMPLE_PITCH, MAX_SPEED, abs(ptb->Speed) ) ;
				if( pitch>=MAX_SAMPLE_PITCH )	TimerGear = 1 ;
				break ;

			case 1:
				pitch = BoundRegleTrois( MAX_SAMPLE_PITCH2, MIN_SAMPLE_PITCH2, TEMPO_GEAR, TimerGear-TimerRefHR ) ;
				break ;

			default:
				pitch = MAX_SAMPLE_PITCH2 ;
		}

		ChangePitchbendSample( ptrobj->SampleAlways, pitch ) ;
	}
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void	DoAnimBuggy( T_OBJET *ptrobj )
{
	S32	x1, y1, z1, yw ;
	S_BUGGY	*ptb = &ListBuggy[0] ;
	T_OBJ_3D	*ptb3d = &ptrobj->Obj ;

//	S32	oldx, oldy, oldz ;

	// wheels rot
	S32	c, d ;

	S32	x, y, z ;

	// Magouille pour ne pas ecraser les groupes dans AffOneObjet()
	ObjectSetInterFrame( ptb3d )	;

//	oldx = ptb3d->X ;
//	oldy = ptb3d->Y ;
//	oldz = ptb3d->Z ;


	if( ptrobj->WorkFlags&(FALLING|ANIM_MASTER_GRAVITY) )
	{
		return ;
	}

	LongRotate( 0, ptb->SpeedInc*1024 , ptb3d->Beta ) ;
	Nxw = ptb3d->X + X0/1024 ;
	Nzw = ptb3d->Z + Z0/1024 ;

	// altitude ideal
//	ny = CalculAltitudeObjet( ptb3d->X, ptb3d->Z ) ;	// ground y pour xz
	yw = CalculAltitudeObjet( Nxw, Nzw, -1 ) ;	// ground y pour xz

	// test altitude #2: triangles interdits

	// roue avant gauche direction
	ptb3d->CurrentFrame[3].Beta = (S16)ptb->BetaWheel ;
	// roue avant droite direction
	ptb3d->CurrentFrame[6].Beta = (S16)ptb->BetaWheel ;


	// gestion 4 roues séparées.

	// +++++++++++ front right wheel

	LongRotate( -400, 400, ptb3d->Beta ) ;
	x = Nxw + X0 ;
	z = Nzw + Z0 ;
	y = yw ;

	if( x>=0 AND x<32768
	AND z>=0 AND z<32768 )
	{
		y += CalculAltitudeObjet( x, z, -1 ) ;
	}

	c = (260 * 31415)/1000 ;	// circonsference * 10
	d = Distance3D( ptb->Wheel[0].X, ptb->Wheel[0].Y, ptb->Wheel[0].Z, x,y,z ) ;

	if( ptb->Speed >= 0 )
	{
		ptb->Wheel[0].Angle += (4096*10*d)/c ;
	}
	else
	{
		ptb->Wheel[0].Angle -= (4096*10*d)/c ;
	}

	ptb->Wheel[0].X = x ;
	ptb->Wheel[0].Y = y ;
	ptb->Wheel[0].Z = z ;

	// +++++++++++ front left wheel

	LongRotate( 400, 400, ptb3d->Beta ) ;
	x = Nxw + X0 ;
	z = Nzw + Z0 ;
	y = yw ;

	if( x>=0 AND x<32768
	AND z>=0 AND z<32768 )
	{
		y += CalculAltitudeObjet( x, z, -1 ) ;
	}

	c = (260 * 31415)/1000 ;	// circonsference * 10
	d = Distance3D( ptb->Wheel[1].X, ptb->Wheel[1].Y, ptb->Wheel[1].Z, x,y,z ) ;

	if( ptb->Speed >= 0 )
	{
		ptb->Wheel[1].Angle += (4096*10*d)/c ;
	}
	else
	{
		ptb->Wheel[1].Angle -= (4096*10*d)/c ;
	}

	ptb->Wheel[1].X = x ;
	ptb->Wheel[1].Y = y ;
	ptb->Wheel[1].Z = z ;

	// +++++++++++ back left wheel

	LongRotate( 400, -350, ptb3d->Beta ) ;
	x = Nxw + X0 ;
	z = Nzw + Z0 ;
	y = yw ;

	if( x>=0 AND x<32768
	AND z>=0 AND z<32768 )
	{
		y += CalculAltitudeObjet( x, z, -1 ) ;
	}

	c = (360 * 31415)/1000 ;	// circonsference * 10
	d = Distance3D( ptb->Wheel[2].X, ptb->Wheel[2].Y, ptb->Wheel[2].Z, x,y,z ) ;

	if( ptb->Speed >= 0 )
	{
		ptb->Wheel[2].Angle += (4096*10*d)/c ;
	}
	else
	{
		ptb->Wheel[2].Angle -= (4096*10*d)/c ;
	}

	ptb->Wheel[2].X = x ;
	ptb->Wheel[2].Y = y ;
	ptb->Wheel[2].Z = z ;

	// +++++++++++ back right wheel

	LongRotate( -400, -350, ptb3d->Beta ) ;
	x = Nxw + X0 ;
	z = Nzw + Z0 ;
	y = yw ;

	if( x>=0 AND x<32768
	AND z>=0 AND z<32768 )
	{
		y += CalculAltitudeObjet( x, z, -1 ) ;
	}

	c = (360 * 31415)/1000 ;	// circonsference * 10
	d = Distance3D( ptb->Wheel[3].X, ptb->Wheel[3].Y, ptb->Wheel[3].Z, x,y,z ) ;

	if( ptb->Speed >= 0 )
	{
		ptb->Wheel[3].Angle += (4096*10*d)/c ;
	}
	else
	{
		ptb->Wheel[3].Angle -= (4096*10*d)/c ;
	}

	ptb->Wheel[3].X = x ;
	ptb->Wheel[3].Y = y ;
	ptb->Wheel[3].Z = z ;

	// +++++++++++ front right wheel
	ptb3d->CurrentFrame[4].Alpha = (S16)ptb->Wheel[1].Angle ;
	// +++++++++++ front left wheel
	ptb3d->CurrentFrame[7].Alpha = (S16)ptb->Wheel[0].Angle ;
	// +++++++++++ back left wheel
	ptb3d->CurrentFrame[11].Alpha = (S16)ptb->Wheel[2].Angle ;
	// +++++++++++ back right wheel
	ptb3d->CurrentFrame[9].Alpha = (S16)ptb->Wheel[3].Angle ;

	// inclinaison voiture (pitch)
	ptb3d->CurrentFrame[1].Type = 0 ;

	LongRotate( 0, 400, ptb3d->Beta ) ;
	x1 = X0 ;
	z1 = Z0 ;
	LongRotate( 0, -400, ptb3d->Beta ) ;

	if( Nxw+x1>=0 AND Nxw+x1<32768
	AND Nzw+z1>=0 AND Nzw+z1<32768 )
	{
		y = CalculAltitudeObjet( Nxw+x1, Nzw+z1, -1 ) ;
	}
	else	y = yw ;

	if( Nxw+X0>=0 AND Nxw+X0<32768
	AND Nzw+Z0>=0 AND Nzw+Z0<32768 )
	{
		y1 = CalculAltitudeObjet( Nxw+X0, Nzw+Z0, -1 ) ;
	}
	else	y1 = yw ;

	ptb3d->CurrentFrame[1].Alpha = (S16)(1024-GetAngle2D( 0, y, 800, y1 ) ) ;
	ptb->Alpha = ptb3d->CurrentFrame[1].Alpha ;

	// inclinaison voiture (roll)
	LongRotate( 400, 0, ptb3d->Beta ) ;
	x1 = X0 ;
	z1 = Z0 ;
	LongRotate( -400, 0, ptb3d->Beta ) ;

	if( Nxw+X0>=0 AND Nxw+X0<32768
	AND Nzw+Z0>=0 AND Nzw+Z0<32768 )
	{
		y = CalculAltitudeObjet( Nxw+X0, Nzw+Z0, -1 ) ;
	}
	else	y = yw ;

	if( Nxw+x1>=0 AND Nxw+x1<32768
	AND Nzw+z1>=0 AND Nzw+z1<32768 )
	{
		y1 = CalculAltitudeObjet( Nxw+x1, Nzw+z1, -1 ) ;
	}
	else	y1 = yw ;

	ptb3d->CurrentFrame[1].Gamma = (S16)GetAngle2D( y, 0, y1, 800 ) ;

	// volant
	ptb3d->CurrentFrame[12].Gamma = (S16)-ptb->BetaWheel ;

	// tete twinsen
	ptb3d->CurrentFrame[14].Beta = (S16)ptb->BetaWheel ;
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
