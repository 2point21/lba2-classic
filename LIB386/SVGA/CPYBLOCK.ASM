;----------------------------------------------------------------------------
			.386P
			.MODEL FLAT, SYSCALL

;----------------------------------------------------------------------------
			.DATA

;----------------------------------------------------------------------------
			EXTRN	SYSCALL	TabOffLine:DWORD
			EXTRN	SYSCALL	ClipXMin:DWORD
			EXTRN	SYSCALL	ClipXMax:DWORD
			EXTRN	SYSCALL	ClipYMin:DWORD
			EXTRN	SYSCALL	ClipYMax:DWORD




;----------------------------------------------------------------------------
			.CODE

;----------------------------------------------------------------------------
			PUBLIC	SYSCALL	CopyBlock

;----------------------------------------------------------------------------
;void	CopyBlock(S32 x0, S32 y0, S32 x1, S32 y1, void *src, S32 xd, S32 yd, void *dst)

;#pragma aux CopyBlock	"*"				\
;	parm caller	[eax] [ebx] [ecx] [edx] [esi]	\
;	modify		[edi]

CopyBlock		proc	,\
			xd:DWORD, yd:DWORD, dst:DWORD

			cmp	eax,[ClipXMax]
			jg	endcopy
			cmp	ebx,[ClipYMax]
			jg	endcopy

			mov	edi,[ClipXMin]

			sub	edi,eax
			jle	@@NoClipXMin

			mov	eax,[ClipXMin]
			add	[xd],edi
@@NoClipXMin:
			mov	edi,[ClipYMin]
			sub	edi,ebx
			jle	@@NoClipYMin

			mov	ebx,[ClipYMin]
			add	[yd],edi
@@NoClipYMin:
			mov	edi,[ClipXMax]
			cmp	ecx,edi
			jle	@@NoClipXMax
			mov	ecx,edi
@@NoClipXMax:
			mov	edi,[ClipYMax]
			cmp	edx,edi
			jle	@@NoClipYMax
			mov	edx,edi
@@NoClipYMax:
			sub	ecx, eax
			js	endcopy

			sub	edx, ebx
			js	endcopy

			mov	edi,[xd]
			cmp	edi,[ClipXMin]
			jl	endcopy
			cmp	edi,[ClipXMax]
			jg	endcopy

			mov	edi,[yd]
			cmp	edi,[ClipYMin]
			jl	endcopy
			cmp	edi,[ClipYMax]
			jg	endcopy


			mov	edi,[xd]
			add	edi,ecx
			sub	edi,[ClipXMax]
			jle	@@NoClipXMaxD
			sub	ecx,edi
			jle	endcopy
@@NoClipXMaxD:

			mov	edi,[yd]
			add	edi,edx
			sub	edi,[ClipYMax]
			jle	@@NoClipYMaxD
			sub	edx,edi
			jle	endcopy
@@NoClipYMaxD:



			add	esi, eax		; Offset Src
			mov	edi, dst

			mov	eax, yd
			add	edi, xd			; Offset Dst

			add	esi, TabOffLine[ebx*4]
			lea	ebx, [ecx+1]		; Delta X

			add	edi, TabOffLine[eax*4]
			mov	eax, TabOffLine[1*4]

			inc	edx                     ; Delta Y
			sub	eax, ebx

			cmp	ebx, 3+4+3
			jae	copydword
nextbyte:
			mov	ecx, ebx
			rep	movsb

			add	esi, eax
			add	edi, eax

			dec	edx
			jnz	nextbyte
endcopy:
			ret
copydword:
			mov	ecx, ebx
			mov	ebx, 4

			sub	ebx, edi

			and	ebx, 3

			sub	ecx, ebx

			mov	bh, cl
			push	ebp

			shr	ecx, 2
			and	bh, 3

			mov	ebp, ecx

			test	bl, bl
			jz	checkend

			test	bh, bh
			jz	juststart
startend:
			xor	ecx, ecx
			mov	cl, bl
			rep	movsb
			mov	ecx, ebp
			rep	movsd
			xor	ecx, ecx
			mov	cl, bh
			rep	movsb

			add	esi, eax
			add	edi, eax

			dec	edx
			jnz	startend

			pop	ebp
			ret
juststart:
			xor	ecx, ecx
			mov	cl, bl
			rep	movsb
			mov	ecx, ebp
			rep	movsd

			add	esi, eax
			add	edi, eax

			dec	edx
			jnz	juststart

			pop	ebp
			ret
checkend:
			test	bh, bh
			jz	noend
justend:
			mov	ecx, ebp
			rep	movsd
			xor	ecx, ecx
			mov	cl, bh
			rep	movsb

			add	esi, eax
			add	edi, eax

			dec	edx
			jnz	justend

			pop	ebp
			ret
noend:
			mov	ecx, ebp
			rep	movsd

			add	esi, eax
			add	edi, eax

			dec	edx
			jnz	noend

			pop	ebp
			ret

CopyBlock		endp

;----------------------------------------------------------------------------
;			The
       			End
