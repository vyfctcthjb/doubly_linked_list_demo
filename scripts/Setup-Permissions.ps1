Param()

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot  = Resolve-Path "$ScriptDir/.."

Write-Host "[perm-setup] Repo: $RepoRoot"

# Mark .ps1/.bat as unblocked and ensure LF endings aren't breaking shebangs on .sh/.command
Get-ChildItem -Path "$RepoRoot/scripts" -Include *.ps1,*.bat -Recurse -ErrorAction SilentlyContinue | ForEach-Object {
  try { Unblock-File $_.FullName } catch {}
}

# Nothing else to do on Windows for .command; users should use .bat/.ps1
Write-Host "[perm-setup] Windows permissions update complete."
