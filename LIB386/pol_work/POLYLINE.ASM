;----------------------------------------------------------------------------
;				S_LINE.ASM 386
;			      (c) Adeline 1993-96
;----------------------------------------------------------------------------
			.386
			.MODEL FLAT, SYSCALL

			INCLUDE FILLER.INC

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			extrn	SYSCALL ClipXMin		:DWORD
			extrn	SYSCALL ClipYMin		:DWORD
			extrn	SYSCALL ClipXMax		:DWORD
			extrn	SYSCALL ClipYMax		:DWORD
			extrn	SYSCALL Log			:DWORD
			extrn	SYSCALL PtrZBuffer		:DWORD
			extrn	SYSCALL PTR_TabOffLine		:DWORD
			extrn	SYSCALL ScreenPitch		:DWORD
			Extrn	SYSCALL	Fill_Flag_Fog		:BYTE
			Extrn	SYSCALL	Fill_Flag_ZBuffer	:BYTE
			Extrn	SYSCALL	Fill_Flag_NZW		:BYTE
			Extrn	SYSCALL	Fill_Logical_Palette	:BYTE

Z1			dd	0			; For the ZBuffer
Z2			dd	0
DZ			dd	0
Color			db	0

s_eax			dd	0
s_ebx			dd	0
s_ecx			dd	0
s_edx			dd	0
s_esi			dd	0
s_edi			dd	0
s_ebp			dd	0

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			public	SYSCALL	Line
			public	SYSCALL	LineZBufNZW
			public	SYSCALL	LineZBuf
			public	SYSCALL	Line_A

			PUBLIC	SYSCALL	First_POLYLINEPatch
			PUBLIC	SYSCALL	Last_POLYLINEPatch


;----------------------------------------------------------------------------
;		Line( x0, y0, x1, y1, coul )
;
Line			PROC	USES esi edi ebp ebx,\
			x0:DWORD, y0:DWORD,\
			x1:DWORD, y1:DWORD,\
			coul:DWORD

; 			X0:ax  Y0:bx
;			X1:cx  Y1:DX
;			couleur: bp

			mov	eax, x0
			mov	ebx, y0
			mov	ecx, x1
			mov	edx, y1

			mov	ebp, coul

			call	Line_Entry

			ret
Line			ENDP

;----------------------------------------------------------------------------
;		LineZBufNZW( x0, y0, z0, x1, y1, z1, coul )
;
LineZBufNZW		PROC	USES esi edi ebp ebx,\
			x0:DWORD, y0:DWORD, zb0:DWORD,\
			x1:DWORD, y1:DWORD, zb1:DWORD,\
			coul:DWORD

; 			X0:ax  Y0:bx
;			X1:cx  Y1:DX
;			couleur: bp

			mov	eax, x0
			mov	ebx, y0
			mov	edi, zb0
			mov	ecx, x1
			mov	edx, y1
			mov	esi, zb1
			mov	ebp, coul

			call	Line_ZBuffer_NZW

			ret
LineZBufNZW		ENDP


;----------------------------------------------------------------------------
;		LineZBufNZW( x0, y0, z0, x1, y1, z1, coul )
;
LineZBuf		PROC	USES esi edi ebp ebx,\
			x0:DWORD, y0:DWORD, zb0:DWORD,\
			x1:DWORD, y1:DWORD, zb1:DWORD,\
			coul:DWORD

; 			X0:ax  Y0:bx
;			X1:cx  Y1:DX
;			couleur: bp

			mov	eax, x0
			mov	ebx, y0
			mov	edi, zb0
			mov	ecx, x1
			mov	edx, y1
			mov	esi, zb1
			mov	ebp, coul

			call	Line_ZBuffer

			ret
LineZBuf		ENDP


First_POLYLINEPatch	Label	DWORD



; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ   Trace de droite                                                      บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel : (EAX,EBX), (ECX,EDX) : points extremes                         บ
; บ         EBP = Color                                                    บ
; บ                                                                        บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4
Line_A			PROC
			cmp	[Fill_Flag_NZW],TRUE
			je	Line_ZBuffer_NZW

			cmp	[Fill_Flag_ZBuffer],TRUE
			je	Line_ZBuffer
Line_Entry::
			push	eax
			mov	eax,ebp
			and	ebp,0FFh
			cmp	[Fill_Flag_Fog],TRUE
			jne	@@Pas_Coul_Convert
			mov	al,[Fill_Logical_Palette+ebp]
@@Pas_Coul_Convert:
			mov	byte ptr [@@ColorX+2],al	; Patch
			mov	byte ptr [@@ColorY+2],al
			mov	[Color],al
			pop	eax


