;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

		include cpu.inc

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
		extrn	SYSCALL	ProcessorSignature:DWORD
		extrn	SYSCALL	ProcessorFeatureFlags:DWORD

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	Chooser_Result

Chooser_Result	dd	0	; Result

		public	SYSCALL	Chooser_RtnMF
		public	SYSCALL	Chooser_RtnM
		public	SYSCALL	Chooser_RtnF
		public	SYSCALL	Chooser_RtnI

Chooser_RtnMF	dd	0	; MMX and FPU
Chooser_RtnM	dd	0	; MMX
Chooser_RtnF	dd	0	; FPU
Chooser_RtnI	dd	0	; Integer

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	Chooser

;*══════════════════════════════════════════════════════════════════════════*
Chooser		proc

		pushad

		mov	ebx, [ProcessorSignature]
		mov	ecx, [Chooser_Result]

		shr	ebx, 8				; remove model/stepping
		mov	eax, [ProcessorFeatureFlags]

		and	ebx, 0F4Fh			; keep manufact/FPU/family
		xor	esi, esi			; esi = 0

		cmp	bl, 45h				; 4 = FPU; 5 = Pentium
		jb 	noFPU				; FPU+Pentium or better?

		test	eax, 1 SHL 23			; MMX flag ?
		jz	noMMX

		cmp	bh, 1				; Manufacturer should be
		jne	noMMXFPU			; Intel to use MMX+FPU

		mov	edx, [Chooser_RtnMF]		; try MMX+FPU

		test	edx, edx			; MMX Rtn exist?
		jnz	ok
noMMXFPU:
		mov	edx, [Chooser_RtnM]		; try MMX

		test	edx, edx			; MMX Rtn exist?
		jnz	ok
noMMX:
		cmp	bh, 1				; Manufacturer should be
		jne	noFPU				; Intel to use FPU

		mov	edx, [Chooser_RtnF]		; try FPU

		test	edx, edx
		jnz	ok				; FPU Rtn exist?
noFPU:
		mov	edx, [Chooser_RtnI]		; default to integer
ok:
		mov	[Chooser_RtnMF], esi		; 0
		mov	[Chooser_RtnM], esi		; 0

		mov	[Chooser_RtnF], esi		; 0
		mov	[Chooser_RtnI], esi		; 0

		mov	[Chooser_Result], edx
		mov	[ecx], edx

		popad

		jmp	[Chooser_Result]

Chooser		endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
