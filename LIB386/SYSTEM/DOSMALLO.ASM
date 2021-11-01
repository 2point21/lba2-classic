;*══════════════════════════════════════════════════════════════════════════*
		.386p

		.model SMALL, SYSCALL

;*══════════════════════════════════════════════════════════════════════════*
		.DATA

;*══════════════════════════════════════════════════════════════════════════*
MemInfo			label	DWORD

LargestBlockAvail       dd      ?
MaxUnlockedPage         dd      ?
LargestLockablePage     dd      ?
LinAddrSpace            dd      ?
NumFreePagesAvail       dd      ?
NumPhysicalPagesFree    dd      ?
TotalPhysicalPages      dd      ?
FreeLinAddrSpace        dd      ?
SizeOfPageFile          dd      ?
Reserved                dd      3 dup (?)

;*══════════════════════════════════════════════════════════════════════════*
		.CODE

;*══════════════════════════════════════════════════════════════════════════*
		public	SYSCALL	NormDosMalloc
		public	SYSCALL	NormDosFree
		public	SYSCALL	NormDosMshrink
		public	SYSCALL	DosAvailableMem

;*══════════════════════════════════════════════════════════════════════════*
;void	*NormDosMalloc( S32 size)

;#pragma aux	NormDosMalloc		\
;	parm   	[ebx]	  		\
;	modify 	[ecx edx esi edi]

NormDosMalloc	proc

		test	ebx, ebx
		jz	Error

		cmp	ebx, 640*1024
		ja	Error

		add	ebx, 15		; round to including paragraph
		mov	eax, 0100h	; Allocate DOS memory

		shr	ebx, 4		; size in paragraph
		push	ebp

		add	ebx, 2		; add 2 para for alignment and header
		int	31h

		pop	ebp
		jc	Error

		shl	eax, 4
		and	edx, 0FFFFh	; handle

		add	eax, 32		; move ahead
		or	edx, 010000h

		and	eax, (0FFFFh SHL 4) AND NOT 31	; align 32

		mov	[eax-4], edx

		ret
Error:
		xor	eax, eax

		ret

NormDosMalloc	endp

;*══════════════════════════════════════════════════════════════════════════*
;void	NormDosFree(void *ptr)

;#pragma aux	NormDosFree		\
;	parm	[edx]	  		\
;	modify 	[eax ebx ecx esi edi]

NormDosFree	proc

		push	ebp
		mov	edx, [edx-4]

		mov	eax, 101h
		and	edx, 0FFFFh


		int	31h

		pop	ebp

		ret

NormDosFree	endp

;*══════════════════════════════════════════════════════════════════════════*
;void	*NormDosMshrink(void *ptr, U32 size)

;#pragma aux	NormDosMshrink		\
;	parm	[edx] [ebx]	  	\
;	modify 	[eax ecx esi edi]

NormDosMshrink	proc

		add	ebx, 15		; round to including paragraph
		push	ebp

		shr	ebx, 4		; size in paragraph
		push	edx

		mov	edx, [edx-4]
		add	ebx, 2		; add 2 para for alignment and header

		mov	eax, 102h
		and	edx, 0FFFFh

		int	31h

		pop	eax
		jnc	ok

		xor	eax, eax
ok:		pop	ebp

		ret

NormDosMshrink	endp

;*══════════════════════════════════════════════════════════════════════════*
;U32	DosAvailableMem()

;#pragma aux	DosAvailableMem		\
;	modify 	[ebx ecx edx esi edi]

DosAvailableMem	proc

		xor	ebx, ebx
		mov	eax, 0100h	; Allocate DOS memory

		push	ebp
		dec	ebx

		int	31h

		and	ebx, 0FFFFh
		pop	ebp

		shl	ebx, 4

		mov	eax, ebx

		ret

DosAvailableMem	endp

;*══════════════════════════════════════════════════════════════════════════*
;		The
		END
