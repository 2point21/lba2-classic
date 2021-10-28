#ifdef	WATCOM9
#pragma	library ("f:\projet\lib386\lib_menu\lb9_menu.lib");
#else
#pragma	library ("f:\projet\lib386\lib_menu\lib_menu.lib");
#endif

#define	SELECT_NO_FLAG		0
#define SELECT_SHOW_EXT		1
#define	SELECT_TEST_EXIST	2
#define	SELECT_TEST_OVERWRITE	4
#define	SELECT_KEEP_POS		8

#define	NO_FLAG			0
#define	FLAG_PUSHED		0x0001
#define	FLAG_RED		0x0002
#define	FLAG_NO_PUSH		0x0004
#define	FLAG_CENTRE		0x0100
#define	FLAG_CONTOUR		0x0200
#define	FLAG_FILENAME		0x0400

#define	TYPE_TEXT		0x0001
#define	TYPE_BUTTON		0x0002
#define	TYPE_WINDOW		0x0004
#define	TYPE_SWITCH		0x0008
#define	TYPE_CHANGE_VALUE	0x0010
#define	TYPE_DEC_VALUE		0x0020
#define	TYPE_INC_VALUE		0x0040

extern	UBYTE	SelectorPathname[] ;

typedef	struct	{	WORD	X1 ;		/* size/pos pixel box */
			WORD	Y1 ;
			WORD	X2 ;
			WORD	Y2 ;

			WORD	Type ;
			WORD	Flags ;		/* texte centré... */

			WORD	Handle ;	/* info retournée si != -1 */
			UBYTE	*PtrString ;

			WORD	*PtrVar ;	/* info switch */
			WORD	Mask ;

		}	T_CLICK_BOX ;


typedef	struct	{       WORD		NbBox ;
			WORD		LastClicked ;
			T_CLICK_BOX	*PtrMallocList ;

			WORD	Largeur ;	/* en caractere */
			WORD	Hauteur ;

			WORD	X1 ;
			WORD	Y1 ;
			WORD	X2 ;
			WORD	Y2 ;

			WORD	Flags ;

			WORD	Repeat ;

		}	T_MENU ;

typedef struct	{	WORD	WindowX1 ;
			WORD	WindowY1 ;
			WORD	WindowX2 ;
			WORD	WindowY2 ;
			WORD	WindowLig ;
			WORD	WindowCol ;
			WORD	WindowTab ;
			WORD	WindowNbIndex ;
			UBYTE	**WindowArray ;

		}	T_WINLIST ;

/*══════════════════════════════════════════════════════════════════════════*/

#define	B_GRIS		1
#define	B_ROUGE		4

#define	BLEU		9
#define	VERT		10
#define	CYAN		11
#define	ROUGE		12
#define	MAGENTA		13
#define	JAUNE		14
#define	BLANC		15

/*══════════════════════════════════════════════════════════════════════════*/
/* asm_sort.asm */

extern	int MySortCompFunc( char**, char** ) ;

/*══════════════════════════════════════════════════════════════════════════*/
/* menu.c */

extern void SetMenuAutoPal(short );
extern void InitPalMenu(void);
extern void CopyPalMenu(unsigned char *);
extern void DrawWindow(long ,long ,long ,long ,long );
extern void OpenWindow(T_WINLIST *,short ,short ,short ,short );
extern void GetWindowButton(T_MENU *,short ,T_WINLIST *);
extern void WinText(T_WINLIST *,unsigned char *,long );
extern void WinList(T_WINLIST *,unsigned char **,short ,short );
extern void WinSelect(T_WINLIST *,short ,short );
extern void WinSepar(T_WINLIST *);
extern void FlipWindow(T_WINLIST *);
extern short GetIndexWinList(T_WINLIST *);
extern void DrawBox(T_MENU *,short ,short ,short );
extern T_CLICK_BOX *AddHeaderButton(T_MENU *,short ,short ,short ,short );
extern void GetCoorButton(T_MENU *,short ,long *,long *,long *,long *);
extern void AddButton(T_MENU *,short ,short ,short ,short ,short ,short ,unsigned char *);
extern void AddChangeValue(T_MENU *,short ,short ,short ,short ,short ,short ,unsigned char *,short *,short ,short ,short );
extern void AddSwitch(T_MENU *,short ,short ,short ,short ,short ,short ,unsigned char *,short *,short );
extern void AddText(T_MENU *,short ,short ,short ,short ,short ,unsigned char *);
extern void AddWindow(T_MENU *,short ,short ,short ,short ,short ,short );
extern short GetOptimalSize(short );
extern short OpenMenu(T_MENU *,short ,short );
extern void CloseMenu(T_MENU *);
extern short DrawMenu(T_MENU *,short ,short );
extern void ClearMenu(T_MENU *);
extern short ScanListClick(T_MENU *);
extern long GereMenu(T_MENU *);

/*──────────────────────────────────────────────────────────────────────────*/
/* menufunc.c */

extern T_CLICK_BOX *GetPtrLcb(T_MENU *,short );
extern void OrButtonFlagIfTrue(T_MENU *,short ,short ,short );
extern void SwitchButton(T_MENU *,short );
extern void ChangeButtonFlags(T_MENU *,short ,short ,short );
extern void ChangeButtonString(T_MENU *,short ,unsigned char *,short );
extern void InputString(T_MENU *,short ,short );
extern void Message(unsigned char *,long );
extern long Confirm(unsigned char *,unsigned char *,unsigned char *);

/*──────────────────────────────────────────────────────────────────────────*/
/* selector.c */

extern void CutFileName(unsigned char *);
extern void DrawListFiles(short );
extern void ScanFiles(unsigned char *,long );
extern short ThereIsFiles(unsigned char *);
extern void ScanDirs(unsigned char *);
extern void ScanDrives(void);
extern long NumDrive(unsigned char *);
extern short FileSelector(unsigned char *,unsigned char *,unsigned char *,long );

/*══════════════════════════════════════════════════════════════════════════*/






