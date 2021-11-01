;----------------------------------------------------------------------------
;				S_LINE.ASM 386
;			      (c) Adeline 1993-96
;----------------------------------------------------------------------------
			.386
			.MODEL FLAT, SYSCALL


			INCLUDE	FILLER.INC


;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
RAIN_RANGE		=	200

;----------------------------------------------------------------------------
			extrn	SYSCALL	ScreenXMin:	DWORD
			extrn	SYSCALL	ScreenYMin:	DWORD
			extrn	SYSCALL	ScreenXMax:	DWORD
			extrn	SYSCALL	ScreenYMax:	DWORD

;----------------------------------------------------------------------------
extrn	SYSCALL 	ClipXMin		:	DWORD
extrn	SYSCALL 	ClipYMin		:	DWORD
extrn	SYSCALL 	ClipXMax		:	DWORD
extrn	SYSCALL 	ClipYMax		:	DWORD
extrn	SYSCALL 	Log			:	DWORD
extrn	SYSCALL 	PtrZBuffer		:	DWORD
extrn	SYSCALL 	PTR_TabOffLine		:	DWORD
extrn	SYSCALL 	ScreenPitch		:	DWORD
Extrn	SYSCALL		Fill_Flag_Fog		:	BYTE
Extrn	SYSCALL		Fill_Flag_ZBuffer	:	BYTE
Extrn	SYSCALL	  	Fill_Logical_Palette	:	BYTE

;----------------------------------------------------------------------------
CurrentZ1		dd	0			; For the ZBuffer
CurrentZ2		dd	0
DZ	       		dd	0
Color	       		db	0

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
IFDEF	_WIN32
			Extrn	SYSCALL	AllowPatchAndPatch	:PROC
ENDIF	; _WIN32

;----------------------------------------------------------------------------
			public	SYSCALL	LineRain


;----------------------------------------------------------------------------
ifdef	_WIN32

;----------------------------------------------------------------------------
Init_LineRain 		PROC

			push	Offset LineRain
			push	Offset EndLineRain
			push	Offset Good_Code
			jmp	AllowPatchAndPatch
Good_Code:
			shl	esi,16
			push	ebp
			shl	edi,16

Init_LineRain 		ENDP

;----------------------------------------------------------------------------
endif;	_WIN32

;----------------------------------------------------------------------------
; ╔════════════════════════════════════════════════════════════════════════╗
; ║   Trace de droite Zbuffer (methode toute con (Lucas)) No ZBuf write    ║
; ╟────────────────────────────────────────────────────────────────────────╢
; ║ Appel : (EAX,EBX), (ECX,EDX) : points extremes                         ║
; ║         STACK= Color                                                   ║
; ║         EDI  = CurrentZ1                                               ║
; ║         ESI  = CurrentZ2                                               ║
; ║			; Return value: bit field                          ║
; ║			;		0 : line not displayed             ║
; ║			;		1 : line at least partially visible║
; ║			;		2 : line intersects the background ║
; ╚════════════════════════════════════════════════════════════════════════╝

;S32	LineRain(S32 x0, S32 y0, S32 z0, S32 x1, S32 y1, S32 z1, S32 coul );

;#pragma aux LineRain	"*"				\
;	parm caller [eax] [ebx] [edi] [ecx] [edx] [esi]

			ALIGN	4
LineRain		PROC
x:

			shl	esi,16
			push	ebp

			shl	edi,16
			mov	ebp, [esp+8]
ifdef	_WIN32
y:
			org	x
			jmp	Init_LineRain
			org	y
endif;	_WIN32
			mov	[CurrentZ2],esi
			mov	[CurrentZ1],edi
			push	eax
			mov	eax,ebp
			and	ebp,0FFh
			cmp	[Fill_Flag_Fog],TRUE
			jne	Pas_Coul_Convert
			mov	al,[Fill_Logical_Palette+ebp]
Pas_Coul_Convert:
			mov	[Color],al
			pop	eax

