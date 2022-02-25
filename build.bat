@echo off
setlocal enabledelayedexpansion

set CC=cl.exe
set OperatingDirectory=%~dp0

:: Setting settings Variables

set CompilerFlags=/W4 /Z7 /D "_CRT_SECURE_NO_WARNINGS" /c /MDd /std:c++latest /EHsc

set LuxLibPath=%OperatingDirectory%/LuxEngine/lib

set LuxIncludePath=-I%LuxLibPath%/stb/ -I%LuxLibPath%/glad/include/  -I%LuxLibPath%/glfw/include/ -I%LuxLibPath%/glm/ -I%OperatingDirectory%/LuxEngine/src/ -I%OperatingDirectory%/LuxEngine/pch/


call vcvarsall.bat x64 >nul 2> nul
echo --- Environment Configured

IF NOT EXIST "LuxEngine\lib\glfw\build\src\Release\glfw3.lib" (
    pushd "LuxEngine/lib/glfw" 
    
        cmake .  -B build 
        cd build && msbuild ALL_BUILD.vcxproj -property:Platform=x64 -property:Configuration=Release

    popd

    echo --- glfw3.lib build 
)

IF NOT EXIST "LuxEngine\lib\glm\build\glm\Release\glm_static.lib" (
    pushd "LuxEngine/lib/glm" 
    
        cmake .  -B build 
        cd build && msbuild ALL_BUILD.vcxproj -property:Platform=x64 -property:Configuration=Release

    popd

    echo --- glm_static.lib build
)

IF NOT EXIST "LuxEngine/lib/glad/glad.obj" (
    pushd "LuxEngine/lib/glad" 
        call cl /Foglad.obj !CompilerFlags! src/glad.c /Iinclude/ /c 

    popd
    echo --- glad.obj build
)

echo --- Building LuxEngine.lib

:: If bin folder does not exist create it
set OutputDir=!%~d0%cd%\LuxEngine\bin\!

if not EXIST "%OutputDir%" (
    mkdir %OutputDir%
)

pushd "LuxEngine/pch"

call cl /nologo !LuxIncludePath!  /Yc"pch.h" !CompilerFlags!  pch.cpp

popd

pushd "LuxEngine/src"

set PrecompiledHeaderUsage=/FI"../pch/pch.h" /Yu"../pch/pch.h"  /Fp"../pch/pch.pch"

for /R %%f IN ("*.cpp") DO (
    set file=%%~dpnxf

    set outfile=%%~fsnxf
    set outfile=!outfile:.cpp=.obj!
    set outfile=!outfile:%cd%=!
    set outfile=!outfile:\=-!
    set outfile=!!OutputDir!!outfile!!

    set LuxCompiledFiles=!!LuxCompiledFiles! !outfile!!

    call cl !PrecompiledHeaderUsage! !LuxIncludePath! /Fo!outfile! !CompilerFlags! !file! 2> nul || (
    
        echo --- Failed to compile %%~fsnxf aborting..
        goto end
    )
)
popd

:: Compile EngineLayer
set OutputDir=!%~d0%cd%\EngineLayer\bin\!

set EngineLayerIncludePath=-I!%~d0%cd%\LuxEngine\src\!  

if not EXIST "%OutputDir%" (
    mkdir %OutputDir%
)

pushd "EngineLayer/src"

for /R %%f IN ("*.cpp") DO (
    set file=%%~dpnxf

    set outfile=%%~fsnxf
    set outfile=!outfile:.cpp=.obj!
    set outfile=!outfile:%cd%=!
    set outfile=!outfile:\=-!
    set outfile=!!OutputDir!!outfile!!

    set EngineLayerCompiledFiles=!!EngineLayerCompiledFiles! !outfile!!

    call cl /Fo!outfile! !CompilerFlags! !file! !EngineLayerIncludePath! !LuxIncludePath! 2> nul || (
    
        echo --- Failed to compile %%~fsnxf aborting..
        goto end
    )
)
popd


call lib /nologo /out:EngineLayer/bin/EngineLayer.lib %EngineLayerCompiledFiles%   2> nul


set PlatformLibs=kernel32.lib user32.lib gdi32.lib shell32.lib

set VendorLibs=LuxEngine/lib/glfw/build/src/Release/glfw3.lib LuxEngine/lib/glm/build/glm/Release/glm_static.lib LuxEngine/lib/glad/glad.obj

set LinkingFlags=/NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT /nologo /debug:fastlink 

echo -- Linking
call link /OUT:program.exe %LinkingFlags% %LuxCompiledFiles% LuxEngine/pch/pch.obj EngineLayer/bin/EngineLayer.lib %VendorLibs% %PlatformLibs% && (
    echo --- Compiled Succesfully
    goto end
) || (
    echo --- Compilation failed
    goto end
)


:end