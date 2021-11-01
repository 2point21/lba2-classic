#include "defines.h"

#define	 	LIMITE_LOIN 	(60*1024)
#define		COLOR_LIMITE	255
#define		SIZE_TEXTURE	64

#define	WORLD_SIZE	32768
#define	NB_POLYS_COTE	4

// Taille 2D mini pour accepter un poly animé
#define	ANI_POLY_STEP_Y		2
#define	ANI_POLY_STEP_X		5

#include	"drawsky.cpp"

S32	Sky_Y ;
//S32	NbDrawnPolys ;

S32	FlagMasque = TRUE ;

#if defined(DEBUG_TOOLS)&&defined(LBA_GAME)
extern	S32	DebugFps	;
#endif

#ifndef	LBA_GAME
S32	Reglage = 0 ;
#endif

#ifdef	LBA_EDITOR	// Special PERSO.EXE
#define	INFO_DECORS	1024
extern	S32	FlagInfos ;
#endif

//****************************************************************************
//****************************************************************************

S32	ListTypePolygone[4] =	{ 0, POLY_SOLID, POLY_GOURAUD_TABLE, POLY_DITHER_TABLE } ;
S32	ListTypePTexture[] =	{
	0, POLY_TEXTURE, POLY_TEXTURE_FLAT, POLY_TEXTURE_GOURAUD,
	0, POLY_TEXTURE_INCRUST, POLY_TEXTURE_INCRUST_FLAT, POLY_TEXTURE_INCRUST_GOURAUD,
	0, POLY_TEXTURE_INCRUST, POLY_TEXTURE_INCRUST_FLAT, POLY_TEXTURE_INCRUST_GOURAUD,
	0, POLY_TEXTURE_INCRUST, POLY_TEXTURE_INCRUST_FLAT, POLY_TEXTURE_INCRUST_GOURAUD,
				} ;

