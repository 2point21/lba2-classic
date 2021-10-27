;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	CopyBoxF

;----------------------------------------------------------------------------
;void	CopyBoxF(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBoxF	       	       	\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxF		PROC

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
			fld	REAL8 ptr[esi+eax]
			fstp	REAL8 ptr[edi+eax]
			fld	REAL8 ptr[esi+eax+8]
			fstp	REAL8 ptr[edi+eax+8]
			add	eax, 16
			jnz	loopCopy
			add	esi, ecx
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopy

			pop	ebp
			ret
odd:
			add	eax, 8
			je	@@Just_A_Col

			sub	esi, 8
			sub	edi, 8

			add	ebp, 8
loopCopyY:
			fld	REAL8 ptr[esi+eax]
			fstp	REAL8 ptr[edi+eax]
loopCopyX:
			fld	REAL8 ptr[esi+eax+8]
			fstp	REAL8 ptr[edi+eax+8]
			fld	REAL8 ptr[esi+eax+16]
			fstp	REAL8 ptr[edi+eax+16]
			add	eax, 16
			jnz	loopCopyX
			add	esi, ecx
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopyY
CopyBoxFEnd::
			pop	ebp
			ret

@@Just_A_Col:
			fld	REAL8 ptr[esi-8]

			add	esi, ecx
			dec	ebx

			fstp	REAL8 ptr[edi-8]

			lea	edi, [edi+edx]
			jnz	@@Just_A_Col

			pop	ebp
			ret
CopyBoxF		ENDP

;----------------------------------------------------------------------------
;			The
			End
