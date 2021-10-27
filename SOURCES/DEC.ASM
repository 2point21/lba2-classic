;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
;			      (c) Adeline 1995
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		.386P
		.model FLAT, SYSCALL

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

;SCANLINE	equ	640
;SHIFT		equ	7

SCANLINE	equ	320
SHIFT		equ	6



;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		.data

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		Public	syscall	BaseOldScr
		Public	syscall	BaseNewScr
		Public	syscall	BaseData

		Public	syscall	Patch_CodeLen
		Public	syscall	Patch_MaxLine

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		Public	syscall	CurLine
		Public	syscall	CurCol
		Public	syscall	DeltaBase
		Public	syscall	NewScr
		Public	syscall	Codes
		Public	syscall	CurCodes
		Public	syscall	Bits
		Public	syscall	Colors
		Public	syscall	Dummy
		Public	syscall	TabLines
		Public	syscall	TabFunc
		Public	syscall	TabCorrect

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

CurLine		db	?
CurCol		db	?

	ALIGN 4

BaseOldScr	dd	?
BaseNewScr	dd	?
DeltaBase	dd	?
NewScr		dd	?
BaseData	dd	?
Codes		dd	?
CurCodes	dd	?
Bits		dd	?
Colors		dd	?

Dummy		dd	?

Tab_Addr	dd	4 dup (?)

TabOfs		dd	2*320+2
		dd	2*320+2+4
		dd	(2+4)*320+2
		dd	(2+4)*320+2+4

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

Patterns	dd	256 dup (?)

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

TabLines	dd	SCANLINE*0
		dd	SCANLINE*1
		dd	SCANLINE*2
		dd	SCANLINE*3
		dd	SCANLINE*4
		dd	SCANLINE*5
		dd	SCANLINE*6
		dd	SCANLINE*7

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		public Raw

		public ZeroMotionDecode

		public SMotion8Decode

		public Motion8Decode

		public SMotion4Decode

		public Motion4Decode

		public Uni8Decode
		public Uni4Decode

		public VR8Decode2
		public VR8Decode4
		public VR8Decode8
		public VR8Decode16

		public VR4Decode2
		public VR4Decode4
		public VR4Decode8

		public CrossDecode

		public PrimeDecode

		public Bank1Decode
		public Bank2Decode

		public BlockDecode

		public BlockBank1Decode

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

TabFunc		dd	offset Raw		; 0

		dd	offset ZeroMotionDecode	; 1
		dd	offset ZeroMotionDecode	; 2
		dd	offset ZeroMotionDecode	; 3
		dd	offset ZeroMotionDecode	; 4

		dd	offset SMotion8Decode	; 5
		dd	offset SMotion8Decode	; 6
		dd	offset SMotion8Decode	; 7
		dd	offset SMotion8Decode	; 8

		dd	offset Motion8Decode	; 9
		dd	offset Motion8Decode	; 10
		dd	offset Motion8Decode	; 11
		dd	offset Motion8Decode	; 12

		dd	offset SMotion4Decode	; 13
		dd	offset SMotion4Decode	; 14
 		dd	offset SMotion4Decode	; 15
		dd	offset SMotion4Decode	; 16

		dd	offset Motion4Decode	; 17
		dd	offset Motion4Decode	; 18
 		dd	offset Motion4Decode	; 19
		dd	offset Motion4Decode	; 20

		dd	offset Uni8Decode	; 21
		dd	offset Uni8Decode	; 22
		dd	offset Uni8Decode	; 23
		dd	offset Uni8Decode	; 24

		dd	offset Uni4Decode	; 25
		dd	offset Uni4Decode	; 26
		dd	offset Uni4Decode	; 27
		dd	offset Uni4Decode	; 28

		dd	offset VR8Decode2	; 29
		dd	offset VR8Decode4	; 30
		dd	offset VR8Decode8	; 31
		dd	offset VR8Decode16	; 32

		dd	offset VR4Decode2	; 33
		dd	offset VR4Decode4	; 34
		dd	offset VR4Decode8	; 35

		dd	offset CrossDecode	; 36
		dd	offset PrimeDecode	; 37

		dd	offset Bank1Decode	; 38
		dd	offset Bank2Decode	; 39

		dd	offset BlockDecode	; 40
		dd	offset BlockDecode	; 41
		dd	offset BlockDecode	; 42
		dd	offset BlockDecode	; 43

		dd	offset BlockBank1Decode	; 44
		dd	offset BlockBank1Decode	; 45
		dd	offset BlockBank1Decode	; 46
		dd	offset BlockBank1Decode	; 47

		dd	offset ROMotion8Decode	; 48
		dd	offset ROMotion8Decode	; 49
		dd	offset ROMotion8Decode	; 50
		dd	offset ROMotion8Decode	; 51

		dd	offset RCMotion8Decode	; 52
		dd	offset RCMotion8Decode	; 53
		dd	offset RCMotion8Decode	; 54
		dd	offset RCMotion8Decode	; 55

		dd	offset ROMotion4Decode	; 56
		dd	offset ROMotion4Decode	; 57
		dd	offset ROMotion4Decode	; 58
		dd	offset ROMotion4Decode	; 59

		dd	offset RCMotion4Decode	; 60
		dd	offset RCMotion4Decode	; 61
		dd	offset RCMotion4Decode	; 62
		dd	offset RCMotion4Decode	; 63

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		public Return
		public Correct_4
		public Correct_8
		public Correct_16

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

