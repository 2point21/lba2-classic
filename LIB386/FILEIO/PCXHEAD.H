//──────────────────────────────────────────────────────────────────────────
#define	SIZE_BUF	2048L

//──────────────────────────────────────────────────────────────────────────
typedef	struct
	{
		char 	password;
		char 	version;
		char 	encoding;
		char 	bits_per_pixel;
		S16 	xmin, ymin, xmax, ymax;
		S16 	xres, yres;
		U8 	palette[48];
		char 	reserved;
		char 	no_of_planes;
		S16 	bytes_per_line;
		S16 	palette_type;
		char 	filler[58];
	} 	PCX_HEADER	;

//──────────────────────────────────────────────────────────────────────────
extern	PCX_HEADER	PcxHeader;

//──────────────────────────────────────────────────────────────────────────
extern	S32		Index	;

//──────────────────────────────────────────────────────────────────────────

