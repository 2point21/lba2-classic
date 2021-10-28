#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_svga\lib_svga.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>

/*══════════════════════════════════════════════════════════════════════════*
			       █▀▀▀█ █▀▀▀▀ ▀▄ ▄▀
			       ██▀▀▀ ██     ▄▀▄
			       ▀▀    ▀▀▀▀▀ ▀   ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/


struct  ffblk   {
    char        ff_reserved[21];
    char        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};

union REGS reg;

struct SREGS segregs;

static struct {
	char password;
	char version;
	char encoding;
	char bits_per_pixel;
	short int xmin, ymin, xmax, ymax;
	short int xres, yres;
	unsigned char palette[48];
	char reserved;
	char no_of_planes;
	short int bytes_per_line;
	short int palette_type;
	char filler[58];
	} pcx_header;

short int srcseg, srcoff, destseg, destoff;
short int xres, yres, active_bank, bank;
short int attr, i, j, k, dir_index= 0, type, length[7];

short int column, end, done, level,key,	start_x, start_y, start_angle,
				cursor_x, cursor_y,	mode;

struct ffblk ffblk;

long int col,row;

short int index = 0	;

#define	SIZE_BUF	2048L

UBYTE	*PtDest	;

LONG	debugline=0	;

/*--------------------------------------------------------------------------*/

void	line_out( UBYTE	*pts )
{
	UBYTE	*pt	;
	WORD	i	;

	pt = pts	;

	for ( i = 0 ; i < 640 ; i++ )	*PtDest++ = *pt++	;

/*	Box( 0, 0, 160, 7, 0 )			;
	Text( 0, 0, "Ligne:%d", debugline++ )	;
	CopyBlockPhys( 0, 0, 160, 7 )		;
*/

}
/*--------------------------------------------------------------------------*/
UBYTE	next_char( ULONG fd, UBYTE *pt)
{
	if ( index == SIZE_BUF )
	{
		index = 0		;
		Read( fd, pt, SIZE_BUF );
	}

	return(pt[index++])	;
}

/*══════════════════════════════════════════════════════════════════════════*
		█     █▀▀▀█ █▀▀▀█ █▀▀▀▄       █▀▀▀█ █▀▀▀▀ ▀▄ ▄▀
		██    ██  █ ██▀▀█ ██  █       ██▀▀▀ ██     ▄▀▄
		▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀  ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀   ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	Load_Pcx(char *file_name, UBYTE *screen, UBYTE *tabcol)
{
	ULONG	fd	;
	FILE *fsave	;
	unsigned char ch,color,buffer[650],file_buf[SIZE_BUF]	;
	short int i,j,k,m,pass,col,row,plane			;


	PtDest = screen			;

	fd = OpenRead( file_name )	;

	Read( fd, &pcx_header, 128L )	;

	Seek( fd, -768L, SEEK_END )	;

	Read( fd, tabcol, 768 )		;

	Seek( fd, 128L, SEEK_START )	;

	index = SIZE_BUF		;/* For Load in next_char */

	for ( row=pcx_header.ymin ; row<=pcx_header.ymax ; row++ )
	{
		for ( col=pcx_header.xmin ; col<=pcx_header.xmax ; col++ )
		{
			ch = next_char( fd, file_buf )	;
			if ((ch & 0xC0) != 0xC0)
				pass = 1		;
			else
			{
				pass = ch & 0x3F	;
				ch = next_char( fd, file_buf );
			}

			for ( m=0 ; m<pass ; m++ )
				buffer[col++] = ch	;
			col--;
		}
		line_out( buffer )	;
	}
	Close( fd )	;
}

/*══════════════════════════════════════════════════════════════════════════*
		██▀▀▀ █▀▀▀█ █   █ █▀▀▀▀       █▀▀▀█ █▀▀▀▀ ▀▄ ▄▀
		▀▀▀▀█ ██▀▀█ ██ ▄▀ ██▀▀        ██▀▀▀ ██     ▄▀▄
		▀▀▀▀▀ ▀▀  ▀ ▀▀▀   ▀▀▀▀▀ ▀▀▀▀▀ ▀▀    ▀▀▀▀▀ ▀   ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void Save_Pcx( char *filename, UBYTE *screen, UBYTE *ptrpalette )
{
	short int index = 0, i,k,number,num_out	;
	unsigned char ch, old_ch, file_buf[640*2];
	ULONG handle ;
	UBYTE c ;

	pcx_header.password = 0x0A;
	pcx_header.version = 0x05;
	pcx_header.encoding = 0x01;
	pcx_header.xmin = 0;
	pcx_header.ymin = 0;
	pcx_header.reserved = 0;
	pcx_header.palette_type = 0;

	/* 640 x 480 - 256 colors */
	pcx_header.bits_per_pixel = 0x08;
	pcx_header.xmax = 639;
	pcx_header.ymax = 479;
	pcx_header.xres = 640;
	pcx_header.yres = 480;
	pcx_header.no_of_planes = 1 ;
	pcx_header.bytes_per_line = 640 ;

	handle = OpenWrite( filename ) ;

	Write( handle, &pcx_header, 128 ) ;

	for ( k = pcx_header.ymin ; k <= pcx_header.ymax ; k++ )
	{
		number = 1	;

		old_ch = *( screen + 640*k )	;

		for ( i = 1 ; i <= 640; i++ )
		{
			if ( i == 640 )	ch = old_ch-1			;
			else		ch = *( screen + 640*k + i ) 	;

			if (( ch == old_ch ) && number < 63 )	number++ ;
			else
			{
				num_out = ((unsigned char) number | 0xC0);
				if ((number != 1) || ((old_ch & 0xC0) == 0xC0))
					file_buf[index++] = num_out;
				file_buf[index++] = old_ch;
				old_ch = ch;
				number = 1;
			}
		}

		Write( handle, file_buf, index );
		index = 0			;
	}

	c = 0x0C ;
	Write( handle, &c, 1 ) ;

	Write( handle, ptrpalette ,768 ) ;

	Close( handle ) ;
}


