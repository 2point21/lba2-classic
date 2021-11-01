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

		public SYSCALL	LongRotatePointI

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotatePointI(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongRotatePointI			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongRotatePointI proc

		push	ebp
		mov	[ Z0 ], eax

		imul	[esi].MAT_M11

		mov     ebp, eax
 		mov	eax, [esi].MAT_M12
		mov     edi, edx

		imul	ebx

		add     ebp, eax
		mov	eax, [esi].MAT_M13
		adc     edi, edx

		imul	ecx

		add     ebp, eax
		mov     eax, [esi].MAT_M21
		adc     edi, edx

		imul    [ Z0 ]
		shrd    ebp, edi, 14

		adc	ebp, 0
		mov     edi, edx
		mov	[ X0 ], ebp
		mov     ebp, eax
		mov	eax, [esi].MAT_M22

		imul	ebx

		add     ebp, eax
		mov	eax, [esi].MAT_M23
		adc     edi, edx

		imul	ecx

		add     ebp, eax
		mov     eax, [esi].MAT_M31
		adc     edi, edx

		imul    [ Z0 ]
		shrd    ebp, edi, 14

		adc	ebp, 0
		mov     edi, edx
		mov	[ Y0 ], ebp
		mov     ebp, eax
		mov	eax, [esi].MAT_M32

		imul	ebx

		add     ebp, eax
		mov	eax, [esi].MAT_M33
		adc     edi, edx

		imul	ecx

		add     eax, ebp
		pop	ebp
		adc     edx, edi

		shrd    eax, edx, 14

		adc	eax, 0

		mov	[ Z0 ], eax
		ret

		; X0=X' Y0=Y' Z0=Z'

LongRotatePointI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

