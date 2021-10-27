
//----------------------------------------------------------------------------


#pragma library ("f:\projet\lib386\lib_cd\lib_cd.lib");


extern  WORD    BufferData[]    ;
extern  WORD    DriveCDR        ;//     Lettre drive 0=A 1=B etc. apres InitCDR
extern  ULONG   LeadOut         ;
extern  UBYTE   LowTrack, HigTrack      ;
extern	LONG	FileCD_Start	;
extern	LONG	FileCD_Sect	;
extern	LONG	FileCD_Size	;

//----------------------------------------------------------------------------
//      Initialiser le lesteur de cdrom, si pas verif alors chaine vide
LONG    InitCDR( char *nameid )         ;

//      Jouer seulement une track pr‚cise
LONG    PlayTrackCDR( LONG track )      ;

//      Arreter le PlayTrackCDR
void    StopCDR()                       ;

//      Verouiller ou d‚verouiller la porte du lecteur
void    LockCDR()                       ;
void    UnLockCDR()                     ;

//      Reset, Init ou suite a une erreur par exemple
void    ResetCDR()                      ;

//      Ejecter le cdrom quand c'est possible ( pas toujours motoris‚ )
void    EjectCDR()                      ;

//      Fermer tiroir electrique quand c'est possible ( pas toujours motoris‚ )
void    CloseTrayCDR()                  ;

//      Reprends une track apres un stopCDR
void    ResumeCDR()                     ;

//-------------------------------------- Fonction bas niveau

//      Return pointeur sur Copyright File name ( pas forcement file )
char    *GetCopyrightName()             ;

//      Conversion Red to Hsg ( belle jambe non ;-) )
LONG    Red2Hsg( LONG value )           ;

//      Return la taille d'une track en HSG
LONG    GetLengthTrackCDR( LONG track ) ;

//      Return secteur depart Track
LONG    StartTrackCDR( LONG track )     ;

//      Return status of CD Drive
LONG    StatusCDR()                     ;

//      Return Track currently play or 0
LONG    GetMusicCDR()                   ;

//      Change volume selon chanel
void    ControlCDR( WORD chan0, WORD vol0, WORD chan1, WORD vol1 )      ;

//      Return position de la tete
LONG    LocateHeadCDR()                 ;

//      Play a part of cd
LONG    PlayCDR( LONG debut, LONG duree );

//      Lecture data
LONG    ReadLongCDR( LONG start, LONG nbsect, void *buffer ) ;

//	Get Directory Entry
LONG	GetFileCDR( char *name )	;

//----------------------------------------------------------------------------


