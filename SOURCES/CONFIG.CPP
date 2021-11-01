/*
  ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

                €€€€€€ø €€€€€€ø €€ø €€ø €€€€€€ø €€€€€€ø €€€€€€ø
                €€⁄ƒƒƒŸ €€⁄ƒ€€≥ €€€ø€€≥ €€⁄ƒƒƒŸ ¿ƒ€€⁄ƒŸ €€⁄ƒƒƒŸ
                €€≥     €€≥ €€≥ €€€€€€≥ €€€€€ø    €€≥   €€≥€€€ø
                €€≥     €€≥ €€≥ €€⁄€€€≥ €€⁄ƒƒŸ    €€≥   €€≥¿€€≥
                €€€€€€ø €€€€€€≥ €€≥¿€€≥ €€≥     €€€€€€ø €€€€€€≥
                ¿ƒƒƒƒƒŸ ¿ƒƒƒƒƒŸ ¿ƒŸ ¿ƒŸ ¿ƒŸ     ¿ƒƒƒƒƒŸ ¿ƒƒƒƒƒŸ

                        (Menu de configuration du clavier)

  ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
  ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
*/

#include        "c_extern.h"

#define CONFIG_SHADE_LVL        MENU_SHADE_LVL

#define START_ACTION_TXT        100
#define START_STATUS_TXT        150
#define START_MENUS_TXT         160
#define START_KEYS_TXT          200

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
#define KEY_STRING_EMPTY        ".............."

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
#define STATUS_MENU     0
#define STATUS_PRESS    1
#define STATUS_ERROR    2
#define STATUS_NOTHING  3

#define CFG_TXT_ACTION  0
#define CFG_TXT_KEY1    1
#define CFG_TXT_KEY2    2
#define CFG_TXT_DEFAUT  3
#define CFG_TXT_LAST    4
#define CFG_TXT_OK      5

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
S32     SelectConfig = 0 ;
S32     SelectKey    = 0 ;

#define CFG_X0  10
#define CFG_Y0  10
#define CFG_X1  630
#define CFG_Y1  (CFG_Y0+MAX_INPUT*10+35)

#define ACTION_X        (CFG_X0+20)
#define KEY1_X          (CFG_X0+371)
#define KEY2_X          (CFG_X0+501)

#define CFG_MENU_X0     10
#define CFG_MENU_Y0     (CFG_Y1+5)
#define CFG_MENU_X1     630
#define CFG_MENU_Y1     (CFG_MENU_Y0+10+3*10)

#define CFG_TXT_X0      10
#define CFG_TXT_Y0      (CFG_MENU_Y1+5)
#define CFG_TXT_X1      630
#define CFG_TXT_Y1      (CFG_TXT_Y0+10+10)

/* old colors (Programmer's colors !)
#define COUL_TXT        (12*16+11)
#define COUL_CADRE      (255)
#define COUL_STATUS     (2*16+13)
#define COUL_ERROR      (4*16+13)
#define COUL_CONFIG     (255)
#define COUL_KEY        (255)
#define COUL_MENU       (8*16+11)
*/

