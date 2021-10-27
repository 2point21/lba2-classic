;----------------------------------------------------------------------------
			.386

			.MODEL	SMALL, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	FastSetM

;----------------------------------------------------------------------------
;void	FastSetF(void *dst, S32 val, U32 len) ;

;#pragma aux FastSetF	       	       	\
;	parm	[edi] [eax] [ecx]	\
;	modify	[edx]

FastSetM		PROC

			movdt		mm0, eax

			punpcklbw	mm0, mm0
			cmp		ecx, 8

			punpcklwd	mm0, mm0
			jb		setbyte

			punpckldq	mm0, mm0
			mov		edx, ecx

			and		ecx, 7
			movq		[edi], mm0

			shr		edx, 4
			jnc		enterset16

			lea		edi, [edi+8]
			jz		setbyte
loopset16:
			movq		[edi], mm0
enterset16:		movq		[edi+8], mm0
			add		edi, 16
			dec		edx
			jnz		loopset16
setbyte:
			emms

			rep		stosb

			ret

FastSetM		ENDP

;----------------------------------------------------------------------------
;			The
			End
