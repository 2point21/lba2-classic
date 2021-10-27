pkunzip -o obj >NUL
wmake /f dll.mak
pkzip -m obj *.obj >NUL
