
#include	"c_extern.h"


//****************************************************************************
//****************************************************************************
//────────────────────────────────────────────────────────────────────────────
//────────────────────────────────────────────────────────────────────────────
//	echelle 256 = 1
void	ScaleRotBody3D( U8 *ptbody, S32 scale, S32 flagrot )
{
	S16	*pt	;
	U32	i	;
	U32	nb, off	;
	S32 x0, y0, z0	;
	S32 x1, y1, z1	;
	T_BODY_HEADER	*pth ;

//----- La ZV
	pth = (T_BODY_HEADER *)ptbody	;

	x0 = (pth->XMin * scale)/256;
	x1 = (pth->XMax * scale)/256;
	y0 = (pth->YMin * scale)/256;
	y1 = (pth->YMax * scale)/256;
	z0 = (pth->ZMin * scale)/256;
	z1 = (pth->ZMax * scale)/256;

	if ( flagrot )
	{
		Rotate( z0, y0, 1024 )	;
		y0 = Z0			;
		z0 = X0			;
		Rotate( z1, y1, 1024 )	;
		y1 = Z0			;
		z1 = X0			;
	}

	pth->XMin = (S16)x0	;
	pth->XMax = (S16)y0	;
	pth->YMin = (S16)z0	;
	pth->YMax = (S16)x1	;
	pth->ZMin = (S16)y1	;
	pth->ZMax = (S16)z1	;

//----- Les Points 3D
	nb  = pth->NbPoints	;//	NbObjPoint
	off = pth->OffPoints	;//	Offset points
	pt = (S16*)(ptbody+off);//	Jump to points

	for ( i = 0 ; i < nb ; i++, pt += 4 )
	{
		x0 = (pt[0] * scale)/256	;//	X
		y0 = (pt[1] * scale)/256	;//	Y
		z0 = (pt[2] * scale)/256	;//	Z
		if ( flagrot )
		{
			Rotate( z0, y0, 1024 )	;
			y0 = Z0			;
			z0 = X0			;
		}
		pt[0] = (S16)x0	;
		pt[1] = (S16)y0	;
		pt[2] = (S16)z0	;
	}

	if ( flagrot )
	{

//----- Les Normals aux points et aux faces
		nb  = pth->NbNormales	;//	Nb Normals aux points
		off = pth->OffNormales	;//	Offset Normals
		nb += pth->NbNormFaces	;//	Add Normals aux faces
		pt = (S16*)(ptbody+off)	;

		for ( i = 0 ; i < nb ; i++, pt += 4 )
		{
			Rotate( pt[2], pt[1], 1024 )	;
			pt[1] = (S16)Z0		;
			pt[2] = (S16)X0		;
		}
	}

//----- Les Rayons des Spheres
	nb  = pth->NbSpheres	;//	NbSpheres
	off = pth->OffSpheres	;//	Offset Spheres
	pt = (S16*)(ptbody+off)	;

	pt += 3		;//	Jump type & coul & p1
	for ( i = 0 ; i < nb ; i++ , pt += 4 )
	{
		*pt = (S16)((*pt * scale)/256)	;//	Rayon
	}
}

//────────────────────────────────────────────────────────────────────────────
U32	GetSizeBody3D( U8 *ptbody )
{
	T_BODY_HEADER *pth ;
	U32	nb, off	;

	pth = (T_BODY_HEADER *)ptbody	;

	nb  = pth->NbTextures	;//	Nb Texture
	off = pth->OffTextures	;//	Offset Texture

	off += nb*4	;//	Add (orgMap+RepMask)*NbTexture

	return(off)	;

}
//────────────────────────────────────────────────────────────────────────────
//	echelle 256 = 1
void	ScaleRotAnim3D( U8 *ptanim, S32 scale, S32 flagrot )
{
	S16	*pt	;
	U16	nbframe	;
	U16	nbgroup	;
	U16	grp, frm;
	S32	x, y, z	;

	pt = (S16*)ptanim	;
	nbframe = *(U16*)pt++	;
	nbgroup = *(U16*)pt	;
	pt += 3			;

	for ( frm = 0 ; frm < nbframe ; frm++ )
	{
		pt++			;//	Jump Time
		x = (pt[0] * scale)/256	;//	StepX
		y = (pt[1] * scale)/256	;//	StepY
		z = (pt[2] * scale)/256	;//	StepZ
		if ( flagrot )
		{
			Rotate( z, y, 1024 )	;
			y = Z0		;
			z = X0		;
		}
		pt[0] = (S16)x		;
		pt[1] = (S16)y		;
		pt[2] = (S16)z		;

		pt += 7			;//	Jump MasterRot, al, be & ga

		for ( grp = 1 ; grp < nbgroup ; grp++, pt += 4 )
		{
			if ( *pt == 0 )	continue;// Only Transition
			x = (pt[1] * scale)/256	;//	StepX
			y = (pt[2] * scale)/256	;//	StepY
			z = (pt[3] * scale)/256	;//	StepZ
			if ( flagrot )
			{
				Rotate( z, y, 1024 )	;
				y = Z0		;
				z = X0		;
			}
			pt[1] = (S16)x		;
			pt[2] = (S16)y		;
			pt[3] = (S16)z		;
		}
	}
}
//────────────────────────────────────────────────────────────────────────────
//	echelle 256 = 1
U32	GetSizeAnim3D( U8 *ptanim )
{
	S16	*pt	;
	U16	nbframe	;
	U16	nbgroup	;

	pt = (S16*)ptanim	;

	nbframe = *(U16*)pt++	;
	nbgroup = *(U16*)pt	;

	pt += (3+(nbframe*(nbgroup+1)*4))	;

	return((U32)pt-(U32)ptanim)	;
}
//────────────────────────────────────────────────────────────────────────────
