		.386
		INCLUDE	FILLER.INC

		.model SMALL, SYSCALL

		.DATA

		.CODE

		public	SYSCALL	TestVuePolyF

; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ TestVisible : Tests the visibility of a poly                    บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Call: ESI = Pointer on an array of STRUC_POINT                  บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Return: EAX = TRUE if polygon visible                           บ
; บ                                                                 บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

TestVuePolyF	PROC
;(x2-x1)*(y1-y3)-(y2-y1)*(x1-x3)
;   P1  *  P2   -   P3  *  P4
							; 0   1   2   3   4   5   6   7
	fild	[esi].Pt_XE				; x1
	fild	[esi].Pt_YE				; y1  x1
	fild	[esi+Size STRUC_POINT].Pt_XE		; x2  y1  x1
	fild	[esi+2*(Size STRUC_POINT)].Pt_YE	; y3  x2  y1  x1
	fild	[esi+Size STRUC_POINT].Pt_YE		; y2  y3  x2  y1  x1
	fild	[esi+2*(Size STRUC_POINT)].Pt_XE	; x3  y2  y3  x2  y1  x1
	fld	st(5)					; x1  x3  y2  y3  x2  y1  x1
	fsubp	st(4),st				; x3  y2  y3  P1  y1  x1
	fld	st(4)					; y1  x3  y2  y3  P1  y1  x1
	fsubrp	st(3),st				; x3  y2  P2  P1  y1  x1
	fsubp	st(5),st				; y2  P2  P1  y1  P4
	fsubrp	st(3),st				; P2  P1  P3  P4
	fmulp	st(1),st				; PP1 P3  P4
	fxch	st(1)					; P3  PP1 P4
	push	eax
	fmulp	st(2),st				; PP1 PP2
	; 2
	fsubrp	st(1),st
	; 2+1
	fstp	dword ptr [esp]

	pop	ecx
	xor	eax,eax

	test	ecx,ecx

	setns	al

	ret

TestVuePolyF	ENDP


;		The
       		End
