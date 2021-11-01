;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		public	SYSCALL LongProjectPoint
LongProjectPoint	dd	offset LongProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	LongProjectPoint3D:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	LongProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
LongProjectPointInit proc

		call	[LongProjectPoint3D]

		push	eax
		mov	eax, [LongProjectPoint3D]

		mov	[LongProjectPoint], eax
		pop	eax

		ret

LongProjectPointInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
