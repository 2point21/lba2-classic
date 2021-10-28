;----------------------------------------------------------------------------
;'			       FILES_A.ASM 386
;'			      (c) Adeline 1994
;----------------------------------------------------------------------------
				.386p

				.model SMALL, SYSCALL

				.data

				.code

			public	NoLanguage	Touch
			extrn	NoLanguage	OpenRead:PROC
			extrn	NoLanguage	Close:PROC

;----------------------------------------------------------------------------
Touch			proc	,\
			filename:DWORD
			local	handle:DWORD
			local	time:WORD
			local	date:WORD

			mov	[time], 0
			mov	[date], 0

;----------------------' Open

			push	filename
			call	OpenRead
			add	esp, 4
			mov	[handle], eax

;----------------------' Read Time and Date

			mov	ah, 2Ch
			int	21h

			xor	ax, ax
			mov	al, dh
			shr	al, 1
			mov	byte ptr [time],al	;' Secondes / 2

			xor	ax, ax
			mov	al, cl
			shl	ax, 5
			or	[time], ax		;' Minutes

			xor	ax, ax
			mov	al, ch
			shl	ax, 11
			or	[time], ax		;' Heure

			mov	ah, 2Ah
			int	21h

			mov	byte ptr [date],dl	;' Jour

			xor	ax, ax
			mov	al, dh
			shl	ax, 5
			or	[date], ax		;' Mois

			xor	ax, ax
			sub	cx, 1980
			mov	al, cl
			shl	ax, 9
			or	[date], ax		;' Année

;----------------------' Change time and date for this file

			mov	ah, 57h
			mov	al, 1
			mov	bx, word ptr [handle]
			mov	cx, [time]
			mov	dx, [date]
			int	21h

;----------------------' Close file

			push	[handle]
			call	Close
			add	esp, 4
			ret

Touch			endp
;----------------------------------------------------------------------------
;				The
				End





















