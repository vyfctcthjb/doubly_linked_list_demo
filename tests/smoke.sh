#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
"$ROOT/build/doubly_linked_list_demo" | tee /dev/stderr | grep -q "Forward:" && echo "[smoke] OK"
