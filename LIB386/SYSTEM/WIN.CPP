#include "adeline.h"
#include "lib_sys.h"
#include "f:\projet\lib386\lib_svg4\lib_svga.h"
#include "f:\projet\lib386\lib_ail4\lib_ail.h"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define	STYLE_FULLSCREEN	WS_POPUP
#define	STYLE_WINDOW		WS_OVERLAPPEDWINDOW
//#define	STYLE_WINDOW		(WS_OVERLAPPED|WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX)

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#ifndef	SPI_GETWORKAREA
#define	SPI_GETWORKAREA		48
#endif

#ifndef	WM_DISPLAYCHANGE
#define	WM_DISPLAYCHANGE	0x007E
#endif

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

HANDLE		hSem					;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

extern S32	DoubleBuffer				;
extern S32	UseFlip					;

extern U32	ScanLine				;

extern S32	_LibMouseXDep, _LibMouseYDep		;
extern S32	_LibMouseClipX0, _LibMouseClipY0	;
extern S32	_LibMouseClipX1, _LibMouseClipY1	;
extern S32	_LibMouseCenterX, _LibMouseCenterY	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴


static S32	AllowWinMode		= TRUE		;
static S32	LiveWin			= TRUE		;

static S32	Cursor			= 0		;
static HCURSOR	hCursor			= NULL		;

static WINDOWPLACEMENT WinPlace				;
static RECT	WinRect					;
static S32	CurrentWidth		= 0		;
static S32	CurrentHeight		= 0		;
static S32	ModeX			= FALSE		;
static S32	InitDoubleBuffer	= FALSE		;
static S32	WantedDoubleBuffer 	= FALSE		;
static S32	AttachedBack		= FALSE		;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// WAIL

void			(*PauseMiles)()		= NULL	;
void			(*ResumeMiles)()	= NULL	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Windows

static LPDIRECTDRAW		lpDD		= NULL	;
static LPDIRECTDRAWSURFACE	lpDDSPrimary	= NULL	;
static LPDIRECTDRAWSURFACE	lpDDSBack	= NULL	;
static LPDIRECTDRAWCLIPPER	lpClipper	= NULL	;
static LPDIRECTDRAWSURFACE	lpDDS			;

static RECT			DestRect, RcRect	;
static POINT			Pt			;

static char		ErrorStr[256]			;
static S32		ScreenLocked = 0		;

HANDLE 			hInstance		;
HWND			hWnd			;
HDC			DisplayContext		;

LPDIRECTDRAWPALETTE	lpDDPal			;	// DirectDraw Palette
PALETTE			WinPal = { 0x300, 256 }	;	// Palette entries
HPALETTE		HandlePal = 0		;

U32			AppActive	= FALSE	;
U32			AppInitialized	= FALSE	;
//U32			ExitVal		= 0	;
//U32			ExitFlag	= 0	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

//Screen

U32			TabOffLine[480]	;
S32			Screen_X	;
S32			Screen_Y	;
U8			*Log		;
U8			*MemoLog	;
U8			*Phys		;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// Timer

static S32		TimerLock	= 0	;
static S32		TimerSave	= 0	;
static S32		SaveTime	= 0	;
static S32		LastTime	= 0	;
static S32		MemoTime	= 0	;
static S32		LastEvaluate	= 0	;

U32			TimerSystemHR	= 0	;
U32			TimerRefHR	= 0	;
U32			TimerRef	= 0	;
U32			NbFramePerSecond= 0	;
U32			CmptFrame	= 0	;

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// Keyboard

#define	BUFFER_SIZE	20

U32			AsciiMode = FALSE	;
U32			BufferKey[BUFFER_SIZE]	;
U32			StartKey = 0		;
U32			EndKey = 0		;
U32			NextEndKey		;
HANDLE			AsciiMutex		;

U32			Key, Joy, Fire, FuncKey	;

static U32		TabSpecialKey[]=
			{
				// Joy
                                VK_UP           ,
                                VK_NUMPAD8      ,
                                VK_DOWN         ,
                                VK_NUMPAD2      ,
                                VK_LEFT         ,
                                VK_NUMPAD4      ,
                                VK_RIGHT        ,
                                VK_NUMPAD6      ,
                                VK_HOME         ,
                                VK_NUMPAD7      ,
                                VK_PRIOR        ,
                                VK_NUMPAD9      ,
                                VK_NEXT         ,
                                VK_NUMPAD3      ,
                                VK_END          ,
                                VK_NUMPAD1      ,

				//Fire
                                VK_SPACE        ,
                                VK_RETURN       ,
                                VK_CONTROL      ,
                                VK_MENU         ,
                                VK_DELETE       ,
                                VK_SHIFT        ,

				// Function
                                VK_F1           ,
                                VK_F2           ,
                                VK_F3           ,
                                VK_F4           ,
                                VK_F5           ,
                                VK_F6           ,
                                VK_F7           ,
                                VK_F8           ,
                                VK_F9           ,
                                VK_F10          ,
                                VK_F11          ,
                                VK_F12          ,
			};

