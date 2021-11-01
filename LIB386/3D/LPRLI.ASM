;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL LongProjectList
LongProjectList	dd	offset LongProjectListInit

;*══════════════════════════════════════════════════════════════════════════*
temp		dd	?

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	LongProjectList3D:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	LongProjectListInit

;*══════════════════════════════════════════════════════════════════════════*
LongProjectListInit proc

		mov	[temp], eax	; save eax
		pop	eax		; get return address

		xchg	[temp], eax	; restore eax and save return address

		call	[LongProjectList3D]

		push	eax
		mov	eax, [LongProjectList3D]

		mov	[LongProjectList], eax
		pop	eax

		jmp	[temp]		; return

LongProjectListInit endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
