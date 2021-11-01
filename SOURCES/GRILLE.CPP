#include	"c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
		      █▀▀▀▀ █▀▀▀█  █    █     █     █▀▀▀▀
		      ██ ▀█ ██▀█▀  ██   ██    ██    ██▀▀
		      ▀▀▀▀▀ ▀▀  ▀  ▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

#define	HEADER_BLOCK	3

#define	DXBLOCK		0
#define	DYBLOCK		1
#define	DZBLOCK		2

extern	S32	XS0,YS0,XS1,YS1,XS2,YS2,XS3,YS3 ;	// bezier.c

/*-------------------------------------------------------------------------*/

S32	XpOrgw ;
S32	YpOrgw ;

#define	NB_COLON	28
#define	MAX_BRICK	150

S16	NbBrickColon[NB_COLON] ;

typedef	struct
{
	S16	Xm ;
	S16	Ym ;
	S16	Zm ;
	S16	Ys ;
	S16	Brick ;
	S16	Col ;

}	T_COLONB ;

T_COLONB	ListBrickColon[NB_COLON][MAX_BRICK] ;

U8	*BufferMaskBrick ;
S32	NbBlock			;
S32	DxBlock, DyBlock, DzBlock;

S32	ForbidenBrick = 0 ;

extern	S32	Nxw, Nyw, Nzw ;
//void	ReajustPos( U8 col ) ;
/*--------------------------------------------------------------------------*/
typedef	struct
{
	U16	Gri_Start		;//	Index of GRI
	U16	Grm_Start		;//	Index of GRM
	U16	Bll_Start		;//	Index of BLL
	U16	Brk_Start		;//	Index of BRK
	U16	Max_Brk			;//	Nb Brk
	U16	ForbidenBrick		;//	NEW : Brick Interdite
	U32	Max_Size_Gri		;//	Max Size Gri
	U32	Max_Size_Bll		;//	Max Size Bll
	U32	Max_Size_Brick_Cube	;//	Max Ideal Style Size
	U32	Max_Size_Mask_Brick_Cube;//	Max Size Mask

} T_BKG_HEADER	;

T_BKG_HEADER	BkgHeader	;

typedef	struct
{
	U8	My_Bll		;//	Index Style ( + Bll_Start )
	U8	My_Grm		;//	Index Grm ( + Grm_Start )
	U8	UsedBlock[32]	;//	Bluck Used in Bll ( Bits )

} T_GRI_HEADER	;

