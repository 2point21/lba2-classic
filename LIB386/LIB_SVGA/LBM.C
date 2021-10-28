#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_svga\lib_svga.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*══════════════════════════════════════════════════════════════════════════*
                               █     █▀▀█  █▄ ▄█                               
                               ██    ██▀▀█ ██▀ █                               
                               ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀                               
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

#define		MAXBUFLIGNE	1500
#define		RowByte(w)	((((w)+15)>>4) << 1)

UWORD	Larg ;

WORD	Lbm_Width = 640 ;

#define	BUFLIGNEMAX	720

static	WORD	Handle ;
static	ULONG 	rejet ;
static	ULONG 	len ;
static	BYTE    head[] = "BMHD" ;
static	WORD 	larg,haut ;
static	BYTE 	nbr_plan ;
static	BYTE 	flgcomp ;	/* flg de compression */
static	BYTE	Masking ;
//static	BYTE	bufflig[BUFLIGNEMAX] ;

static	UBYTE	tabmsk[8]  = {	0x01,0x02,0x04,0x08,
				0x10,0x20,0x40,0x80	} ;

static	LONG	CptIff ;
static	UBYTE	*PtrIff ;
static	UBYTE	*BufferIff ;

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

UBYTE	ReadIff()
{
	if( CptIff == 0 )
	{
		Read( Handle, BufferIff, 64000L ) ;
		CptIff = 64000L ;
		PtrIff =(void *)BufferIff ;
	}
	CptIff-- ;
	return *PtrIff++ ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	WriteIff( void *ptrin, ULONG len )
{
	Write( Handle, ptrin, len );
}

/*──────────────────────────────────────────────────────────────────────────*/

void	WriteIffLong( ULONG data )
{
	WriteIff( (void*)&((UBYTE*)&data)[3], 1L );
	WriteIff( (void*)&((UBYTE*)&data)[2], 1L );
	WriteIff( (void*)&((UBYTE*)&data)[1], 1L );
	WriteIff( (void*)&((UBYTE*)&data)[0], 1L );
}

/*──────────────────────────────────────────────────────────────────────────*/

void	itoml( void *l )
{
	UBYTE c ;
	UBYTE *ptc ;

	ptc = (char*)l ;
	c = ptc[0] ;
	ptc[0] = ptc[3] ;
	ptc[3] = c ;
	c = ptc[1] ;
	ptc[1] = ptc[2] ;
	ptc[2] = c ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void lit_bmhd()
{
	UBYTE a,b ;

	Read( Handle, &a, 1L )	;
	Read( Handle, &b, 1L ) 	;
	larg = ((unsigned int)a)*256 + b ;

	Read( Handle, &a, 1L ) 	;
	Read( Handle, &b, 1L ) 	;
	haut = ((unsigned int)a)*256 + b ;

	Read( Handle, &len, 4L ) 	;	/* lit 4 octets */
	Read( Handle, &nbr_plan, 1L );
	Read( Handle, &Masking, 1L ) 	;
	Read( Handle, &flgcomp, 1L );
	Read( Handle, &a, 1L )	;  		 /* pad */
	Read( Handle, &rejet, 4L ) 	;
	Read( Handle, &rejet, 4L )	;
	Larg = larg;
	if( larg>320 )	larg = 320;
	if( haut>200 )	haut = 200;
	Masking &= 1;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	lit_bloc()      /* on a un bloc inconnu */
{
	WORD	l ;

	for(l = 0 ; l < (int)len ; l++)
		Read(Handle, &rejet, 1L ) ;   /* a jeter */
}

/*──────────────────────────────────────────────────────────────────────────*/

void	lit_ligne( UBYTE *ptdst, WORD nboctet )
{
	UBYTE	*dst;
	BYTE	o;
	WORD	nb;
	UWORD	i;

	dst = ptdst;
	if(flgcomp)
	{
		while(nboctet > 0)
		{
			o = ReadIff();
			if(o >= 0)
			{
				nb = o + 1;
				nboctet -= nb;
				for ( i=0 ; i<nb ; i++)	*dst++ = ReadIff();
			}
			else
			{
				if(o != -128)
				{
					nb = -o + 1;
					nboctet -= nb;
					o = ReadIff();
					while(nb--) *dst++ = o;
				}
			}
		}
	}
	else
		while( nboctet-- )	*dst++ = ReadIff();
}

/*──────────────────────────────────────────────────────────────────────────*/

void	ConvertMCGA(UBYTE *bufligne, UBYTE *dst, UWORD nboctet, WORD nbplan )
{
	WORD	a,b,p,msk ;
	UBYTE 	col ;

	for(a = 0 ; a < nboctet ; a++)
	{
		for(b = 0 ; b < 8; b++)
		{
			col = 0;
			msk = tabmsk[7-b] ;
			for(p = nbplan-1 ; p >= 0 ; p--)
			{
				col <<= 1 ;
				if(*(bufligne + p*nboctet + a) & msk) col++ ;
			}
			*dst++ =   col;
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void 	lit_bodyILBM( UBYTE *ptr )
{
	WORD  	lig,nboctet ;
	UBYTE	*bufligne ;

	if ((bufligne = Malloc(MAXBUFLIGNE)) == 0 )	return ;
	nboctet = RowByte(Larg) ;

	for(lig = 0 ; lig < 200 ; lig++)
	{
		memset( bufligne, 0, MAXBUFLIGNE ) ;
		if (lig < haut )
		{
			lit_ligne( bufligne,nboctet*nbr_plan ) ;
			ConvertMCGA(bufligne,ptr,nboctet,nbr_plan ) ;
			memset( ptr+nboctet*8,0, 320-nboctet*8 ) ;
		}
		else
		{
			memset( ptr, 0, Lbm_Width ) ;
		}
		ptr += Lbm_Width ;
	}
	Free( bufligne ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void 	lit_bodyPBM( UBYTE *ptr )
{
	WORD  	lig ;

	for(lig = 0 ; lig < 200 ; lig++)
	{
		lit_ligne( ptr,larg ) ;
		ptr += Lbm_Width ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
		█     █▀▀▀█ █▀▀▀█ █▀▀▀▄       █     █▀▀█  █▄ ▄█
		██    ██  █ ██▀▀█ ██  █       ██    ██▀▀█ ██▀ █
		▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

LONG	Load_Lbm( char *name, UBYTE *ptrscreen, UBYTE *ptrpal )
{
	ULONG 	len2 ;
	WORD    type = 0 ;				/* 0 = ILBM;	1 = PBM */
	UBYTE	*ptr ;

	CptIff = 0;
	Handle = OpenRead( name ) ;
	if( !Handle )	return FALSE ;

	BufferIff = Malloc( 64000 ) ;

/*	printf( "%s: ", name ) ;	*/

	Read( Handle, head, 4L ) ;
	Read( Handle, &len, 4L ) ;			/* lit taille */
	Read( Handle, head, 4L ) ;			/* lit ILBM */
	if( strcmp( head, "ILBM" )!= 0 ) type = 1;	/* PBM */

	while( TRUE )
	{
		len2 = Read( Handle, head, 4L ) ;
		if( len2 != 4 )	break ;			/* fin de fichier */
		Read( Handle, &len, 4L ) ;		/* lit taille du chunk */
		itoml( &len ) ;
		if( len&1 )	len++ ;
		if(strcmp(head,"BMHD") == 0)		lit_bmhd() ;
		else
			if(strcmp(head,"CMAP") == 0)  	/* pal */
				   Read( Handle, ptrpal, 768 ) ;
			else
				if(strcmp(head,"BODY") == 0)
				{
					if( type==0 )
						lit_bodyILBM( ptrscreen ) ;
					else
						lit_bodyPBM( ptrscreen ) ;
					break;
				}
				else	lit_bloc()		;/* bloc inconnu on le saute */
	}

	Free( BufferIff ) ;
	Close( Handle ) ;

/*	printf( "converted to VGA " ) ;	*/
	return TRUE ;
}


/*══════════════════════════════════════════════════════════════════════════*
		██▀▀▀ █▀▀▀█ █   █ █▀▀▀▀       █     █▀▀█  █▄ ▄█
		▀▀▀▀█ ██▀▀█ ██ ▄▀ ██▀▀        ██    ██▀▀█ ██▀ █
		▀▀▀▀▀ ▀▀  ▀ ▀▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

LONG	Save_Lbm( char *name, UBYTE *ptrscr, UBYTE *ptrpal )
{
	UWORD	nbplan=8,a=0;
	UWORD	resolx = 320;
	UWORD	resoly = 200;
	UWORD	y ;
	UWORD	ligne,colonne;

	Handle = OpenWrite( name );
	if ( Handle == 0) return 0 ;
	WriteIff("FORM",4L);
	WriteIffLong(64816L);
	WriteIff("PBM ",4L);
	WriteIff("BMHD",4L);
	WriteIffLong(20L);
	WriteIff((void *)&((UBYTE*)&resolx)[1],1L);
	WriteIff((void *)&((UBYTE*)&resolx)[0],1L);
	WriteIff((void *)&((UBYTE*)&resoly)[1],1L) ;
	WriteIff((void *)&((UBYTE*)&resoly)[0],1L) ;
	WriteIffLong(0L);			/* lit 4 octets avant le, */
	WriteIff(&nbplan,1L);
	WriteIff(&a,1L);
	a = 0;					/* flg non compilee */
	WriteIff(&a,1L);
	WriteIff(&a,1L);		  		 /* pad */
	WriteIffLong(0L);
	WriteIffLong(0L);

	WriteIff("CMAP",4L);			/* pal */
	WriteIffLong(3*256L);
	WriteIff(ptrpal,3*256L);

	colonne	 = 40;
	ligne	 = 200;

	WriteIff ("BODY",4L);
	WriteIffLong ((LONG)(colonne*nbplan*ligne));

	for( y=0; y<200; y++ )
	{
		Write( Handle, ptrscr, 320 ) ;
		ptrscr += Lbm_Width ;
	}

/*	Write (Handle, ptrscr, 64000L );	*/

	Close( Handle );
	return TRUE ;
}
