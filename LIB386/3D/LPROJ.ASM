;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		public	SYSCALL LongProjectPoint
LongProjectPoint	dd	offset LongProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongProjectPoint3D:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	LongProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
LongProjectPointInit proc

		call	[LongProjectPoint3D]

		push	eax
		mov	eax, [LongProjectPoint3D]

		mov	[LongProjectPoint], eax
		pop	eax

		ret

LongProjectPointInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