Trace_Still:
			mov	edi,[CurrentZ1]
			mov	esi,[CurrentZ2]

			sub	esi,edi
			mov	edi,ecx

			mov	[DZ],esi
			mov	esi,edx

			sub	edi,eax			; EDI = DELTA_X
			jz	DX_Nul

			sub	esi,ebx			; ESI = DELTA_Y
			jz	DY_Nul

			cmp	eax,[ClipXMin]
			jl	Clip_X0

			cmp	eax,[ClipXMax]
			jg	Clip_X1

			cmp	ecx,[ClipXMin]
			jl	Clip_X2

			cmp	ecx,[ClipXMax]
			jg	Clip_X3

			cmp	ebx,[ClipYMin]
			jl	Clip_Y0

			cmp	ebx,[ClipYMax]
			jg	Clip_Y1

			cmp	edx,[ClipYMin]
			jl	Clip_Y2

			cmp	edx,[ClipYMax]
			jg	Clip_Y3
Draw_Line:
			mov	[ScreenXMin],eax
			mov	[ScreenXMax],ecx

			mov	edi,ecx
			mov	ecx,1			; ECX = Incr_X

			sub	edi,eax
			jns	DeltaX_Pos

			neg	edi
			neg	ecx

			mov	[ScreenXMin],ecx
			mov	[ScreenXMax],eax
DeltaX_Pos:						; EDI = ABS(Delta_X)

			mov	[ScreenYMin],ebx
			mov	[ScreenYMax],edx

			mov	esi,edx
			mov	ebp,[ScreenPitch]	; EBP = Incr_Y

			sub	esi,ebx
			jns	DeltaY_Pos

			neg	esi
			neg	ebp

			mov	[ScreenYMin],edx
			mov	[ScreenYMax],ebx
DeltaY_Pos:						; ESI = ABS(Delta_Y)
			shl	ebx,2
			add	ebx,[PTR_TabOffLine]

			mov	ebx,[ebx]

			add	ebx,eax			; EBX = start offset

			cmp	edi,esi
			jb	Draw_Y

							; Delta_X > Delta_Y
			mov	dword ptr[DELTA_Y+2],esi
			mov	dword ptr[DELTA_X+2],edi
			mov	dword ptr[DELTA_X1+2],edi

			mov	eax,[DZ]

			test	ecx,ecx
			jns	Pos1

			neg	eax
Pos1:
			mov	edx,eax			; compute Z gradient
			sar	edx,31
			idiv	edi

			mov	edx,eax
			shr	edx,31
			add	eax,edx

			mov	edx,eax
			shl	eax,16
			sar	edx,16
			mov	dword ptr[DELTA_Z+2],eax
			mov	dword ptr[IZ+2],edx

			mov	dword ptr[Line+2],ebp
			mov	dword ptr[Col+2],ecx

			mov	eax,[Log]
			mov	edx,[PtrZBuffer]
			mov	dword ptr[PtrLog+2],eax
			mov	dword ptr[PtrZBuf+4],edx


			mov	edx,edi
			mov	ebp,edi
			shr	edx,1

			xor	eax,eax
			mov	esi,[CurrentZ1]
			mov	edi,esi
			shl	esi,16
			mov	al,[Color]
			shr	edi,16

			sub	edi,RAIN_RANGE/2
			xor	ecx,ecx

			jmp	$+2
Do_X:
PtrZBuf:		mov	cx,[ebx*2+12345678h]

			sub	ecx,edi
			js	NoDisp

;			cmp	esi,RAIN_RANGE
			cmp	ecx,RAIN_RANGE
			jge	Disp

			or	ah,2
Disp:
			or	ah,1
PtrLog:			mov	[ebx+12345678h],al
NoDisp:
DELTA_Y:		add	edx,12345678h

DELTA_X:		cmp	edx,12345678h
			jb	Plot_X

DELTA_X1:		sub	edx,12345678h
Line:			add	ebx,12345678h
Plot_X:
Col:			add	ebx,12345678h
			xor	ecx,ecx

DELTA_Z:		add	esi,12345678h
IZ:			adc	edi,12345678h

			dec	ebp
			jge	Do_X

			shr	eax,8
			pop	ebp

			ret
