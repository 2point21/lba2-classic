#ifndef	INVENT_H
#define	INVENT_H

/*#ifdef	LBA_EDITOR
#define	BODY_SORT_PROTECT    "F:\\projet\\lba2\\graph\\3D\\throw\\protect1.O4D"
#else
#define	BODY_SORT_PROTECT    62	// f:\projet\lba2\graph\3d\objfix.lst
#endif
*/
//--------------------------	Can change

#define	INV_START_X	8	//	Margin inventory screen X
#define	INV_START_Y	8	//	Margin inventory screen Y

#define	INV_NBOBJ_X	7	//	Nb Object X ex 8
#define	INV_NBOBJ_Y	5	//	Nb Object Y

#define	INV_INTER_X	8	//	Distance between 2 objects in X
#define	INV_INTER_Y	8       //	Distance between 2 objects in Y

//--------------------------	Auto change

#define	INV_DELTA_X		(639-(INV_START_X*2))	//	Size Inventory in X
#define	INV_DELTA_Y		(INV_NAME_Y0-(INV_START_Y*2))//            in Y

#define	INV_END_X	(INV_START_X+INV_DELTA_X)
#define	INV_END_Y	(INV_START_Y+INV_DELTA_Y)

#define	INV_SIZEO_X	((INV_DELTA_X-(INV_INTER_X*(INV_NBOBJ_X+1)))/INV_NBOBJ_X)
#define	INV_SIZEO_Y	((INV_DELTA_Y-(INV_INTER_Y*(INV_NBOBJ_Y+1)))/INV_NBOBJ_Y)

//	Before : 317-120, 218-140, 317+120, 218+140

#define	INV_ZOOM_X0	317-120
#define	INV_ZOOM_Y0	210-120
#define	INV_ZOOM_X1	317+120
#define	INV_ZOOM_Y1	210+120

#define	INV_NAME_X0	INV_START_X		//	Name Object X0
#define	INV_NAME_Y0	429			//	Name Object Y0
#define	INV_NAME_X1	(639-INV_START_X)	//	Name Object X1
#define	INV_NAME_Y1	471			//	Name Object Y1

extern	S32	ProtectActif 	;
extern	S32	FlagZoomed	;
extern	S32	InvSelect 	;

/*--------------------------------------------------------------------------*/
extern void InitInventory(void);
/*--------------------------------------------------------------------------*/
extern S32  TricheInventory(S32 ret);
/*--------------------------------------------------------------------------*/
extern void DrawObjInventory(S32 x,S32 y,S16 *ptrobj,S32 beta,S32 lfactor,S32 zoom);
/*--------------------------------------------------------------------------*/
extern void AffValueBar(S32 x0,S32 y0,S32 x1,S32 y1,S32 value);
/*--------------------------------------------------------------------------*/
extern void SaveBackground(S32 x0,S32 y0,S32 x1,S32 y1);
/*--------------------------------------------------------------------------*/
extern void RestoreBackground(void);
/*--------------------------------------------------------------------------*/
extern U32  DrawOneInventory(S32 numbox);
/*--------------------------------------------------------------------------*/
extern void OpenInventory(void);
/*--------------------------------------------------------------------------*/
extern void DrawListInventory(void);
/*--------------------------------------------------------------------------*/
extern void SaveBlockInventory(void);
/*--------------------------------------------------------------------------*/
extern void AffObjName(S32 num);
/*--------------------------------------------------------------------------*/
extern void CheckProtoPack( void ) ;
/*--------------------------------------------------------------------------*/
extern void MenuInventory( U32 tempo );
/*--------------------------------------------------------------------------*/
extern void DoFoundObj(S32 numobj);
/*--------------------------------------------------------------------------*/
extern void DrawArdoiseArrows(void);
/*--------------------------------------------------------------------------*/
extern void RouleauRight(void);
/*--------------------------------------------------------------------------*/
extern void RouleauLeft(void);
/*--------------------------------------------------------------------------*/
extern void ChangeArdoise(S32 left);
/*--------------------------------------------------------------------------*/
extern void DrawArdoise(void);
/*--------------------------------------------------------------------------*/
extern void ResetArdoise( void ) ;
/*--------------------------------------------------------------------------*/
extern void GereArdoise(void);
/*--------------------------------------------------------------------------*/
extern void BackupAngles( S32 x0, S32 y0, S32 x1, S32 y1 ) ;
/*--------------------------------------------------------------------------*/
extern void RestoreAngles( S32 x0, S32 y0, S32 x1, S32 y1 ) ;
/*--------------------------------------------------------------------------*/
extern void DrawAngles( S32 x0, S32 y0, S32 x1, S32 y1, U32 type ) ;
/*--------------------------------------------------------------------------*/
extern void DrawCadre(S32 x0,S32 y0,S32 x1,S32 y1,U32 type );
/*--------------------------------------------------------------------------*/
extern void DrawCadreBox( S32 x0, S32 y0, S32 x1, S32 y1, S32 color ) ;
/*--------------------------------------------------------------------------*/
extern void DrawCadreShade( S32 x0, S32 y0, S32 x1, S32 y1, S32 color ) ;
/*--------------------------------------------------------------------------*/
extern void BackupScreen( S32 flagscreen ) ;
/*--------------------------------------------------------------------------*/
extern void RestoreScreen( void ) ;
/*--------------------------------------------------------------------------*/
extern void ToggleSortProtection( void ) ;
/*--------------------------------------------------------------------------*/
extern void PlayProtectSamples( T_EXTRA *ptrextra ) ;
/*--------------------------------------------------------------------------*/
extern void StopProtectSamples( void ) ;
/*--------------------------------------------------------------------------*/
#endif	// INVENT_H
