;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	SinTab	:WORD
		extrn	SYSCALL	CosTab	:WORD

		extrn	SYSCALL	X0	:DWORD
		extrn	SYSCALL	Z0	:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

		public	SYSCALL	RotateI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateI(S32 x, S32 z, S32 angle)	;

;#pragma aux	RotateI			\
;	parm	[eax] [ecx] [edx]	\
;	modify	[ebx esi edi]

RotateI		proc

		and	edx, 4095

		add	edx, edx
		jz	norot			; opt edx=0

		movsx	edi, CosTab[edx]	; edi = Cos( T )
		movsx	edx, SinTab[edx]	; edx = Sin( T )

		mov	esi, eax
		mov	ebx, ecx

		imul	eax, edi		; X*cos(T)

		imul	ebx, edx		; Z*sin(T)

		imul	ecx, edi		; Z*cos(T)

		imul	esi, edx		; X*sin(T)

		lea	eax, [eax+ebx+1 SHL 13]	; X' = X*cos(T)+Z*sin(T)+0.5

		sar	eax, 14			; eax = X'
		sub	ecx, esi		; Z' = -X*sin(T) + Z*cos(T)

		sar	ecx, 14			; ecx = Z'

		adc	ecx, 0			; +0.5
norot:		mov	[X0], eax

		mov	[Z0], ecx

		ret

RotateI		endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
