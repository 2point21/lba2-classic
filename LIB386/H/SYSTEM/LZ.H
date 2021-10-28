//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_SYSTEM_LZ
#define LIB_SYSTEM_LZ

//──────────────────────────────────────────────────────────────────────────
#define	RECOVER_AREA	512	// for overlapping ExpandLZ

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
extern	void	ExpandLZ(void *Dst, void *Src, U32 DecompSize, U32 MinBloc);

#pragma aux ExpandLZ	"*"		\
	parm caller	[edi] [ebx] [ecx] [eax]	\
	modify	[edx esi]

//──────────────────────────────────────────────────────────────────────────
#define	ExpandLZSS(Src, Dst, DecompSize)	ExpandLZ(Dst, Src, DecompSize, 2)

//──────────────────────────────────────────────────────────────────────────
#define	ExpandLZMIT(Src, Dst, DecompSize)	ExpandLZ(Dst, Src, DecompSize, 3)

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_SYSTEM_LZ

//──────────────────────────────────────────────────────────────────────────