@@Trace_Still:
			mov	edi,ecx			; EDI = DX
			mov	esi,edx			; ESI = DY

			sub	edi,eax
			je	@@DX_Nul

			sub	esi,ebx
			je	@@DY_Nul

			cmp	eax,[ClipXMin]
			jge	@@Clip_X0
							; Case X1<XMin
			cmp	ecx,[ClipXMin]
			jl	@@End

			push	edx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			imul	esi
			idiv	edi
			sub	ebx,eax
			mov	eax,[ClipXMin]
			pop	edx
			jmp	@@Trace_Still

@@Clip_X0:
			cmp	eax,[ClipXMax]
			jle	@@Clip_X1
							; Case X1>XMax
			cmp	ecx,[ClipXMax]
			jg	@@End

			push	edx
			sub	eax,[ClipXMax]		; y=y1-(x1-XMax)*dy/dx
			imul	esi
			idiv	edi
			sub	ebx,eax
			pop	edx
			mov	eax,[ClipXMax]
			jmp	@@Trace_Still

@@Clip_X1:
			cmp	ecx,[ClipXMin]
			jge	@@Clip_X2
							; Case X2<XMin

			push	eax
			mov	eax,ecx
			push	edx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			imul	esi
			idiv	edi
			pop	edx
			sub	edx,eax
			mov	ecx,[ClipXMin]
			pop	eax
			jmp	@@Trace_Still

@@Clip_X2:
			cmp	ecx,[ClipXMax]
			jle	@@Clip_X3
							; Case X2>XMax
			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMax]
			imul	esi
			idiv	edi
			pop	edx
			sub	edx,eax
			mov	ecx,[ClipXMax]
			pop	eax
			jmp	@@Trace_Still

@@Clip_X3:
			cmp	ebx,[ClipYMin]
			jge	@@Clip_Y0
							; Case Y1<YMin
			cmp	edx,[ClipYMin]
			jl	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMin]
			sub	eax,ebx
			imul	edi
			idiv	esi
			pop	edx
			add	eax,edx
			mov	ebx,[ClipYMin]
			pop	edx
			jmp	@@Trace_Still


@@Clip_Y0:
			cmp	ebx,[ClipYMax]
			jle	@@Clip_Y1
							; Case Y1>YMax
			cmp	edx,[ClipYMax]
			jg	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,ebx
			imul	edi
			idiv	esi
			pop	edx
			add	eax,edx
			mov	ebx,[ClipYMax]
			pop	edx
			jmp	@@Trace_Still

@@Clip_Y1:
			cmp	edx,[ClipYMin]
			jge	@@Clip_Y2
							; Case Y2<YMin

			push	eax
			mov	eax,[ClipYMin]
			sub	eax,edx
			imul	edi
			idiv	esi
			add	ecx,eax
			mov	edx,[ClipYMin]
			pop	eax
			jmp	@@Trace_Still


@@Clip_Y2:
			cmp	edx,[ClipYMax]
			jle	@@Clip_Y3
							; Case Y2>YMax
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,edx
			imul	edi
			idiv	esi
			add	ecx,eax
			mov	edx,[ClipYMax]
			pop	eax
			jmp	@@Trace_Still

@@Clip_Y3:
			mov	edi,ecx			; EDI = ABS(Delta_X)

			mov	ebp,[ScreenPitch]; EBP = Incr_Y
			mov	ecx,1			; ECX = Incr_X

			sub	edi,eax
			jns	@@DeltaX_Pos
			neg	edi
			neg	ecx
@@DeltaX_Pos:
			mov	esi,edx			; ESI = ABS(Delta_Y)
			sub	esi,ebx
			jns	@@DeltaY_Pos
			neg	esi
			neg	ebp
@@DeltaY_Pos:
			shl	ebx,2
			add	ebx,[PTR_TabOffLine]

			mov	ebx,[ebx]

			add	ebx,eax
			add	ebx,[Log]

			cmp	edi,esi
			jle	@@Draw_Y

							; Delta_X > Delta_Y
			mov	edx,edi
			mov	eax,edi
			shr	edx,1
@@Do_X:
@@ColorX:		mov	byte ptr [ebx],12h
			add	edx,esi

			cmp	edx,edi
			jb	@@Plot_X

			sub	edx,edi
			add	ebx,ebp
@@Plot_X:
			add	ebx,ecx
			dec	eax

			jge	@@Do_X

			ret


@@Draw_Y:
			mov	edx,esi
			shr	edx,1
			mov	eax,esi

@@Do_Y:
@@ColorY:		mov	byte ptr [ebx],12h
			add	edx,edi

			cmp	edx,esi
			jb	@@Plot_Y

			sub	edx,esi
			add	ebx,ecx
