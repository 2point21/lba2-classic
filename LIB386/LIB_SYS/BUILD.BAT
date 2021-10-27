call watcom10

pkunzip -o obj >NUL
wmake
pkzip -m obj *.obj >NUL

call watcom9

pkunzip -o obj9 >NUL
wmake WATCOM9=yes
pkzip -m obj9 *.obj >NUL

call watcom10