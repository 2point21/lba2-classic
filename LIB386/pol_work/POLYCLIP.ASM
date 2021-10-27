; **************************************************************************
; **************************************************************************
; **************************************************************************
; ***                                                                    ***
; ***                         POLYGON LIBRARY                            ***
; ***                                                                    ***
; **************************************************************************
; ***                                                                    ***
; ***                              (Adeline Software Intl.)              ***
; ***                           1995-96-97                               ***
; ***                                                                    ***
; **************************************************************************
; **************************************************************************
; **************************************************************************

			OPTION	PROC:PRIVATE
			OPTION	SCOPED
			OPTION	LANGUAGE:SYSCALL


			INCLUDE	FILLER.INC




;			**************
;			*** PUBLIC ***
;			**************

;					******************
;					*** PROCEDURES ***
;					******************
PUBLIC	SYSCALL		ClipperZ



;					*****************
;					*** VARIABLES ***
;					*****************

;					*** PUBLIC DATA ***


;					*** MODULE DATA ***




;			***************
;			*** INCLUDE ***
;			***************

INCLUDE			F:\PROJET\LIB386\H\FILLER.INC
INCLUDE			IAMMX.INC


;			**************
;			*** MACROS ***
;			**************


;			***************
;			*** EQUATES ***
;			***************




;			**************************
;			*** SEGMENTATION MODEL ***
;			**************************

			.386
;			.MODEL	FLAT, SYSCALL



;			************
;			*** DATA ***
;			************
;			.DATA
_DATA			SEGMENT	USE32 PUBLIC PARA 'DATA'

;				******************
;				*** Extrn data ***
;				******************

		extrn	SYSCALL	Chooser_RtnI:DWORD
		extrn	SYSCALL	Chooser_RtnF:DWORD
		extrn	SYSCALL	Chooser_RtnM:DWORD
		extrn	SYSCALL	Chooser_Result:DWORD

Extrn	SYSCALL		Status_Int		:	WORD
Extrn	SYSCALL		Status_Float		:	WORD



;				*******************
;				*** Global data ***
;				*******************

;					*******************
;					*** PUBLIC data ***
;					*******************



;					*******************
;					*** MODULE data ***
;					*******************
			ALIGN	4
Clip_Funct		dd	Init_Clip

Precalc_Offset		Label	DWORD
AA			=	0
REPT 16
			dd	AA*(Size STRUC_CLIPVERTEX)
AA			=	AA+1
ENDM







;				******************
;				*** Local data ***
;				******************
			ALIGN	4




_DATA			ENDS


;			************
;			*** CODE ***
;			************
;			.CODE
_TEXT			SEGMENT	USE32 PARA PUBLIC 'CODE'
			ASSUME	CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT


;				******************
;				*** Extrn proc ***
;				******************
		extrn	SYSCALL	Chooser:PROC


;				*******************
;				*** Global proc ***
;				*******************







; Call:
;	ESI = Src vertex list (list of STRUC_CLIPVERTEX)
;	EDI = Dst vertex list (list of STRUC_CLIPVERTEX)
;	EAX = Clipping plane
;	EBX = Sign of comparison (select the visible half space)
;		0  means positive
;		-1 means negative
;	ECX = Nb of vertex in src list
; Return:
;	EAX = Nb of vertex in new list

ClipperZ		PROC
			push	ebp
			test	ebx,ebx			; Which half-space?

			jne	@@Case_Negative

			mov	ebx,[Precalc_Offset+ecx*4-4]
			xor	ecx,ecx			; ECX = Nb of points in the new vertex list

			add	ebx,esi
			sub	esi,Size STRUC_CLIPVERTEX

			push	ebx			; EBX = Last vertex (in Local Variable)
			mov	ebp,eax			; EBP = Clipping orthogonal plane

			mov	eax,dword ptr [ebx].V_Z0
			xor	edx,edx 		; EDX = 1 if vertex in wrong half-space

			cmp	eax,ebp

			setl	dl			; EDX = Our flag

			; *****************
			; *** Main loop ***
			; *****************
			ALIGN	4
