/*-------------------------------------------------------------------------*/
#ifdef	TITRE
/*-------------------------------------------------------------------------*/

	    €ø   €ø €€€€€€ø €€€€€€ø €€€€€€ø €€€€€€ø €€€€€€ø €€€€€€ø
//	    €€ø €€≥ €€⁄ƒƒƒŸ €€⁄ƒƒƒŸ €€⁄ƒƒƒŸ €€⁄ƒ€€≥ €€⁄ƒƒƒŸ €€⁄ƒƒƒŸ
	    €€€€€€≥ €€€€€ø  €€€€€€ø €€€€€€ø €€€€€€≥ €€≥€€€ø €€€€€ø
//	    €€⁄ƒ€€≥ €€⁄ƒƒŸ  ¿ƒƒƒ€€≥ ¿ƒƒƒ€€≥ €€⁄ƒ€€≥ €€≥¿€€≥ €€⁄ƒƒŸ
	    €€≥ €€≥ €€€€€€ø €€€€€€≥ €€€€€€≥ €€≥ €€≥ €€€€€€≥ €€€€€€ø
	    ¿ƒŸ ¿ƒŸ ¿ƒƒƒƒƒŸ ¿ƒƒƒƒƒŸ ¿ƒƒƒƒƒŸ ¿ƒŸ ¿ƒŸ ¿ƒƒƒƒƒŸ ¿ƒƒƒƒƒŸ
/*-------------------------------------------------------------------------*/
#endif

//#define	CRYPTAGE	1


//#define LORAN	1
//#define LBA_EDITOR 1
/*-------------------------------------------------------------------------*/

#ifdef  LORAN
#include "f:\projet\lib386\lib_sys\adeline.h"
#include "f:\projet\lib386\lib_sys\lib_sys.h"
#include "f:\projet\lib386\lib_svga\lib_svga.h"
#include "f:\projet\lib386\lib_menu\lib_menu.h"
#include "f:\projet\lib386\lib_cd\lib_cd.h"
#include "f:\projet\lib386\lib_3d\lib_3d.h"

#include        <stdlib.h>
#include        <string.h>
#include        <stdio.h>
#include        <conio.h>
#include        <dos.h>
#include        <i86.h>
#else
#include        "c_extern.h"
#endif
#ifdef  LORAN
//================================================================= L O R A N
#define	PathConfigFile	"c:\\adeline.def"
#define	CDROM 1

LONG	Island	;
extern	UBYTE	*BufSpeak	;
/*-------------------------------------------------------------------------*/
void    DrawCadre( LONG x0, LONG y0, LONG x1, LONG y1 )
{
	Rect( x0, y0, x1, y1, 15 )      ;
}
/*-------------------------------------------------------------------------*/
void    *LoadTestMalloc( char *name )
{
	return(LoadMalloc(name))        ;
}
/*-------------------------------------------------------------------------*/
LONG	GetMusicCD()
{
	return(GetMusicCDR())	;
}
/*-------------------------------------------------------------------------*/
void	StopMusicCD()
{
	StopCDR()	;
}
/*-------------------------------------------------------------------------*/
UBYTE   *Screen ;
UBYTE   *LbaFont;
UBYTE   *PalOrg ;
WORD    QuickMessage = FALSE ;
//===========================================================================
#endif

#ifdef	ONE_GAME_DIRECTORY
#define PATH_TXT        ""
#define PATH_NAR        ""
#else
#define PATH_TXT        "f:\\projet\\lba\\text\\"
#define PATH_NAR        "f:\\projet\\lba\\samples\\dial\\"
#endif

#ifdef	CDROM
#define	PATH_NAR_CD	"\\LBA\\VOX\\"
#define	PATH_NAR_HD	"VOX\\"
#define	MAX_FILE_VOICE	42	// SYS CRE GAM 0 1 2 3 4 5 6 7 8 9 10 * MaxLang
#define	EXT_NAR		".VOX"
#define	FROM_HD		0
#define	FROM_CD		1
#endif

#define	NAME_HQR_TEXT	"TEXT.HQR"
#define	MAX_TEXT_LANG	14		// Attention, pour 1 langue seulement!
#define INIT_CODE       01+02+1993

#define INTER_LINE      38
#define INTER_SPACE     7		// 16/08/94  de 5 a 7 !
#define INTER_LEAVE     2

#define MAX_CAR         32
#define SPEAK_SAMPLE    0x1234
/*-------------------------------------------------------------------------*/
static  LONG    Dial_X0=16      ;
static  LONG    Dial_Y0=334     ;
static  LONG    Dial_X1=639-16  ;
static  LONG    Dial_Y1=479-16  ;
static  LONG    DialMaxSize=((623-8)-(16+8));

static  char    BufLine[256]    ;
static  char    *PtLine         ;
static  LONG    SizeLine        ;
static  LONG    NbSpace         ;
static  LONG    NbBigSpace      ;
static  LONG    SizeSpace       ;
static  LONG    LenLine         ;
static  LONG    SizeWord        ;

/*-------------------------------------------------------------------------*/

static	UBYTE	BufferMultiText[256] ;
static	LONG	IslandMultiText = -1 ;
static	LONG	NumMultiText = -1 ;

LONG	FlecheForcee = FALSE ;
static	ULONG	SaveTimerForCar;
/*-------------------------------------------------------------------------*/

UWORD    MaxText         	;

static char     FileText[256]   ;

#define	NB_LANGUAGES	5

LONG	Language = 1 	;		// English
LONG	LastFileInit=-1	;

char	*TabLanguage[] = {
			//	Ne pas toucher l'ordre cause HQR
	"English",
	"Franáais",
	"Deutsch",
	"Espa§ol",
	"Italiano" 	}	;


char  *ListLanguage[] = {     "EN_", "FR_", "DE_", "SP_", "IT_" } ;

char    *ListFileText[] = {     "sys",
				"cre",
				"gam",
				"000",
				"001",
				"002",
				"003",
				"004",
				"005",
				"006",
				"007",
				"008",
				"009",
				"010",
				"011"	  } ;
// Fred flag de cadre shade

WORD	FlagMessageShade = TRUE ;

typedef struct
{
	WORD    c       ;
	WORD    x       ;
	WORD    y       ;

} T_STACKCAR    ;

static  T_STACKCAR      StackCar[MAX_CAR]       ;

