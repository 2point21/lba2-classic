/*══════════════════════════════════════════════════════════════════════════*
	     ██▀▀▀ █▀▀▀█ █   █ █▀▀▀▀       █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀
	     ▀▀▀▀█ ██▀▀█ ██ ▄▀ ██▀▀        ██ ▀█ ██▀▀█ ██▀ █ ██▀▀
	     ▀▀▀▀▀ ▀▀  ▀ ▀▀▀   ▀▀▀▀▀       ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

#include 	"c_extern.h"

extern	char	PlayerName[MAX_SIZE_PLAYER_NAME+1] ;

extern	S32	LastMyJoy ;
extern	S32	LastMyFire ;
extern	S32	LastJoyFlag ;

#ifndef	EDITLBA2
#ifdef __cplusplus
extern "C" U8	*PtrLib3DBufferAnim ;	// P_ANIM.ASM (lib_3D)
#else
extern	U8	*PtrLib3DBufferAnim ;	// P_ANIM.ASM (lib_3D)
#endif

#pragma	aux	PtrLib3DBufferAnim	"*"
#endif

extern	S32	InvSelect 	    ;	// GAMEMENU.C

U8	*PtrSave ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||defined(EDITLBA2)
T_REAL_VALUE_HR	TempoRealAngle ;
S32		LastStepFalling ;
S32		LastStepShifting ;
#endif

#ifdef	DEMO
S32	CurrentDemoSave = 0 ;
S32	NewDemoSave = 0 ;

char	*ListDemoSave[NB_DEMOSAVE] = {
	"demo0.lba",
	"demo1.lba",
	"demo2.lba"
	} ;
#endif

/*══════════════════════════════════════════════════════════════════════════*
    █▀▀█  █   █ █▀▀▀▀ █▀▀▀▀ █▀▀▀▀ █▀▀▀█       █▀▀▀▀ █   █ ██▄ █ █▀▀▀▀ ██▀▀▀
    ██▀▀█ ██  █ ██▀▀  ██▀▀  ██▀▀  ██▀█▀       ██▀▀  ██  █ ██▀██ ██    ▀▀▀▀█
    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀    ▀▀    ▀▀▀▀▀ ▀▀  ▀       ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
#define	PtrOpen()	PtrSave=Screen

//-------- Lecture
#define	LbaRead(ptr,size)	memcpy( ptr, PtrSave, size ) ;\
				PtrSave += size ;

#define	LbaReadByte(ptr)	ptr = *PtrSave++ ;

#define	LbaReadWord(ptr)	ptr = *(S16*)PtrSave ;\
				PtrSave += 2 ;

#define	LbaReadLong(ptr)	ptr = *(S32*)PtrSave ;\
				PtrSave += 4 ;

//-------- Ecriture
#define	LbaWrite(ptr,size)	memcpy( PtrSave, ptr, size ) ;\
				PtrSave += size ;

#define	LbaWriteByte(val)	*PtrSave++ = val ;

#define	LbaWriteWord(val)	*(S16*)PtrSave = val ;\
				PtrSave += 2 ;

#define	LbaWriteLong(val)	*(S32*)PtrSave = val ;\
				PtrSave += 4 ;

/*──────────────────────────────────────────────────────────────────────────*/
// Resultat pas terrible
#ifndef	EDITLBA2
void	RemapPicture( U8 *buf, U32 size )
{
	S32	n ;
	U8	newpal[256] ;
	U8	*ptr ;
	U8	r,v,b ;

//	if( PtrPal==PtrPalNormal )	return ;
	// Mettre un moyen plus clean !!!!!!
	if( !memcmp(PtrPal,PtrPalNormal,768) )	return ;

	ptr = PtrPal ;
	for( n=0; n<768; n++ )	*ptr++ >>= 2 ;

	ptr = PtrPalNormal ;
	for( n=0; n<768; n++ )	*ptr++ >>= 2 ;

//	ptr = PtrPal+10*3 ;
	ptr = PtrPal ;
//	for( n=10; n<245; n++ )
	for( n=0; n<255; n++ )
	{
		// ATTENTION: ne pas passer les (*ptr++) directement en
		// parametre sinon, l'empilage du C passerait b,v,r
		r = *ptr++ ;
		v = *ptr++ ;
		b = *ptr++ ;

		newpal[n] = SearchBoundColRGB( r, v, b,
					       PtrPalNormal, 0, 255 ) ;
//		newpal[n] = SearchBoundColRGB( r, v, b,
//					       PtrPalNormal, 10, 245 ) ;
	}

	for( n=0; n<size; n++, buf++ )
	{
		*buf = newpal[*buf] ;
	}

	ptr = PtrPal ;
	for( n=0; n<768; n++ )	*ptr++ <<= 2 ;

	ptr = PtrPalNormal ;
	for( n=0; n<768; n++ )	*ptr++ <<= 2 ;
}

#ifdef	DEMO
S32	SetDemoSaveGame( S32 num )
{
	NewDemoSave = CurrentDemoSave = num ;

	if( num<NB_DEMOSAVE )
	{
		strcpy( GamePathname, PATH_SAVE ) ;
		strcat( GamePathname, ListDemoSave[CurrentDemoSave] ) ;

		return TRUE ;
	}

	return FALSE ;
}
#endif	// DEMO

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▄  █    ██▀▀▀ █  ▄▀       █▀▀▀▀ █   █ ██▄ █ █▀▀▀▀ ██▀▀▀
	  ██  █  ██   ▀▀▀▀█ ██▀▄        ██▀▀  ██  █ ██▀██ ██    ▀▀▀▀█
	  ▀▀▀▀   ▀▀   ▀▀▀▀▀ ▀▀  ▀       ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
// Version Jeu uniquement
void	AutoSaveGame()
{
#ifndef	DEMO
	U8	memonumversion = NumVersion ;
	char	memoplayername[MAX_SIZE_PLAYER_NAME+1] ;
	char	memogamepathname[_MAX_PATH] ;

	if( DemoSlide OR PLAY_THE_END )	return ;

	SaveTimer() ;

	strcpy( memoplayername, PlayerName ) ;
	strcpy( memogamepathname, GamePathname ) ;

	strcpy( GamePathname, AUTOSAVE_FILENAME ) ;
	strcpy( PlayerName, AUTOSAVE_NAME ) ;

#ifdef	DEBUG_TOOLS
	{
	// ATTENTION : tempo uniquement interne a adeline
	char *pt ;

	DefFileBufferInit( "C:\\DOS\\EXTEND\\NAME.ME", BufSpeak, 640*480 ) ;

	pt = DefFileBufferReadString( "Poste" )	;

	if( pt AND *pt )
	{
		strcat( PlayerName, " " ) ;
		strncat( PlayerName, pt, MAX_SIZE_PLAYER_NAME-9 )  ;
	}
	}
#endif

	NumVersion = NUM_VERSION ;	// Sauvegarde non compactee

	SaveGame( FALSE ) ;

	NumVersion = memonumversion ;
	strcpy( PlayerName, memoplayername ) ;
	strcpy( GamePathname, memogamepathname ) ;

	RestoreTimer() ;
#endif	// DEMO
}