@@Test_Next_Pt:						; Check next point
			mov	eax,dword ptr [esi+Size STRUC_CLIPVERTEX].V_Z0
			add	esi,Size STRUC_CLIPVERTEX; ESI = Current point

			cmp	eax,ebp
			jnl	@@Visible

			; *************************************************
			; *** Current vertex is in the wrong half-space ***
			; *************************************************
			test	edx,edx			; Was last point visible ?
			jne	@@TestEnd

			call	[Clip_Funct]

			add	edi,Size STRUC_CLIPVERTEX; A point was created: registers it!
			inc	ecx

			mov	ebx,esi			; EBX = Old point
			mov	eax,[esp]		; Get Last vertex in Local Variable

			mov	edx,1			; Last point not visible
			cmp	esi,eax

			jne	@@Test_Next_Pt

			jmp	@@End

			; ************************************************
			; *** Current vertex is in the good half-space ***
			; ************************************************
			ALIGN	4
@@Visible:
			test	edx,edx
			je	@@Pas_Inters

			call	[Clip_Funct]

			add	edi,Size STRUC_CLIPVERTEX
			inc	ecx
@@Pas_Inters:
			mov	eax,[esi].V_X0		; Visible
			mov	edx,[esi].V_Y0

			mov	[edi].V_X0,eax
			mov	[edi].V_Y0,edx

			mov	eax,[esi].V_Z0		; Visible
			mov	edx,dword ptr [esi].V_MapU

			mov	[edi].V_Z0,eax
			mov	eax,dword ptr [esi].V_Light

			mov	dword ptr [edi].V_MapU,edx
			mov	dword ptr [edi].V_Light,eax

		mov	ax,[esi].V_Dummy
		mov	[edi].V_Dummy,ax


			inc	ecx
			add	edi,Size STRUC_CLIPVERTEX

			xor	edx,edx			; Last vertex in the correct half-space
@@TestEnd:
			mov	ebx,esi			; EBX = Old point
			mov	eax,[esp]		; Get Last vertex in Local Variable

			cmp	esi,eax
			jne	@@Test_Next_Pt
			jmp	@@End


			; *********************
			; *** Case NEGATIVE ***
			; *********************
@@Case_Negative:
			mov	ebx,[Precalc_Offset+ecx*4-4]
			xor	ecx,ecx			; ECX = Nb of points in the new vertex list

			add	ebx,esi
			sub	esi,Size STRUC_CLIPVERTEX

			push	ebx			; EBX = Last vertex (in Local Variable)
			mov	ebp,eax			; EBP = Clipping orthogonal plane

			mov	eax,dword ptr [ebx].V_Z0
			xor	edx,edx 		; EDX = 1 if vertex in wrong half-space

			cmp	eax,ebp

			setg	dl			; EDX = Our flag

			; *****************
			; *** Main loop ***
			; *****************
			ALIGN	4
@@NegTest_Next_Pt:					; Check next point
			mov	eax,dword ptr [esi+Size STRUC_CLIPVERTEX].V_Z0
			add	esi,Size STRUC_CLIPVERTEX; ESI = Current point

			cmp	eax,ebp
			jng	@@NegVisible

			; *************************************************
			; *** Current vertex is in the wrong half-space ***
			; *************************************************
			test	edx,edx			; Was last point visible ?
			jne	@@NegTestEnd

			call	[Clip_Funct]

			add	edi,Size STRUC_CLIPVERTEX; A point was created: registers it!
			inc	ecx

			mov	ebx,esi			; EBX = Old point
			mov	eax,[esp]		; Get Last vertex in Local Variable

			mov	edx,1			; Last point not visible
			cmp	esi,eax

			jne	@@NegTest_Next_Pt

			jmp	@@End

			; ************************************************
			; *** Current vertex is in the good half-space ***
			; ************************************************
			ALIGN	4
@@NegVisible:
			test	edx,edx
			je	@@NegPas_Inters

			call	[Clip_Funct]

			add	edi,Size STRUC_CLIPVERTEX
			inc	ecx
@@NegPas_Inters:
			mov	eax,[esi].V_X0		; Visible
			mov	edx,[esi].V_Y0

			mov	[edi].V_X0,eax
			mov	[edi].V_Y0,edx

			mov	eax,[esi].V_Z0		; Visible
			mov	edx,dword ptr [esi].V_MapU

			mov	[edi].V_Z0,eax
			mov	eax,dword ptr [esi].V_Light

			mov	dword ptr [edi].V_MapU,edx
			mov	dword ptr [edi].V_Light,eax

		mov	ax,[esi].V_Dummy
		mov	[edi].V_Dummy,ax


			inc	ecx
			add	edi,Size STRUC_CLIPVERTEX

			xor	edx,edx			; Last vertex in the correct half-space
