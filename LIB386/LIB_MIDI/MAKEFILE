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
CC = f:\compil\watcom95\bin\wcc386
WL = f:\compil\watcom95\binb\wlib
!else
CC = f:\compil\watcom10\binb\wcc386
WL = f:\compil\watcom10\binb\wlib
!endif
!endif

ASM = ml

!ifeq WATCOM9 yes
CFLAGS   = /oaxet /s /zp2 /4s /zq /DWATCOM9
!else
CFLAGS   = /oaxet /s /zp2 /5s /zq /wx
!endif
LFLAGS = -b -n -c -q
#AFLAGS  = /m2 /mx /z /zi /t /jMASM51 /jQUIRKS
AFLAGS = /Cx /Zm /c /W0 /Sa /DNoLanguage=SYSCALL /Djumps=;


OBJETS1  = midi.obj ail32.obj
OBJETS2  =
OBJETS3  =
OBJETS4  =

!ifeq WATCOM9 yes
LIB      = LB9_MIDI
!else
LIB      = LIB_MIDI
!endif

$(LIB).LIB: $(OBJETS1) $(OBJETS2) $(OBJETS3) $(OBJETS4)

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
