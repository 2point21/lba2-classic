#include 	"c_extern.h"

/*
WORD	GiveMaxLevel( WORD x, WORD y ) ;
void	DrawOverBrick( WORD xm, WORD ym, WORD zm ) ;
void	DrawOverBrick2( WORD xm, WORD ym, WORD zm ) ;
*/
UBYTE	WorldColBrick( WORD xw, WORD yw, WORD zw ) ;

WORD	Nxw, Nyw, Nzw ;
WORD	SaveNxw, SaveNyw, SaveNzw ;
WORD	OldX, OldY, OldZ ;
WORD	AnimNumObj ;
T_OBJET	*APtObj ;

WORD	LastJoyFlag = FALSE ;
WORD	LastMyJoy = 0 ;
WORD	LastMyFire = 0 ;

WORD	Col1 ;

void	IncrustGrm( WORD numgrm ) ;

WORD	ZoneGrm = -1 ;
WORD	IndexGrm = -1 ;

WORD	BetaUsedObj = 0 ;

extern	LONG	XMap, YMap, ZMap ;
extern	WORD	M_Xmin, M_Xmax, M_Ymin, M_Ymax, M_Zmin, M_Zmax ;
extern	UBYTE	*SearchPtrAnimAction ;

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		   Û฿฿฿Û Û฿฿Û      Û Û฿฿฿฿ Û฿฿฿฿ ฿฿Û฿฿ ÛÛ฿฿฿
		   ÛÛ  Û ÛÛ฿฿Û ÜÜ  Û ÛÛ฿฿  ÛÛ      ÛÛ  ฿฿฿฿Û
		   ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿   ฿฿  ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

extern	UBYTE	*PtrScene ;

void	InitObject( WORD numobj )
{
	T_OBJET *ptrobj ;
	UBYTE	*ptr ;

	ptrobj = &ListObjet[numobj] ;

        ptrobj->GenBody = GEN_BODY_NORMAL ;
	ptrobj->GenAnim = GEN_ANIM_RIEN ;

	ptrobj->PosObjX = 0 ;
	ptrobj->PosObjY = SIZE_BRICK_Y ;
	ptrobj->PosObjZ = 0 ;

	ptrobj->Xmin = 0 ;
	ptrobj->Xmax = 0 ;
	ptrobj->Ymin = 0 ;
	ptrobj->Ymax = 0 ;
	ptrobj->Zmin = 0 ;
	ptrobj->Zmax = 0 ;

	ptrobj->Beta = 0 ;

	ptrobj->SRot = 40 ;
	ptrobj->Move = NO_MOVE ;
	ptrobj->Info = 0 ;
	ptrobj->Info1 = 0 ;
	ptrobj->Info2 = 0 ;
	ptrobj->Info3 = 0 ;

	ptrobj->Col = 0 ;
	ptrobj->ObjCol = -1 ;
	ptrobj->CarryBy = -1 ;

	ptrobj->ZoneSce = -1 ;

	ptrobj->Flags = 0 ;
	ptrobj->WorkFlags = 0 ;

	ptrobj->LifePoint = 50 ;
	ptrobj->Armure = 1 ;
	ptrobj->HitBy = -1 ;

	ptrobj->AnimStepBeta = 0 ;

	ptrobj->AnimStepX = 0 ;
	ptrobj->AnimStepY = 0 ;
	ptrobj->AnimStepZ = 0 ;

//	ptrobj->GenBody = NO_BODY ;
//	ptrobj->GenAnim = NO_ANIM ;
	ptrobj->Body = -1 ;
	ptrobj->Anim = -1 ;
	ptrobj->FlagAnim = 0 ;
	ptrobj->Frame = 0 ;

	InitRealAngle(	0,0, 0,	&ptrobj->RealAngle ) ;

	ptrobj->OffsetTrack = -1 ;
	ptrobj->OffsetLife = 0	;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	StartInitObj( WORD numobj )
{
	T_OBJET	*ptrobj ;
	WORD	m ;

	ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Flags & SPRITE_3D )
	{
		if( ptrobj->HitForce != 0 )
			ptrobj->WorkFlags |= OK_HIT ;

		ptrobj->Body = -1 ;
		InitSprite( ptrobj->Sprite, numobj ) ;

		InitRealAngle(	0,0,0, &ptrobj->RealAngle ) ;

		if( ptrobj->Flags & SPRITE_CLIP )
		{
			ptrobj->AnimStepX = ptrobj->PosObjX ;
			ptrobj->AnimStepY = ptrobj->PosObjY ;
			ptrobj->AnimStepZ = ptrobj->PosObjZ ;
		}
	}
	else
	{
		ptrobj->Body = -1 ;
		InitBody( ptrobj->GenBody, numobj ) ;

		ptrobj->Anim = -1 ;
		ptrobj->FlagAnim = 0 ;
		if( ptrobj->Body != -1 )
		{
			InitAnim( ptrobj->GenAnim, ANIM_REPEAT, NO_ANIM, numobj ) ;
		}

		InitRealAngle(	ptrobj->Beta,
				ptrobj->Beta,
				0, &ptrobj->RealAngle ) ;
	}

	ptrobj->OffsetTrack = -1 ;
	ptrobj->LabelTrack = -1 ;
	ptrobj->OffsetLife = 0 ;
}


/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	StartInitAllObjs()
{
	WORD	n ;

	for( n=1; n<NbObjets; n++ )
	{
		StartInitObj( n ) ;
	}

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	LoadFicPerso()
{
	T_OBJET *ptrobj ;

	ptrobj = &ListObjet[NUM_PERSO] ;

	HQRM_Load( PATH_RESSOURCE"FILE3D.HQR",FILE_3D_SPORTIF , &PtrFile3dSportif ) ;
	CHECK_MEMORY
	ptrobj->PtrFile3D = PtrFile3dSportif ;
	AnimRienSportif = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO ) ;

	HQRM_Load( PATH_RESSOURCE"FILE3D.HQR",FILE_3D_AGRESSIF, &PtrFile3dAgressif ) ;
	CHECK_MEMORY
	ptrobj->PtrFile3D = PtrFile3dAgressif ;
	AnimRienAgressif = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO ) ;

	HQRM_Load( PATH_RESSOURCE"FILE3D.HQR",FILE_3D_DISCRET, &PtrFile3dDiscret ) ;
	CHECK_MEMORY
	ptrobj->PtrFile3D = PtrFile3dDiscret ;
	AnimRienDiscret = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO ) ;

	HQRM_Load( PATH_RESSOURCE"FILE3D.HQR",FILE_3D_PROTOPACK, &PtrFile3dProtopack ) ;
	CHECK_MEMORY
	ptrobj->PtrFile3D = PtrFile3dProtopack ;
	AnimRienProtopack = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO ) ;

	HQRM_Load( PATH_RESSOURCE"FILE3D.HQR",FILE_3D_NORMAL, &PtrFile3dNormal ) ;
	CHECK_MEMORY
	ptrobj->PtrFile3D = PtrFile3dNormal ;
	AnimRienNormal = SearchAnim( GEN_ANIM_RIEN, NUM_PERSO ) ;

	ptrobj->PtrAnimAction = SearchPtrAnimAction ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	InitPerso()
{
	T_OBJET *ptrobj ;

	InitObject( NUM_PERSO ) ;

	ptrobj = &ListObjet[NUM_PERSO] ;

	ptrobj->GenBody = GEN_BODY_NORMAL ;

	ptrobj->LifePoint = 50 ;

	ptrobj->CoulObj = 4 ;	/* bleu twinkel */

	NbGoldPieces = 0 ;
	NbLittleKeys = 0 ;
	MagicPoint = 0 ;
	MagicBall = -1 ;
	NbCloverBox = 2 ;
	NbFourLeafClover = 2 ;
	Weapon = 0 ;
}
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	SetComportement( WORD comportement )
{
	T_OBJET	*ptrobj ;
	WORD	memogenbody ;

	ptrobj = &ListObjet[ NUM_PERSO ] ;

	switch( comportement )
	{
		case C_NORMAL:
			Comportement = C_NORMAL ;
			ptrobj->PtrFile3D = PtrFile3dNormal ;
			break ;

		case C_SPORTIF:
			Comportement = C_SPORTIF ;
			ptrobj->PtrFile3D = PtrFile3dSportif ;
			break ;

		case C_AGRESSIF:
			Comportement = C_AGRESSIF ;
			ptrobj->PtrFile3D = PtrFile3dAgressif ;
			break ;

		case C_DISCRET:
			Comportement = C_DISCRET ;
			ptrobj->PtrFile3D = PtrFile3dDiscret ;
			break ;

		case C_PROTOPACK:
			Comportement = C_PROTOPACK ;
			ptrobj->PtrFile3D = PtrFile3dProtopack ;
			break ;
	}

	memogenbody = ptrobj->GenBody ;
	ptrobj->GenBody = NO_BODY ;
	ptrobj->Body = -1 ;
	InitBody( memogenbody, NUM_PERSO ) ;

	ptrobj->GenAnim = NO_ANIM ;
	ptrobj->FlagAnim = 0 ;
	InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NO_ANIM, NUM_PERSO ) ;
}
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
// reinitialise tout perso sans toucher … sa position

