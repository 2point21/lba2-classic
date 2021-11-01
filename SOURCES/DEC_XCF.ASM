;*══════════════════════════════════════════════════════════════════════════*/
;			      (c) Adeline 1995
;*══════════════════════════════════════════════════════════════════════════*/

		.386P
		.model FLAT, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*/

;SCANLINE	equ	640
;SHIFT		equ	7

SCANLINE	equ	320
SHIFT		equ	6



;*══════════════════════════════════════════════════════════════════════════*/

		.data

;*══════════════════════════════════════════════════════════════════════════*/

		Public	syscall	XCF_BaseOldScr
		Public	syscall	XCF_BaseNewScr
		Public	syscall	XCF_BaseData

;*══════════════════════════════════════════════════════════════════════════*/

CurLine		db	?
CurCol		db	?

		ALIGN	4

XCF_BaseOldScr	dd	?
XCF_BaseNewScr	dd	?
XCF_BaseData	dd	?

DeltaBase	dd	?
NewScr		dd	?
Codes		dd	?
CurCodes	dd	?

PTR_Padd_Data	dd	0		; PTR sur les datas alignees
PTR_Other_Data	dd	0		; PTR sur les datas non alignees

Dummy		dd	?

Tab_Addr	dd	4 dup (?)

TabOfs		dd	2*320+2
		dd	2*320+2+4
		dd	(2+4)*320+2
		dd	(2+4)*320+2+4

;*══════════════════════════════════════════════════════════════════════════*/

Patterns	dd	256 dup (?)

;*══════════════════════════════════════════════════════════════════════════*/

TabLines	dd	SCANLINE*0
		dd	SCANLINE*1
		dd	SCANLINE*2
		dd	SCANLINE*3
		dd	SCANLINE*4
		dd	SCANLINE*5
		dd	SCANLINE*6
		dd	SCANLINE*7

;*══════════════════════════════════════════════════════════════════════════*/


TabFunc		dd	offset Raw		; 0	0

		dd	offset ZeroMotionDecode	; 1	1
		dd	offset ZeroMotionDecode	; 2	2
		dd	offset ZeroMotionDecode	; 3	3
		dd	offset ZeroMotionDecode	; 4	4

		dd	offset SMotion8Decode	; 5	5
		dd	offset SMotion8Decode	; 6	6
		dd	offset SMotion8Decode	; 7	7
		dd	offset SMotion8Decode	; 8	8

		dd	offset Motion8Decode	; 9	9
		dd	offset Motion8Decode	; 10	A
		dd	offset Motion8Decode	; 11	B
		dd	offset Motion8Decode	; 12	C

		dd	offset SMotion4Decode	; 13	D
		dd	offset SMotion4Decode	; 14	E
 		dd	offset SMotion4Decode	; 15	F
		dd	offset SMotion4Decode	; 16	10

		dd	offset Motion4Decode	; 17	11
		dd	offset Motion4Decode	; 18	12
		dd	offset Motion4Decode	; 19	13
		dd	offset Motion4Decode	; 20	14

		dd	offset Uni8Decode	; 21	15
		dd	offset Uni8Decode	; 22	16
		dd	offset Uni8Decode	; 23	17
		dd	offset Uni8Decode	; 24	18

		dd	offset Uni4Decode	; 25	19
		dd	offset Uni4Decode	; 26	1A
		dd	offset Uni4Decode	; 27	1B
		dd	offset Uni4Decode	; 28	1C

		dd	offset VR8Decode2	; 29	1D
		dd	offset VR8Decode4	; 30	1E
		dd	offset VR8Decode8	; 31	1F
		dd	offset VR8Decode16	; 32	20

		dd	offset VR4Decode2	; 33	21
		dd	offset VR4Decode4	; 34	22
		dd	offset VR4Decode8	; 35	23

		dd	offset CrossDecode	; 36	24
		dd	offset PrimeDecode	; 37	25

		dd	offset Bank1Decode	; 38	26
		dd	offset Bank2Decode	; 39	27

		dd	offset BlockDecode	; 40	28
		dd	offset BlockDecode	; 41	29
		dd	offset BlockDecode	; 42	2A
 		dd	offset BlockDecode	; 43	2B

		dd	offset BlockBank1Decode	; 44	2C
		dd	offset BlockBank1Decode	; 45	2D
		dd	offset BlockBank1Decode	; 46	2E
		dd	offset BlockBank1Decode	; 47	2F

		dd	offset ROMotion8Decode	; 48	30
		dd	offset ROMotion8Decode	; 49	31
		dd	offset ROMotion8Decode	; 50	32
		dd	offset ROMotion8Decode	; 51	33

		dd	offset RCMotion8Decode	; 52	34
		dd	offset RCMotion8Decode	; 53	35
		dd	offset RCMotion8Decode	; 54	36
		dd	offset RCMotion8Decode	; 55	37

		dd	offset ROMotion4Decode	; 56	38
		dd	offset ROMotion4Decode	; 57	39
		dd	offset ROMotion4Decode	; 58	3A
		dd	offset ROMotion4Decode	; 59	3B

		dd	offset RCMotion4Decode	; 60	3C
		dd	offset RCMotion4Decode	; 61	3D
		dd	offset RCMotion4Decode	; 62	3E
		dd	offset RCMotion4Decode	; 63	3F



;*══════════════════════════════════════════════════════════════════════════*/

TabCorrect	dd	offset Return
		dd	offset Correct_4
		dd	offset Correct_8
		dd	offset Correct_16

;*══════════════════════════════════════════════════════════════════════════*/


		.code

