
vgainfo		struc
VESASignature	db	4 dup (?)	; 4 signature bytes
VESAVersion	dw	?		; VESA version number
OEMStringPtr	dd	?		; Pointer to OEM string
Capabilities	db	4 dup (?)	; Capabilities of the video environment
VideoModePtr	dd	?		; Pointer to supported Super VGA modes
vgainfo		ends

vesamode	struc
ModeAttributes	dw	?	; mode attributes
WinAAttributes	db	?	; window A attributes
WinBAttributes	db	?	; window B attributes
WinGranularity	dw	?	; window granularity
WinSize		dw	?	; window size
WinASegment	dw	?	; window A start segment
WinBSegment	dw	?	; window B start segment
WinFuncPtr	dd	?	; pointer to window function
BytesPerLine	dw	?	; bytes per scan line
;
; optional information (provided if bit D1 of ModeAttributes is set)
;
XResolution	dw	?	; horizontal resolution
YResolution	dw	?	; vertical resolution
XCharSize	db	?	; character cell width
YCharSize	db	?	; character cell height
NumberOfPlanes	db	?	; number of memory planes
BitsPerPixel	db	?	; bits per pixel
NumberOfBanks	db	?	; number of banks
MemoryModel	db	?	; memory model type
BankSize	db	?	; bank size in kb
vesamode	ends

