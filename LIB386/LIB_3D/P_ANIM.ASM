
		.386p
		jumps

		.model	SMALL, SYSCALL

		include P_define.ash

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;                    P_     Û฿฿฿Û ÛÛÜ Û  Û    ÛÜ ÜÛ
;                           ÛÛ฿฿Û ÛÛ฿ÛÛ  ÛÛ   ÛÛ฿ Û
;                           ฿฿  ฿ ฿฿  ฿  ฿฿   ฿฿  ฿
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

		.data

		include	\projet\lib386\lib_sys\lib_sys.ash

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

ALIGN 4

		extrn NoLanguage	List_Anim_Point:WORD

Offset_Source	dd      0000
Offset_Dest	dd      0000

StartInfos      dd      0000

		public	NoLanguage	MemoTimerRef
		public	NoLanguage	CmptMemoTimerRef

MemoTimerRef		dd	0000
CmptMemoTimerRef	dd	0000

		public  NoLanguage	AnimStepX
		public  NoLanguage	AnimStepY
		public  NoLanguage	AnimStepZ

AnimStepX       dw      00
AnimStepY       dw      00
AnimStepZ       dw      00

		public  NoLanguage	AnimMasterRot
		public  NoLanguage	AnimStepAlpha
		public  NoLanguage	AnimStepBeta
		public  NoLanguage	AnimStepGamma

AnimMasterRot	dw	00
AnimStepAlpha	dw      00
AnimStepBeta	dw      00
AnimStepGamma	dw      00

CXSetInterAnim	dw	00

		.code

		public	NoLanguage	SetAngleGroupe
		public	NoLanguage	GetAngleGroupe
		public	NoLanguage	GetNbFramesAnim
		public	NoLanguage	GetBouclageAnim
		public	NoLanguage	SetAnimObjet

		public	NoLanguage	SetInterAnimObjet
		public	NoLanguage	SetInterAnimObjet2
		public	NoLanguage	SetInterDepObjet

		public	NoLanguage	StockInterAnim
		public	NoLanguage	CopyInterAnim
		public	NoLanguage	SaveTimer
		public	NoLanguage	RestoreTimer

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

SaveTimer	proc

		cmp	[CmptMemoTimerRef], 0
		jnz	st0

		mov     eax, [ TimerRef ]
		mov	[MemoTimerRef], eax

st0:		inc	[CmptMemoTimerRef]

		ret

SaveTimer	endp

RestoreTimer	proc

		dec	[CmptMemoTimerRef]
		jnz	rt0

		mov	eax, [MemoTimerRef]
		mov     [ TimerRef ], eax

rt0:		ret

RestoreTimer	endp


;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

SetAngleGroupe  proc    uses esi,\
		numgroupe:DWORD, palpha:DWORD, pbeta:DWORD, pgamma:DWORD,\
		ptrobj:DWORD

		mov	esi, ptrobj

		mov	ax, [esi]		; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr

		add     esi, 14			; saute ZV

		xor	eax, eax
		mov	ax, [esi]		; size info to skip
		lea	esi, [esi+eax+4]

		mov	ax, [esi-2]		; nb points
		lea	ecx, [eax+eax*2]	; *3
		lea	esi, [esi+ecx*2]	; saute def points

		mov	ax, [esi]		; nb groupes

		cmp     eax, numgroupe
		jb      serr                    ; mauvais numgroupe
		imul	eax, 38
		lea	esi, [esi+eax+2+10]	; si sur def groupe

		mov     eax, palpha
		mov     word ptr[esi], ax
		mov     eax, pbeta
		mov     word ptr[esi+2], ax
		mov     eax, pgamma
		mov     word ptr[esi+4], ax

serr:           ret

SetAngleGroupe  endp

comment	@

SetAngleGroupe  proc    uses esi ebx,\
		numgroupe:DWORD, palpha:DWORD, pbeta:DWORD, pgamma:DWORD,\
		ptrobj:DWORD

		mov	esi, ptrobj

		lodsw				; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr

		add     esi, 12			; saute ZV

		lodsw                           ; size info to skip
		movzx	eax, ax
		add     esi, eax

		lodsw                          	; nb points
		movzx	eax, ax
		mov     ebx, eax
		shl     eax, 1
		add     eax, ebx                  ; *3
		shl     eax, 1                   ; WORD
		add     esi, eax                  ; saute def points

		lodsw                           ; nb groupes
		movzx	ebx, ax

		mov     eax, numgroupe
		cmp     eax, ebx
		jae     serr                     ; mauvais numgroupe
		mov     ecx, 38
		mul     ecx
		add     esi, eax                  ; si sur def groupe