;*══════════════════════════════════════════════════════════════════════════*/

		Public	syscall	XCF_Decompressor
		Public	syscall	XCF_InitDecompressor
		Public	syscall	XCF_DecompressorHeight




;*══════════════════════════════════════════════════════════════════════════*/

XCF_Decompressor PROC
		push	ebx
		push	ebp

		push	esi
		push	edi

		mov	ebx,[XCF_BaseData]			; PTR to raw data
		mov	ecx,[XCF_BaseOldScr]		; Old screen

		mov	eax,[XCF_BaseNewScr]		; Current screen
		mov	esi,[ebx]			; Load Color buffer offset
XCF_Patch_CodeLen label	dword
		lea	edi,[ebx+720+4]			; Load Data buffer offset
		add	esi,ebx

		mov	[PTR_Padd_Data],edi		; PTR sur les donnees alignees
		mov	[PTR_Other_Data],esi		; PTR sur les donnees non alignees

		xor	dl,dl
		add	ebx,4

		mov	[CurLine],dl			; A priori, la position courante 40*24
		mov	[CurCol],dl

		sub	ecx,eax
		mov	[NewScr],eax

		mov	[DeltaBase],ecx			; Delta entre le Old et le New screen
		mov	ecx,[ebx]			; Load Opcode

		add	ebx,3
		mov	edx,ecx

		or	ecx,0FF000000h

		shr	ecx,6
		and	edx,3Fh

		mov	[Codes],ebx
		mov	[CurCodes],ecx

		mov	esi,[PTR_Padd_Data]
		mov	edi,[NewScr]
		mov	ebp,[PTR_Other_Data]
		jmp	TabFunc[edx*4]

		ALIGN	4
Return::	mov	al,[CurCol]
		mov	ecx,[NewScr]
		cmp	al,39
		jae	@@Next_Line

		inc	al
		add	ecx,8
		mov	[CurCol],al
		mov	[NewScr],ecx

		mov	ebx,[CurCodes]
		mov	edx,ebx
		cmp	ebx,0FFh
		je	@@Load_NextOpcodes

		shr	ebx,6
		and	edx,3Fh
		mov	[CurCodes],ebx
;	mov	esi,[PTR_Padd_Data]
	mov	edi,[NewScr]
;	mov	ebp,[PTR_Other_Data]
		jmp	TabFunc[edx*4]

@@Load_NextOpcodes:				; last
		mov	ecx,[Codes]
		mov	ebx,[ecx]
		add	ecx,3
		mov	edx,ebx
		or	ebx,0FF000000h
		and	edx,3Fh
		shr	ebx,6
		mov	[Codes],ecx
		mov	[CurCodes],ebx
;	mov	esi,[PTR_Padd_Data]
	mov	edi,[NewScr]
;	mov	ebp,[PTR_Other_Data]
		jmp	TabFunc[edx*4]

		ALIGN	4
@@Next_Line:					; Exit1
		mov	al,[CurLine]
		add	ecx,8+(SCANLINE-320)+SCANLINE*7
XCF_Patch_MaxLine label	byte
		cmp 	al,23
		jae	@@End

		mov	ebx,[Codes]
		mov	[NewScr],ecx
		inc	al
		xor	dl,dl
		mov	[CurLine],al
		mov	[CurCol],dl

		mov	ecx,[ebx]
		add	ebx,3
		mov	edx,ecx
		or	ecx,0FF000000h
		and	edx,3Fh
		shr	ecx,6
		mov	[Codes],ebx
		mov	[CurCodes],ecx
;	mov	esi,[PTR_Padd_Data]
	mov	edi,[NewScr]
;	mov	ebp,[PTR_Other_Data]
		jmp	TabFunc[edx*4]

		ALIGN	4
@@End:						; Exit2
		pop	edi
		pop	esi
		pop	ebp
		pop	ebx
		ret

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data => 64 octets de couleurs

		ALIGN	4
Raw::
AA		=	0
REPT 8
		mov	eax,[esi+AA*8]
		mov	ebx,[esi+4+AA*8]

		mov	[edi+AA*SCANLINE],eax
		mov	[edi+4+AA*SCANLINE],ebx
AA		=	AA+1
ENDM
		add	esi,64
		mov	[PTR_padd_Data],esi
		jmp	Return

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data  => 8 octets (8*8*1 bits) un bit a 1 signifie que l'octet courant
;		doit etre modifie
; Other_Data => 16 octets de couleurs

		ALIGN	4
Correct_16::
		push	edi
		mov	cl,2
@@Loop_Update_16:
		mov	edx,[esi]
		add	esi,4

BB		=	0
REPT 4
  AA		=	BB
  REPT 8
Local		@@Update_16
		shr	edx,1
		jnc	@@Update_16
		mov	al,[ebp]
		inc	ebp
		mov	[edi+AA],al
  AA		=	AA+1
@@Update_16:
  ENDM
BB		=	BB+SCANLINE
ENDM
		add	edi,4*SCANLINE
		dec	cl
		jne	@@Loop_Update_16

		pop	edi
;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return




;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data  => 8 octets de couleurs Update
; Other_data => 6 octets (48=6*8 bits); 6 bits pour donner une position

		ALIGN	4
Correct_8::
		mov	ecx,Offset Patterns	; Pointeur sur la table des offsets
REPT 2
		mov	eax,[esi]

		mov	edx,[ebp]
		add	esi,4

		add	ebp,3
		push	esi
  REPT 2
		mov	ebx,edx

		shr	edx,6
		and	ebx,63

		mov	esi,edx

		shr	edx,6
		and	esi,63

		mov	ebx,[ecx+ebx*4]

		mov	esi,[ecx+esi*4]

		mov	[edi+ebx],al
		mov	[edi+esi],ah

		shr	eax,16
  ENDM
		pop	esi
