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
			public	SYSCALL	BlitBoxI

;----------------------------------------------------------------------------
;void	BlitBoxI(void *dst, void *src) ;

;#pragma aux BlitBoxI		\
;	parm	[edi] [esi]	\
;	modify	[eax ebx ecx edx]

BlitBoxI		PROC

			mov	edx, offset TabOffPhysLine
			push	ebp

			mov	eax, [edx+140*4]
			mov	ebp, 200

			mov	edx, [edx+1*4]
			lea	edi, [edi+eax+160]

			sub	edx, 320
loopLine:
			mov	eax, 40
loopCopy:
			mov	ebx, [esi]
			mov	ecx, [esi+4]
			add	esi, 8
			mov	[edi], ebx
			mov	[edi+4], ecx
			add	edi, 8
			dec	eax
			jnz	loopCopy
			add	edi, edx
			dec	ebp
			jnz	loopLine

			pop	ebp

			ret

BlitBoxI		ENDP

;----------------------------------------------------------------------------
;			The
			End
