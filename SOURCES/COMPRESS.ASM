;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ
;ณฺฤฤฤฤฤฤฤฤฤฤฤ   ฺฤฤฤฟ                                                     ณ
;ณณ              ณ   ณฺฤฤฟ                                                 ณ
;ณณ              ณ   ณณ  ณฺฤฟ                                              ณ
;ณณ              รฤฤฤูณ  ณณ    ณ                                           ณ
;ณณ              ณ    รฤฤดณ    ณ    ฤฤ                                     ณ
;ณณ              ณ    ณ  ณณ    ณ   ณ                                       ณ
;ณณ              ณ    ณ  ณณ    ภฤฤ  ฤฤ A :-)                               ณ
;ณณ ฺฤฤฤฟ ฺฤฤยฤฤฟ ฺฤฤฤฟ ฺฤฤฤฟ ฺฤฤฤ ฺฤฤฤ ฺฤฤฤ  ฤ  ฺฤฤฟ ฺฤฤฟ                 ณ
;ณณ ณ   ณ ณ  ณ  ณ ณ   ณ ณ   ณ ณ    ณ    ณ        ณ  ณ ณ  ณ                 ณ
;ณณ ณ   ณ ณ  ณ  ณ ณ   ณ ณ   ณ ณ    ณ    ณ     ณ  ณ  ณ ณ  ณ                 ณ
;ณณ ณ   ณ ณ     ณ รฤฤฤู รฤฤยู รฤ   ภฤฤฟ ภฤฤฟ  ณ  ณ  ณ ณ  ณ                 ณ
;ณณ ณ   ณ ณ     ณ ณ     ณ  ภณ ณ       ณ    ณ  ณ  ณ  ณ ณ  ณ                 ณ
;ณณ ณ   ณ ณ     ณ ณ     ณ   ณ ณ       ณ    ณ  ณ  ณ  ณ ณ  ณ                 ณ
;ณณ ภฤฤฤู ณ     ณ ณ     ณ   ณ ภฤฤฤ ฤฤฤู ฤฤฤู  ณ  ภฤฤู ณ  ณ                 ณ
;ณภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ                            ณ
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู






			.386


Set_Raster		MACRO	R,V,B
			push	eax
			push	edx

			mov	dx,3c8h
			xor	al,al
			out	dx,al
			inc	edx

			mov	al,R
			out	dx,al
			mov	al,V
			out	dx,al
			mov	al,B
			out	dx,al

			pop	edx
			pop	eax
			ENDM











;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ Pour le LZ77                                                            บ
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
PUBLIC			AddString
PUBLIC			DeleteString


Struc_Tree		STRUC
  Parent		dd	0
  Smaller_Child		dd	0
  Larger_Child		dd	0
Struc_Tree		ENDS



INDEX_BIT_COUNT      	=	12
LENGTH_BIT_COUNT     	=	4
WINDOW_SIZE          	=	(1 SHL INDEX_BIT_COUNT)
RAW_LOOK_AHEAD_SIZE  	=	(1 SHL LENGTH_BIT_COUNT)
BREAK_EVEN           	=	( ( 1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT ) / 9 )
LOOK_AHEAD_SIZE      	=	( RAW_LOOK_AHEAD_SIZE + BREAK_EVEN )
TREE_ROOT            	=	(WINDOW_SIZE) * (Size Struc_Tree)
UNUSED               	=	-1










			.MODEL	SMALL,SYSCALL


;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ                                                                        บ
;บ  Les datas                                                             บ
;บ                                                                        บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			.DATA

				; *** Pour le LZ77 ***
Extrn			tree		:	DWORD
Extrn			Current_position:	DWORD
Extrn			Match_position	:	DWORD
Extrn			window		:	BYTE


Match_Length		dd	0











;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ                                                                        บ
;บ  Le code                                                               บ
;บ                                                                        บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			.CODE



;ษอออออออออออออออัอออออออออออออออออออออออหออออออออออออออออออออออออออออออออป
;บ               ณ                       บ Conversion par LCA             บ
;บ  Codage LZ77  ณ                       บ                    le 31-07-95 บ
;วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤะฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
;บ                                                                        บ
;บ Pour les commentaires, se referer au source LZSS du Data Compression   บ
;บ                                                     Book :)            บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ


