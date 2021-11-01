/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

    Û฿฿฿฿ Û฿฿฿Û ÛÜ ÜÛ Û฿฿฿Û Û฿฿฿Û Û฿฿฿Û ฿฿Û฿฿ Û฿฿฿฿ ÛÜ ÜÛ Û฿฿฿฿ ÛÛÜ Û ฿฿Û฿฿
    ÛÛ    ÛÛ  Û ÛÛ฿ Û ÛÛ฿฿฿ ÛÛ  Û ÛÛ฿Û฿   ÛÛ  ÛÛ฿฿  ÛÛ฿ Û ÛÛ฿฿  ÛÛ฿ÛÛ   ÛÛ
    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿    ฿฿฿฿฿ ฿฿  ฿   ฿฿  ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿   ฿฿

 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

#include 	"c_extern.h"

#define	ULTRA	-1L
#define	ALWAYS	ULTRA

T_MENU_COMP	ListMenuComp[MAX_MENUS_COMP] = {
	{	//*********** 0--> Normal,Sportif,Agressif,Discret
		ALWAYS,
		3,	// Previous = CONQUE
		1,	// Next     = PROTOPACK
		4,
		{C_NORMAL,C_SPORTIF,C_AGRESSIF,C_DISCRET}
	},

	{	//*********** 1--> Protopack
		FLAG_PROTOPACK,
		0,	// Previous = NORMAL,SPORTIF,AGRESSIF,DISCRET
		3,	// Next     = CONQUE
		1,	// 1 seul au debut du jeu
		{C_PROTOPACK,C_JETPACK,-1,-1}
	},

	{	//*********** 2--> Double
		ALWAYS,
		-1,	// Previous = Aucun
		-1,	// Next	    = Aucun
		1,
		{C_DOUBLE,-1,-1,-1}
	},

	{	//*********** 3--> Conque
		FLAG_CONQUE,
		1,	// Previous = PROTOPACK
		0,	// Next     = NORMAL,SPORTIF,AGRESSIF,DISCRET
		1,
		{C_CONQUE,-1,-1,-1}
	},

	{	//*********** 4--> Scaphandre Interieur
		ALWAYS,
		-1,	// Previous = Aucun
		-1,	// Next     = Aucun
		2,
		{C_SCAPH_INT_NORM,C_SCAPH_INT_SPOR,-1,-1}
	},

	{	//*********** 5--> Scaphandre Exterieur
		ALWAYS,
		-1,	// Previous = Aucun
		-1,	// Next     = Aucun
		2,
		{C_SCAPH_EXT_NORM,C_SCAPH_EXT_SPOR,-1,-1}
	},

	{	//*********** 6--> Buggy
		ALWAYS,
		3,	// Previous = Conque
		0,	// Next     = NORMAL,SPORTIF,AGRESSIF,DISCRET
		1,
		{C_BUGGY,-1,-1,-1}
	},

	{	//*********** 7--> Squelette
		ALWAYS,
		-1,	// Previous = Aucun
		-1,	// Next     = Aucun
		1,
		{C_SKELETON,-1,-1,-1}
	}
} ;


#define	COUL_LIFE_POINTS	74	// couleur de la barre des points de vie
#define	COUL_MAGIC_POINTS	200	// couleur de la barre des points de magie

MOVE		RealRot 	;
T_OBJ_3D	InvObjTwinsen[MAX_COMPORTEMENTS];

S32	DebComportement = 110 ;	// Espacement entre deux comportement
S32	ComportementMenu = 0 ;		// menu comportement en cours
S32	IndexComportement = 0 ;

#define	CTRL_X0	100
#define	CTRL_Y0	100
#define	CTRL_X1	550
#define	CTRL_Y1	290

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

//		      Gestion des menus comportements

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

