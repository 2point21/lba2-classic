;----------------------------------------------------------------------------
			.386

			.MODEL	SMALL, SYSCALL

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	FastSetI

;----------------------------------------------------------------------------
;void	FastSetI(void *dst, S32 val, U32 len) ;

;#pragma aux FastSetI	       	       	\
;	parm	[edi] [eax] [ecx]	\
;	modify	[ebx edx]

FastSetI		PROC

			and	eax, 0FFh	; 000C

			mov	edx, eax	; 000C

			shl	edx, 8		; 00C0

			mov	ebx, edx	; 00C0
			or	edx, eax	; 00CC

			shl	edx, 16		; CC00
			or	eax, ebx	; 00CC

			or	eax, edx	; CCCC

			cmp	ecx, 8
			jb	setbyte

			mov	edx, ecx
			and	ecx, 7

			shr	edx, 4
			jnc	loopset16

			mov	[edi], eax
			mov	[edi+4], eax
			lea	edi, [edi+8]
			jz	setbyte
loopset16:
			mov	[edi], eax
			mov	[edi+4], eax
			mov	[edi+8], eax
			mov	[edi+12], eax
			add	edi, 16
			dec	edx
			jnz	loopset16
setbyte:
			rep	stosb

			ret

FastSetI		ENDP

;----------------------------------------------------------------------------
;			The
			End
