# PowerShell script to build generic_demo and run the REPL tests via CTest (Windows)
Param()

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot  = Resolve-Path "$ScriptDir/.."
$BuildDir  = Join-Path $RepoRoot "build"
if (-Not (Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir | Out-Null }

cmake -S $RepoRoot -B $BuildDir
cmake --build $BuildDir --target generic_demo --config Debug
Push-Location $BuildDir
ctest -V
Pop-Location

Write-Host "All interactive demo (REPL) tests invoked via CTest."
