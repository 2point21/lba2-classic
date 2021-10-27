;-----------------------------------------------------------------------------
				.586
				.MODEL FLAT, SYSCALL


;-----------------------------------------------------------------------------
				.DATA

;-----------------------------------------------------------------------------
				PUBLIC	SYSCALL	ProcessorSignature
				PUBLIC	SYSCALL	ProcessorFeatureFlags
				PUBLIC	SYSCALL	ProcessorManufacturerIDString
				PUBLIC	SYSCALL	ProcessorManufacturerIDString
				PUBLIC  SYSCALL Processor4KBDataTLB
				PUBLIC  SYSCALL Processor4KBInstructionTLB
				PUBLIC  SYSCALL Processor4MBDataTLB
				PUBLIC  SYSCALL Processor4MBInstructionTLB
				PUBLIC  SYSCALL ProcessorL1DataCache
				PUBLIC  SYSCALL ProcessorL1InstructionCache
				PUBLIC  SYSCALL ProcessorL2Cache

;-----------------------------------------------------------------------------
Processor4KBDataTLB		dd	0
Processor4KBInstructionTLB	dd	0

;-----------------------------------------------------------------------------
Processor4MBDataTLB		dd	0
Processor4MBInstructionTLB	dd	0

;-----------------------------------------------------------------------------
ProcessorL1DataCache		dd	0
ProcessorL1InstructionCache	dd	0

;-----------------------------------------------------------------------------
ProcessorL2Cache		dd	0

;-----------------------------------------------------------------------------
ProcessorSignature		dd	0

;		  3...2 2 2 2 1  1 1 1 11 1100 0000 0000
;		  1...3 2 1 0 9  6 5 4 32 1098 7654 3210  Bit position
;		 +-----+-+-+-+----+-+-+--+----+----+----+
;		 |     | | | |    | | |  |    |    |    |
;		 +-----+-+-+-+----+-+-+--+----+----+----+
;		        ^ ^ ^ ^^^^ ^ ^ ^^ ^^^^ ^^^^ ^^^^
;                       | | | |||| | | || |||| |||| ||||
; Cache disabled--------+ | | |||| | | || |||| |||| ||||
; Cache Alloc on Write----+ | |||| | | || |||| |||| ||||
; Cache Write Back----------+ |||| | | || |||| |||| ||||
; Manufacturer (Adeline)------++++ | | || |||| |||| ||||
; CPUID supported (Adeline)--------+ | || |||| |||| ||||
; FPU Present (Adeline)--------------+ || |||| |||| ||||
; Processor Type ----------------------++ |||| |||| ||||
; Family ---------------------------------++++ |||| ||||
; Model ---------------------------------------++++ ||||
; Stepping -----------------------------------------++++
;
; Manufacturer :	  0000 = Unknown
;			  0001 = Intel
;			  0010 = Cyrix
;			  0011 = AMD
;
; FPU Present : 	     1 = Floating point Unit present
;
; Processor Type :	    00 = Original OEM processor
;			    01 = Overdrive processor
;			    10 = Dual processor
;			    11 = Not used (by Intel)
;
; family :		  0011 = 386
;			  0100 = 486
;			  0101 = Pentium
;			  0110 = Pentium Pro
;
; Model (family = 0110):  0001 =
;
; Model (family = 0101):  0001 = Pentium (60/66 Mhz)
;			  0010 = Pentium (others)
;			  0011 = Pentium overdrive processors
;			  0100 = Pentium P55C
;			  0101 = Pentium overdrive for IntelDX4
;			  1111 = Unknown
;
; Model (family = 0100):  0000 = Intel486 DX
;			  0001 = Intel486 DX
;			  0010 = Intel486 SX
;			  0011 = Intel487 (SX), or Intel486 DX
;			  0100 = Intel486 SL
;			  0101 = IntelSX2
;			  0111 = IntelDX2 write-back enhanced
;			  1000 = IntelDX4 and IntelDX4 overdrive
;			  1001 = IntelDX4 and IntelDX4 overdrive
;			  1111 = Unknown
;
; Model (family = 0011):  0000 = Intel386 DX
;			  0010 = Intel386 SX, Intel386 CX, Intel386 EX
;			  0100 = Intel386 SL
;			  1111 = Unknown

