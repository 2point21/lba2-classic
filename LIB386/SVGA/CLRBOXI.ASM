;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	ClearColor:DWORD

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	ClearBoxI

;----------------------------------------------------------------------------
;void	ClearBoxI(void *dst, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBoxI		       	\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxI		PROC

			push	ebp
			mov	ebp, [ebx]		; ebp = eax = (XMin, YMin)

			mov	ebx, [ebx+4]		; ebx = (XMax, YMax)
			mov	eax, ebp

			shr	ebp, 16			; EBP = YMin
			and	eax, 0FFFFh		; EAX = XMin

			mov	ecx, ebx
			add	edi, eax

			add	edi, [edx+ebp*4]
			mov	edx, [edx+4]		; EDX = Pitch Dst

			shr	ebx, 16
			and	ecx, 0FFFFh

			sub	ecx, eax		; ECX = deltaX
			sub 	ebx, ebp		; EBX = deltaY

			sub	edx, ecx
			mov	eax, [ClearColor]

			shr	ecx, 3
			mov	esi, ecx

			jc	odd
loopClear:
			mov	[edi], eax
			mov	[edi+4], eax
			add	edi, 8
			dec	ecx
			jnz	loopClear
			add	edi, edx
			mov	ecx, esi
			dec	ebx
			jnz	loopClear

			pop	ebp
			ret
odd:
			cmp	ecx,4
			je	@@Equal_4		; JBE :)
loopClearY:
			mov	[edi], eax
loopClearX:
			mov	[edi+4], eax
			mov	[edi+8], eax
			add	edi, 8
			dec	ecx
			jnz	loopClearX
			add	edi, edx
			mov	ecx, esi
			dec	ebx
			jnz	loopClearY
ClearBoxIEnd::
			pop	ebp
			ret
@@Equal_4:
			mov	[edi], eax
			add	edi, edx
			dec	ebx
			jnz	@@Equal_4

			pop	ebp
			ret

ClearBoxI		ENDP

;----------------------------------------------------------------------------
;			The
			End
