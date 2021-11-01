#ifndef	HERCULE_H
#define	HERCULE_H

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

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

#define PICK_ATTR	1	// Capture ‚galement les attributs des caracteres
#define	PICK_CRLF	2	// Ajoute des cr/lf … la fin de chaque ligne
				// (a utiliser generalement sans PICK_ATTR)

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#ifdef __cplusplus
extern "C" {
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	S32		HTabY[25] ;	// Adresses Y de la carte hercule

#pragma	aux	HTabY	"*"

extern	HBoxStyle       HBStyles[] ;

extern	S32	HCurX ;
extern	S32	HCurY ;

extern	S32	ModeHercule ;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	S32	TestHercule( void ) ;

#pragma	aux	TestHercule	"*"	\
		parm caller []		\
		modify exact [eax ebx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#ifdef __cplusplus
}
#endif


/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	    ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿
	    ÛÛÚÄÄÄÙ ÛÛ³ ÛÛ³ ÛÛÚÄÛÛ³ ÀÄÛÛÚÄÙ ÛÛÛ¿ÛÛ³ ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ
	    ÛÛ³     ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³   ÛÛ³   ÛÛÛÛÛÛ³ ÛÛÛÛÛ¿  ÛÛÛÛÛÛ¿
	    ÛÛ³     ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³   ÛÛ³   ÛÛÚÛÛÛ³ ÛÛÚÄÄÙ  ÀÄÄÄÛÛ³
	    ÛÛÛÛÛÛ¿ ÛÛ³ ÛÛ³ ÛÛ³ ÛÛ³ ÛÛÛÛÛÛ¿ ÛÛ³ÀÛÛ³ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ³
	    ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

// ATTENTION: Toutes ces Fonctions ne sont pas clipp‚es, donc m‚fie !

// 0<x<79
// 0<y<24

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#ifdef __cplusplus
extern "C" {
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	HCls( void ) ;

#pragma	aux	HCls		"*"	\
		parm caller []		\
		modify [eax ecx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	HPrintCar( S32 x, S32 y, char car ) ;

#pragma	aux	HPrintCar	"*"		\
		parm caller [eax] [ebx] [ecx]	\
		modify exact [eax ebx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	HPrintCarC( S32 x, S32 y, U8 attr, char car ) ;

#pragma	aux	HPrintCarC	"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HString( S32 x, S32 y, char *string ) ;

#pragma	aux	HString		"*"		\
		parm caller [eax] [ebx] [ecx]	\
		modify exact [eax ebx ecx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HStringC( S32 x, S32 y, U8 attr, char *string ) ;

#pragma	aux	HStringC	"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HScrollUp( void ) ;

#pragma	aux     HScrollUp	"*"	\
		parm caller []		\
		modify exact [eax ecx esi edi]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HScrollDown( void ) ;

#pragma	aux     HScrollDown	"*"	\
		parm caller []		\
		modify exact [eax ecx esi edi]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#ifdef __cplusplus
}
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void 	HCentre( S32 y, char *texte ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void 	HCentreC( S32 y, U8 attr, char *texte ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HStringf( S32 x, S32 y, char *format, ... ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HStringfC( S32 x, S32 y, U8 attr, char *format, ... ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HPrintf( char *format, ... ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HPrintfC( U8 attr, char *format, ... ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HGotoXY( S32 x, S32 y ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿
	ÛÛÚÄÄÄÙ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÛÛ³ ÛÛ³ ÀÄÛÛÚÄÙ ÛÛÚÄÄÄÙ ÛÛÚÄÄÄÙ
	ÛÛ³ÛÛÛ¿ ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³   ÛÛ³   ÛÛ³     ÛÛÛÛÛÛ¿
	ÛÛ³ÀÛÛ³ ÛÛÚÛÛÚÙ ÛÛÚÄÛÛ³ ÛÛÚÄÄÄÙ ÛÛÚÄÛÛ³   ÛÛ³   ÛÛ³     ÀÄÄÄÛÛ³
	ÛÛÛÛÛÛ³ ÛÛ³ÀÛÛ¿ ÛÛ³ ÛÛ³ ÛÛ³     ÛÛ³ ÛÛ³ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ³
	ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÙ ÀÄÙ ÀÄÙ     ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ ÀÄÄÄÄÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

// ATTENTION: Toutes ces Fonctions ne sont pas clipp‚es, donc m‚fie !

// 0<x<79
// 0<y<24

#ifdef __cplusplus
extern "C" {
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HLineH( S32 y, S32 x0, S32 x1, char car ) ;

#pragma	aux	HLineH		"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HLineV( S32 x, S32 y0, S32 y1, char car ) ;

#pragma	aux	HLineV		"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx]

#ifdef __cplusplus
}
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HRect( S32 x0, S32 y0, S32 x1, S32 y1, U8 style ) ;

#ifdef __cplusplus
extern "C" {
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HColor( S32 y, S32 x0, S32 x1, U8 attr ) ;

#pragma	aux	HColor		"*"			\
		parm caller [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HColorBox( S32 x0, S32 y0, S32 x1, S32 y1, U8 attr ) ;

#pragma	aux	HColorBox	"*"				\
		parm caller [esi] [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx esi edi]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	HClsBox( S32 x0, S32 y0, S32 x1, S32 y1, U8 attr ) ;

#pragma	aux	HClsBox		"*"				\
		parm caller [esi] [eax] [ebx] [ecx] [edx]	\
		modify exact [eax ebx ecx edx esi edi]

#ifdef __cplusplus
}
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern 	void	HBox( S32 x0, S32 y0, S32 x1, S32 y1, U8 attr, U8 style ) ;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
	    ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿
	    ÛÛÚÄÄÄÙ ÛÛÚÄÛÛ³ ÛÛÚÄÛÛ³ ÀÄÛÛÚÄÙ ÛÛ³ ÛÛ³ ÛÛÚÄÛÛ³ ÛÛÚÄÄÄÙ
	    ÛÛ³     ÛÛÛÛÛÛ³ ÛÛÛÛÛÛ³   ÛÛ³   ÛÛ³ ÛÛ³ ÛÛÛÛÛÛ³ ÛÛÛÛÛ¿
	    ÛÛ³     ÛÛÚÄÛÛ³ ÛÛÚÄÄÄÙ   ÛÛ³   ÛÛ³ ÛÛ³ ÛÛÚÛÛÚÙ ÛÛÚÄÄÙ
	    ÛÛÛÛÛÛ¿ ÛÛ³ ÛÛ³ ÛÛ³       ÛÛ³   ÛÛÛÛÛÛ³ ÛÛ³ÀÛÛ¿ ÛÛÛÛÛÛ¿
	    ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÙ       ÀÄÙ   ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ ÀÄÄÄÄÄÙ
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	CaptureHerculeScreen( char *filename, U32 flags ) ;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#endif	// HERCULE_H
