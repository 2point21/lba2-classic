#ifndef	EXTRA_H
#define	EXTRA_H

/*--------------------------------------------------------------------------*/
extern void InitFly(T_EXTRA *ptrextra,S32 alpha,S32 beta,S32 vitesse,U8 poids);
/*--------------------------------------------------------------------------*/
extern void BounceExtra(T_EXTRA *ptrextra,S32 oldx,S32 oldy,S32 oldz);
/*--------------------------------------------------------------------------*/
extern S32  ExtraBonus(S32 x,S32 y,S32 z,S32 alpha,S32 beta,S16 num,S16 nb);
/*--------------------------------------------------------------------------*/
extern S32  ExtraLabyrinthe(S32 x,S32 y,S32 z);
/*--------------------------------------------------------------------------*/
extern void MagicBallExplo(S32 x,S32 y,S32 z,S32 beta);
/*--------------------------------------------------------------------------*/
extern S32  SearchBonusKey(void);
/*--------------------------------------------------------------------------*/
extern S32  ExtraSearch(U8 owner,S32 x,S32 y,S32 z,S16 num,U8 numobj,S32 speed,U8 hitforce);
/*--------------------------------------------------------------------------*/
extern S32  ExtraSearchKey(U8 owner,S32 x,S32 y,S32 z,S16 num,U8 numobj);
/*--------------------------------------------------------------------------*/
extern void GetExtraZV(T_EXTRA *ptrextra,S32 *xmin,S32 *ymin,S32 *zmin,S32 *xmax,S32 *ymax,S32 *zmax);
/*--------------------------------------------------------------------------*/
extern U8   ExtraCheckObjCol(T_EXTRA *ptrextra,U8 owner);
/*--------------------------------------------------------------------------*/
extern U8   ExtraCheckExtraCol(T_EXTRA *ptrextra,S32 oldx, S32 oldy, S32 oldz,U8 owner);
/*--------------------------------------------------------------------------*/
extern Func_FullWorldColBrick FullWorldColBrick ;
/*--------------------------------------------------------------------------*/
extern void ClearScreenMinMax(void);
/*--------------------------------------------------------------------------*/
extern void AdjustScreenMinMax(void);
/*--------------------------------------------------------------------------*/
extern void Aff2DShape(S16 *ptr,S32 xs,S32 ys,S32 coul,S32 angle,S32 zoom);
/*--------------------------------------------------------------------------*/
extern void AffSpecial(S32 numextra);
/*--------------------------------------------------------------------------*/
extern S32 InitExtraPof( S32 x, S32 y, S32 z, S16 num, S32 alpha, S32 beta, S32 speed, S32 poids, S32 scaledeb, S32 scalefin, S32 duree, S32 rotation, S32 timeout ) ;
/*--------------------------------------------------------------------------*/
extern S32 InitExtraAnimSprite( U8 owner, S32 x, S32 y, S32 z, S16 deb, S16 fin, S32 tempo, S32 scale, S32 transparent, U8 hitforce ) ;
/*--------------------------------------------------------------------------*/
extern S16  CoulRetourBalle(T_EXTRA *ptrextra);
/*--------------------------------------------------------------------------*/
extern void ClearExtra(void);
/*--------------------------------------------------------------------------*/
extern S32  ThrowExtra(U8 owner,S32 x,S32 y,S32 z,S16 num,S32 alpha,S32 beta,S32 vitesse,U8 poids,U8 hitforce);
/*--------------------------------------------------------------------------*/
extern S32  ThrowExtraObj(U8 owner,S32 x,S32 y,S32 z,S32 indexfile3d,S32 alpha,S32 beta,S32 vitesse,S16 alpharot,U8 poids,U8 hitforce);
/*--------------------------------------------------------------------------*/
extern void ThrowExtraFoudre( U8 owner ) ;
/*--------------------------------------------------------------------------*/
extern void InvoqueFoudre( U8 owner, S32 x, S32 y, S32 z ) ;
/*--------------------------------------------------------------------------*/
extern void ThrowExtraProtection( U8 owner ) ;
/*--------------------------------------------------------------------------*/
extern void AnnuleExtraProtection( void ) ;
/*--------------------------------------------------------------------------*/
extern U8   WhichBonus( S32 flags ) ;
/*--------------------------------------------------------------------------*/
extern void GiveExtraBonus(T_OBJET *ptrobj);
/*--------------------------------------------------------------------------*/
extern void ZoneGiveExtraBonus(T_ZONE *ptrz);
/*--------------------------------------------------------------------------*/
extern S32  PlusProcheExtra( S32 x, S32 z, S32 flags ) ;
/*--------------------------------------------------------------------------*/
// Passe par un pointeur de fonction, donc ne peut pas etre en passage par
// registres
extern Func_ReajustPosExtra ReajustPosExtra ;
/*--------------------------------------------------------------------------*/
extern void InitBackMagicBall(T_EXTRA *ptrextra,S16 num,S32 flags);
/*--------------------------------------------------------------------------*/
extern void GereExtras(void);
/*--------------------------------------------------------------------------*/

#endif	// EXTRA_H
