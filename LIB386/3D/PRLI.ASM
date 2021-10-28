;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		public	SYSCALL ProjectList
ProjectList	dd	offset ProjectListInit

;*══════════════════════════════════════════════════════════════════════════*

temp		dd	?

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	TypeProj:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	ProjectList3D:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	ProjectListIso:PROC

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	ProjectListInit

;*══════════════════════════════════════════════════════════════════════════*
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

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
