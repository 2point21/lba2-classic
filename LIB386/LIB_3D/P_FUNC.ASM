
		.386p
		jumps

		.model  SMALL, SYSCALL

		include P_define.ash

		.data

		include \projet\lib386\lib_sys\lib_sys.ash

Comment @

SqrTab  dw      0807Fh,0817Dh,08279h,08374h,0846Ch,08562h,08657h,0874Ah
	dw      0883Ch,0892Bh,08A19h,08B06h,08BF1h,08CDAh,08DC2h,08EA9h
	dw      08F8Eh,09071h,09153h,09234h,09314h,093F2h,094CFh,095AAh
	dw      09684h,0975Dh,09835h,0990Ch,099E2h,09AB6h,09B89h,09C5Bh
	dw      09D2Ch,09DFCh,09ECBh,09F99h,0A066h,0A132h,0A1FCh,0A2C6h
	dw      0A38Fh,0A457h,0A51Eh,0A5E4h,0A6A9h,0A76Dh,0A830h,0A8F3h
	dw      0A9B4h,0AA75h,0AB35h,0ABF4h,0ACB2h,0AD6Fh,0AE2Ch,0AEE7h
	dw      0AFA2h,0B05Ch,0B116h,0B1CFh,0B286h,0B33Eh,0B3F4h,0B4AAh
	dw      0B55Fh,0B613h,0B6C7h,0B77Ah,0B82Ch,0B8DEh,0B98Fh,0BA3Fh
	dw      0BAEEh,0BB9Dh,0BC4Ch,0BCF9h,0BDA6h,0BE53h,0BEFFh,0BFAAh
	dw      0C055h,0C0FFh,0C1A8h,0C251h,0C2FAh,0C3A1h,0C449h,0C4EFh
	dw      0C595h,0C63Bh,0C6E0h,0C784h,0C828h,0C8CCh,0C96Fh,0CA11h
	dw      0CAB3h,0CB55h,0CBF5h,0CC96h,0CD36h,0CDD5h,0CE74h,0CF13h
	dw      0CFB1h,0D04Eh,0D0EBh,0D188h,0D224h,0D2C0h,0D35Bh,0D3F6h
	dw      0D490h,0D52Ah,0D5C4h,0D65Dh,0D6F5h,0D78Eh,0D825h,0D8BDh
	dw      0D954h,0D9EAh,0DA81h,0DB16h,0DBACh,0DC41h,0DCD5h,0DD69h
	dw      0DDFDh,0DE91h,0DF24h,0DFB6h,0E049h,0E0DBh,0E16Ch,0E1FDh
	dw      0E28Eh,0E31Fh,0E3AFh,0E43Eh,0E4CEh,0E55Dh,0E5EBh,0E67Ah
	dw      0E708h,0E795h,0E823h,0E8B0h,0E93Ch,0E9C9h,0EA55h,0EAE0h
	dw      0EB6Ch,0EBF7h,0EC82h,0ED0Ch,0ED96h,0EE20h,0EEA9h,0EF32h
	dw      0EFBBh,0F044h,0F0CCh,0F154h,0F1DCh,0F263h,0F2EAh,0F371h
	dw      0F3F7h,0F47Dh,0F503h,0F589h,0F60Eh,0F693h,0F718h,0F79Ch
	dw      0F821h,0F8A4h,0F928h,0F9ABh,0FA2Fh,0FAB1h,0FB34h,0FBB6h
	dw      0FC38h,0FCBAh,0FD3Ch,0FDBDh,0FE3Eh,0FEBFh,0FF3Fh,0FFBFh

	@

ALIGN 4

		public	NoLanguage Distance
		public	NoLanguage Sqr

Distance	dd	0000

		public	NoLanguage	EX0
		public	NoLanguage	EY0

EX0		dd	0000
EY0		dd	0000

		extrn   nolanguage P_SinTab:WORD

;*══════════════════════════════════════════════════════════════════════════*
		.code

		public	NoLanguage	Rot2D
		public  NoLanguage      Distance2D
		public  NoLanguage      Distance3D
		public	NoLanguage	GetAngle
		public	NoLanguage	RegleTrois32
		public	NoLanguage	BoundRegleTrois
		public	NoLanguage	InitRealValue
		public	NoLanguage	InitRealAngle
		public	NoLanguage	InitRealAngleConst
		public	NoLanguage	GetRealValue
		public	NoLanguage	GetRealAngle

