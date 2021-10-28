;*══════════════════════════════════════════════════════════════════════════*
			.386p

			.model SMALL, SYSCALL

			INCLUDE	IAMMX.INC		; Pour pouvoir assembler du MMx
			include mat.inc

;*══════════════════════════════════════════════════════════════════════════*
			.DATA

;*══════════════════════════════════════════════════════════════════════════*
			Extrn	CameraXLight		:DWORD
			Extrn	CameraYLight		:DWORD
			Extrn	CameraZLight		:DWORD

			Extrn	X0			:DWORD
			Extrn	Y0			:DWORD
			Extrn	Z0			:DWORD

;*══════════════════════════════════════════════════════════════════════════*
			.CODE

;*══════════════════════════════════════════════════════════════════════════*
			Extrn	InverseRotatePointM	:PROC

;*══════════════════════════════════════════════════════════════════════════*
			PUBLIC	LightListM

;*══════════════════════════════════════════════════════════════════════════*
;void	LightListM(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LightListM	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

; ╔════════════════════════════════════════════════════════════════════╗
; ║ ListLight: Calculate the light at each point of the list of vect.  ║
; ╟────────────────────────────────────────────────────────────────────╢
; ║ Call: EBX = Rotation matrix                                        ║
; ║       ECX = Number of lights to calculated                         ║
; ║       ESI = List of vectors (which light has to be calculated)     ║
; ║       EDI = List of light (WORDs) to be filled                     ║
; ║                                                                    ║
; ╟────────────────────────────────────────────────────────────────────╢
; ║ Return: Each calculated light is in the form 8.8                   ║
; ║       EDI = Pointer of the next entry in the light list            ║
; ║                                                                    ║
; ╚════════════════════════════════════════════════════════════════════╝


LightListM		PROC
			test	ecx,ecx
			je	@@Exit

			push	ecx
			push	esi

			push	edi
			mov	esi,ebx

			mov	eax,[CameraXLight]
			mov	ebx,[CameraYLight]

			mov	ecx,[CameraZLight]
			call	InverseRotatePointM	; Calculate the rotated light vector

			pop	edi
			pop	esi

			pop	ecx
			mov	eax,[X0]		; Creates the vector

			mov	ebx,[Y0]		; for the dot-product
			and	eax,0FFFFh

			shl	ebx,16
			mov	edx,[Z0]

			or	eax,ebx
			mov	ebx,ecx			; Save loop counter en EBX

			and	ecx,not 11b		; Main loop handles 4 points at a time
			and	edx,0FFFFh

			movdt	mm7,eax			; MM7 = | 0000 | Y | X |

			lea	edi,[edi+ecx*2]		; EDI = End of dst list for the main loop
							;       (ie: not obviously the end of the list)
			add	ecx,ecx

			movdt	mm6,edx

			lea	esi,[esi+ecx*4]		; ECX*4 => *32
			mov	eax, 80008000h

			movdt	mm2, eax		; mm2 = 0000000080008000h
			punpckldq mm7,mm6		; vector in MM7

			punpckldq mm2, mm2		; mm2 = 8000800080008000h
			xor	ecx,-1

			inc	ecx
			jz	@@Less_4

			mov	edx, esp		; Save ESP in EDX
			and	esp, NOT 7		; Scratch-pad in ESP
							; esp = | 00000 | FFFF0000|

							; MM7 = | 0 | z'| y'| x'|
;			movq	mm0,[esi]		; MM0 = | ? | z | y | x |
;			pmaddwd	mm0,mm7			; MM0 = | 0+zz' | xx'+yy' |
;			movq	mm6,mm0			; MM6 = | 0+zz' | xx'+yy' |
;			psrlq	mm0,32			; MM0 = |   0   |  0 +zz' |
;			paddd	mm0,mm6			; MM0 = |   0   | dot_pro |
;			psrad	mm0,16			; MM0 = |   0   |  Light  |
;			      OR
;			pand	mm0,[esp]		; MM0 = |   0   |  Light  |


			movq	mm6,[esi+ecx*4]		; 1	0 Z Y X

			movq	mm5,[esi+ecx*4+8]	; 2	0 Z Y X
			pmaddwd	mm6,mm7			; 1	 Z' XY'

			movq	mm4,[esi+ecx*4+16]	; 3     0 Z Y X
			pmaddwd	mm5,mm7			; 2      Z' XY'

			movq	mm3,[esi+ecx*4+24]	; 4     0 Z Y X
			pmaddwd	mm4,mm7			; 3      Z' XY'

			push	0			; Push Mask 0000 0000 FFFF 0000

			push	0FFFF0000h
			jmp	@@Enter
							; Flow
@@Loop:
			movq	mm6,[esi+ecx*4]		; 1	0 Z Y X
			paddd	mm3,mm5			; 4	 Z'  L

			pand	mm3,[esp]		; 4	0 0 L 0
			psrld	mm4,16			; 3	0 . 0 L

			por	mm0,mm1			; 	0 . L L
			por	mm4,mm3			;       0 . L L

			movq	mm5,[esi+ecx*4+8]	; 2	0 Z Y X
			punpckldq mm0,mm4		;       L L L L

			movq	mm4,[esi+ecx*4+16]	; 3	0 Z Y X
			paddw	mm0, mm2		; Unsigned saturation

			psubusw	mm0, mm2		; is performed by those two instr.
			pmaddwd	mm6,mm7			; 1      Z' XY'

			movq	mm3,[esi+ecx*4+24]	; 4	0 Z Y X
			pmaddwd	mm5,mm7			; 2      Z' XY'

			movq	[edi+ecx-8],mm0
			pmaddwd	mm4,mm7			; 3      Z' XY'
@@Enter:
			movq	mm0,mm6			; 1      Z' XY'
			pmaddwd	mm3,mm7			; 4      Z' XY'

			psrlq	mm0,32			; 1	 0   Z'
			movq	mm1,mm5			; 2      Z' XY'

			paddd	mm0,mm6			; 1	 Z'  L
			psrlq	mm1,32			; 2	 0   Z'

			psrld	mm0,16			; 1	0 . 0 L
			paddd	mm1,mm5			; 2	 Z'  L

			pand	mm1,[esp]		; 1	0 0 L 0
			movq	mm6,mm4			; 3      Z' XY'

			psrlq	mm4,32			; 3	 0   Z'
			movq	mm5,mm3			; 4      Z' XY'

			paddd	mm4,mm6			; 3	 Z'  L
			psrlq	mm3,32			; 4	 0   Z'

			add	ecx,8
			jnz	@@Loop



						; *** End ***
			paddd	mm3,mm5			; 4	 Z'  L
			por	mm0,mm1			;  	0 . L L

			pand	mm3,[esp]		; 4	0 0 L 0
			psrld	mm4,16			; 3	0 . 0 L

			por	mm4,mm3			;  	0 . L L
			mov	esp, edx

			punpckldq mm0,mm4		;       L L L L

			paddw	mm0, mm2                ; Unsigned saturation

			psubusw	mm0, mm2                ; is performed by those two instr.

			movq	[edi-8],mm0

			and	ebx,3
			je	@@Exit
@@Less_4:
@@Loop_End:
			movq	mm0,[esi]		; MM0 = | ? | z | y | x |

			pmaddwd	mm0,mm7			; MM0 = | 0+zz' | xx'+yy' |
			add	esi,8

			movq	mm6,mm0			; MM1 = | 0+zz' | xx'+yy' |
			add	edi,2

			psrlq	mm0,32			; MM0 = |   0   |  0 +zz' |
			dec	ebx

			paddd	mm0,mm6			; MM0 = |   .   | dot_pro |

			psrad	mm0,16			; MM0 = |   .   |  Light  |

			paddw	mm0, mm2                ; Unsigned saturation

			psubusw	mm0, mm2                ; is performed by those two instr.

			movdf	eax,mm0

			mov	[edi-2],ax
			jne	@@Loop_End
@@Exit:
			ret
LightListM		ENDP

;*══════════════════════════════════════════════════════════════════════════*
;			The
			END

