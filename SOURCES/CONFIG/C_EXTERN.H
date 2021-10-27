#include	<dos.h>
#include	<direct.h>
#include	<ctype.h>

#include	<system.h>
#include	<svga.h>
#include	<3d.h>

#include	<fileio.h>

#include	"AddKeys.h"
#include	"Input.h"
#include	"Config.h"

#define		MAX_BOXES	200

extern	S32	MyKey	;
extern	U8	PtrPalNormal[768+RECOVER_AREA] ;
extern	U8	*PtrPal	;


extern 	void	FadePal( U8 r, U8 v, U8 b, U8 *ptrpal, S32 percent ) ;
extern 	void	FadeToBlack( U8 *ptrpal ) ;
extern	void	FadeToPal( U8 *ptrpal ) ;
extern	void	SetBlackPal() ;

extern	void	Beep() ;


#define	FadeToBlackAndSamples	FadeToBlack
#define	FadeToPalAndSamples	FadeToPal