;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ Recherche si tout ou partie de la chaine en cours se trouve deja dans  บ
;บ le dico, constitue des chaines deja lues.                              บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN	4
AddString		PROC
			push	ebx
			push	esi
			push	edi
			push	ebp


			mov	ebp,Current_Position	; EBP = Current_Position
			mov	edi,[Tree+TREE_ROOT].Larger_Child
			mov	Match_Length,0

			ALIGN	4
@@Main_Loop:
			xor	ecx,ecx			; le compteur: i
			mov	esi,ebp			; ESI:Current_Position+i
			mov	ebx,edi			; EBX:Test_Node+i
			xor	edx,edx
@@Same_Char:
			xor	eax,eax

			and	esi,(WINDOW_SIZE-1)	; Le AND pour le modulo
			and	ebx,(WINDOW_SIZE-1)	; dans la fenetre dico

			mov	al,[Window+esi]		; EAX=Delta
			mov	dl,[Window+ebx]
			inc	esi
			sub	eax,edx
			jne	@@Found

			inc	ecx
			inc	ebx

			cmp	ecx,LOOK_AHEAD_SIZE
			jb	@@Same_Char

@@Found:
			cmp	ecx,Match_Length
			jb	@@Not_Better

			mov	Match_Length,ecx
			cmp	ecx,LOOK_AHEAD_SIZE
			mov	Match_Position,edi
			jae	@@Better

			ALIGN	2
@@Not_Better:
			lea	esi,[edi*4]		; ESI:PTR de Test_Node
			test	eax,eax
							; Multiplication par
			lea	esi,[esi+esi*2]		; 12 :) (thanx must fly to Serge!)

			jge	@@PTR_Ok		; Arg!! comment pairer?
			add	esi,(Smaller_Child-Larger_Child)
@@PTR_Ok:
			lea	esi,[Tree+esi].Larger_Child

			cmp	dword ptr [esi],UNUSED
			je	@@Add_Leaf

			mov	edi,[esi]
			jmp	@@Main_Loop

							; REPLACE_NODE
			ALIGN	4
@@Better:
			push	Offset @@Exit		; PUSH Imm
			jmp	Replace_Node		; on tombe sur un RET :)

			ALIGN	4
@@Add_Leaf:
			mov	[esi],ebp

			shl	ebp,2
			mov	eax,UNUSED		; Instr. gratuite sur 486 et paire sur P5
			lea	ebp,[ebp+ebp*2]

			mov	[Tree+ebp].Parent,edi
			mov	[Tree+ebp].Larger_Child,eax	; et la, on gagne sur le remplissage
			mov	[Tree+ebp].Smaller_Child,eax	; du prefetch

@@Exit:
			pop	ebp
			pop	edi
			pop	esi
			pop	ebx

			mov	eax,Match_Length

			ret
AddString		ENDP













;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ  Remplace un noeud par un nouveau. Utile par exemple, lorsqu'une chaineบ
;บ nouvelle est aussi dans le dico, il vaut mieux mettre la nouvelle, car บ
;บ elle sera supprimee apres l'ancienne... mais si c'est clair! :)        บ
;วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
;บ Appel : EBP = New_Node (son index)                                     บ
;บ         EDI = Old_Node (son index)                                     บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN   4
Replace_Node		PROC
						; EBP = New_Node : Index
						; EDI = Old_Node : Index
			mov	ebx,ebp
			shl	ebx,2
			lea	ebx,[ebx+ebx*2]	; EBX = New_Node : Deplacement


			mov	edx,edi
			shl	edx,2
			lea	edx,[edx+edx*2]	; EDX = Old_Node : Depl.


			mov	esi,[Tree+edx].Parent
						; ESI = Parent

			mov	ecx,esi
			shl	ecx,2
			lea	ecx,[ecx+ecx*2]	; ECX = Parent : depl.


			cmp	[Tree+ecx].Smaller_Child,edi
			jne	@@Not_That_One

			mov	[Tree+ecx].Smaller_Child,ebp
			jmp	@@Chain_Next
@@Not_That_One:
			mov	[Tree+ecx].Larger_Child,ebp
