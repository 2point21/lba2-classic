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

/*══════════════════════════════════════════════════════════════════════════*/


T_CLICK_BOX	*GetPtrLcb( T_MENU *ptrmenu, WORD handle )
{
	WORD		n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			return ptrlcb ;
		}
		ptrlcb++ ;
	}
	return 0L ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void	OrButtonFlagIfTrue( T_MENU *ptrmenu, WORD handle, WORD flags, WORD test )
{
	WORD	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	if( test != 0 )
	{
		nblcb = ptrmenu->NbBox ;
		ptrlcb = ptrmenu->PtrMallocList ;
		for( n=0; n<nblcb; n++ )
		{
			if( ptrlcb->Handle == handle )
			{
				ptrlcb->Flags |= flags ;
				return ;
			}
			ptrlcb++ ;
		}
	}
}

void	SwitchButton( T_MENU *ptrmenu, WORD handle )
{
	WORD	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			ptrlcb->Flags ^= FLAG_PUSHED ;
/*			if( flagaff )
			{
				DrawBox( ptrmenu, n, NO_FLAG, TRUE ) ;
			}
*/
			return ;
		}
		ptrlcb++ ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ChangeButtonFlags( T_MENU *ptrmenu, WORD handle, WORD flags, WORD flagaff )
{
	WORD	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			ptrlcb->Flags = flags ;
			if( flagaff )
			{
				DrawBox( ptrmenu, n, NO_FLAG, TRUE ) ;
			}
			return ;
		}
		ptrlcb++ ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
/*
WORD	GetButtonFlags( T_MENU *ptrmenu, WORD handle )
{
	WORD	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			return ptrlcb->Flags ;
		}
		ptrlcb++ ;
	}
	return 0 ;
}
*/
/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ChangeButtonString( T_MENU *ptrmenu, WORD handle, UBYTE *string, WORD flagaff )
{
	WORD	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			ptrlcb->PtrString = string ;
			if( flagaff )
			{
				DrawBox( ptrmenu, n, NO_FLAG, TRUE ) ;
			}
			return ;
		}
		ptrlcb++ ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
/*
UBYTE*	GetButtonString( T_MENU *ptrmenu, WORD handle )
{
	WORD	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			return ptrlcb->PtrString ;
		}
		ptrlcb++ ;
	}
	return 0L ;
}
*/
/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	InputString( T_MENU *ptrmenu, WORD handle, WORD maxcar )
{
	WORD		n, nblcb, numlcb ;
	WORD		coul,x,y ;
	LONG		x1,y1,x2,y2 ;
	WORD		start,pc ;
	WORD		exit ;
	WORD		len ;
	WORD		nbaffcar ;
	WORD		carsizex ;
	WORD		myclick, mykey ;
	WORD		memoasciimode ;

	UBYTE		car ;
	UBYTE		string[256] ;

	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;
	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )	break ;
		ptrlcb++ ;
	}
	if( n==nblcb )	return ;

	numlcb = n ;
	strcpy( string, ptrlcb->PtrString ) ;

	pc = -1 ;			/* pos curseur (-1 no cursor) */
	start = 0 ;

	x1 = ptrlcb->X1+3 ;
	y1 = ptrlcb->Y1+3 ;

	x2 = ptrlcb->X2-3 ;
	y2 = ptrlcb->Y2-3 ;

	nbaffcar = (carsizex=((x2 - x1)+1)/SizeCar) * (((y2-y1)+1)/SizeCar) ;

	if( ptrlcb->Flags & FLAG_RED )	coul = B_ROUGE+1 ;
	else				coul = B_GRIS+1 ;

	memoasciimode = AsciiMode ;
	AsciiMode = TRUE ;
	CoulText( BLANC,-1 ) ;

	ShowMouse( 0 ) ;

	while( Click ) ;

	exit = FALSE ;
	while( !exit )
	{
		/* affichage */

/*		DrawInputString( string, nbffcar, start, pc,	*/

		len = strlen( string ) ;

		if( pc-start >= nbaffcar )	start = pc - nbaffcar + 1 ;
		if( pc < start )		start = pc ;

		Vsync() ;
		if( pc == -1 )
		{
			x = x1+(len*SizeCar) ;
			if( x>x2 )	x = x2 ;
			Box( x1,y1,x,y2, 9 ) ;
		}
		else
		{
			Box( x1,y1,x2,y2, coul ) ;
		}

		x = x1 ;
		y = y1 ;
		for( n=0; n<=len; n++ )
		{
			if( n < start )		continue ;
			if( n == pc )
			{
				Box(x,y,x+SizeCar-2,y+SizeCar-1,BLANC ) ;
				x += SizeCar ;
				if( x > x2-SizeCar-1 )
				{
					if( y > y2-SizeCar-1 )
					{
						break ;
					}
					else
					{
						y += SizeCar ;
						x = x1 ;
					}
				}
			}

			Text( x,y, "%c", string[n] ) ;
			x += SizeCar ;
			if( x > x2-SizeCar-1 )
			{
				y += SizeCar ;
				if( y > y2-SizeCar-1 )
				{
					break ;
				}
				else
				{
					x = x1 ;
				}
			}
		}
		CopyBlockPhys( ptrlcb->X1, ptrlcb->Y1, ptrlcb->X2, ptrlcb->Y2 ) ;

		/* input */

		while( !(mykey=GetAscii()) AND (!(myclick=Click)) ) ;

		if( myclick )
		{
			exit = TRUE ;
			break ;
		}

		car = mykey & 255 ;

		if( (car >= 32) AND (car < 255) )
		{
			if( pc == -1 )
			{
				pc = 1 ;
				string[0] = car ;
				string[1] = 0 ;
			}
			else
			{
				if( len < maxcar )
				{			/* insert */
					memmove( &string[pc+1],&string[pc],strlen(&string[pc])+1 ) ;
					string[pc] = car ;
					pc++ ;
					if( pc > len )	string[pc] = 0 ;
				}
			}
		}
		else
		{
			switch( mykey )
			{

			case A_ESC:
				exit = TRUE ;
				break ;

			case A_RETURN:
				exit = TRUE ;
				break ;

			case A_END:
				pc = len ;
				break ;

			case A_HOME:
				pc = 0 ;
				break ;

			case A_BACKSPACE:
				if( pc == -1 )	pc = len ;
				if( pc > 0 )
				{
					pc-- ;
					memmove( &string[pc],&string[pc+1],strlen(&string[pc+1])+1 ) ;
				}
				break ;

			case A_SUPPR:
				if( pc == -1 )	pc = len ;
				if( pc < len )
				{
					memmove( &string[pc],&string[pc+1],strlen(&string[pc+1])+1 ) ;
				}
				break ;

			case A_LEFT:
				if( pc == -1 )	pc = len-1 ;
				else
					if( pc > 0 )	pc-- ;
				break ;

			case A_RIGHT:
				if( pc == -1 )	pc = 1 ;
				else
					if( pc < len )	pc++ ;
				break ;

			case A_DOWN:
				if( pc == -1 )
				{
					if( carsizex < len )
						pc = carsizex ;
					else
						pc = 0 ;
				}
				else
					if( pc+carsizex < len )
						pc += carsizex ;
				break ;

			case A_UP:
				if( pc == -1 )	pc = 0 ;
				else
					if( pc-carsizex >= 0 )
						pc -= carsizex ;
				break ;
			}
		}

	}

	if( (mykey==A_RETURN) OR (myclick) )
	{
		strcpy( ptrlcb->PtrString, string ) ;
	}

	DrawBox( ptrmenu, numlcb, NO_FLAG, TRUE ) ;

	AsciiMode = memoasciimode ;
	ShowMouse( 1 ) ;

	while( Key OR Click OR Fire ) AffMouse() ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/* fontions evoluées */

