/*══════════════════════════════════════════════════════════════════════════*

	██████┐ ██┐ ██┐ ██████┐ ██████┐ ██┐ ██┐ ██████┐         ██████┐
	└─██┌─┘ ███┐██│ └─██┌─┘ ██┌───┘ ██│ ██│ └─██┌─┘         ██┌───┘
	  ██│   ██████│   ██│   █████┐  └████┌┘   ██│           ██│
	  ██│   ██┌███│   ██│   ██┌──┘  ██┌─██┐   ██│           ██│
	██████┐ ██│└██│   ██│   ██████┐ ██│ ██│   ██│     ██┐   ██████┐
	└─────┘ └─┘ └─┘   └─┘   └─────┘ └─┘ └─┘   └─┘     └─┘   └─────┘

	Module commun aux mondes interieurs et exterieurs encapsulant
	certaines fonctions.

 *══════════════════════════════════════════════════════════════════════════*/

#include	"c_extern.h"

extern	T_OBJET	*APtObj ;	// dans OBJECT.CPP

S32	SaveVueOffsetX, SaveVueOffsetY, SaveVueOffsetZ ;
S32	SaveAlphaCam, SaveBetaCam, SaveGammaCam ;
S32	SaveVueDistance ;
S32	SaveAlphaLight, SaveBetaLight ;

S32	AddBetaCam = 0 ;

// Pointeurs de fonctions communs au deux mondes
Func_ReajustPos	*PtrReajustPos 	= ReajustPos 	;
Func_GetShadow	*PtrGetShadow  	= GetShadow  	;
Func_DoAnim	*PtrDoAnim	= DoAnim 	;
Func_WorldColBrick		*PtrWorldColBrick	 = WorldColBrick	;
Func_WorldColBrickVisible	*PtrWorldColBrickVisible = WorldColBrickVisible ;

Func_Init3DView	*PtrInit3DView	= Init3DView 	;

#ifndef	COMPILATOR
Func_ProjectShadow	*PtrProjectShadow  = ProjectShadow  ;
Func_ReajustPosExtra	*PtrReajustPosExtra = ReajustPosExtra ;
#endif

Func_InitGrille	*PtrInitGrille	= InitGrille 	;

#ifndef  LBA_EDITOR
Func_FullWorldColBrick	*PtrFullWorldColBrick = FullWorldColBrick ;
#endif