T_GRI_HEADER	*GriHeader	;
/*--------------------------------------------------------------------------*/
/*══════════════════════════════════════════════════════════════════════════*
	     █   █ █▀▀▀█ █▀▀▀█       █▀▀█  █▀▀▀█  █    █▀▀▀▀ █  ▄▀
	     ██▀▀█ ██ ▄█ ██▀█▀       ██▀▀█ ██▀█▀  ██   ██    ██▀▄
	     ▀▀  ▀ ▀▀▀▀  ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀  ▀▀   ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
#define	MAX_BRICK_GAME		20000L
#define	OFFSET_BUFFER_FLAG	153800L

/*--------------------------------------------------------------------------*/
S32	LoadUsedBrick()
{
	U32	i, j, b	;
	U8	*pt	;
	U8	*ptb	;
	U16	*ptw	;
	U32	*ptoff	;
	U8	*ptdata	;
	U32	offset	;
	U32	maxbrk	;
	U32	brick	;
	U32	offseek	;
	U32	*ptseek	;
	U32	handle	;
	U16	*tabflag;
	U16	*ptflag	;
	U32	min, max;
	U8 	*ptdecomp ;
	COMPRESSED_HEADER header	;
	S32	nbbrick	;

	tabflag = (U16*)((U8*)Screen+OFFSET_BUFFER_FLAG) ;

	memset( tabflag, 0, MAX_BRICK_GAME*2L )	;/* Table de U16 pour NewNumBrick */

	min = 60000			;
	max = 0				;

//-------------------------------------- Premiere Passe, Préparation ptflag

	pt = GriHeader->UsedBlock	;/* Debut de Used Block */

	nbbrick = 0			;

	for ( i = 1 ; i < 256 ; i++ )
	{
		b = pt[i>>3] & (1<<(7-(i&7)))	;/*	Recup Bit Block	*/
		if ( !b )	continue	;

		ptb = TabBlock + *(U32 *)(TabBlock+((i-1)<<2));
		ptw = (U16 *)(ptb + 5)	;/* Jump dx dy dz et collis */
		maxbrk = *ptb * *(ptb+1) * *(ptb+2)	;/* dx*dy*dz*/

		for ( j = 0 ; j < maxbrk ; j++, ptw += 2 )
		{
			brick = *ptw	;

			if ( brick )
			{
				brick--					;

				if ( brick < min )	min = brick	;
				if ( brick > max )	max = brick	;

				if ( tabflag[brick] == 0 )
				{
					nbbrick++		;
					tabflag[brick] = 1	;
				}
			}
		}
	}

//-------------------------------------- Deuxieme Passe, Load Brick

	handle = OpenRead( BKG_HQR_NAME )			;
	if ( !handle )	return(0L)			;

	Seek( handle, BkgHeader.Brk_Start<<2, SEEK_START);// Begin offsets Brk

	Read( handle, Screen, BkgHeader.Max_Brk<<2 )	;// Read Offsets Brk

	ptflag = tabflag+min		;

	ptoff = (U32 *)BufferBrick	;

	offset = (nbbrick+1)*4L		;

	*ptoff++ = offset		;/* First Offset        */

	ptdata = BufferBrick+offset	 ;/* Jump nbbrick+1 Offset	*/

	nbbrick = 0			;

	ptseek = (U32 *)Screen	;

	for ( i = min ; i <= max ; i++, ptflag++ )
	{
		if ( *ptflag )
		{
			nbbrick++		;/*	One More*/
			*ptflag = (S16)nbbrick	;/*	Brick+1, for BLL */
			offseek = *(ptseek+i)	;
			Seek( handle, offseek, SEEK_START ) 	;
			Read( handle, &header, sizeof(header))	;
			switch( header.CompressMethod )
			{
				case 0://	No compression
					Read( handle, ptdata, header.SizeFile )	;
					break			;
				case 1://	Compression LZSS
					ptdecomp = ptdata+header.SizeFile-header.CompressedSizeFile+500 ;
					Read( handle, ptdecomp, header.CompressedSizeFile )	;
//					ExpandLZSS( ptdecomp, ptdata, header.SizeFile )	;
					ExpandLZ( ptdata, ptdecomp, header.SizeFile, 2 )	;
					break		;
			}
			ptdata += header.SizeFile	;
			offset += header.SizeFile	;
			*ptoff++ = offset		;
		}
	}

//-------------------------------------- Troisieme passe, Rename Block

	pt = GriHeader->UsedBlock		;/* Debut de Used Block */

	for ( i = 1 ; i < 256 ; i++ )
	{
		b = pt[i>>3] & (1<<(7-(i&7)))	;/*	Recup Bit Block	*/
		if ( !b )	continue	;

		ptb = TabBlock + *(U32 *)(TabBlock+((i-1)<<2));
		ptw = (U16 *)(ptb + 5)			;/* Jump dx dy dz et collis */
		maxbrk = *ptb * *(ptb+1) * *(ptb+2)		;/* dx*dy*dz*/

		for ( j = 0 ; j < maxbrk ; j++, ptw += 2 )
		{
			if ( *ptw )	*ptw = tabflag[(*ptw)-1]	;
		}
	}

	Close(handle)	;

	ForbidenBrick = tabflag[BkgHeader.ForbidenBrick]	;

	return(offset)	;
}
/*-------------------------------------------------------------------------*/
void	InitGrille( U32 numcube )
{
	U32	id_gri	;
	U32	id_bll	;

	numcube = TabAllCube[numcube].Num	;//	Indirection

	id_gri = BkgHeader.Gri_Start+numcube			;

	GriHeader = (T_GRI_HEADER *)BufMap			;

	Load_HQR( BKG_HQR_NAME, BufMap, id_gri )			;

	ManageSystem() ;// for streaming

	id_bll = BkgHeader.Bll_Start+GriHeader->My_Bll		;//+numcube

	Load_HQR( BKG_HQR_NAME, TabBlock, id_bll )		;

//----------------------------------------------	NEW
	if( !LoadUsedBrick() )	return 		;
//----------------------------------------------

	ManageSystem() ;// for streaming

	CreateMaskGph( BufferBrick, BufferMaskBrick ) ;

	ManageSystem() ;// for streaming

	NbBlock = (*(U32*)TabBlock)/4		;

	CopyMapToCube()				;

	ManageSystem() ;// for streaming

	RedrawGRMs() ;// au cas ou il y ait des GRMs actives

	return	;
}

/*--------------------------------------------------------------------------*/
void	FreeGrille(){}
/*-------------------------------------------------------------------------*/
/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▀ ██▄ █ █▀▀▀▄       █   █ █▀▀▀█ █▀▀▀█
		   ██▀▀  ██▀██ ██  █       ██▀▀█ ██ ▄█ ██▀█▀
		   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀        ▀▀  ▀ ▀▀▀▀  ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*--------------------------------------------------------------------------*/