;*══════════════════════════════════════════════════════════════════════════*
;	     █▀▀▀█ █▀▀▀▀ █▀▀▀█ █           ▀▀█▀▀  █    █▄ ▄█ █▀▀▀▀
;	     ██▀█▀ ██▀▀  ██▀▀█ ██            ██   ██   ██▀ █ ██▀▀
;   	     ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀   ▀▀   ▀▀  ▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*/
;*──────────────────────────────────────────────────────────────────────────*/

InitRealValue	proc	\
		StartValue:DWORD,\
		EndValue:DWORD,\
		TimeValue:DWORD,\
		ptrstruct:DWORD

		mov	edx, ptrstruct

		mov	ax, word ptr[StartValue]
		mov	word ptr[edx], ax
		mov	ax, word ptr[EndValue]
		mov	word ptr[edx+2], ax
		mov	ax, word ptr[TimeValue]
		mov	word ptr[edx+4], ax

		mov	eax, [ TimerRef ]
		mov	dword ptr[edx+6], eax		; memoticks init

		ret

InitRealValue	endp

comment	@

InitRealValue	proc	uses edi,\
		StartValue:DWORD,\
		EndValue:DWORD,\
		TimeValue:DWORD,\
		ptrstruct:DWORD

		mov	edi, ptrstruct

		mov	ax, word ptr[StartValue]
		mov	word ptr[edi], ax
		mov	ax, word ptr[EndValue]
		mov	word ptr[edi+2], ax
		mov	ax, word ptr[TimeValue]
		mov	word ptr[edi+4], ax

		mov	eax, [ TimerRef ]
		mov	dword ptr[edi+6], eax		; memoticks init

		ret

InitRealValue	endp

	@

;*──────────────────────────────────────────────────────────────────────────*/

InitRealAngle	proc	\
		StartValue:DWORD,\
		EndValue:DWORD,\
		TimeValue:DWORD,\
		ptrstruct:DWORD

		mov	edx, ptrstruct

		mov	ax, word ptr[StartValue]
		and	ax, 1023
		mov	word ptr[edx], ax
		mov	ax, word ptr[EndValue]
		and	ax, 1023
		mov	word ptr[edx+2], ax

		mov	ax, word ptr[TimeValue]
		mov	word ptr[edx+4], ax

		mov	eax, [TimerRef]
		mov	dword ptr[edx+6], eax		; memoticks init

		ret

InitRealAngle	endp

comment	@

InitRealAngle	proc	uses edi,\
		StartValue:DWORD,\
		EndValue:DWORD,\
		TimeValue:DWORD,\
		ptrstruct:DWORD

		mov	edi, ptrstruct

		mov	ax, word ptr[StartValue]
		and	ax, 1023
		mov	word ptr[edi], ax
		mov	ax, word ptr[EndValue]
		and	ax, 1023
		mov	word ptr[edi+2], ax

		mov	ax, word ptr[TimeValue]
		mov	word ptr[edi+4], ax

		mov	eax, [TimerRef]
		mov	dword ptr[edi+6], eax		; memoticks init

		ret

InitRealAngle	endp

	@

;*──────────────────────────────────────────────────────────────────────────*/

InitRealAngleConst	proc	\
			StartValue:DWORD,\
			EndValue:DWORD,\
			TimeValue:DWORD,\
			ptrstruct:DWORD

		mov	edx, ptrstruct

		xor	eax, eax			; for imul later on
		mov	ax, word ptr[StartValue]
		and	ax, 1023
		mov	word ptr[edx], ax
		mov	cx, word ptr[EndValue]
		and	cx, 1023
		mov	word ptr[edx+2], cx

		sub	ax, cx
		shl	ax, 6
		jns	short irac0

		neg	ax
irac0:
		shr	ax, 6
		imul	eax, dword ptr[TimeValue]
		shr	eax, 8				; /256
		mov	word ptr[edx+4], ax

		mov	eax, [TimerRef]
		mov	dword ptr[edx+6], eax		; memoticks init

		ret

InitRealAngleConst	endp

comment	@

InitRealAngleConst	proc	uses edi ebx,\
			StartValue:DWORD,\
			EndValue:DWORD,\
			TimeValue:DWORD,\
			ptrstruct:DWORD

		mov	edi, ptrstruct

		mov	ax, word ptr[StartValue]
		and	ax, 1023
		mov	bx, ax
		mov	word ptr[edi], ax
		mov	ax, word ptr[EndValue]
		and	ax, 1023
		mov	cx, ax
		mov	word ptr[edi+2], ax


		add	bx, 1024
		sub	bx, cx
		and	bx, 1023
		cmp	bx, 512
		jle	irac0
		sub	bx, 1024
		neg	bx