/*──────────────────────────────────────────────────────────────────────────*/
void	CurrentSaveGame()
{
	U8	memonumversion = NumVersion ;
	char	memoplayername[MAX_SIZE_PLAYER_NAME+1] ;
	char	memogamepathname[_MAX_PATH] ;

	if( DemoSlide OR PLAY_THE_END )	return ;

	SaveTimer() ;

	strcpy( memoplayername, PlayerName ) ;
	strcpy( memogamepathname, GamePathname ) ;

	strcpy( GamePathname, CURRENTSAVE_FILENAME ) ;
	strcpy( PlayerName, CURRENTSAVE_NAME ) ;

#ifdef	DEBUG_TOOLS
	{
	// ATTENTION : tempo uniquement interne a adeline
	char *pt ;

	DefFileBufferInit( "C:\\DOS\\EXTEND\\NAME.ME", BufSpeak, 640*480 ) ;

	pt = DefFileBufferReadString( "Poste" )	;

	if( pt AND *pt )
	{
		strcat( PlayerName, " " ) ;
		strncat( PlayerName, pt, MAX_SIZE_PLAYER_NAME-9 )  ;
	}
	}
#endif

	NumVersion = NUM_VERSION ;	// Sauvegarde non compactee

	SaveGame( FALSE ) ;

	NumVersion = memonumversion ;
	strcpy( PlayerName, memoplayername ) ;
	strcpy( GamePathname, memogamepathname ) ;

	RestoreTimer() ;
}
#endif	// EDITLBA2

/*──────────────────────────────────────────────────────────────────────────*/
void	SaveGame( S32 flagmess )
{
	U8	*sizeptr ;
	U8	*memoptr ;
	S32	sizefile ;
#ifndef	EDITLBA2
	S32	savetimerrefhr ;
//	char string[30] ;
//	S32	x0, y0, x1, y1 ;

	// Image 160x120
	ScaleBox( 0, 0, 639, 479, Log, 0, 0, 159, 119, BufSpeak+50000L ) ;	// old Screen
	SaveBlock( BufSpeak+50000L, BufSpeak+150000L, 0, 0, 159, 119 ) ;
	RemapPicture( BufSpeak+150000L, 160*120 ) ;

	if( flagmess )
	{
/*		strcpy( string, "Sauvegarde en cours ..." ) ;
		ColorFont( LBAWHITE ) ;
		// cadre
		x0 = 320 - SizeFont( string )/2 - 50 ;
		x1 = 320 + SizeFont( string )/2 + 50 ;
		y0 = 240-25 ;
		y1 = 240+25 ;
		ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
		BackupAngles( x0, y0, x1, y1 ) ;
		ShadeBoxBlk( x0, y0, x1, y1, MENU_SHADE_LVL ) ;
		DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
		RestoreAngles( x0, y0, x1, y1 ) ;
		Font( x0+50, y0+7, string ) ;
		BoxStaticFullflip() ;
*/	}

	RestoreTimer() ;
	savetimerrefhr = TimerRefHR ;
	SaveTimer() ;

	PtrSave = BufSpeak+50000L ;	// old Screen
#else
	PtrSave = (U8*)Screen ;
#endif

	if( flagmess )	// non AUTOSAVE ni CURRENTSAVE
	{
		NumVersion &= SAVE_COMPRESS ;	// ecrase old num version
		NumVersion |= NUM_VERSION ;
	}

	LbaWriteByte( NumVersion ) ;

#ifdef	EDITLBA2
	LbaWriteLong( NewCube ) ;
#else
	LbaWriteLong( NumCube ) ;
#endif

	LbaWrite( PlayerName, strlen(PlayerName)+1 ) ;

	if( NumVersion&SAVE_COMPRESS )
	{
		sizeptr = PtrSave ;
		LbaWriteLong( sizefile ) ;
		memoptr = PtrSave ;
	}

#ifndef	EDITLBA2
	LbaWrite( BufSpeak+150000L, 160*120 ) ;
#else
	LbaWrite( PtrScreenSave, 160*120 ) ;
#endif

#ifdef	EDITLBA2
	SaveContexte( KeepTimerRefHR ) ;
#else
	SaveContexte( savetimerrefhr ) ;
#endif

	// sauve position valide si nécessaire
	LbaWriteLong( ValidePos ) ;
	LbaWriteLong( LastValidePos ) ;

	if( !ValidePos )
	{
		LbaWriteLong( ValideCube ) ;
		LbaWriteLong( SizeOfBufferValidePos ) ;
		LbaWrite( BufferValidePos, SizeOfBufferValidePos ) ;
	}

	if( NumVersion&SAVE_COMPRESS )	// Version compactee
	{
		S32	size ;

		sizefile = PtrSave-memoptr ;

		size = Compress_LZSS( (char *)memoptr, (char *)PtrSave, sizefile ) ;
		memcpy( memoptr, PtrSave, size ) ;

		PtrSave = sizeptr ;
		LbaWriteLong( sizefile ) ;
		PtrSave = memoptr+size ;
	}

#ifndef	EDITLBA2
	Save( GamePathname, BufSpeak+50000L, PtrSave-(BufSpeak+50000L) ) ;
#else
	Save( GamePathname, (U8*)Screen, PtrSave-(U8*)Screen ) ;
#endif
}

// Version 4 : LBA II only !!
#define	LoadGameV4(name)	Load( name, (U8*)Screen ) ;\
				PtrSave = (U8*)Screen ;

