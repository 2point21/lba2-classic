;----------------------------------------------------------------------------
			.386p
			.model FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			EXTRN	SYSCALL	ModeResX:DWORD
			EXTRN	SYSCALL	ModeResY:DWORD

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL ClipXMin
			PUBLIC	SYSCALL ClipYMin
			PUBLIC	SYSCALL ClipXMax
			PUBLIC	SYSCALL ClipYMax

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL ClipWindowXMin
			PUBLIC	SYSCALL ClipWindowYMin
			PUBLIC	SYSCALL ClipWindowXMax
			PUBLIC	SYSCALL ClipWindowYMax

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL MemoClipXMin
			PUBLIC	SYSCALL MemoClipYMin
			PUBLIC	SYSCALL MemoClipXMax
			PUBLIC	SYSCALL MemoClipYMax

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL MemoClipWindowXMin
			PUBLIC	SYSCALL MemoClipWindowYMin
			PUBLIC	SYSCALL MemoClipWindowXMax
			PUBLIC	SYSCALL MemoClipWindowYMax

;----------------------------------------------------------------------------
ClipXMin		dd	0
ClipYMin		dd	0
ClipXMax		dd	0
ClipYMax		dd	0

;----------------------------------------------------------------------------
ClipWindowXMin		dd	0
ClipWindowYMin		dd	0
ClipWindowXMax		dd	0
ClipWindowYMax		dd	0

;----------------------------------------------------------------------------
MemoClipXMin		dd	0
MemoClipYMin		dd	0
MemoClipXMax		dd	0
MemoClipYMax		dd	0

;----------------------------------------------------------------------------
MemoClipWindowXMin	dd	0
MemoClipWindowYMin	dd	0
MemoClipWindowXMax	dd	0
MemoClipWindowYMax	dd	0

;----------------------------------------------------------------------------
				.CODE

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	SetClipWindow
			PUBLIC	SYSCALL	UnsetClipWindow
			PUBLIC	SYSCALL	SetClip
			PUBLIC	SYSCALL	UnsetClip
			PUBLIC	SYSCALL	MemoClip
			PUBLIC	SYSCALL	RestoreClip

;----------------------------------------------------------------------------
;void	SetClipWindow(S32 x0, S32 y0, S32 x1, S32 y1)	;

;#pragma aux SetClipWindow	"*"	\
;	parm	[eax] [ebx] [ecx] [edx]

SetClipWindow		proc

			test	eax, eax
			jae	OkLeft
			xor	eax, eax
OkLeft:			mov	ClipWindowXMin, eax

			test	ebx, ebx
			jae	OkUp
			xor	ebx, ebx
OkUp:			mov	ClipWindowYMin, ebx

			cmp	ecx, ModeResX
			jb	OkRight
			mov	ecx, ModeResX
			dec	ecx
OkRight:		mov	ClipWindowXMax, ecx

			cmp	edx, ModeResY
			jb	OkDown
			mov	edx, ModeResY
			dec	edx
OkDown:			mov	ClipWindowYMax, edx

			mov     eax, ClipXMin
			mov	ebx, ClipYMin
			mov     ecx, ClipXMax
			mov	edx, ClipYMax

			jmp	SetClip

SetClipWindow		endp

;----------------------------------------------------------------------------
;void	UnsetClipWindow()				;

;#pragma aux UnsetClipWindow	"*"	\
;	modify	[eax ebx]

UnsetClipWindow		proc

			mov     eax, ModeResX
			mov	ebx, ModeResY
			dec	eax
			dec	ebx
			mov	ClipWindowXMax, eax
			mov	ClipXMax, eax
			xor	eax, eax
			mov	ClipWindowYMax, ebx
			mov	ClipYMax, ebx
			mov	ClipWindowXMin, eax
			mov	ClipXMin, eax
			mov	ClipWindowYMin, eax
			mov	ClipYMin, eax

			ret

UnsetClipWindow		endp

;----------------------------------------------------------------------------
;void	MemoClipWindow()					;

;#pragma aux MemoClipWindow		"*"	\
;	modify	[eax ebx]

MemoClipWindow		proc

			mov	eax,ClipWindowXMin
			mov	ebx,ClipWindowYMin
			mov	MemoClipWindowXMin, eax
			mov	MemoClipWindowYMin, ebx
			mov	eax,ClipWindowXMax
			mov	ebx,ClipWindowYMax
			mov	MemoClipWindowXMax, eax
			mov	MemoClipWindowYMax, ebx

			ret

MemoClipWindow		endp

;----------------------------------------------------------------------------
;void	RestoreClipWindow()					;

;#pragma aux RestoreClipWindow	"*"	\
;	modify	[eax ebx ecx edx]

RestoreClipWindow	proc

			mov	eax,MemoClipWindowXMin
			mov	ebx,MemoClipWindowYMin
			mov	ecx,MemoClipWindowXMax
			mov	edx,MemoClipWindowYMax

			jmp	SetClipWindow

RestoreClipWindow	endp

;----------------------------------------------------------------------------
;void	SetClip(S32 x0, S32 y0, S32 x1, S32 y1)		;

;#pragma aux SetClip		"*"	\
;	parm	[eax] [ebx] [ecx] [edx]

SetClip			proc

			cmp	eax, ClipWindowXMin
			jge	OkLeft
			mov	eax, ClipWindowXMin
OkLeft:			mov	ClipXMin, eax

			cmp	ebx, ClipWindowYMin
			jge	OkUp
			mov	ebx, ClipWindowYMin
OkUp:			mov	ClipYMin, ebx

			cmp	ecx, ClipWindowXMax
			jle	OkRight
			mov	ecx, ClipWindowXMax
OkRight:		mov	ClipXMax, ecx

			cmp	edx, ClipWindowYMax
			jle	OkDown
			mov	edx, ClipWindowYMax
OkDown:			mov	ClipYMax, edx

			ret

SetClip			endp

;----------------------------------------------------------------------------
;void	UnsetClip()					;

;#pragma aux UnsetClip		"*"	\
;	modify	[eax ebx]

UnsetClip		proc

			mov     eax, ClipWindowXMin
			mov	ebx, ClipWindowYMin
			mov	ClipXMin, eax
			mov	ClipYMin, ebx
			mov     eax, ClipWindowXMax
			mov	ebx, ClipWindowYMax
			mov	ClipXMax, eax
			mov	ClipYMax, ebx

			ret

UnsetClip		endp

;----------------------------------------------------------------------------
;void	MemoClip()					;

;#pragma aux MemoClip		"*"	\
;	modify	[eax ebx]

MemoClip		proc

			mov	eax,ClipXMin
			mov	ebx,ClipYMin
			mov	MemoClipXMin, eax
			mov	MemoClipYMin, ebx
			mov	eax,ClipXMax
			mov	ebx,ClipYMax
			mov	MemoClipXMax, eax
			mov	MemoClipYMax, ebx

			ret

MemoClip		endp

;----------------------------------------------------------------------------
;void	RestoreClip()					;

;#pragma aux RestoreClip	"*"	\
;	modify	[eax ebx ecx edx]

RestoreClip		proc

			mov	eax,MemoClipXMin
			mov	ebx,MemoClipYMin
			mov	ecx,MemoClipXMax
			mov	edx,MemoClipYMax

			jmp	SetClip

RestoreClip		endp

;----------------------------------------------------------------------------
;				The
				End
