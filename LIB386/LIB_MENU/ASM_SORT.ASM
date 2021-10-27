	.386P
	.model SMALL, SYSCALL

;----------------------------------------------------------------------------
				.data
;----------------------------------------------------------------------------

public	NoLanguage	MySortCompFunc

strcmp	proto	SYSCALL

			.code

;//-------------------------------------------------------------------------

MySortCompFunc 		proc uses ebx ecx edx esi edi ebp,\
			ptra:DWORD, ptrb:DWORD

			mov	eax, ptrb
			mov	ecx, ptra
			push	[eax]
			push	[ecx]

			call	strcmp

			add	esp, 8

			ret

MySortCompFunc		endp

;----------------------------------------------------------------------------
;				The
				End







