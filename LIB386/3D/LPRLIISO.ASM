;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL ScreenXMin:DWORD
		extrn	SYSCALL ScreenXMax:DWORD
		extrn	SYSCALL ScreenYMin:DWORD
		extrn	SYSCALL ScreenYMax:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	LongProjectListIso

;*══════════════════════════════════════════════════════════════════════════*
;void	LongProjectListIso(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux LongProjectListIso			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARING: OrgX, OrgY OrgZ pushed on the stack

LongProjectListIso	proc

		push	ebp
		lea	ebp, [ecx*4]

		lea	edi, [edi+ecx*4-4]
		xor	ebp, -1

		inc	ebp
		jmp	EnterLoop
LoopProj:
		mov	[edi+ebp], eax
EnterLoop:	mov	ecx, [esi+8]		; Z

		mov	eax, [esi]		; X
		sub	ecx, [esp+16]		; OrgZ

		mov	ebx, [esi+4]		; Y
		sub	eax, [esp+8]		; OrgX

		sub	ebx, [esp+12]		; OrgY
		mov	edx, eax		; save x

		sub	eax, ecx		; x-z
		add	ecx, edx		; x+z

		mov	edx, ebx		; save y
		add	ecx, ecx		; (x+z)*2

		shl	ebx, 4			; y*16
		lea	eax, [eax+eax*2]	; (x-z)*3

		sub	ebx, edx		; y*15
		lea	ecx, [ecx+ecx*2]	; (x+z)*6

		sar	eax, 6			; (x-z)*3/64 IsoScale
		mov	edx, [XCentre]

		add	eax, edx		; Xe
		sub	ecx, ebx		; (x+z)*6-y*15

		sar	ecx, 8			; ((x+z)*6-y*15)/256 IsoScale
		mov	edx, [YCentre]

		add	esi, 12
		lea	ecx, [ecx+edx+1]	; Ye +1 for old lib compatib

		cmp	eax, 32767
		jg	OverFlow

		cmp	eax, -32767
		jl	OverFlow

		cmp	ecx, 32767
		jg	OverFlow

		cmp	ecx, -32767
		jl	OverFlow

		mov	ebx, [ScreenXMin]
		mov	edx, [ScreenXMax]

		cmp	eax, ebx
		jl	UpdateXMin
OkXMin:
		cmp	eax, edx
		jg	UpdateXMax
OkXMax:
		mov	ebx, [ScreenYMin]
		mov	edx, [ScreenYMax]

		cmp	ecx, ebx
		jl	UpdateYMin
OkYMin:
		cmp	ecx, edx
		jg	UpdateYMax
OkYMax:
		shl	ecx, 16
		and	eax, 0FFFFh

		or	eax, ecx
		add	ebp, 4

		jnz	LoopProj
Exit:
		mov	[edi+ebp], ecx
		pop	ebp

		ret
UpdateXMin:
		mov	[ScreenXMin], eax
		jmp	OkXMin
UpdateXMax:
		mov	[ScreenXMax], eax
		jmp	OkXMax
UpdateYMin:
		mov	[ScreenYMin], ecx
		jmp	OkYMin
UpdateYMax:
		mov	[ScreenYMax], ecx
		jmp	OkYMax
OverFlow:
		add	ebp, 4
		mov	ecx, 80008000h

		jnz	LoopProj

		jmp	Exit

LongProjectListIso	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
