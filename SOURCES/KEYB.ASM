;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		.386p

		.model SMALL, SYSCALL

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		.DATA

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		public	SYSCALL		Int15DataStart	; for VMM_lock
		public	SYSCALL		Int15DataEnd

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		public	SYSCALL		Int15Trans

		public	SYSCALL		Old_PM15_Off
		public	SYSCALL		Old_PM15_Sel

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		extrn	SYSCALL	TimerSystemHR:DWORD

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Int15DataStart	label	dword

Int15Trans	dd	0

Int15DataEnd	label	dword

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

Old_PM15_Off	dd	00
Old_PM15_Sel    dd	00

timer_temp	dd	0

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		.CODE

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		public	SYSCALL	Int15CodeStart	; for VMM_lock
		public	SYSCALL	Int15CodeEnd

;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
		public	SYSCALL	Int15CodeStart	; for VMM_lock
		public	SYSCALL	Int15CodeEnd


;//ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Int15CodeStart	label	dword

Int15Handler	PROC    FAR

		push	ebx			; save used registers
		push	ecx
		push	ds

		pushf

		mov	ecx, _DATA		; reset data segment
		xor	ebx, ebx

		mov     ds, ecx

		cmp	ax, 4FEEh		; triggered int 9 ?
		je	ok

		cmp	ax, 4FFEh		; triggered int 9 ?
		jne	skip
ok:
		mov	ecx, [Int15Trans]

		test	ecx, ecx
		jz	skip

		mov	al, cl
		mov	[Int15Trans], ebx
skip:
		popf

		mov	ecx, [Old_PM15_Off]	; mess with the stack
		mov	ebx, [Old_PM15_Sel]	; to reset all the register
				      		; and allow to do a retf
		xchg	ecx, [esp+1*4]		; to chain with the bios
		xchg	ebx, [esp+2*4]		; handler

		pop	ds
		retf				; jmp to bios keyboard handler

Int15Handler 	endp

Int15CodeEnd	label	dword

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
; make sure there is a 5 ms delay between 2 calls

iodelay		PROC

		push	eax
		push	ebx
restart:
		mov	eax, [timer_temp]
wait_timer:
		mov	ebx, [TimerSystemHR]
		cmp	ebx, eax
		jb	wait_timer

		add	ebx, 5
		mov	[timer_temp], ebx

		test	eax, eax
		jz	restart

		pop	ebx
		pop	eax

		ret

iodelay		ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void		keyboard_write(U8 cmd)	;

;#pragma aux	keyboard_write	\
;	parm	[ebx]		\
;	modify	[eax ecx]

keyboard_write	PROC

		mov	ecx, 10000h
wrt_wait:
		in	al, 64h

		call	iodelay

		test	al, 2
		jz	wrt_send

		dec	ecx
		jnz	wrt_wait

		jmp	wrt_error
wrt_send:
		mov	al, bl
		out	60h, al

		call	iodelay

		xor	eax, eax
		ret
wrt_error:
		mov	eax, 1
		ret

keyboard_write	ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;void		keyboard_cmd(U8 cmd)	;

;#pragma aux	keyboard_cmd	\
;	parm	[ebx]		\
;	modify	[eax ecx]

keyboard_cmd	PROC

		mov	ecx, 10000h
cmd_wait:
		in	al, 64h

		call	iodelay

		test	al, 2
		jz	cmd_send

		dec	ecx
		jnz	cmd_wait

		jmp	cmd_error
cmd_send:
		mov	al, bl
		out	64h, al

		call	iodelay

		mov	ecx, 10000h
cmd_accept:
		in	al, 64h

		call	iodelay

		test	al, 2
		jz	cmd_ok

		dec	ecx
		jnz	cmd_accept
cmd_error:
		mov	eax, 1
		ret
cmd_ok:
		xor	eax, eax
		ret

keyboard_cmd	ENDP

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
comment	@

;S32		keyboard_read()	;

;#pragma aux	keyboard_read	\
;modify		[eax ecx]

keyboard_read	PROC

		mov	ecx, 10000h
rd_wait:
		in	al, 64h

		call	iodelay

		test	al, 1
		jz	rd_send

		dec	ecx
		jnz	rd_wait

		jmp	rd_error
rd_send:
		xor	eax, eax
		in	al, 60h

		call	iodelay

		ret
rd_error:
		mov	eax, -1
		ret

keyboard_read	ENDP

	@

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;		The
		END