irac0:		mov	ax, word ptr[TimeValue]
		imul	bx
		mov	al, ah
		mov	ah, dl			; /256

		mov	word ptr[edi+4], ax

		mov	eax, [TimerRef]
		mov	dword ptr[edi+6], eax		; memoticks init

		ret

InitRealAngleConst	endp

	@

;*──────────────────────────────────────────────────────────────────────────*


GetRealValue	proc uses ebx,\
		ptrstruct:DWORD

		mov	ebx, ptrstruct

		movsx	eax, word ptr[ebx+2]	; End Value

		mov	cx, word ptr[ebx+4]	; TimeValue
		or	cx, cx			; TimeValue Reach = 0
		jz	elapsed

		mov	edx, [TimerRef]
		sub	edx, [ebx+6]		; memoticks
		cmp	dx, cx			; if step >= nbstep
		jae	sao2			; poke EndValue

						; nb step = time to come here

		sub	ax, word ptr[ebx+0]	; start value
		imul	dx			; resul DX:AX
		idiv	cx			; /NbStep
		add	ax, word ptr[ebx+0]	; Valeur1
		movsx	eax, ax

		ret

sao2:		mov	word ptr[ebx+4], 0	; TimeValue reset
elapsed:					; return EndValue
		ret

GetRealValue	endp

comment	@

GetRealValue	proc uses esi ebx ebp,\
		ptrstruct:DWORD

		mov	esi, ptrstruct

		mov	dx, word ptr[esi+4]	; TimeValue

		or	dx, dx			; TimeValue Reach = 0
		jz	elapsed

		mov	ecx, [esi+6]		; memoticks

		mov	eax, [TimerRef]

		sub	eax, ecx
		cmp	ax, dx			; if step >= nbstep
		jae	sao2			; poke EndValue

		movzx	ebx, dx			; nb step = time to come here
		mov	ebp, eax		; step en cours

		movsx	eax, word ptr[esi+2]	; End Value
		movsx	ecx, word ptr[esi+0]	; start value
		sub	eax, ecx
		imul	ebp			; resul DX:AX
		idiv	ebx			; /NbStep
		add	eax, ecx		; cx = Valeur1

		ret

sao2:		mov	word ptr[esi+4], 0	; TimeValue reset

elapsed:	movsx	eax, word ptr[esi+2]	; EndValue
		ret

GetRealValue	endp

	@

;*──────────────────────────────────────────────────────────────────────────*

GetRealAngle	proc uses ebx,\
		ptrstruct:DWORD

		mov	ebx, ptrstruct

		xor	eax, eax
		mov	ax, word ptr[ebx+2]	; EndValue

		mov	cx, word ptr[ebx+4]	; TimeAngle
		or	cx, cx			; TimeAngle Reach = 0
		jz	elapsed

		mov	edx, [TimerRef]

		sub	edx, [ebx+6]		; memoticks
		cmp	dx, cx			; if step >= nbstep
		jae	sao2			; poke EndValue

						; nb step = time to come here

		sub	ax, word ptr[ebx+0]	; start value
		cmp	ax, -512
		jl	short art1
		cmp	ax, 512
		jle	short ok

art0:		sub	ax, 2048
art1:		add	ax, 1024
ok:
		imul	dx			; resul DX:AX
		idiv	cx			; / NbStep
		add	ax, word ptr[ebx+0]	; start value
		ret

sao2:		mov	word ptr[ebx+4], 0	; TimeValue reset
elapsed:        				; return EndValue
		ret

GetRealAngle	endp

comment	@

GetRealAngle	proc uses esi ebx ebp,\
		ptrstruct:DWORD

		mov	esi, ptrstruct

		mov	dx, word ptr[esi+4]	; TimeAngle

		or	dx, dx			; TimeAngle Reach = 0
		jz	elapsed

		mov	ecx, [esi+6]		; memoticks

		mov	eax, [TimerRef]

		sub	eax, ecx
		cmp	ax, dx			; if step >= nbstep
		jae	sao2			; poke EndValue

		mov	bx, dx			; nb step = time to come here
		mov	bp, ax			; step en cours

		mov	ax, word ptr[esi+2]	; EndValue
		and	ax, 1023
		mov	cx, word ptr[esi+0]	; start value
		and	cx, 1023
		mov	dx, ax

		sub	ax, cx

		cmp	ax, 512
		jg	art0
		cmp	ax,-512
		jl	art1

		imul	bp			; resul DX:AX
		idiv	bx			; bx ; /NbStep
		add	ax, cx			; cx = Valeur1
		movzx	eax, ax
		ret

