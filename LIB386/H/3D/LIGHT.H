//****************************************************************************
#ifndef	LIB_3D_LIGHT
#define	LIB_3D_LIGHT

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
#define	NORMAL_UNIT	16384

//****************************************************************************
extern		S32	AlphaLight,	BetaLight,	GammaLight	;
extern		S32	NormalXLight,	NormalYLight,	NormalZLight	;
extern		S32	CameraXLight,	CameraYLight,	CameraZLight	;
extern		S32	PosXLight,	PosYLight,	PosZLight	;
extern		S32	TargetXLight,	TargetYLight,	TargetZLight	;
extern		S32	LightNormalUnit					;

//****************************************************************************
void	SetPosLight(S32 x, S32 y, S32 z);

#pragma aux SetPosLight	"*"			\
	parm caller		[eax] [ebx] [ecx]	\
	modify		[edx esi edi]

//****************************************************************************
void	SetTargetLight(S32 targetx, S32 targety, S32 targetz);

#pragma aux SetTargetLight	"*"		\
	parm caller		[eax] [ebx] [ecx]	\
	modify		[edx esi edi]

//****************************************************************************
void	SetLightVector( S32 alpha, S32 beta, S32 gamma ) ;

#pragma aux SetLightVector	"*"		\
	parm caller		[eax] [ebx] [ecx]	\
	modify		[edx esi edi]

//****************************************************************************
#define	SetLightIntensity(intensity)	(LightNormalUnit = (intensity))

//****************************************************************************
void	RecomputeLight() ;	// SHOULD NOT BE USED

#pragma aux RecomputeLight	"*"		\
	modify		[eax ebx ecx edx esi edi]

//****************************************************************************

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LIGHT

//****************************************************************************

