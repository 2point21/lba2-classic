;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	ClearColor:QWORD

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	ClearBoxF

;----------------------------------------------------------------------------
;void	ClearBoxF(void *dst,  U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBoxF	       	       	\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxF		PROC

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

			fld	[ClearColor]

			test	eax, 8
			jnz	odd
loopClear:
			fst	REAL8 ptr[edi+eax]
			fst	REAL8 ptr[edi+eax+8]
			add	eax, 16
			jnz	loopClear
			mov	eax, ecx
			add	edi, edx
			dec	ebx
			jnz	loopClear

			fsubp	st(0), st(0)
			ret
odd:
			add	eax, 8
			je	@@Just_A_Col

			sub	edi, 8

			add	ecx, 8
loopClearY:
			fst	REAL8 ptr[edi+eax]
loopClearX:
			fst	REAL8 ptr[edi+eax+8]
			fst	REAL8 ptr[edi+eax+16]
			add	eax, 16
			jnz	loopClearX
			mov	eax, ecx
			add	edi, edx
			dec	ebx
			jnz	loopClearY
ClearBoxFEnd::
			fsubp	st(0), st(0)
			ret
@@Just_A_Col:
			fst	REAL8 ptr[edi-8]
			add	edi, edx

			dec	ebx
			jnz	@@Just_A_Col

			fsubp	st(0), st(0)
			ret

ClearBoxF		ENDP

;----------------------------------------------------------------------------
;			The
			End
