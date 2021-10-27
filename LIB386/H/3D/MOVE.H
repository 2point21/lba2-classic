//---------------------------------------------------------------------------
#ifndef	LIB_3D_MOVE
#define	LIB_3D_MOVE

//---------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//---------------------------------------------------------------------------
#pragma	pack(push, 4)

//---------------------------------------------------------------------------
typedef	struct
	{
		S32	Speed		;
		S32	Acc		;
		U32	LastTimer	;
	}
	MOVE	;

//---------------------------------------------------------------------------
typedef	struct
	{
		MOVE	Move		;
		S32	Cur		;
		S32	End		;
	}
	BOUND_MOVE	;

//---------------------------------------------------------------------------
#pragma	pack(pop)

//---------------------------------------------------------------------------
S32	GetDeltaAccMove(S32 *acc)	;

#pragma aux GetDeltaAccMove	=       \
"       mov     ecx, 1000       "       \
"       mov     eax, [ebx]      "       \
"	cmp	eax, ecx	"	\
"	jge	ok		"	\
"	cmp	eax, -1000	"	\
"	jg	skip		"	\
"ok:    mov     edx, eax        "       \
"       sar     edx, 31         "       \
"       idiv    ecx            	"       \
"       mov     [ebx], edx      "       \
"	jmp	end		"	\
"skip:	xor	eax, eax	"	\
"end:				"	\
        parm    caller  [ebx]           \
	modify	exact	[ecx edx]	\
	value		[eax]

//---------------------------------------------------------------------------
S32	AdjustSpeedAngle(S32 speed, S32 start, S32 end)	;

#pragma aux AdjustSpeedAngle	=       	\
"	xor	eax, eax	"		\
"	and	ecx, 4095	"		\
"	and	edx, 4095	"		\
"	sub	edx, ecx	"		\
"	jz	ok		"		\
"	mov	eax, ebx	"		\
"	jns	skip		"		\
"	neg	edx		"		\
"	neg	eax		"		\
"skip:	cmp	edx, 2048	"		\
"	jl      ok		"		\
"chg:	neg	eax		"		\
"ok:				"		\
        parm    caller  [ebx] [ecx] [edx]	\
	modify	exact	[ecx edx]		\
	value		[eax]

//---------------------------------------------------------------------------
#define	GetSpeedMove(pmove)		(pmove)->Speed

//---------------------------------------------------------------------------
#define	RestartMove(pmove)			\
{						\
	(pmove)->Acc	   = 500	;	\
	(pmove)->LastTimer = TimerRefHR	;	\
}

//---------------------------------------------------------------------------
void	InitMove(MOVE *pmove, S32 speed);

#pragma aux InitMove	"*"       		\
        parm caller  [edi] [eax]

//---------------------------------------------------------------------------
void	ChangeSpeedMove(MOVE *pmove, S32 speed);

#pragma aux ChangeSpeedMove	"*"       	\
        parm caller  [edi] [eax]

//---------------------------------------------------------------------------
void	ChangeSpeedAngleMove(MOVE *pmove, S32 speed, S32 start, S32 end);

#pragma aux ChangeSpeedAngleMove "*"       	\
        parm caller  [edi] [ebx] [ecx] [edx]

//---------------------------------------------------------------------------
void	InitAngleMove(MOVE *pmove, S32 speed, S32 start, S32 end);

#pragma aux InitAngleMove	"*"       	\
        parm caller  [edi] [ebx] [ecx] [edx]

//---------------------------------------------------------------------------
void	ChangeSpeedBoundAngleMove(BOUND_MOVE *pbmove, S32 speed, S32 end);

#pragma aux ChangeSpeedBoundAngleMove "*"       \
        parm caller  [edi] [ebx] [edx]

//---------------------------------------------------------------------------
void	InitBoundMove(BOUND_MOVE *pbmove, S32 speed, S32 start, S32 end);

#pragma aux InitBoundMove	"*"       	\
        parm caller  [edi] [ebx] [ecx] [edx]

//---------------------------------------------------------------------------
void	InitBoundAngleMove(BOUND_MOVE *pbmove, S32 speed, S32 start, S32 end);

#pragma aux InitBoundAngleMove	"*"       	\
        parm caller  [edi] [ebx] [ecx] [edx]

//---------------------------------------------------------------------------
extern	S32	GetDeltaMove(MOVE *pmove)	;

#pragma	aux GetDeltaMove	"*"	\
	parm caller	[ebx]		\
	modify		[ecx edx]	\
	value		[eax]

//---------------------------------------------------------------------------
extern	S32	GetBoundMove(BOUND_MOVE *pbmove);

#pragma	aux GetBoundMove	"*"	\
	parm caller	[ebx]		\
	modify		[ecx edx]	\
	value		[eax]

//---------------------------------------------------------------------------
extern	S32	GetBoundAngleMove(BOUND_MOVE *pbmove);

#pragma	aux GetBoundAngleMove	"*"	\
	parm caller	[ebx]		\
	modify		[ecx edx]	\
	value		[eax]

//---------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//---------------------------------------------------------------------------
#endif//LIB_3D_MOVE

//---------------------------------------------------------------------------