art0:		add	cx, 1024
		sub	dx, cx
		mov	ax, dx

		imul	bp			; resul DX:AX
		idiv	bx			; bx ; /NbStep
		add	ax, cx			; cx = Valeur1
		movzx	eax, ax
		ret

art1:		add	dx, 1024
		sub	dx, cx
		mov	ax, dx

		imul	bp			; resul DX:AX
		idiv	bx			; bx ; /NbStep
		add	ax, cx			; cx = Valeur1
		movzx	eax, ax
		ret

sao2:		mov	word ptr[esi+4], 0	; TimeValue reset

elapsed:	movsx	eax, word ptr[esi+2]	; EndValue
		ret

GetRealAngle	endp

	@

;*──────────────────────────────────────────────────────────────────────────*
comment	#

StopRealValue	proc uses ds si,\
		value:WORD, ptrstruct:PTR

		lds	si,ptrstruct

		mov	bx,ss:word ptr[FlagFlip] ; Cmpt 60 hz

		mov	ax,value
		cmp	word ptr[si+0],ax	; start value
		jz	srv0

		mov	word ptr[si+0],ax	; start value

		mov	cx,word ptr[si+6]	; memoticks
		mov	ax,word ptr[si+4]	; time value
		sub	bx,cx
		sub	ax,bx
		mov	word ptr[si+4],ax	; new time value

srv0:		mov	word ptr[si+6],bx	; new memoticks
		ret

StopRealValue	endp

	#

;*══════════════════════════════════════════════════════════════════════════*
;                  █▀▀▀█ █▀▀▀▀ █▀▀▀▀ █     █▀▀▀▀       ▀▀▀██
;                  ██▀█▀ ██▀▀  ██ ▀█ ██    ██▀▀        ▀▀▀██
;                  ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

BoundRegleTrois	proc	\
		Valeur1:DWORD, Valeur2:DWORD, \
		NbStep:DWORD, Step:DWORD

		mov	eax, Valeur2
		mov	ecx, Valeur1
		mov	edx, Step

		or	edx, edx
		jle	troppetit
		cmp	edx, NbStep
		jge	tropgrand

		sub	eax, ecx		; delta valeurs
		imul	edx			; * Step
		idiv	NbStep			; /NbStep
		add	eax, ecx		; + Valeur1

tropgrand:	ret

troppetit:	mov	eax, ecx
		ret

BoundRegleTrois	endp

comment	@

BoundRegleTrois	proc uses ebx ,\
		Valeur1:DWORD, Valeur2:DWORD	,\
		NbStep:DWORD, Step:DWORD

		mov	eax, Valeur2
		mov	ecx, Valeur1
		mov	ebx, NbStep
		mov	edx, Step

		or	edx, edx
		jle	troppetit
		cmp	edx, ebx
		jge	tropgrand

		sub	eax, ecx		; delta valeurs
		imul	edx			; * Step
		idiv	ebx			; /NbStep
		add	eax, ecx		; + Valeur1

tropgrand:	ret

troppetit:	mov	eax, ecx
		ret

BoundRegleTrois	endp

	@

RegleTrois32	proc	\
		Valeur1:DWORD, Valeur2:DWORD, \
		NbStep:DWORD, Step:DWORD

		mov	eax, Valeur2

		mov	ecx, NbStep
		or	ecx, ecx		; essai global  8-o
		jle	short erreur

		sub	eax, Valeur1
		imul	Step			; resul DX:AX
		idiv	ecx			; /NbStep
		add	eax, Valeur1		; cx = Valeur1

erreur:		ret

RegleTrois32	endp

comment	@

RegleTrois32	proc uses ebx ,\
		Valeur1:DWORD, Valeur2:DWORD	,\
		NbStep:DWORD, Step:DWORD

		mov	eax, Valeur2
		mov	ecx, Valeur1
		sub	eax, ecx
		imul	Step			; resul DX:AX

		mov	ebx, NbStep
