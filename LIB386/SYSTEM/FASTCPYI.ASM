;----------------------------------------------------------------------------
			.386

			.MODEL	SMALL, SYSCALL

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	FastCopyI

;----------------------------------------------------------------------------
;void	FastCopyI(void *dst, void *src, U32 len) ;

;#pragma aux FastCopyI	       	       	\
;	parm	[edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

FastCopyI		PROC

			cmp	ecx, 8
			jb	cpybyte

			mov	edx, ecx
			and	ecx, 7

			shr	edx, 4
			jnc	loopcpy16

			mov	eax, [esi]
			mov	ebx, [esi+4]
			lea	esi, [esi+8]
			mov	[edi], eax
			mov	[edi+4], ebx
			lea	edi, [edi+8]
			jz	cpybyte
loopcpy16:
			mov	eax, [esi]
			mov	ebx, [esi+4]
			mov	[edi], eax
			mov	[edi+4], ebx
			mov	eax, [esi+8]
			mov	ebx, [esi+12]
			add	esi, 16
			mov	[edi+8], eax
			mov	[edi+12], ebx
			add	edi, 16
			dec	edx
			jnz	loopcpy16
cpybyte:
			rep	movsb

			ret

FastCopyI		ENDP

;----------------------------------------------------------------------------
;			The
			End