TabCorrect	dd	offset Return
		dd	offset Correct_4
		dd	offset Correct_8
		dd	offset Correct_16

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		.code

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		Public	syscall	Decompressor
		Public	syscall	InitDecompressor

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

Decompressor	PROC

		push	ebx
		push	ebp
		push	esi
		push	edi

		mov	ebx, BaseData

		mov	ecx, BaseOldScr
		mov	eax, BaseNewScr

		mov	esi, [ebx]
Patch_CodeLen	label	dword
		lea	edi, [ebx+720+4]
		add	esi, ebx
		mov	Bits, edi
		mov	Colors, esi

		xor	dl, dl

		add	ebx, 4

		mov	CurLine, dl
		mov	CurCol, dl

		sub	ecx, eax
		mov	NewScr, eax
		mov	DeltaBase, ecx

		mov	ecx, [ebx]
		mov	edx, [ebx]
		add	ebx, 3
		or	ecx, 0FF000000h
		and	edx, 3Fh
		shr	ecx, 6
		mov	Codes, ebx
		mov	CurCodes, ecx
		jmp	TabFunc[edx*4]
	ALIGN 4
Return::	mov	al, CurCol
		mov	ecx, NewScr
		cmp	al, 39
		jae	exit1

		inc	al
		add	ecx, 8
		mov	CurCol, al
		mov	NewScr, ecx

		mov	ebx, CurCodes
		mov	edx, CurCodes
		cmp	ebx, 0FFh
		je	last
		shr	ebx, 6
		and	edx, 3Fh
		mov	CurCodes, ebx
		jmp	TabFunc[edx*4]
last:
		mov	ecx, Codes
		mov	ebx, [ecx]
		mov	edx, [ecx]
		add	ecx, 3
		or	ebx, 0FF000000h
		and	edx, 3Fh
		shr	ebx, 6
		mov	Codes, ecx
		mov	CurCodes, ebx
		jmp	TabFunc[edx*4]
	ALIGN 4
exit1:
		mov	al, CurLine
		add	ecx, 8+(SCANLINE-320)+SCANLINE*7
Patch_MaxLine	label	byte
		cmp 	al, 23
		jae	exit2

		mov	ebx, Codes
		mov	NewScr, ecx
		inc	al
		xor	dl, dl
		mov	CurLine, al
		mov	CurCol, dl

		mov	ecx, [ebx]
		mov	edx, [ebx]
		add	ebx, 3
		or	ecx, 0FF000000h
		and	edx, 3Fh
		shr	ecx, 6
		mov	Codes, ebx
		mov	CurCodes, ecx
		jmp	TabFunc[edx*4]
	ALIGN 4
exit2:
		pop	edi
		pop	esi
		pop	ebp
		pop	ebx

		ret

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Raw::

		mov	esi, Colors
		mov	ecx, Colors
		add	esi, 64
		mov	edi, NewScr
		mov	Colors, esi

		ofs = 0
		ofd = 0

		REPEAT	8

		mov	eax, [ecx+ofs]
		mov	ebx, [ecx+4+ofs]
		mov	[edi+ofd], eax
		mov	[edi+4+ofd], ebx

		ofs = ofs + 8
		ofd = ofd + SCANLINE

		ENDM

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Correct_16::

		mov	ebp, Colors	; ebp -> Colors

		mov	bh, 2		; loop2 counter
		mov	cl, 1		; Mask
C_loop2:
		mov	edx, [esi]	; edx = bits
		mov	bl, 4		; loop1 counter
		add	esi, 4
		jmp	C_enter1

	ALIGN	4

C_loop1:	shl	cl, 1
		jc	C_exit1		; last -> exit
C_enter1:
		inc	edi
		test	dl, cl		; get color?
		jz	C_loop1

		mov	al, [ebp]	; read color

		shl	cl, 1
		mov	[edi-1], al	; write
		inc	ebp
		jnc	C_enter1	; not last -> keep rolling
