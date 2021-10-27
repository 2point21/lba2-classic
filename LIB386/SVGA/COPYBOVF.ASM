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
			public	SYSCALL	CopyBoxVESAF

			public	SYSCALL	CopyBoxVESAF1
			public	SYSCALL	CopyBoxVESAF2
			public	SYSCALL	CopyBoxVESAF3
			public	SYSCALL	CopyBoxVESAF4
			public	SYSCALL	CopyBoxVESAF5
			public	SYSCALL	CopyBoxVESAF6

;----------------------------------------------------------------------------
			Extrn	SYSCALL	VirtualLinearFirst:PROC

;----------------------------------------------------------------------------
;void	CopyBoxVESAF(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBoxVESAF		\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxVESAF		PROC
			ASSUME	FS:_DATA

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

			call	VirtualLinearFirst

			test	eax, 8
			jnz	odd
loopCopy:
			fld	REAL8 ptr[esi+eax]
CopyBoxVESAF1::		fstp	REAL8 ptr fs:[edi+eax]
			fld	REAL8 ptr[esi+eax+8]
CopyBoxVESAF2::		fstp	REAL8 ptr fs:[edi+eax+8]
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
			sub	esi, 8
			sub	edi, 8

			add	eax, 8
			je	@@Just_A_Col

			add	ebp, 8
loopCopyY:
			fld	REAL8 ptr[esi+eax]
CopyBoxVESAF3::		fstp	REAL8 ptr fs:[edi+eax]
loopCopyX:
			fld	REAL8 ptr[esi+eax+8]
CopyBoxVESAF4::		fstp	REAL8 ptr fs:[edi+eax+8]
			fld	REAL8 ptr[esi+eax+16]
CopyBoxVESAF5::		fstp	REAL8 ptr fs:[edi+eax+16]
			add	eax, 16
			jnz	loopCopyX
			add	esi, ecx
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopyY
CopyBoxVESAFEnd::
			pop	ebp
			ret

@@Just_A_Col:
			fld	REAL8 ptr[esi]

			add	esi, ecx
			dec	ebx

CopyBoxVESAF6::		fstp	REAL8 ptr fs:[edi]

			lea	edi, [edi+edx]
			jnz	@@Just_A_Col

			pop	ebp
			ret
CopyBoxVESAF		ENDP

;----------------------------------------------------------------------------
;			The
			End
