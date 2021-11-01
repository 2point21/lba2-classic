
#include	"c_extern.h"

#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
#include	<i86.h>

#include	"grille.h"

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

/*-------------------------------------------------------------------------*/
/* prototype */

UBYTE	*GetAdrBlock( LONG numblock ) ;
UBYTE	GetColBrick( WORD xm, WORD ym, WORD zm ) ;
ULONG	CreateMaskGph( UBYTE *pt, UBYTE *dest )	;
/*-------------------------------------------------------------------------*/

LONG	StartXCube = 0	;
LONG	StartYCube = 0	;
LONG	StartZCube = 0	;

LONG	WorldXCube = 0	;
LONG	WorldYCube = 0	;
LONG	WorldZCube = 0	;

WORD	XpOrgw ;
WORD	YpOrgw ;

#define	NB_COLON	28
#define	MAX_BRICK	150

WORD	NbBrickColon[NB_COLON] ;

typedef	struct
{
	WORD	Xm ;
	WORD	Ym ;
	WORD	Zm ;
	WORD	Ys ;
	WORD	Brick ;

}	T_COLONB ;

T_COLONB	ListBrickColon[NB_COLON][MAX_BRICK] ;

UBYTE	*BufCube = 0		;
UBYTE	*BufMap	= 0		;
UBYTE	*TabBlock		;
UBYTE	*BufferBrick		;

UBYTE	*BufferMaskBrick ;

LONG	XMap, YMap, ZMap	;
LONG	NbBlock			;
LONG	XScreen, YScreen	;

LONG	DxBlock, DyBlock, DzBlock;

//LONG	CoulFond=0		;

extern	WORD	Nxw, Nyw, Nzw ;
void	ReajustPos( UBYTE col ) ;
/*--------------------------------------------------------------------------*/
#ifdef	BRICK_HQR
/*══════════════════════════════════════════════════════════════════════════*
	     █   █ █▀▀▀█ █▀▀▀█       █▀▀█  █▀▀▀█  █    █▀▀▀▀ █  ▄▀
	     ██▀▀█ ██ ▄█ ██▀█▀       ██▀▀█ ██▀█▀  ██   ██    ██▀▄
	     ▀▀  ▀ ▀▀▀▀  ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀  ▀▀   ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
#define	MAX_BRICK_GAME		10000L
#define	OFFSET_BUFFER_FLAG	153800L

char	*NameHqrGri=PATH_RESSOURCE"LBA_GRI.HQR"	;
char	*NameHqrBll=PATH_RESSOURCE"LBA_BLL.HQR"	;
char	*NameHqrBrk=PATH_RESSOURCE"LBA_BRK.HQR"	;

typedef	struct	{	ULONG	SizeFile ;
			ULONG	CompressedSizeFile ;
			WORD	CompressMethod ;	/* 0 stored */
							/* 1 LZS */
		}	T_HEADER ;

