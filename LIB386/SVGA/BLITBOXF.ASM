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
			public	SYSCALL	BlitBoxF

;----------------------------------------------------------------------------
;void	BlitBoxF(void *dst, void *src) ;

;#pragma aux BlitBoxF		\
;	parm	[edi] [esi]	\
;	modify	[eax ebx ecx edx]

BlitBoxF		PROC

			mov	edx, offset TabOffPhysLine
			mov	ecx, 200

			mov	eax, [edx+140*4]
			mov	ebx, -320

			mov	edx, [edx+1*4]
			lea	edi, [edi+eax+160+320]
loopCopy:
			fld	REAL8 ptr [esi]
			fstp	REAL8 ptr [edi+ebx]
			fld	REAL8 ptr [esi+8]
			add	esi, 16
			fstp	REAL8 ptr [edi+ebx+8]
			add	ebx, 16
			jnz	loopCopy
			mov	ebx, -320
			add	edi, edx
			dec	ecx
			jnz	loopCopy

			ret

BlitBoxF		ENDP

;----------------------------------------------------------------------------
;			The
			End
