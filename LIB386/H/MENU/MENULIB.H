//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#ifndef	LIB_MENU_MENULIB
#define	LIB_MENU_MENULIB

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#ifdef __cplusplus
extern "C" {
#endif

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// PalMenu colors

#define BLACK		0
#define B_GREY		1	// 2, 3
#define B_RED	 	4	// 5, 6
#define YELLOW		7

// Unused: 8, 9

// Reserved: 10 to 245

#define BLUE		246
#define GREEN		247
#define CYAN		248
#define RED		249
#define MAGENTA		250

// Unused: 251, 252, 253, 254

#define WHITE		255

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define SELECT_NO_FLAG		0
#define SELECT_SHOW_EXT 	1
#define SELECT_TEST_EXIST	2
#define SELECT_TEST_OVERWRITE	4
#define SELECT_KEEP_POS 	8

#define NO_FLAG 		0
#define FLAG_PUSHED		0x00000001
#define FLAG_RED		0x00000002
#define FLAG_NO_PUSH		0x00000004
#define FLAG_CENTRE		0x00000100
#define FLAG_CONTOUR		0x00000200
#define FLAG_FILENAME		0x00000400
#define FLAG_REPEAT		0x00000800

#define TYPE_TEXT		0x00000001
#define TYPE_BUTTON		0x00000002
#define TYPE_WINDOW		0x00000004
#define TYPE_SWITCH		0x00000008
#define TYPE_CHANGE_VALUE	0x00000010
#define TYPE_DEC_VALUE		0x00000020
#define TYPE_INC_VALUE		0x00000040

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
typedef struct	{	S32	X1 ;		// size/pos pixel box
			S32	Y1 ;
			S32	X2 ;
			S32	Y2 ;

			S32	Type ;
			S32	Flags ; 	// texte centr‚...

			S32	Handle ;	// info retourn‚e si != -1
			char	*PtrString ;

			S32	*PtrVar ;	// info switch
			S32	Mask ;

		}	T_CLICK_BOX ;


typedef struct	{	S32		NbBox 		;
			S32		LastClicked 	;
			T_CLICK_BOX	*PtrMallocList 	;

			S32	Largeur ;	// en caractere
			S32	Hauteur ;

			S32	X1 ;
			S32	Y1 ;
			S32	X2 ;
			S32	Y2 ;
			S32	Dx ;
			S32	Dy ;

			S32	Flags ;

			S32	Repeat ;

		}	T_MENU ;

typedef struct	{	S32	WindowX1 ;
			S32	WindowY1 ;
			S32	WindowX2 ;
			S32	WindowY2 ;
			S32	WindowLig ;
			S32	WindowCol ;
			S32	WindowTab ;
			S32	WindowNbIndex ;
			char	**WindowArray ;

		}	T_WINLIST ;

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern	S32		Pointed 		;
extern	S32		FlagAutoPal		;

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define			SetMenuAutoPal(	flag )	\
			FlagAutoPal = (flag)

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern void 		DrawWindow(	S32 x1,	S32 y1,	S32 x2,	S32 y2,
					S32 flags			);

extern void 		DrawBox(	T_MENU *ptrmenu, S32 numlcb,
					S32 pushed			);

extern T_CLICK_BOX 	*AddHeaderButton(T_MENU *ptrmenu,
					S32 x1, S32 y1,	S32 width, S32 height );

extern S32 		ScanListClick(	T_MENU *ptrmenu			);

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern void 		InitPalMenu(	void		);

extern void 		CopyPalMenu(	U8 *ptrpal	);

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern void 		OpenWindow(	T_WINLIST *wlist,
					S32 x1, S32 y1, S32 largeur, S32 hauteur);

extern void 		WinSepar(	T_WINLIST *wlist	);

extern void 		WinText(	T_WINLIST *wlist,
					char *string, S32 value );

extern void 		WinList(	T_WINLIST *wlist,
					char **index, S32 nb, S32 size);

extern void 		WinSelect(	T_WINLIST *wlist,
					S32 selected, S32 coul	);

extern void 		FlipWindow(	T_WINLIST *wlist	);

extern S32 		GetIndexWinList(T_WINLIST *wlist	);

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern S32 		OpenMenu(	T_MENU *ptrmenu, S32 width, S32 height);

extern void 		AddButton(	T_MENU *ptrmenu, S32 handle,
					S32 x1, S32 y1, S32 width, S32 height,
					S32 flags, char *ptrstring	);

extern void 		AddChangeValue(	T_MENU *ptrmenu, S32 handle,
					S32 x1, S32 y1, S32 width, S32 height,
					S32 flags, char *ptrstring, S32 *ptrvar,
					S32 step, S32 minvar, S32 maxvar);

extern void 		AddSwitch(	T_MENU *ptrmenu, S32 handle,
					S32 x1, S32 y1, S32 width, S32 height,
					S32 flags, char *ptrstring, S32 *ptrvar,
					S32 mask			);

extern void 		AddText(	T_MENU *ptrmenu,
					S32 x1, S32 y1, S32 width, S32 height,
					S32 flags, char *ptrstring	);

extern void 		AddWindow(	T_MENU *ptrmenu, S32 handle,
					S32 x1, S32 y1 ,S32 width, S32 height,
					S32 flags			);

extern S32 		DrawMenu(	T_MENU *ptrmenu,
					S32 x1, S32 y1			);

extern S32 		GereMenu(	T_MENU *ptrmenu			);

extern void 		CloseMenu(	T_MENU *ptrmenu			);
extern void 		ClearMenu(	T_MENU *ptrmenu			);

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern void 		GetWindowButton(T_MENU *ptrmenu, S32 handle,
					T_WINLIST *wlist		);

extern void 		GetCoorButton(	T_MENU *ptrmenu, S32 handle,
					S32 *x0, S32 *y0, S32 *x1, S32 *y1);

extern S32 		GetOptimalSize(	S32 deltax		);

extern void 		GetCoorMenu(	T_MENU *ptrmenu,
					S32 *x0, S32 *y0, S32 *x1, S32 *y1);

extern void 		GetSizeMenu(	T_MENU *ptrmenu,
					U32 *pdx, U32 *pdy 	);

extern S32 		MenuBestFitX(	S32 deltax 		);
extern S32 		MenuBestFitY(	S32 deltay 		);

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#ifdef __cplusplus
}
#endif

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#endif//LIB_MENU_MENULIB

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
