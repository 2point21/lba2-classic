#include	"\projet\lib386\lib_sys\adeline.h"
#include	"\projet\lib386\lib_sys\lib_sys.h"
#include	"\projet\lib386\lib_svga\lib_svga.h"

#include	"lib_menu.h"

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<dos.h>

#ifndef	WATCOM9
#include	<mem.h>
#endif

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

UBYTE	PalMenu[16*3] = {	0,0,0,
				71,71,119,
				127,127,215,
				171,167,231,
				115,0,0,
				171,0,0,
				215,0,0,
				0,0,0,
				87,87,87,
				87,87,255,
				87,255,87,
				87,255,255,
				255,87,87,
				255,87,255,
				255,255,87,
				255,255,255	} ;


T_CLICK_BOX	*GetPtrLcb( T_MENU *ptrmenu, WORD handle ) ;


WORD	FlagAutoPal = FALSE ;

char	InputValueString[64] ;

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	SetMenuAutoPal( WORD flag )
{
	FlagAutoPal = flag ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	InitPalMenu()
{
	WORD	n ;

	for( n=0; n<16; n++ )
		PalOne( n, PalMenu[n*3+0], PalMenu[n*3+1], PalMenu[n*3+2] ) ;
}

void	CopyPalMenu( UBYTE *ptrpal )
{
	WORD	n ;

	for( n=0; n<16; n++ )
	{
		*ptrpal++ = PalMenu[n*3+0] ;
		*ptrpal++ = PalMenu[n*3+1] ;
		*ptrpal++ = PalMenu[n*3+2] ;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	DrawWindow(	LONG x1, LONG y1, LONG x2, LONG y2,
			LONG flags )
{
	LONG	coul, up, down ;

	MemoClip() ;
	UnSetClip() ;
	if( FlagAutoPal )	InitPalMenu() ;

	if( flags & FLAG_RED )	coul = B_ROUGE ;
	else			coul = B_GRIS ;

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

	Line( x1  , y1  , x2  ,y1  , up ) ;
	Line( x1  , y1+1, x2-1,y1+1, up ) ;
	Line( x1  , y1+2, x1  ,y2  , up ) ;
	Line( x1+1, y1+2, x1+1,y2-1, up ) ;

	Line( x2  , y1+1, x2  ,y2  , down ) ;
	Line( x2-1, y1+2, x2-1,y2  , down ) ;
	Line( x1+2, y2-1, x2-2,y2-1, down ) ;
	Line( x1+1, y2  , x2-2,y2  , down ) ;

	Box( x1+2,y1+2,x2-2,y2-2, coul+1 ) ;

	Line( x1-1,y1-1,x2+1,y1-1, 0 ) ;
	Line( x2+1,y1-1,x2+1,y2+1, 0 ) ;
	Line( x1-1,y2+1,x2+1,y2+1, 0 ) ;
	Line( x1-1,y1-1,x1-1,y2+1, 0 ) ;

	RestoreClip() ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		   Û   Û  Û    ÛÛÜ Û Û฿฿฿Ü Û฿฿฿Û Û   Û ÛÛ฿฿฿
		   ÛÛ฿ÜÛ  ÛÛ   ÛÛ฿ÛÛ ÛÛ  Û ÛÛ  Û ÛÛ฿ÜÛ ฿฿฿฿Û
		   ฿   ฿  ฿฿   ฿฿  ฿ ฿฿฿฿  ฿฿฿฿฿ ฿   ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	OpenWindow( T_WINLIST *wlist, WORD x1, WORD y1, WORD largeur, WORD hauteur )
{
	WORD	recal, x2, y2 ;

	x1++ ;
	y1++ ;

	if( (x1==0) AND (y1==0) )	/* centre window */
	{
		x1 = (Screen_X/2) - ((largeur*SizeCar-3))/2 ;
		y1 = (Screen_Y/2) - ((hauteur*(SizeCar+1)-3))/2 ;
		x2 = x1 + largeur*SizeCar+14 ;	/* 3 bord 4 espace *2 */
		y2 = y1 + hauteur*(SizeCar+1)+14 ;
	}
	else				/* normal window */
	{
		x2 = x1 + largeur*SizeCar+14 ;
		y2 = y1 + hauteur*(SizeCar+1)+14 ;
	}

	if( y2 >= 479 )
	{
		recal = y2 - 478 ;
		y2 -= recal ;
		y1 -= recal ;
	}
	if( x2 >= 639 )
	{
		recal = x2 - 638 ;
		x2 -= recal ;
		x1 -= recal ;
	}


	wlist->WindowX1 = x1 ;
	wlist->WindowY1 = y1 ;
	wlist->WindowX2 = x2 ;
	wlist->WindowY2 = y2 ;
	wlist->WindowLig = y1 + 7 ;
	wlist->WindowCol = x1 + 7 ;
	wlist->WindowTab = 10 ;

	DrawWindow(	x1,y1,x2,y2, NO_FLAG )	 ;

	CoulText( 0, -1 ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	GetWindowButton( T_MENU *ptrmenu, WORD handle, T_WINLIST *wlist )
{
	WORD		n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	wlist->WindowX1 = 1 ;
	wlist->WindowY1 = 1 ;
	wlist->WindowX2 = Screen_X-2  ;
	wlist->WindowY2 = Screen_Y-2 ;
	wlist->WindowCol = wlist->WindowX1 + 7 ;
	wlist->WindowLig = wlist->WindowY1 + 7 ;
	wlist->WindowTab = 10 ;

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

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	WinText( T_WINLIST *wlist, UBYTE *string, LONG value )
{
	Text( wlist->WindowCol, wlist->WindowLig, "%s%l", string, value ) ;
	wlist->WindowLig += SizeCar+1 ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	WinList( T_WINLIST *wlist, UBYTE **index, WORD nb, WORD size )
{
	WORD	n ;

	wlist->WindowArray = index ;
	wlist->WindowNbIndex = nb ;
	wlist->WindowTab = size ;

	for( n=0; n<nb; n++ )
	{
		Text( wlist->WindowCol, wlist->WindowLig, "%s", *index ) ;
		index ++ ;
		wlist->WindowLig += SizeCar+1 ;
		if( wlist->WindowLig > wlist->WindowY2 -4 -SizeCar )
		{
			wlist->WindowCol += size * SizeCar ;
			wlist->WindowLig = wlist->WindowY1 + 7 ;

			if( wlist->WindowCol > wlist->WindowX2-4-size*SizeCar )
			{
				wlist->WindowNbIndex = n+1 ;
				break ;
			}
		}
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	WinSelect( T_WINLIST *wlist, WORD selected, WORD coul )
{
	WORD	n ;
	UBYTE	**index ;

	index = wlist->WindowArray ;
	wlist->WindowCol = wlist->WindowX1 + 7 ;
	wlist->WindowLig = wlist->WindowY1 + 7 ;

	for( n=0; n<wlist->WindowNbIndex; n++ )
	{
		if( n == selected )
		{
			ShowMouse( 0 ) ;
			CoulText( coul, -1 ) ;
			Text( wlist->WindowCol, wlist->WindowLig, "%s", *index ) ;
			CopyBlockPhys(	wlist->WindowCol,
					wlist->WindowLig,
					wlist->WindowCol+strlen(*index)*SizeCar-1,
					wlist->WindowLig+7 ) ;
			ShowMouse( 1 ) ;
		}
		index ++ ;
		wlist->WindowLig += SizeCar+1 ;
		if( wlist->WindowLig > wlist->WindowY2 -4 -SizeCar )
		{
			wlist->WindowCol += wlist->WindowTab * SizeCar ;
			wlist->WindowLig = wlist->WindowY1 + 7 ;
		}
	}
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	WinSepar( T_WINLIST *wlist )
{
	Line( wlist->WindowX1+2, wlist->WindowLig, wlist->WindowX2-2  ,wlist->WindowLig  , B_GRIS ) ;
	Line( wlist->WindowX1+2, wlist->WindowLig+1, wlist->WindowX2-2  ,wlist->WindowLig+1 , B_GRIS+2 ) ;
	wlist->WindowLig += 3 ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	FlipWindow( T_WINLIST *wlist )
{
	CopyBlockPhys( wlist->WindowX1-1, wlist->WindowY1-1, wlist->WindowX2+1, wlist->WindowY2+1 ) ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

WORD	GetIndexWinList( T_WINLIST *wlist )
{
	WORD	num ;
	LONG	x, y, sizecol ;

	x = Mouse_X ;
	y = Mouse_Y ;

	if( (x>=wlist->WindowX1+7) AND (x<=wlist->WindowX2-7)
	AND (y>=wlist->WindowY1+7) AND (y<=wlist->WindowY2-4) )
	{
		sizecol = ((wlist->WindowY2-2) - (wlist->WindowY1+7)) / (SizeCar+1) ;

		x -= wlist->WindowX1+7 ;
		y -= wlist->WindowY1+7 ;

		x /= wlist->WindowTab*SizeCar ;
		y /= (SizeCar+1) ;

		num = x * sizecol + y ;

		if( num < wlist->WindowNbIndex )
		{
			return num ;
		}
	}
	return -1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			    ÛÜ ÜÛ Û฿฿฿฿ ÛÛÜ Û Û   Û
			    ÛÛ฿ Û ÛÛ฿฿  ÛÛ฿ÛÛ ÛÛ  Û
			    ฿฿  ฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/


void	DrawBox( T_MENU *ptrmenu, WORD numlcb, WORD pushed, WORD copyphys )
{
	WORD		flags ;
	WORD		nbaffcar ;
	WORD		len, x, y ;
	UBYTE		string[256] ;
	UBYTE		valuestring[64] ;
	T_CLICK_BOX	*ptrlcb ;

	MemoClip() ;
	UnSetClip() ;

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

	nbaffcar = ((ptrlcb->X2-3)-(ptrlcb->X1+3))/SizeCar ;

	len = strlen( string ) ;
	if( (len > nbaffcar) AND (len!=1) )
	{
		if( ptrlcb->Flags & FLAG_FILENAME )	/* perd le debut */
		{
			memmove(string,
				string+(len-nbaffcar),
				nbaffcar +1 ) ;	/* perd le debut */
		}
		else					/* perd la fin */
		{
			string[nbaffcar] = 0 ;
		}
	}

	if( ptrlcb->Flags & FLAG_CENTRE )
	{
		len = (len*SizeCar) /2  ;
		x = (ptrlcb->X1+ptrlcb->X2)/2 - len ;
		y = (ptrlcb->Y1+ptrlcb->Y2)/2 - (SizeCar/2) + 1 ;
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
		Text( x-1, y-1, string ) ;
		Text( x  , y-1, string ) ;
		Text( x+1, y-1, string ) ;
		Text( x-1, y  , string ) ;
		Text( x+1, y  , string ) ;
		Text( x-1, y+1, string ) ;
		Text( x  , y+1, string ) ;
		Text( x+1, y+1, string ) ;

		CoulText( JAUNE, -1 ) ;
		Text( x, y, string ) ;
	}
	else
	{
		CoulText( JAUNE, -1 ) ;
		Text( x, y, string ) ;
	}

	if( copyphys )
	{
		ShowMouse( 0 ) ;
		CopyBlockPhys( ptrlcb->X1-1,ptrlcb->Y1-1,ptrlcb->X2+1,ptrlcb->Y2+1 );
		ShowMouse( 1 ) ;
	}

	RestoreClip() ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

T_CLICK_BOX	*AddHeaderButton(	T_MENU *ptrmenu,
					WORD x1, WORD y1,
					WORD largeur, WORD hauteur )
{
	WORD		x2, y2 ;
	T_CLICK_BOX	*ptrlcb ;
	UBYTE		*ptr ;
	ULONG		newsize ;

	x1 = ptrmenu->X1 + 6 + x1*(SizeCar+7)-2 ;
	y1 = ptrmenu->Y1 + 6 + y1*(SizeCar+7)-2 ;
	x2 = x1 + largeur*(SizeCar+7)-2 ;
	y2 = y1 + hauteur*(SizeCar+7)-2 ;

	ptrmenu->NbBox++ ;
	newsize = ptrmenu->NbBox * sizeof(T_CLICK_BOX) ;

	if( Mshrink( ptrmenu->PtrMallocList, newsize ) == 0L )
	{
		ptr = (UBYTE*)ptrmenu->PtrMallocList ;
		ptrmenu->PtrMallocList = Malloc( newsize ) ;
		memmove( ptrmenu->PtrMallocList, ptr, newsize-sizeof(T_CLICK_BOX) ) ;
		Free( ptr ) ;
	}

	ptrlcb = ptrmenu->PtrMallocList + ptrmenu->NbBox - 1 ;

	ptrlcb->X1 = x1 ;
	ptrlcb->Y1 = y1 ;
	ptrlcb->X2 = x2 ;
	ptrlcb->Y2 = y2 ;

	return ptrlcb ;
}

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	GetCoorButton( T_MENU *ptrmenu, WORD handle, LONG *x0, LONG *y0, LONG *x1, LONG *y1 )
{
	WORD		n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	*x0 = 0 ;
	*y0 = 0 ;
	*x1 = Screen_X ;
	*y1 = Screen_Y ;

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

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddButton(	T_MENU	*ptrmenu,
			WORD 	handle,
			WORD	x1, WORD y1,
			WORD	largeur, WORD hauteur,
			WORD	flags,
			UBYTE	*ptrstring	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = ptrstring ;
	ptrlcb->Type = TYPE_BUTTON ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddChangeValue(	T_MENU	*ptrmenu,
			WORD 	handle,
			WORD	x1, WORD y1,
			WORD	largeur, WORD hauteur,
			WORD	flags,
			UBYTE	*ptrstring,
			WORD	*ptrvar,
			WORD	step,
			WORD	minvar,
			WORD	maxvar	)
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

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddSwitch(	T_MENU	*ptrmenu,
			WORD 	handle,
			WORD	x1, WORD y1,
			WORD	largeur, WORD hauteur,
			WORD	flags,
			UBYTE	*ptrstring,
			WORD	*ptrvar,
			WORD	mask	)
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

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddText(	T_MENU	*ptrmenu,
			WORD	x1, WORD y1,
			WORD	largeur, WORD hauteur,
			WORD	flags,
			UBYTE	*ptrstring	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = -1 ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = ptrstring ;
	ptrlcb->Type = TYPE_TEXT ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

void	AddWindow(	T_MENU	*ptrmenu,
			WORD	handle,
			WORD	x1, WORD y1,
			WORD	largeur, WORD hauteur,
			WORD	flags	)
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = AddHeaderButton( ptrmenu, x1,y1, largeur, hauteur ) ;

	ptrlcb->Handle = handle ;
	ptrlcb->Flags = flags ;
	ptrlcb->PtrString = "" ;
	ptrlcb->Type = TYPE_WINDOW ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

WORD	GetOptimalSize( WORD deltax )
{
/*	return ((nbcar*SizeCar) / (SizeCar+7)) + 2 ;	nbcar */
	return (deltax-8)/(SizeCar+7) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

WORD	OpenMenu(	T_MENU *ptrmenu,
			WORD largeur, WORD hauteur )
{
	ptrmenu->LastClicked = -1 ;
	ptrmenu->Repeat = FALSE ;

	ptrmenu->NbBox = 0 ;

	ptrmenu->Largeur = largeur ;
	ptrmenu->Hauteur = hauteur ;

	ptrmenu->X1	= 0 ;
	ptrmenu->Y1	= 0 ;
	ptrmenu->X2	= 0 ;
	ptrmenu->Y2	= 0 ;
	ptrmenu->Flags	= 0 ;

	/* init une fausse clickbox pour first time */
	ptrmenu->PtrMallocList = (T_CLICK_BOX*)Malloc( 42*sizeof(T_CLICK_BOX) ) ;
	if( !ptrmenu->PtrMallocList )	return FALSE ;

	return TRUE ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	CloseMenu( T_MENU *ptrmenu )
{
	Free( ptrmenu->PtrMallocList ) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


WORD	DrawMenu( T_MENU *ptrmenu, WORD x1, WORD y1 )
{
	WORD		n, nb ;
	WORD		x2,y2 ;
	WORD		oldx1,oldy1 ;
	T_CLICK_BOX	*ptrlcb ;

/*	ptrmenu->LastClicked = -1 ;	*/


	oldx1 = ptrmenu->X1 ;
	oldy1 = ptrmenu->Y1 ;

	if( (x1==-1) AND (y1==-1) )	/* centre window */
	{
		x1 = (Screen_X/2) - ((ptrmenu->Largeur*(SizeCar+7)-3))/2 ;
		y1 = (Screen_Y/2) - ((ptrmenu->Hauteur*(SizeCar+7)-3))/2 ;
		x2 = x1 + ptrmenu->Largeur*(SizeCar+7)+6 ;
		y2 = y1 + ptrmenu->Hauteur*(SizeCar+7)+6 ;
	}
	else				/* normal window */
	{
		x1++ ;
		y1++ ;

		x2 = x1 + ptrmenu->Largeur*(SizeCar+7)+6 ;
		y2 = y1 + ptrmenu->Hauteur*(SizeCar+7)+6 ;
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

		DrawBox( ptrmenu, n, NO_FLAG, FALSE ) ;

		ptrlcb++ ;
	}

	CopyBlockPhys( x1-1,y1-1,x2+1,y2+1 ) ;

	return y2 + 1 ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void	ClearMenu( T_MENU *ptrmenu )
{
	ptrmenu->LastClicked = -1 ;

	Box(	ptrmenu->X1, ptrmenu->Y1,
		ptrmenu->X2, ptrmenu->Y2, 0 ) ;

	CopyBlockPhys(	ptrmenu->X1, ptrmenu->Y1,
			ptrmenu->X2, ptrmenu->Y2	) ;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* on scan TOUJOURS une list de click box en DECREMENTANT */

WORD	ScanListClick( T_MENU *ptrmenu )
{
	WORD		n ;
	T_CLICK_BOX	*ptrlcb ;
	WORD		x, y ;
	WORD		nb ;

	nb = ptrmenu->NbBox ;

	if( !nb )	return -2 ;

	x = Mouse_X ;
	y = Mouse_Y ;

	if( (x>=ptrmenu->X1) AND (x<=ptrmenu->X2)
	AND (y>=ptrmenu->Y1) AND (y<=ptrmenu->Y2) )
	{
		ptrlcb = ptrmenu->PtrMallocList + (nb-1) ;
		for( n=nb-1; n>=0; n-- )
		{
			if( (x >= ptrlcb->X1) AND (y >= ptrlcb->Y1)
			AND (x <= ptrlcb->X2) AND (y <=	ptrlcb->Y2) )
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

/*

WORD	ScanListKey( T_MENU *ptrmenu )
{
	WORD		n ;
	T_CLICK_BOX	*ptrlcb ;
	WORD		x, y ;
	WORD		nb ;

	nb = ptrmenu->NbBox ;

	if( !nb )	return -2 ;

	k = Key ;

	ptrlcb = ptrmenu->PtrMallocList ;
	for( n=0; n<nb; n++ )
	{
		if( ptrlcb->Key == k )	return ptrlcb->Handle ;
		ptrlcb++ ;
	}
	return -1 ;
}

WORD	ScanListKey_prog( T_MENU *ptrmenu )
{
	WORD		n ;
	T_CLICK_BOX	*ptrlcb ;
	WORD		x, y ;
	WORD		nb ;

	nb = ptrmenu->NbBox ;

	if( !nb )	return -2 ;

	k = Key ;

	ptrlcb = ptrmenu->PtrMallocList ;
	for( n=0; n<nb; n++ )
	{
		if( ptrlcb->Key == k )	return n ;
		ptrlcb++ ;
	}
	return -1 ;
}

*/

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/
#ifdef	ANCIEN
LONG	GereMenu( T_MENU *ptrmenu, WORD flagwaitclick )
{
	LONG		exitflag = -1 ;
	WORD		select ;

	T_CLICK_BOX	*ptrlcb ;
	T_CLICK_BOX	*ptrlcborg ;
	WORD		value ;
	WORD		vmin, vmax ;

	select = ScanListClick( ptrmenu ) ;

	if( select == -2 )	exitflag = -2 ;

	if( Click )
	{
		if( select > -1 )/* AND (select != ptrmenu->LastClicked) )*/
		{
			ptrlcb = ptrmenu->PtrMallocList + select ;
			exitflag = ptrlcb->Handle ;

			if( ptrmenu->LastClicked == select )
			{
				if( flagwaitclick )
				{
					/* methode du click/run/wait */
					while( Click ) AffMouse() ;
					return -1 ;
				}
				return	exitflag ;
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
					GetMouseDep() ;
					while( Click )
					{
						GetMouseDep() ;
						value += ((WORD)Mouse_X_Dep/4) * ptrlcb->Mask ;
						if( value < vmin ) value = vmin ;
						if( value > vmax ) value = vmax ;
						*(ptrlcb->PtrVar) = value ;
						DrawBox( ptrmenu, select, FLAG_PUSHED, TRUE ) ;
					}
					DrawBox( ptrmenu, select, NO_FLAG, TRUE ) ;
					break ;

				case TYPE_DEC_VALUE:
					ptrlcborg = GetPtrLcb( ptrmenu, ptrlcb->Handle ) ;
					value = *ptrlcb->PtrVar ;
					value -= ptrlcborg->Mask ; /* step */
					if( value < ptrlcb->Mask )
						value = ptrlcb->Mask ; /* minvar */
					*ptrlcb->PtrVar = value ;
					/* draw value box */
					DrawBox( ptrmenu, select-1, NO_FLAG, TRUE ) ;
					break ;

				case TYPE_INC_VALUE:
					ptrlcborg = GetPtrLcb( ptrmenu, ptrlcb->Handle ) ;
					value = *ptrlcb->PtrVar ;
					value += ptrlcborg->Mask ; /* step */
					if( value > ptrlcb->Mask )
						value = ptrlcb->Mask ; /* maxvar */
					*ptrlcb->PtrVar = value ;
					/* draw value box */
					DrawBox( ptrmenu, select-2, NO_FLAG, TRUE ) ;
					break ;
			}

			if( ptrlcb->Type != TYPE_WINDOW )
			{
				DrawBox( ptrmenu, select, FLAG_PUSHED, TRUE ) ;
				ptrmenu->LastClicked = select ;
			}
		}
	}
	else	/* pas de click */
	{
		if( ptrmenu->LastClicked != -1 )
		{
			DrawBox( ptrmenu, ptrmenu->LastClicked, NO_FLAG, TRUE ) ;
			ptrmenu->LastClicked = -1 ;
		}
	}

	return exitflag ;
}
#endif

/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

LONG	GereMenu( T_MENU *ptrmenu )
{
	LONG		exitflag = -1 ;
	WORD		select ;

	T_CLICK_BOX	*ptrlcb ;
	T_CLICK_BOX	*ptrlcborg ;
	LONG		value, ivalue ;
	WORD		vmin, vmax ;
	WORD		click ;
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
				if( !ptrmenu->Repeat )
				{
					/* methode du click/run/wait */
					while( Click ) AffMouse() ;
					return -1 ;
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
					if( click & 2 )	/* slide value */
					{
						GetMouseDep() ;
						while( Click )
						{
							GetMouseDep() ;
							value += (Mouse_X_Dep/4) * ptrlcb->Mask ;
							if( value < vmin ) value = vmin ;
							if( value > vmax ) value = vmax ;
							*(ptrlcb->PtrVar) = (WORD)value ;
							DrawBox( ptrmenu, select, FLAG_PUSHED, TRUE ) ;
						}
					}
					else		/* input value */
					{
						memostring = ptrlcb->PtrString ;
						itoa( value, InputValueString, 10 ) ;
						ptrlcb->PtrString = InputValueString ;
						ptrlcb->Type = TYPE_TEXT ;
						DrawBox( ptrmenu, select, FLAG_PUSHED, TRUE ) ;
						InputString( ptrmenu, ptrlcb->Handle , 63 ) ;
						ivalue = atoi( InputValueString ) ;
						if( ivalue < vmin ) ivalue = vmin ;
						if( ivalue > vmax ) ivalue = vmax ;
						*(ptrlcb->PtrVar) = ivalue ;
						ptrlcb->Type = TYPE_CHANGE_VALUE ;
						ptrlcb->PtrString = memostring ;
					}
					DrawBox( ptrmenu, select, NO_FLAG, TRUE ) ;
					break ;

				case TYPE_DEC_VALUE:
					ptrlcborg = GetPtrLcb( ptrmenu, ptrlcb->Handle ) ;
					value = *ptrlcb->PtrVar ;
					value -= ptrlcborg->Mask ; /* step */
					if( value < ptrlcb->Mask )
						value = ptrlcb->Mask ; /* minvar */
					*ptrlcb->PtrVar = value ;
					/* draw value box */
					DrawBox( ptrmenu, select-1, NO_FLAG, TRUE ) ;
					break ;

				case TYPE_INC_VALUE:
					ptrlcborg = GetPtrLcb( ptrmenu, ptrlcb->Handle ) ;
					value = *ptrlcb->PtrVar ;
					value += ptrlcborg->Mask ; /* step */
					if( value > ptrlcb->Mask )
						value = ptrlcb->Mask ; /* maxvar */
					*ptrlcb->PtrVar = value ;
					/* draw value box */
					DrawBox( ptrmenu, select-2, NO_FLAG, TRUE ) ;
					break ;
			}

			if( ptrlcb->Type != TYPE_WINDOW )
			{
				DrawBox( ptrmenu, select, FLAG_PUSHED, TRUE ) ;
				ptrmenu->LastClicked = select ;
			}
		}
	}
	else	/* pas de click */
	{
		if( ptrmenu->LastClicked != -1 )
		{
			DrawBox( ptrmenu, ptrmenu->LastClicked, NO_FLAG, TRUE ) ;
			ptrmenu->LastClicked = -1 ;
		}
	}

	return exitflag ;
}

