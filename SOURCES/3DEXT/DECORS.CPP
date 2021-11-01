#include "defines.h"

/*══════════════════════════════════════════════════════════════════════════*
		      █▀▀▀▄ █▀▀▀▀ █▀▀▀▀ █▀▀▀█ █▀▀▀█ ██▀▀▀
		      ██  █ ██▀▀  ██    ██  █ ██▀█▀ ▀▀▀▀█
		      ▀▀▀▀  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

#ifdef	LBA_EDITOR	// Special PERSO.EXE
#define	INFO_DECORS	1024
extern	S32	FlagInfos ;

#define SIZE_NAME               19

typedef	struct
{
	char   Name[SIZE_NAME+1] ;
	S16	Value ;			}	T_VAR ;

extern	T_VAR	ListVarGame[] ;
#else
extern	S16	ListVarGame[] ;
#endif

extern	S32	FlagMasque ;

#define	MAX_SOUS_OBJET	20
#define	SIZE_BRICK_Y	256

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
// affiche les objets de decors

#ifdef	LBA_EDITOR	// uniquement PERSO
extern	void	ClearScreenMinMax() ;
extern	void	AdjustScreenMinMax() ;

S32	NumDecorsDraw ;

//****************************************************************************
void	DrawCubeNoBox( 	S32 ox, S32 oy, S32 oz,
			S32 xw0, S32 yw0, S32 zw0,
			S32 xw1, S32 yw1, S32 zw1	)
{
	S32	x0,y0,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7 ;
	S16	coul = 11 ;
	S16	coul1 = 13 ;

	ClearScreenMinMax() ;
	LongWorldRotatePoint( ox,oy,oz ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	Rect( Xp-3, Yp-3, Xp+3, Yp+3, coul ) ;	// point org

	CoulText( GREEN, -1 ) ;
	GraphPrintf( FALSE, Xp, Yp, "%d", NumDecorsDraw ) ;

	LongWorldRotatePoint( xw0,yw0,zw0 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x0 = Xp ;
	y0 = Yp ;
	LongWorldRotatePoint( xw1,yw0,zw0 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x1 = Xp ;
	y1 = Yp ;
	LongWorldRotatePoint( xw1,yw0,zw1 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x2 = Xp ;
	y2 = Yp ;
	LongWorldRotatePoint( xw0,yw0,zw1 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x3 = Xp ;
	y3 = Yp ;
	LongWorldRotatePoint( xw0,yw1,zw0 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x4 = Xp ;
	y4 = Yp ;
	LongWorldRotatePoint( xw1,yw1,zw0 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x5 = Xp ;
	y5 = Yp ;
	LongWorldRotatePoint( xw1,yw1,zw1 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x6 = Xp ;
	y6 = Yp ;
	LongWorldRotatePoint( xw0,yw1,zw1 ) ;
	if( !LongProjectPoint( X0,Y0,Z0 ) )	return ;
	AdjustScreenMinMax() ;
	x7 = Xp ;
	y7 = Yp ;

	Line( x0,y0, x1,y1, coul ) ;
	Line( x1,y1, x2,y2, coul ) ;
	Line( x2,y2, x3,y3, coul ) ;
	Line( x3,y3, x0,y0, coul ) ;

	Line( x0,y0, x4,y4, coul1 ) ;
	Line( x1,y1, x5,y5, coul1 ) ;
	Line( x2,y2, x6,y6, coul1 ) ;
	Line( x3,y3, x7,y7, coul1 ) ;
	Line( x4,y4, x5,y5, coul1 ) ;
	Line( x5,y5, x6,y6, coul1 ) ;
	Line( x6,y6, x7,y7, coul1 ) ;
	Line( x7,y7, x4,y4, coul1 ) ;
}


// Version ZVs absolues
void	AffZVObjet( T_DECORS *ptrobj )
{
	DrawCubeNoBox(	ptrobj->Xworld, ptrobj->Yworld, ptrobj->Zworld,
			ptrobj->XMin, ptrobj->YMin, ptrobj->ZMin,
			ptrobj->XMax, ptrobj->YMax, ptrobj->ZMax	) ;
}
#endif


/*══════════════════════════════════════════════════════════════════════════*/
void	FixeObjetsDecorsInvisibles( void )
{
	S32 i ;
	T_DECORS	*ptrobj ;
	S32	numvar ;

	ptrobj = ListDecors ;

	for( i=0; i<NbObjDecors; i++, ptrobj++ )
	{
		ptrobj->Body &= ~(DEC_DRAWN|DEC_INVISIBLE) ;

		// Regarde si objet visible/invisible
		numvar = ptrobj->Beta>>16 ;

		if( numvar )
		{
#ifdef	LBA_EDITOR
			if( ((numvar<0) AND !ListVarGame[-numvar].Value)
			OR  ((numvar>=0) AND ListVarGame[numvar].Value) )
#else
			if( ((numvar<0) AND !ListVarGame[-numvar])
			OR  ((numvar>=0) AND ListVarGame[numvar]) )
#endif
				ptrobj->Body |= DEC_INVISIBLE ;
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
// sous routine de tri temporaire

int	sub_qsort_decors( const void *a, const void *b )
{
	return( ((T_LIST_TRI *)a)->Zrot - ((T_LIST_TRI *)b)->Zrot ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/
// sous routine de tri temporaire

int	sub_qsort_decors_zbuf( const void *a, const void *b )
{
	return( ((T_LIST_TRI *)b)->Zrot - ((T_LIST_TRI *)a)->Zrot ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

// Version ZVs absolues
int	sub_qsort_zv_decors( const void *a, const void *b )
{
	T_DECORS	*ptrobja ;
	T_DECORS	*ptrobjb ;

	S32	x0a, y0a, z0a, x1a, y1a, z1a ;
	S32	x0b, y0b, z0b, x1b, y1b, z1b ;

	ptrobja = ((T_LIST_TRI *)a)->PtrDec ;
	ptrobjb = ((T_LIST_TRI *)b)->PtrDec ;

	x0a = ptrobja->XMin ;
	y0a = ptrobja->YMin ;
	z0a = ptrobja->ZMin ;
	x1a = ptrobja->XMax ;
	y1a = ptrobja->YMax ;
	z1a = ptrobja->ZMax ;

	x0b = ptrobjb->XMin ;
	y0b = ptrobjb->YMin ;
	z0b = ptrobjb->ZMin ;
	x1b = ptrobjb->XMax ;
	y1b = ptrobjb->YMax ;
	z1b = ptrobjb->ZMax ;

	if( x0a >= x1b )	goto pas_rec_sur_x ;
	if( x1a <= x0b )	goto pas_rec_sur_x ;

	if( z0a >= z1b )	goto pas_rec_sur_z ;
	if( z1a <= z0b )	goto pas_rec_sur_z ;

//	ordre sur y

	// affinement decision sur une des 2 faces paralelle
	if( abs(y0a-CameraY) < abs(y1a-CameraY) )
		return( abs(y0b-CameraY) - abs(y0a-CameraY) ) ;
	else
		return( abs(y1b-CameraY) - abs(y1a-CameraY) ) ;

pas_rec_sur_z:

//	ordre sur z

	if( abs(z0a-CameraZ) < abs(z1a-CameraZ) )
		return( abs(z0b-CameraZ) - abs(z0a-CameraZ) ) ;
	else
		return( abs(z1b-CameraZ) - abs(z1a-CameraZ) ) ;

pas_rec_sur_x:

	if( z0a >= z1b )	goto pas_rec_sur_z_2 ;
	if( z1a <= z0b )	goto pas_rec_sur_z_2 ;

	if( abs(x0a-CameraZ) < abs(x1a-CameraZ) )
		return( abs(x0b-CameraX) - abs(x0a-CameraX) ) ;
	else
		return( abs(x1b-CameraX) - abs(x1a-CameraX) ) ;

	pas_rec_sur_z_2:

	// plus de methode dispo (obj en collision), utilise zrot

	return( ((T_LIST_TRI *)b)->Zrot - ((T_LIST_TRI *)a)->Zrot ) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

#ifdef	LBA_EDITOR
void	AffichageObjetDecorsZV( void )
{
	S32 i ;
	T_LIST_TRI	*ptrtri ;
	T_DECORS	*ptrobj ;

	// affiche les zv des objets statiques
	ptrtri = ListTriExt ;
	for( i=0; i<NbObjScreen; i++, ptrtri++ )
	{
		ptrobj = ptrtri->PtrDec ;

		if( ptrobj->Body&DEC_DRAWN )
		{
			UnsetClip() ;
			NumDecorsDraw = ptrtri->Num ;
			AffZVObjet( ptrobj ) ;
		}
	}

	UnsetClip() ;
}
#endif

/*══════════════════════════════════════════════════════════════════════════*/

// Version masques compris


void	AffichageObjetDecorsZBuf( void )
{
	S32 i ;
	T_LIST_TRI	*ptrtri ;
	T_DECORS	*ptrobj ;
	S32	clipzstart ;
	S32	numvar ;
	S32	zr ;

	ObjPtrMap = ObjTexture;		// texture obj

	ptrtri = ListTriExt ;
	ptrobj = ListDecors ;

	// projection des centres
	NbObjScreen = 0 ;
	for (i=0;i<NbObjDecors;i++,ptrobj++)
	{
		ptrobj->Body &= ~(DEC_DRAWN|DEC_INVISIBLE) ;

		// Regarde si objet visible/invisible
		numvar = ptrobj->Beta>>16 ;

		if( numvar )
		{
#ifdef	LBA_EDITOR
			if( ((numvar<0) AND !ListVarGame[-numvar].Value)
			OR  ((numvar>=0) AND ListVarGame[numvar].Value) )
#else
			if( ((numvar<0) AND !ListVarGame[-numvar])
			OR  ((numvar>=0) AND ListVarGame[numvar]) )
#endif
				ptrobj->Body |= DEC_INVISIBLE ;
//			else	ptrobj->Body &= ~(DEC_INVISIBLE) ;
		}

		// SEB le 12/04/96
		if( !(ptrobj->Body&DEC_INVISIBLE) )
		{
			LongWorldRotatePoint(	ptrobj->XMin,
						ptrobj->YMin,
						ptrobj->ZMin ) ;
			zr = CameraZr - Z0 ;
			if( zr > ClipZFar )	continue ;

			LongWorldRotatePoint(	ptrobj->XMin,
						ptrobj->YMin,
						ptrobj->ZMax ) ;
			zr = CameraZr - Z0 ;
			if( zr > ClipZFar )	continue ;

			LongWorldRotatePoint(	ptrobj->XMax,
						ptrobj->YMin,
						ptrobj->ZMin ) ;
			zr = CameraZr - Z0 ;
			if( zr > ClipZFar )	continue ;

			LongWorldRotatePoint(	ptrobj->XMax,
						ptrobj->YMin,
						ptrobj->ZMax ) ;
			zr = CameraZr - Z0 ;
			if( zr > ClipZFar )	continue ;


			// point chaud obj pour calcul profondeur fog
			LongWorldRotatePoint(	ptrobj->Xworld,
						ptrobj->Yworld,
						ptrobj->Zworld ) ;

			if( LongProjectPoint( X0,Y0,Z0 ) )
			{
				// Test preclipping a mettre ici
//				if( (Xp>VIEW_EXT_X0) AND (Xp<VIEW_EXT_X1)
//				AND (Yp>VIEW_EXT_Y0) AND (Yp<VIEW_EXT_Y1) )
				{
					// add dans listtri
					ptrtri->Zrot = CameraZr - Z0 ;
					ptrtri->Num = i ;
					ptrtri->PtrDec = ptrobj ;
					ptrtri++ ;
					NbObjScreen++ ;
				}
			}
		}
	}

	if( !NbObjScreen )	return ;

	// trie les objet en z 'front to back'
	qsort( ListTriExt, NbObjScreen, sizeof( T_LIST_TRI ), sub_qsort_decors_zbuf ) ;

	clipzstart = ClipZFar - StartZFog ;

//	Switch_Fillers( FILL_POLY_FOG_ZBUFFER );

	// affiche les objets statiques
	ptrtri = ListTriExt ;
	for( i=0; i<NbObjScreen; i++, ptrtri++ )
	{
		UnsetClip() ;
		ptrobj = ptrtri->PtrDec ;

		SetCLUT( PalLevel +
				16 * BoundRegleTrois(0,15,
						clipzstart,
						ptrtri->Zrot - StartZFog )
				) ;

		if( BodyDisplay(	ptrobj->Xworld,
					ptrobj->Yworld,
					ptrobj->Zworld,
					0,
					(ptrobj->Beta&0xFFFF), 0,
					HQR_Get( HQR_Isle_Obj, ptrobj->Body&0xFFFF ) ) )
		{
			SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;
			if( ClipXMin <= ClipXMax
			AND ClipYMin <= ClipYMax )
			{
				ptrobj->Body |= DEC_DRAWN ;
			}
		}
	}

//	Switch_Fillers( FILL_TEXTURES_ON );

	UnsetClip() ;
}

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █▀▀▀█ █     █      █    ██▀▀▀  █    █▀▀▀█ ██▄ █ ██▀▀▀
	  ██    ██  █ ██    ██     ██   ▀▀▀▀█  ██   ██  █ ██▀██ ▀▀▀▀█
	  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀  ▀▀   ▀▀▀▀▀  ▀▀   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

// Version ZVs absolues précalculées
// on teste par rapport a un seul point
S32	TestZVDecors( S32 xw, S32 yw, S32 zw, T_DECORS *objet )
{
	if( objet->Body & DEC_INVISIBLE )	return FALSE ;

	if( objet->XMin > xw )	return	FALSE ;
	if( objet->XMax < xw )	return	FALSE ;

	if( objet->YMin > yw )	return	FALSE ;
	if( objet->YMax < yw )	return	FALSE ;

	if( objet->ZMin > zw )	return	FALSE ;
	if( objet->ZMax < zw )	return	FALSE ;

	return TRUE ;
}