;		mov     word ptr[esi+8], TYPE_ROTATE

		mov     eax, palpha
		mov     word ptr[esi+10], ax
		mov     eax, pbeta
		mov     word ptr[esi+12], ax
		mov     eax, pgamma
		mov     word ptr[esi+14], ax

serr:            ret

SetAngleGroupe  endp

	@

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

GetAngleGroupe  proc    uses esi,\
		numgroupe:DWORD,\
		ptrobj:DWORD

		mov	esi, ptrobj

		mov	ax, [esi]		; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr

		add     esi, 14			; saute ZV

		xor	eax, eax
		mov     ax, [esi]               ; size info to skip
		lea     esi, [esi+eax+4]

		mov	ax, [esi-2]		; nb points
		mov     ecx, eax
		lea	ecx, [ecx+eax*2]	; *3
		lea	esi, [esi+ecx*2]	; saute def points

		mov	ax, [esi]		; nb groupes

		cmp     eax, numgroupe
		jb      serr                    ; mauvais numgroupe
		imul	eax, 38
		lea	edi, [edi+eax+2+10]	; si sur def groupe

		mov     ax, word ptr[esi]
		mov	word ptr[AnimStepAlpha], ax
		mov     ax, word ptr[esi+2]
		mov	word ptr[AnimStepBeta], ax
		mov     ax, word ptr[esi+4]
		mov	word ptr[AnimStepGamma], ax

serr:            ret

GetAngleGroupe  endp

comment	@

GetAngleGroupe  proc    uses esi ebx,\
		numgroupe:DWORD,\
		ptrobj:DWORD

		mov	esi, ptrobj

		lodsw				; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr

		add     esi, 12			; saute ZV

		lodsw                           ; size info to skip
		movzx	eax, ax
		add     esi, eax

		lodsw                          	; nb points
		movzx	eax, ax
		mov     ebx, eax
		shl     eax, 1
		add     eax, ebx                  ; *3
		shl     eax, 1                   ; WORD
		add     esi, eax                  ; saute def points

		lodsw                           ; nb groupes
		movzx	ebx, ax

		mov     eax, numgroupe
		cmp     eax, ebx
		jae     serr                     ; mauvais numgroupe
		mov     ecx, 38
		mul     ecx
		add     esi, eax                  ; si sur def groupe

		mov     ax, word ptr[esi+10]
		mov	word ptr[AnimStepAlpha], ax
		mov     ax, word ptr[esi+12]
		mov	word ptr[AnimStepBeta], ax
		mov     ax, word ptr[esi+14]
		mov	word ptr[AnimStepGamma], ax

serr:            ret

GetAngleGroupe  endp

	@

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

GetNbFramesAnim proc    \
		ptranim:DWORD

		mov	ecx, ptranim
		xor	eax, eax
		mov	ax, word ptr[ecx]
		ret

GetNbFramesAnim endp

GetBouclageAnim proc    \
		ptranim:DWORD

		mov	ecx, ptranim
		add	ecx, 4
		xor	eax, eax
		mov	ax, word ptr[ecx]
		ret

GetBouclageAnim endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

SetAnimObjet    proc    uses esi edi,\
		numframe:DWORD, ptranim:DWORD,ptrobj:DWORD

		mov	esi, ptranim

		xor     eax, eax
		mov	ax, [esi]               ; nbframes

		mov	edx, numframe
		cmp     edx, eax		; si numframe >= nbframes
		jb      noerr

		ret