static U16		TabSpecialFunc[] =
			{
				// Joy
				0, 1,
				0, 1,
				0, 2,
				0, 2,
				0, 4,
				0, 4,
				0, 8,
				0, 8,
				0, 1+4,
				0, 1+4,
				0, 1+8,
				0, 1+8,
				0, 2+8,
				0, 2+8,
				0, 2+4,
				0, 2+4,

				// Fire
				1, 1,
				1, 2,
				1, 4,
				1, 8,
				1, 16,
				1, 32,

				// Function
                                2, 1,
                                2, 2,
                                2, 4,
                                2, 8,
                                2, 16,
                                2, 32,
                                2, 64,
                                2, 128,
                                2, 256,
                                2, 512,
                                2, 1024,
                                2, 2048
			};

void	SpecialKey(S32 key, S32 *type, S32 *bits);

#pragma aux SpecialKey=						\
"               mov     ecx, 34				"       \
"               mov     edi, offset TabSpecialKey       "       \
"               repne   scasd                           "       \
"               jz      special                         "       \
"		mov	ecx, -1				"	\
"		jmp	exitspecial			"	\
"special:						"	\
"               mov     edx, 33				"       \
"               sub     edx, ecx                        "       \
"               mov     edx, dword ptr TabSpecialFunc[edx*4]"	\
"               mov     ecx, edx                        "       \
"               shr     edx, 16                         "       \
"               and     ecx, 0FFFFh                     "       \
"               mov     [esi], edx                      "       \
"exitspecial:   mov     [ebx], ecx                      "       \
                parm caller [eax] [ebx] [esi]                   \
		modify exact [ecx edx edi]

//----------------------------------------------------------------------------
U32	GetAscii()
{
	U32	ret = 0	;


	WaitForSingleObject(AsciiMutex, INFINITE);

	if(EndKey != StartKey)
	{
		ret = BufferKey[StartKey++];
		if(StartKey == BUFFER_SIZE)
		{
			StartKey = 0 ;
		}
	}

        ReleaseMutex(AsciiMutex);

	return ret ;
}

//----------------------------------------------------------------------------

void	ClearAsciiBuffer()
{
	WaitForSingleObject(AsciiMutex, INFINITE);

	StartKey = EndKey = 0	;

        ReleaseMutex(AsciiMutex);
}
//----------------------------------------------------------------------------

void	StyleWin()
{
	DWORD	dwstyle	;

	SetClassLong(hWnd, GCL_HCURSOR, (long)hCursor);

	dwstyle = GetWindowStyle(hWnd)	;
	dwstyle &= ~STYLE_FULLSCREEN	;
	dwstyle |= STYLE_WINDOW		;
	SetWindowLong(	hWnd, GWL_STYLE, dwstyle);

	ShowWindow(hWnd, SW_SHOWNORMAL)	;
}

//----------------------------------------------------------------------------

void	StyleFull()
{
	DWORD	dwstyle	;

	SetClassLong(hWnd, GCL_HCURSOR, (long)NULL);

	dwstyle = GetWindowStyle(hWnd)	;
	dwstyle &= ~STYLE_WINDOW	;
	dwstyle |= STYLE_FULLSCREEN	;
	SetWindowLong(	hWnd, GWL_STYLE, dwstyle);

	ShowWindow(hWnd, SW_SHOWNORMAL)	;
}

//----------------------------------------------------------------------------

void	StyleMinimize()
{
	if(!WinMode)
	{
		StyleWin()		;
	}
	ShowWindow(hWnd, SW_MINIMIZE)	;
	ReleaseObjects()		;
}

//----------------------------------------------------------------------------
void	ApplicationActive()
{
	if(!AppActive)
	{
		UnlockTimer()	;
		AppActive = TRUE;
		Key = 0		;
		if(WinMode)
		{
			ResetPalette()	;
		}
	}
}

//----------------------------------------------------------------------------
void	ApplicationInactive()
{
	if(AppActive)
	{
		LockTimer()			;
		AppActive = FALSE		;
		if(!WinMode&&AppInitialized)
		{
			StyleMinimize()		;
		}
	}
}

//----------------------------------------------------------------------------
U32	CheckBPP()
{
	U32	bpp	;

	// check for 8 bpp
	DisplayContext = GetDC(NULL);
   	bpp = GetDeviceCaps(DisplayContext, PLANES) * GetDeviceCaps(DisplayContext, BITSPIXEL);
   	ReleaseDC(NULL, DisplayContext);

	AllowWinMode = (bpp == 8)	;

	return AllowWinMode		;
}

//----------------------------------------------------------------------------

