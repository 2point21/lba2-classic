//--------------------------------------------------------------------------
#ifndef	_WIN32

//--------------------------------------------------------------------------
#ifndef	LIB_SVGA_VESA
#define	LIB_SVGA_VESA

//--------------------------------------------------------------------------
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------
#pragma	pack(push, 1)

typedef struct
{
  char		VbeSignature[4]		;
  U16		VbeVersion		;
  char		*VbeOemStringPtr	;
  U32		VbeCapabilities		;
  U32		VbeVideoModePtr		;
  U16		VbeTotalMemory		;

  U16		VbeOemSoftwareRev	;
  char		*VbeOemVendorNamePtr	;
  char		*VbeOemProductNamePtr	;
  char		*VbeOemProductRevPtr	;
  U8		VbeReserved[222]	;
  U8		VbeOemData[256]		;
} VBE_INFO_BLOCK;

#pragma	pack(pop)

//--------------------------------------------------------------------------
#define			VESA_WINDOWED		1
#define			VESA_LINEAR		2
#define			VESA_LINEAR_OR_WINDOWED	(VESA_LINEAR|VESA_WINDOWED)

#define			VESAMODE_LINEAR		(1<<14)

//--------------------------------------------------------------------------
extern	U32			VESAMode	;
extern	U32			BankSize	;

//--------------------------------------------------------------------------
extern	PTR_VOID_FUNC		SwitchBankVESA		;
extern	VOID_FUNC		SwitchBankVESA1		;
extern	VOID_FUNC		SwitchBankVESA2		;
extern	VOID_FUNC		SwitchBankVESA2Sel	;

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
extern	VBE_INFO_BLOCK		*PtrVbeInfoBlock;

#pragma	aux PtrVbeInfoBlock	"*"

//--------------------------------------------------------------------------
typedef	void	(MODE_CALLBACK)(S32 resx, S32 resy, S32 depth, S32 attrib, S32 pages);

// Attrib = VESA_WINDOWED / VESA_LINEAR / VESA_LINEAR_OR_WINDOWED

#pragma aux 	ARG_MODE_CALLBACK	"*"		\
	parm caller	[eax] [ebx] [ecx] [edx]	[edi]	\
	modify		[esi]

#pragma aux	(ARG_MODE_CALLBACK)	MODE_CALLBACK

//--------------------------------------------------------------------------
typedef	void	(PROC_SET_PALETTE)(void *pal, S32 start, S32 n)	;

#pragma aux 	ARG_SET_PALETTE	"*"		\
	parm caller	[esi] [edx] [ecx]	\
	modify		[eax ebx edi]

#pragma aux	(ARG_SET_PALETTE)	PROC_SET_PALETTE

//--------------------------------------------------------------------------
extern	PROC_SET_PALETTE	SetPaletteVESA2		;
extern	PROC_SET_PALETTE	SetPaletteVESA1		;
extern	PROC_SET_PALETTE	SetPaletteDirect	;

//--------------------------------------------------------------------------
extern	PROC_SET_PALETTE	*SetPaletteVESA	;

#pragma	aux	SetPaletteVESA	"*"

//--------------------------------------------------------------------------
void   SetPaletteOneVESA(S32 col, S32 red, S32 green, S32 blue);

#pragma	aux SetPaletteOneVESA  "*"		\
	parm caller    [edx] [eax] [ebx] [ecx]	\
	modify	[esi edi]

//--------------------------------------------------------------------------
extern	void	VblVESA()	;

#pragma	aux	VblVESA		"*"		\
	modify	[eax ebx ecx edx esi edi]

//--------------------------------------------------------------------------
extern	void	PaletteVblVESA(S32 bool)	;

#pragma	aux	PaletteVblVESA	"*"		\
	parm	[eax]

//--------------------------------------------------------------------------
extern	S32	InitVESA()	;

#pragma aux 	InitVESA	"*"	\
	modify	[]

//--------------------------------------------------------------------------
extern	void	EndVESA()	;

#pragma aux 	EndVESA		"*"	\
	modify	[]

//--------------------------------------------------------------------------
extern	void	ScanVESAModes(MODE_CALLBACK *callback)	;

#pragma aux 	ScanVESAModes	"*"			\
	parm caller	[]				\
	modify		[eax ebx ecx edx esi edi]

// Will scan all available modes and call callback with info on each mode

//--------------------------------------------------------------------------
extern	S32	DetectVESAMode(U32 ResX, U32 ResY, U32 Depth, U32 Memory);

#pragma aux 	DetectVESAMode	"*"	\
	parm caller	[eax] [ebx] [ecx] [edx]	\
	modify	[esi edi]

// Will find the best (lowest) resolution (mode) containing the required res
// with same Depth. Memory indicate if linear, windowed, or don't care
// ( VESA_WINDOWED / VESA_LINEAR / VESA_LINEAR_OR_WINDOWED )

//--------------------------------------------------------------------------
extern	S32	InitVESAMode(U32 Mode);

#pragma aux 	InitVESAMode	"*"	\
	parm caller	[eax]			\
	modify	[ebx ecx edx esi edi]

// If Mode is ORed with VESAMODE_LINEAR is set then will TRY to set a linear
// frame buffer, else will default to windowed
//
// Return new mode set with bit 14 correctly set (VESAMODE_LINEAR)

//--------------------------------------------------------------------------
extern	S32	DetectInitVESAMode(U32 ResX, U32 ResY, U32 Depth, U32 Memory);

#pragma aux 	DetectInitVESAMode	"*"	\
	parm caller	[eax] [ebx] [ecx] [edx]		\
	modify	[esi edi]

// Will find the best (lowest) resolution containing the required res
// with same Depth. Memory indicate if linear, windowed, or don't care
// ( VESA_WINDOWED / VESA_LINEAR / VESA_LINEAR_OR_WINDOWED )

//--------------------------------------------------------------------------
extern	void	GetLastVESAMode()	;

#pragma aux 	GetLastVESAMode		"*"	\
	modify	[]

//--------------------------------------------------------------------------
extern	void	ResetLastVESAMode()	;

#pragma aux 	ResetLastVESAMode	"*"	\
	modify	[]

//--------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_SVGA_VESA

//--------------------------------------------------------------------------
#endif//_WIN32

//--------------------------------------------------------------------------
