#ifndef	COMPORTE_H
#define	COMPORTE_H

/*--------------------------------------------------------------------------*/
extern S32 GetPreviousMenuComp(void);
/*--------------------------------------------------------------------------*/
extern S32 GetNextMenuComp(void);
/*--------------------------------------------------------------------------*/
extern void DrawObjComportement(S32 x0,S32 y0,S32 x1,S32 y1,S32 y3d,S32 betaflag,T_OBJ_3D *ptrobj3d);
/*--------------------------------------------------------------------------*/
extern void DrawComportement(S32 num,S32 comportement,S32 beta,S32 copyblock);
/*--------------------------------------------------------------------------*/
extern void DrawInfoMenu(S32 x0,S32 y0);
/*--------------------------------------------------------------------------*/
extern void DrawMenuComportement(S32 beta);
/*--------------------------------------------------------------------------*/
extern void CalculeDebComportement(T_MENU_COMP *ptrmcomp);
/*--------------------------------------------------------------------------*/
extern void MenuComportement(S32 affone);
/*--------------------------------------------------------------------------*/
extern void IncrustComportement( U8 Comportement ) ;
/*--------------------------------------------------------------------------*/

#endif	// COMPORTE_H
