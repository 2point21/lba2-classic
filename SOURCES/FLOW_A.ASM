				.386P
				.model SMALL, SYSCALL

				.data
;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

			Extrn	SYSCALL		PtrCLUTGouraud:DWORD

			extrn	SYSCALL		ClipXMin:DWORD
			extrn	SYSCALL		ClipYMin:DWORD
			extrn	SYSCALL		ClipXMax:DWORD
			extrn	SYSCALL		ClipYMax:DWORD

			extrn	SYSCALL		TabOffLine:DWORD
			extrn	SYSCALL		Log:DWORD

			public	SYSCALL      	BoxFlow
			public	SYSCALL      	ShadeBoxBlk
			public	SYSCALL      	CopyBlockShade

;//				.code
Seg_Code		SEGMENT	PUBLIC PARA USE32

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;----------------------------------------------------------------------------

; SEBER

;// ATTENTION: passage de parametres par registres le 04/04/96
;
;			BoxFlow( x,  y, col )
;                               eax ebx ecx
;
;----------------------------------------------------------------------------
;
BoxFlow			proc

			cmp	eax, ClipXMax
			jge	Box_End
			cmp     eax, ClipXMin
			jl	Box_End
			cmp	ebx, ClipYMax
			jge	Box_End
			cmp	ebx, ClipYMin
			jl	Box_End
;----------------------
			mov	edx, TabOffLine[ebx*4]
			add	edx, Log
			add	edx, eax		; EDI Offset Src
;----------------------

			mov	ch, cl
			mov	[edx], cx
			mov	[edx+640], cx
Box_End:
			ret
BoxFlow			endp

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

ShadeBoxBlk		PROC	USES esi edi ebp ebx,\
			x0:DWORD, y0:DWORD, x1:DWORD, y1:DWORD,\
			deccoul:DWORD

			mov	eax,[x0]
			mov	ebx,[y0]
			mov	ecx,[x1]
			mov	edx,[y1]

			cmp	eax,[ClipXMax]
			jg	Box_End
			cmp     ecx,[ClipXMin]
			jl	Box_End
			cmp	ebx,[ClipYMax]
			jg	Box_End
			cmp	edx,[ClipYMin]
			jl	Box_End


			cmp	eax,[ClipXMin]
			jl	Bad_Left
@@Test_Right:
			cmp	ecx,[ClipXMax]
			jg	Bad_Right
@@Test_Up:
			cmp	ebx,[ClipYMin]
			jl	Bad_Up
@@Test_Down:
			cmp	edx,[ClipYMax]
			jg	Bad_Down

@@Disp:
			mov	edi,TabOffLine[ebx*4]
			sub	edx,ebx			; Init Delta Y
			add	edi,[Log]
			mov	ebx,edx
			add	edi,eax			; EDI Offset Src
			inc	ebx


			mov	edx,ecx			; Init Delta X
			mov	esi,TabOffLine[1*4]	; ScanLine
			sub	edx,eax
			mov	ecx,15*256
			inc	edx
			sub	ch,byte ptr [deccoul]
			sub	esi,edx			; ebp Delta Screen
			add	ecx,[PtrCLUTGouraud]
			xor	eax,eax

Again:
			mov	al,[edi]
			mov	ebp,edx
loopx:
			mov	al,[ecx+eax]
			mov	[edi],al
			inc	edi

			dec	ebp
			mov	al,[edi]
			jne	loopx

			add	edi,esi
			dec	ebx
			jne	Again
Box_End:
			ret

Bad_Left:
			mov	eax,[ClipXMin]
			jmp	@@Test_Right
Bad_Right:
			mov	ecx,[ClipXMax]
			jmp	@@Test_Up
Bad_Up:
			mov	ebx,[ClipYMin]
			jmp	@@Test_Down
Bad_Down:
			mov	edx,[ClipYMax]
			jmp	@@Disp
ShadeBoxBlk		ENDP

;//***************************************************************************
;			CopyBlockShade( x0, y0, x1, y1, *src, xd, yd, *dst, shade )
;                                      edx ecx ebx eax   esi
;
CopyBlockShade		proc	uses edi ebp,\
			xd:DWORD, yd:DWORD, dest:DWORD, shade:DWORD

;----------------------

			add	esi, TabOffLine[ecx*4]
			add	esi, edx		; SI Offset Src

			mov	edi, yd
			mov	edi, TabOffLine[edi*4]
			add	edi, dest
			add	edi, xd			; DI Offset Dst

			sub	ebx, edx
			js	@@error
			inc	ebx 			; BX Delta X
			cmp	ebx, 640
			ja	@@error

			sub	eax, ecx
			js	@@error
			inc	eax                     ; AX Delta Y
			cmp	eax, 480
			ja	@@error

			mov	edx, TabOffLine[1*4]	; ScanLine
			inc	eax
			sub	edx, ebx		; DX Delta Screen

			push	ebx
			mov	bl, byte ptr [shade]

			mov	ebp, edx		; deltax
			mov	edx, eax		; nbline

next_line:
			mov	ecx, [esp]

next_one:
			mov	al, byte ptr[esi]
			mov	ah, al
			and	eax, 0F00Fh
			sub	al, bl
			js	lesszero

			add	al, ah
			mov	byte ptr[edi], al
			inc	edi
			inc	esi

			dec	ecx
			jnz	next_one

			add	esi, ebp
			add	edi, ebp

			dec	edx
			jnz	next_line
			pop	ebx
			ret

lesszero:
			mov	byte ptr[edi], ah
			inc	edi
			inc	esi
			dec	ecx
			jnz	next_one

			add	esi, ebp
			add	edi, ebp

			dec	edx
			jnz	next_line
			pop	ebx

@@error:
			ret

CopyBlockShade		endp


;//*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*

;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
Seg_Code		ENDS
;*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*
;				The
				End
