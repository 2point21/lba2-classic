;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL WorldRotateProjectPoint

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
WorldRotateProjectPoint	dd	offset WorldRotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
temp		dd	?

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	WorldRotateProjectPoint3D:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	WorldRotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
WorldRotateProjectPointInit proc

		mov	[temp], eax	; save eax
		pop	eax		; get return address

		xchg	[temp], eax	; restore eax and save return address

		call	[WorldRotateProjectPoint3D]

		push	eax
		mov	eax, [WorldRotateProjectPoint3D]

		mov	[WorldRotateProjectPoint], eax
		pop	eax

		jmp	[temp]		; return

WorldRotateProjectPointInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
