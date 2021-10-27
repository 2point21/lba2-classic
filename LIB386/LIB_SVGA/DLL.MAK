AFLAGS = /Cx /Zm /c /W0 /Sa /DNoLanguage=SYSCALL /Djumps=;
LINKDIR = format os2 lx dll

dll: realtek.dll oak.dll ncr.dll compaq.dll ati.dll cirrus.dll genoa.dll paradise.dll s3.dll trident.dll tseng.dll

#
# Svga driver: REALTEK
#

realtek.dll: dll.asm
   ml $(AFLAGS) /DREALTEK dll.asm
   wlink n realtek.dll f dll $(LINKDIR)

#
# Svga driver: OAK
#

oak.dll: dll.asm
   ml $(AFLAGS) /DOAK dll.asm
   wlink n oak.dll f dll $(LINKDIR)

#
# Svga driver: NCR
#

ncr.dll: dll.asm
   ml $(AFLAGS) /DNCR dll.asm
   wlink n ncr.dll f dll $(LINKDIR)

#
# Svga driver: COMPAQ
#

compaq.dll: dll.asm
   ml $(AFLAGS) /DCOMPAQ dll.asm
   wlink n compaq.dll f dll $(LINKDIR)

#
# Svga driver: ATI
#

ati.dll: dll.asm
   ml $(AFLAGS) /DATI dll.asm
   wlink n ati.dll f dll $(LINKDIR)

#
# Svga driver: CIRRUS
#

cirrus.dll: dll.asm
   ml $(AFLAGS) /DCIRRUS dll.asm
   wlink n cirrus.dll f dll $(LINKDIR)
#
# Svga driver: GENOA
#

genoa.dll: dll.asm
   ml $(AFLAGS) /DGENOA dll.asm
   wlink n genoa.dll f dll $(LINKDIR)

#
# Svga driver: PARADISE
#

paradise.dll: dll.asm
   ml $(AFLAGS) /DPARADISE dll.asm
   wlink n paradise.dll f dll $(LINKDIR)

#
# Svga driver: S3
#

s3.dll: dll.asm
   ml $(AFLAGS) /DS3 dll.asm
   wlink n s3.dll f dll $(LINKDIR)

#
# Svga driver: TRIDENT
#

trident.dll: dll.asm
   ml $(AFLAGS) /DTRIDENT dll.asm
   wlink n trident.dll f dll $(LINKDIR)

#
# Svga driver: Tseng Lab ET3000 & ET4000
#

tseng.dll: dll.asm
   ml $(AFLAGS) /DTSENG dll.asm
   wlink n tseng.dll f dll $(LINKDIR)
