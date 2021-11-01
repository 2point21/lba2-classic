//──────────────────────────────────────────────────────────────────────────
#define	INITGUID

//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<system\logprint.h>
#include	<system\winsys.h>
#include	<svga\logphys.h>
#include	<svga\initmode.h>
#include	<svga\dirtybox.h>
#include	<svga\ddraw.h>

//──────────────────────────────────────────────────────────────────────────
LPDIRECTDRAWSURFACE		lpDDS	= NULL	;

//──────────────────────────────────────────────────────────────────────────
LPDIRECTDRAWPALETTE		lpDDPal	= NULL	;

PALETTE				WinPal = { 0x300, 256 }	; // Palette entries

//──────────────────────────────────────────────────────────────────────────
static	LPDIRECTDRAW2		lpDD2	= NULL	;

//──────────────────────────────────────────────────────────────────────────
static	WINDOW_PROC_PTR		OldDdrawWindowProc;

//──────────────────────────────────────────────────────────────────────────
WINDOW_PROC	DdrawWindowProc	;

//──────────────────────────────────────────────────────────────────────────
static S32	ScreenLocked = 0;

//──────────────────────────────────────────────────────────────────────────
S32	DdrawWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_SETFOCUS:
		if(!AppActive)
		{
			BoxStaticAdd(0, 0, ModeDesiredX-1, ModeDesiredY-1) ;
			BoxCleanClip = TRUE	;
		}
		break		;
	}

	return	OldDdrawWindowProc(hWnd, message, wParam, lParam) ;
}

//──────────────────────────────────────────────────────────────────────────
void RestoreObjects()
{
   	HRESULT         ddrval	;

        ddrval = lpDDS->Restore();
   	if( ddrval != DD_OK )
   	{
	   	exit(1)		;
   	}

	ClearPrimary()		;
}

//──────────────────────────────────────────────────────────────────────────
static void ReleaseObjects()
{
	if(lpDDPal)
	{
		lpDDPal->Release()	;
		lpDDPal = NULL		;
	}
	if(lpDDS)
	{
		lpDDS->Release()	;
		lpDDS = NULL		;
	}
}

//──────────────────────────────────────────────────────────────────────────
static HRESULT CALLBACK WinModeCallback(LPDDSURFACEDESC pdds, LPVOID lParam)
{
	((MODE_CALLBACK *)lParam)(pdds->dwWidth, pdds->dwHeight, pdds->ddpfPixelFormat.dwRGBBitCount, 2, 0);
	return DDENUMRET_OK	;
}

//──────────────────────────────────────────────────────────────────────────
void	ScanVESAModes(MODE_CALLBACK *modecallback)
{
	lpDD2->EnumDisplayModes(0, NULL, (LPVOID)modecallback, WinModeCallback);
}

//──────────────────────────────────────────────────────────────────────────
S32	InitVESA()
{
	LPDIRECTDRAW	lpDD		;
   	HRESULT         ddrval		;

	// check if already initialized
	if(lpDD2)	return	TRUE	;

   	// Create DDraw object (through interface)
   	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
   	if( ddrval != DD_OK )
   	{
		return FALSE	;
   	}

	// Get DDraw2 Interface and release DDraw Interface
	ddrval = lpDD->QueryInterface(IID_IDirectDraw2, (LPVOID *)&lpDD2);
	lpDD->Release()	;
   	if( ddrval != S_OK )
   	{
		return FALSE	;
   	}

   	// Get exclusive mode
   	ddrval = lpDD2->SetCooperativeLevel( 	GetActiveWindow(),
		   	                          DDSCL_EXCLUSIVE
		   	                        | DDSCL_ALLOWREBOOT
		   	                        | DDSCL_FULLSCREEN
		   	                   );
   	if( ddrval != DD_OK )
   	{
		lpDD2->Release();
		lpDD2 = NULL	;
		return FALSE	;
   	}

	OldDdrawWindowProc = WindowProc	;
	WindowProc = DdrawWindowProc	;

   	return	TRUE	;
}

//──────────────────────────────────────────────────────────────────────────
void	EndVESA()
{
	// check if initialized
	if(!lpDD2)	return	;

	// Release objects
	ReleaseObjects()	;

   	// Back to normal mode
	lpDD2->SetCooperativeLevel( 	GetActiveWindow(),
	   	                        DDSCL_NORMAL
		                  );

	// Release DDraw2 object
	lpDD2->Release()	;
	lpDD2 = NULL		;
}

//──────────────────────────────────────────────────────────────────────────
static HRESULT CALLBACK DetectVESAMode(LPDDSURFACEDESC pdds, LPVOID lParam)
{
	lParam = lParam	;

	if(	(ModeDesiredX<=pdds->dwWidth)				&&
		(ModeDesiredY<=pdds->dwHeight)				&&
		(ModeDesiredDepth==pdds->ddpfPixelFormat.dwRGBBitCount)	&&
		(pdds->dwWidth*pdds->dwHeight<(unsigned)ModeResX*(unsigned)ModeResY)	)
	{
		ModeResX	= pdds->dwWidth	;
		ModeResY	= pdds->dwHeight;
		BytesScanLine	= pdds->lPitch	;
	}

	return DDENUMRET_OK	;
}

