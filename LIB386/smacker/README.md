# LIB386\smacker

In order to build video decoding library, following third party SDK is required:
 - Smacker v3.0
 
Smacker v3.0 is licensed under ""Copyright (C) 1994-97 RAD Game Tools, Inc."
terms and so is not delivered in this package.
 
Following files from Smacker SDK are expected to be in LIB386 folder:

```
+---smacker
|   +---sdk
|           DOBACK.BAT
|           DODBUF.BAT
|           DODDF.BAT
|           DODDP.BAT
|           DODDP6.BAT
|           DODDS.BAT
|           DODW.BAT
|           DODW32.BAT
|           DOEXAM.BAT
|           DOFILE.BAT
|           DOFILE32.BAT
|           DOIO.BAT
|           DOMSS.BAT
|           DOMSSW.BAT
|           DOMSSW32.BAT
|           DOMULT.BAT
|           DOSKIP.BAT
|           DOSOS3.BAT
|           DOSOS4.BAT
|           DOTRANS.BAT
|           DOWIN.BAT
|           DOWIN32.BAT
|           EXAMBACK.C
|           EXAMDBUF.C
|           EXAMDDF.C
|           EXAMDDP.C
|           EXAMDDP6.C
|           EXAMDDS.C
|           EXAMDW.C
|           EXAMFILE.C
|           EXAMIO.C
|           EXAMMSS.C
|           EXAMMSSW.C
|           EXAMMULT.C
|           EXAMPLE.C
|           EXAMSKIP.C
|           EXAMSOS3.C
|           EXAMSOS4.C
|           EXAMTRAN.C
|           EXAMWIN.C
|           HISTORY.TXT
|           MSSLITE.LIB
|           RAD.H
|           RADMAL.I
|           RADMALC.I
|           RADMALD.I
|           RADMALW.I
|           SMACK.H
|           SMACK.LIB
|           SMACKE16.DLL
|           SMACKE16.MAP
|           SMACKINF.I
|           SMACKW16.DLL
|           SMACKW16.LIB
|           SMACKW16.MAP
|           SMACKW32.DLL
|           SMACKW32.LIB
|           SMACKW32.MAP
|           SMKW32BC.DEF
|           SMKW32MS.LIB
|           SVGA.H
|
+---H
|   +---smacker
|       RAD.H
|       SMACK.H
|           
+---LIB
        SMACK.LIB
        SMACKW32.DLL
        SMACKW32.LIB
```

The `smacker` directory correspond to the Smacker SDK directory, and
other files found in H and LIB directories are copied from that `smacker`
SDK folder to match the build system.
