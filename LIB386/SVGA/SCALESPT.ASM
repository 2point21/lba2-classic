;*══════════════════════════════════════════════════════════════════════════*/
		.386p
		.model FLAT, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*/
Struc_Sprite	STRUC
  Delta_X	db	0
  Delta_Y	db	0
  Hot_X		db	0
  Hot_Y		db	0
Struc_Sprite	ENDS

;*══════════════════════════════════════════════════════════════════════════*/
		.DATA

;*══════════════════════════════════════════════════════════════════════════*/
		extrn	SYSCALL	Log		:DWORD
		extrn	SYSCALL	TabOffLine	:DWORD

		extrn	SYSCALL	ScreenXMin	:DWORD
		extrn	SYSCALL	ScreenXMax	:DWORD
		extrn	SYSCALL	ScreenYMin	:DWORD
		extrn	SYSCALL	ScreenYMax	:DWORD

		extrn	SYSCALL	ClipXMin	:DWORD
		extrn	SYSCALL	ClipXMax	:DWORD
		extrn	SYSCALL	ClipYMin	:DWORD
		extrn	SYSCALL	ClipYMax	:DWORD

		extrn	SYSCALL	ModeDesiredX	:DWORD

;*══════════════════════════════════════════════════════════════════════════*/
Start		dd	0000

Start_X		dd	0
Start_Y		dd	0
Skip_Next_Line	dd	0

;*══════════════════════════════════════════════════════════════════════════*/
		.CODE

;*══════════════════════════════════════════════════════════════════════════*/
		public   SYSCALL     ScaleSpriteTransp

;*══════════════════════════════════════════════════════════════════════════*/
IFDEF	_WIN32
		extrn  SYSCALL	AllowPatchAndPatch:PROC
ENDIF

;*══════════════════════════════════════════════════════════════════════════*/
IFDEF	_WIN32

ScaleSpriteTranspInit proc

		push	offset ScaleSpriteTranspPatch
		push	offset ScaleSpriteTranspEnd
		push	offset Patch_Jump

		jmp	AllowPatchAndPatch
Patch_Jump::
		mov	dword ptr [Ptr_Transp+2],edx

ScaleSpriteTranspInit endp

ENDIF

;*══════════════════════════════════════════════════════════════════════════*/
ScaleSpriteTransp	proc uses ebp,				\
		numbrick:	DWORD, 				\
		xbrick:		DWORD,	ybrick:		DWORD,	\
		factorx:	DWORD,	factory:	DWORD,	\
		bankbrick:	DWORD,  PtrTransp:	DWORD

		mov	edx,[PtrTransp]

ScaleSpriteTranspPatch::
		mov	dword ptr [Ptr_Transp+2],edx
IFDEF	_WIN32
x:
		org	ScaleSpriteTranspPatch

		jmp	NEAR PTR ScaleSpriteTranspInit; 5 bytes

		org	x
ELSE
		mov	dword ptr [Ptr_Transp+2],edx
ENDIF
		mov	dword ptr [Ptr_Transp1+2],edx

		mov	eax,[factorx]
		mov	ebx,[factory]

		mov	ecx,[numbrick]
		mov	esi,[bankbrick]

		test	eax,eax
		jle	@@CareFactorX
okx:

		cmp	eax,65536
		je	@@TestFastMode
@@NoFastMode:
		test	ebx,ebx
		jle	@@CareFactorY
oky:
		mov	edx,[esi+ecx*4]
		xor	eax,eax

		add	esi,edx

		mov	[Start],esi
		mov	al,[esi].Delta_X

;		movzx	eax,[esi].Delta_X
		imul	[factorx]

		shl	edx,16
		mov	ecx,eax

		shr	ecx,16
		mov	eax,dword ptr [esi].Delta_X

		shl	eax,8
		or	ecx,edx			; ECX = New Delta_X

		sar	eax,24

;		movsx	eax,[esi].Hot_X
		imul	[factorx]

		shl	edx,16
		mov	ebx,eax

		shr	ebx,16
		xor	eax,eax

		or	ebx,edx			; EBX = New Hot_X
		mov	edx,1

		idiv	[factorx]

		mov	[factorx],eax		; Inverse factorx
						; factorx now contains the gradiant
						; for x interpolation
		mov	edx,dword ptr [esi].Delta_X

		and	edx,0FFh
		mov	edi,dword ptr [esi].Delta_X

		shr	edx,1

		shl	edx,16

		shr	edi,1
		jnc	@@NoX

		add	edx,8000h
