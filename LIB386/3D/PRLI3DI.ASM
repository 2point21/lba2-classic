;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL NearClip:DWORD

		extrn	SYSCALL LFactorX:DWORD
		extrn	SYSCALL LFactorY:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL ScreenXMin:DWORD
		extrn	SYSCALL ScreenXMax:DWORD
		extrn	SYSCALL ScreenYMin:DWORD
		extrn	SYSCALL ScreenYMax:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	ProjectList3DI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	ProjectList3DF(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux ProjectList3DF			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARING: OrgX, OrgY OrgZ pushed on the stack

ProjectList3DI proc

		push	ebp
		lea	ebp, [ecx*4]

		lea	esi, [esi+ecx*8]
		neg	ebp

		lea	edi, [edi+ecx*4-4]
		jmp	EnterLoop
LoopProj:
		mov	[edi+ebp], eax
EnterLoop:
		movsx	ebx, word ptr[esi+ebp*2+4]

		mov	eax, [esi+ebp*2]
		mov	ecx, [esp+16]		; OrgZ

		sub	ecx, ebx

		cmp	ecx, [NearClip]		; Z
		jl	OverFlow

		mov	ebx, eax

		shl	eax, 16

		sar	ebx, 16

		sar	eax, 16
		sub	ebx, [esp+12]		; OrgY

		sub	eax, [esp+8]		; OrgX

		neg	ebx			; -Y

		imul	[LFactorX]		; X
		idiv	ecx			; / Z

		xchg	eax, ebx

		imul	[LFactorY]              ; -Y
		idiv	ecx			; / Z

		mov	ecx, [XCentre]
		mov	edx, [YCentre]

		add	ebx, ecx		; Xe
		add	eax, edx		; Ye

		cmp	ebx, 32767
		jg	OverFlow

		cmp	ebx, -32767
		jl	OverFlow

		cmp	eax, 32767
		jg	OverFlow

		cmp	eax, -32767
		jl	OverFlow

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
		shl	eax, 16
		and	ebx, 0FFFFh

		or	eax, ebx
		add	ebp, 4

		jnz	LoopProj
Exit:
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
OverFlow:
		add	ebp, 4
		mov	eax, 80008000h

		jnz	LoopProj

		jmp	Exit

ProjectList3DI endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