@@Plot_Y:
			add	ebx,ebp
			dec	eax
			jge	@@Do_Y
@@End:
			ret


					; *** DeltaX nul ***
@@DX_Nul:
			cmp	eax,[ClipXMin]
			jl	@@End
			cmp	eax,[ClipXMax]
			jg	@@End

			sub	esi,ebx
			je	@@Plot

			cmp	ebx,[ClipYMin]
			jge	@@DX_Y1
			cmp	edx,[ClipYMin]
			jl	@@End
			mov	ebx,[ClipYMin]
@@DX_Y1:
			cmp	ebx,[ClipYMax]
			jle	@@DX_Y1b
			cmp	edx,[ClipYMax]
			jg	@@End
			mov	ebx,[ClipYMax]
@@DX_Y1b:

			cmp	edx,[ClipYMin]
			jge	@@DX_Y2
			mov	edx,[ClipYMin]
@@DX_Y2:
			cmp	edx,[ClipYMax]
			jle	@@DX_Y2b
			mov	edx,[ClipYMax]
@@DX_Y2b:
			mov	esi,edx
			sub	esi,ebx
			jns	@@DX_Ok
			neg	esi
			mov	ebx,edx
@@DX_Ok:
			shl	ebx,2
			add	ebx,[PTR_TabOffLine]
			mov	ebp,[ScreenPitch]
			mov	al,[Color]
			mov	ebx,[ebx]
			inc	esi
			add	ebx,[Log]
			add	ebx,ecx
@@Burst_Y:
			mov	[ebx],al
			add	ebx,ebp
			dec	esi
			jne	@@Burst_Y

			ret

					; *** DeltaY nul ***
@@DY_Nul:
			cmp	ebx,[ClipYMin]
			jl	@@End
			cmp	ebx,[ClipYMax]
			jg	@@End

			cmp	eax,[ClipXMin]
			jge	@@DY_X1
			cmp	ecx,[ClipXMin]
			jl	@@End
			mov	eax,[ClipXMin]
@@DY_X1:
			cmp	eax,[ClipXMax]
			jle	@@DY_X1b
			cmp	ecx,[ClipXMax]
			jg	@@End
			mov	eax,[ClipXMax]
@@DY_X1b:

			cmp	ecx,[ClipXMin]
			jge	@@DY_X2
			mov	ecx,[ClipXMin]
@@DY_X2:
			cmp	ecx,[ClipXMax]
			jle	@@DY_X2b
			mov	ecx,[ClipXMax]
@@DY_X2b:
			mov	edi,ecx
			sub	edi,eax
			jns	@@DY_Ok
			neg	edi
			mov	eax,ecx
@@DY_Ok:
			shl	ebx,2
			add	ebx,[PTR_TabOffLine]
			mov	cl,[Color]
			mov	ebx,[ebx]
			inc	edi
			add	ebx,eax
			add	ebx,[Log]
@@Burst_X:
			mov	[ebx],cl
			inc	ebx
			dec	edi
			jne	@@Burst_X
			ret

					; *** Plot ***
@@Plot:
			cmp	ebx,[ClipYMin]
			jl	@@End
			cmp	ebx,[ClipYMax]
			jg	@@End

			shl	ebx,2
			add	ebx,[PTR_TabOffLine]
			mov	ebx,[ebx]
			add	ebx,eax
			mov	al,[Color]
			add	ebx,[Log]
			mov	[ebx],al

			ret
Line_A			ENDP






; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ   Trace de droite Zbuffer                                              บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel : (EAX,EBX), (ECX,EDX) : points extremes                         บ
; บ         EBP = Color                                                    บ
; บ         EDI = Z1                                                       บ
; บ         ESI = Z2                                                       บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4

Line_ZBuffer		PROC

;			mov	s_eax, eax
;			mov	s_ebx, ebx
;			mov	s_ecx, ecx
;			mov	s_edx, edx
;			mov	s_esi, esi
;			mov	s_edi, edi
;			mov	s_ebp, ebp

			shl	esi,16
			shl	edi,16
			mov	[Z1],edi
			mov	[Z2],esi


			push	eax
			mov	eax,ebp
			and	ebp,0FFh
			cmp	[Fill_Flag_Fog],TRUE
			jne	@@Pas_Coul_Convert
			mov	al,[Fill_Logical_Palette+ebp]
@@Pas_Coul_Convert:
			mov	[Color],al
			pop	eax

