@echo off
REM Windows double-click script: build generic_demo and run REPL tests via CTest
setlocal enabledelayedexpansion

set SCRIPT_DIR=%~dp0
set REPO_ROOT=%SCRIPT_DIR%..\
set BUILD_DIR=%REPO_ROOT%build
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cmake -S "%REPO_ROOT%" -B "%BUILD_DIR%"
cmake --build "%BUILD_DIR%" --target generic_demo --config Debug
pushd "%BUILD_DIR%"
ctest -V
popd

echo All interactive demo (REPL) tests invoked via CTest.
pause