static long FAR PASCAL WindowProc(	HWND hWnd, UINT message,
			   		WPARAM wParam, LPARAM lParam)
{
	S32		t1, t2;

	switch (message)
	{
	case WM_CREATE:
		MemoTime = timeGetTime()	;
		return 0			;

	case WM_CLOSE:
		// Confirm ?
		StyleMinimize()			;
		DestroyWindow(hWnd)		;
		return 0			;

	case WM_DESTROY:
		PostQuitMessage( 0 )		;
		return 0			;

	case WM_SETFOCUS:
		ApplicationActive()		;
		return 0			;

	case WM_KILLFOCUS:
		ApplicationInactive()		;
		return 0			;

	case WM_DISPLAYCHANGE:
		if(IsIconic(hWnd)&&!AppActive)
		{
			CheckBPP()		;
		}
		else
		{
			InvalidateRect(hWnd, 0, FALSE)	;
		}
		break				;

	case WM_PAINT:
		if(lpDDSPrimary&&RedrawFunc&&(AppActive||WinMode))
		{
			(*RedrawFunc)()		;
			if(!WinMode)
			{
				SetCursor(NULL)	;
			}
		}
		else
		{
			HDC	dc 	;
			RECT	rect	;

			dc = GetDC(hWnd)		;
			SelectObject(dc, GetStockObject(BLACK_BRUSH));
			GetClientRect( hWnd, &rect)	;
			Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		   	ReleaseDC(hWnd, dc)		;
		}
		break;

	case WM_SETCURSOR:
		if(WinMode)
		{
			Cursor = timeGetTime()	;
		}
		else
		{
			SetCursor(NULL)		;
		}
		break;

	case WM_MOUSEMOVE:
		if(!WinMode)
		{
			t1 = LOWORD(lParam)-_LibMouseCenterX;
			t2 = HIWORD(lParam)-_LibMouseCenterY;
			if(!(t1|t2))	break;
			_LibMouseXDep += t1 ;
			_LibMouseYDep += t2 ;
			Mouse_X += t1	;
			Mouse_Y += t2	;
			if(Mouse_X>_LibMouseClipX1)	Mouse_X = _LibMouseClipX1;
			if(Mouse_X<_LibMouseClipX0)	Mouse_X = _LibMouseClipX0;
			if(Mouse_Y>_LibMouseClipY1)	Mouse_Y = _LibMouseClipY1;
			if(Mouse_Y<_LibMouseClipY0)	Mouse_Y = _LibMouseClipY0;
			SetCursorPos(_LibMouseCenterX, _LibMouseCenterY);
		}
		break;

	case WM_LBUTTONDOWN:
		Click |= 1;
		break;

	case WM_RBUTTONDOWN:
		Click |= 2;
		break;

	case WM_LBUTTONUP:
		Click &= ~1;
		break;

	case WM_RBUTTONUP:
		Click &= ~2;
		break;

	case WM_KEYDOWN:
		if(!(lParam&0x40000000))
		{
			Key = wParam ;
			SpecialKey(wParam, &t1, &t2);
			switch(t1)
			{
			case 0:
				Joy |= t2 ;
				break;
			case 1:
				Fire |= t2 ;
				break;
			case 2:
				FuncKey |= t2 ;
				break;
			}
		}

		if(AsciiMode)
		{
			WaitForSingleObject(AsciiMutex, INFINITE);

			NextEndKey = EndKey+1	;
			if (NextEndKey == BUFFER_SIZE)
			{
				NextEndKey = 0 ;
			}

			if(NextEndKey != StartKey)
			{
				BufferKey[EndKey] = wParam ;
				EndKey = NextEndKey	   ;
			}

		        ReleaseMutex(AsciiMutex);
		}
		goto endkey;

		break;

	case WM_KEYUP:
		Key = 0 ;
		SpecialKey(wParam, &t1, &t2);
		switch(t1)
		{
		case 0:
			Joy &= ~t2 ;
			break;
		case 1:
			Fire &= ~t2 ;
			break;
		case 2:
			FuncKey &= ~t2 ;
			break;
		}
		goto endkey;

	case WM_SYSKEYUP:
		if(wParam == VK_RETURN)
		{
			if(WinMode)
			{
				SetMode(CurrentWidth, CurrentHeight, FALSE)	;
			}
			else if (AllowWinMode)
			{
				SetMode(CurrentWidth, CurrentHeight, TRUE)	;
			}
			return 0	;
		}

        case WM_SYSKEYDOWN:
        case WM_SYSCHAR:
endkey:
	        {
			U32	lRet	;
			U32	dw	;
			// Fool DefWindowProc into thinking we do NOT have
			// a system menu.  Otherwise it will try to
			// pop up its own.
			dw = GetWindowLong( hWnd, GWL_STYLE ) 		;
			SetWindowLong( hWnd, GWL_STYLE, dw &~WS_SYSMENU );
			lRet = DefWindowProc( hWnd, message, wParam, lParam );
			SetWindowLong( hWnd, GWL_STYLE, dw )		;
			return lRet ;
	        }

	case WM_SYSCOMMAND:
		switch(wParam&0xFFF0)
		{
		case SC_MINIMIZE:
			StyleMinimize()					;
			return 0					;

		case SC_MAXIMIZE:
			SetMode(CurrentWidth, CurrentHeight, FALSE)	;
			return 0					;

		case SC_RESTORE:
			SetMode(CurrentWidth, CurrentHeight, WinMode)	;
			return 0					;
		}
		break;

	case WM_PALETTECHANGED:
		if( (HWND)wParam == hWnd )
		{
			break ;
		}
	case WM_QUERYNEWPALETTE:
		if(WinMode)
		{
			ResetPalette()					;
			return 1					;
		}
		break ;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------------

void	SetDoubleBuffer(S32 dbuf)
{
	WantedDoubleBuffer = dbuf ;

	if(!(WinMode || ModeX))
	{
		DoubleBuffer = UseFlip = WantedDoubleBuffer && InitDoubleBuffer	;
	}
	else
	{
		UseFlip		= TRUE  ;
		DoubleBuffer 	= FALSE ;
	}

	if(UseFlip)
	{
		lpDDS = lpDDSBack 	;
	}
	else
	{
		lpDDS = lpDDSPrimary 	;
	}
}

//----------------------------------------------------------------------------

static void ReleaseObjects()
{
	// Release old objects
	if(lpDDSPrimary)
	{
		IDirectDrawSurface_Release(lpDDSPrimary);
		lpDDSPrimary	= NULL			;
	}
	if(lpDDSBack)
	{
		if(!AttachedBack)
		{
			IDirectDrawSurface_Release(lpDDSBack);
		}
		lpDDSBack	= NULL			;
		AttachedBack	= FALSE			;
	}
	if(lpClipper)
	{
		IDirectDrawClipper_Release(lpClipper)	;
		lpClipper	= NULL			;
	}

	lpDDS = NULL	;
}

//----------------------------------------------------------------------------

S32	CreateObjects( )
{
	U32		initmodex	;
   	DDSURFACEDESC   ddsd		;
	DDSCAPS		ddscaps		;
   	HRESULT         ddrval		;


	initmodex	 = FALSE	;

   	// setup for primary surface
	InitDoubleBuffer = !WinMode	;

restart:
	// Release old objects
	ReleaseObjects();

retry:
	memset(&ddsd, 0, sizeof(ddsd))	;
   	ddsd.dwSize = sizeof( ddsd )	;
	if(InitDoubleBuffer)
	{
		ddsd.dwFlags =		DDSD_CAPS		|
					DDSD_BACKBUFFERCOUNT	;

		if(initmodex)
		{
			ddsd.ddsCaps.dwCaps =	DDSCAPS_PRIMARYSURFACE	|
						DDSCAPS_FLIP		|
						DDSCAPS_COMPLEX		;
		}
		else
		{
			ddsd.ddsCaps.dwCaps =	DDSCAPS_PRIMARYSURFACE	|
						DDSCAPS_FLIP		|
						DDSCAPS_VIDEOMEMORY	|
						DDSCAPS_COMPLEX		;
		}

		ddsd.dwBackBufferCount = 1			;
	}
	else
	{
	   	ddsd.dwFlags =		DDSD_CAPS 		;
   		ddsd.ddsCaps.dwCaps =	DDSCAPS_PRIMARYSURFACE	;
   	}

	// Create primary
   	ddrval = IDirectDraw_CreateSurface( lpDD, &ddsd, &lpDDSPrimary, NULL );
   	if( ddrval != DD_OK )
   	{
		if(InitDoubleBuffer&&!initmodex)
		{
			InitDoubleBuffer = FALSE;
			goto retry		;
		}
		return FALSE		;
   	}

	IDirectDrawSurface_GetCaps(lpDDSPrimary, &ddscaps)	;
	ModeX = ddscaps.dwCaps & DDSCAPS_MODEX			;

	if(ModeX && !initmodex)
	{
		initmodex	= TRUE	;
		InitDoubleBuffer= TRUE	;
		goto restart		;
	}

	if(WinMode)
	{
		// Check if we are in 8 bits palette mode
		ddsd.dwFlags = DDSD_PIXELFORMAT	;
		ddrval = IDirectDrawSurface_GetSurfaceDesc(lpDDSPrimary, &ddsd);
	   	if( ddrval != DD_OK )
	   	{
			return FALSE;
	   	}
		if(!(ddsd.ddpfPixelFormat.dwFlags&DDPF_PALETTEINDEXED8))
		{
			if(WinMode)
			{
				AllowWinMode = FALSE	;
				return SetMode(CurrentWidth, CurrentHeight, FALSE);
			}
			else
			{
				return FALSE;
			}
		}

		// Create Clipper for primary surface
		ddrval = IDirectDraw_CreateClipper( lpDD, 0, &lpClipper, NULL ) ;
	   	if( ddrval != DD_OK )
	   	{
			return FALSE;
	   	}
	   	ddrval = IDirectDrawClipper_SetHWnd(lpClipper, 0, hWnd);
	   	if( ddrval != DD_OK )
	   	{
			return FALSE;
	   	}
	   	ddrval = IDirectDrawSurface_SetClipper(lpDDSPrimary, lpClipper);
	   	if( ddrval != DD_OK )
	   	{
			return FALSE;
	   	}

		// Create offscreen for winmode
	   	ddsd.dwFlags		=	DDSD_CAPS		|
	   					DDSD_HEIGHT		|
	   					DDSD_WIDTH		;
   		ddsd.ddsCaps.dwCaps	= 	DDSCAPS_OFFSCREENPLAIN	|
   						DDSCAPS_SYSTEMMEMORY	;
		ddsd.dwHeight		= 	CurrentHeight 		;
		ddsd.dwWidth		= 	CurrentWidth 		;

	   	ddrval = IDirectDraw_CreateSurface( lpDD, &ddsd, &lpDDSBack, NULL );
	   	if( ddrval != DD_OK )
	   	{
       			return FALSE;
	   	}
	}
	else	// Fullscreen
	{
		if(InitDoubleBuffer)
		{
			// get associated back buffer
			ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		    	ddrval = IDirectDrawSurface_GetAttachedSurface(	lpDDSPrimary,
		    							&ddscaps,
		    							&lpDDSBack);
		    	if( ddrval != DD_OK )
		    	{
				InitDoubleBuffer= FALSE ;
				goto restart		;
		    	}

			// this IS an attached surface
		   	AttachedBack	= TRUE	;
		}
	}

 	if(!WinMode)
 	{
		// attach palette to primary
		IDirectDrawSurface_SetPalette(lpDDSPrimary, lpDDPal )	;
	}

	SetDoubleBuffer(WantedDoubleBuffer) ;	// reset DoubleBuffer and UseFlip

	return TRUE 	;
}

//----------------------------------------------------------------------------

S32	RestoreObjects()
{
   	HRESULT         ddrval	;

        ddrval = IDirectDrawSurface_Restore(lpDDSPrimary);
   	if( ddrval == DD_OK )
   	{
	   	return TRUE	;
   	}

   	if( ddrval != DDERR_WRONGMODE )
   	{
		exit(1);	// God dam it !!!
   	}

	return CreateObjects();	// Ok then try to rebuild everything
}

//----------------------------------------------------------------------------

void	NormalWin(S32 width, S32 height)
{
	RECT		rcwork	;

	StyleWin()		;

	if(!WinMode)
	{
		SetWindowPlacement(hWnd, &WinPlace)	;
	}

	if(!(WinRect.left-WinRect.right)||!(WinRect.top-WinRect.bottom))
	{
		SetRect(&WinRect, 0, 0, width, height);
		AdjustWindowRectEx(	&WinRect,
					GetWindowStyle(hWnd),
					GetMenu(hWnd) != NULL,
					GetWindowExStyle(hWnd)	);

		SetWindowPos(	hWnd, NULL,
				0, 0,
				WinRect.right-WinRect.left, WinRect.bottom-WinRect.top,
				SWP_NOMOVE   	|
				SWP_NOZORDER	|
				SWP_NOACTIVATE	);
	}

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcwork, 0);
	GetWindowRect(	hWnd, &WinRect)	;

	if(WinRect.left < rcwork.left)	WinRect.left = rcwork.left	;
	if(WinRect.top < rcwork.top)	WinRect.top = rcwork.top	;

	SetWindowPos(	hWnd, NULL,
			WinRect.left, WinRect.top,
			0, 0,
			SWP_NOSIZE	|
			SWP_NOZORDER	|
			SWP_NOACTIVATE	);

	SetWindowPos(	hWnd, HWND_NOTOPMOST,
			0, 0,
			0, 0,
			SWP_NOSIZE	|
			SWP_NOMOVE	|
			SWP_NOACTIVATE	);
}

//----------------------------------------------------------------------------

void	FullScreenWin()
{
	RECT		rc	;

	if(WinMode)
	{
		GetWindowPlacement(hWnd, &WinPlace)	;
	}

	StyleFull()		;

	SetRect(&rc, 0, 0,
		GetSystemMetrics( SM_CXSCREEN ),
   	 	GetSystemMetrics( SM_CYSCREEN )	);

	AdjustWindowRectEx(	&rc,
				GetWindowStyle(hWnd),
				GetMenu(hWnd) != NULL,
				GetWindowExStyle(hWnd)	);

	SetWindowPos(	hWnd, NULL,
			0, 0,
			rc.right-rc.left, rc.bottom-rc.top,
			SWP_NOACTIVATE	);

	SetWindowPos(	hWnd, HWND_TOPMOST,
			0, 0,
			0, 0,
			SWP_NOSIZE	|
			SWP_NOMOVE	|
			SWP_NOACTIVATE	);
}

//----------------------------------------------------------------------------

void	CreateIdentityPalette()
{
	PALETTE	tmppal = { 0x300, 256 }	;	// Palette entries
	S32	i			;

	memset(tmppal.ppe, 0, sizeof(PALETTEENTRY)*256);

	for(i=0; i<256; i++)
	{
		tmppal.ppe[i].peFlags	= PC_NOCOLLAPSE	;
	}
	HandlePal	= CreatePalette((LOGPALETTE *)&tmppal)	;

	DisplayContext	= GetDC(NULL)				;

	if(HandlePal)
	{
		HandlePal = SelectPalette(DisplayContext, HandlePal, FALSE)	;
		RealizePalette(DisplayContext)					;
		HandlePal = SelectPalette(DisplayContext, HandlePal, FALSE)	;
		DeleteObject(HandlePal)						;
		HandlePal = NULL						;
	}

	GetSystemPaletteEntries(DisplayContext, 0, 10, &WinPal.ppe[0])	;
	GetSystemPaletteEntries(DisplayContext, 246, 10, &WinPal.ppe[246]);

	ReleaseDC(NULL, DisplayContext)					;
}

//----------------------------------------------------------------------------

S32	SetMode(S32 width, S32 height, S32 winmode)
{
   	HRESULT         ddrval	;

	if(!lpDDPal)
	{
		return FALSE	;
	}

	LockTimer()		;

	winmode	= winmode && AllowWinMode	;

	if((width!=CurrentWidth)||(height!=CurrentHeight)||(winmode!=WinMode))
	{
		memset(&WinRect, 0, sizeof(WinRect));
	}

restart:

	// Setup depending on winmode / fullscreen
 	if(winmode)	// Winmode
	{
	   	// reset display mode
	   	ddrval = IDirectDraw_RestoreDisplayMode( lpDD );
	   	if( ddrval != DD_OK )
	   	{
   			goto error;
	   	}

	   	// Set normal mode
	   	ddrval = IDirectDraw_SetCooperativeLevel( lpDD, hWnd, DDSCL_NORMAL );
	   	if( ddrval != DD_OK )
	   	{
	   		goto error	;
	   	}

		// check for 8 bpp
	   	if(!CheckBPP)
	   	{
	   		winmode		= FALSE	;
	   		goto restart		;
	   	}

		NormalWin(width, height);
	}
	else		// Fullscreen
 	{
	   	// Get exclusive mode
	   	ddrval = IDirectDraw_SetCooperativeLevel( lpDD, hWnd,
				   	                          DDSCL_EXCLUSIVE
				   	                        | DDSCL_FULLSCREEN
				   	                        | DDSCL_ALLOWMODEX
				   	                        | DDSCL_NOWINDOWCHANGES
				   	                );
	   	if( ddrval != DD_OK )
	   	{
	   		goto error;
	   	}

	        while(1)
		{
		   	ddrval = IDirectDraw_SetDisplayMode(lpDD, width, height, 8);

		   	if(ddrval == DD_OK)
		   	{
				break;
		   	}

		        if( ddrval != DDERR_WASSTILLDRAWING )
	            	{
				goto error;
	            	}
		}

		FullScreenWin()	;
	}


	WinMode = winmode	;

	// Current resolution
	CurrentWidth	= width ;
	CurrentHeight	= height;

	// Reset Blt rect
       	RcRect.left	= 0 			;
       	RcRect.top	= 0 			;
       	RcRect.right	= CurrentWidth-1	;
       	RcRect.bottom	= CurrentHeight-1 	;

	if(!CreateObjects())
	{
		goto error	;
	}

	if(WinMode)
	{
		CreateIdentityPalette();
	}

	ResetPalette()		;

	ShowWindow(hWnd, SW_SHOWNORMAL)	;

	UpdateWindow(hWnd)	;

	SetCursor(NULL)		;
	Cursor = 0		;

	InvalidateRect(hWnd, NULL, FALSE);

	UnlockTimer()		;

	return TRUE 		;
error:
    	exit(1)			;
    	return FALSE 		;
} /* SetMode */

//----------------------------------------------------------------------------

static void TerminateWindow()
{
       	if(hSem)
       	{
       		CloseHandle(hSem);
       		hSem = NULL	;
       	}

	if(LiveWin)
	{
		LiveWin = FALSE		;
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		ManageEvents()		;
	}
}

//----------------------------------------------------------------------------

HRESULT CALLBACK WinModeCallback(LPDDSURFACEDESC pdds, LPVOID lParam)
{
	if(pdds->ddpfPixelFormat.dwRGBBitCount == 8)
	{
		(*ModeCallback)(pdds->dwWidth, pdds->dwHeight, pdds->ddsCaps.dwCaps);
	}

	return DDENUMRET_OK	;
}

//----------------------------------------------------------------------------

BOOL InitWindow( HANDLE hinstance, int nCmdShow, char *title )
{
	S32		width, height 	;
	S32		i, j		;
   	WNDCLASSEX	wc		;
   	HRESULT         ddrval		;
	char		semname[_MAX_PATH];


	hInstance	= hinstance	;

	AsciiMutex	= CreateMutex(NULL, FALSE, NULL);

	if(!AsciiMutex)
	{
		exit(1)	;
	}

	strcpy(semname, title);
	strcat(semname, "AdelineProgramSemaphore");

    	// Create or open a named semaphore.
        hSem = CreateSemaphore(
		NULL,         // Security attributes
         	0,            // Initial count
                1,            // Maximum count
                semname);     // Semaphore name

	// Close handle, and return NULL if existing semaphore opened.
        if (hSem != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
        {
        	CloseHandle(hSem);
                hWnd = FindWindow( AppName, title ) ;
                if (hWnd)
                {
			SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		exit(0);
	}

    	//
	// set up globals
	//

	atexit(TerminateWindow);

	Screen_X = 640 ;
	Screen_Y = 480 ;

	for(i=j=0; i<480; i++, j+=640)
	{
		TabOffLine[i]	= j	;
	}

	LastTime = LastEvaluate = timeGetTime()	;
	ManageTime()				;

   	//
   	// set up and register window class
   	//

	wc.cbSize	= sizeof(wc)			;
   	wc.lpszClassName= AppName			;
   	wc.style	= 0				;
   	wc.cbClsExtra	= 0				;
   	wc.cbWndExtra	= 0				;
   	wc.hInstance	= hInstance			;
   	wc.hIcon	= LoadIcon( hInstance, "APLI_ICON" );
   	wc.hIconSm 	= LoadImage( hInstance,
   				"APLI_ICON",
   				IMAGE_ICON,
		        	GetSystemMetrics(SM_CXSMICON),
                		GetSystemMetrics(SM_CYSMICON),
                        	LR_DEFAULTCOLOR)	;
   	wc.hCursor	= NULL				;
//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1)	;
   	wc.hbrBackground= NULL				;
   	wc.lpszMenuName	= NULL				;
   	wc.lpfnWndProc	= WindowProc			;

	hCursor = LoadCursor(NULL, IDC_ARROW)		;

   	if(!RegisterClassEx( &wc ))
   	{
   		return FALSE	;
   	}

   	/*
   	 * create a window
   	 */

   	hWnd = CreateWindowEx(
   	   	 WS_EX_TOPMOST|WS_EX_APPWINDOW,
   	   	 AppName,
   	   	 title,
   	   	 STYLE_FULLSCREEN,
   	   	 0, 0,
   	   	 GetSystemMetrics( SM_CXSCREEN ),
   	   	 GetSystemMetrics( SM_CYSCREEN ),
   	   	 NULL,
   	   	 NULL,
   	   	 hInstance,
   	   	 NULL );

   	if( !hWnd )
   	{
		exit(1);
   	}

   	ShowWindow( hWnd, nCmdShow )	;
   	UpdateWindow( hWnd )		;

	GetWindowPlacement(hWnd, &WinPlace)	;

	memset(WinPal.ppe, 0, sizeof(PALETTEENTRY)*256);

	for(i=0; i<256; i++)
	{
		WinPal.ppe[i].peFlags	= PC_NOCOLLAPSE	;
	}

   	//
   	// create the main DirectDraw object
   	//
   	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
   	if( ddrval != DD_OK )
   	{
   		goto exit;
   	}

   	// Get exclusive mode
   	ddrval = IDirectDraw_SetCooperativeLevel( lpDD, hWnd,
			   	                          DDSCL_EXCLUSIVE
			   	                        | DDSCL_FULLSCREEN
			   	                        | DDSCL_ALLOWMODEX
			   	                        | DDSCL_NOWINDOWCHANGES
			   	                );
   	if( ddrval != DD_OK )
   	{
   		goto exit;
   	}

	// Scan Available modes
	if(ModeCallback)
	{
		IDirectDraw_EnumDisplayModes(lpDD, 0, NULL, NULL, WinModeCallback);
	}

	// Select mode
	if(ModeSelect)
	{
		(*ModeSelect)(&width, &height);
	}
	else
	{
		width	= 640 ;
		height	= 480 ;
	}

	// Create Palette
	IDirectDraw_CreatePalette(lpDD, DDPCAPS_8BIT | DDPCAPS_INITIALIZE, WinPal.ppe, &lpDDPal, NULL );

	CheckBPP()	;

	SetMode(width, height, WinMode);

	AppInitialized = TRUE	;

	return TRUE;
exit:
        sprintf(ErrorStr, "DirectDraw Error #%d", ddrval&0xFFFF);
   	MessageBox( hWnd, ErrorStr, "INIT FAILURE", MB_OK );
   	DestroyWindow( hWnd );
   	return FALSE;
} /* InitWindow */

void	ManageEvents()
{
	U32	milespaused = FALSE	;
	MSG	msg			;

	do
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				if(LiveWin)
				{
					LiveWin = FALSE	;
					exit(0)		;
				}
				return			;
//				ExitVal  = msg.wParam	;
//				ExitFlag = TRUE		;
			}
			TranslateMessage(&msg)	;
			DispatchMessage(&msg)	;
		}

		if(!AppActive)
		{
			if(PauseMiles&&(!milespaused))
			{
				milespaused	= TRUE	;
				(*PauseMiles)()		;
			}
			WaitMessage()	;
		}

	} while(!AppActive)	;

	if(ResumeMiles&&milespaused)
	{
		(*ResumeMiles)();
	}
}

