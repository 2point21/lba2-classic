;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public SYSCALL	RotatePointI

;*══════════════════════════════════════════════════════════════════════════*
;void	RotatePointI(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux RotatePointI			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

RotatePointI	proc

		push	ebp
		mov	edi, [esi].MAT_M11
		mov	ebp, [esi].MAT_M12

		imul	edi, eax
		imul	ebp, ebx

		mov	edx, [esi].MAT_M13
		add	edi, ebp

		imul	edx, ecx

		lea	edx, [edx+edi+(1 SHL 13)]
		mov	ebp, [esi].MAT_M22

		sar	edx, 14
		mov	edi, [esi].MAT_M21

		mov	[X0], edx
		mov	edx, [esi].MAT_M23

		imul	ebp, ebx
		imul	edi, eax
		imul	edx, ecx
		imul	eax, [esi].MAT_M31
		imul	ebx, [esi].MAT_M32
		imul	ecx, [esi].MAT_M33

		lea	edx, [edx+ebp+(1 SHL 13)]
		add	ebx, eax

		add	edx, edi
		lea	ebx, [ebx+ecx+(1 SHL 13)]

		sar	edx, 14
		pop	ebp

		sar	ebx, 14
		mov	[Y0], edx

		mov	[Z0], ebx

		; X0=X' Y0=Y' Z0=Z'

		ret
RotatePointI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		End

