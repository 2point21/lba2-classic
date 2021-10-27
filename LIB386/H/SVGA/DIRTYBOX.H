//--------------------------------------------------------------------------
#ifndef LIB_SVGA_DIRTYBOX
#define LIB_SVGA_DIRTYBOX

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//-----------------------------------------------------------------------------
// Init:
//
// BoxInit( ... );
//
// processing loop:
//
// for( ...
// {
//	BoxMovingAdd( ... ) ;
//	AND/OR
//	BoxStaticAdd( ... ) ;
// }
//
// BoxUpdate() ;
//

//-----------------------------------------------------------------------------
#pragma pack(push, 1)

//-----------------------------------------------------------------------------
typedef struct	STR_T_BOX
	{
		S16			x0	;
		S16			y0	;
		S16			x1	;
		S16			y1	;
		struct STR_T_BOX	*pBoxNext;
	}	T_BOX	;

//-----------------------------------------------------------------------------
#pragma pack(pop)

//-----------------------------------------------------------------------------
typedef void (BOX_FUNC)(T_BOX *pbox)	;

#pragma aux	ARG_BOX_FUNC		\
	parm caller	[ebx]		\
	modify	[eax ecx edx esi edi]

#pragma aux	(ARG_BOX_FUNC)	BOX_FUNC

//-----------------------------------------------------------------------------
extern S32	BoxClipXMin, BoxClipXMax,
		BoxClipYMin, BoxClipYMax	;

//-----------------------------------------------------------------------------
extern		S32	BoxCleanClip		;

//-----------------------------------------------------------------------------
extern		void	*GphSprite			;
extern		S32	DefSprite			;

//-----------------------------------------------------------------------------
extern BOX_FUNC  *BoxOneBlit, *BoxOneClean			;
extern VOID_FUNC *BoxScreenGet, *BoxScreenRelease, *BoxScreenFlip;

//-----------------------------------------------------------------------------
extern BOX_FUNC DefaultBoxOneBlit	;
extern BOX_FUNC DefaultBoxOneClean	;

//-----------------------------------------------------------------------------
extern BOX_FUNC DefaultBoxOneClear	;

//-----------------------------------------------------------------------------
#ifdef	_WIN32
#define SetBoxScreenGet(boxscreenget)		\
	BoxScreenGet = ( (boxscreenget) ? (boxscreenget) : LockPrimary )

#else// _WIN32
#define SetBoxScreenGet(boxscreenget)		\
	BoxScreenGet = (boxscreenget)

#endif//_WIN32

//-----------------------------------------------------------------------------
#ifdef	_WIN32
#define SetBoxScreenRelease(boxscreenrelease)	\
	BoxScreenRelease = ( (boxscreenrelease) ? (boxscreenrelease) : UnlockPrimary )

#else// _WIN32
#define SetBoxScreenRelease(boxscreenrelease)	\
	BoxScreenRelease = (boxscreenrelease)

#endif//_WIN32

//-----------------------------------------------------------------------------
#define SetBoxScreenFlip(boxscreenflip)	\
	BoxScreenFlip = (boxscreenflip)

//-----------------------------------------------------------------------------
#define SetBoxOneBlit(boxoneblit)		\
	BoxOneBlit = ( (boxoneblit) ? (boxoneblit) : DefaultBoxOneBlit )

//-----------------------------------------------------------------------------
#define SetBoxOneClean(boxoneclean)		\
	BoxOneClean = ( (boxoneclean) ? (boxoneclean) : DefaultBoxOneClean )

//-----------------------------------------------------------------------------
extern S32 	BoxInit( S32 nbBox )	;

//-----------------------------------------------------------------------------
extern void	BoxMovingAdd( S32 x0, S32 y0, S32 x1, S32 y1 );

#pragma aux BoxMovingAdd		\
	parm caller	[eax] [ebx] [ecx] [edx]

//-----------------------------------------------------------------------------
extern void	BoxStaticAdd( S32 x0, S32 y0, S32 x1, S32 y1 );

#pragma aux BoxStaticAdd		\
	parm caller	[eax] [ebx] [ecx] [edx]

//-----------------------------------------------------------------------------
extern void	BoxUnstaticAdd( S32 x0, S32 y0, S32 x1, S32 y1 );

#pragma aux BoxUnstaticAdd		\
	parm caller	[eax] [ebx] [ecx] [edx]

//-----------------------------------------------------------------------------
extern void	BoxCls( S32 x0, S32 y0, S32 x1, S32 y1 );

#pragma aux BoxCls			\
	parm caller	[eax] [ebx] [ecx] [edx]

//-----------------------------------------------------------------------------
#define 	Cls()			\
		BoxCls(BoxClipXMin, BoxClipYMin, BoxClipXMax-1, BoxClipYMax-1)

//-----------------------------------------------------------------------------
extern void	BoxStaticFullflip();

#define Flip	BoxStaticFullflip

//-----------------------------------------------------------------------------
extern void	BoxBlitStaticListToScreen();

//-----------------------------------------------------------------------------
extern void	BoxBlit();

//-----------------------------------------------------------------------------
extern void	BoxClean();

//-----------------------------------------------------------------------------
extern void	BoxUpdate();

//-----------------------------------------------------------------------------
extern void	BoxReset();

//-----------------------------------------------------------------------------
extern void	BoxChangeClip(S32 x0, S32 y0, S32 x1, S32 y1, S32 clean);

//-----------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//-----------------------------------------------------------------------------
#endif//LIB_SVGA_DIRTYBOX

//-----------------------------------------------------------------------------

