#ifndef	BEZIER_H
#define	BEZIER_H

#define ALTITUDE_MAX	200

#define FTOW(x) (S16)(((x)+32767) >> 16)
#define WTOF(x) (S32)((S32)(x) << 16)

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	S32	(Func_ProjectShadow)( T_SORT *,S32 ) ;

#pragma	aux	Arg_ProjectShadow	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_ProjectShadow)	Func_ProjectShadow

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß


extern	S32	XS0,YS0,XS1,YS1,XS2,YS2,XS3,YS3 ;
extern	S32	ShadowTestObject ;

/*--------------------------------------------------------------------------*/
void 	CalculeLigneOmbre( S32 x1, S32 y1, S32 x2, S32 y2 ) ;
/*--------------------------------------------------------------------------*/
void	DrawOmbre(S32 x0,S32 y0,S32 x1,S32 y1,S32 x2,S32 y2,S32 x3,S32 y3) ;
/*--------------------------------------------------------------------------*/
void	DrawLineShade(	S32 x0, S32 z0, S32 x1, S32 z1, S32 x2, S32 z2, S32 x3, S32 z3 ) ;
/*--------------------------------------------------------------------------*/
S32	TestZVDecorsXZFull( S32 xw, S32 zw, S32 y0, S32 y1, T_DECORS *objet ) ;
/*--------------------------------------------------------------------------*/
S32	TestZVObjectsXZFull( S32 xw, S32 zw, S32 y0, S32 y1, T_OBJET *objet ) ;
/*--------------------------------------------------------------------------*/
S32	AdjustShadowDecors( S32 xw, S32 zw, S32 y0, S32 y1 ) ;
/*--------------------------------------------------------------------------*/
S32	AdjustShadowObjects( S32 xw, S32 zw, S32 y0, S32 y1 ) ;
/*--------------------------------------------------------------------------*/
extern	Func_ProjectShadow	ProjectShadowExt ;
/*--------------------------------------------------------------------------*/
void	DrawShadow( void ) ;
/*--------------------------------------------------------------------------*/

#endif	// BEZIER_H
