;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		.386P

		.model SMALL, SYSCALL

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		.DATA

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		extrn	SYSCALL		TanTab	: DWORD

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		.CODE

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		PUBLIC  SYSCALL 	GetAngleVector2D

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
;S32 	GetAngleVector2D( S32 x, S32 z)			;

;#pragma aux GetAngleVector2D	"*"			\
;	parm		[eax] [ebx]	 		\
;	modify 		[ecx edx esi]

GetAngleVector2D proc

		xor	esi, esi		; ESI = Bit field

		test	eax, eax
		jz	exit0

		jns	ok1

		neg	eax
		or	esi, 1			; ESI : bit 0 = 1 if x coord is negative
ok1:
		test	ebx, ebx
		jz	exit1

		jns	ok2

		neg	ebx
		or	esi, 2			; ESI : bit 1 = 1 if z coord is negative
ok2:
		cmp	eax, ebx
		jbe	noswap

		mov	edx, eax
		mov	eax, ebx

		mov	ebx, edx
		or	esi, 4			; ESI : bit 2 = 1 if |z|>|x|
noswap:
		mov	ecx, ebx
		mov	edx, eax

		shl	eax, 30

		shr	ecx, 1

		shr	edx, 2
		add	eax, ecx

		adc	edx, 0

		div	ebx			; eax = 1.1.30 = tan(แ)

		mov	ebx, ((0+512)/2)*4	; = 0*2+512*2
		xor	edx, edx

		mov     ecx, 512*2
		cmp	eax, TanTab[ebx]
		jge	ga3
ga0:
		shr	ebx, 1

		mov	ecx, ebx
		add	ebx, edx
ga2:
		test	ebx, 2
		jnz	angle

		cmp	eax, TanTab[ebx]
		jl	ga0
ga3:
		shr	ebx, 1

		mov	edx, ebx
		add	ebx, ecx

		jmp	ga2
angle:
		mov	ebx, TanTab[edx*2]
		mov	ecx, TanTab[edx*2+4]

		shr	edx, 1
		sub	ecx, eax

		sub	eax, ebx

		cmp	eax, ecx
		jbe	ok

		inc	edx
ok:
		mov	eax, edx
		test	esi, 4

		jz	no2

		mov	eax, 1024
		sub	eax, edx
no2:
		mov	edx, eax
		test	esi, 2

		jz	no1

		mov	eax, 2048
		sub	eax, edx
no1:
		test	esi, 1
		jz	no0

		neg	eax
no0:
		and	eax, 4095
		ret
exit0:
		xor	eax, eax
		test	ebx, ebx

		jns	no4

		mov	eax, 2048
no4:
		ret
exit1:
		mov	eax, 1024
		test	esi, 1

		jz	no3

		mov	eax, 4096-1024
no3:
		ret

GetAngleVector2D endp

;*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
		;The
		 End
