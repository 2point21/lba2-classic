		.DATA

Status          EQU     08h     ;DMAC status port (read)     \  same port
Command         EQU     08h     ;DMAC command port (write)   /  (read/write)
Request         EQU     09h     ;DMAC channel request (write-only)
DMA_Mask        EQU     0Ah     ;DMAC DMA_Mask (write-only)
Mode            EQU     0Bh     ;DMAC mode (write)
byte_ptr        EQU     0Ch     ;byte pointer flip-flop

addr            EQU	000h	; per-channel base address
count           EQU	001h	; per-channel byte count

read_cmd        EQU     058h    ; autoinitialising read
write_cmd       EQU     054h    ; auto write

set_cmd         EQU     000h    ; DMA_Mask set (enable dma)
reset_cmd       EQU     004h    ; DMA_Mask reset (disable)

page_table	DW	00087h  ; channel 0
		DW  	00083h  ; channel 1
		DW  	00081h  ; channel 2
		DW  	00082h  ; channel 3
		DW  	0FFFFh  ; ch 4 (not used)
		DW  	0008Bh  ; ch 5
		DW  	00089h  ; ch 6
		DW  	0008Ah  ; ch 7

dmac2        	DB	0    	; Flag set to non-zero when using the 2nd DMA controller

		.CODE

MACRO 		adjust reg	; Adjust register port for 2nd DMA cont
		local no_adjust

		cmp	byte ptr[dmac2], 0
		jz	no_adjust
		shl	reg, 1
		add	reg, 0C0h
no_adjust:

ENDM adjust

;+---------------------------------------------------------------------------+
;| int dma_setup(int Channel, char far *Buffer, unsigned Length)             |
;| ------------------------------------------------------------------------- |
;| Channel = 0-7                                                             |
;| Buffer  = Address of data to transfer                                     |
;| Length  = Length of data to transfer                                      |
;| ------------------------------------------------------------------------- |
;| Returns: 0 if no errors (dma_errno == 0)                                  |
;|         -1 if errors occurred (dma_errno set to indicate error.)          |
;+---------------------------------------------------------------------------+

dma_setup	PROC USES EBX,\
		Channel:DWORD, Buffer:DWORD, Len:DWORD

;Convert 20-bit physical address of buffer into page:offset


		mov	edi, Buffer
		mov	ecx, edi
		shr	ecx, 8

; ch:di == The physical buffer base.

; Adjust channel number

		mov	byte ptr[dmac2], 0
		mov	bx, Channel
		cmp	bx, 4
		jb	OkChannel
		and	bx, 3
		mov	byte ptr[dmac2], 1
OkChannel:

; BX contains the adjusted channel number

		mov	si, read_cmd
		add	si, bx
		mov	cl, set_cmd		;allow dma requests
		add	cl, bl

;si contains READ/WRITE command for DMA controller
;cl contains confirmation command for DMA controller

		shl	bx, 1

;bx == Port # Channel*2

;-------------------------------------------------------------------------
; Calculations have been done ahead of time to minimize time with
; interrupts disabled.
;
; edi (ch:di) == physical base address (must be on word boundary for 16 bits)
;
; cl == Confirmation command    (Unmasks the channel)
;
; bx == I/O port Channel*2      (This is where the address is written)
;
; si == Mode command for DMA
;-------------------------------------------------------------------------

; Now we shift the address and word count right one bit if in 16 bit mode.

		cmp	[dmac2], 0
		jz	AddrOk

		shr	ch, 1
		rcr	di, 1
		shl	ch, 1
		shr	[Len], 1		;Odd byte lengths are rounded down
AddrOk:

;The "byte pointer" is also known as the LSB/MSB flip flop.
;By writing any value to it, the DMA controller registers are prepared
;to accept the address and length values LSB first.

		mov	dx, byte_pt		;Reset byte pointer Flip/flop
		adjust	dx
		out	dx, al

		mov	dx, bx              	;dx=DMAC Base Address port
		adjust	dx
		mov	ax, di              	;ax=LSW of 20-bit address
		out	dx, al              	;Store LSB
		mov	al, ah
		out	dx, al              	;Store MSB

;Write length to port (Channel * 2 + 1)

		inc	dx 	                ;dx=DMAC Count port
		mov	eax, Len
		out  	dx, al			;Write LSB of Length
		mov  	al, ah
		out  	dx, al			;Write MSB

;Write page to port page_table[channel]

		mov  	ebx, Channel
		shl  	bx, 1
		mov  	dx, word ptr[OFFSET page_table + bx]
		mov  	al, ch              	;al=Page number
		out  	dx, al              	;Store the page

;Write Readcmd for channel to Mode port

		mov  	dx, Mode            	;dx=DMAC mode register
		adjust 	dx
		mov  	ax, si              	;Load pre-calculated mode
		out  	dx, al              	;Write it to the DSP

		mov  	dx, DMA_Mask	        ;dx=DMAX DMA_Mask register
		adjust 	dx
		mov  	al, cl              	;al=pre-calulated DMA_Mask value
		out  	dx, al              	;Write DMA_Mask (allow dma on this channel)

		ret

dma_setup	ENDP

;+---------------------------------------------------------------------------+
;| int prevent_dma(int Channel)                                              |
;| ------------------------------------------------------------------------- |
;| Channel = 0-7                                                             |
;|         Prevents DMA requests from Channel by masking bit in DMA_C.       |
;| ------------------------------------------------------------------------- |
;+---------------------------------------------------------------------------+

prevent_dma	PROC ,\
		Channel:DWORD


; Check channel number range

		mov	dx, DMA_Mask

		mov	eax, Channel
		cmp	al, 4
		jb	OkChannel
		and	al, 3
		shl	dx, 1
		add	dx, 0C0h
