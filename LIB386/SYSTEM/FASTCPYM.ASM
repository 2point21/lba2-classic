;----------------------------------------------------------------------------
			.386

			.MODEL	SMALL, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	FastCopyM

;----------------------------------------------------------------------------
;void	FastCopyM(void *dst, void *src, U32 len) ;

;#pragma aux FastCopyM	       	       	\
;	parm	[edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

FastCopyM		PROC

			cmp	ecx, 8
			jb	cpybyte

			mov	edx, ecx
			and	ecx, 7

			shr	edx, 4
			jnc	loopcpy16

			movq	mm0, [esi]
			lea	esi, [esi+8]
			movq	[edi], mm0
			lea	edi, [edi+8]
			jz	endcpy16
loopcpy16:
			movq	mm0, [esi]
			movq	mm1, [esi+8]
			movq	[edi], mm0
			movq	[edi+8], mm1
			add	esi, 16
			add	edi, 16
			dec	edx
			jnz	loopcpy16
endcpy16:
			emms
cpybyte:
			rep	movsb

			ret

FastCopyM		ENDP

;----------------------------------------------------------------------------
;			The
			End
