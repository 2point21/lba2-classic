#include	<system.h>
#include	<svga.h>
#include	<menu.h>

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<dos.h>

/*──────────────────────────────────────────────────────────────────────────*/
static	U8	PalMenu[] =	{
				0,0,0,		// BLACK	0

				71,71,119,	// B_GREY	1
				127,127,215,
				171,167,231,

				115,0,0,	// B_RED	4
				171,0,0,
				215,0,0,

				255,255,87     // YELLOW       7
				} ;

static	U8	PalMenu2[] =	{
				87,87,255,	// BLUE
				87,255,87,	// GREEN
				87,255,255,	// CYAN
				255,87,87,	// RED
				255,87,255     // MAGENTA
				} ;

	S32	Pointed = -1 ;

	S32	FlagAutoPal = FALSE ;

/*══════════════════════════════════════════════════════════════════════════*/
void	InitPalMenu()
{
	S32	n ;

	for( n=0; n<8; n++ )
		PalOne( n, PalMenu[n*3+0], PalMenu[n*3+1], PalMenu[n*3+2] ) ;

	for( n=246; n<251; n++ )
		PalOne( n, PalMenu2[(n-246)*3+0], PalMenu2[(n-246)*3+1], PalMenu2[(n-246)*3+2] ) ;

	PalOne( 255, 255, 255, 255 ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	CopyPalMenu( U8 *ptrpal )
{
	S32	n ;

	for( n=0; n<8; n++ )
	{
		ptrpal[n*3+0] = PalMenu[n*3+0] ;
		ptrpal[n*3+1] = PalMenu[n*3+1] ;
		ptrpal[n*3+2] = PalMenu[n*3+2] ;
	}

	for( n=246; n<251; n++ )
	{
		ptrpal[n*3+0] = PalMenu2[(n-246)*3+0] ;
		ptrpal[n*3+1] = PalMenu2[(n-246)*3+1] ;
		ptrpal[n*3+2] = PalMenu2[(n-246)*3+2] ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
void	DrawWindow(	S32 x1, S32 y1, S32 x2, S32 y2, S32 flags )
{
	S32	coul, up, down ;
	S32	memoclipxmin ;
	S32	memoclipxmax ;
	S32	memoclipymin ;
	S32	memoclipymax ;

	memoclipxmin = ClipXMin ;
	memoclipxmax = ClipXMax ;
	memoclipymin = ClipYMin ;
	memoclipymax = ClipYMax ;

	UnsetClip() ;
	if( FlagAutoPal )	InitPalMenu() ;

	coul = flags & FLAG_RED ? B_RED : B_GREY ;

	if( flags & FLAG_PUSHED )
	{
		up = coul ;
		down = coul+2 ;
	}
	else
	{
		up = coul+2 ;
		down = coul ;
	}

	Line( x1  , y1	, x2  ,y1  , up ) ;
	Line( x1  , y1+1, x2-1,y1+1, up ) ;
	Line( x1  , y1+2, x1  ,y2  , up ) ;
	Line( x1+1, y1+2, x1+1,y2-1, up ) ;

	Line( x2  , y1+1, x2  ,y2  , down ) ;
	Line( x2-1, y1+2, x2-1,y2  , down ) ;
	Line( x1+2, y2-1, x2-2,y2-1, down ) ;
	Line( x1+1, y2	, x2-2,y2  , down ) ;

	Box( x1+2,y1+2,x2-2,y2-2, coul+1 ) ;

	Line( x1-1,y1-1,x2+1,y1-1, 0 ) ;
	Line( x2+1,y1-1,x2+1,y2+1, 0 ) ;
	Line( x1-1,y2+1,x2+1,y2+1, 0 ) ;
	Line( x1-1,y1-1,x1-1,y2+1, 0 ) ;

	SetClip(memoclipxmin, memoclipymin, memoclipxmax, memoclipymax) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	OpenWindow( T_WINLIST *wlist, S32 x1, S32 y1, S32 largeur, S32 hauteur )
{
	S32	recal, x2, y2 ;

	x1++ ;
	y1++ ;

	if( (x1==0) AND (y1==0) )	/* centre window */
	{
		x1 = (ModeDesiredX/2) - ((largeur*SizeChar-3))/2 ;
		y1 = (ModeDesiredY/2) - ((hauteur*(SizeChar+1)-3))/2 ;
		x2 = x1 + largeur*SizeChar+14 ; /* 3 bord 4 espace *2 */
		y2 = y1 + hauteur*(SizeChar+1)+14 ;
	}
	else				/* normal window */
	{
		x2 = x1 + largeur*SizeChar+14 ;
		y2 = y1 + hauteur*(SizeChar+1)+14 ;
	}

	if( y2 >= ModeDesiredY-1 )
	{
		recal = y2 - (ModeDesiredY-2) ;
		y2 -= recal ;
		y1 -= recal ;
	}
	if( x2 >= ModeDesiredX-1 )
	{
		recal = x2 - (ModeDesiredX-2) ;
		x2 -= recal ;
		x1 -= recal ;
	}


	wlist->WindowX1 = x1 ;
	wlist->WindowY1 = y1 ;
	wlist->WindowX2 = x2 ;
	wlist->WindowY2 = y2 ;
	wlist->WindowLig = y1 + 7 ;
	wlist->WindowCol = x1 + 7 ;
	wlist->WindowTab = (wlist->WindowX2-wlist->WindowX1-14)/SizeChar/*10*/ ;

	DrawWindow(	x1,y1,x2,y2, NO_FLAG )	 ;

	CoulText( 0, -1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	GetWindowButton( T_MENU *ptrmenu, S32 handle, T_WINLIST *wlist )
{
	S32		n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	wlist->WindowX1 = 1 ;
	wlist->WindowY1 = 1 ;
	wlist->WindowX2 = ModeDesiredX-2  ;
	wlist->WindowY2 = ModeDesiredY-2 ;
	wlist->WindowCol = wlist->WindowX1 + 7 ;
	wlist->WindowLig = wlist->WindowY1 + 7 ;
	wlist->WindowTab = (wlist->WindowX2-wlist->WindowX1-14)/SizeChar ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;
	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			wlist->WindowX1 = ptrlcb->X1 ;
			wlist->WindowY1 = ptrlcb->Y1 ;
			wlist->WindowX2 = ptrlcb->X2 ;
			wlist->WindowY2 = ptrlcb->Y2 ;
			wlist->WindowCol = wlist->WindowX1 + 7 ;
			wlist->WindowLig = wlist->WindowY1 + 7 ;
			break ;
		}
		ptrlcb++ ;
	}
	DrawWindow(	wlist->WindowX1,wlist->WindowY1,
			wlist->WindowX2,wlist->WindowY2,
			ptrlcb->Flags ) ;
	CoulText( 0, -1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	WinText( T_WINLIST *wlist, char *string, S32 value )
{
	GraphPrintf( FALSE, wlist->WindowCol, wlist->WindowLig, "%s%d", string, value ) ;
	wlist->WindowLig += SizeChar+1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	WinList( T_WINLIST *wlist, char **index, S32 nb, S32 size )
{
	S32	n ;

	wlist->WindowArray = index ;
	wlist->WindowNbIndex = nb ;
	wlist->WindowTab = size ;

	for( n=0; n<nb; n++ )
	{
		GraphPrintf( FALSE, wlist->WindowCol, wlist->WindowLig, "%s", *index ) ;
		index ++ ;
		wlist->WindowLig += SizeChar+1 ;
		if( wlist->WindowLig > wlist->WindowY2 -4 -SizeChar )
		{
			wlist->WindowCol += size * SizeChar ;
			wlist->WindowLig = wlist->WindowY1 + 7 ;

			if( wlist->WindowCol > wlist->WindowX2-4-size*SizeChar )
			{
				wlist->WindowNbIndex = n+1 ;
				break ;
			}
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
void	WinSelect( T_WINLIST *wlist, S32 selected, S32 coul )
{
	S32	n ;
	char	**index ;

	index = wlist->WindowArray ;
	wlist->WindowCol = wlist->WindowX1 + 7 ;
	wlist->WindowLig = wlist->WindowY1 + 7 ;

	for( n=0; n<wlist->WindowNbIndex; n++ )
	{
		if( n == selected )
		{
			CoulText( coul, -1 ) ;
			GraphPrintf( FALSE, wlist->WindowCol, wlist->WindowLig, "%s", *index ) ;
			BoxStaticAdd(	wlist->WindowCol,
					wlist->WindowLig,
					wlist->WindowCol+strlen(*index)*SizeChar-1,
					wlist->WindowLig+7 ) ;
		}
		index ++ ;
		wlist->WindowLig += SizeChar+1 ;
		if( wlist->WindowLig > wlist->WindowY2 -4 -SizeChar )
		{
			wlist->WindowCol += wlist->WindowTab * SizeChar ;
			wlist->WindowLig = wlist->WindowY1 + 7 ;
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
void	WinSepar( T_WINLIST *wlist )
{
	Line( wlist->WindowX1+2, wlist->WindowLig, wlist->WindowX2-2  ,wlist->WindowLig  , B_GREY ) ;
	Line( wlist->WindowX1+2, wlist->WindowLig+1, wlist->WindowX2-2	,wlist->WindowLig+1 , B_GREY+2 ) ;
	wlist->WindowLig += 3 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	FlipWindow( T_WINLIST *wlist )
{
	BoxStaticAdd( wlist->WindowX1-1, wlist->WindowY1-1, wlist->WindowX2+1, wlist->WindowY2+1 ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	GetIndexWinList( T_WINLIST *wlist )
{
	S32	num ;
	S32	x, y, sizecol ;

	x = MouseX ;
	y = MouseY ;

	if( (x>=wlist->WindowX1+7) AND (x<=wlist->WindowX2-7)
	AND (y>=wlist->WindowY1+7) AND (y<=wlist->WindowY2-4) )
	{
		sizecol = ((wlist->WindowY2-2) - (wlist->WindowY1+7)) / (SizeChar+1) ;

		x -= wlist->WindowX1+7 ;
		y -= wlist->WindowY1+7 ;

		x /= wlist->WindowTab*SizeChar ;
		y /= (SizeChar+1) ;

		num = x * sizecol + y ;

		if( num < wlist->WindowNbIndex )
		{
			return num ;
		}
	}
	return -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	DrawBox( T_MENU *ptrmenu, S32 numlcb, S32 pushed)
{
	S32		flags ;
	S32		nbaffcar ;
	S32		len, x, y ;
	char		string[256] ;
	char		valuestring[64] ;
	T_CLICK_BOX	*ptrlcb ;

	MemoClip() 	;
	UnsetClip() 	;

	ptrlcb = ptrmenu->PtrMallocList + numlcb ;

	if( ptrlcb->Type == TYPE_SWITCH )
	{
		ptrlcb->Flags &= ~FLAG_PUSHED ;
		if( *(ptrlcb->PtrVar) & ptrlcb->Mask )
			ptrlcb->Flags |= FLAG_PUSHED ;
	}

	flags = ptrlcb->Flags ;

	/* dessin de la boite */

	if( ptrlcb->Handle != -1 )
	{
		flags |= pushed ;
		DrawWindow(	ptrlcb->X1,ptrlcb->Y1,
				ptrlcb->X2,ptrlcb->Y2,
				flags ) ;
	}

	/* aff du text */

	strcpy( string, ptrlcb->PtrString ) ;
	if( ptrlcb->Type == TYPE_CHANGE_VALUE )
	{
		strcat( string, itoa( *(ptrlcb->PtrVar), valuestring, 10 ) ) ;
	}

	nbaffcar = ((ptrlcb->X2-3)-(ptrlcb->X1+3))/SizeChar ;

	len = strlen( string ) ;
	if( (len > nbaffcar) AND (len!=1) )
	{
		if( ptrlcb->Flags & FLAG_FILENAME )	/* perd le debut */
		{
			memmove(string,
				string+(len-nbaffcar),
				nbaffcar +1 ) ; /* perd le debut */
		}
		else					/* perd la fin */
		{
			string[nbaffcar] = 0 ;
		}
	}

	if( ptrlcb->Flags & FLAG_CENTRE )
	{
		len = (len*SizeChar) /2 ;
		x = (ptrlcb->X1+ptrlcb->X2)/2 - len ;
		y = (ptrlcb->Y1+ptrlcb->Y2)/2 - (SizeChar/2) + 1 ;
	}
	else
	{
		x = ptrlcb->X1 + 3 ;
		y = ptrlcb->Y1 + 3 ;
	}

	if( flags & FLAG_PUSHED )	x++ ;

	if( ptrlcb->Flags & FLAG_CONTOUR )
	{
		CoulText( 0, -1 ) ;
		GraphPrintf( FALSE, x-1, y-1, string ) ;
		GraphPrintf( FALSE, x  , y-1, string ) ;
		GraphPrintf( FALSE, x+1, y-1, string ) ;
		GraphPrintf( FALSE, x-1, y  , string ) ;
		GraphPrintf( FALSE, x+1, y  , string ) ;
		GraphPrintf( FALSE, x-1, y+1, string ) ;
		GraphPrintf( FALSE, x  , y+1, string ) ;
		GraphPrintf( FALSE, x+1, y+1, string ) ;

		CoulText( YELLOW, -1 ) ;
		GraphPrintf( FALSE, x, y, string ) ;
	}
	else
	{
		CoulText( YELLOW, -1 ) ;
		GraphPrintf( FALSE, x, y, string ) ;
	}

	BoxStaticAdd( ptrlcb->X1-1,ptrlcb->Y1-1,ptrlcb->X2+1,ptrlcb->Y2+1 );

	RestoreClip() ;
}

/*══════════════════════════════════════════════════════════════════════════*/
T_CLICK_BOX	*AddHeaderButton(	T_MENU *ptrmenu,
					S32 x1, S32 y1,
					S32 largeur, S32 hauteur )
{
	S32		x2, y2 ;
	T_CLICK_BOX	*ptrlcb ;
	void		*ptr ;
	U32		newsize ;

	x1 = ptrmenu->X1 + 6 + x1*(SizeChar+7)-2 ;
	y1 = ptrmenu->Y1 + 6 + y1*(SizeChar+7)-2 ;
	x2 = x1 + largeur*(SizeChar+7)-2 ;
	y2 = y1 + hauteur*(SizeChar+7)-2 ;

	ptrmenu->NbBox++ ;
	newsize = ptrmenu->NbBox * sizeof(T_CLICK_BOX) ;

	if( Mshrink( (void*)(ptrmenu->PtrMallocList), newsize ) == 0L )
	{
		ptr = (void*)ptrmenu->PtrMallocList ;
		ptrmenu->PtrMallocList = (T_CLICK_BOX*)Malloc( newsize ) ;
		memmove( (void*)ptrmenu->PtrMallocList, ptr, newsize-sizeof(T_CLICK_BOX) ) ;
		Free( ptr ) ;
	}

	ptrlcb = ptrmenu->PtrMallocList + ptrmenu->NbBox - 1 ;

	ptrlcb->X1 = x1 ;
	ptrlcb->Y1 = y1 ;
	ptrlcb->X2 = x2 ;
	ptrlcb->Y2 = y2 ;

	return ptrlcb ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	GetCoorButton( T_MENU *ptrmenu, S32 handle, S32 *x0, S32 *y0, S32 *x1, S32 *y1 )
{
	S32		n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	*x0 = 0 ;
	*y0 = 0 ;
	*x1 = ModeDesiredX ;
	*y1 = ModeDesiredY ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;
	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			*x0 = ptrlcb->X1 + 3 ;
			*y0 = ptrlcb->Y1 + 3 ;
			*x1 = ptrlcb->X2 - 3 ;
			*y1 = ptrlcb->Y2 - 3 ;
			break ;
		}
		ptrlcb++ ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
void	GetCoorMenu( T_MENU *ptrmenu, S32 *x0, S32 *y0, S32 *x1, S32 *y1 )
{
	*x0 = ptrmenu->X1 ;
	*y0 = ptrmenu->Y1 ;
	*x1 = ptrmenu->X2 ;
	*y1 = ptrmenu->Y2 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	AddButton(	T_MENU	*ptrmenu,
			S32	handle,
			S32	x1, S32 y1,
			S32	largeur, S32 hauteur,
			S32	flags,
			char	*ptrstring	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = ptrstring ;
	ptrlcb->Type = TYPE_BUTTON ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	AddChangeValue( T_MENU	*ptrmenu,
			S32	handle,
			S32	x1, S32 y1,
			S32	largeur, S32 hauteur,
			S32	flags,
			char	*ptrstring,
			S32	*ptrvar,
			S32	step,
			S32	minvar,
			S32	maxvar	)
{
	T_CLICK_BOX	*ptrlcborg ;
	T_CLICK_BOX	*ptrlcb ;

	/* bouton valeur */

	ptrlcborg = AddHeaderButton( ptrmenu, x1+hauteur,y1, largeur-2*hauteur, hauteur ) ;

	ptrlcborg->Handle = handle ;
	ptrlcborg->Flags = flags ;
	ptrlcborg->PtrString = ptrstring ;
	ptrlcborg->Type = TYPE_CHANGE_VALUE ;
	ptrlcborg->PtrVar = ptrvar ;
	ptrlcborg->Mask = step ;

	/* bouton - */
	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, hauteur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = FLAG_CENTRE | flags ;
	ptrlcb->PtrString = "-" ;
	ptrlcb->Type = TYPE_DEC_VALUE ;
	ptrlcb->PtrVar = ptrvar ;
	ptrlcb->Mask = minvar ;

	/* bouton + */
	ptrlcb = AddHeaderButton( ptrmenu, x1+largeur-hauteur,y1, hauteur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = FLAG_CENTRE | flags ;
	ptrlcb->PtrString = "+" ;
	ptrlcb->Type = TYPE_INC_VALUE ;
	ptrlcb->PtrVar = ptrvar ;
	ptrlcb->Mask = maxvar ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	AddSwitch(	T_MENU	*ptrmenu,
			S32	handle,
			S32	x1, S32 y1,
			S32	largeur, S32 hauteur,
			S32	flags,
			char	*ptrstring,
			S32	*ptrvar,
			S32	mask	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = ptrstring ;
	ptrlcb->Type = TYPE_SWITCH ;
	ptrlcb->PtrVar = ptrvar ;
	ptrlcb->Mask = mask ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	AddText(	T_MENU	*ptrmenu,
			S32	x1, S32 y1,
			S32	largeur, S32 hauteur,
			S32	flags,
			char	*ptrstring	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = -1 ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = ptrstring ;
	ptrlcb->Type = TYPE_TEXT ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	AddWindow(	T_MENU	*ptrmenu,
			S32	handle,
			S32	x1, S32 y1,
			S32	largeur, S32 hauteur,
			S32	flags	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = "" ;
	ptrlcb->Type = TYPE_WINDOW ;
}

/*══════════════════════════════════════════════════════════════════════════*/
S32	GetOptimalSize( S32 deltax )
{
	return (deltax-8)/(SizeChar+7) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	GetSizeMenu( T_MENU *ptrmenu, U32 *pdx, U32 *pdy )
{
	*pdx = ptrmenu->X2 - ptrmenu->X1 + 1 ;
	*pdy = ptrmenu->Y2 - ptrmenu->Y1 + 1 ;
}

S32	MenuBestFitX( S32 deltax )
{
	return (deltax-14) / SizeChar ;
}

S32	MenuBestFitY( S32 deltax )
{
	return (deltax-14) / (SizeChar+1) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
S32	OpenMenu(	T_MENU *ptrmenu,
			S32 largeur, S32 hauteur )
{
	S32	dx, dy	;

	ptrmenu->LastClicked = -1 ;
	ptrmenu->Repeat = FALSE ;

	ptrmenu->NbBox = 0 ;

	ptrmenu->Largeur = largeur ;
	ptrmenu->Hauteur = hauteur ;

	ptrmenu->X1	= 0 ;
	ptrmenu->Y1	= 0 ;

	dx = ((ptrmenu->Largeur*(SizeChar+7)-3)) ;
	dy = ((ptrmenu->Hauteur*(SizeChar+7)-3)) ;

	ptrmenu->X2	= dx	;
	ptrmenu->Y2	= dy	;
	ptrmenu->Dx	= dx	;
	ptrmenu->Dy	= dy	;

	ptrmenu->Flags	= 0	;

	/* init une fausse clickbox pour first time */
	ptrmenu->PtrMallocList = (T_CLICK_BOX*)Malloc( 42*sizeof(T_CLICK_BOX) ) ;
	if( !ptrmenu->PtrMallocList )	return FALSE ;

	return TRUE ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	CloseMenu( T_MENU *ptrmenu )
{
	S32	x0, y0, x1, y1				;

	GetCoorMenu(ptrmenu, &x0, &y0, &x1, &y1)	;

	BoxUnstaticAdd(x0, y0, x1, y1)			;

	Free( ptrmenu->PtrMallocList )			;
}

/*══════════════════════════════════════════════════════════════════════════*/
S32	DrawMenu( T_MENU *ptrmenu, S32 x1, S32 y1)
{
	S32		n, nb ;
	S32		x2,y2 ;
	S32		oldx1,oldy1 ;
	T_CLICK_BOX	*ptrlcb ;

/*	ptrmenu->LastClicked = -1 ;	*/

	oldx1 = ptrmenu->X1 ;
	oldy1 = ptrmenu->Y1 ;

	if ( x1 == -1 )
	{
		x1 = (ModeDesiredX/2) - (ptrmenu->Dx/*(ptrmenu->Largeur*(SizeChar+7)-3)*/)/2;
		x2 = x1 + ptrmenu->Largeur*(SizeChar+7)+6	;
	}
	else
	{
		x1++						;
		x2 = x1 + ptrmenu->Largeur*(SizeChar+7)+6	;
	}

	if ( y1 == -1 )
	{
		y1 = (ModeDesiredY/2) - (ptrmenu->Dy/*(ptrmenu->Hauteur*(SizeChar+7)-3)*/)/2 ;
		y2 = y1 + ptrmenu->Hauteur*(SizeChar+7)+6	;
	}
	else				/* normal window */
	{
		y1++						;
		y2 = y1 + ptrmenu->Hauteur*(SizeChar+7)+6	;
	}

	ptrmenu->X1	= x1 ;
	ptrmenu->Y1	= y1 ;
	ptrmenu->X2	= x2 ;
	ptrmenu->Y2	= y2 ;

	DrawWindow(	ptrmenu->X1, ptrmenu->Y1,
			ptrmenu->X2, ptrmenu->Y2,
			ptrmenu->Flags ) ;

	nb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;
	for( n=0; n<nb; n++ )
	{
		ptrlcb->X1 += x1-oldx1 ;
		ptrlcb->Y1 += y1-oldy1 ;
		ptrlcb->X2 += x1-oldx1 ;
		ptrlcb->Y2 += y1-oldy1 ;

		DrawBox( ptrmenu, n, NO_FLAG) ;

		ptrlcb++ ;
	}

	BoxStaticAdd( x1-1,y1-1,x2+1,y2+1 ) ;

	return y2 + 1 ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	ClearMenu( T_MENU *ptrmenu )
{
	ptrmenu->LastClicked = -1 ;

	Box(	ptrmenu->X1, ptrmenu->Y1,
		ptrmenu->X2, ptrmenu->Y2, 0 ) ;

	BoxStaticAdd(	ptrmenu->X1, ptrmenu->Y1,
			ptrmenu->X2, ptrmenu->Y2	) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/* on scan TOUJOURS une list de click box en DECREMENTANT */
S32	ScanListClick( T_MENU *ptrmenu )
{
	S32		n ;
	T_CLICK_BOX	*ptrlcb ;
	S32		x, y ;
	S32		nb ;

	nb = ptrmenu->NbBox ;

	if( !nb )	return -2 ;

	x = MouseX ;
	y = MouseY ;

	if( (x>=ptrmenu->X1) AND (x<=ptrmenu->X2)
	AND (y>=ptrmenu->Y1) AND (y<=ptrmenu->Y2) )
	{
		ptrlcb = ptrmenu->PtrMallocList + (nb-1) ;
		for( n=nb-1; n>=0; n-- )
		{
			if( (x >= ptrlcb->X1) AND (y >= ptrlcb->Y1)
			AND (x <= ptrlcb->X2) AND (y <= ptrlcb->Y2) )
			{
				if( ptrlcb->Handle != -1 )	return n ;
				else				return -1 ;
			}
			ptrlcb-- ;
		}
		return -1 ;
	}
	return -2 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	GereMenu( T_MENU *ptrmenu )
{
	char		inputvaluestring[64] ;
	S32		exitflag = -1 ;
	S32		select ;

	T_CLICK_BOX	*ptrlcb ;
	T_CLICK_BOX	*ptrlcborg ;
	S32		value, ivalue ;
	S32		vmin, vmax ;
	S32		click ;
	char		*memostring ;


	select = ScanListClick( ptrmenu ) ;

	if( select == -2 )	exitflag = -2 ;

	if( (click=Click) != 0 )
	{
		if( select > -1 )
		{
			ptrlcb = ptrmenu->PtrMallocList + select ;
			exitflag = ptrlcb->Handle ;

			if( ptrmenu->LastClicked == select )
			{
				if( (!ptrmenu->Repeat) )
				{
					if( !((click==2) AND (ptrlcb->Flags&FLAG_REPEAT)) )
					{
						/* methode du click/run/wait */
						while( Click )
						{
							BoxUpdate()	;
						}
						return -1	;
					}
				}
				else
				{
					ptrmenu->Repeat = FALSE ;
				}
			}
			switch( ptrlcb->Type )
			{
				case TYPE_SWITCH:
					*(ptrlcb->PtrVar) ^= ptrlcb->Mask ;
					break ;

				case TYPE_CHANGE_VALUE: /* scroll value */
					vmin = (ptrmenu->PtrMallocList + select +1 )->Mask ;
					vmax = (ptrmenu->PtrMallocList + select +2 )->Mask ;
					value = *(ptrlcb->PtrVar) ;
					if( click & 2 ) /* slide value */
					{
						GetMouseDep() ;
						while( Click )
						{
							GetMouseDep() ;
							value += (MouseXDep/4) * ptrlcb->Mask ;
							if( value < vmin ) value = vmin ;
							if( value > vmax ) value = vmax ;
							*(ptrlcb->PtrVar) = value ;
							DrawBox( ptrmenu, select, FLAG_PUSHED) ;
							BoxUpdate()	;
						}
					}
					else		/* input value */
					{
						memostring = ptrlcb->PtrString ;
						itoa( value, inputvaluestring, 10 ) ;
						ptrlcb->PtrString = inputvaluestring ;
						ptrlcb->Type = TYPE_TEXT ;
						DrawBox( ptrmenu, select, FLAG_PUSHED) ;
						InputString( ptrmenu, ptrlcb->Handle , 63 ) ;
						ivalue = atoi( inputvaluestring ) ;
						if( ivalue < vmin ) ivalue = vmin ;
						if( ivalue > vmax ) ivalue = vmax ;
						*(ptrlcb->PtrVar) = ivalue ;
						ptrlcb->Type = TYPE_CHANGE_VALUE ;
						ptrlcb->PtrString = memostring ;
					}
					DrawBox( ptrmenu, select, NO_FLAG) ;
					break ;

				case TYPE_DEC_VALUE:
					ptrlcborg = GetPtrLcb( ptrmenu, ptrlcb->Handle ) ;
					value = *ptrlcb->PtrVar ;
					value -= ptrlcborg->Mask ; /* step */
					if( value < ptrlcb->Mask )
						value = ptrlcb->Mask ; /* minvar */
					*ptrlcb->PtrVar = value ;
					/* draw value box */
					DrawBox( ptrmenu, select-1, NO_FLAG) ;
					break ;

				case TYPE_INC_VALUE:
					ptrlcborg = GetPtrLcb( ptrmenu, ptrlcb->Handle ) ;
					value = *ptrlcb->PtrVar ;
					value += ptrlcborg->Mask ; /* step */
					if( value > ptrlcb->Mask )
						value = ptrlcb->Mask ; /* maxvar */
					*ptrlcb->PtrVar = value ;
					/* draw value box */
					DrawBox( ptrmenu, select-2, NO_FLAG) ;
					break ;
			}

			if( ptrlcb->Type != TYPE_WINDOW )
			{
				DrawBox( ptrmenu, select, FLAG_PUSHED) ;
				ptrmenu->LastClicked = select ;
			}
		}
	}
	else	/* pas de click */
	{
		if( ptrmenu->LastClicked != -1 )
		{
			DrawBox( ptrmenu, ptrmenu->LastClicked, NO_FLAG) ;
			ptrmenu->LastClicked = -1 ;
		}
		if (ptrmenu->LastClicked != -1)
		{
			DrawBox(ptrmenu, ptrmenu->LastClicked, NO_FLAG);
			ptrmenu->LastClicked = -1;
		}
		if (select>=0)
		{
			ptrlcb = ptrmenu->PtrMallocList + select;
			Pointed = ptrlcb->Handle;
		}
		else	Pointed=-1;
	}

	return exitflag ;
}

/*──────────────────────────────────────────────────────────────────────────*/