;		dec	ebx
		or	ebx, ebx		; essai global  8-o
		jle	erreur

		idiv	ebx			; /NbStep

		add	eax, ecx		; cx = Valeur1
		ret

erreur:		mov	eax, ecx
		ret

RegleTrois32	endp

	@

comment #

RegleTrois      proc \
		Valeur1:WORD, Valeur2:WORD      ,\
		NbStep:WORD, Step:WORD

		mov     ax, Valeur2
		mov     cx, Valeur1
		sub     ax, cx
		imul    Step                    ; resul DX:AX

		mov     bx, NbStep
		dec     bx
		jbe     erreur

		idiv    bx                      ; /NbStep

		add     ax, cx                  ; cx = Valeur1
		ret

erreur:         xor     ax, ax
		ret

RegleTrois      endp

	#
;*══════════════════════════════════════════════════════════════════════════*
;			       ██▀▀▀ █▀▀▀█ █▀▀▀█
;			       ▀▀▀▀█ ██ ▄█ ██▀█▀
;			       ▀▀▀▀▀ ▀▀▀▀  ▀▀  ▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

;*══════════════════════════════════════════════════════════════════════════*
;          EAX = Sqr(EAX)

Sqr            PROC	NEAR	USES EBX

		cmp	eax, 3
		jbe	short asqr_0_1

		xor     edx, edx
		mov	ebx, eax

		bsr	eax, ebx		; cherche dernier bit à 1
		mov	cl, 33
		sub	cl, al
		and	cl, -2			; calcul de combien decaler
						; à gauche pour avoir le
						; premier doublet non nul
						; dans edx
		shld	edx, ebx, cl
		shl	ebx, cl

		mov	ecx, eax                ; doublets restants
		shr	ecx, 1

		mov     eax, 1
		dec     edx

asqr_loop:      shld    edx, ebx, 2
		shl     ebx, 2
		shl     eax, 2
		cmp     edx, eax
		jc      short asqr_neg

		inc	eax
		sub     edx, eax
		jnc     short asqr_1
		add     edx, eax
asqr_neg:	shr	eax, 1
		dec	ecx
		jnz     short asqr_loop
		ret

asqr_1:         inc	eax
		shr	eax, 1
		dec	ecx
		jnz     short asqr_loop
		ret

asqr_0_1:	or	eax, eax
		jz	short asqr_00
		mov	eax, 1
asqr_00:	ret

Sqr            ENDP

comment	@

;*══════════════════════════════════════════════════════════════════════════*
;          EAX = Sqr(EAX)

Sqr            proc     near

		cmp	eax, 3
		jbe	short asqr_0_1

		mov	esi, eax
		mov     eax, 1
		xor     edx, edx

		bsr	ebx, esi		; cherche dernier bit à 1

		mov	cx, 33
		sub	cx, bx
		and	cx, -2			; calcul de combien decaler
						; à gauche pour avoir le
						; premier doublet non nul
						; dans edx
		shld	edx, esi, cl
		shl	esi, cl

		mov	ecx, ebx                ; doublets restants
		shr	ecx, 1
		inc	cx

		dec     edx
		dec	ecx
		jnz    short asqr_loop
		ret

asqr_loop:      shld    edx, esi, 2
		shl     esi, 2
		shl     eax, 1
		mov     ebx, edx
		shr     ebx, 1
		sub     ebx, eax
		jc      short asqr_neg

		mov     ebx, eax
		stc
		rcl     ebx, 1
		sub     edx, ebx
		jc      short asqr_0
		inc     eax
		dec	ecx
		jnz    short asqr_loop
		ret

asqr_0:         add     edx, ebx
asqr_neg:       dec	ecx
		jnz    short asqr_loop
		ret

asqr_0_1:	or	eax, eax
		jnz	asqr_1
		ret
asqr_1:		mov	eax, 1
		ret

Sqr            endp

	@

comment	@

;*══════════════════════════════════════════════════════════════════════════*
;          AX = Sqr(DX:AX)

Sqr             proc    near

		xor     cx,cx
		test    dx,dx
		jne     sq1
		test    ax,ax
		je      is0
		xchg    ax,dx
		mov     cx,8
sq1:
		test    dh,dh
		jne     sq2
		xchg    dh,dl
		xchg    dl,ah
		xchg    ah,al
		add     cx,4
sq2:
		test    dh,0f0h
		jne     sq3

