;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	CopyBoxM

;----------------------------------------------------------------------------
;void	CopyBoxM(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBoxM	       		\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxM		PROC

			push	ebp
			mov	ebp, [ebx]

			mov	ebx, [ebx+4]
			mov	eax, ebp

			shr	ebp, 16
			and	eax, 0FFFFh

			add	esi, TabOffLine[ebp*4]
			mov	ecx, ebx

			add	edi, [edx+ebp*4]
			mov	edx, [edx+1*4]

			shr	ebx, 16
			and	ecx, 0FFFFh

			sub 	ebx, ebp		; deltaY
			sub	eax, ecx		; -deltaX

			add	esi, ecx
			add	edi, ecx

			mov	ebp, eax
			mov	ecx, TabOffLine[1*4]

			test	eax, 8
			jnz	odd
loopCopy:
			movq	mm1, [esi+eax]
			movq	[edi+eax], mm1
			movq	mm0, [esi+eax+8]
			movq	[edi+eax+8], mm0
			add	eax, 16
			jnz	loopCopy
			add	esi, ecx
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopy

;			emms

			pop	ebp
			ret
odd:
			add	eax, 8
			je	@@Just_A_Col

			sub	esi, 8
			sub	edi, 8

			add	ebp, 8

loopCopyY:
			movq	mm0, [esi+eax]
			movq	[edi+eax], mm0
loopCopyX:
			movq	mm1, [esi+eax+8]
			movq	[edi+eax+8], mm1
			movq	mm0, [esi+eax+16]
			movq	[edi+eax+16], mm0
			add	eax, 16
			jnz	loopCopyX
			add	esi, ecx
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopyY
CopyBoxFEnd::
;			emms

			pop	ebp
			ret

@@Just_A_Col:
			movq	mm0, [esi-8]

			add	esi, ecx
			dec	ebx

			movq	[edi-8], mm0

			lea	edi, [edi+edx]
			jnz	@@Just_A_Col

;			emms

			pop	ebp
			ret

CopyBoxM		ENDP

;----------------------------------------------------------------------------
;			The
			End