;-----------------------------------------------------------------------------
ProcessorFeatureFlags		dd	0

;                                  3.......222211111111110000000000
;                                  1.......321098765432109876543210  Bit position
;                                 +--------------------------------+
;                                 |                                |
;                                 +--------------------------------+
;                                  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
; Reserved-------------------------++++++++||||||||||||||||||||||||
; Multimedia Extensions (MMX)--------------+|||||||||||||||||||||||
; Reserved----------------------------------+++++++||||||||||||||||
; Conditional Move Instruction---------------------+|||||||||||||||
; Machine Check Architecture------------------------+||||||||||||||
; Global Paging Extension----------------------------+|||||||||||||
; Memory Type Range Registers-------------------------+||||||||||||
; Reserved---------------------------------------------+|||||||||||
; Reserved----------------------------------------------+||||||||||
; APIC---------------------------------------------------+|||||||||
; CMPXCHG8B Instruction-----------------------------------+||||||||
; Machine Check Exception----------------------------------+|||||||
; Physical Address Extensions (2MB Pages/36bit adresses)----+||||||
; Model Specific registers-----------------------------------+|||||
; Time Stanp Counter------------------------------------------+||||
; Page size extensions (4MB pages)-----------------------------+|||
; Debugging Extensions (I/O Breakponits)------------------------+||
; Virtual Mode Extensions----------------------------------------+|
; Floating Point Unit on Chip-------------------------------------+

;-----------------------------------------------------------------------------
ProcessorManufacturerIDString	db	12 dup (0),0	; Manufacturer Identification
							; String

;-----------------------------------------------------------------------------
ProcessorRevision		db	0

;-----------------------------------------------------------------------------
IFDEF	_WIN32

_SYSTEM_INFO	STRUCT
			wProcessorArchitecture		WORD	?
			wReserved			WORD	?
			dwPageSize			DWORD	?
			lpMinimumApplicationAddress	DWORD	?
			lpMaximumApplicationAddress	DWORD	?
			dwActiveProcessorMask		DWORD	?
			dwNumberOfProcessors		DWORD	?
			dwProcessorType			DWORD	?
			dwAllocationGranularity		DWORD	?
			dwProcessorLevel		DWORD	?
			dwProcessorRevision		DWORD	?
_SYSTEM_INFO	ENDS

SYSTEM_INFO		TYPEDEF		_SYSTEM_INFO

KeyString			db	"HARDWARE\DESCRIPTION\System\FloatingPointProcessor", 0

ENDIF

;-----------------------------------------------------------------------------
			EXTERN	SYSCALL	OSType:DWORD


;-----------------------------------------------------------------------------
			.CODE

;-----------------------------------------------------------------------------
IFDEF	_WIN32
			EXTERN	SYSCALL	_GetSystemInfo:PROC
			EXTERN	SYSCALL	_RegOpenKeyExA:PROC
			EXTERN	SYSCALL	_RegCloseKey:PROC
ENDIF

;-----------------------------------------------------------------------------
			PUBLIC	SYSCALL	ProcessorIdentification

;-----------------------------------------------------------------------------
PentiumProFillCacheInfo	PROC PRIVATE

			cmp	al, 01h
			je	found1
			cmp	al, 02h
			je	found2
			cmp	al, 03h
			je	found3
			cmp	al, 04h
			je	found4
			cmp	al, 06h
			je	found6
			cmp	al, 08h
			je	found8
			cmp	al, 0Ah
			je	foundA
			cmp	al, 0Ch
			je	foundC
			cmp	al, 41h
			je	found41
			cmp	al, 42h
			je	found42
			cmp	al, 43h
			je	found43
			ret
found1:
			mov	Processor4KBInstructionTLB, 00040040h
			ret
found2:
			mov	Processor4MBInstructionTLB, 00040004h
			ret
found3:
			mov	Processor4KBDataTLB, 00040040h
			ret
found4:
			mov	Processor4MBDataTLB, 00040008h
			ret
found6:
			mov	ProcessorL1InstructionCache, 00084120h
			ret
found8:
			mov	ProcessorL1InstructionCache, 00104120h
			ret
foundA:
			mov	ProcessorL1DataCache, 00082120h
			ret
