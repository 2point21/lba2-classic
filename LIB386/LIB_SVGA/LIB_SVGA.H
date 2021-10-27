/*-----------------------------------------------------------------------------
;                       Extern lib_svga 386
;-----------------------------------------------------------------------------*/

#ifdef	WATCOM9
#pragma library ( "f:\projet\lib386\lib_svga\lb9_svga.lib" );
#else
#pragma library ( "f:\projet\lib386\lib_svga\lib_svga.lib" );
#endif

/*-----------------------------------------------------------------------------*/

#define SVGA_VESA               0	/* VESA compliant board at Adress 0xA0000 */
#define SVGA_TRIDENT            1	/* Trident 8800 & 8900 */
#define SVGA_ATI                2	/* ATI Chipset */
#define SVGA_S3                 3	/* S3 Boards (expect init with VESA mode 102) */
#define SVGA_CIRRUS             4	/* Cirrus (some Genoa) (expect init with VESA mode 102) */
#define SVGA_TSENG              5	/* Tseng ET3000 & ET4000 */
#define SVGA_WESTERN            6	/* Western Digital / Paradise */
#define SVGA_GENOA              7	/* REAL Genoa (not Cirrus or ET3000) */

/*-----------------------------------------------------------------------------*/
extern  WORD    ClipXmin        ;
extern  WORD    ClipYmin        ;
extern  WORD    ClipXmax        ;
extern  WORD    ClipYmax        ;

extern  ULONG   TabOffLine      ;
extern  UBYTE   *Phys           ;
extern  UBYTE   *Log            ;
extern  UBYTE   *MemoLog        ;
extern  WORD    Screen_X        ;
extern  WORD    Screen_Y        ;
extern  UBYTE   Text_Ink        ;
extern  UBYTE   Text_Paper      ;
extern  UBYTE   OldVideo        ;
extern  WORD    SizeCar         ;
/*-----------------------------------------------------------------------------*/
extern  WORD    Svga_Card       ;
extern  void    *BankChange     ;
extern  LONG    BankCurrent     ;
/*-----------------------------------------------------------------------------*/

extern  WORD    TypePoly        ;

extern  WORD    NbPolyPoints    ;

extern  WORD    TabPoly[]       ;
extern  WORD    TabText[]       ;

extern  WORD    TabVerticG[]    ;
extern  WORD    TabVerticD[]    ;
extern  WORD    Ymin ;
extern  WORD    Ymax ;

extern  WORD    TabCoulG[]      ;
extern  WORD    TabCoulD[]      ;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
/*                              MOUSE.C                                     */
extern  volatile LONG   Click   ;
extern  volatile LONG   Mouse_X ;
extern  volatile LONG   Mouse_Y ;
extern  LONG            Mouse_X_Dep ;
extern  LONG            Mouse_Y_Dep ;
extern	UBYTE		*GphMouse ;

extern void __loadds __far click_handler(int ,int ,int );
extern void SetMouseBox(long ,long ,long ,long );
extern void SetMouseSpeed(long ,long );
extern void SetMousePos(long ,long );
extern void GetMouseDep(void);
extern void ShowMouse(long );
extern void AffMouse(void);
extern void SetMouse(short );
extern void InitMouse(void);
extern void ClearMouse(void);

/*-----------------------------------------------------------------------------*/
/*                              S_PAL.asm                                       */
void    PalOne( UBYTE coul, UBYTE r, UBYTE v, UBYTE b ) ;
void    PalMulti( WORD startcoul, WORD nbcoul, UBYTE *pal ) ;
void    Palette( void *pal )                            ;
/*-----------------------------------------------------------------------------*/
/*                              FONT.ASM                                        */
void    CoulFont( LONG couleur )                                ;
void    SetFont( void *font, LONG interleave, LONG interspace ) ;
LONG    SizeFont( void *chaine )                                ;
LONG    CarFont( LONG xcar, LONG ycar, LONG car )               ;
void    Font( LONG xchaine, LONG ychaine, void *chaine )        ;
/*-----------------------------------------------------------------------------*/
/*                              GRAPH_A.ASM                                     */
void    AffGraph( LONG num, LONG x, LONG y, void *bank )                ;
void    GetDxDyGraph( LONG num, LONG *dx, LONG *dy, void *bank )        ;
/*-----------------------------------------------------------------------------*/
/*                              MASK_A.ASM                                      */
void    AffMask( LONG num, LONG x, LONG y, void *bank ) ;
/*-----------------------------------------------------------------------------*/
/*                      InitSvga.asm                                            */
void    SetClip( LONG x0, LONG y0, LONG x1, LONG y1 )   ;
void    UnSetClip( void )                               ;
void    MemoClip( void )                                ;
void    RestoreClip( void )                             ;
/*-----------------------------------------------------------------------------*/
/*                      Box.asm                                                 */
void    Box( LONG x0, LONG y0, LONG x1, LONG y1, LONG couleur ) ;
/*-----------------------------------------------------------------------------*/
/*                      Line.asm                                                        */
void    Line( LONG x0, LONG y0, LONG x1, LONG y1, LONG couleur )        ;
/*-----------------------------------------------------------------------------*/
/*                      Zoom.asm                                                        */
void    ScaleLine( LONG xa0, LONG xa1, void *src, LONG xb0, LONG xb1, void *dst )       ;
void    ScaleBox( LONG xa0, LONG ya0, LONG xa1, LONG ya1, void *src,
		  LONG xb0, LONG yb0, LONG xb1, LONG yb1, void *dst )   ;
