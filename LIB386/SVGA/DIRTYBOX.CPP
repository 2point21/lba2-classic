//****************************************************************************
#include	<system\adeline.h>
#include	<system\cpu.h>
#include	<system\a_malloc.h>
#include	<svga\screenxy.h>
#include	<svga\logphys.h>
#include	<svga\clrbox.h>
#include	<svga\copybox.h>
#include	<svga\savblock.h>
#include	<svga\resblock.h>
#include	<svga\graph.h>
#include	<svga\inimouse.h>
#include	<svga\dirtybox.h>

#include	<stdlib.h>
#include	<string.h>
#include	<limits.h>

//****************************************************************************
#ifdef	_WIN32
#include	<system\winsys.h>
#include	<svga\ddraw.h>

#else //_WIN32
#include	<system\dossys.h>
#include	<svga\copybov.h>
#include	<svga\cleabov.h>

#endif//_WIN32

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

extern	S32	ClipXMin, ClipXMax, ClipYMin, ClipYMax	;

#ifdef	__cplusplus
}
#endif

//****************************************************************************
S32	BoxClipXMin, BoxClipXMax, BoxClipYMin, BoxClipYMax	;

//****************************************************************************
S32	BoxCleanClip						;

//****************************************************************************
BOX_FUNC  *BoxOneBlit, *BoxOneClean				;
VOID_FUNC *BoxScreenGet, *BoxScreenRelease, *BoxScreenFlip	;

//****************************************************************************
S32	DefSprite	= 0			;
void	*GphSprite 	= NULL			;

//****************************************************************************
static U8	BufferSprite[48*48] 		;

//****************************************************************************
static S32	BoxMask = ~7			;
static S32	BoxAlign= 7			;

//****************************************************************************
static T_BOX	*pBoxMalloc = NULL		;

static T_BOX	*pBoxFree = NULL 		;

static T_BOX	*pBoxStaticList = NULL 		;
static T_BOX	*pBoxOptList = NULL		;
static T_BOX	*pBoxMovingList = NULL		;

//****************************************************************************
static void NewBox(T_BOX **boxlist, S32 x0, S32 y0, S32 x1, S32 y1)	;

#pragma	aux NewBox				\
	parm	[esi] [eax] [ebx] [ecx] [edx]

//****************************************************************************
static void	CollapseList(T_BOX **boxlist)
{
	T_BOX	*pbox		;

	pbox = *boxlist		;

	if(pbox)
	{
		T_BOX	*pbox2		;
		S32	x0, y0, x1, y1	;

		x0 = y0	= INT_MAX	;
		x1 = y1	= INT_MIN	;

		do	// compute bounding box
		{
			x0 = __min(x0, pbox->x0)	;
			y0 = __min(y0, pbox->y0)	;
			x1 = __max(x1, pbox->x1)	;
			y1 = __max(y1, pbox->y1)	;

			pbox2   = pbox			;
			pbox	= pbox->pBoxNext	;
		}
		while(pbox)			;

		pbox = *boxlist			;

		pbox2->pBoxNext = pBoxFree	;// link list with free list
		pBoxFree = pbox->pBoxNext	;

		pbox->x0	= (S16)x0	;
		pbox->y0        = (S16)y0       ;
		pbox->x1        = (S16)x1       ;
		pbox->y1        = (S16)y1	;

		pbox->pBoxNext	= NULL		;
	}
}

//****************************************************************************
inline static void CollapseAllLists()
{
	CollapseList(&pBoxMovingList)		;
	CollapseList(&pBoxStaticList)		;
	CollapseList(&pBoxOptList)		;
}

//****************************************************************************
inline static void DelBox(T_BOX **ppbox)
{
	T_BOX	*pbox	;

	pbox		= *ppbox		;

	*ppbox		= pbox->pBoxNext	;

	pbox->pBoxNext	= pBoxFree		;
	pBoxFree	= pbox			;
}

//****************************************************************************
inline static void AddSortBox(T_BOX **boxlist, T_BOX *pbox, S32 y0)
{
	while(*boxlist)
	{
		if((*boxlist)->y0>=y0)
		{
			break	;
		}
		boxlist = &((*boxlist)->pBoxNext);
	}

	pbox->pBoxNext	= *boxlist	;
	*boxlist	= pbox		;
}

