;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL CameraX:DWORD
		extrn	SYSCALL CameraY:DWORD
		extrn	SYSCALL CameraZ:DWORD

		extrn	SYSCALL XCentre:DWORD
		extrn	SYSCALL YCentre:DWORD

		extrn	SYSCALL	Xp:DWORD
		extrn	SYSCALL	Yp:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	RotateProjectPointIsoI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateProjectPointIsoI(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux RotateProjectPointIsoI		\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotateProjectPointIsoI proc

		push	ebp
		mov	edi, [esi].MAT_M11
		mov	ebp, [esi].MAT_M12
		imul	edi, eax
		imul	ebp, ebx
		mov	edx, [esi].MAT_M13
		add	edi, ebp
		imul	edx, ecx
		mov	ebp, [esi].MAT_M22
		lea	edx, [edx+edi+(1 SHL 13)]
		sar	edx, 14
		mov	edi, [esi].MAT_M21
		mov	[Xp], edx
		mov	edx, [esi].MAT_M23
		imul	ebp, ebx
		imul	edi, eax
		imul	edx, ecx
		imul	eax, [esi].MAT_M31
		imul	ebx, [esi].MAT_M32
		imul	ecx, [esi].MAT_M33
		lea	edx, [edx+ebp+(1 SHL 13)]
		lea	ebx, [ebx+eax+(1 SHL 13)]
		add	edx, edi
		add	ebx, ecx
		sar	edx, 14
		pop	ebp
		sar	ebx, 14
		mov	ecx, [Xp]

		sub	ecx, CameraX
		sub	edx, CameraY

		sub	ebx, CameraZ
		mov	eax, ecx		; save x

		sub	ecx, ebx		; x + zrot
		add	ebx, eax		; -(x - zrot)

		mov	eax, edx		; save y
		add	ebx, ebx		; *2

		shl	edx, 4			; *16
		lea	ecx, [ecx+ecx*2]	; *3

		sub	edx, eax		; y*15
		lea	ebx, [ebx+ebx*2]	; *3

		sar	ecx, 6			; /64 IsoScale
		mov	eax, -1

		add	ecx, [XCentre]
		sub	ebx, edx		; - (x-zrot) * 6

		sar	ebx, 8			; /256 IsoScale
		mov	[Xp], ecx

		add	ebx, [YCentre]

		inc	ebx

		mov	[Yp], ebx

		ret

RotateProjectPointIsoI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
