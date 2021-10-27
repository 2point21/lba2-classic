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
			public	SYSCALL	BlitBoxVESAI
			public	SYSCALL	BlitBoxVESAI1
			public	SYSCALL	BlitBoxVESAI2

;----------------------------------------------------------------------------
;void	BlitBoxVESAI(void *dst, void *src) ;

;#pragma aux BlitBoxVESAI	\
;	parm	[edi] [esi]	\
;	modify	[eax ebx ecx edx]

BlitBoxVESAI		PROC
			ASSUME	FS:_DATA

			mov	edx, offset TabOffPhysLine
			push	ebp

			mov	eax, [edx+140*4]
			mov	ebp, 200

			mov	edx, [edx+1*4]
			lea	edi, [edi+eax+160]

			sub	edx, 320
			xor	eax, eax

			call	VirtualLinearFirst
loopLine:
			mov	eax, 40
loopCopy:
			mov	ebx, [esi]
			mov	ecx, [esi+4]
			add	esi, 8
BlitBoxVESAI1::		mov	fs:[edi], ebx
BlitBoxVESAI2::		mov	fs:[edi+4], ecx
			add	edi, 8
			dec	eax
			jnz	loopCopy
			add	edi, edx
			dec	ebp
			jnz	loopLine

			pop	ebp

			ret

BlitBoxVESAI		ENDP

;----------------------------------------------------------------------------
;			The
			End
