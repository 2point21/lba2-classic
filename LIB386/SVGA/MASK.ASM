;----------------------------------------------------------------------------
			.386p
			.model SMALL, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			EXTRN   SYSCALL      ClipXMin        :DWORD
			EXTRN   SYSCALL      ClipYMin        :DWORD
			EXTRN   SYSCALL      ClipXMax        :DWORD
			EXTRN   SYSCALL      ClipYMax        :DWORD
			EXTRN   SYSCALL      ScreenXMin      :DWORD
			EXTRN   SYSCALL      ScreenYMin      :DWORD
			EXTRN   SYSCALL      ScreenXMax      :DWORD
			EXTRN   SYSCALL      ScreenYMax      :DWORD
			EXTRN   SYSCALL      Log             :DWORD
			EXTRN   SYSCALL      TabOffLine      :DWORD

;----------------------------------------------------------------------------
comment @
		DWORD   TabOffset[]
			Brick:
			    - BYTE Delta X
			    - BYTE Delta Y
				Line(Delta Y):
				- BYTE NbBlock
				   Block 0:     Nb Zero to Jump
				   Block 1:     Nb Zero to Write
				   Block 2:     Nb Zero to Jump
					etc...
@

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	ColMask

OffsetBegin     dd      0

ColMask         db      0       ; Noir par Defaut

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			PUBLIC  SYSCALL      AffMask
			PUBLIC  SYSCALL      GetDxDyMask

;----------------------------------------------------------------------------
;#define	ColorMask(color)	ColMask = (U8)(color)

;----------------------------------------------------------------------------
;S32	AffMask(S32 nummask, S32 x, S32 y, void *bankmask)	;

;#pragma aux AffMask	"*"		\
;	parm	[eax] [ebx] [ecx] [esi]	\
;	modify	[edx edi]

AffMask                 proc    uses ebp

			add     esi, [esi+eax*4]; ESI = Begin Data

			xor     eax, eax
			xor	edx, edx
			mov     al, [esi+2]
			mov     dl, [esi+3]
			add     ebx, eax        ; Hot X
			add     ecx, edx        ; Hot Y

;-----------------------------------------------
			mov     dl, [esi]       ; Delta X
			mov     al, [esi+1]     ; Nb Lines ( Delta Y )
			add     esi, 4          ; Jump HotX & HotY

;----------------------------------------------- Test Clipping
			add     edx, ebx
			add     eax, ecx
			dec     edx
			dec     eax

			cmp     ebx, ClipXMin
			jl      ClippingMask
			cmp     ecx, ClipYMin
			jl      ClippingMask
			cmp     edx, ClipXMax
			jg      ClippingMask
			cmp     eax, ClipYMax
			jg      ClippingMask

			mov     [ScreenXMin],ebx
			mov     [ScreenXMax],edx
			mov     [ScreenYMin],ecx
			mov     [ScreenYMax],eax

			inc     edx
			inc     eax
			sub     edx, ebx
			sub     eax, ecx

;----------------------------------------------- Compute Offset Ecran
			mov     edi, Log
			sub     edx, TabOffLine[1*4]; EDX = Offset Screen

			add     edi, TabOffLine[ecx*4]
			xor     ecx, ecx        ; Raz Compteur

			add     edi, ebx
			mov     bh, al          ; BH = NbLine

			neg     edx             ; EDX = width-edx

			mov     al, byte ptr [ColMask]; mask color
			mov     ebp, edx        ; EBP Offset Next Line

			mov     ah, al

;----------------------------------------------- Init NbBlock for this line
NextLine:               mov     bl, [esi]       ; BL = Nb Block for this line
			inc     esi

;----------------------------------------------- Manage One Line
SameLine:               mov     cl, [esi]       ; Nb Zero to Jump
			add     edi, ecx        ; Incrust
			dec     bl
			je      EndBlock
			mov     cl, [esi+1]     ; Nb Zero to Write
			add     esi, 2
;			shr     ecx,1
			db	0C1h, 0E9h, 001h
			rep     stosw           ; Write Zero
			jnc     nobyte
			mov	[edi],al
			inc	edi
nobyte:                 dec     bl              ; Nb Block--
			jne     SameLine        ; Continue Same Line
			add     edi, ebp        ; EDI += Offset Next Line
			dec     bh              ; NbLine--
			jne     NextLine        ; Next Line
			ret
EndBlock:
			inc     esi
			add     edi, ebp        ; EDI += Offset Next Line
			dec     bh              ; NbLine--
			jne     NextLine        ; Next Line
			ret

