;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model FLAT, SYSCALL

		include mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	NextPointRotListI:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
                extrn   SYSCALL patch_MTX       :DWORD
                extrn   SYSCALL patch_MTY       :DWORD
                extrn   SYSCALL patch_MTZ       :DWORD
                extrn   SYSCALL patch_M11       :DWORD
                extrn   SYSCALL patch_M12       :DWORD
                extrn   SYSCALL patch_M13       :DWORD
                extrn   SYSCALL patch_M21       :DWORD
                extrn   SYSCALL patch_M22_M32   :DWORD
                extrn   SYSCALL patch_M23       :DWORD
                extrn   SYSCALL patch_M31       :DWORD
                extrn   SYSCALL patch_M32       :DWORD
                extrn   SYSCALL patch_M33       :DWORD
                extrn   SYSCALL patch_End       :DWORD
                extrn   SYSCALL patch_Delta0    :DWORD
                extrn   SYSCALL patch_Delta1    :DWORD
                extrn   SYSCALL patch_Delta2    :DWORD

                extrn   SYSCALL RotateListI	:DWORD
                extrn   SYSCALL RotateListIEnd	:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public SYSCALL	RotTransListI

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
IFDEF	_WIN32
		extrn  SYSCALL	AllowPatch	:PROC
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

RotTransListIInit	proc

		push	ecx

		mov	eax, offset RotateListI
		mov	edx, offset RotateListIEnd
		call	AllowPatch

		pop	ecx

		push	offset RotTransListI
		push	offset RotTransListIEnd
		push	offset Patch_Jump

		jmp	AllowPatchAndPatch
Patch_Jump::
		sub	esi, edi	; delta src<->tgt
		push	ebp

		push	edi		; target
		mov	[Delta0], esi	; delta+0

RotTransListIInit	endp

ENDIF

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	RotTransListI(MTYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux RotTransListI		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax edx]

RotTransListI	proc

		sub	esi, edi	; delta src<->tgt
		push	ebp

		push	edi		; target
		mov	[Delta0], esi	; delta+0

IFDEF	_WIN32				; This code is 5 bytes long
RotTransListIEnd::
		org	RotTransListI

		jmp	NEAR PTR RotTransListIInit

		org	RotTransListIEnd
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

		mov	eax,[ebx].MAT_MTX
		mov	Mat_11, esi

		shl	eax,14
		mov	Mat_12, edi

		sub	eax,ebp		; TX - M11*M12
		mov	Mat_13, ecx

		add	eax, 1 SHL 13

		mov	[Trans_X], eax
		mov	eax, [ebx].MAT_M32

		mov	esi, [ebx].MAT_M21
		mov	edi, [ebx].MAT_M22

		mov	ebp, esi
		mov	ecx, [ebx].MAT_M23

		imul	ebp, edi	; M21*M22

		sub	edi, eax	; trick. see below
		mov	Mat_21, esi

		mov	edx,[ebx].MAT_MTY
		mov	Mat_22_32, edi

		shl	edx,14
		mov	Mat_23, ecx

		sub	edx,ebp		; TY - M21*M22
		mov	Mat_32, eax

		add	edx, 1 SHL 13

		mov	[Trans_Y],edx
		mov	edx, [ebx].MAT_M31

		mov	ebp, edx
		mov	Mat_31, edx

		imul	ebp, eax	; M31*M32

		mov	eax,[ebx].MAT_MTZ
		mov	ebx, [ebx].MAT_M33

		shl	eax,14
		mov	Mat_33, ebx

		sub	eax,ebp		; TZ - M31*M32
		pop	edi		; target

		add	eax, 1 SHL 13

		mov	[Trans_Z], eax	; - M31*M32
		jmp	NextPointRotListI; Flush prefetch to reload code

RotTransListI	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End

