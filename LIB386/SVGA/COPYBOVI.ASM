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
			public	SYSCALL	CopyBoxVESAI

                        public  SYSCALL CopyBoxVESAI1
                        public  SYSCALL CopyBoxVESAI2
                        public  SYSCALL CopyBoxVESAI3
                        public  SYSCALL CopyBoxVESAI4
                        public  SYSCALL CopyBoxVESAI5
                        public  SYSCALL CopyBoxVESAI6

;----------------------------------------------------------------------------
			Extrn	SYSCALL	VirtualLinearFirst:PROC

;----------------------------------------------------------------------------
IFDEF	_WIN32
			extrn  SYSCALL	AllowPatchAndPatch:PROC
ENDIF

;----------------------------------------------------------------------------
IFDEF	_WIN32

CopyBoxVESAIInit	proc

			push	offset CopyBoxVESAI
			push	offset CopyBoxVESAIEnd
			push	offset Patch_Jump

			jmp	AllowPatchAndPatch
Patch_Jump::
			push	ebp
			mov	ebp, [ebx]

			mov	ebx, [ebx+4]

CopyBoxVESAIInit	endp

ENDIF

;----------------------------------------------------------------------------
;void	CopyBoxVESAI(void *dst, void *src, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux CopyBoxVESAI		       	\
;	parm	[edi] [esi] [edx] [ebx]	\
;	modify	[eax ecx]

CopyBoxVESAI		PROC
			ASSUME	FS:_DATA

			push	ebp
			mov	ebp, [ebx]		; ebp = eax = (XMin, YMin)

			mov	ebx, [ebx+4]		; ebx = (XMax, YMax)
			mov	eax, ebp
IFDEF	_WIN32
x:
			org	CopyBoxVESAI

			jmp	NEAR PTR CopyBoxVESAIInit	; 5 bytes

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
			xor	eax, eax

			call	VirtualLinearFirst

			cmp	ecx, 4
			jz	Equal_4

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
CopyBoxVESAI1::		mov	fs:[edi], eax
CopyBoxVESAI2::		mov	fs:[edi+4], edx
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
CopyBoxVESAI3::		mov	fs:[edi], eax
loopCopyX:
			mov	eax, [esi+4]
			mov	edx, [esi+8]
			add	esi, 8
CopyBoxVESAI4::		mov	fs:[edi+4], eax
CopyBoxVESAI5::		mov	fs:[edi+8], edx
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
CopyBoxVESAIEnd::
			pop	ebp
			ret
Equal_4:
			mov	eax, [esi]
			add	esi, ebp

CopyBoxVESAI6::		mov	fs:[edi], eax
			add	edi, edx

			dec	ebx
			jnz	Equal_4

			pop	ebp
			ret

CopyBoxVESAI		ENDP

;----------------------------------------------------------------------------
;			The
			End
