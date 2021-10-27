# LIB386\ail

In order to build audio library, following third party SDK is required:
 - Miles Sound System 3.6 - Release 3.6B of 7-Mar-97
 
MSS v3.6 is licensed under "Copyright (C) RAD Game Tools, Inc." terms
and so is not delivered in this package.
 
Following files from MSS SDK are expected to be in LIB386 folder:

```
+---ail
|   +---mss
|       |   MSS.H
|       |   MSS.LIB
|       |   MSS32BC.DEF
|       |   mssb16.tsk
|       |   MSSW.H
|       |   README.TXT
|       |   
|       +---DOSDDK
|       |   +---DIG
|       |   |       DDK.ASM
|       |   |       DDK.H
|       |   |       DIGPAS.ASM
|       |   |       DIGRAP10.ASM
|       |   |       DIGSB.ASM
|       |   |       DIGSB16.ASM
|       |   |       DIGSBPRO.ASM
|       |   |       DIGSSYS.ASM
|       |   |       DIGTEST.C
|       |   |       DIGTEST.EXE
|       |   |       DOMAKE.BAT
|       |   |       MSS_DIG.INC
|       |   |       WATCOM.MAK
|       |   |       
|       |   +---MDI
|       |   |       DDKXMIDI.BIN
|       |   |       DOMAKE.BAT
|       |   |       FATMAN.AD
|       |   |       FATMAN.BNK
|       |   |       FATMAN.CAT
|       |   |       FATMAN.OPL
|       |   |       MDIAIL2X.ASM
|       |   |       MDIMPU.ASM
|       |   |       MDINULL.ASM
|       |   |       MDITEST.C
|       |   |       MDITEST.EXE
|       |   |       MDI_DDK.H
|       |   |       MPU401.INC
|       |   |       MSS_MDI.INC
|       |   |       MT32.INC
|       |   |       SAMPLE.AD
|       |   |       SAMPLE.BNK
|       |   |       SAMPLE.CAT
|       |   |       SAMPLE.OPL
|       |   |       SPKR.INC
|       |   |       WATCOM.MAK
|       |   |       YAMAHA.INC
|       |   |       
|       |   +---SNDFONT
|       |           AWE32VSE.C
|       |           AWE32VSE.H
|       |           DOMAKE.BAT
|       |           DPMI.C
|       |           DPMI.H
|       |           SAGA.SBK
|       |           SAGA.XMI
|       |           SBAWE-SF.MDI
|       |           SNDFONT.C
|       |           SNDFONT.EXE
|       |           WATCOM.MAK
|       |           
|       +---DOSDRV
|       |       ADLIB.MDI
|       |       ADLIBG.MDI
|       |       AUDIODRV.DIG
|       |       ESFM.MDI
|       |       FATMAN.AD
|       |       FATMAN.OPL
|       |       IWAV.DIG
|       |       IWAV.MDI
|       |       JAMMER.DIG
|       |       MPU401.MDI
|       |       MSSDRVR.LST
|       |       MSSW95.EXE
|       |       MT32MPU.MDI
|       |       NULL.MDI
|       |       NVDIG.DIG
|       |       NVMPU.MDI
|       |       OPL3.MDI
|       |       PAS.MDI
|       |       PASPLUS.MDI
|       |       PCSPKR.MDI
|       |       PROAUDIO.DIG
|       |       RAP10.DIG
|       |       SB16.DIG
|       |       SBAWE32.MDI
|       |       SBLASTER.DIG
|       |       SBLASTER.MDI
|       |       SBPRO.DIG
|       |       SBPRO1.MDI
|       |       SBPRO2.MDI
|       |       SETSOUND.EXE
|       |       SNDSCAPE.DIG
|       |       SNDSCAPE.MDI
|       |       SNDSYS.DIG
|       |       TANDY.MDI
|       |       ULTRA.DIG
|       |       ULTRA.MDI
|       |       
|       +---DOSSRC
|       |       AIL.C
|       |       AIL.INC
|       |       AILA.ASM
|       |       AILCD.C
|       |       AILDEBUG.C
|       |       AILDEBUG.INC
|       |       AILQAPI.C
|       |       AILSFILE.C
|       |       AILSR.C
|       |       AILSS.C
|       |       AILSSA.ASM
|       |       AILSTRM.C
|       |       AILXDIG.C
|       |       AILXMIDI.C
|       |       BORLAND.MAK
|       |       DOMAKE.BAT
|       |       DOMAKEB.BAT
|       |       MEMFILE.C
|       |       WATCOM.MAK
|       |       
|       +---TOOLS
|       |   |   adpcm.exe
|       |   |   CLAD.EXE
|       |   |   GLIB.EXE
|       |   |   MIDIECHO.EXE
|       |   |   MIDIFORM.EXE
|       |   |   MIDILOG.EXE
|       |   |   MIDIREC.EXE
|       |   |   MLIST.EXE
|       |   |   VTOR.EXE
|       |   |   WAVELIB.EXE
|       |   |   
|       |   +---SRC
|       |           ADPCM.C
|       |           CLAD.C
|       |           DOMAKE.BAT
|       |           GEN.H
|       |           GENA.ASM
|       |           GENC.C
|       |           GLIB.C
|       |           MIDIECHO.C
|       |           MIDIFORM.C
|       |           MIDILOG.C
|       |           MIDIREC.C
|       |           MLIST.C
|       |           MSSW95.C
|       |           REAL_ISR.ASM
|       |           REAL_ISR.BIN
|       |           SETSOUND.C
|       |           TIMER.ASM
|       |           TIMER.H
|       |           VTOR.C
|       |           WATCOM.MAK
|       |           WAVELIB.C
|       |           
|       +---WINSRC
|               DOMAKE.BAT
|               MSS.RC
|               mss.res
|               mss16.lk1
|               mss16.mk
|               mss16.mk1
|               mss16.tgt
|               mss16.wpj
|               mss32.lib
|               mss32.lk1
|               mss32.mk
|               mss32.mk1
|               mss32.sym
|               mss32.tgt
|               mss32.wpj
|               SI.BAT
|               SSMACROS.INC
|               WAIL.C
|               WAIL16.DEF
|               WAIL16.MAK
|               WAIL16.RSP
|               WAIL32.MAK
|               WAIL32.RSP
|               WAILB16.C
|               WAILB16.DEF
|               WAILCD.C
|               WAILDBG.C
|               WAILDBG.INC
|               WAILDS.C
|               WAILQAPI.C
|               WAILSFIL.C
|               WAILSS.C
|               WAILSS16.ASM
|               WAILSS32.ASM
|               WAILSTRM.C
|               WAILSYS.C
|               WAILXDIG.C
|               WAILXMID.C
|       
+---H
|   +---AIL
|       MSS.H
|       MSSW.H
|           
+---LIB
|       MSS.LIB
|       MSS32.DLL
|       MSS32.LIB
|       
+---SYSTEM
        DLLLOAD.C
```

The `mss` directory correspond to the MSS SDK directory, and other files
found in H, LIB and SYSTEM directories are copied from that `mss` SDK
folder to match the build system.
