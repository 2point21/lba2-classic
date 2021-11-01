;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc
		include iammx.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn	SYSCALL CameraX:DWORD
		extrn	SYSCALL CameraY:DWORD
		extrn	SYSCALL CameraZ:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL	Xp:DWORD
		extrn	SYSCALL	Yp:DWORD

		extrn	SYSCALL	MMX_DEMI:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	RotateProjectPointIsoM

;*══════════════════════════════════════════════════════════════════════════*
;void	RotateProjectPointIsoM(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux RotateProjectPointIsoM		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotateProjectPointIsoM proc


		movdt		mm0, eax	; mm0 =  0  0  .  X

		movdt		mm1, ebx	; mm0 =  0  0  .  Y

		movdt		mm2, ecx	; mm0 =  0  0  .  Z
		punpcklwd	mm0, mm1	; mm0 =  .  .  Y  X

		movq		mm5, [esi]	; mm5 =	 0 13 12 11
		punpckldq	mm0, mm2	; mm0 =  .  Z  Y  X

		movq		mm6, [esi+8]	; mm6 =	 0 23 22 21
		pmaddwd		mm5, mm0	; mm5 =  X'Z   X'XY

		pmaddwd		mm6, mm0	; mm6 =  Y'Z   Y'XY

		pmaddwd		mm0, [esi+16]	; mm0 =  Z'Z   Z'XY

		movq		mm1, mm5	; mm1 =  X'Z   X'XY

		punpckldq	mm5, mm6	; mm5 =  Y'XY  X'XY

		movq		mm2, mm0	; mm2 =  Z'Z   Z'XY
		punpckhdq	mm1, mm6	; mm5 =  Y'Z   X'Z

		paddd		mm5, mm1	; mm5 =  Y'    X'	(<<14)
		psrlq		mm0, 32		; mm0 =  0     Z'Z

		paddd		mm5, [MMX_DEMI]
		paddd		mm2, mm0	; mm2 =  .     Z'	(<<14)

		paddd		mm2, [MMX_DEMI]
		psrad		mm5, 14		; mm5 =  Y'    X'

		psrad		mm2, 14		; mm2 =  .     Z'

		movdf		eax, mm5
		psrlq		mm5, 32

		movdf		ecx, mm2

		movdf		ebx, mm5

		mov		edx, CameraY
		sub		eax, ecx

		sub		ebx, edx
		mov		edx, CameraZ

		sub		ecx, edx
		mov		edx, eax	; save x

		sub		eax, ecx	; x + zrot
		add		ecx, edx	; -(x - zrot)

		mov		edx, ebx	; save y

		shl		ebx, 4		; *16
		lea		ecx, [ecx+ecx*2]; *3

		add		ecx, ecx	; *2
		lea		eax, [eax+eax*2]; *3

		sar		eax, 6		; /64 IsoScale
		sub		ebx, edx	; y*15

		mov		edx, [XCentre]
		sub		ecx, ebx	; - (x-zrot) * 6

		add		eax, edx
		mov		edx, [YCentre]

		sar		ecx, 8		; /256 IsoScale
		mov		[Xp], eax

		lea		ecx, [ecx+edx+1];; +1 for compatibility with old
						 ; lib.
						 ; In fact we should never
						 ; truncate but round to nearest!
						 ; |(
		mov		eax, -1

		mov		[Yp], ecx
		ret

		ret

RotateProjectPointIsoM	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