static  LONG    NbCar           ;

static  LONG    NbDegrade       ;
static  LONG    MinDegrade      ;
static  LONG    MaxDegrade      ;
static  LONG    StepDegrade     ;
UBYTE   *BufText=0L     	;
static	UBYTE   *PtText=0L	;
static  LONG    SizeText        ;

UWORD   *BufOrder = 0L  	;

LONG    FlagSpeak=FALSE	        	;//	cf InitLanguage
//------------------------------
#ifdef	CDROM
WORD	NumObjSpeak = -1 ;
LONG	FlagVoiceCD=FALSE		;//	cf InitLanguage
static	LONG	FlagKeepVoice=0		;
	LONG	FlagDisplayText=1	;//	ON par default
static  LONG    FlagNextVoc=FALSE	;
ULONG   *BufMemoSeek=0  		;
static  ULONG   FdNar=0         	;
static	ULONG	FdCd=0			;
typedef	struct
{
	char	NameHD[13+sizeof(PATH_NAR_HD)]	;
	ULONG	SaveTimer			;
	ULONG	SizeFileHD			;

} T_FILEONHD	;

T_FILEONHD	TabFileOnHD[MAX_FILE_VOICE] ;

ULONG	TotalSizeFileHD = 0		;
LONG	LanguageCD = 0			;// Franáais, a changer of course!
LONG	NbFileOnHD = 0			;// No File for the moment
LONG	MaxVoice=0			;
#endif
//------------------------------

static  char    *PtDial         ;
static  char    Word[256]       ;
static  char    *PtWord         ;
static  LONG    X_Dial, Y_Dial  ;
static  LONG    NbLineDial      ;
static  LONG    MaxLineDial=3   ;
static  LONG    FlagEnd3Line=FALSE;
static  LONG    FlagRunningDial=FALSE;
static  LONG    FlagEndDial=FALSE;
static  LONG    CurrentDial=0   ;
static  LONG    LenWord         ;

/*-------------------------------------------------------------------------*/
void	TimeBar( ULONG max, ULONG val )
{
	ULONG	x, dx	;

	MemoClip()	;
	UnSetClip()	;

	CopyBlock( 16, 400, 623, 440, Log, 16, 400, Screen );

	x = RegleTrois32( 17, 622, max, val ) ;//	Lib_3D

	PalOne( 14, 23,0,64 ) 	;
	PalOne( 15, 0,255,255 ) 		;

	Rect( 16, 400, 623, 440, 15 )	;
	Box( 17, 401, x, 439, 14 )	;

	CoulFont( 15 )			;
	dx = SizeFont( PleaseWait )	;
	Font( 320-dx/2, 400, PleaseWait );

	CopyBlockPhys( 16, 400, 623, 440 );

	CopyBlock( 16, 400, 623, 440, Screen, 16, 400, Log );

	RestoreClip()	;
}
/*-------------------------------------------------------------------------*/

void	InitLanguage()
{
	UBYTE	string[256] ;
	LONG	n ;

	strcpy( string, Def_ReadString( PathConfigFile, "Language" ) ) ;

	for( n=0; n<NB_LANGUAGES; n++ )
	{
		if( !stricmp( string, TabLanguage[n] ) )
		{
			Language = n 	;
			break		;
		}
	}

#ifdef	CDROM
	strcpy( string, Def_ReadString( PathConfigFile, "LanguageCD" ) ) ;

	FlagSpeak = FALSE	;

	for( n=0; n<NB_LANGUAGES; n++ )
	{
		if( !stricmp( string, TabLanguage[n] ) )
		{
			LanguageCD = n	;
			FlagSpeak = TRUE;
			break		;
		}
	}

	if ( !Wave_Driver_Enable ) FlagSpeak = FALSE	;// No Sound PC

	strcpy( string, Def_ReadString( PathConfigFile, "FlagKeepVoice" ) ) ;
	if ( !strcmpi( string, "ON" ))	FlagKeepVoice = 1	;
	else				FlagKeepVoice = 0	;// default

	strcpy( string, Def_ReadString( PathConfigFile, "FlagDisplayText" ) ) ;
	if ( !strcmpi( string, "OFF" ))	FlagDisplayText = 0	;
	else				FlagDisplayText = 1	;// default
#endif
}

/*-------------------------------------------------------------------------*/

LONG    FindText( LONG text )
{
	UWORD *pt       ;
	LONG i          ;

	pt = BufOrder   ;

	for ( i = 0 ; i < MaxText ; i++ )
	{
		if ( *pt++ == text )    return( i )     ;
	}
	return( -1 )    ;
}
#ifdef	TITRE
/*-------------------------------------------------------------------------*/
	  €ﬂﬂﬂﬂ €ﬂﬂﬂ‹ €ﬂﬂﬂ€ €ﬂﬂﬂ€ €‹ ‹€       €ﬂﬂﬂ€ €ﬂﬂﬂ€ €ﬂﬂﬂ€ ﬂﬂ€ﬂﬂ
//	  €€    €€  € €€ﬂ€ﬂ €€  € €€ﬂ €       €€ﬂﬂﬂ €€ﬂﬂ€ €€ﬂ€ﬂ   €€
	  ﬂﬂﬂﬂﬂ ﬂﬂﬂﬂ  ﬂﬂ  ﬂ ﬂﬂﬂﬂﬂ ﬂﬂ  ﬂ       ﬂﬂ    ﬂﬂ  ﬂ ﬂﬂ  ﬂ   ﬂﬂ
