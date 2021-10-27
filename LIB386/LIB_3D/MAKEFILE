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
#AFLAGS = /m2 /mx /z /p /q /t /jMASM51 /jQUIRKS
AFLAGS = /Cx /Zm /c /W0 /Sa /DNoLanguage=SYSCALL /Djumps=;

OBJETS1  = P_SINTAB.obj P_ANIM.obj
OBJETS2  = P_OB_ISO.obj P_TRIGO.obj
OBJETS3  = P_FUNC.obj
OBJETS4  =

!ifeq WATCOM9 yes
LIB      = LB9_3D
!else
LIB      = LIB_3D
!endif

$(LIB).LIB: $(OBJETS1) $(OBJETS2) $(OBJETS3) $(OBJETS4)

	@echo $(OBJETS1) > clibmak.rsp
	@echo $(OBJETS2) >> clibmak.rsp
	@echo $(OBJETS3) >> clibmak.rsp
#	@echo $(OBJETS4) >> clibmak.rsp


	@$(WL) $(WLIBFLAGS) $(LIB) @clibmak.rsp
	@del clibmak.rsp
	@prntitre G "$(LIB)"

.asm.obj:
	@PRNTITRE M "$*.ASM"
	@$(ASM) $(AFLAGS) $*.ASM

.c.obj:
	@PRNTITRE M "$*.C"
	@$(CC) $(WCFLAGS) $*
