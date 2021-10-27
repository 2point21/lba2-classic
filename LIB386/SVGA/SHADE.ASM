;----------------------------------------------------------------------------
			.386p
			.MODEL FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

			extrn	SYSCALL	ClipXMin	:DWORD
			extrn	SYSCALL	ClipXMax	:DWORD
			extrn	SYSCALL	ClipYMin	:DWORD
			extrn	SYSCALL	ClipYMax	:DWORD

			extrn	SYSCALL	TabOffLine	:DWORD

			extrn	SYSCALL	Log		:DWORD

;----------------------------------------------------------------------------
			.CODE

			public	SYSCALL	ShadeBox

;----------------------------------------------------------------------------
;void	ShadeBox(S32 x0, S32 y0, S32 x1, S32 y1, S32 deccoul)	;

;#pragma aux ShadeBox	"*"			\
;	parm	[eax] [ebx] [ecx] [edx] [esi]	\
;	modify	[edi]

ShadeBox		proc uses ebp

			cmp	eax, ClipXMax
			jg	Box_End
			cmp     ecx, ClipXMin
			jl	Box_End
			cmp	ebx, ClipYMax
			jg	Box_End
			cmp	edx, ClipYMin
			jl	Box_End
TestLeft:
			cmp	eax, ClipXMin
			jge	TestRight
			mov	eax, ClipXMin
TestRight:
			cmp	ecx, ClipXMax
			jle	TestUp
			mov	ecx, ClipXMax
TestUp:
			cmp	ebx, ClipYMin
			jge	TestDown
			mov	ebx, ClipYMin
TestDown:
			cmp	edx, ClipYMax
			jle	OkNoClip
			mov	edx, ClipYMax
OkNoClip:
;----------------------
			mov	edi, TabOffLine[ebx*4]
			add	edi, Log
			add	edi, eax		; EDI Offset Src
;----------------------
			sub	edx, ebx		; Init Delta Y
			mov	ebx, edx
			inc	ebx

			mov	edx, ecx		; Init Delta X
			sub	edx, eax
			inc	edx

			mov	ecx, esi

			mov	esi, TabOffLine[1*4]
			sub	esi, edx		; ebp Delta Screen

Again:			mov	ebp, edx

loopx:			mov	al, byte ptr[edi]
			mov	ah, al
;							and	ah, 0F0h
;							and	al, 00Fh
			and	eax, 0F00Fh
			sub	al, cl
			jc	lesszero

			add	al, ah

			mov	byte ptr[edi], al
			inc	edi

			dec	ebp
			jnz	loopx

			add	edi, esi
			dec	ebx
			jne	Again
Box_End:
			ret

lesszero:               mov	al, ah
			mov	byte ptr[edi], al
			inc	edi

			dec	ebp
			jnz	loopx

			add	edi, esi
			dec	ebx
			jne	Again

			ret

ShadeBox		endp

;----------------------------------------------------------------------------
;			The
       			End






