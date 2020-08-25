@echo off

git submodule update --init

IF [%1] == [] GOTO DEFAULT

GOTO CUSTOM_COMMAND

:DEFAULT
    Pushd "%~dp0\..\"
        # .\tools\genie.exe --with-android=23 --gcc=android-x86
        # .\tools\genie.exe --with-android=23 --gcc=android-arm
        .\tools\genie.exe --with-android=23 --os=linux --gcc=android-arm64 vs2019
    popd
    exit

:CUSTOM_COMMAND:
    Pushd "%~dp0\..\"
        .\tools\genie.exe %1
    popd
