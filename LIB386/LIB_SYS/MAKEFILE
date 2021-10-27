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

.SILENT

OBJETS1  = FILES.obj MALLOC.obj LOADMALL.obj LOADSAVE.obj DEF_FILE.obj
OBJETS2  = System.obj Timer_a.obj Timer.obj Buffer_A.obj DIVERS.obj
OBJETS3  = Keyb.obj Keyboard.obj Expand.obj HQ_ress.obj hqr.obj
OBJETS4  = DllLoad.obj Adeline.obj HQ_mem.obj HQ_r_m.obj Files_A.obj

!ifeq WATCOM9 yes
LIB      = LB9_SYS
!else
LIB      = LIB_SYS
!endif

$(LIB).LIB: $(OBJETS1) $(OBJETS2) $(OBJETS3) $(OBJETS4)
	@echo $(OBJETS1) > clibmak.rsp
	@echo $(OBJETS2) >> clibmak.rsp
	@echo $(OBJETS3) >> clibmak.rsp
	@echo $(OBJETS4) >> clibmak.rsp

	@$(WL) $(LFLAGS) $(LIB) @clibmak.rsp
	@del clibmak.rsp
	@prntitre G "$(LIB)"

.c.obj :
	PRNTITRE M "$*.C"
	$(CC) $^& $(CFLAGS)

.asm.obj :
	PRNTITRE M "$*.ASM"
	$(ASM) $(AFLAGS) $^&.ASM