OkChannel:
		add	al, reset_cmd      	; Add disable DMA requests
		out	dx, al

		ret

prevent_dma	ENDP

;+---------------------------------------------------------------------------+
;| int allow_dma(int Channel)                                                |
;| ------------------------------------------------------------------------- |
;| Channel = 0-7                                                             |
;|         Unmasks DMA on the specified channel.                             |
;| ------------------------------------------------------------------------- |
;+---------------------------------------------------------------------------+

allow_dma	PROC ,\
		Channel:DWORD

; Check channel number range

		mov	dx, DMA_Mask

		mov	eax, Channel
		cmp	al, 4
		jb	OkChannel
		and	al, 3
		shl	dx, 1
		add	dx, 0C0h
OkChannel:
		add	al, set_cmd		; Add enable DMA requests
		out	dx, al

		ret

allow_dma	ENDP

;+---------------------------------------------------------------------------+
;| int dma_count(Channel)                                                    |
;| ------------------------------------------------------------------------- |
;| Channel = 0-7                                                             |
;| ------------------------------------------------------------------------- |
;| Returns: -1 if DMA transaction completed                                  |
;|          else returns the number of bytes/words left to transfer          |
;+---------------------------------------------------------------------------+

PROC		dma_count ,\
		Channel:DWORD

		mov  	eax, Channel

		mov  	dx, ax
		and  	dx, 3
		shl  	dx, 1
		add  	dx, count

		cmp  	al, 4
		jb   	OkChannel
		shl  	dx, 1
		add  	dx, 0C0h
OkChannel:
		xor	eax, eax
		cli
		in   	al, dx
		mov  	ah, al
		in   	al, dx
		sti
		xchg 	al, ah

		ret

dma_count	ENDP

;+---------------------------------------------------------------------------+
;| unsigned dma_addr(Channel)                                                |
;| ------------------------------------------------------------------------- |
;| Channel = 0-7                                                             |
;| ------------------------------------------------------------------------- |
;| Returns: Current address word of that channel                             |
;| Value must be multiplied by 2 for a 16-bit channel.                       |
;| It is best to start at offset 0, ie on a 64K boundary                     |
;+---------------------------------------------------------------------------+

dma_addr	PROC ,\
		Channel:DWORD

		mov	eax, Channel

		mov  	dx, ax
		and	dx, 3
		shl  	dx, 1

		cmp	al, 4
		jb	OkChannel
		shl	dx, 1
		add	dx, 0C0h
OkChannel:
		xor	eax, eax
		cli
		in   	al, dx
		mov  	ah, al
		in   	al, dx
		sti
		xchg 	al, ah

		ret

dma_addr	ENDP



VOID	*DMABuffer, *DMAAllocBuffer;
BYTE	LockedDMARegion;

#define	MAX_DMABUFFERATTEMPTS	10;

;+---------------------------------------------------------------------------+
;| BYTE *AlignOn4KBoundry( BYTE *Buf )                                       |
;| ------------------------------------------------------------------------- |
;| Align Buf to the nearest 4K page					     |
;+---------------------------------------------------------------------------+

#define	AlignOn4KBoundry(Buf)	((Buf + 0x00000FFF) & 0xFFFFF000)

;+---------------------------------------------------------------------------+
;| BYTE *AllocateDMABuffer( WORD BufferSize )                                |
;| ------------------------------------------------------------------------- |
;| BufferSize = 0-64K                                                        |
;| ------------------------------------------------------------------------- |
;| Returns: Buffer if successful                                             |
;| 	    NULL if error						     |
;+---------------------------------------------------------------------------+

BYTE *AllocateDMABuffer( WORD BufferSize )
{
   VOID	  *Tried[ MAX_DMABUFFERATTEMPTS ] ;
   VOID   *Buf, *DMAAddr ;
   int    Tries, Error ;

   for (Tries = 0; Tries < MAX_DMABUFFERATTEMPTS; Tries++)
      Tried[ Tries ] = NULL ;

   Tries             = 0;
   LockedDMARegion   = FALSE;

   do
   {
//      printf( "DMA buffer allocation attempt: %d\n", Tries + 1 ) ;
      if ((DMAAllocBuffer = (VOID *)DosMalloc( BufferSize + 0xFFF)) != NULL)
      {
	 DMABuffer = AlignOn4KBoundry(DMAAllocBuffer);

	 if ( ((LONG)DMABuffer & 0xF0000) !=
	      (((LONG)DMABuffer + BufferSize) & 0xF0000) )
	 {
	    Tried[ Tries ] = DMAAllocBuffer ;
	    DMAAllocBuffer = NULL ;
	    DMABuffer	   = NULL ;

	 }
      }
      Tries++ ;
      DMAAllocBuf = Buf;
      DMABuffer   = DMAAddr ;
   }
   while ((DMAAllocBuf == NULL) && (Tries < MAX_DMABUFFERATTEMPTS)) ;

   for (wTries = 0; wTries < MAX_DMABUFFERATTEMPTS; wTries++)
   {
      if (alpTried[ wTries ])
	 free( alpTried[ wTries ] ) ;
      else
	 break ;
   }

   return( DMABuffer ) ;


;+---------------------------------------------------------------------------+
;| VOID FreeDMABuffer( VOID )                                                |
;| ------------------------------------------------------------------------- |
;| Free the DMA buffer							     |
;+---------------------------------------------------------------------------+

VOID FreeDMABuffer( VOID )
{
   if (LockedDMARegion)
      UnlockDMARegion( &gDDS, 0 ) ;

   if (DMAAllocBuf)
   {
      free( glpDMAAllocBuf ) ;
      DMAAllocBuf = NULL ;
   }

}
