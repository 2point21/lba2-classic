#ifdef __cplusplus
extern "C" {
#endif

#ifndef	FUNC_H
#define	FUNC_H

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern U32	ComputeTime( void );

#pragma	aux     ComputeTime	"*"	\
		parm caller []		\
		modify exact [eax ebx ecx edx]


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern U32	GetHDFreeSize( void );

#pragma	aux     GetHDFreeSize	"*"	\
		parm caller []		\
		modify exact [eax ebx ecx edx]


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern void	SmallSort(T_SORT *, U32, U32);

#pragma	aux     SmallSort	"*"	\
		parm caller []		\
		modify [eax ecx edx]


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#ifdef	_WIN32
#define		VsyncTimer()
#else
extern void	VsyncTimer( void ) ;

#pragma	aux     VsyncTimer	"*"	\
		parm caller []		\
		modify [eax ecx edx]
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
// Pour l'instant, en attendant d'avoir un PaletteSync propre !!!!!!!!!

#define	PaletteSync(pal)	{ PaletteVblVESA(TRUE); Palette(pal); PaletteVblVESA(FALSE); }
//#define	PaletteSync(pal)	{ VsyncTimer(); Palette(pal); }

/*extern void	PaletteSync( U8 *palette ) ;

#pragma	aux	VsyncTimer

#pragma	aux	PaletteSync \
		parm caller [esi] \
		modify [esi]
*/

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern U8	SearchBoundColRGB( U8 Rouge, U8 Vert, U8 Bleu, U8 *Palette, U8 coulmin, U8 coulmax ) ;

#pragma	aux     SearchBoundColRGB	"*"	\
		parm caller []			\
		modify [eax ecx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#endif	/* FUNC_H */

#ifdef __cplusplus
}
#endif
