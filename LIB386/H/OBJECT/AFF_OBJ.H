//****************************************************************************
#ifndef	LIB_OBJ_AFFOBJ
#define	LIB_OBJ_AFFOBJ

//****************************************************************************
#ifdef	__cplusplus
extern	"C"	{
#endif



//                ***************
//                *** EQUATES ***
//                ***************


#define	TYPE_ROTATE		0
#define	TYPE_TRANSLATE		1


//                ******************
//                *** STRUCTURES ***
//                ******************

//****************************************************************************
#pragma pack(push, 1)

//****************************************************************************
typedef struct	{
			S16		Type			;
			S16		Alpha			;
			S16		Beta			;
			S16		Gamma			;

		}	T_GROUP_INFO	;


typedef union	{
			void	*Ptr				;
			S32	Num				;

		} 	T_PTR_NUM	;


typedef struct {
			S32		X			;	// world coor
			S32		Y			;
			S32		Z			;

			S32		Alpha			;	// local angles
			S32		Beta			;
			S32		Gamma			;

			T_PTR_NUM	Body;			;	// ->.O3D
			T_PTR_NUM	NextBody;               ;	// ->.O3D
			T_PTR_NUM	Anim                    ;	// ->.A3D

			void		*Texture		;	//
			void		*NextTexture		;	//

			U32		LastOfsIsPtr		;

			S32		LastFrame		;
			U32		LastOfsFrame		;
			U32		LastTimer		;	// last TimerRef
			U32		LastNbGroups		;

			S32		NextFrame		;
			U32		NextOfsFrame		;
			U32		NextTimer		;	// TimerRef when frame reached
			U32		NextNbGroups		;

			S32		LoopFrame		;
			U32		LoopOfsFrame		;

			U32		NbFrames		;

			S32		LastAnimStepX		;
			S32		LastAnimStepY		;
			S32		LastAnimStepZ		;

			S32		LastAnimStepAlpha	;
			S32		LastAnimStepBeta	;
			S32		LastAnimStepGamma	;

			U32		Interpolator		;	// lib internal

			U32		Time			;	// TimerRef last modif

			U32		Status			;	// BitField FLAG_*

			U32		Master			;	// BitField ANIM_MASTER_ROT

			U32		NbGroups		;

			T_GROUP_INFO	CurrentFrame[30]	;

		}	T_OBJ_3D	;


//****************************************************************************
typedef struct {
			S16		  X			;
			S16		  Y			;
			S16		  Z			;
			S16		  Group			;

		}	T_OBJ_POINT	;

//****************************************************************************

typedef struct {
	S32	Info		;
	S16	SizeHeader	;
	S16	Dummy		;
	S32	XMin		;
	S32	XMax		;
	S32	YMin		;
	S32	YMax		;
	S32	ZMin		;
	S32	ZMax		;
	S32	NbGroupes	;
	S32	OffGroupes	;
	S32	NbPoints	;
	S32	OffPoints	;
	S32	NbNormales	;
	S32	OffNormales	;
	S32	NbNormFaces	;
	S32	OffNormFaces	;
	S32	NbPolys		;
	S32	OffPolys	;
	S32	NbLines		;
	S32	OffLines	;
	S32	NbSpheres	;
	S32	OffSpheres	;
	S32	NbTextures	;
	S32	OffTextures	;
	}	T_BODY_HEADER	;

//****************************************************************************





#pragma pack(pop)

//****************************************************************************
#define	MASK_OBJECT_VERSION	(0xff)		//; Dans Obj_Info
#define	MASK_OBJECT_ANIMATED	(1 << 8)
#define	MASK_OBJECT_NO_SORT	(1 << 9)
#define	MASK_OBJECT_TRANSPARENT	(1 << 10)

//****************************************************************************
//			AFF_OBJ.ASM			//

//****************************************************************************
extern	U8		*ObjPtrMap;	// Ptr on the current texture page
#pragma aux		ObjPtrMap	"*"

//****************************************************************************
extern	T_OBJ_POINT	Obj_ListRotatedPoints[];
#pragma aux		Obj_ListRotatedPoints	"*"

//****************************************************************************
extern	S32	BodyDisplay(	S32 x, S32 y, S32 z,
				S32 alpha, S32 beta, S32 gamma,
			 	void *obj			);

#pragma aux	BodyDisplay		"*"	       		\
	parm caller	[eax] [ebx] [ecx] [edx] [esi] [edi]

//****************************************************************************
extern	S32	BodyDisplay_AlphaBeta(	S32 x, S32 y, S32 z,
					S32 alpha, S32 beta, S32 gamma,
			 		void *obj			);

#pragma aux	BodyDisplay_AlphaBeta	"*"	       		\
	parm caller	[eax] [ebx] [ecx] [edx] [esi] [edi]

//****************************************************************************
extern	S32	ObjectDisplay(	T_OBJ_3D *obj );

#pragma aux	ObjectDisplay		"*"	       		\
	parm caller	[eax]					\
	modify		[ebx ecx edx esi edi]

//****************************************************************************
#ifdef	__cplusplus
}
#endif

//****************************************************************************
#endif//LIB_OBJ_AFFOBJ

//****************************************************************************