noerr:          mov	ax, [esi+2]		; nb groupes

		mov	ecx, eax

		lea	eax, [eax*8+8]		; infos frame + *4 WORD ax size 1 frame
		imul    eax, edx
		lea	esi, [esi+eax+8]	; si sur start frame

		mov	edi, ptrobj

		mov     ax, [edi]             	; lit infos
		test    ax, INFO_ANIM           ; test si objet possede anim
		jz      serr                    ; non tanpis au revoir

		add     edi, 12 + 2             ; saute ZV + word infos

		mov     [edi+0+2], esi		; memo offset

		mov     eax, [ TimerRef ]
		mov     [edi+4+2], eax         	; memoticks reset

		xor	eax, eax
		mov	ax, word ptr[edi]
		lea	edi, [edi+eax+2]
		mov	ax, word ptr[edi]	; nb points
		mov     edx, eax
		lea	edx, [edx+eax*2]	; *3
		lea	edi, [edi+edx*2+2+8+2]
		mov	ax, word ptr[edi-8-2]	; nb groupes

		cmp     cx, ax
		jbe     ok
		mov     cx, ax                  ; ? - de groupes que d'anim
ok:
		mov	edx, esi
		add     esi, 8                  ; saute Infos frame

sao0:           movsd                           ; type ; alpha
		movsd                           ; beta ; gamma
		add     edi, 12+18
		dec	cx
		jnz     sao0                    ; set tous groupes

		mov     esi, edx                ; sur infos frame
		add     esi, 2                  ; sur stepX

		mov     edi, offset AnimStepX
		movsd
		movsw				; stock var DGROUP

		mov	edi, offset AnimMasterRot
		movsd
		movsd

		mov     eax, 1                   ; retourne flag anim atteinte
		ret

serr:           xor     eax, eax
		ret

SetAnimObjet    endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;comment	#
PatchInterAngle proc    near

		mov	esi, [Offset_Source]
		mov	cx, [esi]		; lastalpha
		add	esi, 2
		mov     [Offset_Source], esi

		mov	esi, [Offset_Dest]
		lodsw				; newalpha
		mov     [Offset_Dest], esi

		and	ax, 1023
		and	cx, 1023

		sub     ax, cx
		jz      optequal

		cmp     ax, -512
		jl      art1
		cmp     ax, 512
		jle     ok

art0:		sub     ax, 2048
art1:		add     ax, 1024
ok:
		imul    bp                      ; resul DX:AX
		idiv    bx                      ; bx ; /NbStep
		add     cx, ax                  ; cx = Valeur1
optequal:	and	cx, 1023
		mov	word ptr[edi], cx
		add	edi, 2
		ret

PatchInterAngle endp
;	#
comment	#
PatchInterAngle MACRO
		local arta, bart, ok, optequal

		mov	esi, [Offset_Source]
		mov	cx, [esi]		; lastalpha
		add	esi, 2
		mov     [Offset_Source], esi

		mov	esi, [Offset_Dest]
		lodsw				; newalpha
		mov     [Offset_Dest], esi

		and	ax, 1023
		and	cx, 1023

		sub     ax, cx
		jz      optequal

		cmp     ax, -512
		jl      arta
		cmp     ax, 512
		jle     ok

arta:		sub     ax, 2048
bart:		add     ax, 1024
ok:
		imul    bp                      ; resul DX:AX
		idiv    bx                      ; bx ; /NbStep
		add     cx, ax                  ; cx = Valeur1
optequal:	and	cx, 1023
		mov	word ptr[edi], cx
		add	edi, 2

		ENDM
	#
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

PatchInterStep  proc    near

		mov     esi, [Offset_Source]
		mov	cx, [esi]		; laststepX
		add	esi, 2
		mov     [Offset_Source], esi

		mov     esi, [Offset_Dest]
		lodsw				; newstepX
		mov     [Offset_Dest], esi

		sub     ax, cx
		jz      optequal


		imul    bp                      ; resul DX:AX
		idiv    bx                      ; bx ; /NbStep
		add     cx, ax                  ; cx = Valeur1

optequal:       mov	word ptr[edi], cx
		add	edi, 2			; stock valeur intermdiare
						; dans struct groupe de l'obj
		ret

PatchInterStep  endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

PatchType       proc    near

		mov	esi, [Offset_Dest]
		lodsw
		mov     [Offset_Dest], esi
		mov	word ptr[edi], ax
		add	edi, 2			; stock type
		add     [Offset_Source], 2	; saute type source

		ret

PatchType       endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