Draw_Y:
			mov	dword ptr [YDELTA_X+2],edi
			mov	dword ptr [YDELTA_Y+2],esi
			mov	dword ptr [YDELTA_Y1+2],esi

			mov	eax,[DZ]

			test	ebp,ebp
			jns	Pos2

			neg	eax
Pos2:
			mov	edx,eax
			sar	edx,31
			idiv	esi

			mov	edx,eax
			shr	edx,31
			add	eax,edx

			mov	edx,eax
			shl	eax,16
			sar	edx,16
			mov	dword ptr [YDELTA_Z+2],eax
			mov	dword ptr [YIZ+2],edx

			mov	dword ptr [Col2+2],ecx
			mov	dword ptr [Line2+2],ebp

			mov	eax,[Log]
			mov	edx,[PtrZBuffer]
			mov	dword ptr [YPtrLog+2],eax
			mov	dword ptr [YPtrZBuf+4],edx

			mov	edx,esi
			mov	ecx,esi
			shr	edx,1

			mov	esi,[CurrentZ1]
			xor	eax,eax
			mov	edi,esi
			mov	al,[Color]
			shl	esi,16
			shr	edi,16

			sub	edi,RAIN_RANGE/2
			xor	ebp,ebp

			jmp	$+2
Do_Y:
YPtrZBuf:		mov	bp,[ebx*2+12345678h]

			sub	ebp,edi
			js	YNoDisp

			cmp	ebp,RAIN_RANGE
			jge	YDisp

			or	ah,2
YDisp:
			or	ah,1
YPtrLog:		mov	[ebx+12345678h],al
YNoDisp:
YDELTA_X:		add	edx,12345678h

YDELTA_Y:		cmp	edx,12345678h
			jb	Plot_Y

YDELTA_Y1:		sub	edx,12345678h
Col2:			add	ebx,12345678h
Plot_Y:
Line2:			add	ebx,12345678h
			xor	ebp,ebp

YDELTA_Z:		add	esi,12345678h
YIZ:			adc	edi,12345678h

			dec	ecx
			jge	Do_Y

			shr	eax,8
			pop	ebp

			ret
Exit:
			mov	ebx,7FFFFFFFh
			xor	eax,eax

			mov	[ScreenXMin],ebx
			mov	[ScreenYMin],ebx

			inc	ebx
			pop	ebp

			mov	[ScreenXMax],ebx
			mov	[ScreenYMax],ebx

			ret

Clip_X0:						; Case X1<XMin
			cmp	ecx,[ClipXMin]
			jl	Exit

			push	edx
			sub	eax,[ClipXMin]

			push	eax

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ1],eax		; z1-=(x1-XMin)*dz/dx
			pop	eax

			imul	esi
			idiv	edi

			sub	ebx,eax			; y1-=(x1-XMin)*dy/dx
			mov	eax,[ClipXMin]		; x1 = ClipXMin

			pop	edx
			jmp	Trace_Still
Clip_X1:						; Case X1>XMax
			cmp	ecx,[ClipXMax]
			jg	Exit

			push	edx
			sub	eax,[ClipXMax]

			push	eax

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ1],eax		; z1-=(x1-XMax)*dz/dx
			pop	eax

			imul	esi
			idiv	edi

			sub	ebx,eax			; y1-=(x1-XMax)*dy/dx
			pop	edx

			mov	eax,[ClipXMax]		; x1 = ClipXMax
			jmp	Trace_Still

Clip_X2:						; Case X2<XMin
			push	eax
			push	edx

			mov	eax,ecx
			sub	eax,[ClipXMin]

			push	eax

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ2],eax		; z2-=(x2-XMin)*dz/dx
			pop	eax

			imul	esi
			idiv	edi

			pop	edx
			mov	ecx,[ClipXMin]		; x2 = ClipXMin

			sub	edx,eax			; y2-=(x2-XMin)*dy/dx
			pop	eax

			jmp	Trace_Still

