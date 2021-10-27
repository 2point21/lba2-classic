;----------------------------------------------------------------------------
;'				BUFFER_A.ASM 386
;'			      (c) Adeline 1993
;----------------------------------------------------------------------------
				.386p

				.model SMALL, SYSCALL

				.data

				.code

			public	NoLanguage	MovMem
			public	NoLanguage	RazMem
			public	NoLanguage	CompBuf

;----------------------------------------------------------------------------
MovMem			proc	uses esi edi,\
			pts:DWORD, ptd:DWORD, nb:DWORD

			mov	esi, pts
			mov	edi, ptd
			mov	eax, nb

			mov	ecx, eax
			shr	ecx, 2
			rep	movsd
			mov	ecx, eax
			and	ecx, 3
			rep	movsb
			ret
MovMem			endp
;----------------------------------------------------------------------------
RazMem			proc	uses edi,\
			buffer:DWORD, longueur:DWORD

			mov	edi, buffer
			mov	ecx, longueur
			mov	edx, ecx
			shr	ecx, 2
			xor	eax, eax
			rep	stosd
			mov	ecx, edx
			and	ecx, 11b
			rep	stosb
			ret
RazMem			endp
;----------------------------------------------------------------------------
CompBuf			proc	uses esi edi,\
			pt0:DWORD, pt1:DWORD, nb0:DWORD

			mov	esi, pt0
			mov	edi, pt1
			mov	eax, nb0

			mov	ecx, eax
			shr	ecx, 2
			rep	cmpsd
			jne	NoGood

			mov	ecx, eax
			and	ecx, 3
			rep	cmpsb
			jne	NoGood

			mov	eax, 1
			ret
NoGood:
			xor	eax, eax
			ret
CompBuf			endp
;----------------------------------------------------------------------------
;				The
				End





















