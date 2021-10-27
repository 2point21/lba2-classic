;----------------------------------------------------------------------------
			.386p
			.model SMALL, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	PtrFont
			PUBLIC	SYSCALL	InterLeave
			PUBLIC	SYSCALL	InterSpace

;----------------------------------------------------------------------------
PtrFont			dd	0
InterLeave		dd	1
InterSpace		dd	10

;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			EXTRN	SYSCALL	AffMask	:PROC

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	SizeFont
			PUBLIC	SYSCALL	CarFont
			PUBLIC	SYSCALL	Font

;----------------------------------------------------------------------------
;#define	ColorFont(color)	ColorMask(color)

;----------------------------------------------------------------------------
;#define	SetFont(ptrfont, interleave, interspace)	\
;{								\
;	PtrFont		= ptrfont	;			\
;	InterLeave	= interleave	;			\
;	InterSpace	= interspace	;			\
;}

;----------------------------------------------------------------------------
;S32	SizeFont(char *str)	;

;#pragma aux SizeFont	"*"	\
;	parm	[esi]		\
;	modify	[eax ecx edx edi]

SizeFont		proc

			mov	edi, [PtrFont]
			xor	eax, eax
			xor	edx, edx
;			--------------------------------
NextCar:
			mov	dl,[esi]
			inc	esi

			test	edx, edx
			jz	EndSizeFont

			cmp	edx, 32
			jne	PlusLoin0

			add	eax, [InterSpace]
			jmp	NextCar
PlusLoin0:
			mov	ecx, [edi+edx*4]
			add	eax, [InterLeave]
			mov	dl,  [edi+ecx]
			add	eax, edx
			jmp	NextCar

;			--------------------------------
EndSizeFont:
			ret
SizeFont		endp

;----------------------------------------------------------------------------
;S32	CarFont(S32 x, S32 y, char c)	;

;#pragma aux CarFont	"*"		\
;	parm	[ebx] [ecx] [eax]	\
;	modify	[edx esi edi]

CarFont			proc

			cmp	eax, 32			;' SPACE
			je	Space

			mov	edi, [PtrFont]

			mov	esi, edi
			add	edi, [edi+eax*4]

			movzx	edx, byte ptr[edi]

			add	edx, [InterLeave]

			push	edx

			Call	AffMask

			pop	eax

			ret
Space:
			mov	eax, [InterSpace]
			ret

CarFont			endp

;----------------------------------------------------------------------------
;S32	Font(S32 x, S32 y, char *str);

;#pragma aux Font	"*"		\
;	parm	[ebx] [ecx] [esi]	\
;	modify	[edx esi edi]

Font			proc	uses ebp

			mov	ebp, [PtrFont]
			xor	eax, eax

			test	ebp, ebp
			jz	EndFont

			jmp	NextCar

;			--------------------------------
Space:			add	ebx, [InterSpace]
NextCar:
			mov	al,[esi]
			inc	esi

			test	al, al
			je	short EndFont

			cmp	al, 32
			je	Space
PasSpace:
			mov	edi, [ebp+eax*4]
			push	esi

			mov	esi, [PtrFont]
			push	edi

			push	ecx
			push	ebx

			Call	AffMask

			pop	ebx
			pop	ecx

			pop	edi
			pop	esi

			xor	eax, eax
			mov	edx, [InterLeave]

			mov	al, [ebp+edi]
			add	ebx, edx

			add	ebx, eax
			jmp	short NextCar
;			--------------------------------
EndFont:
			ret

Font			endp

;----------------------------------------------------------------------------
;				The
				End
