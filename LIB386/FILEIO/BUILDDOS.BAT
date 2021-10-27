@echo off
pkunzip -o objdosr >NUL
wmake BT=DOS CALL=REG
pkzip -m objdosr *.obj >NUL
pkunzip -o objdoss >NUL
wmake BT=DOS CALL=STACK
pkzip -m objdoss *.obj >NUL