/*--------------------------------------------------------------------------*/
LONG	LoadUsedBrick( ULONG size )
{
	ULONG	i, j, b	;
	UBYTE	*pt	;
	UBYTE	*ptb	;
	UWORD	*ptw	;
	ULONG	*ptoff	;
	UBYTE	*ptdata	;
	UBYTE	*ptsrc	;
	UBYTE	*ptused	;
	ULONG	offset	;
	ULONG	maxbrk	;
	ULONG	nbbrick	;
	ULONG	brick	;
	ULONG	offseek	;
	ULONG	*ptseek	;
	ULONG	handle	;
	ULONG	nbentity;
	UWORD	*tabflag;
	UWORD	*ptflag	;
	UWORD	min, max;
	T_HEADER header	;
	UBYTE *ptdecomp	;

	tabflag = (UWORD*)(Screen+OFFSET_BUFFER_FLAG) ;

	RazMem( tabflag, MAX_BRICK_GAME*2L )	;/* Table de UWORD pour NewNumBrick */

	min = 60000			;
	max = 0				;

/*-------------------------------------- Premiere Passe, Préparation ptflag */

	pt = ptused = BufMap+(size-32)		;/* Debut de Used Block */

	for ( i = 1 ; i < 256 ; i++ )
	{
		b = pt[i>>3] & (1<<(7-(i&7)))	;/*	Recup Bit Block	*/
		if ( !b )	continue	;

		ptb = TabBlock + *(ULONG *)(TabBlock+((i-1)<<2));
		ptw = (UWORD *)(ptb + 5)	;/* Jump dx dy dz et collis */
		maxbrk = *ptb * *(ptb+1) * *(ptb+2)	;/* dx*dy*dz*/

		for ( j = 0 ; j < maxbrk ; j++, ptw += 2 )
		{
			brick = *ptw	;

			if ( brick )
			{
				brick--					;

				if ( brick < min )	min = brick	;
				if ( brick > max )	max = brick	;

				tabflag[brick] = 1			;
			}
		}
	}

/*-------------------------------------- Deuxieme Passe, On compte les Bricks*/
	ptflag = tabflag+min		;

	nbbrick = 0			;

	for ( i = min ; i <= max ; i++, ptflag++ )
	{
		if ( *ptflag )	nbbrick++	;
	}

/*-------------------------------------- Troisieme Passe, Load Brick         */

	handle = OpenRead( NameHqrBrk )	;
	if ( !handle )	return(0L)	;

	Read( handle, &nbentity, 4L )	;
	Seek( handle, 0L, SEEK_START )	;

	Read( handle, Screen, nbentity );
	nbentity >>= 2			;

	ptflag = tabflag+min		;

	ptoff = (ULONG *)BufferBrick	;

	offset = (nbbrick+1)*4L		;

	*ptoff++ = offset		;/* First Offset        */

	ptdata = BufferBrick+offset	 ;/* Jump nbbrick+1 Offset	*/

	nbbrick = 0			;

	ptseek = (ULONG *)Screen	;

	for ( i = min ; i <= max ; i++, ptflag++ )
	{
		if ( *ptflag )
		{
			nbbrick++		;/*	One More*/
			*ptflag = nbbrick	;/*	Brick+1	*/
			offseek = *(ptseek+i)	;
			Seek( handle, offseek, SEEK_START ) 	;
			Read( handle, &header, sizeof(header))	;
			switch( header.CompressMethod )
			{
				case 0:
					Read( handle, ptdata, header.SizeFile )	;
					break			;
				case 1:
					ptdecomp = ptdata+header.SizeFile-header.CompressedSizeFile+500 ;
					Read( handle, ptdecomp, header.CompressedSizeFile )	;
					Expand( ptdecomp, ptdata, header.SizeFile )	;
					break		;
			}
			ptdata += header.SizeFile	;
			offset += header.SizeFile	;
			*ptoff++ = offset		;
		}
	}

/*-------------------------------------- Quatrieme Passe, Rename Block      */

	pt = ptused				;/* Debut de Used Block */

	for ( i = 1 ; i < 256 ; i++ )
	{
		b = pt[i>>3] & (1<<(7-(i&7)))	;/*	Recup Bit Block	*/
		if ( !b )	continue	;

		ptb = TabBlock + *(ULONG *)(TabBlock+((i-1)<<2));
		ptw = (UWORD *)(ptb + 5)	;/* Jump dx dy dz et collis */
		maxbrk = *ptb * *(ptb+1) * *(ptb+2)		;/* dx*dy*dz*/

		for ( j = 0 ; j < maxbrk ; j++, ptw += 2 )
		{
			if ( *ptw )	*ptw = tabflag[(*ptw)-1];
		}
	}

	Close(handle)	;

	return(offset)	;
}
/*-------------------------------------------------------------------------*/
LONG	InitGrille( UWORD numcube )
{
	ULONG	i, b, j	;
	UBYTE	*pt	;
	ULONG	sizegri	;
	ULONG	sizebll	;
	ULONG	size	;

//	BufCube = Malloc(SIZE_CUBE_X*SIZE_CUBE_Y*SIZE_CUBE_Z*2L ) ;
//	if ( BufCube == 0L )	return(0L)		;

	sizegri = Size_HQR( NameHqrGri, numcube )	;/*	GRI	*/
	HQM_Alloc( sizegri, &BufMap ) ;
	CHECK_MEMORY
//	BufMap = Malloc( sizegri+500 ) 			;
	if ( BufMap == 0L )	return(0L)		;
	Load_HQR( NameHqrGri, BufMap, numcube )		;
//	Mshrink( BufMap, sizegri )			;

	sizebll = Size_HQR( NameHqrBll, numcube )	;/*	BLL	*/
	HQM_Alloc( sizebll, &TabBlock ) ;
	CHECK_MEMORY
//	TabBlock = Malloc( sizebll+500 )		;	bordel !
	if ( TabBlock == 0L )	return(0L)		;
	Load_HQR( NameHqrBll, TabBlock, numcube )	;
//	Mshrink( TabBlock, sizebll )			;

/*----------------------------------------------	NEW		*/
/*	Message("Loading Brick, Please Wait...", FALSE );*/

	size = LoadUsedBrick( sizegri )			;
	if ( !size )	return(0L)			;

/*	Message("     End Loading Brick       ", FALSE );*/
/*----------------------------------------------*/

	HQM_Alloc( size, &BufferMaskBrick ) ;
	size = CreateMaskGph( BufferBrick, BufferMaskBrick ) ;
	HQM_Shrink_Last( BufferMaskBrick, size ) ;
	CHECK_MEMORY

/*	BufferMaskBrick = Malloc( size ) 	;
	size = CreateMaskGph( BufferBrick, BufferMaskBrick ) ;
	Mshrink( BufferMaskBrick, size )	;// Reduc no test
*/
	NbBlock = (*(ULONG*)TabBlock)/4		;

	CopyMapToCube()				;

	return(1L)	;
}
/*--------------------------------------------------------------------------*/
void	FreeGrille()
{
/*	if( BufMap )
	{
		Free( BufferMaskBrick ) ;
		Free( TabBlock )	;
		Free( BufMap )  	;
//		Free( BufCube )		;

		BufMap = 0 ;
	}
*/
}
/*-------------------------------------------------------------------------*/
/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▀ ██▄ █ █▀▀▀▄       █   █ █▀▀▀█ █▀▀▀█
		   ██▀▀  ██▀██ ██  █       ██▀▀█ ██ ▄█ ██▀█▀
		   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀        ▀▀  ▀ ▀▀▀▀  ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
