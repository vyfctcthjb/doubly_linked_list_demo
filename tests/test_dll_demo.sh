#!/usr/bin/env bash
set -euo pipefail

BIN=${1:-"./doubly_linked_list_demo"}
if [[ ! -x "$BIN" ]]; then
  echo "error: dll_demo not found or not executable at $BIN" >&2
  exit 1
fi

out="$($BIN)"
expect_forward='Forward:   A <-> C <-> B'
expect_backward='Backward:  B <-> C <-> A'

if ! grep -Fq "$expect_forward" <<<"$out"; then
  echo "[fail] expected: $expect_forward" >&2
  echo "----- output -----" >&2
  echo "$out" >&2
  echo "------------------" >&2
  exit 1
fi
if ! grep -Fq "$expect_backward" <<<"$out"; then
  echo "[fail] expected: $expect_backward" >&2
  echo "----- output -----" >&2
  echo "$out" >&2
  echo "------------------" >&2
  exit 1
fi

echo "dll_demo output OK"