;-------------------------------------------------------------------- Clipping
;               Graph : ( EBX, ECX ) ( EDX, EAX )
ClippingMask:
			cmp     ebx, ClipXMax
			jg      EndMask
			cmp     ecx, ClipYMax
			jg      EndMask
			cmp     edx, ClipXMin
			jl      EndMask
			cmp     eax, ClipYMin
			jl      EndMask

			cmp     ecx, ClipYMin
			jge     PasHaut

;---------------------- Clipping Haut, Saute ClipYMin-ECX Line(s)

			sub     ecx, ClipYMin
			mov     ebp, eax
			neg     ecx
			xor     eax, eax

NextH:                  mov     al, [esi]               ; NbBlock
			dec     ecx
			lea     esi, [esi+1+eax]        ; Jump Data
			jnz     NextH

			mov     eax, ebp

			mov     ecx, ClipYMin           ; New Y Haut

;---------------------- Clipping Bas
PasHaut:                cmp     eax, ClipYMax
			jle     PasBas
			mov     eax, ClipYMax
;---------------------- Clipping Gauche
PasBas:                 mov     OffsetBegin, 0
			cmp     ebx, ClipXMin
			jge     PasGauche
			push    eax
			mov     eax, ClipXMin
			sub     eax, ebx
			mov     OffsetBegin, eax
			pop     eax
;---------------------- Clipping Droit
PasGauche:              mov     ebp, edx
			sub     ebp, ebx
			sub     ebp, OffsetBegin
			inc     ebp

			cmp     edx, ClipXMax
			jle     PasDroit

			sub     edx, ClipXMax
			sub     ebp, edx
			mov     edx, ClipXMax
;----------------------
PasDroit:
			mov     [ScreenXMax],edx
			mov	edx,[OffsetBegin]

			add	edx,ebx
			mov     [ScreenYMin],ecx

			mov     [ScreenXMin],edx
			mov     [ScreenYMax],eax


			mov     edi, Log
			add     edi, TabOffLine[ecx*4]
			add     edi, ebx		; EDI = Buffer dest
			add	edi,[OffsetBegin]

			sub     eax, ecx
			inc     al
			mov     bh, al                  ; BH NbLine

;----------------------------------------------
			mov     al, byte ptr [ColMask]
			xor     ecx, ecx
			mov     ah, al			; AX = Col:Col

							; ESI = DATAS LINE
NextL:
			push	edi
			push	ebp
			mov     bl,[esi]		; NbBlock
			inc     esi
			mov	edx,[OffsetBegin]	; edx = Nb d'octets a sauter
Clipper:
			mov	cl,[esi]		; Nb transp
			inc	esi
			dec	bl
			je	End_Line
			sub	edx,ecx
			js	Start_Transp

			mov	cl,[esi]		; Nb Fill
			inc	esi
			sub	edx,ecx
			js	Start_Fill
			dec	bl
			jne	Clipper
			jmp	End_Line
Start_Transp:
			add	ebp,edx
			jle	End_Line
			sub	edi,edx
			jmp	Do_Fill
Start_Fill:
			neg	edx
			cmp	ebp,edx
			jge	ok
			mov	edx,ebp
ok:			sub	ebp,edx
			mov	ecx,edx
			jmp	Do_Fill_1

SameL:                  mov     cl,[esi]
			inc	esi
			dec     bl
			je      End_Line
			sub	ebp,ecx
			jle	End_Line
			add	edi,ecx
Do_Fill:
			mov     cl,[esi]
			inc	esi
			cmp	ebp,ecx
			jge	ok2
			mov	ecx,ebp
ok2:			sub	ebp,ecx
Do_Fill_1:
;			shr     ecx,1
			db	0C1h, 0E9h, 001h
			rep     stosw
			jnc     nobyte2
			mov	[edi],al
			inc	edi
nobyte2:                dec     bl
			jne     SameL

End_Line:		mov	cl,bl
			pop	ebp
			pop	edi
			add     edi,TabOffLine[1*4]
			add	esi,ecx
			dec     bh
			jne     NextL

;----------------------
EndMask:                ret

AffMask			endp

;----------------------------------------------------------------------------
;S32	GetDxDyMask(S32 nummask, S32 *x, S32 *y, void *bankmask);

;#pragma aux GetDxDyMask	"*"		\
;	parm		[eax] [ebx] [ecx] [esi]	\
;	modify exact	[eax edx esi]

GetDxDyMask             proc

			add     esi, [esi+eax*4]
			xor     eax, eax
			xor     edx, edx
			mov     al, [esi]
			mov     dl, [esi+1]
			mov     [ebx], eax
			mov     [ecx], edx

			ret

GetDxDyMask             endp

;----------------------------------------------------------------------------
;			The
			End






































