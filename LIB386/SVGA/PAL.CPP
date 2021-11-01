//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<svga\ddraw.h>
#include 	<svga\pal.h>

//──────────────────────────────────────────────────────────────────────────
static	S32	ColMin	= 0	;
static	S32	ColMax	= 256	;

	S32	PaletteVbl = FALSE;

//──────────────────────────────────────────────────────────────────────────
void FullPalette()
{
	ColMin = 1	;
	ColMax = 255	;
}

//──────────────────────────────────────────────────────────────────────────
static void ResetPalette()
{
	if(!lpDDPal)	return	;

	if(PaletteVbl)	VblVESA();

	lpDDPal->SetEntries(0, ColMin, ColMax-ColMin, &(WinPal.ppe[ColMin]) );
}

//──────────────────────────────────────────────────────────────────────────
void SetPaletteVESA(void *pal, S32 start, S32 n)
{
	if(start<ColMin)	start = ColMin	;

	for(; n>0; n--)
	{
		WinPal.ppe[start].peRed		= ((U8*)pal)[0];
		WinPal.ppe[start].peGreen	= ((U8*)pal)[1];
		WinPal.ppe[start].peBlue	= ((U8*)pal)[2];
		pal = (void*)((U8*)pal+3) ;
		start++	;
	}

	ResetPalette()	;
}

//──────────────────────────────────────────────────────────────────────────
void SetPaletteOneVESA(S32 col, S32 r, S32 g, S32 b)
{
	if(!lpDDPal||(col<ColMin)||(col>=ColMax))
	{
		return	;
	}

	WinPal.ppe[col].peRed	= (U8)r;
	WinPal.ppe[col].peGreen	= (U8)g;
	WinPal.ppe[col].peBlue	= (U8)b;

	if(PaletteVbl)	VblVESA()	;

	lpDDPal->SetEntries(0, col, 1, &(WinPal.ppe[col]) );
}

//──────────────────────────────────────────────────────────────────────────