C_exit1:
		add	edi, SCANLINE-8	; Next Line
		shr	edx, 8		; Next bit field
		dec	bl		; one less to do
		mov	cl, 1		; Mask
		jnz	C_enter1

		dec	bh		; one less to do
		jnz	C_loop2

		mov	Bits, esi	; updates
		mov	Colors, ebp

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Correct_8::

		mov	ecx, Colors	; ecx -> Colors
		mov	ebx, [esi]
		add	ecx, 4
		mov	edx, ebx
		mov	Colors, ecx
		mov	eax, ebx

		shr	edx, 3-2
		and	ebx, 7
		shr	eax, 6
		and	edx, 7*4
		mov	ecx, [ecx-4]
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[ebx+edi], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	edx, [esi+3]
		mov	[edi+ebx], cl
		mov	ecx, Colors
		mov	ebx, [esi+3]
		add	ecx, 4
		mov	eax, [esi+3]
		mov	Colors, ecx

		shr	edx, 3-2
		and	ebx, 7
		shr	eax, 6
		and	edx, 7*4
		mov	ecx, [ecx-4]
		add	ebx, TabLines[edx]

		add	esi, 6

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	Bits, esi	; updates

		mov	[edi+ebx], cl

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Correct_4::

		mov	ecx, Colors	; ecx -> Colors
		mov	ebx, [esi]
		add	ecx, 4
		mov	edx, ebx
		mov	Colors, ecx
		mov	eax, ebx

		shr	edx, 3-2
		and	ebx, 7
		shr	eax, 6
		and	edx, 7*4
		mov	ecx, [ecx-4]
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[ebx+edi], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	esi, 3
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	edx, eax
		mov	[edi+ebx], cl
		mov	ebx, eax

		shr	edx, 3-2
		and	ebx, 7
		shr	ecx, 8
		and	edx, 7*4
		shr	eax, 6
		add	ebx, TabLines[edx]

		mov	Bits, esi		; updates

		mov	[edi+ebx], cl

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

ZeroMotionDecode::

		mov	ecx, NewScr
		mov	esi, Bits
		mov	edi, ecx
		add	ecx, DeltaBase


		of = 0

		REPEAT	8

		mov	eax, [ecx+of]
		mov	ebx, [ecx+4+of]
		mov	[edi+of], eax
		mov	[edi+4+of], ebx

		of = of + SCANLINE

		ENDM

		jmp	TabCorrect[edx*4-1*4]

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

SMotion8Decode::

		mov	esi, Bits
		mov	edi, NewScr
		mov	ecx, [esi-3]
		mov	eax, [esi-3]
		shl	ecx, 4
		sar	eax, 28
		sar	ecx, 28
		shl	eax, SHIFT
		add	ecx, edi
		add	ecx, DeltaBase

		lea	eax, [eax+eax*4]
		inc	esi
		sub	edx, 5
		lea	ecx, [ecx+eax+4*320+4]
		mov	Bits, esi
Copy8x8:
		of = 0

		REPEAT	8

		mov	eax, [ecx+of]
		mov	ebx, [ecx+4+of]
		mov	[edi+of], eax
		mov	[edi+4+of], ebx

		of = of + SCANLINE

		ENDM

		jmp	TabCorrect[edx*4]

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Motion8Decode::

		mov	esi, Bits
		mov	edi, NewScr
		mov	ecx, [esi]
		add	esi, 2
		and	ecx, 0FFFFh
		sub	edx, 9
		add	ecx, BaseOldScr
		mov	Bits, esi

		jmp	Copy8x8

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

ROMotion8Decode::

		mov	esi, Bits
		mov	edi, NewScr
		movsx	ecx, word ptr[esi]
		add	esi, 2
		add	ecx, DeltaBase
		sub	edx, 48
		lea	ecx, [ecx+edi+4*320+4]
		mov	Bits, esi

		jmp	Copy8x8

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

RCMotion8Decode::

		mov	esi, Bits
		mov	edi, NewScr
		movsx	ecx, byte ptr[esi]
		movsx	eax, byte ptr[esi+1]
		add	esi, 2
		add	ecx, edi
		shl	eax, SHIFT
		add	ecx, DeltaBase
		mov	Bits, esi
		lea	eax, [eax+eax*4]
		sub	edx, 52
		lea	ecx, [ecx+eax+4*320+4]

		jmp	Copy8x8

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

SMotion4Decode::

		mov	esi, Bits
		mov	edi, NewScr
		mov	ebp, [esi]
		add	esi, 4
		mov	ebx, -4
		mov	Bits, esi
		mov	esi, DeltaBase
		sub	edx, 13
		add	esi, edi
loopSMotion4:
		ror	ebp, 8
		mov	ecx, ebp
		mov	eax, ebp
		add	ebx, 4
		shl	ecx, 4
		sar	eax, 28
		sar	ecx, 28
		shl	eax, SHIFT
		add	ecx, esi
		add	ecx, TabOfs[ebx]
		lea	eax, [eax+eax*4]
		add	ecx, eax
		cmp	ebx, 12
		mov	Tab_Addr[ebx], ecx
		jne	loopSMotion4

		mov	esi, Bits