/*══════════════════════════════════════════════════════════════════════════*
	     █▀▀▀▀ █   █ ██▄ █ █▀▀▀▀ ▀▀█▀▀  █    █▀▀▀█ ██▄ █ ██▀▀▀
	     ██▀▀  ██  █ ██▀██ ██      ██   ██   ██  █ ██▀██ ▀▀▀▀█
	     ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀   ▀▀   ▀▀   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

/*--------------------------------------------------------------------------*/
U8	*GetAdrColonneMap( S32 x, S32 z )
{
	U32	offset	;

	offset = *(U16*)(BufMap+sizeof(T_GRI_HEADER)+(x+(z*SIZE_CUBE_Z))*2);

	return(BufMap+sizeof(T_GRI_HEADER)+offset )	;
}
/*--------------------------------------------------------------------------*/
U8	*GetAdrColonneCube( S32 x, S32 z )
{
	U32	offset	;

	offset = (x*SIZE_CUBE_Y*2)+(z*SIZE_CUBE_Z*SIZE_CUBE_Y*2);

	return(BufCube+offset)			;
}

/*--------------------------------------------------------------------------*/
U8	WorldColBrickVisible( 	S32 oldx, S32 oldy, S32 oldz,
				S32 xw, S32 yw, S32 zw )
{
	U8 col = GetWorldColBrickVisible( xw, yw, zw ) ;

	if( col )	return( col ) ;

	if( abs(xw-oldx)>=SIZE_BRICK_XZ
	OR  abs(yw-oldy)>=SIZE_BRICK_Y
	OR  abs(zw-oldz)>=SIZE_BRICK_XZ )
	{
		return( WorldColBrickVisible( 	oldx, oldy, oldz,
					oldx+(xw-oldx)/2,
					oldy+(yw-oldy)/2,
					oldz+(zw-oldz)/2 ) ) ;
	}

	return( 0 ) ;// Ok, cette fois, on est sûr !:)
}

/*--------------------------------------------------------------------------*/
/*U8	WorldColBrickVisible( S32 xw, S32 yw, S32 zw )
{
	if( GetBlockBrick( xw, yw, zw ) )
	{
		return( WorldColBrick( xw, yw, zw ) ) ;
	}
	else	return 0 ;
}
*/

/*--------------------------------------------------------------------------*/