@@NoX:		mov	edi,ecx

		shr	edi,1

		imul	eax,edi

		sub	edx,eax
		mov	edi,[xbrick]		; gets sprite posX

		mov	[Start_X],edx
		add	edi,ebx			; EDI = Left_X

		mov	eax,[ClipXMax]
		mov	ebx,[ClipXMin]

		inc	eax
		mov	[ScreenXMin],edi	; Offset in Log screen (X coord)

		cmp	edi,eax			; Out of the screen ?
		jg	exit

		cmp	edi,[ClipXMin]
		jge	No_ClipXMin

		mov	[ScreenXMin],ebx
		dec	ebx

		mov	edx,[Start_X]
		sub	ebx,edi

		imul	ebx,[factorx]

		add	edx,ebx
		mov	ebx,[ClipXMin]

		add	[Start_X],edx		; 16:16
No_ClipXMin:
		add	edi,ecx			; EDI = Right_X

		cmp	edi,ebx
		jl	exit

		mov	[ScreenXMax],edi

		cmp	edi,eax
		jl	No_ClipXMax
		mov	[ScreenXMax],eax
No_ClipXMax:
		mov	eax,dword ptr [esi].Delta_X

		shr	eax,8

		and	eax,0FFh

;		movzx	eax,[esi].Delta_X
		imul	[factory]

		shl	edx,16
		mov	ecx,eax

		shr	ecx,16
		mov	eax,dword ptr [esi].Delta_X

		sar	eax,24
		or	ecx,edx			; ECX = New Delta_Y

;		movsx	eax,[esi].Hot_Y
		imul	[factory]

		shl	edx,16
		mov	ebx,eax

		shr	ebx,16
		xor	eax,eax

		or	ebx,edx			; EBX = New Hot_Y
		mov	edx,1

		idiv	[factory]

		mov	[factory],eax		; Inverse factory
		mov	edx,dword ptr [esi].Delta_Y

		and	edx,0FFh
		mov	edi,dword ptr [esi].Delta_Y

		shr	edx,1

		shl	edx,16

		shr	edi,1
		jnc	@@NoY

		add	edx,8000h
@@NoY:		mov	edi,ecx

		shr	edi,1

		imul	eax,edi

		sub	edx,eax
		mov	edi,[ybrick]		; gets sprite posY

		mov	[Start_Y],edx
		add	edi,ebx			; EDI = Top Y

		mov	eax,[ClipYMax]
		mov	ebx,[ClipYMin]

		inc	eax
		mov	[ScreenYMin],edi	; Offset in Log screen (Y coord)

		cmp	edi,eax			; Out of the screen ?
		jg	exit

		cmp	edi,ebx
		jge	No_ClipYMin

		mov	[ScreenYMin],ebx
		dec	ebx

		mov	edx,[Start_Y]
		sub	ebx,edi

		imul	ebx,[factory]

		add	edx,ebx
		mov	ebx,[ClipYMin]

		add	[Start_Y],edx		; 16:16
No_ClipYMin:
		add	edi,ecx			; EDI = Low_Y

		mov	[ScreenYMax],edi

		cmp	edi,[ClipYMin]
		jle	exit

		cmp	edi,eax
		jle	No_ClipYMax
		mov	[ScreenYMax],eax
No_ClipYMax:

		mov	edi,[Log]
		mov	eax,[ScreenYMin]

		add	edi,[ScreenXMin]
		mov	ebx,TabOffLine[eax*4]

		mov	eax,[Start_Y]
		add	edi,ebx			; EDI = PTR in Log screen

				; Get pointer on sprite's raw buffer
				; according to the first scanline to be drawn
		shr	eax,16
		mov	ebx,dword ptr [esi].Delta_X

		and	ebx,0FFh

		imul	eax,ebx

		mov	[Skip_Next_Line],ebx

		lea	esi,[esi+eax+Size Struc_Sprite]
						; ESI = PTR in RAW
		mov	eax,[factorx]

		shl	eax,16
		mov	ebx,[factorx]

		shr	ebx,16
		mov	dword ptr [X_Dec+2],eax

		mov	dword ptr [X_Int+2],ebx
		mov	eax,[factory]

		shl	eax,16
		mov	ebx,[factory]

		shr	ebx,16
		mov	dword ptr [Y_Dec+2],eax

		mov	dword ptr [Y_Int+2],ebx
		mov	ebp,[ScreenYMax]

		mov	eax,[ScreenYMin]
		mov	ecx,[ScreenXMax]

		sub	ebp,eax			; EBP = Delta Y
		jle	exit

		mov	eax,[Start_X]
		mov	edx,[ScreenXMin]

		sub	ecx,edx			; ECX = Delta X
		jle	exit

		mov	edx,eax
		mov	ebx,[Start_Y]

		shr	eax,16

		add	esi,eax			; ESI = ESI + (Clipped Zone)

		shl	edx,16

		shl	ebx,16
		mov	[Start_X],edx

		xor	edx,edx
		jmp	$+2

Each_Line:
		push	ecx			; ECX = DeltaX
		push	esi			; ESI = Sprite's raw

		push	edi			; EDI = Log
		push	ebx			; EBX = Fract. part of StartY

		mov	ebx,[Start_X]		; EBX = Fract. part of StartX
		xor	eax,eax