//****************************************************************************
static void NewBox(T_BOX **boxlist, S32 x0, S32 y0, S32 x1, S32 y1)
{
	S32		bx0, by0, bx1, by1	       	;
	S32		restart			       	;
	S32		testsurf, bestsurf		;
	T_BOX		**ppboxbest, *pbox, **ppbox	;

	x0 &= BoxMask			;
	x1 = (x1+BoxAlign) & BoxMask	;

	testsurf= (x1-x0) * (y1-y0) 	;
again:
	bestsurf= INT_MAX	;
	do
	{
		restart = FALSE	;
		ppbox = boxlist	;
		for(pbox=*ppbox; pbox; pbox=pbox->pBoxNext)
		{
			S32	comsurf, totalsurf	;
			S32	cx0, cy0, cx1, cy1	;

			cx0 = pbox->x0	;
			cx1 = pbox->x1	;
			cy0 = pbox->y0	;
			cy1 = pbox->y1	;

			if(	(cx0<=x0)	&&
				(cy0<=y0)	&&
				(cx1>=x1)	&&
				(cy1>=y1)		)
			{
				return		;// box included in another one
			}

                        cx0 = __min(cx0,x0)     ;
                        cx1 = __max(cx1,x1)     ;
                        cy0 = __min(cy0,y0)     ;
                        cy1 = __max(cy1,y1)     ;

			comsurf	  = (cx1-cx0) * (cy1-cy0) ;
			totalsurf = testsurf + (pbox->x1-pbox->x0) * (pbox->y1-pbox->y0) ;

			if(comsurf <= totalsurf)
			{
				x0	= cx0		;
				y0	= cy0		;
				x1	= cx1		;
				y1	= cy1		;
				testsurf= comsurf	;

				DelBox(ppbox)		;

				restart = TRUE		;
				break			;
			}
			if(comsurf < bestsurf)
			{
				bx0	= cx0		;
				by0	= cy0		;
				bx1	= cx1		;
				by1	= cy1		;
				bestsurf  = comsurf	;

				ppboxbest = ppbox	;
			}
			ppbox = &(pbox->pBoxNext)	;
		}
	}
	while(restart)	;

	pbox = pBoxFree	;

	if(pbox == NULL)	// no more free boxes
	{
		if(bestsurf==INT_MAX)	// list is empty
		{
			CollapseAllLists()	; // A REVOIR !!!!
			pbox = pBoxFree		;
			if(pbox == NULL)
			{
				return		;
			}
		}
		else
		{
			DelBox(ppboxbest)	; // Delete best merge

			x0	= bx0		; // new box = common surface
			y0	= by0		;
			x1	= bx1		;
			y1	= by1		;
			testsurf= bestsurf	;

			goto again		; // reinject in the list
		}
	}

	pBoxFree	= pbox->pBoxNext;

	pbox->x0	= (S16)x0	;
	pbox->y0	= (S16)y0	;
	pbox->x1	= (S16)x1	;
	pbox->y1	= (S16)y1	;

	AddSortBox(boxlist, pbox, y0)	;
}

//****************************************************************************
static void EmptyList(T_BOX **boxlist)
{
	// This method free the whole chain at once at the end

	T_BOX	**ppBox		;

	ppBox = boxlist		;

	while(*ppBox)		 // find end of list
	{
		ppBox = &((*ppBox)->pBoxNext)	;
	}

	*ppBox	= pBoxFree	;// end of list linked to free list
	pBoxFree= *boxlist	;// free list = list

	*boxlist = NULL		;// list is empty
}

//****************************************************************************
inline static void MergeLists(T_BOX **boxlistdst, T_BOX *pboxsrc)
{
	while(pboxsrc)
	{
		NewBox(boxlistdst, pboxsrc->x0, pboxsrc->y0, pboxsrc->x1, pboxsrc->y1);
		pboxsrc = pboxsrc->pBoxNext	;
	}
}

