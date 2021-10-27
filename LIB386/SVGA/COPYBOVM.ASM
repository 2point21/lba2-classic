;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			SEG_FS	equ	db 64h
;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	CopyBoxVESAM

                        public  SYSCALL CopyBoxVESAM1
                        public  SYSCALL CopyBoxVESAM2
                        public  SYSCALL CopyBoxVESAM3
                        public  SYSCALL CopyBoxVESAM4
                        public  SYSCALL CopyBoxVESAM5
                        public  SYSCALL CopyBoxVESAM6

;----------------------------------------------------------------------------
			Extrn	SYSCALL	VirtualLinearFirst:PROC

;----------------------------------------------------------------------------
;void	CopyBoxVESAM(void *dst, void *src, U32 *TabOffDst, U32 *TabOffSrc, T_BOX *box) ;

;#pragma aux CopyBoxVESAM	       	\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxVESAM		PROC
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
			movq	mm1, [esi+eax]
CopyBoxVESAM1::		SEG_FS
			movq	[edi+eax], mm1
			movq	mm0, [esi+eax+8]
CopyBoxVESAM2::		SEG_FS
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
			sub	esi, 8
			sub	edi, 8

			add	eax, 8
			je	@@Just_A_Col

			add	ebp, 8

loopCopyY:
			movq	mm0, [esi+eax]
CopyBoxVESAM3::		SEG_FS
			movq	[edi+eax], mm0
loopCopyX:
			movq	mm1, [esi+eax+8]
CopyBoxVESAM4::		SEG_FS
			movq	[edi+eax+8], mm1
			movq	mm0, [esi+eax+16]
CopyBoxVESAM5::		SEG_FS
			movq	[edi+eax+16], mm0
			add	eax, 16
			jnz	loopCopyX
			add	esi, ecx
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopyY
CopyBoxVESAFEnd::
;			emms

			pop	ebp
			ret

@@Just_A_Col:
			movq	mm0, [esi]

			add	esi, ecx
			dec	ebx

CopyBoxVESAM6::		SEG_FS
			movq	[edi], mm0

			lea	edi, [edi+edx]
			jnz	@@Just_A_Col

;			emms

			pop	ebp
			ret

CopyBoxVESAM		ENDP

;----------------------------------------------------------------------------
;			The
			End