SetInterAnimObjet       proc    uses esi edi ebx ebp,\
			framedest:DWORD, ptranimdest:DWORD,\
			ptrobj:DWORD

		mov	esi, ptranimdest

		xor	eax, eax
		xor	edx, edx
		mov	ax, word ptr[esi+2]	; nb groupes dest
		mov	cx, ax
		lea	eax, [eax*8+8]		; infos frame + *4 WORD ax size 1 frame
		imul    eax, framedest
		lea	esi, [esi+eax+8]	; si sur time frame dest

		mov	[Offset_Dest], esi

		mov	dx, word ptr[esi]	; dx time to come here

		mov	edi, ptrobj

		mov     ax, [edi]		; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr			; non tanpis au revoir

		add     edi, 12 + 2 + 2		; saute ZV (+ info)

		mov     [StartInfos], edi	; memo start info

		mov     ebx, [edi+0]		; memo offset source anim
		mov     ebp, [edi+4]		; lit memoticks

		or      ebx, ebx		; test init ?
		jnz     sao3

		mov	ebp, edx		; first anim poke last frame
		mov     ebx, esi

sao3:		mov     [Offset_Source], ebx

cont:		xor	eax, eax
		mov	ax, word ptr[edi-2]
		add	edi, eax
		mov	ax, word ptr[edi]	; nb points
		lea     eax, [eax+eax*2]	; *3
		lea	edi, [edi+eax*2+2+8+2]
		mov	ax, word ptr[edi-8-2]	; nb groupes

		cmp	cx, ax
		jbe     ok
		mov	cx, ax			; - de group obj que de group d'anim ?
ok:
		mov     eax, [ TimerRef ]	; Cmpt 50 hz

		sub     eax, ebp
		cmp     eax, edx		; if step >= nbstep

		jae     sao2			; poke dest frame

		mov     ebx, edx		; nb step = time to come here
		mov     ebp, eax		; step en cours

		push	[Offset_Dest]

		add     [Offset_Source], 8 ; saute infos frame
		add     [Offset_Dest], 8   ; saute infos frame

	; la 1ere fois pour le G0 ne tiens pas compte du type
	; et stock valeur calcules

		mov     esi, [Offset_Dest]
		mov	ax, [esi]
		mov	[AnimMasterRot], ax

		mov	ax, [esi + 2]		; StepAlpha
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	[AnimStepAlpha], ax

		mov	ax, [esi + 4]		; StepBeta
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	[AnimStepBeta], ax

		mov	ax, [esi + 6]		; StepGamma
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	[AnimStepGamma], ax

		add	[Offset_Dest], 8
		add	[Offset_Source], 8
		add	edi, 38

		dec	cx
		jz	sao4

	; suite des groupes

		mov	[CXSetInterAnim], cx

sao0:           call    PatchType

		cmp     ax,TYPE_ROTATE
		jnz     ttrans

		call    PatchInterAngle
		call    PatchInterAngle
		call    PatchInterAngle

;		PatchInterAngle
;		PatchInterAngle
;		PatchInterAngle

		add	edi, 12+18
		dec	word ptr[CXSetInterAnim]
		jnz	sao0			; set tous groupes
		jmp	short sao4

ttrans:         cmp     ax,TYPE_TRANSLATE
		jnz     tzoom

		call    PatchInterStep
		call    PatchInterStep
		call    PatchInterStep

		add     edi, 12+18
		dec	word ptr[CXSetInterAnim]
		jnz	sao0			; set tous groupes
		jmp	short sao4

tzoom:          cmp     ax,TYPE_ZOOM
		jnz     argbug

		call    PatchInterStep
		call    PatchInterStep
		call    PatchInterStep

argbug:         add     edi, 12+18
		dec	word ptr[CXSetInterAnim]
		jnz     sao0			; set tous groupes
sao4:

		; calcul StepXYZ

		pop     esi			; esi anim dest info frame
		mov     edi, offset AnimStepX

		mov	ax, [esi + 2]		; StepX
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	word ptr[edi], ax

		mov	ax, [esi + 4]		; StepY
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	word ptr[edi+2], ax

		mov	ax, [esi + 6]		; StepZ
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	word ptr[edi+4], ax

;               mov     word ptr[TimeFrame],bx
;               mov     word ptr[StepFrame],bp

