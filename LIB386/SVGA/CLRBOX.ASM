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
		public	SYSCALL	ClearColor

ClearColor	dq	0

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		public	SYSCALL	ClearBox

ClearBox	dd	offset ClearBoxInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_DATA		ENDS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_TEXT		SEGMENT	USE32 PARA PUBLIC 'CODE'
       		ASSUME	CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	ClearBoxI:PROC
		extrn	SYSCALL	ClearBoxF:PROC
		extrn	SYSCALL	ClearBoxM:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		extrn	SYSCALL	Chooser:PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	ClearBox(void *dst, U32 *TabOffDst, T_BOX *box) ;

;#pragma aux ClearBox	       	       	\
;	parm	[edi] [edx] [ebx]	\
;	modify	[eax ecx esi]

ClearBoxInit	proc

		mov	[Chooser_RtnI], offset ClearBoxI	; Integer
		mov	[Chooser_RtnF], offset ClearBoxF	; FPU
		mov	[Chooser_RtnM], offset ClearBoxM	; MMX
		mov	[Chooser_Result], offset ClearBox	; Pointer
		jmp	Chooser

ClearBoxInit	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	SetClearColor(U32 color)	;

;#pragma aux SetClearColor		\
;	parm	[eax]			\
;	modify	[ecx edx]

SetClearColor	proc

		and	eax, 0FFh	; 000C

		mov	edx, eax	; 000C

		shl	edx, 8		; 00C0

		mov	ecx, edx	; 00C0
		or	edx, eax	; 00CC

		shl	edx, 16		; CC00
		or	eax, ecx	; 00CC

		or	eax, edx	; CCCC

		mov	dword ptr[ClearColor], eax
		mov	dword ptr[ClearColor+4], eax

		ret

SetClearColor	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_TEXT		ENDS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
