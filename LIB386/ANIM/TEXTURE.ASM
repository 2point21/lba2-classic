
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		.CODE

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*
		PUBLIC	SYSCALL	ObjectInitTexture

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void	ObjectInitTexture(T_OBJ *obj, void *texture)

;#pragma aux ObjectInitTexture		\
;	parm		[ebx] [eax]	\
;	modify exact 	[]

ObjectInitTexture proc

		mov	[ebx].NextTexture, eax
		cmp	[ebx].Texture, -1
		jne	oktexture

		mov	[ebx].Texture, eax
oktexture:
		ret

ObjectInitTexture endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; 		The
		END
