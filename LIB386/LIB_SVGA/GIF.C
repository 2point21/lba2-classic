/*
  ╔═══════════════════════════════════════════════════════════════╗
  ║                                                               ║
  ║          GIF = PROGRAM TO READ .GIF GRAPHICS FILES            ║
  ║                                                               ║
  ║  GIF and 'Graphics Interchange Format' are trademarks (tm) of ║
  ║        Compuserve,  Incorporated, an H&R Block Company        ║
  ║                                                               ║
  ║                  By Roger T. Stevens  12-9-91                 ║
  ║                                                               ║
  ╚═══════════════════════════════════════════════════════════════╝
*/

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

/*
struct  ffblk   {
    char        ff_reserved[21];
    char        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};
*/

#define UP_ARROW		328
#define DOWN_ARROW      336
#define LEFT_ARROW      331
#define RIGHT_ARROW     333
#define BACKSPACE         8

void decoder(LONG fd, WORD linewidth);
WORD next_code(LONG fd);

//union REGS reg;

struct
{
	char name[3]	;
	char version[3]	;
	short int xres	;
	short int yres	;
	unsigned short int packed;
	char back_col_index	;
	char aspect_ratio	;
} gif_header;

struct
{
	char red;
	char green;
	char blue;
} color_table[256], loc_color_table[256];

struct
{
	short int start_col	;
	short int start_row	;
	short int width		;
	short int height	;
	char packed		;
} image_descriptor;

char			ch, col_tab_flag, interlace_flag ;

/*
buffer[64],
color_flag,
color_res,
dirbuf[64][13],
filename[32],
loc_sort_flag,
sort_flag;
*/

unsigned char bytes = 0, b1, display_line[640/*2049*/], file_buf[512] ;

//unsigned char last[4096], stack[4096] ;
unsigned char *last, *stack ;

// unsigned char PALETTE[17]={0,1,2,3,4,5,20,7,56,57,58,59,60,61,62,63,0};

short int		bits_left = 0, code_size, clear, col_tab_size,
			linewidth, loc_col_tab_size,
			newcodes, rows,
			slot, top ;

extern	short int	i, end, index ;
/*
mode,
disp_height,
active_page = 0,
column,
dir_index= 0,
done,
end,
i,index = 0,
j, k,
key,
xres;
*/

short unsigned int height, width ;

//short unsigned int link[4096] ;
short unsigned int *link ;

//FILE *fin		;
//struct ffblk ffblk	;

UBYTE	*PtBuff ;
extern	UBYTE	*PtDest;

/*--------------------------------------------------------------------------*/
void	line_out_gif( char *pt, WORD size )
{
	WORD	i	;
	UBYTE	*pts	;

	pts = pt	;

	PtDest = PtBuff + (rows*640)	;

	for ( i = 0 ; i < size ; i++ )	*PtDest++ = *pts++	;

	rows++		;
/*
	Box( 0, 0, 160, 7, 0 )		;
	Text( 0, 0, "Ligne:%d", rows )	;
	CopyBlockPhys( 0, 0, 160, 7 )	;
*/
}
/*--------------------------------------------------------------------------*/
//	fd pointe deja sur le debut du gif sur disk
//	Faire le Close a plus haut niveau !

void	Read_Gif( LONG fd, UBYTE *screen, UBYTE *tabcol, UBYTE *buffers )
{
	char 		color	;
	char		dummy	;
	short int	finished ;

	PtBuff = PtDest = screen	;

	// buffers doit faire au moins 16K

	last  = buffers ;		// 4096
	stack = buffers + 4096 ;	// 4096
	link  = buffers + 8192 ;	// 8192

	Read( fd, &gif_header, 13L )	;

//	color_flag	= (gif_header.packed & 0x80) >> 7	;
//	color_res 	= (gif_header.packed & 0x70) >> 4	;
//	sort_flag 	= (gif_header.packed & 0x08) >> 3	;

//	col_tab_size = (WORD)pow(2,(gif_header.packed & 0x07)+1.0);

	col_tab_size = (WORD)(2<<(gif_header.packed & 0x07)) ;

	if (col_tab_size > 0)
		Read( fd, tabcol, col_tab_size *3L )	;
/*      Si image en 16 couleurs
	for ( i = 0 ; i < 16 ; i++ )
	{
		PALETTE[i] = ((color_table[i].red & 0x40) >> 1) |
			((color_table[i].red & 0x80)	>> 5) 	|
			((color_table[i].green & 0x40) >> 2) 	|
			(color_table[i].green & 0x80) >> 6 	|
			((color_table[i].blue & 0x40) >> 3) 	|
			((color_table[i].blue &	0x80) >> 7)	;
	}
*/
/*	Decalage en library automatique
	for ( i = 0 ; i < 256 ; i++ )
	{
		color_table[i].red   >>=  2	;
		color_table[i].green >>=  2	;
		color_table[i].blue  >>=  2	;
	}
*/

	rows = 0	;
	finished = 0	;

	while ( !finished )
	{
		Read( fd, &ch, 1L )	;

		switch ( ch )
		{
			case ';':     /*End of .GIF data */
				finished = 1	;
				break		;

			case '!':	  /* .GIF extension block - read and discard */

				Read( fd, &dummy, 1L )		;
				Read( fd, &i, 2L )		;
				Read( fd, file_buf, (LONG)i )	;
				break				;

			case ',': 	/* read image description */

				Read( fd, &image_descriptor, 9L )	;
				width = image_descriptor.width		;
				linewidth = min(640,width)		;
				height = image_descriptor.height	;
				col_tab_flag = (image_descriptor.packed & 0x80)   >> 7	;
//				interlace_flag = (image_descriptor.packed & 0x40) >> 6	;
//				loc_sort_flag = (image_descriptor.packed & 0x20)  >> 5	;
				loc_col_tab_size = (WORD)pow(2,(image_descriptor.packed
					& 0x07) + 1.0);

				if (col_tab_flag == 1)
					Read( fd,loc_color_table, loc_col_tab_size * 3L );

				decoder( fd, width )	;
				break			;
			default:
				finished = 1	;
				break		;
		}
	}
}
/*--------------------------------------------------------------------------*/
/*
╔═════════════════════════════════════════════════════════╗
║                                                         ║
║      restore_screen() = Reads and displays a file 	  ║
║                                                         ║
╚═════════════════════════════════════════════════════════╝
*/