//****************************************************************************
void DefaultBoxOneBlit(T_BOX *pbox)
{
	if( pbox->x0 < BoxClipXMin )	pbox->x0 = (S16)BoxClipXMin ;
	if( pbox->x1 > BoxClipXMax )	pbox->x1 = (S16)BoxClipXMax ;
	if( pbox->y0 < BoxClipYMin )	pbox->y0 = (S16)BoxClipYMin ;
	if( pbox->y1 > BoxClipYMax )	pbox->y1 = (S16)BoxClipYMax ;

	if((pbox->x1<=pbox->x0) || (pbox->y1<=pbox->y0)) return	;

#ifdef	_WIN32
	CopyBox(Phys, Log, TabOffPhysLine, pbox);
#else
	CopyBoxVESA(Phys, Log, TabOffPhysLine, pbox);
#endif
}

//****************************************************************************
void DefaultBoxOneClean(T_BOX *pbox)
{
	CopyBox(Log, Screen, TabOffLine, pbox)	;
}

//****************************************************************************
void DefaultBoxOneClear(T_BOX *pbox)
{
	ClearBox(Log, TabOffLine, pbox)		;
}

//****************************************************************************
inline static void BltListAndEmpty(T_BOX **boxlist)
{
	// This method free the whole chain at once at the end because
	// we don't need new boxes during processing.

	T_BOX	**ppBox		;

	ppBox = boxlist		;

	while(*ppBox)		 // walk the list
	{
		BoxOneBlit(*ppBox)		;
		ppBox = &((*ppBox)->pBoxNext)	;
	}

	*ppBox	= pBoxFree	;// end of list linked to free list
	pBoxFree= *boxlist	;// free list = list

	*boxlist = NULL		;// list is empty
}

//****************************************************************************
inline static void CleanList(T_BOX *pboxlist)
{
	while(pboxlist)
	{
		BoxOneClean(pboxlist)		;
		pboxlist = pboxlist->pBoxNext	;
	}
}

//****************************************************************************
static void	MergeListsAndEmpty(T_BOX **boxlistdst, T_BOX **boxlistsrc)
{
	// This method free the block as it walks up the chain.
	// This is slower than freeing the whole list at once at the end
	// but is ensure that we don't run out of boxes while processing
	// (NewBox MAY require a new box every call).

	T_BOX	*pbox, *pbox2	;

	pbox = *boxlistsrc	;

	while(pbox)
	{
		pbox2	= pbox->pBoxNext;	// backup ptr to next block

		pbox->pBoxNext = pBoxFree;	// free block
		pBoxFree = pbox		;

		NewBox(boxlistdst, pbox->x0, pbox->y0, pbox->x1, pbox->y1);

		pbox	= pbox2		;	// next block
	}

	*boxlistsrc = NULL	;// list is empty
}

//****************************************************************************
#define CheckClip()					\
	x1++;						\
	y1++;						\
	if(x0 < 0)		x0 = 0 ;		\
	if(y0 < 0)		y0 = 0 ;		\
	if(x1 > ModeDesiredX)	x1 = ModeDesiredX ;	\
	if(y1 > ModeDesiredY)	y1 = ModeDesiredY ;	\
	if((x1<=x0) || (y1<=y0))	return

//****************************************************************************
void	BoxMovingAdd(S32 x0, S32 y0, S32 x1, S32 y1 )
{
	CheckClip()	;

	NewBox(&pBoxMovingList, x0, y0, x1, y1)	;
}

//****************************************************************************
void	BoxStaticAdd( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	CheckClip()	;

	NewBox(&pBoxStaticList, x0, y0, x1, y1)	;
}

//****************************************************************************
void	BoxUnstaticAdd( S32 x0, S32 y0, S32 x1, S32 y1 )
{
	T_BOX	box	;

	CheckClip()	;

	x0 &= BoxMask			;
	x1 = (x1+BoxAlign) & BoxMask	;

	box.x0	= (S16)x0	;
	box.y0	= (S16)y0	;
	box.x1	= (S16)x1	;
	box.y1	= (S16)y1	;
	box.pBoxNext = NULL	;

	BoxOneClean(&box)	;// offscreen buffer cleanup

	NewBox(&pBoxStaticList, x0, y0, x1, y1)	;
}

