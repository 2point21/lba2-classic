
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.CODE

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		PUBLIC	SYSCALL	ObjectClear

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void	ObjectClear(T_OBJ *obj)

;#pragma aux ObjectClear		\
;	parm		[ebx]		\
;	modify exact 	[eax ecx edi]

ObjectClear	proc

		mov	edi, ebx
		mov	ecx, SIZEOF OBJ_3D / 4	; clear structure
		xor	eax, eax
		rep	stosd

		mov	[ebx].Body, -1
		mov	[ebx].NextBody, -1
		mov	[ebx].Texture, -1
		mov	[ebx].NextTexture, -1
		mov	[ebx].Anim, -1

		ret

ObjectClear	endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; 		The
		END
