
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
		PUBLIC	SYSCALL	ObjectSetInterFrame

;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
;void	ObjectSetInterFrame(T_OBJ *obj)

;#pragma aux ObjectSetInterFrame	\
;	parm		[ebx]		\
;	modify exact 	[eax ecx edx esi edi]

ObjectSetInterFrame proc uses ebp

		mov	eax, [ebx].Status

		test	eax, STATUS_FRAME	; something to do ?
		jnz	exit

		test	eax, STATUS_CHANGE	; something to do ?
		jz	exit

		xor	eax, eax
		mov	esi, [ebx].Anim

		mov	[ebx].Status, eax	; ok nothing to do next time
		mov	eax, [TransFctAnim]

		test	eax, eax
		jz	skiptrans

		call	eax
skipTrans:
		mov	eax, [ebx].Status
		mov	edi, [ebx].LastOfsFrame

		cmp	[ebx].LastOfsIsPtr, 1
		je	skipofs
		add	edi, esi
skipofs:
		add	esi, [ebx].NextOfsFrame

		mov	ecx, [ebx].NbGroups
		add	edi, 16			; -> group 1
		add	esi, 16			; -> group 1
		dec	ecx
		jz	exit			; only one group? exit

		lea	ebp, [ebx].CurrentFrame
		mov	ebx, [ebx].Interpolator
nextgrouprot:
		mov	ax, [esi]
		mov	[ebp], ax
		test	ax, ax
		jnz	trans
rot:
		mov	ax, [esi+2]		; next
		mov	dx, [edi+2]		; last

		sub     eax, edx
		and	eax, 4095
		jz      optequal0

		shl	eax, 20
		sar	eax, 20

		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
		and	edx, 4095
optequal0:
		mov	ax, [esi+4]		; next
		mov	[ebp+2], dx		; Alpha
		mov	dx, [edi+4]		; last

		sub     eax, edx
		and	eax, 4095
		jz      optequal1

		shl	eax, 20
		sar	eax, 20

		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
		and	edx, 4095
optequal1:
		mov	ax, [esi+6]		; next
		mov	[ebp+4], dx		; Beta
		mov	dx, [edi+6]		; last

		sub     eax, edx
		and	eax, 4095
		jz      optequal2

		shl	eax, 20
		sar	eax, 20

		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
		and	edx, 4095
optequal2:
		add	edi, 8
		mov	[ebp+6], dx		; Gamma
		add	esi, 8
		add	ebp, 8

		dec	ecx
		jnz	nextgrouprot

		ret
nextgrouptrans:
		mov	ax, [esi]
		mov	[ebp], ax
		test	ax, ax
		jz	rot
trans:
		movsx	eax, word ptr[esi+2]	; next
		movsx	edx, word ptr[edi+2]	; last

		sub     eax, edx
		jz      optequal3

		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
optequal3:
		movsx	eax, word ptr[esi+4]	; next
		mov	[ebp+2], dx		; X
		movsx	edx, word ptr[edi+4]	; last

		sub     eax, edx
		jz      optequal4

		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
optequal4:
		movsx	eax, word ptr[esi+6]	; next
		mov	[ebp+4], dx		; Y
		movsx	edx, word ptr[edi+6]	; last

		sub     eax, edx
		jz      optequal5

		imul    eax, ebx
		sar	eax, 16
		add     edx, eax
optequal5:
		add	edi, 8
		mov	[ebp+6], dx		; Z
		add	esi, 8
		add	ebp, 8

		dec	ecx
		jnz	nextgrouptrans
exit:
		ret

ObjectSetInterFrame endp

;*袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴*
; 		The
		END
