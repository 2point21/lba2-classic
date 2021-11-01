;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

			.386p

			.model SMALL, SYSCALL

			include mat.inc
			INCLUDE	IAMMX.INC		; Pour pouvoir assembler du MMx

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
			.DATA

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
			extrn SYSCALL	MMX_DEMI:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
			.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
			public SYSCALL	MulMatrixM


;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*


COMMENT \
How to make a matrix*vector product (16 bit elements):
; MM5 = Line 1
; MM6 = Line 2
; MM7 = Line 3

		movq	mm0,[esi]	; MM0 = 0000 n13  n12  n11
		movq	mm1,mm0		; MM1 = 0000 n13  n12  n11
		movq	mm2,mm1		; MM2 = 0000 n13  n12  n11

		pmaddwd	mm0,mm5		; MM0 = n13*m31  n11*m11+n12*m21
		pmaddwd	mm1,mm6		; MM1 = n13*m32  n11*m12+n12*m22
		pmaddwd	mm2,mm7		; MM2 = n13*m33  n11*m13+n12*m23

		movq	mm3,mm0		; MM3 = n13*m31  n11*m11+n12*m21
		movq	mm4,mm2		; MM4 = n13*m33  n11*m13+n12*m23

		punpckldq mm0,mm1	; MM0 = x12(x,y) x11(x,y)
		punpckhdq mm3,mm1	; MM3 = x12(z)   x11(z)

		paddd	mm3,mm0		; MM3 = (x12      x11)shl 14
		psrad	mm3,14		; MM3 =  x12      x11

		psllq	mm4,32		; MM4 = x13(x,y) 0000
		paddd	mm4,mm2		; MM4 = x13      n11*m13+n12*m23
		psrlq	mm4,32		; MM4 = 0000     x13 shl 14
		psrad	mm4,14		; MM4 = 0000     x13

		packssdw mm3,mm4	; MM3 = 0000 x13  x12  x11

		movq	[edi],mm3

\



;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	MulMAtrixM(S32 *MatDst, S32 *MatSrc1, S32* MatSrc2) ;

;#pragma aux MulMAtrixM		       		\
;	parm		[edi] [esi] [ebx]	\
;	modify exact	[]

; PLEASE DO NOT MODIFY ANY IA REGISTER, OR HAVE A LOOK AT "IMATSTDM.ASM"
; BCOZ registers are assumed not changed...

