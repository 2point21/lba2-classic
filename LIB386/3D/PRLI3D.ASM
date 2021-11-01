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

		public	SYSCALL	ProjectList3D

ProjectList3D	dd	offset ProjectList3DInit

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	ProjectList3DI:PROC
		extrn	SYSCALL	ProjectList3DF:PROC
		extrn	SYSCALL	ProjectList3DMF:PROC

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	Chooser:PROC

;*══════════════════════════════════════════════════════════════════════════*
;void	ProjectList3D(S32 *Dst, S32 *Src, S32 NbPt, S32 OrgX, S32 OrgY, OrgZ);

;#pragma aux ProjectList3D			\
;	parm		[edi] [esi] [ecx]	\
;	modify		[eax ebx edx]

;WARING: OrgX, OrgY, OrgZ pushed on the stack

ProjectList3DInit	proc

		mov	[Chooser_RtnI], offset ProjectList3DI	; Integer
		mov	[Chooser_RtnF], offset ProjectList3DF	; FPU
		mov	[Chooser_RtnMF], offset ProjectList3DMF	; MMX+FPU
		mov	[Chooser_Result], offset ProjectList3D	; Pointer
		jmp	Chooser

ProjectList3DInit	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END

