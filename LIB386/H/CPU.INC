CPU_UNKNOWN		EQU	0
CPU_INTEL		EQU	1
CPU_CYRIX		EQU	2
CPU_AMD			EQU	3

CLEAN_MMX	MACRO
		local	skip

		test	[ProcessorFeatureFlags], 1 SHL 23
        	jz	skip

        	emms
skip:
		ENDM
