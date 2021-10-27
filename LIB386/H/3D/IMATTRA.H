//****************************************************************************
#ifndef	LIB_3D_IMATTRA
#define	LIB_3D_IMATTRA

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_InitMatrixTrans)(TYPE_MAT *MatDst, S32 tx, S32 ty, S32 tz) ;

#pragma	aux Arg_InitMatrixTrans	"*"		\
	parm caller		[edi] [eax] [ebx] [ecx]	\
	modify exact	[]

#pragma	aux (Arg_InitMatrixTrans)	Func_InitMatrixTrans

//****************************************************************************
extern	Func_InitMatrixTrans	*InitMatrixTrans	;

#pragma aux	InitMatrixTrans	"*"

//****************************************************************************
// These should not be called directly but through the above pointer
extern	Func_InitMatrixTrans	InitMatrixTransI	;
extern	Func_InitMatrixTrans	InitMatrixTransF	;
extern	Func_InitMatrixTrans	InitMatrixTransM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_IMATTRA

//****************************************************************************