void	ManageTime()
{
	S32		time	;

	TimerSystemHR = timeGetTime() ;

	if(Cursor)
	{
		if((TimerSystemHR-Cursor) >= 1000)
		{
			SetCursor(NULL)	;
			Cursor = 0	;
		}
	}

	if(!TimerLock)
	{
		TimerRefHR += TimerSystemHR - LastTime 	;
	}

	TimerRef = TimerRefHR / 20 		;

	LastTime = TimerSystemHR		;

	time = TimerSystemHR - LastEvaluate	;

	if(time >= 1000)
	{
		NbFramePerSecond = (CmptFrame*1000 + time/2) / time	;
		CmptFrame	 = 0					;
		LastEvaluate	+= (time/1000)*1000			;
	}
}

void	LockTimer()
{
	ManageTime()	;
	TimerLock++	;
}

void	UnlockTimer()
{
	if(TimerLock)
	{
		ManageTime()	;
		TimerLock--	;
	}
}

void	SaveTimer()
{
	if(!TimerSave++)
	{
		ManageTime()			;
		SaveTime = TimerRefHR		;
	}
}

void	RestoreTimer()
{
	if(TimerSave)
	{
		if(!--TimerSave)
		{
			ManageTime()		;
			TimerRefHR = SaveTime	;
		}
	}
}

