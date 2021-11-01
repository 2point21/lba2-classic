#include        "c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
 *══════════════════════════════════════════════════════════════════════════*/

#define HAUTEUR_STANDARD        50
#define MENU_SPACE              6

#define COUL_TEXT_MENU          LBAWHITE        // trouver blanc palette jeu
#define COUL_LETTER_INPUT       LBAWHITE

#ifdef  DEMO
#define COUL_TEXT_MENU_DISABLE  (3*16+11)       // trouver blanc palette jeu
#endif

#define SCREEN_SAVE_WIDTH       160
#define SCREEN_SAVE_HEIGHT      120

U8      CoulTextMenu = COUL_TEXT_MENU ;
S32     LargeurMenu = 550 ;

#ifdef  DEMO
S32     FlagShowEndDemo = FALSE ;
#endif

#define MAX_MAIN_MENUS  6

char	NewGameTxt[20] ;// ATTENTION: max 19 chars

U16     GameMainMenu[4+MAX_MAIN_MENUS*2] ;

// Menu Principal Complet permettant de construire GameMainMenu
U16     RealGameMainMenu[] = {
                        0,      // selected
                        6,      // nb entries
                        335,    // y center
                        0,      // .dia num
                        0, 70,  // reprendre partie
                        0, 71,  // nouvelle partie
                        0, 72,  // charger partie
                        0, 73,  // sauver partie
                        0, 74,  // Options
                        0, 75,  // Quitter
                                }       ;

#define MENU_CAMERA             4
#define MENU_STEREO             2
#define MENU_FULL_SCREEN      	6
#define MENU_DISPLAY_TEXT       7

U16     GameOptionMenu[] = {
			0,      // selected
			8,      // nb entries
			260,    // y center
			0,      // .dia num

			0, 26,  // menu precedent
			0, 11,  // reglage volumes
			0, 12,  // Reverse Stereo ON/OFF
			7, 45,  // Niveau des détails
			0, 47,  // Cameras Scénariques ON/OFF
			0, 14,  // Configuration Clavier
			0, 27,	// Video FullScreen ON/OFF
			0, 16,  // Afficher les Textes ON/OFF
				} ;

U16     SavedConfirmMenu[] = {
			0,      // selected
                        2,      // nb entries
                        350,    // y center.
                        0,      // .dia num

                        0, 24,  // annuler
                        0, 48   // détruire une sauvegarde

                                } ;

U16     GameChoiceMenu[] = {
                        0,      // selected
                        7,      // nb entries
                        0,      // y from top. byte 8(
                        0,      // .dia num

                        0, 0,
                        0, 1,
                        0, 2,
                        0, 3,
                        0, 4,
                        0, 5,
                        0, 6    }       ;

#ifdef  DEMO
U16     VolumeMenu[] = {
                        0,      // selected
			4,      // nb entries
                        240,    // y center
                        0,      // .dia num

                        0, 26,  // menu precedent
                        2, 19,  // reglage volume samples
			4, 21,  // reglage volume musiques (Jingles)
			6, 23,  // reglage volume Master
			} ;
#else
U16	*VolumeMenu	;

U16     VolumeMenuVoice[] = { // integere volume des voix
                        0,      // selected
                        6,      // nb entries
                        240,    // y center
                        0,      // .dia num

                        0, 26,  // menu precedent
                        2, 19,  // reglage volume samples
                        3, 20,  // reglage volume voix
                        4, 21,  // reglage volume musiques (Jingles)
                        5, 22,  // reglage volume CD
			6, 23,  // reglage volume Master
			} ;

U16     VolumeMenuNoVoice[] = { // integere volume des voix
			0,      // selected
			5,      // nb entries
                        240,    // y center
                        0,      // .dia num

                        0, 26,  // menu precedent
                        2, 19,  // reglage volume samples
			4, 21,  // reglage volume musiques (Jingles)
                        5, 22,  // reglage volume CD
                        6, 23,  // reglage volume Master
                        } ;
#endif

U32     TimerSample ;
S32     CursorActif = FALSE ;
S32     CursorX, CursorY ;

extern  S32     EscTimer ;

// save players games

#define NB_CAR_H        14
#define NB_CAR_V        5

char    PathSave[_MAX_PATH] ;
char    GamePathname[_MAX_PATH] ;
#ifdef	_WIN32
char    OldGamePathname[_MAX_PATH] ;
#endif
char    PlayerName[MAX_SIZE_PLAYER_NAME+1] ;

char    *MenuIncrustText = NULL ;
S32     MenuIncrustTimer = 0 ;
S32     MenuIncrustSprite= -1 ;

/*──────────────────────────────────────────────────────────────────────────*/

S32     SelectPlayer = 0 ;

/*──────────────────────────────────────────────────────────────────────────*/

#define SIZE_FOUND_OBJ  130

/*══════════════════════════════════════════════════════════════════════════*
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void    Init3DView( void )
{
        SetIsoProjection( 320-8-1, 240 )                ;
        SetPosCamera( 0,0,0 )                           ;
        SetAngleCamera( 0,0,0 )                         ;
        SetLightVector( AlphaLight, BetaLight, 0 )      ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    Init3DGame()
{
        Init3DView() ;

        // Initialise les pointeurs de fonctions
        PtrReajustPos   = ReajustPos ;
        PtrGetShadow    = GetShadow ;
        PtrDoAnim       = DoAnim ;
        PtrInitGrille   = InitGrille ;
        PtrWorldColBrick= WorldColBrick ;
        PtrFullWorldColBrick = FullWorldColBrick ;
        PtrWorldColBrickVisible= WorldColBrickVisible ;
        PtrProjectShadow    = ProjectShadow ;
        PtrReajustPosExtra   = ReajustPosExtra ;
        PtrInit3DView   = Init3DView ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    SetDetailLevel( void )
{
	S32	oldmaxpolysea  = MaxPolySea 	;
	U8	olddrawhorizon = FlagDrawHorizon;

        switch( DetailLevel )
	{
		case 0: // machine bof !!!!!!!!
			MaxPolySea = 0 		;// Pas d'eau animée
			Shadow     = 1 		;// Pas d'ombres sur extras
			RainEnable = FALSE 	;// Pas d'affichage de pluie
			FlagDrawHorizon = FALSE ;// Pas D'affichage des cubes autour
			break ;

		case 1: // 486 ?
			MaxPolySea = 0 		;// Pas d'eau animée
			Shadow     = 2 		;// pas d'ombres sur impacts
			RainEnable = FALSE 	;// Pas d'affichage de pluie
			FlagDrawHorizon = TRUE 	;// Affichage des cubes autour
			break ;

		case 2: // Pentium de base ?
			MaxPolySea = 0 		;// Pas d'eau animée
			Shadow     = 3 		;// full shadows
			RainEnable = TRUE 	;// Affichage de la pluie
			FlagDrawHorizon = TRUE 	;// Affichage des cubes autour
			break ;

		default:// machine de folie !!!!!
			if( PtrPolySea )        MaxPolySea = MAX_POLY_SEA ;
			Shadow     = 3 		;// full shadows
			RainEnable = TRUE 	;// Affichage de la pluie
			FlagDrawHorizon = TRUE 	;// Affichage des cubes autour
			break ;
	}

	if( FlagDrawHorizon!=olddrawhorizon
	OR  MaxPolySea!=oldmaxpolysea )
	{
		FirstTime = AFF_ALL_FLIP ;
	}
}

//──────────────────────────────────────────────────────────────────────────
void    InitMenuIncrustText( char *text, S32 sprite, S32 time )
{
        ManageTime() ;
        MenuIncrustText = text ;
        MenuIncrustTimer = TimerRefHR + time*1000 ;     // end message timer in s
        MenuIncrustSprite= sprite ;
}

//──────────────────────────────────────────────────────────────────────────
void    DrawFireBar( S32 xmin, S32 ymin, S32 xmax, S32 ymax, S32 coul )
{
        Struc_Point     TabPoint[4];

        TabPoint[0].Pt_XE       = (S16)xmin;
        TabPoint[0].Pt_YE       = (S16)ymin;
        TabPoint[0].Pt_MapU     = (U16)0;
        TabPoint[0].Pt_MapV     = (U16)0;

        TabPoint[1].Pt_XE       = (S16)xmin;
        TabPoint[1].Pt_YE       = (S16)ymax;
        TabPoint[1].Pt_MapU     = (U16)0;//(16<<8);
        TabPoint[1].Pt_MapV     = (U16)((63<<8)+250);

        TabPoint[2].Pt_XE       = (S16)xmax;
        TabPoint[2].Pt_YE       = (S16)ymax;
        TabPoint[2].Pt_MapU     = (U16)(2*((63<<8)+250));//(16<<8);
        TabPoint[2].Pt_MapV     = (U16)((63<<8)+250);

        TabPoint[3].Pt_XE       = (S16)xmax;
        TabPoint[3].Pt_YE       = (S16)ymin;
        TabPoint[3].Pt_MapU     = (U16)(2*((63<<8)+250));
        TabPoint[3].Pt_MapV     = (U16)0;

        RepMask = 0x3f3f; // repmask x=32 y=32

        switch( coul )
        {
                case FIRE_SELECT_MENU:
                case FIRE_INPUT_NAME:
                        SelectPlasmaBank( 12 ) ;
                        break ;

                case FIRE_SLIDER_MENU:
                        SelectPlasmaBank( 4 ) ;
                        break ;
        }

        DoTextureAnimation() ;

        FixePlasmaPtrMap() ;

        Fill_Poly( POLY_TEXTURE, 0, 4, TabPoint );
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*
             ██▀▀▀ █▀▀▀█ █   █ █▀▀▀▀       █     █▀▀▀█ █▀▀▀█ █▀▀▀▄
             ▀▀▀▀█ ██▀▀█ ██ ▄▀ ██▀▀        ██    ██  █ ██▀▀█ ██  █
             ▀▀▀▀▀ ▀▀  ▀ ▀▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

int my_sort_function( const void *a, const void *b)
{
        return( stricmp( *(char**)a, *(char**)b) ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32     PlayerGameList( char **ptrlistname, char *listname, S32 flagnew )
{
        struct  find_t  fileinfo ;
        U32     rc ;
        S32     wlong ;
        char    pathname[_MAX_PATH] ;
        S32     nb = 0 ;
        U32     handle ;
        char    **startptrlist = ptrlistname ;
        U8      wbyte ;

        strcpy( pathname, PathSave ) ;
        strcat( pathname, "*.LBA"  ) ;

        rc = _dos_findfirst( pathname, _A_NORMAL, &fileinfo ) ;
        while( !rc )
        {
                strcpy( pathname, PathSave  ) ;
                strcat( pathname,  fileinfo.name ) ;

                // On ne liste pas l'autosave ni la currentsave
                // D'ailleurs ces sauvegardes pourraient avoir une autre
                // extension que .LBA ce qui éviterait ce test !
                if( stricmp(pathname,AUTOSAVE_FILENAME)
                AND stricmp(pathname,CURRENTSAVE_FILENAME) )
                {
                        handle = OpenRead( pathname ) ;
                        if( handle )
                        {
                                Read( handle, &wbyte, 1 ) ;     // num version
                                Read( handle, &wlong, sizeof(wlong) ) ; // num cube

                                *ptrlistname++ = listname ;
                                do
                                {
                                        Read( handle, &wbyte, 1 ) ;
                                        *listname++ = wbyte ;
                                } while( wbyte != 0 ) ;

                                Close( handle ) ;

                                nb++ ;
                                if( nb == MAX_PLAYER )  return nb ;
                        }
                }
                rc = _dos_findnext( &fileinfo ) ;
        }

        qsort(  (void *)startptrlist,
                nb,
                sizeof( char * ),
                my_sort_function ) ;

        if( !flagnew AND nb<MAX_PLAYER AND FileSize( AUTOSAVE_FILENAME ) )
        {
                strcpy( GamePathname, AUTOSAVE_FILENAME ) ;
                LoadGamePlayerName() ;
                *ptrlistname = listname  ;
                strcpy( listname, PlayerName ) ;
                nb++ ;
        }

        return nb ;
}

/*──────────────────────────────────────────────────────────────────────────*/

