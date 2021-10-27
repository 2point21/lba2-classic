;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	ClearColor:DWORD

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	ClearBoxM

;----------------------------------------------------------------------------
;void	ClearBoxM(void *dst, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBoxM	       		\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxM		PROC

			mov	esi, [ebx]
			mov	ebx, [ebx+4]

			mov	eax, esi
			mov	ecx, ebx

			shr	esi, 16
			and	eax, 0FFFFh

			shr	ebx, 16
			add	edi, [edx+esi*4]

			mov	edx, [edx+1*4]
			and	ecx, 0FFFFh

			sub	eax, ecx		; -deltaX
			add	edi, ecx

			sub 	ebx, esi		; deltaY
			mov	ecx, eax

			movq	mm0, [ClearColor]

			test	eax, 8
			jnz	odd
loopClear:
			movq	[edi+eax], mm0
			movq	[edi+eax+8], mm0
			add	eax, 16
			jnz	loopClear
			mov	eax, ecx
			add	edi, edx
			dec	ebx
			jnz	loopClear

;			emms
			ret
odd:
			add	eax, 8
			je	@@Just_A_Col

			sub	edi, 8
			add	ecx, 8
loopClearY:
			movq	[edi+eax], mm0
loopClearX:
			movq	[edi+eax+8], mm0
			movq	[edi+eax+16], mm0
			add	eax, 16
			jnz	loopClearX
			mov	eax, ecx
			add	edi, edx
			dec	ebx
			jnz	loopClearY
ClearBoxFEnd::
;			emms
			ret

@@Just_A_Col:
			movq	[edi-8], mm0
			add	edi, edx

			dec	ebx
			jnz	@@Just_A_Col

;			emms
			ret

ClearBoxM		ENDP

;----------------------------------------------------------------------------
;			The
			End
