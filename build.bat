@echo off

IF "%~1" == "" GOTO PrintHelp
IF "%~1" == "compile" GOTO Compile
IF "%~1" == "run" GOTO Run

build\bin\premake\premake5.exe %1
GOTO Done

:PrintHelp

echo.
echo Enter "build.bat action", where "action" is one of the following:
echo.
echo   compile			Generate a make file then compile using it
echo   clean			Remove all binaries, intermediates and project files
echo.
echo   vs2022			Generate Visual Studio 2022 project files

GOTO Done

:Compile

build\bin\premake\premake5.exe vs2022

if not defined DevEnvDir (
	call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
)

set solutionFile="DeepWell.sln"
msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %solutionFile%

GOTO Done

:Run
cd W:\bin\Debug-windows-x86_64\DeepWell
start DeepWell.exe

cd W:\

:Done
