
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.CODE

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		PUBLIC	SYSCALL	ObjectInitBody

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void	ObjectInitBody(T_OBJ *obj, T_BODY *body)

;#pragma aux ObjectInitBody		\
;	parm		[ebx] [eax]	\
;	modify exact 	[ecx]

ObjectInitBody	proc
;		pobj:edx, body:eax

		mov	ecx, [ebx].Status
		mov	[ebx].NextBody, eax
		or	ecx, STATUS_BODY
		cmp	[ebx].Body, -1
		mov	[ebx].Status, ecx
		je	firstbody
		ret
firstBody:
		mov	[ebx].Body, eax
		ret

ObjectInitBody	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; 		The
		END