ENDM
;		mov	[PTR_Other_data],ebp
;		mov	[PTR_Padd_Data],esi
		jmp	Return

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data  => 4 octets de couleurs Update
; Other_data => 3 octets (24=6*4 bits); 6 bits pour donner une position

		ALIGN	4
Correct_4::
		mov	ecx,Offset Patterns	; Pointeur sur la table des offsets
		mov	eax,[esi]

		mov	edx,[ebp]
		add	esi,4

		add	ebp,3
		push	esi
REPT 2
		mov	ebx,edx

		shr	edx,6
		and	ebx,63

		mov	esi,edx

		shr	edx,6
		and	esi,63

		mov	ebx,[ecx+ebx*4]

		mov	esi,[ecx+esi*4]

		mov	[edi+ebx],al
		mov	[edi+esi],ah

		shr	eax,16
ENDM
		pop	esi
;		mov	[PTR_Other_data],ebp
;		mov	[PTR_Padd_Data],esi
		jmp	Return



;*══════════════════════════════════════════════════════════════════════════*/
; Copie bete et mechante de la case de l'ancien vers le nouveau...

		ALIGN	4
ZeroMotionDecode::
		mov	ecx,[DeltaBase]
		add	ecx,edi

AA		=	0
REPT 8
		mov	eax,[ecx+AA]
		mov	ebx,[ecx+4+AA]
		mov	[edi+AA],eax
		mov	[edi+4+AA],ebx
AA		=	 AA+SCANLINE
ENDM
		jmp	TabCorrect[edx*4-1*4]

;*══════════════════════════════════════════════════════════════════════════*/
; Other_data => 1 octet: bits 0123 donnent la colonne, 4567 la ligne
; les valeurs sont signees

		ALIGN	4
SMotion8Decode::
		mov	al,[ebp]
		inc	ebp

		mov	ebx,eax
		sub	edx,5

		shl	eax,32-4
		shl	ebx,32-8
		sar	eax,28
		sar	ebx,28

		shl	ebx,6
		add	eax,edi
		add	eax,[DeltaBase]

		lea	ebx,[ebx+ebx*4]
		lea	ecx,[eax+ebx+4*320+4]

Copy8x8:
AA		=	0
REPT 8
		mov	eax,[ecx+AA]
		mov	ebx,[ecx+4+AA]
		mov	[edi+AA],eax
		mov	[edi+4+AA],ebx
AA		=	 AA+SCANLINE
ENDM
;		mov	[PTR_Padd_data],esi
;		mov	[PTR_Other_data],ebp
		jmp	TabCorrect[edx*4]

;*══════════════════════════════════════════════════════════════════════════*/
; Other_Data => 1 WORD qui donne un offset absolu sur l'image
;

		ALIGN	4
Motion8Decode::
		sub	edx,9
		mov	ecx,[ebp]
		add	ebp,2
		and	ecx,0FFFFh
		add	ecx,[XCF_BaseOldScr]
		jmp	Copy8x8

;*══════════════════════════════════════════════════════════════════════════*/


		ALIGN	4
ROMotion8Decode::
		movsx	ecx,word ptr[ebp]
		add	ebp,2
		add	ecx,[DeltaBase]
		sub	edx,48
		lea	ecx,[ecx+edi+4*320+4]

;		mov	[PTR_Other_Data],ebp
		jmp	Copy8x8

;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
RCMotion8Decode::
		movsx	ecx,byte ptr[ebp]
		movsx	eax,byte ptr[ebp+1]

		shl	eax,SHIFT		; 6
		add	ecx,[DeltaBase]

		sub	edx,52
		add	ecx,edi

		lea	eax,[eax+eax*4]		; eax*320
		add	ebp,2

		lea	ecx,[ecx+eax+4*320+4]

;		mov	[PTR_Other_Data],ebp
		jmp	Copy8x8

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data => 4 octets: bits 0123 donnent la colonne, 4567 la ligne
; les valeurs sont signees

		ALIGN	4
SMotion4Decode::
		push	ebp
		mov	ebp,[esi]
		add	esi,4
		mov	ebx,-4
		push	esi

		mov	esi,[DeltaBase]
		sub	edx,13
		add	esi,edi
@@Loop_SM4D:
		ror	ebp,8
		mov	ecx,ebp
		mov	eax,ebp
		add	ebx,4
		shl	ecx,4
		sar	eax,28
		sar	ecx,28
		shl	eax,SHIFT
		add	ecx,esi
		add	ecx,TabOfs[ebx]
		lea	eax,[eax+eax*4]
		add	ecx,eax
		cmp	ebx,12
		mov	Tab_Addr[ebx], ecx
		jne	@@Loop_SM4D

		pop	esi
		pop	ebp

