#ifdef __cplusplus
extern "C" {
#endif

extern	U8	*BaseOldScr	;
extern	U8	*BaseNewScr	;
extern	U8	*BaseData	;

extern	U8	Patch_MaxLine[]	;
extern	U8	Patch_CodeLen[]	;

#define	Dec_MaxLine	Patch_MaxLine[1]
#define	Dec_CodeLen	*(U32 *)(&Patch_CodeLen[2])

#define DecompressorHeight(a)					\
{                                                               \
	Dec_MaxLine = (U8)(a/8-1)                  ;		\
	Dec_CodeLen = ((a/8)*(MAX_WIDTH/8)*6)/8+4  ;		\
}

void	Decompressor( void )		;
void	InitDecompressor( void )	;

#ifdef __cplusplus
}
#endif
