#include	"defines.h"

// nom du HQR (citadele.hqr)

// contient:

// Map
// carre de 16*16 Bytes contenant index de chaque cube

// TexGround
// texture 64K 256*256 Bytes -> *GroundTexture

// TexObj
// texture 64K 256*256 Bytes -> *ObjTexture

// liste des cubes (pointe par index (Map) + 3
//	INF infos sur le cube (nb obj decors)
//	DOB list et pos des objets decors (si exist)
//	GRD carre 64*64*2 T_HALF_POLY des polys de sol
//      Y   carre 65*65 S16 sommet altitudes
//	LUM carre 65*65 U8 intensite des sommets
//	TXD liste des pseudos des testures des polys

#define	HQR_MAP_IDM		0
#define	HQR_TEX_GROUND		1
#define	HQR_TEX_OBJ		2

#define	HQR_START_CUBE		3

#define	HQR_CUBE_INF		0
#define	HQR_CUBE_DOB		1
#define	HQR_CUBE_GRD		2
#define	HQR_CUBE_TXD		3
#define	HQR_CUBE_Y		4
#define	HQR_CUBE_LUM		5

#define	HQR_STEP_CUBE		6

extern	void	TheEndCheckFile( char *filename ) ;
extern	void	TheEnd( S32 num, char *error ) ;

//extern	U32	IsleMem ;
extern	U32	IsleObjMem ;
extern	U32	CubeInfosMem ;
extern	U32	ListDecorsMem ;
extern	U32	MapPGroundMem ;
extern	U32	ListTexDefMem ;
extern	U32	MapSommetYMem ;
extern	U32	MapIntensityMem ;

