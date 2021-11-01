;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL CameraX:DWORD
		extrn	SYSCALL CameraY:DWORD
		extrn	SYSCALL CameraZ:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL Xp:DWORD
		extrn	SYSCALL Yp:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	LongRotateProjectPointIsoF

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotateProjectPointIsoF(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotateProjectPointIsoF		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify		[edx]

LongRotateProjectPointIsoF proc

		mov	[Xp], eax
		mov	[Yp], ebx

		push	ecx
		mov	ecx, CameraX
					; 0  1  2  3  4  5  6  7

		fild	[Xp]		; X

		fild	[Yp]		; Y  X

		fild	dword ptr[esp]	; Z  Y  X

		fld	st(2)		; X  Z  Y  X

		fmul	[esi].MAT_M11	; X1 Z  Y  X

		fld	st(2)		; Y  X1 Z  Y  X

		fmul	[esi].MAT_M12	; Y1 X1 Z  Y  X

		fld	st(2)		; Z  Y1 X1 Z  Y  X

		fmul	[esi].MAT_M13	; Z1 Y1 X1 Z  Y  X

		fld	st(5)		; X  Z1 Y1 X1 Z  Y  X

		fmul	[esi].MAT_M21   ; X2 Z1 Y1 X1 Z  Y  X

		fld	st(5)		; Y  X2 Z1 Y1 X1 Z  Y  X

		fmul	[esi].MAT_M22   ; Y2 X2 Z1 Y1 X1 Z  Y  X
		fxch	st(3)		; Y1 X2 Z1 Y2 X1 Z  Y  X

		faddp	st(4), st(0)    ; X2 Z1 Y2 P1 Z  Y  X
		fxch	st(6)		; X  Z1 Y2 P1 Z  Y  X2

		fmul	[esi].MAT_M31	; X3 Z1 Y2 P1 Z  Y  X2
		fxch	st(6)		; X2 Z1 Y2 P1 Z  Y  X3

		fld	st(4)		; Z  X2 Z1 Y2 P1 Z  Y  X3

		fmul	[esi].MAT_M23	; Z2 X2 Z1 Y2 P1 Z  Y  X3
		fxch	st(1)		; X2 Z2 Z1 Y2 P1 Z  Y  X3

		faddp	st(3), st(0)	; Z2 Z1 P2 P1 Z  Y  X3
		fxch	st(5)		; Y  Z1 P2 P1 Z  Z2 X3

		fmul	[esi].MAT_M32	; Y3 Z1 P2 P1 Z  Z2 X3
		fxch	st(1)		; Z1 Y3 P2 P1 Z  Z2 X3

		faddp	st(3), st(0)	; Y3 P2 1  Z  Z2 X3
		fxch	st(3)		; Z  P2 1  Y3 Z2 X3

		fmul	[esi].MAT_M33	; Z3 P2 1  Y3 Z2 X3
		fxch	st(3)		; Y3 P2 1  Z3 Z2 X3

		faddp	st(5), st(0)	; P2 1  Z3 Z2 P3

		faddp	st(3), st(0)	; 1  Z3 2  P3

		fistp	[Xp]		; Z3 2  P3

		faddp	st(2), st(0)	; 2  3

		fistp	[Yp]

		fistp	dword ptr[esp]

		mov	eax, [Xp]
		mov	ebx, [Yp]

		mov	edx, CameraY
		sub	eax, ecx

		sub	ebx, edx
		mov	edx, CameraZ

		pop	ecx

		sub	ecx, edx
		mov	edx, eax	; save x

		sub	eax, ecx	; x + zrot
		add	ecx, edx	; -(x - zrot)

		mov	edx, ebx	; save y

		shl	ebx, 4		; *16
		lea	ecx, [ecx+ecx*2]; *3

		add	ecx, ecx	; *2
		lea	eax, [eax+eax*2]; *3

		sar	eax, 6		; /64 IsoScale
		sub	ebx, edx	; y*15

		mov	edx, [XCentre]
		sub	ecx, ebx	; - (x-zrot) * 6

		add	eax, edx
		mov	edx, [YCentre]

		sar	ecx, 8		; /256 IsoScale
		mov	[Xp], eax

		lea	ecx, [ecx+edx+1];; +1 for compatibility with old
					 ; lib.
		mov	eax, -1

		mov	[Yp], ecx
		ret

LongRotateProjectPointIsoF endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
