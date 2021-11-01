#include	"c_extern.h"

char		PathAcf[ADELINE_MAX_PATH] ;

char		*ListAcf ;
char		*EndListAcf ;

#ifdef	LBA_EDITOR
#define	SIZE_BUFFER_SMACK	1000000L
U8  	*BufferSmack ;
#else
U8  	*BufferSmack ;// initialis‚ dans MEM.CPP
#endif

//---------------------------------------------------------------------------

/***************************************************************************\
|*									   *|
|*				INITS					   *|
|*									   *|
\***************************************************************************/

//---------------------------------------------------------------------------
void	InitAcf( void )
{
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
	char    *path ;

	path = DefFileBufferReadString( "PathAcf" ) ;

	if( path AND strcmp(path,"") )
	{
		strcpy( PathAcf, path ) ;
		if( PathAcf[strlen(PathAcf)-1]!='\\'
		AND PathAcf[strlen(PathAcf)-1]!=':' )
			strcat( PathAcf, "\\" ) ;
	}
	else
#endif
	{
#if defined(ONE_GAME_DIRECTORY)&&defined(CDROM)

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
		if( !CDDrive )
		{
			strcpy( PathAcf, PATH_ACF ) ;
		}
		else	sprintf( PathAcf, "%c:\\LBA2\\VIDEO\\VIDEO.HQR", CDDrive ) ;
#else
		switch( DistribVersion )
		{
			case UNKNOWN_VERSION:
			case EA_VERSION:
				sprintf( PathAcf, "%c:\\LBA2\\VIDEO\\VIDEO.HQR", CDDrive ) ;
				break ;

			default:
				sprintf( PathAcf, "%c:\\TWINSEN\\VIDEO\\VIDEO.HQR", CDDrive ) ;
		}

		if( !FileSize( PathAcf ) )	TheEnd( PROGRAM_OK, MessageNoCD ) ;
#endif
#else
		// default path Acf
		strcpy( PathAcf, PATH_ACF ) ;
#endif
	}

	ListAcf = (char *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_ACFLIST ) ;
	if( !ListAcf )	TheEndCheckFile( RESS_HQR_NAME ) ;

	EndListAcf = ListAcf + LoadMallocFileSize ;

#ifdef	LBA_EDITOR
	// dispo Screen,ScreenAux,BufSpeak
	SmackRegisterMemory( Screen, (ModeDesiredX*ModeDesiredY+RECOVER_AREA)*3 ) ;

	BufferSmack = (U8*)Malloc( SIZE_BUFFER_SMACK ) ;
	if( !BufferSmack )	TheEnd( NOT_ENOUGH_MEM, "BufferSmack (InitAcf())" ) ;

	SmackRegisterMemory( (void*)BufferSmack, SIZE_BUFFER_SMACK ) ;
#else
	SmackRegisterMemory( MainBuffer, SizeOfMemAllocated ) ;
#endif
}

//---------------------------------------------------------------------------

char	*PtrAcf ;

char	*GetAcfName( char *name )
{
	char	*ptr = name ;
	char	c ;

	do	/* saute cr/lf espace */
	{
		if( PtrAcf>EndListAcf )
		{
			PtrAcf--  ;
			return 0L ;
		}

		c = *PtrAcf++ ;
	}
	while( c <= 32 ) ;

	do
	{
		*ptr++ = c ;
		c = *PtrAcf++ ;
	}
	while( c > 32 ) ;

	if( PtrAcf>EndListAcf )
	{
		PtrAcf--  ;
		return 0L ;
	}

	if( c == 26 )	PtrAcf-- ;

	*ptr++ = 0 ;
	return name ;
}

//---------------------------------------------------------------------------

S32	GetNumAcf( char *name )
{
	S32	n = 0 ;
	char	getname[15] ;
	char	*ptr ;

	PtrAcf = ListAcf ;

	ptr = strchr(name,'.') ;
	if( ptr )	*ptr = 0 ;

	while( GetAcfName( getname ) )
	{
		ptr = strchr(getname,'.') ;
		if( ptr )	*ptr = 0 ;

		if( !stricmp(name,getname) )	return n ;
		n++ ;
	}

	return -1 ;
}

//---------------------------------------------------------------------------

