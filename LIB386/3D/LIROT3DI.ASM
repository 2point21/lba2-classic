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

		public SYSCALL	LongInverseRotatePointI

;*══════════════════════════════════════════════════════════════════════════*
;void	LongInverseRotatePointI(S32 *Mat, S32 x, S32 y, S32 z)	;

;#pragma aux LongInverseRotatePointI			\
;	parm caller	[esi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx edi]

LongInverseRotatePointI proc

		push	ebp
		mov	[ Z0 ], eax

		imul	[esi].MAT_M11		; EBP:EDI = x*M11

		mov     ebp, eax
 		mov	eax, [esi].MAT_M21
		mov     edi, edx

		imul	ebx

		add     ebp, eax		; EBP:EDI = x*M11 + y*M21
		mov	eax, [esi].MAT_M31
		adc     edi, edx

		imul	ecx

		add     ebp, eax		; EBP:EDI = x*M11 + y*M21 + z*M31
		mov     eax, [esi].MAT_M12
		adc     edi, edx

		imul    [ Z0 ]
		shrd    ebp, edi, 14

		adc	ebp, 0
		mov     edi, edx

		mov	[ X0 ], ebp
		mov     ebp, eax		; EBP:EDI = x*M12

		mov	eax, [esi].MAT_M22

		imul	ebx

		add     ebp, eax		; EBP:EDI = x*M12 + y*M22
		mov	eax, [esi].MAT_M32
		adc     edi, edx

		imul	ecx

		add     ebp, eax		; EBP:EDI = x*M12 + y*M22 + z*M32
		mov     eax, [esi].MAT_M13
		adc     edi, edx

		imul    [ Z0 ]
		shrd    ebp, edi, 14

		adc	ebp, 0
		mov     edi, edx

		mov	[ Y0 ], ebp
		mov     ebp, eax		; EBP:EDI = x*M13
		mov	eax, [esi].MAT_M23

		imul	ebx

		add     ebp, eax		; EBP:EDI = x*M13 + y*M23
		mov	eax, [esi].MAT_M33
		adc     edi, edx

		imul	ecx

		add     eax, ebp		; EBP:EDI = x*M13 + y*M23 + z*M33
		pop	ebp

		adc     edx, edi

		shrd    eax, edx, 14

		adc	eax, 0

		mov	[ Z0 ], eax
		ret

		; X0=X' Y0=Y' Z0=Z'

LongInverseRotatePointI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