void Load_Gif(char *filename, UBYTE *screen, UBYTE *tabcol)
{
	LONG	fd	;
	UBYTE	*buffers ;

	fd = OpenRead( filename )	;

	buffers = Malloc( 16384 ) ;

	Read_Gif( fd, screen, tabcol, buffers )	;

	Free( buffers ) ;

	Close(fd)			;
}
/*--------------------------------------------------------------------------*/
/*
  ╔═════════════════════════════════════════════════════════╗
  ║                                                         ║
  ║           decoder() = .GIF file decode                  ║
  ║                                                         ║
  ╚═════════════════════════════════════════════════════════╝
*/

void decoder( LONG fd, WORD width )
{
	short int code, fc=0, old_code=0, counter;
	short int ch, size=0, ret	;
	short int s_index=0	;
	short int l_index=0	;


	Read( fd, &size, 1L )	;
	code_size = size + 1	;
	top = 1 << code_size	;
	clear = 1 << size	;
	end = clear + 1		;

	slot = newcodes = end + 1;
	counter = width		;
	bits_left = 0		;
	b1 = 0			;
	bytes = 0		;

	while ((ch = next_code(fd)) != end)
	{
		if (ch == clear)
		{
			code_size = size + 1	;
			slot = newcodes		;
			top = 1 << code_size	;
			ch = next_code(fd)	;
			old_code = fc = ch	;
			display_line[l_index++] = ch;
			counter--		;
		}
		else
		{
			code = ch	;
			if ( code >= slot )
			{
				code = old_code		;
				stack[s_index++] = fc	;
			}
			while ( code >= newcodes )
			{
				stack[s_index++] = last[code]	;
				code = link[code]		;
			}
			stack[s_index++] = code	;
			if ( slot < top )
			{
				fc = code		;
				last[slot] = code	;
				link[slot++] = old_code	;
				old_code = ch		;
			}
			if ( slot >= top )
				if ( code_size < 12 )
				{
					top <<= 1	;
					++code_size	;
				}
			while ( s_index > 0 )
			{
				display_line[l_index++] = stack[--s_index]	;
				if ( --counter == 0 )
				{
					line_out_gif( display_line, linewidth )	;
					if ( rows >= height )
						return	;

					l_index = 0	;
					counter = width	;
				}
			}
		}
	}
	if ( counter != linewidth )
		line_out_gif( display_line, linewidth-counter );
}

/*--------------------------------------------------------------------------*/
/*
  ╔═════════════════════════════════════════════════════════╗
  ║                                                         ║
  ║         next_code(fd) = reads next code from file         ║
  ║                                                         ║
  ╚═════════════════════════════════════════════════════════╝

	ULONG	code
	UBYTE	b1
	UBYTE	bytes
	WORD	bits_left
*/

WORD next_code( LONG fd )
{
	short int flag=0	;
	unsigned long int code	;

	if ( bits_left == 0 )	flag = 1;	/* BUG	*/

	code = (b1 >> (8 - bits_left))	;

	while ( code_size > bits_left )
	{
		if ( bytes <= 0 )
		{
			index = 0			;
			Read( fd, &bytes, 1L )		;
			Read( fd, file_buf, (LONG)bytes );
		}

		b1 = file_buf[index++]	;

		if (flag == 1)
		{
			/* code = (b1 >> (8 - bits_left))	;			SUPER BUG*/
			code = b1			;
			flag = 0			;	/* BUG	*/
		}
		else	code |= (b1 << bits_left);

		bits_left += 8	;
		--bytes		;
	}

	bits_left -= code_size	;

	code &= ( 0xFFF >> (12 - code_size))	;

	return((WORD)(code))	;
}
/*--------------------------------------------------------------------------*/