CopyMotion4:
		mov	ecx, Tab_Addr[0]
		mov	eax, [ecx+SCANLINE*0]
		mov	ebx, [ecx+SCANLINE*1]
		mov	[edi+SCANLINE*0], eax
		mov	[edi+SCANLINE*1], ebx
		mov	eax, [ecx+SCANLINE*2]
		mov	ebx, [ecx+SCANLINE*3]
		mov	ecx, Tab_Addr[4]
		mov	[edi+SCANLINE*2], eax
		mov	[edi+SCANLINE*3], ebx

		mov	eax, [ecx+SCANLINE*0]
		mov	ebx, [ecx+SCANLINE*1]
		mov	[edi+4+SCANLINE*0], eax
		mov	[edi+4+SCANLINE*1], ebx
		mov	eax, [ecx+SCANLINE*2]
		mov	ebx, [ecx+SCANLINE*3]
		mov	ecx, Tab_Addr[8]
		mov	[edi+4+SCANLINE*2], eax
		mov	[edi+4+SCANLINE*3], ebx

		mov	eax, [ecx+SCANLINE*0]
		mov	ebx, [ecx+SCANLINE*1]
		mov	[edi+SCANLINE*4], eax
		mov	[edi+SCANLINE*5], ebx
		mov	eax, [ecx+SCANLINE*2]
		mov	ebx, [ecx+SCANLINE*3]
		mov	ecx, Tab_Addr[12]
		mov	[edi+SCANLINE*6], eax
		mov	[edi+SCANLINE*7], ebx

		mov	eax, [ecx+SCANLINE*0]
		mov	ebx, [ecx+SCANLINE*1]
		mov	[edi+4+SCANLINE*4], eax
		mov	[edi+4+SCANLINE*5], ebx
		mov	eax, [ecx+SCANLINE*2]
		mov	ebx, [ecx+SCANLINE*3]
		mov	[edi+4+SCANLINE*6], eax
		mov	[edi+4+SCANLINE*7], ebx

		jmp	TabCorrect[edx*4]

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Motion4Decode::

		mov	esi, Bits
		mov	edi, NewScr
		mov	ebx, -4
		sub	edx, 17
loopMotion4:
		movzx	ecx, word ptr[esi]
		add	ebx, 4
		add	esi, 2
		add	ecx, BaseOldScr
		cmp	ebx, 12
		mov	Tab_Addr[ebx], ecx
		jne	loopMotion4

		mov	Bits, esi

		jmp	CopyMotion4

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

ROMotion4Decode::

		mov	esi, Bits
		mov	edi, NewScr
		sub	edx, 56
		mov	eax, DeltaBase
		mov	ebx, -4
		add	eax, edi
loopROMotion4:
		movsx	ecx, word ptr[esi]
		add	ebx, 4
		add	ecx, eax
		add	esi, 2
		add	ecx, TabOfs[ebx]
		cmp	ebx, 12
		mov	Tab_Addr[ebx], ecx
		jne	loopROMotion4

		mov	Bits, esi

		jmp	CopyMotion4

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

RCMotion4Decode::

		mov	esi, Bits
		mov	edi, NewScr
		mov	ebx, -4
		mov	ebp, DeltaBase
		sub	edx, 60
		add	ebp, edi
loopRCMotion4:
		mov	ecx, [esi-2]
		mov	eax, [esi-2]
		shl	ecx, 8
		sar	eax, 24
		sar	ecx, 24
		shl	eax, SHIFT
		add	ecx, ebp
		add	ebx, 4
		add	ecx, TabOfs[ebx]
		lea	eax, [eax+eax*4]
		add	esi, 2
		add	ecx, eax
		cmp	ebx, 12
		mov	Tab_Addr[ebx], ecx
		jne	loopRCMotion4

		mov	Bits, esi

		jmp	CopyMotion4

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Uni8Decode::

		mov	ecx, Colors
		xor	ebx, ebx
		mov	edi, NewScr
		mov	bl, [ecx]
		inc	ecx
		mov	bh, bl
		mov	Colors, ecx
		mov	eax, ebx
		shl	ebx, 16
		mov	esi, Bits
		or	eax, ebx

		ofd = 0

		REPEAT	8

		mov	[edi+ofd], eax
		mov	[edi+4+ofd], eax

		ofd = ofd + SCANLINE

		ENDM

		jmp	TabCorrect[edx*4-21*4]

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Uni4Decode::
		mov	ebp, Colors
		xor	eax, eax
		xor	ebx, ebx
		mov	ecx, [ebp]
		add	ebp, 4
		mov	al, cl
		mov	bl, ch
		mov	ah, cl
		mov	bh, ch
		mov	Colors, ebp
		mov	edi, NewScr
		mov	esi, eax
		mov	ebp, ebx
		shl	eax, 16
		shl	ebx, 16
		or	eax, esi
		or	ebx, ebp

		shr	ecx, 16

	ofd = 0

	REPEAT	4

		mov	[edi+ofd], eax
		mov	[edi+4+ofd], ebx

	ofd = ofd + SCANLINE

	ENDM

		xor	eax, eax
		xor	ebx, ebx
		mov	al, cl
		mov	bl, ch
		mov	ah, cl
		mov	bh, ch
		mov	esi, eax
		mov	ebp, ebx
		shl	eax, 16
		shl	ebx, 16
		or	eax, esi
		or	ebx, ebp

		mov	esi, Bits

	REPEAT	4

		mov	[edi+ofd], eax
		mov	[edi+4+ofd], ebx

	ofd = ofd + SCANLINE

	ENDM

		jmp	TabCorrect[edx*4-25*4]

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR8Decode2::

		mov	esi, Bits	; esi -> bits
		mov	edi, Colors	; edi -> colors

		mov	edx, [esi]	; edx = bits[0]
		mov	ecx, [esi+4]	; ecx = bits[4]

		mov	eax, [edi-2]
		mov	ebx, [edi]	; ebx = colors
		and	eax, 0FFFF0000h
		and	ebx, 00000FFFFh
		add	esi, 8
		add	edi, 2
		or	ebx, eax

		mov	Bits, esi
		mov	Colors, edi

		mov	edi, NewScr	; edi -> NewScr
		mov	esi, 8
		jmp	V2_start
	ALIGN 4
