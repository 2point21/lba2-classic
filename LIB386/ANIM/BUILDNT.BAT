@echo off
pkunzip -o objntr >NUL
wmake BT=NT CALL=REG
pkzip -m objntr *.obj >NUL
pkunzip -o objnts >NUL
wmake BT=NT CALL=STACK
pkzip -m objnts *.obj >NUL