CopyMotion4:
		mov	ecx,[Tab_Addr]
		push	esi
		mov	esi,[Tab_Addr+4]

		mov	eax,[ecx+SCANLINE*0]
		mov	ebx,[ecx+SCANLINE*1]
		mov	[edi+SCANLINE*0],eax
		mov	[edi+SCANLINE*1],ebx
		mov	eax,[ecx+SCANLINE*2]
		mov	ebx,[ecx+SCANLINE*3]
		mov	[edi+SCANLINE*2],eax
		mov	[edi+SCANLINE*3],ebx

		mov	ecx,[Tab_Addr+8]
		mov	eax,[esi+SCANLINE*0]
		mov	ebx,[esi+SCANLINE*1]
		mov	[edi+4+SCANLINE*0],eax
		mov	[edi+4+SCANLINE*1],ebx
		mov	eax,[esi+SCANLINE*2]
		mov	ebx,[esi+SCANLINE*3]
		mov	[edi+4+SCANLINE*2],eax
		mov	[edi+4+SCANLINE*3],ebx

		mov	esi,[Tab_Addr+12]
		mov	eax,[ecx+SCANLINE*0]
		mov	ebx,[ecx+SCANLINE*1]
		mov	[edi+SCANLINE*4],eax
		mov	[edi+SCANLINE*5],ebx
		mov	eax,[ecx+SCANLINE*2]
		mov	ebx,[ecx+SCANLINE*3]
		mov	[edi+SCANLINE*6],eax
		mov	[edi+SCANLINE*7],ebx

		mov	eax,[esi+SCANLINE*0]
		mov	ebx,[esi+SCANLINE*1]
		mov	[edi+4+SCANLINE*4],eax
		mov	[edi+4+SCANLINE*5],ebx
		mov	eax,[esi+SCANLINE*2]
		mov	ebx,[esi+SCANLINE*3]
		mov	[edi+4+SCANLINE*6],eax
		mov	[edi+4+SCANLINE*7],ebx

		pop	esi
;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	TabCorrect[edx*4]

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data => 8 octets = 4 UWORD donnant un offset absolu

		ALIGN	4
Motion4Decode::
		sub	edx,17
		mov	ecx,[XCF_BaseOldScr]

		mov	eax,[esi]
		mov	ebx,eax

		shr	eax,16
		and	ebx,0FFFFh

		add	eax,ecx
		add	ebx,ecx

		mov	[Tab_Addr],ebx
		mov	[Tab_Addr+4],eax

		mov	eax,[esi+4]
		mov	ebx,eax

		shr	eax,16
		and	ebx,0FFFFh

		add	eax,ecx
		add	ebx,ecx

		mov	[Tab_Addr+8],ebx
		mov	[Tab_Addr+12],eax

		add	esi,8
;		mov	[PTR_Padd_Data],esi
		jmp	CopyMotion4

;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
ROMotion4Decode::
		sub	edx,56
		mov	eax,[DeltaBase]
		mov	ebx,-4
		add	eax,edi
loopROMotion4:
		movsx	ecx,word ptr[esi]
		add	ebx,4
		add	ecx,eax
		add	esi,2
		add	ecx,TabOfs[ebx]
		cmp	ebx,12
		mov	Tab_Addr[ebx],ecx
		jne	loopROMotion4

;		mov	[PTR_Padd_Data],esi
		jmp	CopyMotion4

;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
RCMotion4Decode::
		mov	ebx,-4
		mov	ebp,[DeltaBase]
		sub	edx,60
		add	ebp,edi
loopRCMotion4:
		movsx	ecx,byte ptr[esi]
		movsx	eax,byte ptr[esi+1]
		shl	eax,SHIFT
 		add	ebx,4
		add	esi,2
		add	ecx,ebp
		lea	eax,[eax+eax*4]
		add	ecx,TabOfs[ebx]
		add	ecx,eax
		cmp	ebx,12
		mov	Tab_Addr[ebx],ecx
		jne	loopRCMotion4

;		mov	[PTR_Padd_Data],esi
		jmp	CopyMotion4

;*══════════════════════════════════════════════════════════════════════════*/
; Other_Data => 1 octet de couleur du bloc 8*8

		ALIGN	4
Uni8Decode::
		xor	eax,eax
		mov	al,[ebp]
		inc	ebp

		mov	ah,al
		mov	ebx,eax

		shl	eax,16
		or	eax,ebx
AA		=	0
REPT 8
		mov	[edi+AA],eax
		mov	[edi+4+AA],eax
AA		=	AA+SCANLINE
ENDM

;		mov	[PTR_Other_Data],ebp
		jmp	TabCorrect[edx*4-21*4]



;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data => 4 octets qui servent a remplir les blocs 4*4

		ALIGN	4
Uni4Decode::
		mov	eax,[esi]
		add	esi,4

		push	ebp
		push	esi

		xor	ebx,ebx
		xor	ecx,ecx

		mov	bl,al
		mov	cl,ah

		mov	bh,al
		mov	ch,ah

		shr	eax,16
		mov	esi,ebx

		shl	ebx,16
		mov	ebp,ecx

		shl	ecx,16
		or	ebx,esi

		or	ecx,ebp

AA		=	0
REPT 4
		mov	[edi+AA],ebx
		mov	[edi+4+AA],ecx
AA		=	AA+SCANLINE
ENDM

		xor	ebx,ebx
		xor	ecx,ecx

		mov	bl,al
		mov	cl,ah

		mov	bh,al
		mov	ch,ah

		mov	esi,ebx

		shl	ebx,16
		mov	ebp,ecx

		shl	ecx,16
		or	ebx,esi

		or	ecx,ebp
AA		=	4*SCANLINE
REPT 4
		mov	[edi+AA],ebx
		mov	[edi+4+AA],ecx
AA		=	AA+SCANLINE
ENDM

		pop	esi
		pop	ebp

;		mov	[Ptr_Padd_data],esi
		jmp	TabCorrect[edx*4-25*4]

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data  => 8 octets (8*8*1 bits)
; Other_Data => 2 octets de couleurs

		ALIGN	4
VR8Decode2::
		mov	ecx,[ebp]		; (CL, CH) couple de couleurs
		add	ebp,2
		mov	eax,ecx
		and	ecx,0FFFFh
		shl	eax,16
		or	ecx,eax
		mov	bl,2
@@Loop_VR8D2:
		mov	edx,[esi]
		add	esi,4

