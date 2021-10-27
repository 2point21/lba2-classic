;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL TabOffPhysLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			extrn	SYSCALL	VirtualLinearFirst:PROC

;----------------------------------------------------------------------------
			public	SYSCALL	BlitBoxVESAF
			public	SYSCALL	BlitBoxVESAF1
			public	SYSCALL	BlitBoxVESAF2

;----------------------------------------------------------------------------
;void	BlitBoxVESAF(void *dst, void *src) ;

;#pragma aux BlitBoxVESAF	\
;	parm	[edi] [esi]	\
;	modify	[eax ebx ecx edx]

BlitBoxVESAF		PROC
			ASSUME	FS:_DATA

			mov	edx, offset TabOffPhysLine
			mov	ecx, 200

			mov	ebx, [edx+140*4]
			mov	eax, -320

			mov	edx, [edx+1*4]
			lea	edi, [edi+ebx+160+320]

			call	VirtualLinearFirst
loopCopy:
			fld	REAL8 ptr [esi]
BlitBoxVESAF1::		fstp	REAL8 ptr fs:[edi+eax]
			fld	REAL8 ptr [esi+8]
			add	esi, 16
BlitBoxVESAF2::		fstp	REAL8 ptr fs:[edi+eax+8]
			add	eax, 16
			jnz	loopCopy
			mov	eax, -320
			add	edi, edx
			dec	ecx
			jnz	loopCopy

			ret

BlitBoxVESAF		ENDP

;----------------------------------------------------------------------------
;			The
			End