foundC:
			mov	ProcessorL1DataCache, 00102120h
			ret
found41:
			mov	ProcessorL2Cache, 00804120h
			ret
found42:
			mov	ProcessorL2Cache, 01004120h
			ret
found43:
			mov	ProcessorL2Cache, 02004120h
			ret

PentiumProFillCacheInfo	ENDP

;-----------------------------------------------------------------------------
PentiumMMXFillCacheInfo	PROC PRIVATE

			mov	Processor4KBDataTLB, 00040040h
			mov	Processor4MBDataTLB, 00040008h
			mov	Processor4KBInstructionTLB, 00040020h
			mov	ProcessorL1DataCache, 00104120h
			mov	ProcessorL1InstructionCache, 00104120h
			ret

PentiumMMXFillCacheInfo	ENDP

;-----------------------------------------------------------------------------
PentiumFillCacheInfo	PROC PRIVATE

			mov	Processor4KBDataTLB, 00040040h
			mov	Processor4MBDataTLB, 00040008h
			mov	Processor4KBInstructionTLB, 00040020h
			mov	ProcessorL1DataCache, 00082120h
			mov	ProcessorL1InstructionCache, 00082120h
			ret

PentiumFillCacheInfo	ENDP

;-----------------------------------------------------------------------------
CyrixGetCCR		PROC

			mov	bh, al
			mov	al, 0c3h
			out	22h, al		; CCR3
			in	al, 23h
			mov	bl, al		; save CCR3
			mov	ah, al
			and	ah, 0Fh
			or	ah, 1 shl 4	; MAPEN = 1
			mov	al, 0c3h
			out	22h, al
			mov	al, ah
			out	23h, al		; Access ok to all reg

			mov	al, bh
			out	22h, al		; CCRx
			in	al, 23h

			mov	ah, al

			mov	al, 0c3h
			out	22h, al		; CCR3
			mov	al, bl
			out	23h, al		; restore CCR3

			mov	al, ah

			ret

CyrixGetCCR		ENDP

;-----------------------------------------------------------------------------
Cyrix686FillCacheInfo	PROC PRIVATE


			cmp	[OSType], 3
			jae	no_wt_alloc	; can't check WT_ALLOC under NT

			mov	al, 0E9h
			call	CyrixGetCCR

			test	al, 1
			jz	no_wt_alloc

			or	[ProcessorSignature], 1 SHL 21
no_wt_alloc:
			mov	Processor4KBDataTLB, 00010080h
			mov	ProcessorL1DataCache, 00104120h
			ret

Cyrix686FillCacheInfo	ENDP

;-----------------------------------------------------------------------------
IntelDX4FillCacheInfo	PROC PRIVATE

			mov	ProcessorL1DataCache, 00104110h
			ret

IntelDX4FillCacheInfo	ENDP

;-----------------------------------------------------------------------------
Unknown486FillCacheInfo	PROC PRIVATE

			mov	ProcessorL1DataCache, 00084110h
			ret

Unknown486FillCacheInfo	ENDP

;-----------------------------------------------------------------------------
ProcessorIdentification	PROC
			LOCAL	fp_status:DWORD


IFDEF	_WIN32
			LOCAL	Key	:DWORD
			LOCAL	SysInfo	:SYSTEM_INFO


			; Get Processor type

			lea	eax, [SysInfo]
			push	eax
			call	_GetSystemInfo
			mov	eax, SysInfo.dwProcessorType
			xor	edx, edx
			mov	ecx, 100
			div	ecx
			shl	eax, 8
			mov	ebx, eax

			lea	eax, [Key]
			lea	ecx, [KeyString]
			push	eax
			push	20019h				; KEY_EXECUTE
			push	0
			push	ecx
			push	80000002h			; KEY_LOCAL_MACHINE
			call	_RegOpenKeyExA
			test	eax, eax
			jnz	NoFPU

			push	Key
			call	_RegCloseKey

			fninit
			fld1					; 1
			fldz					; 0
			fdiv					; 1/0 = +inf
			fld	st
			fchs					; try create -inf
			fcompp					; same
			fstsw	word ptr [fp_status]
			mov	eax, [fp_status]
			sahf
			je	NoFPU				; too old FPU ignore


			or	ebx, 4000h			; FPU present
			cmp	ebx, 4400h
			jb	NoFPU
			mov	[ProcessorFeatureFlags], 1h	; assume FPU on chip
