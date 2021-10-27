;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public SYSCALL	InverseRotatePointI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	InverseRotatePointI(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux InverseRotatePointI			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

InverseRotatePointI	proc

		push	ebp
		mov	edi, [esi].MAT_M11

		mov	ebp, [esi].MAT_M21

		imul	edi, eax
		imul	ebp, ebx

		mov	edx, [esi].MAT_M31
		add	edi, ebp

		imul	edx, ecx

		mov	ebp, [esi].MAT_M22
		lea	edx, [edx+edi+(1 SHL 13)]

		sar	edx, 14
		mov	edi, [esi].MAT_M12

		mov	X0, edx
		mov	edx, [esi].MAT_M32

		imul	ebp, ebx
		imul	edi, eax
		imul	edx, ecx
		imul	eax, [esi].MAT_M13
		imul	ebx, [esi].MAT_M23
		imul	ecx, [esi].MAT_M33

		add	edx, ebp
		add	ebx, eax

		lea	edx, [edx+edi+1 SHL 13]
		lea	ebx, [ebx+ecx+1 SHL 13]

		sar	edx, 14
		pop	ebp

		sar	ebx, 14
		mov	Y0, edx

		mov	Z0, ebx

		; X0=X' Y0=Y' Z0=Z'

		ret

InverseRotatePointI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End
