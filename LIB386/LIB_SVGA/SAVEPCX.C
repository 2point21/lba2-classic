/*
  ╔═══════════════════════════════════════════════════════════════╗
  ║                                                               ║
  ║          SAVETEST = PROGRAM TO STORE .PCX GRAPHICS FILES      ║
  ║                                                               ║
  ║                                                               ║
  ╚═══════════════════════════════════════════════════════════════╝
*/
#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_svga\lib_svga.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <conio.h>

/* voir pcx.c */

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


/*
  ╔═════════════════════════════════════════════════════════╗
  ║                                                         ║
  ║                                                         ║
  ╚═════════════════════════════════════════════════════════╝
*/

void PcxSave( UBYTE *filename, UBYTE *screen, UBYTE *ptrpalette )

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