NoFPU:
			mov	[ProcessorSignature], ebx
ELSE
			cli
			push	ebp
			mov	ebp, esp
			and	esp, NOT 3
			pushfd
			pop	eax
			mov	ecx, eax
			xor	eax, 40000h
			push	eax
			popfd
			pushfd
			pop	eax
			push	ecx
			popfd
			sti
			mov	esp, ebp
			pop	ebp
			xor	eax, ecx
			mov	ebx, 04300h			; assume 386 + 387
			jz	CPU386

			mov	ebx, 04400h			; assume unknown 486 DX
			mov	[ProcessorFeatureFlags], 1h	; assume FPU on chip
CPU386:
			; Check for an FPU
			fninit
			mov	[fp_status],5A5Ah
			fnstcw	word ptr [fp_status]
			mov	eax, [fp_status]
			test	al, al
			jz	NoFPU

			fnstcw	word ptr [fp_status]
			mov	eax, [fp_status]
			and	eax, 103Fh
			cmp	eax, 3Fh
			jne	NoFPU

			fld1					; 1
			fldz					; 0
			fdiv					; 1/0 = +inf
			fld	st
			fchs					; try create -inf
			fcompp					; same
			fstsw	word ptr [fp_status]
			mov	eax, [fp_status]
			sahf
			jne	okFPU				; too old FPU ignore
NoFPU:
			mov	eax, ebx
			and	eax, 0F00h
			mov	[ProcessorFeatureFlags], 0	; no FPU on chip
			cmp	eax, 0400h
			jne	no486SX

			mov	bl, 20h
no486SX:
okFPU:
			mov	[ProcessorSignature], ebx

ENDIF
			; Check for CPUID
			pushfd			; Save EFLAGS to stack
			pop 	eax 		; Store EFLAGS in EAX
			mov 	ebx, eax	; Save in EBX for testing later
			xor 	eax, 00200000h	; Switch bit 21
			and	ebx, 00200000h	; keep only bit 21
			push 	eax		; Copy "changed" value to stack
			popfd			; Save "changed" EAX to EFLAGS
			pushfd			; Push EFLAGS to top of stack
			pop 	eax 		; Store EFLAGS in EAX
			and	eax, 00200000h	; keep only bit 21
			cmp 	eax, ebx	; See if bit 21 has changed
			je 	Is_Cyrix	; If no change, no CPUID but test if cyrix
OK_CPUID:
			; flag CPUID
			or	[ProcessorSignature], 8000h

			; Get Vendor String
			xor	eax,eax
			cpuid

			mov	dword ptr [ProcessorManufacturerIDString],ebx
			mov	dword ptr [ProcessorManufacturerIDString+4],edx
			mov	dword ptr [ProcessorManufacturerIDString+8],ecx

			cmp	eax,1
			jb	NO_CPUID	; To be safe...

			mov	eax,1
			cpuid

			mov	ebx, [ProcessorSignature]
			and	eax,03fffh	; Mask unknown bits
			and	ebx, NOT 03fffh	; keep adeline bits
			mov	[ProcessorFeatureFlags], edx
			or	eax, ebx

			cmp	dword ptr [ProcessorManufacturerIDString],"uneG"
			je	Check_Intel
			cmp	dword ptr [ProcessorManufacturerIDString],"iryC"
			je	Check_Cyrix
			cmp	dword ptr [ProcessorManufacturerIDString],"htuA"
			je	Check_AMD
			jmp	CPUSignature