/*-------------------------------------------------------------------------*/
#endif
#ifdef	CDROM
/*-------------------------------------------------------------------------*/
//	Pour prendre en compte les eventuels fichiers dÇjÖ sur HD
void	InitVoiceFile()
{
	char	pathname[_MAX_PATH];
	struct	find_t	fileinfo;
	ULONG	time		;
	ULONG	rc		;

	rc = _dos_findfirst( PATH_NAR_HD"*.VOX", _A_NORMAL, &fileinfo ) 	;

	while( !rc )
	{
		time  = fileinfo.wr_date	;
		time += fileinfo.wr_time	;
		strcpy( pathname, PATH_NAR_HD )	;
		strcat( pathname, fileinfo.name);
		AddFileNameOnHD( pathname, fileinfo.size, time ) ;
#ifdef	LORAN
		AffStatusVoiceFile()		;
#endif
		rc = _dos_findnext( &fileinfo ) ;
	}
}
/*-------------------------------------------------------------------------*/
void	ClearVoiceFile()
{
	T_FILEONHD *pt	;
	LONG 	i	;

	if ( FlagKeepVoice )	return	;

#ifdef	DEBUG_TOOLS
Message("FlagKeepVoice=0 !!!!!", TRUE )	;
return	;
#endif

	pt = TabFileOnHD;

	for ( i = 0 ; i < NbFileOnHD ; i++, pt++ )
	{
		if ( FileSize( pt->NameHD ))//	On ne sait jamais,
			Delete( pt->NameHD );// un bug est si vite arrive :-o
	}

	TotalSizeFileHD = 0	;
}
/*-------------------------------------------------------------------------*/
void	CalcTotalSizeFileOnHD()
{
	T_FILEONHD *pt	;
	LONG	i	;

	TotalSizeFileHD = 0	;
	pt = TabFileOnHD	;

	for ( i = 0 ; i < NbFileOnHD ; i++, pt++ )
		TotalSizeFileHD += pt->SizeFileHD	;

}
/*-------------------------------------------------------------------------*/
#ifdef	LORAN
void	AffStatusVoiceFile()
{
	T_FILEONHD *pt	;
	LONG	i	;

	Text( 10, 8, "%FStatus des fichiers voix :")	;

	pt = TabFileOnHD;
	for ( i = 0 ; i < NbFileOnHD ; i++, pt++ )
	{
		Text( 10, i*10+16, "%FName : %s  Size : %L  Time : %L", pt->NameHD, pt->SizeFileHD, pt->SaveTimer ) ;
	}
        while( !Key )	;
	while( Key )	;
}
#endif
/*-------------------------------------------------------------------------*/
void	AddFileNameOnHD( char *filename, ULONG size, ULONG timer )
{
	T_FILEONHD *pt	;
	LONG	i	;

	pt = TabFileOnHD	;

	for ( i = 0 ; i < NbFileOnHD ; i++, pt++ )
	{
		if ( !strcmp( pt->NameHD, filename ))	break	;
	}

	strcpy( pt->NameHD, filename )	;//	Eventuelle recopie...
	pt->SaveTimer = timer		;
	pt->SizeFileHD = size		;

	if ( i == NbFileOnHD )	NbFileOnHD++	;// Maj ou Add

	CalcTotalSizeFileOnHD()		;//	Place prise
}
/*-------------------------------------------------------------------------*/
void	DeleteOlderFileHD()
{
	T_FILEONHD *pt, *pt1	;
	LONG	i		;
	ULONG	min, num	;

	min = 0xFFFFFFFFL	;//	le plus rÇcent, non?

	num = 0			;//	Suis obligÇ dans trouvÇ un!

	pt = TabFileOnHD	;

	for ( i = 0 ; i < NbFileOnHD ; i++, pt++ )
	{
		if ( pt->SaveTimer <= min )
		{
			min = pt->SaveTimer	;
			num = i			;
		}
	}

	pt--	;//	Stay on Last Entry

	pt1 = &TabFileOnHD[num]	;

	if ( FileSize( pt1->NameHD ))	Delete( pt1->NameHD );//  cf Del upper

	TotalSizeFileHD -= pt1->SizeFileHD	;// Maj TotalSizeFileHD

	strcpy( pt1->NameHD, pt->NameHD )	;// Permut Current & Last
	pt1->SaveTimer = pt->SaveTimer		;
	NbFileOnHD--				;// One less
}
/*-------------------------------------------------------------------------*/
//	Copy File CD On HD ( filename for CD must don't contain drive !!!)
LONG	CopyFileCD_HD( char *filecd, char *filehd )
{
	ULONG	fd	;
	LONG	size, wr;
	LONG	i, sect	;
	char	fname[256];


	fname[0] = DriveCDR+'A'	;
	fname[1] = ':'		;
	fname[2] = 0		;
	strcat( fname, filecd )	;

	fd = OpenRead( fname )	;
	if ( !fd )	return(0);
	Close(fd)		;

	GetFileCDR( filecd )	;// => FileCD_Start, FileCD_Sect & FileCD_Size

	size = GetHDFreeSize()	;//	HD Free Space

	size -= (1024L*1024L)	;//	1 megas marge

	if ( FileCD_Size >= size )	return(0);

	fd = OpenWrite( filehd )	;
	if ( !fd )	return(0)	;
//------------------------------------------------ 64Ko fast copy   :-)
	sect = FileCD_Start+FileCD_Sect	;
	size = FileCD_Size		;

	for ( i = FileCD_Start ; i <= sect-32 ; i += 32 )
	{
		if (ReadLongCDR( i, 32, BufSpeak ))
		{
			Close(fd)	;
			Delete( filehd );
			return(0)	;
		}
		wr = Write( fd, BufSpeak, 256L*256L )	;
		if ( wr != 256L*256L )
		{
			Close(fd)	;
			Delete( filehd );
			return(0)	;
		}
		size -= (256L*256L)	;
		TimeBar( FileCD_Size, FileCD_Size-size );
	}
//------------------------------------------------ Copy left Bytes
	sect -= i	;

	if ( sect )
	{
		if (ReadLongCDR( i, sect, BufSpeak ))
		{
			Close(fd)	;
			Delete( filehd );
			return(0)	;
		}
		wr = Write( fd, BufSpeak, size )	;
		if ( wr != size )	//	Juste a la fin !  :-(
		{
			Close(fd)	;
			Delete( filehd );
			return(0)	;
		}
	}

	TimeBar( FileCD_Size, FileCD_Size );
	Close(fd)	;

	return(1)	;
}
/*-------------------------------------------------------------------------*/
LONG	TryCopyFileCD_HD( char *filecd, char *filehd )
{
	ULONG	freesize	;

	freesize = GetHDFreeSize()-(1024L*1024L) ;//	1 Mega de marge

	if (!GetFileCDR( filecd ))	return(0);// Ne doit Jamais Arriver!

	if ( freesize > FileCD_Size )		// Y a la place :-)
		return( CopyFileCD_HD( filecd, filehd ))	;

//------------------------------ Ok, y a pas la place for the moment :-(

	if ( !NbFileOnHD )	return(0)	;// Nothing to delete :-<

	if ( TotalSizeFileHD+freesize < FileCD_Size )
				return(0)	;// We can't do nothing :-[

	while( freesize < FileCD_Size )		 // Keep hope... ;-)
	{
		DeleteOlderFileHD()			;
		freesize = GetHDFreeSize()-(1024L*1024L);
	}

//----------------------------- Ok, maintenant on a forcÇment la place...

	return( CopyFileCD_HD( filecd, filehd ))	;//  :-)
}
/*-------------------------------------------------------------------------*/
void	CloseFdNar()
{
	if ( FdNar )//	Fichier ouvert CD ou HD, on doit le fermer
	{
		Close(FdNar)		;
		FdNar = 0L		;
	}
}
/*-------------------------------------------------------------------------*/
void	InitFileNar( char *file, LONG from )
{
	ULONG	offset, wr	;

	CloseFdNar()		;

	FlagVoiceCD = 0		;//Je touche pas au CD pour le moment

	if (( from == FROM_CD ) AND ( GetMusicCD() != -1 ))
		StopMusicCD()	;

	FdNar = OpenRead( file )		;// CD or HD, it's working
						 // with FdNar, so...  ;-)
	if ( !FdNar )   return                  ;

	offset = 0				;

	while(!offset)				 // Sans Filet
	{
		wr = Read( FdNar, &offset, 4L )	;
	}

	MaxVoice = (offset/4)-1			;// Normalement en phase
						 // avec MaxText mais bon,
						 // en chantier... {:) --D 	(un ouvrier, je sais, il est nul celui la, dÇsolÇ...)
	Seek( FdNar, 0L, SEEK_START )		;

	wr = Read( FdNar, BufMemoSeek, offset ) ;
	if ( wr != offset )
	{
		CloseFdNar()    ;
		return          ;
	}

	FlagVoiceCD = from	;//	0 si HD, 1 si CD (cf InitSpeak )
}
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void    InitSpeak( LONG file )
{
	LONG	size		;
	char	filehd[256]	;
	char	filenar[256]	;

//	if ( DriveCDR < 0 )	return		;// a virer

	strcpy( filehd, PATH_NAR_HD )		;
	strcat( filehd, ListLanguage[LanguageCD] );
	strcat( filehd, ListFileText[file])    ;
	AddExt( filehd, EXT_NAR)                ;

	if ( size = FileSize( filehd ))		//	Already on HD
	{
		AddFileNameOnHD( filehd, size, ComputeTime() );// Maj Timer
		Touch( filehd )			;
		InitFileNar( filehd, FROM_HD )	;
		return				;
	}

	strcpy( filenar, PATH_NAR_CD )		;
	strcat( filenar, ListLanguage[LanguageCD] );
	strcat( filenar, ListFileText[file])    ;
	AddExt( filenar, EXT_NAR)               ;

	if ( GetMusicCD() != -1 )	StopMusicCD()				;

	if ( !TryCopyFileCD_HD( filenar, filehd ) )
	{
		filehd[0] = DriveCDR+'A'	;
		filehd[1] = ':'			;
		filehd[2] = 0			;
		strcat( filehd, filenar )	;
		InitFileNar( filehd, FROM_CD )	;// CD
	}
	else
	{
		AddFileNameOnHD( filehd, FileCD_Size, ComputeTime() );// FileCD_Size TryCopy...
		InitFileNar( filehd, FROM_HD )	;// HD
	}
}
/*-------------------------------------------------------------------------*/
void    PlaySpeakVoc(LONG fd)
{
	LONG    size    ;
	LONG    sizelzss;
	WORD    method  ;
	ULONG	volleft = 512 ;
	ULONG	volright = 512 ;


	Read( fd, &size, 4L )        ;
	Read( fd, &sizelzss, 4L )    ;
	Read( fd, &method, 2L )      ;

	if ( method == 0 )      Read( fd, BufSpeak, size )     ;
	else
	{
		CopyScreen( Screen, Log )	;
		Read( fd, Screen, sizelzss )	;/* 256Ko ds Screen */
		Expand( Screen, BufSpeak, size );
		CopyScreen( Log, Screen )	;
	}

	FlagNextVoc = *BufSpeak   ;
/*
#ifndef	LORAN
	if( NumObjSpeak != -1 )
	{
		ProjettePoint(	ListObjet[NumObjSpeak].PosObjX-WorldXCube,
				ListObjet[NumObjSpeak].PosObjY-WorldYCube,
				ListObjet[NumObjSpeak].PosObjZ-WorldZCube ) ;

		GiveBalance( Xp, Yp, 724, &volleft, &volright ) ;
	}
#endif
*/
//	Loran, modif du 4/10/94 au cas ou SayMessage avant ( crac HP )
	if ( WaveInList(SPEAK_SAMPLE))  WaveStopOne(SPEAK_SAMPLE)	;

#ifdef	LORAN
	WavePlay( SPEAK_SAMPLE, 0x1000, 1, 0, 1024, 1024, BufSpeak )	;
#else
	WavePlay( SPEAK_SAMPLE, 0x1000, 1, 0, volleft, volright, BufSpeak )	;
#endif

	NumObjSpeak = -1 ;


}
/*-------------------------------------------------------------------------*/
LONG	TestSpk( LONG fd )
{
	if ( !FlagSpeak )	return(0)	;//     Le joueur ne veut pas
	if ( !fd )		return(0)	;//     Le programme ne peut pas

	if ( WaveInList( SPEAK_SAMPLE ))	return(1)	;// áa joue...

	if ( FlagNextVoc )			// Ya une suite...
	{
		PlaySpeakVoc( fd )  		;//	Scratch Screen
		return(1)			;// Ca continue de jouer...
	}

	return(0)	;			// A plus, snif... :-(
}
/*-------------------------------------------------------------------------*/
LONG	TestSpeak()
{
	return(TestSpk( FdNar ));
}
/*-------------------------------------------------------------------------*/
//#ifdef	A_VOIR_SI_UTIL_STOP_MUSIC_BEFORE_PLAYING

