//****************************************************************************
#ifndef	LIB_3D_IMATTRA
#define	LIB_3D_IMATTRA

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_GetMatrixTrans)(TYPE_MAT *MatDst, S32 *tx, S32 *ty, S32 *tz) ;

#pragma	aux Arg_GetMatrixTrans	"*"		\
	parm caller		[esi] [eax] [ebx] [ecx]	\
	modify exact	[edx esi edi]

#pragma	aux (Arg_GetMatrixTrans)	Func_GetMatrixTrans

//****************************************************************************
extern	Func_GetMatrixTrans	*GetMatrixTrans	;

#pragma aux	GetMatrixTrans	"*"

//****************************************************************************
// These should not be called directly but through the above pointer
extern	Func_GetMatrixTrans	GetMatrixTransI	;
extern	Func_GetMatrixTrans	GetMatrixTransF	;
extern	Func_GetMatrixTrans	GetMatrixTransM	;

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_IMATTRA

//****************************************************************************