;               shld    dx, ax, 4
;               shl     ax, 4

		shl     ax, 1
		rcl     dx, 1
		shl     ax, 1
		rcl     dx, 1
		shl     ax, 1
		rcl     dx, 1
		shl     ax, 1
		rcl     dx, 1

		add     cx,2
sq3:
		test    dh,0c0h
		jne     sq4

;               shld    dx, ax, 2
;               shl     ax, 2

		shl     ax, 1
		rcl     dx, 1
		shl     ax, 1
		rcl     dx, 1

		inc     cx
sq4:
		mov     bl,dh
		xor     bh,bh
		shl     bx,1
		mov     bx,[ SqrTab + bx-64*2 ]
		cmp     dx,bx
		jae     sq5
		div     bx
		add     ax,bx
		rcr     ax,1
		shr     ax,cl
is0:
		ret
sq5:
		mov     ax,dx
		stc
		rcr     ax,1
		shr     ax,cl

		ret

Sqr             endp

	@

;*══════════════════════════════════════════════════════════════════════════*
;               █▀▀▀▄  █    ██▀▀▀ ▀▀█▀▀ █▀▀▀█ ██▄ █ █▀▀▀▀ █▀▀▀▀
;               ██  █  ██   ▀▀▀▀█   ██  ██▀▀█ ██▀██ ██    ██▀▀
;               ▀▀▀▀   ▀▀   ▀▀▀▀▀   ▀▀  ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

;*══════════════════════════════════════════════════════════════════════════*

Distance2D      proc	\
		px0:DWORD,py0:DWORD, px1:DWORD,py1:DWORD

		mov     eax, px1
		sub     eax, px0                ; Dx = x1-x0
		imul    eax, eax                ; Dx²

		mov     ecx, py1
		sub     ecx, py0                ; Dy = y1-y0
		imul    ecx, ecx                ; Dy²
		add     eax, ecx                ; Dx² + Dy²

		call    Sqr                    ; eax = √ (Dx²+Dy²)

		ret

Distance2D      endp

comment	@

Distance2D      proc uses ebx,\
		px0:DWORD,py0:DWORD, px1:DWORD,py1:DWORD

		mov     eax, px1
		sub     eax, px0                ; Dx = x1-x0
		imul    eax, eax                ; Dx²

		mov     ebx, py1
		sub     ebx, py0                ; Dy = y1-y0
		imul    ebx, ebx                ; Dy²
		add     eax, ebx                ; Dx² + Dy²

		call    Sqr                    ; eax = √ (Dx²+Dy²)

		ret

Distance2D      endp

	@

;*══════════════════════════════════════════════════════════════════════════*

Distance3D      proc	\
		px0:DWORD,py0:DWORD,pz0:DWORD, px1:DWORD,py1:DWORD,pz1:DWORD


		mov     eax, px1
		sub     eax, px0
		imul    eax, eax                ; Dx²

		mov     ecx, py1
		sub     ecx, py0
		imul    ecx, ecx                ; Dy²
		add	eax, ecx

		mov     ecx, pz1
		sub     ecx, pz0
		imul    ecx, ecx                ; Dz²
		add	eax, ecx

		call    Sqr                    ; eax = √ (Dx²+Dy²+Dz²)

		ret

Distance3D      endp

comment	@

Distance3D      proc uses ebx,\
		px0:DWORD,py0:DWORD,pz0:DWORD, px1:DWORD,py1:DWORD,pz1:DWORD


		mov     eax, px1
		sub     eax, px0
		imul    eax, eax                ; Dx²

		mov     ebx, py1
		sub     ebx, py0
		imul    ebx, ebx                ; Dy²
		add	eax, ebx

		mov     ebx, pz1
		sub     ebx, pz0
		imul    ebx, ebx                ; Dz²
		add	eax, ebx

		call    Sqr                    ; eax = √ (Dx²+Dy²+Dz²)

		ret

Distance3D      endp

	@

;*══════════════════════════════════════════════════════════════════════════*
;                        ▀▀█▀▀ █▀▀▀█  █    █▀▀▀▀ █▀▀▀█
;                          ██  ██▀█▀  ██   ██ ▀█ ██  █
;                          ▀▀  ▀▀  ▀  ▀▀   ▀▀▀▀▀ ▀▀▀▀▀
;*══════════════════════════════════════════════════════════════════════════*
;*──────────────────────────────────────────────────────────────────────────*