Each_Col:
		mov	ah,[esi]
X_Dec:		add	ebx,12345678h

X_Int:		adc	esi,12345678h
		mov	al,[edi]

		test	eax,0FF00h
		je	Opacity

Ptr_Transp::	mov	al,[eax+12345678h]

		mov	[edi],al
Opacity:	inc	edi

		dec	ecx
		jne	Each_Col

		pop	ebx
		pop	edi

		pop	esi
Y_Dec:		add	ebx,12345678h

Y_Int:		adc	edx,12345678h
		pop	ecx

		mov	eax,[Skip_Next_Line]
		je	Next_Line
Skip_Line:
		add	esi,eax
		dec	edx
		jne	Skip_Line
Next_Line:
		add	edi,[ModeDesiredX]

		dec	ebp
		jne	Each_Line

		ret
		ALIGN	4
exit:
		mov	[ScreenXMin], 32000
		mov	[ScreenXMax], -32000
		mov	[ScreenYMin], 32000
		mov	[ScreenYMax], -32000

		ret
@@CareFactorX:
		je	exit
		mov	[factorx],7FFFFFFFh
		jmp	okx
@@CareFactorY:
		je	exit
		mov	[factory],7FFFFFFFh
		jmp	oky


@@TestFastMode:
		cmp	ebx,65536
		jne	@@NoFastMode

		mov	edx,[esi+ecx*4]
		mov	edi,[xbrick]

		add	esi,edx
		mov	eax,[ClipXMax]

		; AGI

		inc	eax
		mov	ecx,dword ptr [esi].Delta_X

		shl	ecx,8
		mov	ebx,[ClipXMin]

		sar	ecx,24
		mov	edx,dword ptr [esi].Delta_X

		add	edi,ecx
		and	edx,0FFh

		cmp	edi,eax
		jg	exit

		mov	[ScreenXMin],edi
		mov	[Start_X],0

		cmp	edi,ebx
		jge	@@NoClipXMin

		push	ebx
		mov	[ScreenXMin],ebx

		sub	ebx,edi

		mov	[Start_X],ebx
		pop	ebx
@@NoClipXMin:
		add	edi,edx

		mov	[ScreenXMax],edi

		cmp	edi,ebx
		jl	exit

		cmp	edi,eax
		jl	@@NoClipXMax

		mov	[ScreenXMax],eax
@@NoClipXMax:
		mov	edi,[ybrick]
		mov	eax,[ClipYMax]

		mov	ecx,dword ptr [esi].Delta_X
		mov	ebx,[ClipYMin]

		sar	ecx,24
		add	edi,ecx

		inc	eax
		mov	[ScreenYMin],edi

		cmp	edi,eax
		jg	exit

		mov	[Start_Y],0

		cmp	edi,ebx
		jg	@@NoClipYMin

		push	ebx
		mov	[ScreenYMin],ebx

		sub	ebx,edi

		mov	[Start_Y],ebx
		pop	ebx
@@NoClipYMin:
		mov	edx,dword ptr [esi].Delta_X

		shr	edx,8

		and	edx,0FFh

		add	edi,edx

		cmp	edi,ebx
		jl	exit

		mov	[ScreenYMax],edi

		cmp	edi,eax
		jl	@@NoClipYMax

		mov	[ScreenYMax],eax

@@NoClipYMax:
		mov	ebp,dword ptr [esi].Delta_X
		mov	eax,[ScreenYMin]

		and	ebp,0FFh
		mov	ebx,[ScreenXMin]

		mov	ecx,[ScreenXMax]
		mov	edx,[ScreenYMax]

		sub	ecx,ebx
		jle	exit

		sub	edx,eax
		jle	exit

		mov	edi,[Log]
		mov	eax,TabOffLine[eax*4]

		add	edi,eax
		add	esi,Size Struc_Sprite

		add	edi,ebx
		mov	ebx,[Start_X]

		mov	eax,[Start_Y]

		test	eax,eax
		je	@@NoSkip
		imul	eax,ebp
		add	esi,eax
@@NoSkip:
		add	esi,ebx
		mov	ebx,[ModeDesiredX]

		sub	ebp,ecx
		sub	ebx,ecx

		xor	eax,eax
@@LoopY:
		push	ecx
@@LoopX:
		mov	ah,[esi]
		inc	esi

		mov	al,[edi]

		test	eax,0FF00h
		je	@@Hide
						; AGI
Ptr_Transp1:	mov	al,[eax+12345678h]

		mov	[edi],al
@@Hide:		inc	edi

		dec	ecx
		jne	@@LoopX

		pop	ecx
		add	esi,ebp

		add	edi,ebx

		dec	edx
		jne	@@LoopY

		ret

ScaleSpriteTranspEnd::

ScaleSpriteTransp	endp

;*══════════════════════════════════════════════════════════════════════════*/
;                       The
			End
