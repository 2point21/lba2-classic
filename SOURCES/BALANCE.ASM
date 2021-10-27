
		.386p
		jumps

		.model SMALL, SYSCALL

		.data

		extrn	NoLanguage P_SinTab:WORD

		.code

		public	NoLanguage	Balance
		public	NoLanguage	BalanceWord


Balance		PROC uses ebx,\
		bal:DWORD, vol:DWORD, volleft:DWORD, volright:DWORD

		mov	eax, bal
		shl	eax, 1
		mov	edx, vol

		xor	ecx, ecx
		mov	cx, word ptr[P_SinTab + eax]
		imul	ecx, edx
		shr	ecx, 14

		mov	ebx, volright
		mov	dword ptr[ebx], ecx

		xor	ecx, ecx
		mov	cx, word ptr[P_SinTab + 512 + eax]
		imul	ecx, edx
		shr	ecx, 14

		mov	ebx, volleft
		mov	dword ptr[ebx], ecx

		ret

Balance		ENDP


BalanceWord	PROC uses ebx,\
		bal:DWORD, vol:DWORD, volleft:DWORD, volright:DWORD

		mov	eax, bal
		shl	eax, 1
		mov	edx, vol

		xor	ecx, ecx
		mov	cx, word ptr[P_SinTab + eax]
		imul	ecx, edx
		shr	ecx, 14

		mov	ebx, volright
		mov	word ptr[ebx], cx

		xor	ecx, ecx
		mov	cx, word ptr[P_SinTab + 512 + eax]
		imul	ecx, edx
		shr	ecx, 14

		mov	ebx, volleft
		mov	word ptr[ebx], cx

		ret

BalanceWord	ENDP

		END