LONG    TestSpeakFromCD()
{
	LONG	ret	;

	if ( DriveCDR < 0 )	return(0)	;

	ret = TestSpk( FdCd )	;

	if (( !ret ) AND ( FdCd ))
	{
		Close(FdCd)	;
		FdCd = 0L	;
	}

	return( ret )	;
}
/*-------------------------------------------------------------------------*/
void    SpeakFromCD( LONG file, LONG text )
{
	UWORD	*pt		;
	LONG    num, offset     ;
	LONG	max		;
	char	tmpname[256]	;

	if ( DriveCDR < 0 )	return	;

	strcpy( tmpname, PATH_TXT )            		;// Pas FileText!
	strcat( tmpname, ListLanguage[LanguageCD] )	;
	strcat( tmpname, ListFileText[file])   		;
	AddExt( tmpname, ".ord")               		;


//--------------------------------------
	max = FileSize( tmpname )/2	;
	Load( tmpname, BufSpeak )	;// Ne pas bousiller BufOrder!

	pt = (UWORD*)BufSpeak   	;

	for ( num = 0 ; num < max ; num++ )
	{
		if ( *pt++ == text )
			break     	;
	}

	if ( num == max )	return	;
//--------------------------------------

	tmpname[0] = DriveCDR+'A'			;
	tmpname[1] = ':'				;
	tmpname[2] = 0					;
	strcat( tmpname, PATH_NAR_CD )			;
	strcat( tmpname, ListLanguage[LanguageCD] )	;
	strcat( tmpname, ListFileText[file])   		;
	AddExt( tmpname, EXT_NAR)              		;

	FdCd = OpenRead( tmpname )	;
	if ( !FdCd )	return		;

	Seek( FdCd, num*4L, SEEK_START );

	Read( FdCd, &offset, 4L )	;

	if ( !offset )
	{
		Close(FdCd)	;
		return		;
	}

	Seek( FdCd, offset, SEEK_START );
	PlaySpeakVoc(FdCd) 		;
}
/*-------------------------------------------------------------------------*/
LONG	IsVoiceFileOnHD( LONG island )
{
	char	filehd[_MAX_PATH]	;

	strcpy( filehd, PATH_NAR_HD )			;
	strcat( filehd, ListLanguage[LanguageCD] )	;
	strcat( filehd, ListFileText[island])    	;
	AddExt( filehd, EXT_NAR)                	;

	if ( FileSize( filehd ))	return( 1L )	;

	return(0L)	;
}
/*-------------------------------------------------------------------------*/
LONG    Speak( LONG text )
{
	LONG    num, offset     ;

	if ( !FdNar )           return(0L)	;

	num = FindText( text )	;

	if (( num == -1 )
	OR  ( num >= MaxVoice )) return(0L)	;//	Ne doit pas arrivÇ,
						 // 	sauf en cours de dvp...
	offset = BufMemoSeek[num]       ;

	if ( !offset )  return(0L)	;

	Seek( FdNar, offset, SEEK_START );
	PlaySpeakVoc( FdNar )		;
	return(1L)			;
}
/*-------------------------------------------------------------------------*/
void    StopSpeak()
{
	FlagNextVoc = FALSE		;// Important, dans tout les cas !
	if ( !FlagSpeak )       return  ;
	if ( !FdNar )           return  ;
	if ( WaveInList(SPEAK_SAMPLE))  WaveStopOne(SPEAK_SAMPLE)               ;
}
/*-------------------------------------------------------------------------*/
#endif
#ifdef	TITRE
/*-------------------------------------------------------------------------*/
	     €ﬂﬂﬂﬂ €ﬂﬂﬂ‹ €ﬂﬂﬂ€ €ﬂﬂﬂ€ €‹ ‹€       €ﬂﬂﬂﬂ €€‹ € €ﬂﬂﬂ‹