@@Chain_Next:

			mov	eax,[Tree+edx].Parent
			mov	[Tree+ebx].Parent,eax

			mov	eax,[Tree+edx].Larger_Child
			mov	[Tree+ebx].Larger_Child,eax

			mov	eax,[Tree+edx].Smaller_Child
			mov	[Tree+ebx].Smaller_Child,eax

			mov	[Tree+edx].Parent,UNUSED



			cmp	[Tree+ebx].Smaller_Child,UNUSED
			je	@@Et_Non0
			mov	eax,[Tree+ebx].Smaller_Child
			shl	eax,2
			lea	eax,[eax+eax*2]
			mov	[Tree+eax].Parent,ebp
@@Et_Non0:

			cmp	[Tree+ebx].Larger_Child,UNUSED
			je	@@Exit
			mov	eax,[Tree+ebx].Larger_Child
			shl	eax,2
			lea	eax,[eax+eax*2]
			mov	[Tree+eax].Parent,ebp

@@Exit:
			ret
Replace_Node		ENDP








;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ  Supprime une chaine dans le dico (pour etre exact, dans l'arbre).     บ
;บ Il faut tout ca, car il faut reordonner l'arbre correctement           บ
;บ                                                                        บ
;วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
;บ Appel : Passage de parametre par pile: P = Indice du noeud a supprimer บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN   4
DeleteString		PROC	P:DWORD
			push	edi
			push	esi

			mov	edi,[P]
			mov	esi,[P]
			shl	edi,2
			lea	edi,[edi+edi*2]

			cmp	[Tree+edi].Parent,UNUSED
			je	@@End

			cmp	[Tree+edi].Larger_Child,UNUSED
			jne	@@Not_This_One

			mov	eax,[Tree+edi].Smaller_Child
			push	Offset @@End
			jmp	Contract_Node

			ALIGN	4
@@Not_This_One:
			cmp	[Tree+edi].Smaller_Child,UNUSED
			jne	@@Not_This_One_Too

			mov	eax,[Tree+edi].Larger_Child
			push	Offset @@End
			jmp	Contract_Node

			ALIGN	4
@@Not_This_One_Too:
			call	Find_Next_Node

			push	eax
			push	eax
			call	DeleteString
			add	esp,4
			pop	eax

			push	ebx
			push	ebp

			mov	edi,esi
			mov	ebp,eax
			call	Replace_Node

			pop	ebp
			pop	ebx


@@End:
			pop	esi
			pop	edi
			ret
DeleteString		ENDP








;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ                                                                        บ
;วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
;บ Appel : ESI = Node                                                     บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN   4
Find_Next_Node		PROC

			lea	ecx,[esi*4]
			lea	ecx,[ecx+ecx*2]

			mov	eax,[tree+ecx].Smaller_Child
			ALIGN	4
@@Loop:
			lea	ecx,[eax*4]
			lea	ecx,[ecx+ecx*2]

			cmp	[Tree+ecx].Larger_Child,UNUSED
			je	@@End

			mov	eax,[Tree+ecx].Larger_Child
			jmp	@@Loop

@@End:
			ret
Find_Next_Node		ENDP





;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ                                                                        บ
;วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
;บ Appel : EAX = New Node                                                 บ
;บ         ESI = Old Node                                                 บ
;บ                                                                        บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
			ALIGN   4
Contract_Node		PROC
			push	ebx

			lea	ebx,[esi*4]
			lea	ebx,[ebx+ebx*2]

			cmp	eax, UNUSED
			je	@@skip1

			lea	ecx,[eax*4]
			lea	ecx,[ecx+ecx*2]

			mov	edx,[Tree+ebx].Parent
			mov	[Tree+ebx].Parent,UNUSED
			mov	[Tree+ecx].Parent,edx
			jmp	@@skip2
@@skip1:
			mov	edx,[Tree+ebx].Parent
			mov	[Tree+ebx].Parent,UNUSED
@@skip2:
			shl	edx,2
			lea	edx,[edx+edx*2]


			cmp	[Tree+edx].Larger_Child,esi
			jne	@@Et_Non
			mov	[Tree+edx].Larger_Child,eax
			jmp	@@Fin
@@Et_Non:
			mov	[Tree+edx].Smaller_Child,eax

@@Fin:
			pop	ebx
			ret
Contract_Node		ENDP







;			The
			End

