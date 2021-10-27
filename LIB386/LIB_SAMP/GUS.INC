;*--------------------------------------------------------------------------*

BUFFER_SIZE		equ	1024			; number of samples
							; in half-buffer
LIST_SIZE               equ     50
STRUCT_SIZE             equ     40
SNAP_SIZE		equ	8

SHIFT_SAMPLE		equ	3			; max number of
							; samples mixed

;*--------------------------------------------------------------------------*

EXTERN	SYSCALL		playrate:WORD
EXTERN	SYSCALL		bufferhalf:DWORD
EXTERN	SYSCALL		buffer_dma:DWORD
EXTERN	SYSCALL		current_buffer:DWORD
EXTERN	SYSCALL		DoUpdate:WORD
EXTERN	SYSCALL		Critical:WORD

;*--------------------------------------------------------------------------*

UpdateBuffer		PROTO	SYSCALL

;*--------------------------------------------------------------------------*
