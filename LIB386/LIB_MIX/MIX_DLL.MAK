AFLAGS = /Cx /Zm /c /W0 /Sa /DNoLanguage=SYSCALL /Djumps=;
LINKDIR = format os2 lx dll

dll: m_pas.dll m_mwss.dll m_sb2cd.dll m_sbpro.dll m_sb16.dll m_gold.dll

#
# Mixer driver: AD1848 SoundPort, Microsoft Windows Sound System
#

m_mwss.dll: mixer_a.asm
   ml $(AFLAGS) /DMWSS mixer_a.asm
   wlink n m_mwss.dll f mixer_a $(LINKDIR)

#
# Mixer driver: Pro Audio Spectrum
#

m_pas.dll: mixer_a.asm
   ml $(AFLAGS) /DPAS mixer_a.asm
   wlink n m_pas.dll f mixer_a $(LINKDIR)

#
# Mixer driver: Sound Blaster 2 CD Interface card
#

m_sb2cd.dll: mixer_a.asm
   ml $(AFLAGS) /DSB2CD mixer_a.asm
   wlink n m_sb2cd.dll f mixer_a $(LINKDIR)

#
# Mixer driver: Sound Blaster Pro
#

m_sbpro.dll: mixer_a.asm
   ml $(AFLAGS) /DSBPRO mixer_a.asm
   wlink n m_sbpro.dll f mixer_a $(LINKDIR)

#
# Mixer driver: Sound Blaster 16
#

m_sb16.dll: mixer_a.asm
   ml $(AFLAGS) /DSB16 mixer_a.asm
   wlink n m_sb16.dll f mixer_a $(LINKDIR)

#
# Mixer driver: Yamaha Gold, Adlib Gold
#

m_gold.dll: mixer_a.asm
   ml $(AFLAGS) /DGOLD mixer_a.asm
   wlink n m_gold.dll f mixer_a $(LINKDIR)
