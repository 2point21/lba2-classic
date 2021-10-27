#ifndef	GRILLE_H
#define	GRILLE_H

#ifdef __cplusplus
extern "C" {
#endif

// GRILLE_A.ASM

extern	U8	CodeJeu			;
extern	S32	XMap, YMap, ZMap 	;
extern	S32	XScreen, YScreen	;
extern	PTR_U8	BufCube       		;
extern	PTR_U8	BufferMaskBrick 	;
extern	PTR_U8	BufMap			;
extern	PTR_U8	TabBlock		;
extern	PTR_U8	BufferBrick	       	;

//-------------------------------------------------------------------
extern Func_WorldColBrick	WorldColBrick ;

//-------------------------------------------------------------------
extern U8	WorldColBrickFull( S32 xw, S32 yw, S32 zw, S32 ymax ) ;

#pragma	aux     WorldColBrickFull	"*"	\
		parm caller []			\
		modify [eax ebx ecx edx]

//-------------------------------------------------------------------
extern U8	WorldCodeBrick(    S32 xw, S32 yw, S32 zw ) ;

#pragma aux	WorldCodeBrick	"*"			\
		parm caller	[eax] [ebx] [ecx]	\
		modify [edx]

//-------------------------------------------------------------------
extern U8	GetBlockBrick(     S32 xw, S32 yw, S32 zw ) ;

#pragma	aux	GetBlockBrick	"*"	\
		parm caller [] 		\
		modify [eax ecx edx]

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	U8	GetWorldColBrickVisible( S32 x, S32 y, S32 z ) ;

#pragma	aux     GetWorldColBrickVisible	"*"	\
		parm caller []			\
		modify [eax ecx edx]

//-------------------------------------------------------------------
extern	void	DecompColonne( U8 *pts, U8 *ptd  ) ;

#pragma	aux 	DecompColonne	"*"		\
		parm caller	[esi] [edi]	\
		modify [eax ecx edx]

//-------------------------------------------------------------------
extern	void	Map2Screen( S32 x, S32 y, S32 z ) ;

#pragma	aux	Map2Screen	"*"			\
		parm caller	[eax] [ecx] [edx]	\
		modify exact [eax ebx edx]

//-------------------------------------------------------------------
extern	U8	*GetAdrBlock( S32 numblock ) ;

#pragma	aux 	GetAdrBlock	"*"	\
		parm caller	[ecx]	\
		modify exact [eax]

//-------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------
extern S32  LoadUsedBrick(void);

//-------------------------------------------------------------------
extern Func_InitGrille InitGrille ;

//-------------------------------------------------------------------
extern void FreeGrille(void);

//-------------------------------------------------------------------
extern U8   *GetAdrColonneMap(S32 x,S32 z);
//-------------------------------------------------------------------
extern U8   *GetAdrColonneCube(S32 x,S32 z);
//-------------------------------------------------------------------
extern Func_WorldColBrickVisible WorldColBrickVisible ;
//-------------------------------------------------------------------
extern Func_GetShadow GetShadow ;

//-------------------------------------------------------------------
extern Func_ProjectShadow ProjectShadow ;

//-------------------------------------------------------------------
extern U32  CreateMaskGph(U8 *ptsrc,U8 *ptdst);

//-------------------------------------------------------------------
extern void CopyMapToCube(void);

//-------------------------------------------------------------------
extern void InitBufferCube(void);

//-------------------------------------------------------------------
extern void DrawOverBrick(S32 xm,S32 ym,S32 zm);

//-------------------------------------------------------------------
extern void DrawOverBrick3(S32 xm,S32 ym,S32 zm,S32 ymax);

//-------------------------------------------------------------------
extern void DrawOverBrickCage(S32 xm,S32 ym,S32 zm);

//-------------------------------------------------------------------
extern void AffBrickBlock(S32 block,S32 brick,S16 x,S16 y,S16 z);

//-------------------------------------------------------------------
extern void AffGrille(void);

//-------------------------------------------------------------------
extern void AffBrickBlockColon(S32 block,S32 brick,S16 x,S16 y,S16 z);

//-------------------------------------------------------------------
extern void AffGrilleOnly(void);

//-------------------------------------------------------------------
extern void AffBrickBlockOnly(S32 block,S32 brick,S16 x,S16 y,S16 z);

//-------------------------------------------------------------------
extern void AffGrilleLabyrinthe(void);

//-------------------------------------------------------------------
extern void AffOneBrick(S32 x,S32 y,S32 z);

//-------------------------------------------------------------------
extern void IncrustGrm(T_ZONE *ptrz);

//-------------------------------------------------------------------
extern void DesIncrustGrm(T_ZONE *ptrz);

//-------------------------------------------------------------------
extern void RedrawGRMs( void ) ;

//-------------------------------------------------------------------

#endif	// GRILLE_H