void	GetShadow( S32 xw, S32 yw, S32 zw )
{
	S32	xm, ym, zm ;
	U8	*ptc ;
	S32	y ;
	U8	*adr ;
	S32	block ;

	xm = xw/SIZE_BRICK_XZ ;
	ym = yw/SIZE_BRICK_Y ;
	zm = zw/SIZE_BRICK_XZ ;

	ShadowX = Nxw = xw ;
	ShadowZ = Nzw = zw ;

	if( ym>=SIZE_CUBE_Y )	// au-dessus du plafond de la map
	{
		ym = SIZE_CUBE_Y-1 ;
	}

	ptc = BufCube + ym*2 + (xm*SIZE_CUBE_Y*2) + (zm*SIZE_CUBE_X*SIZE_CUBE_Y*2) ;

	for( y = ym; y>0; y-- )
	{
		if( *ptc != 0 )	break ;
		ptc -= 2 ;
	}

	XMap = xm ;
	YMap = y ;
	ZMap = zm ;

	Nyw = (y+1) * SIZE_BRICK_Y ;

	ShadowCol = 0 ;

	if( *ptc != 0 )
	{
		block = (*ptc++) - 1 ;
		adr = GetAdrBlock( block )	;
		adr += HEADER_BLOCK		;
		adr += (*ptc)<<2 		;/* 4 Bytes to Jump	*/
		ShadowCol = *adr ;
		ReajustPos( ShadowCol ) ;
	}

	ShadowY = AdjustShadowObjects( xw, zw, Nyw, yw ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
/*--------------------------------------------------------------------------*/
S32	ProjectShadow( T_SORT *ptrs, S32 beta )
{
	S32	xmin, zmin, xmax, zmax, xw, yw, zw ;
	S32	ys, cy		;
	S32	altitude_ref	;
	S32	demix, demiz 	;

	ClearScreenMinMax();

	xw = ptrs->Xw ;
	yw = ptrs->Yw ;
	zw = ptrs->Zw ;

	xmin = ptrs->XMin - xw ;
	zmin = ptrs->ZMin - zw ;
	xmax = ptrs->XMax - xw ;
	zmax = ptrs->ZMax - zw ;

	demix = (xmin+xmax)/2 ;
	demiz = (zmin+zmax)/2 ;

	// inclure ici la gestion des objets du decors
	altitude_ref = AdjustShadowObjects( xw, zw, yw, yw ) ;

	Rotate( demix, zmin, beta );

	X0 += xw			;
	Z0 += zw			;
	ys  = altitude_ref+SIZE_BRICK_Y	;

	GetShadow(X0, ys, Z0)		;
	cy = AdjustShadowObjects( X0, Z0, ShadowY, yw ) ;

	if( cy < altitude_ref-ALTITUDE_MAX ) cy = altitude_ref-ALTITUDE_MAX;
	else if( cy > altitude_ref+ALTITUDE_MAX ) cy = altitude_ref+ALTITUDE_MAX;

	if( !LongProjectPoint( X0,cy,Z0 ) )	return FALSE ;
	XS0 = Xp ;
	YS0 = Yp ;
	AdjustScreenMinMax();

	Rotate( xmin, demiz, beta );

	X0 += xw	;
	Z0 += zw	;

	GetShadow(X0, ys, Z0)	;
	cy = AdjustShadowObjects( X0, Z0, ShadowY, yw ) ;

	if( cy < altitude_ref-ALTITUDE_MAX ) cy = altitude_ref-ALTITUDE_MAX;
	else if( cy > altitude_ref+ALTITUDE_MAX ) cy = altitude_ref+ALTITUDE_MAX;

	if( !LongProjectPoint( X0,cy,Z0 ) )	return FALSE ;
	XS1 = Xp ;
	YS1 = Yp ;
	AdjustScreenMinMax();

	Rotate( demix, zmax, beta );

	X0 += xw	;
	Z0 += zw	;

	GetShadow(X0, ys, Z0)	;
	cy = AdjustShadowObjects( X0, Z0, ShadowY, yw ) ;

	if( cy < altitude_ref-ALTITUDE_MAX ) cy = altitude_ref-ALTITUDE_MAX;
	else if( cy > altitude_ref+ALTITUDE_MAX ) cy = altitude_ref+ALTITUDE_MAX;

	if( !LongProjectPoint( X0,cy,Z0 ) )	return FALSE ;
	XS2 = Xp ;
	YS2 = Yp ;
	AdjustScreenMinMax();

	Rotate( xmax, demiz, beta );

	X0 += xw	;
	Z0 += zw	;

	GetShadow(X0, ys, Z0)	;
	cy = AdjustShadowObjects( X0, Z0, ShadowY, yw ) ;

	if( cy < altitude_ref-ALTITUDE_MAX ) cy = altitude_ref-ALTITUDE_MAX;
	else if( cy > altitude_ref+ALTITUDE_MAX ) cy = altitude_ref+ALTITUDE_MAX;

	if( !LongProjectPoint( X0,cy,Z0 ) )	return FALSE ;
	XS3 = Xp ;
	YS3 = Yp ;
	AdjustScreenMinMax();

	UnsetClip() ;
	SetClip( ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax ) ;

	return TRUE ;
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▄ ▄█ █▀▀▀▀ █▄ ▄█ █▀▀▀█  █    █▀▀▀█ █▀▀▀▀
		   ██▀ █ ██▀▀  ██▀ █ ██  █  ██   ██▀█▀ ██▀▀
		   ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

U32	CreateMaskGph( U8 *ptsrc, U8 *ptdst )
{
	U8	*ptd	;
	U32   nbg, off;
	U32	*ptoff	;
	U32	size, i	;

	ptoff = (U32*)ptdst		;

	off = *(U32 *)ptsrc		;/*	First Offset Src	*/

	*ptoff++ = off			;/*	First Offset	*/

	ptd = ptdst+off			;

	nbg = (off-4)>>2		;/*	Nombre de Graph	*/

	for ( i = 0 ; i < nbg ; i++ )
	{
		size = CalcGraphMsk( i, ptsrc, ptd )	;

		off += size			;/*	Maj Offset	*/
		*ptoff++ = off			;/*	Write Offset 	*/
		ptd += size			;/*	Maj Pt Dest	*/
	}
	return(off)	;
}

/*-------------------------------------------------------------------------*/
void	CopyMapToCube()
{
	U8	*pts, *ptd	;
	S32	x, z		;

/*-------------- Copy -------------------------*/
	for ( z = 0 ; z < SIZE_CUBE_Z ; z++ )
	{
		for ( x = 0 ; x < SIZE_CUBE_X ; x++ )
		{
			pts = GetAdrColonneMap( x, z )		;
			ptd = GetAdrColonneCube( x, z )		;

			DecompColonne( pts, ptd )		;
		}
	}
}
/*──────────────────────────────────────────────────────────────────────────*/
void	InitBufferCube()
{
	S32	intmem ;

//	pas de RECOVER_AREA car BkgHeader en stored et pas en LZSS
	Load_HQR( BKG_HQR_NAME, &BkgHeader, 0 )			;// HEADER

// 	pas de RECOVER_AREA car TabAllCube en stored et pas en LZSS
	Load_HQR( BKG_HQR_NAME, (void *)TabAllCube, BkgHeader.Brk_Start+BkgHeader.Max_Brk ) ;

// 	Calcul des tailles nécessaires
	intmem = (BkgHeader.Max_Size_Brick_Cube+RECOVER_AREA)	// BufferBrick
		+(SIZE_CUBE_X*SIZE_CUBE_Y*SIZE_CUBE_Z*2L) ;	// BufCube

//	PtrZvBuffer sert aussi pour les décors intérieurs
	ListMem[MEM_PTR_ZBUFFER].Size = __max(intmem,SIZE_Z_BUFFER) ;

	ListMem[MEM_BUFFER_MASK_BRICK].Size 	= BkgHeader.Max_Size_Mask_Brick_Cube ;
	ListMem[MEM_BUF_MAP].Size 		= BkgHeader.Max_Size_Gri + RECOVER_AREA ;
	ListMem[MEM_TAB_BLOCK].Size 		= BkgHeader.Max_Size_Bll + RECOVER_AREA ;
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▄  █    ██▀▀▀ █▀▀▀█ █     █▀▀▀█ █  ▄▀
		   ██  █  ██   ▀▀▀▀█ ██▀▀▀ ██    ██▀▀█ ██▀
		   ▀▀▀▀   ▀▀   ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DrawOverBrick( S32 xm, S32 ym, S32 zm )
{
	register T_COLONB	*ptrlbc ;
	register S32		colscreen ;
	S32		col, i ;
	S32		startcol, endcol ;
	S32		xw, yw, zw ;
	S32		recouvre ;

	startcol = (ClipXMin+24)/24-1 ;
	endcol = (ClipXMax+24)/24 ;

	for( col = startcol; col <= endcol; col++ )
	{
		ptrlbc = &ListBrickColon[col][0] ;

		colscreen = col*24-24 ;

		for( i=0; i<NbBrickColon[col]; i++ )
		{
			/* bricks devant */
			if( ptrlbc->Ys+38 > ClipYMin
			AND ptrlbc->Ys <= ClipYMax )
			{
			recouvre = FALSE ;

			// essai correction bugs escaliers
//			if( WorldColBrick(ptrlbc->Xm*SIZE_BRICK_XZ,ptrlbc->Ym*SIZE_BRICK_Y,ptrlbc->Zm*SIZE_BRICK_XZ) > 1 )
			if( ptrlbc->Col > 1 )// collis complexe ?
			{
				if( ptrlbc->Ym >= ym+1 )
				{
					recouvre = TRUE ;
				}
			}
			else if( ptrlbc->Ym >= ym )
			{
				recouvre = TRUE ;
			}

			if( recouvre )
			{
			xw = ptrlbc->Xm*SIZE_BRICK_XZ ;
			yw = ptrlbc->Ym*SIZE_BRICK_Y  ;
			zw = ptrlbc->Zm*SIZE_BRICK_XZ ;

			if( (ptrlbc->Xm>=xm AND ptrlbc->Zm>=zm)
			OR (ptrlbc->Xm>xm
				AND WorldColBrick(xw-SIZE_BRICK_XZ,yw,zw)!=1
//				AND WorldColBrick(xw,yw,zw+SIZE_BRICK_XZ)==1 )
				AND WorldColBrick(xw,yw,zw+SIZE_BRICK_XZ)>=1 )
			OR (ptrlbc->Zm>zm
				AND WorldColBrick(xw,yw,zw-SIZE_BRICK_XZ)!=1
//				AND WorldColBrick(xw+SIZE_BRICK_XZ,yw,zw)==1 ) )
				AND WorldColBrick(xw+SIZE_BRICK_XZ,yw,zw)>=1 ) )
			{
				CopyMask( ptrlbc->Brick, colscreen, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}
			}
			}
			ptrlbc++ ;
		}
	}
}

/*--------------------------------------------------------------------------*/
/* recouvrement reel pour obj qui ne depassent pas la ZV */

void	DrawOverBrick3( S32 xm, S32 ym, S32 zm, S32 ymax )
{
	register T_COLONB	*ptrlbc ;
	register S32		colscreen ;
	S32		col, i ;
	S32		startcol, endcol ;

	startcol = (ClipXMin+24)/24-1 ;
	endcol = (ClipXMax+24)/24 ;

	for( col = startcol; col <= endcol; col++ )
	{
		ptrlbc = &ListBrickColon[col][0] ;

		colscreen = col*24-24 ;

		for( i=0; i<NbBrickColon[col]; i++, ptrlbc++ )
		{
			/* bricks devant */
			if( ptrlbc->Ys+38 > ClipYMin
			AND ptrlbc->Ys <= ClipYMax )
			{
				if( ptrlbc->Ym > ymax )
				{
					if( (ptrlbc->Zm >= zm)
					OR  (ptrlbc->Xm >= xm) )
					{
						CopyMask( ptrlbc->Brick, colscreen, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
					}
				}
				else if( ptrlbc->Ym >= ym )
				{
					if( (ptrlbc->Zm == zm)
					AND (ptrlbc->Xm == xm) )
					{
						CopyMask( ptrlbc->Brick, colscreen, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
					}

					if( (ptrlbc->Zm > zm)
					OR  (ptrlbc->Xm > xm) )
					{
						CopyMask( ptrlbc->Brick, colscreen, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
					}
				}
			}
		}
	}
}

/*--------------------------------------------------------------------------*/
// recouvrement reel pour obj qui ne depassent pas la ZV
// GROSSE RUSTINE: Cage à la fin de LBA2 pour ne pas recouvrir l'objet avec
// les briques du sol en 0

void	DrawOverBrickCage( S32 xm, S32 ym, S32 zm )
{
	register T_COLONB	*ptrlbc ;
	register S32		colscreen ;
	S32		col, i ;
	S32		startcol, endcol ;

	startcol = (ClipXMin+24)/24-1 ;
	endcol = (ClipXMax+24)/24 ;

	for( col = startcol; col <= endcol; col++ )
	{
		ptrlbc = &ListBrickColon[col][0] ;

		colscreen = col*24-24 ;

		for( i=0; i<NbBrickColon[col]; i++, ptrlbc++ )
		{
			/* bricks devant */
			if( ptrlbc->Ys+38 > ClipYMin
			AND ptrlbc->Ys <= ClipYMax )
			{
			if( ptrlbc->Ym > 0 // diff ici avec DrawOverBrick3
			AND ptrlbc->Ym >= ym )
			{

			if( (ptrlbc->Zm == zm)
			AND (ptrlbc->Xm == xm) )
			{
				CopyMask( ptrlbc->Brick, colscreen, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}

			if( (ptrlbc->Zm > zm)
			OR  (ptrlbc->Xm > xm) )
			{
				CopyMask( ptrlbc->Brick, colscreen, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}

			}
			}
		}
	}
}

/*--------------------------------------------------------------------------*/

void	AffBrickBlock( S32 block, S32 brick, S16 x, S16 y, S16 z )
{
	U8	*adr	;
	S32	nb, col ;
	register T_COLONB	*ptrlbc ;
	S16	numbrick;
	U8      brickcol ;

	adr = GetAdrBlock( block )	;
	adr += HEADER_BLOCK		;

	adr += ( brick<<2 )		;/* 4 Bytes to Jump	*/
	brickcol = *adr			;
	adr += 2			;

	numbrick = *(U16*)adr		;	/* & 32767 */

	if ( numbrick AND numbrick!=ForbidenBrick )
	{
		Map2Screen( x-StartXCube, y-StartYCube, z-StartZCube )	;

		if( XScreen >= -24
		AND XScreen < 640
		AND YScreen >= -38
		AND YScreen < 480 )
		{
			AffGraph( numbrick-1, XScreen, YScreen, BufferBrick ) ;

			col = (XScreen+24)/24 ; /* 48 / 2 colonne intercalée */

			nb = NbBrickColon[col] ;

			ptrlbc = &ListBrickColon[col][nb] ;

			if( nb < MAX_BRICK )
			{
				ptrlbc->Xm   = x ;
				ptrlbc->Ym   = y ;
				ptrlbc->Zm   = z ;
				ptrlbc->Ys    = (S16)YScreen ;
				ptrlbc->Brick = (S16)(numbrick-1) ;
				ptrlbc->Col  = brickcol ;
				NbBrickColon[ col ]++ ;
			}
			else	Message( "Arg MAX_BRICK Z BUFFER atteint", TRUE ) ;
		}
	}
}

/*--------------------------------------------------------------------------*/
/*ptc = BufCube + y*2 + (x*SIZE_CUBE_Y*2) + (z*SIZE_CUBE_X*SIZE_CUBE_Y*2) ;*/

void	AffGrille()
{
	S32	block		;
	U8	*ptc		;
	S16	z, y, x		;

	PtrProjectPoint( 0, 0, 0 ) ;
	XpOrgw = Xp ;
	YpOrgw = Yp ;

	memset( NbBrickColon, 0, sizeof(S16)*NB_COLON ) ;

	ptc = BufCube		;
	for ( z = 0 ; z < SIZE_CUBE_Z ; z++ )
	{
		for ( x = 0 ; x < SIZE_CUBE_X ; x++ )
		{
			for ( y = 0 ; y < SIZE_CUBE_Y ; y++ )
			{
				block = *ptc++	;

				if( block )
				{
					AffBrickBlock( block-1, *ptc, x, y, z )	;
				}
				ptc++	;
			}
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*
	  █     █▀▀▀█ █▀▀█  █  ▄▀ █▀▀▀█  █    ██▄ █ ▀▀█▀▀ █   █ █▀▀▀▀
	  ██    ██▀▀█ ██▀▀█ ██▀   ██▀█▀  ██   ██▀██   ██  ██▀▀█ ██▀▀
	  ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀    ▀▀  ▀  ▀▀   ▀▀  ▀   ▀▀  ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

/*--------------------------------------------------------------------------*/
// Calcule les masques du Z-Buffer pour DrawOverBrick sans dessiner la
// brick
void	AffBrickBlockColon( S32 block, S32 brick, S16 x, S16 y, S16 z )
{
	U8	*adr	;
	S32	nb, col ;
	register T_COLONB	*ptrlbc ;
	S16	numbrick;
	U8      brickcol ;

	adr = GetAdrBlock( block )	;
	adr += HEADER_BLOCK		;

	adr += ( brick<<2 )		;/* 4 Bytes to Jump	*/
	brickcol = *adr			;
	adr += 2			;

	numbrick = *(U16*)adr		;	/* & 32767 */

	if ( numbrick AND numbrick!=ForbidenBrick )
	{
		Map2Screen( x-StartXCube, y-StartYCube, z-StartZCube )	;

		if( XScreen >= -24
		AND XScreen < 640
		AND YScreen >= -38
		AND YScreen < 480 )
		{
			col = (XScreen+24)/24 ; /* 48 / 2 colonne intercalée */

			nb = NbBrickColon[col] ;

			ptrlbc = &ListBrickColon[col][nb] ;

			if( nb < MAX_BRICK )
			{
				ptrlbc->Xm   = x ;
				ptrlbc->Ym   = y ;
				ptrlbc->Zm   = z ;
				ptrlbc->Ys    = (S16)YScreen ;
				ptrlbc->Brick = (S16)(numbrick-1) ;
				ptrlbc->Col  = brickcol ;
				NbBrickColon[ col ]++ ;

			}
			else	Message( "Arg MAX_BRICK Z BUFFER atteint", TRUE ) ;
		}
	}
}

// Calcule le Z-Buffer pour le DrawOverBrick de toute la grille sans
// afficher les bricks
void	AffGrilleOnly()
{
	S32	block		;
	U8	*ptc		;
	U8	*adr ;
	S16	z, y, x		;
	U8	codejeu		;

	PtrProjectPoint( 0, 0, 0 ) ;
	XpOrgw = Xp ;
	YpOrgw = Yp ;

	memset( NbBrickColon, 0, sizeof(S16)*NB_COLON ) ;

	ptc = BufCube		;

	for ( z = 0 ; z < SIZE_CUBE_Z ; z++ )
	{
		for ( x = 0 ; x < SIZE_CUBE_X ; x++ )
		{
			for ( y = 0 ; y < SIZE_CUBE_Y ; y++ )
			{
				block = *ptc++	;

				if( block )
				{
					block-- ;

					//-------------------
					adr = GetAdrBlock( block )	;
					adr += (HEADER_BLOCK+1)		;

					codejeu = (U8)((*adr)>>4) 	;

					if( codejeu!=CJ_LABYRINTHE )
					//-------------------
					{
						AffBrickBlockColon( block, *ptc, x, y, z )	;
					}
				}
				ptc++	;
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
// Affiche la brick sans calculer le Z-Buffer pour DrawOverBrick
void	AffBrickBlockOnly( S32 block, S32 brick, S16 x, S16 y, S16 z )
{
	U8	*adr	;
	S32	numbrick;

	adr = GetAdrBlock( block )	;
	adr += HEADER_BLOCK		;

	adr += ( brick<<2 )		;/* 4 Bytes to Jump	*/
	adr += 2			;

	numbrick = *(U16*)adr		;	/* & 32767 */

	if( numbrick AND numbrick!=ForbidenBrick )
	{
		Map2Screen( x-StartXCube, y-StartYCube, z-StartZCube )	;

		if( XScreen >= -24
		AND XScreen < 640
		AND YScreen >= -38
		AND YScreen < 480 )
		{
			AffGraph( numbrick-1, XScreen, YScreen, BufferBrick ) ;
		}
	}
}

// Redessine uniquement les block n'ayant pas le code jeu Invisible
void	AffGrilleLabyrinthe()
{
	S32	block		;
	U8	*ptc		;
	U8	*adr ;
	S16	z, y, x		;
	U8	codejeu		;

	PtrProjectPoint( 0, 0, 0 ) ;
	XpOrgw = Xp ;
	YpOrgw = Yp ;

	ptc = BufCube		;

	for ( z = 0 ; z < SIZE_CUBE_Z ; z++ )
	{
		for ( x = 0 ; x < SIZE_CUBE_X ; x++ )
		{
			for ( y = 0 ; y < SIZE_CUBE_Y ; y++ )
			{
				block = *ptc++	;

				if( block )
				{
					block-- ;

					adr = GetAdrBlock( block )	;
					adr += (HEADER_BLOCK+1)		;

					codejeu = (U8)((*adr)>>4) ;

					if( codejeu!=CJ_LABYRINTHE )
					{
						AffBrickBlockOnly( block, *ptc, x, y, z )	;
					}
				}
				ptc++	;
			}
		}
	}
}

// Affiche un cube de block (3*3*3) autour du point passe (coordonnees world)
void	AffOneBrick( S32 x, S32 y, S32 z )
{
	S32	block		;
	U8	*ptc		;
	U8	*adr	;
	S32	xmin, ymin, xmax, ymax ;
	S16	xs, ys, zs ;
	S16	zdeb ;
	U8	codejeu	;

	xmin = 639 ;
	ymin = 479 ;
	xmax = 0 ;
	ymax = 0 ;

	PtrProjectPoint( 0, 0, 0 ) ;
	XpOrgw = Xp ;
	YpOrgw = Yp ;

	x /= SIZE_BRICK_XZ ;
	y /= SIZE_BRICK_Y  ;
	z /= SIZE_BRICK_XZ ;

	zdeb = (S16)(z-1) ;
	if( zdeb<0 )	zdeb = 0 ;

	for( zs=zdeb; zs<=z+1; zs++ )
	{
		for( xs=(S16)(x-1); xs<=(S16)(x+1); xs++ )
		{
			ptc = GetAdrColonneCube( xs, zs )		;
			ptc += (y-1)*2 ;
			for( ys=(S16)(y-1); ys<=(S16)(y+1); ys++ )
			{
				block = *ptc++	;
				if( block )
				{
					block-- ;

					adr = GetAdrBlock( block )	;
					adr += (HEADER_BLOCK+1)		;

					codejeu = (U8)((*adr)>>4) ;

					if( codejeu==CJ_LABYRINTHE )
					{
						AffBrickBlockOnly( block, *ptc, xs, ys, zs )	;
						if( ScreenXMin<xmin )	xmin = ScreenXMin ;
						if( ScreenXMax>xmax )	xmax = ScreenXMax ;
						if( ScreenYMin<ymin )	ymin = ScreenYMin ;
						if( ScreenYMax>ymax )	ymax = ScreenYMax ;
					}
				}
				ptc++ ;
			}
		}
	}

	SetClip( xmin, ymin, xmax, ymax ) ;
	DrawRecover( (x+1)*SIZE_BRICK_XZ, y*SIZE_BRICK_Y, (z+1)*SIZE_BRICK_XZ, 0, 0 ) ;
}

// Fin Labyrinthe

/*--------------------------------------------------------------------------*/

void	IncrustGrm( T_ZONE *ptrz )
{
	S32 numgrm, xo, yo, zo	;
	U8	*pts, *ptd	;
	S32	x, z	 	;
	S32	dx, dy, dz	;

	numgrm = ptrz->Info0		;

	Load_HQR( BKG_HQR_NAME, Screen,
			BkgHeader.Grm_Start+GriHeader->My_Grm+numgrm ) ;

	xo = ptrz->X0/SIZE_BRICK_XZ	;
	yo = ptrz->Y0/SIZE_BRICK_Y	;
	zo = ptrz->Z0/SIZE_BRICK_XZ	;

	pts = (U8*)Screen	;

	dx = *pts++	;
	dy = *pts++	;
	dz = *pts++	;

	yo <<= 1	;//	Cause S16
	dy <<= 1	;//	Cause S16

	for ( z = zo ; z < zo+dz ; z++ )
	{
		for ( x = xo ; x < xo+dx ; x++ )
		{
			ptd = GetAdrColonneCube( x, z )	;
			ptd += yo			;
			memcpy( ptd, pts, dy )		;
			pts += dy			;
		}
	}

	FirstTime = AFF_ALL_FLIP ;
}

/*--------------------------------------------------------------------------*/
void	DesIncrustGrm( T_ZONE *ptrz )
{
	U8	*pts, *ptd	;
	U8	col[50]		;//	1 Colonne decompressee
	S32	x, y, z, dy	;
	S32	z0, z1, x0, x1	;
	S32	y0, y1		;

	x0 = ptrz->X0/SIZE_BRICK_XZ	;
	x1 = ptrz->X1/SIZE_BRICK_XZ	;

	y0 = ptrz->Y0/SIZE_BRICK_Y	;
	y1 = ptrz->Y1/SIZE_BRICK_Y	;

	z0 = ptrz->Z0/SIZE_BRICK_XZ	;
	z1 = ptrz->Z1/SIZE_BRICK_XZ	;

	y = y0 << 1			;//	Cause S16

	dy = ((y1-y0)+1)<<1		;

/*-------------- Copy -------------------------*/
	for ( z = z0 ; z <= z1  ; z++ )
	{
		for ( x = x0 ; x <= x1 ; x++ )
		{
			pts = GetAdrColonneMap( x, z )	;
			DecompColonne( pts, col )	;

			ptd = GetAdrColonneCube( x, z )	;
			memcpy( ptd+y, col+y, dy )	;

		}
	}

	FirstTime = AFF_ALL_FLIP ;
}

/*--------------------------------------------------------------------------*/
void	RedrawGRMs( void )
{
	S32	n ;
	T_ZONE	*ptrz = ListZone ;

	for( n=0; n<NbZones; n++, ptrz++ )
	{
		switch( ptrz->Type )
		{
			case 3:	// GRM
				if( ptrz->Info2 )
				{
					IncrustGrm( ptrz ) ;
				}
				break ;
		}
	}
}
/*--------------------------------------------------------------------------*/
