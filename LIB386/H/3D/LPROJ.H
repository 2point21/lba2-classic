//****************************************************************************
#ifndef	LIB_3D_LPROJ
#define	LIB_3D_LPROJ

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
typedef	S32 (Func_LongProjectPoint)(S32 x, S32 y, S32 z)	;

#pragma aux Args_LongProjectPoint	"*"	\
	parm caller	[eax] [ebx] [ecx]	\
	modify		[edx]

#pragma aux (Args_LongProjectPoint) Func_LongProjectPoint

//****************************************************************************
extern	Func_LongProjectPoint	*LongProjectPoint	;

#pragma	aux LongProjectPoint		"*"

//****************************************************************************
extern	Func_LongProjectPoint	LongProjectPointIso	;

//****************************************************************************
extern	Func_LongProjectPoint	*LongProjectPoint3D	;

#pragma	aux LongProjectPoint3D		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongProjectPoint	LongProjectPoint3DI	;
extern	Func_LongProjectPoint	LongProjectPoint3DF	;

//****************************************************************************
// This function is for the lib INTERNAL USE ONLY
extern	Func_LongProjectPoint	LongProjectPointInit	;

//****************************************************************************
//****************************************************************************
typedef	void (Func_LongRotateProjectPoint)(TYPE_MAT *Mat, S32 x, S32 y, S32 z) ;

#pragma aux Arg_LongRotateProjectPoint	"*"	\
	parm caller	[esi] [eax] [ebx] [ecx]	\
	modify exact	[eax ebx ecx edx edi]

#pragma	aux (Arg_LongRotateProjectPoint)	Func_LongRotateProjectPoint

//****************************************************************************
extern	Func_LongRotateProjectPoint	*LongRotateProjectPoint		;

#pragma	aux LongRotateProjectPoint		"*"

//****************************************************************************
// This function is for the lib INTERNAL USE ONLY
extern	Func_LongRotateProjectPoint	LongRotateProjectPointInit	;

//****************************************************************************
extern	Func_LongRotateProjectPoint	*LongRotateProjectPoint3D	;

#pragma	aux LongRotateProjectPoint3D		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongRotateProjectPoint	LongRotateProjectPoint3DI	;
extern	Func_LongRotateProjectPoint	LongRotateProjectPoint3DM	;
extern	Func_LongRotateProjectPoint	LongRotateProjectPoint3DF	;

//****************************************************************************
extern	Func_LongRotateProjectPoint	*LongRotateProjectPointIso	;

#pragma	aux LongRotateProjectPointIso		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongRotateProjectPoint	LongRotateProjectPointIsoI	;
extern	Func_LongRotateProjectPoint	LongRotateProjectPointIsoM	;
extern	Func_LongRotateProjectPoint	LongRotateProjectPointIsoF	;

//****************************************************************************
//****************************************************************************
typedef	void (Func_LongWorldRotateProjectPoint)(S32 x, S32 y, S32 z) 	;

#pragma aux Arg_LongWorldRotateProjectPoint	"*"	\
	parm caller	[eax] [ebx] [ecx]		\
	modify		[edx esi edi]

#pragma	aux (Arg_LongWorldRotateProjectPoint)	Func_LongWorldRotateProjectPoint

//****************************************************************************
extern	Func_LongWorldRotateProjectPoint	*LongWorldRotateProjectPoint		;

#pragma	aux LongWorldRotateProjectPoint		"*"

//****************************************************************************
// This function is for the lib INTERNAL USE ONLY
extern	Func_LongWorldRotateProjectPoint	LongWorldRotateProjectPointInit	;

//****************************************************************************
extern	Func_LongWorldRotateProjectPoint	*LongWorldRotateProjectPoint3D	;

#pragma	aux LongWorldRotateProjectPoint3D	"*"

//****************************************************************************
extern	Func_LongWorldRotateProjectPoint	LongWorldRotateProjectPointIso	;

//****************************************************************************
//****************************************************************************
typedef	S32 (Func_ProjectList)(	TYPE_PT *Dst, TYPE_VT16 *Src, S32 NbPt,
				S32 OrgX, S32 OrgY, S32 OrgZ)	;

#pragma aux Args_ProjectList	"*"		\
	parm caller	[edi] [esi] [ecx]	\
	modify		[eax ebx edx]

//WARNING: OrgX, OrgY, OrgZ pushed on the stack

#pragma aux (Args_ProjectList) Func_ProjectList

//****************************************************************************
extern	Func_ProjectList	*ProjectList	;

#pragma	aux ProjectList		"*"

//****************************************************************************
extern	Func_ProjectList	ProjectListIso	;

extern	Func_ProjectList	*ProjectList3D	;

#pragma	aux ProjectList3D		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_ProjectList	ProjectList3DI	;
extern	Func_ProjectList	ProjectList3DF	;
extern	Func_ProjectList	ProjectList3DMF	;

//****************************************************************************
// This function is for the lib INTERNAL USE ONLY
extern	Func_ProjectList	ProjectListInit	;


//****************************************************************************
//****************************************************************************
typedef	S32 (Func_LongProjectList)(	TYPE_PT *Dst, TYPE_VT16 *Src, S32 NbPt,
				S32 OrgX, S32 OrgY, S32 OrgZ)	;

#pragma aux Args_LongProjectList	"*"		\
	parm caller	[edi] [esi] [ecx]	\
	modify		[eax ebx edx]

//WARING: OrgX, OrgY, OrgZ pushed on the stack

#pragma aux (Args_LongProjectList) Func_LongProjectList

//****************************************************************************
extern	Func_LongProjectList	*LongProjectList	;

#pragma	aux LongProjectList		"*"

//****************************************************************************
extern	Func_LongProjectList	LongProjectListIso	;

extern	Func_LongProjectList	*LongProjectList3D	;

#pragma	aux LongProjectList3D		"*"

//****************************************************************************
// These functions should not be called directly but through the above pointer
// which auto-initalize to point to the right routine at runtime.
extern	Func_LongProjectList	LongProjectList3DI	;
extern	Func_LongProjectList	LongProjectList3DF	;
extern	Func_LongProjectList	LongProjectList3DMF	;

//****************************************************************************
// This function is for the lib INTERNAL USE ONLY
extern	Func_LongProjectList	LongProjectListInit	;


//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_LPROJ

//****************************************************************************