S32     FindPlayerFile()
{
        struct  find_t  fileinfo ;
        U32     rc ;
        char    pathname[_MAX_PATH] ;
        char    playername[MAX_SIZE_PLAYER_NAME+1] ;
        U8      *ptr ;
        U32     handle ;
        S32     wlong ;
        U8      wbyte ;

        strcpy( pathname, PathSave ) ;
        strcat( pathname, "*.LBA"  ) ;

        rc = _dos_findfirst( pathname, _A_NORMAL, &fileinfo ) ;
        while( !rc )
        {
                strcpy( pathname, PathSave  ) ;
                strcat( pathname,  fileinfo.name ) ;

                handle = OpenRead( pathname ) ;

                Read( handle, &wbyte, 1 ) ;     // num version
                Read( handle, &wlong, sizeof(wlong) ) ; // num cube

                ptr = (U8*)playername ;
                do
                {
                        Read( handle, &wbyte, 1 ) ;
                        *ptr++ = wbyte ;

                } while( wbyte != 0 ) ;

                Close( handle ) ;

                if( !stricmp( PlayerName, playername ) )
                {
                        strcpy( GamePathname, PathSave  ) ;
                        strcat( GamePathname, fileinfo.name ) ;
                        return TRUE ;
                }

                rc = _dos_findnext( &fileinfo ) ;
        }

        if( FileSize( AUTOSAVE_FILENAME ) )
        {
                strcpy( playername, PlayerName ) ;

                strcpy( GamePathname, AUTOSAVE_FILENAME ) ;
                LoadGamePlayerName() ;

                if( !stricmp( PlayerName, playername ) )
                {
                        strcpy( PlayerName, playername ) ;
                        return TRUE ;
                }

                strcpy( PlayerName, playername ) ;
        }

        return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*
           █    ██▄ █ █▀▀▀█ █   █ ▀▀█▀▀       ██▄ █ █▀▀▀█ █▄ ▄█ █▀▀▀▀
           ██   ██▀██ ██▀▀▀ ██  █   ██        ██▀██ ██▀▀█ ██▀ █ ██▀▀
           ▀▀   ▀▀  ▀ ▀▀    ▀▀▀▀▀   ▀▀  ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

/*──────────────────────────────────────────────────────────────────────────*/

#define DrawCursor()    AffGraph( 0, CursorX+4, CursorY, HQR_Get(HQRPtrSprite,9) ) ;

void    DrawOneString( S32 x, S32 y, char *playername, S32 draw )
{
        S32     x0,y0,x1,y1 ;

        x0 = x - LargeurMenu/2 ;
        x1 = x + LargeurMenu/2 ;

        y0 = y - HAUTEUR_STANDARD/2 ;
        y1 = y + HAUTEUR_STANDARD/2 ;

        draw &= 0x0F ;

        // fond

        BackupAngles( x0, y0, x1, y1 ) ;

        if( draw == 0 )
        {
                CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
                ShadeBoxBlk( x0, y0, x1, y1, MENU_SHADE_LVL ) ;
        }
        if( draw == 1 )
        {
                DrawFire( y0, FIRE_INPUT_NAME ) ;
        }
        if( draw == 2 )
        {
                Box( x0, y0, x1, y1, 91 ) ;
        }

        // cadre
        DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;

        RestoreAngles( x0, y0, x1, y1 ) ;

        // text
        ColorFont( CoulTextMenu ) ;
        Font( x - SizeFont( playername )/2, y-18, playername ) ;

        if( CursorActif )       DrawCursor() ;

        // flip

        BoxStaticAdd( x0, y0, x1, y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    ClearOneString( S32 x, S32 y )
{
        S32     x0,y0,x1,y1 ;

        x0 = x - LargeurMenu/2 ;
        x1 = x + LargeurMenu/2 ;

        y0 = y - HAUTEUR_STANDARD/2 ;
        y1 = y + HAUTEUR_STANDARD/2 ;

        // fond
        CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
        if( FlagShadeMenu )     ShadeBoxBlk( x0, y0, x1, y1, SCREEN_SHADE_LVL ) ;
        BoxStaticAdd( x0,y0, x1,y1 ) ;
        BoxUpdate() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    DrawSingleString( S32 x, S32 y, char *string )
{
	S32     x0,y0,x1,y1 ;
	S32     sf ;

	sf = SizeFont( string ) ;

	x0 = x - sf/2 ;
	x1 = x + sf/2 ;

	y0 = y - HAUTEUR_STANDARD/2 ;
	y1 = y + HAUTEUR_STANDARD/2 ;

	// text
	ColorFont( BLACK ) ;
	Font( x0 + 4, y-18 + 4, string ) ;
	ColorFont( CoulTextMenu ) ;
	Font( x0, y-18, string ) ;

	// flip
	BoxStaticAdd( x0,y0, x1,y1 ) ;
	BoxUpdate() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawCadreNewGame( void )
{
	S32	num = 16 ;// Logo Americain
	S32	dx, dy	;
	S32	x, y	;
	U8	*ptr 	;

	switch( DistribVersion )
	{
		case UNKNOWN_VERSION:
		case EA_VERSION:
			num = 11 ;
			break ;
	}

	ptr = (U8*)HQR_Get( HQRPtrSprite, num ) ;

	GetDxDyGraph( 0, &dx, &dy, ptr ) ;

	DrawCadreSave( 240, 50 ) ;

	x = 320 - dx/2 ;
	y = 50+(SCREEN_SAVE_HEIGHT)/2 - dy/2 ;

	AffGraph( 0, x, y, ptr ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
#define NB_GAME_CHOICE  5
#define Y_START_CHOICE  205

S32     InputPlayerName( S32 choice, char *name )
{
        char    memoplayername[255] ;
        S32     flag = 1 ;
        S32     retval = FALSE ;
        S32     akey ;
        S32     memoasciimode = AsciiMode ;
        S32     x, y ;
        S32     len ;
        S32     timeraff, timernoaff ;

        AsciiMode = TRUE ;
        while( GetAscii() ) ;

	strcpy( memoplayername, name ) ;
        strcpy( PlayerName, name ) ;

#ifdef	_WIN32
	FindPlayerFile() ;

	// pour pouvoir effacer une sauvegarde sous Windows si le joueur
	// change le nom de la sauvegarde
	if( stricmp(PlayerName,AUTOSAVE_NAME)		// on n'efface pas l'autosave !
	AND stricmp(PlayerName,CURRENTSAVE_NAME)	// ni le currentsave !
	AND stricmp(PlayerName,"")
	AND stricmp(GamePathname,AUTOSAVE_FILENAME)		// on n'efface pas l'autosave !
	AND stricmp(GamePathname,CURRENTSAVE_FILENAME) )	// ni le currentsave !
	{
		strcpy( OldGamePathname, GamePathname ) ;
	}
#endif

	len = strlen( PlayerName ) ;

        y = Y_START_CHOICE+choice*60 ;

        ManageTime() ;
        timeraff = TimerRefHR+20*20 ;
        CursorActif = TRUE ;

        while( flag!=-1 )
        {
                ManageTime() ;
                if( TimerRefHR<timeraff )
                {
                        CursorX = x-4 ;
                        CursorY = y+2 ;

                        if( !len )      CursorX -= 16 ;

                        CursorActif = TRUE ;
                        timernoaff = TimerRefHR+10*20 ;
                }
                else
                {
                        if( TimerRefHR>=timernoaff )
                        {
                                timeraff = TimerRefHR+20*20 ;
                                CursorActif = TRUE ;
                                CursorX = x-4 ;
                                CursorY = y+2 ;

                                if( !len )      CursorX -= 16 ;
                        }
                        else    CursorActif = FALSE ;
                }

                DrawOneString( 320, y, PlayerName, 1 ) ;
                BoxUpdate() ;

                if( flag==1 )
                {
                        x = 320+SizeFont(PlayerName)/2 ;
                        flag = 0 ;
                }

                do
                {
                akey = GetAscii() ;

                if( !IsValidChar(akey) )        akey = 0 ;

                switch( akey )
                {
                        case A_ESC:
                                strcpy( PlayerName, memoplayername ) ;
                                retval = FALSE ;
                                flag = -1 ;
                                break ;

                        case A_RETURN:
                                if( strcmp(PlayerName,"")
                                AND ((FindPlayerFile()==FALSE)
                                OR (!strcmp(PlayerName,memoplayername))) )
                                {
                                        retval = TRUE ;
                                        flag = -1 ;
                                }
                                else
				{
					PlayErrorSample() ;
					strcpy( PlayerName, memoplayername ) ;
					len = strlen( PlayerName ) ;
                                        flag = 1 ;
                                }
                                break ;

                        case A_LEFT:
                        case A_BACKSPACE:
                                if( len>0 )
                                {
                                        PlayerName[--len] = '\0' ;
                                        flag = 1 ;
                                }
                                break ;

                        default:
                                akey &= 0xFF ;

                                if( x<550
                                AND akey>=32 AND akey<=122 )
                                {
                                        PlayerName[len++] = (char)akey ;
                                        PlayerName[len] = '\0' ;
                                        x = 320+SizeFont(PlayerName)/2 ;
                                }
                                break ;
                }// end_switch
                }
                while( akey ) ; // Tant que Buffer Clavier non vide
        }

        AsciiMode = memoasciimode ;

        WaitNoInput() ;

        CursorActif = FALSE ;

        return retval ;
}

/*──────────────────────────────────────────────────────────────────────────*/
#define TEMPO_SCREEN_SAVE       110
#define TEMPO_REPEAT_KEY        150

S32     ChoosePlayerName( S32 mess, S32 flagflip, S32 flagnew )
{
        S32     flag = 3 ;
        char    *listplayername ;
        char    **ptrlist ;
        S32     n, nb ;
        S32     start = 0 ;
        S32     select = 0 ;
        S32     ys = Y_START_CHOICE ;
        S32     retval = 0 ;
        char    string[256] ;
        U8      *ptr ;
	S32     timerzoom ;
        S32     timeranotherkey ;
        S32     timerdraw = 1 ;
        U32     draw = 1 ;
        U32     newcube = NewCube ;
        char    memoplayername[MAX_SIZE_PLAYER_NAME+1] ;

#ifdef	_WIN32
	strcpy( OldGamePathname, "" ) ;
#endif

	strcpy( memoplayername, PlayerName ) ;

        CopyScreen( Screen, Log ) ;

        if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;

#ifdef  DEBUG_TOOLS
        if( mess&0x80000000 )   // chargement d'un bug
        {
                CoulText( LBAWHITE, 0 ) ;
                GraphPrintf( FALSE, 0, 460, "Chargement d'un bug..." ) ;
        }

        if( mess&0x40000000 )   // sauvegarde d'un bug
        {
                CoulText( LBAWHITE, 0 ) ;
                GraphPrintf( FALSE, 0, 460, "Sauvegarde d'un bug..." ) ;
        }

        mess &= 0x3FFFFFFF ;
#endif

        BoxStaticFullflip() ;
        if( FlagShadeMenu )     PaletteSync( PtrPalNormal ) ;

//      listplayername = (char *)Malloc( MAX_PLAYER * (MAX_SIZE_PLAYER_NAME+1) ) ;
//      ptrlist = (char**)Malloc( MAX_PLAYER * sizeof(char*) ) ;

        listplayername = (char *)BufSpeak ;
        ptrlist = (char**)(BufSpeak + MAX_PLAYER*(MAX_SIZE_PLAYER_NAME+1)) ;

/*      if( !listplayername OR !ptrlist )
        {
                TheEnd( NOT_ENOUGH_MEM, "Choose Player Name" ) ;
	}
*/

        nb = PlayerGameList( ptrlist, listplayername, flagnew ) ;

        strcpy( PlayerName, memoplayername ) ;

        if( !nb AND !flagnew )  return FALSE ;

	if( flagnew AND nb<MAX_PLAYER-1 )
        {
		ptrlist[nb] = NewGameTxt ;
                nb++ ;
        }
        else    flagnew = 0 ;

        if( strcmp(PlayerName,"") )
        {
                for( select=0; select<nb-1; select++ )
                {
                        if( !stricmp(PlayerName,ptrlist[select]) )
                                break ;
                }

                while( select >= (start + NB_GAME_CHOICE) )
                {
                        if( start < (nb-1) )
                        {
                                start++ ;
                        }
                }
        }

        if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
        else                    timerzoom = 0 ;

        InitDial( 0 ) ;

        InitWaitNoKey() ;

        while( MyKey != K_ESC AND !(Input&I_MENUS) )
        {
                ManageTime() ;

                if( flag == 3 )
		{
                        DrawSingleString( 320, 25, (char*)GetMultiText( mess,string ) ) ;
                        flag = 1 ;
                }

                if( flag == 1 )
                {
                        for( n=0; n<NB_GAME_CHOICE; n++ )
                        {
				if( n+start >= nb )     break ;

                                draw = ((n+start) == select) ;

                                if( (n+start) == select )
                                {
                                        ys = Y_START_CHOICE+60*n ;
                                }

                                if( start>0 AND n==0 )  draw |= ARROW_SG ;
                                else if( (start+NB_GAME_CHOICE<nb) AND (n==NB_GAME_CHOICE-1) )
                                                draw |= ARROW_ID ;

                                DrawOneString( 320, Y_START_CHOICE+60*n, ptrlist[n+start], draw ) ;
                        }

                        draw = 1 ;

                        if( start>0 AND select==start ) draw |= ARROW_SG ;
                        else if( (start+NB_GAME_CHOICE<nb) AND (select==start+NB_GAME_CHOICE-1) )
                                        draw |= ARROW_ID ;

/*                      while( Key )
                        {
                                DrawOneString( 320, ys, ptrlist[select], draw ) ;
                        }
*/
                        timeranotherkey = TimerRefHR + TEMPO_REPEAT_KEY ;
                        Input = MyKey = 0 ;
                        flag = 0 ;
                }

                if( timerdraw /*AND TimerRefHR>=timerdraw*/ )
                {
                        strcpy( PlayerName, ptrlist[select] ) ;

			if( !flagnew OR select<nb-1 )
                        {
                                FindPlayerFile() ;
                                ptr = LoadGameScreen( ) ;
                                if( ptr )       DrawScreenSave( ptr, 240, 50 ) ;
                        }
                        else    // Nouvelle partie
                        {
//                                DrawCadreSave( 240, 50 ) ;
//				DrawSingleString( 320, 105, "New" ) ;

				DrawCadreNewGame() ;
                                CopyBlock( 240+80-40, 50+60-15, 240+80+40, 50+60+15, Log, 0, 0, Log ) ;
                        }

                        timerdraw = 0 ;
                }

                if( TimerRefHR>timeranotherkey ) MyGetInput() ;

                DrawOneString( 320, ys, ptrlist[select], draw ) ;
                BoxUpdate() ;

		if( Input & I_DOWN OR MyKey==K_GRAY_DOWN )
                {
                        if( select < (nb-1) )
                        {
                                select++ ;
                                if( select >= (start + NB_GAME_CHOICE) )
                                {
                                        if( start < (nb-1) )
                                        {
                                                start++ ;
                                        }
                                }
                                else
                                {
                                        DrawOneString( 320, ys, ptrlist[select-1], (draw&0xFFFFFFFE) ) ;
                                }
                                flag = 1 ;

                                if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
                                else                    timerzoom = 0 ;
                                timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                        }
                }

		if( Input & I_UP OR MyKey==K_GRAY_UP )
                {
                        if( select > 0 )
                        {
                                select-- ;

                                if( select < start )
                                {
                                        start-- ;
                                }
                                else
                                {
                                        DrawOneString( 320, ys, ptrlist[select+1], (draw&0xFFFFFFFE) ) ;
                                }
                                flag = 1 ;
                                if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
                                else                    timerzoom = 0 ;
                                timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                        }
                }

//              if( !Input )    // on s'assure qu'aucune des touches testées
                                // ci-dessous ne fait partie des Inputs
                {
                        if( MyKey==K_GRAY_PAGE_DOWN )
                        {
                                if( select < (nb-1) )
                                {
                                        S32     oldselect = select ;

                                        select+=NB_GAME_CHOICE ;
                                        if( select>=nb )        select = nb-1 ;

                                        if( select >= (start + NB_GAME_CHOICE) )
                                        {
                                                start += NB_GAME_CHOICE ;
                                                if( start >= (nb-(NB_GAME_CHOICE-1)) )  start = nb-NB_GAME_CHOICE ;
                                        }
                                        else
                                        {
                                                DrawOneString( 320, ys, ptrlist[oldselect], (draw&0xFFFFFFFE) ) ;
                                        }
                                        flag = 1 ;

                                        if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
					else                    timerzoom = 0 ;
                                        timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                                }
                        }

                        if( MyKey==K_GRAY_PAGE_UP )
                        {
                                if( select > 0 )
                                {
                                        S32     oldselect = select ;

                                        select-=NB_GAME_CHOICE ;
                                        if( select<0 )  select=0 ;

                                        if( select < start )
                                        {
                                                start=select ;
                                        }
                                        else
                                        {
                                                DrawOneString( 320, ys, ptrlist[oldselect], (draw&0xFFFFFFFE) ) ;
                                        }
                                        flag = 1 ;
                                        if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
                                        else                    timerzoom = 0 ;
                                        timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                                }
                        }

                        if( MyKey==K_GRAY_HOME )
                        {
                                if( select > 0 )
                                {
                                        S32     oldselect = select ;

                                        select = 0 ;

                                        if( start==0 )
                                        {
                                                DrawOneString( 320, ys, ptrlist[oldselect], (draw&0xFFFFFFFE) ) ;
                                        }
                                        else    start = 0 ;

                                        flag = 1 ;
                                        if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
					else                    timerzoom = 0 ;
                                        timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                                }
                        }
                }

                if( MyKey==K_GRAY_END )
                {
                        if( select < (nb-1) )
                        {
                                S32     oldselect = select ;

                                select = nb-1 ;

                                if( start==(nb-NB_GAME_CHOICE) )
                                {
                                        DrawOneString( 320, ys, ptrlist[oldselect], (draw&0xFFFFFFFE) ) ;
                                }
                                else
                                {
                                        start = nb-NB_GAME_CHOICE ;
                                        if( start<0 )   start = 0 ;
                                }

                                flag = 1 ;
                                if( flagnew AND select==nb-1 )  timerzoom = TimerRefHR ;
                                else                    timerzoom = 0 ;
                                timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                        }
                }

                // Effacement d'une sauvegarde
                if( MyKey==K_D
                OR  MyKey==K_BACKSPACE
                OR  MyKey==K_GRAY_SUPPR )
                {
                        CopyScreen( Screen, Log ) ;
                        if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                        BoxStaticFullflip() ;

                        if( DeleteSavedGame() )
                        {
                                if( nb>1 AND select<nb-1 )
                                {
                                        memcpy( &ptrlist[select], &ptrlist[select+1], ((nb-1)-select)*sizeof(char*) ) ;
				}

                                // recharge list player
                                nb-- ;

                                if( select>=nb )
                                {
                                        select = nb-1 ;
                                        if( select<0 )          select = 0 ;

                                        if( select<start )
                                        {
                                                start = select ;
                                                if( start<0 )   start = 0 ;
                                        }
                                }

                                if( !nb )       break ; // plus de sauvegarde
                        }

                        CopyScreen( Screen, Log ) ;
                        if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                        BoxStaticFullflip() ;

                        flag = 1 ;      // redraw menu
                        timerdraw = TimerRefHR + TEMPO_SCREEN_SAVE ;
                }

                if( Input & I_FIRE OR MyKey==K_ENTER OR MyKey==K_NUMPAD_ENTER )
                {
                        if( timerdraw )
                        {
                                strcpy( PlayerName, ptrlist[select] ) ;

                                if( !flagnew OR select<nb-1 )
                                {
                                        FindPlayerFile() ;
                                        ptr = LoadGameScreen( ) ;
                                        if( ptr )       DrawScreenSave( ptr, 240, 50 ) ;
                                }
                                else    // Nouvelle partie
				{
//					DrawCadreSave( 240, 50 ) ;
//					DrawSingleString( 320, 105, "New" ) ;
					DrawCadreNewGame() ;
				}

                                timerdraw = 0 ;
                        }

                        if( flagnew )   // Nouvelle partie
                        {
                                char    memoname[255] ;

                                if( select==nb-1 )      strcpy( PlayerName, "" ) ;
                                else
                                {
                                        FindPlayerFile() ;
                                        strcpy( memoname, GamePathname ) ;
                                }

                                while( Input & I_FIRE OR MyKey==K_ENTER )       MyGetInput() ;

                                if( InputPlayerName( select-start, PlayerName ) )
                                {
                                        if( select==nb-1 )
                                        {
                                                do
                                                {
                                                        strcpy( GamePathname, PathSave ) ;

#ifdef  _WIN32  // Gestion des noms longs
                                                        strncat( GamePathname, PlayerName, _MAX_PATH-(strlen(GamePathname)+1) ) ;
#else
                                                        strcat( GamePathname, "S" ) ;
                                                        strcat( GamePathname, Itoa( MyRnd(10000) ) ) ;
#endif
							AddExt( GamePathname, ".LBA" ) ;
                                                }
                                                while( FileSize( GamePathname ) != 0 ) ;
                                        }
                                        else    // renommer une sauvegarde
					{
#ifdef  _WIN32  // Gestion des noms longs (renomme la sauvegarde)
						strcpy( GamePathname, PathSave ) ;
						strncat( GamePathname, PlayerName, _MAX_PATH-(strlen(GamePathname)+1) ) ;
						AddExt( GamePathname, ".LBA" ) ;
#else
						strcpy( GamePathname, memoname ) ;
#endif
                                        }

                                        retval = 2 ;
                                        break ;
                                }
                                else
                                {
                                        while( Input & I_FIRE OR MyKey==K_ENTER )       MyGetInput() ;
					if( select==nb-1 )      strcpy( PlayerName, NewGameTxt ) ;
                                        timerdraw = 1 ;
                                        flag = 3 ;      // Rafraichissement ecran
                                }
                        }
                        else
                        {
                                retval = 1 ;
                                break ;
                        }
                }
        }

        if( Key==K_ESC OR Input&I_MENUS )
        {
                NewCube = newcube ;
                strcpy( PlayerName, memoplayername ) ;
        }
        else if( retval==1 )
        {
                strcpy( PlayerName, ptrlist[select] ) ;
                FindPlayerFile() ;

                for( n=0; n<NB_GAME_CHOICE; n++ )
                {
                        if( (n+start) != select )
                        {
                                ClearOneString( 320, Y_START_CHOICE+60*n ) ;
                        }
                }
        }

//      Free( ptrlist ) ;
//      Free( listplayername ) ;

        if( flagflip )
        {
                CopyScreen( Screen, Log ) ;
                if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                BoxStaticFullflip() ;
        }

        SelectPlayer = select-start ;

        return retval ;
}

////////************* TEMPO, devrait etre en LIB ?

S32     HQR_GiveOffset( T_HQR_HEADER *header, S32 index )
{
        S32     handle ;
        S32     nbbloc ;

        handle = OpenRead( header->Name )       ;
        if( !handle )   return 0                ;

        index *= 4                              ;
        Read(handle, &nbbloc, 4)                ;

        if( index >= nbbloc)
        {
                Close(handle)                   ;
                return 0                        ;
        }

        Seek(handle, index, SEEK_START)         ;
        Read(handle, &index, 4)                 ;

        Close( handle ) ;
        return index    ;
}

////////*************

/*══════════════════════════════════════════════════════════════════════════*
                            █▄ ▄█ █▀▀▀▀ ██▄ █ █   █
                            ██▀ █ ██▀▀  ██▀██ ██  █
                            ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

S32     LastSampleVolume;
S32     SampleMenu      ;
S32     LastVoiceVolume ;
S32     VoiceMenu      	;
U32     PanMenu         ;

S32     WayChoice       ;
S32     DeltaChoice     ;
MOVE    MoveChoice      ;

/*──────────────────────────────────────────────────────────────────────────*/
void    InitShakeChoice()
{
        WayChoice = -1  ;//     To the Left First
        DeltaChoice = 0 ;

        InitMove( &MoveChoice, 48 ) ;
}
/*──────────────────────────────────────────────────────────────────────────*/

void    DrawOneChoice( S32 x, S32 y, S32 type, S32 num, S32 select )
{
        S32     x2, x0,y0,x1,y1 ;
        S32     dx              ;
        char    string[256] ;
        S32     value ;
        S32     drawslider = FALSE ;

        x0 = x - LargeurMenu/2 ;
        x1 = x + LargeurMenu/2 ;

        y0 = y - HAUTEUR_STANDARD/2 ;
        y1 = y + HAUTEUR_STANDARD/2 ;

        // fond

        BackupAngles( x0, y0, x1, y1 ) ;

        if( select )
        {
                switch( type )
                {
                        case 2: // sample volume
                                value = SampleVolume ;
                                drawslider = TRUE ;
                                break ;

                        case 3: // voice volume
                                value = VoiceVolume ;
                                drawslider = TRUE ;
                                break ;

                        case 4: // Jingle volume
                                value = JingleVolume ;
                                drawslider = TRUE ;
                                break ;

                        case 5: // CD volume
#ifdef  CDROM
                                value = CDVolume ;
#else
                                value = 127 ;
#endif
                                drawslider = TRUE ;
                                break ;

                        case 6: // master volume
                                value = MasterVolume ;
                                drawslider = TRUE ;
                                break ;

                        case 7: // Niveau de détails
                                x2 = RegleTrois( x0, x1, MAX_DETAIL_LEVEL, DetailLevel ) ;
                                DrawFire( y0, FIRE_SLIDER_MENU ) ;
                                Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;
                                break ;

                        default:
                                DrawFire( y0, FIRE_SELECT_MENU ) ;
                }

                if( drawslider )
                {
                        x2 = RegleTrois( x0, x1, 127, value ) ;
                        DrawFire( y0, FIRE_SLIDER_MENU ) ;
                        Box( x2, y0, x1, y1, COUL_SELECT_MENU ) ;

                        if( SamplesEnable )
                        {
                                switch( type )
                                {
                                        case 2: // sample volume
                                        case 6: // Master volume
                                                // bruitons un peu la situation (violement)

                                                if( (TimerRefHR-TimerSample) > 35*20
                                                AND !IsSamplePlaying(SampleMenu) )
                                                {
                                                        TimerSample = TimerRefHR ;

                                                        do
                                                        {
                                                                SampleMenu =  0+MyRnd(28) ;
                                                        }
                                                        while( !HQR_GiveOffset(HQR_Samples,SampleMenu) ) ;

                                                        PanMenu    = 20+MyRnd(88) ;

                                                        HQ_MixSample( SampleMenu, 0x1000, 600, 1, PanMenu, 127 ) ;
                                                }
                                                else    if( SampleVolume!=LastSampleVolume )
                                                {
                                                        HQ_ChangePanSample( SampleMenu, 127, PanMenu ) ;
                                                        LastSampleVolume = SampleVolume ;
                                                }
                                                break ;

#ifndef DEMO
					case 3: // voice volume
						// bruitons un peu la situation (violement)

						if( (TimerRefHR-TimerSample) > 5000
						AND !IsSamplePlaying(VoiceMenu) )
						{
							TimerSample = TimerRefHR ;

							VoiceMenu = SAMPLE_VOICE_MENU+LanguageCD ;

							PanMenu    = 20+MyRnd(88) ;

							PlaySample( (U8*)GivePtrSample(VoiceMenu), VoiceMenu, 0x1000, 1, VoiceVolume, PanMenu ) ;
						}
						else    if( VoiceVolume!=LastVoiceVolume )
						{
							ChangeVolumePanSample( VoiceMenu, VoiceVolume, PanMenu ) ;
							LastVoiceVolume = VoiceVolume ;
						}
						break ;
#endif

					case 4: // Music Volume
                                                if( (TimerRefHR-TimerSample) > 1000 )
                                                {
                                                        TimerSample = TimerRefHR ;
#ifdef  CDROM
                                                        if( IsCDPlaying() )     StopMusic() ;
#endif
                                                        PlayMusic( JINGLE_TRACK_MENU, TRUE ) ;
                                                }
                                                break ;

                                        case 5: // CD Volume
                                                if( (TimerRefHR-TimerSample) > 1000 )
                                                {
                                                        TimerSample = TimerRefHR ;
#ifdef  CDROM
                                                        if( IsStreamPlaying() ) StopMusic() ;
                                                        PlayMusic( CD_TRACK_MENU, TRUE ) ;
#endif
                                                }
                                                break ;
                                }
                        }
                }

        }
        else
        {
                CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;
                ShadeBoxBlk( x0, y0, x1, y1, MENU_SHADE_LVL ) ;
        }

        // cadre
        DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
        RestoreAngles( x0, y0, x1, y1 ) ;

        // text

        ColorFont( CoulTextMenu ) ;
        GetMultiText( num, string ) ;

//----------------------------------------------------------------------------
	MemoClip()                      ;
	SetClip( x0+1, y0, x1-1, y1 )   ;

	dx = SizeFont( string ) ;

	if ( dx > LargeurMenu )//       Biggest than window
	{
		if ( !select )  Font( x0+8, y-18, string ) ;// To the Left
		else
		{//     Shake Baby, shake...
			if ( WayChoice == -1 )
				DeltaChoice -= GetDeltaMove( &MoveChoice ) ;
			else    DeltaChoice += GetDeltaMove( &MoveChoice ) ;

			if ( DeltaChoice < LargeurMenu-dx-8 )
			{
				DeltaChoice = LargeurMenu-dx-8  ;
				WayChoice = 1                   ;// Right
			}
			if ( DeltaChoice > 8 )
			{
				DeltaChoice = 8         ;
				WayChoice = -1                  ;// Left
			}
			Font( x0 + DeltaChoice, y-18, string )  ;
		}
	}
	else    Font( x - SizeFont( string )/2, y-18, string ) ;

        RestoreClip()   ;
//----------------------------------------------------------------------------

        // flip

        BoxStaticAdd( x0,y0, x1,y1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    DrawGameMenu( U16 *ptrmenu, S32 justone )
{
        S32     n, y ;
        S32     nb ;
        S32     selected, type, num ;
#ifdef  DEMO
        U8      mainmenu = FALSE ;

        if( ptrmenu==GameMainMenu )     mainmenu = TRUE ;
#endif

        selected = *ptrmenu++ ;

        nb = *ptrmenu++ ;

        y = *ptrmenu++ ;

        ptrmenu++ ;     // dial num

        if( y == 0 )    // depuis le haut
        {
                y = HAUTEUR_STANDARD/2 + 10 ;
        }
        else            // centre sur y
        {
                y -= ( HAUTEUR_STANDARD*nb + (nb-1)*MENU_SPACE ) / 2 ;
        }

        for( n=0; n<nb; n++ )
        {
                type = *ptrmenu++ ;
                num = *ptrmenu++ ;

#ifdef  DEMO
                if( mainmenu AND num==72 OR num==73 )// save or load
                {
                        CoulTextMenu = COUL_TEXT_MENU_DISABLE ;
                }
                else
                {
                        CoulTextMenu = COUL_TEXT_MENU ;
                }
#endif

                if( justone )
                {
                        if( n == selected )
                        {
                                DrawOneChoice( 320, y, type, num, TRUE ) ;
                        }
                }
                else
                {
                        DrawOneChoice( 320, y, type, num, (n==selected) ) ;
                }

                y += HAUTEUR_STANDARD + MENU_SPACE ;
        }

#ifdef  DEMO
        if( mainmenu )  CoulTextMenu = COUL_TEXT_MENU ;
#endif

        BoxUpdate() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

#define VOLUME_TIMER_KEY        5

S32     DoGameMenu( U16 *ptrmenu )
{
        S32     flag = 1 ;
        U32     chrono ;
        S32     timerkey ;
        S16     nb, selected ;

        selected = ptrmenu[0] ;
        nb = ptrmenu[1] ;

        ManageTime() ;
        chrono = TimerRefHR ;

        InitWaitNoKey()   ;
        InitWaitNoInput( I_JOY|I_FIRE|I_MENUS|I_ACTION ) ;

        MenuIncrustText = NULL ;
        MenuIncrustSprite = -1 ;

        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;

	InitShakeChoice()       ;//     NEW

#ifdef  _WIN32
        AsciiMode = TRUE        ;
#endif

        while( TRUE )
        {
                ManageTime() ;
                MyGetInput() ;

#ifdef  DEBUG_TOOLS
		CheckSaveLogPcx( PtrPalNormal ) ;
#endif

                // rustine Démo non jouable ...
                if( ptrmenu == GameMainMenu )
                {
                        if( (TimerRefHR - chrono > 50 * 20 * 60)
                        OR (CheckKey(K_D) AND (CheckKey(K_SHIFT) OR CheckKey(K_SHIFT_LEFT) OR CheckKey(K_SHIFT_RIGHT))) )
                        {
#ifdef  DEMO
//                                if( SlideShow() )
//					DemoBumper() ;
				SlideShow() ;

				Load_HQR( SCREEN_HQR_NAME, Log, PCR_MENU ) ;
                                CopyScreen( Log, Screen ) ;
                                BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;

                                FlagShowEndDemo = TRUE ;
#else
                                DemoSlide = TRUE ;

                                InitGame( 1 ) ;
                                NewCube = 193 ;

                                FlagFade = TRUE ;

                                if( MainLoop() )
                                        if( SlideShow() )
                                                Credits( FALSE ) ;

                                UnsetClipWindow() ;
                                CopyScreen( Log, Screen ) ;
                                BoxReset() ;
#endif
                                PtrPal = PtrPalNormal ;
                                if( !FlagBlackPal )     FadeToBlack( PtrPal ) ;

                                FadeMenu = TRUE ;
#ifdef  _WIN32
                                AsciiMode = FALSE ;
#endif
                                return 9999 ;
                        }
                }

                if( Input & I_DOWN OR MyKey==K_GRAY_DOWN )
                {
#ifdef  DEMO
                        if( ptrmenu==GameMainMenu )
                        {
                                do
                                {
                                        selected++ ;
                                }
                                while( selected<nb
                                AND (ptrmenu[4+1+selected*2]==72
                                  OR ptrmenu[4+1+selected*2]==73) ) ;
                        }
                        else
#endif
                        {
                                selected++ ;
                        }

                        if( selected >= nb )    selected = 0 ;
                        flag = 1 ;
                }

                if( Input & I_UP OR MyKey==K_GRAY_UP )
                {
#ifdef  DEMO
                        if( ptrmenu==GameMainMenu )
                        {
                                do
                                {
                                        selected-- ;
                                }
                                while( selected>=0
                                AND (ptrmenu[4+1+selected*2]==72
                                  OR ptrmenu[4+1+selected*2]==73) ) ;
                        }
                        else
#endif
                        {
                                selected-- ;
                        }

                        if( selected < 0 )      selected = (U16)(nb-1) ;
                        flag = 1 ;
                }

                switch( ptrmenu[4 + 0 + selected*2] ) // type
                {
                        case 0: // text normal
                                break ;

                        case 2: // volume sample

                                if( (Input & I_LEFT OR MyKey==K_GRAY_LEFT)
                                AND TimerRefHR>timerkey AND SampleVolume>0 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        SampleVolume -= 1 ;
                                }
                                if( (Input & I_RIGHT OR MyKey==K_GRAY_RIGHT)
                                AND TimerRefHR>timerkey AND SampleVolume<127 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        SampleVolume += 1 ;
                                }
                                break ;

                        case 3: // volume voix (jouer 1 voix ?)

                                if( (Input & I_LEFT OR MyKey==K_GRAY_LEFT)
                                AND TimerRefHR>timerkey AND VoiceVolume>0 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        VoiceVolume -= 1 ;
                                }
                                if( (Input & I_RIGHT OR MyKey==K_GRAY_RIGHT)
                                AND TimerRefHR>timerkey AND VoiceVolume<127 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        VoiceVolume += 1 ;
                                }
                                break ;

                        case 4: // volume musiques

                                if( (Input & I_LEFT OR MyKey==K_GRAY_LEFT)
                                AND TimerRefHR>timerkey AND JingleVolume>0 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        JingleVolume -= 1 ;
                                        SetVolumeJingle( JingleVolume ) ;
                                }
                                if( (Input & I_RIGHT OR MyKey==K_GRAY_RIGHT)
                                AND TimerRefHR>timerkey AND JingleVolume<127 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        JingleVolume += 1 ;
                                        SetVolumeJingle( JingleVolume ) ;
                                }
                                break ;

                        case 5: // volume cd
#ifdef  CDROM
                                if( (Input & I_LEFT OR MyKey==K_GRAY_LEFT)
                                AND TimerRefHR>timerkey AND CDVolume>0 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        CDVolume -= 1 ;
                                        SetVolumeCD( CDVolume ) ;
                                }
                                if( (Input & I_RIGHT OR MyKey==K_GRAY_RIGHT)
                                AND TimerRefHR>timerkey AND CDVolume<127 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        CDVolume += 1 ;
                                        SetVolumeCD( CDVolume ) ;
                                }
#endif
                                break ;

                        case 6: // volume master

                                if( (Input & I_LEFT OR MyKey==K_GRAY_LEFT)
                                AND TimerRefHR>timerkey AND MasterVolume>0 )
                                {
                                        timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
                                        MasterVolume -= 1 ;
                                        SetMasterVolumeSample( MasterVolume ) ;
#ifdef  CDROM
                                        SetVolumeCD( CDVolume ) ;
#endif
				}
				if( (Input & I_RIGHT OR MyKey==K_GRAY_RIGHT)
				AND TimerRefHR>timerkey AND MasterVolume<127 )
				{
					timerkey = TimerRefHR + VOLUME_TIMER_KEY ;
					MasterVolume += 1 ;
					SetMasterVolumeSample( MasterVolume ) ;
#ifdef  CDROM
					SetVolumeCD( CDVolume ) ;
#endif
                                }
                                break ;

                        case 7: // Niveau détails

                                if( (Input & I_LEFT OR MyKey==K_GRAY_LEFT)
                                AND (DetailLevel>0 ) )
                                {
                                        DetailLevel-- ;
                                        InitWaitNoInput( I_LEFT ) ;
                                        InitWaitNoKey() ;
                                }

                                if( (Input & I_RIGHT OR MyKey==K_GRAY_RIGHT)
                                AND (DetailLevel<MAX_DETAIL_LEVEL) )
                                {
                                        DetailLevel++ ;
                                        InitWaitNoInput( I_RIGHT ) ;
                                        InitWaitNoKey() ;
                                }
                                break ;
                }

                if( flag == 1 )
                {
                        InitShakeChoice()       ;//     NEW
                        TimerSample = 0         ;
                        ptrmenu[0] = selected   ;
                        DrawGameMenu( ptrmenu, FALSE ) ;

                        if( ptrmenu == GameMainMenu )
                        {
                                U8      memonumversion = NumVersion ;
                                char    memoplayername[256] ;
                                char    memogamepathname[256] ;
                                U8 *ptr ;

                                strcpy( memoplayername, PlayerName ) ;
                                strcpy( memogamepathname, GamePathname ) ;

                                strcpy( GamePathname, CURRENTSAVE_FILENAME ) ;

                                ptr = LoadGameScreen( ) ;
                                if( ptr )       DrawScreenSave( ptr, 240, 10 ) ;

                                strcpy( GamePathname, memogamepathname ) ;
                                strcpy( PlayerName, memoplayername ) ;
                                NumVersion = memonumversion ;
                                NewCube = -1 ;
                        }

                        if( FadeMenu )
                        {
                                FadeToPal( PtrPalNormal ) ;
                                FadeMenu = FALSE ;
                        }

                        do
                        {
                                DrawGameMenu( ptrmenu, TRUE ) ;
                                MyGetInput() ;
                        }
                        while( MyKey OR Input ) ;

                        flag = 0 ;
                }
                else
                {
                        DrawGameMenu( ptrmenu, TRUE ) ;
                        flag = 0 ;
                }

                if( MenuIncrustText )
                {
                        if( TimerRefHR>MenuIncrustTimer )
                        {
                                CopyBlock( 0, 0, 150, 19, Screen, 0, 0, Log ) ;
                                BoxStaticAdd( 0, 0, 150, 19 ) ;
                                if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 150, 19, SCREEN_SHADE_LVL ) ;
                                MenuIncrustText = NULL ;
                                MenuIncrustSprite = -1 ;
                        }
                        else
                        {
                                S32     x  ;

                                if( (TimerRefHR&0xFF) >= 85 )   // aff 2/3 eff 1/3
                                {
                                if( MenuIncrustSprite!=-1 )
                                {
                                        if( MenuIncrustSprite>=100 )
                                        {
                                                SpriteX = 10 + PtrZvExtra[ MenuIncrustSprite*8 + 0 ] ;
                                                SpriteY = 10 + PtrZvExtra[ MenuIncrustSprite*8 + 1 ] ;
                                        }
                                        else
                                        {
                                                SpriteX = 10 + PtrZvExtraRaw[ MenuIncrustSprite*8 + 0 ] ;
                                                SpriteY = 10 + PtrZvExtraRaw[ MenuIncrustSprite*8 + 1 ] ;
                                        }

                                        ScaleFactorSprite = DEF_SCALE_FACTOR ;
                                        PtrAffGraph( SpriteX, SpriteY, MenuIncrustSprite ) ;
                                        BoxStaticAdd( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
                                        x = ScreenXMax+5 ;
                                }
                                else    x = 5 ;

                                CoulText( LBAWHITE, -1 ) ;
                                GraphPrintf( FALSE, x, 0, MenuIncrustText ) ;
                                BoxStaticAdd(TextXMin, TextYMin, TextXMax, TextYMax) ;
                                }
                                else
                                {
                                CopyBlock( 0, 0, 150, 19, Screen, 0, 0, Log ) ;
                                BoxStaticAdd( 0, 0, 150, 19 ) ;
                                if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 150, 19, SCREEN_SHADE_LVL ) ;
                                }
                        }
                }

                if( Input & I_RETURN
                OR  Input & I_ACTION_M
                OR  MyKey==K_ENTER
                OR  MyKey==K_NUMPAD_ENTER )
                {
                        WaitNoInput() ;
#ifdef  _WIN32
                        AsciiMode = FALSE ;
#endif
                        return ptrmenu[4 + 1 + selected*2] ; // num mess
                }

                if( MyKey == K_ESC
                OR  Input & I_MENUS )
                {
#ifdef  _WIN32
                        AsciiMode = FALSE ;
#endif
                        return 1000 ;
                }

#ifdef  DEBUG_TOOLS
                if( ptrmenu == GameMainMenu )
                {
                        // chargement d'un bug
                        if( MyKey == K_L )
                        {
#ifdef  _WIN32
                                AsciiMode = FALSE ;
#endif
                                return 2000 ;
                        }

                        // sauvegarde d'un bug
                        if( MyKey == K_G )
                        {
#ifdef  _WIN32
                                AsciiMode = FALSE ;
#endif
                                return 2001 ;
                        }
                }
#endif

                if( !DemoSlide AND MyKey )      GereCheatCode()         ;
        }
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

S32     GereVolumeMenu( void )
{
        S32 select ;
        S32 flag = 0 ;

        CopyScreen( Screen, Log ) ;
        BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;

        if( FlagShadeMenu )
        {
                ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
        }

#ifdef  CDROM
        if( IsStreamPlaying() ) StopMusic() ;
        PlayMusic( CD_TRACK_MENU, TRUE ) ;
#endif

#ifndef	DEMO
	if( FlagSpeak )
	{
		VolumeMenu = VolumeMenuVoice ;// integre volume des voix
	}
	else
	{
		VolumeMenu = VolumeMenuNoVoice ;// pas de volume des voix
	}
#endif

	while( !flag )
	{
		select = DoGameMenu( VolumeMenu ) ;

		switch( select )        // num mess
		{
			case 26:   // menu precedent
			case 1000: // <ESC>
				flag = 1 ;
				break ;
		}
	}

        CopyScreen( Screen, Log ) ;
        BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;

        WaitNoInput() ;

        return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

S32     OptionsMenu( S32 flagflip )
{
        S32 select ;
        S32 flag = 0 ;

	BackupScreen( TRUE ) ;

/*        CopyScreen( Log, Screen ) ;

	BoxReset() ;
*/
        if( FlagShadeMenu )
        {
                ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                BoxStaticFullflip() ;
                PaletteSync( PtrPalNormal ) ;
        }

        HQ_StopSample() ;
        RestartRainSample = TRUE ;

#ifdef  CDROM
        if( IsStreamPlaying() ) StopMusic() ;
        PlayMusic( CD_TRACK_MENU, TRUE ) ;
#endif

#ifdef  DEMO
        GameOptionMenu[1] = 6 ;
#else
	// interdire option FlagDisplayText si pas de voix

	if( FlagSpeak )	GameOptionMenu[1] = 8 ;
	else 		GameOptionMenu[1] = 7 ;
#endif

        while( !flag )
        {
                GameOptionMenu[4+0+MENU_CAMERA*2+1] = (S16)(46+AllCameras) ;
                GameOptionMenu[4+0+MENU_STEREO*2+1] = (S16)(12+ReverseStereo) ;
		GameOptionMenu[4+0+MENU_DISPLAY_TEXT*2+1] = (S16)(16+FlagDisplayText) ;
		GameOptionMenu[4+0+MENU_FULL_SCREEN*2+1] = (S16)(27+VideoFullScreen) ;

                select = DoGameMenu( GameOptionMenu ) ;

                switch( select )        // num mess
                {
                        case 15:   // retour au jeu
                        case 26:   // menu precedent
                        case 1000: // <ESC>
                                flag = 1 ;
                                break ;

                        case 47:   // Caméra Scénarique ON
                        case 46:   // Caméra Scénarique OFF
                                AllCameras ^= 1 ;
                                break ;

                        case 11:   // Reglage des volumes
                                GereVolumeMenu() ;
                                break ;

                        case 12:   // Stéréo Inverse OFF
                        case 13:   // Stéréo Inverse ON
                                ReverseStereo ^= 1 ;
                                InverseStereoSample( ReverseStereo ) ;
                                break ;

                        case 14:   // Configuration Clavier
                                MenuConfig() ;
                                break ;

                        case 16:   // Display Text Off
                        case 17:   // Display Text On
                                FlagDisplayText ^= 1 ;
				break ;

			case 27:   // Vidéos réduites
			case 28:   // Vidéos Plein Ecran
				VideoFullScreen ^= 1 ;
                                break ;
		}
        }

        HQ_StopSample() ;
        SetDetailLevel() ;// prise en compte des différentes options

        if( flagflip )
        {
                CopyScreen( Screen, Log ) ;
                BoxStaticFullflip() ;
        }
	else
	{
		// on vient du jeu
		RestoreClipWindow() ;
		RestoreScreen() ;
        }

        WaitNoInput() ;

        return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
S32     DeleteSavedGame( void )
{
        U8      *ptr ;
        S32     ret = -1 ;
        S32     select ;

        DrawOneString( 320, 80, PlayerName, 2 ) ;
        ptr = LoadGameScreen( ) ;
        if( ptr )       DrawScreenSave( ptr, 240, 130 ) ;
        BoxUpdate() ;

        SavedConfirmMenu[0] = 0 ;

        while( ret==-1 )
        {
                select = DoGameMenu( SavedConfirmMenu ) ;

                switch( select )        // num mess
                {
                        case 24:        // annuler
                        case 1000:      // <ESC>
                                ret = 0 ;
                                break ;

                        case 48: // detruire
                                // retourne 0 si Pb -> pas effacé
                                //          1 si Ok
                                ret = Delete( GamePathname ) ;
                                break ;
                }
        }

        InitWaitNoKey() ;
        InitWaitNoInput( I_FIRE | I_ACTION | I_JOY | I_MENUS ) ;

        return ret ;
}

/*══════════════════════════════════════════════════════════════════════════*/

#define TIME_ZOOM_SAVE  (30*20)

/*──────────────────────────────────────────────────────────────────────────*/
void    ZoomSavedGame( S32 x, S32 y )
{
        S32     x0=x, y0=y, x1=x+SCREEN_SAVE_WIDTH-1, y1=y+SCREEN_SAVE_HEIGHT-1 ;
        S32     time, savetimer ;
        S32     flag = FALSE ;

        ManageTime() ;
        savetimer = TimerRefHR ;

        CopyScreen( ScreenAux, Log ) ;  // pour cleaner le Log

        while( !flag )
        {
                ManageTime() ;
                time = TimerRefHR-savetimer ;

                x0 = RegleTrois( x,       0, TIME_ZOOM_SAVE, time ) ;
                x1 = RegleTrois( x+SCREEN_SAVE_WIDTH-1, 639, TIME_ZOOM_SAVE, time ) ;
                y0 = RegleTrois( y,       0, TIME_ZOOM_SAVE, time ) ;
                y1 = RegleTrois( y+SCREEN_SAVE_HEIGHT-1, 479, TIME_ZOOM_SAVE, time ) ;

                if( x0<=0 OR y0<=0 OR x1>=639 OR y1>=479 )
                {
                        flag = TRUE ;
                }
                else
                {
                        ScaleBox( 0, 0, 639, 479, Screen, x0, y0, x1, y1, Log ) ;
                        BoxStaticAdd( x0,y0, x1,y1 ) ;
                        BoxUpdate() ;
                }
        }

        CopyScreen( Screen, Log ) ;
        PaletteSync( PtrPal ) ;
        BoxStaticFullflip() ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    DrawScreenSave( U8 *ptr, S32 x, S32 y )
{
        S32     x1, y1 ;

        x1 = x + SCREEN_SAVE_WIDTH - 1 ;
        y1 = y + SCREEN_SAVE_HEIGHT - 1 ;

        BackupAngles( x-1, y-1, x1+1, y1+1 ) ;
        Box( x, y, x1, y1, 0 ) ;
        RestoreBlock( Log, ptr, x, y, x1, y1 ) ;
        DrawCadre( x-1, y-1, x1+1, y1+1, ALL_ANGLES ) ;
        RestoreAngles( x-1, y-1, x1+1, y1+1 ) ;
        BoxStaticAdd( x-1, y-1, x1+1, y1+1 ) ;
        BoxUpdate() ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    DrawCadreSave( S32 x, S32 y )
{
        S32     x1, y1 ;

        x1 = x + SCREEN_SAVE_WIDTH - 1 ;
        y1 = y + SCREEN_SAVE_HEIGHT - 1 ;

        BackupAngles( x-1, y-1, x1+1, y1+1 ) ;
        Box( x, y, x1, y1, 0 ) ;
        DrawCadre( x-1, y-1, x1+1, y1+1, ALL_ANGLES ) ;
        RestoreAngles( x-1, y-1, x1+1, y1+1 ) ;
        BoxStaticAdd( x-1, y-1, x1+1, y1+1 ) ;
//        BoxUpdate() ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32     IsExistSavedGame( void )
{
        // a optimiser
	return PlayerGameList( (char **)BufSpeak, (char *)BufSpeak+100000L, FALSE ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	FirstGame = FALSE ;

S32	IsFirstGameLaunched( void )
{
	if( !FirstGame )
	{
		if( !IsExistSavedGame()
		AND !FileSize( CURRENTSAVE_FILENAME ) )
		{
			FirstGame = TRUE ;
		}
	}

	return FirstGame ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*
   GameMainMenu:

        0, 70,  // reprendre partie
        0, 71,  // nouvelle partie
        0, 72,  // charger partie
        0, 73,  // sauver partie
        0, 74,  // Options
        0, 75,  // Quitter
*/
void    BuildGameMainMenu( S32 firstloop )
{
        char    memoplayername[MAX_SIZE_PLAYER_NAME+1] ;
        char    memogamepathname[_MAX_PATH] ;
        U16     *ptrreal = RealGameMainMenu ;
        U16     *ptrmain = GameMainMenu ;
        U16     nb = 0 ;
        U16     ycenter = 275 ;

        strcpy( memoplayername, PlayerName ) ;
        strcpy( memogamepathname, GamePathname ) ;

        *ptrmain++ = *ptrreal++ ;       // selected
        *ptrmain++ = *ptrreal++ ;       // nb entries
        *ptrmain++ = *ptrreal++ ;       // y center
        *ptrmain++ = *ptrreal++ ;       // .dia num

        // reprendre partie

        if( FileSize( CURRENTSAVE_FILENAME ) )
        {
                *ptrmain++ = *ptrreal ;
                *ptrmain++ = *(ptrreal+1) ;
                nb++ ;
                ycenter = 335 ;
        }

        ptrreal += 2 ;  // nouvelle partie
        *ptrmain++ = *ptrreal++ ;
        *ptrmain++ = *ptrreal++ ;
        nb++ ;

//#ifndef       DEMO
        // charger partie

        if( IsExistSavedGame() )
        {
                *ptrmain++ = *ptrreal ;
                *ptrmain++ = *(ptrreal+1) ;
                nb++ ;
        }

        ptrreal += 2 ;  // sauver partie

        if( !firstloop
        AND SavingEnable )
        {
                *ptrmain++ = *ptrreal ;
                *ptrmain++ = *(ptrreal+1) ;
                nb++ ;
        }
/*#else
        ptrreal += 2 ;  // sauver partie
#endif
*/
        ptrreal += 2 ;  // Options

        *ptrmain++ = *ptrreal++ ;
        *ptrmain++ = *ptrreal++ ;
        nb++ ;

        // Quitter
        *ptrmain++ = *ptrreal++ ;
        *ptrmain++ = *ptrreal++ ;
        nb++ ;

        if( firstloop==2 ) GameMainMenu[0] = (S16)(nb-1) ;      // selected
        GameMainMenu[1] = nb ;                          // nb entries
        GameMainMenu[2] = ycenter ;                     // y center

        strcpy( PlayerName, memoplayername ) ;
        strcpy( GamePathname, memogamepathname ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

S32     GameInProgress = FALSE ;

S32     MainGameMenu( S32 load )
{
        S32     select ;
        S32     flag = 0 ;
        S32     firstloop = TRUE ;
        S32     savenewcube ;
        S32     gogame = FALSE ;
        S32     ret ;

        HQ_StopSample() ;
        StopMusic() ;   // Pour Stopper la music du Logo

        InitPlasmaMenu() ;
        FlagShadeMenu = FALSE ;

#ifdef  DEMO
        if( !FileSize( CURRENTSAVE_FILENAME )
#ifdef  DEBUG_TOOLS
        OR load
#endif
        )
        {
#ifndef DEBUG_TOOLS
                load = 0 ;
#endif
                FlagBlackPal = FALSE ;
                SetDemoSaveGame( load ) ;
                goto load_game  ;
        }
#else
	if( IsFirstGameLaunched() )
	{
		FlagPlayAcf = TRUE ;
		goto new_game ;
        }
#endif

#ifndef DEMO
        if( load )                      goto load_game ;
#endif

        Load_HQR( SCREEN_HQR_NAME, Screen, PCR_MENU ) ;
        CopyScreen( Screen, Log ) ;
        BoxStaticFullflip() ;
        FadeToPal( PtrPalNormal ) ;

	PtrPal = PtrPalNormal ;

        BackupScreen( FALSE ) ;
        CopyScreen( Log, Screen ) ;

        while( !flag )
        {
                if( gogame )
                {
                        DemoSlide = FALSE ;
                        GameInProgress = FALSE ;

                        ClearPlasmaMenu() ;     // vire plasma menu

                        FlagShadeMenu = TRUE ;

                        ret = MainLoop() ;

                        BackupScreen( FALSE ) ;

                        switch( ret )
                        {
                                case 0: // <ESC>
                                        if( SavingEnable )      CurrentSaveGame() ;     // sauve dans current.lba
                                        break ;

                                case 1: // Gagné !!!!
#ifdef  DEMO
                                        SlideShow() ;
//                                        DemoBumper() ;
					unlink( CURRENTSAVE_FILENAME ) ;
					FlagShowEndDemo = TRUE ;

					Load_HQR( SCREEN_HQR_NAME, Log, PCR_MENU ) ;
					CopyScreen( Log, Screen ) ;
					BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;

					PtrPal = PtrPalNormal ;
					if( !FlagBlackPal )     FadeToBlack( PtrPal ) ;

					FadeMenu = TRUE ;
#else
					Credits( TRUE ) ;
					PlayAcf( "BABY" ) ;
					Load_HQR( SCREEN_HQR_NAME, Screen, PCR_MENU ) ;
					CopyScreen( Screen, Log ) ;
					BoxStaticFullflip() ;
					EscTimer = 0 ;
#endif
                                        break ;

                                case 2: // GameOver :(
                                        // Pour ne pouvoir ni reprendre la partie
                                        // ni la sauver
                                        firstloop = TRUE ;
                                        EscTimer = 0 ;
                                        break ;
                        }

                        CopyScreen( Log, Screen ) ;
                        BoxReset() ;

                        MemoClipWindow() ;
                        UnsetClipWindow() ;

                        ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                        BoxStaticFullflip() ;

                        if( !FadeMenu ) PaletteSync( PtrPalNormal ) ;

                        HQ_PauseSamples() ;
                        FlagShadeMenu = TRUE ;
                        gogame = FALSE ;
                        InitPlasmaMenu() ;
                }

                BuildGameMainMenu( firstloop ) ;

                InitDial( 0 )           ;//     SYS

#ifdef  CDROM
                if( IsStreamPlaying() ) StopMusic() ;
                PlayMusic( CD_TRACK_MENU, TRUE ) ;

#endif

                HQ_StopSample() ;

//              GetMultiText( 49, PleaseWait ) ;

                select = DoGameMenu( GameMainMenu ) ;
                switch( select )        // num mess
                {
                        case 1000: // <ESC>, on reprend la partie en cours
                                if( firstloop )
                                {
                                        firstloop = 2 ;
                                        break ;
                                }

                        case 70:   // reprendre partie
                                if( !firstloop AND !GameInProgress AND !DemoSlide )
                                {
                                        ChoicePalette() ;
                                        FlagPal = TRUE ;
                                        InitDial( START_FILE_ISLAND+Island ) ;
                                        HQ_ResumeSamples() ;

                                        StopMusic() ;

                                        RestoreClipWindow() ;
                                        FlagFade = FALSE ;
                                        gogame = TRUE ;
                                        RestoreTimer() ;
                                }
                                else if( FileSize( CURRENTSAVE_FILENAME ) > 0 )
                                {
                                        char    memoplayername[256] ;
                                        char    memogamepathname[256] ;

                                        DemoSlide = FALSE ;

                                        strcpy( memoplayername, PlayerName ) ;
                                        strcpy( memogamepathname, GamePathname ) ;

                                        StopMusic() ;

                                        firstloop = FALSE ;
                                        strcpy( PlayerName, CURRENTSAVE_NAME ) ;
                                        strcpy( GamePathname, CURRENTSAVE_FILENAME ) ;
                                        InitGame( -1 ) ;
                                        ChangeCube( ) ;

#ifdef  DEMO
                                        // Grosse rustine pour savoir dans quelle
                                        // sauvegarde on se trouve
                                        switch( NumCube )
                                        {
                                                case 183:
                                                case 184:
                                                        CurrentDemoSave = 1 ;
                                                        break ;

                                                case 125:
                                                        CurrentDemoSave = 2 ;
                                                        break ;

                                                default:
                                                        CurrentDemoSave = 0 ;
                                        }

                                        NewDemoSave = CurrentDemoSave ;
                                        FlagShowEndDemo = FALSE ;
#endif

                                        CopyScreen( Log, ScreenAux ) ;

                                        RestoreTimer() ;
                                        SaveTimer() ;
                                        AffScene( AFF_ALL_NO_FLIP ) ;
                                        FixeCinemaMode( FALSE ) ;
                                        CopyScreen( Log, Screen ) ;

                                        // au cas ou le mec meurre avant de faire <ESC>
                                        CurrentSaveGame() ;

                                        BoxReset() ;    // pas de flip

                                        ZoomSavedGame( 240, 10 ) ;

                                        RestoreTimer() ;

                                        FlagFade = FALSE ;
                                        RestartRainSample = TRUE ;

                                        strcpy( PlayerName, memoplayername ) ;
                                        strcpy( GamePathname, memogamepathname ) ;

                                        gogame = TRUE ;
                                        EscTimer = 0 ;
                                }
                                break ;

                        case 71: // newgame
#ifdef  DEMO
                                FlagShowEndDemo = FALSE ;
                                SetDemoSaveGame( 0 ) ;
                                goto load_game ;
#else
new_game:
                                firstloop = FALSE ;
                                strcpy( PlayerName, "" ) ;
                                PtrPal = PtrPalNormal ;
                                StopMusic() ;
                                InitGame( 1 ) ;
                                Introduction() ;
                                HQ_StopSample() ;
                                FlagFade = TRUE ;
                                gogame = TRUE ;
                                EscTimer = 0 ;
                                break ;
#endif

                        case 72: // load
                                switch( ChoosePlayerName( 41, FALSE, FALSE ) )
                                {
                                        case 0:
                                                CopyScreen( Screen, Log ) ;
                                                if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                                                BoxStaticFullflip() ;
                                                break ;

                                        default:// load game
load_game:                                      firstloop = FALSE ;
                                                DemoSlide = FALSE ;
                                                StopMusic() ;
                                                InitGame( -1 ) ;
                                                ChangeCube( ) ;

#ifdef	DEMO
						TakeAllDarts() ;
#endif

                                                CopyScreen( Log, ScreenAux ) ;

                                                RestoreTimer() ;
                                                SaveTimer() ;
                                                AffScene( AFF_ALL_NO_FLIP ) ;
                                                FixeCinemaMode( FALSE ) ;
                                                CopyScreen( Log, Screen ) ;

                                                // au cas ou le mec meurre avant de faire <ESC>
                                                CurrentSaveGame() ;

                                                BoxReset() ;    // pas de flip

                                                ZoomSavedGame( 240, 50 ) ;

                                                HQ_ResumeSamples() ;
                                                RestoreTimer() ;

                                                FlagFade = FALSE ;

                                                gogame = TRUE ;
                                                EscTimer = 0 ;
                                                break ;
                                }
                                break ;

                        case 73:   // Save Game
                                if( !firstloop )
                                {
                                        savenewcube = NewCube ;

                                        if( ChoosePlayerName( 42, FALSE, TRUE ) )
                                        {
                                                NumVersion = NUM_VERSION|SAVE_COMPRESS ;
						CopyScreen( Screen, Log ) ;
#ifdef	_WIN32
						if( FileSize(OldGamePathname) )	Delete( OldGamePathname ) ;
#endif
                                                SaveGame( TRUE ) ;
                                                HQ_ResumeSamples() ;
                                                UnsetClipWindow() ;
                                        }

                                        CopyScreen( Screen, Log ) ;

                                        ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                                        NewCube = savenewcube ;
                                        BoxStaticFullflip() ;
                                        RestoreTimer() ;
                                }
                                break ;

                        case 74:        // Options
                                CopyScreen( Screen, Log ) ;
                                BoxStaticFullflip() ;
                                OptionsMenu( TRUE ) ;
                                break ;

			case 75: // quitter
				FadeToBlack( PtrPalNormal ) ;
				Cls() ;
				BoxUpdate() ;
				flag = 1 ;
				break ;

#ifdef  DEBUG_TOOLS
                        // Gestion des sauvegardes de bugs
                        case 2000:      // chargement d'un bug
                                strcpy( PathSave, PATH_SAVE_BUGS ) ;

                                switch( ChoosePlayerName( 41|0x80000000, FALSE, FALSE ) )
                                {
                                        case 0:
                                                CopyScreen( Screen, Log ) ;
                                                if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                                                BoxStaticFullflip() ;
                                                break ;

                                        default:// load game
                                                firstloop = FALSE ;
                                                DemoSlide = FALSE ;
                                                StopMusic() ;
                                                InitGame( -1 ) ;
                                                ChangeCube( ) ;

                                                CopyScreen( Log, ScreenAux ) ;

                                                RestoreTimer() ;
                                                SaveTimer() ;
                                                AffScene( AFF_ALL_NO_FLIP ) ;
                                                FixeCinemaMode( FALSE ) ;
                                                CopyScreen( Log, Screen ) ;

                                                // au cas ou le mec meurre avant de faire <ESC>
                                                CurrentSaveGame() ;

                                                BoxReset() ;    // pas de flip

                                                ZoomSavedGame( 240, 50 ) ;

                                                HQ_ResumeSamples() ;
                                                RestoreTimer() ;

                                                FlagFade = FALSE ;

                                                gogame = TRUE ;
                                                EscTimer = 0 ;
                                                break ;
                                }

                                strcpy( PathSave, PATH_SAVE ) ;
                                break ;

                        case 2001:      // sauvegarde d'un bug
                                strcpy( PathSave, PATH_SAVE_BUGS ) ;

                                if( !firstloop )
                                {
                                        savenewcube = NewCube ;

                                        if( ChoosePlayerName( 42|0x40000000, FALSE, TRUE ) )
                                        {
                                                NumVersion = NUM_VERSION|SAVE_COMPRESS ;
                                                CopyScreen( Screen, Log ) ;
                                                SaveGame( TRUE ) ;
                                                NewCube = savenewcube ;
                                                HQ_ResumeSamples() ;
                                        }

                                        CopyScreen( Screen, Log ) ;

                                        ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                                        NewCube = savenewcube ;
                                        BoxStaticFullflip() ;
                                        RestoreTimer() ;
                                }

                                strcpy( PathSave, PATH_SAVE ) ;
                                break ;
#endif
                }
        }
        return FALSE ;
}

/*══════════════════════════════════════════════════════════════════════════*
       █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀       █▀▀▀▀ █   █ █▀▀▀█  █    █▀▀▀▀ █▀▀▀▀
       ██ ▀█ ██▀▀█ ██▀ █ ██▀▀        ██    ██▀▀█ ██  █  ██   ██    ██▀▀
       ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀  ▀▀   ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

void    GameAskChoice( S32 nummess )
{
        S32     n ;

        GameChoiceMenu[0] = 0 ;
        GameChoiceMenu[1] = GameNbChoices ;
        GameChoiceMenu[2] = 0 ;
        GameChoiceMenu[3] = (U16)(START_FILE_ISLAND+Island) ;
        for(n=0; n<GameNbChoices; n++ )
        {
                GameChoiceMenu[4+n*2+0] = 0 ;                   // type
                GameChoiceMenu[4+n*2+1] = GameListChoice[n] ;   // mess
        }

        MyDial( nummess ) ;

        InitPlasmaMenu() ;

        if( DoGameMenu( GameChoiceMenu ) != 1000 )
                GameChoice = GameListChoice[GameChoiceMenu[0]] ; // ret num mess
        else
        {
                GameChoice = GameListChoice[0] ; // ESC

                // redessine curseur
                GameChoiceMenu[0] = 0 ;
                DrawGameMenu( GameChoiceMenu, FALSE ) ;
        }

        ClearPlasmaMenu() ;

        InitWaitNoKey() ;
        InitWaitNoInput( I_FIRE|I_ACTION|I_MENUS ) ;

#ifdef  CDROM
        NumObjSpeak = NUM_PERSO ;
        Speak( GameChoice ) ;
        MyGetInput() ;
        while( TestSpeak() AND (MyKey!=K_ESC AND !(Input&I_MENUS)) )    MyGetInput() ;// Wait until silence
        StopSpeak() ;// Security
#endif
}

/*══════════════════════════════════════════════════════════════════════════*
             █▀▀▀▀ █▀▀▀█ █▄ ▄█ █▀▀▀▀       █▀▀▀█ █   █ █▀▀▀▀ █▀▀▀█
             ██ ▀█ ██▀▀█ ██▀ █ ██▀▀        ██  █ ██ ▄▀ ██▀▀  ██▀█▀
             ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀   ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

#define GAMEOVER_ZOOM_TEMPO     1000
#define GAMEOVER_TEMPO          3*1000

#define GAMEOVER_MINZOOM        5000
#define GAMEOVER_MAXZOOM        200

void    GameOver()
{
        S16     *ptr3do ;
        S32     zoom ;
        U32     chrono ;

	HQ_StopSample() ;

	if( ListVarGame[160]>0 )
	{
		SaveTimer() ;

		if( !FlagBlackPal )	FadeToBlack( PtrPal ) ;

		PlayAcf( "DELUGE" ) ;

		if( CubeMode==CUBE_INTERIEUR )
		{
			InitGrille( NumCube ) ;
		}
		RazListPartFlow() ;
		ChoicePalette()   ;

		RestoreTimer() ;
		FlagFade = TRUE ;
		AffScene( AFF_ALL_NO_FLIP ) ;
		CopyScreen( Log, Screen ) ;
		BoxBlit() ;
		BoxReset() ;
	}
	else
	{
		BackupScreen( FALSE ) ;
		CopyScreen( Log, Screen ) ;
		BoxReset() ;
	}

	PaletteSync( PtrPal ) ;

        ptr3do = (S16 *)LoadMalloc_HQR( RESS_HQR_NAME, RESS_GAME_OVER ) ;
        if( !ptr3do )   return ;

	// lance un sample ou music de game over ??

        zoom = GAMEOVER_MINZOOM ;

        SetProjection( 320,240, 128,200,200 ) ;
        SetFollowCamera( 0,0,0, 0,0,0, zoom )   ;
        SetLightVector( 0, 0, 0 ) ;

        ManageTime() ;
        chrono= TimerRefHR ;

        // Magouille pour precharger le sample
        if( SamplesEnable )
        {
                HQRGetDelFunc = DelBlocHQRSample ;
                GivePtrSample( SAMPLE_GAME_OVER ) ;
                HQRGetDelFunc = NULL ;
        }

        MyGetInput() ;

        while( (MyKey != K_ESC AND !(Input&I_MENUS)) AND (TimerRefHR-chrono<=GAMEOVER_ZOOM_TEMPO) )
        {
                ManageTime() ;
                MyGetInput() ;

                zoom = BoundRegleTrois( GAMEOVER_MINZOOM, GAMEOVER_MAXZOOM,
                                        GAMEOVER_ZOOM_TEMPO, TimerRefHR-chrono ) ;

                SetFollowCamera( 0,0,0, 0,0,0, zoom )   ;

                UnsetClip() ;

                if( BodyDisplay( 0,0,0, 0,0,0, ptr3do ) )
                {
                        // Objet pas tout clippé
                        SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
                        if( ClipXMin <= ClipXMax
                        AND ClipYMin <= ClipYMax )
                        {
                                BoxMovingAdd( ClipXMin,ClipYMin, ClipXMax,ClipYMax ) ;
                                BoxUpdate() ;
                        }
                }
        }

        HQ_MixSample( SAMPLE_GAME_OVER, 0x1000, 2000, 1, 64, 127 ) ;

        SetFollowCamera( 0,0,0, 0,0,0, zoom ) ;

        UnsetClip() ;

        if( BodyDisplay( 0,0,0, 0,0,0, ptr3do ) )
        {
                // Objet pas tout clippé
                SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
                if( ClipXMin <= ClipXMax
                AND ClipYMin <= ClipYMax )
                {
                        BoxStaticAdd( ClipXMin,ClipYMin, ClipXMax,ClipYMax ) ;
                        BoxUpdate() ;
                }
        }

        chrono = TimerRefHR + GAMEOVER_TEMPO ;
        while( (TimerRefHR < chrono) AND (MyKey!=K_ESC AND !(Input&I_MENUS)) )
        {
                ManageTime() ;
                MyGetInput() ;
        }

        UnsetClip() ;
        Free( ptr3do ) ;

        PtrInit3DView() ;

        FadeToBlack( PtrPal ) ;

        PtrPal = PtrPalNormal ;
        FadeMenu = TRUE ;

	UnsetClip() ;
}

/*══════════════════════════════════════════════════════════════════════════*
                            █   █ █▀▀▀▀ █     █▀▀▀█
                            ██▀▀█ ██▀▀  ██    ██▀▀▀
                            ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
/*
void    Help( U8 flaggame )
{
        if( !flaggame ) // appelé depuis les menus
        {
                SaveTimer() ;

                FlagFade = FALSE ;

                EffectPcx( PCR_HELP/2, (U8*)Log, 0, flaggame ) ;

                WaitNoInput()   ;
                WaitInput()     ;
                FadeToBlack( PalettePcx ) ;
                CopyScreen( Screen, Log ) ;
                if( FlagShadeMenu )     ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
                BoxStaticFullflip() ;
                FadeMenu = TRUE ;
                RestoreTimer()  ;
        }
        else    EffectPcx( PCR_HELP/2, (U8*)Screen, 0, flaggame ) ;
}
*/

/*══════════════════════════════════════════════════════════════════════════*
                   █▀▀▀▀ █▀▀▀█ █▀▀▀▀ █▀▀▀▄  █    ▀▀█▀▀ ██▀▀▀
                   ██    ██▀█▀ ██▀▀  ██  █  ██     ██  ▀▀▀▀█
                   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀   ▀▀     ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void    Credits( S32 mode )
{
        SaveTimer() ;

//      FadeToBlack( PtrPalNormal ) ;

        WaitNoInput() ;

        SaveCamera() ;

        GamePlayCredits( CREDITS_HQR_NAME, mode ) ;

        RestoreCamera() ;

        GameInProgress = TRUE ;

	if( !mode )
	{
		Load_HQR( SCREEN_HQR_NAME, Screen, PCR_MENU ) ;
		CopyScreen( Screen, Log ) ;
		BoxStaticFullflip() ;
	}

        FadeMenu = TRUE ;

        WaitNoInput() ;

        RestoreTimer() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

// SlideShow de screenshots
#define SCRSHOT_HQR_NAME        PATH_RESSOURCE"scrshot.hqr"

S32     SlideShow( void )
{
	U8	pal[768+RECOVER_AREA] ;
        S32     n ;
        S32     endtimer ;
        U32     nbshots  ;
	U32     handle   ;
	S32	ret = TRUE ;
	S32	memofirsttime = FirstTime ;
	U8	fadepal = FALSE ;
	U8	memobulle = Bulle ;
	U8	memocoul ;

        // recupere nombre de screen shot dans le .HQR directement
        handle = OpenRead( SCRSHOT_HQR_NAME ) ;
        if( handle )
        {
                Read( handle, &nbshots, 4 ) ;
		nbshots = ((nbshots/4)-1)/2 ;// /2 car il y a aussi les palettes
		Close ( handle ) ;
	}
	else    return TRUE ;

	InitDial( 2 ) ;	// pour les textes sur les screen shots

	Bulle = FALSE ;

#ifdef	DEMO
	FlagSlideShow = TRUE ;// pour les tempos des messages
#endif

	FirstTime = AFF_OBJETS_NO_FLIP ;

	memocoul = ListObjet[NUM_PERSO].CoulObj ;
	ListObjet[NUM_PERSO].CoulObj = 0 ;

	NumObjDial = NUM_PERSO ;

	Init3DView() ;

	for( n=0; n<nbshots; n++ )
	{
		InitWaitNoKey() ;

		FadeToBlack( PtrPal ) ;

		// charge image
		Load_HQR( SCRSHOT_HQR_NAME, Log, (n*2) ) ;

		// on peut pas afficher le logo LBA2 car il est dans la palette
		// du jeu
//		AffGraph( 0, 639-105, 3, HQR_Get(HQRPtrSprite,11) ) ;

		Dial( START_SLIDESHOW_TEXT+n, FALSE ) ;

		// charge palette
		Load_HQR( SCRSHOT_HQR_NAME, pal, (n*2)+1 ) ;

		// reforce en blanc la couleur 12
		pal[12*3]   = 255 ;
		pal[12*3+1] = 255 ;
		pal[12*3+2] = 255 ;

		BoxStaticFullflip() ;
		FadeToPal( pal ) ;

		fadepal = TRUE 	;
		PtrPal  = pal 	;

		ManageTime() ;
		endtimer = TimerRefHR + 4*1000 ;

		while( (TimerRefHR<endtimer) AND !MyKey )
		{
			ManageTime() ;
			MyGetInput() ;
		}

		if( MyKey==K_ESC OR (Input&I_MENUS) )
		{
			ret = FALSE ;
			break ;
		}

		if( MyKey==K_D )	break ;
	}

	FirstTime = memofirsttime ;
	Bulle = memobulle ;
	ListObjet[NUM_PERSO].CoulObj = memocoul ;

#ifdef	DEMO
	FlagSlideShow = FALSE ;
#endif

	InitDial( 0 ) ;

	if( fadepal )
	{
		FadeToBlack( pal ) ;
		PtrPal = PtrPalNormal ;
	}

	GameInProgress = TRUE ;// pour recharger la partie en cours

	return ret ;
}

void    SlideDemo( int argc, char *argv[] )
{
        S32     aff = TRUE ;
        U32     cubedeb = 5 ;

#ifdef  DEBUG_TOOLS
        if( argc>1 )    cubedeb = atoi( argv[1] ) ;
#else
        argc = argc ;   // pour virer warning
        argv = argv ;
#endif

        DemoSlide = TRUE ;

        FOREVER
        {
                if( !BumperLogo() )
                {
                        aff = FALSE ;
                        break ;
                }

                InitGame( 1 ) ;
                NewCube = cubedeb ;

                FlagFade = TRUE ;

                if( !MainLoop() )       break ;

                if( !SlideShow() )      break ;

//#ifdef        CDROM
//              StopMusicCD() ;
//#endif

                FadeToBlack( PtrPal ) ;
                Cls()  ;
                BoxUpdate() ;

//              Credits(?) ;

                if( !AdelineLogo() )    break ;
        }

        FadeToBlack( PtrPal ) ;
//#ifdef        CDROM
//      StopMusicCD() ;
//#endif

        if( aff )       BumperLogo() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

S32     AdelineLogo( void )
{
        U32     endtimer ;
        S32     ret = TRUE ;
//      S32     volume ;

        if( !Load_HQR( SCREEN_HQR_NAME, Log, PCR_LOGO ) )
                TheEndCheckFile( SCREEN_HQR_NAME ) ;

        if( !Load_HQR( SCREEN_HQR_NAME, PalettePcx, PCR_LOGO+1 ) )
                TheEndCheckFile( SCREEN_HQR_NAME ) ;

        PtrPal = PalettePcx ;

        Palette( PalettePcx ) ;


//      volume = RegleTrois( 0, SampleVolume, 127, 127 ) ;
        // ATTENTION: déborde sur ScreenAux (voir MEM.CPP) !!!!!!!
//      if( Load_HQR( PATH_RESSOURCE"samples.hqr", PtrZBuffer, SAMPLE_LOGO ) )
        {
                S32     timer ;

                // pause permettant d'attendre (a peu près) la synchro ecran !
                ManageTime() ;
                timer = TimerRefHR+1000 ;

                while( TimerRefHR<=timer )
                {
                        ManageTime() ;
                }

//              PlaySample( PtrZBuffer, SAMPLE_LOGO, 0x1000, 1, volume, 64 ) ;
        }

        InitWaitNoKey() ;

        PlayMusic( MUSIC_LOGO, TRUE ) ;

        BoxStaticFullflip() ;

        ManageTime() ;
	endtimer = TimerRefHR + 3*1000 ;// 3s
	while( (TimerRefHR<=endtimer OR IsStreamPlaying()) AND !MyKey )
        {
                ManageTime() ;
                MyGetInput() ;
        }
        if( MyKey==K_ESC OR (Input&I_MENUS) )   ret = FALSE ;

        FlagBlackPal = FALSE ;
        FadeToBlack( PalettePcx ) ;

        ManageTime() ;
        endtimer = TimerRefHR + 1*1000 ;
        while( TimerRefHR<=endtimer AND !MyKey )
        {
                ManageTime() ;
                MyGetInput() ;
        }
        if( MyKey==K_ESC OR (Input&I_MENUS) )   ret = FALSE ;

	FadeOutVolumeMusic() ;
	StopMusic() ;

#ifdef	CDROM
	SetVolumeCD( CDVolume ) ;
#endif
	SetVolumeJingle( JingleVolume ) ;

//      HQ_StopSample() ;
//      StopMusic() ;

        // ATTENTION: La music du logo est stoppée plus tard (Dans MainGameMenu())

        return ret ;
}

/*──────────────────────────────────────────────────────────────────────────*/

S32     ShowLogo( S32 numscr, S32 nsec )
{
	U32     endtimer ;
	S32     ret = TRUE ;

	if( Load_HQR( SCREEN_HQR_NAME, Log, numscr )
	AND Load_HQR( SCREEN_HQR_NAME, PalettePcx, numscr+1) )
	{
		WaitNoInput() ;

		PtrPal = PalettePcx ;

		BoxStaticFullflip() ;
		FadeToPal( PalettePcx ) ;

		ManageTime() ;
		endtimer = TimerRefHR + nsec*1000 ;// wait nsec

		while( TimerRefHR<=endtimer AND !MyKey )
		{
			ManageTime() ;
			MyGetInput() ;
		}

		if( MyKey==K_ESC OR (Input&I_MENUS) )   ret = FALSE ;

		FadeToBlack( PalettePcx ) ;
	}

	return ret ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    DistribLogo( void )
{
#ifdef	DEMO
	// Bumper 3 Distributeurs
	ShowLogo( 3*2, 5 ) ;
#else
	// depend de la version
	switch( DistribVersion )
	{
		case ACTIVISION_VERSION:
		case ACTIVISION_SUD_VERSION:
			ShowLogo( PCR_ACTIVISION, 2 ) ;
			break ;

		case EA_VERSION:
			ShowLogo( PCR_EA, 2 ) ;
			break ;

		case VIRGIN_VERSION:
		case VIRGIN_ASIA_VERSION:
			ShowLogo( PCR_VIRGIN, 2 ) ;
			break ;
	}
#endif
}

/*──────────────────────────────────────────────────────────────────────────*/

S32     BumperLogo( void )
{
/*	U32     endtimer ;
	S32     ret = TRUE ;

	if( !Load_HQR( SCREEN_HQR_NAME, Log, PCR_BUMPER ) )
		TheEndCheckFile( SCREEN_HQR_NAME ) ;

	if( !Load_HQR( SCREEN_HQR_NAME, PalettePcx, PCR_BUMPER+1) )
		TheEndCheckFile( SCREEN_HQR_NAME ) ;

	WaitNoInput() ;

	PtrPal = PalettePcx ;

	BoxStaticFullflip() ;
	FadeToPal( PalettePcx ) ;

	ManageTime() ;
	endtimer = TimerRefHR + 3*1000 ;

	while( TimerRefHR<=endtimer AND !MyKey )
	{
		ManageTime() ;
		MyGetInput() ;
	}

	if( MyKey==K_ESC OR (Input&I_MENUS) )   ret = FALSE ;

	FadeToBlack( PalettePcx ) ;

	return ret ;
*/
	return( ShowLogo( PCR_BUMPER, 5 ) ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

#ifdef  DEMO
void    DemoLogo( void )
{
	char string[30] ;
	S32     x0, y0, x1, y1 ;
	S32     endtimer ;

	FlagBlackPal = FALSE ;

	FadeToBlackAndSamples( PtrPal ) ;
	BoxReset() ;
	Cls() ;

	strcpy( string, "      Demo  Version      " ) ;
	ColorFont( LBAWHITE ) ;

	// cadre
	x0 = 320 - SizeFont( string )/2 - 50 ;
	x1 = 320 + SizeFont( string )/2 + 50 ;
	y0 = 240-25 ;
	y1 = 240+25 ;
//      ShadeBoxBlk( 0, 0, 639, 479, SCREEN_SHADE_LVL ) ;
	BackupAngles( x0, y0, x1, y1 ) ;
	ShadeBoxBlk( x0, y0, x1, y1, MENU_SHADE_LVL ) ;
	DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
	RestoreAngles( x0, y0, x1, y1 ) ;
	Font( x0+50, y0+7, string ) ;
	BoxStaticFullflip() ;

	FadeToPal( PtrPal ) ;

	InitWaitNoKey() ;

	ManageTime() ;
	endtimer = TimerRefHR + 2*1000 ;

	// wait a key or tempo
	while( TimerRefHR<=endtimer AND !MyKey )
	{
		ManageTime() ;
		MyGetInput() ;
	}

	FadeToBlack( PtrPal ) ;
	Cls() ;
	BoxUpdate() ;
	FlagBlackPal = FALSE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    DemoBumper( void )
{
/*
	U8      pal[768+RECOVER_AREA] ;

	if( Load_HQR( SCREEN_HQR_NAME, Log, PCR_BUMPER )
	AND Load_HQR( SCREEN_HQR_NAME, pal, PCR_BUMPER+1 ) )
	{
		S32     endtimer ;

		WaitNoInput() ;

		FadeToBlack( PtrPal ) ;
		BoxStaticFullflip() ;
		FadeToPal( pal ) ;

		PtrPal = PtrPalNormal ;

		ManageTime() ;
		endtimer = TimerRefHR + 2*1000 ;

		while( (TimerRefHR<endtimer) AND !MyKey )
		{
			ManageTime() ;
			MyGetInput() ;
		}

		FadeToBlack( pal ) ;

		Load_HQR( SCREEN_HQR_NAME, Log, PCR_MENU ) ;
	}
*/
	ShowLogo( PCR_BUMPER, 5 ) ;
	Load_HQR( SCREEN_HQR_NAME, Log, PCR_MENU ) ;
}
#endif  // DEMO

/*══════════════════════════════════════════════════════════════════════════*
                               █▀▀▀█ █▀▀▀▀ ▀▄ ▄▀
                               ██▀▀▀ ██     ▄▀▄
                               ▀▀    ▀▀▀▀▀ ▀   ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
void    EffectPcx( U8 numpcx, U8 *screen, U8 effect, U8 flagbcl )
{
        SaveTimer() ;

        numpcx *= 2 ;   // car il y a les palettes intercalées

        if( !FlagFade ) FadeToBlackAndSamples( PtrPal ) ;

        BoxReset() ;    // pour virer les boites de la scene

        // charge palette qui se trouve derriere dans le .HQR
        if( !Load_HQR( SCREEN_HQR_NAME, PalettePcx, numpcx+1 ) )
                TheEndCheckFile( SCREEN_HQR_NAME ) ;

        switch( effect )
        {
                case PCX_FADE:
                        if( !Load_HQR( SCREEN_HQR_NAME, Log, numpcx ) )
                                TheEndCheckFile( SCREEN_HQR_NAME ) ;

                        BoxStaticFullflip() ;
                        FadeToPal( PalettePcx ) ;
                        break ;

                case PCX_V_SHADE:
                {
                        S32     x, y ;
                        U8      *ptr = screen ;

                        if( !Load_HQR( SCREEN_HQR_NAME, screen, numpcx ) )
                                TheEndCheckFile( SCREEN_HQR_NAME ) ;

                        PaletteSync( PalettePcx ) ;

                        for( y=0; y<=479; y++ )
                        {
                                CopyBlock( 0, y, 639, y+4, screen, 0, y, Log ) ;
                                y+=5 ;
                                ptr+=5*640 ;

                                for( x=y; x<=479; x++ )
                                {
                                        CopyBlock( 0, y, 639, y, screen, 0, x, Log ) ;
                                }

                                BoxStaticAdd( 0, y-5, ModeDesiredX-1, ModeDesiredY-1 ) ;
                                VblVESA() ;
                                BoxUpdate() ;
                        }
                }; break ;
        }

        if( flagbcl )
        {
                WaitNoInput()   ;
                WaitInput()     ;
                FadeToBlack( PalettePcx ) ;
                RestoreTimer()  ;
                AffScene( AFF_ALL_FLIP ) ;
                FlagFade = TRUE ;
        }
        else    RestoreTimer()  ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    EffectPcxMess( U8 numpcx, U8 *screen, U8 effect, S32 mess )
{
#ifdef  CDROM
        S32     memoflagdisplaytext     ;
#endif

        SaveTimer() ;

        EffectPcx( numpcx, screen, effect, FALSE ) ;

        FirstTime = AFF_OBJETS_FLIP ;
        FlagFade = FALSE ;

#ifdef  CDROM
        memoflagdisplaytext = FlagDisplayText ;
        FlagDisplayText = TRUE ;
#endif

        HQ_StopSample()                 ;

        Dial( mess, FALSE )             ;

	RestartRainSample = TRUE        ;

#ifdef  CDROM
        FlagDisplayText = memoflagdisplaytext ;
#endif

        FadeToBlack( PalettePcx ) ;
        RestoreTimer() ;
        FlagFade = FALSE ;
        AffScene( AFF_ALL_FLIP ) ;
        FlagFade = TRUE ;
}

/*══════════════════════════════════════════════════════════════════════════*
     █    ██▄ █ ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀▄ █   █ █▀▀▀▀ ▀▀█▀▀  █    █▀▀▀█ ██▄ █
     ██   ██▀██   ██  ██▀█▀ ██  █ ██  █ ██  █ ██      ██   ██   ██  █ ██▀██
     ▀▀   ▀▀  ▀   ▀▀  ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀  ▀▀▀▀▀ ▀▀▀▀▀   ▀▀   ▀▀   ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void    Introduction( void )
{
	PlayAcf( "INTRO" ) ;
	FlagPlayAcf = FALSE ;
}
