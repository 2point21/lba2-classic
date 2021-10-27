;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD


		public	SYSCALL LightList
LightList	dd	offset LightListInit

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		Extrn	SYSCALL	LightListI	:	PROC
		Extrn	SYSCALL	LightListF	:	PROC
		Extrn	SYSCALL	LightListM	:	PROC

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	Chooser:PROC


;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;void	LightList(TYPE_MAT *Mat, S32 *dst, S32 *src, S32 n)	;

;#pragma aux LightList	"*"		\
;	parm 	[ebx] [edi] [esi] [ecx]	\
;	modify	[eax ebx edx]

LightListInit	proc

		mov	[Chooser_RtnI],   offset LightListI	; Integer
		mov	[Chooser_RtnF],   offset LightListF	; FPU
		mov	[Chooser_RtnM],   offset LightListM	; MMX
		mov	[Chooser_Result], offset LightList	; Pointer
		jmp	Chooser

LightListInit	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END

