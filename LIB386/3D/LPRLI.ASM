;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		.model SMALL, SYSCALL

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL LongProjectList
LongProjectList	dd	offset LongProjectListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
temp		dd	?

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	LongProjectList3D:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	LongProjectListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