@@Trace_Still:
			mov	edi,[Z1]
			mov	esi,[Z2]

			sub	esi,edi

        		mov	[DZ],esi

			mov	edi,ecx			; EDI = DX
			mov	esi,edx			; ESI = DY

			sub	edi,eax
			je	@@DX_Nul

			sub	esi,ebx
			je	@@DY_Nul


			cmp	eax,[ClipXMin]
			jge	@@Clip_X0
							; Case X1<XMin
			cmp	ecx,[ClipXMin]
			jl	@@End


			push	edx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			pop	eax
			imul	esi
			idiv	edi
			sub	ebx,eax
			mov	eax,[ClipXMin]
			pop	edx
			jmp	@@Trace_Still

@@Clip_X0:
			cmp	eax,[ClipXMax]
			jle	@@Clip_X1
							; Case X1>XMax
			cmp	ecx,[ClipXMax]
			jg	@@End


			push	edx
			sub	eax,[ClipXMax]		; y=y1-(x1-XMax)*dy/dx
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			pop	eax
			imul	esi
			idiv	edi
			sub	ebx,eax
			pop	edx
			mov	eax,[ClipXMax]
			jmp	@@Trace_Still

@@Clip_X1:
			cmp	ecx,[ClipXMin]
			jge	@@Clip_X2
							; Case X2<XMin

			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	eax
			imul	esi
			idiv	edi
			pop	edx
			sub	edx,eax
			mov	ecx,[ClipXMin]
			pop	eax
			jmp	@@Trace_Still

@@Clip_X2:
			cmp	ecx,[ClipXMax]
			jle	@@Clip_X3
							; Case X2>XMax
			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMax]
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	eax
			imul	esi
			idiv	edi
			pop	edx
			sub	edx,eax
			mov	ecx,[ClipXMax]
			pop	eax
			jmp	@@Trace_Still

@@Clip_X3:
			cmp	ebx,[ClipYMin]
			jge	@@Clip_Y0
							; Case Y1<YMin
			cmp	edx,[ClipYMin]
			jl	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMin]
			sub	eax,ebx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			pop	eax
			imul	edi
			idiv	esi
			pop	edx
			add	eax,edx
			mov	ebx,[ClipYMin]
			pop	edx
			jmp	@@Trace_Still


@@Clip_Y0:
			cmp	ebx,[ClipYMax]
			jle	@@Clip_Y1
							; Case Y1>YMax
			cmp	edx,[ClipYMax]
			jg	@@End


			push	edx
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,ebx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			pop	eax
			imul	edi
			idiv	esi
			pop	edx
			add	eax,edx
			mov	ebx,[ClipYMax]
			pop	edx
			jmp	@@Trace_Still

@@Clip_Y1:
			cmp	edx,[ClipYMin]
			jge	@@Clip_Y2
							; Case Y2<YMin

			push	eax
			mov	eax,[ClipYMin]
			sub	eax,edx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			pop	eax
			imul	edi
			idiv	esi
			add	ecx,eax
			mov	edx,[ClipYMin]
			pop	eax
			jmp	@@Trace_Still


@@Clip_Y2:
			cmp	edx,[ClipYMax]
			jle	@@Clip_Y3

							; Case Y2>YMax
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,edx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			pop	eax
			imul	edi
			idiv	esi
			add	ecx,eax
			mov	edx,[ClipYMax]
			pop	eax
			jmp	@@Trace_Still

@@Clip_Y3:
@@Draw_Line:
			mov	edi,ecx			; EDI = ABS(Delta_X)

			mov	ebp,[ScreenPitch]; EBP = Incr_Y
			mov	ecx,1			; ECX = Incr_X

			sub	edi,eax
			jns	@@DeltaX_Pos
			neg	edi
			neg	ecx
@@DeltaX_Pos:
			mov	esi,edx			; ESI = ABS(Delta_Y)
			sub	esi,ebx
			jns	@@DeltaY_Pos
			neg	esi
			neg	ebp
@@DeltaY_Pos:
			shl	ebx,2
			add	ebx,[PTR_TabOffLine]

			mov	ebx,[ebx]

			add	ebx,eax

			cmp	edi,esi
			jl	@@Draw_Y

							; Delta_X > Delta_Y
			mov	dword ptr [@@DY+2],esi
			mov	dword ptr [@@DX+2],edi
			mov	dword ptr [@@DX1+2],edi

			mov	eax,[DZ]
			test	ecx,ecx
			jns	@@Pos1
			neg	eax
