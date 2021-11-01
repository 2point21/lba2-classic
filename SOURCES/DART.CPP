#include 	"c_extern.h"

extern	S32	Nxw, Nyw, Nzw ;	// Object.c

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █     █▀▀▀▀ █▀▀▀▀ █   █ █▀▀▀▀ ▀▀█▀▀ ▀▀█▀▀ █▀▀▀▀ ██▀▀▀
	  ██▀▀  ██    ██▀▀  ██    ██▀▀█ ██▀▀    ██    ██  ██▀▀  ▀▀▀▀█
	  ▀▀    ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀   ▀▀    ▀▀  ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
void	InitDarts()
{
	T_DART *ptrd ;
	U8	*ptrbody ;
	U32	t ;
	S32	x0,x1,y0,y1,z0,z1 ;
	S32	size ;
	T_BODY_HEADER	*ptr ;

#ifdef	LBA_EDITOR
	PtrBody[NbBodys] = (U8 *)LoadTestMalloc( BODY_3D_DART ) ;
	if( !PtrBody[NbBodys] )	TheEndCheckFile( BODY_3D_DART ) ;
	ptrbody = PtrBody[NbBodys] ;
#else
	ptrbody = (U8*)GivePtrObjFix( BODY_3D_DART ) ;
	if( !ptrbody )	TheEndCheckFile( PATH_RESSOURCE"ObjFix.hqr" ) ;
#endif
	// Calcule ZV des flechettes
	ptr = (T_BODY_HEADER *)ptrbody ;

	x0 =  ptr->XMin ;
	x1 =  ptr->XMax ;
	y0 =  ptr->YMin ;
	y1 =  ptr->YMax ;
	z0 =  ptr->ZMin ;
	z1 =  ptr->ZMax ;

	// moyenne
	size = ((x1 - x0) + (z1 - z0)) / 4 ;

	ptrd = ListDart ;

	for( t=0; t<MAX_DARTS; t++, ptrd++ )
	{
#ifdef	LBA_EDITOR
		ptrd->Body = NbBodys ;
#else
		ptrd->Body = BODY_3D_DART ;
#endif

		ptrd->XMin = -size ;
		ptrd->XMax =  size ;
		ptrd->YMin =  y0   ;
		ptrd->YMax =  y1   ;
		ptrd->ZMin = -size ;
		ptrd->ZMax =  size ;

		ptrd->Flags   =  0 ;
		ptrd->NumCube = -1 ;
	}

#ifdef	LBA_EDITOR
	NbBodys++ ;
#endif
}

// Recupere premiere flechette libre dans la structure
S32	GetDart()
{
	T_DART *ptrd ;
	U32	t ;

	ptrd = ListDart ;

	for( t=0; t<MAX_DARTS; t++, ptrd++ )
	{
		if( ptrd->Flags&DART_TAKEN )
		{
			return t ;
		}
	}

	return -1 ;
}

// Met Toutes les flechettes in the pocket de Twinsen
void	TakeAllDarts()
{
	T_DART	*ptrd ;
	S32	n ;

	ptrd = ListDart ;

	for( n=0; n<MAX_DARTS; n++, ptrd++ )
	{
		ptrd->Flags |= DART_TAKEN ;
	}

#ifdef	LBA_EDITOR
	ListVarGame[FLAG_DART].Value = MAX_DARTS ;
#else
	ListVarGame[FLAG_DART] = MAX_DARTS ;
#endif
}

void	CheckDartCol( T_OBJET *ptrobj )
{
	S32	n ;
	T_DART	*ptrd ;
	S32	x0,y0,z0, x1,y1,z1 ;
	S32	xt0,yt0,zt0, xt1,yt1,zt1 ;

	if( ptrobj->Flags&INVISIBLE )	return ;

/*	x0 = Nxw + ptrobj->XMin ;
	x1 = Nxw + ptrobj->XMax ;
	y0 = Nyw + ptrobj->YMin ;
	y1 = Nyw + ptrobj->YMax ;
	z0 = Nzw + ptrobj->ZMin ;
	z1 = Nzw + ptrobj->ZMax ;
*/
	x0 = ptrobj->Obj.X + ptrobj->XMin ;
	x1 = ptrobj->Obj.X + ptrobj->XMax ;
	y0 = ptrobj->Obj.Y + ptrobj->YMin ;
	y1 = ptrobj->Obj.Y + ptrobj->YMax ;
	z0 = ptrobj->Obj.Z + ptrobj->ZMin ;
	z1 = ptrobj->Obj.Z + ptrobj->ZMax ;

	ptrd = ListDart ;

	for( n=0; n<MAX_DARTS; n++,ptrd++ )
	{
		if( ptrd->NumCube==NumCube
		AND !(ptrd->Flags&DART_TAKEN) )
		{
			xt0 = ptrd->PosX + ptrd->XMin ;
			xt1 = ptrd->PosX + ptrd->XMax ;
			yt0 = ptrd->PosY + ptrd->YMin ;
			yt1 = ptrd->PosY + ptrd->YMax ;
			zt0 = ptrd->PosZ + ptrd->ZMin ;
			zt1 = ptrd->PosZ + ptrd->ZMax ;

			if( x0 < xt1
			AND x1 > xt0
			AND y0 < yt1
			AND y1 > yt0
			AND z0 < zt1
			AND z1 > zt0 )
			{
				ptrd->Flags |= DART_TAKEN ;

#ifdef	LBA_EDITOR
				ListVarGame[FLAG_DART].Value++ ;

				HQ_3D_MixSample( COMMON_NUM_DEF, SAMPLE_BONUS_TROUVE, 0x1000, 0, 1,
					ptrd->PosX, ptrd->PosY, ptrd->PosZ ) ;
#else
				ListVarGame[FLAG_DART]++ ;

				HQ_3D_MixSample( SAMPLE_BONUS_TROUVE, 0x1000, 0, 1,
					ptrd->PosX, ptrd->PosY, ptrd->PosZ ) ;
#endif

				InitIncrustDisp( INCRUST_SPRITE|INCRUST_YCLIP,
						 SPRITE_DART,
						 15, 30,
						 0, 0, 2 ) ;
			}
		}
	}
}

#ifdef	LBA_EDITOR
void	DrawZVDart( T_DART *ptrd )
{
	DrawCube( ptrd->XMin+ptrd->PosX, ptrd->YMin+ptrd->PosY,
		  ptrd->ZMin+ptrd->PosZ, ptrd->XMax+ptrd->PosX,
		  ptrd->YMax+ptrd->PosY, ptrd->ZMax+ptrd->PosZ, 0 ) ;

	BoxMovingAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
}
#endif