BB		=	0
REPT 4
AA		=	0
  REPT 2
  Local		@@Ok_1, @@Ok_2, @@Ok_3, @@Ok_4
		mov	eax,ecx
		shr	edx,1
		jnc	@@Ok_1
		mov	al,ch
@@Ok_1:
		shr	edx,1
		jc	@@Ok_2
		mov	ah,cl
@@Ok_2:
		ror	eax,16
		shr	edx,1
		jnc	@@Ok_3
		mov	al,ch
@@Ok_3:
		shr	edx,1
		jc	@@Ok_4
		mov	ah,cl
@@Ok_4:
		ror	eax,16
		mov	[edi+AA+BB],eax
AA		=	AA+4
  ENDM
BB		=	BB+SCANLINE
ENDM
		add	edi,4*SCANLINE
		dec	bl
		jne	@@Loop_VR8D2

;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data => 4 octets de couleurs
;	       16 octets (8*8*2 bits)

		ALIGN	4
VR8Decode4::
		lea	ebx,[esi+4]
		push	ebp

		mov	ebp,4
@@Loop_VR8D4:
		push	ebp
		mov	ebp,[ebx]		; Lit les index pour 16 pixels

		mov	ecx,ebp			; => Bit 2-3
		mov	edx,ebp			; => Bit 0-1

		shr	ecx,2
		and	edx,3

		shr	ebp,4
		and	ecx,3

		mov	al,[esi+edx]
		mov	edx,ebp

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		shl	eax,16
		and	edx,3

		shr	ecx,2

		shr	ebp,4
		and	ecx,3

		mov	al,[esi+edx]
		mov	edx,ebp

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		shr	ecx,2
		and	edx,3

		ror	eax,16
		and	ecx,3

		shr	ebp,4
		mov	[edi],eax

		mov	al,[esi+edx]
		mov	edx,ebp

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		shl	eax,16
		and	edx,3

		shr	ecx,2

		shr	ebp,4
		and	ecx,3

		mov	al,[esi+edx]
		mov	edx,ebp

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		ror	eax,16
		and	edx,3

		shr	ecx,2
		mov	[edi+4],eax

		shr	ebp,4
		and	ecx,3

		mov	al,[esi+edx]
		mov	edx,ebp

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		shl	eax,16
		and	edx,3

		shr	ecx,2

		shr	ebp,4
		and	ecx,3

		mov	al,[esi+edx]
		mov	edx,ebp

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		ror	eax,16

		shr	ecx,2
		and	edx,3

		and	ecx,3
		mov	[edi+SCANLINE],eax

		shr	ebp,4
		mov	al,[esi+edx]

		mov	ah,[esi+ecx]
		mov	ecx,ebp

		shl	eax,16
		mov	edx,ebp

		shr	ecx,2
		and	edx,3

		and	ecx,3
		mov	al,[esi+edx]

		add	ebx,4
		pop	ebp

		mov	ah,[esi+ecx]
		add	edi,2*SCANLINE

		ror	eax,16
		dec	ebp

		mov	[edi+SCANLINE+4-2*SCANLINE],eax
		jne	@@Loop_VR8D4

		pop	ebp
		mov	esi,ebx
;		mov	[PTR_Padd_Data],ebx
		jmp	Return


;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data => 24 octets (8*8*3 bits)
; Other_data=> 8 octets de couleurs

		ALIGN	4
VR8Decode8::
		mov	ebx,8
