			.386p

			.model SMALL, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
DIVISOR			equ	5966		; 1193180 (Base Freq) / 200
FREQUENCE		equ	200		; 1193180 (Base Freq) / 5966

;----------------------------------------------------------------------------
ALIGN 4

		public	SYSCALL	TimerDataStart	; for VMM_lock !!!
		public	SYSCALL	TimerDataEnd

TimerDataStart		label	dword

		public	SYSCALL	TimerSystemHR
		public	SYSCALL	TimerRefHR
		public	SYSCALL	TimerRef
		public	SYSCALL	NbFramePerSecond
		public	SYSCALL	TimerLock
		public	SYSCALL	CmptFrame

TimerSystemHR		dd	0000		; always running at 1000 Hz
TimerRefHR		dd	0000		; running at 1000 Hz if not locked
TimerRef		dd	0000		; running at 50 Hz if not locked
TimerLock		dd	0000		; stop TimerRef if TRUE

NbFramePerSecond      	dd	0000
WaitNbTicks		dd	1
CmptFrame		dd	0000

		public	SYSCALL	CmptMemoTimerRef

CmptMemoTimerRef	dd	0000
MemoTimerRefHR		dd	0000
MemoTimerRef		dd	0000

;Cmpt_18			dw	00
Cmpt_50Hz		db	0

;		public	SYSCALL	Old_PM08_Sel
;		public	SYSCALL	Old_PM08_Off

;Old_PM08_Sel		dw	0
;Old_PM08_Off		dd	0

TimerDataEnd		label	dword

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
		public	SYSCALL	TimerStart	; for VMM_lock !!!
		public	SYSCALL	TimerEnd

;		public	SYSCALL	SetDivisor
;		public	SYSCALL	NewIntPM08
		public	SYSCALL	NewProc08
		public	SYSCALL	SaveTimer
		public	SYSCALL	RestoreTimer
		public	SYSCALL	SetTimerHR

;----------------------------------------------------------------------------
;void	SaveTimer()	;

;#pragma aux SaveTimer	"*"	\
;	modify	[eax ecx edx]

SaveTimer	proc

		mov	ecx, [CmptMemoTimerRef]

		test	ecx, ecx
		jne	st0

		mov	edx, [ TimerRefHR ]
		mov     eax, [ TimerRef ]
		mov	[MemoTimerRefHR], edx
		mov	[MemoTimerRef], eax

st0:		inc	ecx

		mov	[CmptMemoTimerRef], ecx

		ret

SaveTimer	endp

;----------------------------------------------------------------------------
;void	RestoreTimer()	;

;#pragma aux RestoreTimer	"*"	\
;	modify	[eax edx]

RestoreTimer	proc

		mov	eax, [CmptMemoTimerRef]

		dec	eax
		js	rt0

		mov	[CmptMemoTimerRef], eax
		jnz	rt0

		mov	edx, [MemoTimerRefHR]
		mov	eax, [MemoTimerRef]
		mov     [ TimerRefHR ], edx
		mov     [ TimerRef ], eax
rt0:
		ret

RestoreTimer	endp

;----------------------------------------------------------------------------
;void	SetTimerHR(U32 timer)	;

;#pragma aux SetTimerHR	"*"	\
;	parm	[eax]
;	modify	[ecx edx]

SetTimerHR	proc

		xor	edx, edx
		mov	ecx, 20
		mov     [ TimerRefHR ], eax
		div	ecx
		mov     [ TimerRef ], eax

		ret

SetTimerHR	endp

;----------------------------------------------------------------------------
;void	SetDivisor(U32 counter)	;

;#pragma aux SetDivisor	"*"	\
;	parm	[ecx]
;	modify	[eax]

Comment	@

SetDivisor      proc

                cli

                mov     al, 36h
                out     43h, al
                mov     al, cl
                out     40h, al
                mov     al, ch
                out     40h, al

                sti

                ret

SetDivisor      endp

	@

;----------------------------------------------------------------------------
TimerStart	label	dword

Comment	@

NewIntPM08	proc	far

		push	eax
		push	ecx
		push	ds

		mov	eax, DGROUP
		mov	ds, eax

		add	dword ptr [TimerSystemHR], 1000/FREQUENCE

		cmp	dword ptr [TimerLock], 0
		jnz	no_timerref

		mov	cl, [Cmpt_50Hz]
		mov	eax, [TimerRefHR]
		inc	cl
		add	eax, 1000/FREQUENCE
		and	cl, 3
		mov	[TimerRefHR], eax
		mov	[Cmpt_50Hz], cl
		jnz	no_timerref
		inc	Dword ptr[TimerRef]
no_timerref:
		dec	dword ptr [WaitNbTicks]
		jnz	short noevaluate

		mov	dword ptr [WaitNbTicks], FREQUENCE

		mov	eax, dword ptr [CmptFrame]
		mov	dword ptr [NbFramePerSecond], eax

		mov	dword ptr [CmptFrame], 0
noevaluate:
		add	word ptr [Cmpt_18], DIVISOR
		jc	short oldint08

		mov	al, 20h
		out	20h, al

		pop	ds
		pop	ecx
		pop	eax
		iretd
oldint08:
		xor	eax, eax
		mov	ecx, [Old_PM08_Off]
		mov	ax,  [Old_PM08_Sel]

		xchg	ecx, [esp+4]
		xchg	eax, [esp+8]

		pop	ds
		retf

NewIntPM08	endp

	@

;----------------------------------------------------------------------------
NewProc08	proc

		push	eax
		push	ecx
		push	ds

		mov	eax, DGROUP
		mov	ds, eax

		add	dword ptr [TimerSystemHR], 1000/FREQUENCE

		cmp	dword ptr [TimerLock], 0
		jnz	no_timerref

		mov	cl, [Cmpt_50Hz]
		mov	eax, [TimerRefHR]
		inc	cl
		add	eax, 1000/FREQUENCE
		and	cl, 3
		mov	[TimerRefHR], eax
		mov	[Cmpt_50Hz], cl
		jnz	no_timerref
		inc	dword ptr [TimerRef]
no_timerref:
		dec	dword ptr [WaitNbTicks]
		jnz	short noevaluate

		mov	dword ptr [WaitNbTicks], FREQUENCE
		mov	eax, dword ptr [CmptFrame]
		mov	dword ptr [NbFramePerSecond], eax
		mov	dword ptr [CmptFrame], 0
noevaluate:
		pop	ds
		pop	ecx
		pop	eax

		ret	4

NewProc08	endp

TimerEnd	label	dword

;----------------------------------------------------------------------------
;		The
		End
