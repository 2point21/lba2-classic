; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ                                                                                                             บ
; บ  Module de copie de blocks, avec ou sans stretching pour les changement de resolutions.                     บ
; บ            et de manip                                                                                      บ
; บ  Par un peu tout le monde chez Adeline :)                                                                   บ
; บ                                                                                                             บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ


YMIN_640x480		EQU	40
DY_IMAGE		EQU	200

;//***************************************************************************
			include	iammx.inc

;//***************************************************************************
			.486
			.MODEL SMALL, SYSCALL


;//***************************************************************************
			.data

;//***************************************************************************
		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

;//***************************************************************************
		extrn	SYSCALL	Log	:DWORD

;//***************************************************************************
		PUBLIC	SYSCALL	CopyFrameDoubleXY

;//***************************************************************************
;void	CopyFrameDoubleXY(void *src)

;#pragma aux CopyFrameDoubleXY	"*"	\
;	parm caller	[esi]		\
;	modify		[eax ebx ecx edx edi]

;//***************************************************************************

CopyFrameDoubleXY	dd	offset CopyFrameDoubleXYInit

;//***************************************************************************
			.code

;//***************************************************************************
		extrn	SYSCALL	Chooser	:PROC

;//***************************************************************************
CopyFrameDoubleXYInit	proc

		mov	[Chooser_RtnI], offset CopyFrameDoubleXYI	; Integer
		mov	[Chooser_RtnM], offset CopyFrameDoubleXYM	; MMX
		mov	[Chooser_Result], offset CopyFrameDoubleXY	; Pointer

		jmp	Chooser

CopyFrameDoubleXYInit	endp

;//***************************************************************************
;void	CopyFrameDoubleXYI(void *src)

;#pragma aux CopyFrameDoubleXYI	"*"	\
;	parm caller	[esi]		\
;	modify		[eax ebx ecx edx edi]

CopyFrameDoubleXYI	PROC	USES ebp

		mov	edi, Log
		mov	ebp, DY_IMAGE

		add	edi, YMIN_640x480*640
		jmp	FirstLine
NextLine:
		add	edi,640
FirstLine:	mov	ecx,320/4
NextQuad:
		mov	edx,[esi]
		add	esi,4

		mov	eax,edx
		mov	ebx,edx

		mov	al,ah
		mov	bh,bl

		shl	eax,16

		shr	edx,16
		mov	ax,bx

		mov	ebx,edx
		mov	[edi],eax

		mov	[edi+640],eax
		mov	dl,dh

		shl	edx,16
		mov	bh,bl

		mov	dx,bx
		dec	ecx

		mov	[edi+4],edx
		mov	[edi+640+4],edx

		lea	edi,[edi+8]
		jnz	NextQuad

		dec	ebp
		jne	NextLine

		ret

CopyFrameDoubleXYI	ENDP

;//***************************************************************************
;void	CopyFrameDoubleXYM(void *src)

;#pragma aux CopyFrameDoubleXYM	"*"	\
;	parm caller	[esi]		\
;	modify		[ecx edx edi]

CopyFrameDoubleXYM	PROC

		mov		edi, Log
		mov		edx, DY_IMAGE

		add		edi, YMIN_640x480*640
		jmp		FirstLine
NextLine:
		add		edi,640
FirstLine:	mov		ecx,320/8
NextQuad:
		movq		mm0,[esi]	; mm0 = 76543210

		add		esi, 8
		dec		ecx

		movq		mm1, mm0	; mm1 = 76543210
		punpcklbw	mm0, mm0	; mm0 =	33221100

		movq		[edi], mm0
		punpckhbw	mm1, mm1	; mm1 =	77665544

		movq		[edi+8], mm1

		movq		[edi+640], mm0

		movq		[edi+648], mm1

		lea		edi,[edi+16]
		jnz		NextQuad

		dec		edx
		jne		NextLine

		ret

CopyFrameDoubleXYM	ENDP

;//***************************************************************************
;			The
			End