@@Loop_VR8D8:
		push	ebx
		mov	ebx,[esi]

		mov	edx,ebx
		mov	ecx,ebx

		shr	ecx,3
		and	edx,7

		shr	ebx,6
		and	ecx,7

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		shl	eax,16
		and	edx,7

		shr	ecx,3

		shr	ebx,6
		and	ecx,7

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		ror	eax,16
		and	edx,7

		shr	ecx,3
		mov	[edi],eax

		shr	ebx,6
		and	ecx,7

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		shr	ecx,3
		and	edx,7

		shl	eax,16
		and	ecx,7

		mov	al,[ebp+edx]		; No pairing :(
		mov	ah,[ebp+ecx]

		ror	eax,16

		mov	[edi+4],eax
		add	edi,SCANLINE

		add	esi,3
		pop	ebx

		dec	ebx
		jne	@@Loop_VR8D8

		add	ebp,8
;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return



;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data  => 32 octets (8*8*4 bits) formant des index dans la liste des couleurs
; Other_Data => Liste des couleurs

		ALIGN	4
VR8Decode16::
		mov	ebx,8
@@Loop_VR8D16:
		push	ebx
		mov	ebx,[esi]

		mov	ecx,ebx
		mov	edx,ebx

		shr	ecx,4
		and	edx,15

		shr	ebx,8
		and	ecx,15

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		shl	eax,16
		and	edx,15

		shr	ecx,4
		add	esi,4

		shr	ebx,8
		and	ecx,15

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		ror	eax,16
		and	edx,15

		shr	ecx,4
		mov	[edi],eax

		shr	ebx,8
		and	ecx,15

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		shl	eax,16
		and	edx,15

		shr	ecx,4

		shr	ebx,8
		and	ecx,15

		mov	al,[ebp+edx]
		mov	edx,ebx

		mov	ah,[ebp+ecx]
		mov	ecx,ebx

		ror	eax,16
		pop	ebx

		mov	[edi+4],eax
		add	edi,SCANLINE

		dec	ebx
		jne	@@Loop_VR8D16

;		mov	[PTR_Padd_Data],esi
		add	ebp,16

;		mov	[PTR_Other_Data],ebp
		jmp	Return

;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
VR4OneDecode2::
		mov	edx,[esi]
		mov	ecx,edx		; (CL, CH) couple de couleurs

		shr	ecx,16
		mov	eax,edx

		and	eax,0FFFF0000h
		lea	ebx,[edi+4*SCANLINE]

		or	ecx,eax
@@Loop_VR4D2:
		mov	eax,ecx
		shr	edx,1
		jnc	@@Ok1
		mov	al,ch
@@Ok1:
		shr	edx,1
		jc	@@Ok2
		mov	ah,cl
@@Ok2:
		ror	eax,16
		shr	edx,1
		jnc	@@Ok3
		mov	al,ch
@@Ok3:
		shr	edx,1
		jc	@@Ok4
		mov	ah,cl
@@Ok4:
		ror	eax,16
		mov	[edi],eax

		add	edi,SCANLINE
		cmp	edi,ebx
		jne	@@Loop_VR4D2

		ret



;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data => 4*(2+2) octets (4*4*1 bits d'indice dans une table de 2 couleurs)

		ALIGN	4
VR4Decode2::
		call	VR4OneDecode2

		add	edi,4-4*SCANLINE
		add	esi,4
		call	VR4OneDecode2

		add	edi,SCANLINE*4-4-4*SCANLINE
		add	esi,4
		call	VR4OneDecode2

		add	edi,4-4*SCANLINE
		add	esi,4
		call	VR4OneDecode2

		add	esi,4
;		mov	[PTR_Padd_Data],esi
		jmp	Return


;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
VR4OneDecode4::
		push	ebp
		mov	edx,[esi]	; PTR_Padd_Data

AA		=	0
REPT 2
		mov	ebp,edx
		mov	ecx,edx

		shr	ebp,4*2
		and	ecx,3

		shr	edx,2
		and	ebp,3

		mov	al,[esi+ecx+4]
		mov	bl,[esi+ebp+4]

		mov	ebp,edx
		mov	ecx,edx

		shr	ebp,4*2
		and	ecx,3

		shr	edx,2
		and	ebp,3

		mov	ah,[esi+ecx+4]
		mov	bh,[esi+ebp+4]

		shl	eax,16
		mov	ebp,edx

		shl	ebx,16
		mov	ecx,edx

		shr	ebp,4*2
		and	ecx,3

		shr	edx,2
		and	ebp,3

		mov	al,[esi+ecx+4]
		mov	bl,[esi+ebp+4]

		mov	ebp,edx
		mov	ecx,edx

		shr	ebp,4*2
		and	ecx,3

		shr	edx,2
		and	ebp,3

		mov	ah,[esi+ecx+4]
		mov	bh,[esi+ebp+4]

		ror	eax,16
		ror	ebx,16

  IF AA EQ 0
		shr	edx,2*4
  ENDIF
		mov	[edi+AA],eax
		mov	[edi+SCANLINE+AA],ebx
AA		=	AA+2*SCANLINE
ENDM
		pop	ebp
		ret

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data => 4*(4+4) octets (4*4*2 bits d'indice dans une table de 4 couleurs)

		ALIGN	4
VR4Decode4::
		call	VR4OneDecode4

		add	edi,4
		add	esi,8
		call	VR4OneDecode4

		add	edi,SCANLINE*4-4
		add	esi,8
		call	VR4OneDecode4

		add	edi,4
		add	esi,8
		call	VR4OneDecode4

		add	esi,8
;		mov	[PTR_Padd_data],esi
		jmp	Return


;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
VR4OneDecode8::
ofs		=	0
ofd		=	0
REPT 2
		mov	edx,[esi+ofs]
		push	esi

		mov	esi,edx
		mov	ecx,edx

		shr	esi,12			; 12 car 4*3 (4 coul de 3 bits par DWORD)
		and	ecx,7

		shr	edx,3
		and	esi,7

		mov	al,[ecx+ebp]
		mov	bl,[esi+ebp]

		mov	esi,edx
		mov	ecx,edx

		shr	esi,12
		and	ecx,7

		shr	edx,3
		and	esi,7

		mov	ah,[ecx+ebp]
		mov	bh,[esi+ebp]

		shl	eax,16
		mov	esi,edx

		shl	ebx,16
		mov	ecx,edx

		shr	esi,12
		and	ecx,7

		shr	edx,3
		and	esi,7

		mov	al,[ecx+ebp]
		mov	bl,[esi+ebp]

		mov	esi,edx
		mov	ecx,edx

		shr	esi,12
		and	ecx,7

		shr	edx,3
		and	esi,7

		mov	ah,[ecx+ebp]
		mov	bh,[esi+ebp]

		ror	eax,16
		pop	esi

		ror	ebx,16
		mov	[edi+ofd],eax

		mov	[edi+ofd+SCANLINE],ebx

ofd		=	ofd+SCANLINE*2
ofs		=	ofs+3
ENDM
		ret

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_Data  => 4*6 octets (8*8*3 bits) designant la couleur a utiliser
; Other_Data => 4*8 octets de couleurs

		ALIGN	4
VR4Decode8::
		call	VR4OneDecode8

		add	edi,4
		add	ebp,8
		add	esi,6

		call	VR4OneDecode8

		add	edi,SCANLINE*4-4
		add	ebp,8
		add	esi,6

		call	VR4OneDecode8

		add	edi,4
		add	ebp,8
		add	esi,6

		call	VR4OneDecode8

		add	ebp,8
		add	esi,6

;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return


;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
CrossOneDecode::
		mov	edx, [esi]
		mov	ecx, [esi]
		ror	edx, 16

		mov	ebx, ecx
		mov	eax, ecx
		mov	bl, ch
		shr	ebp, 1
		mov	ah, cl
		jnc	ok1
		mov	al, ch
ok1:
		ror	ebx, 8
		ror	eax, 16
		mov	bl, cl
		mov	al, cl
		shr	ebp, 1
		jc	ok2
		mov	ah, cl
ok2:
		shr	ebp, 1
		mov	bh, cl
		jnc	ok3
    		mov	bl, ch
ok3:
		shr	ebp, 1
		jnc	ok4
		mov	bh, dh
ok4:
		rol	eax, 16
		rol	ebx, 8
		mov	[edi+SCANLINE*0], eax
		mov	[edi+SCANLINE*1], ebx

		mov	ebx, ecx
		mov	eax, ecx
		mov	bl, ch
		mov	al, ch
		mov	bh, dl
		ror	eax, 8
		rol	ebx, 8
		mov	al, ch
		shr	ebp, 1
		mov	ah, dl
		jnc	ok5
    		mov	al, dl
ok5:
		shr	ebp, 1
		jnc	ok6
		mov	ah, dh
ok6:

		shr	ebp, 1
		jnc	ok7
		mov	bh, dl
ok7:
		shr	ebp, 1
		jc	ok8
		mov	bl, dl
ok8:
		rol	eax, 8
		ror	ebx, 8
		mov	[edi+SCANLINE*2], eax
		mov	[edi+SCANLINE*3], ebx

		ret

;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data => 4 octets de couleurs, puis 4*4 octets de decodage

		ALIGN	4
CrossDecode::
		push	ebp
		mov	ebp,[esi]
		add	esi,4

		call	CrossOneDecode

		add	esi,4
		add	edi,4

		call	CrossOneDecode

		add	esi,4
		add	edi,SCANLINE*4-4

		call	CrossOneDecode

		add	esi,4
		add	edi,4

		call	CrossOneDecode

		add	esi,4
		mov	PTR_Other_Data,esi

		pop	ebp
;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_data],ebp
		jmp	Return



;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data  => 8 octets (8*8*1 bits); un bit a 1 signifie qu'on utilise une
;		couleur provenant du stream; au contraire, 0 signifie qu'on
;		utilise la couleur principale
; Other_data => 1 octet de couleur principale; et n octets de couleur de stream

		ALIGN	4
PrimeDecode::
		mov	ch,[ebp]		; CH = Couleur principale
		inc	ebp
		mov	cl,8
@@Loop_Prime:
		mov	dl,[esi]
		inc	esi

		mov	bl,8
@@Loop_Prime2:
		shr	dl,1
		jc	@@Prime_Load
		mov	[edi],ch
		inc	edi

		dec	bl
		jne	@@Loop_Prime2
@@Ret_Prime:
		add	edi,SCANLINE-8
		dec	cl
		jne	@@Loop_Prime

;		mov	[PTR_Padd_data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return
@@Prime_Load:
		mov	al,[ebp]
		inc	ebp
		mov	[edi],al
		inc	edi
		dec	bl
		jne	@@Loop_Prime2
		jmp	@@Ret_Prime


;*══════════════════════════════════════════════════════════════════════════*/

		ALIGN	4
Bank1Decode::
		mov	dl,[ebp]
		inc	ebp
		mov	dh,8
@@Loop_B1D:
		mov	ebx,[esi]
		add	esi,4
		mov	al,bl
		mov	ah,bl
		shr	ah,4
		and	al,0Fh
		add	al,dl
		add	ah,dl
		shl	eax,16

		mov	al,bh
		mov	ah,bh
		shr	ah,4
		and	al,0Fh
		add	al,dl
		add	ah,dl
		ror	eax,16

		shr	ebx,16
		mov	[edi],eax
		mov	al,bl
		mov	ah,bl
		shr	ah,4
		and	al,0Fh
		add	al,dl
		add	ah,dl
		shl	eax,16

		mov	al,bh
		mov	ah,bh
		shr	ah,4
		and	al,0Fh
		add	al,dl
		add	ah,dl
		ror	eax,16

		mov	[edi+4],eax

		add	edi,SCANLINE
		dec	dh
		jne	@@Loop_B1D

;		mov	[PTR_Padd_Data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return



;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data  => 40 octets (8*8*5 bits) definissant le numero de bank et l'offset
;		dans la bank du pixel courant
; Other_Data => 1 octet donnant les 2 banks

		ALIGN	4
Bank2Decode::
		mov	dl,[ebp]	; Bank
		push	ebp

		mov	dh,dl
		mov	ebp,8
		shl	dl,4
		and	dh,0F0h
		sub	dl,dh
		mov	[Dummy],edx

@@B2_loop1:
		mov	edx,[esi+2]	; quintets (4)
		mov	ecx,[esi]	; quintets (4)
		shr	edx,4
		add	esi,5

REPT 4
local		b1, b2
		mov	al,cl
		mov	bl,dl

		and	al,0Fh
		and	bl,0Fh

		shr	ecx,5
		jc	b1
		add	al,byte ptr[Dummy]
b1:
		shr	edx,5
		jc	b2
		add	bl,byte ptr[Dummy]
b2:
		add	al,byte ptr[Dummy+1]
		add	bl,byte ptr[Dummy+1]
		ror	eax,8
		ror	ebx,8
ENDM

		mov	[edi],eax
		mov	[edi+4],ebx

		add	edi,SCANLINE
		dec	ebp
		jne	@@B2_loop1

		pop	ebp
;		mov	[PTR_Padd_Data],esi

		inc	ebp
;		mov	[PTR_Other_Data],ebp
		jmp	Return



;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data  => 8 octets (8*1 bits), bit=1 signifie lire la prochaine couleur dans la liste des couleurs
; Other_data => Liste des couleurs
; 2 bits de poids faibles de EDX indique le sens de parcours:
; 0 => Horizontal
; 1 => Vertical
; 2 => Oblique de gauche a droite et de haut en bas
; 3 => Oblique de droite a gauche et de haut en bas
		ALIGN	4
BlockDecode::
		and	dl,3
		shl	edx,8			; *64*4
		mov	eax,1 SHL 30
		add	edx,Offset Patterns	; PTR sur la table des adresses
@@BD_MainLoop:
		mov	ebx,[esi]
		add	esi,4
		mov	ah,32
@@BD_Loop:
		mov	ecx,[edx]
		add	edx,4

		shr	ebx,1
		jc	@@BD_LoadNext
@@BD_Write_Byte:
		mov	[edi+ecx],al
		dec	ah
		jne	@@BD_Loop

		mov	cl,al
		add	eax,eax
		mov	al,cl
		jnc	@@BD_MainLoop

;		mov	[PTR_Padd_data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return
@@BD_LoadNext:
		mov	al,[ebp]
		inc	ebp
		jmp	@@BD_Write_Byte


;*══════════════════════════════════════════════════════════════════════════*/
; Padd_data  => 8 octets (8*1 bits), bit=1 signifie lire la prochaine couleur dans la liste des couleurs
; Other_data => Liste de quartets: le 1er est le numero de banks, puis suivent les deplacements
; 2 bits de poids faibles de EDX indiquent le sens de parcours:
; 0 => Horizontal
; 1 => Vertical
; 2 => Oblique de gauche a droite et de haut en bas
; 3 => Oblique de droite a gauche et de haut en bas

		ALIGN	4
BlockBank1Decode::
		and	dl,3
		mov	ch,[ebp]		; Lit le No de bank
		shl	edx,8
		mov	eax,1 SHL 30
		shl	ch,4
		add	edx,Offset Patterns	; PTR sur la table des adresses
		mov	cl,4			; Shifter (soit 0 soit 4)
@@BBD_MainLoop:
		mov	ebx,[esi]
		add	esi,4
		mov	ah,32
		push	esi
@@BBD_Loop:
		mov	esi,[edx]
		add	edx,4

		shr	ebx,1
		jc	@@BBD_LoadNext
@@BBD_Write_Byte:
		mov	[edi+esi],ch
		dec	ah
		jne	@@BBD_Loop

		pop	esi
		add	eax,eax
		jnc	@@BBD_MainLoop

		test	cl,cl			; Si CL=4, il faut incrementer EBP
;		je	@@BBD_End
		je	Return
		inc	ebp
@@BBD_End:
;		mov	[PTR_Padd_data],esi
;		mov	[PTR_Other_Data],ebp
		jmp	Return
@@BBD_LoadNext:
		mov	al,[ebp]
		and	ch,0F0h
		shr	al,cl
		and	al,0Fh
		or	ch,al
		xor	cl,4
		jne	@@BBD_Write_Byte
		inc	ebp
		jmp	@@BBD_Write_Byte



XCF_Decompressor ENDP



;*══════════════════════════════════════════════════════════════════════════*/

XCF_InitDecompressor PROC	uses ebx esi edi ebp

		xor	eax, eax
		xor	esi, esi
		xor	edi, edi
		mov	ebp, -1

		ALIGN	4
loop1:
		mov	ebx, eax
		mov	ecx, eax
		mov	edx, eax
		shr	ebx, 3
		shr	ecx, 3
		and	edx, 7
		shl	ebx, SHIFT
		shl	ecx, SHIFT+2
		add	edx, ebx
		add	edx, ecx
		mov	Patterns[eax*4], edx

		mov	ebx, eax
		mov	ecx, eax
		mov	edx, eax
		and	ebx, 7
		and	ecx, 7
		shr	edx, 3
		shl	ebx, SHIFT
		shl	ecx, SHIFT+2
		add	edx, ebx
		add	edx, ecx
		mov	Patterns[256+eax*4], edx

		mov	ebx, esi
		mov	ecx, esi
		mov	edx, edi
		shl	ebx, SHIFT
		shl	ecx, SHIFT+2
		add	edx, ebx
		add	edx, ecx
		mov	Patterns[512+eax*4], edx

		mov	edx, edi
		add	ebx, ecx
		xor	edx, 7
		add	ebx, edx
		mov	Patterns[768+eax*4], ebx

		inc	eax

		add	esi, ebp
		jl	error1
		sub	edi, ebp
		jl	error2
		cmp	esi, 7
		jg	error3
		cmp	edi, 7
		jg	error4

		cmp	eax, 64
		jb	loop1
		ret
error1:
		sub	edi, ebp
		xor	esi, esi
		jmp	end2
error2:
		cmp	esi, 7
		jg	error5
		xor	edi, edi
		jmp	end2
error3:
		add	edi, 2
		jmp	end1
error4:
		mov	edi, 7
		add	esi, 2
		jmp	end2
error5:
		mov	edi, 1
end1:
		mov	esi, 7
end2:
		neg	ebp
		cmp	eax, 64
		jb	loop1

		ret

XCF_InitDecompressor ENDP

;*══════════════════════════════════════════════════════════════════════════*/

XCF_DecompressorHeight	PROC	height:DWORD
			pushad
			mov	eax,[height]

			mov	ebx,eax
			shr	ebx,3
			dec	ebx
			mov	byte ptr [XCF_Patch_MaxLine+1],bl

			shr	eax,3
			imul	eax,30		; ((a/8)*(MAX_WIDTH/8)*6)/8+4
			add	eax,4
			mov	dword ptr [XCF_Patch_CodeLen+2],eax

			popad
			ret
XCF_DecompressorHeight	ENDP

;*══════════════════════════════════════════════════════════════════════════*/
;			The
			End
