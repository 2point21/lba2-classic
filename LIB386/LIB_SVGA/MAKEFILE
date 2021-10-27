# name the compiler
!ifdef %HOME
!ifeq WATCOM9 yes
CC = c:\compil\watcom\bin\wcc386p
WL = c:\compil\watcom\binb\wlib
!else
CC = c:\compil\watcom10\binb\wcc386
WL = c:\compil\watcom10\binb\wlib
!endif
!else
!ifeq WATCOM9 yes
CC = f:\compil\watcom\bin\wcc386p
WL = f:\compil\watcom\binb\wlib
!else
CC = f:\compil\watcom10\binb\wcc386
WL = f:\compil\watcom10\binb\wlib
!endif
!endif

ASM = ml

!ifeq WATCOM9 yes
CFLAGS = /oeaxt /zp2 /4s /zq /s /DWATCOM9
!else
CFLAGS = /oeaxt /zp2 /5s /zq /s
!endif
LFLAGS = /c /q /b /n
#AFLAGS  = /m2 /mx /z /zi /t /jMASM51 /jQUIRKS
AFLAGS = /Cx /Zm /c /W0 /Sa /DNoLanguage=SYSCALL /Djumps=;


OBJETS1  = InitMode.obj InitSvga.obj Graph_A.obj
OBJETS2  = S_Text.obj S_string.obj S_Mouse.Obj Mask_A.obj Font_A.obj
OBJETS3  = S_Screen.obj S_Block.obj S_Block2.obj S_Pal.obj
OBJETS4  = S_Plot.obj S_Box.obj S_Line.obj S_Poly.obj S_FillV.obj
OBJETS5  = S_MouseA.obj Rect.obj Zoom.obj MaskGph.obj GraphMsk.obj
OBJETS6  = S_Phys.obj texture.obj s_block3.obj Lbm.obj Pcx.obj
OBJETS7  = S_Dll.obj vesa.obj Gif.obj

!ifeq WATCOM9 yes
LIB      = LB9_SVGA
!else
LIB      = LIB_SVGA
!endif

$(LIB).LIB: $(OBJETS1) $(OBJETS2) $(OBJETS3) $(OBJETS4) $(OBJETS5) $(OBJETS6) $(OBJETS7)

	@echo $(OBJETS1) > clibmak.rsp
	@echo $(OBJETS2) >> clibmak.rsp
	@echo $(OBJETS3) >> clibmak.rsp
	@echo $(OBJETS4) >> clibmak.rsp
	@echo $(OBJETS5) >> clibmak.rsp
	@echo $(OBJETS6) >> clibmak.rsp
	@echo $(OBJETS7) >> clibmak.rsp

	@$(WL) $(LIBFLAGS) $(LIB) @clibmak.rsp
	@del clibmak.rsp
	@prntitre M "$(LIB) OK"

.asm.obj:
	@PRNTITRE M "$*.ASM"
	@$(ASM) $(AFLAGS) $*.ASM

.c.obj:
	@PRNTITRE M "$*.C"
	@$(CC) $(CFLAGS) $*
