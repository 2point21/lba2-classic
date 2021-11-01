//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <SYSTEM.H>
#include <SVGA\INITMODE.H>
#include <FILEIO\SAVEPCX.H>

#include "PCXHEAD.H"

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
static void next_char( U32 fd, U8 *pt, S32 val)
{
	pt[Index++] = (U8)val	;

	if ( Index == SIZE_BUF )
	{
		Index = 0			;
		Write( fd, pt, SIZE_BUF )	;
	}
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void SavePCX(char *filename, void *screen, S32 xres, S32 yres, void *ptrpalette)
{
	S32	i, k, number, num_out	;
	S32	ch, old_ch 		;
	U8	file_buf[SIZE_BUF]	;
	S32	fd 			;

	PcxHeader.password	= 0x0A	;
	PcxHeader.version	= 0x05	;
	PcxHeader.encoding	= 0x01	;
	PcxHeader.xmin 		= 0	;
	PcxHeader.ymin 		= 0	;
	PcxHeader.reserved 	= 0	;
	PcxHeader.palette_type 	= 0	;

	// resx * resy - 256 colors
	PcxHeader.bits_per_pixel= 0x08		;
	PcxHeader.xmax 		= (S16)(xres-1)	;
	PcxHeader.ymax 		= (S16)(yres-1)	;
	PcxHeader.xres 		= (S16)xres	;
	PcxHeader.yres 		= (S16)yres	;
	PcxHeader.no_of_planes	= 1 		;
	PcxHeader.bytes_per_line= (S16)xres	;

	fd = OpenWrite(filename) 	;

	if(!fd)
	{
		return	;
	}

	Write(fd, &PcxHeader, 128)	;

	Index = 0	;

	for ( k = 0; k < yres; k++ )
	{
		number = 1	;

                old_ch = ((U8*)screen)[ModeDesiredX*k]   ;

		for ( i = 1 ; i <= xres; i++ )
		{
			if ( i == xres )
			{
				ch = old_ch-1	;
			}
			else
			{
                                ch = ((U8*)screen)[ModeDesiredX*k+i]     ;
			}

			if (( ch == old_ch ) && (number < 63) )
			{
				number++ ;
			}
			else
			{
				num_out = ((U8) number | 0xC0)	;
				if ((number != 1) || ((old_ch & 0xC0) == 0xC0))
				{
					next_char(fd, file_buf, num_out);
				}
				next_char(fd, file_buf, old_ch)	;
				old_ch	= ch			;
				number	= 1			;
			}
		}
	}

	next_char(fd, file_buf, 0x0C)	;

	Write(fd, file_buf, Index)	;

	Write(fd, ptrpalette, 768)	;

	Close(fd) 	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
