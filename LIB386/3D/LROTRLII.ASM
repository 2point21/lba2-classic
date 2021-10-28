
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
Mat_TX			equ	dword ptr[patch_MTX+2]
Mat_TY			equ	dword ptr[patch_MTY+2]
Mat_TZ			equ	dword ptr[patch_MTZ+2]


;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*
IFDEF	_WIN32
		extrn	AllowPatchAndPatch: PROC
ENDIF

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	LongRotTransListI

;*══════════════════════════════════════════════════════════════════════════*
IFDEF	_WIN32

LongRotTransListIInit	proc

		push	offset LongRotTransListI
		push	offset LongRotTransListIEnd
		push	offset Patch_Jump

		jmp	AllowPatchAndPatch
Patch_Jump::
		mov	eax, [ebx].MAT_M11

LongRotTransListIInit	endp

ENDIF

;*══════════════════════════════════════════════════════════════════════════*
;void	LongRotTransListI(MTYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LongRotTransListI		\
;	parm   	[ebx] [edi] [esi] [ecx]	\
;	modify 	[eax edx]

LongRotTransListI	proc

		mov	eax, [ebx].MAT_M11
		mov	edx, [ebx].MAT_M12

IFDEF	_WIN32				; This code is 5 bytes long
x:
		org	LongRotTransListI

		jmp	NEAR PTR LongRotTransListIInit

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
		mov	edx, [ebx].MAT_MTX

		neg	ecx

		mov	[Mat_13], eax
		mov	[Mat_TX], edx

		mov	eax, [ebx].MAT_M21
		mov	edx, [ebx].MAT_M22

		mov	[Mat_21], eax
		mov	[Mat_22], edx

		mov	eax, [ebx].MAT_M23
		mov	edx, [ebx].MAT_MTY

		mov	[Mat_23], eax
		mov	[Mat_TY], edx

		mov	eax, [ebx].MAT_M31
		mov	edx, [ebx].MAT_M32

		mov	[Mat_31], eax
		mov	[Mat_32], edx

		mov	eax, [ebx].MAT_M33
		mov	edx, [ebx].MAT_MTZ

		mov	[Mat_33], eax
		mov	[Mat_TZ], edx

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

		shrd    ebx, ebp, 14

patch_MTX:	adc	ebx, 12345678h

		imul    dword ptr[esi+ecx]

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

		shrd    ebx, ebp, 14

patch_MTY:	adc	ebx, 12345678h

		imul    dword ptr[esi+ecx]

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
patch_MTZ:	adc	eax, 12345678h
		mov	[edi+ecx+8], eax

		add	ecx, 12
		jnz	NextPoint

		pop	ebp
Exit:
		ret

LongRotTransListIEnd::

LongRotTransListI	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END


