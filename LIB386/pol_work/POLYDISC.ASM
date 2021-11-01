
; ╔═══════════════════════════════════════════╗
; ║ ┌─────┐ ┌─────┐ │      │    │             ║
; ║ │     │ │     │ │      │    │             ║
; ║ │     │ │     │ │      │    │             ╚═══════════════════════╤═══════════╗
; ║ │     │ │     │ │      │    │    ┌──── ─                          │Début:     ║
; ║ ├─────┘ │     │ │      └────┤    │       │    │    ┌─── ├──┐      │le 02/08/95║
; ║ │       │     │ │           │    ├──   │ │    │    │    │         │           ║
; ║ │       │     │ │           │ /  │     │ │    │    ├─   │         │           ║
; ║ │       │     │ │           │ \  │     │ │    │    │    │         │           ║
; ║ │       └─────┘ └─────      │ /  │     │ └─── └─── └─── │         │           ║
; ║                                                                   │           ║
; ║                                                                   │           ║
; ╟────────────────────────────────────────┐                          │           ║
; ║Module: Fillers DISQUE                  │                          │           ║
; ╚════════════════════════════════════════╧╗                         │ MAJ:      ║
;                                           ║                         │ 09/11/95  ║
;                                           ╚═════════════════════════╧═══════════╝




PUBLIC			Fill_Sphere

PUBLIC			Fill_VLDisc1
PUBLIC			Fill_VLDisc2
PUBLIC			Fill_VLDisc3
PUBLIC			Fill_VLDisc4


PUBLIC			First_POLYDISCPatch
PUBLIC			Last_POLYDISCPatch




INCLUDE			F:\PROJET\LIB386\H\FILLER.INC


			.386
			.MODEL FLAT, SYSCALL



; ╔═══════════════════════════════════════════════════════════════════════════╗
; ║                                                                           ║
; ║  Les donnees                                                              ║
; ║                                                                           ║
; ╚═══════════════════════════════════════════════════════════════════════════╝

			.DATA
Extrn	SYSCALL	ClipXMin		:	DWORD	; Fenetre de clipping
Extrn	SYSCALL	ClipXMax		:	DWORD
Extrn	SYSCALL	ClipYMin		:	DWORD
Extrn	SYSCALL	ClipYMax		:	DWORD

Extrn			TabOffLine		:	DWORD	; Table des offsets des debuts de ligne
Extrn			Log			:	DWORD	; PTR sur l'ecran logique

Extrn			Fill_Filler		:	DWORD

Extrn			Fill_Flag_Fog		:	BYTE	; Flag pour savoir si l'on est en mode Fog ou non
Extrn			Fill_Flag_ZBuffer	:	BYTE	; TRUE if Zbuffer active
Extrn			Fill_Flag_NZW		:	BYTE	; TRUE if Zbuffer without ZBuf update active
Extrn			Fill_Logical_Palette	:	BYTE	; Table de conversion de la palette en mode fog
Extrn			PtrZBuffer		: 	DWORD


			ALIGN	4
Z_Value			dd	0			; Z Value for the ZBuffer
Sph_YMin		dd	0			; YMin et YMax du disque a l'ecran
Sph_YMax		dd	0
Sph_XMin		dd	0			; XMin et XMax pour le trace d'une ligne de balayage du disque
Sph_XMax		dd	0
Sph_Coul		db	0








; ╔═══════════════════════════════════════════════════════════════════════════╗
; ║                                                                           ║
; ║  Le code                                                                  ║
; ║                                                                           ║
; ╚═══════════════════════════════════════════════════════════════════════════╝
			.CODE
;Seg_Code		SEGMENT	PUBLIC PARA USE32





; ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
; ║  Fill_Sphere : Trace un disque a l'ecran                                                                   ║
; ╟────────────────────────────────────────────────────────────────────────────────────────────────────────────╢
; ║  Appel : EAX = C_x                                                                                         ║
; ║          EBI = C_y                                                                                         ║
; ║          ECX = Rayon                                                                                       ║
; ║          EDX = Couleur                                                                                     ║
; ║          ESI = Type du disque (soit Flat, soit Transparent=2)                                              ║
; ║          EDI = Zbuffer value for the sphere                                                                ║
; ║                                                                                                            ║
; ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
			ALIGN	4
Fill_Sphere		PROC
							; xc	equ	ecx
							; yc	equ	esi

							; px	equ	edi
							; py	equ	edx
							; sum	equ	ebp

			cmp	[Fill_Flag_ZBuffer],TRUE
			jne	@@No_ZBuff
			mov	[Z_Value],edi

			mov	edi,Offset Sph_Line_FlatZBuf
			cmp	[Fill_Flag_NZW],TRUE
			jne	@@No_NZW
			mov	edi,Offset Sph_Line_FlatNZW
