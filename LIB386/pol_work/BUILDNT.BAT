@echo off
pkunzip -o objntr >NUL
wmake BT=NT CALL=REG
pkzip -m objntr *.obj >NUL