Clip_X3:						; Case X2>XMax
			push	eax
			mov	eax,ecx

			push	edx
			sub	eax,[ClipXMax]

			push	eax

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ2],eax		; z2-=(x2-XMax)*dz/dx
			pop	eax

			imul	esi
			idiv	edi

			pop	edx
			mov	ecx,[ClipXMax]		; x2 = ClipXMax

			sub	edx,eax			; y2-=(x2-XMax)*dy/dx
			pop	eax

			jmp	Trace_Still

Clip_Y0:						; Case Y1<YMin
			cmp	edx,[ClipYMin]
			jl	Exit

			push	edx
			push	eax

			mov	eax,ebx

			sub	eax,[ClipYMin]

			push	eax

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ1],eax		; z1-=(y1-YMin)*dz/dy
			pop	eax

			imul	edi
			idiv	esi

			mov	edx,eax
			pop	eax

			sub	eax,edx			; x1-=(y1-YMin)*dz/dy
			mov	ebx,[ClipYMin]		; y1 = ClipYMin

			pop	edx
			jmp	Trace_Still

Clip_Y1:						; Case Y1>YMax
			cmp	edx,[ClipYMax]
			jg	Exit

			push	edx
			push	eax

			mov	eax,ebx

			sub	eax,[ClipYMax]

			push	eax

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ1],eax		; z1-=(y1-YMax)*dz/dy
			pop	eax

			imul	edi
			idiv	esi

			mov	edx,eax
			pop	eax

			sub	eax,edx			; x1-=(y1-YMax)*dz/dy
			mov	ebx,[ClipYMax]		; y1 = ClipYMax

			pop	edx
			jmp	Trace_Still

Clip_Y2:						; Case Y2<YMin
			push	eax
			mov	eax,edx

			sub	eax,[ClipYMin]

			push	eax

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ2],eax		; z2-=(y2-YMin)*dz/dy
			pop	eax

			imul	edi
			idiv	esi

			sub	ecx,eax			; x2-=(y2-YMin)*dz/dy
			mov	edx,[ClipYMin]		; y2 = ClipYMin

			pop	eax
			jmp	Trace_Still

Clip_Y3:						; Case Y2>YMax
			push	eax
			mov	eax,edx

			sub	eax,[ClipYMax]

			push	eax

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ2],eax		; z2-=(y2-YMax)*dz/dy
			pop	eax

			imul	edi
			idiv	esi

			sub	ecx,eax			; x2-=(y2-YMax)*dz/dy
			mov	edx,[ClipYMax]		; y2 = ClipYMax

			pop	eax
			jmp	Trace_Still

							; *** DeltaX nul ***
DX_Nul:
			cmp	eax,[ClipXMin]
			jl	Exit
			cmp	eax,[ClipXMax]
			jg	Exit

			sub	esi,ebx			; ESI = DELTA_Y
			jz	Plot

			cmp	ebx,[ClipYMin]
			jge	DX_Y1
			cmp	edx,[ClipYMin]
			jl	Exit

			push	edx
			push	eax

			mov	eax,ebx
			sub	eax,[ClipYMin]

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ1],eax		; z1-=(y1-YMin)*dz/dy
			pop	eax

			pop	edx
			mov	ebx,[ClipYMin]		; y1 = YMin

			mov	esi,edx

			sub	esi,ebx			; ESI = DELTA_Y
			jz	Plot

DX_Y1:
			cmp	ebx,[ClipYMax]
			jle	DX_Y1b
			cmp	edx,[ClipYMax]
			jg	Exit

			push	edx
			push	eax

			mov	eax,ebx
			sub	eax,[ClipYMax]

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ1],eax		; z1-=(y1-YMax)*dz/dy
			pop	eax

			pop	edx
			mov	ebx,[ClipYMax]		; y1 = YMax

			mov	esi,edx

			sub	esi,ebx			; ESI = DELTA_Y
			jz	Plot
