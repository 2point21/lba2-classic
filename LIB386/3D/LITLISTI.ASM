;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			.386p

			.model SMALL, SYSCALL

			include mat.inc

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			.DATA

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			Extrn	CameraXLight		:DWORD
			Extrn	CameraYLight		:DWORD
			Extrn	CameraZLight		:DWORD
			Extrn	X0			:DWORD
			Extrn	Y0			:DWORD
			Extrn	Z0			:DWORD

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			.CODE

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			Extrn	InverseRotatePointI	:PROC

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
			PUBLIC	LightListI

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void	LightListI(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LightListI	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ ListLight: Calculate the light at each point of the list of vect.  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Call: EBX = Rotation matrix                                        บ
; บ       ECX = Number of light to calculated                          บ
; บ       ESI = List of vectors (which light has to be calculated)     บ
; บ       EDI = List of light (WORDs) to be filled                     บ
; บ                                                                    บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Return: Each calculated light is in the form 8.8                   บ
; บ       EDI = Pointer of the next entry in the light list            บ
; บ                                                                    บ
; บ                                                                    บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

LightListI		PROC
			test	ecx,ecx
			je	@@Ret

			push	ebp
			push	ecx

			push	esi
			push	edi

			mov	esi,ebx
			mov	eax,[CameraXLight]

			mov	ebx,[CameraYLight]
			mov	ecx,[CameraZLight]

			call	InverseRotatePointI	; Calculate the rotated light vector

			pop	edi
			pop	esi

			pop	ecx
			mov	ebx,[X0]

			mov	edx,[Y0]
			sub	edi, 2

			jmp	@@Loop

@@Write0:
			mov	word ptr [edi],0

			dec	ecx
			jz	@@Exit
@@Loop:
			movsx	ebp,[esi].VT_X

			imul	ebp,ebx

			movsx	eax,[esi].VT_Y

			imul	eax,edx

			add	ebp,eax
			add	edi, 2

			movsx	eax,[esi].VT_Z

			imul	eax,[Z0]

			add	eax,ebp
			add	esi,Size STRUC_VERTEX

			sar	eax,16			; 4.8 = 4.24 >> 16
			js	@@Write0

			adc	eax, 0
			dec	ecx

			mov	[edi],ax
			jne	@@Loop
@@Exit:
			pop	ebp
			add	edi,2

@@Ret:			ret
LightListI		ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;			The
			End
