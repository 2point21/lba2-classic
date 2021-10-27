//****************************************************************************
#ifndef	LIB_3D_IMAT
#define	LIB_3D_IMAT

//****************************************************************************
#include	"datamat.h"

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	void (Func_InitMatrix)(TYPE_MAT *MatDst, S32 alpha, S32 beta, S32 gamma) ;

#pragma	aux Arg_InitMatrix	"*"		\
	parm caller		[edi] [eax] [ebx] [ecx]	\
	modify exact	[eax ebx ecx edx esi]

#pragma	aux (Arg_InitMatrix)	Func_InitMatrix

//****************************************************************************
extern	Func_InitMatrix	*InitMatrix	;

#pragma	aux	InitMatrix	"*"

//****************************************************************************
extern	void	MatrixSTD();

#pragma	aux MatrixSTD	"*"	\
	modify exact []

// Switch to a 3DS camera orientation
// call this function BEFORE initialising camera

//****************************************************************************
extern	void	Matrix3DS();

#pragma	aux Matrix3DS	"*"	\
	modify exact []

// Switch to a "Normal" camera orientation
// call this function BEFORE initialising camera

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_IMAT

//****************************************************************************
