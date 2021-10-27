call watcom10

pkunzip -o obj >NUL
copy wr\ail32.obj
wmake
pkzip -m obj *.obj >NUL

call watcom9

pkunzip -o obj9 >NUL
copy wr\ail32.obj
wmake WATCOM9=yes
pkzip -m obj9 *.obj >NUL

call watcom10
