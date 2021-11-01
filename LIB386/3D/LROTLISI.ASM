;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model FLAT, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn SYSCALL	X0:DWORD
		extrn SYSCALL	Y0:DWORD
		extrn SYSCALL	Z0:DWORD

Mat_11			equ	dword ptr[patch_M11+1]
Mat_12			equ	dword ptr[patch_M12+1]
Mat_13			equ	dword ptr[patch_M13+1]
Mat_21			equ	dword ptr[patch_M21+1]
Mat_22			equ	dword ptr[patch_M22+1]
Mat_23			equ	dword ptr[patch_M23+1]
Mat_31			equ	dword ptr[patch_M31+1]
Mat_32			equ	dword ptr[patch_M32+1]
Mat_33			equ	dword ptr[patch_M33+1]


;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*
IFDEF	_WIN32
		extrn	AllowPatchAndPatch: PROC
ENDIF

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	LongRotateListI

;*══════════════════════════════════════════════════════════════════════════*
IFDEF	_WIN32

LongRotateListIInit	proc

		push	offset LongRotateListI
		push	offset LongRotateListIEnd
		push	offset Patch_Jump

		jmp	AllowPatchAndPatch
Patch_Jump::
		mov	eax, [ebx].MAT_M11

LongRotateListIInit	endp

ENDIF

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotateListI(MTYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LongRotateListI		\
;	parm   	[ebx] [edi] [esi] [ecx]	\
;	modify 	[eax edx]

LongRotateListI	proc

		mov	eax, [ebx].MAT_M11
		mov	edx, [ebx].MAT_M12

IFDEF	_WIN32				; This code is 5 bytes long
x:
		org	LongRotateListI

		jmp	NEAR PTR LongRotateListIInit

		org	x
ENDIF
		test	ecx, ecx
		jz	Exit

		lea	ecx, [ecx+ecx*2]
		mov	[Mat_11], eax

		mov	[Mat_12], edx
		mov	eax, [ebx].MAT_M13

		lea	esi, [esi+ecx*4]
		lea	ecx, [ecx*4]

		add	edi, ecx
		mov	[Mat_13], eax

		neg	ecx

		mov	eax, [ebx].MAT_M21
		mov	edx, [ebx].MAT_M22

		mov	[Mat_21], eax
		mov	[Mat_22], edx

		mov	eax, [ebx].MAT_M23
		mov	edx, [ebx].MAT_M31

		mov	[Mat_23], eax
		mov	[Mat_31], edx

		mov	eax, [ebx].MAT_M32
		mov	edx, [ebx].MAT_M33

		mov	[Mat_32], eax
		mov	[Mat_33], edx

		push	ebp
		jmp	NextPoint
NextPoint:
patch_M11:	mov	eax, 12345678h

		imul	dword ptr[esi+ecx]

		mov     ebx, eax
patch_M12:	mov	eax, 12345678h
		mov     ebp, edx

		imul	dword ptr[esi+ecx+4]

		add     ebx, eax
patch_M13:	mov	eax, 12345678h
		adc     ebp, edx

		imul	dword ptr[esi+ecx+8]

		add     ebx, eax
patch_M21:	mov     eax, 12345678h
		adc     ebp, edx

		imul    dword ptr[esi+ecx]

		shrd    ebx, ebp, 14

		adc	ebx, 0
		mov     ebp, edx
		mov	[edi+ecx], ebx
		mov     ebx, eax
patch_M22:	mov	eax, 12345678h

		imul	dword ptr[esi+ecx+4]

		add     ebx, eax
patch_M23:	mov	eax, 12345678h
		adc     ebp, edx

		imul	dword ptr[esi+ecx+8]

		add     ebx, eax
patch_M31:	mov     eax, 12345678h
		adc     ebp, edx

		imul    dword ptr[esi+ecx]

		shrd    ebx, ebp, 14

		adc	ebx, 0
		mov     ebp, edx
		mov	[edi+ecx+4], ebx
		mov     ebx, eax
patch_M32:	mov	eax, 12345678h

		imul	dword ptr[esi+ecx+4]

		add     ebx, eax
patch_M33:	mov	eax, 12345678h
		adc     ebp, edx

		imul	dword ptr[esi+ecx+8]

		add     eax, ebx

		adc     edx, ebp

		shrd    eax, edx, 14

		adc	eax, 0

		mov	[edi+ecx+8], eax

		add	ecx, 12
		jnz	NextPoint

		pop	ebp
Exit:
		ret
LongRotateListIEnd::

LongRotateListI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END