@@NegTestEnd:
			mov	ebx,esi			; EBX = Old point
			mov	eax,[esp]		; Get Last vertex in Local Variable

			cmp	esi,eax
			jne	@@NegTest_Next_Pt

@@End:
			pop	ebx
			mov	eax,ecx

			pop	ebp
			ret
ClipperZ		ENDP





;				*******************
;				*** Local proc ***
;				*******************

Clipping_Z		PROC
			push	ecx
			push	ebp

					; *** Factor ***
			mov	ecx,[esi].V_Z0
			mov	edx,[ebx].V_Z0

			sub	ecx,edx			; ECX = (CurZ-LastZ)
			sub	edx,ebp

			mov	eax,edx			; EAX:EDX = 65536*(LastZ-ClipZ)
			mov	[edi].V_Z0,ebp

			shl	eax,24

			sar	edx,8

			idiv	ecx

			mov	ecx,eax
			neg	ecx			; ECX = (65536*(ClipZ-LastZ)) / (CurZ-LastZ)

					; *** X0 ***
			mov	eax,[esi].V_X0
			mov	ebp,[ebx].V_X0
			sub	eax,ebp			; EAX = CurX-LastX

			imul	ecx

			shr	eax,24
			shl	edx,8
			or	eax,edx

			add	ebp,eax
			mov	[edi].V_X0,ebp

					; *** Y0 ***
			mov	eax,[esi].V_Y0
			mov	ebp,[ebx].V_Y0
			sub	eax,ebp

			imul	ecx

			shr	eax,24
			shl	edx,8
			or	eax,edx

			add	ebp,eax
			mov	[edi].V_Y0,ebp

					; *** MapU ***
			mov	eax,dword ptr [esi].V_MapU
			mov	ebp,dword ptr [ebx].V_MapU

			and	eax,0FFFFh
			and	ebp,0FFFFh

			sub	eax,ebp

			imul	ecx

			shr	eax,24
			shl	edx,8
			or	eax,edx

			add	ebp,eax
			mov	[edi].V_MapU,bp


					; *** MapV ***
			mov	eax,dword ptr [esi].V_MapU
			mov	ebp,dword ptr [ebx].V_MapU

			shr	eax,16
			shr	ebp,16

			sub	eax,ebp

			imul	ecx

			shr	eax,24
			shl	edx,8
			or	eax,edx

			add	ebp,eax
			mov	[edi].V_MapV,bp


			; dummy (index point)

			mov	ax,word ptr [esi].V_Dummy
			mov	[edi].V_Dummy, ax

					; *** Light ***
			mov	eax,dword ptr [esi].V_Light
			mov	ebp,dword ptr [ebx].V_Light

			and	eax,0FFFFh
			and	ebp,0FFFFh

			sub	eax,ebp

			imul	ecx

			shr	eax,24
			shl	edx,8
			or	eax,edx

			add	ebp,eax
			mov	[edi].V_Light,bp

			pop	ebp
			pop	ecx

			ret
Clipping_Z		ENDP




Clipping_ZMMX		PROC
			emms
