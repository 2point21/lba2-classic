;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
;                              MIXER_I.ASM 386
;                             (c) Adeline 1993
;*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*

;*--------------------------------------------------------------------------*

		.386
		jumps

		.model  SMALL, SYSCALL

		.DATA

		PUBLIC	NoLanguage	Mixer_listfcts
		PUBLIC	NoLanguage	Mixer_Driver_Enable

Mixer_listfcts		dd	0
Mixer_Driver_Enable	dd	0

		.CODE

		PUBLIC	NoLanguage	MixerAskVars
		PUBLIC	NoLanguage	MixerChangeVolume
		PUBLIC	NoLanguage	MixerGetVolume

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

MixerCallFuncs	PROC

		cmp	byte ptr[Mixer_Driver_Enable], 0
		je	return

		shl	eax, 2
		add	eax, dword ptr[Mixer_listfcts]
		jmp	dword ptr[eax]
return:
		ret

MixerCallFuncs	ENDP

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------


MixerAskVars	PROC

		mov	eax, 0
		jmp	MixerCallFuncs

MixerAskVars	ENDP

;----------------------------------------------------------------------------

MixerChangeVolume PROC

		mov	eax, 1
		jmp	MixerCallFuncs

MixerChangeVolume ENDP

;----------------------------------------------------------------------------

MixerGetVolume	PROC

		mov	eax, 2
		jmp	MixerCallFuncs

MixerGetVolume	ENDP

;----------------------------------------------------------------------------

MixerGetInfo	PROC

		mov	eax, 3
		jmp	MixerCallFuncs

MixerGetInfo	ENDP

;----------------------------------------------------------------------------

		END