void	SetTimerHR(S32 time)
{
	LockTimer()		;
	TimerRefHR = time	;
	UnlockTimer()		;
}

void	LockScreen()
{
	if(!ScreenLocked)
	{
		DDSURFACEDESC	ddsd	;
		HRESULT		ddrval	;

		if(!lpDDS)
		{
			SetMode(CurrentWidth, CurrentHeight, WinMode);
		}

	        ddsd.dwSize = sizeof(ddsd);
        	ddsd.dwFlags = DDSD_PITCH;

	        while( 1 )
	        {
	            ddrval = IDirectDrawSurface_Lock(lpDDS, NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR, NULL);
	            if( ddrval == DD_OK )
	            {
	                break;
	            }
	            if( ddrval == DDERR_SURFACELOST )
	            {
			RestoreObjects();
			continue;
	            }
	            if( ddrval != DDERR_WASSTILLDRAWING )
	            {
			sprintf(ErrorStr, "DirectDraw Lock FAILED (%d)", ddrval&0xFFFF);
			MessageBox( hWnd, ErrorStr, "LOCK ERROR", MB_OK );
	               	exit(1);
	            }
	        }
	        ScanLine = ddsd.lPitch ;
        	Phys = (U8 *)(ddsd.lpSurface);
	}

	ScreenLocked++	;
}

