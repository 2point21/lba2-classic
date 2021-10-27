/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		 ÛÛÛÛ¿  ÛÛÛÛÛ¿          ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿
		ÛÛÚÄÛÛ¿ ÛÛÚÄÛÛ¿         ÛÛÚÄÄÄÙ ÛÛ³ ÛÛ³ ÀÄÛÛÚÄÙ
		ÀÄÙÛÛÚÙ ÛÛ³ ÛÛ³         ÛÛÛÛÛ¿  ÀÛÛÛÛÚÙ   ÛÛ³
		ÛÛ¿ÀÛÛ¿ ÛÛ³ ÛÛ³         ÛÛÚÄÄÙ  ÛÛÚÄÛÛ¿   ÛÛ³
		ÀÛÛÛÛÚÙ ÛÛÛÛÛÚÙ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ³ ÛÛ³   ÛÛ³
		 ÀÄÄÄÙ  ÀÄÄÄÄÙ  ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ   ÀÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

#include "c_extern.h"

extern	S32	Nxw, Nyw, Nzw ;
extern	S32	SaveNxw, SaveNzw ;
extern	S32	OldX, OldY, OldZ ;

extern	S32	NbTriObj ;	// OBJECT.CPP

extern	T_OBJET	*APtObj ;

extern	U8	AnimNumObj ;
extern	S32	YMap ;
extern	S32	Col1 ;

S32	FlagHeroOutX	= FALSE ;	// Twinsen sort du cube en X
S32	FlagHeroOutZ	= FALSE ;	// Twinsen sort du cube en Z

T_DECORS	*PtrDecorsCol = 0 ;	// Objet du decors en collision
S16		NumDecorsCol = -1 ;	// avec l'objet courant

U8	*PtrNuances ;	// Nuances gouraud

S32	LastIsland = -1 ;

#ifdef	LBA_EDITOR
char	*GenAnimSkate ;	/* identificateur anim en cours */
#else
U16	GenAnimSkate ;	/* identificateur anim en cours */
#endif

#define	SKATING_TIME_OUT	(25*20)	// 1/2s
S32	TimerSkating	;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
			    Ûßßßß ÛßßßÛ ÛÜ ÜÛ Ûßßßß
			    ÛÛ ßÛ ÛÛßßÛ ÛÛß Û ÛÛßß
			    ßßßßß ßß  ß ßß  ß ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

