;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public	SYSCALL	GetMatrixTransF

;*══════════════════════════════════════════════════════════════════════════*
;void	GetMatrixTransF(S32 *MatDst, S32 *tx, S32 *ty, S32 *tz) ;

;#pragma aux GetMatrixTransF	"*"		\
;	parm		[esi] [eax] [ebx] [ecx]	\
;	modify exact	[]

GetMatrixTransF	PROC

		fld	[esi].MAT_MTX	; X

		fld	[esi].MAT_MTY	; Y X

		fld	[esi].MAT_MTZ	; Z Y X

		fistp	dword ptr[ecx]	; Y X

		fistp	dword ptr[ebx]	; Y

		fistp	dword ptr[eax]

		ret

GetMatrixTransF	ENDP

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
