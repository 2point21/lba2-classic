;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL LongWorldRotateProjectPoint

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
LongWorldRotateProjectPoint	dd	offset LongWorldRotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
temp		dd	?

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	LongWorldRotateProjectPoint3D:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	LongWorldRotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
LongWorldRotateProjectPointInit proc

		mov	[temp], eax	; save eax
		pop	eax		; get return address

		xchg	[temp], eax	; restore eax and save return address

		call	[LongWorldRotateProjectPoint3D]

		push	eax
		mov	eax, [LongWorldRotateProjectPoint3D]

		mov	[LongWorldRotateProjectPoint], eax
		pop	eax

		jmp	[temp]		; return

LongWorldRotateProjectPointInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
