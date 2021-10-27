;----------------------------------------------------------------------------
;				TIMER_A.ASM 386
;			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p

				.model SMALL, SYSCALL

				.data

DIVISOR			equ	23864		; 1193180 (Base Freq) / 50
FREQUENCE		equ	50		; 1193180 (Base Freq) / 23864

		public	NoLanguage	TimerSystem
		public	NoLanguage	TimerRef
		public	NoLanguage	NbFramePerSecond
		public	NoLanguage	WaitNbTicks
		public	NoLanguage	CmptFrame
		public	NoLanguage	Cmpt_18

TimerRef		dd	0000
NbFramePerSecond      	dw	00
WaitNbTicks		dw	1
CmptFrame		dw	00
Cmpt_18			dw	00
TimerSystem		dd	0000

                		.data

		extrn	NoLanguage      Old_PM08_Sel:WORD
		extrn	NoLanguage      Old_PM08_Off:DWORD

				.code

		public	NoLanguage	SetTimer
		public	NoLanguage	NewIntPM08
		public	NoLanguage	NewProc08

;----------------------------------------------------------------------------
SetTimer		proc	,\
			counter:DWORD

			cli

			mov	ecx, counter
			mov	al, 36h
			out	43h, al
			mov	al, cl
			out	40h, al
			mov	al, ch
			out	40h, al

			sti
			ret
SetTimer		endp

;----------------------------------------------------------------------------
comment	@

NewIntPM08		proc	far

			push	eax
			push	ds

			mov	ax, DGROUP
			mov	ds, ax

			inc	dword ptr [TimerSystem]
			inc	dword ptr [TimerRef]

			dec	word ptr [WaitNbTicks]
			jnz	short noevaluate

			mov	word ptr [WaitNbTicks], FREQUENCE

			mov	ax, word ptr [CmptFrame]
			mov	word ptr [NbFramePerSecond], ax

			mov	word ptr [CmptFrame], 0
noevaluate:
			add	word ptr [Cmpt_18], DIVISOR
			jc	short oldint08

			mov	al, 20h
			out	20h, al

			pop	ds
			pop	eax
			iretd
oldint08:
			pop	ds
			pop	eax

			int	0F2h				; old int08

			iretd

NewIntPM08		endp

	@

;----------------------------------------------------------------------------
NewIntPM08		proc	far

			push	eax
			push	ecx
			push	ds

			mov	ax, DGROUP
			mov	ds, ax

			inc	dword ptr [TimerSystem]
			inc	dword ptr [TimerRef]

			dec	word ptr [WaitNbTicks]
			jnz	short noevaluate

			mov	word ptr [WaitNbTicks], FREQUENCE

			mov	ax, word ptr [CmptFrame]
			mov	word ptr [NbFramePerSecond], ax

			mov	word ptr [CmptFrame], 0
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

NewIntPM08		endp

;----------------------------------------------------------------------------

NewProc08		proc

			push	eax
			push	ds

			mov	ax, DGROUP
			mov	ds, ax

			inc	dword ptr [TimerRef]
			inc	dword ptr [TimerSystem]

			dec	word ptr [WaitNbTicks]
			jnz	short noevaluate

			mov	word ptr [WaitNbTicks], FREQUENCE
			mov	ax, word ptr [CmptFrame]
			mov	word ptr [NbFramePerSecond], ax
			mov	word ptr [CmptFrame], 0
noevaluate:

			pop	ds
			pop	eax

			ret

NewProc08		endp

;----------------------------------------------------------------------------
;				The
				End