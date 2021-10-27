//--------------------------------------------------------------------------
#ifdef	_WIN32

//--------------------------------------------------------------------------
#ifndef	LIB_SVGA_DDRAW
#define	LIB_SVGA_DDRAW

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------
typedef struct
        {
                WORD            Version         ;
                WORD            NumberOfEntries ;
                PALETTEENTRY    ppe[256]        ;
        }       PALETTE ;

//--------------------------------------------------------------------------
#define			VESA_WINDOWED		1
#define			VESA_LINEAR		2
#define			VESA_LINEAR_OR_WINDOWED	(VESA_LINEAR|VESA_WINDOWED)

//--------------------------------------------------------------------------
extern	LPDIRECTDRAWSURFACE	lpDDS	;

//--------------------------------------------------------------------------
extern	PALETTE			WinPal	;
extern	LPDIRECTDRAWPALETTE	lpDDPal	;

//--------------------------------------------------------------------------
typedef	void	(MODE_CALLBACK)(S32 resx, S32 resy, S32 depth, S32 attrib, S32 pages);

// Attrib = VESA_WINDOWED / VESA_LINEAR / VESA_LINEAR_OR_WINDOWED

//--------------------------------------------------------------------------
extern	void 	RestoreObjects()	;

//--------------------------------------------------------------------------
extern	S32	InitVESA()		;

//--------------------------------------------------------------------------
extern	void	EndVESA()		;

//--------------------------------------------------------------------------
extern	void	ScanVESAModes(MODE_CALLBACK *callback)	;

// Will scan all available modes and call callback with info on each mode

//--------------------------------------------------------------------------
extern	S32	DetectInitVESAMode(U32 ResX, U32 ResY, U32 Depth, U32 Memory);

// Will find the best (lowest) resolution containing the required res
// with same Depth. Memory indicate if linear, windowed, or don't care
// ( VESA_WINDOWED / VESA_LINEAR / VESA_LINEAR_OR_WINDOWED )

//--------------------------------------------------------------------------
extern	VOID_FUNC	LockPrimary	;

//--------------------------------------------------------------------------
extern	VOID_FUNC	UnlockPrimary	;

//--------------------------------------------------------------------------
extern	void 	ClearPrimary()		;

//--------------------------------------------------------------------------
extern	void	VblVESA()		;

//--------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_SVGA_DDRAW

//--------------------------------------------------------------------------
#endif//_WIN32

//--------------------------------------------------------------------------
