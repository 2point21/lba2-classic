#include	"c_extern.h"

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
				  Ûßßßß ÛßßßÜ
				  ÛÛ    ÛÛ  Û
				  ßßßßß ßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#ifdef	CDROM
char		CDDrive = 'Z'	;// Z by default
U32		CDVolume     = DEF_CD_VOLUME	;

#ifndef	LBA_EDITOR
void	InitTabTracks( void ) ;// voir plus bas...
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

S32	InitCD( char *name )
{
#ifndef	LBA_EDITOR
	InitTabTracks() ;
#endif
	CDDrive = OpenCD( name ) ;

	if( CDDrive )
	{
		SetVolumeCD( CDVolume ) ;

		return TRUE ;
	}

	return FALSE ;
}
#endif	// CDROM

 /*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
			  Û  Û    ÛÛÜ Û Ûßßßß Û     Ûßßßß
		      ÜÜ  Û  ÛÛ   ÛÛßÛÛ ÛÛ ßÛ ÛÛ    ÛÛßß
		      ßßßßß  ßß   ßß  ß ßßßßß ßßßßß ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

char	PathJingle[ADELINE_MAX_PATH] ;

U32	JingleVolume = DEF_JINGLE_VOLUME ;

#define	FIRST_JINGLE	2
#define	LAST_JINGLE	26

#define	NB_JINGLE	26

char	ListJingle[NB_JINGLE][9] = {
	"",		// pour interfacer avec la music CD (car Track 0 == NO_TRACK)
	"TADPCM1",
	"TADPCM2",
	"TADPCM3",
	"TADPCM4",
	"TADPCM5",
	"JADPCM01",
	"",	// Track6.wav
	"JADPCM02",
	"JADPCM03",
	"JADPCM04",
	"JADPCM05",
	"JADPCM06",
	"JADPCM07",
	"JADPCM08",
	"JADPCM09",
	"JADPCM10",
	"JADPCM11",
	"JADPCM12",
	"JADPCM13",
	"JADPCM14",
	"JADPCM15",
	"JADPCM16",
	"JADPCM17",
	"JADPCM18",
	"LOGADPCM",
	} ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	InitJingle( void )
{
	char    *path ;

	OpenStream() ;// init Stream System

	// fix path jingles
	path = DefFileBufferReadString( "PathJingle" ) ;

	if( path AND strcmp(path,"") )
	{
		strcpy( PathJingle, path ) ;
		if( PathJingle[strlen(PathJingle)-1]!='\\'
		AND PathJingle[strlen(PathJingle)-1]!=':' )
			strcat( PathJingle, "\\" ) ;
	}
	else
	{
#if defined(ONE_GAME_DIRECTORY)&&defined(CDROM)
		switch( DistribVersion )
		{
			case UNKNOWN_VERSION:
			case EA_VERSION:
				sprintf( PathJingle, "%c:\\LBA2\\MUSIC\\", CDDrive ) ;
				break ;

			default:
				sprintf( PathJingle, "%c:\\TWINSEN\\MUSIC\\", CDDrive ) ;
		}
#else
#ifdef	CDROM
		if( CDDrive )
		{
			sprintf( PathJingle, "%c:\\LBA2\\MUSIC\\", CDDrive ) ;
		}
		else
#endif
		{	// default path Acf
			strcpy( PathJingle, PATH_JINGLE ) ;
		}
#endif
	}

	SetVolumeJingle( JingleVolume ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

S32	GetNumJingle( char *name )
{
	char	filename[9] ;
	S32	n ;

	if( name AND strcmp(name,"") )
	{
		strcpy( filename, GetFileName(name) ) ;

		for( n=1; n<NB_JINGLE; n++ )
		{
			if( !stricmp(ListJingle[n],filename) )
			{
				return (n+FIRST_JINGLE-1) ;// -1 car 0==NO_TRACK
			}
		}
	}

	return 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	PlayJingle( S32 num )
{
	char	name_jingle[ADELINE_MAX_PATH] ;

	if( num>=FIRST_JINGLE
	AND num<=LAST_JINGLE
	AND JingleVolume>0 )
	{
		LockTimer() ;

		num -= FIRST_JINGLE ;
		num++ ;// car track 0 == NO_TRACK

#ifdef ONE_GAME_DIRECTORY
		// tente d'abord de jouer le jingle dans le sous-r‚pertoire
		// MUSIC
		strcpy( name_jingle, "MUSIC\\" ) ;
		strcat( name_jingle, ListJingle[num] ) ;
		AddExt( name_jingle, ".WAV" ) ;

		if( !FileSize( name_jingle ) )
#endif
		{
			strcpy( name_jingle, PathJingle ) ;
			strcat( name_jingle, ListJingle[num] ) ;
			AddExt( name_jingle, ".WAV" ) ;
		}

		// a remplacer par GetNumJingle() ?
		if( stricmp(name_jingle,StreamName()) )
		{
			PlayStream( name_jingle ) ;
		}

		UnlockTimer() ;
	}
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
			 ÛÜ ÜÛ Û   Û ÛÛßßß  Û    Ûßßßß
			 ÛÛß Û ÛÛ  Û ßßßßÛ  ÛÛ   ÛÛ
			 ßß  ß ßßßßß ßßßßß  ßß   ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/


// Tableau d'indirection au cas o— l'ordre change sur le CD aprŠs la saisie
// dans l'outil

#define	JINGLE	0x80	// flag indiquant une track stream‚e
#define	MUSIC	0x7F	// pour avoir le numero de musique

#define	FIRST_CD_TRACK		6
#define	FIRST_CD_TRACK_US	0

U8	FirstCDTrack = FIRST_CD_TRACK ;

U8	TrackCD[] = {
JINGLE| 2,	// Track01.wav
JINGLE| 3,	// Track02.wav
JINGLE| 4,	// Track03.wav
JINGLE| 5,	// Track04.wav
JINGLE| 6,	// Track05.wav
JINGLE| 7,	// Jingle01.wav
	8,	// Track06.wav
JINGLE| 9,	// Jingle02.wav
JINGLE|10,	// Jingle03.wav
JINGLE|11,	// Jingle04.wav
JINGLE|12,	// Jingle05.wav
JINGLE|13,	// Jingle06.wav
JINGLE|14,	// Jingle07.wav
JINGLE|15,	// Jingle08.wav
JINGLE|16,	// Jingle09.wav
JINGLE|17,	// Jingle10.wav
JINGLE|18,	// Jingle11.wav
JINGLE|19,	// Jingle12.wav
JINGLE|20,	// Jingle13.wav
JINGLE|21,	// Jingle14.wav
JINGLE|22,	// Jingle15.wav
JINGLE|23,	// Jingle16.wav
JINGLE|24,	// Jingle17.wav
JINGLE|25,	// Jingle18.wav
JINGLE|26,	// LogAdpcm.wav
	} ;

U8	TrackCDUS[] = {
	2,	// Track01.wav
	3,	// Track02.wav
	4,	// Track03.wav
	5,	// Track04.wav
	6,	// Track05.wav
	7,	// Jingle01.wav
	8,	// Track06.wav
JINGLE| 9,	// Jingle02.wav
JINGLE|10,	// Jingle03.wav
JINGLE|11,	// Jingle04.wav
JINGLE|12,	// Jingle05.wav
JINGLE|13,	// Jingle06.wav
JINGLE|14,	// Jingle07.wav
JINGLE|15,	// Jingle08.wav
JINGLE|16,	// Jingle09.wav
JINGLE|17,	// Jingle10.wav
JINGLE|18,	// Jingle11.wav
JINGLE|19,	// Jingle12.wav
JINGLE|20,	// Jingle13.wav
JINGLE|21,	// Jingle14.wav
JINGLE|22,	// Jingle15.wav
JINGLE|23,	// Jingle16.wav
JINGLE|24,	// Jingle17.wav
JINGLE|25,	// Jingle18.wav
JINGLE|26,	// LogAdpcm.wav
	} ;

U8	*PtrTrackCD ;

#define	TEST_MUSIC_TEMPO	(2*1000)	// 2s

S32	NextMusic = -1 ;
S32	StopLastMusic = FALSE ;
S32	NextMusicTimer ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#if defined(CDROM)&&!defined(LBA_EDITOR)
void	InitTabTracks( void )
{
	switch( DistribVersion )
	{
		case UNKNOWN_VERSION:
		case EA_VERSION:
			PtrTrackCD = TrackCD ;
			FirstCDTrack = FIRST_CD_TRACK ;
			break ;

		default:
			PtrTrackCD = TrackCDUS ;
			FirstCDTrack = FIRST_CD_TRACK_US ;
	}
}
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Cette fonction doit orienter la musique soit sur une Track Audio, soit
// sur du Streaming ADPCM

void	PlayMusic( S32 num, S32 playit )
{
	NextMusic = -1 ;

	PIRACY_PROTECT() ;

	if( num!=-1 )
	{
		S32 cur ;

		LockTimer() ;

		if( playit OR StopLastMusic )	cur = 0 ;
		else				cur = GetMusic() ;

		StopLastMusic = playit ;

		if( cur==0 )
		{
			if( !playit
			OR  GetMusic()!=(PtrTrackCD[num]&MUSIC) )
			{
				StopMusic() ;

				if( PtrTrackCD[num]&JINGLE )
				{
					PlayJingle( PtrTrackCD[num]&MUSIC ) ;
				}
#ifdef	CDROM
				else
				{
					if( CDVolume>0 )
					{
						PlayCD( (PtrTrackCD[num]&MUSIC)-FirstCDTrack ) ;
					}
				}
#endif
			}
		}
		else
		{
//			if( PtrTrackCD[cur-1]&JINGLE )// on n'empile pas sur une track
			if( cur!=(PtrTrackCD[num]&MUSIC) )// on n'empile pas sur la meme
			{
				NextMusic = num ;// empile musique … jouer +tard
//				NextMusicTimer = TimerRefHR+TEST_MUSIC_TEMPO ;
			}
		}

		NextMusicTimer = TimerRefHR+TEST_MUSIC_TEMPO ;

		UnlockTimer() ;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

S32	GetMusic( void )
{
	S32	num ;

#ifdef	CDROM
	num = IsCDPlaying() ;
#else
	num = 0 ;
#endif

	if( !num )	// pas de music CD
	{
		num = GetNumJingle( StreamName() ) ;
	}
	else	num += FirstCDTrack ;

	return num ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	StopMusic( void )
{
#ifdef	CDROM
	StopCD() ;
#endif
	StopStream()  ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	PauseMusic( S32 fade )
{
	SaveTimer() ;

	if( fade )	FadeOutVolumeMusic() ;

#ifdef	CDROM
	PauseCD() ;
#endif
	PauseStream() ;

	if( fade )
	{
#ifdef	CDROM
		SetVolumeCD( CDVolume ) ;
#endif
		SetVolumeJingle( JingleVolume ) ;
	}

	RestoreTimer() ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	ResumeMusic( S32 fade )
{
	SaveTimer() ;

	if( fade )
	{
#ifdef	CDROM
		SetVolumeCD(0) ;
#endif
		SetVolumeJingle( 0 ) ;
	}

	StopMusic() ;

#ifdef	CDROM
	ResumeCD() ;
#endif
	ResumeStream()  ;

	if( fade )	FadeInVolumeMusic() ;

	StopLastMusic = FALSE ;

	RestoreTimer() ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#define	FADE_MUSIC_TIME		100	// 1/5s

void	FadeOutVolumeMusic( void )
{
	S32	timer ;
	U32	startjvol, jvolume, oldjvol ;
#ifdef	CDROM
	U32	startcdvol, cdvolume, oldcdvol ;
#endif

	ManageTime();
	SaveTimer() ;

	timer = TimerRefHR ;

	startjvol = oldjvol = jvolume = GetVolumeStream() ;

#ifdef	CDROM
	startcdvol  = oldcdvol = cdvolume = GetVolumeCD() ;
#endif

	while(
#ifdef	CDROM
	cdvolume>0 OR
#endif
	jvolume>0 )
	{
		ManageTime();
#ifdef	CDROM
		// volume du CD
		cdvolume = BoundRegleTrois( startcdvol, 0, FADE_MUSIC_TIME, TimerRefHR-timer ) ;

		if( cdvolume!=oldcdvol )
		{
			SetVolumeCD( cdvolume ) ;
			oldcdvol = cdvolume ;
		}
#endif
		// volume des Jingles
		jvolume = BoundRegleTrois( startjvol, 0, FADE_MUSIC_TIME, TimerRefHR-timer ) ;

		if( jvolume!=oldjvol )
		{
			SetVolumeJingle( jvolume ) ;
			oldjvol = jvolume ;
		}
	}

#ifdef	CDROM
	SetVolumeCD( 0 ) ;
#endif
	SetVolumeJingle( 0 ) ;

	RestoreTimer() ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	FadeInVolumeMusic( void )
{
	S32	timer ;
	U32	startjvol, jvolume, oldjvol ;
#ifdef	CDROM
	U32	startcdvol, cdvolume, oldcdvol ;
#endif

	ManageTime();
	SaveTimer() ;

	timer = TimerRefHR ;

#ifdef	CDROM
	startcdvol = oldcdvol = cdvolume = GetVolumeCD() ;
#endif

	startjvol = oldjvol = jvolume = GetVolumeStream() ;

	while(
#ifdef	CDROM
	cdvolume<CDVolume OR
#endif
	jvolume<JingleVolume )
	{
		ManageTime();
#ifdef	CDROM
		// volume du CD
		cdvolume = BoundRegleTrois( startcdvol, CDVolume, FADE_MUSIC_TIME, TimerRefHR-timer ) ;

		if( cdvolume!=oldcdvol )
		{
			SetVolumeCD( cdvolume ) ;
			oldcdvol = cdvolume ;
		}
#endif
		// volume des Jingles
		jvolume = BoundRegleTrois( startjvol, JingleVolume, FADE_MUSIC_TIME, TimerRefHR-timer ) ;

		if( jvolume!=oldjvol )
		{
			SetVolumeJingle( jvolume ) ;
			oldjvol = jvolume ;
		}
	}

#ifdef	CDROM
	SetVolumeCD( CDVolume ) ;
#endif
	SetVolumeJingle( JingleVolume ) ;

	RestoreTimer() ;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	CheckNextMusic( void )
{
	if( NextMusic!=-1 )
	{
		if( TimerRefHR>NextMusicTimer )
		{
			PlayMusic( NextMusic, FALSE ) ;
		}
	}
}