MulMatrixM	PROC

		push	ebp
		mov	ebp,esp

			; *** Transpose second matrix ***

		movq	mm5,[ebx]	; MM5 = 0000 m13  m12  m11
		pxor	mm0,mm0		; MM0 = 0000 0000 0000 0000

		movq	mm6,[ebx+8]	; MM6 = 0000 m23  m22  m21
		movq	mm7,mm5		; MM7 = 0000 m13  m12  m11

		movq	mm4,[ebx+16]	; MM4 = 0000 m33  m32  m31
		movq	mm3,mm6		; MM3 = 0000 m23  m22  m21

		punpcklwd mm5,mm6	; MM5 = m22  m12  m21  m11
		sub	esp,8*3

		movq	mm2,mm4		; MM2 = 0000 m33  m32  m31
		and	esp,not 7	; Create an 8 bytes aligned scratch-pad

		punpcklwd mm4,mm0	; MM4 = 0000 m32  0000 m31
		movq	mm6,mm5		; MM6 = m22  m12  m21  m11

		movq	mm0,[esi]	; MM0 = 0000 n13  n12  n11
		punpckhwd mm7,mm3	; MM7 = 0000 0000 m23  m13

		movq	mm1,mm0		; MM1 = 0000 n13  n12  n11
		psrlq	mm2,32		; MM2 = 0000 0000 0000 m33

		punpckldq mm7,mm2	; MM7 = 0000 m33  m23  m13
		movq	mm2,mm0		; MM2 = 0000 n13  n12  n11

		punpckldq mm5,mm4	; MM5 = 0000 m31  m21  m11
		pmaddwd	mm2,mm7		; MM2 = n13*m33  n11*m13+n12*m23

		punpckhdq mm6,mm4	; MM6 = 0000 m32  m22  m12
		pmaddwd	mm0,mm5		; MM0 = n13*m31  n11*m11+n12*m21

		movq	[esp],mm5	; [ESP+0] = 0000 m31  m21  m11
		pmaddwd	mm1,mm6		; MM1 = n13*m32  n11*m12+n12*m22

		movq	[esp+8],mm6	; [ESP+8]  = 0000 m32  m22  m12
		movq	mm4,mm2		; MM4 = n13*m33  n11*m13+n12*m23

		movq	[esp+16],mm7	; [ESP+16] = 0000 m33  m23  m13
		movq	mm3,mm0		; MM3 = n13*m31  n11*m11+n12*m21

		movq	mm5,[esi+8]	; MM5 = 0000 n13  n12  n11
		punpckldq mm0,mm1	; MM0 = x12(x,y) x11(x,y)

		movq	mm7,mm5		; MM7 = 0000 n13  n12  n11
		punpckhdq mm3,mm1	; MM3 = x12(z)   x11(z)

		paddd	mm3,mm0		; MM3 = (x12      x11)shl 14
		psllq	mm4,32		; MM4 = x13(x,y) 0000

		paddd	mm3,[MMX_DEMI]
		paddd	mm4,mm2		; MM4 = x13      n11*m13+n12*m23

		paddd	mm4,[MMX_DEMI]
		psrad	mm3,14		; MM3 =  x12      x11

		pmaddwd	mm5,[esp]	; MM5 = n13*m31  n11*m11+n12*m21
		psrlq	mm4,32		; MM4 = 0000     x13 shl 14

		movq	mm6,mm7		; MM7 = 0000 n13  n12  n11
		psrad	mm4,14		; MM4 = 0000     x13

		pmaddwd	mm7,[esp+16]	; MM7 = n13*m33  n11*m13+n12*m23
		packssdw mm3,mm4	; MM3 = 0000 x13  x12  x11

		pmaddwd	mm6,[esp+8]	; MM6 = n13*m32  n11*m12+n12*m22
		movq	mm0,mm5		; MM0 = n13*m31  n11*m11+n12*m21

		movq	[edi],mm3
		punpckldq mm5,mm6	; MM5 = x12(x,y) x11(x,y)

		movq	mm2,[esi+16]	; mm2 = 0000 n13  n12  n11
		movq	mm1,mm7		; mm1 = n13*m33  n11*m13+n12*m23

		movq	mm4,mm2		; mm4 = 0000 n13  n12  n11
		punpckhdq mm0,mm6	; mm0 = x12(z)   x11(z)

		pmaddwd	mm2,[esp]	; mm2 = n13*m31  n11*m11+n12*m21
		movq	mm3,mm4		; mm3 = 0000 n13  n12  n11

		pmaddwd	mm4,[esp+16]	; mm4 = n13*m33  n11*m13+n12*m23
		paddd	mm0,mm5		; mm0 = (x12      x11)shl 14

		psllq	mm1,32		; mm1 = x13(x,y) 0000
		movq	mm6,mm2		; mm6 = n13*m31  n11*m11+n12*m21

		paddd	mm0,[MMX_DEMI]
		paddd	mm1,mm7		; mm1 = x13      n11*m13+n12*m23

		paddd	mm1,[MMX_DEMI]
		psrad	mm0,14		; mm0 =  x12      x11

		pmaddwd	mm3,[esp+8]	; mm3 = n13*m32  n11*m12+n12*m22
		psrlq	mm1,32		; mm1 = 0000     x13 shl 14

		psrad	mm1,14		; mm1 = 0000     x13
		movq	mm7,mm4		; mm7 = n13*m33  n11*m13+n12*m23

		packssdw mm0,mm1	; mm0 = 0000 x13  x12  x11
		pxor	mm1, mm1

		movq	[edi+8],mm0
		punpckldq mm2,mm3	; mm2 = x12(x,y) x11(x,y)

		movq	[edi+24],mm1	; clear trans word
		punpckhdq mm6,mm3	; mm6 = x12(z)   x11(z)

		psllq	mm7,32		; mm7 = x13(x,y) 0000
		paddd	mm6,mm2		; mm6 = (x12      x11)shl 14

		paddd	mm6,[MMX_DEMI]
		paddd	mm7,mm4		; mm7 = x13      n11*m13+n12*m23

		paddd	mm7,[MMX_DEMI]
		psrad	mm6,14		; mm6 =  x12      x11

		psrlq	mm7,32		; mm7 = 0000     x13 shl 14
		mov	esp,ebp

		movq	[edi+32],mm1	; clear trans long (X,Y)
		psrad	mm7,14		; mm7 = 0000     x13

		movq	[edi+40],mm1	; clear trans long (Z)
		packssdw mm6,mm7	; mm6 = 0000 x13  x12  x11

		movq	[edi+16],mm6

		pop	ebp

		ret

MulMatrixM	ENDP

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		End
