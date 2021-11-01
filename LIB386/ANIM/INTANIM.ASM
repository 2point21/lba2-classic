
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		.CODE

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	ObjectSetInterDep	:PROC
		EXTRN	SYSCALL	ObjectSetInterFrame	:PROC

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		PUBLIC	SYSCALL	ObjectSetInterAnim

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
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

;*袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴*
; 		The
		END
