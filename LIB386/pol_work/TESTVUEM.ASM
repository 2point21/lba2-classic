			.386
			INCLUDE	FILLER.INC
			INCLUDE	IAMMX.INC

			.model SMALL, SYSCALL

			.DATA

			.CODE


			public	SYSCALL	TestVuePolyM

; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ TestVisible : Tests the visibility of a poly                    บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Call: ESI = Pointer on an array of STRUC_POINT                  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Return: EAX = TRUE if polygon visible                           บ
; บ                                                                 บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

TestVuePolyM		PROC	; Pentium 13
;(x2-x1)*(y1-y3)-(y2-y1)*(x1-x3)
;   P1  *  P2   -   P3  *  P4

			movdt		mm0, [esi]	; .  . Y0 X0

			movdt		mm1, [esi+16]	; .  . Y1 X1
			movq		mm3, mm0

			movdt		mm2, [esi+32]	; .  . Y2 X2
			punpcklwd	mm0, mm1	; MM0 = Y1 Y0 X1 X0

			punpcklwd	mm3, mm2	; MM3 = Y2 Y0 X2 X0
			movq		mm2, mm0	; MM2 = Y1 Y0 X1 X0

			punpckhwd	mm0, mm0	; MM0 = Y1 Y1 Y0 Y0
			movq		mm1, mm3	; MM1 = Y2 Y0 X2 X0

			punpckhwd	mm3, mm3	; MM3 = Y2 Y2 Y0 Y0
			movq		mm4, mm2	; MM4 = Y1 Y0 X1 X0

			psrlq		mm2, 32		; MM2 =  0  0 Y1 Y0
			movq		mm5, mm1	; MM5 = Y2 Y0 X2 X0

			psrlq		mm1, 32		; MM1 =  0  0 Y2 Y0

			psllq		mm2, 16		; MM2 =  0 Y1 Y0  0

			psllq		mm1, 16		; MM1 =  0 Y2 Y0  0
			psubw		mm0, mm2	; MM0 = Y1   0   0 Y0

			punpckldq	mm4, mm4	; MM4 = X1  X0  X1 X0
			psubw		mm3, mm1	; MM3 = Y2   0   0 Y0

			punpckldq	mm5, mm5	; MM5 = X2  X0  X2 X0
			psubw		mm0, mm2	; MM0 = Y1 -Y1 -Y0 Y0

			psubw		mm3, mm1	; MM3 = Y2 -Y2 -Y0 Y0
			pmaddwd		mm0, mm5	; MM0 = Y1(X2-X0) Y0(X0-X2)

			pmaddwd		mm3, mm4	; MM3 = Y2(X1-X0) Y0(X0-X1)
			pxor		mm2, mm2

			psubd		mm0, mm3	; MM0 = Y1(X2-X0)-Y2(X1-X0) Y0(X0-X2)-Y0(X0-X1)

			movq		mm1, mm0	; MM1 = .  Y0(X0-X2)-Y0(X0-X1)
			psrlq		mm0, 32		; MM0 = 0  Y1(X2-X0)-Y2(X1-X0)

							; MM1 = . -Y0(X2-X0)+Y0(X1-X0)
							; MM0 = 0  Y1(X2-X0)-Y2(X1-X0)

			paddd		mm0, mm1	; MM0 = . (Y1-Y0)*(X2-X0)-(Y2-Y0)*(X1-X0)

			pcmpgtd		mm0, mm2

			psrld		mm0, 31

			movdf		eax, mm0

			ret

TestVuePolyM		ENDP

;			The
			End
