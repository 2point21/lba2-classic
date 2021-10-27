#ifndef	ANIMTEX_H
#define	ANIMTEX_H

/*--------------------------------------------------------------------------*/
//				fonctions ASM

#ifdef __cplusplus
extern "C" {
#endif
/*--------------------------------------------------------------------------*/
extern	void	Do_Plasma( U8 *PtrEffectStruct ) ;

#pragma	aux	Do_Plasma	"*" 		\
		parm caller [esi]		\
		modify [eax ebx ecx edx edi]

/*--------------------------------------------------------------------------*/
extern	void	Do_Fire( U8 *buff1, U8 *buff2, U8 *colortable, U8 *TileMap ) ;

#pragma	aux	Do_Fire		"*" 	\
		parm caller []		\
		modify [eax ecx edx]

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------------*/
//				fonctions C (ANIMTEX.CPP)

extern	S32     	NbTexEffects ;

/*--------------------------------------------------------------------------*/
extern	void	ScanTextureAnimation( U8 *ptrtex, S32 nbscanlines ) ;
/*--------------------------------------------------------------------------*/
extern	void	InitTextureAnimation( void ) ;
/*--------------------------------------------------------------------------*/
extern	void	DoTextureAnimation() ;
/*--------------------------------------------------------------------------*/
extern	void	InitPlasmaMenu() ;
/*--------------------------------------------------------------------------*/
extern	void	SelectPlasmaBank( U8 bank ) ;
/*--------------------------------------------------------------------------*/
extern	void	ClearPlasmaMenu( void ) ;
/*--------------------------------------------------------------------------*/
extern	void	FixePlasmaPtrMap( void ) ;
/*--------------------------------------------------------------------------*/

#endif	// ANIMTEX_H
