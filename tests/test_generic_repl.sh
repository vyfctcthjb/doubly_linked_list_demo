#!/usr/bin/env bash
set -euo pipefail

BIN=${1:-"./generic_demo"}
if [[ ! -x "$BIN" ]]; then
  echo "error: generic_demo not found or not executable at $BIN" >&2
  exit 1
fi

strip_ansi() {
  # remove ANSI color codes for reliable matching
  sed -E 's/\x1B\[[0-9;]*[A-Za-z]//g'
}

run_case() {
  local name="$1"; shift
  local input="$1"; shift
  local -a expects=("$@")

  echo "[case] $name"
  # shellcheck disable=SC2059
  output=$(printf "%b" "$input" | "$BIN" | strip_ansi)
  for expect in "${expects[@]}"; do
    if ! grep -Fq "$expect" <<<"$output"; then
      echo "[fail] $name: expected to find: $expect" >&2
      echo "-------- output --------" >&2
      echo "$output" >&2
      echo "------------------------" >&2
      exit 1
    fi
  done
}

# 1) push_front/back, print/print_rev, size
run_case "push+print+size" \
  "3\npush_back A\npush_back B\npush_front X\nprint\nprint_rev\nsize\nmenu\n4\n" \
  "X <-> A <-> B" \
  "B <-> A <-> X" \
  "3"

# 2) insert_after INDEX, insert_before INDEX
run_case "insert index forms" \
  "3\npush_back A\npush_back B\ninsert_after 1 C\ninsert_before 0 Y\nprint\nmenu\n4\n" \
  "Y <-> A <-> B <-> C"

# 3) shorthand insert_after VALUE (after tail), insert_before VALUE (before head)
run_case "insert shorthand" \
  "3\npush_back A\ninsert_after C\ninsert_before Z\nprint\nmenu\n4\n" \
  "Z <-> A <-> C"

# 4) detach_at and remove_at
run_case "detach/remove" \
  "3\npush_back A\npush_back B\npush_back C\ndetach_at 1\nprint\nsize\nremove_at 1\nprint\nsize\nmenu\n4\n" \
  "A <-> C" \
  $'\n2\n' \
  "A" \
  $'\n1\n'

# 5) clear
run_case "clear" \
  "3\npush_back A\npush_back B\nclear\nprint\nsize\nmenu\n4\n" \
  $'\n0\n'

# 6) bad usage should not create empty nodes
run_case "bad usage insert_after" \
  "3\npush_back AA\ninsert_after\nprint\nmenu\n4\n" \
  "AA"

# 7) bad usage push_front/back should not create empty nodes
run_case "bad usage push_front/back" \
  "3\npush_front\npush_back guida\npush_back 1 justin\nprint\nmenu\n4\n" \
  "guida <-> 1 justin"

# 8) find VALUE should report index and not found
run_case "find command" \
  "3\npush_back A\npush_back B\npush_back C\nfind B\nfind Z\nmenu\n4\n" \
  $'found at 1\n' \
  "not found"

echo "All REPL tests passed."
