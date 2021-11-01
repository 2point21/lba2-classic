
#define	SELECT_NO_FLAG		0
#define SELECT_SHOW_EXT		1

#define	NO_FLAG			0
#define	FLAG_PUSHED		0x0001
#define	FLAG_RED		0x0002
#define	FLAG_NO_PUSH		0x0004
#define	FLAG_CENTRE		0x0100
#define	FLAG_CONTOUR		0x0200

typedef	struct	{	WORD	X1 ;		/* size/pos pixel box */
			WORD	Y1 ;
			WORD	X2 ;
			WORD	Y2 ;

			WORD	Handle ;	/* info retournée si != -1 */
			WORD	Flags ;		/* texte centré... */

			UBYTE	*PtrString ;

		}	T_CLICK_BOX ;


typedef	struct	{       WORD		NbBox ;
			WORD		MaxBox ;
			WORD		LastClicked ;
			WORD		LastSelected ;
			T_CLICK_BOX	*PtrMallocList ;

			WORD	Largeur ;	/* en caractere */
			WORD	Hauteur ;

			WORD	X1 ;
			WORD	Y1 ;
			WORD	X2 ;
			WORD	Y2 ;

			WORD	Flags ;

		}	T_MENU ;


/*══════════════════════════════════════════════════════════════════════════*/

WORD	OpenMenu(	T_MENU *ptrmenu,
			WORD largeur, WORD hauteur,
			WORD maxbox )			;

WORD	AddButton(	T_MENU	*ptrmenu,
			WORD 	handle,
			WORD	x1, WORD y1,
			WORD	largeur, WORD hauteur,
			WORD	flags,
			UBYTE	*ptrstring	)	;

void	DrawMenu( T_MENU *ptrmenu, WORD x1, WORD y1 )	;

WORD	GereMenu( T_MENU *ptrmenu, WORD flagwaitclick )	;

/*──────────────────────────────────────────────────────────────────────────*/

void	ChangeButtonFlags( T_MENU *ptrmenu, WORD handle, WORD flags, WORD flagaff ) ;
WORD	GetButtonFlags( T_MENU *ptrmenu, WORD handle ) ;

void	ChangeButtonString( T_MENU *ptrmenu, WORD handle, UBYTE *string, WORD flagaff ) ;
UBYTE*	GetButtonString( T_MENU *ptrmenu, WORD handle ) ;

/*══════════════════════════════════════════════════════════════════════════*/