// Recupere le menu comportement precedent disponible
S32	GetPreviousMenuComp( )
{
	T_MENU_COMP *ptrmcomp = &ListMenuComp[ComportementMenu] ;

	S32	flag = FALSE ;
	S32	n = -1 ;

	while( !flag AND n!=ComportementMenu )
	{
		n = ptrmcomp->Previous ;

		if( n==-1 )	return( ComportementMenu ) ;

		ptrmcomp = &ListMenuComp[n] ;

		if( ptrmcomp->Enable==ALWAYS OR
#ifdef	LBA_EDITOR
		ListVarGame[ptrmcomp->Enable].Value )	return( n ) ;
#else
		ListVarGame[ptrmcomp->Enable] )	return( n ) ;
#endif
	}

	return( ComportementMenu ) ;
}

// Recupere le menu comportement suivant disponible
S32	GetNextMenuComp( )
{
	T_MENU_COMP *ptrmcomp = &ListMenuComp[ComportementMenu] ;

	S32	flag = FALSE ;
	S32	n = -1 ;

	while( !flag AND n!=ComportementMenu )
	{
		n = ptrmcomp->Next ;

		if( n==-1 )	return( ComportementMenu ) ;

		ptrmcomp = &ListMenuComp[n] ;

		if( ptrmcomp->Enable==ALWAYS OR
#ifdef	LBA_EDITOR
		ListVarGame[ptrmcomp->Enable].Value )	return( n ) ;
#else
		ListVarGame[ptrmcomp->Enable] )	return( n ) ;
#endif
	}

	return( ComportementMenu ) ;
}


