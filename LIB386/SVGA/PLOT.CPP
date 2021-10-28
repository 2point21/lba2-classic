//──────────────────────────────────────────────────────────────────────────
#include	<system\adeline.h>
#include	<svga\logphys.h>
#include	<svga\clip.h>
#include	<svga\plot.h>

//──────────────────────────────────────────────────────────────────────────
void	Plot(S32 x0, S32 y0, S32 coul)
{
	if(	(x0 < ClipXMin)	||
		(x0 > ClipXMax)	||
		(y0 < ClipYMin)	||
		(y0 > ClipYMax)		)
	{
		return	;
	}

	((U8*)Log)[y0*ModeDesiredX+x0] = (U8)coul	;
}

//──────────────────────────────────────────────────────────────────────────
S32	GetPlot(S32 x0, S32 y0)
{
	if(	(x0 < ClipXMin)	||
		(x0 > ClipXMax)	||
		(y0 < ClipYMin)	||
		(y0 > ClipYMax)		)
	{
		return	0	;
	}

	return	((U8*)Log)[y0*ModeDesiredX+x0]	;
}

//──────────────────────────────────────────────────────────────────────────
