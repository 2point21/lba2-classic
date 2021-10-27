//--------------------------------------------------------------------------
#ifndef	LIB_SVGA_GRAPH
#define	LIB_SVGA_GRAPH

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------
S32	AffGraph(S32 numgraph, S32 x, S32 y, void *bankgraph)	;

#pragma aux AffGraph	"*"		\
	parm caller	[eax] [ebx] [ecx] [esi]	\
	modify	[edx edi]

//--------------------------------------------------------------------------
S32	GetDxDyGraph(S32 numgraph, S32 *x, S32 *y, void *bankgraph);

#pragma aux GetDxDyGraph	"*"		\
	parm caller		[eax] [ebx] [ecx] [esi]	\
	modify exact	[eax edx esi]

//--------------------------------------------------------------------------
S32	GetBoxGraph(S32 numgraph, S32 *x0, S32 *y0, S32 *x1, S32 *y1, void *bankgraph);

#pragma aux GetBoxGraph	"*"					\
	parm caller		[eax] [ebx] [ecx] [edx] [edi] [esi]	\
	modify exact	[eax ebx ecx esi]

//--------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_SVGA_GRAPH

//--------------------------------------------------------------------------
