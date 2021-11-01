;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn	SYSCALL	SinTab:WORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public	SYSCALL	ArcSin

;*══════════════════════════════════════════════════════════════════════════*
;void	ArcSin(S32 sin)		;

;#pragma aux	ArcSin			\
;	parm		[eax]		\
;	modify		[ebx ecx edx]


ArcSin		proc
					; edx = 17.1.14
		test	eax, eax
		jz	exit0

		mov     ecx, 1024
		mov	ebx, 2048*2

		mov	edx, 3072
		cmp	ax, SinTab[ebx]

		jge	ga1
ga0:
		shr	ebx, 1
		mov	ecx, ebx

		add	ebx, edx
ga2:
		test	ebx, 1
		jnz	angle

		cmp	ax, SinTab[ebx]
		jl	ga0
ga1:
		shr	ebx, 1
		mov	edx, ebx

		add	ebx, ecx
		jmp	ga2
angle:
		movsx	ecx, SinTab[edx*2-2]

		movsx	ebx, SinTab[edx*2]

		sub	ecx, eax
		sub	eax, ebx

		cmp	eax, ecx
		mov	eax, 2048

		jbe	ok

		dec	edx
ok:
		sub	eax, edx
		and	eax, 4095
exit0:
		ret

ArcSin		endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
