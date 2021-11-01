;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

			.586

			.model SMALL, SYSCALL

			include mat.inc

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			.DATA

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			Extrn	CameraXLight		:DWORD
			Extrn	CameraYLight		:DWORD
			Extrn	CameraZLight		:DWORD
			Extrn	FactorLight		:REAL4
			Extrn	X0			:DWORD
			Extrn	Y0			:DWORD
			Extrn	Z0			:DWORD

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			.CODE

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			Extrn	LongInverseRotatePointF	:PROC

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			PUBLIC	LightListF

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void	LightListF(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LightListF	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ ListLight: Calculate the light at each point of the list of vect.  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Call: EBX = Rotation matrix                                        บ
; บ       ECX = Number of lights to calculated                         บ
; บ       ESI = List of vectors (which light has to be calculated)     บ
; บ       EDI = List of light (WORDs) to be filled                     บ
; บ                                                                    บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Return: Each calculated light is in the form 8.8                   บ
; บ       EDI = Pointer of the next entry in the light list            บ
; บ                                                                    บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ


LightListF		PROC
			test	ecx,ecx			; Nothing to do ?
			jz	@@Ret

			push	esi
			push	edi

			push	ecx
			mov	esi,ebx

			mov	eax,[CameraXLight]
			mov	ebx,[CameraYLight]

			mov	ecx,[CameraZLight]
			call	LongInverseRotatePointF	; Calculate the rotated light vector

			pop	ecx
			pop	edi
							; 00  01  02  03  04  05  06  07

			fild	[Z0]			; Z0

			pop	esi
			lea	edi,[edi+ecx*2-2]

			fild	[Y0]			; Y0  Z0

			fild	[X0]			; X0  Y0  Z0

			lea	esi,[esi+ecx*8]
			xor	ecx,-1

			xor	ebx, ebx
			add	ecx, 2

			fmul	[FactorLight]		; X1  Y0  Z0
			fxch	st(1)			; Y0  X1  Z0

			fmul	[FactorLight]		; Y1  X1  Z0
			fxch	st(2)			; Z0  X1  Y1

			fmul	[FactorLight]		; Z1  X1  Y1
			fxch	st(2)			; Y1  X1  Z1

			fstp	[Y0]			; X1  Z1

			fstp	[X0]			; Z1

			fstp	[Z0]			;

							; 00  01  02  03  04  05  06  07
			fild	[esi+ecx*8-8].VT_X	; X
			fild	[esi+ecx*8-8].VT_Y	; Y   X
			fild	[esi+ecx*8-8].VT_Z	; Z   Y   X

			fxch	st(2)			; X   Y   Z

			fmul	[X0]			; X'  Y   Z
			fxch	st(1)			; Y   X'  Z

			fmul	[Y0]			; Y'  X'  Z
			fxch	st(2)			; Z   X'  Y'

			fmul	[Z0]			; Z'  X'  Y'
			fxch	st(1)			; X'  Z'  Y'

			push	ebx			; <=> sub esp,4; but doesn't modify the flags
			jz	@@End1

			fild	[esi+ecx*8].VT_Z	; Z   X'  Z'  Y'
			fild	[esi+ecx*8].VT_Y	; Y   Z   Z'  X'  Z'  Y'
			fild	[esi+ecx*8].VT_X	; X   Y   Z   X'  Z'  Y'

			fxch	st(1)			; Y   X   Z   X'  Z'  Y'

			fmul	[Y0]			; y'  X   Z   X'  Z'  Y'
			fxch	st(5)			; Y'  X   Z   X'  Z'  y'

			faddp	st(4),st		; X   Z   X'  I   y'

			fmul	[X0]			; x'  Z   X'  I   y'
			fxch	st(2)			; X'  Z   x'  I   y'

			faddp	st(3),st		; Z   x'  L   y'

			inc	ecx
			jne	@@Loop
			jmp	@@ExitLoop

@@Loop2:
			mov	[edi+ecx*2-2],ax

			inc	ecx
			jz	@@ExitLoop
							; 00  01  02  03  04  05  06  07
@@Loop:
			fmul	[Z0]			; z'  x'  L   y'
			fxch	st(2)			; L   x'  z'  y'

			fild	[esi+ecx*8].VT_X	; X   L   X'  Z'  Y'

			fild	[esi+ecx*8].VT_Y	; Y   X   L   X'  Z'  Y'

			fild	[esi+ecx*8].VT_Z	; Z   Y   X   L   X'  Z'  Y'

			fxch	st(3)			; L   Y   X   Z   X'  Z'  Y'

			fistp	dword ptr[esp]		; Y   X   Z   X'  Z'  Y'

			fmul	[Y0]			; y'  X   Z   X'  Z'  Y'
			fxch	st(5)			; Y'  X   Z   X'  Z'  y'

			faddp	st(4),st		; X   Z   X'  I   y'

			mov	eax, [esp]

			fmul	[X0]			; x'  Z   X'  I   y'
			fxch	st(2)			; X'  Z   x'  I   y'

			faddp	st(3),st		; Z   x'  L   y'

			test	eax, eax
			jns	@@Loop2

			mov	[edi+ecx*2-2],bx

			inc	ecx
			jnz	@@Loop


@@ExitLoop:
			fmul	[Z0]			; z'  x'  L   y'
			fxch	st(2)			; L   x'  z'  y'

			fistp	dword ptr[esp]		; x'  z'  y'

			faddp	st(2),st		; z'  I

			mov	eax, [esp]

			faddp	st(1),st		; L

			test	eax, eax
			jns	@@Pos_Ok1

			xor	eax, eax
@@Pos_Ok1:
			fistp	dword ptr[esp]

			mov	[edi-2],ax
@@End:			pop	eax

			test	eax, eax
			jns	@@Pos_Ok2

			xor	eax, eax
@@Pos_Ok2:
			mov	[edi],ax
@@Ret:
			ret

@@End1:			faddp	st(2),st		; Z'
			faddp	st(1),st		; L
			fistp	dword ptr[esp]
			jmp	@@End

LightListF		ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		The
		END