DX_Y1b:
			cmp	edx,[ClipYMin]
			jge	DX_Y2

			push	eax
			mov	eax,edx

			sub	eax,[ClipYMin]

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ2],eax		; z2-=(y2-YMin)*dz/dy
			mov	edx,[ClipYMin]		; y2 = YMin

			pop	eax
			mov	esi,edx

			sub	esi,ebx			; ESI = DELTA_Y
			jz	Plot
DX_Y2:
			cmp	edx,[ClipYMax]
			jle	Draw_Line

			push	eax
			mov	eax,edx

			sub	eax,[ClipYMax]

			imul	[DZ]
			idiv	esi

			sub	[CurrentZ2],eax		; z2-=(y2-YMax)*dz/dy
			mov	edx,[ClipYMax]		; y2 = YMax

			mov	esi,edx
			pop	eax

			sub	esi,ebx			; ESI = DELTA_Y
			jnz	Draw_Line

			jmp	Plot
							; *** DeltaY nul ***
DY_Nul:
			cmp	ebx,[ClipYMin]
			jl	Exit
			cmp	ebx,[ClipYMax]
			jg	Exit

			cmp	eax,[ClipXMin]
			jge	DY_X1
			cmp	ecx,[ClipXMin]
			jl	Exit

			push	edx
			sub	eax,[ClipXMin]

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ1],eax		; z1-=(x1-XMin)*dz/dx
			mov	edi,ecx

			mov	eax,[ClipXMin]		; x1 = XMin
			pop	edx

			sub	edi,eax
			jz	Plot

DY_X1:
			cmp	eax,[ClipXMax]
			jle	DY_X1b
			cmp	ecx,[ClipXMin]
			jg	Exit

			push	edx
			sub	eax,[ClipXMax]

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ1],eax		; z1-=(x1-XMax)*dz/dx
			mov	edi,ecx

			mov	eax,[ClipXMax]		; x1 = XMax
			pop	edx

			sub	edi,eax
			jz	Plot
DY_X1b:

			cmp	ecx,[ClipXMin]
			jge	DY_X2

			push	eax
			mov	eax,ecx

			push	edx
			sub	eax,[ClipXMin]

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ2],eax		; z2-=(x2-XMin)*dz/dx
			mov	ecx,[ClipXMin]		; x2 = XMin

			pop	edx
			mov	edi,ecx

			pop	eax

			sub	edi,eax
			jz	Plot
DY_X2:
			cmp	ecx,[ClipXMax]
			jle	Draw_Line

			push	eax
			mov	eax,ecx

			push	edx
			sub	eax,[ClipXMax]

			imul	[DZ]
			idiv	edi

			sub	[CurrentZ2],eax		; z2-=(x2-XMax)*dz/dx
			mov	ecx,[ClipXMax]		; x2 = XMax

			pop	edx
			mov	edi,ecx

			pop	eax

			sub	edi,eax
			jnz	Draw_Line
							; *** Plot ***
Plot:
			cmp	ebx,[ClipYMin]
			jl	Exit
			cmp	ebx,[ClipYMax]
			jg	Exit
			cmp	eax,[ClipXMin]
			jl	Exit
			cmp	eax,[ClipXMax]
			jg	Exit

			mov	[ScreenXMin],eax
			mov	[ScreenYMin],ebx
			mov	[ScreenXMax],eax
			mov	[ScreenYMax],ebx

			shl	ebx,2
			mov	edi,[PTR_TabOffLine]

			add	ebx,edi
			mov	edi,[Log]

			mov	edx,[CurrentZ1]
			mov	esi,[PtrZBuffer]

			sar	edx,16
			mov	ebx,[ebx]

			add	ebx,eax			; EBX = start offset
			xor	eax,eax

			xor	ebp,ebp
			mov	al,[Color]

			mov	bp,word ptr[ebx*2+esi]
			sub	edx,RAIN_RANGE/2

			sub	ebp,edx
			js	Exit

			cmp	ebp,RAIN_RANGE
			jge	PlotDisp

			or	ah,2
PlotDisp:		mov	[ebx+edi],al

	        	or	ah,1
			pop	ebp

			shr	eax, 8

			ret
EndLineRain::

LineRain		ENDP

;			The
			End