@@Pos1:
			mov	edx,eax
			sar	edx,31
			idiv	edi
			mov	edx,eax
			shl	eax,16
			sar	edx,16
			mov	dword ptr [@@DZ+2],eax
			mov	dword ptr [@@IZ+2],edx

			mov	dword ptr [@@Line+2],ebp

			mov	eax,[Log]
			mov	dword ptr [@@PtrLog+2],eax
			mov	eax,[PtrZBuffer]
			mov	dword ptr [@@PtrZBuf+4],eax
			mov	dword ptr [@@PtrZBuf1+4],eax

			mov	edx,edi
			mov	ebp,edi
			shr	edx,1

			mov	al,[Color]
			mov	esi,[Z1]
			mov	edi,esi
			shl	esi,16
			sar	edi,16

			jmp	$+2

@@Do_X:
@@PtrZBuf:		cmp	word ptr [ebx*2+12345678h],di
			jb	@@NoDisp

@@PtrZBuf1:		mov	word ptr [ebx*2+12345678h],di
@@PtrLog:		mov	byte ptr [ebx+12345678h],al
@@NoDisp:
@@DY:			add	edx,12345678h

@@DX:			cmp	edx,12345678h
			jb	@@Plot_X

@@DX1:			sub	edx,12345678h
@@Line:			add	ebx,12345678h
@@Plot_X:
			add	ebx,ecx

@@DZ:			add	esi,12345678h
@@IZ:			adc	edi,12345678h

			dec	ebp
			jge	@@Do_X

			ret


@@Draw_Y:
			mov	dword ptr [@@YDX+2],edi
			mov	dword ptr [@@YDY+2],esi
			mov	dword ptr [@@YDY1+2],esi

			mov	eax,[DZ]
			test	ebp,ebp
			jns	@@Pos2
			neg	eax
@@Pos2:
			mov	edx,eax
			sar	edx,31
			idiv	esi
			mov	edx,eax
			shl	eax,16
			sar	edx,16
			mov	dword ptr [@@YDZ+2],eax
			mov	dword ptr [@@YIZ+2],edx

			mov	dword ptr [@@Row+2],ecx

			mov	eax,[Log]
			mov	dword ptr [@@YPtrLog+2],eax
			mov	eax,[PtrZBuffer]
			mov	dword ptr [@@YPtrZBuf+4],eax
			mov	dword ptr [@@YPtrZBuf1+4],eax

			mov	edx,esi
			mov	ecx,esi
			shr	edx,1

			mov	al,[Color]
			mov	esi,[Z1]
			mov	edi,esi
			shl	esi,16
			sar	edi,16

			jmp	$+2

@@Do_Y:
@@YPtrZBuf:		cmp	word ptr [ebx*2+12345678h],di
			jb	@@YNoDisp

@@YPtrZBuf1:		mov	word ptr [ebx*2+12345678h],di
@@YPtrLog:		mov	byte ptr [ebx+12345678h],al
@@YNoDisp:
@@YDX:			add	edx,12345678h

@@YDY:			cmp	edx,12345678h
			jb	@@Plot_Y

@@YDY1:			sub	edx,12345678h
@@Row:			add	ebx,12345678h
@@Plot_Y:
@@YDZ:			add	esi,12345678h
@@YIZ:			adc	edi,12345678h

			add	ebx,ebp

			dec	ecx
			jge	@@Do_Y
@@End:
			ret




					; *** DeltaX nul ***
@@DX_Nul:
			cmp	eax,[ClipXMin]
			jl	@@End
			cmp	eax,[ClipXMax]
			jg	@@End

			sub	esi,ebx
			je	@@Plot

			cmp	ebx,[ClipYMin]
			jge	@@DX_Y1
			cmp	edx,[ClipYMin]
			jl	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMin]
			sub	eax,ebx
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			mov	ebx,[ClipYMin]
			pop	eax
			pop	edx
			jmp	@@Trace_Still

@@DX_Y1:
			cmp	ebx,[ClipYMax]
			jle	@@DX_Y1b
			cmp	edx,[ClipYMax]
			jg	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,ebx
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			pop	eax
			pop	edx
			mov	ebx,[ClipYMax]
			jmp	@@Trace_Still
@@DX_Y1b:

			cmp	edx,[ClipYMin]
			jge	@@DX_Y2

			push	eax
			mov	eax,[ClipYMin]
			sub	eax,edx
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			mov	edx,[ClipYMin]
			pop	eax
			jmp	@@Trace_Still

@@DX_Y2:
			cmp	edx,[ClipYMax]
			jle	@@Draw_Line

			push	eax
			mov	eax,[ClipYMax]
			sub	eax,edx
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			mov	edx,[ClipYMax]
			pop	eax
			jmp	@@Trace_Still



					; *** DeltaY nul ***
@@DY_Nul:
			cmp	ebx,[ClipYMin]
			jl	@@End
			cmp	ebx,[ClipYMax]
			jg	@@End

			cmp	eax,[ClipXMin]
			jge	@@DY_X1
			cmp	ecx,[ClipXMin]
			jl	@@End

			push	edx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			mov	eax,[ClipXMin]
			pop	edx
			jmp	@@Trace_Still

