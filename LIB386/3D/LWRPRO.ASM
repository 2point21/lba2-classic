;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL LongWorldRotateProjectPoint

;*══════════════════════════════════════════════════════════════════════════*
LongWorldRotateProjectPoint	dd	offset LongWorldRotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
temp		dd	?

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	LongWorldRotateProjectPoint3D:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	LongWorldRotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