;*══════════════════════════════════════════════════════════════════════════*

GetAngle        proc uses esi edi ebx ebp,\
		x0:DWORD, z0:DWORD, x1:DWORD, z1:DWORD


		mov     edx, z1
		sub     edx, z0         ; z = z1 - z0
		mov     edi, edx        ; save z

		mov     eax, x1
		sub     eax, x0         ; x = x1 - x0
		mov     ebp, eax        ; save x

		imul    eax, eax	; x²
		imul    edx, edx        ; z²
		cmp	eax, edx
		jae	noswap
		xchg	ebp, edi	; permute x z
		or	ebp, 1
		jmp	short carre
noswap:
		and	ebp, -2
carre:
		add     eax, edx        ; eax = x² + z²
		call    Sqr		; eax = sqr(x² + z²)

		mov	[Distance], eax

		or	eax, eax	; si longueur 0 alors angle 0
		jz	fgetangle

		xchg    eax, edi
		sal     eax, 14
		cdq
		idiv    edi

		mov     esi, offset P_SinTab + 384 * 2
		lea	edi, [esi+256*2]

ga2:            mov     ebx, esi
		add     ebx, edi
		shr     ebx, 1

		cmp     ax, [ebx]
		jle     ga0
		mov     edi, ebx
		jmp     short ga1
ga0:            mov     esi, ebx
		je	short fgaloop

		mov	ebx, edi
ga1:		sub	ebx, esi
		dec	ebx
		jnz	short ga2

		movsx	ebx, word ptr [esi]
		movsx	edx, word ptr [edi]
		add	ebx, edx
		sar	ebx, 1
		cmp	bx, ax
		jg	short fgaloop
		mov	esi, edi

fgaloop:	sub     esi, offset P_SinTab + 256 * 2
		mov     eax, esi
		shr     eax, 1

		or      ebp, ebp
		jns     short fga
		neg     eax
fga:
		test	ebp, 1
		jz      short norestore
		neg	eax
		add	eax, 256
norestore:
		and     eax, 1023
fgetangle:
		ret

GetAngle        endp


comment	@


GetAngle        proc uses esi edi ebx ebp,\
		x0:DWORD, z0:DWORD, x1:DWORD, z1:DWORD


		mov     edx, z1
		sub     edx, z0         ; z = z1 - z0
		mov     edi, edx        ; save z
		jns	nonegedx	; abs(z)
		neg	edx
nonegedx:
		mov     eax, x1
		sub     eax, x0         ; x = x1 - x0
		mov     ebp, eax        ; save x
		jns     nonegeax	; abs(x)
		neg	eax
nonegeax:
		cmp	eax, edx
		jae	noswap
		xchg	eax, edx	; permute x z
		xchg	ebp, edi
		or	ebp, 1
		jmp	short carre
noswap:
		and	ebp, -2
carre:
		imul    eax, eax
		imul    edx, edx
		add     eax, edx        ; eax = x² + z²

		call    Sqr

		mov	[Distance], eax

		or	eax, eax	; si longueur 0 alors angle 0
		jz	fgetangle


		xchg    eax, edi
		sal     eax, 14
		cdq
		idiv    edi

		mov     esi, offset P_SinTab + 384 * 2
		mov     edi, esi
		add	edi, 256 * 2

ga2:            mov     ebx, esi
		add     ebx, edi
		shr     ebx, 1

		cmp     ax, [ebx]
		jle     ga0
		mov     edi, ebx
		jmp     short ga1
ga0:            mov     esi, ebx
		je	fgaloop

		mov	ebx, edi
ga1:		sub	ebx, esi
		dec	ebx
		jnz	ga2

		movsx	ebx, word ptr [esi]
		movsx	edx, word ptr [edi]
		add	ebx, edx
		sar	ebx, 1
		cmp	bx, ax
		jg	fgaloop
		mov	esi, edi


fgaloop:	sub     esi, offset P_SinTab + 256 * 2
		mov     eax, esi
		shr     eax, 1

		or      ebp, ebp
		jns     fga
		neg     eax

fga:
		test	ebp, 1
		jz      norestore
		mov	ebx, eax
		mov	eax, 256
		sub	eax, ebx
norestore:
		and     eax, 1023
fgetangle:
		ret

GetAngle        endp

	@

comment	@

