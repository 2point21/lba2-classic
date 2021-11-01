;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public	SYSCALL	InitMatrixTransF

;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrixTransF(S32 *MatDst, S32 tx, S32 ty, S32 tz) ;

;#pragma aux InitMatrixTransF	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[]

InitMatrixTransF	PROC

		push	eax
		push	ebx

		push	ecx

		fild	dword ptr[esp]		; Z

		fild	dword ptr[esp+4]	; Y Z

		fild	dword ptr[esp+8]	; X Y Z

		add	esp, 4*3

		fstp	[edi].MAT_MTX

		fstp	[edi].MAT_MTY

		fstp	[edi].MAT_MTZ

		ret

InitMatrixTransF	ENDP

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