@@DY_X1:
			cmp	eax,[ClipXMax]
			jle	@@DY_X1b
			cmp	ecx,[ClipXMin]
			jg	@@End

			push	edx
			sub	eax,[ClipXMax]		; y=y1-(x1-XMax)*dy/dx
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			pop	edx
			mov	eax,[ClipXMax]
			jmp	@@Trace_Still
@@DY_X1b:

			cmp	ecx,[ClipXMin]
			jge	@@DY_X2

			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	edx
			pop	eax
			mov	ecx,[ClipXMin]
			jmp	@@Trace_Still
@@DY_X2:
			cmp	ecx,[ClipXMax]
			jle	@@Draw_Line

			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMax]
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	edx
			pop	eax
			mov	ecx,[ClipXMax]
			jmp	@@Trace_Still

					; *** Plot ***
@@Plot:
			cmp	ebx,[ClipYMin]
			jl	@@End
			cmp	ebx,[ClipYMax]
			jg	@@End

			shl	ebx,2
			add	ebx,[PTR_TabOffLine]
			mov	ebx,[ebx]
			add	ebx,eax
			mov	al,[Color]
			add	ebx,[Log]
			mov	[ebx],al

			ret

;			mov	eax, s_eax
;			mov	ebx, s_ebx
;			mov	ecx, s_ecx
;			mov	edx, s_edx
;			mov	esi, s_esi
;			mov	edi, s_edi
;			mov	ebp, s_ebp

Line_ZBuffer		ENDP




; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ   Trace de droite Zbuffer No ZBuf write                                บ
; วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
; บ Appel : (EAX,EBX), (ECX,EDX) : points extremes                         บ
; บ         EBP = Color                                                    บ
; บ         EDI = Z1                                                       บ
; บ         ESI = Z2                                                       บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4
Line_ZBuffer_NZW	PROC

;			mov	s_eax, eax
;			mov	s_ebx, ebx
;			mov	s_ecx, ecx
;			mov	s_edx, edx
;			mov	s_esi, esi
;			mov	s_edi, edi
;			mov	s_ebp, ebp

			shl	esi,16
			shl	edi,16
			mov	[Z1],edi
			mov	[Z2],esi


			push	eax
			mov	eax,ebp
			and	ebp,0FFh
			cmp	[Fill_Flag_Fog],TRUE
			jne	@@Pas_Coul_Convert
			mov	al,[Fill_Logical_Palette+ebp]
@@Pas_Coul_Convert:
			mov	[Color],al
			pop	eax

@@Trace_Still:
			mov	edi,[Z1]
			mov	esi,[Z2]

			sub	esi,edi

        		mov	[DZ],esi

			mov	edi,ecx			; EDI = DX
			mov	esi,edx			; ESI = DY

			sub	edi,eax
			je	@@DX_Nul

			sub	esi,ebx
			je	@@DY_Nul

			cmp	eax,[ClipXMin]
			jge	@@Clip_X0
							; Case X1<XMin
			cmp	ecx,[ClipXMin]
			jl	@@End


			push	edx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			pop	eax
			imul	esi
			idiv	edi
			sub	ebx,eax
			mov	eax,[ClipXMin]
			pop	edx
			jmp	@@Trace_Still

@@Clip_X0:
			cmp	eax,[ClipXMax]
			jle	@@Clip_X1
							; Case X1>XMax
			cmp	ecx,[ClipXMax]
			jg	@@End


			push	edx
			sub	eax,[ClipXMax]		; y=y1-(x1-XMax)*dy/dx
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			pop	eax
			imul	esi
			idiv	edi
			sub	ebx,eax
			pop	edx
			mov	eax,[ClipXMax]
			jmp	@@Trace_Still

@@Clip_X1:
			cmp	ecx,[ClipXMin]
			jge	@@Clip_X2
							; Case X2<XMin

			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	eax
			imul	esi
			idiv	edi
			pop	edx
			sub	edx,eax
			mov	ecx,[ClipXMin]
			pop	eax
			jmp	@@Trace_Still

@@Clip_X2:
			cmp	ecx,[ClipXMax]
			jle	@@Clip_X3
							; Case X2>XMax
			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMax]
			push	eax
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	eax
			imul	esi
			idiv	edi
			pop	edx
			sub	edx,eax
			mov	ecx,[ClipXMax]
			pop	eax
			jmp	@@Trace_Still

