;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		.386p

		include	mat.inc

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
DGROUP		GROUP	_DATA

_DATA		SEGMENT USE32 PARA PUBLIC 'DATA'

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
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

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_DATA		ENDS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
