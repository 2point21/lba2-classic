//****************************************************************************
#ifndef	LIB_SVGA_SCALESPI
#define	LIB_SVGA_SCALESPI

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
void	ScaleSpriteTransp(	S32	num,
				S32	x,		S32	y,
				S32	factorx,	S32	factory,
				void	*ptrbank,	void	*ptr_transp ) ;


#pragma aux	ScaleSpriteTransp "*"			\
	parm caller	[]				\
	modify		[eax ebx ecx edx esi edi]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_SVGA_SCALESPI

//****************************************************************************

