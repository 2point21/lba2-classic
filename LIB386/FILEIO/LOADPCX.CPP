//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include <SYSTEM.H>
#include <FILEIO\LOADPCX.H>

#include "PCXHEAD.H"

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
static	S32 next_char( U32 fd, U8 *pt)
{
	if ( Index == SIZE_BUF )
	{
		Index = 0		;
		Read( fd, pt, SIZE_BUF );
	}

	return(pt[Index++])	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void	LoadPCX(char *file_name, void *screen, S32 *resx, S32 *resy, void *tabcol)
{
	S32 	col, row, sizex			;
	S32	fd				;
	U8	*buffer, file_buf[SIZE_BUF]	;


	buffer = (U8*)screen		;

	fd = OpenRead( file_name )	;

	if(!fd)
	{
		return	;
	}

	Read( fd, &PcxHeader, 128L )	;

	Index = SIZE_BUF		;// For Load in next_char

	row	= PcxHeader.ymax-PcxHeader.ymin	;
	sizex	= PcxHeader.xmax-PcxHeader.xmin	;

	if(resy)	*resy = row+1	;
	if(resx)	*resx = sizex+1	;

	for (; row>=0; row-- )
	{
		for ( col=sizex; col>=0; )
		{
			S32	ch, pass	;

			ch = next_char( fd, file_buf )	;
			if ((ch & 0xC0) != 0xC0)
			{
				pass = 1		;
			}
			else
			{
				pass = ch & 0x3F	;
				ch = next_char( fd, file_buf );
			}

			col -= pass	;

			for (; pass>0; pass-- )
			{
				*buffer++ = (U8)ch	;
			}
		}
	}

	Seek( fd, -768, SEEK_END )	;

	Read( fd, tabcol, 768 )		;

	Close( fd )	;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