#endif
/*--------------------------------------------------------------------------*/
/*══════════════════════════════════════════════════════════════════════════*
	     █▀▀▀▀ █   █ ██▄ █ █▀▀▀▀ ▀▀█▀▀  █    █▀▀▀█ ██▄ █ ██▀▀▀
	     ██▀▀  ██  █ ██▀██ ██      ██   ██   ██  █ ██▀██ ▀▀▀▀█
	     ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀   ▀▀   ▀▀   ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

/*--------------------------------------------------------------------------*/
UBYTE	*GetAdrColonneMap( LONG x, LONG z )
{
	ULONG	offset	;

	offset = (x+(z*SIZE_CUBE_Z))*2		;
	offset = *(UWORD*)(BufMap+offset )	;

	return(BufMap+offset)			;
}
/*--------------------------------------------------------------------------*/
UBYTE	*GetAdrColonneCube( LONG x, LONG z )
{
	ULONG	offset	;

	offset = (x*SIZE_CUBE_Y*2)+(z*SIZE_CUBE_Z*SIZE_CUBE_Y*2);

	return(BufCube+offset)			;
}

/*--------------------------------------------------------------------------*/

void	GetShadow( WORD xw, WORD yw, WORD zw )
{
	WORD	xm, ym, zm ;
	UBYTE	*ptc ;
	WORD	y ;
	UBYTE	*adr ;
	LONG	block ;

	xm = (xw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ ;
	ym = yw/SIZE_BRICK_Y ;
	zm = (zw+DEMI_BRICK_XZ)/SIZE_BRICK_XZ ;

	ptc = BufCube + ym*2 + (xm*SIZE_CUBE_Y*2) + (zm*SIZE_CUBE_X*SIZE_CUBE_Y*2) ;

	for( y = ym; y>0; y-- )
	{
		if( *(WORD*)ptc != 0 )	break ;
		ptc -= 2 ;
	}

	XMap = xm ;
	YMap = y ;
	ZMap = zm ;

	Nxw = xw ;
	Nyw = (y+1) * SIZE_BRICK_Y ;
	Nzw = zw ;

	ShadowCol = 0 ;

	if( *ptc != 0 )
	{
		block = (*ptc++) - 1 ;
		adr = GetAdrBlock( block )	;
		adr += HEADER_BLOCK		;
		adr += (*ptc)<<2 		;/* 4 Bytes to Jump	*/
		ShadowCol = *adr ;
		ReajustPos( ShadowCol ) ;
/*		ShadowCol-- ;	*//* pour num gph */
	}

	ShadowX = Nxw ;
	ShadowY = Nyw ;
	ShadowZ = Nzw ;
}

/*──────────────────────────────────────────────────────────────────────────*/

/*══════════════════════════════════════════════════════════════════════════*
		   █▄ ▄█ █▀▀▀▀ █▄ ▄█ █▀▀▀█  █    █▀▀▀█ █▀▀▀▀
		   ██▀ █ ██▀▀  ██▀ █ ██  █  ██   ██▀█▀ ██▀▀
		   ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀  ▀▀   ▀▀  ▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

ULONG	CreateMaskGph( UBYTE *ptsrc, UBYTE *ptdst )
{
	UBYTE	*ptd	;
	ULONG   nbg, off;
	ULONG	*ptoff	;
	ULONG	size, i	;

	ptoff = (ULONG*)ptdst		;

	off = *(ULONG *)ptsrc		;/*	First Offset Src	*/

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
void	MixteMapToCube( UBYTE *map )
{
	UBYTE	*ptb		;
	LONG	blk, pos, nb	;
	UBYTE	*pts, *ptd	;
	LONG	x, y, z 	;
	ULONG	offset, flg=1	;

	for ( z = 0 ; z < SIZE_CUBE_Z ; z++ )
	{
		for ( x = 0 ; x < SIZE_CUBE_X ; x++ )
		{
			offset = (x+(z*SIZE_CUBE_Z))*2		;
			offset = *(UWORD*)(map+offset )		;

			pts = map+offset			;
			ptd = GetAdrColonneCube( x, z )		;
			MixteColonne( pts, ptd )		;
		}
	}
}
/*--------------------------------------------------------------------------*/
void	CopyMapToCube()
{
	UBYTE	*pts, *ptd, *ptb	;
	LONG	x, y, z, blk, pos, nb	;
	LONG	flg=1			;

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
	BufCube = Malloc(SIZE_CUBE_X*SIZE_CUBE_Y*SIZE_CUBE_Z*2L ) ;
	if ( BufCube == 0L )
		TheEnd( NOT_ENOUGH_MEM, "BufCube" )		;

	//	Init Buffer Brick
	BufferBrick = Malloc(MAX_SIZE_BRICK_CUBE)	;
	if ( BufferBrick == 0L )
		TheEnd( NOT_ENOUGH_MEM, "BufferBrick" )		;
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▄  █    ██▀▀▀ █▀▀▀█ █     █▀▀▀█ █  ▄▀
		   ██  █  ██   ▀▀▀▀█ ██▀▀▀ ██    ██▀▀█ ██▀
		   ▀▀▀▀   ▀▀   ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀▀  ▀ ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DrawOverBrick( WORD xm, WORD ym, WORD zm )
{
	T_COLONB	*ptrlbc ;
	WORD		col, i ;
	WORD		startcol, endcol ;

	startcol = (ClipXmin+24)/24-1 ;
	endcol = (ClipXmax+24)/24 ;

	for( col = startcol; col <= endcol; col++ )
	{
		ptrlbc = &ListBrickColon[col][0] ;

		for( i=0; i<NbBrickColon[col]; i++ )
		{
			/* bricks devant */
			if( ptrlbc->Ys+38 > ClipYmin
			AND ptrlbc->Ys <= ClipYmax )
			{
			if( ptrlbc->Ym >= ym )
			{
			if( (ptrlbc->Xm+ptrlbc->Zm) > (xm+zm) )
			{
				CopyMask( ptrlbc->Brick, col*24-24, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}
			}
			}
			ptrlbc++ ;
		}
	}
}

/*--------------------------------------------------------------------------*/
/*
void	DrawOverBrick2( WORD xm, WORD ym, WORD zm )
{
	T_COLONB	*ptrlbc ;
	WORD		col, i ;
	WORD		startcol, endcol ;

	startcol = (ClipXmin+24)/24-1 ;
	endcol = (ClipXmax+24)/24 ;

	for( col = startcol; col <= endcol; col++ )
	{
		ptrlbc = &ListBrickColon[col][0] ;

		for( i=0; i<NbBrickColon[col]; i++ )
		{
			if( ptrlbc->Ys+38 > ClipYmin
			AND ptrlbc->Ys <= ClipYmax )
			{
			if( ptrlbc->Ym >= ym )
			{
			if( (ptrlbc->Zm >= zm)
			AND (ptrlbc->Xm >= xm) )
			{
				CopyMask( ptrlbc->Brick, col*24-24, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}

			}
			}
			ptrlbc++ ;
		}
	}
}
*/
/*--------------------------------------------------------------------------*/
/* recouvrement reel pour obj qui ne depassent pas la ZV */

void	DrawOverBrick3( WORD xm, WORD ym, WORD zm )
{
	T_COLONB	*ptrlbc ;
	WORD		col, i ;
	WORD		startcol, endcol ;

	startcol = (ClipXmin+24)/24-1 ;
	endcol = (ClipXmax+24)/24 ;

	for( col = startcol; col <= endcol; col++ )
	{
		ptrlbc = &ListBrickColon[col][0] ;

		for( i=0; i<NbBrickColon[col]; i++ )
		{
			/* bricks devant */
			if( ptrlbc->Ys+38 > ClipYmin
			AND ptrlbc->Ys <= ClipYmax )
			{
			if( ptrlbc->Ym >= ym )
			{

			if( (ptrlbc->Zm == zm)
			AND (ptrlbc->Xm == xm) )
			{
				CopyMask( ptrlbc->Brick, col*24-24, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}

			if( (ptrlbc->Zm > zm)
			OR  (ptrlbc->Xm > xm) )
			{
				CopyMask( ptrlbc->Brick, col*24-24, ptrlbc->Ys, BufferMaskBrick, Screen ) ;
			}

			}
			}
			ptrlbc++ ;
		}
	}
}


/*--------------------------------------------------------------------------*/
void	AffBrickBlock( LONG block, LONG brick, LONG x, LONG y, LONG z )
{
	UBYTE	*adr	;
	LONG	numbrick;
	LONG	nb, col ;
	WORD	bc ;

	adr = GetAdrBlock( block )	;
	adr += HEADER_BLOCK		;

	adr += ( brick<<2 )		;/* 4 Bytes to Jump	*/
	adr += 2			;

	numbrick = *(UWORD*)adr		;	/* & 32767 */

	if ( numbrick )
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
			if( nb < MAX_BRICK )
			{
				// ca moche mettre PTR

				ListBrickColon[ col ][ nb ].Xm   = x ;
				ListBrickColon[ col ][ nb ].Ym   = y ;
				ListBrickColon[ col ][ nb ].Zm   = z ;
				ListBrickColon[ col ][ nb ].Ys    = YScreen ;
				ListBrickColon[ col ][ nb ].Brick = numbrick-1 ;
				NbBrickColon[ col ]++ ;

			}
			else
				Message( "Arg MAX_BRICK Z BUFFER atteint", TRUE ) ;
		}
	}
}

/*--------------------------------------------------------------------------*/
/*ptc = BufCube + y*2 + (x*SIZE_CUBE_Y*2) + (z*SIZE_CUBE_X*SIZE_CUBE_Y*2) ;*/

void	AffGrille()
{
	LONG	z, y, x		;
	LONG	block		;
	UBYTE	*ptc		;

	WorldXCube = StartXCube*SIZE_BRICK_XZ ;
	WorldYCube = StartYCube*SIZE_BRICK_Y ;
	WorldZCube = StartZCube*SIZE_BRICK_XZ ;

	ProjettePoint(	-WorldXCube, -WorldYCube, -WorldZCube ) ;
	XpOrgw = Xp ;
	YpOrgw = Yp ;

	for( x=0; x<NB_COLON; x++ )	NbBrickColon[x] = 0 ;

	if( !FlagAffGrille )	return ;

	ptc = BufCube		;
	for ( z = 0 ; z < SIZE_CUBE_Z ; z++ )
	{
		for ( x = 0 ; x < SIZE_CUBE_X ; x++ )
		{
			for ( y = 0 ; y < SIZE_CUBE_Y ; y++ )
			{
				block = *ptc++	;

				if ( block )
				{
					AffBrickBlock( block-1, *ptc, x, y, z )	;
				}
				ptc++	;
			}
		}
	}

/*	y = 0 ;
	for( x=0; x<NB_COLON; x++ )
	{
		Text( 0,y, "%l", NbBrickColon[x] ) ;
		y += 8 ;
	}
*/


}

void	IncrustGrm( WORD numgrm )
{
	UBYTE	*ptrgrm ;

	ptrgrm = LoadMalloc_HQR(PATH_RESSOURCE"lba_gri.hqr",
				numgrm + OFFSET_GRM_HQR  ) ;
	if( !ptrgrm )
	{
		Message( "arg grm not found in lba_gri", TRUE ) ;
		return ;
	}

	MixteMapToCube( ptrgrm ) ;

	Free( ptrgrm ) ;

	FirstTime = TRUE ;
}



















