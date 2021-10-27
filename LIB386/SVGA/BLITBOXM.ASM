;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

			include	iammx.inc

;----------------------------------------------------------------------------
			SEG_FS	equ	db 64h
;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL TabOffPhysLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	BlitBoxM

;----------------------------------------------------------------------------
;void	BlitBoxM(void *dst, void *src) ;

;#pragma aux BlitBoxM		\
;	parm	[edi] [esi]	\
;	modify	[eax ebx ecx edx]

BlitBoxM		PROC

			mov	edx, offset TabOffPhysLine
			mov	ecx, 200

			mov	eax, [edx+140*4]
			mov	ebx, -320

			mov	edx, [edx+1*4]
			lea	edi, [edi+eax+160+320]
loopCopy:
			movq	mm0, [esi]
			movq	mm1, [esi+8]
			add	esi, 16
			movq	[edi+ebx], mm0
			movq	[edi+ebx+8], mm1
			add	ebx, 16
			jnz	loopCopy
			mov	ebx, -320
			add	edi, edx
			dec	ecx
			jnz	loopCopy

			ret

BlitBoxM		ENDP

;----------------------------------------------------------------------------
;			The
			End
