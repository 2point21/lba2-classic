
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*──────────────────────────────────────────────────────────────────────────*
		.DATA

;*──────────────────────────────────────────────────────────────────────────*
		EXTRN	SYSCALL	TransFctBody	:DWORD

;*──────────────────────────────────────────────────────────────────────────*
		PUBLIC	SYSCALL	Lib3DBufferAnim
		PUBLIC	SYSCALL	EndLib3DBufferAnim
		PUBLIC	SYSCALL	PtrLib3DBufferAnim

		PUBLIC	SYSCALL	TransFctAnim

;*──────────────────────────────────────────────────────────────────────────*
;	Buffer
MallocBuffer		dd	00
Lib3DBufferAnim		dd	00
EndLib3DBufferAnim	dd	00
PtrLib3DBufferAnim	dd	00

;	Translation Function
TransFctAnim		dd	00

;*──────────────────────────────────────────────────────────────────────────*
		.CODE

;*──────────────────────────────────────────────────────────────────────────*
IFDEF	REG_CALL
		EXTRN	SYSCALL	malloc_:PROC
		EXTRN	SYSCALL	free_:	PROC
		EXTRN	SYSCALL	atexit_:PROC
ELSE
		EXTRN	SYSCALL	malloc:	PROC
		EXTRN	SYSCALL	free:	PROC
		EXTRN	SYSCALL	atexit:	PROC
ENDIF

;*──────────────────────────────────────────────────────────────────────────*
		PUBLIC	SYSCALL	InitObjects
		PUBLIC	SYSCALL	ClearObjects

;*──────────────────────────────────────────────────────────────────────────*
;void	InitObjects(void *buffer, U32 size, TRANS_FUNC *fctbody, TRANS_FUNC *fctanim)

;#pragma aux InitObjects	       		\
;	parm		[eax] [ecx] [edx] [ebx]	\
;	modify exact 	[eax ecx edx]

InitObjects	proc

		mov	TransFctBody, edx
		mov	TransFctAnim, ebx

		test	eax, eax
		jz	alloc
returnalloc:
		mov	Lib3DBufferAnim, eax
		lea	ecx, [eax+ecx-31*(SIZEOF GROUP_INFO)+1]
		mov	PtrLib3DBufferAnim, eax
		mov	EndLib3DBufferAnim, ecx
exit:
		ret
alloc:
		mov	eax, [Lib3DBufferAnim]

		test	eax, eax
		jnz	exit
IFDEF	REG_CALL
		mov	eax, ecx
		call	malloc_
ELSE
		push	ecx
		call	malloc
		pop	ecx
ENDIF
		test	eax, eax
		jz	error
		push	eax
IFDEF	REG_CALL
		mov	eax, offset ClearObjects; register ClearObjects
		call	atexit_		       	; for exit
ELSE
		push	offset ClearObjects	; register ClearObjects
		call	atexit		       	; for exit
		pop	edx
ENDIF
		pop	eax

		mov	MallocBuffer, eax
		jmp	returnalloc
error:
		ret

InitObjects	endp

;*──────────────────────────────────────────────────────────────────────────*
;void	ClearObjects()

;#pragma aux ClearObjects	       	\
;	modify	[eax ecx edx]

ClearObjects	proc

		mov	eax, MallocBuffer
		test	eax, eax
		jz	exit

IFDEF	REG_CALL
		call	free_
ELSE
		push	eax
		call	free
		pop	eax
ENDIF
		xor	eax, eax
		mov	MallocBuffer, eax
		mov	Lib3DBufferAnim, eax
exit:
		ret

ClearObjects	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