void	UnlockScreen()
{
	if(ScreenLocked&&lpDDS)
	{
		ScreenLocked--	;
      		if(!ScreenLocked)
      		{
      			IDirectDrawSurface_Unlock(lpDDS, NULL);
      		}
	}
}

void	FlipScreen()
{
        HRESULT ddrval	;

	if(!lpDDS)
	{
		return	;
	}

	if(WinMode)
	{
		S32	Redraw	= TRUE	;

		if(IsIconic(hWnd))
		{
			return	;
		}

		GetClientRect( hWnd, &DestRect)		;

		if((DestRect.bottom==0)||(DestRect.right==0))
		{
			return	;
		}

		Pt.x = Pt.y = 0 			;
		ClientToScreen( hWnd, &Pt)		;
		OffsetRect(&DestRect, Pt.x, Pt.y)	;

	        while( 1 )
	        {
	            ddrval = IDirectDrawSurface_Blt(lpDDSPrimary, &DestRect, lpDDSBack, &RcRect, 0, NULL);
	            if( ddrval == DD_OK )
	            {
	                break;
	            }
	            if( ddrval == DDERR_SURFACELOST )
	            {
			RestoreObjects()	;
			if(Redraw&&RedrawFunc)
			{
				Redraw = FALSE	;
				(*RedrawFunc)()	;
				return		;
			}
			continue		;
	            }
	            if( ddrval != DDERR_WASSTILLDRAWING )
	            {
			sprintf(ErrorStr, "DirectDraw FAILED (%d)", ddrval&0xFFFF);
			MessageBox( hWnd, ErrorStr, "BLIT ERROR", MB_OK );
	               	exit(1);
	            }
	        }
	}
	else
	{
	        while( 1 )
	        {
	            ddrval = IDirectDrawSurface_Flip(lpDDSPrimary, NULL, 0);
	            if( ddrval == DD_OK )
	            {
	                break;
	            }
	            if( ddrval == DDERR_SURFACELOST )
	            {
			RestoreObjects();
			continue;
	            }
	            if( ddrval != DDERR_WASSTILLDRAWING )
	            {
			sprintf(ErrorStr, "DirectDraw FAILED (%d)", ddrval&0xFFFF);
			MessageBox( hWnd, ErrorStr, "FLIP ERROR", MB_OK );
	               	exit(1);
	            }
	        }
        }
}

void	WindowSendMessage(U32 message, U32 wParam, U32 lParam)
{
	SendMessage(hWnd, message, wParam, lParam);
}
