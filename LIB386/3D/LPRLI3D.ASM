;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnMF:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	LongProjectList3D

LongProjectList3D	dd	offset LongProjectList3DInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	LongProjectList3DI:PROC
		extrn	SYSCALL	LongProjectList3DF:PROC
		extrn	SYSCALL	LongProjectList3DMF:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	LongProjectList3D(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux LongProjectList3D			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARING: OrgX, OrgY, OrgZ pushed on the stack

LongProjectList3DInit	proc

		mov	[Chooser_RtnI], offset LongProjectList3DI	; Integer
		mov	[Chooser_RtnF], offset LongProjectList3DF	; FPU
		mov	[Chooser_RtnMF], offset LongProjectList3DMF	; MMX+FPU
		mov	[Chooser_Result], offset LongProjectList3D	; Pointer
		jmp	Chooser

LongProjectList3DInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

