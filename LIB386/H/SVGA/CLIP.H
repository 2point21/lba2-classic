//--------------------------------------------------------------------------
#ifndef	LIB_SVGA_CLIP
#define	LIB_SVGA_CLIP

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------
extern	S32	ClipXMin;
extern	S32	ClipYMin;
extern	S32	ClipXMax;
extern	S32	ClipYMax;

//--------------------------------------------------------------------------
extern	S32	ClipWindowXMin;
extern	S32	ClipWindowYMin;
extern	S32	ClipWindowXMax;
extern	S32	ClipWindowYMax;

//--------------------------------------------------------------------------
extern	S32	MemoClipXMin;
extern	S32	MemoClipYMin;
extern	S32	MemoClipXMax;
extern	S32	MemoClipYMax;

//--------------------------------------------------------------------------
extern	S32	MemoClipWindowXMin;
extern	S32	MemoClipWindowYMin;
extern	S32	MemoClipWindowXMax;
extern	S32	MemoClipWindowYMax;

//--------------------------------------------------------------------------
void	SetClipWindow(S32 x0, S32 y0, S32 x1, S32 y1)	;

#pragma aux SetClipWindow	"*"	\
	parm caller	[eax] [ebx] [ecx] [edx]

//--------------------------------------------------------------------------
void	UnsetClipWindow()				;

#pragma aux UnsetClipWindow	"*"	\
	modify	[eax ebx]

//--------------------------------------------------------------------------
void	MemoClipWindow()				;

#pragma aux MemoClipWindow	"*"	\
	modify	[eax ebx]

//--------------------------------------------------------------------------
void	RestoreClipWindow()				;

#pragma aux RestoreClipWindow	"*"	\
	modify	[eax ebx ecx edx]

//--------------------------------------------------------------------------
void	SetClip(S32 x0, S32 y0, S32 x1, S32 y1)		;

#pragma aux SetClip		"*"	\
	parm caller	[eax] [ebx] [ecx] [edx]

//--------------------------------------------------------------------------
void	UnsetClip()					;

#pragma aux UnsetClip		"*"	\
	modify	[eax ebx]

//--------------------------------------------------------------------------
void	MemoClip()					;

#pragma aux MemoClip		"*"	\
	modify	[eax ebx]

//--------------------------------------------------------------------------
void	RestoreClip()					;

#pragma aux RestoreClip		"*"	\
	modify	[eax ebx ecx edx]

//--------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_SVGA_CLIP

//--------------------------------------------------------------------------

