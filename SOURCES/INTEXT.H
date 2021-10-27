/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*

	ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛÛÛÛÛ¿ ÛÛ¿ ÛÛ¿ ÛÛÛÛÛÛ¿         ÛÛ¿ ÛÛ¿
	ÀÄÛÛÚÄÙ ÛÛÛ¿ÛÛ³ ÀÄÛÛÚÄÙ ÛÛÚÄÄÄÙ ÛÛ³ ÛÛ³ ÀÄÛÛÚÄÙ         ÛÛ³ ÛÛ³
	  ÛÛ³   ÛÛÛÛÛÛ³   ÛÛ³   ÛÛÛÛÛ¿  ÀÛÛÛÛÚÙ   ÛÛ³           ÛÛÛÛÛÛ³
	  ÛÛ³   ÛÛÚÛÛÛ³   ÛÛ³   ÛÛÚÄÄÙ  ÛÛÚÄÛÛ¿   ÛÛ³           ÛÛÚÄÛÛ³
	ÛÛÛÛÛÛ¿ ÛÛ³ÀÛÛ³   ÛÛ³   ÛÛÛÛÛÛ¿ ÛÛ³ ÛÛ³   ÛÛ³     ÛÛ¿   ÛÛ³ ÛÛ³
	ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ   ÀÄÙ   ÀÄÄÄÄÄÙ ÀÄÙ ÀÄÙ   ÀÄÙ     ÀÄÙ   ÀÄÙ ÀÄÙ

	Module commun aux mondes interieurs et exterieurs encapsulant
	certaines fonctions.

 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

extern	S32	SaveVueOffsetX, SaveVueOffsetY, SaveVueOffsetZ ;
extern	S32	SaveAlphaCam, SaveBetaCam, SaveGammaCam ;
extern	S32	SaveVueDistance ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Game
extern	void	PtrInit3DGame() ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Objects
/*--------------------------------------------------------------------------*/
extern	S32	DrawRecover( S32 xw, S32 yw, S32 zw, S32 ymax, S32 zvmax ) ;

/*--------------------------------------------------------------------------*/
extern	void	DrawRecover3( S32 xw, S32 yw, S32 zw, S32 ymax, S32 zvmax ) ;

/*--------------------------------------------------------------------------*/

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Map
extern	void	RefreshGrille() ;
/*--------------------------------------------------------------------------*/
extern	U8	PtrWorldCodeBrick( S32 xw, S32 yw, S32 zw ) ;

/*--------------------------------------------------------------------------*/

#ifdef	LBA_EDITOR
extern	void	RefreshClippedGrille() ;
extern	void	PtrSearchFirstPseudo() ;
extern	S32	PtrGetScreenBrick( S32 xp, S32 yp ) ;
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Graph
/*--------------------------------------------------------------------------*/
extern	void	PtrProjectSprite( S32 xw, S32 yw, S32 zw, S32 sprite ) ;

/*--------------------------------------------------------------------------*/
extern	void	PtrGetDxDyGraph( S32 sprite, S32 *dx, S32 *dy ) ;

/*--------------------------------------------------------------------------*/
extern	void	PtrAffGraph( S32 xp, S32 yp, S32 sprite ) ;

/*--------------------------------------------------------------------------*/
extern	U8	*GetPtrSprite( S32 sprite ) ;

/*--------------------------------------------------------------------------*/

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// World
/*--------------------------------------------------------------------------*/
extern	S32	PtrProjectPoint( S32 xw, S32 yw, S32 zw ) ;

/*--------------------------------------------------------------------------*/
extern	void	CameraCenter( S32 flagbeta ) ;

/*--------------------------------------------------------------------------*/
extern	void	FlyCamera( 	S32 offx, S32 offy, S32 offz,
				S32 alpha, S32 beta, S32 gamma, S32 distance,
				S32 delay ) ;
/*--------------------------------------------------------------------------*/
extern	void	SaveCamera() ;
/*--------------------------------------------------------------------------*/
extern	void	RestoreCamera() ;
/*--------------------------------------------------------------------------*/
extern	S32	PtrCalculAltitudeY( S32 x, S32 z ) ;

/*--------------------------------------------------------------------------*/

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Tools (PERSO)
#ifdef	LBA_EDITOR
extern	void	GereBrickMouse() ;
extern	S32	PtrGereKeys( U32 key, U32 flagmenu ) ;
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Pointeurs de fonctions communs au deux mondes et initialis‚s par PtrInit3DGame
//			(voir PTRFUNC.H)
/*--------------------------------------------------------------------------*/

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_ReajustPos	*PtrReajustPos ;

#pragma	aux	PtrReajustPos	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_GetShadow	*PtrGetShadow ;

#pragma	aux	PtrGetShadow	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_DoAnim	*PtrDoAnim ;

#pragma	aux	PtrDoAnim	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_WorldColBrick	*PtrWorldColBrick ;

#pragma	aux	PtrWorldColBrick	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_WorldColBrickVisible	*PtrWorldColBrickVisible ;

#pragma	aux	PtrWorldColBrickVisible	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_ProjectShadow	*PtrProjectShadow ;

#pragma	aux	PtrFunc_ProjectShadow	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_ReajustPosExtra	*PtrReajustPosExtra ;

#pragma	aux	PtrReajustPosExtra	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_Init3DView	*PtrInit3DView ;

#pragma	aux	PtrInit3DView	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	Func_InitGrille	*PtrInitGrille ;

#pragma	aux	PtrInitGrille	"*"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#ifndef	LBA_EDITOR
extern	Func_FullWorldColBrick	*PtrFullWorldColBrick ;

#pragma	aux	PtrFullWorldColBrick	"*"
#endif	// LBA_EDITOR

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