serr:		xor     eax, eax		; anim pas finie
		ret




		; poke dest frame directly

sao2:
;//		add	ebp, edx

		mov     edx, esi
		add     esi, 8			; saute infos frame

sao1:           movsd				; type ; alpha
		movsd				; beta ; gamma
		add	edi, 12+18
		dec	cx
		jnz	sao1			; set tous groupes

		mov	edi, [StartInfos]

		mov	eax, [Offset_Dest]
		mov	[edi+0], eax		; memo offset

		mov     eax, [ TimerRef ]
		mov	[edi+4], eax		; memoticks reset
comment	@
		mov	eax, [ TimerRef ]
		cmp	eax, ebp
		jae	okmemo

		mov	ebp, eax
okmemo:
		mov	[edi+4], ebp		; memoticks reset
	@
		; stock step X Y Z

		mov	esi, edx		; sur infos frame
		add	esi, 2			; sur stepX

		mov	edi, offset AnimStepX
		movsd
		movsw				; stock var DGROUP

		mov	edi, offset AnimMasterRot
		movsd
		movsd				; + AnimStepAlpha, ...


		mov	eax, 1			; retourne flag anim atteinte

		ret

SetInterAnimObjet       endp
;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

SetInterAnimObjet2      proc    uses esi edi ebx ebp,\
			framedest:DWORD, ptranimdest:DWORD,\
			ptrobj:DWORD

		mov	esi, ptranimdest
		mov     edx, framedest

		xor	eax, eax
		mov	ax, word ptr[esi + 2]	; nb groupes dest
		mov	cx, ax
		lea	eax, [8+eax*8]		; (eax+1)*4 WORD ax size 1 frame
		mul	edx
		lea	esi, [esi+eax+8]	; si sur time frame dest

		mov	[Offset_Dest], esi

		xor	edx, edx
		mov	dx, word ptr[esi]	; dx time to come here

		mov	edi, ptrobj

		mov	ax, [edi]		; lit infos
		test	ax, INFO_ANIM		; test si objet possede anim
		jz	serr			; non tanpis au revoir

		lea	edi,[edi+12+2+2]	; saute ZV (+ info)

		mov	[StartInfos], edi	; memo start info

		mov	ebx, [edi+0]		; memo offset source anim
		mov	ebp, [edi+4]		; lit memoticks

		or      ebx, ebx		; test init ?
		jnz     sao3

		mov	ebp, edx		; first anim poke last frame
		mov     ebx, esi

sao3:		mov     [Offset_Source], ebx

cont:		xor	eax, eax
		mov	ax, word ptr[edi-2]
		lea	edi, [edi+eax]
		mov	ax, word ptr[edi]	; nb points
		lea     eax, [eax+eax*2]	; *3
		lea	edi, [edi+eax*2+2+8+2]
		mov	ax, word ptr[edi-8-2]	; nb groupes

		cmp	cx, ax
		jbe     ok
		mov	cx, ax			; - de group obj que de group d'anim ?
ok:
		mov     eax, [ TimerRef ]	; Cmpt 50 hz

		sub     eax, ebp
		cmp     eax, edx		; if step >= nbstep

		mov     ebx, edx		; nb step = time to come here
		mov     ebp, eax		; step en cours

		jae     sao2			; poke dest frame

		add	[Offset_Dest], 16
		add	[Offset_Source], 16
		add	edi, 38

		dec	cx
		jz	sao4

	; suite des groupes

		mov	[CXSetInterAnim], cx

sao0:           call    PatchType

		cmp     ax,TYPE_ROTATE
		jnz     ttrans

		call    PatchInterAngle		; ax val2 cx val1 bp step bx nbstep
		call    PatchInterAngle		; ax val2 cx val1 bp step bx nbstep
		call    PatchInterAngle		; ax val2 cx val1 bp step bx nbstep

;		call    PatchInterStep
;		call    PatchInterStep
;		call    PatchInterStep

		add	edi, 12+18
		dec	word ptr[CXSetInterAnim]
		jnz	sao0			; set tous groupes
		jmp	short sao4

