;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p
		OPTION	LANGUAGE:SYSCALL
		OPTION	SCOPED

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
DGROUP		GROUP	_DATA

_DATA		SEGMENT USE32 PARA PUBLIC 'DATA'

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	BlitBox

BlitBox	dd	offset BlitBoxInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_DATA		ENDS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_TEXT		SEGMENT	USE32 PARA PUBLIC 'CODE'
       		ASSUME	CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	BlitBoxI:PROC
		extrn	SYSCALL	BlitBoxF:PROC
		extrn	SYSCALL	BlitBoxM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_TEXT		ENDS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
