@echo off
pkunzip -o objdos >NUL
wmake BT=DOS
pkzip -m objdos *.obj >NUL
