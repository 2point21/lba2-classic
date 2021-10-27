//****************************************************************************
#ifndef	LIB_3D_REGLE3
#define	LIB_3D_REGLE3

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
S32	RegleTrois(S32 Val1, S32 Val2, S32 NbSteps, S32 CurrentStep);

#pragma aux RegleTrois		=		\
"		test	ecx, ecx	"	\
"		jle	exitregle3	"	\
"		sub	eax, ebx	"	\
"		imul	edx		"	\
"		idiv	ecx		"	\
"		add	eax, ebx	"	\
"exitregle3:				"	\
	parm caller	[ebx] [eax] [ecx] [edx]	\
	modify exact	[eax edx]		\
	value		[eax]

//****************************************************************************
S32	BoundRegleTrois(S32 Val1, S32 Val2, S32 NbSteps, S32 CurrentStep);

#pragma aux BoundRegleTrois	=		\
"		test	edx, edx	"	\
"		jle	smallboundregle3"	\
"		cmp	edx, ecx	"	\
"		jge	exitboundregle3	"	\
"		sub	eax, ebx	"	\
"		imul	edx		"	\
"		idiv	ecx		"	\
"		add	eax, ebx	"	\
"		jmp	exitboundregle3	"	\
"smallboundregle3:			"	\
"		mov	eax, ebx	"	\
"exitboundregle3:			"	\
	parm caller	[ebx] [eax] [ecx] [edx]	\
	modify exact	[eax edx]		\
	value		[eax]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_REGLE3

//****************************************************************************