//           €€    €€  € €€ﬂ€ﬂ €€  € €€ﬂ €       €€ﬂﬂ  €€ﬂ€€ €€  €
	     ﬂﬂﬂﬂﬂ ﬂﬂﬂﬂ  ﬂﬂ  ﬂ ﬂﬂﬂﬂﬂ ﬂﬂ  ﬂ       ﬂﬂﬂﬂﬂ ﬂﬂ  ﬂ ﬂﬂﬂﬂ
/*-------------------------------------------------------------------------*/
#endif
/*-------------------------------------------------------------------------*/
void    InitDial( LONG file )
{
	char	*pt	;
	LONG i, j, size	;
	LONG offset0	;
	LONG offset1	;
	LONG code	;

	if ( LastFileInit == file )	return	;
	LastFileInit = file			;

#ifdef	CRYPTAGE
	strcpy( FileText, PATH_TXT )            ;
	strcat( FileText, ListLanguage[Language] )   ;
	strcat( FileText, ListFileText[file])   ;
	AddExt( FileText, ".ord")               ;

	MaxText = FileSize( FileText ) / 2      ;/* WORD        */
	Load( FileText, BufOrder )   		;
	AddExt( FileText, ".dia")               ;
	Load( FileText, BufText )		;
//-------------------------- Decryptage

	for ( i = 0 ; i < MaxText ; i++ )
	{
		offset0 = *(UWORD*)(BufText+(i+0)*2)	;
		offset1 = *(UWORD*)(BufText+(i+1)*2)	;

		size = offset1 - offset0		;

		code = INIT_CODE                	;

		pt = BufText + offset0			;

		for ( j = 0 ; j < size ; j++ )
			*pt++ ^= (code++)       	;
	}

#else
	strcpy( FileText, PATH_TXT )            ;
	strcat( FileText, NAME_HQR_TEXT )	;

	MaxText = Load_HQR( FileText, BufOrder,
			  ( Language*MAX_TEXT_LANG*2)+(file*2)+0)/2 ;

	Load_HQR( FileText, BufText,
			  ( Language*MAX_TEXT_LANG*2)+(file*2)+1) ;


#endif

#ifdef	CDROM
	if ( FlagSpeak )        InitSpeak(file) ;
#endif
}
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void    ClearDial()
{
}
/*-------------------------------------------------------------------------*/
void    GetNextWord(char *ptchaine, char *mot)
{
	char    *dst    ;

	dst = mot       ;
	LenWord = 0     ;

	while(( *ptchaine != 0 )
	AND ( *ptchaine != 1 )
	AND ( *ptchaine != ' ' ))
	{
		*dst++ = *ptchaine++    ;
		LenWord++               ;
	}

	*dst = 0                        ;
	SizeWord = SizeFont(mot)        ;
}
/*-------------------------------------------------------------------------*/
void    PushCar( LONG x, LONG y, LONG c )
{
	LONG i  ;

	if ( NbCar < MAX_CAR )
	{
		StackCar[NbCar].c = c   ;
		StackCar[NbCar].x = x   ;
		StackCar[NbCar].y = y   ;
		NbCar++                 ;
	}
	else
	{
		for ( i = 0 ; i < MAX_CAR-1 ; i++ )
		{
			StackCar[i].c = StackCar[i+1].c ;
			StackCar[i].x = StackCar[i+1].x ;
			StackCar[i].y = StackCar[i+1].y ;
		}
		StackCar[MAX_CAR-1].c = c       ;
		StackCar[MAX_CAR-1].x = x       ;
		StackCar[MAX_CAR-1].y = y       ;
	}


}
/*-------------------------------------------------------------------------*/
void    AffOneCar( LONG x, LONG y, LONG c, LONG coul )
{
	LONG	x0, y0, x1, y1	;

	if ( c != 32 )
	{
#ifdef  LBA_EDITOR
		if ( !QuickMessage )
#endif
		{
			CoulFont( 0 )           ;
			CarFont( x+2, y+4, c )  ;
		}
		CoulFont( coul )                ;
		CarFont( x, y, c )              ;
		x0 = x		;
		y0 = y		;
		x1 = x+32	;
		y1 = y+38	;
                if ( x0 < Dial_X0+1 )	x0 = Dial_X0+1	;
		if ( y0 < Dial_Y0+1 )	y0 = Dial_Y0+1	;
		if ( x1 > Dial_X1-1 )	x1 = Dial_X1-1	;
		if ( y1 > Dial_Y1-1 )	y1 = Dial_Y1-1	;

		CopyBlockPhys( x0, y0, x1, y1 )		;

//		CopyBlockPhys( x, y, x+32, y+38 );// Loran le 20/09/94
	}
}
/*-------------------------------------------------------------------------*/
void    AffAllCar()
{
	T_STACKCAR *pt  ;
	LONG i, c, x, y ;


	c = MaxDegrade  ;

	pt = &StackCar[NbCar-1] ;

	x = pt->x       ;
	y = pt->y       ;

#ifdef  LBA_EDITOR
	if ( !QuickMessage )
#endif
		while( SaveTimerForCar == TimerRef )	;

	SaveTimerForCar = TimerRef			;

	for ( i = NbCar-1 ; i >= 0 ; i--, pt-- )
	{
		CoulFont( c )                           ;
		AffOneCar( pt->x, pt->y, pt->c, c )     ;

		if ( c-StepDegrade/* >= */ <= MinDegrade ) c -= StepDegrade     ;
		else                               c = MinDegrade       ;
	}
}
/*-------------------------------------------------------------------------*/
void    CoulDial( LONG min, LONG max, LONG step )
{
	MinDegrade = min        ;
	MaxDegrade = max        ;
	StepDegrade = step      ;
	NbDegrade = (((MaxDegrade-MinDegrade)+1)/StepDegrade)   ;
}

