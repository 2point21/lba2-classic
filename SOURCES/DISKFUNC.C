#include	"c_extern.h"
#include	"direct.h"

#include	<dos.h>

UBYTE	*PtrSce ;
UBYTE	*PtrScene ;

#define	GET_BYTE	(*PtrSce++)
#define	GET_WORD	(*(WORD*)PtrSce) ; PtrSce+=2

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	     Û฿฿฿Ü  Û    ÛÛ฿฿฿ Û  Ü฿       Û฿฿฿฿ Û   Û ÛÛÜ Û Û฿฿฿฿
	     ÛÛ  Û  ÛÛ   ฿฿฿฿Û ÛÛ฿Ü        ÛÛ฿฿  ÛÛ  Û ÛÛ฿ÛÛ ÛÛ
	     ฿฿฿฿   ฿฿   ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿    ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	GetDiskEnv( UBYTE *progpath )
{
	_splitpath( (char*)progpath, ProgDrive, ProgDir, Name, Ext ) ;
	getcwd( OrgDir, _MAX_DIR ) ;
	_dos_getdrive( &OrgDrive ) ;
}

void	RestoreDiskEnv()
{
	ULONG	total ;

	_dos_setdrive( OrgDrive, &total ) ;
	chdir( OrgDir ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	  ÛÛ฿฿฿ Û฿฿฿฿ Û฿฿฿฿ ÛÛÜ Û Û฿฿฿฿       Û฿฿฿฿  Û    Û     Û฿฿฿฿
	  ฿฿฿฿Û ÛÛ    ÛÛ฿฿  ÛÛ฿ÛÛ ÛÛ฿฿        ÛÛ฿฿   ÛÛ   ÛÛ    ÛÛ฿฿
	  ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿     ฿฿   ฿฿฿฿฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

WORD	LoadScene( WORD numscene )
{
	T_OBJET	*ptrobj ;
	WORD	n, info3 ;
	UBYTE	string[256] ;
	UBYTE	mess[256] ;
	WORD	sizetoload ;
	WORD	indexfile3d ;

//	PtrScene = PtrSce = LoadMalloc_HQR( PATH_RESSOURCE"scene.hqr", numscene ) ;

	HQRM_Load( PATH_RESSOURCE"scene.hqr", numscene, &PtrScene ) ;
	CHECK_MEMORY

	PtrSce = PtrScene ;

	if( !PtrSce )
	{
		Message( "Scene.Hqr introuvable ou pas assez mem", TRUE ) ;
	}

// info world: INFO_WORLD

		NewCube = numscene ;

		Island = GET_BYTE ;

		GameOverCube = GET_BYTE ;

		n = GET_WORD ;
		n = GET_WORD ;

//		ListHoloObj[NUM_PERSO].Alpha = GET_WORD ;
//		ListHoloObj[NUM_PERSO].Beta = GET_WORD ;
//		ListHoloObj[NUM_PERSO].Size = GET_WORD ;

// ambiance: AMBIANCE

		AlphaLight = GET_WORD ;
		BetaLight = GET_WORD ;

		SampleAmbiance[0] = GET_WORD ;
		SampleRepeat[0] = GET_WORD ;
		SampleRnd[0] = GET_WORD ;
		SampleAmbiance[1] = GET_WORD ;
		SampleRepeat[1] = GET_WORD ;
		SampleRnd[1] = GET_WORD ;
		SampleAmbiance[2] = GET_WORD ;
		SampleRepeat[2] = GET_WORD ;
		SampleRnd[2] = GET_WORD ;
		SampleAmbiance[3] = GET_WORD ;
		SampleRepeat[3] = GET_WORD ;
		SampleRnd[3] = GET_WORD ;
		SecondMin = GET_WORD ;
		SecondEcart = GET_WORD ;

		CubeJingle = GET_BYTE ;
//		PlayMidiFile( CubeJingle ) ;

// hero inits: HERO_START

		ptrobj = ListObjet ;

		CubeStartX = GET_WORD ;
		CubeStartY = GET_WORD ;
		CubeStartZ = GET_WORD ;

		sizetoload = GET_WORD ;
		ptrobj->PtrTrack = PtrSce ;
		PtrSce += sizetoload ;

		sizetoload = GET_WORD ;
		ptrobj->PtrLife = PtrSce ;
		PtrSce += sizetoload ;

		ptrobj++ ;

// objets: OBJECT

		NbObjets = GET_WORD ;
		for( n=1; n<NbObjets; n++, ptrobj++ )
		{
			InitObject( n ) ;

			ptrobj->Flags = GET_WORD ;

			indexfile3d = GET_WORD ;

			if( !(ptrobj->Flags & SPRITE_3D) )
			{

	HQRM_Load( PATH_RESSOURCE"File3D.hqr", indexfile3d, &ptrobj->PtrFile3D ) ;
	CHECK_MEMORY

/*				ptrobj->PtrFile3D =
					LoadMalloc_HQR(
						PATH_RESSOURCE"File3D.hqr",
						indexfile3d ) ;
*/

			}

			ptrobj->GenBody = GET_BYTE ;
			ptrobj->GenAnim = GET_BYTE ;
/*
if( n==5 )
{
	CoulText( 15,0 ) ;
	Text( 10, 300, "obj 5 org anim: %d", ptrobj->GenAnim ) ;
}
*/

			ptrobj->Sprite = GET_WORD ;

			ptrobj->OldPosX = ptrobj->PosObjX = GET_WORD ;
			ptrobj->OldPosY = ptrobj->PosObjY = GET_WORD ;
			ptrobj->OldPosZ = ptrobj->PosObjZ = GET_WORD ;
			ptrobj->HitForce = GET_BYTE ;
			ptrobj->OptionFlags = GET_WORD ;
			ptrobj->OptionFlags &= ~EXTRA_GIVE_NOTHING ;
			ptrobj->Beta = GET_WORD ;
			ptrobj->SRot = GET_WORD ;
			ptrobj->Move = GET_WORD ;

			ptrobj->Info  = GET_WORD ;
			ptrobj->Info1 = GET_WORD ;
			ptrobj->Info2 = GET_WORD ;
			ptrobj->Info3 = GET_WORD ;

			ptrobj->NbBonus = GET_BYTE ;
			ptrobj->CoulObj = GET_BYTE ;
			ptrobj->Armure = GET_BYTE ;
			ptrobj->LifePoint = GET_BYTE ;

			sizetoload = GET_WORD ;
			ptrobj->PtrTrack = PtrSce ;
			PtrSce = PtrSce + sizetoload ;

			sizetoload = GET_WORD ;
			ptrobj->PtrLife = PtrSce ;
			PtrSce = PtrSce + sizetoload ;
		}

// zone declechement: ZONE

		NbZones = GET_WORD ;
		ListZone = (T_ZONE*)PtrSce ;
		PtrSce += NbZones * 12 * 2 ;

// point track: TRACK

		NbBrickTrack = GET_WORD ;
		ListBrickTrack = (T_TRACK*)PtrSce ;

		return TRUE ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			 Û฿฿฿Ü Û฿฿฿฿ Û฿฿Û  Û   Û Û฿฿฿฿
			 ÛÛ  Û ÛÛ฿฿  ÛÛ฿฿Û ÛÛ  Û ÛÛ ฿Û
			 ฿฿฿฿  ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

/*
void	*LoadTestMalloc( char *filename )
{
	void	*ptr ;
	UBYTE	string[256] ;

	ptr = LoadMalloc( filename ) ;

	if( !ptr )
	{
		strcpy( string, "ARG ! Mem:" ) ;
		strcat( string, Itoa( (ULONG)Malloc(-1) ) ) ;
		strcat( string, " fichier:" ) ;
		strcat( string, filename ) ;

		Message( string, TRUE ) ;
	}
	return ptr ;
}
*/

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