@@No_NZW:
			cmp	esi,2			; Type Transparent
			jne	@@Type_Ok
			mov	edi,Offset Sph_Line_Transp
			jmp	@@Type_Ok

@@No_ZBuff:
			mov	edi,Offset Sph_Line_Flat
			cmp	esi,2			; Type Transparent
			jne	@@Type_Ok
			mov	edi,Offset Sph_Line_Transp
@@Type_Ok:
			and	edx,0FFh
			cmp	[Fill_Flag_Fog],TRUE
			jne	@@No_Coul_Convert
			mov	dl,[Fill_Logical_Palette+edx]
@@No_Coul_Convert:
			mov	[Sph_Coul],dl
			mov	[Fill_Filler],edi


			mov	ebp,ecx
			mov	edi,ecx
			mov	ecx,eax
			mov	esi,ebx
			xor	edx,edx
			neg	ebp

			mov	eax,esi
			xor	ebx,ebx
			sub	eax,edi			; esi - rayon
			cmp	eax,[ClipYMax]
			jg	nosphere
			cmp	eax,[ClipYMin]
			jge	csc0
			inc	ebx
			mov	eax,[ClipYMin]
csc0:
			mov	[Sph_YMin],eax

			lea	eax,[esi+edi]		; esi + rayon
			cmp	eax,[ClipYMin]
			jl	nosphere
			cmp	eax,[ClipYMax]
			jle	csc1
			inc	ebx
			mov	eax,[ClipYMax]
csc1:
			mov	[Sph_YMax],eax

			lea	eax,[ecx+edi]		; ecx + rayon
			cmp	eax,[ClipXMin]
			jl	nosphere
			cmp	eax,[ClipXMax]
			jle	csc2
			inc	ebx

csc2:
			mov	eax,ecx
			sub	eax,edi			; ecx - rayon
			cmp	eax,[ClipXMax]
			jg	nosphere

			cmp	eax,[ClipXMin]
			jge	csc3
			inc	ebx

csc3:
			test	ebx,ebx
			jne	clipping


			ALIGN 4
nccs0:
			cmp	edx,edi				; while( edx <= edi )
			jg	fin

			lea	ebx,[esi+edx]
			lea	eax,[ecx+edi]
			mov	[Sph_XMax],eax
			mov	eax,ecx
			sub	eax,edi
			mov	[Sph_XMin],eax
			call	[Fill_Filler]

			mov	ebx,esi
			sub	ebx,edx
			mov	[Sph_XMin],eax
			lea	eax,[ecx+edi]
			mov	[Sph_XMax],eax

			call	[Fill_Filler]

			add	ebp,edx
			jnc	ncnoovf

			lea	ebx,[esi+edi]
			lea	eax,[ecx+edx]
			mov	[Sph_XMax],eax
			mov	eax,ecx
			sub	eax,edx
			mov	[Sph_XMin],eax

			call	[Fill_Filler]

			mov	ebx,esi
			sub	ebx,edi
			mov	[Sph_XMin],eax
			lea	eax,[ecx+edx]
			mov	[Sph_XMax],eax

			call	[Fill_Filler]

			dec	edi
			sub	ebp,edi

ncnoovf:
			inc	edx
			jmp	nccs0

fin:
			mov	eax,1
			ret

						; *** Gestion du clipping ***
			ALIGN	4
clipping:
cs0:
			cmp	edx,edi			; while( edx <= edi )
			jg	finclip

			lea	ebx,[esi+edx]
			cmp	ebx,[Sph_YMax]
			jg	cs1
			cmp	ebx,[Sph_YMin]
			jl	cs1

			; gauche
			mov	eax,ecx
			sub	eax,edi
			cmp	eax,[ClipXMin]
			jl	nocxi
			cmp	eax,[ClipXMax]
			jle	nocxi2
			lea	eax,[ebx-1]
			mov	[Sph_YMax],eax
			jmp	cs1
nocxi:
			mov	eax,[ClipXMin]
nocxi2:
			mov	[Sph_XMin],eax

			; droite
			lea	eax,[ecx+edi]
			cmp	eax,[ClipXMax]
			jg	nocxa
			cmp	eax,[ClipXMin]
			jge	nocxa2
			lea	eax,[ebx-1]
			mov	[Sph_YMax],eax
			jmp	cs1
nocxa:
			mov	eax,[ClipXMax]
nocxa2:
			mov	[Sph_XMax],eax

			call	[Fill_Filler]

