@echo off
pkunzip -o objnt >NUL
wmake BT=NT
pkzip -m objnt *.obj >NUL
