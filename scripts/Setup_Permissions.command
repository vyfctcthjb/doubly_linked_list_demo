#!/usr/bin/env bash
# Fix macOS permissions for .command/.sh scripts after download/zip.
# - Grants +x under scripts/ and tests/
# - Removes Gatekeeper quarantine
# - Safe if paths are absent; safe to run multiple times

set -Eeuo pipefail
shopt -s nullglob

# Resolve locations
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"

echo "[perm-setup] Repo: $REPO_DIR"

# Ensure this file is not quarantined so it can rerun cleanly
if command -v xattr >/dev/null 2>&1; then
  xattr -d com.apple.quarantine "$0" 2>/dev/null || true
fi

fix_exec_dir() {
  local dir="$1"
  [ -d "$dir" ] || return 0
  echo "[perm-setup] Marking executables in: $dir"
  while IFS= read -r -d '' f; do
    chmod +x "$f" || true
  done < <(find "$dir" -type f \( -name '*.command' -o -name '*.sh' \) -print0 2>/dev/null || true)
}

fix_exec_dir "$REPO_DIR/scripts"
fix_exec_dir "$REPO_DIR/tests"

if command -v xattr >/dev/null 2>&1; then
  [ -d "$REPO_DIR/scripts" ] && xattr -dr com.apple.quarantine "$REPO_DIR/scripts" 2>/dev/null || true
  [ -d "$REPO_DIR/tests" ] && xattr -dr com.apple.quarantine "$REPO_DIR/tests" 2>/dev/null || true
fi

echo "[perm-setup] Done. If Finder still blocks a .command the first time, right‑click → Open once,"
echo "or run it from Terminal: bash ./scripts/Run_Educational_Demo.command"
