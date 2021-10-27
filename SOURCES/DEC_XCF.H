#ifdef __cplusplus
extern "C" {
#endif

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	PTR_VOID	XCF_BaseOldScr	;
extern	PTR_VOID	XCF_BaseNewScr	;
extern	PTR_VOID	XCF_BaseData	;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	XCF_InitDecompressor();

#pragma	aux	XCF_InitDecompressor	"*"	\
		parm caller []			\
		modify [eax ecx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	XCF_DecompressorHeight(U32 height);

#pragma	aux	XCF_DecompressorHeight	"*"	\
		parm caller []			\
		modify [eax ecx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	XCF_Decompressor();

#pragma	aux	XCF_Decompressor	"*"	\
		parm caller []			\
		modify [eax ecx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

typedef	struct
	{
		U32	struct_size	;
		U32	DeltaX		;
		U32	DeltaY		;
		U32	FrameSize	;
		U32	KeySize		;
		U32	KeyRate		;
		U32	PlayRate	;

		U32	SamplingRate	;
		U32	SampleType	;
		U32	SampleFlags	;

		U32	Compressor	;
	}	FORMAT;

#ifdef __cplusplus
}
#endif