void    TestCoulDial( WORD coul )
{
	MinDegrade = coul*16+12 ;	//	Loran
	MaxDegrade = coul*16  ;
	StepDegrade = -1 ;
	NbDegrade = 14 ;		//	Loran
}
/*-------------------------------------------------------------------------*/
LONG    GetText( LONG text )
{
	UWORD   offset0 ;
	UWORD   offset1 ;
	LONG    num 	;

	num = FindText( text )          ;/*     Must have good number   */
	if ( num == -1 )        return(0L);

	offset0 = *(UWORD*)(BufText+((num+0)*2L));
	offset1 = *(UWORD*)(BufText+((num+1)*2L));

	PtText = BufText+offset0	;

	SizeText = offset1-offset0      ;

	return(1L)                      ;
}
/*-------------------------------------------------------------------------*/
void    AffFleche()
{
	TabPoly[0] = MinDegrade         ;
	TabPoly[1] = Dial_X1 - 3        ;
	TabPoly[2] = Dial_Y1 - 24       ;

	TabPoly[3] = MinDegrade         ;
	TabPoly[4] = Dial_X1 - 24       ;
	TabPoly[5] = Dial_Y1 - 3        ;

	TabPoly[6] = MaxDegrade         ;
	TabPoly[7] = Dial_X1 - 3        ;
	TabPoly[8] = Dial_Y1 - 3        ;

	TypePoly = 0 ;
	NbPolyPoints = 3 ;
	if( ComputePoly() )
		FillVertic( TypePoly, MinDegrade ) ;

	CopyBlockPhys( Dial_X1-24, Dial_Y1-24, Dial_X1-3, Dial_Y1-3 ) ;
}
/*-------------------------------------------------------------------------*/
void    InitDialWindow()
{
	CopyBlock(      Dial_X0, Dial_Y0, Dial_X1, Dial_Y1, Screen,
			Dial_X0, Dial_Y0, Log )         ;

	// Fred message shade on/off
	if( FlagMessageShade )
	{
		DrawCadre( Dial_X0, Dial_Y0, Dial_X1, Dial_Y1 ) ;
		ShadeBox( Dial_X0+1, Dial_Y0+1, Dial_X1-1, Dial_Y1-1, 3 )               ;
	}
	CopyBlockPhys( Dial_X0, Dial_Y0, Dial_X1, Dial_Y1 )     ;
	NbCar = 0       ;
	CopyBlock( Dial_X0, Dial_Y0, Dial_X1, Dial_Y1, Log,
		   Dial_X0, Dial_Y0, Screen )     ;
//	CopyScreen( Log, Screen ) ;
}
/*-------------------------------------------------------------------------*/
void	SecondInitDialWindow()
{
	CopyBlock(      Dial_X0, Dial_Y0, Dial_X1, Dial_Y1, Screen,
			Dial_X0, Dial_Y0, Log )         ;

	CopyBlockPhys( Dial_X0, Dial_Y0, Dial_X1, Dial_Y1 )     ;
	NbCar = 0       ;
}
/*-------------------------------------------------------------------------*/
void    NormalWinDial()
{
	Dial_X0 = 16    ;
	Dial_Y0 = 334   ;
	Dial_X1 = 639-16;
	Dial_Y1 = 479-16;

	MaxLineDial = 3         ;
	DialMaxSize = ((Dial_X1-8)-(Dial_X0+8))    ;
}
/*-------------------------------------------------------------------------*/
void    BigWinDial()
{
	Dial_X0 = 8     ;
	Dial_Y0 = 8     ;
	Dial_X1 = 639-8 ;
	Dial_Y1 = 479-8 ;

	MaxLineDial = 11        ;
	DialMaxSize = ((Dial_X1-8)-(Dial_X0+8)) ;

}
/*-------------------------------------------------------------------------*/
/*
void    DefWinDial( LONG x0, LONG y0, LONG x1, LONG y1 )
{
	WORD    yh, yb  ;
	WORD    xl, xr  ;

	Dial_X0 = x0    ;
	Dial_Y0 = y0    ;
	Dial_X1 = x1    ;
	Dial_Y1 = y1    ;

	xl = Dial_X0+8  ;
	xr = Dial_X1-8  ;
	yh = Dial_Y0+8  ;
	yb = Dial_Y1-40 ;

	DialMaxSize = ((Dial_X1-8)-(Dial_X0+8)) ;
	MaxLineDial = (yb-yh)/36        ;
}
*/
/*-------------------------------------------------------------------------*/

