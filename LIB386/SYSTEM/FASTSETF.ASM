;----------------------------------------------------------------------------
			.386

			.MODEL	SMALL, SYSCALL

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	FastSetF

;----------------------------------------------------------------------------
;void	FastSetF(void *dst, S32 val, U32 len) ;

;#pragma aux FastSetF	       	       	\
;	parm	[edi] [eax] [ecx]	\
;	modify	[ebx edx esi]

FastSetF		PROC

			and	eax, 0FFh	; 000C
			mov	esi, esp

			mov	edx, eax	; 000C
			and	esp, NOT 7

			shl	edx, 8		; 00C0
			sub	esp, 8

			mov	ebx, edx	; 00C0
			or	edx, eax	; 00CC

			shl	edx, 16		; CC00
			or	eax, ebx	; 00CC

			or	eax, edx	; CCCC
			cmp	ecx, 8

			mov	[esp], eax
			mov	[esp+4], eax

			fld	qword ptr[esp]

			mov	esp, esi
			jb	setbyte

			mov	edx, ecx
			and	ecx, 7

			shr	edx, 4
			jnc	loopset16

			fst	qword ptr[edi]

			lea	edi, [edi+8]
			jz	endset16
loopset16:
			fst	qword ptr[edi]
			fst	qword ptr[edi+8]
			add	edi, 16
			dec	edx
			jnz	loopset16
endset16:
			fsubp	st(0), st(0)
setbyte:
			rep	stosb

			ret

FastSetF		ENDP

;----------------------------------------------------------------------------
;			The
			End
