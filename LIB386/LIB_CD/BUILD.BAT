call watcom10
pkunzip -o obj >NUL
wmake /f lib_CD.mak
pkzip -m obj *.obj >NUL
call watcom9