void	CommonOpenDial( LONG text )
{
	CloseDial()             ;
	if ( !GetText( text ))  return  ;
	CurrentDial = text      ;
	X_Dial = Dial_X0+8      ;
	Y_Dial = Dial_Y0+8      ;
	NbLineDial = 0          ;
	PtDial = PtText		;
	*Word = 0               ;
	PtWord = Word           ;
	*BufLine = 0            ;
	PtLine = BufLine        ;
	NbCar = 0               ;
	FlagEndDial = FALSE     ;
	FlagEnd3Line = FALSE    ;
	FlagRunningDial = TRUE  ;

	SetFont( LbaFont, INTER_LEAVE, INTER_SPACE )    ;
}

/*-------------------------------------------------------------------------*/

void    OpenDial(LONG text)
{
	CommonOpenDial( text ) ;

	InitDialWindow()        ;
#ifdef	LBA_EDITOR
	if ( QuickMessage )
	{
		CoulText( JAUNE, -1 )	;
		Text( Dial_X0+2, Dial_Y0+2, "%FMess:%l   ", text ) ;
	}
#endif
}
/*-------------------------------------------------------------------------*/
void    OpenDialNoWindow(LONG text)
{
	CommonOpenDial( text ) ;
}
/*-------------------------------------------------------------------------*/
void    GetNextLine()
{
	LONG    dx, dd  ;
	LONG    flag    ;

	SizeLine = 0    ;
	NbSpace = 0     ;
	*BufLine = 0    ;
	SizeSpace = INTER_SPACE ;
	flag = 1        ;//     Justifie

	while(TRUE)
	{
		while( *PtDial == ' ' ) PtDial++;
		if ( *PtDial == 0 )     break   ;
		GetNextWord( PtDial, Word )     ;
		if ( SizeLine+SizeSpace+SizeWord > DialMaxSize ) break  ;
//------------- Caractere SpÇciaux, saut de ligne, de page, etc.
		if ( *PtDial == 1 )
		{
			PtDial++        ;
			flag = 0        ;
			break           ;
		}
		if ( *Word == '@' )
		{
			flag = 0                ;
			PtDial++                ;
			if ( SizeLine == 0 )// Line Vide
			{
				strcpy( BufLine, " " )  ;
				SizeLine = INTER_SPACE  ;
			}
			if ( *(Word+1) == 'P' )
			{
				NbLineDial = MaxLineDial;
				PtDial++                ;
			}
			break   ;
		}
//-------------
		PtDial += LenWord               ;
		strcat( BufLine, Word )         ;
		strcat( BufLine, " " )          ;
		SizeLine += (SizeWord+SizeSpace);
		NbSpace++                       ;
		if ( *PtDial == 0 )     break   ;
		PtDial++                        ;
	}

	if ( NbSpace )  NbSpace--       ;// Ne doit jamais arrive :-]

	if (( *PtDial != 0 )             // Derniere Ligne ou Ligne seule
	AND ( flag == 1 ))               // Pas tout le temps, c'est a c...
	{
		dx = DialMaxSize-SizeLine       ;
		dd = dx/NbSpace                 ;
		SizeSpace += dd                 ;
		dd = NbSpace*dd                 ;
		NbBigSpace = dx-dd              ;
	}

	LenLine = strlen(BufLine)       ;
	PtLine = BufLine                ;
}
/*-------------------------------------------------------------------------*/
void    InitEndPage()
{
	LONG    i       ;

	*BufLine = 0                    ;
	for ( i = 0 ; i < NbDegrade ; i++ )
		strcat( BufLine, " ")   ;
	PtLine = BufLine                ;
	LenLine = NbDegrade             ;
	SizeLine = 16                   ;// Bidon
	NbLineDial = 0                  ;
}
/*-------------------------------------------------------------------------*/
LONG    NextDialCar()
{
	LONG    dx, dy       ;

	if ( FlagRunningDial == FALSE ) return(0)       ;

	if ( *PtLine == 0 )
	{
		if ( FlagEndDial )
		{
			if( FlecheForcee )
			{
				AffFleche() ;
			}
			CloseDial()             ;
			FlagRunningDial = FALSE ;
			return(0)               ;//     RETURN 0
		}
		if (  FlagEnd3Line )
		{
			SecondInitDialWindow()        ;
			FlagEnd3Line = FALSE    ;
			X_Dial = Dial_X0+8      ;
			Y_Dial = Dial_Y0+8      ;
		}
		if ( *PtDial == 0 )
		{
			InitEndPage()           ;
			FlagEndDial = TRUE      ;
			return(1)               ;//     RETURN 1
		}

		GetNextLine()   ;

	}

	if ( *PtLine != 0 )
	{
		PushCar( X_Dial, Y_Dial, *PtLine )      ;
		AffAllCar()                             ;
		GetDxDyMask( *PtLine, &dx, &dy, LbaFont );

		if ( *PtLine != ' ' )   X_Dial += dx+INTER_LEAVE;
		else
		{
			if ( NbBigSpace )
			{
				NbBigSpace--    ;
				X_Dial++        ;
			}
			X_Dial += SizeSpace     ;
		}

		PtLine++                                ;
		if ( *PtLine == 0 )
		{
			X_Dial = Dial_X0+8      ;
			Y_Dial += INTER_LINE    ;

			if( ( FlagEnd3Line == TRUE )
			AND (!FlagEndDial) )
			{
				AffFleche()     ;
				return(2)       ;//     RETURN 2
			}

			NbLineDial++            ;
			if ( NbLineDial >= MaxLineDial )
			{
				InitEndPage()           ;
				FlagEnd3Line = TRUE     ;
				if ( *PtDial == 0 )
					FlagEndDial = TRUE;
			}

		}
	}

	return(1)       ;
}
/*-------------------------------------------------------------------------*/
void    CloseDial()
{
	FlagRunningDial = FALSE ;
}
#ifdef	CDROM
/*-------------------------------------------------------------------------*/
//  			     D I A L    C D R O M
/*-------------------------------------------------------------------------*/
void    Dial( LONG text )
{
	LONG    ret = 0	;
	LONG flagabort=0;


	MemoClip()      ;
	UnSetClip()     ;

	CopyScreen( Log, Screen )       ;

	if ( FlagSpeak )
		ret = Speak(text)	;

	if (( !FlagDisplayText )	// Pas de texte et Voix OK
	AND ( ret ))
	{
		while((TestSpeak()) AND ( Key != K_ESC )) ;
		StopSpeak()	;
		RestoreClip()	;
		return		;
	}

	OpenDial( text );

	while(TRUE)
	{
		ret = NextDialCar()     ;
		TestSpeak()        ;
		if ( ret == 2 ) /*      Attente de Touche       */
		{
			while(( Key )
			OR    ( Fire )
			OR    ( Joy ))	TestSpeak()	;// Tu relaches ?

			while(( !Key )
			AND   ( !Fire )
			AND   ( !Joy ))	TestSpeak()	;// Tu appuies ?
		}

		if ( Key == K_ESC )//	Player Abort...
		{
			flagabort = 1	;
			break		;
		}

		if ((( ret == 0 ) AND ( !TestSpeak()))// Tout fini
		OR ( flagabort )) break ;	      // Ou Abort by player
	}

	StopSpeak()     ;
	CloseDial()     ;
#ifndef LORAN
	if(( ret == 0 ) AND ( !flagabort ))
	{
		while(!(( !Key ) AND ( !Fire ) AND ( !Joy ))) ;

		while(( !Key ) AND ( !Fire ) AND ( !Joy )) ;
	}
#endif
/*
	if( FlagMessageShade )
		while( Key == K_ESC ) ; // attente relachement que pour escape
	// essai: ET que si il y a un cadre (ouf ouf)
*/
	RestoreClip()   ;
}
/*ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ*/
void	MyDial( WORD nummess )	// attends une touche si autre page sinon continue
{
	WORD	dialstat = 1 ;

	if ( FlagSpeak )	Speak(nummess)	;

	OpenDial( nummess ) ;

	while( dialstat )
	{
		dialstat = NextDialCar();
		TestSpeak() 		;
		if( dialstat == 2 )		// encore 1 page
		{
			while( Key OR Fire OR Joy ) 	 TestSpeak() ;
			while( !Key AND !Fire AND !Joy ) TestSpeak() ;
		}
	}

	while(TestSpeak())	;// Wait until silence

	StopSpeak() ;//	Security
	CloseDial() ;
}
#else
/*-------------------------------------------------------------------------*/
//			     D I A L  N O  C D
/*-------------------------------------------------------------------------*/
void    Dial( LONG text )
{
	LONG    ret     ;

	MemoClip()      ;
	UnSetClip()     ;

	CopyScreen( Log, Screen )       ;

	OpenDial( text );

	while(TRUE)
	{
		ret = NextDialCar()     ;
		if ( ret == 2 ) /*      Attente de Touche       */
		{
			while(!(( !Key ) AND ( !Fire ) AND ( !Joy )))	;
			if ( Key == K_ESC )     break   		;
			while(( !Key ) AND ( !Fire ) AND ( !Joy ))	;
		}

		if (( ret == 0 ) OR ( Key == K_ESC )) break ;
	}
	CloseDial()     ;
#ifndef LORAN
	if( ret == 0 )
	{
		while(!(( !Key ) AND ( !Fire ) AND ( !Joy ))) ;

		while(( !Key ) AND ( !Fire ) AND ( !Joy )) ;
	}
#endif
/*
	if( FlagMessageShade )
		while( Key == K_ESC ) ; // attente relachement que pour escape
	// essai: ET que si il y a un cadre (ouf ouf)
*/
	RestoreClip()   ;
}
/*ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ*/

