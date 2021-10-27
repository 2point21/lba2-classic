
		.386p

		.MODEL	SMALL, SYSCALL

		include	aff_obj.inc

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		.DATA

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	X0			:DWORD
		EXTRN	SYSCALL	Y0			:DWORD
		EXTRN	SYSCALL	Z0			:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	MatrixLib2		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	InitMatrixStd		:DWORD
		EXTRN	SYSCALL	RotatePoint		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	TimerRefHR		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		EXTRN	SYSCALL	TransFctAnim		:DWORD

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		.CODE

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
		PUBLIC	SYSCALL	ObjectSetInterDep

;*袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴*
;void	ObjectSetInterDep(T_OBJ *obj)

;#pragma aux ObjectSetInterDep			\
;	parm		[ebx]			\
;	modify exact 	[eax ecx edx esi edi]

ObjectSetInterDep proc uses ebp

		mov	eax, [ebx].NextTexture
		mov	ecx, [ebx].NextBody

		mov	[ebx].Texture, eax
		mov	eax, [TimerRefHR]

		mov	[ebx].Body, ecx
		mov	edx, [ebx].ObjTime		; same time as before?

		cmp	eax,edx
		je	exit1
		jb	fixerror
forcerecomp:
		mov	[ebx].ObjTime, eax		; upgrade time

		mov	esi, [ebx].Anim
		mov	eax, [TransFctAnim]

		test	eax, eax
		jz	skiptrans

		call	eax
skiptrans:
		push	esi
		mov	eax, [ebx].ObjTime

		mov	edx, [ebx].NextOfsFrame
		add	esi, edx

		xor	edx, edx
		mov	ecx, [ebx].NextTimer

		mov	ebp, [ebx].LastTimer

		cmp	eax, ecx
		jae	timesup

		mov	[ebx].Status, STATUS_CHANGE

		sub	ecx, ebp		; ecx = nb steps
		sub	eax, ebp		; eax = delta steps
		mov	ebp, ecx
		inc	ecx
		shl	eax, 16			; delta steps  (1.15.16)
		shr	ecx, 1			; nb steps / 2
		add	eax, ecx		; to get nearest neighbour
		div	ebp			; delta steps / nb steps
		mov	ebp, eax		; factor (1.15.16)

returntime:
		mov	ecx, [ebx].Master	; Master
		mov	eax, [ebx].ObjAlpha
		mov	edx, [ebx].ObjBeta
		test	ecx, 1			; MasterRot ?
		jnz	animmaster
		mov	ecx, [ebx].ObjGamma
returnmaster:

		push	ebx
		push	esi

		mov	ebx, edx
		mov	edi, offset MatrixLib2

		call	[InitMatrixStd]

		pop	esi
		pop	ebx

		movsx	edx, word ptr[esi+2]	; X		(17.15.0)
		movsx	edi, word ptr[esi+4]	; Y
		movsx	eax, word ptr[esi+6]	; Z
		imul	edx, ebp		; result	(1.15.16)
		imul	edi, ebp
		sar	edx, 16			; result	(17.15.0)
		imul	eax, ebp
		mov	ecx, edx
		sar	edi, 16
		sub	ecx, [ebx].LastAnimStepX
		mov	[ebx].LastAnimStepX, edx; X
		push	ecx
		sar	eax, 16
		mov	edx, edi
		mov	ecx, eax
		sub	edx, [ebx].LastAnimStepY
		sub	ecx, [ebx].LastAnimStepZ
		mov	[ebx].LastAnimStepY, edi; Y
		mov	[ebx].LastAnimStepZ, eax; Z
		mov	[ebx].Interpolator, ebp	; interpolator

		pop	eax
		mov	esi, offset MatrixLib2
		mov	ebp, ebx
		mov	ebx, edx

		call	[RotatePoint]

		mov	ebx, ebp

		mov	eax, [X0]
		mov	edx, [Y0]
		mov	ecx, [Z0]
		add	eax, [ebx].ObjX
		add	edx, [ebx].ObjY
		add	ecx, [ebx].ObjZ
		mov	[ebx].ObjX, eax
		mov	eax, [ebx].Status
		mov	[ebx].ObjY, edx
		cmp	eax, STATUS_CHANGE
		pop	ebp
		mov	[ebx].ObjZ, ecx
		jne	frame

		ret
timesup:
		mov	ebp, 10000h
		mov	[ebx].Status, STATUS_FRAME+STATUS_CHANGE
		jmp	returntime
