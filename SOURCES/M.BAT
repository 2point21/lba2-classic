@echo off

@if "%1"=="?"	goto syntaxe
@if "%1"=="/?"	goto syntaxe

@if "%1"=="hd"	goto lba2hd
@if "%1"=="HD"	goto lba2hd
@if "%1"=="Hd"	goto lba2hd
@if "%1"=="hD"	goto lba2hd

@if "%1"=="demo"  goto lba2demo
@if "%1"=="DEMO"  goto lba2demo


@rem ******** Version LBA2OK.EXE
:lba2ok
@if NOT EXIST lba2demo.mad goto test2hd
@if NOT EXIST *.obj goto oklba2
@echo Erreur: Derniere compilation faite en mode LBA2DEMO !!!
@echo    Taper CLEAR puis 'M' pour recompiler la Version LBA2OK.EXE
@goto fin
:test2hd
@if EXIST lba2ok.mad goto oklba2
@if NOT EXIST *.obj goto oklba2
@echo Erreur: Derniere compilation faite en mode LBA2HD !!!
@echo    Taper CLEAR puis 'M' pour recompiler la Version LBA2OK.EXE
@goto fin
:oklba2
@IF EXIST lba2demo.mad del lba2demo.mad
@echo lba2ok > lba2ok.mad

@goto isclean

@rem ******** Version LBA2HD.EXE
:lba2hd
@if NOT EXIST lba2ok.mad goto test2demo
@if NOT EXIST *.obj goto okhd
@echo Erreur: Derniere compilation faite en mode LBA2OK !!!
@echo    Taper CLEAR puis 'M HD' pour recompiler la Version LBA2HD.EXE
@goto fin
:test2demo
@if NOT EXIST lba2demo.mad goto okhd
@if NOT EXIST *.obj goto okhd
@echo Erreur: Derniere compilation faite en mode LBA2DEMO !!!
@echo    Taper CLEAR puis 'M HD' pour recompiler la Version LBA2HD.EXE
@goto fin
:okhd
@IF EXIST lba2demo.mad del lba2demo.mad
@IF EXIST lba2ok.mad del lba2ok.mad
@shift
@SET LBA2HD=TRUE

@goto isclean

@rem ******** Version LBA2DEMO.EXE
:lba2demo
@if NOT EXIST lba2ok.mad goto testhd
@if NOT EXIST *.obj goto okdemo
@echo Erreur: Derniere compilation faite en mode LBA2OK !!!
@echo    Taper CLEAR puis 'M DEMO' pour recompiler la Version LBA2DEMO.EXE
@goto fin
:testhd
@if EXIST lba2demo.mad goto okdemo
@if NOT EXIST *.obj goto okdemo
@echo Erreur: Derniere compilation faite en mode LBA2HD !!!
@echo    Taper CLEAR puis 'M DEMO' pour recompiler la Version LBA2DEMO.EXE
@goto fin
:okdemo
@IF EXIST lba2ok.mad del lba2ok.mad
@shift
@SET LBA2DEMO=TRUE
@echo lba2demo > lba2demo.mad

@goto isclean

@rem ******** Affiche la syntaxe de la compilation
:syntaxe
@echo ****
@echo **** Makefile pour compiler la version jeu de LBA2
@echo ****
@echo ÿ
@echo   M             - Compile LBA0.EXE pour la version LBA2OK.EXE
@echo   M clean       - Compile LBA0.EXE pour la version LBA2OK.EXE
@echo                   en copiant tous les fichiers commun de l'outil
@echo ÿ
@echo   M hd          - Compile LBA0.EXE pour la version LBA2HD.EXE
@echo   M hd clean    - Compile LBA0.EXE pour la version LBA2HD.EXE
@echo                   en copiant tous les fichiers commun de l'outil
@echo ÿ
@echo   M demo        - Compile LBA0.EXE pour la version LBA2DEMO.EXE
@echo   M demo clean  - Compile LBA0.EXE pour la version LBA2DEMO.EXE
@echo                   en copiant tous les fichiers commun de l'outil
@echo ÿ
@echo ****

@goto fin

@rem ******** Mise … jour des fichiers communs avec l'outil
:isclean

@if "%1"=="clean" goto clean
@goto pasclean
:clean
attrib +a f:\projet\lba2\tools\perso\*.h
attrib +a f:\projet\lba2\tools\perso\*.cpp
attrib +a f:\projet\lba2\tools\perso\*.asm
@shift
:pasclean
@echo off

rem goto pas_besoin

@call getsrc.bat

:pas_besoin

@echo on

:compil

@if "%RAMSYS%"=="" goto normal
e:\compil\watcom\binw\wmake %1 %2 %3 %4 %5 %6 %7 %8 %9
@goto fin
:normal
%WATCOM%\binw\wmake %1 %2 %3 %4 %5 %6 %7 %8 %9
:fin
@SET LBA2HD=
@SET LBA2DEMO=
