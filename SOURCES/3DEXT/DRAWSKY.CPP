
S32	Draw_Poly( STRUC_CLIPVERTEX src[], S32 type )
{
	S32	k, l ;
	STRUC_CLIPVERTEX	dst[10];
	STRUC_CLIPVERTEX	dst1[10];

	k = ClipperZ(dst1, src, 4, NearClip + 1, 0);

	if ( k )
	{
		k = ClipperZ(dst, dst1, k, Fill_Z_Fog_Far, -1);
		if ( k )
		{
			for ( l=0; l!=k; l++ )
			{
				if (!LongProjectPoint(dst[l].V_X0+CameraXr, dst[l].V_Y0+CameraYr, CameraZr-dst[l].V_Z0 )) return FALSE ;
				if( (Xp!=(S32)(S16)Xp) OR (Yp!=(S32)(S16)Yp) ) return FALSE ;
				Tab_Points[l].Pt_XE = (S16)Xp;
				Tab_Points[l].Pt_YE = (S16)Yp;
				Tab_Points[l].Pt_Light = dst[l].V_Light;
				Tab_Points[l].Pt_MapU = dst[l].V_MapU;
				Tab_Points[l].Pt_MapV = dst[l].V_MapV;
				Tab_Points[l].Pt_W  = INV64(dst[l].V_Z0);
				Tab_Points[l].Pt_ZO = GET_ZO(dst[l].V_Z0);
			}

			if( Fill_Poly(type, 0, k, Tab_Points) ) return TRUE ;
		}
	}
	return FALSE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	DrawSky(S32 y)
{
	U32	xx, zz, i, j;
	STRUC_CLIPVERTEX	src[10];

	PtrMap = SkySeaTexture + 128 ;
	RepMask = 0x7F7F ;
	PtrCLUTGouraud = PtrCLUTFog ;
//	SetCLUT( PalLevel ) ;

	src[0].V_MapU  = 0;
	src[0].V_MapV  = 0;
	src[1].V_MapU  = 0;
	src[1].V_MapV  = 127*256+255 ;
	src[2].V_MapU  = 127*256+255 ;
	src[2].V_MapV  = 127*256+255 ;
	src[3].V_MapU  = 127*256+255 ;
	src[3].V_MapV  = 0;

	zz = -WORLD_SIZE;
	for ( i=0; i<NB_POLYS_COTE*3; i++, zz+=WORLD_SIZE/NB_POLYS_COTE )
	{
	xx = -WORLD_SIZE;
	for ( j=0; j<NB_POLYS_COTE*3; j++, xx+=WORLD_SIZE/NB_POLYS_COTE )
	{

		LongWorldRotatePoint( xx, y, zz ) ;
		src[0].V_X0    = X0-CameraXr ;
		src[0].V_Y0    = Y0-CameraYr ;
		src[0].V_Z0    = CameraZr-Z0 ;

		LongWorldRotatePoint( xx+WORLD_SIZE/NB_POLYS_COTE, y, zz ) ;
		src[1].V_X0    = X0-CameraXr ;
		src[1].V_Y0    = Y0-CameraYr ;
		src[1].V_Z0    = CameraZr-Z0 ;

		LongWorldRotatePoint( xx+WORLD_SIZE/NB_POLYS_COTE, y, zz+WORLD_SIZE/NB_POLYS_COTE ) ;
		src[2].V_X0    = X0-CameraXr ;
		src[2].V_Y0    = Y0-CameraYr ;
		src[2].V_Z0    = CameraZr-Z0 ;

		LongWorldRotatePoint( xx, y, zz+WORLD_SIZE/NB_POLYS_COTE ) ;
		src[3].V_X0    = X0-CameraXr ;
		src[3].V_Y0    = Y0-CameraYr ;
		src[3].V_Z0    = CameraZr-Z0 ;

		Draw_Poly(&src[0],24) ;
	}
	}
}


void	DrawOneSea(S32 y)			// Si on souhaite que l'eau ne soit pas au niveau 0 :)
{
	STRUC_CLIPVERTEX	src[10];
	U32	i,j,xx, zz;
	S32	b ;

	PtrMap = SkySeaTexture ;
	RepMask = 0x7F7F ;
	PtrCLUTGouraud = PtrCLUTFog ;
	SetCLUT( PalLevel ) ;

	b = 0 ;

	src[0].V_MapU = 0	 	;	// xt0
	src[0].V_MapV = 0	 	;	// yt0
	src[1].V_MapU = 0	 	;	// xt1
	src[1].V_MapV = 127*256+255		;	// yt1
	src[2].V_MapU = 127*256+255		;	// xt2
	src[2].V_MapV = 127*256+255		;	// yt2
	src[3].V_MapU = 127*256+255		;	// xt3
	src[3].V_MapV = 0	 	;	// yt3

	zz = 0 ;
	for ( i=0; i<NB_POLYS_COTE; i++, zz+=WORLD_SIZE/NB_POLYS_COTE )
	{
	xx = 0 ;
	for ( j=0; j<NB_POLYS_COTE; j++, xx+=WORLD_SIZE/NB_POLYS_COTE )
	{
		if( CubeBitField & (1<<b) )
		{
			LongWorldRotatePoint( xx, y, zz ) ;
			src[0].V_X0    = X0-CameraXr ;
			src[0].V_Y0    = Y0-CameraYr ;
			src[0].V_Z0    = CameraZr-Z0 ;

			LongWorldRotatePoint( xx, y, zz+WORLD_SIZE/NB_POLYS_COTE ) ;
			src[1].V_X0    = X0-CameraXr ;
			src[1].V_Y0    = Y0-CameraYr ;
			src[1].V_Z0    = CameraZr-Z0 ;

			LongWorldRotatePoint( xx+WORLD_SIZE/NB_POLYS_COTE, y, zz+WORLD_SIZE/NB_POLYS_COTE ) ;
			src[2].V_X0    = X0-CameraXr ;
			src[2].V_Y0    = Y0-CameraYr ;
			src[2].V_Z0    = CameraZr-Z0 ;

			LongWorldRotatePoint( xx+WORLD_SIZE/NB_POLYS_COTE, y, zz ) ;
			src[3].V_X0    = X0-CameraXr ;
			src[3].V_Y0    = Y0-CameraYr ;
			src[3].V_Z0    = CameraZr-Z0 ;

			Draw_Poly(src,24) ;
		}
		else
		{
			// poly noir ?
/* on verra		LongWorldRotatePoint( xx, y, zz ) ;
			src[0].V_X0    = X0-CameraXr ;
			src[0].V_Y0    = Y0-CameraYr ;
			src[0].V_Z0    = CameraZr-Z0 ;

			LongWorldRotatePoint( xx, y, zz+WORLD_SIZE/NB_POLYS_COTE ) ;
			src[1].V_X0    = X0-CameraXr ;
			src[1].V_Y0    = Y0-CameraYr ;
			src[1].V_Z0    = CameraZr-Z0 ;

			LongWorldRotatePoint( xx+WORLD_SIZE/NB_POLYS_COTE, y, zz+WORLD_SIZE/NB_POLYS_COTE ) ;
			src[2].V_X0    = X0-CameraXr ;
			src[2].V_Y0    = Y0-CameraYr ;
			src[2].V_Z0    = CameraZr-Z0 ;

			LongWorldRotatePoint( xx+WORLD_SIZE/NB_POLYS_COTE, y, zz ) ;
			src[3].V_X0    = X0-CameraXr ;
			src[3].V_Y0    = Y0-CameraYr ;
			src[3].V_Z0    = CameraZr-Z0 ;

			Draw_Poly(src,0) ;
*/
		}
		b++ ;
	}
	}
}

