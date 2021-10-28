#include	<system.h>
#include	<svga.h>
#include	<menu.h>

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<dos.h>

/*══════════════════════════════════════════════════════════════════════════*/


T_CLICK_BOX	*GetPtrLcb( T_MENU *ptrmenu, S32 handle )
{
	S32		n, nblcb ;
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

void	SwitchButton( T_MENU *ptrmenu, S32 handle )
{
	S32	n, nblcb ;
	T_CLICK_BOX	*ptrlcb ;

	nblcb = ptrmenu->NbBox ;
	ptrlcb = ptrmenu->PtrMallocList ;

	for( n=0; n<nblcb; n++ )
	{
		if( ptrlcb->Handle == handle )
		{
			ptrlcb->Flags ^= FLAG_PUSHED ;

			return ;
		}
		ptrlcb++ ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ChangeButtonFlags( T_MENU *ptrmenu, S32 handle, S32 flags, S32 flagaff )
{
	S32	n, nblcb ;
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
				DrawBox( ptrmenu, n, NO_FLAG) ;
			}
			return ;
		}
		ptrlcb++ ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
//	Remise en fonction le 09/10/95
S32	GetButtonFlags( T_MENU *ptrmenu, S32 handle )
{
	S32	n, nblcb ;
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

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	ChangeButtonString( T_MENU *ptrmenu, S32 handle, char *string, S32 flagaff )
{
	S32	n, nblcb ;
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
				DrawBox( ptrmenu, n, NO_FLAG) ;
			}
			return ;
		}
		ptrlcb++ ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
//	Remise en fonction le 09/10/95
char	*GetButtonString( T_MENU *ptrmenu, S32 handle )
{
	S32	n, nblcb ;
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
/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	InputString( T_MENU *ptrmenu, S32 handle, S32 maxcar )
{
	S32		n, nblcb, numlcb ;
	S32		coul,x,y ;
	S32		x1,y1,x2,y2 ;
	S32		start,pc ;
	S32		exit ;
	S32		len ;
	S32		nbaffcar ;
	S32		carsizex ;
	S32		myclick, mykey ;
	S32		memoasciimode ;

	char		car ;
	char		string[256] ;
	S32	memoclipxmin ;
	S32	memoclipxmax ;
	S32	memoclipymin ;
	S32	memoclipymax ;

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

	nbaffcar = (carsizex=((x2 - x1)+1)/SizeChar) * (((y2-y1)+1)/SizeChar) ;

	if( ptrlcb->Flags & FLAG_RED )	coul = B_RED+1 ;
	else				coul = B_GREY+1 ;

	memoasciimode = AsciiMode ;
	AsciiMode = TRUE ;
	CoulText( WHITE,-1 ) ;

	while( Click )
	{
		BoxUpdate()	;
	}

	memoclipxmin = ClipXMin ;
	memoclipxmax = ClipXMax ;
	memoclipymin = ClipYMin ;
	memoclipymax = ClipYMax ;

	UnsetClip()	;

	exit = FALSE ;

	Box( x1,y1,x2,y2, coul ) 	;
	BoxStaticAdd( x1,y1,x2,y2 )	;

	while( !exit )
	{
		/* affichage */

		len = strlen( string ) ;

		if( pc-start >= nbaffcar )	start = pc - nbaffcar + 1 ;
		if( pc < start )		start = pc ;

		if( pc == -1 )
		{
			x = x1+(len*SizeChar) ;
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
			if( n < start ) 	continue ;
			if( n == pc )
			{
				Box(x,y,x+SizeChar-2,y+SizeChar-1,WHITE ) ;
				x += SizeChar ;
				if( x > x2-SizeChar-1 )
				{
					if( y > y2-SizeChar-1 )
					{
						break ;
					}
					else
					{
						y += SizeChar ;
						x = x1 ;
					}
				}
			}

			GraphPrintf( FALSE, x,y, "%c", string[n] ) ;
			x += SizeChar ;
			if( x > x2-SizeChar-1 )
			{
				y += SizeChar ;
				if( y > y2-SizeChar-1 )
				{
					break ;
				}
				else
				{
					x = x1 ;
				}
			}
		}

		BoxStaticAdd( ptrlcb->X1, ptrlcb->Y1, ptrlcb->X2, ptrlcb->Y2 ) ;

		/* input */

		do
		{
			mykey	= GetAscii()	;
			myclick	= Click		;
			BoxUpdate()		;
		} while( !(mykey OR myclick) )	;

		if( myclick )
		{
			exit = TRUE ;
			break ;
		}

		car = (char)mykey ;

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

	DrawBox( ptrmenu, numlcb, NO_FLAG) ;

	AsciiMode = memoasciimode ;

	while( Key OR Click )
	{
		BoxUpdate()	;
	}

	SetClip(memoclipxmin, memoclipymin, memoclipxmax, memoclipymax)	;

}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
/* fontions evoluées */

void	Message( char *message, S32 flag )
{
	T_MENU	menu ;
	S32	len ;

	len = strlen(message)*SizeChar / (SizeChar+7) + 2 ;

	if( !OpenMenu( &menu, len,2 ) ) return ;
	AddText( &menu, 0,0, len,2, FLAG_CENTRE+FLAG_CONTOUR, message ) ;

	DrawMenu( &menu, -1, -1 ) ;

	if( flag )
	{
		while( Click OR Key )
		{
			BoxUpdate()	;
		}

		while( !Click AND !Key )
		{
			BoxUpdate()	;
		}

		CloseMenu(&menu)	;

		while( Click OR Key)	;
		{
			BoxUpdate()	;
		}
	}
	else
	{
		Free( menu.PtrMallocList );
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/* Confirm( "Voulez-vous vraiment ?", "OUI" (RETURN), "NON"(ESC) ) ; */

S32	Confirm( char *message, char *oui, char *non )
{
	T_MENU	menu ;
	S32	len, select ;

	len = strlen(message)*SizeChar / (SizeChar+7) + 2 ;

	if( !OpenMenu( &menu, len,3 ) ) return 2 ;
	AddText( &menu, 0,0, len,2, FLAG_CENTRE+FLAG_CONTOUR, message ) ;
	AddButton( &menu, 1, 0,2, len/2,1, FLAG_CENTRE+FLAG_RED, oui ) ;
	AddButton( &menu, 2, len/2,2, len/2,1, FLAG_CENTRE, non ) ;
	DrawMenu( &menu, -1, -1 ) ;

	ShowMouse( 1 ) 		;

	do
	{
		BoxUpdate()	;
	}
	while( Click OR Key )	;

	do
	{
		select = GereMenu( &menu ) ;
		if( Key == K_ENTER )
		{
			select = 1 ;
			DrawBox( &menu, 1, FLAG_PUSHED) ;
		}
		if( Key == K_ESC )
		{
			select = 2 ;
			DrawBox( &menu, 2, FLAG_PUSHED) ;
		}
		BoxUpdate()	;
	}
	while( select < 1 )	;

	CloseMenu( &menu ) 	;

	do
	{
		BoxUpdate()	;
	}
	while( Click OR Key )	;

	return select 		;
}

/*══════════════════════════════════════════════════════════════════════════*/

void ChangeButtonValueLimit( T_MENU *ptrmenu, S32 handle, S32 valmin, S32 valmax )
{
	T_CLICK_BOX	*ptrlcb ;

	ptrlcb = GetPtrLcb( ptrmenu, handle ) ;

	if( (ptrlcb != 0) AND (ptrlcb->Type==TYPE_CHANGE_VALUE) )
	{
		ptrlcb++; // next button (TYPE_DEC_VALUE)
		if( ptrlcb->Type == TYPE_DEC_VALUE )	ptrlcb->Mask = valmin; /* minvar */
		else					return;
		ptrlcb++; // next button (TYPE_INC_VALUE)
		if( ptrlcb->Type == TYPE_INC_VALUE ) ptrlcb->Mask = valmax; /* maxvar */
		else					return;
	}
}
