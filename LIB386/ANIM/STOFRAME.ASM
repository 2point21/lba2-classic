		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*

		.DATA

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	TimerRefHR		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	Lib3DBufferAnim		:DWORD
		EXTRN	SYSCALL	EndLib3DBufferAnim	:DWORD
		EXTRN	SYSCALL	PtrLib3DBufferAnim	:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		.CODE

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		PUBLIC	SYSCALL	ObjectStoreFrame

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
;void	ObjectStoreFrame(T_OBJ *Obj)

;#pragma aux ObjectStoreFrame		\
;	parm		[ebx]		\
;	modify exact	[eax ecx esi edi]

ObjectStoreFrame proc

		mov	edi, PtrLib3DBufferAnim	; edi -> looping anim buffer

		mov	eax, TimerRefHR
		mov	[ebx].LastTimer, eax	; struct -> LastTimer
		mov	[ebx].ObjTime, eax	; struct -> ObjTime
		xor	eax, eax
		mov	ecx, [ebx].NbGroups
		inc	eax
		mov	[ebx].LastNbGroups, ecx	; struct -> LastNbGroups
		lea	ecx, [ecx*2-2]		; 2 DWORDs per group, no group 0
		mov	[ebx].LastOfsFrame, edi	; copy will be last frame
		mov	[ebx].LastOfsIsPtr, eax
		add	edi, 16
		dec	eax
		lea	esi, [ebx].CurrentFrame	; esi -> CurrentFrame

		mov	[ebx].Status, eax
		mov	dword ptr[edi-16], eax
		mov	dword ptr[edi-12], eax
		mov	dword ptr[edi-8], eax
		mov	dword ptr[edi-4], eax
		dec	eax
		rep	movsd			; do copy...

		cmp	edi, EndLib3DBufferAnim	; check buffer for next time
		jle	okbuf

		mov	edi, Lib3DBufferAnim
okbuf:
		mov	PtrLib3DBufferAnim, edi	; write back buffer pointer
		mov	[ebx].LastFrame, eax	; LastFrame = -1

		ret

ObjectStoreFrame endp

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
;		The
		END