Clipping_ZFPU::
			push	ecx
			push	ebp

			fldcw	[Status_Int]

					; *** Factor ***
			mov	ecx,[esi].V_Z0
			mov	edx,[ebx].V_Z0

			mov	[edi].V_Z0,ebp
			sub	ecx,edx			; ECX = (CurZ-LastZ)

			sub	ebp,edx			; EBP = (ClipZ-LastZ)
			push	ecx

			push	ebp

			fild	dword ptr [esp]		; (ClipZ-LastZ)

			fild	dword ptr [esp+4]	; (CurZ-LastZ)  (ClipZ-LastZ)

			fdivp	st(1),st		; Delta


					; *** X0 ***
			mov	ecx,[esi].V_X0
			mov	ebp,[ebx].V_X0

			sub	ecx,ebp			; EAX = CurX-LastX
			mov	[esp+4],ebp		; Save LastX

			mov	[esp],ecx		; Save dX
			mov	ecx,[esi].V_Y0

			mov	ebp,[ebx].V_Y0
			mov	edx,dword ptr [esi].V_MapU

			sub	ecx,ebp
			push	ebp			; Save LastY

			push	ecx			; Save dY
			mov	ebp,dword ptr [ebx].V_MapU

			and	edx,0FFFFh
			and	ebp,0FFFFh

			sub	edx,ebp
			push	ebp			; Save LastU

			push	edx			; Save dU
			mov	eax,dword ptr [esi].V_MapU

			shr	eax,16
			mov	ebp,dword ptr [ebx].V_MapU

			shr	ebp,16
			mov	edx,dword ptr [esi].V_Light

			sub	eax,ebp
			push	ebp			; Save LastV

			push	eax			; Save dV
			mov	ebp,dword ptr [ebx].V_Light

			and	edx,0FFFFh
			and	ebp,0FFFFh

			sub	edx,ebp
			push	ebp			; Save LastL

			push	edx			; Save dL


			mov	ebp,[esp+10*4]
			mov	ecx,[esp+11*4]

			; Stack(ESP):
			; ESP + 9*4  => LastX
			; ESP + 8*4  => dX
			; ESP + 7*4  => LastY
			; ESP + 6*4  => dY
			; ESP + 5*4  => LastU
			; ESP + 4*4  => dU
			; ESP + 3*4  => LastV
			; ESP + 2*4  => dV
			; ESP + 1*4  => LastL
			; ESP + 0*4  => dL


			fild	dword ptr [esp+9*4]	; LastX  D

			fild	dword ptr [esp+8*4]	; dX  LastX  D

			fild	dword ptr [esp+7*4]	; LastY  dX  LastX  D

			fild	dword ptr [esp+6*4]	; dY  LastY  dX  LastX  D

			fxch	st(2)			; dX  LastY  dY  LastX  D

			fmul	st,st(4)		; mX  LastY  dY  LastX  D

			fild	dword ptr [esp+5*4]	; LastU  mX  LastY  dY  LastX  D

			fild	dword ptr [esp+4*4]	; dU  LastU  mX  LastY  dY  LastX  D

			fxch	st(4)			; dY  LastU  mX  LastY  dU  LastX  D

			fmul	st,st(6)		; mY  LastU  mX  LastY  dU  LastX  D
			fxch	st(2)			; mX  LastU  mY  LastY  dU  LastX  D

			faddp	st(5),st		; LastU  mY  LastY  dU  NX  D
			fxch	st(3)			; dU  mY  LastY  LastU  NX  D

			fmul	st,st(5)		; mU  mY  LastY  LastU  NX  D
			fxch	st(1)			; mY  mU  LastY  LastU  NX  D

			faddp	st(2),st		; mU  NY  LastU  NX  D

			fild	dword ptr [esp+2*4]	; dV  mU  NY  LastU  NX  D

			fild	dword ptr [esp+3*4]	; LastV  dV  mU  NY  LastU  NX  D

			fxch	st(5)			; NX  dV  mU  NY  LastU  LastV  D

			fistp	[edi].V_X0		; dV  mU  NY  LastU  LastV  D

			fmul	st,st(5)		; mV  mU  NY  LastU  LastV  D
			fxch	st(1)			; mU  mV  NY  LastU  LastV  D

			faddp	st(3),st		; mV  NY  NU  LastV  D
			fxch	st(1)			; NY  mV  NU  LastV  D

			fistp	[edi].V_Y0		; mV  NU  LastV  D

			fild	dword ptr [esp+0*4]	; dL  mV  NU  LastV  D

			fild	dword ptr [esp+1*4]	; LastL  dL  mV  NU  LastV  D

			fxch	st(2)			; mV  dL  LastL  NU  LastV  D

			faddp	st(4),st		; dL  LastL  NU  NV  D

			fmulp	st(4),st		; LastL  NU  NV  mL
			fxch	st(1)			; NU  LastL  NV  mL

			fistp	dword ptr [esp] 	; LastL  NV  mL

			faddp	st(2),st		; NV  NL

			fistp	dword ptr [esp+4]

			fistp	[edi].V_Light

			mov	eax,[esp]
			mov	edx,[esp+4]

			shl	edx,16
			and	eax,0FFFFh

			or	eax,edx
			add	esp,12*4

			mov	dword ptr [edi].V_MapU,eax


		; dummy (index point)
		mov	ax,word ptr [esi].V_Dummy
		mov	[edi].V_Dummy, ax



			fldcw	[Status_Float]

			ret
Clipping_ZMMX		ENDP




Init_Clip		PROC

			mov	[Chooser_RtnI], offset Clipping_Z	; Integer
			mov	[Chooser_RtnF], offset Clipping_ZFPU	; FPU
			mov	[Chooser_RtnM], offset Clipping_ZMMX	; MMX
			mov	[Chooser_Result], offset Clip_Funct	; Pointer
			jmp	Chooser

Init_Clip		ENDP



_TEXT			ENDS

;			The
			End