V2_loop1:
		add	edi, SCANLINE
V2_start:

	of = 0

	REPEAT 2
	local ok1, ok2, ok3, ok4

		shrd	edx, ecx, 1
		mov	eax, ebx
		jnc	ok1
		mov	al, bh
ok1:
		shr	ecx, 1
		shrd	edx, ecx, 1
		jc	ok2
		mov	ah, bl
ok2:
		shr	ecx, 1
		ror	eax, 16
		shrd	edx, ecx, 1
		jnc	ok3
		mov	al, bh
ok3:
		shr	ecx, 1
		shrd	edx, ecx, 1
		jc	ok4
		mov	ah, bl
ok4:
		rol	eax, 16
		shr	ecx, 1

		mov	[edi+of], eax

	of = of + 4

	ENDM
		dec	esi
		jnz	V2_loop1

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR8Decode4::

		mov	ebp, 4
		mov	esi, Bits	; esi -> bits
		mov	ebx, Colors	; ebx -> colors
		mov	edi, NewScr	; edi -> NewScr

	ALIGN 4
V4_loop1:
		mov	Dummy, ebp
		mov	edx, [esi]
		mov	ebp, [esi]
		mov	ecx, [esi]

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		ror	eax, 16
		mov	ecx, edx

		mov	[edi], eax

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		ror	eax, 16
		mov	ecx, edx

		mov	[edi+4], eax

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		ror	eax, 16
		mov	ecx, edx

		mov	[edi+SCANLINE], eax


		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 2
		and	ecx, 3
		and	ebp, 3
		mov	al, [ebx+ecx]
		shr	edx, 4
		mov	ah, [ebx+ebp]

		add	edi, SCANLINE*2
		ror	eax, 16
		mov	ebp, Dummy

		mov	[edi+4-SCANLINE], eax

		add	esi, 4
		dec	ebp
		jnz	V4_loop1

		add	ebx, 4

		mov	Bits, esi
		mov	Colors, ebx

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR8Decode8::

		mov	ebp, 8
		mov	esi, Bits	; esi -> bits
		mov	ebx, Colors     ; ebx -> colors
		mov	edi, NewScr	; edi -> NewScr

	ALIGN 4
V8_loop1:
		mov	Dummy, ebp
		mov	edx, [esi]
		mov	ebp, [esi]
		mov	ecx, [esi]

		shr	ebp, 3
		and	ecx, 7
		and	ebp, 7
		mov	al, [ebx+ecx]
		shr	edx, 6
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 3
		and	ecx, 7
		and	ebp, 7
		mov	al, [ebx+ecx]
		shr	edx, 6
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		ror	eax, 16
		mov	ecx, edx

		mov	[edi], eax

		shr	ebp, 3
		and	ecx, 7
		and	ebp, 7
		mov	al, [ebx+ecx]
		shr	edx, 6
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 3
		and	ecx, 7
		and	ebp, 7
		mov	al, [ebx+ecx]
		shr	edx, 6
		mov	ah, [ebx+ebp]

		add	edi, SCANLINE
		ror	eax, 16
		mov	ebp, Dummy

		mov	[edi+4-SCANLINE], eax

		add	esi, 3
		dec	ebp
		jnz	V8_loop1


		add	ebx, 8
		mov	Bits, esi
		mov	Colors, ebx

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR8Decode16::

		mov	ebp, 8
		mov	esi, Bits	; esi -> bits
		mov	ebx, Colors	; ebx -> colors
		mov	edi, NewScr	; edi -> NewScr

	ALIGN 4
V16_loop1:
		mov	Dummy, ebp
		mov	edx, [esi]
		mov	ebp, [esi]
		mov	ecx, [esi]

		shr	ebp, 4
		and	ecx, 0Fh
		and	ebp, 0Fh
		mov	al, [ebx+ecx]
		shr	edx, 8
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 4
		and	ecx, 0Fh
		and	ebp, 0Fh
		mov	al, [ebx+ecx]
		shr	edx, 8
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		ror	eax, 16
		mov	ecx, edx

		mov	[edi], eax

		shr	ebp, 4
		and	ecx, 0Fh
		and	ebp, 0Fh
		mov	al, [ebx+ecx]
		shr	edx, 8
		mov	ah, [ebx+ebp]

		mov	ebp, edx
		mov	ecx, edx

		ror	eax, 16

		shr	ebp, 4
		and	ecx, 0Fh
		and	ebp, 0Fh
		mov	al, [ebx+ecx]
		shr	edx, 8
		mov	ah, [ebx+ebp]

		add	edi, SCANLINE
		ror	eax, 16
		mov	ebp, Dummy

		mov	[edi+4-SCANLINE], eax

		add	esi, 4
		dec	ebp
		jnz	V16_loop1

		add	ebx, 16

		mov	Bits, esi
		mov	Colors, ebx

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR4OneDecode2::

		mov	edx, [esi]	; edx = bits[0]

		mov	ecx, [ebp]	; ecx = colors
		mov	eax, [ebp-2]
		and	ecx, 00000FFFFh
		and	eax, 0FFFF0000h
		or	ecx, eax

	of = 0

	REPEAT 4
	local ok1, ok2, ok3, ok4

		shr	edx, 1
		mov	eax, ecx
		jnc	ok1
		mov	al, ch
