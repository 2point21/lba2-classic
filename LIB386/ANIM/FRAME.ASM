
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		.DATA

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	TimerRefHR		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*

		EXTRN	SYSCALL	TransFctAnim		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		.CODE

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		PUBLIC	SYSCALL	ObjectSetFrame

;*袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴*
;void	ObjectSetFrame(T_OBJ *obj, U32 frame)

;#pragma aux ObjectSetFrame			\
;	parm		[ebx] [edx]		\
;	modify exact 	[eax ecx edx esi edi]


ObjectSetFrame	proc	uses ebp

		mov	ecx, [ebx].NextBody
		mov	eax, [ebx].NextTexture
		cmp	edx, [ebx].NbFrames
		jae	error
		mov	[ebx].Body, ecx
		mov	[ebx].Texture, eax

		mov	esi, [ebx].Anim
		mov	eax, [TransFctAnim]

		test	eax, eax
		jz	skiptrans

		mov	ebp, edx
		call	eax
		mov	edx, ebp
skiptrans:
		push	esi
		xor	eax, eax

		mov	[ebx].Interpolator, eax
		mov	[ebx].LastAnimStepX, eax; struct -> stepX = 0
		mov	[ebx].LastAnimStepY, eax; struct -> stepY = 0
		mov	[ebx].LastAnimStepZ, eax; struct -> stepZ = 0

		movzx	ecx, word ptr[esi+2]	; ecx = nb groups in anim

		mov	[ebx].LastAnimStepAlpha, eax ; struct -> stepAlpha = 0
		mov	[ebx].LastAnimStepBeta, eax  ; struct -> stepBeta  = 0
		mov	[ebx].LastAnimStepGamma, eax ; struct -> stepGamma = 0
		mov	[ebx].LastOfsIsPtr, eax
		lea	eax, [ecx*8+8]		; infos frame + 4 WORD per group
		mov	[ebx].LastNbGroups, ecx	; struct -> LastNbGroups
		mov	[ebx].NextNbGroups, ecx	; struct -> NextNbGroups
		mov	[ebx].NbGroups, ecx	; struct -> NbGroups
		imul	eax, edx		;  * frame
		mov	ebp, TimerRefHR
		add	eax, 8			; skip header
		mov	[ebx].LastTimer, ebp	; struct -> LastTimer
		mov	[ebx].ObjTime, ebp	; struct -> ObjTime
		mov	[ebx].Status, STATUS_FRAME
		mov	[ebx].LastOfsFrame, eax	; struct -> LastOfsFrame
		mov	[ebx].LastFrame, edx	; struct -> LastFrame
		lea	ecx, [ecx*2-2]		; 2 DWORDs per group, no group 0
		mov	ebp, esi		; save ptr
		lea	esi, [esi+eax+16]	; esi -> group 1
		lea	edi, [ebx].CurrentFrame
		inc	edx

		rep	movsd			; init frame

		cmp	edx, [ebx].NbFrames
		je	lloopframe

		movzx	eax, word ptr[esi]	; get time
		sub	esi, ebp
fixedloop:
		pop	ecx
		add	eax, [ebx].LastTimer
		mov	[ebx].NextFrame, edx	; struct -> NextFrame
		mov	[ebx].NextOfsFrame, esi	; struct -> NextOfsFrame
		movzx	ecx, word ptr[ecx+esi+8]
		mov	[ebx].NextTimer, eax	; struct -> NextTimer
		mov	[ebx].Master, ecx
		mov     eax, STATUS_FRAME	; ok
		ret				; exit
lloopframe:
		movzx	eax, word ptr[ebp+8]	; get time
		xor	edx, edx
		mov	esi, 8
		jmp	fixedloop
error:
		xor	eax, eax		; error
		ret

ObjectSetFrame	endp

;*袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴*
; 		The
		END
