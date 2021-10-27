@if "%1"=="" goto syntaxe
@if "%2"=="" goto syntaxe
@wdw /tr=tcp;%1.%2 %3 %4 %5 %6 %7 %8 %9
@goto fin
:syntaxe
@echo Remote Watcom Debugger
@echo Usage: RWD socket_num machine [prog] [args]
@echo        (socket_num is obtained by tcpserv)
@echo        (machine is the name of executing machine)   
:fin