cs1:
			mov	ebx,esi
			sub	ebx,edx
			cmp	ebx,[Sph_YMax]
			jg	cs2
			cmp	ebx,[Sph_YMin]
			jl	cs2
			; gauche
			mov	eax,ecx
			sub	eax,edi
			cmp	eax,[ClipXMin]
			jl	nocxia
			cmp	eax,[ClipXMax]
			jle	nocxia2
			lea	eax,[ebx+1]
			mov	[Sph_YMin],eax
			jmp	short cs2
nocxia:			mov	eax,[ClipXMin]
nocxia2:
			mov	[Sph_XMin],eax
			; droite
			lea	eax,[ecx+edi]
			cmp	eax,[ClipXMax]
			jg	nocxaa
			cmp	eax,[ClipXMin]
			jge	nocxaa2
			lea	eax,[ebx+1]
			mov	[Sph_YMin],eax
			jmp	cs2
nocxaa:
			mov	eax,[ClipXMax]
nocxaa2:
			mov	[Sph_XMax],eax

			call	[Fill_Filler]

cs2:
			add	ebp,edx
			jnc	noovf

			lea	ebx,[esi+edi]
			cmp	ebx,[Sph_YMax]
			jg	cs3
			cmp	ebx,[Sph_YMin]
			jl	cs3
			; gauche
			mov	eax,ecx
			sub	eax,edx
			cmp	eax,[ClipXMin]
			jl	nocxib
			cmp	eax,[ClipXMax]
			jle	nocxib2
			lea	eax,[ebx-1]
			mov	[Sph_YMax],eax
			jmp	cs3
nocxib:
			mov	eax,[ClipXMin]
nocxib2:
			mov	[Sph_XMin],eax
			; droite
			lea	eax,[ecx+edx]
			cmp	eax,[ClipXMax]
			jg	nocxab
			cmp	eax,[ClipXMin]
			jge	nocxab2
			lea	eax,[ebx-1]
			mov	[Sph_YMax],eax
			jmp	cs3
nocxab:
			mov	eax,[ClipXMax]
nocxab2:
			mov	[Sph_XMax],eax

			call	[Fill_Filler]

cs3:
			mov	ebx,esi
			sub	ebx,edi
			cmp	ebx,[Sph_YMax]
			jg	cs4
			cmp	ebx,[Sph_YMin]
			jl	cs4
			; gauche
			mov	eax,ecx
			sub	eax,edx
			cmp	eax,[ClipXMin]
			jl	nocxic
			cmp	eax,[ClipXMax]
			jle	nocxic2
			lea	eax,[ebx+1]
			mov	[Sph_YMin],eax
			jmp	cs4
nocxic:
			mov	eax,[ClipXMin]
nocxic2:
			mov	[Sph_XMin],eax
			; droite
			lea	eax,[ecx+edx]
			cmp	eax,[ClipXMax]
			jg	nocxac
			cmp	eax,[ClipXMin]
			jge	nocxac2
			lea	eax,[ebx+1]
			mov	[Sph_YMin],eax
			jmp	cs4
nocxac:
			mov	eax,[ClipXMax]
nocxac2:
			mov	[Sph_XMax],eax

			call	[Fill_Filler]

cs4:
			dec	edi
			sub	ebp,edi

noovf:
			inc	edx
			jmp     cs0

finclip:
			mov	eax,[Sph_YMin]
			cmp	eax,[Sph_YMax]

; 11/9/94 pas de sphere d'une ligne ??? (jg -> jge)
			jge	nosphere

			mov	eax,1			; ok sphere
			ret

nosphere:
			xor	eax,eax
			ret
Fill_Sphere		ENDP






First_POLYDISCPatch	Label	DWORD




; ╔═════════════════════════════════════════════════════════════════════════════════╗
; ║ Sph_Line_Flat : Trace un segment horizontal en Flat                             ║
; ╟─────────────────────────────────────────────────────────────────────────────────╢
; ║ Appel : [Sph_XMin], [Sph_XMax] : Extremites du segment                          ║
; ║         EBX = Y a l'ecran                                                       ║
; ║         [Sph_XCoul] : Couleur                                                   ║
; ╟─────────────────────────────────────────────────────────────────────────────────╢
; ║ REGISTRES NON MODIFIES                                                          ║
; ║                                                                                 ║
; ╚═════════════════════════════════════════════════════════════════════════════════╝
			ALIGN	4
Sph_Line_Flat		PROC
			pushad