// new colors (Artist's colors !)
#define COUL_TXT        (12*16+11)
#define COUL_CADRE      (2*16+13)
#define COUL_STATUS     (2*16+13)
#define COUL_ERROR      (4*16+10)
#define COUL_CONFIG     (255)
#define COUL_KEY        (255)
#define COUL_MENU       (10*16+14)

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
char    KeyStrings[MAX_INPUT*2][50] ;   // 14 char max

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
U8      ScanKnown[] =
{
        K_NUMPAD_1      ,
        K_NUMPAD_2      ,
        K_NUMPAD_3      ,
        K_NUMPAD_4      ,
        K_NUMPAD_5      ,
        K_NUMPAD_6      ,
        K_NUMPAD_7      ,
        K_NUMPAD_8      ,
        K_NUMPAD_9      ,
        K_NUMPAD_0      ,
        K_NUMPAD_POINT  ,
        K_NUMPAD_ENTER  ,
        K_NUMPAD_DIV    ,
        K_NUMPAD_MUL    ,
        K_NUMPAD_PLUS   ,
        K_NUMPAD_MOINS  ,

        K_GRAY_UP       ,
        K_GRAY_DOWN     ,
        K_GRAY_LEFT     ,
        K_GRAY_RIGHT    ,
        K_GRAY_HOME     ,
        K_GRAY_END      ,
        K_GRAY_PAGE_UP  ,
        K_GRAY_PAGE_DOWN,
        K_GRAY_INSER    ,
        K_GRAY_SUPPR    ,

        K_SHIFT_LEFT    ,
        K_SHIFT_RIGHT   ,
        K_CTRL_LEFT     ,
        K_CTRL_RIGHT    ,
        K_ALT_LEFT      ,
        K_ALT_RIGHT     ,

        K_TAB           ,
        K_NUMLOCK       ,
        K_CAPSLOCK      ,

        K_F1            ,
        K_F2            ,
        K_F3            ,
        K_F4            ,
        K_F5            ,
        K_F6            ,
        K_F7            ,
        K_F8            ,
        K_F9            ,
        K_F10           ,
        K_F11           ,
        K_F12           ,

        K_1             ,
        K_2             ,
        K_3             ,
        K_4             ,
        K_5             ,
        K_6             ,
        K_7             ,
        K_8             ,
        K_9             ,
        K_0             ,

        K_SPACE         ,
        K_ENTER         ,

        K_WINDOWS_LEFT  ,
        K_WINDOWS_RIGHT ,
        K_APPLICATION   ,

        K_BACKSPACE     ,
        K_PAUSE         ,
        K_SCROLLLOCK    ,
        K_PRINTSCR      ,

        K_SHIFT         ,
        K_CTRL          ,
        K_ALT           ,

        0               // End
} ;

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawConfigFire( S32 xmin, S32 ymin, S32 xmax, S32 ymax, S32 coul )
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

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    GetKeyText(char *string, U32 scan)
{
        S32             i               ;

        if( scan )
        {
#ifdef  _WIN32
                if((scan>=VK_F1)&&(scan<=VK_F24))
                {
                        sprintf(string, "F%d", scan-VK_F1+1) ;
                        goto end        ;
                }
                else if(scan>=256)
                {
                        S32     joy     ;
                        S32     button  ;
                        char    rest[30];

                        scan -= 256     ;

                        joy = 1+(scan>>6);
                        button = scan&63;
                        sprintf(string, "Joy %d ", joy) ;

                        if(button<32)
                        {
                                sprintf(rest, "Btn %d", button+1)       ;
                        }
                        else
                        {
                                switch(button)
                                {
                                case 32:
                                case 33:
                                case 34:
                                case 35:
                                        button -= 32    ;
                                        break   ;
                                case 36:
                                case 37:
                                        button -= 34    ;
                                        strcat(string, "Z ")    ;
                                        break   ;
                                case 38:
                                case 39:
                                        button -= 36    ;
                                        strcat(string, "R ")    ;
                                        break   ;
                                case 40:
                                case 41:
                                        button -= 38    ;
                                        strcat(string, "U ")    ;
                                        break   ;
                                case 42:
                                case 43:
                                        button -= 40    ;
                                        strcat(string, "V ")    ;
                                        break   ;
                                default:
                                        button -= 44    ;
                                        strcat(string, "H ")    ;
                                        break   ;
                                }

                                button = 3-button       ;
                                GetMultiText( START_KEYS_TXT+16+button, rest ) ;
                        }

                        strcat(string, rest)    ;
                        goto end                ;
                }
#endif
                for(i=0; ScanKnown[i]; i++)
                {
                        if(ScanKnown[i]==scan)  break ;
                }

                if(ScanKnown[i])
                {
                        GetMultiText( START_KEYS_TXT+i, string ) ;
                }
                else
                {
                        char    ascii   ;

                        ascii = (char)ScanToAscii(scan);

                        if(ascii)
                        {
                                string[0] = ascii ;
                                string[1] = 0     ;
                        }
                        else
                        {
                                sprintf(string, "Key %02X", (U32)scan)  ;
                        }
                }
        }
        else
        {
                strcpy( string, KEY_STRING_EMPTY ) ;
        }
end:
        // Pour l'instant, en blindage
        string[14] = 0 ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    InitKeyStrings( void )
{
        S32     n ;

        for( n=0; n<MAX_INPUT; n++ )
        {
                GetKeyText(KeyStrings[n*2],   DefKeys[n].Key1) ;
                GetKeyText(KeyStrings[n*2+1], DefKeys[n].Key2) ;
        }
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawOneConfig( S32 n )
{
        char    string[100] ;
        S32     y = CFG_Y0+30+n*10 ;
        S32     x = ACTION_X ;
        S32     x0, y0, x1, y1 ;

        // Action

        x0 = CFG_X0+5 ;
        y0 = y-2 ;
        x1 = KEY1_X-15 ;
        y1 = y+8 ;

        CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;

//      if( FlagShadeMenu )
        {
                ShadeBoxBlk( x0, y0, x1, y1, CONFIG_SHADE_LVL ) ;
        }

        if( n==SelectConfig )
        {
                CoulText( COUL_CONFIG, -1 ) ;
                x -= 5 ;
        }
        else
        {
                CoulText( COUL_TXT, -1 ) ;
        }

        BoxStaticAdd( x0, y0, x1, y1 ) ;

        GetMultiText( START_ACTION_TXT+n, string ) ;
        GraphPrintf( FALSE, x, y, string  ) ;

        // Key 1

        x0 = KEY1_X-5 ;
        y0 = y-2 ;
        x1 = KEY2_X-15 ;
        y1 = y+8 ;

        CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;

//      if( FlagShadeMenu )
        {
                ShadeBoxBlk( x0, y0, x1, y1, CONFIG_SHADE_LVL ) ;
        }

        BackupAngles( x0, y0, x1, y1 ) ;

        x = KEY1_X ;

        if( n==SelectConfig )
        {
                if( SelectKey==0 )
                {
                        CoulText( COUL_KEY, -1 ) ;
                        DrawConfigFire( x0, y0, x1, y1, FIRE_SELECT_MENU ) ;
                        DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
                }
                else    x -= 5 ;
        }

        RestoreAngles( x0, y0, x1, y1 ) ;
        BoxStaticAdd( x0, y0, x1, y1 ) ;

        GraphPrintf( FALSE, x, y, KeyStrings[n*2] ) ;

        // Key 2

        x0 = KEY2_X-5 ;
        y0 = y-2 ;
        x1 = CFG_X1-5 ;
        y1 = y+8 ;

        CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;

//      if( FlagShadeMenu )
        {
                ShadeBoxBlk( x0, y0, x1, y1, CONFIG_SHADE_LVL ) ;
        }

        BackupAngles( x0, y0, x1, y1 ) ;

        x = KEY2_X ;

        if( n==SelectConfig )
        {
                if( SelectKey==1 )
                {
                        CoulText( COUL_KEY, -1 ) ;
                        DrawConfigFire( x0, y0, x1, y1, FIRE_SELECT_MENU ) ;
                        DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
                }
                else
                {
                        CoulText( COUL_CONFIG, -1 ) ;
                        x -= 5 ;
                }
        }

        RestoreAngles( x0, y0, x1, y1 ) ;
        BoxStaticAdd( x0, y0, x1, y1 ) ;

        GraphPrintf( FALSE, x, y, KeyStrings[n*2+1] ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawOneConfigMenu( S32 n )
{
        char    txt[100] ;
        S32     x0, y0, x1, y1 ;
        S32     y ;

        if( n<MAX_INPUT )
        {
                DrawOneConfig( n ) ;
                return ;
        }

        n -= MAX_INPUT ;
        y = CFG_MENU_Y0+7+n*10 ;

        GetMultiText( START_MENUS_TXT+CFG_TXT_DEFAUT+n, txt ) ;

        x0 = 320-(strlen(txt)*SizeChar)/2-10 ;
        y0 = y-2 ;
        x1 = 320+(strlen(txt)*SizeChar)/2+10 ;
        y1 = y+8 ;

        CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;

//      if( FlagShadeMenu )
        {
                ShadeBoxBlk( x0, y0, x1, y1, CONFIG_SHADE_LVL ) ;
        }

        if( (SelectConfig-MAX_INPUT)==n )
        {
                CoulText( COUL_KEY, -1 ) ;
                BackupAngles( x0, y0, x1, y1 ) ;
                DrawConfigFire( x0, y0, x1, y1, FIRE_SELECT_MENU ) ;
                DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
                RestoreAngles( x0, y0, x1, y1 ) ;
        }
        else
        {
                CoulText( COUL_MENU, -1 ) ;
        }

        GraphPrintf( FALSE, 320-(strlen(txt)*SizeChar)/2, y, txt ) ;

        BoxStaticAdd( x0, y0, x1, y1 ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawCadreConfig( S32 x0, S32 y0, S32 x1, S32 y1, S32 flagclean )
{
        if( flagclean )
        {
                CopyBlock( x0, y0, x1, y1, Screen,
                           x0, y0, Log ) ;
        }

        BackupAngles( x0, y0, x1, y1 ) ;

//      if( FlagShadeMenu )
        {
                ShadeBoxBlk( x0, y0, x1, y1, CONFIG_SHADE_LVL ) ;
        }

        DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
        RestoreAngles( x0, y0, x1, y1 ) ;

        if( flagclean )
        {
                BoxStaticAdd( x0, y0, x1, y1 ) ;
        }
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawStatusConfig( S32 num )
{
        char txt[100] ;

        GetMultiText( START_STATUS_TXT+num, txt ) ;

        DrawCadreConfig( CFG_TXT_X0, CFG_TXT_Y0, CFG_TXT_X1, CFG_TXT_Y1, TRUE ) ;

        if( num==STATUS_ERROR )
        {
                CoulText( COUL_ERROR, -1 ) ;
        }
        else
        {
                CoulText( COUL_STATUS, -1 ) ;
        }

        GraphPrintf( FALSE, 320-(strlen(txt)*SizeChar)/2, CFG_TXT_Y0+7, txt ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawScreenConfig( void )
{
        S32     n ;
        char    string[100] ;

        CopyScreen( Screen, Log ) ;

        // Fenetres Action/Key1/Key2

        DrawCadreConfig( CFG_X0, CFG_Y0, KEY1_X-11, CFG_Y0+20, FALSE ) ;
        DrawCadreConfig( KEY1_X-9, CFG_Y0, KEY2_X-11, CFG_Y0+20, FALSE ) ;
        DrawCadreConfig( KEY2_X-9, CFG_Y0, CFG_X1, CFG_Y0+20, FALSE ) ;

        DrawCadreConfig( CFG_X0, CFG_Y0+20+2, KEY1_X-11, CFG_Y1, FALSE ) ;
        DrawCadreConfig( KEY1_X-9, CFG_Y0+20+2, KEY2_X-11, CFG_Y1, FALSE ) ;
        DrawCadreConfig( KEY2_X-9, CFG_Y0+20+2, CFG_X1, CFG_Y1, FALSE ) ;

        CoulText( COUL_CADRE, -1 ) ;

        GetMultiText( START_MENUS_TXT+CFG_TXT_ACTION, string ) ;
        GraphPrintf( FALSE, ACTION_X, CFG_Y0+7, string ) ;

        GetMultiText( START_MENUS_TXT+CFG_TXT_KEY1, string ) ;
        GraphPrintf( FALSE, KEY1_X, CFG_Y0+7, string ) ;

        GetMultiText( START_MENUS_TXT+CFG_TXT_KEY2, string ) ;
        GraphPrintf( FALSE, KEY2_X, CFG_Y0+7, string ) ;

        // Menu

        DrawCadreConfig( CFG_MENU_X0, CFG_MENU_Y0, CFG_MENU_X1, CFG_MENU_Y1, FALSE ) ;

        // Barre de Status

        DrawStatusConfig( STATUS_MENU ) ;

        for( n=0; n<MAX_INPUT; n++ )
        {
                DrawOneConfig( n ) ;
        }

        for( n=MAX_INPUT; n<MAX_INPUT+3; n++ )
        {
                DrawOneConfigMenu( n ) ;
        }

        BoxStaticAdd( 0, 0, ModeDesiredX, ModeDesiredY ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
S32     CheckKeyConfig( S32 key )
{
        S32     n ;

        for( n=0; n<MAX_INPUT; n++ )
        {
                if( DefKeys[n].Key1==key
                OR  DefKeys[n].Key2==key )
                {
                        return n ;
                }
        }

        return -1 ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
#define REPEATDELAY     400
#define REPEATRATE      40

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    MenuConfig( void )
{
        S32     oldselect ;
        U32     lasttime  ;
        U32     repeattime;
	S32     ok = FALSE ;
	S32	memoclipwindow = FALSE ;

	if( ClipWindowYMin>0 )
	{
		// on vient du jeu en cinema mode
		MemoClipWindow() ;
		UnsetClipWindow() ;
		memoclipwindow = TRUE ;
	}

        // premiere fois
        if( !KeyStrings[0][0] ) InitKeyStrings() ;

        PaletteSync( PtrPalNormal ) ;

        DrawScreenConfig() ;

        WaitNoInput() ;

        oldselect = SelectConfig        ;

        memcpy( MemoDefKeys, DefKeys, sizeof(T_DEF_KEY)*MAX_INPUT ) ;

        MyKey = 0       ;
        ManageTime()    ;
        lasttime = TimerRefHR;

        while( !ok )
        {
                S32     oldkey  ;
                S32     time    ;

                DrawOneConfigMenu( SelectConfig ) ;

                BoxUpdate()     ;

                oldkey = MyKey  ;
#ifdef  _WIN32
                DetectJoys()    ;
#endif
                MyGetInput()    ;

#ifdef  _WIN32
                switch(MyKey)
                {
                case K_NUMPAD_8:
                        MyKey = K_UP;
                        break   ;
                case K_NUMPAD_2:
                        MyKey = K_DOWN;
                        break   ;
                case K_NUMPAD_4:
                        MyKey = K_LEFT;
                        break   ;
                case K_NUMPAD_6:
                case K_TAB:
                        MyKey = K_RIGHT;
                        break   ;
                case K_NUMPAD_9:
                        MyKey = K_PAGE_UP;
                        break   ;
                case K_NUMPAD_3:
                        MyKey = K_PAGE_DOWN;
                        break   ;
                case K_NUMPAD_7:
                        MyKey = K_HOME;
                        break   ;
                case K_NUMPAD_1:
                        MyKey = K_END;
                        break   ;
                case K_NUMPAD_POINT:
                case K_BACKSPACE:
                        MyKey = K_SUPPR;
                        break   ;
                case K_NUMPAD_ENTER:
                case K_SPACE:
                        MyKey = K_ENTER;
                        break   ;
                case K_UP:
                case K_DOWN:
                case K_LEFT:
                case K_RIGHT:
                case K_PAGE_UP:
                case K_PAGE_DOWN:
                case K_HOME:
                case K_END:
                case K_SUPPR:
                case K_ENTER:
                case K_ESC:
                        break   ;
                default:
                        MyKey = 0       ;
                        break   ;
                }
#else// _WIN32
                switch(MyKey)
                {
                case K_GRAY_UP:
                        MyKey = K_UP;
                        break   ;
                case K_GRAY_DOWN:
                        MyKey = K_DOWN;
                        break   ;
                case K_GRAY_LEFT:
                        MyKey = K_LEFT;
                        break   ;
                case K_GRAY_RIGHT:
                case K_TAB:
                        MyKey = K_RIGHT;
                        break   ;
                case K_GRAY_PAGE_UP:
                        MyKey = K_PAGE_UP;
                        break   ;
                case K_GRAY_PAGE_DOWN:
                        MyKey = K_PAGE_DOWN;
                        break   ;
                case K_GRAY_HOME:
                        MyKey = K_HOME;
                        break   ;
                case K_GRAY_END:
                        MyKey = K_END;
                        break   ;
                case K_GRAY_SUPPR:
                case K_BACKSPACE:
                        MyKey = K_SUPPR;
                        break   ;
                case K_NUMPAD_ENTER:
                case K_SPACE:
                        MyKey = K_ENTER;
                        break   ;
                case K_UP:
                case K_DOWN:
                case K_LEFT:
                case K_RIGHT:
                case K_PAGE_UP:
                case K_PAGE_DOWN:
                case K_HOME:
                case K_END:
                case K_SUPPR:
                case K_ENTER:
                case K_ESC:
                        break   ;
                default:
                        MyKey = 0       ;
                        break   ;
                }
#endif//_WIN32

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
		CheckSaveLogPcx( PtrPalNormal ) ;
#endif
                if(!MyKey)
                {
                        if(Input&I_UP)
                        {
                                MyKey = K_UP    ;
                        }
                        else if(Input&I_DOWN)
                        {
                                MyKey = K_DOWN  ;
                        }
                        else if(Input&I_LEFT)
                        {
                                MyKey = K_LEFT  ;
                        }
                        else if(Input&I_RIGHT)
                        {
                                MyKey = K_RIGHT ;
                        }

                        if(Input&~I_JOY)
                        {
                                MyKey = K_ENTER ;
                        }
                }

#ifdef  _WIN32
                if(!MyKey)
                {
                        S32     i       ;
                        U32     *joys   ;

                        joys = (U32*)&TabKeys[256]      ;

                        for(i=0; i<16*2; i+=2)
                        {
                                if(joys[i])
                                {
                                        MyKey = K_ENTER ;
                                        break   ;
                                }
                                if(joys[i+1]&1)
                                {
                                        MyKey = K_RIGHT ;
                                        break   ;
                                }
                                if(joys[i+1]&2)
                                {
                                        MyKey = K_LEFT  ;
                                        break   ;
                                }
                                if(joys[i+1]&4)
                                {
                                        MyKey = K_DOWN  ;
                                        break   ;
                                }
                                if(joys[i+1]&8)
                                {
                                        MyKey = K_UP    ;
                                        break   ;
                                }
                        }
                }
#endif//_WIN32
                ManageTime()    ;
                time = TimerRefHR;

                if(oldkey==MyKey)
                {
                        if(MyKey==K_ESC)
                        {
                                continue;
                        }
                        if(time-lasttime<REPEATDELAY)
                        {
                                repeattime = 0  ;
                                continue;
                        }

                        if(repeattime)
                        {
                                if(time-repeattime<REPEATRATE)
                                {
                                        continue;
                                }
                                repeattime += REPEATRATE ;
                        }
                        else
                        {
                                repeattime = time       ;
                        }
                }
                else
                {
                        lasttime = time ;
                }

                if(MyKey==K_UP)
                {
                        SelectConfig-- ;
                        if( SelectConfig<0 )    SelectConfig = MAX_INPUT-1+3 ;
                }
                else if(MyKey==K_DOWN)
                {
                        SelectConfig++ ;
                        if( SelectConfig>=(MAX_INPUT+3) )       SelectConfig = 0 ;
                }
                else if(MyKey==K_ENTER)
                {
                        if( SelectConfig<MAX_INPUT )
                        {
                                S32     memokey ;
                                U32     *ptr ;

                                // define new key
                                if( SelectKey==0 )
                                {
                                        ptr = &DefKeys[SelectConfig].Key1 ;
                                }
                                else
                                {
                                        ptr = &DefKeys[SelectConfig].Key2 ;
                                }

                                memokey = *ptr ;

                                WaitNoInput() ;

                                *ptr = 0 ;

#ifdef  _WIN32
                                KeyDown()       ;
                                MyKey = 0       ;
#endif
                                while( !MyKey )
                                {
#ifdef  _WIN32
                                        MyKey = KeyDown()       ;
#else
                                        MyGetInput()            ;
#endif

                                        if( (TimerRefHR&0xFF) < 127 )   // aff 2/3 eff 1/3
                                        {
                                                DrawStatusConfig( STATUS_NOTHING ) ;
                                                strcpy( KeyStrings[SelectConfig*2+SelectKey], "              " ) ;
                                        }
                                        else
                                        {
                                                DrawStatusConfig( STATUS_PRESS ) ;
                                                strcpy( KeyStrings[SelectConfig*2+SelectKey], KEY_STRING_EMPTY ) ;
                                        }

                                        DrawOneConfig( SelectConfig ) ;

                                        BoxUpdate() ;
                                }

                                if( MyKey==K_ESC )
                                {
                                        MyKey = memokey ;
                                        DrawStatusConfig( STATUS_MENU ) ;
                                }
                                else
                                {
                                        if( CheckKeyConfig(MyKey)!=-1 )
					{
						PlayErrorSample() ;
						MyKey = memokey ;
                                                DrawStatusConfig( STATUS_ERROR ) ;
                                        }
                                        else
                                        {
                                                DrawStatusConfig( STATUS_MENU ) ;
                                        }
                                }

                                *ptr = MyKey ;
                                GetKeyText( KeyStrings[SelectConfig*2+SelectKey], MyKey ) ;

                                WaitNoInput() ;
                        }
                        else
                        {
                                switch( (SelectConfig-MAX_INPUT) )
                                {
                                case 0: // Configuration par DÇfaut
                                        memcpy( MemoDefKeys, DefKeys, sizeof(T_DEF_KEY)*MAX_INPUT ) ;
                                        RestoreInput() ;
                                        InitKeyStrings() ;
                                        DrawScreenConfig() ;
                                        break ;

                                case 1: // Restaurer Configuration PrÇcÇdente
                                        memcpy( DefKeys, MemoDefKeys, sizeof(T_DEF_KEY)*MAX_INPUT ) ;
                                        InitKeyStrings() ;
                                        DrawScreenConfig() ;
                                        break ;

                                case 2: // Accepter Configuration
                                        // memo config ici
                                        ok = TRUE ;
                                        break ;
                                }
                        }
                }

                switch(MyKey)
                {
                case K_PAGE_UP:
                        SelectConfig-=10 ;
                        if( SelectConfig<0 )    SelectConfig = 0 ;
                        break;

                case K_PAGE_DOWN:
                        SelectConfig+=10 ;
                        if( SelectConfig>=(MAX_INPUT+3) )       SelectConfig = MAX_INPUT-1+3 ;
                        break;

                case K_HOME:
                        SelectConfig = 0 ;
                        break;

                case K_END:
                        SelectConfig = MAX_INPUT-1+3 ;
                        break;

                case K_ESC:
                        if( SelectConfig!=MAX_INPUT+2 )
                        {
                                SelectConfig = MAX_INPUT+2 ;// Menu Accepter Configuration
                        }
                        else
                        {
                                goto endloop    ;
                        }
                        break;

                }

                if(oldselect!=SelectConfig)
                {
                        DrawOneConfigMenu( oldselect ) ;
                        oldselect = SelectConfig ;
                }

                if( SelectConfig<MAX_INPUT )
                {
                        if( MyKey==K_LEFT
                        OR  MyKey==K_RIGHT )
                        {
                                SelectKey ^= 1 ;
                        }

                        if( MyKey==K_SUPPR )
                        {
                                if( SelectKey==0 )
                                        DefKeys[SelectConfig].Key1 = 0 ;
                                else    DefKeys[SelectConfig].Key2 = 0 ;

                                strcpy( KeyStrings[SelectConfig*2+SelectKey], KEY_STRING_EMPTY ) ;
                        }
                }

        }
endloop:
        if( !ok )       // Sortie par <ESC>
        {
                // restaure config
                memcpy( DefKeys, MemoDefKeys, sizeof(T_DEF_KEY)*MAX_INPUT ) ;
                InitKeyStrings() ;
        }

        CopyScreen( Screen, Log ) ;
        BoxStaticAdd( 0, 0, ModeDesiredX, ModeDesiredY ) ;

        InitInput() ;

        InitWaitNoInput( Input|I_MENUS|I_FIRE ) ;
	InitWaitNoKey() ;

	if( memoclipwindow )
	{
		RestoreClipWindow() ;
	}
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
//
//              Petit Module pour demander l'insertion du CDROM
//
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#define CONFIRM_SHADE_LVL        15

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DetourGraphPrintf( S32 flip, S32 coul, S32 x, S32 y, char *txt )
{
        CoulText( 0, -1 ) ;

        GraphPrintf( FALSE, x-1, y-1, txt ) ;
        GraphPrintf( FALSE, x  , y-1, txt ) ;
        GraphPrintf( FALSE, x+1, y-1, txt ) ;
        GraphPrintf( FALSE, x-1, y  , txt ) ;
        GraphPrintf( FALSE, x+1, y  , txt ) ;
        GraphPrintf( FALSE, x-1, y+1, txt ) ;
        GraphPrintf( FALSE, x  , y+1, txt ) ;
        GraphPrintf( FALSE, x+1, y+1, txt ) ;

        CoulText( coul, -1 ) ;
        GraphPrintf( flip, x, y, txt ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawCadreConfirm( S32 x0, S32 y0, S32 x1, S32 y1 )
{
        BackupAngles( x0, y0, x1, y1 ) ;
        ShadeBoxBlk( x0, y0, x1, y1, CONFIRM_SHADE_LVL ) ;

        DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
        RestoreAngles( x0, y0, x1, y1 ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
void    DrawConfirmChoice( S32 xlen, S32 y0, char *txt, S32 select )
{
        S32     x0, x1, y1, coul = COUL_MENU ;

        x0 = 320-xlen ;
        x1 = 320+xlen ;
        y1 = y0+SizeChar+3;
        y0-- ;

        CopyBlock( x0, y0, x1, y1, Screen, x0, y0, Log ) ;

        if( select )
        {
                coul = COUL_KEY ;
                BackupAngles( x0, y0, x1, y1 ) ;
                DrawConfigFire( x0, y0, x1, y1, FIRE_SELECT_MENU ) ;
                DrawCadre( x0, y0, x1, y1, ALL_ANGLES ) ;
                RestoreAngles( x0, y0, x1, y1 ) ;
        }

        DetourGraphPrintf( FALSE, coul, 320-(strlen(txt)*SizeChar)/2, y0+3, txt ) ;
        BoxStaticAdd( x0, y0, x1, y1 ) ;
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
S32     ConfirmMenu( char *ask, char *ok, char *cancel )
{
        char    *ptr            ;
        S32     larg = (strlen(ask)+6)*SizeChar ;
        S32     haut = 4*SizeChar ;
        S32     x0, y0, x1, y1  ;
        S32     by0, by1        ;// boutons y
        S32     blen            ;// boutons lenght
        S32     select = 0      ;
        S32     oldselect = 1   ;
        S32     flag = -1       ;
#ifndef _WIN32
        S32     firstaff  = TRUE;
#endif

        x0 = 320-larg/2 ;
        x1 = 320+larg/2 ;
        y0 = 240-haut/2 ;
        y1 = 240+haut/2 ;

        by0 = y1+2              ;
        by1 = by0+4*SizeChar    ;

	DrawCadreConfig( x0, y0, x1, by1, FALSE ) ;

#ifdef	_WIN32
	BoxStaticAdd( x0, y0, x1, by1 ) ;
#endif

        DetourGraphPrintf( FALSE, COUL_STATUS, x0+3*SizeChar, y0+2*SizeChar, ask ) ;

        CopyScreen( Log, Screen ) ;

        // ATTENTION: pas blindÇ si question trop petite ...
        blen = ((x1-x0)/2)-80 ;

        InitWaitNoKey() ;// attente de relache de touche dans la boucle

        // on va dessiner en premier le bouton cancel (car oldselect==1)
        ptr = cancel    ;

        while( flag==-1 )
        {
                MyGetInput() ;

                switch( MyKey )
                {
                        case K_ESC:
                                flag   = 1 ;
                                select = 1 ;
                                break ;

                        case K_ENTER:
                                flag = select ;
                                break ;

                        // pave numerique
                        case K_UP:
                        case K_DOWN:
                        case K_PAGE_UP:
                        case K_PAGE_DOWN:
                        case K_HOME:
                        case K_END:

                        // touches de controle
#ifdef  _WIN32                        
                        case K_NUMPAD_8:
                        case K_NUMPAD_2:
                        case K_NUMPAD_9:
                        case K_NUMPAD_3:
                        case K_NUMPAD_7:
                        case K_NUMPAD_1:
#else                        
                        case K_GRAY_UP:
                        case K_GRAY_DOWN:
                        case K_GRAY_PAGE_UP:
                        case K_GRAY_PAGE_DOWN:
                        case K_GRAY_HOME:
                        case K_GRAY_END:
#endif
                        case K_TAB:
                                select ^= 1 ;
                                break ;
                }

                if( select!=oldselect )
		{
#ifndef _WIN32
                        if( firstaff )
                        {
                                FadeToPal( PtrPalNormal) ;
                                firstaff = FALSE ;
			}
#endif

                        DrawConfirmChoice( blen, by0+(SizeChar+4)*oldselect, ptr, FALSE ) ;

                        if( select )    ptr = cancel    ;
                        else            ptr = ok        ;

                        oldselect = select ;

                        InitWaitNoKey() ;// attente de relache de touche dans la boucle
                }

                DrawConfirmChoice( blen, by0+(SizeChar+4)*select, ptr, TRUE ) ;
                BoxUpdate() ;
        }

        return( flag^1 ) ;// ok=TRUE, cancel=FALSE
}

//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
S32     AskForCD( void )
{
        S32     ret             ;
        char    ask_txt[255]    ;
        char    ok_txt[20]      ;
        char    cancel_txt[20]  ;

        // ATTENTION: pour entrer lÖ, le InitDial(0) doit etre fait avant !

#ifndef _WIN32
        Load_HQR( SCREEN_HQR_NAME, Log, PCR_CDROM ) ;
        BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;
#endif

        InitPlasmaMenu() ;

	switch( DistribVersion )
	{
		case UNKNOWN_VERSION:
		case EA_VERSION:
			GetMultiText( 7, ask_txt ) ;
			break ;

		default:
			GetMultiText( 6, ask_txt ) ;
	}

	GetMultiText( 8, ok_txt ) ;
        GetMultiText( 9, cancel_txt ) ;

        ret = ConfirmMenu( ask_txt, ok_txt, cancel_txt ) ;

#ifdef  _WIN32
        Load_HQR( SCREEN_HQR_NAME, Log, PCR_CDROM ) ;
        BoxStaticFullflip() ;
#else
        FadeToBlack( PtrPalNormal ) ;
#endif

        return ret ;
}
