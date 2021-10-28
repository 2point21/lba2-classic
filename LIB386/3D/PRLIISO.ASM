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

		public	SYSCALL	ProjectListIso

;*══════════════════════════════════════════════════════════════════════════*
;void	ProjectListIso(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux ProjectListIso			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARING: OrgX, OrgY OrgZ pushed on the stack

ProjectListIso	proc

		push	ebp
		lea	ebp, [ecx*4]

		lea	esi, [esi+ecx*8]
		xor	ebp, -1

		lea	edi, [edi+ecx*4]
		inc	ebp
LoopProj:
		mov	ebx, [esi+ebp*2]	; Load YX
		mov	ecx, [esi+ebp*2+4]	; ECX = Z

		shl	ecx, 16
		mov	eax, ebx

		sar	ecx, 16

		shl	eax, 16
		mov	edx, ecx

		sar	eax, 16
		sub	ecx, [esp+16]		; ecx = z

		sar	ebx, 16
		add	edx, eax		; new Z for sort

		sar	edx, 1			; /2 to avoid overflows
		sub	eax, [esp+8]		; eax = x

		mov	[esi+ebp*2+4], edx	; write back new Z
		sub	ebx, [esp+12]		; ebx = y

		lea 	edx, [ecx+eax]		; x+z
		sub	eax, ecx		; x-z

		mov	ecx, ebx
		add	edx, edx		; (x+z)*2

		shl	ebx, 4			; y*16
		lea	eax, [eax+eax*2]	; (x-z)*3

		sub	ebx, ecx		; y*15
		lea	edx, [edx+edx*2]	; (x+z)*6

		sar	eax, 6			; (x-z)*3/64 IsoScale
		mov	ecx, [XCentre]

		add	eax, ecx		; Xe
		sub	edx, ebx		; (x+z)*6-y*15

		sar	edx, 8			; ((x+z)*6-y*15)/256 IsoScale
		mov	ecx, [YCentre]

		cmp	eax, 32767
		jg	OverFlow

		cmp	eax, -32767
		jl	OverFlow

		lea	edx, [edx+ecx+1]	; Ye+1 (for old lib comp)

		cmp	edx, 32767
		jg	OverFlow

		cmp	edx, -32767
		jl	OverFlow

		mov	ebx, [ScreenXMin]
		mov	ecx, [ScreenXMax]

		cmp	eax, ebx
		jl	UpdateXMin
OkXMin:
		cmp	eax, ecx
		jg	UpdateXMax
OkXMax:
		mov	ebx, [ScreenYMin]
		mov	ecx, [ScreenYMax]

		cmp	edx, ebx
		jl	UpdateYMin
OkYMin:
		cmp	edx, ecx
		jg	UpdateYMax
OkYMax:
		shl	edx, 16
		and	eax, 0FFFFh

		or	eax, edx
FixedOverFlow:
		mov	[edi+ebp], eax

		add	ebp, 4
		jnz	LoopProj

		pop	ebp
		ret
UpdateXMin:
		mov	[ScreenXMin], eax
		jmp	OkXMin
UpdateXMax:
		mov	[ScreenXMax], eax
		jmp	OkXMax
UpdateYMin:
		mov	[ScreenYMin], edx
		jmp	OkYMin
UpdateYMax:
		mov	[ScreenYMax], edx
		jmp	OkYMax
OverFlow:
		mov	eax, 80008000h
		jmp	FixedOverFlow

		ret

ProjectListIso	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

