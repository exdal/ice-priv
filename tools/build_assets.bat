@echo off

IF [%1] == [] GOTO SHOW_USAGE

Pushd "%~dp0\..\%1"
    ..\tools\AssetCompiler.exe .\Assets .\.build\
popd

echo Done
exit

:SHOW_USAGE
    echo Usage: .\tools\build_assets.bat {Project Name}
