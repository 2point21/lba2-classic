;-----------------------------------------------------------------------------
			.386
			.MODEL	SMALL, SYSCALL

;-----------------------------------------------------------------------------
Struc_Plasma		STRUC

  Plasma_PTR_TabVirgule	dd	0
  Plasma_PTR_TabSpeed	dd	0
  Plasma_PTR_TabAcc	dd	0
  Plasma_PTR_TabCouls	dd	0
  Plasma_PTR_TexOff	dd	0

  Plasma_Interleave	db	0
  Plasma_Nb_PtsActifs	db	0
  Plasma_Nb_Couleurs	db	0
  Plasma_Speed		db	0

Struc_Plasma		ENDS

Struc_RealPlasma	STRUC
			db	Size Struc_Plasma dup (0)
  Plasma_TabControl	db	0
Struc_RealPlasma	ENDS

;-----------------------------------------------------------------------------
			.DATA

;-----------------------------------------------------------------------------
Nb_Pts_Inter		dd	0
Nb_Pts_Control		dd	0

;-----------------------------------------------------------------------------
			.CODE

;-----------------------------------------------------------------------------
			PUBLIC	SYSCALL		Do_Plasma

;-----------------------------------------------------------------------------
Do_Plasma		PROC

			push	ebp
			mov	edi, dword ptr [esi].Plasma_PTR_TexOff

			; *** Calcul des coeff de Gouraud ***
			push	esi
			push	edi

			mov	ecx,dword ptr [esi].Plasma_Interleave
			mov	edx,1
			shl	edx,cl
			mov	[Nb_Pts_Inter],edx
			mov	ebp, edx
			dec	ebp
			mov	byte ptr [Nb_Pts_Control],ch

			mov	dl,ch
			mov	esi,dword ptr [esi].Plasma_PTR_TabVirgule
@@Lines:
			push	edx			; Chaque ligne
			push	ecx

			mov	edi,esi			; Memo le ptr de controle
			dec	ch			; si 4 pts de controle, alors 3 iterations plus le cas particulier
							; du dernier point, qui doit boucler avec le premier
@@Col:
			mov	edx,ebp
			xor	ebx,ebx

			mov	bx,[esi]
			xor	eax,eax

			mov	ax,[esi+ebp*2+2]
			add	esi,2			; Le premier point existe deja

			sub	eax,ebx

			sar	eax,cl

			add	eax,eax

			adc	eax,1

			sar	eax,1
@@Fill:
			add	ebx,eax
			dec	edx

			mov	[esi],bx
			lea	esi,[esi+2]

			jne	@@Fill

			dec	ch
			jne	@@Col

							; Traite le dernier point
			mov	edx,ebp
			xor	ebx,ebx

			mov	bx,[esi]
			xor	eax,eax

			mov	ax,[edi]
			add	esi,2			; Le premier point existe deja

			sub	eax,ebx

			sar	eax,cl

			add	eax,eax

			adc	eax,1

			sar	eax,1
@@Fill_Last:
			add	ebx,eax
			dec	edx

			mov	[esi],bx
			lea	esi,[esi+2]

			jne	@@Fill_Last


			pop	ecx
			pop	edx
			dec	dl
			jne	@@Lines

			pop	edi
			pop	esi


			; *** Copy ***
			mov	ebx,[esi].Plasma_PTR_TabCouls
			mov	esi,dword ptr [esi].Plasma_PTR_TabVirgule
			xor	eax,eax
			mov	dl,ch

			movzx	ebp,ch
			imul	ebp,[Nb_Pts_Inter]	; EBP = Offset de la ligne suivante
@@Copy_0:
			push	edx
			push	ecx
			push	edi
			push	ebp

			cmp	dl,1
			mov	edx,ebp
			jne	@@Copy_1

			mov	eax,ebp
			imul	ebp,[Nb_Pts_Control]
			sub	ebp,eax
			neg	ebp
@@Copy_1:
			xor	eax, eax
			push	ebp
			push	ecx
			push	edi

			mov	ax,[esi]
			mov	dh,byte ptr [Nb_Pts_Inter]

			movzx	ebp,word ptr[esi+ebp*2]

			sub	ebp,eax

			sar	ebp,cl

			add	ebp,ebp

			adc	ebp,1
			xor	ecx,ecx

			sar	ebp,1
@@Copy_2:
			mov	cl,ah
			add	eax,ebp

			mov	cl,[ebx+ecx]

			mov	[edi],cl
			inc	edi

			dec	dh
			jne	@@Copy_2

			pop	edi
			pop	ecx
			pop	ebp

			add	edi,256
			add	esi,2
			dec	dl
			jne	@@Copy_1

			pop	ebp
			pop	edi
			pop	ecx
			pop	edx

			add	edi,[Nb_Pts_Inter]
			dec	dl
			jne	@@Copy_0

			pop	ebp

			ret

Do_Plasma		ENDP

;-----------------------------------------------------------------------------
;			The
			End

;-----------------------------------------------------------------------------
