;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL LongRotateProjectPoint

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
LongRotateProjectPoint	dd	offset LongRotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
temp		dd	?

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	TypeProj:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	LongRotateProjectPoint3D:DWORD
		extrn	SYSCALL	LongRotateProjectPointIso:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	LongRotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
