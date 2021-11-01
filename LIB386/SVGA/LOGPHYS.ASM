;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

		.386p
		OPTION	LANGUAGE:SYSCALL
		OPTION	SCOPED

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
MAX_SCREEN_Y_RES	equ	1024

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
DGROUP		GROUP	_DATA

_DATA		SEGMENT USE32 PAGE PUBLIC 'DATA'

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
		PUBLIC	SYSCALL	Phys
		PUBLIC	SYSCALL	Log
		PUBLIC	SYSCALL	Screen
		PUBLIC	SYSCALL	TabOffLine
		PUBLIC	SYSCALL	TabOffPhysLine

		PUBLIC	SYSCALL	ModeResX
		PUBLIC	SYSCALL	ModeResY
		PUBLIC	SYSCALL	ModeMemory

		PUBLIC	SYSCALL	BytesScanLine

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
ALIGN	32					; already aligned

Phys		dd	0
Log		dd	0
Screen		dd	0

ModeResX	dd	0
ModeResY	dd	0
ModeMemory	dd	0

BytesScanLine	dd	0

TabOffLine	dd	MAX_SCREEN_Y_RES dup (?)

TabOffPhysLine	dd	MAX_SCREEN_Y_RES dup (?)

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
_DATA		ENDS

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;		The
		END
