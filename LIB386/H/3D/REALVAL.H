//****************************************************************************
#ifndef	LIB_3D_REALVAL
#define	LIB_3D_REALVAL

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef struct {	S32	StartValue ;
			S32	EndValue  ;
			U32	TimeValue ;
			U32	MemoTicks ;	} T_REAL_VALUE_HR ;

//****************************************************************************
void	InitRealValueHR(S32 StartValue, S32 EndValue, S32 TimeValue, T_REAL_VALUE_HR *RealVal);

#pragma aux InitRealValueHR	"*"		\
	parm caller		[eax] [ebx] [ecx] [edi]	\
	modify exact	[edx]

//****************************************************************************
void	InitRealAngleHR(S32 StartAngle, S32 EndAngle, S32 TimeValue, T_REAL_VALUE_HR *RealVal);

#pragma aux InitRealAngleHR	"*"		\
	parm caller		[eax] [ebx] [ecx] [edi]	\
	modify exact	[eax ebx edx]

//****************************************************************************
void	InitRealAngleConstHR(S32 StartAngle, S32 EndAngle, S32 TimeQuart, T_REAL_VALUE_HR *RealVal);

#pragma aux InitRealAngleConstHR	"*"	\
	parm caller		[eax] [ebx] [ecx] [edi]	\
	modify exact	[eax ebx ecx edx]

//****************************************************************************
S32	GetRealValueHR(T_REAL_VALUE_HR *RealVal);

#pragma aux GetRealValueHR	"*"		\
	parm caller		[esi]			\
	modify		[eax ebx ecx edx]

//****************************************************************************
S32	GetRealAngleHR(T_REAL_VALUE_HR *RealVal);

#pragma aux GetRealAngleHR	"*"		\
	parm caller		[esi]			\
	modify		[eax ebx ecx edx]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_REALVAL

//****************************************************************************

