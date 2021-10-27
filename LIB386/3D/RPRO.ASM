;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL RotateProjectPoint

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
RotateProjectPoint	dd	offset RotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
temp		dd	?

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	TypeProj:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	RotateProjectPoint3D:DWORD
		extrn	SYSCALL	RotateProjectPointIso:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	RotateProjectPointInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
RotateProjectPointInit proc

		mov	[temp], eax	; save eax
		pop	eax		; get return address

		xchg	[temp], eax	; restore eax and save return address

		cmp	[TypeProj], TYPE_3D
		jne	iso

		call	[RotateProjectPoint3D]

		push	eax
		mov	eax, [RotateProjectPoint3D]

		jmp	save
iso:
		call	[RotateProjectPointIso]

		push	eax
		mov	eax, [RotateProjectPointIso]
save:
		mov	[RotateProjectPoint], eax
		pop	eax

		jmp	[temp]		; return

RotateProjectPointInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
