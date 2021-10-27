#include	"c_extern.h"

//***************************************************************************
S32	PofDisplay3DExt( S32 x, S32 y, S32 z, S32 num_pof, S32 scale, S32 rotation )
{
	S32	coul, n;
	S32	x0, y0, scale3d ;
	S32	xpc, ypc ;
	S16	*ptr_point, *ptrw;
	U8	p;
	U8	*ptr_pof ;

//	ptr_pof = (U8*)HQR_Get( HQR_Pof, num_pof ) ;

	ptr_pof = BufferPof + *(((U32*)BufferPof)+num_pof) ;

	LongWorldRotatePoint( x, y, z ) ;
	x0 = Z0;

	if( !LongProjectPoint( X0, Y0+1000, Z0 ) )	return FALSE ;
	y0 = Yp;

	if( !LongProjectPoint( X0, Y0, Z0 ) )	return FALSE ;
	xpc = Xp*65536+32768;
	ypc = Yp*65536+32768;

	scale3d = ((Yp-y0)*65536+scale/2)/scale ;

	coul = *ptr_pof++; // couleur

	n = *ptr_pof++; // nb point

	ClearScreenMinMax() ;

	if( rotation != 0 )
	{
		ptrw = (S16*)ptr_pof;
		ptr_point = (S16*)Obj_ListRotatedPoints;

		for( p=0; p<n; p++ )
		{
			Rotate( ptrw[0], ptrw[1], rotation );

			Xp = (xpc+X0*scale3d)/65536 ;
			Yp = (ypc+Z0*scale3d)/65536 ;

			AdjustScreenMinMax() ;

			*ptr_point++ = (S16)Xp ;
			*ptr_point++ = (S16)Yp ;
			ptrw += 2;
		}

		ptr_pof = (U8*)ptrw;
	}
	else
	{
		ptrw = (S16*)ptr_pof;
		ptr_point = (S16*)Obj_ListRotatedPoints;

		for( p=0; p<n; p++ )
		{
			Xp = (xpc+(*ptrw++)*scale3d)/65536 ;
			Yp = (ypc+(*ptrw++)*scale3d)/65536 ;

			AdjustScreenMinMax() ;

			*ptr_point++ = (S16)Xp ;
			*ptr_point++ = (S16)Yp ;
		}

		ptr_pof = (U8*)ptrw;
	}

	n = *ptr_pof++; // nb line
	while( n>0 )
	{
		p = *ptr_pof++;

		ptr_point = (S16*)Obj_ListRotatedPoints+p*2 ;

		x0 = *ptr_point++ ;
		y0 = *ptr_point ;

		p = *ptr_pof++;

		ptr_point = (S16*)Obj_ListRotatedPoints+p*2 ;

		Xp = *ptr_point++ ;
		Yp = *ptr_point ;

		Line( x0, y0, Xp, Yp, coul );

		n--;
	}

	return(TRUE);
}
