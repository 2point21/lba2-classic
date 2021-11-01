;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL WorldRotateProjectPoint

;*══════════════════════════════════════════════════════════════════════════*
WorldRotateProjectPoint	dd	offset WorldRotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
temp		dd	?

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	WorldRotateProjectPoint3D:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	WorldRotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