/*══════════════════════════════════════════════════════════════════════════*
			    █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀
			    ██ ▀█ ██▀▀█ ██▀ █ ██▀▀
			    ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/

// Initialise la projection de la camera ainsi que les pointeurs de fonctions
// suivant le mode du cube courant (intérieur ou extérieur)
void	PtrInit3DGame()
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		Init3DGame() ;
	}
	else
	{
		Init3DExtGame() ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀█ █▀▀█      █ █▀▀▀▀ █▀▀▀▀ ▀▀█▀▀ ██▀▀▀
		   ██  █ ██▀▀█ ▄▄  █ ██▀▀  ██      ██  ▀▀▀▀█
		   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/

// Routine de recouvrement: trouver le moyen de passer les mêmes paramètres
// en extérieur et en intérieur pour optimiser avec un pointeur de fonction
S32	DrawRecover( S32 xw, S32 yw, S32 zw, S32 ymax, S32 zvmax )
{
	S32	flagbox ;

	if( CubeMode==CUBE_INTERIEUR )
	{
		DrawOverBrick( 	xw/SIZE_BRICK_XZ,
				yw/SIZE_BRICK_Y,
				zw/SIZE_BRICK_XZ ) ;

		BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;

		flagbox = 0 ;
	}
	else
	{
		S32	z0, z1 ;

		LongWorldRotatePoint( xw, yw, zw ) ;

//		z0 = CameraZr-Z0-256 ;	// calculer par rapport à la zv
		z0 = CameraZr-Z0-zvmax ;// calculer par rapport à la zv

		LongWorldRotatePoint( xw, yw+ymax, zw ) ;

		// ~ max [ (xmax-xmin)/2, (zmax-zmin)/2 ]
//		z1 = CameraZr-Z0-256 ;	// calculer par rapport à la zv
		z1 = CameraZr-Z0-zvmax ;// calculer par rapport à la zv

		flagbox = ZBufBoxOverWrite2( z0, z1, ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;

		if( !flagbox )	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
	}

	return flagbox ;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Routine de recouvrement des sprites: trouver le moyen de passer les mêmes paramètres
// en extérieur et en intérieur pour optimiser avec un pointeur de fonction
void	DrawRecover3( S32 xw, S32 yw, S32 zw, S32 ymax, S32 zvmax )
{
	S32	flagbox ;

	if( CubeMode==CUBE_INTERIEUR )
	{
		DrawOverBrick3( xw/SIZE_BRICK_XZ,
				yw/SIZE_BRICK_Y,
				zw/SIZE_BRICK_XZ,
				(yw+ymax)/SIZE_BRICK_Y ) ;

		BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
	}
	else
	{
		S32	z0 ;

		LongWorldRotatePoint( xw, yw, zw ) ;

//		z0 = CameraZr-Z0-256 ;	// calculer par rapport à la zv
		z0 = CameraZr-Z0-zvmax ;	// calculer par rapport à la zv

		flagbox = ZBufBoxOverWrite2( z0, z0, ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;

		if( !flagbox )	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
	}
}


/*══════════════════════════════════════════════════════════════════════════*
			       █▄ ▄█ █▀▀▀█ █▀▀▀█
			       ██▀ █ ██▀▀█ ██▀▀▀
			       ▀▀  ▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/

// Affiche la grille complete
void	RefreshGrille()
{
#ifdef	LBA_EDITOR
	if( CubeClip )	RefreshClippedGrille() ;
	else
	{
#endif
		switch( CubeMode )
		{
			case CUBE_INTERIEUR:
				if( ModeLabyrinthe )
				{
					AffGrilleOnly() ;	// Pour recalculer le Z-Buffer
					AffGrilleLabyrinthe() ;	// Pour redessiner la Map
				}
				else	AffGrille() ;
				break ;

			case CUBE_EXTERIEUR:
				AffGrilleExt() ;
				break ;
		}
#ifdef	LBA_EDITOR
	}
#endif
}

#ifdef	LBA_EDITOR
/*──────────────────────────────────────────────────────────────────────────*/
// Affiche la grille clippée
void	RefreshClippedGrille()
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		ClippedAffGrille() ;
	}
	else
	{
		// inserer affichage clippé en mode extérieur ? ben voyons !
		AffGrilleExt() ;
	}
}
#endif

/*──────────────────────────────────────────────────────────────────────────*/
// Recupere numero code jeu ou sample en xw,yw,zw

U8	PtrWorldCodeBrick( S32 xw, S32 yw, S32 zw )
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		if( xw<0 OR xw>=(SIZE_BRICK_XZ*SIZE_CUBE_X)
		OR  yw<0 OR yw>=(SIZE_BRICK_Y*SIZE_CUBE_Y)
		OR  zw<0 OR zw>=(SIZE_BRICK_XZ*SIZE_CUBE_Z) )
		{
			return 0 ;
		}
		else	return( WorldCodeBrick( xw, yw, zw ) ) ;
	}
	else
	{
		if( APtObj->CarryBy!=-1 )
		{
			S32	numdec = APtObj->CarryBy&~(CARRY_BY_DECORS) ;

			if( APtObj->CarryBy&CARRY_BY_DECORS )
				return( (U8)(ListDecors[APtObj->CarryBy&~(CARRY_BY_DECORS)].CodeJeu) ) ;
			else	return 0 ;	// porté par un objet scénarique
		}
		else if( !(APtObj->WorkFlags&FALLING)
		     AND  (APtObj->Flags&OBJ_FALLABLE) )
		{
			return( GiveTerrainCodeJeu( xw, yw, zw ) ) ;
		}
	}

	return 0 ;
}