Is_Cyrix:
			xor	eax, eax	; clear ax
			sahf			; clear flags, bit 1 is always 1 in flags
			mov	al, 5
			mov	bl, 2
			div	bl		; do an operation that does not change flags
			lahf			; get flags
			cmp	ah, 2		; check for change in flags
			jne	NO_CPUID	; flags changed not Cyrix

			; if we are here, it's got to be a Cyrix
			or	[ProcessorSignature], 20000h

			cmp	[OSType], 3
			jae	NO_CPUID	; can't activate cyrix CPUID under NT

			mov	eax, [ProcessorFeatureFlags]

			test	al, 1
			jz	NO_CPUID	; if no FPU then
						; it is NOT a 6x86
			push	eax
			finit			; inialize fpu
			fldpi			; fpu loads pi
			f2xm1			; square pi
			fstp	dword ptr[esp]	; get pi squared
			pop	eax
			shr	eax, 16

			cmp	ax, 3FC9h	; compare pi squared to Cyrix
			jne	NO_CPUID	; this is not a cyrix FPU
						; so it is not a 6x86
			mov	al, 0c3h
			out	22h, al		; CCR3
			in	al, 23h
			mov	bl, al		; save CCR3
			mov	ah, al
			and	ah, 0Fh
			or	ah, 1 shl 4	; MAPEN = 1
			mov	al, 0c3h
			out	22h, al
			mov	al, ah
			out	23h, al		; Access ok to all reg

			mov	al, 0c0h
			out	22h, al		; CCR0, to change bus
			in	al, 23h
			mov	al, 0c3h
			out	22h, al		; CCR3
			in	al, 23h
			cmp	al, ah
			jne	NO_CPUID	; Something is wrong...


			mov	al, 0e8h
			out	22h, al		; CCR4
			in	al, 23h
			mov	ah, al
			or	ah, 1 shl 7	; CPUID=1
			mov	al, 0e8h
			out	22h, al
			mov	al, ah
			out	23h, al		; cpuid enabled

			mov	al, 0c0h
			out	22h, al		; CCR0, to change bus
			in	al, 23h
			mov	al, 0e8h
			out	22h, al		; CCR4
			in	al, 23h
			cmp	al, ah
			jne	NO_CPUID	; Something is wrong...

			mov	al, 0c3h
			out	22h, al		; CCR3
			mov	al, bl
			out	23h, al		; restore CCR3

			; Check if CPUID enabled ok
			pushfd			; Save EFLAGS to stack
			pop 	eax 		; Store EFLAGS in EAX
			mov 	ebx, eax	; Save in EBX for testing later
			xor 	eax, 00200000h	; Switch bit 21
			push 	eax		; Copy "changed" value to stack
			popfd			; Save "changed" EAX to EFLAGS
			pushfd			; Push EFLAGS to top of stack
			pop 	eax 		; Store EFLAGS in EAX
			cmp 	eax, ebx	; See if bit 21 has changed

			jz 	NO_CPUID	; Not a cyrix with CPUID support


			; reset manufacturer, we'll get it from CPUID
			and	[ProcessorSignature], NOT 0F0000h

			jmp	OK_CPUID

Check_Intel:
			cmp	dword ptr [ProcessorManufacturerIDString+4],"Ieni"
			jne	CPUSignature
			cmp	dword ptr [ProcessorManufacturerIDString+8],"letn"
			jne	CPUSignature
			or	eax, 10000h
			jmp	CPUSignature
Check_Cyrix:
			cmp	dword ptr [ProcessorManufacturerIDString+4],"snIx"
			jne	CPUSignature
			cmp	dword ptr [ProcessorManufacturerIDString+8],"daet"
			jne	CPUSignature
			or	eax, 20000h
			jmp	CPUSignature
Check_AMD:
			cmp	dword ptr [ProcessorManufacturerIDString+4],"itne"
			jne	CPUSignature
			cmp	dword ptr [ProcessorManufacturerIDString+8],"DMAc"
			jne	CPUSignature
			or	eax, 30000h
CPUSignature:
			mov	[ProcessorSignature], eax
NO_CPUID:

			cmp	[OSType], 3
			jae	CacheOk		; can't test cache under NT

			; Check if cache is WB
			mov	eax, cr0
			test	eax, 1 SHL 29
			jnz	NoWB

			or	[ProcessorSignature], 1 SHL 20
NoWB:
			test	eax, 1 SHL 30
			jz	CacheOk

			or	[ProcessorSignature], 1 SHL 22
