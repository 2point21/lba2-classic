;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			SEG_FS	equ	db 64h
;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	ClearColor:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	ClearBoxVESAM

                        public  SYSCALL ClearBoxVESAM1
                        public  SYSCALL ClearBoxVESAM2
                        public  SYSCALL ClearBoxVESAM3
                        public  SYSCALL ClearBoxVESAM4
                        public  SYSCALL ClearBoxVESAM5
                        public  SYSCALL ClearBoxVESAM6

;----------------------------------------------------------------------------
			Extrn	SYSCALL	VirtualLinearFirst:PROC

;----------------------------------------------------------------------------
;void	ClearBoxVESAM(void *dst, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBoxVESAM	       	\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxVESAM		PROC
			push	ebp
			mov	ebp, [ebx]

			mov	ebx, [ebx+4]
			mov	eax, ebp

			shr	ebp, 16
			and	eax, 0FFFFh

			movq	mm0, [ClearColor]
			mov	ecx, ebx

			add	edi, [edx+ebp*4]
			mov	edx, [edx+1*4]

			shr	ebx, 16
			and	ecx, 0FFFFh

			sub 	ebx, ebp		; deltaY
			sub	eax, ecx		; -deltaX

			add	edi, ecx
			mov	ebp, eax

			call	VirtualLinearFirst

			test	eax, 8
			jnz	odd
loopCopy:
ClearBoxVESAM1::	SEG_FS
			movq	[edi+eax], mm0
ClearBoxVESAM2::	SEG_FS
			movq	[edi+eax+8], mm0
			add	eax, 16
			jnz	loopCopy
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopy

;			emms

			pop	ebp
			ret
odd:
			sub	edi, 8

			add	eax, 8
			je	@@Just_A_Col

			add	ebp, 8

loopCopyY:
ClearBoxVESAM3::	SEG_FS
			movq	[edi+eax], mm0
loopCopyX:
ClearBoxVESAM4::	SEG_FS
			movq	[edi+eax+8], mm0
ClearBoxVESAM5::	SEG_FS
			movq	[edi+eax+16], mm0
			add	eax, 16
			jnz	loopCopyX
			mov	eax, ebp
			add	edi, edx
			dec	ebx
			jnz	loopCopyY
ClearBoxVESAFEnd::
;			emms

			pop	ebp
			ret

@@Just_A_Col:
			dec	ebx

ClearBoxVESAM6::	SEG_FS
			movq	[edi], mm0

			lea	edi, [edi+edx]
			jnz	@@Just_A_Col

;			emms

			pop	ebp
			ret

ClearBoxVESAM		ENDP

;----------------------------------------------------------------------------
;			The
			End
