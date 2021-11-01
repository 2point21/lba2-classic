
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*──────────────────────────────────────────────────────────────────────────*
		.CODE

;*──────────────────────────────────────────────────────────────────────────*
		EXTRN	SYSCALL	ObjectSetInterDep	:PROC
		EXTRN	SYSCALL	ObjectSetInterFrame	:PROC

;*──────────────────────────────────────────────────────────────────────────*
		PUBLIC	SYSCALL	ObjectSetInterAnim

;*──────────────────────────────────────────────────────────────────────────*
;void	ObjectSetInterAnim(T_OBJ *obj)

;#pragma aux ObjectSetInterAnim		\
;	parm	[ebx]			\
;	modify	[eax ecx edx esi edi]


ObjectSetInterAnim proc

		push	ebx
		call	ObjectSetInterDep
		pop	ebx
		test	eax, STATUS_CHANGE	; any change?
		jz	exit
		test	eax, STATUS_FRAME	; frame ?
		jnz	exit

		push	eax
		call	ObjectSetInterFrame
		pop	eax
exit:
		ret

ObjectSetInterAnim	endp

;*══════════════════════════════════════════════════════════════════════════*
; 		The
		END
