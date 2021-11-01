/*══════════════════════════════════════════════════════════════════════════*

	██████┐ ██┐ ██┐ ██████┐ ██████┐ ██┐ ██┐ ██████┐         ██┐ ██┐
	└─██┌─┘ ███┐██│ └─██┌─┘ ██┌───┘ ██│ ██│ └─██┌─┘         ██│ ██│
	  ██│   ██████│   ██│   █████┐  └████┌┘   ██│           ██████│
	  ██│   ██┌███│   ██│   ██┌──┘  ██┌─██┐   ██│           ██┌─██│
	██████┐ ██│└██│   ██│   ██████┐ ██│ ██│   ██│     ██┐   ██│ ██│
	└─────┘ └─┘ └─┘   └─┘   └─────┘ └─┘ └─┘   └─┘     └─┘   └─┘ └─┘

	Module commun aux mondes interieurs et exterieurs encapsulant
	certaines fonctions.

 *══════════════════════════════════════════════════════════════════════════*/

extern	S32	SaveVueOffsetX, SaveVueOffsetY, SaveVueOffsetZ ;
extern	S32	SaveAlphaCam, SaveBetaCam, SaveGammaCam ;
extern	S32	SaveVueDistance ;

/*──────────────────────────────────────────────────────────────────────────*/
// Game
extern	void	PtrInit3DGame() ;

/*──────────────────────────────────────────────────────────────────────────*/
// Objects
/*--------------------------------------------------------------------------*/
extern	S32	DrawRecover( S32 xw, S32 yw, S32 zw, S32 ymax, S32 zvmax ) ;

/*--------------------------------------------------------------------------*/
extern	void	DrawRecover3( S32 xw, S32 yw, S32 zw, S32 ymax, S32 zvmax ) ;

/*--------------------------------------------------------------------------*/

/*──────────────────────────────────────────────────────────────────────────*/
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

/*──────────────────────────────────────────────────────────────────────────*/
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

/*──────────────────────────────────────────────────────────────────────────*/
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

/*──────────────────────────────────────────────────────────────────────────*/
// Tools (PERSO)
#ifdef	LBA_EDITOR
extern	void	GereBrickMouse() ;
extern	S32	PtrGereKeys( U32 key, U32 flagmenu ) ;
#endif

/*──────────────────────────────────────────────────────────────────────────*/
// Pointeurs de fonctions communs au deux mondes et initialisés par PtrInit3DGame
//			(voir PTRFUNC.H)
/*--------------------------------------------------------------------------*/

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_ReajustPos	*PtrReajustPos ;

#pragma	aux	PtrReajustPos	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_GetShadow	*PtrGetShadow ;

#pragma	aux	PtrGetShadow	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_DoAnim	*PtrDoAnim ;

#pragma	aux	PtrDoAnim	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_WorldColBrick	*PtrWorldColBrick ;

#pragma	aux	PtrWorldColBrick	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_WorldColBrickVisible	*PtrWorldColBrickVisible ;

#pragma	aux	PtrWorldColBrickVisible	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_ProjectShadow	*PtrProjectShadow ;

#pragma	aux	PtrFunc_ProjectShadow	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_ReajustPosExtra	*PtrReajustPosExtra ;

#pragma	aux	PtrReajustPosExtra	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_Init3DView	*PtrInit3DView ;

#pragma	aux	PtrInit3DView	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	Func_InitGrille	*PtrInitGrille ;

#pragma	aux	PtrInitGrille	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
#ifndef	LBA_EDITOR
extern	Func_FullWorldColBrick	*PtrFullWorldColBrick ;

#pragma	aux	PtrFullWorldColBrick	"*"
#endif	// LBA_EDITOR

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

/*══════════════════════════════════════════════════════════════════════════*/
