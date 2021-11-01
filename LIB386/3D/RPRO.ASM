;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL RotateProjectPoint

;*══════════════════════════════════════════════════════════════════════════*
RotateProjectPoint	dd	offset RotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
temp		dd	?

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	TypeProj:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	RotateProjectPoint3D:DWORD
		extrn	SYSCALL	RotateProjectPointIso:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	RotateProjectPointInit

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
