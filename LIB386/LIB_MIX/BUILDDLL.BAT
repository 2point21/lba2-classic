pkunzip -o obj >NUL
wmake /f mix_dll.mak
pkzip -m obj *.obj >NUL
