;----------------------------------------------------------------------------
			.386

			.MODEL	FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	CopyBoxI

;----------------------------------------------------------------------------
IFDEF	_WIN32
		extrn  SYSCALL	AllowPatchAndPatch:PROC
ENDIF

;----------------------------------------------------------------------------
IFDEF	_WIN32

CopyBoxIInit	proc

			push	offset CopyBoxI
			push	offset CopyBoxIEnd
			push	offset Patch_Jump

			jmp	AllowPatchAndPatch
Patch_Jump::
			push	ebp
			mov	ebp, [ebx]

			mov	ebx, [ebx+4]

CopyBoxIInit	endp

ENDIF

;----------------------------------------------------------------------------
;void	CopyBoxI(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBoxI		       	\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxI		PROC

			push	ebp
			mov	ebp, [ebx]		; ebp = eax = (XMin, YMin)

			mov	ebx, [ebx+4]		; ebx = (XMax, YMax)
			mov	eax, ebp
IFDEF	_WIN32
x:
			org	CopyBoxI

			jmp	NEAR PTR CopyBoxIInit	; 5 bytes

			org	x
ENDIF
			shr	ebp, 16			; EBP = YMin
			and	eax, 0FFFFh		; EAX = XMin

			add	esi, eax
			mov	ecx, ebx

			add	edi, eax
			add	esi, TabOffLine[ebp*4]

			add	edi, [edx+ebp*4]
			mov	edx, [edx+4]		; EDX = Pitch Dst

			shr	ebx, 16
			and	ecx, 0FFFFh

			sub	ecx, eax		; ECX = deltaX
			sub 	ebx, ebp		; EBX = deltaY

			mov	ebp, TabOffLine[1*4]	; EBP = Pitch Src

			cmp	ecx, 4
			je	Equal_4

			sub	edx, ecx
			sub	ebp, ecx

			shr	ecx, 3
			jc	odd

			mov	[PatchECX1+1], ecx
			mov	[PatchEDI1+2], edx
			jmp	loopCopy		; empty pretfetch
loopCopy:
			mov	eax, [esi]
			mov	edx, [esi+4]
			add	esi, 8
			mov	[edi], eax
			mov	[edi+4], edx
			add	edi, 8
			dec	ecx
			jnz	loopCopy
			add	esi, ebp
PatchEDI1	label	DWORD
			add	edi, 12345678h
			dec	ebx
PatchECX1	label	DWORD
			mov	ecx, 12345678h
			jnz	loopCopy

			pop	ebp
			ret
odd:
			add	edx, 4
			add	ebp, 4
			mov	[PatchECX2+1], ecx
			mov	[PatchEDI2+2], edx
			jmp	loopCopyY		; empty pretfetch
loopCopyY:
			mov	eax, [esi]
			mov	[edi], eax
loopCopyX:
			mov	eax, [esi+4]
			mov	edx, [esi+8]
			add	esi, 8
			mov	[edi+4], eax
			mov	[edi+8], edx
			add	edi, 8
			dec	ecx
			jnz	loopCopyX
			add	esi, ebp
PatchEDI2	label	DWORD
			add	edi, 12345678h
			dec	ebx
PatchECX2	label	DWORD
			mov	ecx, 12345678h
			jnz	loopCopyY
CopyBoxIEnd::
			pop	ebp
			ret
Equal_4:
			mov	eax, [esi]
			add	esi, ebp

			mov	[edi], eax
			add	edi, edx

			dec	ebx
			jnz	Equal_4

			pop	ebp
			ret

CopyBoxI		ENDP

;----------------------------------------------------------------------------
;			The
			End
