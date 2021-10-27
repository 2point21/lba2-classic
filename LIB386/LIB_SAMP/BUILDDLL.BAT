pkunzip -o obj >NUL
wmake /f wave_dll.mak
pkzip -m obj *.obj >NUL