//──────────────────────────────────────────────────────────────────────────
static void CreateIdentityPalette()
{
	PALETTE	tmppal = { 0x300, 256 }	;	// Palette entries
	HDC	dc			;
	HANDLE	hpal			;
	S32	i			;

	memset(tmppal.ppe, 0, sizeof(PALETTEENTRY)*256);

	for(i=0; i<256; i++)
	{
		tmppal.ppe[i].peFlags	= PC_NOCOLLAPSE	;
	}

	hpal	= CreatePalette((LOGPALETTE *)&tmppal)	;

	dc	= GetDC(NULL)				;

	if(hpal)
	{
		hpal = SelectPalette(dc, hpal, FALSE)	;
		RealizePalette(dc)			;
		hpal = SelectPalette(dc, hpal, FALSE)	;
		DeleteObject(hpal)			;
	}

	GetSystemPaletteEntries(dc, 0, 10, &WinPal.ppe[0])	;
	GetSystemPaletteEntries(dc, 246, 10, &WinPal.ppe[246])	;

	ReleaseDC(NULL, dc)	;
}

//──────────────────────────────────────────────────────────────────────────
static void ResetPalette()
{
	S32	i ;

	memset(WinPal.ppe, 0, sizeof(PALETTEENTRY)*256);

	for(i=0; i<256; i++)
	{
		WinPal.ppe[i].peFlags	= PC_NOCOLLAPSE	;
	}
}

//──────────────────────────────────────────────────────────────────────────
S32	DetectInitVESAMode(U32 resX, U32 resY, U32 depth, U32 memory)
{
   	DDSURFACEDESC   ddsd			;
   	DDCAPS   	ddcapshal, ddcapshel	;
   	HRESULT         ddrval			;

	if(!lpDD2 && !InitVESA())
	{
		return	FALSE		;
	}

	if(memory == 1)	return	FALSE	;

	ModeResX		= 40000	;
	ModeResY		= 40000	;

	ModeDesiredX		= resX	;
	ModeDesiredY		= resY	;
	ModeDesiredDepth	= depth	;

	// Release all Objects
	ReleaseObjects()		;

	// get best video mode
	ddrval = lpDD2->EnumDisplayModes(0, NULL, NULL, DetectVESAMode);
   	if( ddrval != DD_OK )
   	{
		return FALSE	;
   	}
	if(ModeResY==40000)
	{
		return FALSE	;
	}

	// set video mode
	ddrval = lpDD2->SetDisplayMode(ModeResX, ModeResY, ModeDesiredDepth, 0, 0);
   	if( ddrval != DD_OK )
   	{
		return FALSE	;
   	}

	// Create surface
	memset(&ddsd, 0, sizeof(ddsd))			;
   	ddsd.dwSize 		= sizeof( ddsd )	;
   	ddsd.dwFlags		= DDSD_CAPS 		;
	ddsd.ddsCaps.dwCaps 	= DDSCAPS_PRIMARYSURFACE;
   	ddrval = lpDD2->CreateSurface(&ddsd, &lpDDS, NULL );
   	if( ddrval != DD_OK )
   	{
error:		ReleaseObjects()		;
		lpDD2->RestoreDisplayMode()	;
		return FALSE			;
   	}

	// clear surface
	ClearPrimary()		;

	// Get surface caps
	memset(&ddcapshal, 0, sizeof(ddcapshal))	;
   	ddcapshal.dwSize 	= sizeof( ddcapshal )	;
	memset(&ddcapshel, 0, sizeof(ddcapshel))	;
   	ddcapshel.dwSize 	= sizeof( ddcapshel )	;
   	lpDD2->GetCaps(&ddcapshal, &ddcapshel)		;


	// set flag for linear/bank-switched
   	ModeMemory = ddcapshal.dwCaps & DDCAPS_BANKSWITCHED ? 1 : 2	;

	// Create Palette if in 256 colors
	if(ModeDesiredDepth == 8)
	{
		ResetPalette()		;	// clear pal
		CreateIdentityPalette()	;	// get system colors

		// create pal
		ddrval = lpDD2->CreatePalette(DDPCAPS_8BIT|DDPCAPS_ALLOW256, WinPal.ppe, &lpDDPal, NULL );
	   	if( ddrval != DD_OK )
	   	{
			goto error	;
	   	}

		// attach to primary
		ddrval = lpDDS->SetPalette(lpDDPal );
	   	if( ddrval != DD_OK )
	   	{
			goto error	;
	   	}
	}

	return TRUE	;
}

//──────────────────────────────────────────────────────────────────────────
void LockPrimary()
{
	if(!ScreenLocked)
	{
		DDSURFACEDESC	ddsd	;
		HRESULT		ddrval	;

	        ddsd.dwSize = sizeof(ddsd);
        	ddsd.dwFlags = 0	;

	        while( TRUE )
	        {
			ddrval = lpDDS->Lock(NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR, NULL);
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
			    LogPrintf("DirectDraw Lock FAILED (%d)", ddrval&0xFFFF);
			    exit(1);
			}
	        }

	        Phys = ddsd.lpSurface	;

	        ScreenLocked++		;
	}
}

//──────────────────────────────────────────────────────────────────────────
void UnlockPrimary()
{
	if(ScreenLocked&&!--ScreenLocked)
	{
		lpDDS->Unlock(NULL)	;
	}
}

//──────────────────────────────────────────────────────────────────────────
void	ClearPrimary()
{
	S32	i, w	;
	U8	*ptr	;

   	LockPrimary()	;

	w = ModeResX*ModeDesiredDepth/8	;

   	ptr = (U8*)Phys			;

	for(i=ModeResY; i>0; i--)
	{
	   	memset((void*)ptr, 0, w);
	   	ptr += w		;

	}

   	UnlockPrimary()	;
}

//──────────────────────────────────────────────────────────────────────────
void	VblVESA()
{
	while(lpDD2->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL) == DDERR_WASSTILLDRAWING)	;
}

//──────────────────────────────────────────────────────────────────────────
