		.386p
		jumps
		.model SMALL, SYSCALL

		.data

		include svga.ash

EVEN

Virgule         dw      00

ALIGN 4

Largeur         dd      0000
OffsetLine      dd      0000
PoidFort        dd      0000
Hauteur         dd      0000

		.code

		public   NoLanguage     ScaleLine
		public   NoLanguage     ScaleBox

;*══════════════════════════════════════════════════════════════════════════*/

ScaleBox        proc uses ebx esi edi ebp,\
		xs0:DWORD, ys0:DWORD, xs1:DWORD, ys1:DWORD, ptrs:DWORD,\
		xd0:DWORD, yd0:DWORD, xd1:DWORD, yd1:DWORD, ptrd:DWORD

		mov     esi, ptrs
		mov     edi, ptrd

		mov     eax, xd0
		add     edi, eax
		mov     ecx, xd1
		sub     ecx, eax                ; ECX delta x screen


		mov     edx, xs0                ; start x source
		add     esi, edx
		mov     eax, xs1                ; end x source
		sub     eax, edx                ; delta source
		inc     eax
		shl     eax, 16                 ; *65536

		xor     edx, edx
		div     ecx
		mov     [Virgule], ax
		shr     eax, 16                 ; poid fort
		mov     [PoidFort], eax

		inc     ecx
		mov     [Largeur], ecx
		mov     eax, 640
		sub     eax, ecx
		mov     [OffsetLine], eax

		mov     eax, yd0
		add     edi, TabOffLine[eax*4]
		mov     ecx, yd1
		sub     ecx, eax

		mov     eax, ys0
		add     esi, TabOffLine[eax*4]
		mov     edx, eax

		mov     eax, ys1
		sub     eax, edx
		inc     eax
		shl     eax, 16

		xor     edx, edx
		div     ecx
		inc     ecx
		mov     [Hauteur], ecx
		mov     ebp, eax
		shr     ebp, 16
		mov     dx, ax
		xor     bx, bx

yloop:          push    ebp
		push    edx
		push    ebx
		push    esi

		mov     ebp, [PoidFort]
		mov     dx, [Virgule]
		mov     ecx, [Largeur]
		xor     bx, bx
	EVEN
xloop:          mov     al, byte ptr[esi]
		mov	[edi], al
		inc	edi
		add     bx, dx                  ; virgule 16 bits
		adc     esi, ebp                ; poid fort
		dec	ecx
		jnz	xloop

		add     edi, [OffsetLine]       ; 640-delta x screen

		pop     esi
		pop     ebx
		pop     edx
		pop     ebp

		xor     eax, eax
		add     bx, dx                  ; virgule 16 bits
		adc     eax, ebp                ; poid fort
		add     esi, TabOffLine[eax*4]

		dec     [Hauteur]
		jnz     yloop

		ret

ScaleBox        endp

;*══════════════════════════════════════════════════════════════════════════*/

ScaleLine       proc uses ebx esi edi ebp,\
		xs0:DWORD, xs1:DWORD, ptrs:DWORD,\
		xe0:DWORD, xe1:DWORD, ptrd:DWORD


		mov     esi, ptrs
		mov     edi, ptrd

		mov     eax, xe0
		mov     ecx, xe1

		sub     ecx, eax                ; ECX delta x screen
		; jz

		mov     edx, xs0                ; start x source
		mov     eax, xs1                ; end x source

		sub     eax, edx                ; delta source

		shl     eax, 16                 ; *65536
		xor     edx, edx

		div     ecx
		mov     ebp, eax                ; EBP step intensite, EDX reste

		shr     ebp, 16                 ; poid fort

		mov     dx, ax

		xor     bx, bx

	EVEN
xloop:          mov     al, byte ptr[esi]
		mov	[edi], al
		inc	edi
		add     bx, dx                  ; virgule
		adc     esi, ebp                ; poid fort
		dec	ecx
		jnz	xloop

		ret

ScaleLine       endp

;----------------------------------------------------------------------------
;                       The
			End




cumul_low       dw      00
cumul_high      dw      00
y_sprite        dw      00

ScaleSprite     proc    uses es ds si di,\
		Xaff:WORD,Yaff:WORD,NumSpf:WORD,PtrSpf:PTR,\
		largeur:WORD,hauteur:WORD

		mov     ax,largeur
		mov     cs:word ptr[patch_largeur],ax

		mov     ax,hauteur
		mov     cs:word ptr[patch_hauteur],ax
		mov     cx,ax

		les     di,Log

		mov     bx,Yaff
		shl     bx,1
		mov     di,ss:[bx]
		add     di,Xaff

		lds     si,PtrSpf

		mov     bx,NumSpf
		shl     bx,1
		add     si,word ptr[si+bx]
		add     si,4                    ; saute start x y

		lodsw
		mov     cs:word ptr[patch_largeur_sprite],ax    ; largeur sprite
		mov     bx,ax

		lodsw                           ; hauteur sprite

		xor     dx,dx
		div     cx
		mov     bp,ax
		xor     ax,ax
		mov     y_sprite,ax
		div     cx
		mov     cumul_low,ax

		mov     ax,bp
		mul     bx
		mov     cumul_high,ax

		jmp     $+2

patch_hauteur   equ     $+1
		mov     cx,1234h

l1:             push    cx
		push    di

patch_largeur_sprite    equ     $+1
		mov     bx,1234h

patch_largeur   equ     $+1
		mov     cx,1234h

		mov     ax,bx                   ; delta
		xor     dx,dx
		div     cx                      ; /NbStep
		mov     bp,ax
		xor     ax,ax                   ; reste*65536
		div     cx                      ; /NbStep
		mov     dx,bp                   ; low
		mov     bp,ax                   ; high

		xor     bx,bx

		push    si

cl0:            mov     al,byte ptr[si]
		stosb
		add     bx,bp
		adc     si,dx
		loop    cl0

		pop     si

		mov     ax,cumul_low
		add     y_sprite,ax
		jnc     l4
		add     si,cs:word ptr[patch_largeur_sprite]
l4:             add     si,cumul_high

		pop     di
		add     di,320
		pop     cx
		loop    l1

		ret

ScaleSprite     endp





















