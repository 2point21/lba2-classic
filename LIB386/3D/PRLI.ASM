;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		public	SYSCALL ProjectList
ProjectList	dd	offset ProjectListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

temp		dd	?

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	TypeProj:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	ProjectList3D:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	ProjectListIso:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	ProjectListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
ProjectListInit proc

		mov	[temp], eax	; save eax
		pop	eax		; get return address

		xchg	[temp], eax	; restore eax and save return address

		call	[ProjectList3D]

		push	eax
		mov	eax, [ProjectList3D]

		mov	[ProjectList], eax
		pop	eax

		jmp	[temp]		; return

ProjectListInit endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