//****************************************************************************
void	BoxCls(S32 x0, S32 y0, S32 x1, S32 y1)
{
	T_BOX	box	;

	CheckClip()	;

	x0 &= BoxMask			;
	x1 = (x1+BoxAlign) & BoxMask	;

	box.x0	= (S16)x0	;
	box.y0	= (S16)y0	;
	box.x1	= (S16)x1	;
	box.y1	= (S16)y1	;
	box.pBoxNext = NULL	;

	DefaultBoxOneClear(&box);// offscreen buffer clear

	NewBox(&pBoxStaticList, x0, y0, x1, y1)	;
}

//****************************************************************************
static void BoxPhysClear(S32 x0, S32 y0, S32 x1, S32 y1)
{
	T_BOX	box	;

	x0 &= BoxMask			;
	x1 = (x1+BoxAlign+1) & BoxMask	;
	y1++	;

	box.x0	= (S16)x0	;
	box.y0	= (S16)y0	;
	box.x1	= (S16)x1	;
	box.y1	= (S16)y1	;
	box.pBoxNext = NULL	;

#ifdef	_WIN32
	ClearBox(Phys, TabOffPhysLine, &box)	;
#else
	ClearBoxVESA(Phys, TabOffPhysLine, &box);
#endif
}

//****************************************************************************
void	BoxReset()
{
	EmptyList(&pBoxMovingList)	;
	EmptyList(&pBoxStaticList)	;
	EmptyList(&pBoxOptList)		;
}

//****************************************************************************
void	BoxBlitStaticListToScreen()
{
	T_BOX	*pBox		;

	pBox = pBoxStaticList	;

	while(pBox)		 // walk the list
	{
		CopyBox(Screen, Log, TabOffLine, pBox);
		pBox = pBox->pBoxNext ;
	}
}

//****************************************************************************
void	BoxBlit()
{
	S32	SaveClipXMin, SaveClipYMin, SaveClipXMax, SaveClipYMax  ;
	S32	x0, y0, x1, y1	;

	ManageEvents()	;

	MergeLists(&pBoxOptList, pBoxMovingList);		// rebuild BoxOptList

	if(pBoxStaticList)
	{
		MergeListsAndEmpty(&pBoxOptList, &pBoxStaticList);// add static boxes and clear
	}

	if( FlagMouse )	// Display mouse
	{
		S32	sx, sy	;

		GetBoxGraph(DefSprite, &x0, &y0, &x1, &y1, GphSprite)	;

		sx = MouseX	;
		sy = MouseY	;

		x0 += sx	;
		y0 += sy	;
		x1 += sx+1	;
		y1 += sy+1	;

		if( x0 < 0 )	x0 = 0	;
		if( y0 < 0 )	y0 = 0	;

		if( x1 > ModeDesiredX )	x1 = ModeDesiredX	;
		if( y1 > ModeDesiredY )	y1 = ModeDesiredY	;

		if((x1>x0) && (y1>y0))
		{
			// Save log under mouse
			SaveBlock(Log, BufferSprite, x0, y0, x1-1, y1-1);

			SaveClipXMin	= ClipXMin			;
			SaveClipYMin	= ClipYMin			;
			SaveClipXMax	= ClipXMax			;
			SaveClipYMax	= ClipYMax			;

			ClipXMin	= 0				;
			ClipYMin	= 0				;
			ClipXMax	= ModeDesiredX-1		;
			ClipYMax	= ModeDesiredY-1		;

			AffGraph(DefSprite, sx, sy, GphSprite)		;

			// add box in optlist for immediate blit
			NewBox(&pBoxOptList, x0, y0, x1, y1)		;
		}
	}

	if(BoxScreenGet)	(*BoxScreenGet)()	;// Get access to the screen

	BltListAndEmpty(&pBoxOptList)			;// Blit boxes to screen

	if(BoxCleanClip)
	{
		if(BoxClipYMin>0)
		{
			BoxPhysClear(0, 0, ModeResX-1, BoxClipYMin-1);
		}

		if(BoxClipXMin>0)
		{
			BoxPhysClear(0, BoxClipYMin, BoxClipXMin-1, BoxClipYMax-1);
		}

		if(BoxClipXMax<ModeResX-1)
		{
			BoxPhysClear(BoxClipXMax, BoxClipYMin, ModeResX-1, BoxClipYMax-1);
		}

		if(BoxClipYMax<ModeResY-1)
		{
			BoxPhysClear(0, BoxClipYMax, ModeResX-1, ModeResY-1) ;
		}

		BoxCleanClip = FALSE	;
	}

	if(BoxScreenRelease)	(*BoxScreenRelease)()	;// Release screen

	if( FlagMouse && (x1>x0) && (y1>y0)) // remove sprite from Log
	{
		// Clean Log
		RestoreBlock( Log, BufferSprite, x0, y0, x1-1, y1-1);

		ClipXMin	= SaveClipXMin		;
		ClipYMin	= SaveClipYMin		;
		ClipXMax	= SaveClipXMax		;
		ClipYMax	= SaveClipYMax		;

		// Add sprite to static list to get it
		// cleaned in phys at next blit
		NewBox(&pBoxStaticList, x0, y0, x1, y1)	;
	}
}

