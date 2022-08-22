@echo off
pushd %~dp0\..
echo ::Building Lux::
call scripts\bin\premake5.exe vs2022 
popd
pause