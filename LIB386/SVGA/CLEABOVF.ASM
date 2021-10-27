;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	ClearColor:QWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	ClearBoxVESAF

			public	SYSCALL	ClearBoxVESAF1
			public	SYSCALL	ClearBoxVESAF2
			public	SYSCALL	ClearBoxVESAF3
			public	SYSCALL	ClearBoxVESAF4
			public	SYSCALL	ClearBoxVESAF5
			public	SYSCALL	ClearBoxVESAF6

;----------------------------------------------------------------------------
			Extrn	SYSCALL	VirtualLinearFirst:PROC

;----------------------------------------------------------------------------
;void	ClearBoxVESAF(void *dst, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBoxVESAF		\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxVESAF		PROC
			ASSUME	FS:_DATA

			push	ebp
			mov	ebp, [ebx]

			mov	ebx, [ebx+4]
			mov	eax, ebp

			shr	ebp, 16
			and	eax, 0FFFFh

			mov	ecx, ebx

			add	edi, [edx+ebp*4]
			mov	edx, [edx+1*4]

			shr	ebx, 16
			and	ecx, 0FFFFh

			sub 	ebx, ebp		; deltaY
			sub	eax, ecx		; -deltaX

			add	edi, ecx
			mov	ebp, eax

			fld	[ClearColor]

			call	VirtualLinearFirst

			test	eax, 8
			jnz	odd
loopCopy:
ClearBoxVESAF1::	fst	REAL8 ptr fs:[edi+eax]
ClearBoxVESAF2::	fst	REAL8 ptr fs:[edi+eax+8]
			add	eax, 16
			jnz	loopCopy
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopy

			fsubp	st(0), st(0)

			pop	ebp
			ret
odd:
			sub	edi, 8

			add	eax, 8
			je	@@Just_A_Col

			add	ebp, 8
loopCopyY:
ClearBoxVESAF3::	fst	REAL8 ptr fs:[edi+eax]
loopCopyX:
ClearBoxVESAF4::	fst	REAL8 ptr fs:[edi+eax+8]
ClearBoxVESAF5::	fst	REAL8 ptr fs:[edi+eax+16]
			add	eax, 16
			jnz	loopCopyX
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopyY
ClearBoxVESAFEnd::
			fsubp	st(0), st(0)

			pop	ebp
			ret

@@Just_A_Col:
			dec	ebx

ClearBoxVESAF6::	fst	REAL8 ptr fs:[edi]

			lea	edi, [edi+edx]
			jnz	@@Just_A_Col

			fsubp	st(0), st(0)

			pop	ebp
			ret
ClearBoxVESAF		ENDP

;----------------------------------------------------------------------------
;			The
			End
