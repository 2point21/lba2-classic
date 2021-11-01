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

		public	SYSCALL	ProjectList3DMF
		public	SYSCALL	ProjectList3DF

;*══════════════════════════════════════════════════════════════════════════*
;void	ProjectList3DF(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux ProjectList3DF			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARNING: OrgX, OrgY OrgZ pushed on the stack

;*══════════════════════════════════════════════════════════════════════════*
;void	ProjectList3DMF(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux ProjectList3DMF			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARNING: OrgX, OrgY OrgZ pushed on the stack

ProjectList3DMF	PROC

		test	ecx, ecx
		jz	RetProj

		emms

		jmp	enterprojli
ProjectList3DF::
		test	ecx, ecx
		jz	RetProj
enterprojli:
		push	ebp
		lea	ebp,[esi+ecx*8]

		mov	ecx, [esi]		; ecx = Y X
		mov	edx, [NearClip]

		movsx	eax, word ptr[esi+4]	; eax = Z

		add	esi, 8
		sub	eax, [esp+16]		; eax = Z-OrgZ

		neg	eax			; eax = OrgZ-Z

		cmp	eax, edx
		jl	Clip0
OkClip0:
		movsx	edx, cx			; edx = X

		sar	ecx, 16	       		; ecx = Y
		mov	[Z0], eax      		; Z0 =  OrgZ-Z

		sub	ecx, [esp+12]		; ecx = Y-OrgY
		sub	edx, [esp+8]		; edx = X-OrgX

		mov	[Y0], ecx		; Y0  = Y-OrgY
		mov	[X0], edx		; X0  = X-OrgX

		mov	ebx, edi

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
		mov	ecx, [esi]		; ECX = Y X
		add	edi, 4

		movsx	eax, word ptr[esi+4]	; eax =   Z

		sub	eax, [esp+16]		; EAX = Z-OrgZ
		add	esi, 8

		neg	eax			; eax = OrgZ-Z

		cmp	eax, [NearClip]
		jl	Clip
OkClip:
		movsx	edx, cx			; EDX = X

		sar	ecx, 16	       		; ECX = Y
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
FixedOverFlow:
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

		; PENALTY one cycle on Pentium

		fistp	[Xp]			; Ye

		fistp	[Yp]			;

		mov	ecx, [Xp]
		mov	eax, [Yp]

		add	ecx, [XCentre]		; Center point on screen
		add	eax, [YCentre]

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
     		mov	[edi], 80008000h
		add	edi, 4

		cmp	esi, ebp
     		je	ExitExit

		movsx	eax, word ptr[esi+4]	; EAX = Z

		add	esi, 8
		sub	eax, [esp+16]		; EAX = Z-OrgZ

		neg	eax			; EAX = OrgZ-Z

		cmp	eax, edx
		jl	Clip0

		mov	ecx, [esi-8]		; ecx = Y X
		jmp	OkClip0
Clip:
  		mov	edx, [NearClip]
ClipLoop:
		mov	[edi], 80008000h
		add	edi, 4

		cmp	esi, ebp
     		je	ExitLoop

		movsx	eax, word ptr[esi+4]	; EAX = Z

		add	esi, 8
		sub	eax, [esp+16]		; EAX = Z-OrgZ

		neg	eax			; EAX = OrgZ-Z

		cmp	eax, edx
		jl	ClipLoop

		mov	ecx, [esi-8]		; ecx = Y X
		jmp	OkClip
OverFlow:
		mov	ecx,80008000h
		jmp	FixedOverFlow
OverFlow1:
		mov	ecx,80008000h
		jmp	FixedOverFlow1

ProjectList3DMF	ENDP

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
