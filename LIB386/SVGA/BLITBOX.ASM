;*══════════════════════════════════════════════════════════════════════════*
		.386p
		OPTION	LANGUAGE:SYSCALL
		OPTION	SCOPED

;*══════════════════════════════════════════════════════════════════════════*
DGROUP		GROUP	_DATA

_DATA		SEGMENT USE32 PARA PUBLIC 'DATA'

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	BlitBox

BlitBox	dd	offset BlitBoxInit

;*══════════════════════════════════════════════════════════════════════════*
_DATA		ENDS

;*══════════════════════════════════════════════════════════════════════════*
_TEXT		SEGMENT	USE32 PARA PUBLIC 'CODE'
       		ASSUME	CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	BlitBoxI:PROC
		extrn	SYSCALL	BlitBoxF:PROC
		extrn	SYSCALL	BlitBoxM:PROC

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	BlitBox(void *dst, U32 *TabOffDst, void *src) ;

;#pragma aux BlitBox	       	       	\
;	parm	[edi] [edx] [esi]	\
;	modify	[eax ebx ecx]

BlitBoxInit	proc

		mov	[Chooser_RtnI], offset BlitBoxI	; Integer
		mov	[Chooser_RtnF], offset BlitBoxF	; FPU
		mov	[Chooser_RtnM], offset BlitBoxM	; MMX
		mov	[Chooser_Result], offset BlitBox	; Pointer
		jmp	Chooser

BlitBoxInit	endp

;*══════════════════════════════════════════════════════════════════════════*
_TEXT		ENDS

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
