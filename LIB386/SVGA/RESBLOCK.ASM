;----------------------------------------------------------------------------
			.386P
			.MODEL SMALL, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			EXTRN	SYSCALL	TabOffLine:DWORD

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	RestoreBlock

;----------------------------------------------------------------------------
;void	RestoreBlock( void *screen, void *buffer, S32 x0, S32 y0, S32 x1, S32 y1)

;#pragma aux RestoreBlock	"*"			\
;	parm	[edi] [esi] [eax] [ebx] [ecx] [edx]

RestoreBlock		proc

			sub	ecx, eax		; ECX = DeltaX-1
			sub	edx, ebx		; EDX = DeltaY-1

			inc	ecx			; ECX = DeltaX
			add	edx, 2			; EDX = DeltaY+1

			mov	ebx, TabOffLine[ebx*4]
			add	edi, eax

			mov	eax, TabOffLine[1*4]
			add	edi, ebx

			push	ebp
			sub	eax, ecx

			mov	ebx, ecx		; EBX = DeltaX
			mov	ebp, ecx

			shr	ebx, 2
			and	ebp, 11b

			shr	edx, 1
			jnc	odd
Again:
			mov	ecx, ebx		; Delta X
			rep	movsd
			mov	ecx, ebp
			rep	movsb

			add	edi, eax
odd:
			mov	ecx, ebx		; Delta X
			rep	movsd
			mov	ecx, ebp
			rep	movsb

			add	edi, eax

			dec	edx
			jnz	Again

			pop	ebp

			ret
RestoreBlock		endp

;----------------------------------------------------------------------------
;			The
       			End