void	MyDial( WORD nummess )	// attends une touche si autre page sinon continue
{
	WORD	dialstat = 1 ;

	OpenDial( nummess ) ;

	while( dialstat )
	{
		dialstat = NextDialCar() ;
		if( dialstat == 2 )		// encore 1 page
		{
			while( Key OR Fire OR Joy ) ;
			while( !Key AND !Fire AND !Joy ) ;
		}
	}

	CloseDial() ;
}
#endif
/*-------------------------------------------------------------------------*/
// attention size max ou tronquÇe Ö 255 car + 0
char    *GetMultiText( LONG text, char *dst )
{
	char    *pts    ;
	char    *ptd    ;
	LONG	smax ;

	if( (text == NumMultiText ) AND (Island == IslandMultiText) )
	{
		strcpy( dst, BufferMultiText ) ;
		return dst ;
	}

	if ( !GetText( text ))
	{
		*dst = 0	;
		return 0L       ;
	}

	pts = PtText		;
	ptd = dst		;

	smax = SizeText-1 	;

	if( smax > 255 )	smax = 255 ;

	MovMem( PtText, dst, smax ) ;
	dst[smax] = 0 ;

	MovMem( dst, BufferMultiText, smax+1 ) ;
	NumMultiText = text ;
	IslandMultiText = Island ;

	return(dst)     ;
}
/*-------------------------------------------------------------------------*/
#ifdef	LBA_EDITOR
void    CleanMessage( UBYTE *string, LONG flag )
{
	CopyScreen( Log, Screen )       ;
	Message( string, flag )         ;
	CopyScreen( Screen, Log )       ;
	Flip()                          ;
}
#endif
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/