@@Clip_X3:
			cmp	ebx,[ClipYMin]
			jge	@@Clip_Y0
							; Case Y1<YMin
			cmp	edx,[ClipYMin]
			jl	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMin]
			sub	eax,ebx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			pop	eax
			imul	edi
			idiv	esi
			pop	edx
			add	eax,edx
			mov	ebx,[ClipYMin]
			pop	edx
			jmp	@@Trace_Still


@@Clip_Y0:
			cmp	ebx,[ClipYMax]
			jle	@@Clip_Y1
							; Case Y1>YMax
			cmp	edx,[ClipYMax]
			jg	@@End


			push	edx
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,ebx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			pop	eax
			imul	edi
			idiv	esi
			pop	edx
			add	eax,edx
			mov	ebx,[ClipYMax]
			pop	edx
			jmp	@@Trace_Still

@@Clip_Y1:
			cmp	edx,[ClipYMin]
			jge	@@Clip_Y2
							; Case Y2<YMin

			push	eax
			mov	eax,[ClipYMin]
			sub	eax,edx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			pop	eax
			imul	edi
			idiv	esi
			add	ecx,eax
			mov	edx,[ClipYMin]
			pop	eax
			jmp	@@Trace_Still


@@Clip_Y2:
			cmp	edx,[ClipYMax]
			jle	@@Clip_Y3

							; Case Y2>YMax
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,edx
			push	eax
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			pop	eax
			imul	edi
			idiv	esi
			add	ecx,eax
			mov	edx,[ClipYMax]
			pop	eax
			jmp	@@Trace_Still

@@Clip_Y3:
@@Draw_Line:
			mov	edi,ecx			; EDI = ABS(Delta_X)

			mov	ebp,[ScreenPitch]; EBP = Incr_Y
			mov	ecx,1			; ECX = Incr_X

			sub	edi,eax
			jns	@@DeltaX_Pos
			neg	edi
			neg	ecx
@@DeltaX_Pos:
			mov	esi,edx			; ESI = ABS(Delta_Y)
			sub	esi,ebx
			jns	@@DeltaY_Pos
			neg	esi
			neg	ebp
@@DeltaY_Pos:
			shl	ebx,2
			add	ebx,[PTR_TabOffLine]

			mov	ebx,[ebx]

			add	ebx,eax

			cmp	edi,esi
			jl	@@Draw_Y

							; Delta_X > Delta_Y
			mov	dword ptr [@@DY+2],esi
			mov	dword ptr [@@DX+2],edi
			mov	dword ptr [@@DX1+2],edi

			mov	eax,[DZ]
			test	ecx,ecx
			jns	@@Pos1
			neg	eax
@@Pos1:
			mov	edx,eax
			sar	edx,31
			idiv	edi
			mov	edx,eax
			shl	eax,16
			sar	edx,16
			mov	dword ptr [@@DZ+2],eax
			mov	dword ptr [@@IZ+2],edx

			mov	dword ptr [@@Line+2],ebp

			mov	eax,[Log]
			mov	dword ptr [@@PtrLog+2],eax
			mov	eax,[PtrZBuffer]
			mov	dword ptr [@@PtrZBuf+4],eax

			mov	edx,edi
			mov	ebp,edi
			shr	edx,1

			mov	esi,[Z1]
			mov	edi,esi
			shl	esi,16
			mov	al,[Color]
			sar	edi,16

			jmp	$+2

@@Do_X:
@@PtrZBuf:		cmp	word ptr [ebx*2+12345678h],di
			jb	@@NoDisp
@@PtrLog:		mov	byte ptr [ebx+12345678h],al
@@NoDisp:
@@DY:			add	edx,12345678h

@@DX:			cmp	edx,12345678h
			jb	@@Plot_X

@@DX1:			sub	edx,12345678h
@@Line:			add	ebx,12345678h
@@Plot_X:
			add	ebx,ecx

@@DZ:			add	esi,12345678h
@@IZ:			adc	edi,12345678h

			dec	ebp
			jge	@@Do_X

			ret


@@Draw_Y:
			mov	dword ptr [@@YDX+2],edi
			mov	dword ptr [@@YDY+2],esi
			mov	dword ptr [@@YDY1+2],esi

			mov	eax,[DZ]
			test	ebp,ebp
			jns	@@Pos2
			neg	eax
@@Pos2:
			mov	edx,eax
			sar	edx,31
			idiv	esi
			mov	edx,eax
			shl	eax,16
			sar	edx,16
			mov	dword ptr [@@YDZ+2],eax
			mov	dword ptr [@@YIZ+2],edx

			mov	dword ptr [@@Row+2],ecx

			mov	eax,[Log]
			mov	dword ptr [@@YPtrLog+2],eax
			mov	eax,[PtrZBuffer]
			mov	dword ptr [@@YPtrZBuf+4],eax

			mov	edx,esi
			mov	ecx,esi
			shr	edx,1

			mov	esi,[Z1]
			mov	al,[Color]
			mov	edi,esi
			shl	esi,16
			sar	edi,16

			jmp	$+2

