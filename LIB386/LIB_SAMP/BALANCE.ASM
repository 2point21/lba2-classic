
		.386p
		jumps

		.model SMALL, C

		.data

		extrn	NoLanguage P_SinTab:WORD


		.code

		public	NoLanguage	Balance


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

		END
