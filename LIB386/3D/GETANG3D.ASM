;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	X0			:DWORD
		extrn	SYSCALL	Y0			:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	GetAngleVector2D	:PROC
		extrn	SYSCALL	QSqr			:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	GetAngleVector3D

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	GetAngleVector3D(S32 x, S32 y, S32 z)	;

;#pragma aux GetAngleVector3D	"*"		\
;	parm		[eax] [ebx] [ecx]	\
;	modify		[edx esi edi]

GetAngleVector3D proc

		push	ebx		; Y
		push	eax		; X

		mov	ebx, ecx	; Z
		mov	edi, ecx

		call	GetAngleVector2D

		xor	eax, -1
		pop	ecx		; X

		lea	ebx, [eax+2048+1]
		mov	eax, edi	; Z

		imul	eax		; Z^2

		mov	esi, eax
		mov	edi, edx

		and	ebx, 4095
		mov	eax, ecx	; X

		imul	eax		; X^2

		add	eax, esi
		mov	[Y0], ebx

		adc	edx, edi
		call	QSqr

		mov	ebx, eax
		pop	eax

		call	GetAngleVector2D

		neg	eax

		and	eax, 4095

		mov	[X0], eax

		ret

GetAngleVector3D endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