/*══════════════════════════════════════════════════════════════════════════*
		   ▀▀█▀▀ █▀▀▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀█  █    ██▄ █
		     ██  ██▀▀  ██▀█▀ ██▀█▀ ██▀▀█  ██   ██▀██
		     ▀▀  ▀▀▀▀▀ ▀▀  ▀ ▀▀  ▀ ▀▀  ▀  ▀▀   ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawSkySeaZBuf()
{
	// draw Sky
//	Sky_Y = 12500 ;

	PtrMap = SkySeaTexture ;
	RepMask = 0x7F7F ;

	DrawSky( Sky_Y ) ;
//	DrawSea( Y_SEA ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	ClsTerrainZBuf()
{
	SetClearColor( (U8)FogCoul ) 	;
	Cls()				;
	SetClearColor( 0 ) 		;

	memset(PtrZBuffer, 255, SIZE_Z_BUFFER );	// Clear ZBuffer
}

/*──────────────────────────────────────────────────────────────────────────*/
static inline void DrawFeuillePolyZBuf( S32 xi, S32 zi, T_SOMMET_ROT *ptrs)
{
	S32 		color		;
	T_HALF_POLY	*ptrh		;
	S32 		f0, f1, f2, f3 	;
	S32		i0, i1, i2, i3	;
	S32 		x0, z0 		;
	S32 		x1, z1 		;
	S32 		x2, z2 		;
	S32 		x3, z3 		;
	U8		type		;
	U16		fog		;

	ptrh = &MapPolyGround[zi*(64*2)+xi*2] ;

	// optimisation globale affichage terrain
	if( !	(ptrh->PolyFlag + ptrh->TexFlag +
		(ptrh+1)->PolyFlag + (ptrh+1)->TexFlag) )
	{
		return ;
	}

	x0 = ptrs[0].X2D	;
	z0 = ptrs[0].Y2D 	;
	x1 = ptrs[65].X2D 	;
	z1 = ptrs[65].Y2D 	;
	x3 = ptrs[1].X2D 	;
	z3 = ptrs[1].Y2D 	;
	x2 = ptrs[66].X2D 	;
	z2 = ptrs[66].Y2D 	;

	f0 = ptrs[0].Zrot ;

/*	PtrCLUTGouraud = PtrCLUTFog +
		(16*256) * BoundRegleTrois(0,15,
					ClipZFar - StartZFog,
					f0 - StartZFog ) ;
*/
	fog = (U16)(PalLevel + 16 * BoundRegleTrois(0,15,
						ClipZFar - StartZFog,
						f0 - StartZFog ) )	;
	SetCLUT( fog ) ;

	f0 = GET_ZO(f0)			;
	f1 = GET_ZO(ptrs[65].Zrot)	;
	f3 = GET_ZO(ptrs[1].Zrot)	;
	f2 = GET_ZO(ptrs[66].Zrot)	;

	{
		U8	*i = &MapIntensity[zi*65+xi] 	;

		i0 = (i[0]&15)*256 - 255		;
		i1 = (i[65]&15)*256 - 255		;
		i3 = (i[1]&15)*256 - 255		;
		i2 = (i[65+1]&15)*256 - 255	;

		if( i0 < 0 )	i0 = 0 ; // groumpf
		if( i1 < 0 )	i1 = 0 ; // groumpf
		if( i2 < 0 )	i2 = 0 ; // groumpf
		if( i3 < 0 )	i3 = 0 ; // groumpf

/*		i0 = i[0]*256 		;
		i1 = i[65]*256		;
		i3 = i[1]*256 		;
		i2 = i[65+1]*256 	;
*/	}

	if (ptrh->Sens == 0)
	{
		if( ptrh->PolyFlag + ptrh->TexFlag )
		{
			// type 0 triangle 0----------------------------

			Tab_Points[0].Pt_XE	= (S16)x0 ;
			Tab_Points[0].Pt_YE	= (S16)z0 ;
			Tab_Points[1].Pt_XE	= (S16)x1 ;
			Tab_Points[1].Pt_YE	= (S16)z1 ;
			Tab_Points[2].Pt_XE	= (S16)x2 ;
			Tab_Points[2].Pt_YE	= (S16)z2 ;

			if( TestVuePoly( Tab_Points ) )
			{
				color = (ptrh->Bank << 4) + 11	;

				Tab_Points[0].Pt_Light	= (S16)i0 ;
				Tab_Points[1].Pt_Light	= (S16)i1 ;
				Tab_Points[2].Pt_Light	= (S16)i2 ;

				Tab_Points[0].Pt_ZO     = (U16)f0 ;
				Tab_Points[1].Pt_ZO     = (U16)f1 ;
				Tab_Points[2].Pt_ZO     = (U16)f2 ;

				if( ptrh->PolyFlag )
				{
					if( ptrh->PolyFlag == 1 )
					{
						color += ((i0+i1+i2)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}

					Fill_Poly( ListTypePolygone[ptrh->PolyFlag],
						       color, 3,
						       Tab_Points ) ;
				}

				if( ptrh->TexFlag )
				{
					T_HALF_TEX	*ptrt		;

					ptrt = &ListTexDef[ ptrh->IndexTex ] ;

					Tab_Points[0].Pt_MapU = ptrt->Tx0 ;
					Tab_Points[0].Pt_MapV = ptrt->Ty0 ;
					Tab_Points[1].Pt_MapU = ptrt->Tx1 ;
					Tab_Points[1].Pt_MapV = ptrt->Ty1 ;
					Tab_Points[2].Pt_MapU = ptrt->Tx2 ;
					Tab_Points[2].Pt_MapV = ptrt->Ty2 ;

					if( ptrh->TexFlag == 2 )
					{
						color = ((i0+i1+i2)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}

					type = (U8)ListTypePTexture[ptrh->TexFlag+(ptrh->PolyFlag<<2)] ;

					if( Fill_Poly( type, color, 3, Tab_Points ) )
					{
						if( MaxPolySea AND (ptrh->CodeJeu & 12) == 12 )
						{ // hihi je suis un polygone animé...

							if( NbAniPoly < MaxPolySea-1 )
							{
								PtrAniPoly->X0 = (S16)Min3( x0, x1, x2 ) ;
								PtrAniPoly->Y0 = (S16)Min3( z0, z1, z2 ) ;
								PtrAniPoly->X1 = (S16)Max3( x0, x1, x2 ) ;
								PtrAniPoly->Y1 = (S16)Max3( z0, z1, z2 ) ;

								if( ((PtrAniPoly->Y1-PtrAniPoly->Y0)>ANI_POLY_STEP_Y)
								AND ((PtrAniPoly->X1-PtrAniPoly->X0)>ANI_POLY_STEP_X) )
								{
									PtrAniPoly->Type = type ;
									PtrAniPoly->Color = (U8)color ;

									PtrAniPoly->Fog = fog ;

									PtrAniPoly->Tab_Points[0] = Tab_Points[0] ;
									PtrAniPoly->Tab_Points[1] = Tab_Points[1] ;
									PtrAniPoly->Tab_Points[2] = Tab_Points[2] ;

									NbAniPoly++ ;
									PtrAniPoly++ ;
								}
							}
#ifdef DEBUG_TOOLS
							else
							{
								Message( "MaxPolySea Atteint !", TRUE ) ;
							}
#endif
						}
					}
				}
			}
		}
			// type 0 triangle 1----------------------------------

		ptrh++ ;

		if( ptrh->PolyFlag + ptrh->TexFlag )
		{
			Tab_Points[0].Pt_XE 	= (S16)x2 ;
			Tab_Points[0].Pt_YE 	= (S16)z2 ;
			Tab_Points[1].Pt_XE 	= (S16)x3 ;
			Tab_Points[1].Pt_YE 	= (S16)z3 ;
			Tab_Points[2].Pt_XE 	= (S16)x0 ;
			Tab_Points[2].Pt_YE 	= (S16)z0 ;

			if( TestVuePoly( Tab_Points ) )
			{
				color = (ptrh->Bank << 4) + 11 	;

				Tab_Points[0].Pt_Light	= (S16)i2 ;
				Tab_Points[1].Pt_Light 	= (S16)i3 ;
				Tab_Points[2].Pt_Light 	= (S16)i0 ;

				Tab_Points[0].Pt_ZO     = (U16)f2 ;
				Tab_Points[1].Pt_ZO     = (U16)f3 ;
				Tab_Points[2].Pt_ZO     = (U16)f0 ;

				if( ptrh->PolyFlag )
				{
					if( ptrh->PolyFlag == 1 )
					{
						color += ((i0+i2+i3)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}
					Fill_Poly( ListTypePolygone[ptrh->PolyFlag],
							color, 3,
							Tab_Points ) ;
				}

				if( ptrh->TexFlag )
				{
					T_HALF_TEX	*ptrt		;

					ptrt = &ListTexDef[ ptrh->IndexTex ] ;

					Tab_Points[0].Pt_MapU = ptrt->Tx0 ;
					Tab_Points[0].Pt_MapV = ptrt->Ty0 ;
					Tab_Points[1].Pt_MapU = ptrt->Tx1 ;
					Tab_Points[1].Pt_MapV = ptrt->Ty1 ;
					Tab_Points[2].Pt_MapU = ptrt->Tx2 ;
					Tab_Points[2].Pt_MapV = ptrt->Ty2 ;

					if( ptrh->TexFlag == 2 )
					{
						color = ((i0+i2+i3)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}

					type = (U8)ListTypePTexture[ptrh->TexFlag+(ptrh->PolyFlag<<2)] ;

					if( Fill_Poly( type, color, 3, Tab_Points ) )
					{
						if( MaxPolySea AND (ptrh->CodeJeu & 12) == 12 )
						{ // hihi je suis un polygone animé...
							if( NbAniPoly < MaxPolySea-1 )
							{
								PtrAniPoly->X0 = (S16)Min3( x0, x2, x3 ) ;
								PtrAniPoly->Y0 = (S16)Min3( z0, z2, z3 ) ;
								PtrAniPoly->X1 = (S16)Max3( x0, x2, x3 ) ;
								PtrAniPoly->Y1 = (S16)Max3( z0, z2, z3 ) ;

								if( ((PtrAniPoly->Y1-PtrAniPoly->Y0)>ANI_POLY_STEP_Y)
								AND ((PtrAniPoly->X1-PtrAniPoly->X0)>ANI_POLY_STEP_X) )
								{
									PtrAniPoly->Type = type ;
									PtrAniPoly->Color = (U8)color ;

									PtrAniPoly->Fog = fog ;

									PtrAniPoly->Tab_Points[0] = Tab_Points[0] ;
									PtrAniPoly->Tab_Points[1] = Tab_Points[1] ;
									PtrAniPoly->Tab_Points[2] = Tab_Points[2] ;

									NbAniPoly++ ;
									PtrAniPoly++ ;
								}
							}
#ifdef DEBUG_TOOLS
							else
							{
								Message( "MaxPolySea Atteint !", TRUE ) ;
							}
#endif
						}

					}
				}
			}
		}
	}
	else	// type == 1---------------------------------------------------
	{
		// type 1 triangle 0

		if( ptrh->PolyFlag + ptrh->TexFlag )
		{
			Tab_Points[0].Pt_XE	= (S16)x3 ;
			Tab_Points[0].Pt_YE	= (S16)z3 ;
			Tab_Points[1].Pt_XE	= (S16)x0 ;
			Tab_Points[1].Pt_YE	= (S16)z0 ;
			Tab_Points[2].Pt_XE	= (S16)x1 ;
			Tab_Points[2].Pt_YE	= (S16)z1 ;

			if( TestVuePoly( Tab_Points ) )
			{
				color = (ptrh->Bank << 4) + 11 	;

				Tab_Points[0].Pt_Light	= (S16)i3 ;
				Tab_Points[1].Pt_Light	= (S16)i0 ;
				Tab_Points[2].Pt_Light	= (S16)i1 ;

				Tab_Points[0].Pt_ZO     = (U16)f3 ;
				Tab_Points[1].Pt_ZO     = (U16)f0 ;
				Tab_Points[2].Pt_ZO     = (U16)f1 ;

				if( ptrh->PolyFlag )
				{
					if( ptrh->PolyFlag == 1 )
					{
						color += ((i0+i1+i3)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}
					Fill_Poly( ListTypePolygone[ptrh->PolyFlag],
							color, 3,
							Tab_Points ) ;
				}

				if( ptrh->TexFlag )
				{
					T_HALF_TEX	*ptrt		;

					ptrt = &ListTexDef[ ptrh->IndexTex ] ;

					Tab_Points[0].Pt_MapU = ptrt->Tx0 ;
					Tab_Points[0].Pt_MapV = ptrt->Ty0 ;
					Tab_Points[1].Pt_MapU = ptrt->Tx1 ;
					Tab_Points[1].Pt_MapV = ptrt->Ty1 ;
					Tab_Points[2].Pt_MapU = ptrt->Tx2 ;
					Tab_Points[2].Pt_MapV = ptrt->Ty2 ;

					if( ptrh->TexFlag == 2 )
					{
						color = ((i0+i1+i3)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}

					type = (U8)ListTypePTexture[ptrh->TexFlag+(ptrh->PolyFlag<<2)] ;

					if( Fill_Poly( type, color, 3, Tab_Points ) )
					{
						if( MaxPolySea AND (ptrh->CodeJeu & 12) == 12 )
						{ // hihi je suis un polygone animé...
							if( NbAniPoly < MaxPolySea-1 )
							{
								PtrAniPoly->X0 = (S16)Min3( x0, x1, x3 ) ;
								PtrAniPoly->Y0 = (S16)Min3( z0, z1, z3 ) ;
								PtrAniPoly->X1 = (S16)Max3( x0, x1, x3 ) ;
								PtrAniPoly->Y1 = (S16)Max3( z0, z1, z3 ) ;

								if( ((PtrAniPoly->Y1-PtrAniPoly->Y0)>ANI_POLY_STEP_Y)
								AND ((PtrAniPoly->X1-PtrAniPoly->X0)>ANI_POLY_STEP_X) )
								{
									PtrAniPoly->Type = type ;
									PtrAniPoly->Color = (U8)color ;

									PtrAniPoly->Fog = fog ;

									PtrAniPoly->Tab_Points[0] = Tab_Points[0] ;
									PtrAniPoly->Tab_Points[1] = Tab_Points[1] ;
									PtrAniPoly->Tab_Points[2] = Tab_Points[2] ;

									NbAniPoly++ ;
									PtrAniPoly++ ;
								}
							}
#ifdef DEBUG_TOOLS
							else
							{
								Message( "MaxPolySea Atteint !", TRUE ) ;
							}
#endif
						}
					}
				}
			}
		}
			// type 1 triangle 1

		ptrh++ ;

		if( ptrh->PolyFlag + ptrh->TexFlag )
		{
			Tab_Points[0].Pt_XE 	= (S16)x1 ;
			Tab_Points[0].Pt_YE 	= (S16)z1 ;
			Tab_Points[1].Pt_XE 	= (S16)x2 ;
			Tab_Points[1].Pt_YE 	= (S16)z2 ;
			Tab_Points[2].Pt_XE	= (S16)x3 ;
			Tab_Points[2].Pt_YE	= (S16)z3 ;

			if( TestVuePoly( Tab_Points ) )
			{
				color = (ptrh->Bank << 4) + 11 ;

				Tab_Points[0].Pt_Light 	= (S16)i1 ;
				Tab_Points[1].Pt_Light 	= (S16)i2 ;
				Tab_Points[2].Pt_Light	= (S16)i3 ;

				Tab_Points[0].Pt_ZO     = (U16)f1 ;
				Tab_Points[1].Pt_ZO     = (U16)f2 ;
				Tab_Points[2].Pt_ZO     = (U16)f3 ;

				if( ptrh->PolyFlag )
				{
					if( ptrh->PolyFlag == 1 )
					{
						color += ((i1+i2+i3)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}

					Fill_Poly( ListTypePolygone[ptrh->PolyFlag],
							color, 3,
							Tab_Points ) ;
				}

				if( ptrh->TexFlag )
				{
					T_HALF_TEX	*ptrt		;

					ptrt = &ListTexDef[ ptrh->IndexTex ] ;

					Tab_Points[0].Pt_MapU = ptrt->Tx0 ;
					Tab_Points[0].Pt_MapV = ptrt->Ty0 ;
					Tab_Points[1].Pt_MapU = ptrt->Tx1 ;
					Tab_Points[1].Pt_MapV = ptrt->Ty1 ;
					Tab_Points[2].Pt_MapU = ptrt->Tx2 ;
					Tab_Points[2].Pt_MapV = ptrt->Ty2 ;

					if( ptrh->TexFlag == 2 )
					{
						color = ((i1+i2+i3)*21845)>>24 ;	// *21845 >> 16 EQU / 3
					}

					type = (U8)ListTypePTexture[ptrh->TexFlag+(ptrh->PolyFlag<<2)] ;

					if( Fill_Poly( type, color, 3, Tab_Points ) )
					{
						if( MaxPolySea AND (ptrh->CodeJeu & 12) == 12 )
						{ // hihi je suis un polygone animé...
							if( NbAniPoly < MaxPolySea-1 )
							{
								PtrAniPoly->X0 = (S16)Min3( x1, x2, x3 ) ;
								PtrAniPoly->Y0 = (S16)Min3( z1, z2, z3 ) ;
								PtrAniPoly->X1 = (S16)Max3( x1, x2, x3 ) ;
								PtrAniPoly->Y1 = (S16)Max3( z1, z2, z3 ) ;

								if( ((PtrAniPoly->Y1-PtrAniPoly->Y0)>ANI_POLY_STEP_Y)
								AND ((PtrAniPoly->X1-PtrAniPoly->X0)>ANI_POLY_STEP_X) )
								{
									PtrAniPoly->Type = type ;
									PtrAniPoly->Color = (U8)color ;

									PtrAniPoly->Fog = fog ;

									PtrAniPoly->Tab_Points[0] = Tab_Points[0] ;
									PtrAniPoly->Tab_Points[1] = Tab_Points[1] ;
									PtrAniPoly->Tab_Points[2] = Tab_Points[2] ;

									NbAniPoly++ ;
									PtrAniPoly++ ;
								}
							}
#ifdef DEBUG_TOOLS
							else
							{
								Message( "MaxPolySea Atteint !", TRUE ) ;
							}
#endif
						}

					}
				}
			}
		}
	}

#if !(defined(DEMO)||defined(LBA_GAME))
	if( FillFeuille & 4 )
	{
		DrawColFeuille3D( 	x0, z0,
					x1, z1,
					x2, z2,
					x3, z3,
					MapPolyGround[zi*64*2+xi*2].Sens,
					CYAN,
					MapPolyGround[zi*64*2+xi*2].Col,
					MapPolyGround[zi*64*2+xi*2+1].Col
					);
	}

	if( FillFeuille & 2 )
	{
		DrawFeuille3D( 	x0, z0,
				x1, z1,
				x2, z2,
				x3, z3,
				MapPolyGround[zi*64*2+xi*2].Sens,
				GREEN,
				FALSE );
	}
#endif
}

/*──────────────────────────────────────────────────────────────────────────*/
static inline void FillBlackPolyZBuf( S32 xi, S32 zi, T_SOMMET_ROT *ptrs, U8 *ptra)
{
	T_HALF_POLY	*ptrh		;
//	S32 		f0, f1, f2, f3 	;
//	S32 		f00, f10, f20, f30 ;
	S32 		x0, z0 		;
	S32 		x1, z1 		;
	S32 		x2, z2 		;
	S32 		x3, z3 		;
	U8		b0, b1, b2, b3	;

	ptrh = &MapPolyGround[zi*(64*2)+xi*2] ;

	x0 = ptrs[0].X2D	;
	z0 = ptrs[0].Y2D 	;
	b0 = (U8)(ptra[0]&32) 	;
	x1 = ptrs[65].X2D 	;
	z1 = ptrs[65].Y2D 	;
	b1 = (U8)(ptra[65]&32) 	;
	x3 = ptrs[1].X2D 	;
	z3 = ptrs[1].Y2D 	;
	b3 = (U8)(ptra[1]&32) 	;
	x2 = ptrs[66].X2D 	;
	z2 = ptrs[66].Y2D 	;
	b2 = (U8)(ptra[66]&32) 	;

/*
	xi *= 512 ;
	zi *= 512 ;

	f0 = GET_ZO(ptrs[0].Zrot)	;
	f1 = GET_ZO(ptrs[65].Zrot)	;
	f3 = GET_ZO(ptrs[1].Zrot)	;
	f2 = GET_ZO(ptrs[66].Zrot)	;

	LongWorldRotatePoint(xi, 0, zi)	;
	f00 = GET_ZO(CameraZr - Z0)	;
	LongWorldRotatePoint(xi, 0, zi+512);
	f10 = GET_ZO(CameraZr - Z0)	;
	LongWorldRotatePoint(xi+512, 0, zi);
	f30 = GET_ZO(CameraZr - Z0)	;
	LongWorldRotatePoint(xi+512, 0, zi+512);
	f20 = GET_ZO(CameraZr - Z0)	;
*/

	if(!(b0|b1))
	{
		Tab_Points[0].Pt_XE	= (S16)x0 ;
		Tab_Points[0].Pt_YE	= (S16)z0 ;
		Tab_Points[1].Pt_XE	= (S16)x1 ;
		Tab_Points[1].Pt_YE	= (S16)z1 ;
		Tab_Points[2].Pt_XE	= (S16)x1 ;
		Tab_Points[2].Pt_YE	= (S16)479;
		Tab_Points[3].Pt_XE	= (S16)x0 ;
		Tab_Points[3].Pt_YE	= (S16)479;

		Tab_Points[0].Pt_Light	= (S16)0  ;
		Tab_Points[1].Pt_Light 	= (S16)0  ;
		Tab_Points[2].Pt_Light 	= (S16)0  ;
		Tab_Points[3].Pt_Light 	= (S16)0  ;

		Tab_Points[0].Pt_ZO     = (U16)0  ;
		Tab_Points[1].Pt_ZO     = (U16)0  ;
		Tab_Points[2].Pt_ZO     = (U16)0  ;
		Tab_Points[3].Pt_ZO     = (U16)0  ;

		Fill_Poly( 0, 0, 4, Tab_Points ) ;
	}

	if(!(b1|b2))
	{
		Tab_Points[0].Pt_XE	= (S16)x1 ;
		Tab_Points[0].Pt_YE	= (S16)z1 ;
		Tab_Points[1].Pt_XE	= (S16)x2 ;
		Tab_Points[1].Pt_YE	= (S16)z2 ;
		Tab_Points[2].Pt_XE	= (S16)x2 ;
		Tab_Points[2].Pt_YE	= (S16)479;
		Tab_Points[3].Pt_XE	= (S16)x1 ;
		Tab_Points[3].Pt_YE	= (S16)479;

		Tab_Points[0].Pt_Light	= (S16)0  ;
		Tab_Points[1].Pt_Light 	= (S16)0  ;
		Tab_Points[2].Pt_Light 	= (S16)0  ;
		Tab_Points[3].Pt_Light 	= (S16)0  ;

		Tab_Points[0].Pt_ZO     = (U16)0  ;
		Tab_Points[1].Pt_ZO     = (U16)0  ;
		Tab_Points[2].Pt_ZO     = (U16)0  ;
		Tab_Points[3].Pt_ZO     = (U16)0  ;

		Fill_Poly( 0, 0, 4, Tab_Points ) ;
	}

	if(!(b2|b3))
	{
		Tab_Points[0].Pt_XE	= (S16)x2 ;
		Tab_Points[0].Pt_YE	= (S16)z2 ;
		Tab_Points[1].Pt_XE	= (S16)x3 ;
		Tab_Points[1].Pt_YE	= (S16)z3 ;
		Tab_Points[2].Pt_XE	= (S16)x3 ;
		Tab_Points[2].Pt_YE	= (S16)479;
		Tab_Points[3].Pt_XE	= (S16)x2 ;
		Tab_Points[3].Pt_YE	= (S16)479;

		Tab_Points[0].Pt_Light	= (S16)0  ;
		Tab_Points[1].Pt_Light 	= (S16)0  ;
		Tab_Points[2].Pt_Light 	= (S16)0  ;
		Tab_Points[3].Pt_Light 	= (S16)0  ;

		Tab_Points[0].Pt_ZO     = (U16)0  ;
		Tab_Points[1].Pt_ZO     = (U16)0  ;
		Tab_Points[2].Pt_ZO     = (U16)0  ;
		Tab_Points[3].Pt_ZO     = (U16)0  ;

		Fill_Poly( 0, 0, 4, Tab_Points ) ;
	}

	if(!(b3|b0))
	{
		Tab_Points[0].Pt_XE	= (S16)x3 ;
		Tab_Points[0].Pt_YE	= (S16)z3 ;
		Tab_Points[1].Pt_XE	= (S16)x0 ;
		Tab_Points[1].Pt_YE	= (S16)z0 ;
		Tab_Points[2].Pt_XE	= (S16)x0 ;
		Tab_Points[2].Pt_YE	= (S16)479;
		Tab_Points[3].Pt_XE	= (S16)x3 ;
		Tab_Points[3].Pt_YE	= (S16)479;

		Tab_Points[0].Pt_Light	= (S16)0  ;
		Tab_Points[1].Pt_Light 	= (S16)0  ;
		Tab_Points[2].Pt_Light 	= (S16)0  ;
		Tab_Points[3].Pt_Light 	= (S16)0  ;

		Tab_Points[0].Pt_ZO     = (U16)0  ;
		Tab_Points[1].Pt_ZO     = (U16)0  ;
		Tab_Points[2].Pt_ZO     = (U16)0  ;
		Tab_Points[3].Pt_ZO     = (U16)0  ;

		Fill_Poly( 0, 0, 4, Tab_Points ) ;
	}
}
/*──────────────────────────────────────────────────────────────────────────*/
struct S_Terrain
	{
		S8 x ;
		S8 y ;
	} ;

struct S_Terrain	TerrNO[9] = {
	{-1,-1}, {0,-1}, {1,-1},
	{-1,0}, {0,0}, {1,0},
	{-1,1}, {0,1}, {1,1}
	} ;

struct S_Terrain	TerrNE[9] = {
	{1,-1}, {0,-1}, {-1,-1},
	{1,0}, {0,0}, {-1,0},
	{1,1}, {0,1}, {-1,1}
	} ;

struct S_Terrain	TerrSO[9] = {
	{-1,1}, {0,1}, {1,1},
	{-1,0}, {0,0}, {1,0},
	{-1,-1}, {0,-1}, {1,-1}
	} ;

struct S_Terrain	TerrSE[9] = {
	{1,1}, {0,1}, {-1,1},
	{1,0}, {0,0}, {-1,0},
	{1,-1}, {0,-1}, {-1,-1}
	} ;

struct S_Terrain	*TerrainTri ;

S32	OffPosCamera[3]	= {	// offsets camera precalculés
	-(512*64),	// -1
	0,		//  0
	(512*64)	//  1
	} ;

extern	S32	PtrProjectPoint( S32 xw, S32 yw, S32 zw ) ;

void DrawHorizon2ZBuf()
{
	S32	flag = FALSE ;
	S32	cx = CameraX ;
	S32	cy = CameraY ;
	S32	cz = CameraZ ;

	if( BetaCam>=0 AND BetaCam<1024 )
	{
		TerrainTri = TerrSO ;
	}
	else if( BetaCam>=1024 AND BetaCam<2048 )
	{
		TerrainTri = TerrNO ;
	}
	else if( BetaCam>=2048 AND BetaCam<3072 )
	{
		TerrainTri = TerrNE ;
	}
	else if( BetaCam>=3072 AND BetaCam<4096 )
	{
		TerrainTri = TerrSE ;
	}
/*
	if( BetaCam>=0 AND BetaCam<1024 )
	{
		TerrainTri = TerrNE ;
	}
	else if( BetaCam>=1024 AND BetaCam<2048 )
	{
		TerrainTri = TerrSE ;
	}
	else if( BetaCam>=2048 AND BetaCam<3072 )
	{
		TerrainTri = TerrSO ;
	}
	else if( BetaCam>=3072 AND BetaCam<4096 )
	{
		TerrainTri = TerrNO ;
	}
*/
	for(S32 n=9; n>0; n--, TerrainTri++ )
	{
		S32	x,y ;

		x = CurrentCubeX + TerrainTri->x ;
		y = CurrentCubeY + TerrainTri->y ;

		if( x>=0 AND x<=15 AND y>=0 AND y<=15 )
		{
			if( TerrainTri->x OR TerrainTri->y )
			{
				// PreClipping du Cube
/*				if( PtrProjectPoint( 0, 0, 0 )
				OR  PtrProjectPoint( 32767, 0, 0 )
				OR  PtrProjectPoint( 32767, 0, 32767 )
				OR  PtrProjectPoint( 0, 0, 32767 ) )
*/				{
#ifdef	LBA_GAME
				SetPosCamera( cx-OffPosCamera[TerrainTri->x+1]+TerrainTri->x, cy, cz-OffPosCamera[TerrainTri->y+1]+TerrainTri->y ) ;
#else
				SetPosCamera( cx-OffPosCamera[TerrainTri->x+1]+TerrainTri->x*Reglage, cy, cz-OffPosCamera[TerrainTri->y+1]+TerrainTri->y*Reglage ) ;
#endif
					if( LoadCube( x, y ) )
					{
						AffichageTerrainZBuf() ;
						flag = TRUE ;
					}
					else	DrawOneSea( Y_SEA ) ;
				}
			}
		}
	}

	SetPosCamera( cx, cy, cz ) ;

	if( flag )
	{
		if( !LoadCube( CurrentCubeX, CurrentCubeY ) )
		{
			// on est dans un cube phantom !!!!!!!
			InitSeaCube() ;
			DrawOneSea( Y_SEA ) ;
		}

	}

/*#ifndef	_WIN32
if( ModeHercule )	HPrintf( "CubeBitField= %d\n", CubeBitField ) ;
#endif
*/
}

/*──────────────────────────────────────────────────────────────────────────*/
U32	MaskVisible(U8 *ptr)	;

#pragma	aux MaskVisible	=		\
"       mov     al, [esi]       "       \
"       mov     dl, [esi+65]    "       \
"				"	\
"	mov	ah, al		"	\
"	and	al, dl		"	\
"				"	\
"	or	ah, dl		"	\
"       mov     dl, [esi+1]     "       \
"				"	\
"       and     al, dl          "       \
"	or	ah, dl		"	\
"				"	\
"       mov     dl, [esi+66]    "       \
"				"	\
"	or	ah, dl		"	\
"       and     al, dl          "       \
"				"	\
"       and     eax, 20FFh      "       \
	parm		[esi]		\
	modify exact	[eax edx]

/*──────────────────────────────────────────────────────────────────────────*/
void	AffichageTerrainZBuf( void )
{
	S16		*ptrm	;
	U8		*ptra	;
	T_SOMMET_ROT	*ptrs	;

	// affichage des objets du decors
	AffichageObjetDecorsZBuf() ;

	// calcul les coordonnées 2D de chaque points
	ptra = &SommetAffichable[0][0];
	ptrm = MapSommetY ;
	ptrs = &SommetRot[0][0] ;

	for (S32 z=0; z<=NB_COTE*512; z+=512)
	{
		for (S32 x=0; x<=NB_COTE*512; x+=512)
		{
			S32	zr ;

			LongWorldRotatePoint(x, *ptrm, z);

			zr = CameraZr - Z0 ;

			// cone clipping

			if(zr<=NearClip)
			{
				*ptra = 32	;
			}
			else if( (zr<ClipZFar)
				AND LongProjectPoint( X0,Y0,Z0 ) )
			{
				U8	flag	;

				flag = 16	;

				if( Xp < ClipXMin )	flag |= 1 ;
				if( Xp > ClipXMax )	flag |= 2 ;
				if( Yp < ClipYMin )	flag |= 4 ;
				if( Yp > ClipYMax )	flag |= 8 ;

				*ptra		= flag 	;

				ptrs->Zrot 	= zr;

				ptrs->X2D	= (S16)Xp;
				ptrs->Y2D	= (S16)Yp;

			}
			else
			{
				*ptra = 0	;
			}

			ptra++;
			ptrm++;
			ptrs++;
		}
	}

	PtrMap = GroundTexture ;
	RepMask = 0xFFFF ;

	ptra  = &SommetAffichable[0][0] ;
	ptrs  = &SommetRot[0][0] ;

	for(z=0; z<NB_COTE; z++)
	{
		for(S32 x=0; x<NB_COTE; x++)
		{
			S32 mask ;

			mask = MaskVisible(ptra) ;

			switch(mask&0xFF)
			{
			case 32:
				break;
			case 16:
				DrawFeuillePolyZBuf( x, z, ptrs) ;
				break;
			case 0:
				if(mask)
				{
					FillBlackPolyZBuf( x, z, ptrs, ptra) ;
				}
				break;
			}

			ptra++	;
			ptrs++	;
		}

		ptra++	;
		ptrs++	;
	}

	DrawOneSea( Y_SEA ) ;

#ifdef	LBA_EDITOR
	if( FlagInfos&INFO_DECORS )
	{
		AffichageObjetDecorsZV() ;
	}
#endif

	BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;
}

/*

void	AffichageTerrainZBuf( void )
{
	S16		*ptrm	;
	U8		*ptra	;
	T_SOMMET_ROT	*ptrs	;

	// affichage des objets du decors
	AffichageObjetDecorsZBuf() ;

	// calcul les coordonnées 2D de chaque points
	ptra = &SommetAffichable[0][0];
	ptrm = MapSommetY ;
	ptrs = &SommetRot[0][0] ;

	for (S32 z=0; z<=NB_COTE*512; z+=512)
	{
		for (S32 x=0; x<=NB_COTE*512; x+=512)
		{
			S32	zr ;

			LongWorldRotatePoint(x, *ptrm, z);

			zr = CameraZr - Z0 ;// l'ecran

			// cone clipping

			if( (zr<ClipZFar)
			AND (zr>NearClip)
			AND LongProjectPoint( X0,Y0,Z0 ) )
			{
				U8	flag	;

				flag = 16	;

				if( Xp < ClipXMin )	flag |= 1 ;
				if( Xp > ClipXMax )	flag |= 2 ;
				if( Yp < ClipYMin )	flag |= 4 ;
				if( Yp > ClipYMax )	flag |= 8 ;

				*ptra		= flag 	;

				ptrs->Zrot 	= zr;

				ptrs->X2D	= (S16)Xp;
				ptrs->Y2D	= (S16)Yp;

			}
			else
			{
				*ptra = 0	;
			}

			ptra++;
			ptrm++;
			ptrs++;
		}
	}

	PtrMap = GroundTexture ;
	RepMask = 0xFFFF ;

	ptra  = &SommetAffichable[0][0] ;
	ptrs  = &SommetRot[0][0] ;

	for(z=0; z<NB_COTE; z++)
	{
		for(S32 x=0; x<NB_COTE; x++)
		{
			if (MaskVisible(ptra) == 16)
			{
				DrawFeuillePolyZBuf( x, z, ptrs) ;
			}

			ptra++	;
			ptrs++	;
		}

		ptra++	;
		ptrs++	;
	}

	BoxStaticAdd( 0, 0, ModeDesiredX-1, ModeDesiredY-1 ) ;
}

*/
/*──────────────────────────────────────────────────────────────────────────*/
//****************************************************************************

void	AffichageTerrainRapide( void )
{
	for (S32 xi=0; xi<=NB_COTE; xi++)
	{
		for (S32 zi=0; zi<=NB_COTE; zi++)
		{
			LongWorldRotatePoint( 	xi*512,
						(S32)MapSommetY[zi*65+xi],
						zi*512 );

			if( LongProjectPoint( X0,Y0,Z0 ) )
			{
				Plot( Xp, Yp, WHITE ) ;
			}
		}
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
/*──────────────────────────────────────────────────────────────────────────*/

void	CopyBlock256( U8 *sour, U8 *dest )
{
	S32 y ;

	for( y=0; y<32; y++ )
	{
		memcpy( dest, sour, 32 ) ;
		dest += 256 ;
		sour += 256 ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	RefreshAniPoly = TRUE ;
S32	NbToRefresh = 0 ;

/*──────────────────────────────────────────────────────────────────────────*/
void	ClearAniPoly( void )
{
	NbAniPoly = 0 ;
	PtrAniPoly = PtrPolySea ;
	RefreshAniPoly = NbAniPoly ;
	NbToRefresh    = NbAniPoly/4 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void	DoAnimatedPolys()
{
	if( !NbAniPoly OR !MaxPolySea )	return ;

	if( TimerRefHR > TimerAniPoly )
	{
		CntAniPoly8++ ;
		CntAniPoly8&=7 ;// 0-8
		CntAniPoly6++ ;
		CntAniPoly6%=6 ;// 0-5

		TimerAniPoly = TimerRefHR + 1000/4 ; // voir pour nb frame/sec

		if( OffsetAniPoly1 != -1 )
		{
			CopyBlock256(	SkySeaTexture+ 128*256 + CntAniPoly8*32,
					GroundTexture + OffsetAniPoly1 ) ;
		}

		if( OffsetAniPoly2 != -1 )
		{
			CopyBlock256(	SkySeaTexture + (128+32)*256 + CntAniPoly8*32,
					GroundTexture + OffsetAniPoly2 ) ;
		}

		if( OffsetAniPoly3 != -1 )
		{
			CopyBlock256(	SkySeaTexture+ (128+32+32)*256 + 32*2 + CntAniPoly6*32,
					GroundTexture + OffsetAniPoly3 ) ;
		}

		if( OffsetAniPoly4 != -1 )
		{
			CopyBlock256(	SkySeaTexture + (128+32+32+32)*256 + 32*2 + CntAniPoly6*32,
					GroundTexture + OffsetAniPoly4 ) ;
		}

		RefreshAniPoly = NbAniPoly ;
		NbToRefresh    = NbAniPoly/4 ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawAnimatedPolys()
{
	S32		n ;
	S_ANI_POLY	*ptr ;

	if( !NbAniPoly OR !MaxPolySea OR !RefreshAniPoly )	return ;

	PtrMap = GroundTexture ;
	RepMask = 0xFFFF ;

	Switch_Fillers( FILL_POLY_FOG_NZW ) ;

	if( NbToRefresh>RefreshAniPoly )	NbToRefresh = RefreshAniPoly ;

	ptr = PtrAniPoly = (S_ANI_POLY*)&PtrPolySea[RefreshAniPoly-1] ;
	for( n=0; n<NbToRefresh; n++, ptr-- )
	{
		UnsetClip() ;

		SetClip( ptr->X0,ptr->Y0,ptr->X1,ptr->Y1 ) ;

		if( ClipXMin <= ClipXMax
		AND ClipYMin <= ClipYMax )
		{
			SetCLUT( ptr->Fog ) ;

			Fill_Poly( ptr->Type, ptr->Color, 3, ptr->Tab_Points ) ;

			BoxStaticAdd( ClipXMin, ClipYMin, ClipXMax, ClipYMax ) ;
		}

		RefreshAniPoly-- ;
	}

	// protec au cas ou
	if( RefreshAniPoly<0 )	RefreshAniPoly = 0 ;

	// Met a jour Screen pour recouvrements
	BoxBlitStaticListToScreen() ;

	Switch_Fillers( FILL_POLY_TEXTURES ) ;
	SetCLUT( PalLevel ) ;

	UnsetClip() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	FiltreAnimatedPolys()
{
	S32		n ;
	S_ANI_POLY	*ptr ;

	if( !NbAniPoly OR !MaxPolySea )	return ;

	ptr = PtrAniPoly = (S_ANI_POLY*)PtrPolySea ;

	for( n=0; n<NbAniPoly; )
	{
		Fill_Poly( POLY_FLAG_ZBUFFER, 0, 3, ptr->Tab_Points ) ;

		if( IsPolygonHidden )
		{
			if( NbAniPoly>1 )
			{
				memcpy( (char*)ptr, (char*)&PtrPolySea[NbAniPoly-1], sizeof(S_ANI_POLY) ) ;
			}
			NbAniPoly-- ;
		}
		else
		{
			n++ ;
			ptr++ ;
		}
	}
}
