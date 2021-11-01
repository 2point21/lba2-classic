;*══════════════════════════════════════════════════════════════════════════*

		.386p
		OPTION	LANGUAGE:SYSCALL
		OPTION	SCOPED

		include	3d.inc

;*══════════════════════════════════════════════════════════════════════════*
DGROUP		GROUP	_DATA

_DATA		SEGMENT USE32 PAGE PUBLIC 'DATA'

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	MatriceWorld:DWORD

		extrn	SYSCALL	InitMatrix:DWORD
		extrn	SYSCALL	LongRotatePoint:DWORD
		extrn	SYSCALL	LongInverseRotatePoint:DWORD

;*══════════════════════════════════════════════════════════════════════════*
ALIGN	32					; already aligned
		public	SYSCALL Xp
		public	SYSCALL Yp

Xp		dd	0000
Yp		dd	0000

		public	SYSCALL XCentre
		public	SYSCALL YCentre

XCentre		dd	0000
YCentre		dd	0000

		public	SYSCALL X0
		public	SYSCALL Y0
		public	SYSCALL Z0

X0		dd	0000
Y0		dd	0000
Z0		dd	0000

		public	SYSCALL NearClip
NearClip	dd	1

ALIGN	32					; already aligned
		public	SYSCALL LFactorX
		public	SYSCALL LFactorY

LFactorX	dd	0000
LFactorY	dd	0000

		public	SYSCALL FRatioX
		public	SYSCALL FRatioY

FRatioX		dd	0000
FRatioY		dd	0000

		public	SYSCALL CameraXr
		public	SYSCALL CameraYr
		public	SYSCALL CameraZr
CameraXr	dd	0000
CameraYr	dd	0000
CameraZr	dd	0000

		public	SYSCALL TypeProj
TypeProj	dd	0000

ALIGN	32					; already aligned
		public	SYSCALL CameraX
		public	SYSCALL CameraY
		public	SYSCALL CameraZ
CameraX		dd	0000
CameraY		dd	0000
CameraZ		dd	0000

		public	SYSCALL CameraZrClip
CameraZrClip	dd	0000

		public	SYSCALL CameraAlpha
		public	SYSCALL CameraBeta
		public	SYSCALL CameraGamma

CameraAlpha	dd	0000
CameraBeta	dd	0000
CameraGamma	dd	0000

		; One DWORD Available HERE
ALIGN	32
		public	SYSCALL MMX_DEMI
MMX_DEMI	dd	1 SHL 13
		dd	1 SHL 13

		public	SYSCALL MMX_DEMI2
MMX_DEMI2	dd	1 SHL 12
		dd	1 SHL 12

;*══════════════════════════════════════════════════════════════════════════*
_DATA		ENDS

;*══════════════════════════════════════════════════════════════════════════*
_TEXT		SEGMENT	USE32 PARA PUBLIC 'CODE'
       		ASSUME	CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	GetAngleVector3D:PROC
		extrn	SYSCALL	RecomputeLight:PROC

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	SetTargetCamera
		public	SYSCALL	SetAngleCamera
		public	SYSCALL	SetPosCamera
		public	SYSCALL	SetFollowCamera

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	SetCamera	; for use in LIB ONLY

;*══════════════════════════════════════════════════════════════════════════*
;void	InitCamera(	S32 camx, S32 camy, S32 camz,
;			S32 tgtx, S32 tgty, S32 tgtz,
;			S32 gamma)			;

;#pragma aux InitCamera	"*"					\
;	parm		[edx] [esi] [edi] [eax] [ecx] [ebx]

; WARNING: Gamma goes ON THE STACK

InitCamera	proc

		mov	CameraX, edx
		mov	CameraY, esi
		mov	CameraZ, edi

		pop	esi	; return address
		pop	edx	; gamma
		push	esi	; return address

;		WARNING continue below

;*══════════════════════════════════════════════════════════════════════════*
;void	SetTargetCamera(S32 x, S32 y, S32 z, S32 gamma)	;