void	Message( UBYTE *message, LONG flag )
{
	T_MENU	menu ;
	WORD	len ;

	len = strlen(message)*SizeCar / (SizeCar+7) + 2 ;

	if( !OpenMenu( &menu, len,2 ) )	return ;
	AddText( &menu, 0,0, len,2, FLAG_CENTRE+FLAG_CONTOUR, message ) ;

	DrawMenu( &menu, -1, -1 ) ;

	if( flag )
	{
		ShowMouse( 1 ) ;
		while( Click OR Fire OR Key ) ;
		while( !Click AND !Fire AND !Key ) AffMouse() ;
		while( Click OR Fire OR Key) ;
	}

	CloseMenu( &menu ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/* Confirm( "Voulez-vous vraiment ?", "OUI" (RETURN), "NON"(ESC) ) ; */

LONG	Confirm( UBYTE *message, UBYTE *oui, UBYTE *non )
{
	T_MENU	menu ;
	WORD	len, select ;

	len = strlen(message)*SizeCar / (SizeCar+7) + 2 ;

	if( !OpenMenu( &menu, len,3 ) )	return 2 ;
	AddText( &menu, 0,0, len,2, FLAG_CENTRE+FLAG_CONTOUR, message ) ;
	AddButton( &menu, 1, 0,2, len/2,1, FLAG_CENTRE+FLAG_RED, oui ) ;
	AddButton( &menu, 2, len/2,2, len/2,1, FLAG_CENTRE, non ) ;
	DrawMenu( &menu, -1, -1 ) ;

	ShowMouse( 1 ) ;
	while( Click OR Fire OR Key ) 	AffMouse() ;
	select = 0 ;
	while( select < 1 )
	{
		AffMouse() ;
		select = GereMenu( &menu ) ;

		if( Fire & F_RETURN )
		{
			select = 1 ;
			DrawBox( &menu, 1, FLAG_PUSHED, TRUE ) ;
		}
		if( Key == K_ESC )
		{
			select = 2 ;
			DrawBox( &menu, 2, FLAG_PUSHED, TRUE ) ;
		}
	}
	while( Click OR Fire OR Key ) 	AffMouse() ;

	CloseMenu( &menu ) ;

	return select ;
}

/*══════════════════════════════════════════════════════════════════════════*/