@@Do_Y:
@@YPtrZBuf:		cmp	word ptr [ebx*2+12345678h],di
			jb	@@YNoDisp
@@YPtrLog:		mov	byte ptr [ebx+12345678h],al
@@YNoDisp:
@@YDX:			add	edx,12345678h

@@YDY:			cmp	edx,12345678h
			jb	@@Plot_Y

@@YDY1:			sub	edx,12345678h
@@Row:			add	ebx,12345678h
@@Plot_Y:
@@YDZ:			add	esi,12345678h
@@YIZ:			adc	edi,12345678h

			add	ebx,ebp

			dec	ecx
			jge	@@Do_Y
@@End:
			ret

					; *** DeltaX nul ***
@@DX_Nul:
			cmp	eax,[ClipXMin]
			jl	@@End
			cmp	eax,[ClipXMax]
			jg	@@End

			sub	esi,ebx
			je	@@Plot

			cmp	ebx,[ClipYMin]
			jge	@@DX_Y1
			cmp	edx,[ClipYMin]
			jl	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMin]
			sub	eax,ebx
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			mov	ebx,[ClipYMin]
			pop	eax
			pop	edx
			jmp	@@Trace_Still
@@DX_Y1:
			cmp	ebx,[ClipYMax]
			jle	@@DX_Y1b
			cmp	edx,[ClipYMax]
			jg	@@End

			push	edx
			push	eax
			mov	eax,[ClipYMax]
			sub	eax,ebx
			imul	[DZ]
			idiv	esi
			add	[Z1],eax
			pop	eax
			pop	edx
			mov	ebx,[ClipYMax]
			jmp	@@Trace_Still
@@DX_Y1b:

			cmp	edx,[ClipYMin]
			jge	@@DX_Y2

			push	eax
			mov	eax,[ClipYMin]
			sub	eax,edx
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			mov	edx,[ClipYMin]
			pop	eax
			jmp	@@Trace_Still

@@DX_Y2:
			cmp	edx,[ClipYMax]
			jle	@@Draw_Line

			push	eax
			mov	eax,[ClipYMax]
			sub	eax,edx
			imul	[DZ]
			idiv	esi
			add	[Z2],eax
			mov	edx,[ClipYMax]
			pop	eax
			jmp	@@Trace_Still



					; *** DeltaY nul ***
@@DY_Nul:
			cmp	ebx,[ClipYMin]
			jl	@@End
			cmp	ebx,[ClipYMax]
			jg	@@End

			cmp	eax,[ClipXMin]
			jge	@@DY_X1
			cmp	ecx,[ClipXMin]
			jl	@@End

			push	edx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			mov	eax,[ClipXMin]
			pop	edx
			jmp	@@Trace_Still
@@DY_X1:
			cmp	eax,[ClipXMax]
			jle	@@DY_X1b
			cmp	ecx,[ClipXMin]
			jg	@@End

			push	edx
			sub	eax,[ClipXMax]		; y=y1-(x1-XMax)*dy/dx
			imul	[DZ]
			idiv	edi
			sub	[Z1],eax
			pop	edx
			mov	eax,[ClipXMax]
			jmp	@@Trace_Still
@@DY_X1b:

			cmp	ecx,[ClipXMin]
			jge	@@DY_X2

			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMin]		; y=y1-(x1-XMin)*dy/dx
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	edx
			pop	eax
			mov	ecx,[ClipXMin]
			jmp	@@Trace_Still
@@DY_X2:
			cmp	ecx,[ClipXMax]
			jle	@@Draw_Line

			push	eax
			push	edx
			mov	eax,ecx
			sub	eax,[ClipXMax]
			imul	[DZ]
			idiv	edi
			sub	[Z2],eax
			pop	edx
			pop	eax
			mov	ecx,[ClipXMax]
			jmp	@@Trace_Still

					; *** Plot ***
@@Plot:
			cmp	ebx,[ClipYMin]
			jl	@@End
			cmp	ebx,[ClipYMax]
			jg	@@End

			shl	ebx,2
			add	ebx,[PTR_TabOffLine]
			mov	ebx,[ebx]
			add	ebx,eax
			mov	al,[Color]
			add	ebx,[Log]
			mov	[ebx],al

			ret

Line_ZBuffer_NZW	ENDP






Last_POLYLINEPatch	Label	DWORD



;			The
			End
