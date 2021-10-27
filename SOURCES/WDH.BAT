
@REM *** Lance le deboggeur sur carte HERCULE

wd.exe /trap=rsi /Monochrome /swap %1 %2 %3 %4 %5 %6 %7 %8 %9
rtc
mouse /off
mouse /on
call systime

