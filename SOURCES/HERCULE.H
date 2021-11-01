#ifndef	HERCULE_H
#define	HERCULE_H

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#define	BASEH	0xB0000L	// Adresse de base carte Hercule
#define	ADRH(x,y)	((char *)(HTabY[y]+x*2))

// Modes de rectangles

#define	HBOX_SIMPLE	0
#define	HBOX_DOUBLE	1

typedef	struct
	{
		char sg ;	// superieur gauche
		char sd ;	// superieur droit
		char ig ;	// inferieur gauche
		char id ;	// inferieur droit
		char h  ;	// horizontal
		char v  ;	// vertical
	} HBoxStyle ;

// Attributs de couleurs

#define	HNORM	0x07
#define	HUNDER	0x01	// Souligne
#define	HINV	0x70	// Video inverse
#define	HBLIND	0x80	// Clignotant

// defines pour la capture d'ecran

#define	SIZE_LINE_MAX	(80*2+2)
#define	SIZE_SCREEN_MAX	(SIZE_LINE_MAX*25)

// Flags pour la capture d'ecran

#define PICK_ATTR	1	// Capture également les attributs des caracteres
#define	PICK_CRLF	2	// Ajoute des cr/lf à la fin de chaque ligne
				// (a utiliser generalement sans PICK_ATTR)

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#ifdef __cplusplus
extern "C" {
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	S32		HTabY[25] ;	// Adresses Y de la carte hercule

#pragma	aux	HTabY	"*"

extern	HBoxStyle       HBStyles[] ;

extern	S32	HCurX ;
extern	S32	HCurY ;

extern	S32	ModeHercule ;

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	S32	TestHercule( void ) ;

#pragma	aux	TestHercule	"*"	\
		parm caller []		\
		modify exact [eax ebx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#ifdef __cplusplus
}
#endif


/*══════════════════════════════════════════════════════════════════════════*
	    ██████┐ ██┐ ██┐ ██████┐ ██████┐ ██┐ ██┐ ██████┐ ██████┐
	    ██┌───┘ ██│ ██│ ██┌─██│ └─██┌─┘ ███┐██│ ██┌───┘ ██┌───┘
	    ██│     ██████│ ██████│   ██│   ██████│ █████┐  ██████┐
	    ██│     ██┌─██│ ██┌─██│   ██│   ██┌███│ ██┌──┘  └───██│
	    ██████┐ ██│ ██│ ██│ ██│ ██████┐ ██│└██│ ██████┐ ██████│
	    └─────┘ └─┘ └─┘ └─┘ └─┘ └─────┘ └─┘ └─┘ └─────┘ └─────┘
 *══════════════════════════════════════════════════════════════════════════*/

// ATTENTION: Toutes ces Fonctions ne sont pas clippées, donc méfie !

// 0<x<79
// 0<y<24

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#ifdef __cplusplus
extern "C" {
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	HCls( void ) ;

#pragma	aux	HCls		"*"	\
		parm caller []		\
		modify [eax ecx edx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	HPrintCar( S32 x, S32 y, char car ) ;

#pragma	aux	HPrintCar	"*"		\
		parm caller [eax] [ebx] [ecx]	\
		modify exact [eax ebx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	HPrintCarC( S32 x, S32 y, U8 attr, char car ) ;

#pragma	aux	HPrintCarC	"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx edx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HString( S32 x, S32 y, char *string ) ;

#pragma	aux	HString		"*"		\
		parm caller [eax] [ebx] [ecx]	\
		modify exact [eax ebx ecx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HStringC( S32 x, S32 y, U8 attr, char *string ) ;

#pragma	aux	HStringC	"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx edx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HScrollUp( void ) ;

#pragma	aux     HScrollUp	"*"	\
		parm caller []		\
		modify exact [eax ecx esi edi]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HScrollDown( void ) ;

#pragma	aux     HScrollDown	"*"	\
		parm caller []		\
		modify exact [eax ecx esi edi]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
#ifdef __cplusplus
}
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void 	HCentre( S32 y, char *texte ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void 	HCentreC( S32 y, U8 attr, char *texte ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HStringf( S32 x, S32 y, char *format, ... ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HStringfC( S32 x, S32 y, U8 attr, char *format, ... ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HPrintf( char *format, ... ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HPrintfC( U8 attr, char *format, ... ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HGotoXY( S32 x, S32 y ) ;
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

/*══════════════════════════════════════════════════════════════════════════*
	██████┐ ██████┐ ██████┐ ██████┐ ██┐ ██┐ ██████┐ ██████┐ ██████┐
	██┌───┘ ██┌─██│ ██┌─██│ ██┌─██│ ██│ ██│ └─██┌─┘ ██┌───┘ ██┌───┘
	██│███┐ ██████│ ██████│ ██████│ ██████│   ██│   ██│     ██████┐
	██│└██│ ██┌██┌┘ ██┌─██│ ██┌───┘ ██┌─██│   ██│   ██│     └───██│
	██████│ ██│└██┐ ██│ ██│ ██│     ██│ ██│ ██████┐ ██████┐ ██████│
	└─────┘ └─┘ └─┘ └─┘ └─┘ └─┘     └─┘ └─┘ └─────┘ └─────┘ └─────┘
 *══════════════════════════════════════════════════════════════════════════*/

// ATTENTION: Toutes ces Fonctions ne sont pas clippées, donc méfie !

// 0<x<79
// 0<y<24

#ifdef __cplusplus
extern "C" {
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HLineH( S32 y, S32 x0, S32 x1, char car ) ;

#pragma	aux	HLineH		"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HLineV( S32 x, S32 y0, S32 y1, char car ) ;

#pragma	aux	HLineV		"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx]

#ifdef __cplusplus
}
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HRect( S32 x0, S32 y0, S32 x1, S32 y1, U8 style ) ;

#ifdef __cplusplus
extern "C" {
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HColor( S32 y, S32 x0, S32 x1, U8 attr ) ;

#pragma	aux	HColor		"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HColorBox( S32 x0, S32 y0, S32 x1, S32 y1, U8 attr ) ;

#pragma	aux	HColorBox	"*"				\
		parm caller [esi] [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx esi edi]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	HClsBox( S32 x0, S32 y0, S32 x1, S32 y1, U8 attr ) ;

#pragma	aux	HClsBox		"*"				\
		parm caller [esi] [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx edx esi edi]

#ifdef __cplusplus
}
#endif

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern 	void	HBox( S32 x0, S32 y0, S32 x1, S32 y1, U8 attr, U8 style ) ;

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

/*══════════════════════════════════════════════════════════════════════════*
	    ██████┐ ██████┐ ██████┐ ██████┐ ██┐ ██┐ ██████┐ ██████┐
	    ██┌───┘ ██┌─██│ ██┌─██│ └─██┌─┘ ██│ ██│ ██┌─██│ ██┌───┘
	    ██│     ██████│ ██████│   ██│   ██│ ██│ ██████│ █████┐
	    ██│     ██┌─██│ ██┌───┘   ██│   ██│ ██│ ██┌██┌┘ ██┌──┘
	    ██████┐ ██│ ██│ ██│       ██│   ██████│ ██│└██┐ ██████┐
	    └─────┘ └─┘ └─┘ └─┘       └─┘   └─────┘ └─┘ └─┘ └─────┘
 *══════════════════════════════════════════════════════════════════════════*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	void	CaptureHerculeScreen( char *filename, U32 flags ) ;

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#endif	// HERCULE_H