/*-----------------------------------------------------------------------------*/
/*                      CopyMask.asm                                                    */
void    CopyMask( LONG num, LONG x, LONG y, void *bank, void* screen ) ;
/*-----------------------------------------------------------------------------*/
/*                      Mask_A.asm                                                      */
void    CoulMask( LONG coulmask )                               ;
void    AffMask( LONG num, LONG x, LONG y, void *bank )         ;
void    GetDxDyMask( LONG num, LONG *dx, LONG *dy, void *bank ) ;
/*-----------------------------------------------------------------------------*/
/*                      S_PLOT.asm.asm                                                  */
void    Plot( LONG x, LONG y, LONG couleur )    ;
LONG    GetPlot( LONG x, LONG y )               ;
/*-----------------------------------------------------------------------------*/
/*                      S_SCREEN.asm                                                    */
void    Cls( void )                             ;
void    CopyScreen( void *src, void *dst )      ;
void    Vsync( void )                           ;
void    Flip( void )                            ;
void    FlipComp( void )                        ;
/*-----------------------------------------------------------------------------*/
/*                      S_STRING.asm                                                    */
void    AffString( LONG x, LONG y, void *text ) ;
void    CoulText( LONG coul0, LONG coul1 )	;
extern	UBYTE	Font8X8 ;

/*-----------------------------------------------------------------------------*/
/*                      S_BLOCK2.asm                                                    */
void    CopyBlockOnBlack( LONG x0, LONG y0, LONG x1, LONG y1, void *src, void *dst )    ;
/*-----------------------------------------------------------------------------*/
/*                      GRAPHMSK.asm                                                    */
LONG    CalcGraphMsk( LONG num, void *bank, void *mask )        ;
/*-----------------------------------------------------------------------------*/
/*                      S_BLOCK.asm                                                     */
void    CopyBlock( LONG x0, LONG y0, LONG x1, LONG y1, void *src,
		   LONG xd, LONG yd, void *dst )                        ;

/*-----------------------------------------------------------------------------*/
/*                      S_BLOCK3.asm                                                     */
void    CopyBlockIncrust( LONG x0, LONG y0, LONG x1, LONG y1, void *src,
		   LONG xd, LONG yd, void *dst )                        ;

/*-----------------------------------------------------------------------------*/
/*                      S_PHYS.asm                                                     */
// void    CopyBlockIncrust( LONG x0, LONG y0, LONG x1, LONG y1 )          ;
void    CopyBlockPhysClip( LONG x0, LONG y0, LONG x1, LONG y1 )         ;
void    CopyBlockPhys( LONG x0, LONG y0, LONG x1, LONG y1 )             ;
void    SaveBlock( void *screen, void *buffer, LONG x, LONG y, LONG dx, LONG dy ) ;
void    RestoreBlock( void *screen, void *buffer, LONG x, LONG y, LONG dx, LONG dy ) ;

/*-----------------------------------------------------------------------------*/
/*                      LBM.c	320*200					*/

extern	WORD Lbm_Width ;	// 640 or 320 (for source format)

extern unsigned char ReadIff(void);
extern void WriteIff(void *,unsigned long );
extern void WriteIffLong(unsigned long );
extern void itoml(void *);
extern void lit_bmhd(void);
extern void lit_bloc(void);
extern void lit_ligne(unsigned char *,short );
extern void ConvertMCGA(unsigned char *,unsigned char *,unsigned short ,short );
extern void lit_bodyILBM(unsigned char *);
extern void lit_bodyPBM(unsigned char *);
extern long Load_Lbm(char *,unsigned char *,unsigned char *);
extern long Save_Lbm(char *,unsigned char *,unsigned char *);

/*-----------------------------------------------------------------------------*/
/*                      PCX.c	640*480					*/

extern void line_out(unsigned char *);
extern unsigned char next_char(unsigned long ,unsigned char *);
extern void Load_Pcx(char *,unsigned char *,unsigned char *);
extern void Save_Pcx(char *,unsigned char *,unsigned char *);

/*-----------------------------------------------------------------------------*/
/*                      s_dll.c						*/

extern long SvgaInitDLL(char *);
/*-----------------------------------------------------------------------------*/
/*                      s_text.c					       */
extern void Text(short ,short ,char * ,... );

/*-----------------------------------------------------------------------------*/
/*                      s_rect.c					       */

extern void Rect(long ,long ,long ,long ,long );

/*-----------------------------------------------------------------------------*/
/*                      initmode.c					       */

extern void InitGraphSvga(void);
extern void ClearGraphSvga(void);
extern void InitGraphMcga(void);
extern void ClearGraphMcga(void);