#ifndef	EDITLBA2
/*──────────────────────────────────────────────────────────────────────────*/
void	LoadGameNumCube( )
{
	U8	numversion ;
	U8	wbyte ;
	U8	*ptr  ;

	LoadGameV4( GamePathname ) ;

	LbaReadByte( numversion ) ;	// num version
	LbaReadLong( NewCube ) ;

	ptr = (U8 *)PlayerName ;
	do
	{
		LbaReadByte( wbyte ) ;
		*ptr++ = wbyte ;

	} while( wbyte != 0 ) ;

	if( numversion&SAVE_COMPRESS )	// Version compactee
	{
		U8	*ptrdecomp ;
		S32	sizefile ;
		S32	size ;

		LbaReadLong( sizefile ) ;
		size = FileSize( GamePathname )-(PtrSave-(U8*)Screen) ;

		ptrdecomp = (U8*)PtrSave + sizefile + RECOVER_AREA ;
		memcpy( ptrdecomp, PtrSave, size ) ;
		ExpandLZ( PtrSave, ptrdecomp, sizefile, 2 ) ;
	}

	PtrSave += 160*120 ;	// Skip Image

	// list Var game
	LbaRead( ListVarGame, MAX_VARS_GAME*sizeof(ListVarGame[0]) ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	LoadGamePlayerName( )
{
	U8	wbyte ;
	S32	newcube ;
	U8	*ptr ;

#ifdef	EDITLBA2
	PtrSave = BufTempo ;
#else
	PtrSave = BufSpeak+50000L ;
#endif

	Load( GamePathname, PtrSave ) ;

	LbaReadByte( wbyte ) ;	// num version
	LbaReadLong( newcube ) ;

	ptr = (U8 *)PlayerName ;
	do
	{
		LbaReadByte( wbyte ) ;
		*ptr++ = wbyte ;

	} while( wbyte != 0 ) ;
}
#endif

#define	NB_GAME_CHOICE	5

/*──────────────────────────────────────────────────────────────────────────*/
U8	*LoadGameScreen( )
{
	U8	wbyte ;
	U8	*ptr ;

#ifdef	EDITLBA2
	PtrSave = BufTempo ;
#else
	PtrSave = BufSpeak+50000L ;
#endif

	if( !Load( GamePathname, PtrSave ) )	return( NULL ) ;

	LbaReadByte( NumVersion ) ;	// num version
	LbaReadLong( NewCube ) ;

	ptr = (U8 *)PlayerName ;
	do
	{
		LbaReadByte( wbyte ) ;
		*ptr++ = wbyte ;

	} while( wbyte != 0 ) ;

	if( NumVersion&SAVE_COMPRESS )	// Version compactee
	{
		U8	*ptrdecomp ;
		S32	sizefile ;
		S32	size ;

		LbaReadLong( sizefile ) ;
#ifdef	EDITLBA2
		size = FileSize( GamePathname )-(PtrSave-BufTempo) ;
#else
		size = FileSize( GamePathname )-(PtrSave-(BufSpeak+50000L)) ;
#endif

		ptrdecomp = (U8*)PtrSave + sizefile + RECOVER_AREA ;
		memcpy( ptrdecomp, PtrSave, size ) ;
		ExpandLZ( PtrSave, ptrdecomp, sizefile, 2 ) ;
	}

	return( PtrSave ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	LoadGame( void )
{
	U8	*ptr ;
	U8	wbyte ;
#ifndef	EDITLBA2
	S32	flaginit = FALSE ;
	S32	savetimerrefhr ;

	HQ_StopSample() ;
#endif

	LoadGameV4( GamePathname ) ;

	LbaReadByte( NumVersion ) ;	// num version
	LbaReadLong( NewCube ) ;

	ptr = (U8 *)PlayerName ;
	do
	{
		LbaReadByte( wbyte ) ;
		*ptr++ = wbyte ;

	} while( wbyte != 0 ) ;

	if( NumVersion&SAVE_COMPRESS )	// Version compactee
	{
		U8	*ptrdecomp ;
		S32	sizefile ;
		S32	size ;

		LbaReadLong( sizefile ) ;
		size = FileSize( GamePathname )-(PtrSave-(U8*)Screen) ;

		ptrdecomp = (U8*)PtrSave + sizefile + RECOVER_AREA ;
		memcpy( ptrdecomp, PtrSave, size ) ;
		ExpandLZ( PtrSave, ptrdecomp, sizefile, 2 ) ;

#ifdef	EDITLBA2
		SizeFile = sizefile + (PtrSave-(U8*)Screen-sizeof(sizefile)) ;
		CompressedSizeFile = size + (PtrSave-(U8*)Screen) ;
#endif
	}
#ifdef	EDITLBA2
	else
	{
		SizeFile = FileSize( GamePathname ) ;
		CompressedSizeFile = 0 ;
	}
#endif

	PtrSave += 160*120 ;

#ifdef	EDITLBA2
	LoadContexte( &KeepTimerRefHR ) ;
#else
	flaginit = LoadContexte( &savetimerrefhr ) ;
#endif

	// recupere position valide si nécessaire
	LbaReadLong( ValidePos ) ;
	LbaReadLong( LastValidePos ) ;

	if( !ValidePos )
	{
		LbaReadLong( ValideCube ) ;
		LbaReadLong( SizeOfBufferValidePos ) ;
		LbaRead( BufferValidePos, SizeOfBufferValidePos ) ;
	}

#ifndef	EDITLBA2
	RestartMusic = TRUE ;
	ChoicePalette() ;

	CameraCenter( 0 ) ;
	SetTimerHR( savetimerrefhr ) ;
	SaveTimer() ;

	return flaginit ;
#else
	return	FALSE ;
#endif
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	LoadGameOldVersion( void )
{
	T_INV_OBJ	*ptri ;
	T_ARROW		*ptra ;
	S32		n ;
	U8		*ptr ;
	S32		savetimerrefhr ;
	S32		wlong ;
	U8		wbyte ;

	LoadGameV4( GamePathname ) ;

	LbaReadByte( NumVersion ) ;	// num version
	LbaReadLong( NewCube ) ;

	ptr = (U8 *)PlayerName ;
	do
	{
		LbaReadByte( wbyte ) ;
		*ptr++ = wbyte ;

	} while( wbyte != 0 ) ;

	if( NumVersion&SAVE_COMPRESS )	// Version compactee
	{
		U8	*ptrdecomp ;
		S32	sizefile ;
		S32	size ;

		LbaReadLong( sizefile ) ;
		size = FileSize( GamePathname )-(PtrSave-(U8*)Screen) ;

		ptrdecomp = (U8*)PtrSave + sizefile /*- size*/ + RECOVER_AREA ;
		memcpy( ptrdecomp, PtrSave, size ) ;
		ExpandLZ( PtrSave, ptrdecomp, sizefile, 2 ) ;

#ifdef	EDITLBA2
		SizeFile = sizefile + (PtrSave-(U8*)Screen-sizeof(sizefile)) ;
		CompressedSizeFile = size + (PtrSave-(U8*)Screen) ;
#endif
	}
#ifdef	EDITLBA2
	else
	{
		SizeFile = FileSize( GamePathname ) ;
		CompressedSizeFile = 0 ;
	}
#endif

	PtrSave += 160*120 ;

	// list Var game and cube
#ifdef	EDITLBA2
	LbaRead( SaveListVarGame, MAX_VARS_GAME*sizeof(SaveListVarGame[0]) ) ;
	LbaRead( SaveListVarCube, MAX_VARS_CUBE*sizeof(SaveListVarCube[0]) ) ;
#else
	LbaRead( ListVarGame, MAX_VARS_GAME*sizeof(ListVarGame[0]) ) ;
	LbaRead( ListVarCube, MAX_VARS_CUBE*sizeof(ListVarCube[0]) ) ;
#endif

	LbaReadByte( Comportement ) ;

#ifndef	EDITLBA2
	SaveComportement = Comportement ;
#endif

	LbaReadLong( wlong ) ;

	NbGoldPieces = wlong&0xFFFF ;
	NbZlitosPieces = (wlong>>16)&0xFFFF ;

	LbaReadByte( MagicLevel ) ;
	LbaReadByte( MagicPoint ) ;
	LbaReadByte( NbLittleKeys ) ;
	LbaReadWord(  NbCloverBox ) ;

	LbaReadLong( SceneStartX ) ;
	LbaReadLong( SceneStartY ) ;
	LbaReadLong( SceneStartZ ) ;

	LbaReadLong( StartXCube ) ;
	LbaReadLong( StartYCube ) ;
	LbaReadLong( StartZCube ) ;

	LbaReadByte( Weapon ) ;

	LbaReadLong( savetimerrefhr ) ;

	LbaReadByte( NumObjFollow ) ;

	LbaReadByte( SaveComportementHero ) ;
	LbaReadByte( SaveBodyHero ) ;

	ptra = TabArrow ;

	for( n=0; n<(MAX_OBJECTIF+MAX_CUBE); n++, ptra++ )
	{
		LbaReadByte( wbyte ) ;
		wbyte &= 3 ;// seuls les 2 bits de poids faible sont
			    // susceptibles de bouger
		ptra->FlagHolo |= wbyte ;
	}

	// used inventory
	ptri = TabInv ;

	for( n=0; n<MAX_INVENTORY; n++, ptri++ )
	{
		LbaReadLong( ptri->PtMagie ) ;
		LbaReadLong( ptri->FlagInv ) ;
		LbaReadWord( ptri->IdObj3D ) ;
	}

	// Init position StartPos
#ifndef	EDITLBA2
//	SceneStartX = CubeStartX ;
//	SceneStartY = CubeStartY ;
//	SceneStartZ = CubeStartZ ;
	ListObjet[NUM_PERSO].Obj.X = SceneStartX ;
	ListObjet[NUM_PERSO].Obj.Y = SceneStartY ;
	ListObjet[NUM_PERSO].Obj.Z = SceneStartZ ;

	StartXCube = ListObjet[NumObjFollow].Obj.X/SIZE_BRICK_XZ ;
	StartYCube = (ListObjet[NumObjFollow].Obj.Y+SIZE_BRICK_Y)/SIZE_BRICK_Y ;
	StartZCube = ListObjet[NumObjFollow].Obj.Z/SIZE_BRICK_XZ ;

	if( FlagChgCube == 2
	OR  FlagChgCube == 0 )
	{
		CameraCenter( 1 ) ;
	}
	else
	{
		CameraCenter( 0 ) ;
	}
#else
	SceneStartX = -1 ;
#endif

	return TRUE ;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Sauvegarde de l'etat du jeu dans PtrSave
void	SaveContexte( S32 savetimerrefhr )
{
	T_OBJET		*ptrobj ;
	S_PART_FLOW	*ptrf ;
	T_INV_OBJ	*ptri ;
	T_DART		*ptrd ;
	T_ZONE		*ptrz ;
	T_ARROW		*ptra ;
	S32	n ;
#ifndef	EDITLBA2
	U8	wbyte ;
	U8	wbyte2 ;
	U8	*saveptr ;
	U8	*saveptr2 ;
	T_PATCH		*ptrpatch ;
	T_EXTRA		*ptre ;
	T_INCRUST_DISP	*ptrdisp ;
	S_ONE_DOT	*ptrpt ;
	S32	i ;
#endif

	// list vars game et cube
#ifdef	EDITLBA2
	LbaWrite( SaveListVarGame, MAX_VARS_GAME*sizeof(SaveListVarGame[0]) ) ;
	LbaWrite( SaveListVarCube, MAX_VARS_CUBE*sizeof(SaveListVarCube[0]) ) ;
#else
	LbaWrite( ListVarGame, MAX_VARS_GAME*sizeof(ListVarGame[0]) ) ;
	LbaWrite( ListVarCube, MAX_VARS_CUBE*sizeof(ListVarCube[0]) ) ;
#endif

	// Globales
	LbaWriteByte( Comportement ) ;

	LbaWriteLong( ((NbZlitosPieces<<16)+(NbGoldPieces&0xFFFF)) ) ;
	LbaWriteByte( MagicLevel ) ;
	LbaWriteByte( MagicPoint ) ;
	LbaWriteByte( NbLittleKeys ) ;
	LbaWriteWord( NbCloverBox ) ;

	LbaWriteLong( SceneStartX ) ;
	LbaWriteLong( SceneStartY ) ;
	LbaWriteLong( SceneStartZ ) ;

	LbaWriteLong( StartXCube ) ;
	LbaWriteLong( StartYCube ) ;
	LbaWriteLong( StartZCube ) ;

	LbaWriteByte( Weapon ) ;

	LbaWriteLong( savetimerrefhr ) ;
	LbaWriteByte( NumObjFollow ) ;

	LbaWriteByte( SaveComportementHero ) ;
	LbaWriteByte( SaveBodyHero ) ;

	ptra = TabArrow ;

	for( n=0; n<(MAX_OBJECTIF+MAX_CUBE); n++, ptra++ )
	{
		LbaWriteByte( ptra->FlagHolo ) ;
	}

	// used inventory
	ptri = TabInv ;

	for( n=0; n<MAX_INVENTORY; n++, ptri++ )
	{
		LbaWriteLong( ptri->PtMagie ) ;
		LbaWriteLong( ptri->FlagInv ) ;
		LbaWriteWord( ptri->IdObj3D ) ;
	}

	LbaWriteLong( Checksum )	;

	// Debut d'incompatibilité des sauvegardes

	LbaWriteLong( LastMyFire ) ;
	LbaWriteLong( LastMyJoy ) ;
	LbaWriteLong( LastInput ) ;
	LbaWriteLong( LastJoyFlag ) ;
	LbaWriteByte( Bulle ) ;
	LbaWriteByte( ActionNormal ) ;
	LbaWriteLong( InventoryAction ) ;
	LbaWriteLong( MagicBall ) ;
	LbaWriteByte( MagicBallType ) ;
	LbaWriteByte( MagicBallCount ) ;
	LbaWriteLong( MagicBallFlags ) ;
	LbaWriteByte( FlagClimbing ) ;
	LbaWriteLong( StartYFalling ) ;
	LbaWriteByte( CameraZone ) ;
	LbaWriteLong( InvSelect ) ;
	LbaWriteLong( ExtraConque ) ;
	LbaWriteByte( PingouinActif ) ;

	LbaWriteLong( (S32)PtrZoneClimb ) ;

	// Flechettes
	ptrd = ListDart ;

	for( n=0; n<MAX_DARTS; n++, ptrd++ )
	{
		LbaWriteLong( ptrd->PosX ) ;
		LbaWriteLong( ptrd->PosY ) ;
		LbaWriteLong( ptrd->PosZ ) ;
		LbaWriteLong( ptrd->Alpha ) ;
		LbaWriteLong( ptrd->Beta ) ;
		LbaWriteLong( ptrd->NumCube ) ;
		LbaWriteLong( ptrd->Flags ) ;
	}
//		LbaWrite( ptrd, sizeof( T_DART ) ) ;

	// Objets
	LbaWriteLong( NbObjets ) ; // nb d'objets pour EditLba2

	ptrobj = ListObjet ;

	for( n=0; n<NbObjets; n++, ptrobj++ )
	{
		LbaWriteByte( ptrobj->GenBody ) ;
		LbaWriteByte( ptrobj->Col ) ;
		LbaWriteWord( ptrobj->GenAnim ) ;
		LbaWriteWord( ptrobj->NextGenAnim ) ;
		LbaWriteLong( ptrobj->OldPosX ) ;
		LbaWriteLong( ptrobj->OldPosY ) ;
		LbaWriteLong( ptrobj->OldPosZ ) ;
		LbaWriteLong( ptrobj->Info ) ;
		LbaWriteLong( ptrobj->Info1 ) ;
		LbaWriteLong( ptrobj->Info2 ) ;
		LbaWriteLong( ptrobj->Info3 ) ;

		LbaWrite( &(ptrobj->Coord), sizeof(ptrobj->Coord) ) ;

		LbaWriteWord( ptrobj->SizeSHit ) ;
		LbaWriteByte( ptrobj->HitBy ) ;
		LbaWriteByte( ptrobj->HitForce ) ;
		LbaWriteWord( ptrobj->LifePoint ) ;
		LbaWriteWord( ptrobj->OptionFlags ) ;

//	U8	*PtrAnimAction ;

		LbaWriteWord( ptrobj->Sprite ) ;
		LbaWriteWord( ptrobj->OffsetLabelTrack ) ;

//	T_OBJ_3D Obj	;
//	U8	*PtrFile3D ;

		LbaWriteLong( ptrobj->IndexFile3D ) ;

//	S16	NbBonus ;
		LbaWriteByte( ptrobj->Armure ) ;
//	U8	CoulObj ;

		LbaWriteWord( ptrobj->XMin ) ;
		LbaWriteWord( ptrobj->XMax ) ;
		LbaWriteWord( ptrobj->YMin ) ;
		LbaWriteWord( ptrobj->YMax ) ;
		LbaWriteWord( ptrobj->ZMin ) ;
		LbaWriteWord( ptrobj->ZMax ) ;

		LbaWriteLong( ptrobj->OldBeta ) ;
		LbaWrite( &(ptrobj->BoundAngle),sizeof(ptrobj->BoundAngle) ) ;
//	U8	*PtrTrack ;
		LbaWriteWord( ptrobj->OffsetTrack ) ;
		LbaWriteWord( ptrobj->SRot ) ;
		LbaWriteWord( ptrobj->OffsetLife ) ;
		LbaWriteWord( ptrobj->AnimDial ) ;
		LbaWriteWord( ptrobj->CarryBy ) ;
		LbaWriteByte( ptrobj->Move ) ;
		LbaWriteByte( ptrobj->ObjCol ) ;
		LbaWriteWord( ptrobj->ZoneSce ) ;
		LbaWriteWord( ptrobj->LabelTrack ) ;
		LbaWriteWord( ptrobj->MemoLabelTrack ) ;
		LbaWriteWord( ptrobj->MemoComportement ) ;
		LbaWriteLong( ptrobj->Flags ) ;
		LbaWriteLong( ptrobj->WorkFlags ) ;
		LbaWriteWord( ptrobj->DoorWidth ) ;
		LbaWriteByte( ptrobj->FlagAnim ) ;
		LbaWriteByte( ptrobj->CodeJeu ) ;
		LbaWrite( &(ptrobj->ExeSwitch),sizeof(ptrobj->ExeSwitch) ) ;

//	S16	MessageChapter[MAX_CHAPTER] ;

		LbaWriteLong( ptrobj->SampleAlways ) ;
		LbaWriteByte( ptrobj->SampleVolume ) ;

//	T_ZONE	*PtrZoneRail ;

		// on ne veut pas sauver le champ CurrentFrame
		LbaWrite( &(ptrobj->Obj), sizeof(ptrobj->Obj)-sizeof(ptrobj->Obj.CurrentFrame) ) ;
	}

	//------------- PATCHES -------------
	LbaWriteLong( NbPatches ) ;

#ifdef	EDITLBA2
	LbaWriteLong( SizePatches ) ;
	LbaWrite( BufferPatches, SizePatches ) ;
#else
	saveptr = PtrSave ;
	PtrSave += 4 ;

	ptrpatch = ListPatches ;

	for( n=0; n<NbPatches; n++, ptrpatch++ )
	{
		switch( ptrpatch->Size )
		{
			case 1:
				LbaWriteByte( *(PtrScene+ptrpatch->Offset) ) ;
				break ;

			case 2:
				LbaWriteWord( *(S16*)(PtrScene+ptrpatch->Offset) ) ;
				break ;

			case 4:
				LbaWriteLong( *(S32*)(PtrScene+ptrpatch->Offset) ) ;
				break ;

			default:
				LbaWrite( PtrScene+ptrpatch->Offset, ptrpatch->Size ) ;
		}
	}

	*(U32*)saveptr = (U32)(PtrSave-saveptr-4) ;
#endif
	//-----------------------------------

	// Extras
#ifdef	EDITLBA2
	LbaWriteByte( NbExtras ) ;
	LbaWrite( ListExtra, sizeof(T_EXTRA)*NbExtras ) ;
#else
	saveptr = PtrSave++ ;
	wbyte = 0 ;
	ptre = ListExtra ;

	for( n=0; n<MAX_EXTRAS; n++, ptre++ )
	{
		if( ptre->Sprite!=-1 )
		{
			LbaWrite( ptre, sizeof(T_EXTRA) ) ;
			wbyte++ ;
		}
	}

	*saveptr = wbyte ;
#endif

	// Zones
	LbaWriteLong( NbZones ) ;	// pour EditLba2

	ptrz = ListZone ;

	for( n=0; n<NbZones; n++, ptrz++ )
	{
		LbaWriteLong( ptrz->Info1 ) ;
		LbaWriteLong( ptrz->Info2 ) ;
		LbaWriteLong( ptrz->Info3 ) ;
		LbaWriteLong( ptrz->Info7 ) ;
	}

	// Incrust
#ifdef	EDITLBA2
	LbaWriteByte( NbIncrust ) ;
	LbaWrite( ListIncrustDisp, sizeof(T_INCRUST_DISP)*NbIncrust ) ;
#else
	saveptr = PtrSave++ ;
	wbyte = 0 ;
	ptrdisp = ListIncrustDisp ;

	for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
	{
		if( ptrdisp->Num!=-1 )
		{
			LbaWrite( ptrdisp, sizeof(T_INCRUST_DISP) ) ;
			wbyte++ ;
		}
	}

	*saveptr = wbyte ;
#endif

	// Flows
	ptrf = ListPartFlow ;

#ifndef	EDITLBA2
	saveptr = PtrSave++ ;
	wbyte = 0 ;

	for( n=0; n<MAX_FLOWS; n++, ptrf++ )
	{
		if( ptrf->NbDot>0 )
		{
			LbaWrite( ptrf, sizeof(S_PART_FLOW) ) ;

			ptrpt  = ptrf->PtrListDot ;
			wbyte2 = 0 ;
			saveptr2 = PtrSave++ ;

			for( i=0; i<ptrf->NbDot; i++, ptrpt++ )
			{
				if( ptrpt->Mode )
				{
					LbaWrite( ptrpt, sizeof(S_ONE_DOT) );
					wbyte2++ ;
				}
			}

			*saveptr2 = wbyte2 ;
			wbyte++ ;
		}
	}

	*saveptr = wbyte ;
#else
	LbaWriteByte( NbFlows ) ;

	for( n=0; n<NbFlows; n++, ptrf++ )
	{
		LbaWrite( ptrf, sizeof(S_PART_FLOW) ) ;

		LbaWriteByte( NbDots[n] ) ;
		LbaWrite( ListPtrListDot[n], sizeof(S_ONE_DOT)*NbDots[n] );
	}
#endif

	// Infos camera
	LbaWriteLong( VueDistance ) ;
	LbaWriteLong( AlphaCam ) ;
	LbaWriteLong( BetaCam ) ;
	LbaWriteLong( GammaCam ) ;
	LbaWriteLong( AddBetaCam ) ;

	LbaWriteLong( VueOffsetX ) ;
	LbaWriteLong( VueOffsetY ) ;
	LbaWriteLong( VueOffsetZ ) ;

	LbaWriteByte( CinemaMode ) ;
	LbaWriteLong( TimerCinema ) ;
	LbaWriteLong( LastYCinema ) ;
	LbaWriteLong( DebCycleCinema ) ;	// en pixels Y
	LbaWriteLong( DureeCycleCinema ) ;	// en TimerRefHR

#ifdef	EDITLBA2
	LbaWriteLong( EditClipWindowYMin ) ;
	LbaWriteLong( EditClipWindowYMax ) ;
#else
	RestoreClipWindow() ;
	LbaWriteLong( ClipWindowYMin ) ;
	LbaWriteLong( ClipWindowYMax ) ;
#endif

	LbaWriteByte( AnimateTexture ) ;

	LbaWriteLong( ParmSampleDecalage ) ;
	LbaWriteLong( ParmSampleFrequence ) ;
	LbaWriteLong( ParmSampleVolume ) ;

	LbaWriteByte( NumBuggy ) ;

	LbaWrite( ListBuggy, sizeof(S_BUGGY)*MAX_BUGGYS ) ;

	// a integrer dans la prochaine version de sauvegarde
	LbaWrite( ListArdoise, MAX_ARDOISE*sizeof(S8) ) ;

	LbaWriteByte( CurrentArdoise ) ;
	LbaWriteByte( NbArdoise ) ;

	LbaWriteLong( VueCamera ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Chargement d'une partie se trouvant dans PtrSave
S32	LoadContexte( S32 *savetimerrefhr )
{
	T_OBJET		*ptrobj ;
	S_PART_FLOW	*ptrf ;
	T_INV_OBJ	*ptri ;
	T_DART		*ptrd ;
	T_ZONE		*ptrz ;
	T_ARROW		*ptra ;
	S32		n ;
	S32		wlong ;
#ifndef	EDITLBA2
	S32		wlong2 ;
	T_EXTRA		*ptrextra ;
	T_PATCH		*ptrpatch ;
	S_ONE_DOT	*ptrdot ;
	S32	flaginit = FALSE ;
	U8	wbyte2 ;
#endif
	U8		wbyte ;

	// list Var game and cube
#ifdef	EDITLBA2
	LbaRead( SaveListVarGame, MAX_VARS_GAME*sizeof(SaveListVarGame[0]) ) ;
	LbaRead( SaveListVarCube, MAX_VARS_CUBE*sizeof(SaveListVarCube[0]) ) ;
#else
	LbaRead( ListVarGame, MAX_VARS_GAME*sizeof(ListVarGame[0]) ) ;
	LbaRead( ListVarCube, MAX_VARS_CUBE*sizeof(ListVarCube[0]) ) ;
#endif

	// Globales
	LbaReadByte( Comportement ) ;

#ifndef	EDITLBA2
	SaveComportement = Comportement ;
#endif

	LbaReadLong( wlong ) ;

	NbGoldPieces = wlong&0xFFFF ;
	NbZlitosPieces = (wlong>>16)&0xFFFF ;

	LbaReadByte( MagicLevel ) ;
	LbaReadByte( MagicPoint ) ;
	LbaReadByte( NbLittleKeys ) ;
	LbaReadWord(  NbCloverBox ) ;

	LbaReadLong( SceneStartX ) ;
	LbaReadLong( SceneStartY ) ;
	LbaReadLong( SceneStartZ ) ;

	LbaReadLong( StartXCube ) ;
	LbaReadLong( StartYCube ) ;
	LbaReadLong( StartZCube ) ;

	LbaReadByte( Weapon ) ;

	LbaReadLong( *savetimerrefhr ) ;

	LbaReadByte( NumObjFollow ) ;

	LbaReadByte( SaveComportementHero ) ;
	LbaReadByte( SaveBodyHero ) ;

	ptra = TabArrow ;

	for( n=0; n<(MAX_OBJECTIF+MAX_CUBE); n++, ptra++ )
	{
		LbaReadByte( wbyte ) ;
		wbyte &= 3 ;// seuls les 2 bits de poids faible sont
			    // susceptibles de bouger
		ptra->FlagHolo |= wbyte ;
	}

	// used inventory
	ptri = TabInv ;

	for( n=0; n<MAX_INVENTORY; n++, ptri++ )
	{
		LbaReadLong( ptri->PtMagie ) ;
		LbaReadLong( ptri->FlagInv ) ;
		LbaReadWord( ptri->IdObj3D ) ;
	}

#ifdef	EDITLBA2
	LbaReadLong( Checksum )	;
#else
	LbaReadLong( wlong )	;

	if( wlong!=Checksum )
	{
#if defined(DEBUG_TOOLS) || defined(TEST_TOOLS)
		Message( "Warning: La sauvegarde ne correspond pas au scénario !", TRUE ) ;
#endif
		SceneStartX=-1 ;
	}
#endif

#ifndef	EDITLBA2
	if( SceneStartX==-1 )
	{
		SceneStartX = CubeStartX ;
		SceneStartY = CubeStartY ;
		SceneStartZ = CubeStartZ ;
		ListObjet[NUM_PERSO].Obj.X = SceneStartX ;
		ListObjet[NUM_PERSO].Obj.Y = SceneStartY ;
		ListObjet[NUM_PERSO].Obj.Z = SceneStartZ ;

		StartXCube = ListObjet[NumObjFollow].Obj.X/SIZE_BRICK_XZ ;
		StartYCube = (ListObjet[NumObjFollow].Obj.Y+SIZE_BRICK_Y)/SIZE_BRICK_Y ;
		StartZCube = ListObjet[NumObjFollow].Obj.Z/SIZE_BRICK_XZ ;

		if( FlagChgCube == 2
		OR  FlagChgCube == 0 )
		{
			CameraCenter( 1 ) ;
		}
		else
		{
			CameraCenter( 0 ) ;
		}

		// Magouille : Pour récupérer le body du heros lors d'un
		// START_POS dans EDITLBA2, il est sauvé dans le dernier
		// VAR_CUBE par EDITLBA2

		ListObjet[NUM_PERSO].GenBody = ListVarCube[MAX_VARS_CUBE-1] ;

		flaginit = TRUE ;
	}
	else
#endif
	{
	// Debut incompatibilité des sauvegardes


#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||defined(EDITLBA2)
	if( NumVersion<34 )// Compatibilité sauvegardes
	{
		LbaReadLong( LastStepFalling ) ;
		LbaReadLong( LastStepShifting ) ;
	}
#endif

	LbaReadLong( LastMyFire ) ;
	LbaReadLong( LastMyJoy ) ;
	LbaReadLong( LastInput ) ;
	LbaReadLong( LastJoyFlag ) ;
	LbaReadByte( Bulle ) ;
	LbaReadByte( ActionNormal ) ;
	LbaReadLong( InventoryAction ) ;
	LbaReadLong( MagicBall ) ;
	LbaReadByte( MagicBallType ) ;
	LbaReadByte( MagicBallCount ) ;
	LbaReadLong( MagicBallFlags ) ;
	LbaReadByte( FlagClimbing ) ;
	LbaReadLong( StartYFalling ) ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||defined(EDITLBA2)
	if( NumVersion<34 )// Compatibilité sauvegardes
	{
		LbaRead( &RealFalling, sizeof( RealFalling ) ) ;
		LbaReadLong( StepFalling ) ;
		LbaRead( &RealShifting, sizeof( RealShifting ) ) ;
		LbaReadLong( StepShifting ) ;
	}
#endif

	LbaReadByte( CameraZone ) ;
	LbaReadLong( InvSelect ) ;
	LbaReadLong( ExtraConque ) ;
	LbaReadByte( PingouinActif ) ;

	PtrZoneClimb = (T_ZONE*)*(U32*)PtrSave ;
	PtrSave+=4 ;

	// Flechettes
	ptrd = ListDart ;

	for( n=0; n<MAX_DARTS; n++, ptrd++ )
	{
		LbaReadLong( ptrd->PosX ) ;
		LbaReadLong( ptrd->PosY ) ;
		LbaReadLong( ptrd->PosZ ) ;
		LbaReadLong( ptrd->Alpha ) ;
		LbaReadLong( ptrd->Beta ) ;
		LbaReadLong( ptrd->NumCube ) ;
		LbaReadLong( ptrd->Flags ) ;
	}

	// Objets
	LbaReadLong( NbObjets ) ; // nb d'objets pour EditLba2
	ptrobj = ListObjet ;

	for( n=0; n<NbObjets; n++, ptrobj++ )
	{
		LbaReadByte( ptrobj->GenBody ) ;
		LbaReadByte( ptrobj->Col ) ;
		LbaReadWord( ptrobj->GenAnim ) ;
		LbaReadWord( ptrobj->NextGenAnim ) ;
		LbaReadLong( ptrobj->OldPosX ) ;
		LbaReadLong( ptrobj->OldPosY ) ;
		LbaReadLong( ptrobj->OldPosZ ) ;
		LbaReadLong( ptrobj->Info  ) ;
		LbaReadLong( ptrobj->Info1 ) ;
		LbaReadLong( ptrobj->Info2 ) ;
		LbaReadLong( ptrobj->Info3 ) ;

		LbaRead( &(ptrobj->Coord), sizeof(ptrobj->Coord) ) ;

		LbaReadWord( ptrobj->SizeSHit ) ;
		LbaReadByte( ptrobj->HitBy ) ;
		LbaReadByte( ptrobj->HitForce ) ;
		LbaReadWord( ptrobj->LifePoint ) ;
		LbaReadWord( ptrobj->OptionFlags ) ;

//	U8	*PtrAnimAction ;

		LbaReadWord( ptrobj->Sprite ) ;
		LbaReadWord( ptrobj->OffsetLabelTrack ) ;

//	T_OBJ_3D Obj	;
//	U8	*PtrFile3D ;

		LbaReadLong( ptrobj->IndexFile3D ) ;

//	S16	NbBonus ;
		LbaReadByte( ptrobj->Armure ) ;
//	U8	CoulObj ;

		LbaReadWord( ptrobj->XMin ) ;
		LbaReadWord( ptrobj->XMax ) ;
		LbaReadWord( ptrobj->YMin ) ;
		LbaReadWord( ptrobj->YMax ) ;
		LbaReadWord( ptrobj->ZMin ) ;
		LbaReadWord( ptrobj->ZMax ) ;

		LbaReadLong( ptrobj->OldBeta ) ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||defined(EDITLBA2)
		if( NumVersion<34 )// Compatibilité sauvegardes
		{
			LbaRead( &TempoRealAngle, sizeof(TempoRealAngle) ) ;
		}
		else
#endif
		LbaRead( &(ptrobj->BoundAngle), sizeof(ptrobj->BoundAngle) ) ;

//	U8	*PtrTrack ;
		LbaReadWord( ptrobj->OffsetTrack ) ;
		LbaReadWord( ptrobj->SRot ) ;
		LbaReadWord( ptrobj->OffsetLife ) ;
		LbaReadWord( ptrobj->AnimDial ) ;
		LbaReadWord( ptrobj->CarryBy ) ;
		LbaReadByte( ptrobj->Move ) ;
		LbaReadByte( ptrobj->ObjCol ) ;
		LbaReadWord( ptrobj->ZoneSce ) ;
		LbaReadWord( ptrobj->LabelTrack ) ;
		LbaReadWord( ptrobj->MemoLabelTrack ) ;
		LbaReadWord( ptrobj->MemoComportement ) ;
		LbaReadLong( ptrobj->Flags ) ;
		LbaReadLong( ptrobj->WorkFlags ) ;
		LbaReadWord( ptrobj->DoorWidth ) ;
		LbaReadByte( ptrobj->FlagAnim ) ;
		LbaReadByte( ptrobj->CodeJeu ) ;
		LbaRead( &(ptrobj->ExeSwitch),sizeof(ptrobj->ExeSwitch) ) ;

//	S16	MessageChapter[MAX_CHAPTER] ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||defined(EDITLBA2)
		if( NumVersion>=35 )// Compatibilité sauvegardes
#endif
			LbaReadLong( ptrobj->SampleAlways ) ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||defined(EDITLBA2)
		if( NumVersion>=36 )// Compatibilité sauvegardes
#endif
			LbaReadByte( ptrobj->SampleVolume ) ;

//	T_ZONE	*PtrZoneRail ;

		// on ne veut pas sauver le champ CurrentFrame
		LbaRead( &(ptrobj->Obj), sizeof(ptrobj->Obj)-sizeof(ptrobj->Obj.CurrentFrame) ) ;
	}

	//------------- PATCHES -------------
	LbaReadLong( NbPatches ) ;

#ifdef	EDITLBA2
	LbaReadLong( SizePatches ) ;
	LbaRead( BufferPatches, SizePatches ) ;
#else
	LbaReadLong( wlong ) ;

	ptrpatch = ListPatches ;

	for( n=0; n<NbPatches; n++, ptrpatch++ )
	{
		switch( ptrpatch->Size )
		{
			case 1:
				LbaReadByte( *(PtrScene+ptrpatch->Offset) ) ;
				break ;

			case 2:
				LbaReadWord( *(S16*)(PtrScene+ptrpatch->Offset) ) ;
				break ;

			case 4:
				LbaReadLong( *(S32*)(PtrScene+ptrpatch->Offset) ) ;
				break ;

			default:
				LbaRead( PtrScene+ptrpatch->Offset, ptrpatch->Size ) ;
		}
	}
#endif
	//-----------------------------------

	// Extras

	LbaReadByte( wbyte ) ;

#ifdef	EDITLBA2
	NbExtras = wbyte ;
#endif

	LbaRead( ListExtra, sizeof(T_EXTRA)*wbyte ) ;

#ifndef	EDITLBA2
	// Magouille pour éviter de changer le format des sauvegardes
	// Pourrait etre changé en version finale en sauvant ProtectActif
	ProtectActif = 0 ;
	ptrextra = ListExtra ;

	for( n=0; n<wbyte; n++, ptrextra++ )
	{
		if( ptrextra->Flags&EXTRA_PROTECT )
		{
			ProtectActif = 1 ;
			break ;// 1 seul suffit pour qu'il soit actif
		}
	}
#endif

	// Zones
	LbaReadLong( NbZones ) ;	// pour EditLba2

	ptrz = ListZone ;

	for( n=0; n<NbZones; n++, ptrz++ )
	{
		LbaReadLong( ptrz->Info1 ) ;
		LbaReadLong( ptrz->Info2 ) ;
		LbaReadLong( ptrz->Info3 ) ;
		LbaReadLong( ptrz->Info7 ) ;

#ifndef	EDITLBA2
		switch( ptrz->Type )
		{
			case 3:	// GRM
				if( ptrz->Info2 )
				{
					IncrustGrm( ptrz ) ;
				}
				break ;
		}
#endif
	}

	// Incrust
	LbaReadByte( wbyte ) ;

#ifdef	EDITLBA2
	NbIncrust = wbyte ;
#endif

	LbaRead( ListIncrustDisp, sizeof(T_INCRUST_DISP)*wbyte ) ;

#ifndef	EDITLBA2
	for( n=0; n<wbyte; n++, ptrf++ )
	{
		if( (ListIncrustDisp[n].Type&0xFF)==INCRUST_PLUIE )
		{
			FlagRain = TRUE ;
			RestartRainSample = TRUE ;
		}
	}
#endif

	// Flows
	ptrf = ListPartFlow ;

	LbaReadByte( wbyte ) ;

#ifdef	EDITLBA2
	NbFlows = wbyte ;
#endif

	for( n=0; n<wbyte; n++, ptrf++ )
	{
#ifdef	EDITLBA2
		LbaRead( ptrf, sizeof(S_PART_FLOW) ) ;
		LbaReadByte( NbDots[n] ) ;
		LbaRead( ListPtrListDot[n], sizeof(S_ONE_DOT)*NbDots[n] );
#else
		ptrdot = ptrf->PtrListDot ;
		LbaRead( ptrf, sizeof(S_PART_FLOW) ) ;
		ptrf->PtrListDot = ptrdot ;
		LbaReadByte( wbyte2 ) ;
		LbaRead( ptrf->PtrListDot, sizeof(S_ONE_DOT)*wbyte2 ) ;
#endif
	}

	// Infos camera
	LbaReadLong( VueDistance ) ;
	LbaReadLong( AlphaCam ) ;
	LbaReadLong( BetaCam ) ;
	LbaReadLong( GammaCam ) ;
	LbaReadLong( AddBetaCam ) ;

	LbaReadLong( VueOffsetX ) ;
	LbaReadLong( VueOffsetY ) ;
	LbaReadLong( VueOffsetZ ) ;

	LbaReadByte( CinemaMode ) ;
	LbaReadLong( TimerCinema ) ;
	LbaReadLong( LastYCinema ) ;
	LbaReadLong( DebCycleCinema ) ;	// en pixels Y
	LbaReadLong( DureeCycleCinema ) ;	// en TimerRefHR

#ifdef	EDITLBA2
	LbaReadLong( EditClipWindowYMin ) ;
	LbaReadLong( EditClipWindowYMax ) ;
#else
	LbaReadLong( wlong ) ;
	LbaReadLong( wlong2 ) ;

	SetClipWindow( ClipWindowXMin, wlong, ClipWindowXMax, wlong2 ) ;
	MemoClipWindow() ;
#endif

	LbaReadByte( AnimateTexture ) ;

	LbaReadLong( ParmSampleDecalage ) ;
	LbaReadLong( ParmSampleFrequence ) ;
	LbaReadLong( ParmSampleVolume ) ;

	LbaReadByte( NumBuggy ) ;
	LbaRead( ListBuggy, sizeof(S_BUGGY)*MAX_BUGGYS ) ;

	LbaRead( ListArdoise, MAX_ARDOISE*sizeof(S8) ) ;

	LbaReadByte( CurrentArdoise ) ;
	LbaReadByte( NbArdoise ) ;
	LbaReadLong( VueCamera ) ;
	}

#ifndef	EDITLBA2
	TimerAniPoly = 0 ;	// reinit animation de l'eau

	InitRain() 	 ;	// Init Pluie
	LastTimer = 0 	 ;

	return flaginit ;
#else
	return FALSE ;
#endif
}