// Initialisation des buffers n‚cessaires … la 3D Ext‚rieur: a appeler une
// seule fois en d‚but de programme (PERSO.C)
void	Init3DExt( )
{
	InitExternalView() ;			// mallocs

/*#ifdef	LBA_EDITOR
	ScreenAux		= (U8 *)Malloc( 640*480+RECOVER_AREA ) ;
	if( !ScreenAux )	TheEnd( NOT_ENOUGH_MEM, "ScreenAux (Init3DExt)" ) ;
#endif
*/
	// Faire pointer sur un autre buffer
	ListTriExt		= (T_LIST_TRI *)Malloc( sizeof(T_LIST_TRI) * MAX_OBJ_DECORS );
	if( !ListTriExt )	TheEnd( NOT_ENOUGH_MEM, "ListTriExt (Init3DExt)" ) ;

#ifdef	LBA_EDITOR
	strcpy( IslandName, GetRessName("citadele") ) ;
#else
	strcpy( IslandName, PATH_RESSOURCE"citadele" ) ;
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Restaure buffers allou‚s pour la 3D Ext‚rieur: appel‚ en quittant le
// programme (PERSO.C)
void	Clear3dExt()
{
	// libere memoire
	ClearExternalView() ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Positionne la cam‚ra pour la 3d Ext‚rieure
void	Init3DExtView( void )
{
	SetProjection( 320, 240, CLIP_NEAR, ChampX, ChampZ ) ;
	SetFollowCamera(	VueOffsetX, VueOffsetY, VueOffsetZ,
				AlphaCam, BetaCam, GammaCam,
				VueDistance ) ;

	SetLightVector( AlphaLight, BetaLight, 0 ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// a appeler a chaque fois que l'on change de cube en 3d exterieur
// pour r‚initialiser la cam‚ra et les pointeurs de fonctions
void	Init3DExtGame()
{
	Init3DExtView() ;

	// Initialise les pointeurs de fonctions
#ifndef	COMPILATOR
	PtrReajustPos 		= ReajustPosExt 	;
	PtrWorldColBrick	= WorldColBrickExt 	;
	PtrWorldColBrickVisible	= WorldColBrickVisibleExt ;
	PtrProjectShadow 	= ProjectShadowExt 	;
	PtrReajustPosExtra 	= ReajustPosExtraExt 	;
#ifndef	LBA_EDITOR
	PtrFullWorldColBrick 	= FullWorldColBrickExt 	;
#endif
#endif

	PtrGetShadow  		= GetShadowExt 		;
	PtrDoAnim       	= DoAnimExt 		;
	PtrInitGrille		= InitGrilleExt 	;
	PtrInit3DView  	 	= Init3DExtView 	;
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
			       ÛÜ ÜÛ ÛßßßÛ ÛßßßÛ
			       ÛÛß Û ÛÛßßÛ ÛÛßßß
			       ßß  ß ßß  ß ßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

// Charge grille lors d'un changement de cube
#ifdef	LBA_EDITOR
void	InitGrilleExt( char *defpathname )
{
	S32	numskysea ;

	InitDefFile( defpathname ) ;
	CubeMode = DefFileBufferReadValue( "Mode" )	;

	if( CubeMode != CUBE_EXTERIEUR )
	{
		MessageF( TRUE, "Erreur: cube %s non exterieur !", defpathname ) ;
		return ;
	}

	if( NumCube!=NUM_CUBE_PHANTOM )
	{
		strcpy( IslandName, DefFileBufferReadStringDefault( "Island", "citadel" ) ) ;
		if( strchr(IslandName,';') )
			*(char *)strchr(IslandName,';') = '\0' ;
	}
	else	Island = PhantomIsland ;

	numskysea = RESS_SKYSEA0 + Island ;

	// GROSSE Rustine pour charger la citadelle quand il fait beau ou
	// quand il pleut
	if( Island==0 AND TEMPETE_FINIE
	AND strstr(IslandName,"citadel") )
	{
		strcpy( IslandName, GetRessName("citabau") ) ;
		LastIsland = 1 ;
		numskysea = RESS_SKYSEA00 ;
	}
	else
	{
		if( Island==5 AND ListVarGame[FLAG_CELEBRATION].Value )
		{
			strcpy( IslandName, GetRessName("celebra2") ) ;
			LastIsland = 1 ;
		}
	}

	if( NumCube!=NUM_CUBE_PHANTOM )
	{
		CurrentCubeX = DefFileBufferReadValue( "CubeX" )	;
		CurrentCubeY = DefFileBufferReadValue( "CubeY" )	;

		if( LastIsland!=Island )
		{
			Load_HQR( RESS_HQR_NAME, SkySeaTexture, numskysea ) ;
			LoadIsland() ;

			LastIsland = Island ;
		}

		LoadCube( CurrentCubeX,	CurrentCubeY ) ;
		FixeObjetsDecorsInvisibles() ;
	}
	else
	{
		CurrentCubeX += PhantomX	;
		CurrentCubeY += PhantomY	;

		InitSeaCube() ;
	}
}
#else

char	*IleLst[] = {
	"citadel",
	"sendell",
	"desert",
	"emeraude",
	"otringal",
	"celebrat",
	"platform",
	"mosquibe",
	"knartas",
	"ilotcx",
	"ascence",
	"souscelb",
	} ;

void	InitGrilleExt( U32 numcube )
{
	S32	numskysea ;

	// Pour virer Warning
	numcube = numcube ;

	if( NumCube==NUM_CUBE_PHANTOM )
	{
		Island = PhantomIsland ;
	}

	// GROSSE Rustine pour charger la citadelle quand il fait beau ou
	// quand il pleut
	switch( Island )
	{
		case 0:
			if( TEMPETE_FINIE )
			{
				if( !strstr(IslandName,"citabeau") )
					LastIsland = 1 ;	// pour que le chargement s'effectue

				strcpy( IslandName, PATH_RESSOURCE ) ;
				strcat( IslandName, "citabau" ) ;
				numskysea = RESS_SKYSEA00 ;
			}
			else
			{
				if( !strstr(IslandName,"citadel") )
					LastIsland = 1 ;	// pour que le chargement s'effectue

				strcpy( IslandName, PATH_RESSOURCE ) ;
				strcat( IslandName, "citadel" ) ;

				numskysea = RESS_SKYSEA0 ;
			}
			break ;

		case 5:
			if( ListVarGame[FLAG_CELEBRATION] )
			{
				if( !strstr(IslandName,"celebra2") )
					LastIsland = 1 ;	// pour que le chargement s'effectue

				strcpy( IslandName, PATH_RESSOURCE ) ;
				strcat( IslandName, "celebra2" ) ;
			}
			else
			{
				if( !strstr(IslandName,"celebrat") )
					LastIsland = 1 ;	// pour que le chargement s'effectue

				strcpy( IslandName, PATH_RESSOURCE ) ;
				strcat( IslandName, "celebrat" ) ;
			}
			numskysea = RESS_SKYSEA5 ;
			break ;

		default:
//			DefFileBufferInit( PATH_RESSOURCE"ile.lst", Screen, 640*480 ) ;

			strcpy( IslandName, PATH_RESSOURCE ) ;
//			strcat( IslandName, DefFileBufferReadStringDefault( Itoa( Island ), "citadel" ) ) ;
			strcat( IslandName, IleLst[Island] ) ;

			if( strchr(IslandName,';') )
				*(char *)strchr(IslandName,';') = '\0' ;

			numskysea = RESS_SKYSEA0 + Island ;
	}

	if( NumCube!=NUM_CUBE_PHANTOM )
	{
		if( LastIsland!=Island )
		{
			Load_HQR( RESS_HQR_NAME, SkySeaTexture, numskysea ) ;

			LoadIsland() ;
			LastIsland = Island ;
		}

		ManageSystem() ;// for streaming

		LoadCube( CurrentCubeX,	CurrentCubeY ) ;

		ManageSystem() ;// for streaming

		FixeObjetsDecorsInvisibles() ;
	}
	else
	{
#ifdef	DEMO
		// Grosse rustine pour ne pas voir les plaques d'‚gouts
		ListVarGame[164] = 1 ;
		ListVarGame[165] = 1 ;
#endif
		InitSeaCube() ;
	}
}
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Affiche la grille complŠte … la position actuelle de la cam‚ra
void	AffGrilleExt()
{
	SetFollowCamera(	VueOffsetX, VueOffsetY, VueOffsetZ,
				AlphaCam, BetaCam, GammaCam,
				VueDistance ) ;

	SetLightVector( AlphaLight, BetaLight, 0 ) ;

	Switch_Fillers( FILL_POLY_FOG_ZBUFFER );

	ClearAniPoly() ;

	ClsTerrainZBuf() ;

	ManageSystem() ;// for streaming

#ifdef	LBA_EDITOR
	if( FlagInfos&INFO_HORIZON )
#else
	if( FlagDrawHorizon )
#endif
	{
		DrawHorizon2ZBuf() ;
	}

	ManageSystem() ;// for streaming

	AffichageTerrainZBuf() ;

	ManageSystem() ;// for streaming

	DrawSkySeaZBuf() ;

	ManageSystem() ;// for streaming

	FiltreAnimatedPolys() ;

	Switch_Fillers( FILL_POLY_TEXTURES );

	CopyScreen( Log, Screen ) ;

#ifdef	LBA_EDITOR
	if( FlagInfos&INFO_CAMERA )
	{
		DrawCameraInfos() ;
	}
#endif
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
			 Ûßßßß ÛßßßÛ ÛßßßÛ ÛßßßÛ Û   Û
			 ÛÛ ßÛ ÛÛßÛß ÛÛßßÛ ÛÛßßß ÛÛßßÛ
			 ßßßßß ßß  ß ßß  ß ßß    ßß  ß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
// Calcule le facteur de zoom d'un sprite pour l'afficher: a revoir si c'est
// bon ! En int‚rieur, on le fixe a la valeur par d‚faut (afficher en taille
// normale). D'ailleurs, cette fonction devrait se trouver dans INTEXT.C avec
// pourquoi pas un pointeur sur fonction
void	CalculeScaleFactorSprite( S32 x, S32 y, S32 z, S32 scaleorg )
{
	if( CubeMode==CUBE_INTERIEUR AND scaleorg==-1 )
	{
		ScaleFactorSprite = DEF_SCALE_FACTOR ;
	}
	else
	{
		S32	yp0 ;

		LongWorldRotatePoint( x, y, z ) ;
		if( !LongProjectPoint( X0, Y0+1000, Z0 ) )	return ;

		yp0 = Yp ;

		if( !LongProjectPoint( X0, Y0, Z0 ) )	return ;

		if( scaleorg<=0 )	scaleorg = 70 ;

		ScaleFactorSprite = ((Yp-yp0)*DEF_SCALE_FACTOR) / scaleorg ;
	}
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		   ÛßßßÛ ÛßßÛ      Û Ûßßßß Ûßßßß ßßÛßß ÛÛßßß
		   ÛÛ  Û ÛÛßßÛ ÜÜ  Û ÛÛßß  ÛÛ      ÛÛ  ßßßßÛ
		   ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß   ßß  ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

// Calcule coordonnees de l'ombre
void	GetShadowExt( S32 xw, S32 yw, S32 zw )
{
	S32	cj = -1 ;// tient compte de la profondeur de l'eau

	if( yw>=0 )// l'objet est au-dessus de l'eau (enfin presque car l'eau n'est pas tjs en 0!)
	{
		cj = 0 ;// ne tient pas compte de la profondeur de l'eau
	}

	ShadowX = xw ;
	ShadowY = CalculAltitudeObjet(xw,zw,cj) ;
	ShadowZ = zw ;

// peut-etre faut-il rajouter ces deux lignes pour etre plus exact dans le tri
	if( ShadowY!=yw )
	{
		ShadowY = AdjustShadowDecors( xw, zw, ShadowY, yw ) ;
		ShadowY = AdjustShadowObjects( xw, zw, ShadowY, yw ) ;
	}
	else
	{
		ShadowTestObject = -1 ;
	}
}

#ifndef	COMPILATOR
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Pour recentrage de la camera: teste si la camera se trouve … l'int‚rieur
// d'un objet sc‚narique. si oui, on lŠve la que.. euh.. la cam‚ra
// (voir 3DEXT\MAPTOOLS.CPP --> SearchCameraPos() )

void	TestCameraPosObjets( void )
{
	S32	xmin, ymin, zmin ;
	S32	xmax, ymax, zmax ;
	S32	i ;
	T_OBJET	*ptrobj = ListObjet ;
	T_OBJ_3D *pt3d ;

	if( CameraZone OR CinemaMode )	return ;

	for ( i=0; i<NbObjets; i++, ptrobj++ )
	{
		pt3d = &ptrobj->Obj ;

		if( (pt3d->Body.Num!=-1)
		AND !(ptrobj->Flags&INVISIBLE) )
		{
			xmin = pt3d->X + ptrobj->XMin ;
			ymin = pt3d->Y + ptrobj->YMin ;
			zmin = pt3d->Z + ptrobj->ZMin ;
			xmax = pt3d->X + ptrobj->XMax ;
			ymax = pt3d->Y + ptrobj->YMax ;
			zmax = pt3d->Z + ptrobj->ZMax ;

			if( CamPosX>=xmin AND CamPosX<=xmax
			AND CamPosY>=ymin AND CamPosY<=ymax
			AND CamPosZ>=zmin AND CamPosZ<=zmax )
			{
				// cam dans objet
				CamPosY = ymax + 200 ;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Detecte collision avec le decors en un point (terrain + objets du decors)
U8	WorldColBrickExt( S32 xw, S32 yw, S32 zw )
{
	U32	i ;
	S32	y = CalculAltitudeObjet( xw, zw, APtObj->CodeJeu>>4 ) ;

	PtrDecorsCol = ListDecors ;

	// decors
	for ( i=0; i<NbObjDecors; i++, PtrDecorsCol++ )
	{
		if( TestZVDecors( xw, yw, zw, PtrDecorsCol ) )
		{
			// verifie qu'il n'y ait pas de terrain au-dessus
			// de l'objet
			if( y<=PtrDecorsCol->YMax )
			{
				return 1 ;
			}
		}
	}

	PtrDecorsCol = NULL ;

	// terrain
	if( y > yw )
	{
		if( GiveTerrainCol( xw, zw ) ) return 1 ;
		else			       return 3 ;
	}

	return 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
U8	WorldColBrickVisibleExt(S32 oldx, S32 oldy, S32 oldz,
				S32 xw, S32 yw, S32 zw )
{
	U8 col = WorldColBrickExt( xw, yw, zw ) ;

	if( col )	return( col ) ;

	if( abs(xw-oldx)>=SIZE_BRICK_XZ
	OR  abs(yw-oldy)>=SIZE_BRICK_Y
	OR  abs(zw-oldz)>=SIZE_BRICK_XZ )
	{
		return( WorldColBrickVisible( 	oldx, oldy, oldz,
					oldx+(xw-oldx)/2,
					oldy+(yw-oldy)/2,
					oldz+(zw-oldz)/2 ) ) ;
	}

	return( 0 ) ;// Ok, cette fois, on est s–r !:)
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	ReajustPosExtraExt( T_EXTRA *ptrextra, U8 col, S16 *ptrymin )
{
	S32	y ;

	if( !col )	return ;

	y = CalculAltitudeObjet( ptrextra->PosX, ptrextra->PosZ, -1 ) - *ptrymin ;

	if( y>ptrextra->PosY )	ptrextra->PosY = y ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Detecte collision des avec le decors entre deux positions
// (uniquement en version jeu)
U8	FullWorldColBrickExt(	S32 oldx, S32 oldy, S32 oldz,
				S32 newx, S32 newy, S32 newz )
{
	S32	x0, y0, z0 ;

	if( WorldColBrickExt( newx,newy,newz )==1 )	return 1 ;

	x0 = (newx+oldx)/2 ;
	y0 = (newy+oldy)/2 ;
	z0 = (newz+oldz)/2 ;

	if( WorldColBrickExt( x0,y0,z0 )==1 )
		return 1 ;

	if( WorldColBrickExt( (newx+x0)/2, (newy+y0)/2, (newz+z0)/2 )==1 )
		return 1 ;

	if( WorldColBrickExt( (x0+oldx)/2, (y0+oldy)/2, (z0+oldz)/2 )==1 )
		return 1 ;

	return 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// D‚tecte collision avec un objet du d‚cors en tenant compte de la ZV
// de l'objet sc‚narique test‚.
// La ZV pass‚e de l'objet sc‚narique est pr‚calcul‚e en absolu

// Version ZVs absolues pr‚calcul‚es des Decors
S32	TestZVDecorsZV( T_DECORS *objet,
			S32 xpmin, S32 ypmin, S32 zpmin,
			S32 xpmax, S32 ypmax, S32 zpmax )
{
	if( objet->Body & DEC_INVISIBLE )	return FALSE ;

	if( objet->XMin > xpmax )	return	FALSE ;
	if( objet->XMax < xpmin )	return	FALSE ;

	if( objet->ZMin > zpmax )	return	FALSE ;
	if( objet->ZMax < zpmin )	return	FALSE ;

	if( objet->YMin > ypmax )	return	FALSE ;
	if( objet->YMax < ypmin-1 )	return	FALSE ;

	return TRUE ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// D‚tecte collision de l'objet sc‚narique courant avec les objets du d‚cors
U8	WorldColBrickDecors( S32 xpmin, S32 ypmin, S32 zpmin,
			     S32 xpmax, S32 ypmax, S32 zpmax )
{
	U16	i ;

	PtrDecorsCol = ListDecors ;

	// decors
	for ( i=0; i<NbObjDecors; i++, PtrDecorsCol++ )
	{
		if( i==NumDecorsCol )	continue ;

		if( TestZVDecorsZV( PtrDecorsCol,
				    xpmin, ypmin, zpmin,
				    xpmax, ypmax, zpmax ) )
		{
			NumDecorsCol = i ;
			return 1 ;
		}
	}

	PtrDecorsCol = NULL ;	// No Collision

	return 0 ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
// Version ZVs absolues pr‚calcul‚es
S32	TestZVDecorsXZFullY( S32 xmin, S32 zmin, S32 xmax, S32 zmax, S32 y0, S32 y1, T_DECORS *objet )
{
	if( objet->Body & DEC_INVISIBLE )	return FALSE ;

	if( objet->XMin > xmax )	return	FALSE ;
	if( objet->XMax < xmin )	return	FALSE ;

	if( objet->ZMin > zmax )	return	FALSE ;
	if( objet->ZMax < zmin )	return	FALSE ;

	if( objet->YMin > y1 )		return	FALSE ;
	else if( objet->YMax > y1 )	return FALSE ;

	if( objet->YMax < y0 )		return	FALSE ;

	return TRUE ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
S32	ReajustPosDecors( S32 xmin, S32 zmin,
			  S32 xmax, S32 zmax,
			  S32 y0, S32 y1 )
{
	U16	i ;

	PtrDecorsCol = ListDecors ;

	// decors
	for ( i=0; i<NbObjDecors; i++, PtrDecorsCol++ )
	{
		if( TestZVDecorsXZFullY( xmin, zmin, xmax, zmax,
					 y0, y1, PtrDecorsCol ) )
		{
			y0 = PtrDecorsCol->YMax ;
			NumDecorsCol = i ;
		}
	}

	if( NumDecorsCol==-1 )	PtrDecorsCol = NULL ;	// No Collision
	else			PtrDecorsCol = &ListDecors[NumDecorsCol] ;

	return y0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// D‚tecte collision de l'objet sc‚narique courant avec le terrain
U8	WorldColBrickFullExt( S32 xw, S32 yw, S32 zw )
{
	// terrain
	if( GiveTerrainCol( xw, zw ) ) return 1 ;

	if( CalculAltitudeObjet( xw, zw, APtObj->CodeJeu>>4 ) >= yw )	return 3 ;

	return 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Initialise la glissage dans les pench‚s interdits
void	InitSkating( void )
{
	S32	angle	;
	S32	beta	;

	if( APtObj->WorkFlags&SKATING )	return ;

	beta = GetSkateBeta( OldX, OldZ ) ;

	angle = (APtObj->Obj.Beta-beta) & (MAX_ANGLE-1) ;
	if( angle>2048 )	angle = 4096-angle ;

	if( angle > 1024 )
	{
		GenAnimSkate = GEN_ANIM_SKATEG ;
		beta = (beta+2048)&4095 ;
	}
	else
	{
		GenAnimSkate = GEN_ANIM_SKATE ;
	}

	// pass‚e en anim_all_then ici pour forcee l'anim
	InitAnim( GenAnimSkate, ANIM_ALL_THEN, NUM_PERSO ) ;
	APtObj->WorkFlags |= SKATING ;
	StartYFalling = 0 ;
	TimerSkating = TimerRefHR+SKATING_TIME_OUT ;

	InitBoundAngleMove( &APtObj->BoundAngle, APtObj->SRot, APtObj->Obj.Beta, beta ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	AnnuleSkating( void )
{
	InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
	APtObj->WorkFlags &= ~(SKATING) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// R‚ajuste hauteur (Nyw) de l'objet sc‚narique courant en fonction du terrain
// et des objets du d‚cors

void	ReajustPosExt( U8 col )
{
	S32	coly ;

	if( !col )	return ;

	coly = CalculAltitudeObjet( Nxw, Nzw, APtObj->CodeJeu>>4 ) ;

	if( PtrDecorsCol )
	{
		if( coly>PtrDecorsCol->YMax )
		{
			Nyw = coly ;
			PtrDecorsCol = NULL ;
			APtObj->CarryBy = -1 ;// attention si port‚ par obj
					      // scenarique ?
		}
		else
		{
			Nyw = PtrDecorsCol->YMax + 1 ;
		}
	}
	else
	{
		if( Nyw>=coly ) // on se trouve au-dessus de la pente du polygone
		{
			if( APtObj->CarryBy==-1 )
			{
				S32	ny ;

/*				if( !(APtObj->WorkFlags & ANIM_MASTER_GRAVITY) )
				{
					Nyw = coly ;

					if( AnimNumObj==NUM_PERSO
					AND GiveTerrainCol(OldX,OldZ) )
					{
						InitSkating() ;
					}
				}
				else
				{
					if( AnimNumObj==NUM_PERSO
					AND GiveTerrainCol(Nxw,Nzw) )
					{
						InitSkating() ;
					}
				}
*/
				// Blindage buggy
				if( (APtObj->Move==MOVE_BUGGY_MANUAL OR
				     APtObj->Move==MOVE_BUGGY)
				AND GiveTerrainCol(Nxw,Nzw) )
				{
					InitSkating() ;
				}

				// V‚rifie qu'il n'y ait pas d'objet scenarique ou du decors
				// entre le sol et nous
				FirstLoop = TRUE ;// Magouille pour bien passer par les tests
				ny = AdjustShadowObjects( Nxw, Nzw, coly, Nyw ) ;
				FirstLoop = FALSE ;

				Nyw = ReajustPosDecors( Nxw+APtObj->XMin, Nzw+APtObj->ZMin,
							Nxw+APtObj->XMax, Nzw+APtObj->ZMax,
							coly, Nyw ) ;

				Nyw = __max(Nyw,ny) ;

				if( ShadowTestObject!=AnimNumObj )
				{
					APtObj->CarryBy = (S16)ShadowTestObject ;
				}
				else if( NumDecorsCol!=-1 )
				{
//					APtObj->CarryBy = (S16)(NumDecorsCol|CARRY_BY_DECORS) ;
					Nxw = OldX ;
					Nyw = OldY ;
					Nzw = OldZ ;
				}
				else
				{
					Nyw = coly ;
				}
			}
		}
		else
		{
			if( AnimNumObj==NUM_PERSO
			AND APtObj->CarryBy==-1 )
			{
				if( APtObj->WorkFlags & ANIM_MASTER_GRAVITY )
				{
					if( GiveTerrainCol(Nxw,Nzw) )
					{
						InitSkating() ;
					}
				}
				else
				{
					if( GiveTerrainCol(OldX,OldZ) )
					{
						InitSkating() ;
					}
				}
			}

			Nyw = coly ;
		}

		// Teste si Twinsen est sur une pente interdite
/*		if( AnimNumObj==NUM_PERSO
		AND GiveTerrainCol(Nxw,Nzw) )
		{
			if( (APtObj->WorkFlags & ANIM_MASTER_GRAVITY) )
			{
//				InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
			}
			else
			{
				InitSkating() ;
			}
		}
*/
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Recale Twinsen contre un objet du d‚cors en X

#define	RecaleXExt(ptrd)				\
{							\
	if( OldX>Nxw )					\
	{						\
		Nxw = ptrd->XMax - APtObj->XMin + 1 ;	\
	}						\
	else						\
	{						\
		Nxw = ptrd->XMin - APtObj->XMax - 1 ;	\
	}						\
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Recale Twinsen contre un objet du d‚cors en Z

#define	RecaleZExt(ptrd)				\
{							\
	if( OldZ>Nzw )					\
	{						\
		Nzw = ptrd->ZMax - APtObj->ZMin + 1 ;	\
	}						\
	else						\
	{						\
		Nzw = ptrd->ZMin - APtObj->ZMax - 1 ;	\
	}						\
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Reajuste l'objet sc‚narique courant contre les objets du d‚cors
// probablement optimisable !

S32    DoCornerReajustDecors( )
{
	S32	xpmin, ypmin, zpmin, xpmax, ypmax, zpmax ;

	if( Nxw < 0 )	return 0 ;
	if( Nzw < 0 )	return 0 ;
	if( Nxw > 64*SIZE_BRICK_XZ-1 )	return 0 ;
	if( Nzw > 64*SIZE_BRICK_XZ-1 )	return 0 ;

	xpmin = Nxw + APtObj->XMin ;
	ypmin = Nyw + APtObj->YMin ;
	zpmin = Nzw + APtObj->ZMin ;
	xpmax = Nxw + APtObj->XMax ;
	ypmax = Nyw + APtObj->YMax ;
	zpmax = Nzw + APtObj->ZMax ;

	// OPTIMISATION: Calculer ici la zv de l'objet pour optimiser les
	// appels a TestZVDecorsZV

	if( WorldColBrickDecors( xpmin, ypmin, zpmin, xpmax, ypmax, zpmax ) == 1 )
	{
		// On est en collision avec un objet du decors
		if( OldY >= PtrDecorsCol->YMax-SIZE_BRICK_Y/2 )
		{
			Nyw = CalculAltitudeObjet(Nxw,Nzw,APtObj->CodeJeu>>4) ;

			if( Nyw<=PtrDecorsCol->YMax )
			{
				// on est au dessus de l'objet
				Nyw = PtrDecorsCol->YMax+1 ;
				APtObj->CarryBy = (S16)(NumDecorsCol|CARRY_BY_DECORS) ;
			}
		}
		else if( !TestZVDecorsZV( PtrDecorsCol,
					 xpmin, OldY+APtObj->YMin, zpmin,
					 xpmax, OldY+APtObj->YMax, zpmax ) )

		{
			// on rentre dans l'objet par dessous
			Nyw = PtrDecorsCol->YMin - APtObj->YMax -1 ;
			if( Nyw<OldY-DEMI_BRICK_Y )	Nyw = OldY-DEMI_BRICK_Y ;
		}
		else
		{
			Col1 = TRUE ;

			// On est en collision avec un objet du decors
			if( TestZVDecorsZV( PtrDecorsCol,
					    xpmin, ypmin+1, OldZ+APtObj->ZMin,
					    xpmax, ypmax, OldZ+APtObj->ZMax ) )
			{
				if( !TestZVDecorsZV( PtrDecorsCol,
						     OldX+APtObj->XMin, ypmin, zpmin,
						     OldX+APtObj->XMax, ypmax, zpmax ) )
				{
					RecaleXExt( PtrDecorsCol ) ;
				}
				else
				{
					if( !TestZVDecorsZV( PtrDecorsCol,
							     OldX+APtObj->XMin, ypmin, OldZ+APtObj->ZMin,
							     OldX+APtObj->XMax, ypmax, OldZ+APtObj->ZMax ) )
					{
						RecaleXExt( PtrDecorsCol ) ;
						RecaleZExt( PtrDecorsCol ) ;
					}
				}
			}
			else
			{
				if( TestZVDecorsZV( PtrDecorsCol,
						    OldX+APtObj->XMin, ypmin, zpmin,
						    OldX+APtObj->XMax, ypmax, zpmax ) )
				{
					RecaleZExt( PtrDecorsCol ) ;
				}
			}
		}

		return TRUE ;
	}

	return FALSE ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// reajust pos objet en 2D (x,z) sur le terrain uniquement au point chaud

#define	SQR_2	1518500250	/* sqr(2)<<30 */

S32    DoCornerReajustExtPt( void )
{
	U32	flagspecial = FALSE ;
	U32	algo = 1 ;	// ORTHO
	S32	t ;
	S32	modif = FALSE ;

	// teste des decors en meme temps ?

	if( GiveTerrainCol( Nxw, Nzw ) == 1 )
	{
		// s'il y a un objet du decors entre le sol et nous,
		// on ne tient pas compte de la collision

		S32	y = CalculAltitudeObjet( Nxw, Nzw, APtObj->CodeJeu>>4 ) ;

		if( ReajustPosDecors( Nxw+APtObj->XMin, Nzw+APtObj->ZMin,
				      Nxw+APtObj->XMax, Nzw+APtObj->ZMax,
				      y, Nyw )!=y )
		{
			return FALSE ;
		}

		// On est en collision avec le terrain
		// determine quel type de collision

		if( Nxw/512 == OldX/512  AND  Nzw/512 == OldZ/512 )
		{
			algo = 2 ;	// meme carre, col diagonale
		}
		else
		{
			// carres differents

			t = MapPolyGround[IndexMapPolyTested].Col + MapPolyGround[IndexMapPolyTested+1].Col ;

			if( (Nxw/512 == OldX/512) OR (Nzw/512 == OldZ/512) )
			{
				if( t == 2 )
				{
					// Full Carre Collision
					if( GiveTerrainCol( Nxw, OldZ ) == 1
					AND GiveTerrainCol( OldX, Nzw ) == 1 )
					{
						algo = 2 ;
					}
				}
				else	// le carre dest demi col
				{
					S32	numtriangle = GiveTriangle( Nxw, Nzw ) ;

					if( MapPolyGround[IndexMapPolyTested].Sens==0 )
					{
						if( numtriangle==0 )	// poly de gauche
						{
							if( OldX>Nxw AND OldZ<Nzw )
							{
								algo = 2 ;	// diagonale
							}
						}
						else	// poly de droite
						{
							if( OldX<Nxw AND OldZ<Nzw )
							{
								algo = 2 ;	// diagonale
							}
						}
					}
					else
					{
						if( numtriangle==0 )	// poly de gauche
						{
							if( OldX>Nxw AND OldZ>Nzw )
							{
								algo = 2 ;	// diagonale
							}
						}
						else	// poly de droite
						{
							if( OldX<Nxw AND OldZ>Nzw )
							{
								algo = 2 ;	// diagonale
							}
						}
					}
				}
			}
		}

		if( algo==1 )	// orthogonale
		{
			if( GiveTerrainCol( Nxw, OldZ ) == 1 )
			{
				if( GiveTerrainCol( OldX, Nzw ) != 1 )
				{
					// Recale Twinsen contre le mur en X
					if( OldX>Nxw )
					{
						SaveNxw = (OldX/SIZE_BRICK_XZ)*SIZE_BRICK_XZ + 1 ;
					}
					else
					{
						SaveNxw = (OldX/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - 1 ;
					}
				}
				else
				{
					if( GiveTerrainCol( OldX, OldZ ) != 1 )
					{
						// Recale Twinsen contre le mur en X
						if( OldX>Nxw )
						{
							SaveNxw = (OldX/SIZE_BRICK_XZ)*SIZE_BRICK_XZ + 1 ;
						}
						else
						{
							SaveNxw = (OldX/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - 1 ;
						}

						// Recale Twinsen contre le mur en Z
						if( OldZ>Nzw )
						{
							SaveNzw = (OldZ/SIZE_BRICK_XZ)*SIZE_BRICK_XZ + 1 ;
						}
						else
						{
							SaveNzw = (OldZ/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - 1 ;
						}
					}
					else
					{
#ifdef	LBA_EDITOR
						if( OptionsDebug&OPT_COL_ANGLE )
						{
							MessageF( FALSE, "Old Pos dans Col 1: %s", APtObj->Name ) ;
						}
#endif

						SaveNxw = OldX	;
						SaveNzw = OldZ	;
					}
				}
			}
			else
			{
				// Recale Twinsen contre le mur en Z
				if( OldZ>Nzw )
				{
					SaveNzw = (OldZ/SIZE_BRICK_XZ)*SIZE_BRICK_XZ + 1 ;
				}
				else
				{
					SaveNzw = (OldZ/SIZE_BRICK_XZ+1)*SIZE_BRICK_XZ - 1 ;
				}
			}
		}
		else
		{
			// algo diagonale

			S32	angle, d, tx, tz ;

			angle = GetAngle2D( OldX, OldZ, Nxw, Nzw ) ;

			d = Distance2D( OldX, OldZ, Nxw, Nzw ) ;

			if( MapPolyGround[IndexMapPolyTested].Sens==0 )
			{
				// sens 0
				tx = tz = MULDIV64( d, (S32)(SinTab[ (angle - 512 + 1024)&4095 ])<<16, SQR_2 ) ;
			}
			else
			{
				// sens 1
				tz = MULDIV64( d, (S32)(SinTab[ (angle + 512 + 1024)&4095 ])<<16, SQR_2 ) ;
				tx = -tz	;
			}

			SaveNxw = OldX + tx ;
			SaveNzw = OldZ + tz ;

			if( GiveTerrainCol( SaveNxw, SaveNzw ) == 1 )
			{
				// pos pas bonne, essaie l'autre point
				tx = -tx	;
				tz = -tz	;

				SaveNxw = Nxw + tx ;
				SaveNzw = Nzw + tz ;

				if( GiveTerrainCol( SaveNxw, SaveNzw ) == 1 )
				{
					// pos pas bonne, tant pis, on restore l'ancienne
					SaveNxw = OldX	;
					SaveNzw = OldZ	;
				}
			}
		}
	}

//fincorner:
	if( SaveNxw!=Nxw
	OR  SaveNzw!=Nzw )
	{
		modif = TRUE ;
	}

	Nxw = SaveNxw ;
	Nzw = SaveNzw ;

	return modif ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
S32    GetSkateBeta( S32 nx, S32 nz )
{
	S32	numtriangle ;
	S32	beta ;
	S32	xi, zi ;
	S32	y0, y1, y2, y3 ;

	xi = nx>>9 ;	// div512
	zi = nz>>9 ;	// div512

	zi *= 65 ;

	y0 = MapSommetY[zi+xi] ;
	y1 = MapSommetY[(zi+65)+xi] ;
	y2 = MapSommetY[(zi+65)+(xi+1)] ;
	y3 = MapSommetY[zi+(xi+1)] ;

	numtriangle = GiveTriangle( Nxw, Nzw ) ;

	if( MapPolyGround[IndexMapPolyTested].Sens==0 )
	{
		if( numtriangle==0 )	// Poly de gauche
		{
			beta = GetAngle2D( y2, y1, y1, y0 ) ;
		}
		else	// Poly de droite
		{
			beta = GetAngle2D( y3, y2, y0, y3 ) ;
		}
	}
	else
	{
		if( numtriangle==0 )	// Poly de gauche
		{
			beta = GetAngle2D( y3, y1, y0, y0 ) ;
		}
		else	// Poly de droite
		{
			beta = GetAngle2D( y2, y2, y1, y3 ) ;
		}
	}

	return( beta ) ;
}
#endif	// COMPILATOR

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Gestion de l'animation de l'objet sc‚narique pass‚ (numobj) en mode
// ext‚rieur. Fait appel … certaines fonctions utilis‚es aussi en int‚rieur
void	DoAnimExt( U8 numobj  )
{
#ifndef	COMPILATOR
	T_OBJET	*ptrobj ;
	S32	n ;
	S32	validepos = TRUE ;
	S32	oldcarryby ;
	S32	oldworkflags   ;
	U8	col ;

	APtObj = ptrobj = &ListObjet[(AnimNumObj = numobj)] ;

	if( APtObj->Obj.Body.Num == -1 )	return ;

	OldX = APtObj->OldPosX ;
	OldY = APtObj->OldPosY ;
	OldZ = APtObj->OldPosZ ;

	oldcarryby     = APtObj->CarryBy   ;
	oldworkflags   = APtObj->WorkFlags ;

#ifdef	LBA_EDITOR
	if( numobj!=NUM_PERSO )	validepos = FALSE ;
#else
	if( numobj==NUM_PERSO )
	{
		LastValidePos = ValidePos ;
		ValidePos     = TRUE	;	// considŠre par d‚faut qu'on
						// est sur une position valide
	}
	else	validepos = FALSE ;
#endif

/* gestion DEP OBJET SPRITE */

	if( APtObj->Flags & SPRITE_3D )
	{
		if( APtObj->HitForce != 0 )
			APtObj->WorkFlags |= OK_HIT ;

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
			}
		}
	}
/* gestion anim/DEP OBJET 3D */
	else
	{
		GereObjAnim( numobj ) ;	// pas SPRITE_3D

		// Special Twinsen
		if( APtObj->Move==MOVE_BUGGY_MANUAL )
		{
			DoAnimBuggy( APtObj ) ;
		}
	}

	if( !(APtObj->WorkFlags & FALLING)
	AND  (APtObj->Flags & PUSHABLE) )
	{
		Nxw += APtObj->Coord.Push.PushX ;
		Nyw += APtObj->Coord.Push.PushY ;
		Nzw += APtObj->Coord.Push.PushZ ;

		APtObj->Coord.Push.PushX = 0 ;
		APtObj->Coord.Push.PushY = 0 ;
		APtObj->Coord.Push.PushZ = 0 ;
	}

// pour tous obj/sprite 3d
	if( !CheckCarryBy( numobj ) )
	{
		validepos = FALSE ;
	}

// je tombe
	if( APtObj->WorkFlags & FALLING )
	{
		/* gestion chute avec proportion sur y */
		Nxw = OldX ;
		Nyw = OldY + StepFalling ;
		Nzw = OldZ ;

		validepos = FALSE ;
	}

// Je Glisse sur une collision interdite
	if( APtObj->WorkFlags & SKATING )
	{
		// Verifie qu'on doit toujours glisser
		if( GiveTerrainCol(Nxw,Nzw) )
		{
			S32	beta = GetSkateBeta( Nxw, Nzw ) ;

			// Se trouve-t-on sur les genoux ou sur les fesses ?
#ifdef	LBA_EDITOR
			if( !strcmp( GenAnimSkate, GEN_ANIM_SKATEG ) )
#else
			if( GenAnimSkate==GEN_ANIM_SKATEG )
#endif
			{
				// Sur les genoux, on glisse en arriŠre !

				beta += 2048 ;
				beta &= 4095 ;
			}

			ChangeSpeedBoundAngleMove( &APtObj->BoundAngle,
						   APtObj->SRot,
						   beta ) ;

			InitAnim( GenAnimSkate, ANIM_REPEAT, numobj ) ;

			Nyw = CalculAltitudeObjet( Nxw, Nzw, APtObj->CodeJeu>>4 ) ;

			validepos = FALSE ;
		}
		else
		{
			AnnuleSkating() ;
		}
	}
	else
	{
		if( numobj==NUM_PERSO
		AND GiveTerrainCol(Nxw,Nzw) )
		{
			validepos = FALSE ;
		}
	}

/* test/reajuste pos */

	if( APtObj->Flags&CHECK_BRICK_COL )
	{
		S32	memox, memoy, memoz ;

		YMap = 0 ;

		/* reajuste le nyw (nouvelle pos) par rapport … la col en cours	*/
		if( APtObj->CarryBy==-1
		AND (col = WorldColBrickExt( OldX, OldY, OldZ )) != 0 )
		{
			ReajustPosExt( col ) ;
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
		// Optimisation en inversant l'ordre des 2 tests
		if( APtObj->WorkFlags&FALLING )
		{
			if( APtObj->CarryBy != -1 )
			{
				ReceptionObj() ;
			}
		}

// şşşşşşşşş 4 coins test la col de la nouvelle pos + size zv

		memox = SaveNxw = Nxw ;
		memoy = Nyw ;
		memoz = SaveNzw = Nzw ;

		Col1 = 0 ;
		NumDecorsCol = -1 ;

		if( DoCornerReajustDecors( ) )
		{
			// Tente un second recalage au cas ou se serait fait
			// recaler dans un autre objet

// ATTENTION: peut-etre dangereux de mettre ca en commentaire
			if( DoCornerReajustDecors( ) )
			{
				// on y est pas arriv‚, tant pis, on restore
				// sa position
				if( WorldColBrickDecors( Nxw+APtObj->XMin, Nyw+APtObj->YMin+1, Nzw+APtObj->ZMin,
							 Nxw+APtObj->XMax, Nyw+APtObj->YMax, Nzw+APtObj->ZMax ) )
				{
					Nxw = OldX ;
					Nzw = OldZ ;
				}
			}

			if( APtObj->CarryBy==-1
			AND (oldcarryby==-1)// pas de reajustement si on vient de quitter un CarryBy
			AND !(APtObj->WorkFlags&ANIM_MASTER_GRAVITY)
			AND GiveTerrainCol( Nxw, Nzw ) )// tiens, on m'a mis dans une
			{				// collision interdite !
				Nxw = OldX ;
				Nzw = OldZ ;
			}
		}

		SaveNxw = Nxw ;
		SaveNzw = Nzw ;

		if( (!(APtObj->WorkFlags & (ANIM_MASTER_GRAVITY|SKATING))
		   OR (APtObj->WorkFlags & FLY_JETPACK))
		AND (APtObj->CarryBy==-1)
		AND (oldcarryby==-1) )// pas de reajustement si on vient de quitter un CarryBy
		{
			if( DoCornerReajustExtPt() )
			{
				if( WorldColBrickDecors( Nxw+APtObj->XMin, Nyw+APtObj->YMin+1, Nzw+APtObj->ZMin,
							 Nxw+APtObj->XMax, Nyw+APtObj->YMax, Nzw+APtObj->ZMax )
				OR  GiveTerrainCol( Nxw, Nzw ) )
				{
					// on m'a pouss‚ dans un objet du d‚cors
					Nxw = OldX ;
					Nzw = OldZ ;
				}
			}
		}

		//************** ATTENTION: test de blindage collis avec objs
		if( memox!=Nxw
		OR  memoy!=Nyw
		OR  memoz!=Nzw )
		{
			// houullla !!!!!!!!!
			// on s'est fait recaler par le decors, v‚rifie qu'on
			// n'a pas ‚t‚ ‚jecter dans un objet sce

			if( IsObjCol( numobj ) )
			{
				// tant pis, on restore l'ancienne pos, et toc!
				Nxw = OldX ;
				Nyw = OldY ;
				Nzw = OldZ ;
			}
		}
		//**************


// şşşşşşşşş	test la col de la nouvelle pos reelle (pour le sol)
		if( (APtObj->Col = col = WorldColBrickExt( Nxw, Nyw, Nzw )) != 0 )
		{
			if( col == 1 )
			{
				if( APtObj->WorkFlags & FALLING )
				{
					ReceptionObj() ;
					ReajustPosExt( col ) ;
				}
				else
				{
			/* fait glisser (dernier recours) */

/*şşşşşşşşş*/	/* regarde de dep X puis Z par rapport … old pos */

					if( (!(APtObj->WorkFlags & ANIM_MASTER_GRAVITY)
					   OR (APtObj->WorkFlags &FLY_JETPACK))
					AND (APtObj->CarryBy==-1) )
					{
					if( WorldColBrickExt( Nxw, Nyw, OldZ ) != 0 )
					{
						if( WorldColBrickExt( OldX, Nyw, Nzw ) != 0 )
						{
							// Cas particulier
							// Pour virer bug peintre dans 0_EGOUT2.SCE
							// Objet etant en ANIM_MASTER_GRAVITY, ayant le flag
							// BRICK_COL mais pas le flag OBJ_FALLABLE alors
							// que son anim boucle sur une frame ayant une
							// translation en Y (plonge)
							if( !(ptrobj->Flags&OBJ_FALLABLE) AND ptrobj->WorkFlags&ANIM_MASTER_GRAVITY )
							{
								ReajustPosExt( col ) ;
							}
							else
							{
								validepos = FALSE ;
								goto findoanimext ;	/* pos pas acceptee */
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
			}
			else
			{
				if( APtObj->WorkFlags & FALLING )
				{
					ReceptionObj() ;
				}

// orrection bug chariot dans descente twinsen touche col pench‚e
				if( APtObj->CarryBy == -1 )
				{
					ReajustPosExt( col ) ;
				}
			}

			APtObj->WorkFlags &= ~FALLING ;
		}
		else    /* col == 0 */
		{
/* si sous les pieds != cube plein: susceptible de tomber ou reajuste y */
			if( (APtObj->Flags & OBJ_FALLABLE) AND (APtObj->CarryBy == -1) )
			{
				col = WorldColBrickExt( Nxw, Nyw-1, Nzw ) ;
				if( col )
				{
					if( APtObj->WorkFlags & FALLING )
					{
						ReceptionObj() ;
					}

					ReajustPosExt( col ) ;
				}
				else
				{
					validepos = FALSE ;

					if( !(APtObj->WorkFlags & ANIM_MASTER_GRAVITY) )
					{
/*						if( !StartYFalling
						AND (numobj==NUM_PERSO)
						AND !FlagClimbing )
						{
							StartYFalling = Nyw ;
						}
*/
					if( !(APtObj->WorkFlags&FALLING) )
					{
						Nyw -= DEMI_BRICK_Y ;// Magouille pour checkobjcol
						CheckObjCol(numobj) ;
						if( ptrobj->ObjCol==255 )	Nyw += DEMI_BRICK_Y ;// Magouille pour checkobjcol
					}

					if( ptrobj->ObjCol==255 )
					{
						if( APtObj->Flags & SPRITE_3D )
						{
							if( WorldColBrickExt( Nxw+APtObj->XMin, Nyw-1, Nzw+APtObj->ZMin )!=1
							AND WorldColBrickExt( Nxw+APtObj->XMin, Nyw-1, Nzw+APtObj->ZMax )!=1
							AND WorldColBrickExt( Nxw+APtObj->XMax, Nyw-1, Nzw+APtObj->ZMin )!=1
							AND WorldColBrickExt( Nxw+APtObj->XMax, Nyw-1, Nzw+APtObj->ZMax )!=1 )
							{
								APtObj->WorkFlags |= FALLING ;
							}
						}
						else	// obj 3D
						{
							if( !WorldColBrickExt( Nxw, Nyw-SIZE_BRICK_Y/2, Nzw )
							AND !(APtObj->WorkFlags&SKATING)
							AND ((APtObj->CodeJeu>>4)==0)
							AND ( GiveTerrainCol(Nxw,Nzw)
							      OR oldcarryby!=-1
							      OR oldworkflags&ANIM_MASTER_GRAVITY
							      OR (APtObj->WorkFlags&(FALLING|FLY_JETPACK|CHECK_FALLING)) ))
//							      OR APtObj->Col) )
							// rajouter test terrain collision interdite sur OldX, OldY, OldZ ?
							{
								if( numobj!=NUM_PERSO
								OR !FlagClimbing )
								{
/*#ifdef	LBA_EDITOR
									if( stricmp(APtObj->StringAnim,GEN_ANIM_ARRIMAGE) )
#else
									if( APtObj->GenAnim!=GEN_ANIM_ARRIMAGE )
#endif
*/									{
										InitAnim( GEN_ANIM_TOMBE, ANIM_TEMPO, numobj ) ;
										APtObj->WorkFlags |= FALLING ;
										PtrZoneClimb = NULL ;

										if( !StartYFalling
										AND numobj == NUM_PERSO )
										{
											StartYFalling = Nyw ;
										}
									}
								}
							}
							else
							{
								ReajustPosExt( 1 ) ;

								// Bug Plaque ejectee quand on saute
								// dessus
								CheckObjCol( numobj ) ;
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

		// Mort si l'objet touche le fond du cube - 
//		if( YMap<0 )	APtObj->LifePoint = 0 ;	// bye bye
//		if( APtObj->Obj.Y<=0 )	APtObj->LifePoint = 0 ;	// bye bye
	}
	else	/* ! Flags & CHECK_BRICK_COL */
	{
		if( APtObj->Flags & CHECK_OBJ_COL )
		{
			CheckObjCol( numobj ) ;
		}
	}

	// test pour le pingouin: bit 7 champ Col … 1 si touche mur
	if( Col1 != 0 )		ptrobj->Col |= 128 ;

/* protege sortie du cube */

	// … virer puisqu'on pourra descendre en dessous de 0
//	if( Nyw < 0 ) Nyw = 0 ;

	// si on passe par l'un de ces tests, cela signifie que l'on n'est
	// pas dans une zone de changement de cube, donc on tente de lancer
	// le cube phantom.sce pour aller y mourir

	FlagHeroOutX = FlagHeroOutZ = FALSE ;
	PhantomX = PhantomY = 0 ;	// peut etre sorti ailleurs

	if( !NumBuggy OR numobj!=NUM_BUGGY OR !IsBuggyPresent() )
	{
		S32	flagchgcube = TRUE ;
		S32	marge = DEMI_BRICK_XZ ;

		if( numobj==NUM_PERSO )
		{
#ifdef	LBA_EDITOR
			if( !stricmp(APtObj->StringAnim,GEN_ANIM_CHOC)
			OR  !stricmp(APtObj->StringAnim,GEN_ANIM_CHOC2)
			OR  !stricmp(APtObj->StringAnim,GEN_ANIM_ENCAISSE) )
#else
			if( APtObj->GenAnim==GEN_ANIM_CHOC
			OR  APtObj->GenAnim==GEN_ANIM_CHOC2
			OR  APtObj->GenAnim==GEN_ANIM_ENCAISSE )
#endif
			{
				marge += 10 ;
				flagchgcube = FALSE ;
			}
		}
		else	flagchgcube = FALSE ;

		if( Nxw < marge )
		{
			if( flagchgcube )	FlagHeroOutX = TRUE ;
			Nxw = marge ;
			PhantomX = -1 ;
		}

		if( Nzw < marge )
		{
			if( flagchgcube )	FlagHeroOutZ = TRUE ;
			Nzw = marge ;
			PhantomY = -1 ;
		}

		if( Nxw > 64*SIZE_BRICK_XZ-marge )
		{
			if( flagchgcube )	FlagHeroOutX = TRUE ;
			Nxw = 64*SIZE_BRICK_XZ-marge ;
			PhantomX = 1 ;
		}

		if( Nzw > 64*SIZE_BRICK_XZ-marge )
		{
			if( flagchgcube )	FlagHeroOutZ = TRUE ;
			Nzw = 64*SIZE_BRICK_XZ-marge ;
			PhantomY = 1 ;
		}
	}

	if( !GereCodesJeu( numobj ) )	validepos = FALSE ;

	if( (PhantomX OR PhantomY)
	AND (numobj==NUM_PERSO)
	AND (APtObj->CarryBy==-1 OR APtObj->CarryBy&CARRY_BY_DECORS)
	AND ((Comportement != C_PROTOPACK)	// on ne meurre pas en marche jetpack
	AND (Comportement != C_JETPACK)
#ifdef	LBA_EDITOR
	OR  stricmp(APtObj->StringAnim,GEN_ANIM_MARCHE)) )
#else
	OR  APtObj->GenAnim!=GEN_ANIM_MARCHE) )
#endif
	{
		S32	index ;

		index = (CurrentCubeY+PhantomY)*16 + (CurrentCubeX+PhantomX) ;
		index = IsleMapIndex[index]&127 ; // bug compilo ou debug ?

		if( !index )	// pas de cube
		{
			// allez, on va mourir dans le cube phantom !
			NewCube = NUM_CUBE_PHANTOM ;
			FlagChgCube = 1 ;		// passe en mode NewPos

			NewPosY = Nyw ;

			switch( PhantomX )
			{
				case -1:
					NewPosX = 32768-SIZE_BRICK_XZ-SIZE_BRICK_XZ/2 ;
					break ;

				case 1:
					NewPosX = SIZE_BRICK_XZ+SIZE_BRICK_XZ/2 ;
					break ;

				default:
					NewPosX = Nxw ;
					break ;
			}

			switch( PhantomY )
			{
				case -1:
					NewPosZ = 32768-SIZE_BRICK_XZ-SIZE_BRICK_XZ/2 ;
					break ;

				case 1:
					NewPosZ = SIZE_BRICK_XZ+SIZE_BRICK_XZ/2 ;
					break ;

				default:
					NewPosZ = Nzw ;
					break ;
			}

#ifndef	LBA_EDITOR
			PhantomX += CurrentCubeX ;
			PhantomY += CurrentCubeY ;
#endif

			PhantomIsland = Island ;	// memo island
			FlagReajustPosTwinsen = TRUE ;
			validepos = FALSE ;
		}
	}

	if( APtObj->WorkFlags & SKATING )
	{
		if( abs(Nxw-OldX)>=10
		OR  abs(Nyw-OldY)>=10
		OR  abs(Nzw-OldZ)>=10 )
		{
			TimerSkating = TimerRefHR+SKATING_TIME_OUT ;
		}
		else if( TimerRefHR>TimerSkating )
		{
			AnnuleSkating() ;
		}
	}

	APtObj->Obj.X = Nxw ;
	APtObj->Obj.Y = Nyw ;
	APtObj->Obj.Z = Nzw ;

findoanimext:

	if( numobj==NUM_PERSO )
	{
		S16	nu = APtObj->CarryBy ;

#ifndef	LBA_EDITOR
		if( !LastValidePos AND APtObj->LifePoint<=0 )
		{
			validepos = FALSE ;
		}
#endif

		// pour le protopack
		if( Comportement==C_PROTOPACK
#ifdef	LBA_EDITOR
		AND stricmp(APtObj->StringAnim,GEN_ANIM_MARCHE) )
#else
		AND APtObj->GenAnim==GEN_ANIM_MARCHE )
#endif
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

#ifdef	LBA_EDITOR
		if( validepos )
		{
			ValideCube = NumCube ;
			ValidePosX = OldX ;
			ValidePosY = OldY ;
			ValidePosZ = OldZ ;
			ValideBeta = APtObj->Obj.Beta ;
		}
#else
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
						// ‚t‚ faite
			{
				SaveValidePos() ;
			}
		}

		FirstSave = FALSE ;
#endif
	}
	else
	{
		FlagHeroOutX = FALSE ;
		FlagHeroOutZ = FALSE ;
	}
#endif
}


/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
			 ßßÛßß ÛßßßÛ ÛßßßÛ Û     ÛÛßßß
			   ÛÛ  ÛÛ  Û ÛÛ  Û ÛÛ    ßßßßÛ
			   ßß  ßßßßß ßßßßß ßßßßß ßßßßß
		     (partie seulement utilis‚e dans l'outil)
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

#ifdef	LBA_EDITOR
// Affiche la feuille (carr‚) d'un polygone index‚ par xi,zi
void	DrawGrilleFeuille3D( S32 xi, S32 zi )
{
	S32	x0,z0,x3,z3 ;
	S32	x2,z2,x1,z1 ;
	S32	cx0,cy0,cx1,cy1 ;
	S32	sens ;
	S32	beta ;
	S32	y0, y1, y2, y3 ;

	y0 = MapSommetY[zi*65+xi] ;
	y1 = MapSommetY[(zi+1)*65+xi] ;
	y2 = MapSommetY[(zi+1)*65+xi+1] ;
	y3 = MapSommetY[zi*65+xi+1] ;

	x0 = SommetRot[zi+0][xi+0].X2D ;
	z0 = SommetRot[zi+0][xi+0].Y2D ;
	x1 = SommetRot[zi+1][xi+0].X2D ;
	z1 = SommetRot[zi+1][xi+0].Y2D ;
	x2 = SommetRot[zi+1][xi+1].X2D ;
	z2 = SommetRot[zi+1][xi+1].Y2D ;
	x3 = SommetRot[zi+0][xi+1].X2D ;
	z3 = SommetRot[zi+0][xi+1].Y2D ;

	UnsetClip() ;

	cx0 = __min( __min(x0,x1), __min(x2,x3) ) ;
	cy0 = __min( __min(z0,z1), __min(z2,z3) ) ;
	cx1 = __max( __max(x0,x1), __max(x2,x3) ) ;
	cy1 = __max( __max(z0,z1), __max(z2,z3) ) ;

	sens = MapPolyGround[zi*64*2+xi*2].Sens ;

	DrawFeuille3D( 	x0, z0,
			x1, z1,
			x2, z2,
			x3, z3,
			sens,
			GREEN,
			FALSE );

	CoulText( YELLOW, BLACK ) ;

	if( !sens )
	{
		Line( x0, z0, x2, z2, GREEN ) ;

		// poly de gauche (0)
		beta = GetAngle2D( y2, y1, y1, y0 ) ;
		GraphPrintf( FALSE, 0, 50, "Beta0= %d", beta ) ;
		GraphPrintf( FALSE, 0, 60, "y0-y1=%d   y1-y2=%d ", y0-y1, y1-y2 ) ;

		// Poly de droite (1)
		beta = GetAngle2D( y3, y2, y0, y3 ) ;
		GraphPrintf( FALSE, 0, 80, "Beta1= %d", beta ) ;
		GraphPrintf( FALSE, 0, 90, "y3-y2=%d   y0-y3=%d ", y3-y2, y0-y3 ) ;
	}
	else
	{
		Line( x1, z1, x3, z3, YELLOW ) ;

		// Poly de gauche (0)
		beta = GetAngle2D( y3, y1, y0, y0 ) ;
		GraphPrintf( FALSE, 0, 50, "Beta0= %d", beta ) ;
		GraphPrintf( FALSE, 0, 60, "y0-y1=%d   y0-y3=%d ", y0-y1, y0-y3 ) ;

		// Poly de droite (1)
		beta = GetAngle2D( y2, y2, y1, y3 ) ;
		GraphPrintf( FALSE, 0, 80, "Beta1= %d", beta ) ;
		GraphPrintf( FALSE, 0, 90, "y3-y2=%d   y1-y2=%d ", y3-y2, y1-y2 ) ;
	}

	BoxStaticAdd( cx0,cy0,cx1,cy1 ) ;
}
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Affiche la table de fog dans un box pass‚ en paramŠtres
void	AffFogTable( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	S32	x, y, xl, yl, fi ;
	S32	yo ;
	S32	xo = x0 ;

	for( xl=0; xl<256; xl++ )
	{
		yo = y0 ;
		x = RegleTrois( x0,x1, 256, xl+1 ) ;

		for( fi=0; fi<16; fi++ )
		{
			for( yl=0; yl<16; yl++ )
			{
				y = RegleTrois( y0,y1, 256, (fi*16)+yl+1 ) ;
				Box( xo, yo, x, y, PtrNuances[(fi*16*256)+yl*256+xl] ) ;

				yo = y+1 ;
			}
		}
		xo = x+1 ;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Affiche la palette a l'ecran (Eph‚mŠre car aucun rapport avec 3Dext)
void AffPalette( )
{
	S32	t ;
	S32	x, y ;

	x = y = 0 ;

	Cls ( ) ;

	for( t=0; t<256; t++ )
	{
		Box ( x, y, x+17, y+9, t ) ;

		y += 10 ;

		if ( y>=160 )
		{
			GraphPrintf( FALSE, x+2, 165, "%d", t/16 ) ;
			y = 0 ;
			x += 18 ;
		}
	}

	// Init Textures
	PtrMap = ObjPtrMap = BufferTexture ;
	RepMask = 0xFFFF ;

	// Affiche page de texture
	RestoreBlock( Log, PtrMap, 383, 223, 638, 478 ) ;
	Rect( 382, 222, 639, 479, LBAWHITE ) ;

	// Affiche page de texture SkySea
	RestoreBlock( Log, GroundTexture, 5, 223, 260, 478 ) ;
	Rect( 4, 222, 261, 479, LBAWHITE ) ;

	// Affiche Table de Fog
//	AffFogTable( 383, 0, 643, 220 ) ;

	BoxUpdate ( ) ;

	while( MyKey )
	{
		ManageTime() ;
		MyGetInput() ;

		if( AnimateTexture )
		{
			DoTextureAnimation() ;
			// Affiche page de texture
			RestoreBlock( Log, PtrMap, 383, 223, 638, 478 ) ;
			BoxStaticAdd( 383, 223, 638, 478 ) ;
			BoxUpdate() ;
		}
	}

	FirstTime = AFF_ALL_FLIP ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Affiche le terrain en mode "SPOTS"
void	RedrawSpeedTerrain()
{
	Cls() ;
	SetFollowCamera( VueOffsetX, VueOffsetY, VueOffsetZ, AlphaCam, BetaCam, GammaCam, VueDistance ) ;
	AffichageTerrainRapide();
	BoxUpdate() ;

#ifdef	LBA_EDITOR
	if( FlagInfos&INFO_REPERE )
	{
		DrawRepereXYZ( 560, 420, 639, 479 ) ;
	}

	if( FlagInfos&INFO_CAMERA )
	{
		DrawCameraInfos() ;
	}

	GereDebug() ;
#endif

	FirstTime = AFF_ALL_FLIP ;
}

#ifdef	LBA_EDITOR
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Rotation du terrain … la souris
S32	GereMouseMovements( S32 boutons )
{
	if( CubeMode!=CUBE_EXTERIEUR )	return FALSE ;

	if( MyClick==boutons )
	{
		SaveTimer() ;

		GetMouseDep() ;
		while( MyClick )
		{
			MyGetInput() ;
			GetMouseDep() ;

			AlphaCam += MouseYDep ;
			BetaCam += MouseXDep ;

			AlphaCam &= 4095 ;
			BetaCam  &= 4095 ;

			RedrawSpeedTerrain() ;
		}

		FirstTime = AFF_ALL_FLIP ;
		RestoreTimer() ;
		return( TRUE ) ;
	}

	return( FALSE ) ;
}
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Gestion zoom/pivot du terrain et position cam‚ra
#ifdef	LBA_EDITOR
S32	GereExtKeys( U32 key, U32 flagmenu )
#else
S32	GereExtKeys( U32 key )
#endif
{
	S32	flag = 0 ;
	S32	savetimer ;
	S32	savetimer2 ;

	if( CubeMode!=CUBE_EXTERIEUR OR !key )	return( FALSE ) ;

	savetimer = TimerRefHR ;
	savetimer2 = TimerSystemHR ;

	SaveTimer() ;

#ifdef	LBA_EDITOR
	while( key==K_PAGE_DOWN )
	{
		MyGetInput() ;
		key = Key ;
		flag = 1 ;

		if( Reglage>0 )
		{
			if( TimerRefHR>savetimer+100 )
			{
				Reglage-- ;
				savetimer = TimerRefHR ;
			}
		}

		FirstTime = AFF_ALL_FLIP ;
	}

	while( key==K_PAGE_UP )
	{
		MyGetInput() ;
		key = Key ;
		flag = 1 ;

		if( Reglage<100 )
		{
			if( TimerRefHR>savetimer+100 )
			{
				Reglage++ ;
				savetimer = TimerRefHR ;
			}
		}

		FirstTime = AFF_ALL_FLIP ;
	}

	while( key==K_PLUS )
	{
		MyGetInput() ;
		key=Key ;
		flag = 1 ;

		if( TimerRefHR>savetimer+1000 )	VueDistance += 500 ;
		else				VueDistance += 100 ;

		RedrawSpeedTerrain() ;
	}

	while( key==K_MOINS )
	{
		MyGetInput() ;
		key=Key ;
		flag = 1 ;

		if( TimerRefHR>savetimer+1000 )	VueDistance -= 500 ;
		else				VueDistance -= 100 ;

		RedrawSpeedTerrain() ;
	}

	while( key==K_U
	OR (flagmenu AND key==K_PAGE_DOWN) )
	{
		key=Key ;
		flag = 1 ;
		if( TimerRefHR>savetimer+1000 )	VueOffsetY += 500 ;
		else				VueOffsetY += 100 ;
		StartYCube = VueOffsetY/SIZE_BRICK_Y ;
		RedrawSpeedTerrain() ;
	}

	while( key==K_J
	OR (flagmenu AND key==K_PAGE_UP) )
	{
		key=Key ;
		flag = 1 ;
		if( TimerRefHR>savetimer+1000 )	VueOffsetY -= 500 ;
		else				VueOffsetY -= 100 ;
		StartYCube = VueOffsetY/SIZE_BRICK_Y ;
		RedrawSpeedTerrain() ;
	}

	while( key==K_5 )
	{
		key=Key ;
		flag = 1 ;
		if( TimerRefHR>savetimer+1000 )	GammaCam += 30 ;
		else				GammaCam += 10 ;
		RedrawSpeedTerrain() ;
	}

	while( key==K_6 )
	{
		key=Key ;
		flag = 1 ;
		if( TimerRefHR>savetimer+1000 )	GammaCam -= 30 ;
		else				GammaCam -= 10 ;
		RedrawSpeedTerrain() ;
	}

	if( flagmenu )
	{
		while( Input&I_JOY )
		{
			if( Input & I_LEFT )
			{
				if( TimerRefHR>savetimer+1000 )	VueOffsetX -= 500 ;
				else				VueOffsetX -= 100 ;
			}

			if( Input & I_RIGHT )
			{
				if( TimerRefHR>savetimer+1000 )	VueOffsetX += 500 ;
				else				VueOffsetX += 100 ;
			}

			if( Input & I_UP )
			{
				if( TimerRefHR>savetimer+1000 )	VueOffsetZ -= 500 ;
				else				VueOffsetZ -= 100 ;
			}

			if( Input & I_DOWN )
			{
				if( TimerRefHR>savetimer+1000 )	VueOffsetZ += 500 ;
				else				VueOffsetZ += 100 ;
			}

			StartXCube = VueOffsetX/SIZE_BRICK_XZ ;
			StartZCube = VueOffsetZ/SIZE_BRICK_XZ ;
			RedrawSpeedTerrain() ;
			MyGetInput() ;
			flag = 1 ;
		}
	}
/*
	// Reglage ChampX/ChampZ- (Focale)
	while( Key==K_9 )
	{
		if( ChampX>0 )
		{
			ChampX -= 10 ;
			ChampZ -= 10 ;
			PtrInit3DGame() ;
			CameraCenter( 2 ) ;
			FirstTime = AFF_ALL_FLIP ;
			RedrawSpeedTerrain() ;
		}
	}

	// Reglage ChampX/ChampZ+ (Focale)
	while( Key==K_0 )
	{
		if( ChampX<2048 )
		{
			ChampX += 10 ;
			ChampZ += 10 ;
			PtrInit3DView() ;
			CameraCenter( 2 ) ;
			FirstTime = AFF_ALL_FLIP ;
			RedrawSpeedTerrain() ;
		}
	}
*/
	// Reglage StartZFog
	while( CheckKey(K_9) )
	{
		if( CheckKey(K_DOWN) AND StartZFog>NearClip )
		{
			if( TimerSystemHR>savetimer2+3000 )	StartZFog -= 2000 ;
			else				StartZFog -= 100  ;
			PtrInit3DGame() ;
			CameraCenter( 2 ) ;
			FirstTime = AFF_ALL_FLIP ;
			SetFog( StartZFog, ClipZFar) ;
			AffScene( AFF_ALL_FLIP ) ;
		}
		else if( CheckKey(K_UP) AND StartZFog<ClipZFar-100 )
		{
			if( TimerSystemHR>savetimer2+3000 )	StartZFog += 2000 ;
			else				StartZFog += 100  ;
			PtrInit3DGame() ;
			CameraCenter( 2 ) ;
			FirstTime = AFF_ALL_FLIP ;
			SetFog( StartZFog, ClipZFar) ;
			AffScene( AFF_ALL_FLIP ) ;
		}
	}

	// Reglage ClipZFar
	while( CheckKey(K_0) )
	{
		if( CheckKey(K_DOWN) AND ClipZFar>StartZFog+100 )
		{
			if( TimerSystemHR>savetimer2+3000 )	ClipZFar -= 2000 ;
			else				ClipZFar -= 100  ;
			PtrInit3DGame() ;
			CameraCenter( 2 ) ;
			FirstTime = AFF_ALL_FLIP ;
			SetFog( StartZFog, ClipZFar) ;
			AffScene( AFF_ALL_FLIP ) ;
		}
		else if( CheckKey(K_UP) AND ClipZFar<200000L )
		{
			if( TimerSystemHR>savetimer2+3000 )	ClipZFar += 2000 ;
			else				ClipZFar += 100  ;
			PtrInit3DGame() ;
			CameraCenter( 2 ) ;
			FirstTime = AFF_ALL_FLIP ;
			SetFog( StartZFog, ClipZFar) ;
			AffScene( AFF_ALL_FLIP ) ;
		}
	}
#else	// LBA_EDITOR
	// Mode Jeu uniquement
// penser … remettre en DEBUG_TOOLS
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
	while( key==K_PAGE_DOWN )
	{
		MyGetInput() ;
		key = Key ;
		flag = 1 ;

		if( AlphaCam<600 )
		{
			if( TimerRefHR>savetimer+1000 )	AlphaCam += 10 ;
			else				AlphaCam += 3 ;
		}

		RedrawSpeedTerrain() ;
	}

	while( key==K_PAGE_UP )
	{
		MyGetInput() ;
		key = Key ;
		flag = 1 ;

		if( AlphaCam>150 )
		{
			if( TimerRefHR>savetimer+1000 )	AlphaCam -= 10 ;
			else			        AlphaCam -= 3 ;
		}

		RedrawSpeedTerrain() ;
	}
#endif

	if( Input&I_CAMERA_LEVEL_MOINS )
	{
		if( VueCamera>0 )
		{
			VueCamera-- ;
			FirstTime = AFF_ALL_FLIP ;
			CameraCenter( 2 ) ;
		}
		InitWaitNoInput( I_CAMERA_LEVEL_MOINS ) ;
	}

	if( Input&I_CAMERA_LEVEL_PLUS )
	{
		if( VueCamera<NB_VUE_CAMERA-1 )
		{
			VueCamera++ ;
			FirstTime = AFF_ALL_FLIP ;
			CameraCenter( 2 ) ;
		}
		InitWaitNoInput( I_CAMERA_LEVEL_PLUS ) ;
	}
#endif

	RestoreTimer() ;

	return( flag ) ;
}

#ifdef	LBA_EDITOR

#define	COUL_REPERE	YELLOW
#define	TAILLE_REPERE	1000/35

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Dessine le repŠre dans un rectangle pour ... se rep‚rer !!!
void	DrawRepereXYZ( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	S32	x, z ;
	S32	demiw = (x1-x0)/2 ;
	S32	demih = (y1-y0)/2 ;

	if( CubeMode!=CUBE_EXTERIEUR )	return ;

	// repŠre des petits axes x, y et z
	SetProjection( x0+demiw, y0+demih, 10, 1024, 1024 ) ;
	SetFollowCamera( 0, 0, 0, AlphaCam, BetaCam, GammaCam, 1000 ) ;

	CoulText( YELLOW, -1 ) ;

	LongWorldRotatePoint( 0, 0, 0 );
	LongProjectPoint( X0,Y0,Z0 );
	x = Xp;
	z = Yp;
	LongWorldRotatePoint( TAILLE_REPERE, 0, 0 );
	LongProjectPoint( X0,Y0,Z0 );
	Line( x, z, Xp, Yp, COUL_REPERE );
	GraphPrintf(FALSE,Xp,Yp,"X");
//	AffString(Xp,Yp,"X");

	LongWorldRotatePoint( 0, 0, 0 );
	LongProjectPoint( X0,Y0,Z0 );
	x = Xp;
	z = Yp;
	LongWorldRotatePoint( 0, 0,TAILLE_REPERE );
	LongProjectPoint( X0,Y0,Z0 );
	Line( x, z, Xp, Yp, COUL_REPERE );
	GraphPrintf(FALSE,Xp,Yp,"Z");
//	AffString(Xp,Yp,"Z");

	LongWorldRotatePoint( 0, 0, 0 );
	LongProjectPoint( X0,Y0,Z0 );
	x = Xp;
	z = Yp;
	LongWorldRotatePoint( 0, TAILLE_REPERE, 0 );
	LongProjectPoint( X0,Y0,Z0 );
	Line( x, z, Xp, Yp, COUL_REPERE );
	GraphPrintf(FALSE,Xp, Yp,"Y");
//	AffString(Xp, Yp,"Y");

	BoxStaticAdd( x0, y0, x1, y1 );

	PtrInit3DView( ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Le nom de la fonction est assez parlant, non ?
void	DrawCameraInfos()
{
	InitPalMenu() ;

	OpenWindow( &DWin, 10,20, 20,25 ) ;

	WinText( &DWin, "VueOffsetX  : ", VueOffsetX ) ;
	WinText( &DWin, "VueOffsetY  : ", VueOffsetY ) ;
	WinText( &DWin, "VueOffsetZ  : ", VueOffsetZ ) ;
	WinSepar( &DWin ) ;
	WinText( &DWin, "AlphaCam    : ", AlphaCam ) ;
	WinText( &DWin, "BetaCam     : ", BetaCam ) ;
	WinText( &DWin, "GammaCam    : ", GammaCam ) ;
	WinSepar( &DWin ) ;
	WinText( &DWin, "CameraAlpha : ", CameraAlpha ) ;
	WinText( &DWin, "CameraBeta  : ", CameraBeta ) ;
	WinText( &DWin, "CameraGamma : ", CameraGamma ) ;
	WinSepar( &DWin ) ;
	WinText( &DWin, "VueDistance : ", VueDistance ) ;

	WinSepar( &DWin ) ;
	WinText( &DWin, "ChampX      : ", ChampZ ) ;
	WinText( &DWin, "ChampZ      : ", ChampX ) ;

	WinSepar( &DWin ) ;
	WinText( &DWin, "AlphaLight  : ", AlphaLight ) ;
	WinText( &DWin, "BetaLight   : ", BetaLight ) ;

	WinSepar( &DWin ) ;
	WinText( &DWin, "CameraX     : ", CameraX ) ;
	WinText( &DWin, "CameraY     : ", CameraY ) ;
	WinText( &DWin, "CameraZ     : ", CameraZ ) ;
	WinSepar( &DWin ) ;
	WinText( &DWin, "CameraXr    : ", CameraXr ) ;
	WinText( &DWin, "CameraYr    : ", CameraYr ) ;
	WinText( &DWin, "CameraZr    : ", CameraZr ) ;
	WinSepar( &DWin ) ;
	WinText( &DWin, "NearClip    : ", NearClip ) ;
	WinText( &DWin, "StartZFog   : ", StartZFog ) ;
	WinText( &DWin, "ClipZFar    : ", ClipZFar ) ;

	FlipWindow( &DWin ) ;
}


#define	CREATE_ZONE_NORD	1
#define	CREATE_ZONE_SUD		2
#define	CREATE_ZONE_EST		4
#define	CREATE_ZONE_OUEST	8

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Cherche le num‚ro d'un cube dans CUBE.LST d'aprŠs sa position dans la carte
// Cette fonction va gicler car il n'y a plus de normalisation des noms de cubes
// ext‚rieurs ! (domage !!!!)
S16	SearchNumCube( S32 x, S32 y )
{
	char	*pt ;
	char	scename[20] ;
	S16	n = 0 ;
	S32	len ;

	if( Island<10 )
		sprintf( scename, "%1d_x%02d_%02d", Island, x, y ) ;
	else	sprintf( scename, "%2d_%02d_%02d", Island, x, y ) ;

	len = strlen(scename) ;

	InitDefFile( CubeLstName ) ;

	do
	{
		pt = DefFileBufferReadString( Itoa( n ) )	;


		if( pt AND strchr(pt,';') )
			*(char *)strchr(pt,';') = '\0' ;

		if( pt!=0 AND !strnicmp(scename,pt,len) )	return n ;

		n++ ;
	}
	while( pt ) ;

	return 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Cr‚er une zone de changement de cube automatiquement
void	CreateExtZone( S32 x0, S32 y0, S32 z0, S32 x1, S32 y1, S32 z1,
		       S32 xd, S32 yd, S32 zd, S32 cubex, S32 cubey )
{
	T_ZONE *ptrz ;

	if( NbZones<MAX_ZONES-1 )
	{
		ptrz = &ListZone[NbZones] ;

		ptrz->Num = SearchNumCube( cubex, cubey ) ;
		if( !ptrz->Num )	return ;

		ptrz->X0 = x0 ;
		ptrz->Y0 = y0 ;
		ptrz->Z0 = z0 ;
		ptrz->X1 = x1 ;
		ptrz->Y1 = y1 ;
		ptrz->Z1 = z1 ;
		ptrz->Snap = TRUE ;

		ptrz->Type = 0 ;
		ptrz->Info0 = xd ;
		ptrz->Info1 = yd ;
		ptrz->Info2 = zd ;
		ptrz->Info3 = 0 ;
		ptrz->Info4 = 0 ;
		ptrz->Info5 = 0 ;
		ptrz->Info6 = 0 ; // Reajust pos twinsen
		ptrz->Info7 = 1 | ZONE_ON ;

		NbZones++ ;
		Modif = TRUE ;

		IndexZone = NbZones-1 ;
	}
	else
	{
		Message( "MAX ZONES DEC atteind!", TRUE ) ;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Cr‚er toutes les zones de changement de cube s‚lectionn‚es dans un cube
// ext‚rieur automatiquement
void	CreateExtZones( S32 zones, S32 hauteur )
{
	if( CubeMode!=CUBE_EXTERIEUR )	return ;

	if( zones&CREATE_ZONE_NORD )
	{
		CreateExtZone( 0, 0, 0, 32767, hauteur, 512,
			       0, 0, 32768-1024, CurrentCubeX, CurrentCubeY-1 ) ;
	}

	if( zones&CREATE_ZONE_SUD )
	{
		CreateExtZone( 0, 0, 32768-512, 32767, hauteur, 32767,
			       0, 0, 512, CurrentCubeX, CurrentCubeY+1 ) ;
	}

	if( zones&CREATE_ZONE_OUEST )
	{
		CreateExtZone( 0, 0, 0, 512, hauteur, 32767,
			       32768-1024, 0, 0, CurrentCubeX-1, CurrentCubeY ) ;
	}

	if( zones&CREATE_ZONE_EST )
	{
		CreateExtZone( 32768-512, 0, 0, 32767, hauteur, 32767,
			       512, 0, 0, CurrentCubeX+1, CurrentCubeY ) ;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Menu de cr‚ation automatique des zones de changement de cube
void	MenuCreateExtZones()
{
	T_MENU	menu ;
	S32	zones = CREATE_ZONE_NORD|CREATE_ZONE_SUD|CREATE_ZONE_EST|CREATE_ZONE_OUEST ;
	S32	select, flag = FALSE ;
	S32	hauteur = 15000L ;

	BackupScreen( TRUE ) ;

	OpenMenu( &menu, 15, 9 ) ;
	AddText(   &menu,     0, 0, 15,1, FLAG_CENTRE+FLAG_CONTOUR, "Cr‚ation des zones" ) ;
	AddButton( &menu, 98, 0, 1,  1,1, NO_FLAG, "ş" ) ;

	AddSwitch( &menu,  1, 0, 1, 15,1, FLAG_CENTRE, "ZONE NORD", &zones, CREATE_ZONE_NORD ) ;
	AddSwitch( &menu,  2, 0, 2, 15,1, FLAG_CENTRE, "ZONE SUD",  &zones, CREATE_ZONE_SUD ) ;
	AddSwitch( &menu,  3, 0, 3, 15,1, FLAG_CENTRE, "ZONE EST", &zones, CREATE_ZONE_EST ) ;
	AddSwitch( &menu,  4, 0, 4, 15,1, FLAG_CENTRE, "ZONE OUEST", &zones, CREATE_ZONE_OUEST ) ;

	AddChangeValue( &menu, 10, 0, 6, 15, 1, NO_FLAG, "Hauteur:", &hauteur, 1, 256, 65536 ) ;

	AddButton( &menu, 99, 0, 8, 15,1, FLAG_RED+FLAG_CENTRE+FLAG_CONTOUR, "OK" ) ;

	DrawMenu( &menu, -1, -1 ) ;

	while( flag!=-1 )
	{
		MyGetInput() ;

		select = GereMenu( &menu ) ;
		BoxUpdate() ;// pour la souris

		if( MyKey == K_ESC )	select = 98 ;
		if( MyKey == K_ENTER )	select = 99 ;

		switch( select )
		{
			case 98:	// Exit
			case 99:	// Ok
				flag = -1 ;
				break ;
		}
	}

	if( select!=98 AND zones )	CreateExtZones( zones, hauteur ) ;

	RestoreScreen() ;
}
#endif	// LBA_EDITOR