Fill_VLDisc1::		mov	edi,[TabOffLine+ebx*4]
			add	edi,[Log]		; Ecran logique

			mov	ebx,[Sph_XMin]
			mov	ecx,[Sph_XMax]
			add	edi,ebx
			sub	ecx,ebx
			jle	@@Fin

			mov	al,[Sph_Coul]
			test	edi,1			; Test de l'alignement
			mov	ah,al
			je	@@Align_Ok
			mov	[edi],al
			inc	edi
			dec	ecx
@@Align_Ok:
;			shr	ecx,1
			db	0C1h,0E9h,01h
			rep	stosw
			jnc	@@Fin
			mov	[edi],al
@@Fin:
			popad
			ret
Sph_Line_Flat		ENDP




Sph_Line_FlatZBuf	PROC
			pushad

Fill_VLDisc3::		mov	edi,[TabOffLine+ebx*4]
			mov	ebx,edi
			add	edi,[Log]		; Ecran logique
			add	ebx,ebx
			add	ebx,[PtrZBuffer]

			mov	edx,[Sph_XMin]
			mov	ecx,[Sph_XMax]
			add	edi,edx
			add	ebx,edx
			add	ebx,edx
			sub	ecx,edx
			jle	@@Fin

			mov	al,[Sph_Coul]
			mov	edx,[Z_Value]
@@Loop:
			cmp	[ebx],dx
			jb	@@NoDisp
			mov	[ebx],dx
			mov	[edi],al
@@NoDisp:
			add	ebx,2
			inc	edi

			dec	ecx
			jne	@@Loop
@@Fin:
			popad
			ret
Sph_Line_FlatZBuf	ENDP





Sph_Line_FlatNZW	PROC
			pushad

Fill_VLDisc4::		mov	edi,[TabOffLine+ebx*4]
			mov	ebx,edi
			add	edi,[Log]		; Ecran logique
			add	ebx,ebx
			add	ebx,[PtrZBuffer]

			mov	edx,[Sph_XMin]
			mov	ecx,[Sph_XMax]
			add	edi,edx
			add	ebx,edx
			add	ebx,edx
			sub	ecx,edx
			jle	@@Fin

			mov	al,[Sph_Coul]
			mov	edx,[Z_Value]
@@Loop:
			cmp	[ebx],dx
			jb	@@NoDisp
			mov	[edi],al
@@NoDisp:
			add	ebx,2
			inc	edi

			dec	ecx
			jne	@@Loop
@@Fin:
			popad
			ret
Sph_Line_FlatNZW	ENDP









; ╔═════════════════════════════════════════════════════════════════════════════════╗
; ║ Sph_Line_Transp : Trace un segment horizontal en Transparence                   ║
; ╟─────────────────────────────────────────────────────────────────────────────────╢
; ║ Appel : [Sph_XMin], [Sph_XMax] : Extremites du segment                          ║
; ║         EBX = Y a l'ecran                                                       ║
; ║         [Sph_XCoul] : Couleur                                                   ║
; ╟─────────────────────────────────────────────────────────────────────────────────╢
; ║ REGISTRES NON MODIFIES                                                          ║
; ║                                                                                 ║
; ╚═════════════════════════════════════════════════════════════════════════════════╝
			ALIGN	4
Sph_Line_Transp		PROC
			pushad

Fill_VLDisc2::		mov	edi,[TabOffLine+ebx*4]
			add	edi,[Log]		; Ecran logique

			mov	ebx,[Sph_XMin]
			mov	ecx,[Sph_XMax]
			add	edi,ebx
			sub	ecx,ebx
			jle	@@Fin

			mov	bl,[Sph_Coul]
			and	bl,0F0h
			mov	bh,bl

			test	edi,1			; Test de l'alignement
			mov	ah,al
			je	@@Align_Ok
			mov	al,[edi]
			and	al,0Fh
			add	al,bl
			mov	[edi],al
			inc	edi
			dec	ecx
@@Align_Ok:
			mov	edx,ecx
;			shr	ecx,1
			db	0C1h,0E9h,01h
			and	edx,1
			ALIGN	4
@@Fill:
			test	ecx,ecx
			je	@@Add_One

			mov	ax,[edi]
			and	eax,0F0Fh
			add	eax,ebx
			mov	[edi],ax

			add	edi,2
			dec	ecx
			jmp	@@Fill


			ALIGN	4
@@Add_One:
			test	edx,edx
			je	@@Fin
			mov	al,[edi]
			and	al,0Fh
			add	al,bl
			mov	[edi],al
@@Fin:
			popad
			ret
Sph_Line_Transp		ENDP



Last_POLYDISCPatch	Label	DWORD



;Seg_Code		ENDS

;			The
			End
