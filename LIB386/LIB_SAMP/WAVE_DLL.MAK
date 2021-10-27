ASM = ml
#AFLAGS  = /m2 /mx /z /zi /t /jMASM51 /jQUIRKS
AFLAGS = /Cx /Zm /c /W0 /Sa /DNoLanguage=SYSCALL /Djumps=;
LINKDIR = format os2 lx dll

dll: w_pas16.dll w_pas.dll w_gus.dll w_mwss.dll w_sb.dll w_sb2.dll w_sbpro.dll w_sb16.dll w_gold.dll

#
# Wave driver: GF1, Gravis UltraSound
#

w_gus.dll: wave_a.asm gusdig32.obj
   $(ASM) $(AFLAGS) /DGUS wave_a.asm
   wlink n w_gus.dll f wave_a,gusdig32,pminit_d l gf1_ospm.lib $(LINKDIR)

#
# Wave driver: PAS16
#

w_pas16.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DPAS16 wave_a.asm
   wlink n w_pas16.dll f wave_a $(LINKDIR)

#
# Wave driver: PAS
#

w_pas.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DPAS wave_a.asm
   wlink n w_pas.dll f wave_a $(LINKDIR)

#
# Wave driver: AD1848 SoundPort, Microsoft Windows Sound System
#

w_mwss.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DMWSS wave_a.asm
   wlink n w_mwss.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster
#

w_sb.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSBLASTER1 wave_a.asm
   wlink n w_sb.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster 2
#

w_sb2.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSBLASTER wave_a.asm
   wlink n w_sb2.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster Pro
#

w_sbpro.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSBPRO wave_a.asm
   wlink n w_sbpro.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster 16, STEREO, 16 BIT
#

w_sb16.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSB16 /DSTEREO /DSAMPLE16BIT wave_a.asm
   wlink n w_sb16.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster 16, STEREO, 16 BIT, SURROUND
#

w_sb16_s.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSB16 /DSTEREO /DSAMPLE16BIT /DSURROUND wave_a.asm
   wlink n w_sb16_s.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster 16, STEREO, 8 BIT
#

w_sb16_8.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSB16 /DSTEREO wave_a.asm
   wlink n w_sb16_8.dll f wave_a $(LINKDIR)

#
# Wave driver: Sound Blaster 16, MONO, 16 BIT
#

w_sb16_m.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DSB16 /DSAMPLE16BIT wave_a.asm
   wlink n w_sb16_m.dll f wave_a $(LINKDIR)

#
# Wave driver: Yamaha Gold, Adlib Gold
#

w_gold.dll: wave_a.asm
   $(ASM) $(AFLAGS) /DGOLD wave_a.asm
   wlink n w_gold.dll f wave_a $(LINKDIR)