/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	DrawObjComportement( S32 x0, S32 y0, S32 x1, S32 y1,
	S32 y3d , S32 betaflag ,
	T_OBJ_3D *ptrobj3d )
{
	S32	beta ;

	SetIsoProjection( (x1+x0)/2, (y1+y0)/2 );

	SetClip( x0,y0, x1,y1 ) 		;

	if( betaflag == -1 )
	{
		beta = ptrobj3d->Beta + GetDeltaMove( &RealRot ) ;
		beta &= 4095 ;
	}
	else
	{
		beta = betaflag ;
	}

	ptrobj3d->X = 0		;
	ptrobj3d->Y = y3d	;
	ptrobj3d->Z = 0		;
	ptrobj3d->Beta = beta	;

	ObjectDisplay(ptrobj3d)	;//	Animated
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
void	DrawComportement( S32 num, S32 comportement, S32 beta, S32 copyblock )
{
	S32	x0, y0, x1, y1 	;

	x0 = CTRL_X0+DebComportement + num * 110 -1	;
	y0 = CTRL_Y0+10-1 			;
	x1 = x0+99+1 				;
	y1 = y0+119+1 				;

	ObjectSetInterAnim( &InvObjTwinsen[comportement] )	;

	if( !copyblock )	/* init */
	{
		// cadre des comportements
		DrawCadre( x0, y0, x1, y1, ALL_ANGLES|BACKUP_ANGLES ) ;

		// cadre texte
		DrawCadre( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40,
			   ALL_ANGLES|BACKUP_ANGLES ) ;

	}

	MemoClip() ;
	UnsetClip() ;

	if( comportement != Comportement )
	{
		DrawCadreBox( x0,y0, x1,y1, 0 ) ;
	}
	else
	{
		U32	arrows	= 0 ;

		// cadre des comportements
		DrawCadreBox( x0,y0, x1,y1, COUL_SELECT_MENU ) ;

		// cadre texte
		DrawCadreBox( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40, 0 ) ;

		if( GetNextMenuComp()!=ComportementMenu )	arrows |= ARROW_ID	;
		if( GetPreviousMenuComp()!=ComportementMenu )	arrows |= ARROW_SG	;

		if( arrows )
		{
			DrawAngles( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40,
				    arrows ) ;
		}

		ColorFont( LBAWHITE ) ;

		GetMultiText( comportement+START_TEXT_COMPORTEMENT, String ) ;

		Font( (CTRL_X1+CTRL_X0-SizeFont( String ))/2, CTRL_Y0+140, String )		;
	}

	DrawObjComportement(	x0+1, y0+1, x1-1, y1-1,
			-600, beta,
			&InvObjTwinsen[comportement] ) ;

	if( copyblock )
	{
		BoxStaticAdd( x0,y0,x1,y1 ) ;
		BoxStaticAdd( CTRL_X0+10, CTRL_Y0+139, CTRL_X1-10, CTRL_Y0+139+40 ) ;
		BoxUpdate()	;
	}

	RestoreClip() ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	DrawInfoMenu( S32 x0, S32 y0 )
{
	S32	x,x1 ;
	S32	l ;
	S32	lifepoint ;

	UnsetClip() ;

	BackupAngles( x0,y0, x0+450, y0+80 ) ;
	DrawCadre( x0,y0, x0+450, y0+80, 0 ) ;
	Box( x0+1,y0+1, x0+450-1, y0+80-1, 0 ) ;
	DrawAngles( x0,y0, x0+450, y0+80, ALL_ANGLES ) ;
	RestoreAngles( x0,y0, x0+450, y0+80 ) ;

	// Life Points	(max 255)

	ScaleFactorSprite = DEF_SCALE_FACTOR ;

	PtrAffGraph( x0+9, y0+13, SPRITE_COEUR ) ;

	lifepoint = ListObjet[NUM_PERSO].LifePoint ;

	// blindage
	if( lifepoint>MAX_LIFE_POINTS )	lifepoint = MAX_LIFE_POINTS ;

	x = RegleTrois( x0+25, x0+325, MAX_LIFE_POINTS, lifepoint ) ;

	BackupAngles( x0+25, y0+10, x0+325, y0+10+15 ) ;
	Box( x0+25, y0+10, x, y0+10+15, COUL_LIFE_POINTS ) ;
	Box( x, y0+10, x0+325, y0+10+15, 0 ) ;

	DrawCadre( x0+25, y0+10, x0+325, y0+10+15, ALL_ANGLES ) ;
	RestoreAngles( x0+25, y0+10, x0+325, y0+10+15 ) ;

	// Magic level	(max 4)	MagicPoint (max 80)

	PtrAffGraph( x0+9, y0+36, SPRITE_MAGIE ) ;

	if( MagicLevel )
	{
		x1 = RegleTrois( x0+25, x0+325, 40, MagicLevel*10 ) ;
		x = RegleTrois( x0+25, x0+325, 80, MagicPoint ) ;

		BackupAngles( x0+25, y0+35, x1, y0+35+15 ) ;
		Box( x0+25, y0+35, x, y0+35+15, COUL_MAGIC_POINTS ) ;
		DrawCadre( x0+25, y0+35, x1, y0+35+15, ALL_ANGLES ) ;
		RestoreAngles( x0+25, y0+35, x1, y0+35+15 ) ;
	}

	// Nb Gold Pieces

	PtrAffGraph( x0+340, y0+15, SPRITE_PIECE ) ;
	ColorFont( 155 ) ;

	if( Planet>=2 )	l = NbZlitosPieces 	;
	else		l = NbGoldPieces 	;

	Font( x0+370, y0+5, Itoa(l) ) ;

	// Nb Little Keys

	PtrAffGraph( x0+340, y0+55, SPRITE_CLE ) ;
	ColorFont( 155 ) ;
	Font( x0+370, y0+40, Itoa(NbLittleKeys) ) ;

	// Nb Four-leaf Clover

#ifdef	LBA_EDITOR
	for( l=0; l<ListVarGame[FLAG_CLOVER].Value; l++ )
#else
	for( l=0; l<ListVarGame[FLAG_CLOVER]; l++ )
#endif
	{
		x1 = RegleTrois( x0+25, x0+325, MAX_CLOVER_BOX, l ) ;
		PtrAffGraph( x1, y0+58, SPRITE_FULL_CLOVER_BOX ) ;
	}

	// Nb CloverBox

#ifdef	LBA_EDITOR
	for( l=ListVarGame[FLAG_CLOVER].Value; l<NbCloverBox; l++ )
#else
	for( l=ListVarGame[FLAG_CLOVER]; l<NbCloverBox; l++ )
#endif
	{
		x1 = RegleTrois( x0+25, x0+325, MAX_CLOVER_BOX, l ) ;
		PtrAffGraph( x1, y0+58, SPRITE_CLOVER_BOX ) ;
	}

	BoxStaticAdd( x0,y0, x0+450, y0+80 ) ;
	BoxUpdate()	;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	DrawMenuComportement( S32 beta )
{
	S32	n ;
	T_MENU_COMP *ptrmcomp = &ListMenuComp[ComportementMenu] ;

	SetIsoProjection( 320-8-1, 240 ) 			;
	SetPosCamera( 0,0,0 ) 				;
	SetAngleCamera( 0,0,0 ) 			;
	SetLightVector( AlphaLight, BetaLight, 0 ) 	;

	BackupAngles( CTRL_X0  , CTRL_Y0  , CTRL_X1, CTRL_Y1 ) ;
	DrawCadre( CTRL_X0  , CTRL_Y0  , CTRL_X1, CTRL_Y1, ALL_ANGLES ) ;
	ShadeBoxBlk( CTRL_X0+1,CTRL_Y0+1, CTRL_X1-1, CTRL_Y1-1, 8 ) ;
	RestoreAngles( CTRL_X0  , CTRL_Y0  , CTRL_X1, CTRL_Y1 ) ;

	for( n=0; n<ptrmcomp->NbComps; n++ )
	{
		if( ptrmcomp->Comps[n]==Comportement )	IndexComportement = n ;

		DrawComportement( n, ptrmcomp->Comps[n], beta, FALSE ) ;
	}

	DrawInfoMenu( CTRL_X0, CTRL_Y1+10 ) ;

	BoxStaticAdd( CTRL_X0,CTRL_Y0, CTRL_X1, CTRL_Y1 ) ;
	BoxUpdate()	;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	CalculeDebComportement( T_MENU_COMP *ptrmcomp )
{
	// Calcule ordonnee du premier comportement
	DebComportement = ((ptrmcomp->NbComps*100)+((ptrmcomp->NbComps-1)*10))/2 ;
	DebComportement = 10+((CTRL_X1-10)-(CTRL_X0+10))/2-DebComportement ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
// parametre release: TRUE si on dsire laisser le menu comportement actif
// si on lache rapidement la touche <CTRL>

void	MenuComportement( S32 affone )
{
	T_OBJET	*ptrobj 	;
	U8	*memoptrfile3d 	;
	S32	oldindex 	;
	S32	oldmenu		;
	S32	memoisland 	;
	S32	bodyhero	;
	S32	release = TRUE	;
	S32	n		;
	S32	flag = FALSE	;
	S32	savetimer	;
	S32	oldcomp		;
	T_MENU_COMP *ptrmcomp	;
	S32	flagproto = FALSE ;

	SaveTimer() ;

	UnsetClip() ;

	SaveCamera() ;

	ptrobj = &ListObjet[NUM_PERSO] ;

	// au cas ou l'on soit appel pendant un eclair
	Palette( PtrPal ) ;

	// Blindage Buggy
	if( ptrobj->Move==MOVE_BUGGY
	OR  ptrobj->Move==MOVE_BUGGY_MANUAL )
	{
		// modifie MenuComportement de la conque
		ListMenuComp[3].Next = 6 ;	// branche sur comp Buggy
		// modifie MenuComportement Normal
		ListMenuComp[0].Previous = 6 ;	// branche sur comp Buggy
	}
	else
	{
		// modifie MenuComportement de la conque
		ListMenuComp[3].Next = 0 ;	// branche sur comp Normal
		// modifie MenuComportement Normal
		ListMenuComp[0].Previous = 3 ;	// branche sur comp Conque
	}

	oldcomp = Comportement ;

	ComportementMenu = ListComportement[Comportement].Menu ;
	ptrmcomp = &ListMenuComp[ComportementMenu] ;

	CheckProtoPack() ;

	CalculeDebComportement( ptrmcomp ) ;

	memoptrfile3d = ptrobj->PtrFile3D ;

	bodyhero = ptrobj->Obj.Body.Num	  ;

	// stop sample protopack
	if( Comportement==C_PROTOPACK
	OR  Comportement==C_JETPACK )
	{
#ifdef	LBA_EDITOR
		HQ_StopOneSample( NUM_SAMPLE(COMMON_NUM_DEF,SAMPLE_PROTOPACK) ) ;
#else
		HQ_StopOneSample( SAMPLE_PROTOPACK ) ;
#endif
		ptrobj->SampleAlways = 0 ;

		// pour pouvoir restaurer anim marche protopack
#ifdef	LBA_EDITOR
		if( !stricmp(ptrobj->StringAnim,GEN_ANIM_MARCHE) )
#else
		if( ptrobj->GenAnim==GEN_ANIM_MARCHE )
#endif
		{
			flagproto = TRUE ;
		}
	}

	// Init bodys
	for( n=0; n<MAX_COMPORTEMENTS; n++ )
	{
		// Pour tre propre (c'est le Barbu qui m'a dit ca)
		ObjectClear( &InvObjTwinsen[n] ) ;

		ptrobj->PtrFile3D = ListComportement[n].PtrFile3d ;

#ifdef	LBA_EDITOR
		bodyhero = SearchBody( ptrobj->StringBody, NUM_PERSO ) ;
		if( bodyhero==-1 )
		{
			bodyhero = SearchBody( GEN_BODY_NORMAL, NUM_PERSO ) ;
			if( bodyhero==-1 )
			{
				MessageF( TRUE, "Erreur: Body %s Comportement %d introuvable", GEN_BODY_NORMAL, n ) ;
			}
		}
#else
		bodyhero = SearchBody( ptrobj->GenBody, NUM_PERSO ) ;

		if( bodyhero==-1 )
		{
			bodyhero = SearchBody( GEN_BODY_NORMAL, NUM_PERSO ) ;
			if( bodyhero==-1 )
			{
				TheEnd( NAME_NOT_FOUND, Itoa( GEN_BODY_NORMAL ) ) ;
			}
		}
#endif

		ObjectInitBody( &InvObjTwinsen[n], (void*)bodyhero ) ;
		ObjectInitAnim( &InvObjTwinsen[n], (void*)ListComportement[n].AnimRien )	;
	}

	ptrobj->PtrFile3D = memoptrfile3d ;

	InitMove( &RealRot, -1024 ) ;

	BackupScreen( TRUE ) 			;// Screen => ScreenAux

	memoisland = Island ;
	Island = 255 ;
	InitDial( 0 ) ;

	DrawMenuComportement( ptrobj->Obj.Beta ) ;

	oldindex = IndexComportement ;
	oldmenu  = ComportementMenu  ;

	ManageTime() ;
	savetimer = TimerRefHR ;

	while( Input&I_JOY )
	{
		ManageTime() ;
		MyGetInput() ;
		DrawComportement( IndexComportement, Comportement, -1, TRUE ) ;
	}

	while( !flag )
	{
		ManageTime() ;
		if( affone AND (TimerRefHR>affone+savetimer) )	flag = TRUE ;

#ifdef	DEBUG_TOOLS
		CheckSaveLogPcx( PtrPalNormal ) ;
#endif
		if( !affone )	MyGetInput() ;

		if( release )
		{
			if( !(Input&I_COMPORTEMENT) )
			{
				if( TimerRefHR-savetimer<2*20 )
				{
					release = FALSE ;
				}
				else	flag = TRUE ;
			}
		}
		else
		{
			if( Input&(I_COMPORTEMENT|I_RETURN|I_MENUS)
			OR  MyKey==K_ESC )
			{
				flag = TRUE ;
			}
		}

		if( Input & I_UP )
		{
			ComportementMenu = GetPreviousMenuComp() ;
		}

		if( Input & I_DOWN )
		{
			ComportementMenu = GetNextMenuComp() ;
		}

		if( ComportementMenu != oldmenu )
		{
			oldmenu = ComportementMenu ;

			ptrmcomp = &ListMenuComp[ComportementMenu] ;

			CalculeDebComportement( ptrmcomp ) ;

			if( IndexComportement>=ptrmcomp->NbComps )
				IndexComportement = ptrmcomp->NbComps-1 ;

			oldindex = IndexComportement ;
			Comportement = (U8)ptrmcomp->Comps[IndexComportement] ;

			CopyBlock( CTRL_X0, CTRL_Y0, CTRL_X1, CTRL_Y1, Screen,
				   CTRL_X0, CTRL_Y0, Log ) ;

			DrawMenuComportement( -1 ) ;

			while( Input & I_JOY )
			{
				ManageTime() ;
				MyGetInput() ;
				DrawComportement( IndexComportement, Comportement, -1, TRUE ) ;
			}
		}

		if( Input & I_RIGHT )
		{
			IndexComportement++ ;
			if( IndexComportement>=ptrmcomp->NbComps )
				IndexComportement = 0 ;
		}

		if( Input & I_LEFT )
		{
			IndexComportement-- ;
			if( IndexComportement<0 )
				IndexComportement = ptrmcomp->NbComps-1 ;
		}

		if( IndexComportement != oldindex )
		{
			Comportement = (U8)ptrmcomp->Comps[IndexComportement] ;

			DrawComportement( oldindex, ptrmcomp->Comps[oldindex], ptrobj->Obj.Beta, TRUE ) ;

			oldindex = IndexComportement ;

			InitMove( &RealRot, -1024 ) ;

			while( Input & I_JOY )
			{
				ManageTime() ;
				MyGetInput() ;
				DrawComportement( IndexComportement, Comportement, -1, TRUE ) ;
			}
		}
		DrawComportement( IndexComportement, Comportement, -1, TRUE ) ;
	}

	InitWaitNoInput( I_COMPORTEMENT|I_RETURN|I_MENUS ) ;// fait dans la boucle du jeu
	InitWaitNoKey() ;

	if( oldcomp==C_BUGGY AND Comportement!=C_BUGGY )
	{
		ptrobj->Move = MOVE_MANUAL ;
		LeaveBuggy( (U8)Comportement ) ;
	}

	SetComportement( Comportement ) ;

	Island = memoisland ;
	InitDial( START_FILE_ISLAND+Island ) ;

	PtrInit3DView() ;

	RestoreCamera() ;
	if( CubeMode==CUBE_INTERIEUR )	CameraCenter( 0 ) ;

	RestoreScreen() ;

	RestoreTimer() ;

	if( oldcomp!=Comportement )
	{
		// repasse en anim rien
		// peut-etre faut-il le faire mme si olcomp==Comportement
		InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
	}
	else	if( flagproto )
	{
		// restore marche proto en ANIM_ALL_THEN pour que le
		// joueur ait le temps de se reprendre
		InitAnim( GEN_ANIM_MARCHE, ANIM_REPEAT, NUM_PERSO ) ;
		ObjectSetFrame( &(ptrobj->Obj), 5 ) ;
		TimerProto = TimerRefHR+TEMPO_PROTO_AUTO ;// 1/2 s
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	IncrustComportement( U8 comportement )
{
	S32	n ;
	T_INCRUST_DISP	*ptrdisp = ListIncrustDisp ;

	// clear old SYS_TEXT
	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		if( ptrdisp->Type==INCRUST_SYS_TEXT )
		{
			ptrdisp->Num = -1 ;	// gicle !
		}
	}

	InitIncrustDisp( INCRUST_SYS_TEXT,
			 (S16)comportement,
			 10, ClipWindowYMax-40,
			 LBAWHITE, 0, 1 ) ;
}
