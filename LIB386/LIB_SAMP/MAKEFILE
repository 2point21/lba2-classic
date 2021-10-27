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

.SILENT

OBJETS1  = wave.obj wave_i.obj

!ifeq WATCOM9 yes
LIB      = LB9_WAVE
!else
LIB      = LIB_WAVE
!endif

$(LIB).LIB: $(OBJETS1)

	@echo $(OBJETS1) > clibmak.rsp
	@$(WL) $(LIBFLAGS) $(LIB) @clibmak.rsp
	@del clibmak.rsp
	@prntitre M "$(LIB) OK"

.asm.obj:
	@PRNTITRE M "$*.ASM"
	@$(ASM) $(AFLAGS) $*.ASM

.c.obj:
	@PRNTITRE M "$*.C"
	@$(CC) $(CFLAGS) $*