;#pragma aux SetTargetCamera	"*"		\
;	parm		[eax] [ecx] [ebx] [edx]	\
;	modify		[esi edi]

SetTargetCamera::

		push	edx

		sub	ecx, CameraZ
		sub	ebx, CameraY
		sub	eax, CameraX

		call	GetAngleVector3D

		mov	eax, [X0]
		mov	ebx, [Y0]
		pop	ecx

;		WARNING continue below

;*══════════════════════════════════════════════════════════════════════════*
;void	SetAngleCamera(S32 alpha, S32 beta, S32 gamma)	;

;#pragma aux SetAngleCamera	"*"		\
;	parm		[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

SetAngleCamera::
		cmp	[TypeProj], TYPE_3D
		jne	tiso

		and	eax, 4095
		and	ebx, 4095
		and	ecx, 4095
tback:
		mov	edi, offset MatriceWorld

		mov	CameraAlpha, eax
		mov	CameraBeta, ebx

		mov	CameraGamma, ecx
		call	[InitMatrix]

		call	RecomputeLight

		mov     eax, CameraX
		mov     ebx, CameraY
		mov     ecx, CameraZ

SetCamera::					; for use in lib
						; see SetPosCamera
		cmp	[TypeProj], TYPE_3D
		jne	iso

		mov	esi, offset MatriceWorld

		call    [LongRotatePoint]

		mov     eax, X0
		mov     ebx, Y0
		mov     ecx, Z0

		mov     CameraXr, eax
		mov     edx, ecx
		mov     CameraYr, ebx
		sub	edx, NearClip
		mov     CameraZr, ecx
		mov     CameraZrClip, edx

		ret
iso:
		mov	CameraXr, eax
		xor	eax, eax
		mov	CameraYr, ebx
		mov	CameraZr, ecx
		mov	CameraZrClip, ecx
		mov	NearClip, eax

		ret
tiso:
		xor	eax, eax
		xor	ebx, ebx
		xor	ecx, ecx
		jmp	tback

InitCamera	endp

;*══════════════════════════════════════════════════════════════════════════*
;void	SetPosCamera(S32 x, S32 y, S32 z)	;

;#pragma aux SetAngleCamera	"*"		\
;	parm		[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

SetPosCamera	proc

		mov	CameraX, eax
		mov	CameraY, ebx
		mov	CameraZ, ecx
		jmp	SetCamera

SetPosCamera	endp

;*══════════════════════════════════════════════════════════════════════════*
;void	SetFollowCamera(S32 targetx, S32 targety, S32 targetz,
;			S32 alpha, S32 beta, S32 gamma,
;			S32 camzoom)	;

;#pragma aux SetAngleCamera	"*"			\
;	parm		[edx] [esi] [edi] [eax] [ebx] [ecx]

; WARNING : last param "camzoom" passed on the stack!

SetFollowCamera	proc,	\
		camzoom:DWORD

		mov     [CameraX], edx
		mov     [CameraY], esi
		mov     [CameraZ], edi

		call	SetAngleCamera

		cmp	[TypeProj], TYPE_3D
		jne	iso

		mov	esi, camzoom
		mov	eax, [CameraXr]
		mov	ebx, [CameraYr]
		mov	ecx, [CameraZr]
		mov	edx, [CameraZrClip]
		add	ecx, esi
		add	edx, esi
		mov	[CameraZr], ecx
		mov	[CameraZrClip], edx

		mov	esi, offset MatriceWorld

		call	[LongInverseRotatePoint]

		mov	eax, [X0]
		mov	ebx, [Y0]
		mov	ecx, [Z0]
		mov	[CameraX], eax
		mov	[CameraY], ebx
		mov	[CameraZ], ecx
iso:
		ret

SetFollowCamera	endp

;*══════════════════════════════════════════════════════════════════════════*
_TEXT		ENDS

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