ok1:
		shr	edx, 1
		jc	ok2
		mov	ah, cl
ok2:
		ror	eax, 16
		shr	edx, 1
		jnc	ok3
		mov	al, ch
ok3:
		shr	edx, 1
		jc	ok4
		mov	ah, cl
ok4:
		rol	eax, 16

		mov	[edi+of], eax

	of = of + SCANLINE

	ENDM

		ret

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR4Decode2::

		mov	edi, NewScr
		mov	esi, Bits
		mov	ebp, Colors

		call	VR4OneDecode2

		add	edi, 4
		add	esi, 2
		add	ebp, 2

		call	VR4OneDecode2

		add	edi, SCANLINE*4-4
		add	esi, 2
		add	ebp, 2

		call	VR4OneDecode2

		add	edi, 4
		add	esi, 2
		add	ebp, 2

		call	VR4OneDecode2

		add	esi, 2
		add	ebp, 2
		mov	Bits, esi
		mov	Colors, ebp

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR4OneDecode4::

		mov	edx, [esi]	; bits

	of = 0
	f  = 0

	REPEAT 2

	REPEAT 4

	IF f EQ 0
		mov	ebp, [esi]	; bits
		mov	ecx, [esi]
	ELSE
		mov	ebp, edx
		mov	ecx, edx
	ENDIF
		shr	ebp, 8
		and	ecx, 3
		and	ebp, 3
		shr	edx, 2
		add	ecx, Colors
		add	ebp, Colors
		mov	al, [ecx]	; colors
		mov	bl, [ebp]
		ror	eax, 8
		ror	ebx, 8

	f = 1

	ENDM

	IF of EQ 0
		shr	edx, 8
	ENDIF

		mov	[edi+of], eax
		mov	[edi+of+SCANLINE], ebx

	of = of + SCANLINE*2

	ENDM
		ret

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR4Decode4::

		mov	esi, Bits
		mov	edi, NewScr

		call	VR4OneDecode4

		mov	eax, Colors
		add	edi, 4
		add	eax, 4
		add	esi, 4
		mov	Colors, eax

		call	VR4OneDecode4

		mov	eax, Colors
		add	edi, SCANLINE*4-4
		add	eax, 4
		add	esi, 4
		mov	Colors, eax

		call	VR4OneDecode4

		mov	eax, Colors
		add	edi, 4
		add	eax, 4
		add	esi, 4
		mov	Colors, eax

		call	VR4OneDecode4

		mov	eax, Colors
		add	esi, 4
		add	eax, 4
		mov	Bits, esi
		mov	Colors, eax

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR4OneDecode8::

	ofs = 0
	ofd = 0

	REPEAT 2

		mov	edx, [esi+ofs]

	f = 0

	REPEAT 4

	IF f EQ 0
		mov	ebp, [esi+ofs]
		mov	ecx, [esi+ofs]
	ELSE
		mov	ebp, edx
		mov	ecx, edx
	ENDIF
		shr	ebp, 16
		and	ecx, 7
		and	ebp, 3
		add	ecx, Colors
		add	ebp, Colors
		shr	edx, 3
		mov	al, [ecx]
		mov	bl, [ebp]
		ror	eax, 8
		ror	ebx, 8

	f = 1

	ENDM

		mov	[edi+ofd], eax
		mov	[edi+ofd+SCANLINE], eax

	ofd = ofd + SCANLINE*2
	ofs = ofs + 3

	ENDM

		ret

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

VR4Decode8::

		mov	esi, Bits
		mov	edi, NewScr

		call	VR4OneDecode8

		mov	eax, Colors
		add	edi, 4
		add	eax, 8
		add	esi, 6
		mov	Colors, eax

		call	VR4OneDecode8

		mov	eax, Colors
		add	edi, SCANLINE*4-4
		add	eax, 8
		add	esi, 6
		mov	Colors, eax

		call	VR4OneDecode8

		mov	eax, Colors
		add	edi, 4
		add	eax, 8
		add	esi, 6
		mov	Colors, eax

		call	VR4OneDecode8

		mov	eax, Colors
		add	esi, 6
		add	eax, 8
		mov	Bits, esi
		mov	Colors, eax

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

CrossOneDecode::

		mov	edx, [esi]
		mov	ecx, [esi]
		ror	edx, 16

		mov	ebx, ecx
		mov	eax, ecx
		mov	bl, ch
		shr	ebp, 1
		mov	ah, cl
		jnc	ok1
		mov	al, ch
ok1:
		ror	ebx, 8
		ror	eax, 16
		mov	bl, cl
		mov	al, cl
		shr	ebp, 1
		jc	ok2
		mov	ah, cl
ok2:
		shr	ebp, 1
		mov	bh, cl
		jnc	ok3
    		mov	bl, ch
