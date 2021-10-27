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
			PUBLIC	SYSCALL	CopyBlockIncrust

;----------------------------------------------------------------------------
;void	CopyBlockIncrust(S32 x0, S32 y0, S32 x1, S32 y1, void *src, S32 xd, S32 yd, void *dst)

;#pragma aux CopyBlockIncrust	"*"				\
;	parm caller	[eax] [ebx] [ecx] [edx] [esi]	\
;	modify		[edi]

CopyBlockIncrust	proc uses ebp,\
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
			xor	ecx, -1

			add	edi, TabOffLine[eax*4]
			mov	ebp, TabOffLine[1*4]

			mov	ebx, ecx
			inc	edx                     ; Delta Y

			sub	esi, ebx
			sub	edi, ebx
nextline:
			mov	ecx, ebx
nextbyte:
			mov	al, [esi+ecx]

			test	al, al
			jz	skip

			mov	[edi+ecx], al
skip:			inc	ecx

			jnz	nextbyte

			add	esi, ebp
			add	edi, ebp

			dec	edx
			jnz	nextline
endcopy:
			ret

CopyBlockIncrust	endp

;----------------------------------------------------------------------------
;			The
       			End
