@echo off
pkunzip -o objdosr >NUL
wmake BT=DOS CALL=REG
pkzip -m objdosr *.obj >NUL
