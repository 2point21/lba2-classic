;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

		include	3d.inc
		include	iammx.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	ProcessorSignature:DWORD
		extrn	SYSCALL	ProcessorFeatureFlags:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	XCentre:	DWORD
		extrn	SYSCALL	YCentre:	DWORD

		extrn	SYSCALL	NearClip:	DWORD

		extrn	SYSCALL	LFactorX:	DWORD
		extrn	SYSCALL	LFactorY:	DWORD

		extrn	SYSCALL	TypeProj:	DWORD

		extrn	SYSCALL	FRatioX:	DWORD
		extrn	SYSCALL	FRatioY:	DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
	        extrn   SYSCALL LongProjectPoint:		DWORD
	        extrn   SYSCALL LongRotateProjectPoint:		DWORD
	        extrn   SYSCALL LongWorldRotateProjectPoint:	DWORD

                extrn   SYSCALL ProjectList:			DWORD
                extrn   SYSCALL LongProjectList:		DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
                extrn   SYSCALL LongProjectPointInit:           PROC
                extrn   SYSCALL LongRotateProjectPointInit:     PROC
                extrn   SYSCALL LongWorldRotateProjectPointInit:PROC

                extrn   SYSCALL LongProjectPointIso:		PROC
                extrn   SYSCALL LongWorldRotateProjectPointIso:	PROC

                extrn   SYSCALL ProjectListInit:		PROC
                extrn   SYSCALL LongProjectListInit:		PROC

                extrn   SYSCALL ProjectListIso:			PROC
                extrn   SYSCALL LongProjectListIso:		PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	SetProjection
		public	SYSCALL	SetIsoProjection

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	SetProjection(xc, yc, clip, factorx, factory)

;#pragma aux SetProjection	"*"			\
;	parm caller [eax] [ebx] [ecx] [edx] [esi]	\
;	modify exact [eax ebx ecx edx]

SetProjection	PROC

		mov	[LFactorX], edx
		mov	[LFactorY], esi

		mov	edx, [ProcessorSignature]

		and	edx, 400h
		jz	noFPU

		test	[ProcessorFeatureFlags], 1 SHL 23
		jz	noMMX

		emms
noMMX:
		fild	[LFactorX]	; fx
		fild	[LFactorY]	; fy fx
		fxch	st(1)		; fx fy
		fst	[FRatioX]	; fx fy
		fchs			;-fx fy
		fdivp	st(1), st(0)	; -fy/fx
noFPU:
		mov	[XCentre], eax
		mov	[YCentre], ebx

		mov	[NearClip], ecx
		mov	[TypeProj], TYPE_3D

		mov	eax, offset LongProjectPointInit
		mov	ebx, offset LongRotateProjectPointInit
		mov	ecx, offset LongWorldRotateProjectPointInit
		mov	LongProjectPoint, eax
		mov	LongRotateProjectPoint, ebx
		mov	LongWorldRotateProjectPoint, ecx
		mov	eax, offset ProjectListInit
		mov	ebx, offset LongProjectListInit
		mov	ProjectList, eax
		mov	LongProjectList, ebx

		test	edx, edx
		jz	noFPU2

		fstp	[FRatioY]
noFPU2:
		ret

SetProjection	ENDP

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	SetIsoProjection(xc, yc)

;#pragma aux SetIsoProjection	"*"	\
;	parm caller [eax] [ebx]		\
;	modify	[ecx]

SetIsoProjection PROC

		mov	[XCentre], eax
		mov	[YCentre], ebx

		mov	[TypeProj], TYPE_ISO

		mov	eax, offset LongProjectPointIso
		mov	ebx, offset LongRotateProjectPointInit
		mov	ecx, offset LongWorldRotateProjectPointIso
		mov	LongProjectPoint, eax
		mov	LongRotateProjectPoint, ebx
		mov	LongWorldRotateProjectPoint, ecx
		mov	eax, offset ProjectListIso
		mov	ebx, offset LongProjectListIso
		mov	ProjectList, eax
		mov	LongProjectList, ebx

		ret

SetIsoProjection ENDP

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