//****************************************************************************
void	BoxClean()
{
	if(!pBoxOptList)
	{
		CleanList(pBoxMovingList)		;// offscreen buffer cleanup

		pBoxOptList	= pBoxMovingList	;// OptList = BoxMoving
 	       	pBoxMovingList	= NULL			;// Clear BoxMoving
	}
}

//****************************************************************************
void	BoxUpdate()
{
	BoxBlit()	;
	BoxClean()	;
}

//****************************************************************************
void	BoxStaticFullflip()
{
	BoxReset()	;
	BoxStaticAdd(BoxClipXMin, BoxClipYMin, BoxClipXMax-1, BoxClipYMax-1);
	BoxUpdate()	;
}

//****************************************************************************
static void BoxClear()
{
	BoxReset()	;

	if(!pBoxMalloc)
	{
		return	;
	}

	Free((void*)pBoxMalloc)	;
	pBoxMalloc = NULL	;
}

//****************************************************************************
void	BoxChangeClip(S32 x0, S32 y0, S32 x1, S32 y1, S32 clean)
{
	x1++	;
	y1++	;

	if( x0 < 0 )		x0 = 0 ;
	if( y0 < 0 )		y0 = 0 ;
	if( x1 > ModeDesiredX )	x1 = ModeDesiredX ;
	if( y1 > ModeDesiredY )	y1 = ModeDesiredY ;

	if((x1<=x0) || (y1<=y0))	return	;

	x0 &= BoxMask			;
	x1 = (x1+BoxAlign) & BoxMask	;

	BoxClipXMin	= x0	;
	BoxClipYMin	= y0	;
	BoxClipXMax	= x1	;
	BoxClipYMax	= y1	;

	BoxCleanClip	|= clean;
}

//****************************************************************************
S32 BoxInit( S32 nbBox )
{
	if(	(ProcessorSignature.FPU==1) && (ProcessorSignature.Family>=5)		// At least Pentium
	    &&	((ProcessorSignature.Manufacturer==1) || (ProcessorFeatureFlags.MMX==1))// either Intel or MMX
	  )
	{
		BoxMask = ~7	;
		BoxAlign= 7	;
	}
	else
	{
		BoxMask = ~3	;
		BoxAlign= 3	;
	}

	if(nbBox<4)	// minimum of one box per list
	{
		nbBox = 4	;
	}

	SetBoxScreenGet(NULL)			;
	SetBoxScreenRelease(NULL)		;
	SetBoxScreenFlip(NULL)			;
	SetBoxOneBlit(NULL)			;
	SetBoxOneClean(NULL)			;

	BoxChangeClip(0, 0, ModeDesiredX-1, ModeDesiredY-1, TRUE);

	// Allocate chained list of boxes
	pBoxMalloc = (T_BOX*)Malloc(nbBox*sizeof(T_BOX));
	if(!pBoxMalloc)
	{
		return FALSE	;
	}
	else
	{
		register T_BOX	*pbox	;

		pbox = pBoxMalloc	;

		for(; nbBox>1; nbBox--)	// link the list
		{
			register T_BOX	*pbox2	;

			pbox2		= pbox+1;
			pbox->pBoxNext  = pbox2	;
			pbox		= pbox2	;
		}
		pbox->pBoxNext	= NULL	;	// end points to nothing
	}

	pBoxFree = pBoxMalloc	;	// init the empty list

	atexit(BoxClear);

	return TRUE	;
}

//****************************************************************************
