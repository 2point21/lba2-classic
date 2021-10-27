			.386

PUBLIC	SYSCALL		TestVuePoly


			.model SMALL, SYSCALL

			.DATA
				; Chooser.asm
Extrn	SYSCALL		Chooser_RtnI	:	DWORD
Extrn	SYSCALL		Chooser_RtnF	:	DWORD
Extrn	SYSCALL		Chooser_RtnM	:	DWORD
Extrn	SYSCALL		Chooser_Result	:	DWORD

TestVuePoly		dd	Init_TestVuePoly




			.CODE
Extrn	SYSCALL		Chooser		:	PROC
Extrn	SYSCALL		TestVuePolyI	:	PROC
Extrn	SYSCALL		TestVuePolyF	:	PROC
Extrn	SYSCALL		TestVuePolyM	:	PROC

Init_TestVuePoly	PROC
			mov	[Chooser_RtnI],Offset TestVuePolyI	; Integer
			mov	[Chooser_RtnF],Offset TestVuePolyF	; FPU
			mov	[Chooser_RtnM],Offset TestVuePolyM	; MMX
			mov	[Chooser_Result],Offset TestVuePoly	; Pointer
			jmp	Chooser
Init_TestVuePoly	ENDP


;			The
			End

