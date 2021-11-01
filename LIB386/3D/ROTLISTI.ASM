;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model FLAT, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public SYSCALL	RotateListI
		public SYSCALL	NextPointRotListI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
                public SYSCALL	patch_MTX
                public SYSCALL	patch_MTY
                public SYSCALL	patch_MTZ
                public SYSCALL	patch_M11
                public SYSCALL	patch_M12
                public SYSCALL	patch_M13
                public SYSCALL	patch_M21
                public SYSCALL	patch_M22_M32
                public SYSCALL	patch_M23
                public SYSCALL	patch_M31
                public SYSCALL	patch_M32
                public SYSCALL	patch_M33
                public SYSCALL	patch_End
                public SYSCALL	patch_Delta0
                public SYSCALL	patch_Delta1
                public SYSCALL	patch_Delta2
                public SYSCALL	RotateListIEnd

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
IFDEF	_WIN32
		extrn  SYSCALL	AllowPatchAndPatch:PROC
ENDIF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
Trans_X			equ	dword ptr[patch_MTX+3]
Trans_Y			equ	dword ptr[patch_MTY+3]
Trans_Z			equ	dword ptr[patch_MTZ+3]

Mat_11			equ	dword ptr[patch_M11+2]
Mat_12			equ	dword ptr[patch_M12+2]
Mat_13			equ	dword ptr[patch_M13+2]
Mat_21			equ	dword ptr[patch_M21+2]
Mat_22_32		equ	dword ptr[patch_M22_M32+1]	; WARNING trick see code
Mat_23			equ	dword ptr[patch_M23+2]
Mat_31			equ	dword ptr[patch_M31+2]
Mat_32			equ	dword ptr[patch_M32+1]
Mat_33			equ	dword ptr[patch_M33+2]

End_Loop		equ	dword ptr[patch_End+2]

Delta0			equ	dword ptr[patch_Delta0+3]
Delta1			equ	dword ptr[patch_Delta1+3]
Delta2			equ	dword ptr[patch_Delta2+3]

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
IFDEF	_WIN32

RotateListIInit	proc

		push	offset RotateListI
		push	offset RotateListIEnd
		push	offset Patch_Jump

		jmp	AllowPatchAndPatch
Patch_Jump::
		sub	esi, edi	; delta src<->tgt
		push	edi		; target

		push	ebp
		mov	[Delta0], esi	; delta+0

RotateListIInit	endp

ENDIF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotateListI(MTYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux RotateListI		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax edx]

RotateListI	proc

		sub	esi, edi	; delta src<->tgt
		push	ebp

		push	edi		; target
		mov	[Delta0], esi	; delta+0

IFDEF	_WIN32				; This code is 5 bytes long
x:
		org	RotateListI

		jmp	NEAR PTR RotateListIInit

		org	x

ENDIF
		add	esi, 2
		lea	ecx, [ecx*8+edi]; final address

		mov	Delta1, esi	; delta+2
		add	esi, 2

		mov	End_Loop, ecx	; final address for end of loop
		mov	Delta2, esi	; delta+4

		mov	esi, [ebx].MAT_M11
		mov	edi, [ebx].MAT_M12

		mov	ebp, esi
		mov	ecx, [ebx].MAT_M13

		imul	ebp, edi	; M11*M12

		mov	Mat_11, esi
		xor	ebp, -1		; - M11*M12

		mov	Mat_12, edi
		add	ebp, (1 SHL 13)+1

		mov	[Trans_X], ebp	; - M11*M12

		mov	Mat_13, ecx
		mov	eax, [ebx].MAT_M32

		mov	esi, [ebx].MAT_M21
		mov	edi, [ebx].MAT_M22

		mov	ebp, esi
		mov	ecx, [ebx].MAT_M23

		imul	ebp, edi	; M21*M22

		sub	edi, eax	; trick. see below
		mov	Mat_21, esi

		xor	ebp, -1		; - M21*M22
		mov	Mat_22_32, edi

		add	ebp, (1 SHL 13)+1
		mov	Mat_23, ecx

		mov	[Trans_Y], ebp	; - M21*M22
		mov	edx, [ebx].MAT_M31

		mov	ebp, edx
		mov	ebx, [ebx].MAT_M33

		imul	ebp, eax	; M31*M32

		mov	Mat_31, edx
		xor	ebp, -1		; - M31*M32

		mov	Mat_32, eax
		add	ebp, (1 SHL 13)+1

		pop	edi		; target
		mov	[Trans_Z], ebp	; - M31*M32

		mov	Mat_33, ebx
		jmp	NextPointRotListI; Flush prefetch to reload code

NextPointRotListI::

patch_Delta0::	movsx	ebp, word ptr[edi+12345678h]	; X			1

patch_Delta1::	movsx	edx, word ptr[edi+12345678h]	; Y

patch_M12::	lea	ebx, [ebp+12345678h]		; X+12			2
patch_M11::	lea	esi, [edx+12345678h]		; Y+11

		mov	eax, ebp			; X

		imul	ebp, edx			; XY

patch_Delta2::	movsx	ecx, word ptr[edi+12345678h]	; Z			3

		imul	ebx, esi			; (X+12)(Y+11)

		add	edi, 8

patch_M13::	imul	esi, ecx, 12345678h		; Z13			4

							; Penalty 2 cycles on K5

		sub	ebx, ebp			; X11+Y12+12*11		6
patch_M32::	add	eax, 12345678h			; X+32

patch_MTX::	lea	ebx, [ebx+esi+12345678h]	; X11+Y12+Z13+MTX	7
patch_M21::	lea	esi, [edx+12345678h]		; Y+21

patch_M31::	add	edx, 12345678h			; Y+31

		imul	edx, eax			; X31+Y32+32*31+XY	8

patch_M22_M32::	add	eax, 12345678h			; X+22 (+32-32)

		sar	ebx, 14

		imul	eax, esi			; X21+Y22+22*21+XY	9

		mov	[edi-8], ebx

patch_M23::	imul	ebx, ecx, 12345678h		; Z23			10

							; Penalty 1 cycles on K5

		sub	edx, ebp			; X31+Y32+32*31		11

patch_M33::	imul	ecx, 12345678h			; Z33

							; Penalty 1 cycles on K5

		sub	eax, ebp			; X21+Y22+22*21		12

							; Penalty 2 cycles on K5

patch_MTZ::	lea	ebp, [edx+ecx+12345678h]	; X31+Y32+Z33+MTZ	14
patch_MTY::	lea	eax, [eax+ebx+12345678h]	; X21+Y22+Z23+MTY

		sar	ebp, 14				;			15
		sar	eax, 14

patch_End::	cmp	edi, 12345678h			;			16

		mov	[edi-6], ax
		mov	[edi-4], ebp

		jne	NextPointRotListI

		pop	ebp
		ret
RotateListIEnd::

RotateListI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End

