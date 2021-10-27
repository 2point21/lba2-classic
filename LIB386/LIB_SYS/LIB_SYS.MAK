# name the compiler
!ifdef %HOME
CC = c:\compil\watcom10\binb\wcc386
WL = c:\compil\watcom10\binb\wlib
!else
CC = f:\compil\watcom10\binb\wcc386
WL = f:\compil\watcom10\binb\wlib
!endif

ASMFLAGS  = /m2 /mx /z /p /q /t /jMASM51 /jQUIRKS
CFLAGS   = /oaxet /zp2 /s /4s /x /c /l=dos4g /zq
LIBFLAGS = /c /q /b /n

OBJETS1  = FILES.obj MALLOC.obj LOADMALL.obj LOADSAVE.obj DEF_FILE.obj
OBJETS2  = System.obj Timer_a.obj Timer.obj Buffer_A.obj DIVERS.obj
OBJETS3  = Keyb.obj Keyboard.obj Expand.obj HQ_ress.obj hqr.obj
OBJETS4  = DllLoad.obj Adeline.obj HQ_mem.obj HQ_r_m.obj

LIB      = LIB_SYS

$(LIB).LIB: $(OBJETS1) $(OBJETS2) $(OBJETS3) $(OBJETS4)

	@echo $(OBJETS1) > clibmak.rsp
	@echo $(OBJETS2) >> clibmak.rsp
	@echo $(OBJETS3) >> clibmak.rsp
	@echo $(OBJETS4) >> clibmak.rsp

	@$(WL) $(LIBFLAGS) $(LIB) @clibmak.rsp
	@del clibmak.rsp
	@prntitre G "$(LIB)"

.asm.obj:
	@PRNTITRE M "$*.ASM"
	@TASM $(ASMFLAGS) $*;

.c.obj:
	@PRNTITRE M "$*.C"
	@$( $(CFLAGS) $*

