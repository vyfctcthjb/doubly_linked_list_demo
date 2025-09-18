#!/usr/bin/env bash
# Bootstrap script for macOS ZIP/clone users:
# - Detects 'noexec' volumes and explains how to fix it
# - Grants +x to *.command and *.sh under scripts/ and tests/
# - Removes Gatekeeper quarantine flags so double‑click works

set -Eeuo pipefail
shopt -s nullglob

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"

DEVICE="$(df "$REPO_DIR" | tail -1 | awk '{print $1}')"
MOUNT_LINE="$(mount | grep -F "$DEVICE" || true)"
if [[ "$MOUNT_LINE" == *noexec* ]]; then
  cat <<EOF
[bootstrap] This folder is on a 'noexec' volume:
$MOUNT_LINE

macOS won't execute .command files here. Move the repo under your home drive (APFS), e.g.:
  ~/Dev/doubly_linked_list_demo
Then re-run:  bash scripts/bootstrap_mac.command
EOF
  exit 1
fi

echo "[bootstrap] Marking scripts executable…"
chmod +x "$SCRIPT_DIR"/*.command 2>/dev/null || true
chmod +x "$REPO_DIR/tests"/*.sh  2>/dev/null || true
# Mark any app bundle launchers executable
chmod +x "$REPO_DIR"/macos/*.app/Contents/MacOS/* 2>/dev/null || true

if command -v xattr >/dev/null 2>&1; then
  echo "[bootstrap] Removing quarantine flags…"
  xattr -dr com.apple.quarantine "$REPO_DIR" 2>/dev/null || true
fi

echo "[bootstrap] Done. Try: make run   or   double-click scripts/Run_Educational_Demo.command"