ok3:
		shr	ebp, 1
		jnc	ok4
		mov	bh, dh
ok4:
		rol	eax, 16
		rol	ebx, 8
		mov	[edi+SCANLINE*0], eax
		mov	[edi+SCANLINE*1], ebx

		mov	ebx, ecx
		mov	eax, ecx
		mov	bl, ch
		mov	al, ch
		mov	bh, dl
		ror	eax, 8
		rol	ebx, 8
		mov	al, ch
		shr	ebp, 1
		mov	ah, dl
		jnc	ok5
    		mov	al, dl
ok5:
		shr	ebp, 1
		jnc	ok6
		mov	ah, dh
ok6:

		shr	ebp, 1
		jnc	ok7
		mov	bh, dl
ok7:
		shr	ebp, 1
		jc	ok8
		mov	bl, dl
ok8:
		rol	eax, 8
		ror	ebx, 8
		mov	[edi+SCANLINE*2], eax
		mov	[edi+SCANLINE*3], ebx

		ret

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

CrossDecode::

		mov	eax, Bits
		mov	esi, Colors
		add	eax, 4
		mov	edi, NewScr

		mov	ebp, [eax-4]
		mov	Bits, eax

		call CrossOneDecode

		add	esi, 4
		add	edi, 4

		call CrossOneDecode

		add	esi, 4
		add	edi, SCANLINE*4-4

		call CrossOneDecode

		add	esi, 4
		add	edi, 4


		call CrossOneDecode

		add	esi, 4
		mov	Colors, esi

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

PrimeDecode::

		mov	ebp, Colors	; ebp -> Colors
		mov	esi, Bits	; esi -> bits
		mov	edi, NewScr	; edi -> dst

		mov	ah, [ebp]	; ah  = Prime Color
		mov	bh, 2		; loop2 counter
		mov	cl, 1		; Mask
		inc	ebp
P_loop2:
		mov	edx, [esi]	; edx = bits
		mov	bl, 4		; loop1 counter
		add	esi, 4
		jmp	P_enter1

	ALIGN	4

P_loop1:	shl	cl, 1
		mov	[edi-1], ah	; write Prime
		jc	P_exit1		; last -> exit
P_enter1:
		inc	edi
		test	dl, cl		; Prime?
		jz	P_loop1

		mov	al, [ebp]	; read color

		shl	cl, 1
		mov	[edi-1], al	; write
		inc	ebp
		jnc	P_enter1	; not last -> keep rolling
P_exit1:
		add	edi, SCANLINE-8	; Next Line
		shr	edx, 8		; Next bit field
		dec	bl		; one less to do
		mov	cl, 1		; Mask
		jnz	P_enter1

		dec	bh		; one less to do
		jnz	P_loop2

		mov	Bits, esi	; updates
		mov	Colors, ebp

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Bank1Decode::

		xor	edx, edx

		mov	esi, Bits	; esi -> bits
		mov	eax, Colors	; eax -> colors
		mov	edi, NewScr	; edi -> dst

		mov	dl, [eax]	; Bank
		mov	ebp, 8		; counter
		inc	eax
		mov	dh, dl
		mov	Colors, eax
		mov	eax, edx
		shl	edx, 16
		or	edx, eax
		mov	Dummy, edx

	ALIGN 4

B1_loop1:
		mov	edx, [esi]
		mov	ecx, [esi]	; quartets (4)
		shr	edx, 16		; quartets (4)
		add	esi, 4

	REPEAT	2

		mov	al, cl
		mov	bl, dl
		mov	ah, cl
		mov	bh, dl
		and	al, 15
		and	bl, 15
		shr	ah, 4
		shr	bh, 4
		shr	ecx, 8
		shr	edx, 8
		ror	eax, 16
		ror	ebx, 16

	ENDM
		or	eax, Dummy
		or	ebx, Dummy
		mov	[edi], eax
		mov	[edi+4], ebx

		add	edi, SCANLINE
		dec	ebp
		jnz	B1_loop1

		mov	Bits, esi

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

Bank2Decode::

		mov	esi, Bits	; esi -> bits
		mov	eax, Colors	; eax -> colors
		mov	edi, NewScr	; edi -> dst

		mov	dl, [eax]	; Bank
		mov	ebp, 8
		inc	eax
		mov	dh, dl
		mov	Colors, eax
		shl	dl, 4
		and	dh, 0F0h
		sub	dl, dh

		mov	Dummy, edx

	ALIGN 4

B2_loop1:
		mov	edx, [esi+2]	; quintets (4)
		mov	ecx, [esi]	; quintets (4)
		shr	edx, 4
		add	esi, 5

	REPEAT	4
	local	b1, b2

		mov	al, cl
		mov	bl, dl
		and	al, 0Fh
		and	bl, 0Fh
		shr	ecx, 5
		jc	b1
		add	al, byte ptr[Dummy]
b1:
		shr	edx, 5
		jc	b2
		add	bl, byte ptr[Dummy]
b2:
		add	al, byte ptr[Dummy+1]
		add	bl, byte ptr[Dummy+1]
		ror	eax, 8
		ror	ebx, 8

	ENDM

		mov	[edi], eax
		mov	[edi+4], ebx

		add	edi, SCANLINE
		dec	ebp
		jnz	B2_loop1

		mov	Bits, esi

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

