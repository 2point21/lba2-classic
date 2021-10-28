;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL LongRotateProjectPoint

;*══════════════════════════════════════════════════════════════════════════*
LongRotateProjectPoint	dd	offset LongRotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
temp		dd	?

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	TypeProj:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	LongRotateProjectPoint3D:DWORD
		extrn	SYSCALL	LongRotateProjectPointIso:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	LongRotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
LongRotateProjectPointInit proc

		mov	[temp], eax	; save eax
		pop	eax		; get return address

		xchg	[temp], eax	; restore eax and save return address

		cmp	[TypeProj], TYPE_3D
		jne	iso

		call	[LongRotateProjectPoint3D]

		push	eax
		mov	eax, [LongRotateProjectPoint3D]

		jmp	save
iso:
		call	[LongRotateProjectPointIso]

		push	eax
		mov	eax, [LongRotateProjectPointIso]
save:
		mov	[LongRotateProjectPoint], eax
		pop	eax

		jmp	[temp]		; return

LongRotateProjectPointInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
