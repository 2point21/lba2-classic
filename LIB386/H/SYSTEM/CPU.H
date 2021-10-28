//──────────────────────────────────────────────────────────────────────────
#ifndef LIB_SYSTEM_CPU
#define LIB_SYSTEM_CPU

//──────────────────────────────────────────────────────────────────────────
#ifdef __cplusplus
extern "C" {
#endif

//──────────────────────────────────────────────────────────────────────────
extern	struct
	{
		U32	Stepping		:4	;
		U32	Model			:4	;
		U32	Family			:4	;
		U32	Type			:2	;
		U32	FPU			:1	;
		U32	CPUID			:1	;
		U32	Manufacturer		:4	;
		U32	CacheWriteBack		:1	;
		U32	CacheAllocOnWrite	:1	;
		U32	CacheDisabled		:1	;
		U32	Undefined		:9	;
	}
	ProcessorSignature	;

#pragma aux ProcessorSignature	"*"

/*
; CacheDisabled :	     1 = L1 and L2 cache are disabled

; CacheAllocOnWrite :	     1 = Cache fetch lines on writes (read before write)

; CacheWriteBack :	     1 = Cache is write_bakc

; Manufacturer :	  0000 = Unknown
;			  0001 = Intel
;			  0010 = Cyrix
;			  0011 = AMD
;
; CPUID Supported : 	     1 = CPUID supported

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
; Model (family = 0110):  0000 = Pentium Pro A-Step (non commercial)
; Model (family = 0110):  0001 = Pentium Pro
; Model (family = 0110):  0100 = Pentium Pro overdrive for Pentium
;
; Model (family = 0101):  0000 = Pentium A-Step (non commercial)
; 			  0001 = Pentium P5 (60/66 Mhz)
;			  0010 = Pentium P54C
;			  0011 = Pentium P24T overdrive processors
;			  0100 = Pentium P55C MMX
;			  0101 = Pentium overdrive for IntelDX4
;			  0110 = Pentium overdrive for P5
;			  0110 = Pentium P54C
;			  1111 = Unknown
;
; Model (family = 0100):  0000 = Intel486 DX-25/33
;			  0001 = Intel486 DX-50
;			  0010 = Intel486 SX
;			  0011 = Intel487 (SX), or Intel486 DX2
;			  0100 = Intel486 SL
;			  0101 = IntelSX2
;			  0111 = IntelDX2 WB enhanced
;			  1000 = IntelDX4 and IntelDX4 overdrive
;			  1001 = IntelDX4 and IntelDX4 overdrive
;			  1110 = AMD5x86
;			  1111 = AMD5x86 WB
;
; Model (family = 0011):  0000 = Intel386 DX
;			  0010 = Intel386 SX, Intel386 CX, Intel386 EX
;			  0100 = Intel386 SL
;			  1111 = Unknown

*/

//──────────────────────────────────────────────────────────────────────────
extern	struct
	{
		U32	FPUOnChip			:1	;
		U32	VitualModeExtensions		:1	;
		U32	DebugExtensions			:1	;
		U32	PageSizeExtensions		:1	;
		U32	TimeStampCounter		:1	;
		U32	ModelSpecificRegisters		:1	;
		U32	PhysicalAddressExtensions	:1	;
		U32	MachineCheckException		:1	;
		U32	CMPXCHG8BInstruction		:1	;
		U32	APIC				:1	;
		U32	Reserved1			:2	;
		U32	MemoryTypeRangeRegisters	:1	;
		U32	GlobalPagingExtensions		:1	;
		U32	MachineCheckArchitecture	:1	;
		U32	ConditionalMoveInstruction	:1	;
		U32	Reserved2			:7	;
		U32	MMX				:1	;
		U32	Reserved3			:8	;
	}
	ProcessorFeatureFlags	;

#pragma aux ProcessorFeatureFlags	"*"

/*
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
*/

//──────────────────────────────────────────────────────────────────────────
extern	char	ProcessorManufacturerIDString[]	;

#pragma	aux	ProcessorManufacturerIDString	"*"

//──────────────────────────────────────────────────────────────────────────
typedef	struct
	{
		U32	Entries		:16	;
		U32	Ways		:8	;
		U32	Reserved	:8	;
	}
	TLB	;

typedef	struct
	{
		U32	LineSize	:8	;
		U32	Tags		:4	;
		U32	Ways		:4	;
		U32	Size		:16	;
	}
	CACHE	;

//──────────────────────────────────────────────────────────────────────────
extern	TLB	Processor4KBDataTLB		;
extern	TLB	Processor4KBInstructionTLB	;

extern	TLB	Processor4MBDataTLB		;
extern	TLB	Processor4MBInstructionTLB	;

extern	CACHE	ProcessorL1DataCache		;
extern	CACHE	ProcessorL1InstructionCache	;

extern	CACHE	ProcessorL2Cache		;

//──────────────────────────────────────────────────────────────────────────
#pragma	aux	Processor4KBDataTLB		"*"
#pragma	aux	Processor4KBInstructionTLB	"*"

#pragma	aux	Processor4MBDataTLB		"*"
#pragma	aux	Processor4MBInstructionTLB	"*"

#pragma	aux	ProcessorL1DataCache		"*"
#pragma	aux	ProcessorL1InstructionCache	"*"

#pragma	aux	ProcessorL2Cache		"*"

//──────────────────────────────────────────────────────────────────────────
extern	U32	ProcessorIdentification()	;

#pragma	aux	ProcessorIdentification	"*"	\
	modify	[eax ebx ecx edx esi edi]

//──────────────────────────────────────────────────────────────────────────
#ifdef __cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif// LIB_SYSTEM_CPU

//──────────────────────────────────────────────────────────────────────────