BlockDecode::

		xor	eax, eax

		mov	ebp, edx

		mov	esi, Bits	; esi -> bits

		sub	ebp, 40

		mov	edi, NewScr	; edi -> dst

		shl	ebp, 8

		mov	ebx, Colors	; ebx -> Colors

		add	ebp, offset Patterns; epb -> Pattern array


	REPEAT 2
	local	loop1, enter1, exit1

		mov	edx, [esi]	; edx = bits
		or	eax, 0FFFC0100h	; loop1 counter(-4) + Mask (1)
		add	esi, 4
		jmp	enter1

	ALIGN	4

loop1:		shl	ah, 1
		mov	[edi+ecx], al	; write last color
		jc	exit1		; last -> exit
enter1:
		add	ebp, 4
		test	dl, ah		; New color?
		mov	ecx, [ebp-4]
		jz	loop1

		mov	al, [ebx]	; read color
		shl	ah, 1
		inc	ebx
		mov	[edi+ecx], al	; write
		jnc	enter1	; not last -> keep rolling
exit1:
		mov	ah, 1		; Mask
		shr	edx, 8		; Next bit field
		add	eax, 10000h	; one less to do
		jnc	enter1

	ENDM

		mov	Bits, esi	; updates
		mov	Colors, ebx

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

	ALIGN 4

BlockBank1Decode::

		xor	ecx, ecx
		mov	ebx, Colors	; ebx -> Colors
		mov	ebp, edx
		xor	eax, eax
		sub	ebp, 44
		mov	cl, [ebx]
		shl	ebp, 8
		shl	cl, 4
		mov	edi, NewScr	; edi -> dst
		mov	Dummy, ecx
		add	ebp, offset Patterns; epb -> Pattern array
		xor	cl, cl

	REPEAT 2
	local	loop1, enter1, exit1

		mov	edx, Bits	; edx -> bits
		add	edx, 4
		or	eax, 0FFFC0100h	; loop1 counter(-4) + Mask (1)
		mov	Bits, edx	; updates
		mov	edx, [edx-4]	; edx = bits
		jmp	enter1

	ALIGN	4

loop1:		shl	ah, 1
		mov	[edi+esi], al	; write last color
		jc	exit1		; last -> exit
enter1:
		add	ebp, 4
		test	dl, ah		; New color?
		mov	esi, [ebp-4]
		jz	loop1

		xor	cl, 4
		mov	al, [ebx]	; read color
		shr	al, cl
		shr	cl, 2
		and	al, 0Fh
		add	ebx, ecx
		add	eax, Dummy
		shl	cl, 2
		shl	ah, 1
		mov	[edi+esi], al	; write
		jnc	enter1		; not last -> keep rolling
exit1:
		mov	ah, 1		; Mask
		shr	edx, 8		; Next bit field
		add	eax, 10000h	; one less to do
		jnc	enter1

	ENDM
		xor	cl, 4
		shr	cl, 2
		add	ebx, ecx
		mov	Colors, ebx

		jmp	Return

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

Decompressor	ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

InitDecompressor PROC	uses ebx esi edi ebp

		xor	eax, eax
		xor	esi, esi
		xor	edi, edi
		mov	ebp, -1

	ALIGN 4

loop1:
		mov	ebx, eax
		mov	ecx, eax
		mov	edx, eax
		shr	ebx, 3
		shr	ecx, 3
		and	edx, 7
		shl	ebx, SHIFT
		shl	ecx, SHIFT+2
		add	edx, ebx
		add	edx, ecx
		mov	Patterns[eax*4], edx

		mov	ebx, eax
		mov	ecx, eax
		mov	edx, eax
		and	ebx, 7
		and	ecx, 7
		shr	edx, 3
		shl	ebx, SHIFT
		shl	ecx, SHIFT+2
		add	edx, ebx
		add	edx, ecx
		mov	Patterns[256+eax*4], edx

		mov	ebx, esi
		mov	ecx, esi
		mov	edx, edi
		shl	ebx, SHIFT
		shl	ecx, SHIFT+2
		add	edx, ebx
		add	edx, ecx
		mov	Patterns[512+eax*4], edx

		mov	edx, edi
		add	ebx, ecx
		xor	edx, 7
		add	ebx, edx
		mov	Patterns[768+eax*4], ebx

		inc	eax

		add	esi, ebp
		jl	error1
		sub	edi, ebp
		jl	error2
		cmp	esi, 7
		jg	error3
		cmp	edi, 7
		jg	error4

		cmp	eax, 64
		jb	loop1
		ret
error1:
		sub	edi, ebp
		xor	esi, esi
		jmp	end2
error2:
		cmp	esi, 7
		jg	error5
		xor	edi, edi
		jmp	end2
error3:
		add	edi, 2
		jmp	end1
error4:
		mov	edi, 7
		add	esi, 2
		jmp	end2
error5:
		mov	edi, 1
end1:
		mov	esi, 7
end2:
		neg	ebp
		cmp	eax, 64
		jb	loop1

		ret

InitDecompressor ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

		END
