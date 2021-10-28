
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*──────────────────────────────────────────────────────────────────────────*
		.DATA

;*──────────────────────────────────────────────────────────────────────────*
		EXTRN	SYSCALL	TimerRefHR		:DWORD

;*──────────────────────────────────────────────────────────────────────────*
		EXTRN	SYSCALL	Lib3DBufferAnim		:DWORD
		EXTRN	SYSCALL	EndLib3DBufferAnim	:DWORD
		EXTRN	SYSCALL	PtrLib3DBufferAnim	:DWORD

		EXTRN	SYSCALL	TransFctAnim		:DWORD

;*──────────────────────────────────────────────────────────────────────────*
		.CODE

;*──────────────────────────────────────────────────────────────────────────*
		PUBLIC	SYSCALL	ObjectInitAnim

;*══════════════════════════════════════════════════════════════════════════*
;void	ObjectInitAnim(T_OBJ *obj, void *anim)

;#pragma aux ObjectInitAnim			\
;	parm		[ebx] [esi]		\
;	modify exact 	[eax ecx edx esi edi]

ObjectInitAnim	proc uses ebp

		mov	ecx, [ebx].NextBody
		mov	eax, [ebx].NextTexture
		cmp	esi, [ebx].Anim		; same anim ?
		mov	[ebx].Body, ecx
		mov	[ebx].Texture, eax
		je	exit

		mov	[ebx].Anim, esi		; init struct -> anim
		mov	eax, [TransFctAnim]

		test	eax, eax
		jz	skiptrans

		call	eax
skiptrans:
		xor	ebp, ebp
		movzx	eax, word ptr[esi+16]

		mov	[ebx].Interpolator, ebp
		mov	[ebx].Master, eax

		movzx	eax, word ptr[esi]	; eax = nb frames
		movzx	ecx, word ptr[esi+2]	; ecx = nb groups in anim
		movzx	edx, word ptr[esi+4]	; edx = loop frame

		mov	[ebx].NbFrames, eax	; struct -> NbFrames
		lea	eax, [ecx*8+8]		; infos frame + 4 WORD per group
		mov	[ebx].LoopFrame, edx	; struct -> LoopFrame
		imul    edx, eax
		mov	[ebx].NextNbGroups, ecx	; struct -> NextNbGroups
		add	edx, 8			; skip header anim

		mov	[ebx].LastAnimStepX, ebp; struct -> stepX = 0
		mov	[ebx].LastAnimStepY, ebp; struct -> stepY = 0
		mov	[ebx].LastAnimStepZ, ebp; struct -> stepZ = 0

		mov	[ebx].LoopOfsFrame, edx	; struct -> LoopOfsFrame

		mov	[ebx].LastAnimStepAlpha, ebp; struct -> stepAlpha = 0
		mov	[ebx].LastAnimStepBeta, ebp ; struct -> stepBeta  = 0
		mov	[ebx].LastAnimStepGamma, ebp; struct -> stepGamma = 0

		mov	edx, TimerRefHR
		cmp	[ebx].LastOfsFrame, ebp	; First anim ?
		mov	[ebx].LastTimer, edx	; struct -> LastTimer
		mov	[ebx].ObjTime, edx	; struct -> ObjTime
		je	firstanim

		mov	eax, 8
		mov	[ebx].Status, ebp
		mov	[ebx].NextOfsFrame, eax	; struct -> NextFrame 0
		mov	eax, [ebx].NbGroups
		mov	[ebx].NextFrame, ebp	; struct -> NextFrame 0

		mov	[ebx].LastNbGroups, ecx	; struct -> LastNbGroups

		cmp	ecx, eax
		jne	changegroups
okgroups:
		movzx	eax, word ptr[esi+8]	; get deltatime for frame
		lea	ecx, [ecx*2-2]		; 2 DWORDs per group, no group 0
		add	edx, eax
		xor	eax, eax
		mov	edi, PtrLib3DBufferAnim	; edi -> looping anim buffer
		inc	eax
		mov	[ebx].NextTimer, edx	; struct -> NextTimer
		mov	[ebx].LastOfsFrame, edi	; copy will be last frame
		mov	[ebx].LastOfsIsPtr, eax
		add	edi, 16
		dec	eax
		lea	esi, [ebx].CurrentFrame	; esi -> CurrentFrame

		mov	dword ptr[edi-16], eax
		mov	dword ptr[edi-12], eax
		mov	dword ptr[edi-8], eax
		mov	dword ptr[edi-4], eax
		dec	eax
		mov	[ebx].LastFrame, eax	; LastFrame = -1
		rep	movsd			; do copy...

		test	ebp, ebp
		jz	noincrease

		mov	eax, [TransFctAnim]
		mov	esi, [ebx].Anim

		test	eax, eax
		jz	skiptrans2

		call	eax
skiptrans2:
		add	esi, [ebx].NextOfsFrame
		mov	ecx, [ebx].NbGroups

		sub	ecx, ebp

		shl	ecx, 1			; *2
		lea	esi, [esi+ebp*8+8]

		rep	movsd
noincrease:
		cmp	edi, EndLib3DBufferAnim	; check buffer for next time
		jg	loopbuf
okbuf:
		mov	PtrLib3DBufferAnim, edi	; write back buffer pointer
exit:
		mov	eax, 1			; ok, exit
		ret
changegroups:
		jb	reducegroups
increasegroups:
		mov	ebp, eax
		mov	[ebx].NbGroups, ecx
		mov	ecx, eax
		jmp	okgroups
reducegroups:
		mov	[ebx].NbGroups, ecx
		mov	eax, ecx
		jmp	okgroups

loopbuf:
		mov	edi, Lib3DBufferAnim
		jmp	okbuf
lloopframe:
		movzx	eax, word ptr[ebp+8]	; get time
		mov	esi, 8
		jmp	fixedloop
firstanim:
		mov	ebp, 8
		xor	eax, eax
		mov	[ebx].Status, STATUS_FRAME
		mov	[ebx].LastOfsFrame, ebp	; struct -> LastOfsFrame -> 0
		mov	[ebx].LastOfsIsPtr, eax
		mov	[ebx].LastFrame, eax	; struct -> LastFrame 0

		mov	[ebx].LastNbGroups, ecx	; struct -> LastNbGroups
		mov	[ebx].NbGroups, ecx	; struct -> NbGroups

		lea	ecx, [ecx*2-2]		; 2 DWORDs per group, no group 0

		mov	ebp, esi		; save ptr
		add	esi, 16+8		; esi -> group 1
		lea	edi, [ebx].CurrentFrame

		rep	movsd			; init frame

		cmp	[ebx].NbFrames, 1
		je	lloopframe

		movzx	eax, word ptr[esi]	; get time
		sub	esi, ebp
		inc	ecx
fixedloop:
		mov	[ebx].NextOfsFrame, esi	; struct -> NextOfsFrame -> 1
		mov	[ebx].NextFrame, ecx	; struct -> NextFrame (0/1)
		add	edx, eax
		mov     eax, 1			; ok
		mov	[ebx].NextTimer, edx	; struct -> NextTimer
		ret				; exit

ObjectInitAnim	endp

;*══════════════════════════════════════════════════════════════════════════*
; 		The
		END
