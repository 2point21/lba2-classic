#ifndef	DECORS_H
#define	DECORS_H

/*--------------------------------------------------------------------------*/
extern void FixeObjetsDecorsInvisibles( void ) ;
/*--------------------------------------------------------------------------*/
extern int sub_qsort_decors(const void *a,const void *b);
/*--------------------------------------------------------------------------*/
extern int sub_qsort_decors_zbuf(const void *a,const void *b);
/*--------------------------------------------------------------------------*/
extern void AffichageObjetDecorsZV(void) ;
/*--------------------------------------------------------------------------*/
extern void AffichageObjetDecorsZBuf(void);
/*--------------------------------------------------------------------------*/
extern S32 TestZVDecors(S32 xw,S32 yw,S32 zw,T_DECORS *objet);

#endif	// DECORS_H
