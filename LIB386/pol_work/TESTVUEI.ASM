			.386
			INCLUDE	FILLER.INC

			.model SMALL, SYSCALL

			.DATA

			.CODE

			public	SYSCALL	TestVuePolyI

; ╔═════════════════════════════════════════════════════════════════╗
; ║ TestVisible : Tests the visibility of a poly                    ║
; ╟─────────────────────────────────────────────────────────────────╢
; ║ Call: ESI = Pointer on an array of STRUC_POINT                  ║
; ╟─────────────────────────────────────────────────────────────────╢
; ║ Return: EAX = TRUE if polygon visible                           ║
; ║                                                                 ║
; ╚═════════════════════════════════════════════════════════════════╝

TestVuePolyI		PROC

			mov	eax,dword ptr[esi].Pt_XE
			mov	ecx,dword ptr[esi+Size STRUC_POINT].Pt_XE

			mov	ebx,eax
			mov	edx,ecx

			movsx	eax,ax	; eax = X0
			movsx	ecx,cx	; ecx = X1

			sar	ebx,16	; ebx = Y0
			mov	edi,dword ptr[esi+2*(Size STRUC_POINT)].Pt_XE

			sar	edx,16	; edx = Y1
			mov	esi,edi

			movsx	edi,di	; edi = X2

			sar	esi,16	; esi = Y2
			sub	ecx,eax	; ecx = X1-X0

			sub	edi,eax	; edi = X2-X0
			sub	edx,ebx	; edx = Y1-Y0

			sub	esi,ebx	; esi = Y2-Y0
			xor	eax,eax

			imul	edx,edi	; edx = (Y1-Y0)*(X2-X0)

			imul	esi,ecx ; esi = (Y2-Y0)*(X1-X0)

			sub	edx,esi

			setns	al

			ret

TestVuePolyI		ENDP

;			The
			End