animmaster:
		mov	dx, [esi+10]		; Alpha
		shl	edx, 20
		mov	di, [esi+12]		; Beta
		shl	edi, 20
		mov	ax, [esi+14]		; Gamma
		shl	eax, 20
		sar	edx, 20			; data		(17.15.0)
		sar	edi, 20
		sar	eax, 20
		imul	edx, ebp		; result	(1.15.16)
		imul	edi, ebp
		sar	edx, 16			; result	(17.15.0)
		mov	ecx, [ebx].ObjAlpha
		imul	eax, ebp
		sar	edi, 16
		add	ecx, edx
		sub	ecx, [ebx].LastAnimStepAlpha
		mov	[ebx].LastAnimStepAlpha, edx	; Alpha
		and	ecx, 4095
		mov	edx, [ebx].ObjBeta
		mov	[ebx].ObjAlpha, ecx
		push	ecx
		add	edx, edi
		sar	eax, 16
		mov	ecx, [ebx].ObjGamma
		sub	edx, [ebx].LastAnimStepBeta
		add	ecx, eax
		and	edx, 4095
		mov	[ebx].LastAnimStepBeta, edi	; Beta
		sub	ecx, [ebx].LastAnimStepGamma
		mov	[ebx].LastAnimStepGamma, eax	; Gamma
		and	ecx, 4095
		mov	[ebx].ObjBeta, edx
		mov	[ebx].ObjGamma, ecx
		pop	eax
		jmp	returnmaster

		mov	eax, [edi+4]		; next

		and	eax, 4095
		jz      optequal1

		cmp     eax, 2048
		jle     ok1

		sub     eax, 4096
ok1:
		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
		and	edx, 4095
optequal1:

frame:
		xor	eax, eax
		mov	[ebx].Interpolator, eax

		mov	[ebx].LastAnimStepX, eax; struct -> stepX = 0
		mov	[ebx].LastAnimStepY, eax; struct -> stepY = 0
		mov	[ebx].LastAnimStepZ, eax; struct -> stepZ = 0

		mov	[ebx].LastAnimStepAlpha, eax ; struct -> stepAlpha = 0
		mov	[ebx].LastAnimStepBeta, eax  ; struct -> stepBeta  = 0
		mov	[ebx].LastAnimStepGamma, eax ; struct -> stepGamma = 0

		mov	edx, [ebx].NextFrame
		mov	esi, [ebx].NextOfsFrame
		mov	[ebx].LastFrame, edx
		mov	[ebx].LastOfsFrame, esi
		mov	[ebx].LastOfsIsPtr, eax

		inc	edx			; next frame

		mov	eax, [ebx].NextNbGroups	; nb groups
		mov	[ebx].LastNbGroups, eax	; LastNbGroups
		mov	[ebx].NbGroups, eax	; nb groups

		lea	ecx, [eax*2-2]		; 2 dwords per group, no group 0
		cmp	edx, [ebx].NbFrames
		jge	lframe

		lea	eax, [esi+ecx*4+8+8]
backframe:
		add	esi, 16			; esi -> group 1
		mov	[ebx].NextOfsFrame, eax
		add	eax, ebp
		add	esi, ebp
		mov     edi, [ebx].ObjTime
		movzx	ebp, word ptr[eax+8]
		movzx	eax, word ptr[eax]
		mov	[ebx].Master, ebp
		mov	[ebx].NextFrame, edx
		mov	[ebx].LastTimer, edi	; this time
		add	eax, edi
		lea	edi, [ebx].CurrentFrame
		mov	[ebx].NextTimer, eax	; next time
		mov	eax, [ebx].Status

		rep	movsd			; copy frame

		ret				; ok, return
lframe:
		mov	edx, [ebx].LoopFrame
		mov	eax, [ebx].LoopOfsFrame
		mov	[ebx].Status, STATUS_FRAME+STATUS_LAST+STATUS_CHANGE
		jmp	backframe
exit1:
		test	[ebx].Status, STATUS_BODY
		jnz	forcerecomp
		jmp	exit0
fixerror:
		mov	edx, [ebx].ObjTime
		mov	[ebx].ObjTime, eax
		sub	edx, eax
		mov	eax, [ebx].NextTimer
		mov	ecx, [ebx].LastTimer
		sub	eax, edx
		sub	ecx, edx
		mov	[ebx].NextTimer, eax
		mov	[ebx].LastTimer, ecx
exit0:
		xor	eax, eax
		mov	[ebx].Status, eax	; didn't do anything
		ret

ObjectSetInterDep endp

;*袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴*
; 		The
		END
