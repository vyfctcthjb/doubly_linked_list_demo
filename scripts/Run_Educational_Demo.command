#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
mkdir -p "$ROOT/build"
cd "$ROOT/build"
cmake .. >/dev/null 2>&1 || cmake ..
cmake --build . --target doubly_linked_list_demo
./doubly_linked_list_demo
