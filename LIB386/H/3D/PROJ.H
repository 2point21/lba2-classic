//****************************************************************************
#ifndef	LIB_3D_PROJ
#define	LIB_3D_PROJ

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
#define	TYPE_3D		0
#define	TYPE_ISO	1

//****************************************************************************
void	SetProjection(S32 xcentre, S32 ycentre, S32 clip, S32 factorx, S32 factory);

#pragma aux SetProjection	"*"			\
	parm caller [eax] [ebx] [ecx] [edx] [esi]	\
	modify exact [eax ebx ecx edx]


//****************************************************************************
void	SetIsoProjection(S32 xcentre, S32 ycentre);

#pragma aux SetIsoProjection	"*"	\
	parm caller [eax] [ebx]		\
	modify	[ecx]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_PROJ

//****************************************************************************

