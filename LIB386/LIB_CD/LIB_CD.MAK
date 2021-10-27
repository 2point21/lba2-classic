
TASMFLAGS = /m2 /mx /q /p /z /t /jMASM51 /jQUIRKS
WCFLAGS   = /l=dos4g /oaxet /s /zp2 /5s /c /x /zq
WLIBFLAGS = -b -n -c -q

OBJETS1  = cdrom.obj
OBJETS2  = 
OBJETS3  = 
OBJETS4  =

LIB      = LIB_CD

$(LIB).LIB: $(OBJETS1) $(OBJETS2) $(OBJETS3) $(OBJETS4)
	@echo $(OBJETS1) > clibmak.rsp
#	@echo $(OBJETS2) >> clibmak.rsp
#	@echo $(OBJETS3) >> clibmak.rsp
#       @echo $(OBJETS4) >> clibmak.rsp

	@WLIB $(WLIBFLAGS) $(LIB) @clibmak.rsp
	@del clibmak.rsp
	@prntitre G "$(LIB)"

.asm.obj:
	@PRNTITRE M "$*.ASM"
	@TASM $(TASMFLAGS) $*;

.c.obj:
	@PRNTITRE M "$*.C"
	@wcl386 $(WCFLAGS) $*


