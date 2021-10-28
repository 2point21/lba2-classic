;*══════════════════════════════════════════════════════════════════════════*
		.386p

		include	mat.inc

;*══════════════════════════════════════════════════════════════════════════*
DGROUP		GROUP	_DATA

_DATA		SEGMENT USE32 PARA PUBLIC 'DATA'

;*══════════════════════════════════════════════════════════════════════════*
ALIGN 8
		public	SYSCALL	MatriceWorld

MatriceWorld	Struc_MatriceMAT	<>

ALIGN 8
		public	SYSCALL	MatrixLib1

MatrixLib1	Struc_MatriceMAT	<>

ALIGN 8
		public	SYSCALL MatrixLib2

MatrixLib2	Struc_MatriceMAT	<>

ALIGN 8
		public	SYSCALL MatriceRot

MatriceRot	Struc_MatriceMAT	<>

;*══════════════════════════════════════════════════════════════════════════*
_DATA		ENDS

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
