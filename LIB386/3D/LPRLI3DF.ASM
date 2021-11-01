;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model FLAT, SYSCALL

		include	mat.inc
		include	iammx.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL NearClip:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL Xp:DWORD
		extrn	SYSCALL Yp:DWORD

		extrn	SYSCALL X0:DWORD
		extrn	SYSCALL Y0:DWORD
		extrn	SYSCALL Z0:DWORD

		extrn	SYSCALL FRatioX:DWORD
		extrn	SYSCALL FRatioY:DWORD

		extrn	SYSCALL ScreenXMin:DWORD
		extrn	SYSCALL ScreenXMax:DWORD
		extrn	SYSCALL ScreenYMin:DWORD
		extrn	SYSCALL ScreenYMax:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	LongProjectList3DMF
		public	SYSCALL	LongProjectList3DF

;*══════════════════════════════════════════════════════════════════════════*
;void	LongProjectList3DF(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux LongProjectList3DF			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARNING: OrgX, OrgY OrgZ pushed on the stack

;*══════════════════════════════════════════════════════════════════════════*
;void	LongProjectList3DMF(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux LongProjectList3DMF		\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARNING: OrgX, OrgY OrgZ pushed on the stack

LongProjectList3DMF PROC

		emms

LongProjectList3DF::

		test	ecx, ecx
		jz	RetProj

		lea	edx, [ecx+ecx*2]
		push	ebp

		mov	ecx, [esi+4]		; ecx = Y
		mov	eax, [esp+16]		; eax = OrgZ

		sub	eax, [esi+8]		; eax = OrgZ-Z
		lea	ebp, [esi+edx*4]

		cmp	eax, [NearClip]
		jl	Clip0
OkClip0:
		mov	edx, [esi]		; edx = X
		add	esi, 12

		mov	[Z0], eax      		; Z0 =  OrgZ-Z
		mov	ebx, edi

		sub	ecx, [esp+12]		; ecx = Y-OrgY
		sub	edx, [esp+8]		; edx = X-OrgX

		mov	[Y0], ecx		; Y0  = Y-OrgY
		mov	[X0], edx		; X0  = X-OrgX

		fld	[FRatioX]		; fx

		fld	[FRatioY]		; fy  fx

		fild	dword ptr[Z0]		; Z

		fild	dword ptr[X0]		; X    Z

		fild	dword ptr[Y0]		; Y    X    Z

		fxch	st(2)			; Z    X    Y

		fdivr	st(0), st(4)		; fx/Z X    Y  fy  fx

		cmp	esi, ebp
		je	ExitLoop
MainLoop:
		mov	ecx, [esi+4]		; ECX = Y
		mov	eax, [esp+16]		; EAX = OrgZ

		add	edi, 4
		sub	eax, [esi+8]		; eax = OrgZ-Z

		cmp	eax, [NearClip]
		jl	Clip
OkClip:
		mov	edx, [esi]		; EDX = X
		mov	[Z0], eax      		; Z0 =  OrgZ-Z

		sub	ecx, [esp+12]		; ECX = Y-OrgY
		sub	edx, [esp+8]		; EDX = X-OrgX

						; 0    1    2    3
		fmul	st(2),st		; fx/Z X    Y'

		mov	[Y0], ecx		; Y0  = Y-OrgY
		mov	[X0], edx		; X0  = X-OrgX

		fmulp	st(1),st		; Xe   Y'

		fild	dword ptr[Z0]		; Z    Xe   Y'

		fxch	st(2)			; Y'   Xe   Z

		fmul	st(0), st(3)		; Ye   Xe   Z
		fxch	st(1)			; Xe   Ye   Z

		fistp	[Xp]			; Ye   Z

		fistp	[Yp]			; Z

		fild	dword ptr[X0]		; X    Z

		fild	dword ptr[Y0]		; Y    X    Z

		fxch	st(2)			; Z    X    Y

		fdivr	st(0), st(4)		; fx/Z X    Y

		mov	ecx, [Xp]
		mov	eax, [Yp]

		add	ecx, [XCentre]		; Center point on screen
		add	eax, [YCentre]

		cmp	ecx, 32767
		jg	OverFlow

		cmp	ecx, -32767
		jl	OverFlow

		cmp	eax, 32767
		jg	OverFlow

		cmp	eax, -32767
		jl	OverFlow

		cmp	ecx, [ScreenXMin]
		jl	UpdateXMin
OkXMin:
		cmp	ecx, [ScreenXMax]
		jg	UpdateXMax
OkXMax:
		cmp	eax, [ScreenYMin]
		jl	UpdateYMin
OkYMin:
		cmp	eax, [ScreenYMax]
		jg	UpdateYMax
OkYMax:
		shl	eax, 16
		and	ecx, 0FFFFh

		or	ecx, eax
FixedOverFlow:	add	esi, 12

		mov	[ebx], ecx		; Save (Xe, Ye)
		mov	ebx, edi

		cmp	esi, ebp
		jne	MainLoop
ExitLoop:
						; 0    1    2    3    4    5    6    7

		fmul	st(2),st		; fx/Z X    Y'

		fmulp	st(1),st		; Xe   Y'
		fxch	st(1)			; Y'   Xe

		fmul	st(0), st(2)
		fxch	st(1)			; Xe   Ye

		mov	ecx, [XCentre]		; Center point on screen
		mov	eax, [YCentre]

		fistp	[Xp]			; Ye

		fistp	[Yp]			;

		add	ecx, [Xp]
		add	eax, [Yp]

		cmp	ecx, 32767
		jg	OverFlow1

		cmp	ecx, -32767
		jl	OverFlow1

		cmp	eax, 32767
		jg	OverFlow1

		cmp	eax, -32767
		jl	OverFlow1

		cmp	ecx, [ScreenXMin]
		jl	UpdateXMin1
OkXMin1:
		cmp	ecx, [ScreenXMax]
		jg	UpdateXMax1
OkXMax1:
		cmp	eax, [ScreenYMin]
		jl	UpdateYMin1
OkYMin1:
		cmp	eax, [ScreenYMax]
		jg	UpdateYMax1
OkYMax1:
		shl	eax, 16
		and	ecx, 0FFFFh

		or	ecx, eax
FixedOverFlow1:
		mov	[ebx], ecx		; Save (Xe, Ye)

		fsubp	st(0), st(0)
		fsubp	st(0), st(0)
ExitExit:
		pop	ebp
RetProj:
		ret
UpdateXMin:
		mov	[ScreenXMin], ecx
		jmp	OkXMin
UpdateXMax:
		mov	[ScreenXMax], ecx
		jmp	OkXMax
UpdateYMin:
		mov	[ScreenYMin], eax
		jmp	OkYMin
UpdateYMax:
		mov	[ScreenYMax], eax
		jmp	OkYMax
UpdateXMin1:
		mov	[ScreenXMin], ecx
		jmp	OkXMin1
UpdateXMax1:
		mov	[ScreenXMax], ecx
		jmp	OkXMax1
UpdateYMin1:
		mov	[ScreenYMin], eax
		jmp	OkYMin1
UpdateYMax1:
		mov	[ScreenYMax], eax
		jmp	OkYMax1
Clip0:
  		mov	edx, [NearClip]
ClipLoop0:
		add	esi, 12
     		mov	[edi], 80008000h

		cmp	esi, ebp
     		je	ExitExit

  		mov	eax, [esp+16]		; EAX = OrgZ
		mov	ecx, [esi+8]		; ECX = Z

		add	edi, 4
		sub	eax, ecx		; EAX = OrgZ-Z

		cmp	eax, edx
		jl	ClipLoop0

		mov	ecx, [esi+4]		; ecx = Y
		jmp	OkClip0
Clip:
  		mov	edx, [NearClip]
  		mov	ecx, [esp+16]
ClipLoop:
		add	esi, 12
		mov	[edi], 80008000h

		cmp	esi, ebp
     		je	ExitLoop

  		mov	eax, [esp+16]		; EAX = OrgZ
		mov	ecx, [esi+8]		; ECX = Z

		add	edi, 4
		sub	eax, ecx		; EAX = OrgZ-Z

		cmp	eax, edx
		jl	ClipLoop

		mov	ecx, [esi+4]		; ecx = Y
		jmp	OkClip
OverFlow:
		mov	ecx,80008000h
		jmp	FixedOverFlow
OverFlow1:
		mov	ecx,80008000h
		jmp	FixedOverFlow1

LongProjectList3DMF ENDP

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
