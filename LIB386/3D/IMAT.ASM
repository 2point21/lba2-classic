;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p

		.model SMALL, SYSCALL

		include	3d.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.DATA

		public	SYSCALL InitMatrix
InitMatrix	dd	offset InitMatrixInitStd

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		extrn	SYSCALL	InitMatrixStd:DWORD
		extrn	SYSCALL	InitMatrix3DS:DWORD

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.CODE

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		public	SYSCALL	MatrixStd
		public	SYSCALL	Matrix3DS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
InitMatrixInitStd proc

		call	[InitMatrixStd]

		push	eax
		mov	eax, [InitMatrixStd]

		mov	[InitMatrix], eax
		pop	eax

		ret

InitMatrixInitStd endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
InitMatrixInit3DS proc

		call	[InitMatrix3DS]

		push	eax
		mov	eax, [InitMatrix3DS]

		mov	[InitMatrix], eax
		pop	eax

		ret

InitMatrixInit3DS endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;extern  void    MatrixS3td();

;#pragma aux MatrixStd   "*"     \
;        modify exact []

MatrixStd	proc

		mov	[InitMatrix], offset InitMatrixInitStd
		ret

MatrixStd	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;extern  void    Matrix3DS();

;#pragma aux Matrix3DS   "*"     \
;        modify exact []

Matrix3DS	proc

		mov	[InitMatrix], offset InitMatrixInitStd
		ret

Matrix3DS	endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