void	PlayAllAcf( void )
{
	S32	firsttime = TRUE ;
	S32	n = 0 ;
	S32	var = FLAG_ACF ;
	char	getname[13] ;

	PtrAcf = ListAcf ;

#if defined(LBA_EDITOR)||defined(CDROM)
	FadeOutVolumeMusic() ;
#endif

	while( GetAcfName( getname ) )
	{
#ifdef	LBA_EDITOR
		if( (ListVarGame[var].Value & (1<<n))
#else
		if( (ListVarGame[var] & (1<<n))
#endif
		AND (strnicmp(getname,"INTRO",5)) )// on ne rejoue pas l'intro
		{
			if( !FlagFade )
			{
				if( firsttime )
				{
					FadeToBlackAndSamples( PtrPal ) ;
					firsttime = FALSE ;
				}
				else	FadeToBlack( PtrPal ) ;
			}

			FlagFade = TRUE ;

			PlayAcf( getname ) ;
//			if( PlayAcf( getname ) == K_ESC )	break ;
		}

		n++ ;

		if( n>=16 )
		{
			var++ ;
			n = 0 ;
		}
	}

#if defined(LBA_EDITOR)||defined(CDROM)
	FadeInVolumeMusic() ;
#endif

	FirstTime = AFF_ALL_FLIP ;
}

//---------------------------------------------------------------------------

//extern	void	CopyFrameDoubleXY( U8 *ptrsrc ) ;

/***************************************************************************\
|*									   *|
|*				MAIN					   *|
|*									   *|
\***************************************************************************/

S32	PlayAcf( char *name )
{
	Smack		*smk	      	;
	U32		frame	      	;
	S32		n 		;
	S32		ret 		;
	U8		*decompbuf	;
	U32		smackflags	;
#ifdef	LBA_EDITOR
	char		name_acf[256] 	;

	ShowMouse( 0 ) ;

	smackflags = SMACKTRACK1|SMACKNEEDVOLUME ;
#else
	smackflags = SMACKFILEHANDLE|SMACKTRACK1|SMACKNEEDVOLUME ;
#endif

	// si c'est l'intro, on joue la track de la langue choisie
	if( !strnicmp(name,"INTRO",5) )
	{
#ifdef	CDROM
		if( FlagSpeak )
		{
			switch( LanguageCD )
			{
				case 0:  // English
					smackflags |= SMACKTRACK4 ;
					break ;

				case 1:	// Fran‡ais
					smackflags |= SMACKTRACK2 ;
					break ;

				case 2:	// Deutsch
					smackflags |= SMACKTRACK3 ;
					break ;

//				default:// other ...
					// no spoken track
			}
		}
#else
		smackflags |= SMACKTRACK4 ;// English and other ...
#endif
	}

	// Ah ? pas de son ? tiens donc ... gicle-moi les tracks !
/*	if( !Sample_Driver_Enabled )
	{
		smackflags &= ~(SMACKTRACKS|SMACKNEEDVOLUME) ;
	}
*/
	n = GetNumAcf( name ) ;

	if( n==-1 )	return 0 ;

	{
		S32	var	;
		S32	bit	;

		var = n/16	;
		if( n>=32 )
		{
			var = FLAG_ACF2 ;
			bit = 1<<(n-32) ;
		}
		else
		{
			var = FLAG_ACF 	;
			bit = 1<<n 	;
		}

#ifdef	LBA_EDITOR
		ListVarGame[FLAG_ACF+(n/16)].Value |= (S16)(1<<(n&15)) ;
#else
		ListVarGame[FLAG_ACF+(n/16)] |= (S16)(1<<(n&15)) ;
#endif
	}

	StopMusic() ;

	SmackResetMemory() ;

#ifdef	LBA_EDITOR
	strcpy( name_acf, PathAcf ) ;
	strcat( name_acf, name ) ;
	AddExt( name_acf, ".SMK" ) ;

	smk = SmackOpen( name_acf, smackflags, SMACKAUTOEXTRA ) ;
#else
//	Fd = OpenRead(name_acf)	;// Ouvrir ressource + Seek

	// ouvre la ressource et positionne le handle … l'offet ou se trouve
	// la sequence
	if( !HQF_Init( PathAcf, n ) )
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||!defined(CDROM)
		return 0 ;
#else
		TheEnd( PROGRAM_OK, MessageNoCD ) ;
#endif

	smk = SmackOpen( (char*)HQF_File, smackflags, SMACKAUTOEXTRA ) ;
#endif

	if( !smk )
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)||!defined(CDROM)
		return 0 ;
#else
		TheEnd( PROGRAM_OK, MessageNoCD ) ;
#endif

	decompbuf = (U8*)radmalloc( 320*200 )	;

	if( !decompbuf )
	{
		SmackClose( smk ) ;
#ifndef	LBA_EDITOR
		HQF_Close() ;
#endif
		return 0 ;
	}

	memset( decompbuf, 0, 320*200 ) ;

	SmackToBuffer( smk, 0, 0, 320, 200, decompbuf, FALSE ) ;

//	if( Sample_Driver_Enabled )
	{
		S32	vol ;

#ifdef	_WIN32
		if( UseWaveMixer )
		{
			vol = SampleVolume ;
		}
		else
#endif
		{
#ifdef	LBA_EDITOR
			vol = SampleVolume ;
#else
			vol = BoundRegleTrois( 0, MasterVolume, 127, SampleVolume ) ;
#endif
		}

		vol = (vol==0?vol:(vol+1))*512 ;// 0..65536

		SmackVolumePan( smk, SMACKTRACKS, vol, 32768 ) ;
	}

	SetBlackPal();

	BoxReset()  ;// pour etre s–r de ne pas utiliser Screen !
	Cls() ;
	BoxUpdate() ;

	InitWaitNoKey()			;
	InitWaitNoInput( I_JOY|I_FIRE|I_MENUS ) ;

	for( frame=0; frame<smk->Frames; frame++ )
	{
		SmackDoFrame( smk ) ;

#ifndef	LBA_EDITOR
		if( !VideoFullScreen )
		{
			if( smk->NewPalette )
			{
#ifndef	LBA_EDITOR
				FlagBlackPal = FALSE ;
#endif
				Palette(smk->Palette) ;
			}
#ifdef	_WIN32
			BlitBox( (void*)Phys, (void*)decompbuf ) ;
#else
			BlitBoxVESA( (void*)Phys, (void*)decompbuf ) ;
#endif

		}
		else
#endif
		{
			CopyFrameDoubleXY( (void*)decompbuf ) ;
			BoxStaticAdd( 0, 40, 639, 439 ) ;
		}

#ifdef	LBA_EDITOR
		if( FlagInfos & INFO_FRAME_SPEED )
		{
			GraphPrintf(FALSE, 0,0, "Frame: %d", frame++);
		}
#endif

#ifndef	LBA_EDITOR
		if( VideoFullScreen )
#endif
		{
			if( smk->NewPalette )
			{
#ifndef	LBA_EDITOR
				FlagBlackPal = FALSE ;
#endif
				// Pour eviter flash sur frame precedente
//				Cls()  ;
//				BoxUpdate() ;

				Palette(smk->Palette) ;
			}

			BoxUpdate() ;
		}

		if( frame!=smk->Frames )
		{
			SmackNextFrame( smk ) ;
		}

		do
		{
			MyGetInput() ;
			ManageTime() ;

#ifndef	LBA_EDITOR
			CheckSaveLogPcx( smk->Palette ) ;// F9: Sauve Screen dans PATH_PCX_SAVE
#endif

			if( (MyKey==K_ESC OR (Input&I_MENUS))
#ifndef	LBA_EDITOR
			AND (!FlagPlayAcf OR frame>290)
#endif
			)
			{
				goto fin_play ;
			}
		}
		while( SmackWait( smk ) ) ;
	}

fin_play:

	SmackClose( smk ) ;

#ifndef	LBA_EDITOR
	HQF_Close() ;
	LastIdxPalette = 0 ;// pour reforcer le chargement du .XPL (cf ChoicePalette())
#endif

	radfree( decompbuf ) ;

	FadeToBlack( smk->Palette ) ;
	Cls() ;
	BoxStaticFullflip() ;
	SetBlackPal() ;

	ret = MyKey ;

#ifdef	LBA_EDITOR
	SetMouseBox( 0,0, 639,479 ) ;
#endif

	InitWaitNoKey()			;
	InitWaitNoInput( I_MENUS ) ;

#ifdef	LBA_EDITOR
	ShowMouse( 1 ) ;
#endif

	return ret ;
}

//---------------------------------------------------------------------------
