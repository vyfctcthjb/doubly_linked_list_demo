#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
mkdir -p "$ROOT/build"
cd "$ROOT/build"
cmake .. >/dev/null 2>&1 || cmake ..
cmake --build . --target generic_demo >/dev/null || cmake --build . --target generic_demo

# Pretty UI for humans (does not affect tests)
export PRETTY_UI=1
export PAUSE_AFTER_SECTIONS=1
export KEEP_LEGACY=0
export CLEAR_MENU=1
export PAGINATE_HELP=1
export LANG=${LANG:-en_US.UTF-8}
export LC_ALL=${LC_ALL:-en_US.UTF-8}
./generic_demo