CacheOk:
			mov	eax, [ProcessorSignature]
			mov	ebx, eax
			and	eax, 0F0000h
			and	ebx, 000F00h

			; Check TLB and Cache Information
			cmp	ebx, 00300h
			jbe	EndCache

			; Check Pentium
			cmp	ebx, 00500h
			jb	NoPPro

			; Check Pentium pro style cache info
			push	eax
			push	ebx
			xor	eax, eax
			cpuid
			cmp	eax, 2
			pop	ebx
			pop	eax
			jb	NoPPro	; cache info not supported

			; Cache Allocate on Writes
			or	[ProcessorSignature], 1 SHL 21

			; get Pentium Pro cache information
			mov	eax, 2
			cpuid
			test	al, al
			jz	NoPPro	; error

			dec	eax
			push	eax
AnalyseCache:
			mov	esi, 4
AnalyseInput:
			call	PentiumProFillCacheInfo
			shr	eax, 8
			jnz	AnalyseInput

			mov	eax, ebx
			mov	ebx, ecx
			mov	ecx, edx

			dec	esi
			jnz	AnalyseInput

			pop	eax
			test	al, al
			jz	EndPPro
			dec	al

			push	eax
			mov	eax, 2
			cpuid
			jmp	AnalyseCache
EndPPro:
			jmp	EndCache
NoPPro:
			; Check Intel
			cmp	eax, 10000h
			jne	NoIntel

			; Check Pentium
			cmp	ebx,0500h
			jb	NoPentium

			; Check Pentium MMX
			test	ProcessorFeatureFlags, 1 SHL 23
			jz	NoPentiumMMX

			call	PentiumMMXFillCacheInfo
			jmp	EndCache
NoPentiumMMX:
			call	PentiumFillCacheInfo
			jmp	EndCache
NoPentium:
			mov	eax, [ProcessorSignature]
			and	eax, 0F0h

			; Check if at least DX2 WB
			cmp	al, 70h
			jb	Cache486

			; Check DX4		; DX2 WB as 16 KB of cache ?
;			cmp	al, 80h		; Model 8 and 9 are DX4
;			jb	Cache486
nowb486:
			call	IntelDX4FillCacheInfo
			jmp	EndCache
NoIntel:
			; Check Cyrix
			cmp	eax, 20000h
			jne	NoCyrix

			; check family
			cmp	ebx, 00500h
			jb	Cache486

			; ok 686
			call	Cyrix686FillCacheInfo
			jmp	EndCache
NoCyrix:
			; Check AMD
			cmp	eax, 30000h
			jne	NoAMD

			; Check family
			cmp	ebx, 00500h
			jb	Cache486
			ja	okK5

			; check K5 Model 0
			mov	eax, [ProcessorSignature]
			and	eax, 0F0h
			jnz	okK5

			; for model 0 fill registers manually
			mov	ebx, 04800000h
			mov	ecx, 08040120h
			mov	edx, 10040120h
			jmp	K5CacheInfo
okK5:
			mov	eax, 80000000h
			cpuid
			cmp	eax, 80000005h
			jb	Cache486	; No Cache Info

			mov	eax, 80000005h
			cpuid
K5CacheInfo:
			push	ebx
			mov	eax, ebx
			and	ebx, 0FF00h
			and	eax, 000FFh
			shl	ebx, 8
			or	eax, ebx
			mov	Processor4KBInstructionTLB, eax

			pop	ebx
			shr	ebx, 16
			mov	eax, ebx
			and	ebx, 0FF00h
			and	eax, 000FFh
			shl	ebx, 8
			or	eax, ebx
			mov	Processor4KBDataTLB, eax

			mov	eax, ecx
			mov	ebx, ecx
			and	eax, 0000F0000h
			and	ebx, 0FF000000h
			shr	eax, 4
			and	ecx, 000000FFFh
			shr	ebx, 8
			or	eax, ecx
			or	eax, ebx
			mov	ProcessorL1DataCache, eax

			mov	eax, edx
			mov	ebx, edx
			and	eax, 0000F0000h
			and	ebx, 0FF000000h
			shr	eax, 4
			and	edx, 000000FFFh
			shr	ebx, 8
			or	eax, edx
			or	eax, ebx
			mov	ProcessorL1InstructionCache, eax
			jmp	EndCache
NoAMD:

Cache486:
			call	Unknown486FillCacheInfo
EndCache:
			mov	eax, ProcessorSignature
			ret

ProcessorIdentification	ENDP

;			The
			End
