;*──────────────────────────────────────────────────────────────────────────*
			.386P
			.MODEL	SMALL, SYSCALL

;*──────────────────────────────────────────────────────────────────────────*
			.CODE

;*──────────────────────────────────────────────────────────────────────────*
IFDEF	_WIN32
			public	SYSCALL	AllowPatchAndPatch
			public	SYSCALL	AllowPatch
ENDIF;	_WIN32

;*──────────────────────────────────────────────────────────────────────────*
IFDEF	_WIN32
			extrn	SYSCALL	_VirtualProtect:PROC
ENDIF;	_WIN32

;*──────────────────────────────────────────────────────────────────────────*
IFDEF	_WIN32

;S32	AllowPatch(void *start, void *end)

;#pragma aux AllowPatch	"*"	\
;	parm	[eax] [edx]	\
;	modify 	[ecx]


AllowPatch		PROC

			push	0			; dummy (local variable)
			sub	edx, eax

			push	esp			; ->dummy
			push	40h			; PAGE_EXECUTE_READWRITE

			push	edx
			push	eax

			call	_VirtualProtect		; allow EXECUTE_READWRITE on range

			pop	edx

			ret

AllowPatch		ENDP

ENDIF;	_WIN32

;*──────────────────────────────────────────────────────────────────────────*
IFDEF	_WIN32

; NEVER CALL AllowPatchAndPatch FROM C CODE !

AllowPatchAndPatch	PROC

			push	eax
			push	ebx
			push	ecx
			push	edx

			mov	edx, [esp+4*4+1*4]	; ebx = end
			mov	eax, [esp+4*4+2*4]	; eax = start

			call	AllowPatch		; allow code patches on range

			mov	ebx, [esp+4*4+0*4]	; ecx = patch data
			mov	ecx, [esp+4*4+2*4]	; eax = start

			mov	eax, [ebx]		; restore code sequence
			mov	dl, [ebx+4]		; by patching code

			mov	[ecx], eax
			mov	[ecx+4], dl

			pop	edx
			pop	ecx
			pop	ebx
			pop	eax

			add	esp, 2*4		; remove 2 params

			ret

AllowPatchAndPatch	ENDP

ENDIF;	_WIN32

;*──────────────────────────────────────────────────────────────────────────*
;			The
			end