ttrans:         cmp     ax,TYPE_TRANSLATE
		jnz     tzoom

		call    PatchInterStep		; ax val2 cx val1 bp step bx nbstep
		call    PatchInterStep		; ax val2 cx val1 bp step bx nbstep
		call    PatchInterStep		; ax val2 cx val1 bp step bx nbstep

		add     edi, 12+18
		dec	word ptr[CXSetInterAnim]
		jnz	sao0			; set tous groupes
		jmp	short sao4

tzoom:          cmp     ax,TYPE_ZOOM
		jnz     argbug

		call    PatchInterStep		; ax val2 cx val1 bp step bx nbstep
		call    PatchInterStep		; ax val2 cx val1 bp step bx nbstep
		call    PatchInterStep		; ax val2 cx val1 bp step bx nbstep

argbug:         add     edi, 12+18
		dec	word ptr[CXSetInterAnim]
		jnz	sao0			; set tous groupes
sao4:

serr:		xor     eax, eax		; anim pas finie
		ret

		; poke dest frame directly

sao2:    	add     esi, 8			; saute infos frame

sao1:           movsd				; type ; alpha
		movsd				; beta ; gamma
		add     edi, 12+18
		dec	cx
		jnz     sao1			; set tous groupes

		mov     edi, [StartInfos]

		mov     eax, [Offset_Dest]
		mov     [edi+0], eax		; memo offset

;		mov     eax, [ TimerRef ]
;		mov     [edi+4], eax		; memoticks reset

		mov	eax, 1			; retourne flag anim atteinte

		ret

SetInterAnimObjet2       endp


;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

SetInterDepObjet	proc    uses esi edi ebx ebp,\
			framedest:DWORD, ptranimdest:DWORD,\
			ptrobj:DWORD

		mov	esi, ptranimdest
		mov     edx, framedest

		xor	eax, eax
		mov	ax, [esi+2]
		lea     eax, [8+eax*8]		; (ax+1)*4 WORD ax size 1 frame
		mul     edx
		lea	esi, [esi+eax+8]	; si sur time frame dest

		mov	[Offset_Dest], esi

		xor	edx, edx
		mov	dx, word ptr[esi]	; dx time to come here

		mov	edi, ptrobj

		mov     ax, [edi]		; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz     serr			; non tanpis au revoir

		lea	edi, [edi+12+2+2]	; saute ZV (+ info)

		mov     [StartInfos], edi	; memo start info

		mov     eax, [edi+0]		; memo offset source anim
		mov     ebp, [edi+4]		; lit memoticks

		or      eax, eax		; test init ?
		jnz     sao3

		mov     eax, esi
		mov	ebp, edx		; first anim poke last frame
sao3:
		mov     [Offset_Source], eax

		mov	eax, ebp
		neg	ebp
		add	ebp, [ TimerRef ]	; Cmpt 50 hz
		cmp	ebp, edx		; if step >= nbstep
		jae	sao2			; poke dest frame

		mov     ebx, edx		; nb step = time to come here

		mov	ecx, [Offset_Dest]

		add     [Offset_Source], 8	; saute infos frame
		add     [Offset_Dest], 8	; saute infos frame

	; la 1ere fois pour le G0 ne tiens pas compte du type
	; et stock valeur calcules

		mov     esi, [Offset_Dest]
		mov	ax, [esi]
		mov	[AnimMasterRot], ax

		mov	ax, [esi+2]		; StepAlpha
		imul    bp                      ; resul DX:AX
		idiv    bx                      ; /NbStep
		mov	[AnimStepAlpha], ax

		mov	ax, [esi+4]		; StepBeta
		imul    bp                      ; resul DX:AX
		idiv    bx                      ; /NbStep
		mov	[AnimStepBeta], ax

		mov	ax, [esi+6]		; StepGamma
		imul    bp                      ; resul DX:AX
		idiv    bx                      ; /NbStep
		mov	[AnimStepGamma], ax

		add	[Offset_Dest], 8
		add	[Offset_Source], 8

sao4:		; calcul StepXYZ

		mov     edi, offset AnimStepX

		mov	ax, [ecx+2]		; StepX
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	word ptr[edi], ax

		mov	ax, [ecx+4]		; StepY
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	word ptr[edi+2], ax

		mov	ax, [ecx+6]		; StepZ
		imul    bp			; resul DX:AX
		idiv    bx			; /NbStep
		mov	word ptr[edi+4], ax

