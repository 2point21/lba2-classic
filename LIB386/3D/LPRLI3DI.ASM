;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL NearClip:DWORD

		extrn	SYSCALL LFactorX:DWORD
		extrn	SYSCALL LFactorY:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL ScreenXMin:DWORD
		extrn	SYSCALL ScreenXMax:DWORD
		extrn	SYSCALL ScreenYMin:DWORD
		extrn	SYSCALL ScreenYMax:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	LongProjectList3DI

;*══════════════════════════════════════════════════════════════════════════*
;void	LongProjectList3DF(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux LongProjectList3DF			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARING: OrgX, OrgY OrgZ pushed on the stack

LongProjectList3DI proc

		push	ebp
		lea	ebp, [ecx*4]

		neg	ebp

		lea	edi, [edi+ecx*4-4]
		jmp	EnterLoop

LoopProj:
		cmp	edx, 32767*2
		ja	OverFlow
FixedOverFlow:
		mov	[edi+ebp], eax
EnterLoop:
		mov	ebx, [esi+8]
		mov	ecx, [esp+16]		; OrgZ

		sub	ecx, ebx
		mov	ebx, [NearClip]

		cmp	ecx, ebx		; Z
		jl	Clip

		mov	eax, [esi]
		mov	ebx, [esp+12]		; OrgY

		sub	eax, [esp+8]		; OrgX
		sub	ebx, [esi+4]

		imul	[LFactorX]		; X
		idiv	ecx			; / Z

		xchg	eax, ebx
		add	esi, 12

		imul	[LFactorY]              ; -Y
		idiv	ecx			; / Z

		mov	ecx, [XCentre]
		mov	edx, [YCentre]

		add	ebx, ecx		; Xe
		add	eax, edx		; Ye

		mov	ecx, [ScreenXMin]
		mov	edx, [ScreenXMax]

		cmp	ebx, ecx
		jl	UpdateXMin
OkXMin:
		cmp	ebx, edx
		jg	UpdateXMax
OkXMax:
		mov	ecx, [ScreenYMin]
		mov	edx, [ScreenYMax]

		cmp	eax, ecx
		jl	UpdateYMin
OkYMin:
		cmp	eax, edx
		jg	UpdateYMax
OkYMax:
		lea	edx, [ebx+32767]	; Saturation
		lea	ecx, [eax+32767]

		shl	eax, 16
		and	ebx, 0FFFFh

		or	edx, ecx
		or	eax, ebx

		add	ebp, 4
		jnz	LoopProj

		cmp	edx, 32767*2
		ja	OverFlow2
FixedOverFlow2:
		mov	[edi+ebp], eax
		pop	ebp

		ret
UpdateXMin:
		mov	[ScreenXMin], ebx
		jmp	OkXMin
UpdateXMax:
		mov	[ScreenXMax], ebx
		jmp	OkXMax
UpdateYMin:
		mov	[ScreenYMin], eax
		jmp	OkYMin
UpdateYMax:
		mov	[ScreenYMax], eax
		jmp	OkYMax
Clip:
		mov	eax, 80008000h

		add	ebp, 4
		jz	FixedOverFlow2

		jmp	FixedOverFlow

OverFlow:
		mov	eax, 80008000h
		jmp	FixedOverFlow
OverFlow2:
		mov	eax, 80008000h
		jmp	FixedOverFlow2

LongProjectList3DI endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
