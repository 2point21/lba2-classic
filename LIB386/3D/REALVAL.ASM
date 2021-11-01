
		.386p

		.model	SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
		.data

T_REAL_VALUE_HR	STRUCT

	StartValue		DWORD		0
	EndValue		DWORD		0
	TimeValue      		DWORD		0
	MemoTicks		DWORD		0

T_REAL_VALUE_HR	ENDS

		extrn	SYSCALL	TimerRefHR:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		.code

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	InitRealAngleHR
		public	SYSCALL	InitRealValueHR
		public	SYSCALL	GetRealValueHR
		public	SYSCALL	InitRealAngleConstHR

;*══════════════════════════════════════════════════════════════════════════*
;void	InitRealAngleHR(	S32 StartAngle, S32 EndAngle, S32 TimeValue,
;				T_REAL_VALUE_HR *RealVal);

;#pragma aux InitRealAngleHR	"*"		\
;	parm		[eax] [ebx] [ecx] [edi]	\
;	modify exact	[eax ebx edx]

InitRealAngleHR	proc

		and	eax, 4095
		and	ebx, 4095

;*══════════════════════════════════════════════════════════════════════════*
;void	InitRealValueHR(	S32 StartValue, S32 EndValue, S32 TimeValue,
;				T_REAL_VALUE_HR *RealVal);

;#pragma aux InitRealValueHR	"*"		\
;	parm		[eax] [ebx] [ecx] [edi]	\
;	modify exact	[edx]

InitRealValueHR::

		mov	edx, [TimerRefHR]
		mov	[edi].StartValue, eax
		mov	[edi].EndValue, ebx
		mov	[edi].TimeValue, ecx
		mov	[edi].MemoTicks, edx		; memoticks init

		ret

InitRealAngleHR	endp

;*══════════════════════════════════════════════════════════════════════════*
;void	InitRealAngleConstHR(	S32 StartAngle, S32 EndAngle, S32 TimeQuart,
;				T_REAL_VALUE_HR *RealVal);

;#pragma aux InitRealAngleConstHR	"*"	\
;	parm		[eax] [ebx] [ecx] [edi]	\
;	modify exact	[eax ebx ecx edx]

InitRealAngleConstHR	proc

		push	eax
		sub	eax, ebx

		test	eax, 2048
		jz	short irac0

		neg	eax
irac0:
		and	eax, 4095

		imul	ecx

		and	ebx, 4095
		add	eax, 512	; round to nearest before divide

		adc	edx, 0

		shrd	eax, edx, 10	; / 1024 = 1/4 rotation

		mov	ecx, eax
		pop	eax

		jmp	InitRealValueHR

InitRealAngleConstHR	endp

;*══════════════════════════════════════════════════════════════════════════*
;S32	GetRealValueHR(T_REAL_VALUE_HR *RealVal);

;#pragma aux GetValueHR	"*"			\
;	parm		[esi]			\
;	modify exact	[eax ebx ecx edx esi]

GetRealValueHR	proc

		mov	edx, [TimerRefHR]
		mov	ecx, [esi].MemoTicks	; memoticks

		mov	eax, [esi].EndValue	; End Value
		sub	edx, ecx

		mov	ebx, [esi].StartValue	; StartValue
		mov	ecx, [esi].TimeValue	; TimeValue

		cmp	edx, ecx		; if step >= nbstep
		jae	elapsed			; exit

		mov	esi, ecx

		shr	esi, 1
		sub	eax, ebx		; StartValue

		imul	edx			; resul EDX:EAX

		add	eax, esi

		adc	edx, 0

		idiv	ecx			; /NbStep

		add	eax, ebx		; StartValue
						; return EndValue
		ret
elapsed:
		xor	ecx, ecx

		mov	[esi].TimeValue, ecx
		ret

GetRealValueHR	endp

;*══════════════════════════════════════════════════════════════════════════*
;S32	GetRealAngleHR(T_REAL_VALUE_HR *RealVal);

;#pragma aux GetAngleHR	"*"			\
;	parm		[esi]			\
;	modify exact	[eax ebx ecx edx esi]

GetRealAngleHR	proc

		mov	edx, [TimerRefHR]
		mov	ecx, [esi].MemoTicks	; memoticks

		mov	eax, [esi].EndValue	; End Value
		sub	edx, ecx

		mov	ebx, [esi].StartValue	; StartValue
		mov	ecx, [esi].TimeValue	; TimeValue

		cmp	edx, ecx		; if step >= nbstep
		jae	elapsed			; exit

		mov	esi, ecx

		shr	esi, 1
		sub	eax, ebx		; Delta Value

		cmp	eax, -2048
		jl	short art1

		cmp	eax, 2048
		jle	short ok

		sub	eax, 8192
art1:
		add	eax, 4096
ok:
		imul	edx

		add	eax, esi

		adc	edx, 0

		idiv	ecx			; /NbStep

		add	eax, ebx		; StartValue
						; return EndValue
		and	eax, 4095

		ret
elapsed:
		xor	ecx, ecx
		and	eax, 4095

		mov	[esi].TimeValue, ecx
		ret

GetRealAngleHR	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