serr:		xor     eax, eax		; anim pas finie
		ret

		; poke dest frame directly

sao2:
;//		lea	ebp, [eax+edx]

		mov     edi, [StartInfos]

		mov     eax, [Offset_Dest]
		mov     [edi+0], eax		; memo offset

		mov     eax, [ TimerRef ]
		mov     [edi+4], eax		; memoticks reset
comment	@
		mov	eax, [ TimerRef ]
		cmp	eax, ebp
		jae	okmemo

		mov	ebp, eax
okmemo:
		mov     [edi+4], ebp		; memoticks reset
	@
		; stock step X Y Z

		add     esi, 2			; sur stepX

		mov	edi, offset AnimStepX
		movsd
		movsw				; stock var DGROUP

		mov	edi, offset AnimMasterRot
		movsd
		movsd				; + AnimStepAlpha, ...

		mov	eax, 1			; retourne flag anim atteinte

		ret

SetInterDepObjet       endp


;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

StockInterAnim  proc    uses esi edi,\
		ptranimbuf:DWORD,\
		ptrobj:DWORD

		mov	edi, ptranimbuf

		mov	esi, ptrobj
		mov     ax, [esi]		; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr			; non tanpis au revoir

		lea     esi, [esi+12+2+2]	; saute ZV (+ info)

		mov     eax, [ TimerRef ]
		mov     [esi+4], eax		; reset memoticks
		mov     [esi+0], edi		; memo off source anim

cont:   	xor	eax, eax
		mov	ecx, eax
		mov	ax, word ptr[esi-2]	; skip param infos
		lea	esi, [esi+eax]		; skip param infos
		mov	ax, word ptr[esi]	; nb points
		lea     eax, [eax+eax*2]	; *3
		lea	esi, [esi+eax*2+2]	; saute def points
		mov     cx, word ptr[esi]	; nb groupes

		lea	eax, [ecx*8+8]		; trick pour calculer ecx*8+8 :-)

		lea     edi, [edi+8]		; fausse frame info
		lea     esi, [esi+8+2]		; saute groupe info

sao1:           movsd				; type ; alpha
		movsd				; beta ; gamma
		add     esi, 12+18
		dec	cx
		jnz	sao1			; set tous groupes

		ret

serr:           xor	eax, eax
		ret

StockInterAnim  endp

;*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*

CopyInterAnim   proc	uses esi edi,\
		ptrobjs:DWORD,\
		ptrobjd:DWORD

		mov	esi, ptrobjs
		mov     ax, word ptr[esi]	; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr			; non tanpis au revoir

		mov	edi, ptrobjd
		mov     ax, word ptr[edi]	; lit infos
		test    ax, INFO_ANIM		; test si objet possede anim
		jz      serr			; non tanpis au revoir

		add     esi, 12 + 2 + 2		; saute ZV (+ info)
		add     edi, 12 + 2 + 2		; saute ZV (+ info)

		mov     eax, [esi+0]
		mov     [edi+0], eax		; copy memoticks
		mov     eax, [esi+4]
		mov     [edi+4], eax		; copy off source anim

		xor	eax, eax
		mov	ax, word ptr[esi-2]	; skip param infos
		lea	esi, [esi+eax]		; skip param infos
		mov	ax, word ptr[esi]	; nb points
		lea     eax, [eax+eax*2]	; *3
		lea	esi, [esi+eax*2+2]
		mov	cx, word ptr[esi]	; nb groupes

		mov	ax, word ptr[edi-2]	; skip param infos
		lea	edi, [edi+eax]		; skip param infos
		mov	ax, word ptr[edi]	; nb points
		lea     eax, [eax+eax*2]	; *3
		lea	edi, [edi+eax*2+2]
		mov	ax, word ptr[edi]	; nb groupes

		cmp	cx, ax			; 2 body nb groups different ?
		jbe	ok
		mov	cx, ax
ok:
		add	esi, 8 + 2
		add	edi, 8 + 2

cia0:		movsd
		movsd
		add	esi, 22 + 8
		add	edi, 22 + 8
		dec	cx
		jnz	cia0

serr:		ret

CopyInterAnim  endp


;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*

		END