void	RestartPerso()
{
	T_OBJET	*ptrobj ;
	UBYTE	memogenbody ;

	ptrobj = &ListObjet[NUM_PERSO] ;

	ptrobj->Move = MOVE_MANUAL ;

	ptrobj->WorkFlags = 0 ;
	ptrobj->Flags =	  OBJ_FALLABLE
			+ CHECK_ZONE
			+ CHECK_OBJ_COL
			+ CHECK_BRICK_COL
			+ CHECK_CODE_JEU ;

	ptrobj->Armure = 1 ;

	ptrobj->OffsetTrack = -1 ;
	ptrobj->LabelTrack = -1 ;
	ptrobj->OffsetLife = 0 ;
	ptrobj->ZoneSce = -1 ;

ptrobj->Beta = SaveBeta ;

	InitRealAngle(	ptrobj->Beta,
			ptrobj->Beta,
			0, &ptrobj->RealAngle ) ;

SetComportement( SaveComportement ) ;
//	SetComportement( Comportement ) ;

	FlagWater = FALSE ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/*	Call Every Change Cube	    */

void	ClearFlagsCube()
{
	WORD	n ;

	for( n=0; n<MAX_FLAGS_CUBE; n++ )
	{
		ListFlagCube[n] = 0 ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	  Û฿฿฿฿ Û฿฿฿฿ Û฿฿฿Û Û฿฿฿฿       ÛÛ฿฿฿ Û฿฿฿฿ Û฿฿฿฿ ÛÛÜ Û Û฿฿฿฿
	  ÛÛ ฿Û ÛÛ฿฿  ÛÛ฿Û฿ ÛÛ฿฿        ฿฿฿฿Û ÛÛ    ÛÛ฿฿  ÛÛ฿ÛÛ ÛÛ฿฿
	  ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ClearScene()
{
	UBYTE	stringanim[256] ;
	UBYTE	stringbody[256] ;
	LONG	n ;
	T_OBJET *ptrobj ;
	UBYTE	*ptr ;

	ClearExtra() ;
	ClearFlagsCube() 	;

	for( n=0; n<MAX_INCRUST_DISP; n++ )
	{
		ListIncrustDisp[n].Num = -1 ;
	}

	HQR_Reset_Ressource( HQR_Anims ) ;

	HQM_Free_All() ;

	NbBodys = 0 ;
	FlagPalettePcx = FALSE ;

	ClearDial() ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// need: NewCube

// FlagChgCube == 1 : zone de chg cube recupere pos relative
// FlagChgCube == 2 : instruction chg cube positionne sur saved StartPos

void	ChangeCube()
{
	WORD	oldcube ;
	T_OBJET *ptrobj ;
	WORD	newjingle = -1 ;

	// grosse rustine scenarique
	if( (NewCube == 4) AND (ListFlagGame[30]!=0) )
	{
		NewCube = 118 ;
	}

	oldcube = NumCube ;
	NumCube = NewCube ;

// clear

	HQ_StopSample() ;

	if( FlagPalettePcx )	FadeToBlack( PalettePcx ) ;
	else			FadeToBlack( PtrPal ) ;

	if( !FlagMCGA )
	{
		Cls() ;
		Flip() ;
	}

// demo version
/*
#ifdef	DEMO
	if( NumCube == 3 )
	{
//		PlayMidiFile( 6 ) ;
//		Credits() ;
//		TheEnd( PROGRAM_OK, "* End of Demo version." ) ;

		FlagTheEnd = 1 ;
		return ;
	}
#endif
*/
	FreeGrille() ;
	ClearScene() ;

// perso reinit

	LoadFicPerso() ;

	ListObjet[NUM_PERSO].Move = MOVE_MANUAL ;
	ListObjet[NUM_PERSO].ZoneSce = -1 ;
	ListObjet[NUM_PERSO].OffsetLife = 0 ;
	ListObjet[NUM_PERSO].OffsetTrack = -1 ;
	ListObjet[NUM_PERSO].LabelTrack = -1 ;

// lance usine

	LoadScene( NewCube ) ;

	if( NumHolomapTraj != -1 )
	{
		TestRestoreModeSVGA( FALSE ) ;
		SetBlackPal() ;
		HoloTraj( NumHolomapTraj ) ;
		NumHolomapTraj = -1 ;
	}

	if( (NumCube == 116) OR (NumCube == 117) )	Island = 10 ;

	InitDial( START_FILE_ISLAND+Island ) ;// Pas Mini (Loran)

// play midi
	if( CubeJingle != 255 )
	{
		if( (NumXmi != CubeJingle) AND (IsMidiPlaying()) )
		{
			FadeMusicMidi( 1 ) ;
		}
	}

// load gri

	InitGrille( NewCube ) 		;/*	HQR	*/

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

	ListObjet[NUM_PERSO].PosObjX = SceneStartX ;
	StartYFalling = ListObjet[NUM_PERSO].PosObjY = SceneStartY ;
	ListObjet[NUM_PERSO].PosObjZ = SceneStartZ ;

//	CopyListFlagGame()	;	// save tat en rentrant dans le cube

	SetLightVector( AlphaLight, BetaLight, 0 ) ;
//	PlayMidiFile( CubeJingle ) ;

	if( NewCube != oldcube )
	{
		SaveComportement = Comportement ;
		SaveBeta = ListObjet[NUM_PERSO].Beta ;
		SaveGame() ;
	}

	RestartPerso() ;

	StartInitAllObjs() ;

	NbLittleKeys = 0 ;
	MagicBall = -1 ;
	LastJoyFlag = TRUE ;
	ZoneGrm = -1 ;
	IndexGrm = -1 ;
	FirstTime = TRUE ;
	FlagFade = TRUE ;
	CameraZone = FALSE ;
//	NumCube = NewCube ;
	NewCube = -1 ;
	FlagChgCube = 0 ;
	FlagAffGrille = TRUE ;

	SamplePlayed = 2+4+8 ;	// joue le premier sample en 1er
	TimerNextAmbiance = 0 ;

	StartXCube = ((ListObjet[NumObjFollow].PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
	StartYCube = ((ListObjet[NumObjFollow].PosObjY+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
	StartZCube = ((ListObjet[NumObjFollow].PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;

	SetLightVector( AlphaLight, BetaLight, 0 ) ;

	if( CubeJingle != 255 )
	{
		PlayMusic( CubeJingle ) ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	  Û฿฿฿฿ Û฿฿฿Û Û     Û      Û    ÛÛ฿฿฿  Û    Û฿฿฿Û ÛÛÜ Û ÛÛ฿฿฿
	  ÛÛ    ÛÛ  Û ÛÛ    ÛÛ     ÛÛ   ฿฿฿฿Û  ÛÛ   ÛÛ  Û ÛÛ฿ÛÛ ฿฿฿฿Û
	  ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿  ฿฿   ฿฿฿฿฿  ฿฿   ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	HitObj( WORD numhitter, WORD num, WORD hitforce, WORD beta )
{
	WORD	memo ;
	T_OBJET *ptrobjt ;

	ptrobjt = &ListObjet[ num ] ;

	if( ptrobjt->LifePoint <= 0 )	return ;

	ptrobjt->HitBy = numhitter ;

	if( ptrobjt->Armure <= hitforce )
	{
		if((ptrobjt->GenAnim == GEN_ANIM_CHOC)
		OR (ptrobjt->GenAnim == GEN_ANIM_CHOC2) )
		{
			memo = ptrobjt->Frame ;
			ptrobjt->Frame = 1 ;
			GereAnimAction( ptrobjt, num ) ;
			ptrobjt->Frame = memo ;
		}
		else
		{
			if( beta != -1 )
				InitRealAngle( beta,beta, 0, &ptrobjt->RealAngle ) ;

			if( !(rand()&1) )
				InitAnim( GEN_ANIM_CHOC, ANIM_INSERT, NO_ANIM, num ) ;
			else
				InitAnim( GEN_ANIM_CHOC2, ANIM_INSERT, NO_ANIM, num ) ;
		}

		/* effet special choc */
		/* etoile */
		InitSpecial(	ptrobjt->PosObjX,
				ptrobjt->PosObjY+1000,
				ptrobjt->PosObjZ,
				0 ) ;

		if( num == NUM_PERSO )
		{
			LastJoyFlag = TRUE ;
		}

		ptrobjt->LifePoint -= hitforce ;

		/* attention dans l'outil LifePoint est un word
		si ca devient un UBYTE pas de signe */
//		if( ptrobjt->LifePoint > 127 )	ptrobjt->LifePoint = 0 ;
		if( ptrobjt->LifePoint < 0 )	ptrobjt->LifePoint = 0 ;
	}
	else
	{
		InitAnim( GEN_ANIM_ENCAISSE, ANIM_INSERT, NO_ANIM, num ) ;
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

LONG	CheckZvOnZv( WORD numobj, WORD numobjt )
{
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	WORD	x0,y0,z0, x1,y1,z1 ;
	WORD	xt0,yt0,zt0, xt1,yt1,zt1 ;

	ptrobj = &ListObjet[numobj] ;

	x0 = Nxw + ptrobj->Xmin ;
	x1 = Nxw + ptrobj->Xmax ;
	y0 = Nyw + ptrobj->Ymin ;
	y1 = Nyw + ptrobj->Ymax ;
	z0 = Nzw + ptrobj->Zmin ;
	z1 = Nzw + ptrobj->Zmax ;

	ptrobjt = &ListObjet[numobjt] ;

	xt0 = ptrobjt->PosObjX + ptrobjt->Xmin ;
	xt1 = ptrobjt->PosObjX + ptrobjt->Xmax ;
	yt0 = ptrobjt->PosObjY + ptrobjt->Ymin ;
	yt1 = ptrobjt->PosObjY + ptrobjt->Ymax ;
	zt0 = ptrobjt->PosObjZ + ptrobjt->Zmin ;
	zt1 = ptrobjt->PosObjZ + ptrobjt->Zmax ;

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
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

WORD	CheckObjCol( WORD numobj )
{
	WORD	n, angle ;
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	WORD	x0,y0,z0, x1,y1,z1 ;
	WORD	xt0,yt0,zt0, xt1,yt1,zt1 ;

	ptrobj = &ListObjet[numobj] ;

	x0 = Nxw + ptrobj->Xmin ;
	x1 = Nxw + ptrobj->Xmax ;
	y0 = Nyw + ptrobj->Ymin ;
	y1 = Nyw + ptrobj->Ymax ;
	z0 = Nzw + ptrobj->Zmin ;
	z1 = Nzw + ptrobj->Zmax ;

	ptrobjt = ListObjet ;

	ptrobj->ObjCol = -1 ;

	for( n=0; n<NbObjets; n++,ptrobjt++ )
	{
		if( (n != numobj)
		AND (ptrobjt->Body != -1)
		AND (!(ptrobj->Flags&INVISIBLE))
		AND (ptrobjt->CarryBy != numobj) )
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
			ptrobj->ObjCol = n ;

			if( ptrobjt->Flags & OBJ_CARRIER )
			{
				// je touche un transporteur
				if( ptrobj->WorkFlags & FALLING )
				{
					Nyw = yt1 - ptrobj->Ymin + 1 ;
					ptrobj->CarryBy = n ;
					continue ;
				}
				else
				{
					if( CheckZvOnZv( numobj, n ) )
					{
						// je marche sur un transporteur */
						Nyw = yt1 - ptrobj->Ymin + 1 ;
						ptrobj->CarryBy = n ;
						continue ;
					}
				}
			}
			else
			{	// je marche sur quelqu'un
				if( CheckZvOnZv( numobj, n ) )
				{
					HitObj( numobj, n, 1, -1 ) ;
				}
			}

			angle = GetAngle( Nxw,Nzw,
					  ptrobjt->PosObjX,ptrobjt->PosObjZ ) ;

			// test obj pushable
			if( (ptrobjt->Flags & PUSHABLE)
			AND !(ptrobj->Flags & PUSHABLE) ) // protect reaction en chaine
			{
/*				if( numobj == NUM_PERSO )
				{
					InitAnim( "pousse", ANIM_REPEAT, 0, NUM_PERSO ) ;
				}
*/
				ptrobjt->AnimStepY = 0 ;

				if( ptrobjt->Flags & MINI_ZV )
				{ // magouille boxxle
					if( angle >= 128 AND angle < 384 )
						if( ptrobj->Beta >= 128 AND ptrobj->Beta < 384 )
							ptrobjt->AnimStepX = SIZE_BRICK_XZ/4 + SIZE_BRICK_XZ/8 ;
					if( angle >= 384 AND angle < 640 )
						if( ptrobj->Beta >= 384 AND ptrobj->Beta < 640 )
							ptrobjt->AnimStepZ = -SIZE_BRICK_XZ/4 + SIZE_BRICK_XZ/8 ;
					if( angle >= 640 AND angle < 896 )
						if( ptrobj->Beta >= 640 AND ptrobj->Beta < 896 )
							ptrobjt->AnimStepX = -SIZE_BRICK_XZ/4 + SIZE_BRICK_XZ/8 ;
					if( angle >= 896 OR  angle < 128 )
						if( ptrobj->Beta >= 896 OR ptrobj->Beta < 128 )
							ptrobjt->AnimStepZ = SIZE_BRICK_XZ/4 + SIZE_BRICK_XZ/8 ;
				}
				else
				{ // induit deplacement avant reajustement ?

					ptrobjt->AnimStepX = Nxw - ptrobj->OldPosX ;
					ptrobjt->AnimStepZ = Nzw - ptrobj->OldPosZ ;
				}
			}

			// joli mais ne fonctionne qu'avec
			// des ZV carres

			// donc rustine tempo
//			if( ptrobjt->Xmax-ptrobjt->Xmin ==
//			    ptrobjt->Zmax-ptrobjt->Zmin )
			if( (ptrobjt->Xmax-ptrobjt->Xmin ==
			    ptrobjt->Zmax-ptrobjt->Zmin )
			    AND
			    (ptrobj->Xmax-ptrobj->Xmin ==
			    ptrobj->Zmax-ptrobj->Zmin) )
			{   // si ZV carre


				if( angle >= 128 AND angle < 384 )
					Nxw = xt0 - ptrobj->Xmax ;
				if( angle >= 384 AND angle < 640 )
					Nzw = zt1 - ptrobj->Zmin ;
				if( angle >= 640 AND angle < 896 )
					Nxw = xt1 - ptrobj->Xmin ;
				if( angle >= 896 OR  angle < 128 )
					Nzw = zt0 - ptrobj->Zmax ;
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
		}
		}
	}

	// test deplace la ZV plus en avant si frappe

	if( ptrobj->WorkFlags & OK_HIT )
	{
		Rotate( 0, 200, ptrobj->Beta ) ;

		x0 = Nxw + ptrobj->Xmin + X0 ;
		x1 = Nxw + ptrobj->Xmax + X0 ;
		y0 = Nyw + ptrobj->Ymin ;
		y1 = Nyw + ptrobj->Ymax ;
		z0 = Nzw + ptrobj->Zmin + Y0 ;
		z1 = Nzw + ptrobj->Zmax + Y0 ;

		ptrobjt = ListObjet ;

		for( n=0; n<NbObjets; n++,ptrobjt++ )
		{
			if( (n != numobj)
			AND (ptrobjt->Body != -1)
			AND (!(ptrobj->Flags&INVISIBLE))
			AND (ptrobjt->CarryBy != numobj) )
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
					HitObj( numobj, n, ptrobj->HitForce, ptrobj->Beta+512 ) ;
					ptrobj->WorkFlags &= ~OK_HIT ;
				}
			}
		}
	}
	return ptrobj->ObjCol ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	CheckCarrier( WORD numobj )
{
	WORD	n ;

	if( ListObjet[numobj].Flags & OBJ_CARRIER )
	{
		for( n=0; n<NbObjets; n++ )
		{
			if( ListObjet[n].CarryBy == numobj )
			{
				ListObjet[n].CarryBy = -1 ;
			}
		}
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

LONG	CheckValidObjPos( WORD numobj )
{
	WORD	n, angle ;
	T_OBJET	*ptrobj ;
	T_OBJET	*ptrobjt ;
	WORD	x0,y0,z0, x1,y1,z1 ;
	WORD	xt0,yt0,zt0, xt1,yt1,zt1 ;

	ptrobj = &ListObjet[numobj] ;

	x0 = ptrobj->PosObjX + ptrobj->Xmin ;
	x1 = ptrobj->PosObjX + ptrobj->Xmax ;
	y0 = ptrobj->PosObjY + ptrobj->Ymin ;
	y1 = ptrobj->PosObjY + ptrobj->Ymax ;
	z0 = ptrobj->PosObjZ + ptrobj->Zmin ;
	z1 = ptrobj->PosObjZ + ptrobj->Zmax ;

	if( (x0<0) OR (x0>SIZE_BRICK_XZ*63) )	return FALSE ;
	if( (x1<0) OR (x1>SIZE_BRICK_XZ*63) )	return FALSE ;
	if( (z0<0) OR (z0>SIZE_BRICK_XZ*63) )	return FALSE ;
	if( (z1<0) OR (z1>SIZE_BRICK_XZ*63) )	return FALSE ;

	// test decors

	if( WorldColBrickFull( x0, y0, z0, ptrobj->Ymax ) )	return FALSE ;
	if( WorldColBrickFull( x1, y0, z0, ptrobj->Ymax ) )	return FALSE ;
	if( WorldColBrickFull( x1, y0, z1, ptrobj->Ymax ) )	return FALSE ;
	if( WorldColBrickFull( x0, y0, z1, ptrobj->Ymax ) )	return FALSE ;

	// test liste des objets

	ptrobjt = ListObjet ;
	for( n=0; n<NbObjets; n++,ptrobjt++ )
	{
		if( (n != numobj)
		AND (ptrobjt->Body != -1)
		AND (!(ptrobj->Flags&INVISIBLE))
		AND (ptrobjt->CarryBy != numobj) )
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
			{	// collision pos refuse
				return FALSE ;
			}
		}
	}
	return TRUE ;
}




/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			 Û฿฿Û  Û฿฿฿Û Û฿฿฿Ü Û  Ü฿
			 ÛÛ฿฿Û ÛÛ  Û ÛÛ  Û ÛÛ฿
			 ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿  ฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	InitBody(	UBYTE gennewbody,
			WORD numobj )
{
	WORD	newbody ;
	WORD	oldbody ;
	T_OBJET *ptrobj ;
	WORD	*ptr ;
	WORD	size ;
	WORD	x0,x1,z0,z1 ;
	UBYTE	*ptr3do ;

	ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Flags & SPRITE_3D )	return ;


	// grosse rustine anti boom okazou
	if( numobj == NUM_PERSO )
	{
		if( Comportement == C_PROTOPACK )
		{
			if( (gennewbody != GEN_BODY_NORMAL)
			AND (gennewbody != GEN_BODY_TUNIQUE) )
			{
				SetComportement( C_NORMAL ) ;
			}
		}
	}

//	if( gennewbody == ptrobj->GenBody ) return ;

	if( gennewbody != NO_BODY )
	{
		newbody = SearchBody( gennewbody, numobj ) ;
	}
	else
	{
		newbody = -1 ;
	}

	if( newbody != -1 )
	{
		if( newbody != ptrobj->Body )
		{
			oldbody = ptrobj->Body ;
			ptrobj->Body = newbody ;
			ptrobj->GenBody = gennewbody ;

			if( M_Xmin == -32000 )
			{
				ptr = (WORD*)(PtrBody[ptrobj->Body] + 2) ;

				x0 =  *ptr++ ;
				x1 =  *ptr++ ;
				ptrobj->Ymin =  *ptr++ ;
				ptrobj->Ymax =  *ptr++ ;
				z0 =  *ptr++ ;
				z1 =  *ptr++ ;

				if( ptrobj->Flags & MINI_ZV )
				{
					// plus petit
					if( x1 - x0  <  z1 - z0 )
						size = (x1 - x0) / 2 ;
					else
						size = (z1 - z0) / 2 ;
				}
				else
				{
					// moyenne
					size = ((x1 - x0) + (z1 - z0)) / 4 ;
				}

				ptrobj->Xmin =  -size ;
				ptrobj->Xmax =  +size ;
				ptrobj->Zmin =  -size ;
				ptrobj->Zmax =  +size ;
			}
			else
			{
				ptrobj->Xmin =  M_Xmin ;
				ptrobj->Xmax =  M_Xmax ;
				ptrobj->Ymin =  M_Ymin ;
				ptrobj->Ymax =  M_Ymax ;
				ptrobj->Zmin =  M_Zmin ;
				ptrobj->Zmax =  M_Zmax ;
			}

			if( (oldbody != -1) AND	(ptrobj->Anim != -1) )
			{
				CopyInterAnim( PtrBody[oldbody], PtrBody[ptrobj->Body] ) ;
			}
		}
	}
	else
	{

		ptrobj->GenBody = NO_BODY ;
		ptrobj->Body = -1 ;

		ptrobj->Ymin =  0 ;
		ptrobj->Ymax =  0 ;
		ptrobj->Xmin =  0 ;
		ptrobj->Xmax =  0 ;
		ptrobj->Zmin =  0 ;
		ptrobj->Zmax =  0 ;
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	InitSprite(	WORD newsprite,
			WORD numobj )
{
	T_OBJET	*ptrobj ;
	WORD	*ptr ;

	ptrobj = &ListObjet[numobj] ;

	if( !(ptrobj->Flags & SPRITE_3D) )	return ;

	if( newsprite != -1 AND newsprite != ptrobj->Body )
	{
		ptrobj->Body = newsprite ;

		ptr = &PtrZvExtra[ newsprite*8 + 2 ] ;

		ptrobj->Xmin =  *ptr++ ;
		ptrobj->Xmax =  *ptr++ ;
		ptrobj->Ymin =  *ptr++ ;
		ptrobj->Ymax =  *ptr++ ;
		ptrobj->Zmin =  *ptr++ ;
		ptrobj->Zmax =  *ptr++ ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			 Û฿฿฿Û ÛÛÜ Û  Û    ÛÜ ÜÛ ÛÛ฿฿฿
			 ÛÛ฿฿Û ÛÛ฿ÛÛ  ÛÛ   ÛÛ฿ Û ฿฿฿฿Û
			 ฿฿  ฿ ฿฿  ฿  ฿฿   ฿฿  ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

WORD	InitAnim(	UBYTE gennewanim,
			WORD flag,
			UBYTE gennextanim,
			WORD numobj )
{
	WORD	newanim, nextanim ;
	T_OBJET *ptrobj ;
	UBYTE	tempstringnext[256] ;

	ptrobj = &ListObjet[numobj] ;

/*
if( numobj == 11 )
{
	CoulText( 15,0 ) ;
	Text( 40,40, "%Fanim %d sur 11", gennewanim ) ;
}
if( numobj == 5 )
{
	CoulText( 15,0 ) ;
	Text( 40,100, "%Fanim %d sur 5", gennewanim ) ;

	if( gennewanim != 0 )
	{
		Text( 70,100, "%Fexplose" ) ;
	}
}

*/
	// debug protect
	if( ptrobj->Body == -1 )
	{
#ifdef	DEBUG_TOOLS
		Message( "Anim sur un objet avec Body = -1 !", TRUE ) ;
#endif
		return FALSE ;
	}
	if( ptrobj->Flags & SPRITE_3D )	return FALSE ; // -1 ;

	if( (gennewanim == ptrobj->GenAnim)
	AND (ptrobj->Anim != -1)
//	AND (newanim == ptrobj->Anim)
//	AND (flag == ptrobj->FlagAnim)
					)	return TRUE ;

	if( gennextanim == NO_ANIM )
	{
		if( ptrobj->FlagAnim != ANIM_ALL_THEN )
		{
			gennextanim = ptrobj->GenAnim ;
		}
	}

	newanim = SearchAnim( gennewanim, numobj ) ;
	if( newanim == -1 )
	{
		newanim = SearchAnim( GEN_ANIM_RIEN, numobj ) ;
	}

	if( flag != ANIM_SET )
	{
		if( ptrobj->FlagAnim == ANIM_ALL_THEN )
		{
			ptrobj->NextGenAnim = gennewanim ;
			return FALSE ;
		}
	}

/*if( numobj==2 )
{
CoulText( 15, 0 ) ;
Text( 10,100, "%FInitAnim GenAnim:%d Anim:%d  ", gennewanim, newanim ) ;
}*/


//	if( (newanim != ptrobj->Anim) OR (flag != ptrobj->FlagAnim) )
	{
		if( flag == ANIM_INSERT )
		{
			flag = ANIM_ALL_THEN ;
			gennextanim = ptrobj->GenAnim ;

			if( (gennextanim == GEN_ANIM_LANCE)
			OR  (gennextanim == GEN_ANIM_TOMBE)
			OR  (gennextanim == GEN_ANIM_RECEPTION)
			OR  (gennextanim == GEN_ANIM_RECEPTION_2) )
				gennextanim = GEN_ANIM_RIEN ;
				// peut etre gennextanim = ptrobj->GenNextAnim ;
		}

		if( flag == ANIM_SET )
		{
			flag = ANIM_ALL_THEN ;
		}

/*		if( flag == ANIM_REPEAT )
		{
			gennextanim = ptrobj->GenAnim ;
		}
*/
		if( ptrobj->Anim == -1 )
		{
			SetAnimObjet(	0,
					HQR_Get( HQR_Anims, newanim ),
					PtrBody[ptrobj->Body] ) ;
		}
		else
		{
			PtrBufferAnim += StockInterAnim( PtrBufferAnim, PtrBody[ptrobj->Body] ) ;
			if( PtrBufferAnim > BufferAnim + 5000 - 512 )	PtrBufferAnim = BufferAnim ;
		}

		ptrobj->Anim = newanim ;
		ptrobj->GenAnim = gennewanim ;
		ptrobj->NextGenAnim = gennextanim ;
		ptrobj->PtrAnimAction = SearchPtrAnimAction ;

		ptrobj->FlagAnim = flag ;
		ptrobj->Frame = 0 ;

		ptrobj->WorkFlags &= ~(OK_HIT + ANIM_END) ;
		ptrobj->WorkFlags |= NEW_FRAME ;

		if( ptrobj->PtrAnimAction != 0 )
		{
/*
if( numobj == 11 )
{
	CoulText( 15,0 ) ;
	Text( 40,50, "%Faction sur 11" ) ;
}
if( numobj == 5 )
{
	CoulText( 15,0 ) ;
	Text( 40,110, "%Faction sur 5" ) ;
}
*/


			GereAnimAction( ptrobj, numobj ) ;
		}
/*
if( numobj == 5 )
{
	CoulText( 15,0 ) ;
	Text( 40,120, "%Fapres sur 5" ) ;
}
*/

		ptrobj->AnimStepBeta = 0 ;
		ptrobj->AnimStepX = 0 ;
		ptrobj->AnimStepY = 0 ;
		ptrobj->AnimStepZ = 0 ;
	}
	return TRUE ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* gestion anim */

void	ReajustPos( UBYTE col )
{
	WORD	xw, yw, zw ;

	if( !col )	return ;

	xw = XMap * SIZE_BRICK_XZ - DEMI_BRICK_XZ ; /* coin superieur gauche de la brick */
	yw = YMap * SIZE_BRICK_Y ;
	zw = ZMap * SIZE_BRICK_XZ - DEMI_BRICK_XZ ;

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
		Nyw = yw + BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ, Nxw - xw ) ;
		break ;

	case 3:
		Nyw = yw + BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ, Nzw - zw ) ;
		break ;

	case 4:
		Nyw = yw + BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ, Nzw - zw ) ;
		break ;

	case 5:
		Nyw = yw + BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ, Nxw - xw ) ;
		break ;
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	ReceptionObj()
{
	if( AnimNumObj == NUM_PERSO )
	{
		if( StartYFalling - Nyw >= 16 * SIZE_BRICK_Y )
		{ // trop haut mort directe
			InitSpecial(	APtObj->PosObjX,
					APtObj->PosObjY+1000,
					APtObj->PosObjZ,
					0 ) ;
			APtObj->LifePoint = 0 ;
			InitAnim( GEN_ANIM_RECEPTION_2, ANIM_ALL_THEN, GEN_ANIM_RIEN, AnimNumObj ) ;
		}
		else
		if( StartYFalling - Nyw >= 8 * SIZE_BRICK_Y )
		{ // se fait mal
			InitSpecial(	APtObj->PosObjX,
					APtObj->PosObjY+1000,
					APtObj->PosObjZ,
					0 ) ;
			APtObj->LifePoint -- ;
			InitAnim( GEN_ANIM_RECEPTION_2, ANIM_ALL_THEN, GEN_ANIM_RIEN, AnimNumObj ) ;
		}
		else
		if( StartYFalling - Nyw > 1 )
		{ // reception normale
			InitAnim( GEN_ANIM_RECEPTION, ANIM_ALL_THEN, GEN_ANIM_RIEN, AnimNumObj ) ;
		}
		else
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, 0, AnimNumObj ) ;
		}

		StartYFalling = 0 ;
	}
	else
	{
		InitAnim( GEN_ANIM_RECEPTION, ANIM_ALL_THEN, APtObj->NextGenAnim, AnimNumObj ) ;
	}
//	APtObj->Falling = FALSE ;
	APtObj->WorkFlags &= ~FALLING ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void    DoCornerReajust( WORD nx, WORD ny, WORD nz, WORD coin )
{
	UBYTE	col, orgcol  ;

	orgcol = WorldColBrick( Nxw, Nyw, Nzw ) ;

	Nxw += nx ;
	Nyw += ny ;
	Nzw += nz ;

	if( Nxw < 0 )	goto fincorner ;
	if( Nzw < 0 )	goto fincorner  ;
	if( Nxw > 63*SIZE_BRICK_XZ )	goto fincorner  ;
	if( Nzw > 63*SIZE_BRICK_XZ )	goto fincorner  ;

	ReajustPos( orgcol ) ;

	if( (col = WorldColBrick( Nxw, Nyw, Nzw )) != 0 )
	{
		if( col == 1 )
		{
			Col1 |= coin ;

			if( WorldColBrick( Nxw, Nyw, OldZ+nz ) == 1 )
			{
				if( WorldColBrick( OldX+nx, Nyw, Nzw ) != 1 )
				{
					SaveNxw = OldX ;
				}
			}
			else
			{
				SaveNzw = OldZ ;
			}
		}
	}

fincorner:

	Nxw = SaveNxw ;
	Nyw = SaveNyw ;
	Nzw = SaveNzw ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void    DoCornerReajustTwinkel( WORD nx, WORD ny, WORD nz, WORD coin )
{
	UBYTE	col, orgcol  ;

	orgcol = WorldColBrick( Nxw, Nyw, Nzw ) ;

	Nxw += nx ;
	Nyw += ny ;
	Nzw += nz ;

	if( Nxw < 0 )	goto fincorner ;
	if( Nzw < 0 )	goto fincorner  ;
	if( Nxw > 63*SIZE_BRICK_XZ )	goto fincorner  ;
	if( Nzw > 63*SIZE_BRICK_XZ )	goto fincorner  ;

	ReajustPos( orgcol ) ;

	if( (col = WorldColBrickFull( Nxw, Nyw, Nzw, APtObj->Ymax )) != 0 )
	{
		if( col == 1 )
		{
			Col1 |= coin ;

			if( WorldColBrickFull( Nxw, Nyw, OldZ+nz, APtObj->Ymax ) == 1 )
			{
				if( WorldColBrickFull( OldX+nx, Nyw, Nzw, APtObj->Ymax ) != 1 )
				{
					SaveNxw = OldX ;
				}
			}
			else
			{
				SaveNzw = OldZ ;
			}
		}
	}

fincorner:

	Nxw = SaveNxw ;
	Nyw = SaveNyw ;
	Nzw = SaveNzw ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	DoAnim( WORD numobj  )
{
	LONG	flag ;
	T_OBJET	*ptrobj ;
	WORD	numanim ;
	UBYTE	*ptranim ;
	UBYTE	*ptrbody ;
	WORD	xw, yw, zw ;
	UBYTE	col, orgcol ;
	LONG	angle ;
	WORD	nb, n, nu ;

	APtObj = ptrobj = &ListObjet[(AnimNumObj = numobj)] ;

	if( APtObj->Body == -1 )	return ;

	OldX = APtObj->OldPosX ;
	OldY = APtObj->OldPosY ;
	OldZ = APtObj->OldPosZ ;


/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/* gestion DEP OBJET SPRITE */

	if( APtObj->Flags & SPRITE_3D )
	{
		if( APtObj->HitForce != 0 )
			APtObj->WorkFlags |= OK_HIT ;

		/* gestion anim pour sprite */

		Nxw = APtObj->PosObjX ;
		Nyw = APtObj->PosObjY ;
		Nzw = APtObj->PosObjZ ;

		if( !(APtObj->WorkFlags & FALLING) )
		{
			if( APtObj->SRot != 0 )	/* vitesse deplacement */
			{
				n = GetRealValue( &APtObj->RealAngle ) ;
				if( !n ) /* tanpis machine trop speed */
				{
					if( APtObj->RealAngle.EndValue > 0 )
						n = 1 ;
					else
						n = -1 ;
				}

				Rotate( n,0, APtObj->FlagAnim ) ; /* alpha */
				Nyw = APtObj->PosObjY - Y0 ;
				Rotate( 0, X0, APtObj->Beta ) ;
				Nxw = APtObj->PosObjX + X0 ;
				Nzw = APtObj->PosObjZ + Y0 ;

				InitRealValue( 0, APtObj->SRot,
						50, &APtObj->RealAngle ) ;

				if( APtObj->WorkFlags & AUTO_STOP_DOOR )
				{
				// c'est obligatoirement un sprite_clip
				if( APtObj->DoorWidth )
				{
				// distance ouverture max
					if( Distance2D( Nxw,
							Nzw,
							APtObj->AnimStepX,
							APtObj->AnimStepZ )
						    >= APtObj->DoorWidth )
					{
						// recalage violent
						switch( APtObj->Beta )
						{
						case 768: // left
							Nxw = APtObj->AnimStepX - APtObj->DoorWidth ;
							break ;

						case 256: // right
							Nxw = APtObj->AnimStepX + APtObj->DoorWidth ;
							break ;

						case 512: // up
							Nzw = APtObj->AnimStepZ - APtObj->DoorWidth ;
							break ;

						case 0: // down
							Nzw = APtObj->AnimStepZ + APtObj->DoorWidth ;
							break ;
						}
						APtObj->WorkFlags &= ~AUTO_STOP_DOOR ;
						APtObj->SRot = 0 ;
					}
				}
				else
				{ // je me ferme
					flag = FALSE ;

					switch( APtObj->Beta )
					{
					case 768: // left
						if( Nxw >= APtObj->AnimStepX ) flag = TRUE ;
						break ;

					case 256: // right
						if( Nxw <= APtObj->AnimStepX ) flag = TRUE ;
						break ;

					case 512: // up
						if( Nzw >= APtObj->AnimStepZ ) flag = TRUE ;
						break ;

					case 0: // down
						if( Nzw <= APtObj->AnimStepZ ) flag = TRUE ;
						break ;
					}

/*					if( Distance2D( Nxw,
							Nzw,
							APtObj->AnimStepX,
							APtObj->AnimStepZ )
						    < 100 )
					// attention si porte decale
*/					if( flag )
					{
						// position d'origine

						Nxw = APtObj->AnimStepX ;
						Nyw = APtObj->AnimStepY ;
						Nzw = APtObj->AnimStepZ ;

						APtObj->WorkFlags &= ~AUTO_STOP_DOOR ;
						APtObj->SRot = 0 ;
					}
				}
				} // auto stop door
			}

			if( APtObj->Flags & PUSHABLE )
			{
				Nxw += APtObj->AnimStepX ;
				Nyw += APtObj->AnimStepY ;
				Nzw += APtObj->AnimStepZ ;

				if( APtObj->Flags & MINI_ZV ) // boxxle
				{
					Nxw = (Nxw / (SIZE_BRICK_XZ/4)) * (SIZE_BRICK_XZ/4) ;
					Nzw = (Nzw / (SIZE_BRICK_XZ/4)) * (SIZE_BRICK_XZ/4) ;
				}

				APtObj->AnimStepX = 0 ;
				APtObj->AnimStepY = 0 ;
				APtObj->AnimStepZ = 0 ;
			}
		}

/*		if( Nxw == OldX
		AND Nyw == OldY
		AND Nzw == OldZ )	return ; // did not move
*/
	}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/* gestion anim/DEP OBJET 3D */

	else	/* pas SPRITE_3D donc obj articuls */

	if( APtObj->Anim != -1 )
	{
		numanim = APtObj->Anim ;

		ptranim = HQR_Get( HQR_Anims, numanim ) ;
		ptrbody = PtrBody[APtObj->Body] ;

		flag = SetInterDepObjet( APtObj->Frame, ptranim, ptrbody ) ;

		if( AnimMasterRot )
			APtObj->WorkFlags |= ANIM_MASTER_ROT ;
		else
			APtObj->WorkFlags &= ~ANIM_MASTER_ROT ;
		APtObj->Beta = (APtObj->Beta + AnimStepBeta - APtObj->AnimStepBeta) & 1023 ;

		APtObj->AnimStepBeta = AnimStepBeta ;

		Rotate(	AnimStepX, AnimStepZ, APtObj->Beta ) ;
		AnimStepX = X0 ;
		AnimStepZ = Y0 ;

		Nxw = APtObj->PosObjX + AnimStepX - APtObj->AnimStepX ;
		Nyw = APtObj->PosObjY + AnimStepY - APtObj->AnimStepY ;
		Nzw = APtObj->PosObjZ + AnimStepZ - APtObj->AnimStepZ ;

		APtObj->AnimStepX = AnimStepX ;
		APtObj->AnimStepY = AnimStepY ;
		APtObj->AnimStepZ = AnimStepZ ;

		APtObj->WorkFlags &= ~(ANIM_END+NEW_FRAME) ;
		if( flag )
		{
/*-----------------------------------------------------------------*/
/* si nouvelle frame */
			APtObj->Frame++ ;
			APtObj->WorkFlags |= NEW_FRAME ;

/*-----------------------------------------------------------------*/
/* gestion des actions dclenches par des anims/frames */

			if( APtObj->PtrAnimAction != 0 )
			{
				GereAnimAction( APtObj, numobj ) ;
			}

/*-----------------------------------------------------------------*/
/* frame suivante test fin anim */
			if( APtObj->Frame == GetNbFramesAnim( ptranim ) )
			{
				APtObj->WorkFlags &= ~OK_HIT ;

				if( APtObj->FlagAnim == ANIM_REPEAT )
				{
					APtObj->Frame = GetBouclageAnim( ptranim ) ;
				}
				else	/* ANIM[_ALL]_THEN */
				{
					APtObj->GenAnim = APtObj->NextGenAnim ;
					APtObj->Anim = SearchAnim( APtObj->GenAnim, numobj ) ;
					if( APtObj->Anim == -1 )
					{
						APtObj->Anim = SearchAnim( GEN_ANIM_RIEN, numobj ) ;
						APtObj->GenAnim = GEN_ANIM_RIEN ;
					}
					APtObj->PtrAnimAction = SearchPtrAnimAction ;

					APtObj->FlagAnim = ANIM_REPEAT ;
					APtObj->Frame = 0 ;
					APtObj->HitForce = 0 ;
				}

				if( APtObj->PtrAnimAction != 0 )
				{
					GereAnimAction( APtObj, numobj ) ;
				}
				APtObj->WorkFlags |= ANIM_END ;
			}
			APtObj->AnimStepBeta = 0 ;
			APtObj->AnimStepX = 0 ;
			APtObj->AnimStepY = 0 ;
			APtObj->AnimStepZ = 0 ;

/*-----------------------------------------------------------------*/

		}/* new frame */
	}/* if anim != -1 */

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
// pour tous obj/sprite 3d

	// je suis porte par un CARRIER / par quelqu'un ?
	if( (nu = APtObj->CarryBy) != -1 )
	{
		Nxw -= ListObjet[nu].OldPosX ;
		Nyw -= ListObjet[nu].OldPosY ;
		Nzw -= ListObjet[nu].OldPosZ ;

		Nxw += ListObjet[nu].PosObjX ;
		Nyw += ListObjet[nu].PosObjY ;
		Nzw += ListObjet[nu].PosObjZ ;

		if( !CheckZvOnZv( numobj, nu ) )
		{
			APtObj->CarryBy = -1 ;
		}
	}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
// je tombe

	if( APtObj->WorkFlags & FALLING )
	{
		/* gestion chute avec proportion sur y */
		Nxw = OldX ;
		Nyw = OldY + StepFalling ;
		Nzw = OldZ ;
	}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/* test/reajuste pos */

// ?	APtObj->ObjCol = -1 ;

if( APtObj->Flags & CHECK_BRICK_COL )
{
	YMap = 0 ;

/* reajuste le nyw (nouvelle pos) par rapport … la col en cours	*/

	if( (col = WorldColBrick( OldX, OldY, OldZ )) != 0 )
	{
		if( col == 1 )
		{
			Message( "currentpos dans col 1 ?", FALSE ) ;

			Nyw = (Nyw/SIZE_BRICK_Y)*SIZE_BRICK_Y+SIZE_BRICK_Y ;

			APtObj->PosObjY = Nyw ; // essai rejette perso
		}
		else
		{
			ReajustPos( col ) ;
		}
	}

	if( APtObj->Flags & CHECK_OBJ_COL )
	{
		CheckObjCol( numobj ) ;
	}

	if( APtObj->CarryBy != -1 )
	{
		if( APtObj->WorkFlags & FALLING )
		{
			ReceptionObj() ;
		}
	}

// þþþþþþþþþ 4 coins test la col de la nouvelle pos + size zv

//	if( !(APtObj->Flags & SPRITE_3D) )
	{

	SaveNxw = Nxw ;
	SaveNyw = Nyw ;
	SaveNzw = Nzw ;

	Col1 = 0 ;

	if( (numobj == NUM_PERSO) AND ((ptrobj->Flags&COL_BASSE) == 0) )
	{
		DoCornerReajustTwinkel( APtObj->Xmin, APtObj->Ymin, APtObj->Zmin, 1 ) ;
		DoCornerReajustTwinkel( APtObj->Xmax, APtObj->Ymin, APtObj->Zmin, 2 ) ;
		DoCornerReajustTwinkel( APtObj->Xmax, APtObj->Ymin, APtObj->Zmax, 4 ) ;
		DoCornerReajustTwinkel( APtObj->Xmin, APtObj->Ymin, APtObj->Zmax, 8 ) ;
	}
	else
	{
		DoCornerReajust( APtObj->Xmin, APtObj->Ymin, APtObj->Zmin, 1 ) ;
		DoCornerReajust( APtObj->Xmax, APtObj->Ymin, APtObj->Zmin, 2 ) ;
		DoCornerReajust( APtObj->Xmax, APtObj->Ymin, APtObj->Zmax, 4 ) ;
		DoCornerReajust( APtObj->Xmin, APtObj->Ymin, APtObj->Zmax, 8 ) ;
	}

	// choc si on court

	if( (Col1 != 0)
	AND !(APtObj->WorkFlags & FALLING)
	AND (AnimNumObj == NUM_PERSO)
	AND (Comportement == C_SPORTIF)
	AND (APtObj->GenAnim == GEN_ANIM_MARCHE) )
	{
		Rotate( APtObj->Xmin, APtObj->Zmin, APtObj->Beta + 896 + 512 ) ;
		X0 += Nxw ;
		Y0 += Nzw ;

		if( (X0 >= 0) AND (Y0 >= 0) AND (X0 <= 63*SIZE_BRICK_XZ) AND (Y0 <= 63*SIZE_BRICK_XZ)
		AND (WorldColBrick( X0, Nyw+256, Y0 ) != 0) )
		{
			InitSpecial(	APtObj->PosObjX,
					APtObj->PosObjY+1000,
					APtObj->PosObjZ,
					0 ) ;
			InitAnim( GEN_ANIM_CHOC, ANIM_ALL_THEN, GEN_ANIM_RIEN, AnimNumObj ) ;
			if( AnimNumObj == NUM_PERSO )		LastJoyFlag = TRUE ;
			APtObj->LifePoint -= 1 ;
		}
	}
	}

// þþþþþþþþþ	test la col de la nouvelle pos reelle

/*	if( (AnimNumObj == NUM_PERSO) AND ((ptrobj->Flags&COL_BASSE) == 0) )
	{
		col = WorldColBrickFull( Nxw, Nyw, Nzw, APtObj->Ymax ) ;
	}
	else
	{
*/
		col = WorldColBrick( Nxw, Nyw, Nzw ) ;

//	}

	APtObj->Col = col ;

//	if( (APtObj->Col = col = WorldColBrick( Nxw, Nyw, Nzw )) != 0 )
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
				if( numobj == NUM_PERSO )
				{
					if( (Comportement == C_SPORTIF)
					AND (APtObj->GenAnim == GEN_ANIM_MARCHE) )
					{
						InitSpecial(	APtObj->PosObjX,
								APtObj->PosObjY+1000,
								APtObj->PosObjZ,
								0 ) ;
						InitAnim( GEN_ANIM_CHOC, ANIM_ALL_THEN, GEN_ANIM_RIEN, AnimNumObj ) ;
						if( numobj == NUM_PERSO )	LastJoyFlag = TRUE ;
						APtObj->LifePoint -= 1 ;
					}

				}

/*				if( (AnimNumObj == NUM_PERSO) AND ((ptrobj->Flags&COL_BASSE) == 0) )
				{
					// fait glisser twinsen
//þþþþþþþþþ				// regarde de dep X puis Z par rapport … old pos
					if( WorldColBrickFull( Nxw, Nyw, OldZ, APtObj->Ymax ) != 0 )
					{
						if( WorldColBrickFull( OldX, Nyw, Nzw, APtObj->Ymax ) != 0 )
						{
							return ;	// pos pas acceptee
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
				else
				{
*/
					// fait glisser
//þþþþþþþþþ				// regarde de dep X puis Z par rapport … old pos

					if( WorldColBrick( Nxw, Nyw, OldZ ) != 0 )
					{
						if( WorldColBrick( OldX, Nyw, Nzw ) != 0 )
						{
							return ;	/* pos pas acceptee */
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
//				}
			}
		}
		else
		{
//			if( APtObj->Falling )
			if( APtObj->WorkFlags & FALLING )
			{
				ReceptionObj() ;
			}
			ReajustPos( col ) ;
		}

//		APtObj->Falling = FALSE ;
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
				if( !(APtObj->WorkFlags & ANIM_MASTER_ROT) )
				{
					APtObj->WorkFlags |= FALLING ;
					if( numobj == NUM_PERSO AND !StartYFalling )
						StartYFalling = Nyw ;

					InitAnim( GEN_ANIM_TOMBE, ANIM_REPEAT, NO_ANIM , numobj ) ;
				}
			}
		}
	}

	if( YMap == -1 )	APtObj->LifePoint = 0 ;	// bye bye
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

	if( Nxw < 0 ) Nxw = 0 ;
	if( Nzw < 0 ) Nzw = 0 ;
	if( Nyw < 0 ) Nyw = 0 ;
	if( Nxw > 63*SIZE_BRICK_XZ )	Nxw = 63*SIZE_BRICK_XZ ;
	if( Nzw > 63*SIZE_BRICK_XZ )	Nzw = 63*SIZE_BRICK_XZ ;

/* ok enregistre position */

	APtObj->PosObjX = Nxw ;
	APtObj->PosObjY = Nyw ;
	APtObj->PosObjZ = Nzw ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			    ÛÜ ÜÛ Û฿฿฿Û Û   Û Û฿฿฿฿
			    ÛÛ฿ Û ÛÛ  Û ÛÛ Ü฿ ÛÛ฿฿
			    ฿฿  ฿ ฿฿฿฿฿ ฿฿฿   ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ManualRealAngle( T_OBJET *ptrobj )
{
	WORD	angle = 0 ;

	if( MyJoy & J_LEFT )	angle = +256 ;
	if( MyJoy & J_RIGHT )	angle = -256 ;

	InitRealAngleConst(	ptrobj->Beta,
				ptrobj->Beta + angle,
				ptrobj->SRot,
				&ptrobj->RealAngle ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	ClearRealAngle( T_OBJET *ptrobj )
{
	InitRealAngle(	ptrobj->Beta,
			ptrobj->Beta,
			0,
			&ptrobj->RealAngle ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* T_OBJET.	Info		Info1		Info2		Info3

FOLLOW								num obj
FOLLOW_2	etat		label1/label2	distance	num obj


		NumTrack	OffsetTrack

TRACK
TRACK_ATTACK

*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


void	DoDir( WORD numobj  )
{
	LONG	f ;
	T_OBJET	*ptrobj ;
	UBYTE	*ptr ;
	WORD	info,info1,info2,info3 ;
	WORD	flag, angle ;
	UBYTE	macro ;
	UBYTE	string[256] ;

	ptrobj = &ListObjet[numobj] ;

	if( ptrobj->Body == -1 )	return ;

/*-------------------------------------------------------------------------*/
	if( ptrobj->WorkFlags & FALLING )
	{
		if( ptrobj->Move == MOVE_MANUAL )
		{
			ManualRealAngle( ptrobj ) ;
			LastMyJoy = MyJoy ;
		}
		return ;
	}

//	if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
	{
		if( !(ptrobj->Flags & SPRITE_3D)
		AND ptrobj->Move != MOVE_MANUAL )
		{
			ptrobj->Beta = GetRealAngle( &ptrobj->RealAngle ) ;
		}
	}
/*	else
	{
		ClearRealAngle( ptrobj ) ;
	}
*/
	switch( ptrobj->Move )
	{
/*-------------------------------------------------------------------------*/
	case MOVE_MANUAL:

	if( numobj == NUM_PERSO )	/* protection anim speciales */
	{
		ActionNormal = FALSE ;
/*
		if( MyKey == K_F1 )	SetComportement( C_NORMAL ) ;
		if( MyKey == K_F2 )	SetComportement( C_SPORTIF ) ;
		if( MyKey == K_F3 )	SetComportement( C_AGRESSIF ) ;
		if( MyKey == K_F4 )	SetComportement( C_DISCRET ) ;
*/
		switch( Comportement )
		{
		case C_NORMAL:
			if( MyFire & F_SPACE )
			{
				ActionNormal = TRUE ;
			}
			break ;

		case C_SPORTIF:
			if( MyFire & F_SPACE )
			{
				InitAnim( GEN_ANIM_SAUTE, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
			}
			break ;

		case C_AGRESSIF:
			if( MyFire & F_SPACE )
			{
				if( CombatAuto )
				{
					LastJoyFlag = TRUE ;
					/* essai control direction pendant combat */
					ptrobj->Beta = GetRealAngle( &ptrobj->RealAngle ) ;

					if(  (LastFire & F_SPACE)
					AND( ptrobj->GenAnim != GEN_ANIM_RIEN ) )
						break ;

					switch( Rnd(3) )
					{
					case 0:
						InitAnim( GEN_ANIM_COUP_1, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
						break ;
					case 1:
						InitAnim( GEN_ANIM_COUP_2, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
						break ;
					case 2:
						InitAnim( GEN_ANIM_COUP_3, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
						break ;
					}
				}
				else
				{
					if( MyJoy & J_RIGHT )
						InitAnim( GEN_ANIM_COUP_2, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
					if( MyJoy & J_LEFT )
						InitAnim( GEN_ANIM_COUP_3, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
					if( MyJoy & J_UP )
						InitAnim( GEN_ANIM_COUP_1, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
				}
			}
			break ;

		case C_DISCRET:
			if( MyFire & F_SPACE )
			{
				InitAnim( GEN_ANIM_CACHE, ANIM_REPEAT, NO_ANIM, numobj ) ;
			}
			break ;
		}

		/* lance balle magique */

		if( (MyFire & F_ALT)
		AND (ListFlagGame[FLAG_CONSIGNE] == 0) )
		{
		if( Weapon == 0 )	// balle magique
		{
			if( ListFlagGame[FLAG_BALLE_MAGIQUE] == 1 )
			{
				if( MagicBall == -1 )
				{
					InitAnim( GEN_ANIM_LANCE, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;
				}
				LastJoyFlag = TRUE ;
				/* control direction pendant vise */
				ptrobj->Beta = GetRealAngle( &ptrobj->RealAngle ) ;
			}
		}
		else	// sabre magique
		{
			if( ListFlagGame[FLAG_SABRE_MAGIQUE] == 1 )
			{
				if( ptrobj->GenBody != GEN_BODY_SABRE )
				{
					InitBody( GEN_BODY_SABRE, NUM_PERSO ) ;
				}
				// anim frappe rapide

				InitAnim( GEN_ANIM_SABRE, ANIM_THEN, GEN_ANIM_RIEN, numobj ) ;

				LastJoyFlag = TRUE ;
				/* control direction pendant vise */
				ptrobj->Beta = GetRealAngle( &ptrobj->RealAngle ) ;
			}
		}
		}

	} /* endif numperso */

	if( !MyFire OR ActionNormal )
	{
		if( MyJoy & J_UP+J_DOWN )	LastJoyFlag = FALSE ;

		if( ((MyJoy != LastMyJoy) OR (MyFire != LastMyFire) )
		AND LastJoyFlag )
		{
			InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NO_ANIM, numobj ) ;
		}

		LastJoyFlag = FALSE ;

		if( MyJoy & J_UP )
		{
			if( !FlagClimbing )
			{
				InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NO_ANIM, numobj ) ;
			}
			LastJoyFlag = TRUE ;
		}

		if( MyJoy & J_DOWN )
		{
			InitAnim( GEN_ANIM_RECULE, ANIM_REPEAT, NO_ANIM, numobj ) ;
			LastJoyFlag = TRUE ;
		}

		if( MyJoy & J_LEFT )
		{
			LastJoyFlag = TRUE ;
//			if( ptrobj->Anim == AnimRien )
			if( ptrobj->GenAnim == GEN_ANIM_RIEN )
			{
				InitAnim( GEN_ANIM_GAUCHE, ANIM_REPEAT, NO_ANIM, numobj ) ;
			}
			else
			{
				if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
				{
					ptrobj->Beta = GetRealAngle( &ptrobj->RealAngle ) ;
				}
			}
		}

		if( MyJoy & J_RIGHT )
		{
			LastJoyFlag = TRUE ;
//			if( ptrobj->Anim == AnimRien )
			if( ptrobj->GenAnim == GEN_ANIM_RIEN )
			{
				InitAnim( GEN_ANIM_DROITE, ANIM_REPEAT, NO_ANIM, numobj ) ;
			}
			else
			{
				if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
				{
					ptrobj->Beta = GetRealAngle( &ptrobj->RealAngle ) ;
				}
			}
		}
	}

	ManualRealAngle( ptrobj ) ;
	LastMyJoy = MyJoy ;
	LastMyFire = MyFire ;

	break ;

/*-------------------------------------------------------------------------*/
	case MOVE_FOLLOW:
//		if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
		{
			info3 = ptrobj->Info3 ;	/* num obj to follow */

			angle = GetAngle(	ptrobj->PosObjX,
						ptrobj->PosObjZ,
						ListObjet[info3].PosObjX,
						ListObjet[info3].PosObjZ ) ;

			if( ptrobj->Flags & SPRITE_3D )
			{
				ptrobj->Beta = angle ;
			}
			else
			{
				InitRealAngleConst(	ptrobj->Beta,
							angle,
							ptrobj->SRot,
							&ptrobj->RealAngle ) ;
			}
		}
		break ;

/*-------------------------------------------------------------------------*/
	case MOVE_RANDOM:
		if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
		{
			if( ptrobj->Col & 128 )
			{
				angle = (ptrobj->Beta + (rand()&511) - 256 + 512) & 1023 ;
				InitRealAngleConst(	ptrobj->Beta,
							angle,
							ptrobj->SRot,
							&ptrobj->RealAngle ) ;

				*(ULONG*)(&ptrobj->Info) = TimerRef + Rnd( 300 ) + 300 ;
				InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NO_ANIM, numobj ) ;
			}
			if( !ptrobj->RealAngle.TimeValue )
			{
				InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NO_ANIM, numobj ) ;
				if( TimerRef > *(ULONG*)(&ptrobj->Info) )
				{
					angle = (ptrobj->Beta + (rand()&511) - 256) & 1023 ;
					InitRealAngleConst(	ptrobj->Beta,
								angle,
								ptrobj->SRot,
								&ptrobj->RealAngle ) ;

					*(ULONG*)(&ptrobj->Info) = TimerRef + Rnd( 300 ) + 300 ;
				}
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
		ptrobj->PosObjX = ListObjet[info3].PosObjX ;
		ptrobj->PosObjZ = ListObjet[info3].PosObjZ ;
		break ;
	}
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		   Û฿฿฿Ü  Û    ÛÛ฿฿฿ Û฿฿฿Û Û     Û฿฿฿Û Û  Ü฿
		   ÛÛ  Û  ÛÛ   ฿฿฿฿Û ÛÛ฿฿฿ ÛÛ    ÛÛ฿฿Û ÛÛ฿
		   ฿฿฿฿   ฿฿   ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿  ฿ ฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

#ifdef	DEBUG_TOOLS

LONG	DrawCube( 	WORD xw0, WORD yw0, WORD zw0,
			WORD xw1, WORD yw1, WORD zw1	)
{
	LONG	x0,y0,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7 ;
	WORD	coul = 11 ;
	WORD	coul1 = 13 ;

	ClearScreenMinMax() ;

	ProjettePoint( xw0,yw0,zw0 ) ;
	AdjustScreenMax() ;
	x0 = Xp ;
	y0 = Yp ;
	ProjettePoint( xw1,yw0,zw0 ) ;
	AdjustScreenMax() ;
	x1 = Xp ;
	y1 = Yp ;
	ProjettePoint( xw1,yw0,zw1 ) ;
	AdjustScreenMax() ;
	x2 = Xp ;
	y2 = Yp ;
	ProjettePoint( xw0,yw0,zw1 ) ;
	AdjustScreenMax() ;
	x3 = Xp ;
	y3 = Yp ;
	ProjettePoint( xw0,yw1,zw0 ) ;
	AdjustScreenMax() ;
	x4 = Xp ;
	y4 = Yp ;
	ProjettePoint( xw1,yw1,zw0 ) ;
	AdjustScreenMax() ;
	x5 = Xp ;
	y5 = Yp ;
	ProjettePoint( xw1,yw1,zw1 ) ;
	AdjustScreenMax() ;
	x6 = Xp ;
	y6 = Yp ;
	ProjettePoint( xw0,yw1,zw1 ) ;
	AdjustScreenMax() ;
	x7 = Xp ;
	y7 = Yp ;

	SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
	if( ClipXmin <= ClipXmax
	AND ClipYmin <= ClipYmax )
	{
		Line( x0,y0, x1,y1, coul ) ;
		Line( x1,y1, x2,y2, coul ) ;
		Line( x2,y2, x3,y3, coul ) ;
		Line( x3,y3, x0,y0, coul ) ;

		Line( x0,y0, x4,y4, coul1 ) ;
		Line( x1,y1, x5,y5, coul1 ) ;
		Line( x2,y2, x6,y6, coul1 ) ;
		Line( x3,y3, x7,y7, coul1 ) ;
		Line( x4,y4, x5,y5, coul1 ) ;
		Line( x5,y5, x6,y6, coul1 ) ;
		Line( x6,y6, x7,y7, coul1 ) ;
		Line( x7,y7, x4,y4, coul1 ) ;
		return 1 ;
	}
	return 0 ;
}


void	DrawZV( T_OBJET *ptr )
{
	if( DrawCube(	ptr->Xmin - WorldXCube,
			ptr->Ymin - WorldYCube,
			ptr->Zmin - WorldZCube,
			ptr->Xmax - WorldXCube,
			ptr->Ymax - WorldYCube,
			ptr->Zmax - WorldZCube  ) )
	{
		AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
	}
	UnSetClip() ;
}

#endif



/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AffScene( LONG flagflip )
{
	LONG	n, c, cmpt ;
	LONG	xmin, ymin, xmax, ymax, dx, dy ;
	T_OBJET		*ptrobj ;
	T_EXTRA		*ptrextra ;
	T_TRACK		*ptrtrack ;
	T_ZONE		*ptrz ;
	T_INCRUST_DISP	*ptrdisp ;
	T_SORT		*ptrtri ;
	WORD	*ptr ;
	LONG	nbobjets ;
	LONG	err ;
	WORD	numobj, typeobj ;
	WORD	x,y,z, x0,y0, xw, yw, zw, xm, ym, zm ;
	WORD	num ;
	WORD	ztri ;
	WORD	oldxporg ;
	WORD	oldyporg ;
	UBYTE	string[40] ;
	UBYTE	*ptranim, *ptrbody ;

	WORD	txmin, tymin, tzmin, txmax, tymax, tzmax ;
	WORD	pxmin, pymin, pzmin, pxmax, pymax, pzmax ;
	WORD	twinsenpos, twinsenz ;
	LONG	shadowtwinsen, i ;

/*------------------------------------------------------------------------*/

//	xw = StartXCube*SIZE_BRICK_XZ ;
//	yw = StartYCube*SIZE_BRICK_Y ;
//	zw = StartZCube*SIZE_BRICK_XZ ;

	oldxporg = XpOrgw ;
	oldyporg = YpOrgw ;

/*------------------------------------------------------------------------*/
/* cls */

	UnSetClip() ;

	if( !flagflip )
	{
		ClsBoxes() ;			/* nettoie Log */
	}
	else
	{
		SaveTimer() ;
		Cls() ;
		AffGrille() ;
		ChangeIncrustPos( oldxporg,oldyporg, XpOrgw, YpOrgw ) ;
		HQ_ChangeBalanceSamples( oldxporg, oldyporg ) ;
		CopyScreen( Log, Screen ) ;
	}

/*------------------------------------------------------------------------*/
/* objets scenarique 3D ou sprite */

	nbobjets = 0 ;
	ptrobj = ListObjet ;

	for( n=0; n<NbObjets; n++, ptrobj++ )
	{
		ptrobj->WorkFlags &= ~WAS_DRAWN ;

		// si zone grm en cours n'affiche pas les objets au dessus !
		if( ZoneGrm != -1 )
		{
			ptrz = &ListZone[IndexGrm] ;
			if( ptrobj->PosObjY > ListZone[IndexGrm].Y1 )
			{
				continue ;
			}
		}

		if( (ptrobj->Flags & OBJ_BACKGROUND)
		AND (flagflip == FALSE) )
		{
			// rustine: renseigne qd meme moteur presence sprite
			// pour obj_back si dans l'cran
			ProjettePoint(	ptrobj->PosObjX-WorldXCube,
					ptrobj->PosObjY-WorldYCube,
					ptrobj->PosObjZ-WorldZCube ) ;
			if( (Xp>VIEW_X0) AND (Xp<VIEW_X1) AND (Yp>VIEW_Y0) AND (Yp<VIEW_Y1) )
			{
				ptrobj->WorkFlags |= WAS_DRAWN ;
			}
			continue ;
		}

		if( (ptrobj->Body != -1)
		AND (!(ptrobj->Flags&INVISIBLE)) )
		{
			/* preclip */
			ProjettePoint(	ptrobj->PosObjX-WorldXCube,
					ptrobj->PosObjY-WorldYCube,
					ptrobj->PosObjZ-WorldZCube ) ;

			// agrandie la zone de visualisation que pour les portes (sprite_clip)

			if( ( (ptrobj->Flags&SPRITE_CLIP)
				AND (Xp>-112) AND (Xp<(640+112)) AND (Yp>-50) AND (Yp<(480+171)))

			OR  ( (!(ptrobj->Flags&SPRITE_CLIP))
				AND (Xp>VIEW_X0) AND (Xp<VIEW_X1) AND (Yp>VIEW_Y0) AND (Yp<VIEW_Y1)) )

//			if( (Xp>VIEW_X0) AND (Xp<VIEW_X1) AND (Yp>VIEW_Y0) AND (Yp<VIEW_Y1) )
			{
				ztri = ptrobj->PosObjX - WorldXCube
				       + ptrobj->PosObjZ - WorldZCube ;

				/* magouille pour etre sur qu'un carrier est
				dessous l'eventuel objet port */

				// arg ! moche si 2 obj sur carrier
				if( (num=ptrobj->CarryBy) != -1 )
				{
					ztri = ListObjet[num].PosObjX - WorldXCube
					       + ListObjet[num].PosObjZ - WorldZCube
					       + 2 ;
				}

				if( ptrobj->Flags & SPRITE_3D )
				{
					ListTri[nbobjets].NumObj = n + TYPE_OBJ_SPRITE ;

					if( ptrobj->Flags & SPRITE_CLIP )
					{
						ztri = ptrobj->AnimStepX - WorldXCube
						       + ptrobj->AnimStepZ - WorldZCube ;
					}
				}
				else
				{
					ListTri[nbobjets].NumObj = n + TYPE_OBJ_3D ;

				}
				ListTri[nbobjets].Z = ztri ;
				nbobjets++ ;

				if( Shadow )
				{
					if( !(ptrobj->Flags & NO_SHADOW) )
					{
						if( (c=ptrobj->CarryBy) != -1 )
						{
							ShadowX = ptrobj->PosObjX ;
							ShadowY = ptrobj->PosObjY-1 ;
							ShadowZ = ptrobj->PosObjZ ;
						}
						else
						{
							GetShadow( ptrobj->PosObjX,ptrobj->PosObjY,ptrobj->PosObjZ ) ;
						}

						ListTri[nbobjets].Z = ztri - 1 ;

						/* ptrobj->PosObjX-xw + ptrobj->PosObjZ-zw - 1 ; */
						ListTri[nbobjets].NumObj = TYPE_SHADOW + n ;	/* shadow */
						ListTri[nbobjets].Num = 2 ;
						ListTri[nbobjets].Xw = ShadowX ;
						ListTri[nbobjets].Yw = ShadowY ;
						ListTri[nbobjets].Zw = ShadowZ ;
						nbobjets++ ;
					}
				}

				if( FlagMCGA )
				{
					if( n == NumObjFollow )
					{
						xmin = Xp ;
						ymin = Yp ;
					}
				}
			}
		}
	}

/*------------------------------------------------------------------------*/
/* objet temporaires (throw, bonus...) */

	ptrextra = ListExtra ;
	for( n=0; n<MAX_EXTRAS; n++, ptrextra++ )
	{
		if( ptrextra->Sprite != -1 )
		{
			if( ptrextra->Flags & EXTRA_TIME_IN )
			{
				if( (TimerRef - ptrextra->Timer) > 35 )
				{
					ptrextra->Timer = TimerRef ;
					ptrextra->Flags &= ~EXTRA_TIME_IN ;

					HQ_3D_MixSample( 11, 0x1000, 1,
						ptrextra->PosX, ptrextra->PosY, ptrextra->PosZ ) ;
				}
				continue ;
			}

			if( ptrextra->Flags & EXTRA_TIME_OUT )
			{
				/* fait clignoter aff */
				if( ptrextra->Flags & EXTRA_FLASH )
				{
				if( TimerRef >= ptrextra->Timer + ptrextra->TimeOut - 50 * 3 )
				{
					if( (TimerRef+ptrextra->Timer) & 8 )
						continue ;
				}
				}
			}

			ProjettePoint(	ptrextra->PosX-WorldXCube,
					ptrextra->PosY-WorldYCube,
					ptrextra->PosZ-WorldZCube ) ;

			/* mettre test du preclip en fonction de la taille */

			if( (Xp>VIEW_X0) AND (Xp<VIEW_X1) AND (Yp>VIEW_Y0) AND (Yp<VIEW_Y1) )
			{
				ListTri[nbobjets].Z = ptrextra->PosX-WorldXCube + ptrextra->PosZ-WorldZCube ;
				ListTri[nbobjets].NumObj = n + TYPE_EXTRA ;
				nbobjets++ ;

				/* les effets n'ont pas d'ombres */
				if( (Shadow==2) AND !(ptrextra->Sprite&32768) )
				{
					GetShadow( ptrextra->PosX,ptrextra->PosY,ptrextra->PosZ ) ;

					ListTri[nbobjets].Z = ptrextra->PosX-WorldXCube + ptrextra->PosZ-WorldZCube - 1 ;
					ListTri[nbobjets].NumObj = TYPE_SHADOW ;	/* shadow */
/*					GetDxDyGraph( ptrextra->Sprite, &dx, &dy, PtrSpriteExtra ) ;	*/
					ListTri[nbobjets].Num = 0 ;
					ListTri[nbobjets].Xw = ShadowX ;
					ListTri[nbobjets].Yw = ShadowY ;
					ListTri[nbobjets].Zw = ShadowZ ;
					nbobjets++ ;
				}
			}
		}
	}

/*------------------------------------------------------------------------*/
/* tri objets */

	SmallSort( ListTri, nbobjets, sizeof( T_SORT ) ) ;

	// corrections des erreurs de tri

	ptrobj = &ListObjet[NUM_PERSO] ;	// Twinsen ZV

	// cherche twinsen

	if( (ptrobj->Body != -1)
	AND (!(ptrobj->Flags&INVISIBLE)) )
	{
		txmin = ptrobj->PosObjX + ptrobj->Xmin ;
		tymin = ptrobj->PosObjY + ptrobj->Ymin ;
		tzmin = ptrobj->PosObjZ + ptrobj->Zmin ;
		txmax = ptrobj->PosObjX + ptrobj->Xmax ;
		tymax = ptrobj->PosObjY + ptrobj->Ymax ;
		tzmax = ptrobj->PosObjZ + ptrobj->Zmax ;

		twinsenpos = -1 ;
		ptrtri = ListTri ;
		for( n=0; n<nbobjets; n++, ptrtri++ )
		{
			if( ptrtri->NumObj == NUM_PERSO + TYPE_OBJ_3D )
			{
				twinsenpos = n ;
				twinsenz = ptrtri->Z ;
				break ;
			}
		}

		// cherche bug porte
		if( twinsenpos != -1 )
		{
			ptrtri = ListTri ;
			for( n=0; n<nbobjets; n++, ptrtri++ )
			{
				numobj = ptrtri->NumObj ;
				typeobj = numobj & ~1023 ;
				numobj &= 1023 ;
				ptrobj = &ListObjet[ numobj ] ;

				switch( typeobj )
				{
				case TYPE_OBJ_SPRITE:

				if( ptrobj->Flags & SPRITE_CLIP ) // tiens une porte ?
				{
					pxmin = ptrobj->AnimStepX + ptrobj->Xmin ;
					pymin = ptrobj->AnimStepY + ptrobj->Ymin ;
					pzmin = ptrobj->AnimStepZ + ptrobj->Zmin ;
					pxmax = ptrobj->AnimStepX + ptrobj->Xmax ;
					pymax = ptrobj->AnimStepY + ptrobj->Ymax ;
					pzmax = ptrobj->AnimStepZ + ptrobj->Zmax ;

					if( (pxmax > txmin) AND (pxmin < txmax) )
					{
						// axe des x commun
						// regarde Z

						if( pzmax <= tzmin )	// twinsen aprs
						{
							if( twinsenz < ptrtri->Z )
							{
								// correction erreur

								ListTri[twinsenpos].Z      = ptrtri->Z ;
								ListTri[twinsenpos].NumObj = ptrtri->NumObj ;

								ptrtri->NumObj = NUM_PERSO + TYPE_OBJ_3D ;
								ptrtri->Z = twinsenz ;

								twinsenpos = n ;
								numobj = -1 ;
								break ;
							}
						}

						if( pzmin >= tzmax )	// twinsen avant
						{
							if( twinsenz > ptrtri->Z )
							{
								// correction erreur

								ListTri[twinsenpos].Z      = ptrtri->Z ;
								ListTri[twinsenpos].NumObj = ptrtri->NumObj ;

								ptrtri->NumObj = NUM_PERSO + TYPE_OBJ_3D ;
								ptrtri->Z = twinsenz ;

								twinsenpos = n ;
								numobj = -1 ;
								break ;
							}
						}

						// axe des Z commun
						// regarde Y min

						// tanpis
						break ;
					}

					if( (pzmax > tzmin) AND (pzmin < tzmax ) )
					{
						// axe des Z commun
						// regarde X

						if( pxmax <= txmin )	// twinsen aprs
						{
							if( twinsenz < ptrtri->Z )
							{
								// correction erreur

								ListTri[twinsenpos].Z      = ptrtri->Z ;
								ListTri[twinsenpos].NumObj = ptrtri->NumObj ;

								ptrtri->NumObj = NUM_PERSO + TYPE_OBJ_3D ;
								ptrtri->Z = twinsenz ;

								twinsenpos = n ;
								numobj = -1 ;
								break ;
							}
						}
						else
						{	// twinsen avant
							if( twinsenz > ptrtri->Z )
							{
								// correction erreur

								ListTri[twinsenpos].Z      = ptrtri->Z ;
								ListTri[twinsenpos].NumObj = ptrtri->NumObj ;

								ptrtri->NumObj = NUM_PERSO + TYPE_OBJ_3D ;
								ptrtri->Z = twinsenz ;

								twinsenpos = n ;
								numobj = -1 ;
								break ;
							}
						}
					}
				}
				break ;

				}

				if( numobj == -1 ) 	break ;
			}
		}
	}

/*------------------------------------------------------------------------*/
/* boucle aff all objs */

	shadowtwinsen = FALSE ;

	NbPhysBox = 0 ;
	for( n=0; n<nbobjets; n++ )
	{

	numobj  = ListTri[n].NumObj ;
	typeobj = numobj & ~1023 ;
	numobj &= 1023 ;

	ptrobj = &ListObjet[ numobj ] ;

	switch( typeobj )
	{
	/*------------------------------------------------------------------------*/
	case TYPE_OBJ_3D:

		if( numobj == NUM_PERSO )
		{
		// correction rustine du petit bug cr par la grosse maguouille du tri

			if( Shadow )
			{
			if( !shadowtwinsen )
			{
					// cherche ombre twinsen

				for( i=n; i<nbobjets; i++ )
				{
					numobj  = ListTri[i].NumObj ;
					typeobj = numobj & ~1023 ;
					numobj &= 1023 ;

					if( ListTri[i].NumObj == TYPE_SHADOW + NUM_PERSO )
					{
						// affiche ombre twinsen

						shadowtwinsen = TRUE ;

						ProjettePoint(	ListTri[i].Xw-WorldXCube,
								ListTri[i].Yw-WorldYCube,
								ListTri[i].Zw-WorldZCube ) ;
						GetDxDyGraph( ListTri[n].Num, &dx, &dy, BufferShadow )	;
						ScreenXmin = Xp -(dx/2) ;
						ScreenXmax = Xp +(dx/2) ;
						ScreenYmin = Yp -(dy/2) ;
						ScreenYmax = Yp +(dy/2) ;
						SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
						if( ClipXmin <= ClipXmax
						AND ClipYmin <= ClipYmax )
						{
							AffGraph( ListTri[i].Num, ScreenXmin, ScreenYmin, BufferShadow ) ;
							xm = ((ListTri[i].Xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
							ym = ((ListTri[i].Yw)/SIZE_BRICK_Y) ;
							zm = ((ListTri[i].Zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
							DrawOverBrick( xm, ym, zm ) ;
							AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
						}
						UnSetClip() ;

						// detruit entree liste
						ListTri[i].NumObj = -1 ; // type inconnu

						break ; // sort boucle for
					}
				}

			}
			}

			// gestion cliping noyade

			if( FlagWater )
			{
				SetClip( 0,0, 639,FlagWater ) ;
			}

				// affiche twinsen
		}


		ptranim = HQR_Get( HQR_Anims, ptrobj->Anim ) ;
		ptrbody = PtrBody[ ptrobj->Body ] ;

		SetInterAnimObjet2( ptrobj->Frame, ptranim, ptrbody ) ;

		err = AffObjetIso(	ptrobj->PosObjX-WorldXCube, ptrobj->PosObjY-WorldYCube, ptrobj->PosObjZ-WorldZCube,
					0, ptrobj->Beta, 0,
					ptrbody ) ;

		if( !err )	/* objet pas tout clipp */
		{
			SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
			if( ClipXmin <= ClipXmax
			AND ClipYmin <= ClipYmax )
			{
				ptrobj->WorkFlags |= WAS_DRAWN ;

				xm = ((ptrobj->PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				ym = ((ptrobj->PosObjY)/SIZE_BRICK_Y) ;
				if( (ptrobj->Col&127) != 0 )	ym++ ;
				zm = ((ptrobj->PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				DrawOverBrick( xm, ym, zm ) ;
				AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;

				if( (ptrobj->Flags & OBJ_BACKGROUND)
				AND (flagflip == TRUE) )
				{
					CopyBlock( ClipXmin, ClipYmin, ClipXmax, ClipYmax, Log,
						   ClipXmin, ClipYmin, Screen ) ;
				}

//DrawZV( &ListObjet[numobj] ) ;

			}
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_OBJ_SPRITE:

		ProjettePoint(	ptrobj->PosObjX-WorldXCube,
				ptrobj->PosObjY-WorldYCube,
				ptrobj->PosObjZ-WorldZCube ) ;

		num = ptrobj->Body ;

		GetDxDyGraph( 0, &dx, &dy, HQR_Get(HQRPtrSpriteExtra,num) ) ;

		ScreenXmin = Xp + PtrZvExtra[ num*8 + 0 ] ;
		ScreenYmin = Yp + PtrZvExtra[ num*8 + 1 ] ;
		ScreenXmax = ScreenXmin + dx ;
		ScreenYmax = ScreenYmin + dy ;

		if( ptrobj->Flags & SPRITE_CLIP )
		{
			SetClip(ptrobj->Info+XpOrgw, ptrobj->Info1+YpOrgw,
				ptrobj->Info2+XpOrgw, ptrobj->Info3+YpOrgw ) ;
		}
		else
		{
			SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
		}

		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			AffGraph( 0, ScreenXmin, ScreenYmin, HQR_Get(HQRPtrSpriteExtra,num) ) ;

			ptrobj->WorkFlags |= WAS_DRAWN ;

			if( ptrobj->Flags & SPRITE_CLIP )
			{
				xm = ((ptrobj->AnimStepX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				ym = ((ptrobj->AnimStepY)/SIZE_BRICK_Y) ;
				/*	if( ptrobj->Col != 0 )	ym++ ;	*/
				zm = ((ptrobj->AnimStepZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				DrawOverBrick3( xm, ym, zm ) ;
			}
			else
			{
				xm = ((ptrobj->PosObjX+ptrobj->Xmax+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
				ym = ((ptrobj->PosObjY)/SIZE_BRICK_Y) ;
				if( (ptrobj->Col&127) != 0 )	ym++ ;
				zm = ((ptrobj->PosObjZ+ptrobj->Zmax+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;

				DrawOverBrick3( xm, ym, zm ) ;
			}

			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;

			if( (ptrobj->Flags & OBJ_BACKGROUND)
			AND (flagflip == TRUE) )
			{
				CopyBlock( ClipXmin, ClipYmin, ClipXmax, ClipYmax, Log,
					   ClipXmin, ClipYmin, Screen ) ;
			}
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_SHADOW:

		if( numobj == NUM_PERSO )	shadowtwinsen = TRUE ;

		ProjettePoint(	ListTri[n].Xw-WorldXCube,
				ListTri[n].Yw-WorldYCube,
				ListTri[n].Zw-WorldZCube ) ;

		GetDxDyGraph( ListTri[n].Num, &dx, &dy, BufferShadow )	;

		ScreenXmin = Xp -(dx/2) ;
		ScreenXmax = Xp +(dx/2) ;
		ScreenYmin = Yp -(dy/2) ;
		ScreenYmax = Yp +(dy/2) ;
		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;

		/* mettre affpoly (lba2) */

		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			AffGraph( ListTri[n].Num, ScreenXmin, ScreenYmin, BufferShadow )	;

//			ptrobj = ListObjet ; ?
			xm = ((ListTri[n].Xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
			ym = ((ListTri[n].Yw)/SIZE_BRICK_Y) ;
			zm = ((ListTri[n].Zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
			DrawOverBrick( xm, ym, zm ) ;

			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
		}
		break ;

	/*------------------------------------------------------------------------*/
	case TYPE_EXTRA:

		ptrextra = &ListExtra[ numobj ] ;

		ProjettePoint(	ptrextra->PosX-WorldXCube,
				ptrextra->PosY-WorldYCube,
				ptrextra->PosZ-WorldZCube ) ;

		num = ptrextra->Sprite ;
		if( num & 32768 )	/* effet spcial */
		{
			AffSpecial( numobj, Xp, Yp ) ;
		}
		else			/* sprite */
		{
			GetDxDyGraph( 0, &dx, &dy, HQR_Get(HQRPtrSpriteExtra,num) ) ;

			ScreenXmin = Xp + PtrZvExtra[ num*8 + 0 ] ;
			ScreenYmin = Yp + PtrZvExtra[ num*8 + 1 ] ;
			ScreenXmax = ScreenXmin + dx ;
			ScreenYmax = ScreenYmin + dy ;

			AffGraph( 0, ScreenXmin, ScreenYmin, HQR_Get(HQRPtrSpriteExtra,num) ) ;
		}

		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			xm = ((ptrextra->PosX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
			ym = ((ptrextra->PosY)/SIZE_BRICK_Y) ;
			zm = ((ptrextra->PosZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;

			DrawOverBrick( xm, ym, zm ) ;

			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
		}

		break ;
	/*------------------------------------------------------------------------*/
	}	/* end switch typeobj */
	UnSetClip() ;

	}	/* end for nbobjets */

/*------------------------------------------------------------------------*/
// special incrust disp

	ptrdisp = ListIncrustDisp ;
	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
	if( ptrdisp->Num == -1 )	continue ;

	switch( ptrdisp->Move )
	{
	case 0:	// wait nb sec and die
		if( TimerRef > ptrdisp->TimerEnd )
		{
			ptrdisp->Num = -1 ;
			continue ;
		}
		break ;

	case 1:	// follow obj coor for nb sec and die
		num = ptrdisp->Info ;

		ProjettePoint(	ListObjet[num].PosObjX - WorldXCube,
				ListObjet[num].PosObjY + ListObjet[num].Ymax - WorldYCube,
				ListObjet[num].PosObjZ - WorldZCube ) ;

		ptrdisp->X = Xp ;
		ptrdisp->Y = Yp ;

		if( TimerRef > ptrdisp->TimerEnd )
		{
			ptrdisp->Num = -1 ;
			continue ;
		}
		break ;
	}

	switch( ptrdisp->Type )
	{
	case INCRUST_NUM:

		strcpy( string, Itoa( ptrdisp->Num ) ) ;
		dx = SizeFont( string ) ;
		dy = 48 ;

		ScreenXmin = ptrdisp->X - dx/2 ;
		ScreenYmin = ptrdisp->Y - dy/2 ;
		ScreenXmax = ScreenXmin + dx ;
		ScreenYmax = ScreenYmin + dy ;

		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
		CoulFont( ptrdisp->Info ) ;
		Font( ScreenXmin, ScreenYmin, string ) ;

		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
		}
		break ;

	case INCRUST_TEXT:

		GetMultiText( ptrdisp->Num, string ) ;
		dx = SizeFont( string ) + 1 ;	// because /2
		dy = 48 ;

		ScreenXmin = ptrdisp->X - dx/2 ;
		ScreenYmin = ptrdisp->Y - dy/2 ;
		ScreenXmax = ScreenXmin + dx ;
		ScreenYmax = ScreenYmin + dy ;

		if( ScreenXmax >= 640 )
		{
			ScreenXmin -= ScreenXmax - 639 ;
			ScreenXmax = 639 ;
		}

		if( ScreenXmin < 0 )
		{
			ScreenXmax = dx ;
			ScreenXmin = 0 ;
		}

		if( ScreenYmax >= 480 )
		{
			ScreenYmin -= ScreenYmax - 479 ;
			ScreenYmax = 479 ;
		}

		if( ScreenYmin < 0 )
		{
			ScreenYmax = dy ;
			ScreenYmin = 0 ;
		}

		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
//		CoulFont( 0 ) ;
//		Font( ClipXmin+2, ClipYmin+4, string ) ;
		CoulFont( ListObjet[ptrdisp->Info].CoulObj*16 + 14 ) ;
		Font( ClipXmin, ClipYmin, string ) ;

		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
		}
		break ;

	case INCRUST_SPRITE:

		GetDxDyGraph( 0, &dx, &dy, HQR_Get(HQRPtrSpriteExtra,ptrdisp->Num) ) ;
		ScreenXmin = ptrdisp->X + PtrZvExtra[ ptrdisp->Num*8 + 0 ] ;
		ScreenYmin = ptrdisp->Y + PtrZvExtra[ ptrdisp->Num*8 + 1 ] ;
		ScreenXmax = ScreenXmin + dx ;
		ScreenYmax = ScreenYmin + dy ;

		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
		AffGraph( 0, ScreenXmin, ScreenYmin, HQR_Get(HQRPtrSpriteExtra,ptrdisp->Num) ) ;
		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
		}
		break ;

	case INCRUST_CMPT:

		cmpt = ptrdisp->TimerEnd-TimerRef ;
		c = BoundRegleTrois( ptrdisp->Info, ptrdisp->Num, 50*2, cmpt-50 ) ;

		strcpy( string, Itoa( c ) ) ;
		dx = SizeFont( string ) ;
		dy = 48 ;

		ScreenXmin = ptrdisp->X ;
		ScreenYmin = ptrdisp->Y - dy/2 ;
		ScreenXmax = ScreenXmin + dx ;
		ScreenYmax = ScreenYmin + dy ;

		SetClip( ScreenXmin, ScreenYmin, ScreenXmax, ScreenYmax ) ;
		CoulFont( 155 ) ;
		Font( ScreenXmin, ScreenYmin, string ) ;

		if( ClipXmin <= ClipXmax
		AND ClipYmin <= ClipYmax )
		{
			AddPhysBox( ClipXmin, ClipYmin, ClipXmax, ClipYmax ) ;
		}
		break ;

	case INCRUST_OBJ:
		{
		UBYTE	*ptr3do ;

		Box( 10, 10, 69, 69, 0 ) ;
		SetClip(  10, 10, 69, 69 ) ;

		ptr3do = HQR_Get( InventoryObj, ptrdisp->Num ) ;
		if( HQR_Flag )
		{
			PatchObjet( ptr3do ) ;
		}

		SetProjection( 40,40, 128,200,200 ) ;

		SetFollowCamera( 0,0,0, 60,0,0, 16000 ) ;
		AffObjetIso(	0,0,0,	0,BetaUsedObj+=8,0, ptr3do ) ;

		DrawCadre( 10, 10, 69, 69 ) ;
		AddPhysBox( 10, 10, 69, 69 ) ;

		Init3DGame() ;
		}
		break ;
	}
	}
	UnSetClip() ;

/*------------------------------------------------------------------------*/
/* flip boxes */

	if( FlagVsync )	Vsync() ;
	if( FlagMCGA )
	{
		CopyBlockPhysMCGA( xmin, ymin, xmax, ymax ) ;
		FlipOptList() ;
		if( flagflip )	RestoreTimer() ;
	}
	else
	{
		if( !flagflip )
		{
			FlipBoxes() ;			/* Flip Log vers Phys */
		}
		else
		{
			Flip() ;
			FlipOptList() ;
			RestoreTimer() ;
		}
	}
	if( FlagFade )
	{
		if( FlagPalettePcx )	FadeToPal( PalettePcx ) ;
		else			FadeToPal( PtrPal ) ;
		FlagFade = FALSE ;
	}
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		฿฿฿Û฿ Û฿฿฿Û ÛÛÜ Û Û฿฿฿฿       Û฿฿฿Ü Û฿฿฿฿ Û฿฿฿฿
		 ÜÛ   ÛÛ  Û ÛÛ฿ÛÛ ÛÛ฿฿        ÛÛ  Û ÛÛ฿฿  ÛÛ
		฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿  ฿฿฿฿฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	CheckZoneSce( T_OBJET *ptrobj, WORD numobj )
{
	WORD	n ;
	T_ZONE	*ptrz ;
	WORD	x,y,z ;
	WORD	flaggrm = FALSE ;

	x = ptrobj->PosObjX ;
	y = ptrobj->PosObjY ;
	z = ptrobj->PosObjZ ;

	ptrz = ListZone ;

	ptrobj->ZoneSce = -1 ;

	if( numobj == NUM_PERSO )
	{
		FlagClimbing = FALSE ;
	}

	for( n=0; n<NbZones; n++ )
	{
		if( x >= ptrz->X0
		AND x <= ptrz->X1
		AND y >= ptrz->Y0
		AND y <= ptrz->Y1
		AND z >= ptrz->Z0
		AND z <= ptrz->Z1 )
		{
			// si type test angle

			switch( ptrz->Type )
			{
			case 0:	// change cube perso
				if( numobj == NUM_PERSO )
				{
				// protection meurt dans un autre cube
				if( ptrobj->LifePoint > 0 )
				{
					NewCube = ptrz->Num ;
					NewPosX = ptrobj->PosObjX - ptrz->X0 + ptrz->Info0 ;
					NewPosY = ptrobj->PosObjY - ptrz->Y0 + ptrz->Info1 ;
					NewPosZ = ptrobj->PosObjZ - ptrz->Z0 + ptrz->Info2 ;
					FlagChgCube = 1 ;
					return ;
				}
				}
				break ;

			case 1:	// set pos camera
				if( numobj == NumObjFollow )
				{
					// signale pos force
					CameraZone = TRUE ;

					if( StartXCube != ptrz->Info0
					OR  StartYCube != ptrz->Info1
					OR  StartZCube != ptrz->Info2 )
					{
						StartXCube = ptrz->Info0 ;
						StartYCube = ptrz->Info1 ;
						StartZCube = ptrz->Info2 ;
						FirstTime = TRUE ;
					}
				}
				break ;

			case 2:	// zone scenarique
				ptrobj->ZoneSce = ptrz->Num ;
				break ;

			case 3: // zone grm
				if( numobj == NumObjFollow )
				{
					flaggrm = TRUE ;
					if( ZoneGrm != ptrz->Num )
					{
						if( ZoneGrm != -1 )
							CopyMapToCube() ;
						ZoneGrm = ptrz->Num ;
						IndexGrm = n ;
						SaveTimer() ;
						IncrustGrm( ptrz->Num ) ;
						RestoreTimer() ;
					}
				}
				break ;

			case 4:	// zone giver
				if( numobj == NUM_PERSO )
				{
					if( ActionNormal )
					{
						InitAnim( GEN_ANIM_ACTION, ANIM_THEN, GEN_ANIM_RIEN, NUM_PERSO ) ;
						ZoneGiveExtraBonus( ptrz ) ;
					}
				}
				break ;

			case 5:	// zone message
				if( numobj == NUM_PERSO )
				{
					if( ActionNormal )
					{
						SaveTimer() ;
						TestRestoreModeSVGA( TRUE ) ;
						TestCoulDial( ptrz->Info0 ) ;
#ifdef	CDROM
						NumObjSpeak = NUM_PERSO ;
#endif
						Dial( ptrz->Num ) ;
						RestoreTimer() ;

						AffScene(TRUE)	;

						WaitReleaseSpace() ;
					}
				}
				break ;

			case 6: // echelle
				if( (numobj==NUM_PERSO) AND (Comportement!=C_PROTOPACK) )
				{
				if( ( ptrobj->GenAnim == GEN_ANIM_MARCHE )
				    OR ( ptrobj->GenAnim == GEN_ANIM_ECHELLE )
				    OR ( ptrobj->GenAnim == GEN_ANIM_MONTE ) )
				{
					Rotate( ptrobj->Xmin, ptrobj->Zmin, ptrobj->Beta + 896 + 512 ) ;
					X0 += Nxw ;
					Y0 += Nzw ;

					if( (X0 >= 0) AND (Y0 >= 0)
					AND (X0 <= 63*SIZE_BRICK_XZ) AND (Y0 <= 63*SIZE_BRICK_XZ)
					AND (WorldColBrick( X0, ptrobj->PosObjY+256, Y0 ) != 0) )
					{
						FlagClimbing = TRUE ;

						if( ptrobj->PosObjY >= ((ptrz->Y0+ptrz->Y1)/2) )
						{
							InitAnim( GEN_ANIM_ECHELLE, ANIM_ALL_THEN, GEN_ANIM_RIEN, numobj ) ;
						}
						else
						{
							InitAnim( GEN_ANIM_MONTE, ANIM_REPEAT, NO_ANIM, numobj ) ;
						}
					}
				}
				}
				break ;
			}
		}
		ptrz++ ;
	}

	if( flaggrm == FALSE
	AND numobj == NumObjFollow
	AND ZoneGrm != -1 )
	{
		IndexGrm = ZoneGrm = -1 ;
		CopyMapToCube() ;
		FirstTime = TRUE;
	}
}

#ifdef	DEBUG_TOOLS
void	ClearZoneSce()
{
	WORD	n ;
	T_ZONE	*ptrz ;

	ptrz = ListZone ;

	for( n=0; n<NbZones; n++ )
	{
		switch( ptrz->Type )
		{
		case 4:	// zone giver

			ptrz->Info2 = 0 ;
			break ;
		}
		ptrz++ ;
	}
}
#endif