;*══════════════════════════════════════════════════════════════════════════*
; Rotation 2D autour d'un axe
; entrée: eax=X  ecx=Y  edx=T
;
; sortie: eax = X'=X*cos(T)-Y*sin(T)
;	  ecx = Y'=X*sin(T)+Y*cos(T)
;
; ( ebx, ebp, edi détruits )
;*══════════════════════════════════════════════════════════════════════════*

RotEXY		proc

		or	edx, edx
		jz	short norot

		and	edx, 03FFh
		movsx	ebx, word ptr[P_SinTab + edx*2]	; ebx = Sin( T )
		add	dx, 256
		and	dx, 03FFh
		movsx	edx, word ptr[P_SinTab + edx*2]	; edx = Cos( T )

		mov	ebp, eax		; ebp = X
		mov	edi, ecx		; edi = Y

		imul	eax, edx		; X*cos(T)
		imul	edi, ebx		; Y*sin(T)
		sub	eax, edi
		sar	eax, 15			; eax = X' = X*cos(T)-Y*sin(T)

		imul	ecx, edx		; Y*cos(T)
		imul	ebp, ebx		; X*sin(T)
		add	ecx, ebp
		sar	ecx, 15			; ecx = Y' = X*sin(T) + Y*cos(T)
norot:		ret

RotEXY		endp

;*══════════════════════════════════════════════════════════════════════════*
; Rotation 2D autour d'un axe
; entrée: ebx=X  ecx=Y  ebp=T
;
; sortie: eax = X'=X*cos(T)-Y*sin(T)
;	  edx = Y'=X*sin(T)+Y*cos(T)
;
; ( ebx, ebp, edi détruits )
;*══════════════════════════════════════════════════════════════════════════*

RotEXY		proc

		or	ebp, ebp
		jz	norot

		mov	eax, ebp			; T
		and	eax, 03FFh
		mov	edi, offset P_SinTab
		movsx	ebp, word ptr[ edi+eax*2 ]	; ebp = Sin( T )
		add	eax, 256
		and	eax, 03FFh
		movsx	edx, word ptr[ edi+eax*2 ]	; edx = Cos( T )
; X*cos(T)
		mov	eax, ebx
		imul	eax, edx
		mov	edi, eax
; Y*sin(T)
		mov	eax, ecx
		imul	eax, ebp
		sub	edi, eax
		sar	edi, 15			; di= X'= X*cos(T)-Y*sin(T)
; Y*cos(T)
		imul	edx, ecx
; X*sin(T)
		imul	ebx, ebp
		add	edx, ebx		; Y' = X*sin(T) + Y*cos(T)
		sar	edx, 15			; edx = Y'

		mov	eax, edi		; eax = X'
		xor	ecx, ecx

		ret

norot:		mov	eax, ebx		; ax = X' = X
		mov	edx, ecx		; dx = Y' = Y
		ret

RotEXY		endp

	@

;*══════════════════════════════════════════════════════════════════════════*

Rot2D		proc	uses ebx,\
		coorx:DWORD,coory:DWORD,angle:DWORD

		mov	edx, angle
		mov	eax, coorx
		mov	ecx, coory

		or	edx, edx
		jz	short norot

		and	edx, 03FFh
		movsx	ebx, word ptr[P_SinTab + edx*2]	; ebx = Sin( T )
		add	dx, 256
		and	dx, 03FFh
		movsx	edx, word ptr[P_SinTab + edx*2]	; edx = Cos( T )

		imul	eax, edx		; X*cos(T)
		imul	ecx, ebx		; Y*sin(T)
		sub	eax, ecx
		sar	eax, 15			; eax = X' = X*cos(T)-Y*sin(T)
		mov	[EX0], eax

		imul	edx, coory		; Y*cos(T)
		imul	ebx, coorx		; X*sin(T)
		add	edx, ebx
		sar	edx, 15			; edx = Y' = X*sin(T) + Y*cos(T)
		mov	[EY0], edx

		ret

norot:		mov	[EX0], eax
		mov	[EY0], ecx
		ret

Rot2D		endp

comment	@

Rot2D		proc	uses ebx ebp edi,\
		coorx:DWORD,coory:DWORD,angle:DWORD

		mov	eax, coorx
		mov	ecx, coory
		mov	edx, angle
		call	RotEXY
		mov	[EX0], eax
		mov	[EY0], ecx
		ret

Rot2D		endp

	@

;*══════════════════════════════════════════════════════════════════════════*

		END