#ifdef	LBA_EDITOR
/*──────────────────────────────────────────────────────────────────────────*/
void	PtrSearchFirstPseudo()
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		SearchFirstPseudo() ;
	}
	else
	{
		// Toujours en 0,0,0 donc il vaut mieux laisser la camera
		// là où elle est, puisque de toute manière on ne peut pas
		// atterrir dans une zone noire
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
// Recupere les coordonnees brick a la position ecran xp,yp (pour la souris)
// dans X0, Y0 et Z0
S32	PtrGetScreenBrick( S32 xp, S32 yp )
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		return( GetScreenBrick( xp, yp ) ) ;
	}
	else
	{
		if( SelectPoly(xp, yp) )
		{
			X0 = IndXPolySelected ;
			Z0 = IndZPolySelected ;
			Y0 = CalculAltitudeObjet( X0*SIZE_BRICK_XZ+256, Z0*SIZE_BRICK_XZ+256, -1 )/SIZE_BRICK_Y ;
			return TRUE ;
		}

		return FALSE ;
	}
}
#endif

/*══════════════════════════════════════════════════════════════════════════*
			 █▀▀▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀█ █   █
			 ██ ▀█ ██▀█▀ ██▀▀█ ██▀▀▀ ██▀▀█
			 ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀    ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/

// Calcule les coordonnees du sprite (SCENARIQUE) suivant le mode
void	PtrProjectSprite( S32 xw, S32 yw, S32 zw, S32 sprite )
{
	S32	mindx, mindy ;
	S16		*zvptr	;

	PtrProjectPoint( xw, yw, zw ) ;

	if( sprite>=100 )
	{
		ScaleFactorSprite = DEF_SCALE_FACTOR ;
		zvptr  = PtrZvExtra ;
	}
	else
	{
		CalculeScaleFactorSprite( xw, yw, zw, -1 ) ;
		zvptr  = PtrZvExtraRaw ;
	}

	mindx = zvptr[ sprite*8 + 0 ] ;
	mindy = zvptr[ sprite*8 + 1 ] ;

	SpriteX = Xp + mindx ;
	SpriteY = Yp + mindy ;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Calcule la taille du sprite suivant le mode
void	PtrGetDxDyGraph( S32 sprite, S32 *dx, S32 *dy )
{
	S16	*zvptr	;

	// ne tient pas compte, pour l'instant en tout cas, du
	// ScaleSprite

	if( sprite>=100 )
	{
		zvptr  = PtrZvExtra ;
	}
	else
	{
		zvptr  = PtrZvExtraRaw ;
	}

	*dx = zvptr[ sprite*8 + 2 ] ;
	*dy = zvptr[ sprite*8 + 3 ] ;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Affiche un sprite (GPH en intérieur, RAW en extérieur pour le zoomer).
// Trouver un moyen pour avoir les mêmes paramètres pour optimiser avec
// un pointeur de fonction (aligner la fonction intérieur sur extérieur)
void	PtrAffGraph( S32 xp, S32 yp, S32 sprite )
{
	if( sprite>=100 )	// c'est un sprite goodies (interieur)
	{
		AffGraph( 0, xp, yp, HQR_Get(HQRPtrSprite,sprite) ) ;
	}
	else
	{
		ScaleSprite( 0, xp, yp, ScaleFactorSprite, ScaleFactorSprite, HQR_Get( HQRPtrSpriteRaw, sprite ) ) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
U8	*GetPtrSprite( S32 sprite )
{
	if( sprite>=100 )
	{
		return( (U8 *)HQR_Get( HQRPtrSprite, sprite ) ) ;
	}
	else
	{
		return( (U8 *)HQR_Get( HQRPtrSpriteRaw, sprite ) ) ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
			 █   █ █▀▀▀█ █▀▀▀█ █     █▀▀▀▄
			 ██▀▄█ ██  █ ██▀█▀ ██    ██  █
			 ▀   ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/

// Project un point dans l'espace. Comment faire pour passer ici par un
// pointeur de fonction vu que LongProjectPoint est déclaré en passage de
// paramètres par registres (voir lib_3D)
S32	PtrProjectPoint( S32 xw, S32 yw, S32 zw )
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		return( LongProjectPoint( xw, yw, zw ) ) ;
	}
	else
	{
		LongWorldRotatePoint( xw, yw, zw ) ;
		return( LongProjectPoint( X0, Y0, Z0 ) ) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
// Recentre la camera.
//
//	En mode exterieur, flagbeta peut avoir les valeurs suivantes:
//		0 --> Ne pas reorienter la camera
//		1 --> Reorienter la camera derriere NumObjFollow
//		2 --> Reorienter la camera derriere NumObjFollow en tenant
//		      compte de AddBetaCam (cameras orientees)
//		3 --> Don't touch Globales. Juste appel des fonctions Caméras

void	CameraCenter( S32 flagbeta )
{
	T_OBJ_3D	*ptr3d = &ListObjet[NumObjFollow].Obj ;
	S32	newbeta ;

	switch( flagbeta )
	{
		case 2:
#ifdef	LBA_EDITOR
			VueDistance = DEFAULT_DISTANCE_CAM ;
			AlphaCam = DEFAULT_ALPHA_CAM ;
#else
			VueDistance = DefVueDistance[VueCamera] ;
			AlphaCam = DefAlphaCam[VueCamera] ;
#endif

			newbeta = AddBetaCam + ptr3d->Beta ;
			newbeta &= 4095 ;

			Rotate( 0, DISTANCE_VISEE, newbeta ) ;

			VueOffsetX = ptr3d->X + X0 ;
			VueOffsetY = ptr3d->Y  ;
			VueOffsetZ = ptr3d->Z + Z0 ;

			StartXCube = VueOffsetX/SIZE_BRICK_XZ ;
			StartYCube = VueOffsetY/SIZE_BRICK_Y  ;
			StartZCube = VueOffsetZ/SIZE_BRICK_XZ ;

			BetaCam = 2048 - newbeta ;
			BetaCam &= 4095 ;
			break ;

		case 1:
			AddBetaCam = 0 ;
			GammaCam = DEFAULT_GAMMA_CAM ;
#ifdef	LBA_EDITOR
			VueDistance = DEFAULT_DISTANCE_CAM ;
			AlphaCam = DEFAULT_ALPHA_CAM ;
#else
			VueDistance = DefVueDistance[VueCamera] ;
			AlphaCam = DefAlphaCam[VueCamera] ;
#endif

			BetaCam = 2048 - ptr3d->Beta ;
			BetaCam &= 4095 ;

			Rotate( 0, DISTANCE_VISEE, ptr3d->Beta ) ;

			VueOffsetX = ptr3d->X+X0 ;
			VueOffsetY = ptr3d->Y    ;
			VueOffsetZ = ptr3d->Z+Z0 ;

			StartXCube = VueOffsetX/SIZE_BRICK_XZ ;
			StartYCube = VueOffsetY/SIZE_BRICK_Y  ;
			StartZCube = VueOffsetZ/SIZE_BRICK_XZ ;

			break ;

		case 0:
			VueOffsetX = StartXCube*SIZE_BRICK_XZ ;
			VueOffsetY = StartYCube*SIZE_BRICK_Y  ;
			VueOffsetZ = StartZCube*SIZE_BRICK_XZ ;
			break ;
	}


	if( CubeMode==CUBE_INTERIEUR )
	{
		SetPosCamera( StartXCube*SIZE_BRICK_XZ,
			      StartYCube*SIZE_BRICK_Y,
			      StartZCube*SIZE_BRICK_XZ ) ;

		if( FlagWater )	// reajuste clipping y de l'eau
		{
			LongProjectPoint( ListObjet[NUM_PERSO].Obj.X,
					  ListObjet[NUM_PERSO].Obj.Y,
					  ListObjet[NUM_PERSO].Obj.Z ) ;

			FlagWater = Yp ;
		}
	}
	else
	{
		SetFollowCamera(	VueOffsetX, VueOffsetY, VueOffsetZ,
					AlphaCam, BetaCam, GammaCam,
					VueDistance ) ;

		if( !CameraZone AND flagbeta!=3 )
		{
			if( SearchCameraPos( ptr3d->X, ptr3d->Y, ptr3d->Z, ptr3d->Beta, 1 ) )
			{
				FlagCameraForcee = TRUE ;

				SetPosCamera( CamPosX, CamPosY, CamPosZ ) ;
				SetTargetCamera( VueOffsetX, VueOffsetY, VueOffsetZ, 0 ) ;
			}
		}
	}

	SampleAlwaysMove.Acc = 1000 ;	// On veut reforcer les samples
					// à se repaner lors d'1 chgt de camera
}


#ifdef	LBA_EDITOR
void	FlyCamera( 	S32 offx, S32 offy, S32 offz,
			S32 alpha, S32 beta, S32 gamma, S32 distance,
			S32 delay )
{
	S32	time = 0 ;
	S32	savetimer ;

	SaveTimer() ;
	SaveCamera() ;

	ManageTime() ;
	savetimer = TimerSystemHR ;

	while( time<delay AND MyKey!=K_ESC )
	{
		ManageTime() ;
		MyGetInput() ;

		time = TimerSystemHR-savetimer ;

		VueOffsetX = RegleTrois( SaveVueOffsetX, offx, delay, time ) ;
		VueOffsetY = RegleTrois( SaveVueOffsetY, offy, delay, time ) ;
		VueOffsetZ = RegleTrois( SaveVueOffsetZ, offz, delay, time ) ;

		AlphaCam = RegleTrois( SaveAlphaCam, alpha, delay, time )&4095 ;
		BetaCam  = RegleTrois( SaveBetaCam,  beta,  delay, time )&4095 ;
		GammaCam = RegleTrois( SaveGammaCam, gamma, delay, time )&4095 ;

		VueDistance = RegleTrois( SaveVueDistance, distance, delay, time ) ;

		SetFollowCamera(	VueOffsetX, VueOffsetY, VueOffsetZ,
					AlphaCam, BetaCam, GammaCam,
					VueDistance ) ;

		if( FlagInfos&INFO_FRAME_SPEED )
		{
			if( FlagInfos&INFO_REPERE )
			{
				Switch_Fillers( FILL_POLY_FOG_ZBUFFER );

				NbAniPoly = 0 ;
				PtrAniPoly = PtrPolySea ;

				ClsTerrainZBuf() ;

#ifdef	LBA_EDITOR
				if( FlagInfos&INFO_HORIZON )
#else
				if( FlagDrawHorizon )
#endif
				{
					DrawHorizon2ZBuf() ;
				}

				AffichageTerrainZBuf() ;

				DrawSkySeaZBuf() ;

				Switch_Fillers( FILL_POLY_TEXTURES );
			}
			else
			{
				Cls() ;
				AffichageTerrainRapide() ;
			}

			BoxUpdate();
		}
		else	AffScene( AFF_ALL_FLIP ) ;
	}

	WaitNoInput() ;

	VueOffsetX = offx ;
	VueOffsetY = offy ;
	VueOffsetY = offy ;

	AlphaCam = alpha ;
	BetaCam  = beta  ;
	GammaCam = gamma ;

	VueDistance = distance ;

	CameraCenter( 0 ) ;
	AffScene( AFF_ALL_FLIP ) ;

	RestoreTimer() ;
}
#endif

/*──────────────────────────────────────────────────────────────────────────*/
// Sauve etat actuel de la camera
void	SaveCamera()
{
	SaveVueOffsetX 	= VueOffsetX 	;
	SaveVueOffsetY 	= VueOffsetY 	;
	SaveVueOffsetZ 	= VueOffsetZ 	;

	SaveAlphaCam	= AlphaCam 	;
	SaveBetaCam	= BetaCam	;
	SaveGammaCam	= GammaCam	;

	SaveVueDistance	= VueDistance	;

	SaveAlphaLight	= AlphaLight	;
	SaveBetaLight	= BetaLight	;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Restaure la camera précédemment sauvée avec SaveCamera
void	RestoreCamera()
{
	VueOffsetX	= SaveVueOffsetX 	;
	VueOffsetY	= SaveVueOffsetY 	;
	VueOffsetZ	= SaveVueOffsetZ 	;

	AlphaCam	= SaveAlphaCam		;
	BetaCam		= SaveBetaCam		;
	GammaCam	= SaveGammaCam		;

	VueDistance	= SaveVueDistance	;

	AlphaLight	= SaveAlphaLight	;
	BetaLight	= SaveBetaLight		;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Calculte la hauteur du decors sous un point 3D
S32	PtrCalculAltitudeY( S32 x, S32 z )
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		return 0 ;
	}
	else
	{
		return( CalculAltitudeObjet( x, z, -1 ) ) ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
			 ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █     ██▀▀▀
			   ██  ██  █ ██  █ ██    ▀▀▀▀█
			   ▀▀  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
		     (partie seulement utilisée dans l'outil)
 *══════════════════════════════════════════════════════════════════════════*/

#ifdef	LBA_EDITOR
// Appelee lors d'un click sur le decors pour la gestion du BrickMenu
void	GereBrickMouse()
{
	static	timernextclick ;

	if( CubeMode==CUBE_INTERIEUR )
	{
		if( GetScreenBrick( MouseX, MouseY ) )
		{
			if( MyClick & 2 )	/* repositionne perso */
			{
				if( TimerRefHR>timernextclick )
				{
					ListObjet[NUM_PERSO].Obj.X = X0 * SIZE_BRICK_XZ + DEMI_BRICK_XZ ;
					ListObjet[NUM_PERSO].Obj.Y = Y0 * SIZE_BRICK_Y + SIZE_BRICK_Y ;
					ListObjet[NUM_PERSO].Obj.Z = Z0 * SIZE_BRICK_XZ + DEMI_BRICK_XZ ;

					timernextclick = TimerRefHR + 200 ;
				}

				NumObjFollow = NUM_PERSO ;
			}
			else
			{
				if( LastXmBrickSelected == X0
				AND LastYmBrickSelected == Y0
				AND LastZmBrickSelected == Z0 )
				{
					SaveTimer() ;
					GereBrickMenu() ;
					CameraCenter( 1 ) ;
					RestoreTimer() ;
					FirstTime = AFF_ALL_FLIP ;
				}
				else
				{
					LastXmBrickSelected = X0 ;
					LastYmBrickSelected = Y0 ;
					LastZmBrickSelected = Z0 ;
					WaitNoInput() ;
				}
			}
		}
	}
	else	// Gestion souris mode exterieur
	{
		if( !GereMouseMovements(3) )
		{
		if( SelectPoly(MouseX, MouseY) )
		{
			DrawGrilleFeuille3D( IndXPolySelected, IndZPolySelected ) ;

			if( MyClick & 2 )
			{
				if( TimerRefHR>timernextclick )
				{
					ListObjet[NUM_PERSO].Obj.X = IndXPolySelected*SIZE_BRICK_XZ+256 ;
					ListObjet[NUM_PERSO].Obj.Z = IndZPolySelected*SIZE_BRICK_XZ+256 ;
					ListObjet[NUM_PERSO].Obj.Y = CalculAltitudeObjet( ListObjet[NUM_PERSO].Obj.X, ListObjet[NUM_PERSO].Obj.Z, -1 ) ;

					timernextclick = TimerRefHR + 200 ;
				}

				UnsetClip() ;
				GraphPrintf( FALSE, 0,10,"xi=%d zi=%d yi=%d y=%d ", IndXPolySelected, IndZPolySelected, ListObjet[NUM_PERSO].Obj.Y/256, ListObjet[NUM_PERSO].Obj.Y ) ;
			}
			else
			{
				S32	xi, yi, zi ;

				xi = IndXPolySelected ;
				zi = IndZPolySelected ;
				yi = CalculAltitudeObjet( xi*SIZE_BRICK_XZ, zi*SIZE_BRICK_XZ, -1 )/SIZE_BRICK_Y ;

				if( LastXmBrickSelected == xi
				AND LastYmBrickSelected == yi
				AND LastZmBrickSelected == zi )
				{
					SaveTimer() ;
					GereBrickMenu() ;
					RestoreTimer() ;
					FirstTime = AFF_ALL_FLIP ;
				}
				else
				{
					LastXmBrickSelected = xi ;
					LastYmBrickSelected = yi ;
					LastZmBrickSelected = zi ;
					while( MyClick )
					{
						MyGetInput() ;
						if( MyClick&2 )	break ;
					}
				}
			}
		}
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
// La gestion des touches est différente suivant le mode du cube.
// Mettre un pointeur de fonction ? Bof, c'est juste pour l'outil !
S32	PtrGereKeys( U32 key, U32 flagmenu )
{
	if( CubeMode==CUBE_INTERIEUR )
	{
		return( GereSpecialKeys( key, flagmenu ) ) ;
	}
	else
	{
		return( GereExtKeys( key, flagmenu ) ) ;
	}
}

#endif
