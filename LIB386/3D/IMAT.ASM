;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		public	SYSCALL InitMatrix
InitMatrix	dd	offset InitMatrixInitStd

;*══════════════════════════════════════════════════════════════════════════*

		extrn	SYSCALL	InitMatrixStd:DWORD
		extrn	SYSCALL	InitMatrix3DS:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

;*══════════════════════════════════════════════════════════════════════════*

		public	SYSCALL	MatrixStd
		public	SYSCALL	Matrix3DS

;*══════════════════════════════════════════════════════════════════════════*
InitMatrixInitStd proc

		call	[InitMatrixStd]

		push	eax
		mov	eax, [InitMatrixStd]

		mov	[InitMatrix], eax
		pop	eax

		ret

InitMatrixInitStd endp

;*══════════════════════════════════════════════════════════════════════════*
InitMatrixInit3DS proc

		call	[InitMatrix3DS]

		push	eax
		mov	eax, [InitMatrix3DS]

		mov	[InitMatrix], eax
		pop	eax

		ret

InitMatrixInit3DS endp

;*══════════════════════════════════════════════════════════════════════════*
;extern  void    MatrixS3td();

;#pragma aux MatrixStd   "*"     \
;        modify exact []

MatrixStd	proc

		mov	[InitMatrix], offset InitMatrixInitStd
		ret

MatrixStd	endp

;*══════════════════════════════════════════════════════════════════════════*
;extern  void    Matrix3DS();

;#pragma aux Matrix3DS   "*"     \
;        modify exact []

Matrix3DS	proc

		mov	[InitMatrix], offset InitMatrixInitStd
		ret

Matrix3DS	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
