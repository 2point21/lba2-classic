//--------------------------------------------------------------------------
#ifndef LIB_POLY_FILLER
#define LIB_POLY_FILLER

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------
#define	POLY_SOLID				0
#define	POLY_FLAT				1
#define	POLY_TRANS				2
#define	POLY_TRAME				3
#define	POLY_GOURAUD				4
#define	POLY_DITHER				5
#define	POLY_GOURAUD_TABLE			6
#define	POLY_DITHER_TABLE			7
#define	POLY_TEXTURE				8
#define	POLY_TEXTURE_FLAT			9
#define	POLY_TEXTURE_GOURAUD			10
#define	POLY_TEXTURE_DITHER			11
#define	POLY_TEXTURE_INCRUST			12
#define	POLY_TEXTURE_INCRUST_FLAT		13
#define	POLY_TEXTURE_INCRUST_GOURAUD		14
#define	POLY_TEXTURE_INCRUST_DITHER		15
#define	POLY_TEXTURE_Z				16
#define	POLY_TEXTURE_Z_FLAT			17
#define	POLY_TEXTURE_Z_GOURAUD			18
#define	POLY_TEXTURE_Z_DITHER			19
#define	POLY_TEXTURE_Z_INCRUST			20
#define	POLY_TEXTURE_Z_INCRUST_FLAT		21
#define	POLY_TEXTURE_Z_INCRUST_GOURAUD		22
#define	POLY_TEXTURE_Z_INCRUST_DITHER		23
#define	POLY_TEXTURE_Z_FOG			24
#define	POLY_FLAG_ZBUFFER			25

//--------------------------------------------------------------------------
#define	POLY_BIT_ENV_MAPPING			(1<<14)

//--------------------------------------------------------------------------
#define POLY_ENV_MAPPING                        (POLY_BIT_ENV_MAPPING|POLY_TEXTURE)
#define POLY_ENV_MAPPING_FLAT                   (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_FLAT)
#define POLY_ENV_MAPPING_GOURAUD                (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_GOURAUD)
#define POLY_ENV_MAPPING_DITHER                 (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_DITHER)
#define POLY_ENV_MAPPING_INCRUST                (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_INCRUST)
#define POLY_ENV_MAPPING_INCRUST_FLAT           (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_INCRUST_FLAT)
#define POLY_ENV_MAPPING_INCRUST_GOURAUD        (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_INCRUST_GOURAUD)
#define POLY_ENV_MAPPING_INCRUST_DITHER         (POLY_BIT_ENV_MAPPING|POLY_TEXTURE_INCRUST_DITHER)




//--------------------------------------------------------------------------
#pragma	pack(push, 1)

//--------------------------------------------------------------------------
typedef struct
	{
		S16	Pt_XE;		// Screen coordinates
		S16	Pt_YE;
		U16	Pt_MapU;	// texture coordinates
		U16	Pt_MapV;
		U16	Pt_Light;	// intensity (0.8.0)
		U16	Pt_ZO;		// normalized Z
		U32	Pt_W;		// 2^31 / Z (non normalized)
	}	Struc_Point;

//--------------------------------------------------------------------------
#pragma	pack(pop)

//--------------------------------------------------------------------------
extern	PTR_U8		PtrCLUTGouraud		;
extern	PTR_U8		PtrCLUTFog		;
//extern	PTR_U8		PtrSourceCopyPoly	;

extern	PTR_U8		PtrMap			;
extern	PTR_U16		PtrZBuffer 		;
extern	S32		RepMask			;

extern	S32		Fill_Z_Fog_Near		;// Fog Range
extern	S32		Fill_Z_Fog_Far		;
extern	U32		Fill_ZBuffer_Factor	;// normalizing factor for Z Buffer
extern	U32		IsPolygonHidden		;// normalizing factor for Z Buffer



//--------------------------------------------------------------------------
#define		NB_BITS_INV64		31

//--------------------------------------------------------------------------
S32		INV64(S32 a);

#pragma	aux	INV64= 					\
		"	mov	eax, 80000000h"		\
		"	xor	edx, edx"		\
		"	idiv	ebx"			\
		parm caller	[ebx]			\
		modify		[edx]

// Renvoie :    Nombre_passe_en_parametre en 1.0.31

//--------------------------------------------------------------------------
#define		GET_ZO(a)	(U16)(((a)*Fill_ZBuffer_Factor)>>16)

// Renvoie :    Le Z norme sur 16 bitspour le zbuffer 16 bits

//--------------------------------------------------------------------------
extern	void	Switch_Fillers(U32 Bank)	;

#pragma	aux	Switch_Fillers	"*"		\
		parm caller	[eax]

#define         FILL_POLY_TEXTURES	0       // Normal
#define         FILL_POLY_NO_TEXTURES	1       // Simplified (no mapping)
#define         FILL_POLY_FOG           2       // Fog
#define         FILL_POLY_NO_TEXTDITH   3       // No Dithered-Gouraud texturing (replaced by Gouraud texturing)
#define         FILL_POLY_ZBUFFER       4       // Z-Buffer
#define         FILL_POLY_FOG_ZBUFFER   5       // Z-Buffer + Fog
#define         FILL_POLY_NZW		6       // Z-Buffer without zbuffer update
#define         FILL_POLY_FOG_NZW	7       // Z-Buffer + Fog without zbuffer update



//--------------------------------------------------------------------------
typedef	S32	(FUNC_FILL_POLY)(	S32 Type_Poly, S32 Color_Poly,
					S32 Nb_Points, Struc_Point *Ptr_Points) ;

#pragma aux	ARG_FILL_POLY	"*"				\
		parm caller	[eax] [ebx] [ecx] [esi]		\
		modify 		[edx edi]

#pragma aux	(ARG_FILL_POLY)	FUNC_FILL_POLY

//--------------------------------------------------------------------------
extern	FUNC_FILL_POLY	*Fill_Poly	;

#pragma	aux	Fill_Poly	"*"

//--------------------------------------------------------------------------
extern	FUNC_FILL_POLY	Fill_PolyFast	;

//--------------------------------------------------------------------------
extern	void	Fill_Sphere( 	S32 Type_Sphere, S32 Color_Sphere,
				S32 Centre_X, S32 Centre_Y, S32 Rayon)	;

#pragma aux	Fill_Sphere	"*"				\
		parm caller	[esi] [edx] [eax] [ebx] [ecx]	\
		modify		[edi]

//--------------------------------------------------------------------------
extern	void	SetCLUT(	U32 defaultline		);

#pragma	aux	SetCLUT "*"				\
		parm caller	[eax]

//		Set the line (in PtrCLUTGouraud) corresponding to the actual
//		palette.

//--------------------------------------------------------------------------
extern	void	SetScreenPitch(	U32 *New_TabOffLine	);

#pragma aux	SetScreenPitch "*"			\
		parm caller	[ebx]				\
		modify exact	[]

//		Patch all the fillers routine with the new scanlines offsets

//--------------------------------------------------------------------------
extern	void	SetFog(S32 z_near, S32 z_far);

#pragma aux	SetFog		"*"				\
		parm caller	[eax] [ebx]

//		Defines Fog range

//--------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_POLY_FILLER

//--------------------------------------------------------------------------