/*══════════════════════════════════════════════════════════════════════════*
	     █     █▀▀▀█ █▀▀▀█ █▀▀▀▄        █    ██▀▀▀ █     █▀▀▀▀
	     ██    ██  █ ██▀▀█ ██  █        ██   ▀▀▀▀█ ██    ██▀▀
	     ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀         ▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

// MAX_OBJ_DECORS pour tous les cubes de toutes les iles
// devra etre ajusté au max
// celui de l'editeur:
// #define		MAX_OBJ_DECORS		200

S32	InitExternalView()
{
#ifdef	LBA_EDITOR
	IsleMapIndex	= (U8*)		Malloc( 256 ) ; // stored file no recover
	if( !IsleMapIndex )	TheEnd( NOT_ENOUGH_MEM, "IsleMapIndex (3DExt)" ) ;

	ObjTexture	= (U8*)		Malloc( 256 * 256 + RECOVER_AREA ) ;
	if( !ObjTexture )	TheEnd( NOT_ENOUGH_MEM, "ObjTexture (3DExt)" ) ;

	GroundTexture	= (U8*)		Malloc( 256 * 256 + RECOVER_AREA ) ;
	if( !GroundTexture )	TheEnd( NOT_ENOUGH_MEM, "GroundTexture (3DExt)" ) ;

	// Gros Malloc ZBuffer
	PtrZBuffer = (U16*)Malloc( SIZE_Z_BUFFER ) ;
	if( !PtrZBuffer )	TheEnd( NOT_ENOUGH_MEM, "PtrZvBuffer (3DExt)" ) ;
#endif

	HQR_Isle_Obj	= HQR_Init_Ressource(	"", IsleObjMem, 200 ) ;
	if( !HQR_Isle_Obj )	TheEnd( NOT_ENOUGH_MEM, "HQR_Isle_Obj (3DExt)" ) ;

	HQR_CubeInfos	= HQR_Init_Ressource(	"", CubeInfosMem, MAX_CUBES_PER_ISLE ) ;
	if( !HQR_CubeInfos )	TheEnd( NOT_ENOUGH_MEM, "HQR_CubeInfos (3DExt)" ) ;

	HQR_ListDecors	= HQR_Init_Ressource(	"", ListDecorsMem, MAX_CUBES_PER_ISLE ) ;
	if( !HQR_ListDecors )	TheEnd( NOT_ENOUGH_MEM, "HQR_ListDecors (3DExt)" ) ;

	HQR_MapPGround	= HQR_Init_Ressource(	"", MapPGroundMem, 200/*MAX_CUBES_PER_ISLE*/ ) ;
	if( !HQR_MapPGround )	TheEnd( NOT_ENOUGH_MEM, "HQR_MapPGround (3DExt)" ) ;

	HQR_ListTexDef	= HQR_Init_Ressource(	"", ListTexDefMem, MAX_CUBES_PER_ISLE ) ;
	if( !HQR_ListTexDef )	TheEnd( NOT_ENOUGH_MEM, "HQR_ListTexDef (3DExt)" ) ;

	HQR_MapSommetY	= HQR_Init_Ressource(	"", MapSommetYMem, MAX_CUBES_PER_ISLE ) ;
	if( !HQR_MapSommetY )	TheEnd( NOT_ENOUGH_MEM, "HQR_MapSommetY (3DExt)" ) ;

	HQR_MapIntensity= HQR_Init_Ressource(	"", MapIntensityMem, MAX_CUBES_PER_ISLE ) ;
	if( !HQR_MapIntensity )	TheEnd( NOT_ENOUGH_MEM, "HQR_MapIntensity (3DExt)" ) ;

	ClipZFar  = 48000 ;
	StartZFog = 12000 ;

	SetFog( StartZFog, ClipZFar) ;

	return TRUE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	ClearExternalView()
{
	HQR_Free_Ressource( HQR_Isle_Obj ) ;
	HQR_Free_Ressource( HQR_CubeInfos ) ;
	HQR_Free_Ressource( HQR_ListDecors ) ;
	HQR_Free_Ressource( HQR_MapPGround ) ;
	HQR_Free_Ressource( HQR_ListTexDef ) ;
	HQR_Free_Ressource( HQR_MapSommetY ) ;
	HQR_Free_Ressource( HQR_MapIntensity ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	LoadIsland()
{
	// reinit ressources HQR

	AddExt( IslandName, "OBL" ) ;
	if( !HQR_Change_Ressource( HQR_Isle_Obj, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	AddExt( IslandName, "ILE" ) ;

	if( !HQR_Change_Ressource( HQR_CubeInfos, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	if( !HQR_Change_Ressource( HQR_ListDecors, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	if( !HQR_Change_Ressource( HQR_MapPGround, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	if( !HQR_Change_Ressource( HQR_ListTexDef, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	if( !HQR_Change_Ressource( HQR_MapSommetY, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	if( !HQR_Change_Ressource( HQR_MapIntensity, IslandName ) )
		TheEndCheckFile( IslandName ) ;

	// Charge buffer permanent par ile

	if( !Load_HQR( IslandName, IsleMapIndex, HQR_MAP_IDM ) )
		TheEndCheckFile( IslandName ) ;

	if( !Load_HQR( IslandName, GroundTexture, HQR_TEX_GROUND ) )
		TheEndCheckFile( IslandName ) ;

	if( !Load_HQR( IslandName, ObjTexture, HQR_TEX_OBJ ) )
		TheEndCheckFile( IslandName ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

S32	LoadCube( S32 x, S32 y )
{
	S32	index ;

	index = y*16 + x ;
	index = IsleMapIndex[index]&127 ; // bug compilo ou debug ?

	if( !index )
	{
		CubeBitField = 0xFFFF ;
		return FALSE ;	// pas de cube
	}

	// index-1 pour vrai index fichier (0 == pas de cube, 1 == 1er cube (num 0) )
	index = HQR_START_CUBE + HQR_STEP_CUBE * (index-1) ;

	ListCubeInfos = (S32 *)HQR_Get( HQR_CubeInfos, index+HQR_CUBE_INF ) ;

	NbObjDecors 	= ListCubeInfos[INFO_NB_DECORS] ;
	AlphaLight  	= ListCubeInfos[INFO_ALPHA_LIGHT]&0xFFFF ;
	CubeBitField 	= (U16)(ListCubeInfos[INFO_ALPHA_LIGHT]>>16) ;
	BetaLight   	= ListCubeInfos[INFO_BETA_LIGHT] ;
	Sky_Y		= ListCubeInfos[INFO_SKY_Y] ;

	ClipZFar  	= ListCubeInfos[INFO_CLIP_Z_FAR] ;
	StartZFog 	= ListCubeInfos[INFO_START_Z_FOG] ;

	OffsetAniPoly1	= ListCubeInfos[INFO_OFFSET_ANI_POLY_1] ;
	OffsetAniPoly2	= ListCubeInfos[INFO_OFFSET_ANI_POLY_2] ;
	OffsetAniPoly3	= ListCubeInfos[INFO_OFFSET_ANI_POLY_3] ;
	OffsetAniPoly4	= ListCubeInfos[INFO_OFFSET_ANI_POLY_4] ;


//********* tests temporaires de protection
	if( !ClipZFar )		ClipZFar  = 48000 ;
	if( !StartZFog )	StartZFog = 12000 ;
//*********

//********* tests temporaires de protection
	if( !OffsetAniPoly3 )	OffsetAniPoly3 = -1 ;
	if( !OffsetAniPoly4 )	OffsetAniPoly4 = -1 ;
//*********

	SetFog( StartZFog, ClipZFar) ;

	if( NbObjDecors != 0 )
	{
		ListDecors = (T_DECORS *)HQR_Get( HQR_ListDecors, index+HQR_CUBE_DOB ) ;
	}

	MapPolyGround = (T_HALF_POLY *)HQR_Get( HQR_MapPGround, index+HQR_CUBE_GRD ) ;

	ListTexDef = (T_HALF_TEX *)HQR_Get( HQR_ListTexDef, index+HQR_CUBE_TXD ) ;

	MapSommetY = (S16 *)HQR_Get( HQR_MapSommetY, index+HQR_CUBE_Y ) ;

	MapIntensity = (U8 *)HQR_Get( HQR_MapIntensity, index+HQR_CUBE_LUM ) ;

	return TRUE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

#define	CJ_WATER        1	// code jeu noyade

extern "C" PTR_U8  	BufSpeak      ;

// initialise un cube vide rempli d'eau (codes jeux compris)

void	InitSeaCube( void )
{
	S32	n ;
	T_HALF_POLY *mappoly ;

	NbObjDecors = 0 ;

	// ATTENTION: explose BufSpeak
	MapSommetY    = (S16*)BufSpeak	;
	MapPolyGround = (T_HALF_POLY*)(BufSpeak+(NB_COTE+1)*(NB_COTE+1)*2) ;// *2 car S16

	mappoly = MapPolyGround ;

	// initialise tous les sommet à 0 (niveau de l'eau)
	memset( MapSommetY, 0, (NB_COTE+1)*(NB_COTE+1)*2 ) ;	// *2 car S16

	// vire tous les polygones de la carte
	memset( MapPolyGround, 0, NB_COTE*NB_COTE*2*sizeof(T_HALF_POLY) ) ;

	// rajoute les codes jeux pour qu'on puisse se noyer
	for( n=0; n<NB_COTE*NB_COTE*2; n++, mappoly++ )
	{
		// ATTENTION: CJ_WATER defini dans ..\common.h redefini juste au-dessus
		mappoly->CodeJeu = CJ_WATER ;
	}

	CubeBitField = 0xFFFF ;
}
