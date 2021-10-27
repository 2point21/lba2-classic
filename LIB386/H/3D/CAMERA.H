//****************************************************************************
#ifndef	LIB_3D_CAMERA
#define	LIB_3D_CAMERA

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif

//****************************************************************************
extern  S32 LFactorX	            	;
extern  S32 LFactorY	            	;

extern  S32 CameraAlpha               	;
extern  S32 CameraBeta                	;
extern  S32 CameraGamma               	;

extern  S32 CameraX             	;
extern  S32 CameraY             	;
extern  S32 CameraZ             	;

extern  S32 CameraXr            	;
extern  S32 CameraYr            	;
extern  S32 CameraZr            	;

extern  S32 CameraZrClip        	;

extern  S32 NearClip            	;

extern  S32 XCentre	            	;
extern  S32 YCentre	            	;

extern  S32 Xp		            	;
extern  S32 Yp		            	;

extern	S32 X0				;
extern	S32 Y0				;
extern	S32 Z0				;

extern  S32 TypeProj            	;

//****************************************************************************
extern  float FRatioX	            	;
extern  float FRatioY	            	;

#pragma aux FRatioX	"*"
#pragma aux FRatioY	"*"

//****************************************************************************
extern void	InitCamera(	S32 camx, S32 camy, S32 camz,
				S32 tgtx, S32 tgty, S32 tgtz,
				S32 gamma)			;

#pragma aux InitCamera	"*"					\
	parm		[edx] [esi] [edi] [eax] [ecx] [ebx]

// WARNING this function uses PASCAL calling convention for 'gamma' on the stack.
// DO NOT ADD 'CALLER' in the PRAGMA

//****************************************************************************
extern void	SetTargetCamera(S32 x, S32 y, S32 z, S32 gamma);

#pragma aux SetTargetCamera	"*"		\
	parm caller	[eax] [ecx] [ebx] [edx]	\
	modify		[esi edi]

//****************************************************************************
extern void	SetAngleCamera(S32 alpha, S32 beta, S32 gamma);

#pragma	aux SetAngleCamera	"*"		\
	parm caller	[eax] [ebx] [ecx]	\
	modify		[edx esi edi]

//****************************************************************************
extern void	SetPosCamera(S32 x, S32 y, S32 z)	;

#pragma	aux SetPosCamera	"*"		\
	parm caller	[eax] [ebx] [ecx]	\
	modify		[edx esi edi]

//****************************************************************************
extern void	SetFollowCamera(S32 targetx, S32 targety, S32 targetz,
				S32 alpha, S32 beta, S32 gamma,
				S32 camzoom)	;

#pragma	aux SetFollowCamera	"*"				\
	parm caller	[edx] [esi] [edi] [eax] [ebx] [ecx]

// WARNING : last param "camzoom" passed on the stack!

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_3D_CAMERA

//****************************************************************************

