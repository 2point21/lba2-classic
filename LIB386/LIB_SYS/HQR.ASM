	.386P
	.MODEL	SMALL, SYSCALL

	.CODE

	public	NoLanguage	HQR_GiveIndex

HQR_GiveIndex	proc ,\
		index:DWORD, nbindex:DWORD, ptrlist:DWORD

		mov	ecx, nbindex
		or	ecx, ecx
		jz	not_found

		mov	edx, ecx
		shl	edx, 1			; edx = ecx * 2
		mov	eax, ecx
		shl	eax, 4			; eax = ecx * 16
		sub	eax, edx		; eax = ecx * 14 T_HQR_BLOC

		mov	edx, ptrlist
		add	edx, eax

		mov	eax, index

	ALIGN	4
se0:
	REPT	8
		sub	edx, 14			; sizeof( T_HQR_BLOC )
		cmp	word ptr[edx], ax
		jz	found
		dec	ecx
		jz	not_found
	ENDM
		jmp	se0

not_found:	xor	eax, eax
		ret

found:		mov	eax, edx
		ret

HQR_GiveIndex	endp

		end
