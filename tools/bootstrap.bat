@echo off

git submodule update --init

IF [%1] == [] GOTO DEFAULT

GOTO CUSTOM_COMMAND

:DEFAULT
    Pushd "%~dp0\..\"
        .\tools\genie.exe vs2019
    popd
    exit

:CUSTOM_COMMAND:
    Pushd "%~dp0\..\"
        .\tools\genie.exe %1
    popd
