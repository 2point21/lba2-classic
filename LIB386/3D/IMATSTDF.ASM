;*══════════════════════════════════════════════════════════════════════════*

		.386p

		.model SMALL, SYSCALL

		include mat.inc

;*══════════════════════════════════════════════════════════════════════════*

		.DATA

		extrn	SYSCALL	SinTabF:DWORD

;*══════════════════════════════════════════════════════════════════════════*

		.CODE

		public	SYSCALL	InitMatrixStdF

COMMENT \
Explication sur la composition de la matrice de rotation:

M(Alpha) =	1   0   0
		0   Ca  -Sa
		0   Sa  Ca

M(Gamma) =	Cg  -Sg 0
		Sg  Cg  0
		0   0   1


M(Beta)  =	Cb  0   Sb
		0   1   0
		-Sb 0   Cb


M(Finale) = {M(Alpha)*M(Gamma)}*M(Beta)

\



;*══════════════════════════════════════════════════════════════════════════*
;void	InitMatrixStdF(S32 *MatDst, S32 alpha, S32 beta, S32 gamma) ;

;#pragma aux InitMatrixStdF	"*"		\
;	parm		[edi] [eax] [ebx] [ecx]	\
;	modify exact	[eax ebx ecx edx esi]

InitMatrixStdF	PROC

		lea	ecx,[ecx*4]
		lea	ebx,[ebx*4]

		lea	eax,[eax*4]
		and	ecx,4095*4

		and	ebx,4095*4
		and	eax,4095*4


SINA		EQU	[SinTabF+eax]		; Mettre l'adresse dans les reg
COSA		EQU	[SinTabF+eax+1024*4]	; pour l'optimisation
SINB		EQU	[SinTabF+ebx]
COSB		EQU	[SinTabF+ebx+1024*4]
SING		EQU	[SinTabF+ecx]
COSG		EQU	[SinTabF+ecx+1024*4]


				; ST(0) ST(1) ST(2) ST(3) ST(4) ST(5) ST(6) ST(7)

		fld	COSG	; Cg

		fmul	COSB	; CgCb

		fld	SING	; Sg    CgCb

		fchs		; -Sg   CgCb

		fld	COSG	; Cg    -Sg   CgCb

		fmul	SINB	; CgSb  -Sg   CgCb
		fxch	st(2)	; CgCb  -Sg   CgSb

		fstp	[edi].MAT_M11		; M11 = Cos(Gamma)*Cos(Beta)
				; -Sg   CgSb
		fstp	[edi].MAT_M12		; M12 = -Sin(Gamma)
				; CgSb

		fld     SINB	; Sb    CgSb

		fmul	SINA	; SbSa  CgSb

		fld	SING	; Sg    SbSa  CgSb

		fmul	COSA	; SgCa  SbSa  CgSb

		fld	COSG	; Cg    SgCa  SbSa  CgSb

		fmul	COSA	; CgCa  SgCa  SbSa  CgSb
		fxch	st(3)	; CgSb  SgCa  SbSa  CgCa

		fstp	[edi].MAT_M13		; M13 = Cos(Gamma)*Sin(Beta)
				; SgCa  SbSa  CgCa
		fmul	COSB	;SgCaCb SbSa  CgCa

		fld	SINB	; Sb   SgCaCb SbSa  CgCa

		fmul	SING	; SbSg SgCaCb SbSa  CgCa
		fxch	st(3)	; CgCa SgCaCb SbSa  SbSg


		fstp	[edi].MAT_M22		; M22 = Cos(Alpha)*Cos(Gamma)
				;SgCaCb SbSa  SbSg

		faddp	st(1),st; M21   SbSg
		fxch	st(1)	; SbSg  M21

		fmul	COSA	;SbSgCa M21

		fld	COSB	; Cb   SbSgCa M21

		fmul	SINA	; CbSa SbSgCa M21
		fxch	st(2)	; M21  SbSgCa CbSa

		fstp	[edi].MAT_M21		; M21 = Sin(Gamma)*Cos(Alpha)*Cos(Beta)
				;SbSgCa CbSa	;      +Sin(Beta)*Sin(Alpha)

		fsubrp	st(1),st; M23

		fld	SINA	; Sa    M23

		fmul	SING	; SaSg  M23

		fld	COSA	; Ca    SaSg  M23

		fmul	SINB	; CaSb  SaSg  M23

		fld	COSA	; Ca    CaSb  SaSg  M23

				; ST(0) ST(1) ST(2) ST(3) ST(4) ST(5) ST(6) ST(7)
		fmul	COSB	; CaCb  CaSb  SaSg  M23
		fxch	st(2)	; SaSg  CaSb  CaCb  M23

		fld	st(0)	; SaSg  SaSg  CaSb  CaCb  M23

		fmul	COSB	;SaSgCb SaSg  CaSb  CaCb  M23
		fxch	st(4)	; M23   SaSg  CaSb  CaCb  SaSgCb

		fstp	[edi].MAT_M23   ; M23 = Sin(Beta)*Sin(Gamma)*Cos(Alpha)
					;      -Cos(Beta)*Sin(Alpha)
				; SaSg  CaSb  CaCb SaSgCb
		fmul	SINB	;SaSgSb CaSb  CaCb SaSgCb

		fld	SINA	; Sa   SaSgSb CaSb  CaCb SaSgCb

		fmul	COSG	; SaCg SaSgSb CaSb  CaCb SaSgCb
		fxch	st(4)	;SaSgCb SaSgSb CaSb CaCb  SaCg

		fsubrp	st(2),st;SaSgSb M31   CaCb  SaCg

		faddp	st(2),st; M31   M33   M32
		fxch	st(2)	; M32   M33   M31

		fstp	[edi].MAT_M32	; M32 = Sin(Alpha)*Cos(Gamma)

		fstp	[edi].MAT_M33	; M33 = Sin(Alpha)*Sin(Gamma)*Sin(Beta)
					;      +Cos(Alpha)*Cos(Beta)

		xor	eax, eax

		fstp	[edi].MAT_M31	; M31 = Sin(Alpha)*Sin(Gamma)*Cos(Beta)
					;      -Cos(Alpha)*Sin(Beta)

		mov	[edi].MAT_MTX, eax

		mov	[edi].MAT_MTY, eax
		mov	[edi].MAT_MTZ, eax

		ret

InitMatrixStdF	ENDP

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
